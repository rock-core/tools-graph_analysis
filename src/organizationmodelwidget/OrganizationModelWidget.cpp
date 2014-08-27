#include "OrganizationModelWidget.hpp"
#include "ui_organization_model_widget.h"
#include "AddActorDialog.hpp"

#include <QApplication>
#include <boost/foreach.hpp>
#include <base/Time.hpp>
#include <omviz/Utils.hpp>

namespace omviz
{

OrganizationModelWidget::OrganizationModelWidget(QWidget* parent)
    : QTabWidget(parent)
    , mpOrganizationModel()
    , mUi(new Ui::OrganizationModelWidget())
    , mCurrentWidgetItem(0)
{
    mUi->setupUi(this);
    this->show();

    connect(mUi->pushButton_AddActor, SIGNAL(clicked()), this, SLOT(createNewFromModel()));
    connect(mUi->pushButton_Refresh, SIGNAL(clicked()), this, SLOT(refresh()));

    connect(mUi->treeWidget_Plain, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(itemClicked(QTreeWidgetItem*, int)));
    connect(mUi->treeWidget_AtomicActors, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(itemClicked(QTreeWidgetItem*, int)));
    connect(mUi->treeWidget_CompositeActors, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(itemClicked(QTreeWidgetItem*, int)));
}

OrganizationModelWidget::~OrganizationModelWidget()
{
    delete mUi;
    mUi = NULL;
}

void OrganizationModelWidget::updateFromModel()
{ 
    using namespace owl_om;
    using namespace owl_om::vocabulary;

    updateTreeWidget(mUi->treeWidget_Plain);
    updateTreeWidget(mUi->treeWidget_AtomicActors, OM::Actor(), true/*directInstances only*/);
    updateTreeWidget(mUi->treeWidget_CompositeActors, OM::CompositeActor(), true/*directInstances only*/);
}

void OrganizationModelWidget::updateTreeWidget(QTreeWidget* treeWidget, const owl_om::IRI& modelType, bool directInstances)
{
    treeWidget->clear();
    using namespace owl_om;
    Ontology::Ptr ontology = mpOrganizationModel->ontology();
    IRIList instances;
    if(modelType.empty())
    {
        instances = ontology->allInstances();
    } else {
        instances = ontology->allInstancesOf(modelType, directInstances);
    }

    LOG_DEBUG_S << "Update: instances #" << instances.size();
    QTreeWidgetItem* ontologyWidgetItem = Utils::createTreeWidgetItem("organization model");
    treeWidget->insertTopLevelItem(0, ontologyWidgetItem);

    BOOST_FOREACH(const IRI& instance, instances)
    {

        QList<QTreeWidgetItem *> items;

        QTreeWidgetItem* iri = Utils::createTreeWidgetItem(instance.getFragment(), instance.toString());

        IRIList objectProperties = ontology->allObjectProperties();
        BOOST_FOREACH(const IRI& relation, objectProperties)
        {
            {
                QTreeWidgetItem* relationItem = Utils::createTreeWidgetItem(relation.getFragment(), relation.toString());

                IRIList related = ontology->allRelatedInstances(instance, relation);
                BOOST_FOREACH(const IRI& other, related)
                {
                    QTreeWidgetItem* otherItem = Utils::createTreeWidgetItem(other.getFragment(), other.toString());
                    relationItem->addChild(otherItem);
                }
                if(relationItem->childCount() != 0)
                {
                    iri->addChild(relationItem);
                }
            }

            {
                QTreeWidgetItem* relationItem = Utils::createTreeWidgetItem("-" + relation.getFragment(), "-" + relation.toString());

                IRIList related = ontology->allInverseRelatedInstances(instance, relation);
                BOOST_FOREACH(const IRI& other, related)
                {
                    QTreeWidgetItem* otherItem = Utils::createTreeWidgetItem(other.getFragment(), other.toString());
                    relationItem->addChild(otherItem);
                }
                if(relationItem->childCount() != 0)
                {
                    iri->addChild(relationItem);
                }
            }

        }

        IRI className = ontology->typeOf(instance);
        QTreeWidgetItem* typeOfLabel = Utils::createTreeWidgetItem(owl_om::vocabulary::RDF::type().getFragment(), owl_om::vocabulary::RDF::type().toString());
        QTreeWidgetItem* iriClass = Utils::createTreeWidgetItem(className.getFragment(), className.toString());
        typeOfLabel->addChild(iriClass);
        iri->addChild(typeOfLabel);

        items.append(iri);
        ontologyWidgetItem->addChildren(items);
    }

    // Adapt to contents: column index start with 0 as usual
    treeWidget->resizeColumnToContents(0);
}

void OrganizationModelWidget::refresh()
{
    if(mpOrganizationModel)
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        LOG_WARN_S << "Organization model going to be refreshed. Upper instance bound at: " << mpOrganizationModel->upperCombinationBound(); 
        LOG_INFO_S << "Please wait ...";
        mpOrganizationModel->refresh();
        QApplication::restoreOverrideCursor();
        LOG_WARN_S << "Organization model updated.";
        LOG_WARN_S << mpOrganizationModel->getStatistics().toString();
        emit modelChanged();
    }
}

void OrganizationModelWidget::createNewFromModel()
{
    using namespace owl_om;

    if(mpOrganizationModel)
    {
        AddActorDialog dialog(this);
        IRIList actorModels = mpOrganizationModel->ontology()->allInstancesOf(vocabulary::OM::ActorModel(), true);
        dialog.setModelList(actorModels);

        if( dialog.exec())
        {
            IRI instance = mpOrganizationModel->createNewFromModel(dialog.getModel().toString(), true /* create dependant resources */);
            LOG_DEBUG_S << "Created new model instance: " << instance << " of type: " << mpOrganizationModel->ontology()->typeOf(instance);
            emit modelChanged();
        }
    }
}

void OrganizationModelWidget::itemClicked(QTreeWidgetItem* item, int column)
{
    LOG_DEBUG_S << "Item active: " << item->data(0, Qt::DisplayRole).toString().toStdString();
    mCurrentWidgetItem = item;

    QTreeWidgetItem* parentInstanceItem = getParentInstance(item);
    QString itemDataParent = parentInstanceItem->data(0, Qt::ToolTipRole).toString();
    QString currentItemData = item->data(0, Qt::ToolTipRole).toString();

    LOG_DEBUG_S << "Current selection changed: " << itemDataParent.toStdString();
    emit currentSelectionChanged(itemDataParent, currentItemData);
}

QTreeWidgetItem* OrganizationModelWidget::getParentInstance(QTreeWidgetItem* item)
{
    if(!item->parent())
    {
        // top level root selected
        return item;
    } else if(!item->parent()->parent())
    {
        // second level root selected
        return item;
    } else {
        return getParentInstance(item->parent());
    }
}

} // end namespace omviz
