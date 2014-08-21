#include "OrganizationModelWidget.hpp"
#include "ui_organization_model_widget.h"
#include <boost/foreach.hpp>
#include "AddActorDialog.hpp"

namespace omviz
{

OrganizationModelWidget::OrganizationModelWidget(QWidget* parent)
    : QTabWidget(parent)
    , mpOrganizationModel()
    , mUi(new Ui::OrganizationModelWidget())
{
    mUi->setupUi(this);
    this->show();

    connect(mUi->pushButton_AddActor, SIGNAL(clicked()), this, SLOT(createNewFromModel()));
    connect(mUi->pushButton_Refresh, SIGNAL(clicked()), this, SLOT(refresh()));
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

    BOOST_FOREACH(const IRI& instance, instances)
    {

        QList<QTreeWidgetItem *> items;

        QTreeWidgetItem* iri = createWidgetItem(instance.toString());

        IRIList objectProperties = ontology->allObjectProperties();
        BOOST_FOREACH(const IRI& relation, objectProperties)
        {
            QTreeWidgetItem* relationItem = createWidgetItem(relation.toString());

            IRIList related = ontology->allRelatedInstances(instance, relation);
            BOOST_FOREACH(const IRI& other, related)
            {
                QTreeWidgetItem* otherItem = createWidgetItem(other.toString());
                relationItem->addChild(otherItem);
            }

            if(relationItem->childCount() != 0)
            {
                iri->addChild(relationItem);
            }
        }

        IRI className = ontology->typeOf(instance);
        QTreeWidgetItem* typeOfLabel = createWidgetItem( owl_om::vocabulary::RDF::type().toString());
        QTreeWidgetItem* iriClass = createWidgetItem(className.toString());
        typeOfLabel->addChild(iriClass);
        iri->addChild(typeOfLabel);

        items.append(iri);
        treeWidget->insertTopLevelItems(0,items);
    }

    // Adapt to contents: column index start with 0 as usual
    treeWidget->resizeColumnToContents(0);
}

void OrganizationModelWidget::refresh()
{
    if(mpOrganizationModel)
    {
        mpOrganizationModel->refresh();
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
            IRI instance = mpOrganizationModel->createNewFromModel(vocabulary::OM::Actor(), dialog.getModel().toString(), true /* create dependant resources */);
            LOG_DEBUG_S << "Created new actor: " << instance;
            emit modelChanged();
        }
    }
}

QTreeWidgetItem* OrganizationModelWidget::createWidgetItem(const std::string& label)
{
    return new QTreeWidgetItem( (QTreeWidget*) 0, QStringList(QString("%1").arg(label.c_str())) );
}

} // end namespace omviz
