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
    QObject::connect(mUiOmviz->comboBox, SIGNAL(activated(QString)),  mGraphWidget, SLOT(setLayout(QString)));
    QObject::connect(mUiOmviz->actionOpen, SIGNAL(triggered()), this, SLOT(loadOntology()));
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





} // end namespace omviz
