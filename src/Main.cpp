#include <iostream>
#include "ui_omviz.h"

#include <omviz/graphwidget/GraphWidget.hpp>

#include <omviz/graphwidget/NodeItem.hpp>
#include <omviz/graphwidget/GraphWidget.hpp>

#include <QApplication>
#include <QTime>
#include <QMainWindow>

#include <omviz/IRINode.hpp>
#include <omviz/IRIEdge.hpp>
#include <omviz/graphwidget/graphitem/Resource.hpp>
#include <boost/foreach.hpp>

#include <owl_om/OrganizationModel.hpp>

int main(int argc, char** argv)
{
    using namespace omviz;
    QApplication app(argc, argv);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    QMainWindow mainWindow;

    Ui::Omviz omviz;
    omviz.setupUi(&mainWindow);
    mainWindow.show();

    omviz::GraphWidget* graphWidget = new omviz::GraphWidget();
    omviz.tabWidget->clear();
    omviz.tabWidget->addTab(graphWidget, "GraphView");

    // Create vertices
    for(int i = 0; i < 1; ++i)
    {
        graph_analysis::Vertex::Ptr vertex(new graph_analysis::Vertex());
        graphWidget->addVertex(vertex);
    }

    owl_om::Ontology::Ptr ontology;
    owl_om::OrganizationModel om;

    if(argc == 2)
    {
        qDebug("Loading from file %s",argv[1]);
//        ontology = owl_om::Ontology::fromFile(argv[1]);
        om = owl_om::OrganizationModel(argv[1]);
        ontology = om.ontology();

    } else {
        printf("Usage: %s <file>", argv[0]);
        exit(-1);
    }

    // Create instances of models
    using namespace owl_om;
    using namespace owl_om::vocabulary;
    {
        om.createNewFromModel(OM::Actor(), OM::resolve("Sherpa"), true);
        //om.createNewFromModel(OM::Actor(), OM::resolve("CREX"), true);
        //om.createNewFromModel(OM::Actor(), OM::resolve("PayloadCamera"), true);
        //om.refresh();
    }

    // Create edges for all relations
    {
        using namespace owl_om;
        std::map<IRI, omviz::IRINode::Ptr> iriNodeMap;
        {
            owl_om::IRIList instances = ontology->allInstances();
            BOOST_FOREACH(const owl_om::IRI& instance, instances)
            {
                qDebug("Instance -> %s", instance.toString().c_str());
                omviz::IRINode::Ptr node(new IRINode(instance, ontology));
                graphWidget->addVertex(node);

                iriNodeMap[instance] = node;
            }
        }

        IRIList instances = ontology->allInstances();
        BOOST_FOREACH(const IRI& instance, instances)
        {
            IRIList objectProperties = ontology->allObjectProperties();
            BOOST_FOREACH(const IRI& relation, objectProperties)
            {
                IRIList related = ontology->allRelatedInstances(instance, relation);
                BOOST_FOREACH(const IRI& other, related)
                {
                    omviz::IRIEdge::Ptr edge(new IRIEdge(relation, ontology));
                    // get IRINodes by IRI
                    edge->setSourceVertex( iriNodeMap[instance] );
                    edge->setTargetVertex( iriNodeMap[other] );

                    graphWidget->addEdge(edge);
                }
            }
        }
    }
    graphWidget->updateFromGraph();

    return app.exec();
}
