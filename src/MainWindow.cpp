#include "MainWindow.hpp"
#include "ui_omviz.h"

#include <omviz/graphwidget/GraphWidget.hpp>

#include <omviz/graphwidget/NodeItem.hpp>
#include <omviz/graphwidget/GraphWidget.hpp>

#include <omviz/IRINode.hpp>
#include <omviz/IRIEdge.hpp>
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
#include "planningwidget/PlanningWidget.hpp"
#include "organizationmodelwidget/OrganizationModelWidget.hpp"
#include "propertieswidget/PropertiesWidget.hpp"
#include "filterwidget/FilterWidget.hpp"
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

    mPlanningWidget = new PlanningWidget();
    mUiOmviz->tabWidget->addTab(mPlanningWidget, "Planning");

    // Setup signal/slots
    //connect(mUiOmviz->comboBox_Layout, SIGNAL(activated(QString)),  mGraphWidget, SLOT(setLayout(QString)));
    connect(mUiOmviz->actionOpen, SIGNAL(triggered()), this, SLOT(loadOntology()));

    // Setup standard view by populating dock widgets
    // Left dock widget
    mOrganizationModelWidget = new OrganizationModelWidget();
    mOrganizationModelWidget->show();
    mUiOmviz->dockWidget_Left->setWidget(mOrganizationModelWidget);
    connect(mOrganizationModelWidget, SIGNAL(modelChanged()), this, SLOT(organizationModelChanged()));
    connect(mOrganizationModelWidget, SIGNAL(currentSelectionChanged(QString, QString)), this, SLOT(organizationModelSelectionChanged(QString, QString)));

    // RightTop dock widget
    mPropertiesWidget = new PropertiesWidget();
    mPropertiesWidget->show();
    mUiOmviz->dockWidget_RightTop->setWidget(mPropertiesWidget);
    connect(mPropertiesWidget, SIGNAL(layoutChanged(const QString&)), mGraphWidget, SLOT(setLayout(const QString&)));

    // RightBottom dock widget
    mFilterWidget = new FilterWidget();
    mFilterWidget->show();
    mUiOmviz->dockWidget_RightBottom->setWidget(mFilterWidget);
    connect(mFilterWidget, SIGNAL(updated()), this, SLOT(updateFilters()));
}

MainWindow::~MainWindow()
{
    delete mUiOmviz;
    mUiOmviz = NULL;
}

void MainWindow::organizationModelChanged()
{
    // First retrieve update of organization model widget
    // since here inference takes place
    mOrganizationModelWidget->setModel(mpOrganizationModel);
    mOrganizationModelWidget->updateFromModel();

    // Perform consumer / view updates afterwards
    mPlanningWidget->populate(mpOrganizationModel);
    updateFromModel();
}

void MainWindow::loadOntology()
{

    QString filename = QFileDialog::getOpenFileName(this, "Open organization model", QDir::currentPath(), "OWL Based Model (*.owl)");

    try {
        mpOrganizationModel = owl_om::OrganizationModel::Ptr( new owl_om::OrganizationModel(filename.toStdString()));
        organizationModelChanged();
    } catch(const std::runtime_error& e)
    {
        qDebug("Loading from file %s failed",filename.toStdString().c_str());
        return;
    }
    qDebug("Loading from file %s succeeded",filename.toStdString().c_str());
}

void MainWindow::updateFromModel()
{
    // Create instances of models
    using namespace owlapi::model;

    // Prepare graph widget to load new ontology
    mGraphWidget->reset();

    OWLOntology::Ptr ontology = mpOrganizationModel->ontology();
    OWLOntologyAsk ask(ontology);

    // Create edges for all relations
    {
        using namespace owl_om;
        std::map<IRI, omviz::IRINode::Ptr> iriNodeMap;
        {
            IRIList instances = ask.allInstances();
            BOOST_FOREACH(const IRI& instance, instances)
            {
                omviz::IRINode::Ptr node(new IRINode(instance, ontology));
                mGraphWidget->addVertex(node);

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

                    mGraphWidget->addEdge(edge);
                }
            }
        }
    }
    mGraphWidget->updateFromGraph();
}

void MainWindow::organizationModelSelectionChanged(QString parentItem, QString currentItem)
{
    std::string regex;
    if(parentItem == QString(""))
    {
        regex = ".*";
    } else {
        regex = Utils::escapeString( owlapi::model::IRI(parentItem.toStdString()).getFragment() );
    }

    filters::VertexRegexFilter sourceNodeFilter(regex);
    filters::VertexRegexFilter targetNodeFilter(".*");
    filters::EdgeRegexFilter edgeFilter(".*");
    EdgeContextFilter::Ptr filter( new filters::CombinedEdgeRegexFilter( sourceNodeFilter, edgeFilter, targetNodeFilter ));
    std::vector< Filter< graph_analysis::Edge::Ptr >::Ptr > edgeFilters;
    edgeFilters.push_back(filter);

    LOG_DEBUG_S << "FILTER is: " << filter->toString();

    mGraphWidget->setNodeFilters( mFilterWidget->getNodeFilters() );
    mGraphWidget->setEdgeFilters(edgeFilters);
    mGraphWidget->refresh();
}

void MainWindow::updateFilters()
{
    mGraphWidget->setNodeFilters( mFilterWidget->getNodeFilters() );
    mGraphWidget->setEdgeFilters( mFilterWidget->getEdgeFilters() );

    mGraphWidget->refresh();
}

} // end namespace omviz
