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

#include <set>
#include <math.h>
#include <sstream>
#include <QDir>
#include <QTime>
#include <QMenu>
#include <QAction>
#include <QKeyEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QInputDialog>
#include <QSignalMapper>
#include <boost/regex.hpp>
#include <base/Logging.hpp>

#include <graph_analysis/Filter.hpp>
#include <graph_analysis/io/GVGraph.hpp>
#include <graph_analysis/io/YamlWriter.hpp>
#include <graph_analysis/io/GexfWriter.hpp>
#include <graph_analysis/io/GexfReader.hpp>
#include <graph_analysis/io/YamlReader.hpp>
#include <graph_analysis/filters/EdgeContextFilter.hpp>
#include <graph_analysis/gui/graphitem/edges/EdgeLabel.hpp>

#include <exception>
#include <boost/foreach.hpp>
#include <base/Time.hpp>
#define DEFAULT_SCALING_FACTOR 1.4

using namespace graph_analysis;

namespace graph_analysis {
namespace gui {

GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent)
    , mpGraph()
    , mpGVGraph(0)
    , mpYamlWriter(new io::YamlWriter())
    , mpGexfWriter(new io::GexfWriter())
    , mpGexfReader(new io::GexfReader())
    , mpYamlReader(new io::YamlReader())
    , mFiltered(false)
    , mTimerId(0)
    , mScaleFactor(DEFAULT_SCALING_FACTOR)
    , mLayout("dot")
    , mpVertexFilter(new Filter< graph_analysis::Vertex::Ptr>())
    , mpEdgeFilter(new filters::EdgeContextFilter())
    , mVertexSelected(false)
    , mEdgeSelected(false)
    , mEdgeStartVertex(false)
    , mEdgeEndVertex(false)
    , mDragDrop(false)
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
    // End of setting up filters


    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
        this, SLOT(showContextMenu(const QPoint &)));

    reset();
    mpPropertyDialog = new PropertyDialog(this);
}

GraphWidget::~GraphWidget()
{
    if(mpPropertyDialog)
    {
        mpPropertyDialog->closeDialog();
    }
    if(mpYamlWriter)
    {
        delete mpYamlWriter;
    }
    if(mpGexfWriter)
    {
        delete mpGexfWriter;
    }
    if(mpGexfReader)
    {
        delete mpGexfReader;
    }
//    destroy();
}

void GraphWidget::showContextMenu(const QPoint& pos)
{
    ActionCommander comm(this);
    QPoint position = mapTo(this, pos);
    QMenu contextMenu(tr("Context menu"), this);

    QAction *actionChangeEdgeLabel = comm.addAction("Change Selected Edge Label", SLOT(changeSelectedEdgeLabel()));
    QAction *actionRemoveEdge = comm.addAction("Remove Selected Edge", SLOT(removeSelectedEdge()));
    QAction *actionChangeLabel = comm.addAction("Change Selected Node Label", SLOT(changeSelectedVertexLabel()));
    QAction *actionRemoveNode = comm.addAction("Remove Selected Node", SLOT(removeSelectedVertex()));
    QAction *actionStartNewEdgeHere = comm.addAction("Start New Edge Here", SLOT(startNewEdgeHere()));
    QAction *actionEndNewEdgeHere = comm.addAction("End New Edge Here", SLOT(endNewEdgeHere()));
    QAction *actionAddNode = comm.addMappedAction("Add Node", SLOT(addNodeAdhoc(QObject*)), (QObject*)&position);
    QAction *actionRefresh = comm.addAction("Refresh", SLOT(refresh()));
    QAction *actionShuffle = comm.addAction("Shuffle", SLOT(shuffle()));
    QAction *actionImportFromXml = comm.addAction("Import .gexf", SLOT(importGraphFromXml()));
    QAction *actionImportFromYml = comm.addAction("Import .yml", SLOT(importGraphFromYml()));
    QAction *actionExportToXml = comm.addAction("Export as .gexf", SLOT(exportGraphToXml()));
    QAction *actionExportToYml = comm.addAction("Export as .yml", SLOT(exportGraphToYml()));
    QAction *actionExportToDot = comm.addAction("Export as .dot", SLOT(exportGraphToDot()));
    QAction *actionLayout = comm.addAction("Change Layout", SLOT(changeLayout()));
    QAction *actionSetDragDrop = comm.addAction("Drag-n-Drop Mode", SLOT(setDragDrop()));
    QAction *actionUnsetDragDrop = comm.addAction("Move-around Mode", SLOT(unsetDragDrop()));
    QAction *actionSave = comm.addAction("Save", SLOT(save()));
    QAction *actionOpen = comm.addAction("Open", SLOT(open()));
    QAction *actionReloadPropertyDialog = comm.addAction("Reload Property Dialog", SLOT(reloadPropertyDialog()));

    // (conditionally) adding the actions to the context menu
    if(mEdgeSelected)
    {
        contextMenu.addAction(actionChangeEdgeLabel);
        contextMenu.addAction(actionRemoveEdge);
    }
    if(mVertexSelected)
    {
        contextMenu.addAction(actionChangeLabel);
        contextMenu.addAction(actionRemoveNode);
        contextMenu.addAction(actionStartNewEdgeHere);
        contextMenu.addAction(actionEndNewEdgeHere);
    }
    contextMenu.addAction(actionAddNode);
    contextMenu.addAction(actionRefresh);
    contextMenu.addAction(actionShuffle);
    contextMenu.addAction(actionImportFromXml);
    contextMenu.addAction(actionImportFromYml);
    contextMenu.addAction(actionExportToXml);
    contextMenu.addAction(actionExportToYml);
    contextMenu.addAction(actionExportToDot);
    contextMenu.addAction(actionLayout);
    if(mDragDrop)
    {
        contextMenu.addAction(actionUnsetDragDrop);
    }
    else
    {
        contextMenu.addAction(actionSetDragDrop);
    }
    contextMenu.addAction(actionSave);
    contextMenu.addAction(actionOpen);
    if(!mpPropertyDialog->isRunning())
    {
        contextMenu.addAction(actionReloadPropertyDialog);
    }
    contextMenu.exec(mapToGlobal(pos));
}

void GraphWidget::reloadPropertyDialog()
{
    if(mpPropertyDialog)
    {
        delete mpPropertyDialog;
    }
    mpPropertyDialog = new PropertyDialog(this);
}

void GraphWidget::save()
{
    QStringList items;
    items << tr("GEXF (XML)") << tr("YML (YAML)");

    bool ok;
    QString format = QInputDialog::getItem(this, tr("Choose File Format"),
                                         tr("Format:"), items, 0, false, &ok);
    if (ok && !format.isEmpty())
    {
        switch(format[0].toAscii())
        {
            case 'G':
                exportGraphToXml();
            break;

            case 'Y':
                exportGraphToYml();
            break;
        }
    }
}

void GraphWidget::open()
{
    QStringList items;
    items << tr("GEXF (XML)") << tr("YML (YAML)");

    bool ok;
    QString format = QInputDialog::getItem(this, tr("Choose File Format"),
                                         tr("Format:"), items, 0, false, &ok);
    if (ok && !format.isEmpty())
    {
        switch(format[0].toAscii())
        {
            case 'G':
                importGraphFromXml();
            break;

            case 'Y':
                importGraphFromYml();
            break;
        }
    }
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
//        update();
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
        mpSelectedVertex->setLabel(label.toStdString());
        NodeItem* nodeItem = mNodeItemMap[mpSelectedVertex];
        nodeItem->updateLabel();
    }
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
    mpGraph->removeEdge(mpSelectedEdge);
}

void GraphWidget::removeSelectedVertex()
{
    EdgeIterator::Ptr edgeIt = mpGraph->getEdgeIterator(mpSelectedVertex);
    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();
        scene()->removeItem(mEdgeItemMap[edge]);
        mpGraph->removeEdge(edge);
    }
    scene()->removeItem(mNodeItemMap[mpSelectedVertex]);
    mpGraph->removeVertex(mpSelectedVertex);
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

void GraphWidget::startNewEdgeHere()
{
    mpStartVertex = mpSelectedVertex;
    mEdgeStartVertex = true;
    if(mEdgeStartVertex && mEdgeEndVertex)
    {
        bool ok;
        QString label = QInputDialog::getText(this, tr("Input New Edge Label"),
                                             tr("New Edge Label:"), QLineEdit::Normal,
                                             QString("newEdge"), &ok);
        if (ok && !label.isEmpty())
        {
            spawnEdge(label.toStdString());
        }
        mEdgeStartVertex    = false;
        mEdgeEndVertex      = false;
    }
}

void GraphWidget::spawnEdge(const std::string& label)
{
    /*
    Edge::Ptr edge(new Edge());
    edge->setSourceVertex(mpStartVertex);
    edge->setTargetVertex(mpEndVertex);
    mpGraph->addEdge(edge);
    enableEdge(edge);
    // Registering new node edge items
    Vertex::Ptr source = edge->getSourceVertex();
    Vertex::Ptr target = edge->getTargetVertex();

    NodeItem* sourceNodeItem = mNodeItemMap[ source ];
    NodeItem* targetNodeItem = mNodeItemMap[ target ];

    if(sourceNodeItem && targetNodeItem)
    {
        EdgeItem* edgeItem = EdgeTypeManager::getInstance()->createItem(this, sourceNodeItem, targetNodeItem, edge);
        mEdgeItemMap[edge] = edgeItem;
        graphitem::edges::EdgeLabel* edgeLabel = edgeItem->getLabel();
        edgeLabel->setPlainText(QString(label.c_str()));

        scene()->addItem(edgeItem);
        edgeItem->adjust();
    }
     */
}

void GraphWidget::endNewEdgeHere()
{
    mpEndVertex = mpSelectedVertex;
    mEdgeEndVertex = true;
    if(mEdgeStartVertex && mEdgeEndVertex)
    {
        bool ok;
        QString label = QInputDialog::getText(this, tr("Input New Edge Label"),
                                             tr("New Edge Label:"), QLineEdit::Normal,
                                             QString("newEdge"), &ok);
        if (ok && !label.isEmpty())
        {
            spawnEdge(label.toStdString());
        }
        mEdgeStartVertex    = false;
        mEdgeEndVertex      = false;
    }
}

void GraphWidget::exportGraphToYml()
{
    QString label =  QFileDialog::getSaveFileName(this, tr("Choose Ouput File"), QDir::currentPath(), tr("YAML (*.yaml *.yml)"));
    if (!label.isEmpty())
    {
        toYmlFile(label.toStdString());
    }
}

void GraphWidget::exportGraphToDot()
{
    QString label =  QFileDialog::getSaveFileName(this, tr("Choose Ouput File"), QDir::currentPath());
    if (!label.isEmpty())
    {
        toDotFile(label.toStdString());
    }
}

void GraphWidget::importGraphFromXml()
{
    QString label =  QFileDialog::getOpenFileName(this, tr("Choose Input File"), QDir::currentPath(), tr("GEXF (*.gexf *.xml)"));
    if (!label.isEmpty())
    {
        fromXmlFile(label.toStdString());
    }
}

void GraphWidget::importGraphFromYml()
{
    QString label =  QFileDialog::getOpenFileName(this, tr("Choose Input File"), QDir::currentPath(), tr("YAML (*.yaml *.yml)"));
    if (!label.isEmpty())
    {
        fromYmlFile(label.toStdString());
    }
}

void GraphWidget::exportGraphToXml()
{
    QString label =  QFileDialog::getSaveFileName(this, tr("Choose Ouput File"), QDir::currentPath(), tr("GEXF (*.gexf *.xml)"));
    if (!label.isEmpty())
    {
        toXmlFile(label.toStdString());
    }
}

void GraphWidget::toYmlFile(const std::string& filename)
{
    try
    {
        mpYamlWriter->write(filename, mpGraph);
    }
    catch(std::runtime_error e)
    {
        LOG_ERROR_S << "graph_analysis::gui::GraphWidget: export failed: " << e.what();
        QMessageBox::critical(this, tr("Graph Export Failed"), QString(e.what()));
    }
}

void GraphWidget::toDotFile(const std::string& filename)
{
    try
    {
        mpGVGraph->renderToFile(filename, mLayout.toStdString());
    }
    catch(std::runtime_error e)
    {
        LOG_ERROR_S << "graph_analysis::gui::GraphWidget: export via graphviz failed: " << e.what();
        QMessageBox::critical(this, tr("Graph Export via GraphViz Failed"), QString(e.what()));
    }
}

void GraphWidget::fromXmlFile(const std::string& filename)
{
    mpGexfReader->read(filename, mpGraph);
    mpSubGraph->enableAllVertices();
    mpSubGraph->enableAllEdges();
    refresh();
}

void GraphWidget::fromYmlFile(const std::string& filename)
{
    mpYamlReader->read(filename, mpGraph);
    mpSubGraph->enableAllVertices();
    mpSubGraph->enableAllEdges();
    refresh();
}

void GraphWidget::toXmlFile(const std::string& filename)
{
    try
    {
        mpGexfWriter->write(filename, mpGraph);
    }
    catch(std::runtime_error e)
    {
        LOG_ERROR_S << "graph_analysis::gui::GraphWidget: export to .gexf failed: " << e.what();
        QMessageBox::critical(this, tr("Graph Export to .gexf Failed"), QString(e.what()));
    }
}

void GraphWidget::reset(bool keepData)
{
    clear();

    if(!keepData)
    {
        mpGraph = BaseGraph::Ptr( new gl::DirectedGraph() );
    }

    if(mpGVGraph)
    {
        delete mpGVGraph;
    }
    mpGVGraph = new io::GVGraph(mpGraph, "GVGraphWidget");
}

void GraphWidget::clear()
{
    // TO-DO: add filtering clearing
    if(mpGVGraph)
    {
        mpGVGraph->clearEdges();
        mpGVGraph->clearNodes();
    }

    mNodeItemMap.clear();
    mEdgeItemMap.clear();
    mPortMap.clear();
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
    LOG_DEBUG_S << "Enabled a vertex of ID: " << mpSubGraph->getBaseGraph()->getVertexId(vertex);
}
void GraphWidget::enableEdge(graph_analysis::Edge::Ptr edge)
{
    mpSubGraph->enable(edge);
    LOG_DEBUG_S << "Enabled an edge of ID:  " << mpSubGraph->getBaseGraph()->getEdgeId(edge);
}

void GraphWidget::updateFromGraph()
{
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
//            EdgeItem* edgeItem = EdgeTypeManager::getInstance()->createItem(this, sourceNodeItem, targetNodeItem, edge);
//            mEdgeItemMap[edge] = edgeItem;
//
//            scene()->addItem(edgeItem);
//            mpGVGraph->addEdge(edge);
        }
        else if (
//                    ("graph_analysis::ClusterVertex" == source->getClassName() && "graph_analysis::PortVertex" == target->getClassName()) ||
                    ("graph_analysis::PortVertex" == source->getClassName() && "graph_analysis::ClusterVertex" == target->getClassName())
                )
        {
            // semantical edge: links a cluster vertex to one of its ports
            std::string warn_msg = std::string("graph_analysis::GraphWidget::updateFromGraph: found reversed edge from source Port vertex '") +
                                        source->toString() + "' of type '" + source->getClassName() + "' to target Cluster vertex '" +
                                        target->toString() + "' of type '" + target->getClassName() + "'!";
            LOG_WARN_S << warn_msg; // warn. due to cluster being set as target of the semantically valid edge
//            NodeItem* sourceNodeItem = mNodeItemMap[ source ];
            NodeItem* targetNodeItem = mNodeItemMap[ target ];
            if(!targetNodeItem)
            {
                continue;
            }
            mPortMap[source] = targetNodeItem;
            mPortIDMap[source] = targetNodeItem->addPort(source);
//            sourceNodeItem->setParentItem(targetNodeItem);
        }
        else if (
                    ("graph_analysis::ClusterVertex" == source->getClassName() && "graph_analysis::PortVertex" == target->getClassName())
//                    || ("graph_analysis::PortVertex" == source->getClassName() && "graph_analysis::ClusterVertex" == target->getClassName())
                )
        {
            // semantical edge: links a cluster vertex to one of its ports
            NodeItem* sourceNodeItem = mNodeItemMap[ source ];
            if(!sourceNodeItem)
            {
                continue;
            }
            mPortMap[target] = sourceNodeItem;
//            NodeItem* targetNodeItem = mNodeItemMap[ target ];
            mPortIDMap[target] = sourceNodeItem->addPort(target);
//            targetNodeItem->setParentItem(sourceNodeItem);
        }
        else if (
                    ("graph_analysis::ClusterVertex" == source->getClassName() && "graph_analysis::ClusterVertex" == target->getClassName())
                )
        {
            // automatically added edge - useful for the layouting stage
            continue;
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
            EdgeItem* edgeItem = EdgeTypeManager::getInstance()->createItem(this, sourceNodeItem, mPortIDMap[source], targetNodeItem, mPortIDMap[target], edge);
            mEdgeItemMap[edge] = edgeItem;

            scene()->addItem(edgeItem);
            Edge::Ptr default_edge(new Edge(sourceNodeItem->getVertex(), targetNodeItem->getVertex()));
            mpGraph->addEdge(default_edge);
            mpGVGraph->addEdge(default_edge);
        }
    }

    if(mLayout.toLower() != "force")
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);

        LOG_INFO_S << "GV started layouting the graph. This can take a while ...";
        base::Time start = base::Time::now();
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
        if("graph_analysis::ClusterVertex" == it->second->getVertex()->getClassName())
        {
            it->second->setFlag(QGraphicsItemGroup::ItemIsMovable, !mDragDrop);
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
        if("graph_analysis::ClusterVertex" == it->second->getVertex()->getClassName())
        {
            it->second->setFlag(QGraphicsItemGroup::ItemIsMovable, !mDragDrop);
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
        if("graph_analysis::ClusterVertex" == it->second->getVertex()->getClassName())
        {
            it->second->setFlag(QGraphicsItemGroup::ItemIsMovable, !mDragDrop);
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
    // check for a combination of user clicks
    Qt::KeyboardModifiers modifiers = event->modifiers();

    if(modifiers & Qt::ControlModifier)
    {
        switch (event->key())
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

            case Qt::Key_Left:
                rotate(qreal(-1.13));
            break;

            case Qt::Key_Right:
                rotate(qreal( 1.13));
            break;
        }
    }

    switch (event->key())
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
            refresh();
    break;

    case Qt::Key_S:
        shuffle();
    break; // TOO close to CTRL+S?!
    //default:
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
        return;

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
            item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
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

void GraphWidget::setLayout(QString layoutName)
{
    mLayout = layoutName;
    updateFromGraph();
}

} // end namespace gui
} // end namespace graph_analysis
