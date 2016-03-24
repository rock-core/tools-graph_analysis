#include "ComponentGraphEditor.hpp"

#include <exception>
#include <set>
#include <math.h>
#include <string>
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

#include <boost/foreach.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <base/Time.hpp>
#include <base/Logging.hpp>

#include <graph_analysis/io/GVGraph.hpp>

#include <graph_analysis/VertexTypeManager.hpp>
#include <graph_analysis/EdgeTypeManager.hpp>

#include <graph_analysis/gui/ComponentGraphEditor/PortConnection.hpp>
#include <graph_analysis/gui/ComponentGraphEditor/HasFeature.hpp>
#include <graph_analysis/gui/ComponentGraphEditor/ComponentItem.hpp>
#include <graph_analysis/gui/ComponentGraphEditor/PortConnectionItem.hpp>

using namespace graph_analysis;

namespace graph_analysis
{
namespace gui
{

ComponentGraphEditor::ComponentGraphEditor(QWidget *parent) : GraphWidget(parent)
{

    VertexTypeManager* vertexManager = VertexTypeManager::getInstance();
    vertexManager->registerType(Component::vertexType(), Vertex::Ptr(new Component()));
    vertexManager->registerType(InputPort::vertexType(), Vertex::Ptr(new InputPort()));
    vertexManager->registerType(OutputPort::vertexType(), Vertex::Ptr(new OutputPort()));

    EdgeTypeManager* edgeManager = EdgeTypeManager::getInstance();
    edgeManager->registerType(HasFeature::edgeType(), Edge::Ptr(new HasFeature()));
    edgeManager->registerType(PortConnection::edgeType(), Edge::Ptr(new PortConnection()));
}

ComponentGraphEditor::~ComponentGraphEditor() {}

QString ComponentGraphEditor::getClassName() const
{
    return "graph_analysis::gui::ComponentGraphEditor";
}

void ComponentGraphEditor::shuffle()
{
    int diff = 600;
    foreach(QGraphicsItem *item, scene()->items())
    {
        if(dynamic_cast<ComponentItem *>(item)) {
            item->setPos(-diff/2 + qrand() % diff, -diff/2 + qrand() % diff);
        }
    }
    updateStatus(
        "Shuffelled all nodes representing a 'Vertex' of the ComponentGraphEditor");
}

void ComponentGraphEditor::updateLayout()
{
    VertexIterator::Ptr nodeIt = mpGraph->getVertexIterator();
    while(nodeIt->next())
    {
        Vertex::Ptr vertex = nodeIt->current();

        // this widget only knows one type of "top level item", and this is the
        // component. so creating then:
        graph_analysis::Component::Ptr comp =
            dynamic_pointer_cast<Component>(vertex);
        if(comp)
        {
            ComponentItem *v = new ComponentItem(this, comp, NULL);
            scene()->addItem(v);
            v_map[vertex] = v;
        }
    }

    EdgeIterator::Ptr edgeIt = mpGraph->getEdgeIterator();
    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();

        graph_analysis::PortConnection::Ptr conn =
            dynamic_pointer_cast<PortConnection>(edge);
        if(!conn)
        {
            continue;
        }

        VertexItemBase *sourceItem = v_map[conn->getSourcePort(mpGraph)->getComponent(mpGraph)];
        VertexItemBase *targetItem = v_map[conn->getTargetPort(mpGraph)->getComponent(mpGraph)];

        // creating new edge items
        PortConnectionItem* e = new PortConnectionItem(this, conn, sourceItem, targetItem, NULL) ;

        scene()->addItem(e);
        e_map[conn] = e;
    }

    shuffle();
}

} // end namespace gui
} // end namespace graph_analysis
