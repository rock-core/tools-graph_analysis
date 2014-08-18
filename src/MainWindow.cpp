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
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDebug>

#include <graph_analysis/filters/RegexFilters.hpp>

using namespace graph_analysis;

namespace omviz {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , mUiOmviz(new Ui::Omviz)
{
    mUiOmviz->setupUi(this);
    this->show();

    // Setup pane
    mGraphWidget = new omviz::GraphWidget();
    mUiOmviz->tabWidget->clear();
    mUiOmviz->tabWidget->addTab(mGraphWidget, "GraphView");

    // Setup signal/slots
    QObject::connect(mUiOmviz->comboBox_Layout, SIGNAL(activated(QString)),  mGraphWidget, SLOT(setLayout(QString)));
    QObject::connect(mUiOmviz->actionOpen, SIGNAL(triggered()), this, SLOT(loadOntology()));

    QObject::connect(mUiOmviz->pushButton_addFilter, SIGNAL(pressed()), this, SLOT(addFilter()));
    QObject::connect(mUiOmviz->pushButton_removeFilter, SIGNAL(pressed()), this, SLOT(removeFilter()));
    QObject::connect(mUiOmviz->tableWidget_NodesFilter, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(activateNodeFilter(QTableWidgetItem*)));
    QObject::connect(mUiOmviz->tableWidget_EdgesFilter, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(activateEdgeFilter(QTableWidgetItem*)));
}

MainWindow::~MainWindow()
{
    delete mUiOmviz;
    mUiOmviz = NULL;
}

void MainWindow::loadOntology()
{

    QString filename = QFileDialog::getOpenFileName(this, "Open organization model", QDir::currentPath(), "OWL Based Model (*.owl)");

    try {
        mOrganizationModel = owl_om::OrganizationModel(filename.toStdString());
    } catch(const std::runtime_error& e)
    {
        qDebug("Loading from file %s failed",filename.toStdString().c_str());
        return;
    }

    qDebug("Loading from file %s succeeded",filename.toStdString().c_str());


    // Create instances of models
    using namespace owl_om;
    using namespace owl_om::vocabulary;

    // Prepare graph widget to load new ontology
    mGraphWidget->reset();

    Ontology::Ptr ontology = mOrganizationModel.ontology();

    {
        mOrganizationModel.createNewFromModel(OM::Actor(), OM::resolve("Sherpa"), true);
        mOrganizationModel.createNewFromModel(OM::Actor(), OM::resolve("CREX"), true);
        //om.createNewFromModel(OM::Actor(), OM::resolve("PayloadCamera"), true);
        mOrganizationModel.refresh();
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
                mGraphWidget->addVertex(node);

                iriNodeMap[instance] = node;
            }
        }

        IRIList instances = ontology->allInstances();
        BOOST_FOREACH(const IRI& instance, instances)
        {

            QList<QTreeWidgetItem *> items;

            QTreeWidgetItem* iri = new QTreeWidgetItem((QTreeWidget*) 0, QStringList(QString("%1").arg(instance.toString().c_str())));


            IRIList objectProperties = ontology->allObjectProperties();
            BOOST_FOREACH(const IRI& relation, objectProperties)
            {
                QTreeWidgetItem* relationItem = new QTreeWidgetItem((QTreeWidget*) 0, QStringList(QString("%1").arg(relation.toString().c_str())));

                IRIList related = ontology->allRelatedInstances(instance, relation);
                BOOST_FOREACH(const IRI& other, related)
                {
                    omviz::IRIEdge::Ptr edge(new IRIEdge(relation, ontology));
                    // get IRINodes by IRI
                    edge->setSourceVertex( iriNodeMap[instance] );
                    edge->setTargetVertex( iriNodeMap[other] );

                    mGraphWidget->addEdge(edge);

                    QTreeWidgetItem* otherItem = new QTreeWidgetItem((QTreeWidget*) 0, QStringList(QString("%1").arg(other.toString().c_str())));

                    relationItem->addChild(otherItem);
                }
                if(relationItem->childCount() != 0)
                {
                    iri->addChild(relationItem);
                }
            }

            IRI className = ontology->typeOf(instance);
            QTreeWidgetItem* typeOfLabel = new QTreeWidgetItem((QTreeWidget*) 0, QStringList( QString(RDF::type().toString().c_str())));
            QTreeWidgetItem* iriClass = new QTreeWidgetItem((QTreeWidget*) 0, QStringList(QString("%1").arg(className.toString().c_str())));
            typeOfLabel->addChild(iriClass);
            iri->addChild(typeOfLabel);

            items.append(iri);
            mUiOmviz->treeWidget->insertTopLevelItems(0,items);
        }

        // Adapt to contents: column index start with 0 as usual
        mUiOmviz->treeWidget->resizeColumnToContents(0);
    }
    mGraphWidget->updateFromGraph();
}

void MainWindow::addFilter()
{
    QTableWidget* edgesFilter = mUiOmviz->tableWidget_EdgesFilter;
    QTableWidget* nodesFilter = mUiOmviz->tableWidget_NodesFilter;

    QTableWidget* activeTable = 0;

    if(mUiOmviz->tabWidget_Filters->currentWidget() == dynamic_cast<QWidget*>(edgesFilter->parent()))
    {
        activeTable = edgesFilter;
    } else if(mUiOmviz->tabWidget_Filters->currentWidget() == dynamic_cast<QWidget*>(nodesFilter->parent()))
    {
        activeTable = nodesFilter;
    } else {
        LOG_DEBUG_S << "Not added any filter";
        // nothing to do
        return;
    }


    int row = activeTable->rowCount();
    activeTable->blockSignals(true);
    activeTable->insertRow(row);

    // Create regex item
    QTableWidgetItem* regexItem = new QTableWidgetItem();
    activeTable->setItem(row, 0, regexItem);

    //QTableWidgetItem* typeItem = new QTableWidgetItem();
    QComboBox* combo = new QComboBox();
    //typeItem->setFlags( typeItem->flags() & ~Qt::ItemIsEditable);
    //typeItem->setData(Qt::DisplayRole, QString( filters::TypeTxt[ filters::CONTENT].c_str() ) );
    combo->addItem(QString( filters::TypeTxt[ filters::CONTENT] ));
    combo->addItem(QString( filters::TypeTxt[ filters::CLASS] ));

    activeTable->setItem(row, 1, combo); 

    if(activeTable == edgesFilter)
    {
        activateEdgeFilter(regexItem);
    } else if(activeTable == nodesFilter)
    {
        activateNodeFilter(regexItem);
    }
    activeTable->blockSignals(false);
}

void MainWindow::activateNodeFilter(QTableWidgetItem* item)
{
    if(item)
    {
        LOG_DEBUG_S << "Activate node filter";
        QTableWidget* table = mUiOmviz->tableWidget_NodesFilter;

        // Check the field next to this one
        int row = table->row(item);
        int column = table->column(item);

        //QTableWidgetItem* classTypeItem = table->item(row, column + 1);
        //if(classTypeItem)
        //{
        //    std::string typeName = classTypeItem->data(Qt::DisplayRole).toString().toStdString();
        //    filters::Type type = filters::TxtType[ typeName ];

        //    QVariant regex = item->data(Qt::DisplayRole);
        //    Filter<Vertex::Ptr>::Ptr nodeFilter(
        //            new filters::VertexRegexFilter(regex.toString().toStdString()
        //                , type
        //                , false));
        //    try {
        //        mGraphWidget->replaceNodeFilter(nodeFilter, row);

        //    } catch(const std::out_of_range& e)
        //    {
        //        mGraphWidget->addNodeFilter(nodeFilter);
        //    }
        //}
    }
}

void MainWindow::activateEdgeFilter(QTableWidgetItem* item)
{
    using namespace graph_analysis;

    QTableWidget* table = mUiOmviz->tableWidget_EdgesFilter;
    int contentColumnIndex = 0;
    int classColumnIndex = 1;

    int column = table->column(item);
    int row = table->row(item);

    QTableWidgetItem* contentRegexItem = table->item(row, contentColumnIndex);
    if(contentRegexItem)
    {
        try {
            QVariant contentRegex = contentRegexItem->data(Qt::DisplayRole);
            Filter<Edge::Ptr>::Ptr edgeFilter(
                    new filters::EdgeRegexFilter(contentRegex.toString().toStdString()
                        , filters::CONTENT
                        , false));
           
            try {
                mGraphWidget->replaceEdgeFilter(edgeFilter, row);
            } catch(const std::out_of_range& e)
            {
                mGraphWidget->addEdgeFilter(edgeFilter);
            }

            // Set default graph information for that field
            contentRegexItem->setBackground( QBrush());
            contentRegexItem->setToolTip( QString("Regex filter") );
        } catch(const boost::regex_error& e)
        {
            // Handle regex error and display
            contentRegexItem->setToolTip( QString(e.what()) );
            contentRegexItem->setBackground( QBrush(Qt::red));
        }
    }
    mGraphWidget->update();
}

void MainWindow::removeFilter()
{
    QTableWidget* edgesFilter = mUiOmviz->tableWidget_EdgesFilter;
    QTableWidget* nodesFilter = mUiOmviz->tableWidget_NodesFilter;

    if(mUiOmviz->tabWidget_Filters->currentWidget() == dynamic_cast<QWidget*>(edgesFilter->parent()))
    {
        int rowId = edgesFilter->currentRow(); 
        LOG_DEBUG_S << "Remove edge filter at e: " << rowId;
        edgesFilter->removeRow(rowId);

        // +1 here since the graphwidget used one additional permit all filter
        if(rowId >= 0)
        {
            LOG_DEBUG_S << "Remove edge filter at: " << rowId;
            mGraphWidget->removeEdgeFilter(rowId);
        }
    } else if(mUiOmviz->tabWidget_Filters->currentWidget() == dynamic_cast<QWidget*>(nodesFilter->parent()))
    {
        int rowId = nodesFilter->currentRow(); 
        LOG_DEBUG_S << "Remove node filter at e: " << rowId;
        nodesFilter->removeRow(rowId);

        // +1 here since the graphwidget used one additional permit all filter
        if(rowId >= 0)
        {
            LOG_DEBUG_S << "Remove node filter at: " << rowId;
            mGraphWidget->removeNodeFilter(rowId);
        }
    } else {
        // nothing to do
    }
}





} // end namespace omviz
