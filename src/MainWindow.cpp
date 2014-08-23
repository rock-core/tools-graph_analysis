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

    //connect(mUiOmviz->pushButton_addFilter, SIGNAL(pressed()), this, SLOT(addFilter()));
    //connect(mUiOmviz->pushButton_removeFilter, SIGNAL(pressed()), this, SLOT(removeFilter()));
    //connect(mUiOmviz->tableWidget_NodesFilter, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(activateNodeFilter(QTableWidgetItem*)));
    //connect(mUiOmviz->tableWidget_EdgesFilter, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(activateEdgeFilter(QTableWidgetItem*)));

    // Setup standard view by populating dock widgets
    // Left dock widget
    mOrganizationModelWidget = new OrganizationModelWidget();
    mOrganizationModelWidget->show();
    mUiOmviz->dockWidget_Left->setWidget(mOrganizationModelWidget);
    connect(mOrganizationModelWidget, SIGNAL(modelChanged()), this, SLOT(organizationModelChanged()));

    // RightTop dock widget
    mPropertiesWidget = new PropertiesWidget();
    mPropertiesWidget->show();
    mUiOmviz->dockWidget_RightTop->setWidget(mPropertiesWidget);
    connect(mPropertiesWidget, SIGNAL(layoutChanged(const QString&)), mGraphWidget, SLOT(setLayout(const QString&)));

    // RightBottom dock widget
    mFilterWidget = new FilterWidget();
    mFilterWidget->show();
    mUiOmviz->dockWidget_RightBottom->setWidget(mFilterWidget);
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
    using namespace owl_om;
    // Prepare graph widget to load new ontology
    mGraphWidget->reset();

    Ontology::Ptr ontology = mpOrganizationModel->ontology();

    // Create edges for all relations
    {
        using namespace owl_om;
        std::map<IRI, omviz::IRINode::Ptr> iriNodeMap;
        {
            owl_om::IRIList instances = ontology->allInstances();
            BOOST_FOREACH(const owl_om::IRI& instance, instances)
            {
                omviz::IRINode::Ptr node(new IRINode(instance, ontology));
                mGraphWidget->addVertex(node);

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

                    mGraphWidget->addEdge(edge);
                }
            }
        }
    }
    mGraphWidget->updateFromGraph();
}

void MainWindow::addFilter()
{
   // QTableWidget* edgesFilter = mUiOmviz->tableWidget_EdgesFilter;
   // QTableWidget* nodesFilter = mUiOmviz->tableWidget_NodesFilter;

   // QTableWidget* activeTable = 0;

   // if(mUiOmviz->tabWidget_Filters->currentWidget() == dynamic_cast<QWidget*>(edgesFilter->parent()))
   // {
   //     activeTable = edgesFilter;
   // } else if(mUiOmviz->tabWidget_Filters->currentWidget() == dynamic_cast<QWidget*>(nodesFilter->parent()))
   // {
   //     activeTable = nodesFilter;
   // } else {
   //     LOG_DEBUG_S << "Not added any filter";
   //     // nothing to do
   //     return;
   // }

   // LOG_DEBUG_S << "Added row at: " << activeTable->rowCount();

   // int row = activeTable->rowCount();
   // activeTable->blockSignals(true);
   // activeTable->insertRow(row);

   // // Create regex item
   // QTableWidgetItem* regexItem = new QTableWidgetItem();
   // activeTable->setItem(row, 0, regexItem);

   // // Create combo box to select CONTENT/CLASS
   // QComboBox* combo = new QComboBox();

   // QSignalMapper* signalMapper = new QSignalMapper(this);
   // QObject::connect(combo, SIGNAL(currentIndexChanged(int)), signalMapper, SLOT(map()));
   // signalMapper->setMapping(combo, row);

   // combo->addItem(QString( filters::TypeTxt[ filters::CONTENT].c_str() ));
   // combo->addItem(QString( filters::TypeTxt[ filters::CLASS].c_str() ));
   // activeTable->setCellWidget(row, 1, combo);

   // // Create checkbox to select inversion
   // QCheckBox* checkbox = new QCheckBox();
   // activeTable->setCellWidget(row, 2, checkbox);
   // QObject::connect(checkbox, SIGNAL(stateChanged(int)), signalMapper, SLOT(map()));
   // signalMapper->setMapping(checkbox, row);

   // if(activeTable == edgesFilter)
   // {
   //     activateEdgeFilter(regexItem);
   //     connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(activateEdgeFilter(int)));
   // } else if(activeTable == nodesFilter)
   // {
   //     activateNodeFilter(regexItem);
   //     connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(activateNodeFilter(int)));
   // }
   // activeTable->blockSignals(false);
}

void MainWindow::activateNodeFilter(QTableWidgetItem* item)
{
    //if(item)
    //{
    //    QTableWidget* table = mUiOmviz->tableWidget_NodesFilter;

    //    // Check the field next to this one
    //    int row = table->row(item);
    //    int column = table->column(item);

    //    QComboBox* classTypeSelection = dynamic_cast<QComboBox*>( table->cellWidget(row, column + 1) );
    //    QCheckBox* inversionSelection = dynamic_cast<QCheckBox*>( table->cellWidget(row, column + 2));
    //    if(classTypeSelection)
    //    {
    //        std::string typeName = classTypeSelection->currentText().toStdString();
    //        filters::Type type = filters::TxtType[ typeName ];

    //        QVariant regex = item->data(Qt::DisplayRole);
    //        try {
    //            bool invert = false;
    //            if(inversionSelection->checkState() == Qt::Checked)
    //            {
    //                invert = true;
    //            }

    //            Filter<Vertex::Ptr>::Ptr nodeFilter(
    //                    new filters::VertexRegexFilter(regex.toString().toStdString()
    //                        , type
    //                        , invert));

    //            LOG_DEBUG_S << "Activate node filter: " << nodeFilter->getName();
    //            try {
    //                mGraphWidget->replaceNodeFilter(nodeFilter, row);

    //            } catch(const std::out_of_range& e)
    //            {
    //                mGraphWidget->addNodeFilter(nodeFilter);
    //            }

    //            item->setForeground(QBrush());
    //            item->setToolTip(QString("Regex filter"));
    //        } catch(const boost::regex_error& e)
    //        {
    //            item->setForeground(QBrush(Qt::red));
    //            item->setToolTip(QString(e.what()));
    //        }
    //    }
    //    table->resizeColumnToContents(1);
    //    table->resizeColumnToContents(2);
    //}
    //mGraphWidget->update();
}

void MainWindow::activateEdgeFilter(QTableWidgetItem* item)
{
    //if(item)
    //{
    //    QTableWidget* table = mUiOmviz->tableWidget_EdgesFilter;

    //    // Check the field next to this one
    //    int row = table->row(item);
    //    int column = table->column(item);

    //    QComboBox* classTypeSelection = dynamic_cast<QComboBox*>( table->cellWidget(row, column + 1) );
    //    QCheckBox* inversionSelection = dynamic_cast<QCheckBox*>( table->cellWidget(row, column + 2));
    //    if(classTypeSelection)
    //    {
    //        std::string typeName = classTypeSelection->currentText().toStdString();
    //        filters::Type type = filters::TxtType[ typeName ];

    //        QVariant regex = item->data(Qt::DisplayRole);
    //        try {
    //            bool invert = false;
    //            if(inversionSelection->checkState() == Qt::Checked)
    //            {
    //                invert = true;
    //            }

    //            Filter<Edge::Ptr>::Ptr edgeFilter(
    //                    new filters::EdgeRegexFilter(regex.toString().toStdString()
    //                        , type
    //                        , invert));

    //            LOG_DEBUG_S << "Activate edge filter: " << edgeFilter->getName();
    //            try {
    //                mGraphWidget->replaceEdgeFilter(edgeFilter, row);

    //            } catch(const std::out_of_range& e)
    //            {
    //                mGraphWidget->addEdgeFilter(edgeFilter);
    //            }

    //            item->setForeground(QBrush());
    //            item->setToolTip(QString("Regex filter"));
    //        } catch(const boost::regex_error& e)
    //        {
    //            item->setForeground(QBrush(Qt::red));
    //            item->setToolTip(QString(e.what()));
    //        }
    //    }
    //}
    //mGraphWidget->update();
}

void MainWindow::removeFilter()
{
    //QTableWidget* edgesFilter = mUiOmviz->tableWidget_EdgesFilter;
    //QTableWidget* nodesFilter = mUiOmviz->tableWidget_NodesFilter;

    //if(mUiOmviz->tabWidget_Filters->currentWidget() == dynamic_cast<QWidget*>(edgesFilter->parent()))
    //{
    //    int rowId = edgesFilter->currentRow();
    //    LOG_DEBUG_S << "Remove edge filter at e: " << rowId;
    //    edgesFilter->removeRow(rowId);

    //    // +1 here since the graphwidget used one additional permit all filter
    //    if(rowId >= 0)
    //    {
    //        LOG_DEBUG_S << "Remove edge filter at: " << rowId;
    //        mGraphWidget->removeEdgeFilter(rowId);
    //    }
    //} else if(mUiOmviz->tabWidget_Filters->currentWidget() == dynamic_cast<QWidget*>(nodesFilter->parent()))
    //{
    //    int rowId = nodesFilter->currentRow();
    //    LOG_DEBUG_S << "Remove node filter at e: " << rowId;
    //    nodesFilter->removeRow(rowId);

    //    // +1 here since the graphwidget used one additional permit all filter
    //    if(rowId >= 0)
    //    {
    //        LOG_DEBUG_S << "Remove node filter at: " << rowId;
    //        mGraphWidget->removeNodeFilter(rowId);
    //    }
    //} else {
    //    // nothing to do
    //}
}


void MainWindow::activateNodeFilter(int row)
{
    //activateNodeFilter(mUiOmviz->tableWidget_NodesFilter->item(row,0));
}

void MainWindow::activateEdgeFilter(int row)
{
    //activateEdgeFilter(mUiOmviz->tableWidget_EdgesFilter->item(row,0));
}



} // end namespace omviz
