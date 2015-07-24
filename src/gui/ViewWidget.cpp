/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "ViewWidget.hpp"
#include "LayerWidget.hpp"
#include "EdgeItem.hpp"
#include "NodeItem.hpp"
#include "AddNodeDialog.hpp"
#include "PropertyDialog.hpp"
#include "NodeTypeManager.hpp"
#include "EdgeTypeManager.hpp"
#include "ActionCommander.hpp"
#include "SwapPortsDialog.hpp"
#include "RenamePortDialog.hpp"

#include <set>
#include <math.h>
#include <sstream>
#include <QDir>
#include <QTime>
#include <QMenu>
#include <QLabel>
#include <sstream>
#include <QMenuBar>
#include <QAction>
#include <QKeyEvent>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QInputDialog>
#include <QSignalMapper>
#include <boost/regex.hpp>
#include <base/Logging.hpp>
#include <boost/lexical_cast.hpp>

#include <graph_analysis/Filter.hpp>
#include <graph_analysis/io/GVGraph.hpp>
#include <graph_analysis/GraphIO.hpp>
#include <graph_analysis/io/YamlWriter.hpp>
#include <graph_analysis/io/GexfWriter.hpp>
#include <graph_analysis/io/GexfReader.hpp>
#include <graph_analysis/io/YamlReader.hpp>
#include <graph_analysis/io/GraphvizWriter.hpp>
#include <graph_analysis/filters/EdgeContextFilter.hpp>
#include <graph_analysis/gui/graphitem/edges/EdgeLabel.hpp>

#include <exception>
#include <boost/foreach.hpp>
#include <base/Time.hpp>
//#define DEFAULT_PATH_TO_ICONS "../../resources/icons/"
#define DEFAULT_PATH_TO_ICONS "icons/"
// StatusBar standard timeout (in ms)
#define DEFAULT_TIMEOUT 6900

// comment out to toggle-out focused node be re-doule-clicking it; leave untouched to be able to cancel node focusing by double-clicking the background
#define CLEAR_BY_BACKGROUND

using namespace graph_analysis;

namespace graph_analysis {
namespace gui {

ViewWidget::ViewWidget(QMainWindow *mainWindow, QWidget *parent)
    : GraphWidget(parent)
    , mpMainWindow(mainWindow)
    , mpStackedWidget(new QStackedWidget())
    , mpLayoutingGraph()
    , mVertexFocused(false)
    , mEdgeFocused(false)
    , mInitialized(false)
    , mMaxNodeHeight(0)
    , mMaxNodeWidth (0)
    , mpLayerWidget(new LayerWidget(this, mpGraph))
    , mpStatus(mpMainWindow->statusBar())
{
    // Add seed for force layout
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);

    setCacheMode(CacheBackground);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(400, 400);
    setWindowTitle(tr("Graphview"));
    // Setting up filtering
    mGraphView.setVertexFilter(mpVertexFilter);
    mGraphView.setEdgeFilter(mpEdgeFilter);

    // setting up the Reader- and Writer- Maps
    io::YamlWriter *yamlWriter = new io::YamlWriter();
    mWriterMap["yaml"]  = yamlWriter;
    mWriterMap["Yaml"]  = yamlWriter;
    mWriterMap["yml"]   = yamlWriter;
    mWriterMap["Yml"]   = yamlWriter;
    mWriterMap["YAML"]  = yamlWriter;
    mWriterMap["YML"]   = yamlWriter;
    io::YamlReader *yamlReader = new io::YamlReader();
    mReaderMap["yaml"]  = yamlReader;
    mReaderMap["Yaml"]  = yamlReader;
    mReaderMap["yml"]   = yamlReader;
    mReaderMap["Yml"]   = yamlReader;
    mReaderMap["YAML"]  = yamlReader;
    mReaderMap["YML"]   = yamlReader;
    io::GexfWriter *gexfWriter = new io::GexfWriter();
    mWriterMap["gexf"]  = gexfWriter;
    mWriterMap["Gexf"]  = gexfWriter;
    mWriterMap["xml"]   = gexfWriter;
    mWriterMap["Xml"]   = gexfWriter;
    mWriterMap["GEXF"]  = gexfWriter;
    mWriterMap["XML"]   = gexfWriter;
    io::GexfReader *gexfReader = new io::GexfReader();
    mReaderMap["gexf"]  = gexfReader;
    mReaderMap["Gexf"]  = gexfReader;
    mReaderMap["xml"]   = gexfReader;
    mReaderMap["Xml"]   = gexfReader;
    mReaderMap["GEXF"]  = gexfReader;
    mReaderMap["XML"]   = gexfReader;
    io::GraphvizWriter *gvWriter = new io::GraphvizWriter();
    mWriterMap["dot"]  = gvWriter;
    mWriterMap["Dot"]  = gvWriter;
    mWriterMap["DOT"]  = gvWriter;

    // setting up the icons
    std::string pathToIcons = DEFAULT_PATH_TO_ICONS;
    //        taken_from: www.softicons.com         //        commercial_usage: NOT allowed
    loadIcon(mIconMap["addNode"], pathToIcons + "addNode.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["import"], pathToIcons + "import.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["export"], pathToIcons + "export.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["refresh"], pathToIcons + "refresh.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["shuffle"], pathToIcons + "shuffle.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["reset"], pathToIcons + "reset.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["layout"], pathToIcons + "layout.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["dragndrop"], pathToIcons + "dragndrop.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["label"], pathToIcons + "label.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["portLabel"], pathToIcons + "portLabel.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["remove"], pathToIcons + "remove.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["removeAll"], pathToIcons + "removeAll.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["addPort"], pathToIcons + "addPort.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["move"], pathToIcons + "move.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["reload"], pathToIcons + "reload.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["swap"], pathToIcons + "swap.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["layers"], pathToIcons + "layers.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["properties"], pathToIcons + "properties.png");

    //        taken_from: www.softicons.com         //        commercial_usage: NOT allowed
    loadIcon(mIconMap["addNode_white"], pathToIcons + "addNode_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["import_white"], pathToIcons + "import_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["export_white"], pathToIcons + "export_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["refresh_white"], pathToIcons + "refresh_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["shuffle_white"], pathToIcons + "shuffle_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["reset_white"], pathToIcons + "reset_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["layout_white"], pathToIcons + "layout_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["dragndrop_white"], pathToIcons + "dragndrop_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["label_white"], pathToIcons + "label_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["portLabel_white"], pathToIcons + "portLabel_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["remove_white"], pathToIcons + "remove_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["removeAll_white"], pathToIcons + "removeAll_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["addPort_white"], pathToIcons + "addPort_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["move_white"], pathToIcons + "move_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["reload_white"], pathToIcons + "reload_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["swap_white"], pathToIcons + "swap_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["layers_white"], pathToIcons + "layers_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["properties_white"], pathToIcons + "properties_white.png");

    // setting up the context menu
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
        this, SLOT(showContextMenu(const QPoint &)));
    mpStackedWidget->addWidget((QWidget *) this);
    mpStackedWidget->addWidget((QWidget *) mpLayerWidget);
    mpStackedWidget->setCurrentIndex(0);
    mpMainWindow->setCentralWidget(mpStackedWidget);
    reset();
    mpPropertyDialog = new PropertyDialog(this, mpLayerWidget, mpMainWindow, mpStackedWidget);

    // setting up the Menus ToolBar
    ActionCommander comm(this);
    QMenuBar *bar = mpMainWindow->menuBar();

    // needed menus
    QMenu *MainMenu = new QMenu(tr("&Graph"));
    QMenu *NodeMenu = new QMenu(tr("&Node"));
    QMenu *EdgeMenu = new QMenu(tr("&Edge"));

    // needed actions
    QAction *actionChangeEdgeLabel = comm.addAction("Rename Edge", SLOT(changeFocusedEdgeLabelMainWindow()), mIconMap["label_white"]);
    QAction *actionRemoveEdge  = comm.addAction("Remove Edge", SLOT(removeFocusedEdgeMainWindow()), mIconMap["remove_white"]);
    QAction *actionChangeLabel = comm.addAction("Rename Node", SLOT(changeFocusedVertexLabelMainWindow()), mIconMap["label_white"]);
    QAction *actionRemoveNode  = comm.addAction("Remove Node", SLOT(removeFocusedVertexMainWindow()), mIconMap["remove_white"]);
    QAction *actionAddPort     = comm.addAction("Add Port", SLOT(addPortFocusedMainWindow()), mIconMap["addPort_white"]);
    QAction *actionSwapPorts   = comm.addAction("Swap Ports", SLOT(swapPortsFocusedMainWindow()), mIconMap["swap_white"]);
    QAction *actionRenamePort  = comm.addAction("Rename a Port", SLOT(renamePortFocusedMainWindow()), mIconMap["portLabel_white"]);
    QAction *actionRemovePort  = comm.addAction("Remove a Port", SLOT(removePortFocusedMainWindow()), mIconMap["remove_white"]);
    QAction *actionRemovePorts = comm.addAction("Remove Ports", SLOT(removePortsFocusedMainWindow()), mIconMap["removeAll_white"]);
    QAction *actionAddNode = comm.addAction("Add Node", SLOT(addNodeAdhocMainWindow()), mIconMap["addNode_white"]);
    QAction *actionRefresh = comm.addAction("Refresh", SLOT(refreshMainWindow()), mIconMap["refresh_white"]);
    QAction *actionShuffle = comm.addAction("Shuffle", SLOT(shuffleMainWindow()), mIconMap["shuffle_white"]);
    QAction *actionImport = comm.addAction("Import", SLOT(importGraph()), mIconMap["import_white"]);
    QAction *actionExport = comm.addAction("Export", SLOT(exportGraph()), mIconMap["export_white"]);
    QAction *actionReset  = comm.addAction("Reset", SLOT(resetGraph()), mIconMap["reset_white"]);
    QAction *actionLayout = comm.addAction("Layout", SLOT(changeLayoutMainWindow()), mIconMap["layout_white"]);
    QAction *actionToggleDragDrop = comm.addAction("Toggle Drag-n-Drop", SLOT(toggleDragDrop()), mIconMap["dragndrop_white"]);
    QAction *actionReloadPropertyDialog = comm.addAction("Reload Properties", SLOT(reloadPropertyDialogMainWindow()), mIconMap["reload_white"]);

    // loading different actions in different menus
    MainMenu->addAction(actionAddNode);
    MainMenu->addSeparator();
    MainMenu->addAction(actionImport);
    MainMenu->addAction(actionExport);
    MainMenu->addSeparator();
    MainMenu->addAction(actionRefresh);
    MainMenu->addAction(actionShuffle);
    MainMenu->addAction(actionReset);
    MainMenu->addAction(actionLayout);
    MainMenu->addSeparator();
    MainMenu->addAction(actionToggleDragDrop);
    MainMenu->addSeparator();
    MainMenu->addAction(actionReloadPropertyDialog);

    NodeMenu->addAction(actionChangeLabel);
    NodeMenu->addAction(actionAddPort);
    NodeMenu->addAction(actionSwapPorts);
    NodeMenu->addAction(actionRenamePort);
    NodeMenu->addAction(actionRemovePort);
    NodeMenu->addAction(actionRemovePorts);
    NodeMenu->addAction(actionRemoveNode);

    EdgeMenu->addAction(actionChangeEdgeLabel);
    EdgeMenu->addAction(actionRemoveEdge);

    // loading menus in the bar
    bar->addMenu(MainMenu);
    bar->addMenu(NodeMenu);
    bar->addMenu(EdgeMenu);

    mpMainWindow->setWindowTitle(tr("Graph Analysis"));
    mpStatus->addWidget(new QLabel("Ready!"));
}

void ViewWidget::updateStatus(const std::string& message, int timeout)
{
    mpStatus->showMessage(QString(message.c_str()), timeout);
}

void ViewWidget::importGraphLayer()
{
    importGraph();
    mpLayerWidget->refresh();
}

void ViewWidget::resetGraphLayer()
{
    resetGraph();
    mpLayerWidget->refresh();
}

void ViewWidget::loadIcon(QIcon& icon, std::string file)
{
    icon.addFile(QString::fromUtf8(file.c_str()), QSize(), QIcon::Normal, QIcon::Off);
}

ViewWidget::~ViewWidget()
{
    WriterMap::iterator it_writer = mWriterMap.begin();
    for(; mWriterMap.end() != it_writer; ++it_writer)
    {
        if(it_writer->second)
        {
            delete it_writer->second;
        }
    }
    ReaderMap::iterator it_reader = mReaderMap.begin();
    for(; mReaderMap.end() != it_reader; ++it_reader)
    {
        if(it_reader->second)
        {
            delete it_reader->second;
        }
    }
}

void ViewWidget::showContextMenu(const QPoint& pos)
{
    ActionCommander comm(this);
    QPoint position = mapTo(this, pos);
    QMenu contextMenu(tr("Context menu"), this);

    QAction *actionChangeEdgeLabel = comm.addAction("Rename Edge", SLOT(changeSelectedEdgeLabel()), mIconMap["label"]);
    QAction *actionRemoveEdge  = comm.addAction("Remove Edge", SLOT(removeSelectedEdge()), mIconMap["remove"]);
    QAction *actionChangeLabel = comm.addAction("Rename Node", SLOT(changeSelectedVertexLabel()), mIconMap["label"]);
    QAction *actionRemoveNode  = comm.addAction("Remove Node", SLOT(removeSelectedVertex()), mIconMap["remove"]);
    QAction *actionAddPort     = comm.addAction("Add Port", SLOT(addPortSelected()), mIconMap["addPort"]);
    QAction *actionSwapPorts   = comm.addAction("Swap Ports", SLOT(swapPortsSelected()), mIconMap["swap"]);
    QAction *actionRenamePort  = comm.addAction("Rename a Port", SLOT(renamePortSelected()), mIconMap["portLabel"]);
    QAction *actionRemovePort  = comm.addAction("Remove a Port", SLOT(removePortSelected()), mIconMap["remove"]);
    QAction *actionRemovePorts = comm.addAction("Remove Ports", SLOT(removePortsSelected()), mIconMap["removeAll"]);
    QAction *actionAddNode = comm.addMappedAction("Add Node", SLOT(addNodeAdhoc(QObject*)), (QObject*)&position, mIconMap["addNode"]);
    QAction *actionRefresh = comm.addAction("Refresh", SLOT(refresh()), mIconMap["refresh"]);
    QAction *actionShuffle = comm.addAction("Shuffle", SLOT(shuffle()), mIconMap["shuffle"]);
    QAction *actionImport = comm.addAction("Import", SLOT(importGraph()), mIconMap["import"]);
    QAction *actionExport = comm.addAction("Export", SLOT(exportGraph()), mIconMap["export"]);
    QAction *actionReset  = comm.addAction("Reset", SLOT(resetGraph()), mIconMap["reset"]);
    QAction *actionLayout = comm.addAction("Layout", SLOT(changeLayout()), mIconMap["layout"]);
    QAction *actionSetDragDrop = comm.addAction("Drag-n-Drop", SLOT(setDragDrop()), mIconMap["dragndrop"]);
    QAction *actionUnsetDragDrop = comm.addAction("Move-around", SLOT(unsetDragDrop()), mIconMap["move"]);
    QAction *actionReloadPropertyDialog = comm.addAction("Reload Command Panel", SLOT(reloadPropertyDialog()), mIconMap["reload"]);

    // (conditionally) adding the actions to the context menu
    if(mEdgeSelected)
    {
        contextMenu.addAction(actionChangeEdgeLabel);
        contextMenu.addAction(actionRemoveEdge);
    }
    if(mVertexSelected)
    {
        if(mEdgeSelected)
        {
            contextMenu.addSeparator();
        }
        contextMenu.addAction(actionChangeLabel);
        contextMenu.addAction(actionAddPort);
        contextMenu.addAction(actionSwapPorts);
        contextMenu.addAction(actionRenamePort);
        contextMenu.addAction(actionRemovePort);
        contextMenu.addAction(actionRemovePorts);
        contextMenu.addAction(actionRemoveNode);
    }
    if(mVertexSelected || mEdgeSelected)
    {
        contextMenu.addSeparator();
    }
    contextMenu.addAction(actionAddNode);
    contextMenu.addSeparator();
    contextMenu.addAction(actionImport);
    contextMenu.addAction(actionExport);
    contextMenu.addSeparator();
    contextMenu.addAction(actionRefresh);
    contextMenu.addAction(actionShuffle);
    if(!mpGraph->empty())
    {
        contextMenu.addAction(actionReset);
    }
    contextMenu.addAction(actionLayout);
    contextMenu.addSeparator();
    if(mDragDrop)
    {
        contextMenu.addAction(actionUnsetDragDrop);
    }
    else
    {
        contextMenu.addAction(actionSetDragDrop);
    }
    if(!mpPropertyDialog->isRunning())
    {
        contextMenu.addSeparator();
        contextMenu.addAction(actionReloadPropertyDialog);
    }
    contextMenu.exec(mapToGlobal(pos));
}

bool ViewWidget::dialogIsRunning()
{
    return mpPropertyDialog->isRunning();
}

void ViewWidget::addPortFocused()
{
    addPort(mpFocusedVertex);
}

void ViewWidget::addPortSelected()
{
    addPort(mpSelectedVertex);
}

void ViewWidget::addPort(graph_analysis::Vertex::Ptr vertex)
{
    updateStatus(std::string("adding a port to vertex '") + vertex->toString() + "' of type '" + vertex->getClassName() + "'...");

    NodeItem *item = mNodeItemMap[vertex];
    if(!item)
    {
        std::string error_msg = std::string("graph_analysis::ViewWidget::addPort: provided vertex '") + vertex->getLabel() + "' is not registered with the GUI";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
        updateStatus(std::string("Failed to add a port to vertex '") + vertex->toString() + "' of type '" + vertex->getClassName() + "': " + error_msg + "!", DEFAULT_TIMEOUT);
    }
    bool ok;
    QStringList ports_options;
    ports_options << tr("input");
    ports_options << tr("output");

    QString strPortType = QInputDialog::getItem(this, tr("Choose Port Type"),
                                         tr("Port Type:"), ports_options,
                                         0, false, &ok);
    if (ok && !strPortType.isEmpty())
    {
        graph_analysis::Vertex::Ptr portVertex;
        if("input" == strPortType)
        {
            portVertex = VertexTypeManager::getInstance()->createVertex("inputport", "newInputPort");
        }
        else // ("output" == strPortType)
        {
            portVertex = VertexTypeManager::getInstance()->createVertex("outputport", "newOutputPort");
        }
        mpGraph->addVertex(portVertex);
        enableVertex(portVertex);
        createEdge(vertex, portVertex, "portRegistrationEdge");
        mPortMap[portVertex] = item;
        mPortIDMap[portVertex] = item->addPort(portVertex);
        updateStatus(std::string("Added an ") + strPortType.toStdString() + " port to vertex '" + vertex->toString() + "' of type '" + vertex->getClassName() + "'!", DEFAULT_TIMEOUT);
        item->update();
    }
    else
    {
        updateStatus(std::string("Failed to add an ") + strPortType.toStdString() + " port to vertex '" + vertex->toString() + "' of type '" + vertex->getClassName() + "': aborted by user!", DEFAULT_TIMEOUT);
    }
}

void ViewWidget::renamePortFocused()
{
    renamePort(mpFocusedVertex);
}

void ViewWidget::renamePortSelected()
{
    renamePort(mpSelectedVertex);
}

void ViewWidget::renamePort(graph_analysis::Vertex::Ptr concernedVertex)
{
    updateStatus(std::string("renaming a port in vertex '") + concernedVertex->toString() + "' of type '" + concernedVertex->getClassName() + "'...");
    NodeItem *item = mNodeItemMap[concernedVertex];
    if(!item)
    {
        std::string error_msg = std::string("graph_analysis::ViewWidget::renamePort: provided vertex '") + concernedVertex->getLabel() + "' is not registered with the GUI";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    int portCount = item->getPortCount();
    if(!portCount)
    {
        QMessageBox::critical(this, tr("Cannot Rename a Port"), tr("The selected vertex had no ports!"));
        updateStatus(std::string("Failed to rename a port of vertex '") + concernedVertex->toString() + "' of type '" + concernedVertex->getClassName() + ": there are no ports!", DEFAULT_TIMEOUT);
        return;
    }
    RenamePortDialog dialog(item);
    if(dialog.isValid())
    {
        std::string newLabel    = dialog.getNewLabel();
        std::string strPortID   = dialog.getPortID();
        int portID;
        std::stringstream ss(strPortID);
        ss >> portID;
        item->setPortLabel(portID, newLabel);
        updateStatus(std::string("Renamed the port of local ID '" + boost::lexical_cast<std::string>(portID) + "' of vertex '") + concernedVertex->toString() + "' of type '" + concernedVertex->getClassName() + "' to '" + newLabel + "'!", DEFAULT_TIMEOUT);
    }
    else
    {
        updateStatus(std::string("Failed to rename a port of vertex '") + concernedVertex->toString() + "' of type '" + concernedVertex->getClassName() + ": aborted by user!", DEFAULT_TIMEOUT);
    }
}

void ViewWidget::removePortFocused()
{
    removePort(mpFocusedVertex);
}

void ViewWidget::removePortSelected()
{
    removePort(mpSelectedVertex);
}

void ViewWidget::removePort(graph_analysis::Vertex::Ptr concernedVertex)
{
    updateStatus(std::string("removing a port from vertex '") + concernedVertex->toString() + "' of type '" + concernedVertex->getClassName() + "'...");
    NodeItem *item = mNodeItemMap[concernedVertex];
    if(!item)
    {
        std::string error_msg = std::string("graph_analysis::ViewWidget::removePort: provided vertex '") + concernedVertex->getLabel() + "' is not registered with the GUI";
        LOG_ERROR_S << error_msg;
        updateStatus(std::string("Failed to remove a port of vertex '") + concernedVertex->toString() + "' of type '" + concernedVertex->getClassName() + ": " + error_msg + "!", DEFAULT_TIMEOUT);
        throw std::runtime_error(error_msg);
    }
    int portCount = item->getPortCount();
    if(!portCount)
    {
        QMessageBox::critical(this, tr("Cannot Remove a Port"), tr("The selected vertex had no ports!"));
        updateStatus(std::string("Failed to remove a port of vertex '") + concernedVertex->toString() + "' of type '" + concernedVertex->getClassName() + ": there are no ports!", DEFAULT_TIMEOUT);
        return;
    }
    bool ok;
    QStringList ports_options;
    foreach(NodeItem::VTuple tuple, item->getVertices())
    {
        graph_analysis::Vertex::Ptr vertex = tuple.second;
        std::string option = boost::lexical_cast<std::string>(tuple.first) + ": " + vertex->getLabel();
        ports_options << tr(option.c_str());
    }
    QString strPortID = QInputDialog::getItem(this, tr("Remove a Port"),
                                         tr("Port ID:"), ports_options,
                                         0, false, &ok);
    if (ok && !strPortID.isEmpty())
    {
        std::stringstream ss(strPortID.toStdString());
        int portID;
        ss >> portID;
        // remove conceptual edges
        EdgeIterator::Ptr edgeIt = mpGraph->getEdgeIterator(item->getPort(portID));
        while(edgeIt->next())
        {
            Edge::Ptr edge = edgeIt->current();
            mpGraph->removeEdge(edge);
        }
        // remove physical edges and their graphics
        graph_analysis::Vertex::Ptr cluster = item->getVertex();
        edgeIt = mpLayoutingGraph->getEdgeIterator(cluster);
        while(edgeIt->next())
        {
            Edge::Ptr edge = edgeIt->current();
            if(mEdgeItemMap.count(edge))
            {
                EdgeItem *edgeItem = mEdgeItemMap[edge];
                if  (
                        edgeItem
                            &&
                        (
                            (item == edgeItem->sourceNodeItem() && portID == edgeItem->getSourcePortID()) ||
                            (item == edgeItem->targetNodeItem() && portID == edgeItem->getTargetPortID())
                        )
                    )
                    {
                        mpLayoutingGraph->removeEdge(edge);
                        syncEdgeItemMap(edge);
                        scene()->removeItem(edgeItem);
                    }
            }
        }
        // remove port graphics
        item->removePort(portID);
        updateStatus(std::string("Removed the port of local ID '" + boost::lexical_cast<std::string>(portID) + "' of vertex '") + concernedVertex->toString() + "' of type '" + concernedVertex->getClassName() + "'!", DEFAULT_TIMEOUT);
    }
    else
    {
        updateStatus(std::string("Failed to remove a port of vertex '") + concernedVertex->toString() + "' of type '" + concernedVertex->getClassName() + ": aborted by user!", DEFAULT_TIMEOUT);
    }
}

void ViewWidget::importGraph()
{
    updateStatus(std::string("importing graph..."));
    QString selectedFilter;
    QString label =  QFileDialog::getOpenFileName(this, tr("Choose Input File"), QDir::currentPath(), tr("GEXF (*.gexf *.xml);;YAML/YML (*.yaml *.yml)"), &selectedFilter);

    if (!label.isEmpty())
    {
        // removing trailing whitespaces in the filename
        label = label.trimmed();
        updateStatus(std::string("Imported graph: from input file '") + label.toStdString() + "'!", DEFAULT_TIMEOUT); // moved up out of sync reasons
        if(label.contains('.'))
        {
            if(label.endsWith(QString(".gexf")) || label.endsWith(QString(".xml")))
            {
                fromXmlFile(label.toStdString());
            }
            else if(label.endsWith(QString(".yml")) || label.endsWith(QString(".yaml")))
            {
                fromYmlFile(label.toStdString());
            }
            else
            {
                QMessageBox::critical(this, tr("Graph Import Failed"), QString(std::string(std::string("Unsupported file format for file '") + label.toStdString() + "'!").c_str()));
                updateStatus(std::string("Failed to import graph: from input file '") + label.toStdString() + "': unsupported file format!", DEFAULT_TIMEOUT);
                return;
            }
        }
        else
        {
            if(selectedFilter.startsWith("YAML"))
            {
                fromYmlFile(label.toStdString() + ".yml");
            }
            else // if(selectedFilter.startsWith("GEXF"))
            {
                fromXmlFile(label.toStdString() + ".gexf");
            }
        }
    }
    else
    {
        updateStatus(std::string("Failed to import graph: aborted by user - an empty input filename was provided!"), DEFAULT_TIMEOUT);
    }
}

void ViewWidget::exportGraph()
{
    updateStatus(std::string("exporting graph..."));
    QString selectedFilter;
    QString label =  QFileDialog::getSaveFileName(this, tr("Choose Export File"), QDir::currentPath(), tr("GEXF (*.gexf *.xml);;YAML/YML (*.yaml *.yml);;DOT (*.dot)"), &selectedFilter);

    if (!label.isEmpty())
    {
        // removing trailing whitespaces in the filename
        label = label.trimmed();
        updateStatus(std::string("Exported graph: to output file '") + label.toStdString() + "'!", DEFAULT_TIMEOUT); // moved up out of sync reasons
        if(label.contains('.'))
        {
            if(label.endsWith(QString(".gexf")) || label.endsWith(QString(".xml")))
            {
                toXmlFile(label.toStdString());
            }
            else if(label.endsWith(QString(".yml")) || label.endsWith(QString(".yaml")))
            {
                toYmlFile(label.toStdString());
            }
            else if(label.endsWith(QString(".dot")))
            {
                toDotFile(label.toStdString());
            }
            else
            {
                QMessageBox::critical(this, tr("Graph Export Failed"), QString(std::string(std::string("Unsupported file format for file '") + label.toStdString() + "'!").c_str()));
                updateStatus(std::string("Failed to export graph: to output file '") + label.toStdString() + "': unsupported file format!", DEFAULT_TIMEOUT);
                return;
            }
        }
        else
        {
            if(selectedFilter.startsWith("GEXF"))
            {
                toXmlFile(label.toStdString() + ".gexf");
            }
            else if(selectedFilter.startsWith("YAML"))
            {
                toYmlFile(label.toStdString() + ".yml");
            }
            else
            {
                toDotFile(label.toStdString() + ".dot");
            }
        }
    }
    else
    {
        updateStatus(std::string("Failed to export graph: aborted by user - an empty output filename was provided!"), DEFAULT_TIMEOUT);
    }
}

void ViewWidget::reloadPropertyDialog()
{
    updateStatus(std::string("reloading command panel..."));
    if(mpPropertyDialog)
    {
        delete mpPropertyDialog;
    }
    mpPropertyDialog = new PropertyDialog(this, mpLayerWidget, mpMainWindow, mpStackedWidget, mDragDrop);
    updateStatus(std::string("Reloaded command panel!"), DEFAULT_TIMEOUT);
}

Edge::Ptr ViewWidget::createEdge(Vertex::Ptr sourceNode, Vertex::Ptr targetNode, const std::string& label)
{
    Edge::Ptr edge(new Edge(sourceNode, targetNode, label));
    mpGraph->addEdge(edge);
    enableEdge(edge);
    return edge;
}

Vertex::Ptr ViewWidget::createVertex(const std::string& type, const std::string& label)
{
    std::set<std::string> types = VertexTypeManager::getInstance()->getSupportedTypes();
    std::set<std::string>::iterator type_it = types.find(type);
    if(types.end() == type_it)
    {
        std::string error_msg = std::string("graph_analysis::ViewWidget::createVertex: Given Vertex Type '") + type + "' is not registered!";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    graph_analysis::Vertex::Ptr vertex = VertexTypeManager::getInstance()->createVertex(type, label);
    mpGraph->addVertex(vertex);
    enableVertex(vertex);
    return vertex;
}

Edge::Ptr ViewWidget::createStandaloneEdge(Vertex::Ptr sourceNode, Vertex::Ptr targetNode, const std::string& label)
{
    Edge::Ptr edge(new Edge(sourceNode, targetNode, label));
    return edge;
}

Vertex::Ptr ViewWidget::createStandaloneVertex(const std::string& type, const std::string& label)
{
    std::set<std::string> types = VertexTypeManager::getInstance()->getSupportedTypes();
    std::set<std::string>::iterator type_it = types.find(type);
    if(types.end() == type_it)
    {
        std::string error_msg = std::string("graph_analysis::ViewWidget::createVertex: Given Vertex Type '") + type + "' is not registered!";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    graph_analysis::Vertex::Ptr vertex = VertexTypeManager::getInstance()->createVertex(type, label);
    return vertex;
}

void ViewWidget::addNodeAdhoc(QObject *pos)
{
    updateStatus(std::string("adding new node..."));
    QPoint *position = (QPoint *)pos;
    AddNodeDialog nodeDialog;
    if(nodeDialog.isValid())
    {
        graph_analysis::Vertex::Ptr vertex = VertexTypeManager::getInstance()->createVertex(nodeDialog.getNodeType(), nodeDialog.getNodeLabel());
        mpGraph->addVertex(vertex);
        mpLayoutingGraph->addVertex(vertex);
        enableVertex(vertex);
        // Registering the new node item
        NodeItem* nodeItem = NodeTypeManager::getInstance()->createItem(this, vertex);
        nodeItem->setPos((double) position->x(), (double) position->y());
        mNodeItemMap[vertex] = nodeItem;
        scene()->addItem(nodeItem);
        updateStatus(std::string("Added new node '") + vertex->toString() + "' of type '" + vertex->getClassName() + "'!", DEFAULT_TIMEOUT);
    }
    else
    {
        updateStatus(std::string("Failed to add new node: aborted by user!"), DEFAULT_TIMEOUT);
    }
}

void ViewWidget::changeFocusedVertexLabel()
{
    updateStatus(std::string("renaming focused node..."));
    bool ok;
    QString label = QInputDialog::getText(this, tr("Input Node Label"),
                                         tr("New Label:"), QLineEdit::Normal,
                                         QString(mpFocusedVertex->getLabel().c_str()), &ok);
    if (ok && !label.isEmpty())
    {
        std::string old_label = mpFocusedVertex->toString();
        std::string new_label = label.toStdString();
        changeVertexLabel(mpFocusedVertex, new_label);
        updateStatus(std::string("Renamed focused node '") + old_label + "' to '" + new_label + "'!", DEFAULT_TIMEOUT);
    }
    else
    {
        updateStatus(std::string("Failed to rename focused node: aborted by user!"), DEFAULT_TIMEOUT);
    }
}

void ViewWidget::changeSelectedVertexLabel()
{
    updateStatus(std::string("renaming selected node..."));
    bool ok;
    QString label = QInputDialog::getText(this, tr("Input Node Label"),
                                         tr("New Label:"), QLineEdit::Normal,
                                         QString(mpSelectedVertex->getLabel().c_str()), &ok);
    if (ok && !label.isEmpty())
    {
        std::string old_label = mpSelectedVertex->toString();
        std::string new_label = label.toStdString();
        changeVertexLabel(mpSelectedVertex, new_label);
        updateStatus(std::string("Renamed selected node '") + old_label + "' to '" + new_label + "'!", DEFAULT_TIMEOUT);
    }
    else
    {
        updateStatus(std::string("Failed to rename selected node: aborted by user!"), DEFAULT_TIMEOUT);
    }
}

void ViewWidget::changeVertexLabel(graph_analysis::Vertex::Ptr vertex, const std::string& label)
{
    vertex->setLabel(label);
    NodeItem* nodeItem = mNodeItemMap[vertex];
    if(!nodeItem)
    {
        std::string error_msg = std::string("graph_analysis::ViewWidget::changeVertexLabel: provided vertex '") + vertex->getLabel() + "' is not registered with the GUI";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    nodeItem->updateLabel();
}

void ViewWidget::changeFocusedEdgeLabel()
{
    updateStatus(std::string("renaming focused edge..."));
    bool ok;
    QString label = QInputDialog::getText(this, tr("Input Edge Label"),
                                         tr("New Label:"), QLineEdit::Normal,
                                          QString(mpFocusedEdge->getLabel().c_str()), &ok);
    if (ok && !label.isEmpty())
    {
        std::string old_label = mpFocusedEdge->toString();
        std::string new_label = label.toStdString();
        changeEdgeLabel(mpFocusedEdge, new_label);
        updateStatus(std::string("Renamed focused edge '") + old_label + "' to '" + new_label + "'!", DEFAULT_TIMEOUT);
    }
    else
    {
        updateStatus(std::string("Failed to rename focused edge: aborted by user!"), DEFAULT_TIMEOUT);
    }
}

void ViewWidget::changeSelectedEdgeLabel()
{
    updateStatus(std::string("renaming selected edge..."));
    bool ok;
    QString label = QInputDialog::getText(this, tr("Input Edge Label"),
                                         tr("New Label:"), QLineEdit::Normal,
                                          QString(mpSelectedEdge->getLabel().c_str()), &ok);
    if (ok && !label.isEmpty())
    {
        std::string old_label = mpSelectedEdge->toString();
        std::string new_label = label.toStdString();
        changeEdgeLabel(mpSelectedEdge, new_label);
        updateStatus(std::string("Renamed selected edge '") + old_label + "' to '" + new_label + "'!", DEFAULT_TIMEOUT);
    }
    else
    {
        updateStatus(std::string("Failed to rename selected edge: aborted by user!"), DEFAULT_TIMEOUT);
    }
}

void ViewWidget::changeEdgeLabel(graph_analysis::Edge::Ptr concernedEdge, const std::string& label)
{
    EdgeItem* edge = mEdgeItemMap[concernedEdge];
    if(!edge)
    {
        std::string error_msg = std::string("graph_analysis::ViewWidget::changeEdgeLabel: provided edge '") + concernedEdge->getLabel() + "' is not registered with the GUI";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    concernedEdge->setLabel(label);
    mEdgeMap[concernedEdge]->setLabel(label);
    graphitem::edges::EdgeLabel* edgeLabel = (graphitem::edges::EdgeLabel *) edge->getLabel();
    edgeLabel->setPlainText(QString(label.c_str()));
    edge->adjustLabel();
}

void ViewWidget::removeFocusedEdge()
{
    clearEdge(mpFocusedEdge);
    clearEdgeFocus();
}

void ViewWidget::removeSelectedEdge()
{
    clearEdge(mpSelectedEdge);
}

void ViewWidget::clearEdge(graph_analysis::Edge::Ptr concernedEdge)
{
    std::string concernedEdgeLabel = concernedEdge->toString();
    updateStatus(std::string("removing edge '") + concernedEdgeLabel + "'...");
    EdgeItem *edge = mEdgeItemMap[concernedEdge];
    if(!edge)
    {
        std::string error_msg = std::string("graph_analysis::ViewWidget::clearEdge: provided edge '") + concernedEdge->getLabel() + "' is not registered with the GUI";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    scene()->removeItem(edge);
    const std::string label = concernedEdge->getLabel();
    const graph_analysis::Vertex::Ptr sourceClusterVertex = concernedEdge->getSourceVertex();
    const graph_analysis::Vertex::Ptr targetClusterVertex = concernedEdge->getTargetVertex();
    const NodeItem * sceneSourceNodeItem = mNodeItemMap[sourceClusterVertex];
    const NodeItem * sceneTargetNodeItem = mNodeItemMap[targetClusterVertex];
    if(!sceneSourceNodeItem || !sceneTargetNodeItem)
    {
        std::string error_msg = std::string("graph_analysis::gui::ViewWidget::removeSelectedEdge: the selected edge '")
                                        + label + "' in the layouting graph mpLayoutingGraph has invalid Vertex endpoints or invalid scene items correspondands";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }

    EdgeIterator::Ptr edgeIt = mpGraph->getEdgeIterator();
    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();
        if(label != edge->getLabel())
        {
            continue;
        }
        NodeItem * sourceNodeItem = mPortMap[edge->getSourceVertex()];
        NodeItem * targetNodeItem = mPortMap[edge->getTargetVertex()];
        if(!sourceNodeItem || !targetNodeItem)
        {
            std::string error_msg = std::string("graph_analysis::gui::ViewWidget::removeSelectedEdge: the iterated-over edge '")
                                            + label + "' in the main graph mpGraph has invalid Vertex endpoints or invalid scene items correspondands";
            LOG_ERROR_S << error_msg;
            throw std::runtime_error(error_msg);
        }
        if  (
                    sourceClusterVertex == sourceNodeItem->getVertex() && sceneSourceNodeItem == sourceNodeItem
                &&  targetClusterVertex == targetNodeItem->getVertex() && sceneTargetNodeItem == targetNodeItem
            )
        {
            mpGraph->removeEdge(edge);
            break; // assuming there is a single correspondent edge in the main graph mpGraph
        }
    }
    mpLayoutingGraph->removeEdge(concernedEdge);
    syncEdgeItemMap(concernedEdge);
    updateStatus(std::string("Removed edge '") + concernedEdgeLabel + "'!", DEFAULT_TIMEOUT);
}

void ViewWidget::removeFocusedVertex()
{
    clearVertex(mpFocusedVertex);
    clearNodeFocus();
}

void ViewWidget::removeSelectedVertex()
{
    clearVertex(mpSelectedVertex);
}

void ViewWidget::clearVertex(graph_analysis::Vertex::Ptr concernedVertex)
{
    std::string concernedVertexLabel = concernedVertex->toString();
    updateStatus(std::string("removing node '") + concernedVertexLabel + "'...");
    // removing possible (default?) edges of this cluster node within the main graph (and its port-vertices)
    if("graph_analysis::ClusterVertex" != concernedVertex->getClassName())
    {
        std::string error_msg = std::string("graph_analysis::ViewWidget::clearVertex: the supplied vertex '") + concernedVertex->getLabel()
                                        + "' is of unexpected type '" + concernedVertex->getClassName() + "'";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    EdgeIterator::Ptr edgeIt = mpGraph->getEdgeIterator(concernedVertex);
    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();
        Vertex::Ptr portVertex;
        Vertex::Ptr sourceVertex = edge->getSourceVertex();
        Vertex::Ptr targetVertex = edge->getTargetVertex();
        if(sourceVertex != concernedVertex)
        {
            portVertex = sourceVertex;
        }
        else
        {
            portVertex = targetVertex;
        }
        // removing all edges of the portVertex
        EdgeIterator::Ptr edgeIterator = mpGraph->getEdgeIterator(portVertex);
        while(edgeIterator->next())
        {
            mpGraph->removeEdge(edgeIterator->current());
        }
        mpGraph->removeVertex(portVertex);
        // already removed the edge itself up above //       mpGraph->removeEdge(edge);
    }
    // removing default edges (in the secondary/layouting graph and elliminating their corresponding graphical representations off the screen)
    edgeIt = mpLayoutingGraph->getEdgeIterator(concernedVertex);
    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();
        EdgeItem *edgeItem = mEdgeItemMap[edge];
        if(edgeItem)
        {
            scene()->removeItem(edgeItem);
        }
        syncEdgeItemMap(edge);
//        mpLayoutingGraph->removeEdge(edge); // commented out since it introduces bugs when mpLayoutingGraph is dirty
    }
//    mpLayoutingGraph->removeVertex(concernedVertex); // commented out since it introduces bugs when mpLayoutingGraph is 'dirty'
    NodeItem *item = mNodeItemMap[concernedVertex];
    if(!item)
    {
        std::string error_msg = std::string("graph_analysis::ViewWidget::removeVertex: provided vertex '") + concernedVertex->getLabel() + "' is not registered with the GUI";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    scene()->removeItem(item);
    mpGraph->removeVertex(concernedVertex);
    updateStatus(std::string("Removed node '") + concernedVertexLabel + "'!", DEFAULT_TIMEOUT);
}

void ViewWidget::changeLayout()
{
    updateStatus(std::string("changing graph layout..."));
    bool ok;
    QStringList options;
    std::set<std::string> supportedLayouts = mpGVGraph->getSupportedLayouts();
//    options << tr("circo") << tr("dot") << tr("fdp") << tr("neato") << tr("osage") << tr("sfdp") << tr("twopi"); // [circo, dot, fdp, neato, osage, sfdp, twopi]
    foreach(std::string supportedLayout, supportedLayouts)
    {
        options << tr(supportedLayout.c_str());
    }
    QString layout = QInputDialog::getItem(this, tr("Input New Layout"),
                                         tr("select a layout:"), options,
                                         0, false, &ok);
    if (ok && !layout.isEmpty())
    {
        std::string desiredLayout = layout.toStdString();
        reset(true /*keepData*/);
        setLayout(QString(desiredLayout.c_str()));
        updateStatus(std::string("Changed graph layout to '") + desiredLayout + "'!", DEFAULT_TIMEOUT);
    }
    else
    {
        updateStatus(std::string("Failed to change graph layout: aborted by user!"), DEFAULT_TIMEOUT);
    }
}

void ViewWidget::setStartVertex(graph_analysis::Vertex::Ptr startVertex, int portID)
{
    updateStatus(std::string("drag-n-drop: setting source node to '") + startVertex->toString() + "' (port=" + boost::lexical_cast<std::string>(portID) + ")...");
    if("graph_analysis::ClusterVertex" != startVertex->getClassName())
    {
        std::string error_msg = std::string("graph_analysis::gui::ViewWidget::setStartVertex: expected startVertex to be of type 'graph_analysis::ClusterVertex'; instead, found type '")
                                        + startVertex->getClassName() + "'";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    mpStartVertex   = startVertex;
    NodeItem *item  = mNodeItemMap[startVertex];
    mpStartPort     = item->getPort(portID);
    updateStatus(std::string("Drag-n-drop: set source node to '") + startVertex->toString() + "' (port=" + boost::lexical_cast<std::string>(portID) + ")...", DEFAULT_TIMEOUT);
}

void ViewWidget::setEndVertex(graph_analysis::Vertex::Ptr endVertex, int portID)
{
    updateStatus(std::string("drag-n-drop: setting target node to '") + endVertex->toString() + "' (port=" + boost::lexical_cast<std::string>(portID) + ")...");
    if("graph_analysis::ClusterVertex" != endVertex->getClassName())
    {
        std::string error_msg = std::string("graph_analysis::gui::ViewWidget::setEndVertex: expected endVertex to be of type 'graph_analysis::ClusterVertex'; instead, found type '")
                                        + endVertex->getClassName() + "'";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    mpEndVertex     = endVertex;
    NodeItem *item  = mNodeItemMap[endVertex];
    if(!item)
    {
        std::string error_msg = std::string("graph_analysis::ViewWidget::setEndVertex: provided vertex '") + endVertex->getLabel() + "' is not registered with the GUI";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    mpEndPort       = item->getPort(portID);
    NodeItem *sourceNodeItem = mNodeItemMap[mpStartVertex];
    NodeItem *targetNodeItem = item;
    updateStatus(std::string("Drag-n-drop: set target node to '") + endVertex->toString() + "' (port=" + boost::lexical_cast<std::string>(portID) + ")...", DEFAULT_TIMEOUT);
    if(sourceNodeItem == targetNodeItem)
    {
        // preventing self-edges and handling it into ports swapping
        NodeItem::portID_t start_portID = mPortIDMap[mpStartPort];
        NodeItem::portID_t   end_portID = mPortIDMap[mpEndPort];
        updateStatus(std::string("drag-n-drop: found identical source and target node '") + endVertex->toString()
                        + "' -> swapping ports of IDs " + boost::lexical_cast<std::string>(start_portID) + " and "
                        + boost::lexical_cast<std::string>(end_portID) + "..."
                    );
        if(mpStartPort->getClassName() != mpEndPort->getClassName())
        {
            std::string error_msg = std::string("The two ports are of different types '") + mpStartPort->getClassName() + "' and '" + mpEndPort->getClassName() + "'";
            LOG_WARN_S << "graph_analysis::gui::ViewWidget::setEndVertex: failed to initiate ports swapping: " << error_msg;
            QMessageBox::critical(this, tr("Ports Swapping Failed"), QString(error_msg.c_str()));
            updateStatus(std::string("Drag-n-drop failed: tried to swap ports '") + mpStartPort->toString() + "' and '"
                            + mpEndPort->toString() + "' of IDs " + boost::lexical_cast<std::string>(start_portID) + " and "
                            + boost::lexical_cast<std::string>(end_portID) + " respectively, of different types '"
                            + mpStartPort->getClassName() + "' and '" + mpEndPort->getClassName() + "' respectively!"
                            , DEFAULT_TIMEOUT
                        );
            return;
        }
        sourceNodeItem->swapPorts(start_portID, end_portID);
        updateStatus(std::string("Drag-n-drop: swapped ports '") + mpStartPort->toString() + "' and '"
                        + mpEndPort->toString() + "' of IDs " + boost::lexical_cast<std::string>(start_portID)
                        + " and " + boost::lexical_cast<std::string>(end_portID) + " respectively and of consistent type '"
                        + mpStartPort->getClassName() + "'!"
                        , DEFAULT_TIMEOUT
                    );
    }
    else
    {
        if("graph_analysis::OutputPortVertex" != mpStartPort->getClassName())
        {
            std::string error_msg = std::string("Expected associated source portVertex to be of type 'graph_analysis::OutputPortVertex'; instead, found type '")
                                            + mpStartPort->getClassName() + "'";
            LOG_WARN_S << "graph_analysis::gui::ViewWidget::setEndVertex: " << error_msg;
            QMessageBox::critical(this, tr("Edge Creation Failed"), QString(error_msg.c_str()));
            updateStatus(std::string("Drag-n-drop failed: '") + error_msg + "'!"
                            , DEFAULT_TIMEOUT
                        );
            return;
        }
        if("graph_analysis::InputPortVertex" != mpEndPort->getClassName())
        {
            std::string error_msg = std::string("Expected associated target portVertex to be of type 'graph_analysis::InputPortVertex'; instead, found type '")
                                            + mpEndPort->getClassName() + "'";
            LOG_WARN_S << "graph_analysis::gui::ViewWidget::setEndVertex: " << error_msg;
            QMessageBox::critical(this, tr("Edge Creation Failed"), QString(error_msg.c_str()));
            updateStatus(std::string("Drag-n-drop failed: '") + error_msg + "'!"
                            , DEFAULT_TIMEOUT
                        );
            return;
        }
        addEdgeAdHoc();
    }
}

void ViewWidget::addEdgeAdHoc() // assumes the concerned edge-creation member fields are properly set already
{
    updateStatus(std::string("drag-n-drop: adding edge..."));
    bool ok;
    QString label = QInputDialog::getText(this, tr("Input New Edge Label"),
                                         tr("New Edge Label:"), QLineEdit::Normal,
                                         QString("newEdge"), &ok);
    if (ok && !label.isEmpty())
    {
        std::string edge_label = label.toStdString();
        spawnEdge(edge_label); // assumes the concerned edge-creation member fields are properly set already
        NodeItem::portID_t start_portID = mPortIDMap[mpStartPort];
        NodeItem::portID_t   end_portID = mPortIDMap[mpEndPort];
        updateStatus(std::string("Drag-n-drop completed: added edge '") + edge_label + "' in between ports '"
                        + mpStartPort->toString() + "' and '"
                        + mpEndPort->toString() + "' of IDs " + boost::lexical_cast<std::string>(start_portID)
                        + " and " + boost::lexical_cast<std::string>(end_portID) + " respectively and of consistent type '"
                        + mpStartPort->getClassName() +  "' of clusters '"
                        + mpStartVertex->toString() + "' and '" + mpEndVertex->toString() + "'!"
                        , DEFAULT_TIMEOUT
                    );
    }
    else
    {
        updateStatus(std::string("Drag-n-drop failed: aborted by user!"), DEFAULT_TIMEOUT);
    }
}

void ViewWidget::spawnEdge(const std::string& label) // assumes the concerned edge-creation member fields are properly set already
{
    NodeItem *sourceNodeItem = mNodeItemMap[mpStartVertex];
    NodeItem *targetNodeItem = mNodeItemMap[mpEndVertex];
    if(sourceNodeItem && targetNodeItem)
    {
        if(sourceNodeItem == targetNodeItem)
        {
            // preventing self-edges (handled it into ports swapping already)
            return;
        }
        Edge::Ptr edge(new Edge(mpStartPort, mpEndPort, label));
        mpGraph->addEdge(edge);
        enableEdge(edge);
        Edge::Ptr default_edge(new Edge(mpStartVertex, mpEndVertex, label));
        mpLayoutingGraph->addEdge(default_edge);

        EdgeItem* edgeItem = EdgeTypeManager::getInstance()->createItem(this, sourceNodeItem, mPortIDMap[mpStartPort], targetNodeItem, mPortIDMap[mpEndPort], default_edge);
        scene()->addItem(edgeItem);
        edgeItem->adjust();
        mEdgeItemMap[default_edge] = edgeItem;
//            mpGVGraph->addEdge(default_edge); // not vital since the mpGVGraph gets updated every single time before (rendering with) layouting engines
    }
    else
    {
        std::string error_msg = std::string("graph_analysis::gui::ViewWidget::spawnEdge: could not insert new edge of label '") + label
                                        + "' since its endpoint vertices are not both registered in mNodeItemMap";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
}

void ViewWidget::toYmlFile(const std::string& filename)
{
    try
    {
        mWriterMap["yaml"]->write(filename, mpGraph);
    }
    catch(std::runtime_error e)
    {
        LOG_ERROR_S << "graph_analysis::gui::ViewWidget::toYmlFile: export failed: " << e.what();
        QMessageBox::critical(this, tr("Graph Export Failed"), QString(e.what()));
        updateStatus(std::string("Yaml Graph export failed: ") + std::string(e.what()), DEFAULT_TIMEOUT);
    }
}

void ViewWidget::toDotFile(const std::string& filename)
{
    try
    {
        mWriterMap["dot"]->write(filename, mpGraph);
    }
    catch(std::runtime_error e)
    {
        LOG_ERROR_S << "graph_analysis::gui::ViewWidget::toDotFile: export via graphviz failed: " << e.what();
        QMessageBox::critical(this, tr("Graph Export via GraphViz Failed"), QString(e.what()));
        updateStatus(std::string("Dot Graph export failed: ") + std::string(e.what()), DEFAULT_TIMEOUT);
    }
}

void ViewWidget::gvRender(const std::string& filename)
{
    try
    {
        mpGVGraph->renderToFile(filename, mLayout.toStdString());
    }
    catch(std::runtime_error e)
    {
        LOG_ERROR_S << "graph_analysis::gui::ViewWidget::toDotFile: export via graphviz failed: " << e.what();
        QMessageBox::critical(this, tr("Graph Export via GraphViz Failed"), QString(e.what()));
        updateStatus(std::string("Dot Graph export failed: ") + std::string(e.what()), DEFAULT_TIMEOUT);
    }
}

void ViewWidget::toXmlFile(const std::string& filename)
{
    try
    {
        mWriterMap["gexf"]->write(filename, mpGraph);
    }
    catch(std::runtime_error e)
    {
        LOG_ERROR_S << "graph_analysis::gui::ViewWidget::toXmlFile: export to .gexf failed: " << e.what();
        QMessageBox::critical(this, tr("Graph Export to .gexf Failed"), QString(e.what()));
        updateStatus(std::string("Gexf Graph export failed: ") + std::string(e.what()), DEFAULT_TIMEOUT);
    }
}

void ViewWidget::fromXmlFile(const std::string& filename)
{
    graph_analysis::BaseGraph::Ptr graph = BaseGraph::Ptr( new gl::DirectedGraph() );
    try
    {
        mReaderMap["gexf"]->read(filename, graph);
    }
    catch(std::runtime_error e)
    {
        LOG_ERROR_S << "graph_analysis::gui::ViewWidget::fromXmlFile: import from .gexf failed: " << e.what();
        QMessageBox::critical(this, tr("Graph Import from .gexf Failed"), QString(e.what()));
        updateStatus(std::string("Gexf Graph import failed: ") + std::string(e.what()), DEFAULT_TIMEOUT);
        return;
    }

    mpGraph = graph;
    mpLayerWidget->setGraph(mpGraph);
    mpSubGraph = mGraphView.apply(mpGraph);
    mFiltered = true;

    mpSubGraph->enableAllVertices();
    mpSubGraph->enableAllEdges();
    refresh();
}

void ViewWidget::fromYmlFile(const std::string& filename)
{
    graph_analysis::BaseGraph::Ptr graph = BaseGraph::Ptr( new gl::DirectedGraph() );
    try
    {
        mReaderMap["yaml"]->read(filename, graph);
    }
    catch(std::runtime_error e)
    {
        LOG_ERROR_S << "graph_analysis::gui::ViewWidget::fromYmlFile: import from .yaml failed: " << e.what();
        QMessageBox::critical(this, tr("Graph Import from .yaml Failed"), QString(e.what()));
        updateStatus(std::string("Yaml Graph import failed: ") + std::string(e.what()), DEFAULT_TIMEOUT);
        return;
    }

    mpGraph = graph;
    mpLayerWidget->setGraph(mpGraph);
    mpSubGraph = mGraphView.apply(mpGraph);
    mFiltered = true;

    mpSubGraph->enableAllVertices();
    mpSubGraph->enableAllEdges();
    refresh();
}

void ViewWidget::reset(bool keepData)
{
    clear();

    if(!keepData)
    {
        mpGraph = BaseGraph::Ptr( new gl::DirectedGraph() );
        mpLayerWidget->setGraph(mpGraph);
        mpSubGraph = mGraphView.apply(mpGraph);
        mFiltered = true;
    }

    if(mpGVGraph)
    {
        delete mpGVGraph;
    }
    mMaxNodeHeight  = 0;
    mMaxNodeWidth   = 0;
    mpLayoutingGraph = BaseGraph::Ptr( new gl::DirectedGraph() );
    mpGVGraph = new io::GVGraph(mpLayoutingGraph, "GVViewWidget");
}

void ViewWidget::clear()
{
    // TO-DO: add filtering clearing?
    if(mpGVGraph)
    {
        mpGVGraph->clearEdges();
        mpGVGraph->clearNodes();
    }

    mNodeItemMap.clear();
    mEdgeItemMap.clear();
    mPortIDMap.clear();
    mPortMap.clear();
    mEdgeMap.clear();
    scene()->clear();
}

void ViewWidget::refresh()
{
    if(mInitialized)
    {
        updateStatus(std::string("refreshing graph..."));
    }
    reset(true /*keepData*/);
    updateFromGraph();
    update();
    if(mInitialized)
    {
        updateStatus(std::string("Refreshed graph!"), DEFAULT_TIMEOUT);
    }
    else
    {
        mInitialized = true;
    }
}

void ViewWidget::enableVertex(graph_analysis::Vertex::Ptr vertex)
{
    mpSubGraph->enable(vertex);
    LOG_DEBUG_S << "Enabled vertex '" << vertex->getLabel() << "' of ID: " << mpSubGraph->getBaseGraph()->getVertexId(vertex);
}
void ViewWidget::enableEdge(graph_analysis::Edge::Ptr edge)
{
    mpSubGraph->enable(edge);
    LOG_DEBUG_S << "Enabled edge '" << edge->getLabel() << "' of ID:  " << mpSubGraph->getBaseGraph()->getEdgeId(edge);
}

void ViewWidget::disableVertex(graph_analysis::Vertex::Ptr vertex)
{
    mpSubGraph->disable(vertex);
    LOG_DEBUG_S << "Disabled vertex '" << vertex->getLabel() << "' of ID: " << mpSubGraph->getBaseGraph()->getVertexId(vertex);
}
void ViewWidget::disableEdge(graph_analysis::Edge::Ptr edge)
{
    mpSubGraph->disable(edge);
    LOG_DEBUG_S << "Disabled edge '" << edge->getLabel() << "' of ID:  " << mpSubGraph->getBaseGraph()->getEdgeId(edge);
}

void ViewWidget::updateFromGraph()
{
    /* !!! reset(true); shall be called apriori (unless you know what you're doing) !!! */
    VertexIterator::Ptr nodeIt = mpGraph->getVertexIterator();
    while(nodeIt->next())
    {
        Vertex::Ptr vertex = nodeIt->current();

        // Check on active filter
        if(mFiltered && !mpSubGraph->enabled(vertex))
        {
            LOG_DEBUG_S << "Filtered out a vertex of filtering value: " << mpSubGraph->enabled(vertex);
            continue;
        }

        if(mNodeItemMap.count(vertex))
        {
            continue;
        }

        if("graph_analysis::ClusterVertex" == vertex->getClassName())
        {
            // Registering new Cluster node items only
            NodeItem* nodeItem = NodeTypeManager::getInstance()->createItem(this, vertex);
            mNodeItemMap[vertex] = nodeItem;
            scene()->addItem(nodeItem);
            mpLayoutingGraph->addVertex(vertex);
            mpGVGraph->addNode(vertex);
        }
    }
    EdgeIterator::Ptr edgeIt = mpGraph->getEdgeIterator();
    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();

        // Check on active filter
        if(mFiltered && !mpSubGraph->enabled(edge))
        {
            LOG_DEBUG_S << "Filtered out an edge of filtering value: " << mpSubGraph->enabled(edge);
            continue;
        }

        if(mEdgeItemMap.count(edge))
        {
            continue;
        }

        Vertex::Ptr source = edge->getSourceVertex();
        Vertex::Ptr target = edge->getTargetVertex();

        if("graph_analysis::OutputPortVertex" == source->getClassName() && "graph_analysis::InputPortVertex" == target->getClassName())
        {
            // physical edge - processing deflected until after all ports will have been registered
            continue;
        }
        else if (
                    ("graph_analysis::PortVertex" == source->getClassName() && "graph_analysis::ClusterVertex" == target->getClassName())
                )
        {
            // semantical edge: links a cluster vertex to one of its ports
            std::string warn_msg = std::string("graph_analysis::ViewWidget::updateFromGraph: found reversed edge from source Port vertex '") +
                                        source->toString() + "' of type '" + source->getClassName() + "' to target Cluster vertex '" +
                                        target->toString() + "' of type '" + target->getClassName() + "'!";
            LOG_WARN_S << warn_msg; // warn. due to cluster being set as target of the semantically valid edge
            NodeItem* targetNodeItem = mNodeItemMap[ target ];
            if(!targetNodeItem)
            {
                continue;
            }
            mPortMap[source] = targetNodeItem;
            mPortIDMap[source] = targetNodeItem->addPort(source);
        }
        else if (
                    (
                        "graph_analysis::ClusterVertex" == source->getClassName()
                            &&
                        (
                            "graph_analysis::InputPortVertex" == target->getClassName()
                                ||
                            "graph_analysis::OutputPortVertex" == target->getClassName()
                        )
                    )
                )
        {
            // semantical edge: links a cluster vertex to one of its ports
            NodeItem* sourceNodeItem = mNodeItemMap[ source ];
            if(!sourceNodeItem)
            {
                continue;
            }
            mPortMap[target] = sourceNodeItem;
            mPortIDMap[target] = sourceNodeItem->addPort(target);
        }
        else
        {
            // invalid edge
            std::string error_msg = std::string("graph_analysis::ViewWidget::updateFromGraph: found invalid edge from source vertex '") +
                                        source->toString() + "' of type '" + source->getClassName() + "' to target vertex '" +
                                        target->toString() + "' of type '" + target->getClassName() + "'!";
            LOG_ERROR_S << error_msg;
            throw std::runtime_error(error_msg);
        }
    }

    edgeIt = mpGraph->getEdgeIterator();
    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();

        // Check on active filter
        if(mFiltered && !mpSubGraph->enabled(edge))
        {
            LOG_DEBUG_S << "Filtered out an edge of filtering value: " << mpSubGraph->enabled(edge);
            continue;
        }

        if(mEdgeItemMap.count(edge))
        {
            continue;
        }

        Vertex::Ptr source = edge->getSourceVertex();
        Vertex::Ptr target = edge->getTargetVertex();

        if("graph_analysis::OutputPortVertex" == source->getClassName() && "graph_analysis::InputPortVertex" == target->getClassName())
        {
            NodeItem* sourceNodeItem = mPortMap[ source ];
            NodeItem* targetNodeItem = mPortMap[ target ];
            // physical edge - processing was deflected until after all ports will have been registered
            Edge::Ptr default_edge(new Edge(sourceNodeItem->getVertex(), targetNodeItem->getVertex(), edge->getLabel()));
            mpLayoutingGraph->addEdge(default_edge);
            EdgeItem* edgeItem = EdgeTypeManager::getInstance()->createItem(this, sourceNodeItem, mPortIDMap[source], targetNodeItem, mPortIDMap[target], default_edge);
            scene()->addItem(edgeItem);
            mEdgeItemMap[default_edge] = edgeItem;
            mpGVGraph->addEdge(default_edge);
            mEdgeMap[default_edge] = edge;
        }
    }

    NodeItemMap::iterator node_it = mNodeItemMap.begin();
    for(; mNodeItemMap.end() != node_it; ++node_it)
    {
        QRectF nodeBoundingRect = node_it->second->boundingRect();
        qreal height = nodeBoundingRect.height();
        qreal width  = nodeBoundingRect.width();
        if(mMaxNodeHeight < height)
        {
            height = mMaxNodeHeight;
        }
        if(mMaxNodeWidth < width)
        {
            mMaxNodeWidth = width;
        }
    }

    syncDragDrop();

    if(mLayout.toLower() != "force")
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        LOG_INFO_S << "GV started layouting the graph. This can take a while ...";
        base::Time start = base::Time::now();
        mpGVGraph->setNodeAttribute("height", boost::lexical_cast<std::string>(mMaxNodeHeight));
        mpGVGraph->setNodeAttribute("width" , boost::lexical_cast<std::string>(mMaxNodeWidth ));
        mpGVGraph->applyLayout(mLayout.toStdString());
        base::Time delay = base::Time::now() - start;
        QApplication::restoreOverrideCursor();
        LOG_INFO_S << "GV layouted the graph after " << delay.toSeconds();
        {
            using namespace graph_analysis::io;
            std::vector<GVNode> nodes = mpGVGraph->nodes();
            std::vector<GVNode>::const_iterator cit = nodes.begin();
            for(; cit != nodes.end(); ++cit)
            {
                GVNode gvNode = *cit;
                NodeItem* nodeItem = mNodeItemMap[gvNode.getVertex()];
                if(!nodeItem)
                {
                    LOG_WARN_S << "NodeItem: mapped from " <<  gvNode.getVertex()->toString() << "is null";
                }
                nodeItem->setPos(mScaleFactor * gvNode.x(), mScaleFactor * gvNode.y());
            }
        }

//        {
//            using namespace graph_analysis::io;
//            std::vector<GVEdge> edges = mpGVGraph->edges();
//            std::vector<GVEdge>::const_iterator cit = edges.begin();
//            for(; cit != edges.end(); ++cit)
//            {
//                GVEdge gvEdge = *cit;
//                EdgeItem* edgeItem = mEdgeItemMap[ gvEdge.getEdge() ];
//                edgeItem->setPainterPath( edge.path );
//            }
//        }
    }
}

void ViewWidget::addVertex(Vertex::Ptr vertex)
{
    mpGraph->addVertex(vertex);
}

void ViewWidget::addEdge(Edge::Ptr edge)
{
    mpGraph->addEdge(edge);
}

void ViewWidget::removeVertex(Vertex::Ptr vertex)
{
    mpGraph->removeVertex(vertex);
}

void ViewWidget::removeEdge(Edge::Ptr edge)
{
    mpGraph->removeEdge(edge);
}

void ViewWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
#ifdef CLEAR_BY_BACKGROUND
    bool defocusNodes = !(
                            (mVertexSelected && mVertexFocused && mpSelectedVertex == mpFocusedVertex)
                            ||
                            (mVertexFocused && mEdgeSelected)
                        )
                        ;
    bool defocusEdges = !(
                            (mEdgeSelected && mEdgeFocused && mpSelectedEdge == mpFocusedEdge)
                            ||
                            (mVertexSelected && mEdgeFocused)
                        )
                        ;
    if(defocusNodes)
    {
        clearNodeFocus();
    }
    if(defocusEdges)
    {
        clearEdgeFocus();
    }
#endif
    QGraphicsView::mouseDoubleClickEvent(event);
}

void ViewWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MidButton)
    {
        //Use ScrollHand Drag Mode to enable Panning
        setDragMode(ScrollHandDrag);
        QMouseEvent fake(event->type(), event->pos(), Qt::LeftButton, Qt::LeftButton, event->modifiers());
        QGraphicsView::mousePressEvent(&fake);
    }
    else
    {
        QGraphicsView::mousePressEvent(event);
    }
}

void ViewWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MidButton)
    {
        //Use ScrollHand Drag Mode to desable Panning
        setDragMode(NoDrag);
        QMouseEvent fake(event->type(), event->pos(), Qt::LeftButton, Qt::LeftButton, event->modifiers());
        QGraphicsView::mouseReleaseEvent(&fake);
    }
    else
    {
        QGraphicsView::mouseReleaseEvent(event);
    }
}

void ViewWidget::updateDragDrop(bool dragDrop)
{
    updateStatus(std::string("toggling drag-n-drop mode to ") + (dragDrop ? "true" : "false") + "...");
    mDragDrop = dragDrop;
    NodeItemMap::iterator it = mNodeItemMap.begin();
    for(; mNodeItemMap.end() != it; ++it)
    {
        NodeItem *current = it->second;
        if("graph_analysis::ClusterVertex" == current->getVertex()->getClassName())
        {
            current->setHandlesChildEvents(!mDragDrop);
            current->setFlag(QGraphicsItem::ItemIsMovable, !mDragDrop);
        }
    }
    updateStatus(std::string("Toggled drag-n-drop mode to ") + (dragDrop ? "true" : "false") + "...", DEFAULT_TIMEOUT);
}

void ViewWidget::syncDragDrop()
{
    NodeItemMap::iterator it = mNodeItemMap.begin();
    for(; mNodeItemMap.end() != it; ++it)
    {
        NodeItem *current = it->second;
        if("graph_analysis::ClusterVertex" == current->getVertex()->getClassName())
        {
            current->setHandlesChildEvents(!mDragDrop);
            current->setFlag(QGraphicsItem::ItemIsMovable, !mDragDrop);
        }
    }
}

void ViewWidget::setDragDrop()
{
    updateStatus(std::string("toggling drag-n-drop mode to true..."));
    mDragDrop = true;
    mpPropertyDialog->setDragDrop(true);
    if(mVertexSelected)
    {
        NodeItem *item = mNodeItemMap[mpSelectedVertex];
        item->unselect();
    }
    NodeItemMap::iterator it = mNodeItemMap.begin();
    for(; mNodeItemMap.end() != it; ++it)
    {
        NodeItem *current = it->second;
        if("graph_analysis::ClusterVertex" == current->getVertex()->getClassName())
        {
            current->setHandlesChildEvents(false); // of !mDragDrop
            current->setFlag(QGraphicsItem::ItemIsMovable, false);
        }
    }
    updateStatus(std::string("Toggled drag-n-drop mode to true!"), DEFAULT_TIMEOUT);
}

void ViewWidget::unsetDragDrop()
{
    updateStatus(std::string("toggling drag-n-drop mode to false..."));
    mDragDrop = false;
    mpPropertyDialog->setDragDrop(false);
    NodeItemMap::iterator it = mNodeItemMap.begin();
    for(; mNodeItemMap.end() != it; ++it)
    {
        NodeItem *current = it->second;
        if("graph_analysis::ClusterVertex" == current->getVertex()->getClassName())
        {
            current->setHandlesChildEvents(true); // of !mDragDrop
            current->setFlag(QGraphicsItem::ItemIsMovable);
        }
    }
    updateStatus(std::string("Toggled drag-n-drop mode to false!"), DEFAULT_TIMEOUT);
}

void ViewWidget::itemMoved()
{
    if (!mTimerId)
    {
        mTimerId = startTimer(1000 / 25);
    }
}

void ViewWidget::keyPressEvent(QKeyEvent *event)
{
    // check for a keys combination
    Qt::KeyboardModifiers modifiers = event->modifiers();

    if(modifiers & Qt::ControlModifier)
    {
        switch(event->key())
        {
            case Qt::Key_Q:
            case Qt::Key_W:
                exit(0);
            break;

            case Qt::Key_Plus:
                zoomIn();
            break;

            case Qt::Key_Minus:
                zoomOut();
            break;

            case Qt::Key_R:
                resetGraph();
            break;

            case Qt::Key_S:
                exportGraph();
            break;

            case Qt::Key_A:
                if(!mDragDrop)
                {
                    addNodeAdhoc();
                }
            break;

            case Qt::Key_I:
            case Qt::Key_O:
                importGraph();
            break;

            case Qt::Key_P:
                if(!mpPropertyDialog->isRunning())
                {
                    reloadPropertyDialog();
                }
            break;

            case Qt::Key_Left:
                if(!mDragDrop)
                {
                    rotate(qreal(-1.13));
                }
            break;

            case Qt::Key_Right:
                if(!mDragDrop)
                {
                    rotate(qreal( 1.13));
                }
            break;

            case Qt::Key_CapsLock:
            case Qt::Key_D:
                mDragDrop ? unsetDragDrop() : setDragDrop();
            break;
        }
    }
    else if(!mDragDrop)
    {
        switch(event->key())
        {
        //case Qt::Key_Up:
        //    break;
        //case Qt::Key_Down:
        //    break;
        //case Qt::Key_Left:
        //    break;
        //case Qt::Key_Right:
        //    break;
        case Qt::Key_Plus:
            zoomIn();
        break;

        case Qt::Key_Minus:
            zoomOut();
        break;

        case Qt::Key_Space:
        case Qt::Key_Enter:
        case Qt::Key_R:
            refresh();
        break;

        case Qt::Key_S:
            shuffle();
        break;

        case Qt::Key_L:
            changeLayout();
        break;

        //default:
        }
    }

    switch(event->key())
    {

    case Qt::Key_Escape:
        clearFocus();
    break;

    //default:
    }

    QGraphicsView::keyPressEvent(event);
}

void ViewWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    if(mLayout.toLower() == "force")
    {
        QList<NodeItem* > nodes;
        foreach (QGraphicsItem *item, scene()->items())
        {
            if (NodeItem* node = qgraphicsitem_cast<NodeItem* >(item))
            {
                nodes << node;
            }
        }

        foreach (NodeItem* node, nodes)
        {
            node->calculateForces();
        }

        bool itemsMoved = false;
        foreach (NodeItem* node, nodes)
        {
            if (node->advance())
                itemsMoved = true;
        }

        if (!itemsMoved)
        {
            killTimer(mTimerId);
            mTimerId = 0;
        }
    }
}

#ifndef QT_NO_WHEELEVENT
void ViewWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, - event->delta() / 240.0));
}
#endif

void ViewWidget::drawBackground(QPainter *painter, const QRectF& rect)
{
    Q_UNUSED(rect);
    //// Shadow
//    QRectF sceneRect = this->sceneRect();
    //QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    //QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    //if (rightShadow.intersects(rect) || rightShadow.contains(rect))
    //    painter->fillRect(rightShadow, Qt::darkGray);
    //if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
    //    painter->fillRect(bottomShadow, Qt::darkGray);

    // Fill
    //QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    //gradient.setColorAt(0, Qt::white);
    //gradient.setColorAt(1, Qt::lightGray);
    //painter->fillRect(rect.intersected(sceneRect), gradient);
    //painter->setBrush(Qt::NoBrush);
    //painter->drawRect(sceneRect);

    //// Text
    //QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
    //                sceneRect.width() - 4, sceneRect.height() - 4);
    //QString message(tr("Click and drag the nodes around, and zoom with the mouse "
    //                   "wheel or the '+' and '-' keys"));

    //QFont font = painter->font();
    //font.setBold(true);
    //font.setPointSize(14);
    //painter->setFont(font);
    //painter->setPen(Qt::lightGray);
    //painter->drawText(textRect.translated(2, 2), message);
    //painter->setPen(Qt::black);
    //painter->drawText(textRect, message);
}

void ViewWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
    {
        return;
    }
    scale(scaleFactor, scaleFactor);
    std::string status_msg = scaleFactor > 1. ? "Zoomed-in" : "Zoomed-out";
    updateStatus(status_msg, DEFAULT_TIMEOUT);
}

void ViewWidget::shuffle()
{
    updateStatus(std::string("shuflling all the nodes..."));
    foreach (QGraphicsItem *item, scene()->items())
    {
        if (qgraphicsitem_cast<NodeItem* >(item))
        {
            item->setPos(-150 * mScaleFactor + mScaleFactor * (qrand() % 300), -150 * mScaleFactor + mScaleFactor * (qrand() % 300));
        }
    }
    updateStatus(std::string("Shuflled all nodes!"), DEFAULT_TIMEOUT);
}

void ViewWidget::zoomIn()
{
    scaleView(qreal(1.13));
}

void ViewWidget::zoomOut()
{
    scaleView(1 / qreal(1.13));
}

void ViewWidget::resetGraph()
{
    updateStatus(std::string("resetting the graph..."));
    if(mpGraph->empty())
    {
        QMessageBox::information(this, tr("Nothing to Reset"), "The graph is already empty!");
        updateStatus(std::string("Failed to reset graph: the graph was already empty!"), DEFAULT_TIMEOUT);
    }
    else
    {
        QMessageBox::StandardButton button = QMessageBox::question(this, tr("Confirm Reset"), tr("The Graph will be completely erased! Are you sure you want to continue?"), QMessageBox::Yes | QMessageBox::No);
        switch(button)
        {
            case QMessageBox::Yes:
                reset();
                updateStatus(std::string("The graph was reset!"), DEFAULT_TIMEOUT);
            break;

            default:
                updateStatus(std::string("Failed to reset graph: aborted by user!"), DEFAULT_TIMEOUT);
            break;
        }
    }
}

void ViewWidget::setLayout(QString layoutName)
{
    /* !!! reset(true); shall be called apriori (unless you know what you're doing - e.g. at init) !!! */
    mLayout = layoutName;
    updateFromGraph();
}

void ViewWidget::setVertexFocused(bool focused)
{
    mVertexFocused = focused;
    mpPropertyDialog->setVertexFocused(focused);
}

void ViewWidget::setEdgeFocused(bool focused)
{
    mEdgeFocused = focused;
    mpPropertyDialog->setEdgeFocused(focused);
}

void ViewWidget::clearNodeFocus()
{
    if(mVertexFocused)
    {
        NodeItem *item = mNodeItemMap[mpFocusedVertex];
        item->releaseFocus();
        updateStatus(std::string("Cleared node focus off node '") + mpFocusedVertex->toString() + "'!", DEFAULT_TIMEOUT);
    }
}

void ViewWidget::clearEdgeFocus()
{
    if(mEdgeFocused)
    {
        EdgeItem *item = mEdgeItemMap[mpFocusedEdge];
        item->releaseFocus();
        updateStatus(std::string("Cleared edge focus off edge '") + mpFocusedEdge->toString() + "'!", DEFAULT_TIMEOUT);
    }
}

void ViewWidget::clearFocus()
{
    clearNodeFocus();
    clearEdgeFocus();
    updateStatus(std::string("Cleared focuses: ")
                    + "off node '" + mpFocusedVertex->toString()
                    + "' and off edge '" + mpFocusedEdge->toString() + "'!"
                    , DEFAULT_TIMEOUT
                );
}


void ViewWidget::removePortsFocused()
{
    removePorts(mpFocusedVertex);
}

void ViewWidget::removePortsSelected()
{
    removePorts(mpSelectedVertex);
}

void ViewWidget::removePorts(graph_analysis::Vertex::Ptr concernedVertex)
{
    updateStatus(std::string("removing all ports from vertex '") + concernedVertex->toString() + "' of type '" + concernedVertex->getClassName() + "'...");
    NodeItem *item = mNodeItemMap[concernedVertex];
    if(!item)
    {
        std::string error_msg = std::string("graph_analysis::ViewWidget::removePorts: provided vertex '") + concernedVertex->getLabel() + "' is not registered with the GUI";
        LOG_ERROR_S << error_msg;
        updateStatus(std::string("Failed to remove all ports from vertex '") + concernedVertex->toString() + "' of type '" + concernedVertex->getClassName() + "': " + error_msg + "!", DEFAULT_TIMEOUT);
        throw std::runtime_error(error_msg);
    }
    int nports = item->getPortCount();
    if(!nports)
    {
        QMessageBox::critical(this, tr("No Ports to Remove"), "The cluster is already empty!");
        updateStatus(std::string("Failed to remove all ports from vertex '") + concernedVertex->toString() + "' of type '" + concernedVertex->getClassName() + "': there are no ports!", DEFAULT_TIMEOUT);
        return;
    }
    else
    {
        QMessageBox::StandardButton button = QMessageBox::question(this, tr("Confirm Complete Ports-Removal"), tr((QString("All ports in node '") + QString(concernedVertex->getLabel().c_str()) + QString("' will be deleted! Are you sure you want to continue?")).toAscii()), QMessageBox::Yes | QMessageBox::No);
        EdgeIterator::Ptr edgeIt;
        graph_analysis::Vertex::Ptr cluster;
        switch(button)
        {
            case QMessageBox::Yes:
                // remove conceptual edges
                foreach(NodeItem::VTuple tuple, item->getVertices())
                {
                    edgeIt = mpGraph->getEdgeIterator(tuple.second);
                    while(edgeIt->next())
                    {
                        Edge::Ptr edge = edgeIt->current();
                        mpGraph->removeEdge(edge);
                    }
                }
                // remove physical edges and their graphics
                cluster = item->getVertex();
                edgeIt = mpLayoutingGraph->getEdgeIterator(cluster);
                while(edgeIt->next())
                {
                    Edge::Ptr edge = edgeIt->current();
                    if(mEdgeItemMap.count(edge))
                    {
                        EdgeItem *edgeItem = mEdgeItemMap[edge];
                        if(edgeItem)
                        {
                            mpLayoutingGraph->removeEdge(edge);
                            syncEdgeItemMap(edge);
                            scene()->removeItem(edgeItem);
                        }
                    }
                    else
                    {
                        mpLayoutingGraph->removeEdge(edge);
                        syncEdgeItemMap(edge);
                    }
                }
                // remove ports graphics
                item->removePorts();
                updateStatus(std::string("Removed all ports from vertex '") + concernedVertex->toString() + "' of type '" + concernedVertex->getClassName() + "'!", DEFAULT_TIMEOUT);
            break;

            default:
                updateStatus(std::string("Failed to remove all ports from vertex '") + concernedVertex->toString() + "' of type '" + concernedVertex->getClassName() + "': aborted by user!", DEFAULT_TIMEOUT);
            break;
        }
    }
}

void ViewWidget::syncEdgeItemMap(graph_analysis::Edge::Ptr concernedEdge)
{
    EdgeItemMap::iterator it = mEdgeItemMap.find(concernedEdge);
    if(mEdgeItemMap.end() != it)
    {
        mEdgeItemMap.erase(it);
    }
}

void ViewWidget::swapPortsFocused()
{
    swapPorts(mpFocusedVertex);
}

void ViewWidget::swapPortsSelected()
{
    swapPorts(mpSelectedVertex);
}

void ViewWidget::swapPorts(graph_analysis::Vertex::Ptr concernedVertex)
{
    updateStatus(std::string("swapping ports within node '") + concernedVertex->toString() + "'...");
    NodeItem *item = mNodeItemMap[concernedVertex];
    if(!item)
    {
        std::string error_msg = std::string("graph_analysis::ViewWidget::swapPorts: provided vertex '") + concernedVertex->getLabel() + "' is not registered with the GUI";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    int portCount = item->getPortCount();
    if(portCount < 2)
    {
        QMessageBox::critical(this, tr("Cannot Swap Ports"), tr("The selected vertex did not have enough ports!"));
        updateStatus(std::string("Failed to swap ports within node '") + concernedVertex->toString() + "': this vertex did not have enough ports!", DEFAULT_TIMEOUT);
        return;
    }
    SwapPortsDialog dialog(item);
    if(dialog.isValid())
    {
        std::string strPort1ID   = dialog.getPort1ID();
        std::string strPort2ID   = dialog.getPort2ID();
        int port1ID;
        std::stringstream ss1(strPort1ID);
        ss1 >> port1ID;
        int port2ID;
        std::stringstream ss2(strPort2ID);
        ss2 >> port2ID;
        if(port1ID - port2ID)
        {
            try
            {
                item->swapPorts(port1ID, port2ID);
            }
            catch(std::runtime_error e)
            {
                LOG_ERROR_S << "graph_analysis::gui::ViewWidget::swapPorts: swapping operation failed: " << e.what();
                QMessageBox::critical(this, tr("Swapping Failed"), QString(e.what()));
                updateStatus(std::string("Failed to swap ports within node '") + concernedVertex->toString() + "': " + std::string(e.what()) + "!", DEFAULT_TIMEOUT);
            }
        }
        else
        {
            QMessageBox::information(this, tr("Swapped Ports In-place"), tr("identical ports were selected!"));
        }
        updateStatus(std::string("Swapped ports of IDs ") + boost::lexical_cast<std::string>(port1ID) + " and "
                        + boost::lexical_cast<std::string>(port2ID) + " within node '" + concernedVertex->toString()
                        + "'!"
                        , DEFAULT_TIMEOUT
                    );
    }
    else
    {
        updateStatus(std::string("Failed to swap ports within node '") + concernedVertex->toString() + "': aborted by user!", DEFAULT_TIMEOUT);
    }
}

void ViewWidget::changeFocusedEdgeLabelMainWindow()
{
    if(mEdgeFocused)
    {
        changeFocusedEdgeLabel();
    }
    else
    {
        QMessageBox::information(this, tr("Cannot Rename Focused Edge"), tr("Cannot Rename Focused Edge: no edge is focused on!"));
    }
}

void ViewWidget::removeFocusedEdgeMainWindow()
{
    if(mEdgeFocused)
    {
        removeFocusedEdge();
    }
    else
    {
        QMessageBox::information(this, tr("Cannot Remove Focused Edge"), tr("Cannot Remove Focused Edge: no edge is focused on!"));
    }
}

void ViewWidget::changeFocusedVertexLabelMainWindow()
{
    if(mVertexFocused)
    {
        changeFocusedVertexLabel();
    }
    else
    {
        QMessageBox::information(this, tr("Cannot Rename Focused Node"), tr("Cannot Rename Focused Node: no node is focused on!"));
    }
}

void ViewWidget::removeFocusedVertexMainWindow()
{
    if(mVertexFocused)
    {
        removeFocusedVertex();
    }
    else
    {
        QMessageBox::information(this, tr("Cannot Remove Focused Node"), tr("Cannot Remove Focused Node: no node is focused on!"));
    }
}

void ViewWidget::addPortFocusedMainWindow()
{
    if(mVertexFocused)
    {
        addPortFocused();
    }
    else
    {
        QMessageBox::information(this, tr("Cannot Add a Port to the Focused Node"), tr("Cannot Add a Port to the Focused Node: no node is focused on!"));
    }
}

void ViewWidget::swapPortsFocusedMainWindow()
{
    if(mVertexFocused)
    {
        swapPortsFocused();
    }
    else
    {
        QMessageBox::information(this, tr("Cannot Swap Ports of the Focused Node"), tr("Cannot Swap Ports of the Focused Node: no node is focused on!"));
    }
}

void ViewWidget::renamePortFocusedMainWindow()
{
    if(mVertexFocused)
    {
        renamePortFocused();
    }
    else
    {
        QMessageBox::information(this, tr("Cannot Rename a Port of the Focused Node"), tr("Cannot Rename a Port of the Focused Node: no node is focused on!"));
    }
}

void ViewWidget::removePortFocusedMainWindow()
{
    if(mVertexFocused)
    {
        removePortFocused();
    }
    else
    {
        QMessageBox::information(this, tr("Cannot Remove a Port of the Focused Node"), tr("Cannot Remove a Port of the Focused Node: no node is focused on!"));
    }
}

void ViewWidget::removePortsFocusedMainWindow()
{
    if(mVertexFocused)
    {
        removePortsFocused();
    }
    else
    {
        QMessageBox::information(this, tr("Cannot Remove the Ports of the Focused Node"), tr("Cannot Remove the Ports of the Focused Node: no node is focused on!"));
    }
}

void ViewWidget::addNodeAdhocMainWindow()
{
    addNodeAdhoc();
}

void ViewWidget::refreshMainWindow()
{
    switch(mpStackedWidget->currentIndex())
    {
    case 0:
        this->refresh();
    break;

    case 1:
        mpLayerWidget->refresh();
    break;
    }
}

void ViewWidget::shuffleMainWindow()
{
    switch(mpStackedWidget->currentIndex())
    {
    case 0:
        this->shuffle();
    break;

    case 1:
        mpLayerWidget->shuffle();
    break;
    }
}

void ViewWidget::changeLayoutMainWindow()
{
    switch(mpStackedWidget->currentIndex())
    {
    case 0:
        this->changeLayout();
    break;

    case 1:
        mpLayerWidget->changeLayout();
    break;
    }
}

void ViewWidget::toggleDragDrop()
{
    mDragDrop ? unsetDragDrop() : setDragDrop();
}

void ViewWidget::reloadPropertyDialogMainWindow()
{
    if(!mpPropertyDialog->isRunning())
    {
        reloadPropertyDialog();
    }
    else
    {
        QMessageBox::information(this, tr("Cannot Reload Property Dialog"), tr("Cannot Reload Property Dialog: the dialog is still active! It needs closing first."));
    }
}

} // end namespace gui
} // end namespace graph_analysis
