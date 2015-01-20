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

int main(int argc, char **argv)
{
    using namespace omviz;

    QApplication app(argc, argv);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    GraphWidget* widget = new GraphWidget;

    owlapi::model::OWLOntology::Ptr ontology;
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
    // TODO: SCHOKO remove code
    //using namespace owlapi::vocabulary;
    //{
    //    om.createNewFromModel(OM::resolve("Sherpa"), true);
    //    om.createNewFromModel(OM::resolve("CREX"), true);
    //    om.createNewFromModel(OM::resolve("PayloadCamera"), true);
    //    om.refresh();
    //}

    // Create edges for all relations
    {
        using namespace owlapi::model;
        OWLOntologyAsk ask(ontology);
        std::map<IRI, omviz::IRINode::Ptr> iriNodeMap;
        {
            IRIList instances = ask.allInstances();
            BOOST_FOREACH(const IRI& instance, instances)
            {
                omviz::IRINode::Ptr node(new IRINode(instance, ontology));
                widget->addVertex(node);

                iriNodeMap[instance] = node;
            }
        }

        IRIList instances = ask.allInstances();
        BOOST_FOREACH(const IRI& instance, instances)
        {
            IRIList objectProperties = ask.allObjectProperties();
            BOOST_FOREACH(const IRI& relation, objectProperties)
            {
                IRIList related = ask.allRelatedInstances(instance, relation);
                BOOST_FOREACH(const IRI& other, related)
                {
                    omviz::IRIEdge::Ptr edge(new IRIEdge(relation, ontology));
                    // get IRINodes by IRI
                    edge->setSourceVertex( iriNodeMap[instance] );
                    edge->setTargetVertex( iriNodeMap[other] );

                    widget->addEdge(edge);
                }
            }
        }
    }
    widget->updateFromGraph();
    //widget->shuffle();

    QMainWindow mainWindow;
    mainWindow.setCentralWidget(widget);

    mainWindow.show();
    return app.exec();
}
