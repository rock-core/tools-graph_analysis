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

#include "GraphWidget.hpp"
#include "EdgeItem.hpp"
#include "NodeItem.hpp"
#include "NodeTypeManager.hpp"
#include "EdgeTypeManager.hpp"
#include "ActionCommander.hpp"
#include "AddNodeDialog.hpp"
#include "PropertyDialog.hpp"
#include "RenamePortDialog.hpp"

#include <set>
#include <math.h>
#include <sstream>
#include <QDir>
#include <QTime>
#include <QMenu>
#include <sstream>
#include <QAction>
#include <QKeyEvent>
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
#define DEFAULT_SCALING_FACTOR 2.269
//#define DEFAULT_PATH_TO_ICONS "../../resources/icons/"
#define DEFAULT_PATH_TO_ICONS "icons/"

// comment out to toggle-out focused node be re-doule-clicking it; leave untouched to be able to cancel node focusing by double-clicking the background
#define CLEAR_BY_BACKGROUND

using namespace graph_analysis;

namespace graph_analysis {
namespace gui {

GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent)
    , mpGraph()
    , mpLayoutingGraph()
    , mpGVGraph(0)
    , mFiltered(false)
    , mTimerId(0)
    , mScaleFactor(DEFAULT_SCALING_FACTOR)
    , mLayout("dot")
    , mpVertexFilter(new Filter< graph_analysis::Vertex::Ptr>())
    , mpEdgeFilter(new filters::EdgeContextFilter())
    , mVertexSelected(false)
    , mVertexFocused(false)
    , mEdgeSelected(false)
    , mEdgeFocused(false)
    , mDragDrop(false)
    , mMaxNodeHeight(0)
    , mMaxNodeWidth (0)
{
    // Add seed for force layout
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);

    setCacheMode(CacheBackground);
    setContextMenuPolicy(Qt::CustomContextMenu);
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
    loadIcon(mIconMap["addPort"], pathToIcons + "addPort.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["move"], pathToIcons + "move.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["reload"], pathToIcons + "reload.png");

    // setting up the context menu
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
        this, SLOT(showContextMenu(const QPoint &)));

    reset();
    mpPropertyDialog = new PropertyDialog(this, (QMainWindow *)parentWidget());
}

void GraphWidget::loadIcon(QIcon& icon, std::string file)
{
    icon.addFile(QString::fromUtf8(file.c_str()), QSize(), QIcon::Normal, QIcon::Off);
}

GraphWidget::~GraphWidget()
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

void GraphWidget::showContextMenu(const QPoint& pos)
{
    ActionCommander comm(this);
    QPoint position = mapTo(this, pos);
    QMenu contextMenu(tr("Context menu"), this);

    QAction *actionChangeEdgeLabel = comm.addAction("Change Edge Label", SLOT(changeSelectedEdgeLabel()), mIconMap["label"]);
    QAction *actionRemoveEdge = comm.addAction("Remove Edge", SLOT(removeSelectedEdge()), mIconMap["remove"]);
    QAction *actionChangeLabel = comm.addAction("Change Node Label", SLOT(changeSelectedVertexLabel()), mIconMap["label"]);
    QAction *actionRemoveNode = comm.addAction("Remove Node", SLOT(removeSelectedVertex()), mIconMap["remove"]);
    QAction *actionAddPort = comm.addAction("Add Port", SLOT(addPortSelected()), mIconMap["addPort"]);
    QAction *actionRenamePort = comm.addAction("Rename a Port", SLOT(renamePortSelected()), mIconMap["portLabel"]);
    QAction *actionRemovePort = comm.addAction("Remove a Port", SLOT(removePortSelected()), mIconMap["remove"]);
    QAction *actionAddNode = comm.addMappedAction("Add Node", SLOT(addNodeAdhoc(QObject*)), (QObject*)&position, mIconMap["addNode"]);
    QAction *actionRefresh = comm.addAction("Refresh", SLOT(refresh()), mIconMap["refresh"]);
    QAction *actionShuffle = comm.addAction("Shuffle", SLOT(shuffle()), mIconMap["shuffle"]);
    QAction *actionImport = comm.addAction("Import", SLOT(importGraph()), mIconMap["import"]);
    QAction *actionExport = comm.addAction("Export", SLOT(exportGraph()), mIconMap["export"]);
    QAction *actionReset  = comm.addAction("Reset", SLOT(resetGraph()), mIconMap["reset"]);
    QAction *actionLayout = comm.addAction("Layout", SLOT(changeLayout()), mIconMap["layout"]);
    QAction *actionSetDragDrop = comm.addAction("Drag-n-Drop Mode", SLOT(setDragDrop()), mIconMap["dragndrop"]);
    QAction *actionUnsetDragDrop = comm.addAction("Move-around Mode", SLOT(unsetDragDrop()), mIconMap["move"]);
    QAction *actionReloadPropertyDialog = comm.addAction("Reload Property Dialog", SLOT(reloadPropertyDialog()), mIconMap["reload"]);

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
        contextMenu.addAction(actionRenamePort);
        contextMenu.addAction(actionRemovePort);
        contextMenu.addAction(actionRemoveNode);
    }
    if(mVertexSelected || mEdgeSelected)
    {
        contextMenu.addSeparator();
    }
    contextMenu.addAction(actionAddNode);
    contextMenu.addSeparator();
    contextMenu.addAction(actionRefresh);
    contextMenu.addAction(actionShuffle);
    contextMenu.addAction(actionLayout);
    if(!mpGraph->empty())
    {
        contextMenu.addAction(actionReset);
    }
    contextMenu.addSeparator();
    contextMenu.addAction(actionImport);
    contextMenu.addAction(actionExport);
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

void GraphWidget::addPortFocused()
{
    addPort(mpFocusedVertex);
}

void GraphWidget::addPortSelected()
{
    addPort(mpSelectedVertex);
}

void GraphWidget::addPort(graph_analysis::Vertex::Ptr vertex)
{
    NodeItem *item = mNodeItemMap[vertex];
    graph_analysis::Vertex::Ptr portVertex = VertexTypeManager::getInstance()->createVertex("port", "newPort");
    mpGraph->addVertex(portVertex);
    enableVertex(portVertex);
    createEdge(vertex, portVertex, "portRegistrationEdge");
    mPortMap[portVertex] = item;
    mPortIDMap[portVertex] = item->addPort(portVertex);
    item->update();
}

void GraphWidget::renamePortFocused()
{
    renamePort(mpFocusedVertex);
}

void GraphWidget::renamePortSelected()
{
    renamePort(mpSelectedVertex);
}

void GraphWidget::renamePort(graph_analysis::Vertex::Ptr concernedVertex)
{
    NodeItem *item = mNodeItemMap[concernedVertex];
    if(!item)
    {
        std::string error_msg = std::string("graph_analysis::GraphWidget::renamePort: provided vertex '") + concernedVertex->getLabel() + "' is not registered with the GUI";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    int portCount = item->getPortCount();
    if(!portCount)
    {
        QMessageBox::critical(this, tr("Cannot Rename a Port"), tr("The selected vertex had no ports!"));
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
    }
}

void GraphWidget::removePortFocused()
{
    removePort(mpFocusedVertex);
}

void GraphWidget::removePortSelected()
{
    removePort(mpSelectedVertex);
}

void GraphWidget::removePort(graph_analysis::Vertex::Ptr concernedVertex)
{
    NodeItem *item = mNodeItemMap[concernedVertex];
    if(!item)
    {
        std::string error_msg = std::string("graph_analysis::GraphWidget::removePort: provided vertex '") + concernedVertex->getLabel() + "' is not registered with the GUI";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    int portCount = item->getPortCount();
    if(!portCount)
    {
        QMessageBox::critical(this, tr("Cannot Remove a Port"), tr("The selected vertex had no ports!"));
        return;
    }
    bool ok;
    QStringList ports_options;
    int portID = 0;
    foreach(graph_analysis::Vertex::Ptr vertex, item->getVertices())
    {
        std::string option = boost::lexical_cast<std::string>(portID++) + ": " + vertex->getLabel();
        ports_options << tr(option.c_str());
    }
    QString strPortID = QInputDialog::getItem(this, tr("Choose Port to Remove"),
                                         tr("Port ID:"), ports_options,
                                         0, false, &ok);
    if (ok && !strPortID.isEmpty())
    {
        std::stringstream ss(strPortID.toStdString());
        ss >> portID;
        // remove conceptual edges
        EdgeIterator::Ptr edgeIt = mpGraph->getEdgeIterator(item->getPort(portID));
        while(edgeIt->next())
        {
            Edge::Ptr edge = edgeIt->current();
            mpGraph->removeEdge(edge);
//            if(mEdgeItemMap.count(edge))
//            {
//                EdgeItem* edgeItem = mEdgeItemMap[edge];
//                scene()->removeItem(edgeItem);
//                if(edgeItem)
//                {
//                    delete edgeItem;
//                }
//            }
        }
        // remove physical edges and their graphics
        graph_analysis::Vertex::Ptr cluster = item->getVertex();
        edgeIt = mpLayoutingGraph->getEdgeIterator(cluster);
        while(edgeIt->next())
        {
            Edge::Ptr edge = edgeIt->current();
            if(mEdgeItemMap.count(edge))
            {
                EdgeItem* edgeItem = mEdgeItemMap[edge];
                if  (
                        (item == edgeItem->sourceNodeItem() && portID == edgeItem->getSourcePortID()) ||
                        (item == edgeItem->targetNodeItem() && portID == edgeItem->getTargetPortID())
                    )
                    {
                        mpLayoutingGraph->removeEdge(edge);
                        scene()->removeItem(edgeItem);
                        break;
                    }
            }
        }
        // remove port graphics
        item->removePort(portID);
    }
}

void GraphWidget::importGraph()
{
    QString label =  QFileDialog::getOpenFileName(this, tr("Choose Input File"), QDir::currentPath(), tr("GEXF (*.gexf *.xml);;YAML/YML (*.yaml *.yml)"));

    if (!label.isEmpty())
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
        }
    }
}

void GraphWidget::exportGraph()
{
    QString label =  QFileDialog::getSaveFileName(this, tr("Choose Input File"), QDir::currentPath(), tr("GEXF (*.gexf *.xml);;YAML/YML (*.yaml *.yml);;DOT (*.dot)"));

    if (!label.isEmpty())
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
        }
    }
}

void GraphWidget::reloadPropertyDialog()
{
    if(mpPropertyDialog)
    {
        delete mpPropertyDialog;
    }
    mpPropertyDialog = new PropertyDialog(this, (QMainWindow *)parentWidget(), mDragDrop);
}

Edge::Ptr GraphWidget::createEdge(Vertex::Ptr sourceNode, Vertex::Ptr targetNode, const std::string& label)
{
    Edge::Ptr edge(new Edge(sourceNode, targetNode, label));
    mpGraph->addEdge(edge);
    enableEdge(edge);
    return edge;
}

Vertex::Ptr GraphWidget::createVertex(const std::string& type, const std::string& label)
{
    std::set<std::string> types = VertexTypeManager::getInstance()->getSupportedTypes();
    std::set<std::string>::iterator type_it = types.find(type);
    if(types.end() == type_it)
    {
        std::string error_msg = std::string("graph_analysis::GraphWidget::createVertex: Given Vertex Type '") + type + "' is not registered!";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    graph_analysis::Vertex::Ptr vertex = VertexTypeManager::getInstance()->createVertex(type, label);
    mpGraph->addVertex(vertex);
    enableVertex(vertex);
    return vertex;
}

Edge::Ptr GraphWidget::createStandaloneEdge(Vertex::Ptr sourceNode, Vertex::Ptr targetNode, const std::string& label)
{
    Edge::Ptr edge(new Edge(sourceNode, targetNode, label));
    return edge;
}

Vertex::Ptr GraphWidget::createStandaloneVertex(const std::string& type, const std::string& label)
{
    std::set<std::string> types = VertexTypeManager::getInstance()->getSupportedTypes();
    std::set<std::string>::iterator type_it = types.find(type);
    if(types.end() == type_it)
    {
        std::string error_msg = std::string("graph_analysis::GraphWidget::createVertex: Given Vertex Type '") + type + "' is not registered!";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    graph_analysis::Vertex::Ptr vertex = VertexTypeManager::getInstance()->createVertex(type, label);
    return vertex;
}

void GraphWidget::addNodeAdhoc(QObject *pos)
{
    QPoint *position = (QPoint *)pos;
    AddNodeDialog nodeDialog;
    if(nodeDialog.isValid())
    {
        graph_analysis::Vertex::Ptr vertex = VertexTypeManager::getInstance()->createVertex(nodeDialog.getNodeType(), nodeDialog.getNodeLabel());
        mpGraph->addVertex(vertex);
        enableVertex(vertex);
        // Registering the new node item
        NodeItem* nodeItem = NodeTypeManager::getInstance()->createItem(this, vertex);
        nodeItem->setPos((double) position->x(), (double) position->y());
        mNodeItemMap[vertex] = nodeItem;
        scene()->addItem(nodeItem);
    }
}

void GraphWidget::changeFocusedVertexLabel()
{
    bool ok;
    QString label = QInputDialog::getText(this, tr("Input Node Label"),
                                         tr("New Label:"), QLineEdit::Normal,
                                         QString(mpFocusedVertex->getLabel().c_str()), &ok);
    if (ok && !label.isEmpty())
    {
        changeVertexLabel(mpFocusedVertex, label.toStdString());
    }
}

void GraphWidget::changeSelectedVertexLabel()
{
    bool ok;
    QString label = QInputDialog::getText(this, tr("Input Node Label"),
                                         tr("New Label:"), QLineEdit::Normal,
                                         QString(mpSelectedVertex->getLabel().c_str()), &ok);
    if (ok && !label.isEmpty())
    {
        changeVertexLabel(mpSelectedVertex, label.toStdString());
    }
}

void GraphWidget::changeVertexLabel(graph_analysis::Vertex::Ptr vertex, const std::string& label)
{
    vertex->setLabel(label);
    NodeItem* nodeItem = mNodeItemMap[vertex];
    nodeItem->updateLabel();
}

void GraphWidget::changeSelectedEdgeLabel()
{
    bool ok;
    EdgeItem* edge = mEdgeItemMap[mpSelectedEdge];
    QString label = QInputDialog::getText(this, tr("Input Edge Label"),
                                         tr("New Label:"), QLineEdit::Normal,
                                          edge->getLabel()->toPlainText(), &ok);
    if (ok && !label.isEmpty())
    {
        graphitem::edges::EdgeLabel* edgeLabel = edge->getLabel();
        edgeLabel->setPlainText(QString(label.toStdString().c_str()));
        graph_analysis::Edge::Ptr graph_edge = edge->getEdge();
        graph_edge->setLabel(label.toStdString());
    }
}

void GraphWidget::removeSelectedEdge()
{
    scene()->removeItem(mEdgeItemMap[mpSelectedEdge]);
    const std::string label = mpSelectedEdge->getLabel();
    const graph_analysis::Vertex::Ptr sourceClusterVertex = mpSelectedEdge->getSourceVertex();
    const graph_analysis::Vertex::Ptr targetClusterVertex = mpSelectedEdge->getTargetVertex();
    const NodeItem * sceneSourceNodeItem = mNodeItemMap[sourceClusterVertex];
    const NodeItem * sceneTargetNodeItem = mNodeItemMap[targetClusterVertex];
    if(!sceneSourceNodeItem || !sceneTargetNodeItem)
    {
        std::string error_msg = std::string("graph_analysis::gui::GraphWidget::removeSelectedEdge: the selected edge '")
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
            std::string error_msg = std::string("graph_analysis::gui::GraphWidget::removeSelectedEdge: the iterated-over edge '")
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
    mpLayoutingGraph->removeEdge(mpSelectedEdge);
}

void GraphWidget::removeFocusedVertex()
{
    clearVertex(mpFocusedVertex);
}

void GraphWidget::removeSelectedVertex()
{
    clearVertex(mpSelectedVertex);
}

void GraphWidget::clearVertex(graph_analysis::Vertex::Ptr concernedVertex)
{
    // removing possible (default?) edges of this cluster node within the main graph (and its port-vertices)
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
//    // removing default edges (in the secondary/layouting graph and elliminating their corresponding graphical representations off the screen)
//    edgeIt = mpLayoutingGraph->getEdgeIterator(concernedVertex);
//    while(edgeIt->next())
//    {
//        Edge::Ptr edge = edgeIt->current();
//        scene()->removeItem(mEdgeItemMap[edge]);
//        mpLayoutingGraph->removeEdge(edge);
//    }  // commented out since it introduces bugs when mpLayoutingGraph is dirty
//    mpLayoutingGraph->removeVertex(concernedVertex); // commented out since it introduces bugs when mpLayoutingGraph is 'dirty'
    NodeItem *item = mNodeItemMap[concernedVertex];
    if(!item)
    {
        std::string error_msg = std::string("graph_analysis::GraphWidget::removeVertex: provided vertex '") + concernedVertex->getLabel() + "' is not registered with the GUI";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    scene()->removeItem(item);
    mpGraph->removeVertex(concernedVertex);
}

void GraphWidget::changeLayout()
{
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
    }
}

void GraphWidget::setStartVertex(graph_analysis::Vertex::Ptr startVertex, int portID)
{
    if("graph_analysis::ClusterVertex" != startVertex->getClassName())
    {
        std::string error_msg = std::string("graph_analysis::gui::GraphWidget::setStartVertex: expected startVertex to be of type 'graph_analysis::ClusterVertex'; instead, found type '")
                                        + startVertex->getClassName() + "'";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    mpStartVertex   = startVertex;
    NodeItem *item  = mNodeItemMap[startVertex];
    mpStartPort     = item->getPort(portID);
    if("graph_analysis::PortVertex" != mpStartPort->getClassName())
    {
        std::string error_msg = std::string("graph_analysis::gui::GraphWidget::setStartVertex: expected associated portVertex to be of type 'graph_analysis::PortVertex'; instead, found type '")
                                        + mpStartPort->getClassName() + "'";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
}

void GraphWidget::setEndVertex(graph_analysis::Vertex::Ptr endVertex, int portID)
{
    if("graph_analysis::ClusterVertex" != endVertex->getClassName())
    {
        std::string error_msg = std::string("graph_analysis::gui::GraphWidget::setEndVertex: expected endVertex to be of type 'graph_analysis::ClusterVertex'; instead, found type '")
                                        + endVertex->getClassName() + "'";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    mpEndVertex     = endVertex;
    NodeItem *item  = mNodeItemMap[endVertex];
    mpEndPort       = item->getPort(portID);
    if("graph_analysis::PortVertex" != mpEndPort->getClassName())
    {
        std::string error_msg = std::string("graph_analysis::gui::GraphWidget::setEndVertex: expected associated portVertex to be of type 'graph_analysis::PortVertex'; instead, found type '")
                                        + mpEndPort->getClassName() + "'";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    addEdgeAdHoc(); // unconditionally trigger edge insertion
}

void GraphWidget::addEdgeAdHoc() // assumes the concerned edge-creation member fields are properly set already
{
    bool ok;
    QString label = QInputDialog::getText(this, tr("Input New Edge Label"),
                                         tr("New Edge Label:"), QLineEdit::Normal,
                                         QString("newEdge"), &ok);
    if (ok && !label.isEmpty())
    {
        spawnEdge(label.toStdString()); // assumes the concerned edge-creation member fields are properly set already
    }
}

void GraphWidget::spawnEdge(const std::string& label) // assumes the concerned edge-creation member fields are properly set already
{
    NodeItem *sourceNodeItem = mNodeItemMap[mpStartVertex];
    NodeItem *targetNodeItem = mNodeItemMap[mpEndVertex];
    if(sourceNodeItem && targetNodeItem)
    {
        Edge::Ptr edge(new Edge(mpStartPort, mpEndPort, label));
        mpGraph->addEdge(edge);
        enableEdge(edge);
        Edge::Ptr default_edge(new Edge(mpStartVertex, mpEndVertex, label));
        mpLayoutingGraph->addEdge(default_edge);

        EdgeItem* edgeItem = EdgeTypeManager::getInstance()->createItem(this, sourceNodeItem, mPortIDMap[mpStartPort], targetNodeItem, mPortIDMap[mpEndPort], default_edge);
        scene()->addItem(edgeItem);
        edgeItem->adjust();
        mEdgeItemMap[default_edge] = edgeItem;
        mpGVGraph->addEdge(default_edge); // not vital since the mpGVGraph gets updated every single time before (rendering with) layouting engines
    }
    else
    {
        std::string error_msg = std::string("graph_analysis::gui::GraphWidget::spawnEdge: could not insert new edge of label '") + label
                                        + "' since its endpoint vertices are not both registered in mNodeItemMap";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
}

void GraphWidget::toYmlFile(const std::string& filename)
{
    try
    {
        mWriterMap["yaml"]->write(filename, mpGraph);
    }
    catch(std::runtime_error e)
    {
        LOG_ERROR_S << "graph_analysis::gui::GraphWidget::toYmlFile: export failed: " << e.what();
        QMessageBox::critical(this, tr("Graph Export Failed"), QString(e.what()));
    }
}

void GraphWidget::toDotFile(const std::string& filename)
{
    try
    {
        mWriterMap["dot"]->write(filename, mpGraph);
    }
    catch(std::runtime_error e)
    {
        LOG_ERROR_S << "graph_analysis::gui::GraphWidget::toDotFile: export via graphviz failed: " << e.what();
        QMessageBox::critical(this, tr("Graph Export via GraphViz Failed"), QString(e.what()));
    }
}

void GraphWidget::gvRender(const std::string& filename)
{
    try
    {
        mpGVGraph->renderToFile(filename, mLayout.toStdString());
    }
    catch(std::runtime_error e)
    {
        LOG_ERROR_S << "graph_analysis::gui::GraphWidget::toDotFile: export via graphviz failed: " << e.what();
        QMessageBox::critical(this, tr("Graph Export via GraphViz Failed"), QString(e.what()));
    }
}

void GraphWidget::toXmlFile(const std::string& filename)
{
    try
    {
        mWriterMap["gexf"]->write(filename, mpGraph);
    }
    catch(std::runtime_error e)
    {
        LOG_ERROR_S << "graph_analysis::gui::GraphWidget::toXmlFile: export to .gexf failed: " << e.what();
        QMessageBox::critical(this, tr("Graph Export to .gexf Failed"), QString(e.what()));
    }
}

void GraphWidget::fromXmlFile(const std::string& filename)
{
    graph_analysis::BaseGraph::Ptr graph = BaseGraph::Ptr( new gl::DirectedGraph() );
    try
    {
        mReaderMap["gexf"]->read(filename, graph);
    }
    catch(std::runtime_error e)
    {
        LOG_ERROR_S << "graph_analysis::gui::GraphWidget::fromXmlFile: import from .gexf failed: " << e.what();
        QMessageBox::critical(this, tr("Graph Import from .gexf Failed"), QString(e.what()));
        return;
    }

    mpGraph = graph;
    mpSubGraph = mGraphView.apply(mpGraph);
    mFiltered = true;

    mpSubGraph->enableAllVertices();
    mpSubGraph->enableAllEdges();
    refresh();
}

void GraphWidget::fromYmlFile(const std::string& filename)
{
    graph_analysis::BaseGraph::Ptr graph = BaseGraph::Ptr( new gl::DirectedGraph() );
    try
    {
        mReaderMap["yaml"]->read(filename, graph);
    }
    catch(std::runtime_error e)
    {
        LOG_ERROR_S << "graph_analysis::gui::GraphWidget::fromYmlFile: import from .yaml failed: " << e.what();
        QMessageBox::critical(this, tr("Graph Import from .yaml Failed"), QString(e.what()));
        return;
    }

    mpGraph = graph;
    mpSubGraph = mGraphView.apply(mpGraph);
    mFiltered = true;

    mpSubGraph->enableAllVertices();
    mpSubGraph->enableAllEdges();
    refresh();
}

void GraphWidget::reset(bool keepData)
{
    clear();

    if(!keepData)
    {
        mpGraph = BaseGraph::Ptr( new gl::DirectedGraph() );
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
    mpGVGraph = new io::GVGraph(mpLayoutingGraph, "GVGraphWidget");
}

void GraphWidget::clear()
{
    // TO-DO: add filtering clearing?
    if(mpGVGraph)
    {
        mpGVGraph->clearEdges();
        mpGVGraph->clearNodes();
    }

    mNodeItemMap.clear();
    mEdgeItemMap.clear();
    mPortMap.clear();
    mPortIDMap.clear();
    scene()->clear();
}

void GraphWidget::refresh()
{
    reset(true /*keepData*/);
    updateFromGraph();
    update();
}

void GraphWidget::enableVertex(graph_analysis::Vertex::Ptr vertex)
{
    mpSubGraph->enable(vertex);
    LOG_DEBUG_S << "Enabled vertex '" << vertex->getLabel() << "' of ID: " << mpSubGraph->getBaseGraph()->getVertexId(vertex);
}
void GraphWidget::enableEdge(graph_analysis::Edge::Ptr edge)
{
    mpSubGraph->enable(edge);
    LOG_DEBUG_S << "Enabled edge '" << edge->getLabel() << "' of ID:  " << mpSubGraph->getBaseGraph()->getEdgeId(edge);
}

void GraphWidget::disableVertex(graph_analysis::Vertex::Ptr vertex)
{
    mpSubGraph->disable(vertex);
    LOG_DEBUG_S << "Disabled vertex '" << vertex->getLabel() << "' of ID: " << mpSubGraph->getBaseGraph()->getVertexId(vertex);
}
void GraphWidget::disableEdge(graph_analysis::Edge::Ptr edge)
{
    mpSubGraph->disable(edge);
    LOG_DEBUG_S << "Disabled edge '" << edge->getLabel() << "' of ID:  " << mpSubGraph->getBaseGraph()->getEdgeId(edge);
}

void GraphWidget::updateFromGraph()
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

        if( mNodeItemMap.count(vertex))
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

        if( mEdgeItemMap.count(edge))
        {
            continue;
        }

        Vertex::Ptr source = edge->getSourceVertex();
        Vertex::Ptr target = edge->getTargetVertex();

        if("graph_analysis::PortVertex" == source->getClassName() && "graph_analysis::PortVertex" == target->getClassName())
        {
            // physical edge - processing deflected until after all ports will have been registered
            continue;
        }
        else if (
                    ("graph_analysis::PortVertex" == source->getClassName() && "graph_analysis::ClusterVertex" == target->getClassName())
                )
        {
            // semantical edge: links a cluster vertex to one of its ports
            std::string warn_msg = std::string("graph_analysis::GraphWidget::updateFromGraph: found reversed edge from source Port vertex '") +
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
                    ("graph_analysis::ClusterVertex" == source->getClassName() && "graph_analysis::PortVertex" == target->getClassName())
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
            std::string error_msg = std::string("graph_analysis::GraphWidget::updateFromGraph: found invalid edge from source vertex '") +
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

        if( mEdgeItemMap.count(edge))
        {
            continue;
        }

        Vertex::Ptr source = edge->getSourceVertex();
        Vertex::Ptr target = edge->getTargetVertex();

        if("graph_analysis::PortVertex" == source->getClassName() && "graph_analysis::PortVertex" == target->getClassName())
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

void GraphWidget::addVertex(Vertex::Ptr vertex)
{
    mpGraph->addVertex(vertex);
}

void GraphWidget::addEdge(Edge::Ptr edge)
{
    mpGraph->addEdge(edge);
}

void GraphWidget::removeVertex(Vertex::Ptr vertex)
{
    mpGraph->removeVertex(vertex);
}

void GraphWidget::removeEdge(Edge::Ptr edge)
{
    mpGraph->removeEdge(edge);
}

void GraphWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
#ifdef CLEAR_BY_BACKGROUND
    if(
        !( // Achtung!!! Negating the following statement
            (mVertexSelected && mVertexFocused && mpSelectedVertex == mpFocusedVertex) ||
            (mEdgeSelected && mEdgeFocused && mpSelectedEdge == mpFocusedEdge)
        )
    )
    {
        clearFocus();
        QGraphicsView::mouseDoubleClickEvent(event);
    }
    else
    {
        clearFocus();
    }
#else
    QGraphicsView::mouseDoubleClickEvent(event);
#endif
}

void GraphWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MidButton)
    {
        //Use ScrollHand Drag Mode to enable Panning
        setDragMode(ScrollHandDrag);
        QMouseEvent fake(event->type(), event->pos(), Qt::LeftButton, Qt::LeftButton, event->modifiers());
        QGraphicsView::mousePressEvent(&fake);
    }
    else QGraphicsView::mousePressEvent(event);
}

void GraphWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MidButton)
    {
        //Use ScrollHand Drag Mode to enable Panning
        setDragMode(NoDrag);
        QMouseEvent fake(event->type(), event->pos(), Qt::LeftButton, Qt::LeftButton, event->modifiers());
        QGraphicsView::mouseReleaseEvent(&fake);
    }
    else QGraphicsView::mouseReleaseEvent(event);
}

void GraphWidget::updateDragDrop(bool dragDrop)
{
    mDragDrop = dragDrop;
    NodeItemMap::iterator it = mNodeItemMap.begin();
    for(; mNodeItemMap.end() != it; ++it)
    {
        NodeItem *current = it->second;
        if("graph_analysis::ClusterVertex" == current->getVertex()->getClassName())
        {
            current->setHandlesChildEvents(!mDragDrop);
        }
    }
}

void GraphWidget::syncDragDrop()
{
    NodeItemMap::iterator it = mNodeItemMap.begin();
    for(; mNodeItemMap.end() != it; ++it)
    {
        NodeItem *current = it->second;
        if("graph_analysis::ClusterVertex" == current->getVertex()->getClassName())
        {
            current->setHandlesChildEvents(!mDragDrop);
        }
    }
}

void GraphWidget::setDragDrop()
{
    mDragDrop = true;
    mpPropertyDialog->setDragDrop(true);
    NodeItemMap::iterator it = mNodeItemMap.begin();
    for(; mNodeItemMap.end() != it; ++it)
    {
        NodeItem *current = it->second;
        if("graph_analysis::ClusterVertex" == current->getVertex()->getClassName())
        {
            current->setHandlesChildEvents(false); // of !mDragDrop
        }
    }
}

void GraphWidget::unsetDragDrop()
{
    mDragDrop = false;
    mpPropertyDialog->setDragDrop(false);
    NodeItemMap::iterator it = mNodeItemMap.begin();
    for(; mNodeItemMap.end() != it; ++it)
    {
        NodeItem *current = it->second;
        if("graph_analysis::ClusterVertex" == current->getVertex()->getClassName())
        {
            current->setHandlesChildEvents(true); // of !mDragDrop
        }
    }
}

void GraphWidget::itemMoved()
{
    if (!mTimerId)
    {
        mTimerId = startTimer(1000 / 25);
    }
}

void GraphWidget::keyPressEvent(QKeyEvent *event)
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

            case Qt::Key_R:
                resetGraph();
            break;

            case Qt::Key_S:
                exportGraph();
            break;

            case Qt::Key_A:
                addNodeAdhoc();
            break;

            case Qt::Key_I:
            case Qt::Key_O:
                importGraph();
            break;

            case Qt::Key_Minus:
                zoomOut();
            break;

            case Qt::Key_Left:
                rotate(qreal(-1.13));
            break;

            case Qt::Key_Right:
                rotate(qreal( 1.13));
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
        break; // too close to CTRL+S?!

        case Qt::Key_L:
            changeLayout();
        break;

        case Qt::Key_Escape:
            clearFocus();
        break;

        //default:
        }
    }
    QGraphicsView::keyPressEvent(event);
}

void GraphWidget::timerEvent(QTimerEvent *event)
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
void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, - event->delta() / 240.0));
}
#endif

void GraphWidget::drawBackground(QPainter *painter, const QRectF& rect)
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

void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
    {
        return;
    }
    scale(scaleFactor, scaleFactor);
}

void GraphWidget::setNodeFilters(std::vector< Filter<Vertex::Ptr>::Ptr > filters)
{
    mpVertexFilter->clear();
    BOOST_FOREACH(Filter<Vertex::Ptr>::Ptr filter, filters)
    {
        mpVertexFilter->add(filter);
    }
    mGraphView.setVertexFilter(mpVertexFilter);
    if(!mFiltered)
    {
        mpSubGraph = mGraphView.apply(mpGraph);
        mFiltered = true;
    }
}

void GraphWidget::setEdgeFilters(std::vector< Filter<Edge::Ptr>::Ptr > filters)
{
    mpEdgeFilter->clear();
    BOOST_FOREACH(Filter<Edge::Ptr>::Ptr filter, filters)
    {
        mpEdgeFilter->add(filter);
    }
    mGraphView.setEdgeFilter(mpEdgeFilter);
    if(!mFiltered)
    {
        mpSubGraph = mGraphView.apply(mpGraph);
        mFiltered = true;
    }
}

void GraphWidget::shuffle()
{
    foreach (QGraphicsItem *item, scene()->items())
    {
        if (qgraphicsitem_cast<NodeItem* >(item))
        {
            item->setPos(-150 * mScaleFactor + mScaleFactor * (qrand() % 300), -150 * mScaleFactor + mScaleFactor * (qrand() % 300));
        }
    }
}

void GraphWidget::zoomIn()
{
    scaleView(qreal(1.13));
}

void GraphWidget::zoomOut()
{
    scaleView(1 / qreal(1.13));
}

void GraphWidget::resetGraph()
{
    if(mpGraph->empty())
    {
        QMessageBox::information(this, tr("Nothing to Reset"), "The graph is already empty!");
//        reset();
    }
    else
    {
        QMessageBox::StandardButton button = QMessageBox::question(this, tr("Confirm Reset"), tr("The Graph will be completely erased! Are you sure you want to continue?"), QMessageBox::Yes | QMessageBox::No);
        switch(button)
        {
            case QMessageBox::Yes:
                reset();
            break;

            default:
            break;
        }
    }
}

void GraphWidget::setLayout(QString layoutName)
{
    /* !!! reset(true); shall be called apriori (unless you know what you're doing - e.g. at init) !!! */
    mLayout = layoutName;
    updateFromGraph();
}

void GraphWidget::setVertexFocused(bool focused)
{
    mVertexFocused = focused;
    mpPropertyDialog->setVertexFocused(focused);
}

void GraphWidget::setEdgeFocused(bool focused)
{
    mEdgeFocused = focused;
    mpPropertyDialog->setEdgeFocused(focused);
}

void GraphWidget::clearNodeFocus()
{
    if(mVertexFocused)
    {
        NodeItem *item = mNodeItemMap[mpFocusedVertex];
        item->releaseFocus();
    }
}

void GraphWidget::clearEdgeFocus()
{
    if(mEdgeFocused)
    {
        EdgeItem *item = mEdgeItemMap[mpFocusedEdge];
        item->releaseFocus();
    }
}

void GraphWidget::clearFocus()
{
    clearNodeFocus();
    clearEdgeFocus();
}

} // end namespace gui
} // end namespace graph_analysis
