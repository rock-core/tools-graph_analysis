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

#include <graph_analysis/gui/NodeItem.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>

#include <QTime>
#include <QHBoxLayout>
#include <QDockWidget>
#include <QMainWindow>
#include <QApplication>

#include <string>
#include <sstream>
#include <boost/foreach.hpp>
#include <graph_analysis/gui/graphitem/Resource.hpp>
#include <graph_analysis/filters/RegexFilters.hpp>
// test graphs
#define TEST_1ST_GRAPH 0
#define TEST_IMPORT 1
#define TEST_TYPES 2
#define TEST_TYPES_FAIL 3
#define TEST_PORTS_EDGES_INTERACTION_DEPTH1 4
#define TEST_PORTS_EDGES_INTERACTION_DEPTH2 5

// choosing the test graph to display
#define TESTNO TEST_IMPORT
//#define RENDER
#define IMPORT
#define FILENAME  "../../resources/gexf/export_samples/port__types_test.gexf"

//  possible layouts: circo, dot, fdp, neato, osage, sfdp, twopi
//#define LAYOUT "dot"

void help_setup(std::stringstream& ss, const std::string& cmd)
{
    ss << "usage: " << cmd << " [ --help ]" << std::endl;
    ss << std::endl;
    ss << "Options:" << std::endl;
    ss << "-------"  << std::endl;
    ss << "    --help " << "displays this informative message" << std::endl;
    ss << std::endl;
    ss << "Details:" << std::endl;
    ss << "-------"  << std::endl;
    ss << " The current graph GUI implementation illustrates the Rock Orogen Components diagrams use-case."; // << std::endl;
    ss << " The Orogen Components are modeled as cluster nodes in an internal base graph, while their input/output "; // << std::endl;
    ss << "ports correspond to port vertices through which clusters form edges."; // << std::endl;
    ss << std::endl;
    ss << std::endl;
    ss << "Features:" << std::endl;
    ss << "--------"  << std::endl;
    ss << "The GUI contains a main windnow representing the Orogen Components Diagram (the view of the graph) and a dockable panel with button-commands." << std::endl;
    ss << "The commands panel is accompanied by homologous commands in the context menu of the main window. "; // << std::endl;
    ss << "The commands panel can be reloaded (if previously closed) via the extra command 'Reload Properties' made available in the context menu "; // << std::endl;
    ss << "or by the keyboard short-cut CTRL+P (CTRL+p)" << std::endl;
    ss << std::endl;
    ss << "In the so-called 'move-around' mode, the user can re-arranged and closely manipulate displayed components (a.k.a. node items)." << std::endl;
    ss << "The re-arranging is performed by dragging the components with the mouse while the manipulation includes the following commands "; // << std:endl;
    ss << "made available in both the commands panel and the main window context menu:" << std::endl;
    ss << "    At all times, the following commands are available:" << std::endl;
    ss << "        'Add Node' - adds a new node to the graph"       << std::endl;
    ss << "        'Import'   - opens an existing graph file"       << std::endl;
    ss << "        'Export'   - saves the graph to file"            << std::endl;
    ss << "        'Refresh'  - reloads the graph view"             << std::endl;
    ss << "        'Shuffle'  - randomly spreads vertices around"   << std::endl;
    ss << "        'Reset'    - completely deletes the graph"       << std::endl;
    ss << "        'Layout'   - applies a new layout to the graph"  << std::endl;
    ss << "        Drag'n'Drop- toggles the drag-n-drop mode"       << std::endl;
    ss << "    When a component has been focused on - i.e. the user double-clicked it";     // << std::endl;
    ss << " the following commands are made available in the commands panel:"               << std::endl;
    ss << "    (or in the context menu when the component has been right-clicked instead)"  << std::endl;
    ss << "        'Rename Node'    - changes the label of the selected vertex"             << std::endl;
    ss << "        'Add Port'       - adds a port to the selected vertex"                   << std::endl;
    ss << "        'Swap Ports'     - swaps the positions of two ports of that vertex"      << std::endl;
    ss << "        'Rename a Port'  - changes the label of a port of the selected vertex"   << std::endl;
    ss << "        'Remove a Port'  - removes a port of the selected vertex"                << std::endl;
    ss << "        'Remove Ports'   - removes ALL ports of the selected vertex"             << std::endl;
    ss << "        'Remove Node'    - removes the selected vertex from the graph"           << std::endl;
    ss << "    When an edge has been focused on - i.e. the user double-clicked its label";  // << std::endl;
    ss << " the following commands are made available in the commands panel:"               << std::endl;
    ss << "    (or in the context menu when the component has been right-clicked instead)"  << std::endl;
    ss << "        'Rename Edge'    - changes the label of the selected edge"               << std::endl;
    ss << "        'Remove Edge'    - removes the selected edge from the graph"             << std::endl;
    ss << "The view can generally be changed at all times by zooming in or out with the";   // << std::endl;
    ss << " mouse wheel or by 2D translations via dragging with the middle mouse button."   << std::endl;
    ss << std::endl;
    ss << "In the 'drag-n-drop' mode, the user can spawn new edges from output ports of one component to input ports of another component "; // << std::endl;
    ss << "and swap same-type ports of the same component." << std::endl;
    ss << "The user can toggle in between the 2 modes either by using the <Drag'n'Drop> button command in the dockable panel, "; // << std::endl;
    ss << "or the homologous one in the main window context menu, or by employing one of the 2 keyboard short-cuts: CTRL+CapsLock or CTRL+D (CTRL+d)." << std::endl;
    ss << std::endl;
    ss << "Keys Combinations:" << std::endl;
    ss << "---- ------------"  << std::endl;
    ss << "    Minus    (-)     -*  zoom-out the view" << std::endl;
    ss << "    Plus     (+)     -*  zoom-in  the view" << std::endl;
    ss << "    Space    (space) -*  refresh the graph view" << std::endl;
    ss << "    R        (r)     -*  refresh the graph view" << std::endl;
    ss << "    S        (s)     -*  shuffle the graph nodes" << std::endl;
    ss << "    L        (l)     -*  change graph layout" << std::endl;
    ss << std::endl;
    ss << "    Escape   (ESC)   -   clear focus (if any node and/or edge is being focused on)" << std::endl;
    ss << std::endl;
    ss << "    CTRL+Minus(CTRL-)-   zoom-out the view" << std::endl;
    ss << "    CTRL+Plus (CTRL+)-   zoom-in  the view" << std::endl;
    ss << "    CTRL+Q   (CTRL+q)-   quit" << std::endl;
    ss << "    CTRL+W   (CTRL+w)-   quit" << std::endl;
    ss << "    CTRL+R   (CTRL+r)-   reset graph" << std::endl;
    ss << "    CTRL+S   (CTRL+s)-   save (export graph)" << std::endl;
    ss << "    CTRL+A   (CTRL+a)-   add new node to graph" << std::endl;
    ss << "    CTRL+I   (CTRL+i)-   open (import graph)" << std::endl;
    ss << "    CTRL+O   (CTRL+o)-   open (import graph)" << std::endl;
    ss << "    CTRL+P   (CTRL+p)-   reload buttons panel (if closed)" << std::endl;
    ss << "    CTRL+Left        -   rotate view anti-clockwise" << std::endl;
    ss << "    CTRL+Right       -   rotate view clockwise" << std::endl;
    ss << "    CTRL+D   (CTRL+d)-   toggle in between the two modes" << std::endl;
    ss << "    CTRL+CapsLock    -   toggle in between the two modes" << std::endl;
    ss << std::endl;
    ss << "    SHIFT+Up         -** shift up the port that is being edited" << std::endl;
    ss << "    SHIFT+Down       -** shift down the port that is being edited" << std::endl;
    ss << std::endl;
    ss << "Notes:" << std::endl;
    ss << "    -*  available in move-around mode only" << std::endl;
    ss << "    -** available in drag-n-drop mode only" << std::endl;
    ss << std::endl;
}

int main(int argc, char **argv)
{
    if(argc > 1)
    {
        if("--help" == std::string(argv[1]))
        {
            std::stringstream help_message;
            help_setup(help_message, std::string(argv[0]));
            std::cout << help_message.rdbuf();
            return 0;
        }
        else
        {
            std::cout << "usage: " << argv[0] << " [--help]\n";
            return 0;
        }
    }

    using namespace graph_analysis::gui;

    QApplication app(argc, argv);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    QMainWindow mainWindow;
    mainWindow.setMinimumSize(850, 400);
    GraphWidget* widget = new GraphWidget(&mainWindow, &mainWindow);
    widget->reset();

    using namespace graph_analysis;

    filters::VertexRegexFilter sourceNodeFilter(".*");
    filters::VertexRegexFilter targetNodeFilter(".*");
    filters::EdgeRegexFilter edgeFilter(".*");

    filters::EdgeContextFilter::Ptr filter(new filters::CombinedEdgeRegexFilter(sourceNodeFilter, edgeFilter, targetNodeFilter));
    std::vector< Filter< graph_analysis::Edge::Ptr >::Ptr > edgeFilters;
    edgeFilters.push_back(filter);
    widget->setEdgeFilters(edgeFilters);


    int testNo = TESTNO;
    switch(testNo)
    {
        case TEST_1ST_GRAPH:
        {
            /*
             * TEST_1ST_GRAPH:
             * --------------
             */
            Vertex::Ptr v0(new Vertex("v0"));
            Vertex::Ptr v1(new Vertex("v1"));
            Vertex::Ptr v2(new Vertex("v2"));
            Vertex::Ptr v3(new Vertex("v3"));
            Vertex::Ptr v4(new Vertex("v4"));

            Edge::Ptr edge0(new Edge("edge0"));
            Edge::Ptr edge1(new Edge("edge1"));
            Edge::Ptr edge2(new Edge("edge2"));
            Edge::Ptr edge3(new Edge("edge3"));
            Edge::Ptr edge4(new Edge("edge4"));
            Edge::Ptr edge5(new Edge("edge5"));

            // Note: no bidirectional edge in this example
            edge0->setSourceVertex(v0);
            edge0->setTargetVertex(v1);

            edge1->setSourceVertex(v0);
            edge1->setTargetVertex(v3);

            edge2->setSourceVertex(v1);
            edge2->setTargetVertex(v2);

            edge3->setSourceVertex(v1);
            edge3->setTargetVertex(v4);

            edge4->setSourceVertex(v2);
            edge4->setTargetVertex(v3);

            edge5->setSourceVertex(v3);
            edge5->setTargetVertex(v4);

            widget->addVertex(v0);
            widget->addVertex(v1);
            widget->addVertex(v2);
            widget->addVertex(v3);
            widget->addVertex(v4);
            widget->enableVertex(v0);
            widget->enableVertex(v1);
            widget->enableVertex(v2);
            widget->enableVertex(v3);
            widget->enableVertex(v4);

            widget->addEdge(edge0);
            widget->addEdge(edge1);
            widget->addEdge(edge2);
            widget->addEdge(edge3);
            widget->addEdge(edge4);
            widget->addEdge(edge5);
            widget->enableEdge(edge0);
            widget->enableEdge(edge1);
            widget->enableEdge(edge2);
            widget->enableEdge(edge3);
            widget->enableEdge(edge4);
            widget->enableEdge(edge5);
        }
        break;

        case TEST_IMPORT:
        {
            // working on a void graph
        }
        break;

        case TEST_TYPES:
        {
            Vertex::Ptr c0 = widget->createVertex("cluster", "c0");
            Vertex::Ptr c1 = widget->createVertex("cluster", "c1");
            Vertex::Ptr c2 = widget->createVertex("cluster", "c2");

            Vertex::Ptr c0p0 = widget->createVertex("port", "c0p0");
            Vertex::Ptr c0p1 = widget->createVertex("port", "c0p1");
            Vertex::Ptr c0p2 = widget->createVertex("port", "c0p2");
            widget->createEdge(c0, c0p0, "c0e0");
            widget->createEdge(c0, c0p1, "c0e1");
            widget->createEdge(c0, c0p2, "c0e2");

            Vertex::Ptr c1p0 = widget->createVertex("port", "c1p0");
            Vertex::Ptr c1p1 = widget->createVertex("port", "c1p1");
            widget->createEdge(c1, c1p0, "c1e0");
            widget->createEdge(c1, c1p1, "c1e1");

            Vertex::Ptr c2p0 = widget->createVertex("port", "c2p0");
            Vertex::Ptr c2p1 = widget->createVertex("port", "c2p1");
            widget->createEdge(c2, c2p0, "c2e0");
            widget->createEdge(c2, c2p1, "c2e1");

            widget->createEdge(c0p0, c2p0, "edge0");
//            widget->createEdge(c1p1, c2p0, "edge1");
//            widget->createEdge(c2p1, c0p2, "edge2");
//            widget->createEdge(c1p0, c0p1, "edge3");
        }
        break;

        case TEST_TYPES_FAIL:
        {
            Vertex::Ptr c0 = widget->createVertex("coconut", "c0");
            Vertex::Ptr c0p0 = widget->createVertex("port", "c0p0");
        }
        break;

        case TEST_PORTS_EDGES_INTERACTION_DEPTH1:
        {
            /* firstly manipulating the conceptual view of the graph */
            // 2 cluster nodes: c0, c1
            Vertex::Ptr c0 = widget->createVertex("cluster", "c0");
            Vertex::Ptr c1 = widget->createVertex("cluster", "c1");

            // 1 port for c0: c0p0
            Vertex::Ptr c0p0 = widget->createVertex("port", "c0p0");
            widget->createEdge(c0, c0p0, "c0e0");

            // 1 port for c1: c1p0
            Vertex::Ptr c1p0 = widget->createVertex("port", "c1p0");
            widget->createEdge(c1, c1p0, "c1e0");

            // 1 physical edge: edge0
            widget->createEdge(c0p0, c1p0, "edge0");
        }
        break;

        case TEST_PORTS_EDGES_INTERACTION_DEPTH2:
        {
            /* manipulating the conceptual view of the graph */
            // 3 cluster nodes: c0, c1, c2
            Vertex::Ptr c0 = widget->createVertex("cluster", "c0");
            Vertex::Ptr c1 = widget->createVertex("cluster", "c1");
            Vertex::Ptr c2 = widget->createVertex("cluster", "c2");

            // 4 ports for c0: c0p0, c0p1, c0p2, c0p3
            Vertex::Ptr c0p0 = widget->createVertex("port", "c0p0");
            widget->createEdge(c0, c0p0, "c0e0");
            Vertex::Ptr c0p1 = widget->createVertex("port", "c0p1");
            widget->createEdge(c0, c0p1, "c0e1");
            Vertex::Ptr c0p2 = widget->createVertex("port", "c0p2");
            widget->createEdge(c0, c0p2, "c0e2");
            Vertex::Ptr c0p3 = widget->createVertex("port", "c0p3");
            widget->createEdge(c0, c0p3, "c0e3");

            // 5 ports for c1: c1p0, c1p1, c1p2, c1p3, c1p4
            Vertex::Ptr c1p0 = widget->createVertex("port", "c1p0");
            widget->createEdge(c1, c1p0, "c1e0");
            Vertex::Ptr c1p1 = widget->createVertex("port", "c1p1");
            widget->createEdge(c1, c1p1, "c1e1");
            Vertex::Ptr c1p2 = widget->createVertex("port", "c1p2");
            widget->createEdge(c1, c1p2, "c1e2");
            Vertex::Ptr c1p3 = widget->createVertex("port", "c1p3");
            widget->createEdge(c1, c1p3, "c1e3");
            Vertex::Ptr c1p4 = widget->createVertex("port", "c1p4");
            widget->createEdge(c1, c1p4, "c1e4");

            // 2 ports for c2: c2p0, c2p1
            Vertex::Ptr c2p0 = widget->createVertex("port", "c2p0");
            widget->createEdge(c2, c2p0, "c2e0");
            Vertex::Ptr c2p1 = widget->createVertex("port", "c2p1");
            widget->createEdge(c2, c2p1, "c2e1");

            // many physical edges: edge0, edge1, ...
            widget->createEdge(c0p0, c1p0, "edge0");
            widget->createEdge(c0p0, c1p1, "edge1");
            widget->createEdge(c0p3, c1p3, "edge2");
            widget->createEdge(c1p2, c0p2, "edge3");
            widget->createEdge(c2p0, c0p3, "edge4");
            widget->createEdge(c1p1, c2p1, "edge5");
        }
        break;
    }

#ifdef LAYOUT
    widget->setLayout(QString(LAYOUT));
#else
    widget->updateFromGraph();
#endif
#ifdef RENDER
    widget->toFile("graph.dot");
#endif

    mainWindow.setCentralWidget(widget);

#ifdef IMPORT
    widget->fromXmlFile
                (
                    FILENAME
                );
#endif
    mainWindow.show();
    return app.exec();
}
