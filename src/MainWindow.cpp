#include "MainWindow.hpp"
#include "ui_omviz.h"

#include <omviz/graphwidget/GraphWidget.hpp>

#include <omviz/graphwidget/NodeItem.hpp>
#include <omviz/graphwidget/GraphWidget.hpp>

#include <omviz/graphwidget/graphitem/Resource.hpp>
#include <boost/foreach.hpp>

#include <QFileDialog>
#include <QTreeWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDebug>
#include <QSignalMapper>
#include <QHBoxLayout>
#include <QCheckBox>

#include <graph_analysis/filters/RegexFilters.hpp>
#include "Utils.hpp"

using namespace graph_analysis;

namespace omviz {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , mUiOmviz(new Ui::Omviz)
{
    mUiOmviz->setupUi(this);
    this->show();

    // Setup pane
    mGraphWidget = new GraphWidget();
    mUiOmviz->tabWidget->clear();
    mUiOmviz->tabWidget->addTab(mGraphWidget, "GraphView");
}

MainWindow::~MainWindow()
{
    delete mUiOmviz;
    mUiOmviz = NULL;
}

void MainWindow::updateFromModel()
{
    // Prepare graph widget to load new ontology
    mGraphWidget->reset();

   
    graph_analysis::Vertex::Ptr vertex0(new graph_analysis::Vertex());
    graph_analysis::Vertex::Ptr vertex1(new graph_analysis::Vertex());
    graph_analysis::Edge::Ptr edge(new graph_analysis::Edge());
    edge->setSourceVertex(vertex0);
    edge->setTargetVertex(vertex1);
    // Add vertices and add edges
    mGraphWidget->addVertex(vertex0);
    mGraphWidget->addVertex(vertex1);
    mGraphWidget->addEdge(edge);

    mGraphWidget->updateFromGraph();
}

} // end namespace omviz
