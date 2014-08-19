#include "PlanningWidget.hpp"
#include "ui_planning_widget.h"

#include <QTreeWidgetItem>
#include <boost/foreach.hpp>

namespace omviz {

PlanningWidget::PlanningWidget(QWidget* parent)
    : QWidget(parent)
    , mUiPlanningWidget( new Ui::PlanningWidget)
{
    mUiPlanningWidget->setupUi(this);
    this->show();
}

PlanningWidget::~PlanningWidget()
{
    delete mUiPlanningWidget;
    mUiPlanningWidget = NULL;
}

void PlanningWidget::populate(const owl_om::OrganizationModel& model)
{
    pddl_planner::representation::Domain domain = mExporter.toDomain(model);
    pddl_planner::representation::Problem problem = mExporter.toProblem(model);

    populateDomainView(mUiPlanningWidget->treeWidget_PDDLDomain, domain);
    populateProblemView(mUiPlanningWidget->treeWidget_PDDLProblem, problem);
}


void PlanningWidget::populateDomainView(QTreeWidget* domainView, const pddl_planner::representation::Domain& domain)
{
    QTreeWidgetItem* domainLabel = createWidgetItem(domain.name);
    domainView->insertTopLevelItem(0,domainLabel);

    // types
    QTreeWidgetItem* typesWidgetItem = createTypesWidgetItem(domain.types, "types");
    domainLabel->addChild(typesWidgetItem);
    // constants
    QTreeWidgetItem* constantsWidgetItem = createTypedItemListWidgetItem(domain.constants, "constants");
    domainLabel->addChild(constantsWidgetItem);
    // predicates
    QTreeWidgetItem* predicatesWidgetItem = createPredicateListWidgetItem(domain.predicates, "predicates");
    domainLabel->addChild(predicatesWidgetItem);

    // requirements
    QTreeWidgetItem* requirementsWidgetItem = createTypesWidgetItem(domain.requirements, "requirements");
    domainLabel->addChild(requirementsWidgetItem);

    // action list
    QTreeWidgetItem* actionsWidgetItem = createActionsWidgetItem(domain.actions);
    domainLabel->addChild(actionsWidgetItem);

    // function list
    QTreeWidgetItem* functionsWidgetItem = createPredicateListWidgetItem(domain.functions, "functions");
    domainLabel->addChild(functionsWidgetItem);
}

void PlanningWidget::populateProblemView(QTreeWidget* problemView, const pddl_planner::representation::Problem& problem)
{
}

void PlanningWidget::populateSolutionsView(QTreeWidget* problemView, const pddl_planner::PlanCandidates& planCandidates)
{
}


QTreeWidgetItem* PlanningWidget::createWidgetItem(const std::string& label)
{
    return new QTreeWidgetItem( (QTreeWidget*) 0, QStringList(QString("%1").arg(label.c_str())) );
}

QTreeWidgetItem* PlanningWidget::createActionsWidgetItem(const pddl_planner::representation::ActionList& actions)
{
    using namespace pddl_planner::representation;
    QTreeWidgetItem* actionsWidgetItem = createWidgetItem("actions");
    BOOST_FOREACH(const Action& action, actions)
    {
        QTreeWidgetItem* actionWidgetItem = createActionWidgetItem(action);
        actionsWidgetItem->addChild(actionWidgetItem);
    }

    return actionsWidgetItem;
}

QTreeWidgetItem* PlanningWidget::createActionWidgetItem(const pddl_planner::representation::Action& action)
{
    using namespace pddl_planner::representation;

    QTreeWidgetItem* actionWidgetItem = createWidgetItem(action.label);
    QTreeWidgetItem* arguments = createWidgetItem("arguments");
    BOOST_FOREACH(const TypedItem& typedItem, action.arguments)
    {
        QTreeWidgetItem* typedItemWidgetItem = createWidgetItem(typedItem.label + ": " + typedItem.type);
        arguments->addChild(typedItemWidgetItem);
    }

    actionWidgetItem->addChild(arguments);

    QTreeWidgetItem* preconditions = createWidgetItem("preconditions");
    BOOST_FOREACH(const Expression& expression, action.preconditions)
    {
        QTreeWidgetItem* expressionWidgetItem = createWidgetItem(expression.toLISP());
        preconditions->addChild(expressionWidgetItem);
    }
    actionWidgetItem->addChild(preconditions);


    QTreeWidgetItem* effects = createWidgetItem("effects");
    BOOST_FOREACH(const Expression& expression, action.effects)
    {
        QTreeWidgetItem* expressionWidgetItem = createWidgetItem(expression.toLISP());
        effects->addChild(expressionWidgetItem);

    }
    actionWidgetItem->addChild(effects);

    return actionWidgetItem;
}

QTreeWidgetItem* PlanningWidget::createTypesWidgetItem(const pddl_planner::representation::TypeList& types, const std::string& label)
{
    QTreeWidgetItem* typesWidgetItem = createWidgetItem(label);
    BOOST_FOREACH(const pddl_planner::representation::Type& type, types)
    {
        QTreeWidgetItem* typeWidgetItem = createWidgetItem(type);
        typesWidgetItem->addChild(typeWidgetItem);
    }
    return typesWidgetItem;
}

QTreeWidgetItem* PlanningWidget::createTypedItemListWidgetItem(const pddl_planner::representation::TypedItemList& typedItems, const std::string& label)
{
    QTreeWidgetItem* typedItemListWidgetItem = createWidgetItem(label);
    BOOST_FOREACH(const pddl_planner::representation::TypedItem& typedItem, typedItems)
    {
        QTreeWidgetItem* typedItemWidgetItem = createWidgetItem(typedItem.label + ": " + typedItem.type);
        typedItemListWidgetItem->addChild(typedItemWidgetItem);
    }
    return typedItemListWidgetItem;
}

QTreeWidgetItem* PlanningWidget::createPredicateListWidgetItem(const pddl_planner::representation::PredicateList& predicates, const std::string& label)
{
    QTreeWidgetItem* predicateItemListWidgetItem = createWidgetItem(label);
    BOOST_FOREACH(const pddl_planner::representation::Predicate& predicate, predicates)
    {
        QTreeWidgetItem* predicateLabelWidgetItem = createWidgetItem(predicate.label);
        predicateItemListWidgetItem->addChild(predicateLabelWidgetItem);

        QTreeWidgetItem* argumentListWidgetItem = createTypedItemListWidgetItem(predicate.arguments, "arguments");
        predicateLabelWidgetItem->addChild(argumentListWidgetItem);
    }
    return predicateItemListWidgetItem;
}

} // end namespace omviz
