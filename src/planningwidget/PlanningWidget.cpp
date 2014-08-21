#include "PlanningWidget.hpp"
#include "ui_planning_widget.h"

#include <QTreeWidgetItem>
#include <boost/foreach.hpp>
#include "PlanningProblemDialog.hpp"

namespace omviz {

PlanningWidget::PlanningWidget(QWidget* parent)
    : QWidget(parent)
    , mUiPlanningWidget( new Ui::PlanningWidget)
    , mDomain("default-domain")
    , mProblem("default-problem", mDomain)
    , mPlannerName("LAMA")
{
    mUiPlanningWidget->setupUi(this);
    this->show();

    connect(mUiPlanningWidget->pushButton_ProblemAddItem, SIGNAL(clicked()), this, SLOT(addProblemItem()));
    connect(mUiPlanningWidget->textEdit_Goal, SIGNAL(textChanged()), this, SLOT(checkGoalExpression()));
    connect(mUiPlanningWidget->buttonBox_Goal, SIGNAL(accepted()), this, SLOT(plan()));
    connect(mUiPlanningWidget->buttonBox_Goal, SIGNAL(clicked(QAbstractButton*)), this, SLOT(plan()));
}

PlanningWidget::~PlanningWidget()
{
    delete mUiPlanningWidget;
    mUiPlanningWidget = NULL;
}

void PlanningWidget::populate(const owl_om::OrganizationModel& model)
{
    mDomain = mExporter.toDomain(model);
    mProblem = mExporter.toProblem(model);

    repopulate();
}

void PlanningWidget::repopulate()
{
    populateDomainView(mUiPlanningWidget->treeWidget_PDDLDomain, mDomain);
    populateProblemView(mUiPlanningWidget->treeWidget_PDDLProblem, mProblem);
}

void PlanningWidget::checkGoalExpression()
{
    QTextEdit* textEdit = mUiPlanningWidget->textEdit_Goal;
    QString goalText = textEdit->toPlainText();

    LOG_DEBUG_S << "Check goal expression for " << goalText.toStdString();
    try {
        pddl_planner::representation::Expression mGoal = pddl_planner::representation::Expression::fromString( goalText.toStdString());
        QPalette pallete = textEdit->palette();
        pallete.setBrush(QPalette::Text, QBrush(Qt::black));
        textEdit->setPalette(pallete);

    } catch(const std::exception& e)
    {
        QPalette pallete = textEdit->palette();
        pallete.setBrush(QPalette::Text, QBrush(Qt::red));
        textEdit->setPalette(pallete);
        textEdit->setToolTip(e.what());
    }
    textEdit->update();
}

void PlanningWidget::plan()
{
    LOG_DEBUG_S << "PLAN towards goal" << mGoal.toLISP();
    mProblem.setGoal(mGoal);
    pddl_planner::PlanCandidates candidates = mPlanning.plan(mDomain, mProblem, mPlannerName);
    LOG_DEBUG_S << "PLAN towards goal" << mGoal.toLISP() << " result: " << candidates.toString();
    populateSolutionsView(mUiPlanningWidget->treeWidget_Solutions, candidates);
}

void PlanningWidget::populateDomainView(QTreeWidget* domainView, const pddl_planner::representation::Domain& domain)
{
    domainView->clear();

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
    problemView->clear();

    QTreeWidgetItem* problemLabel = createWidgetItem(problem.name);
    problemView->insertTopLevelItem(0,problemLabel);

    // constants
    QTreeWidgetItem* constantsWidgetItem = createTypedItemListWidgetItem(problem.objects, "objects");
    problemLabel->addChild(constantsWidgetItem);

    // status
    QTreeWidgetItem* statusWidgetItem = createExpressionListWidgetItem(problem.status, "status");
    problemLabel->addChild(statusWidgetItem);
}

void PlanningWidget::populateSolutionsView(QTreeWidget* solutionsView, const pddl_planner::PlanCandidates& candidates)
{

    solutionsView->clear();
    QTreeWidgetItem* plansWidgetItem = createWidgetItem("plans");
    solutionsView->insertTopLevelItem(0, plansWidgetItem);

    if(candidates.plans.empty())
    {
        return;
    }

    int count = 0;
    using namespace pddl_planner;
    BOOST_FOREACH(const Plan& candidate, candidates.plans)
    {
        std::stringstream ss;
        ss << "candidate #" << count++;
        QTreeWidgetItem* planWidgetItem = createWidgetItem(ss.str());
        BOOST_FOREACH(const Action& action, candidate.action_sequence)
        {
            QTreeWidgetItem* actionWidgetItem = createWidgetItem(action.toString());
            planWidgetItem->addChild(actionWidgetItem);
        }
        plansWidgetItem->addChild(planWidgetItem);
    }
}

void PlanningWidget::addProblemItem()
{
    PlanningProblemDialog dialog(this);
    dialog.setTypeList( mDomain.types );
    dialog.exec();

    pddl_planner::representation::Constant constant = dialog.getObject();
    if(constant.label != "")
    {
        mProblem.addObject(constant);
    }

    std::string status = dialog.getStatusExpression();
    if(!status.empty())
    {
        mProblem.addInitialStatus( pddl_planner::representation::Expression::fromString(status) );
    }

    repopulate();
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
    QTreeWidgetItem* arguments = createTypedItemListWidgetItem(action.arguments, "arguments");
    actionWidgetItem->addChild(arguments);

    QTreeWidgetItem* preconditions = createExpressionListWidgetItem(action.preconditions, "preconditions");
    actionWidgetItem->addChild(preconditions);

    QTreeWidgetItem* effects = createExpressionListWidgetItem(action.effects, "effects");
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

QTreeWidgetItem* PlanningWidget::createExpressionListWidgetItem(const pddl_planner::representation::ExpressionList& expressions, const std::string& label)
{
    QTreeWidgetItem* expressionsWidgetItem = createWidgetItem(label);
    BOOST_FOREACH(const pddl_planner::representation::Expression& expression, expressions)
    {
        QTreeWidgetItem* expressionWidgetItem = createWidgetItem(expression.toLISP());
        expressionsWidgetItem->addChild(expressionWidgetItem);
    }
    return expressionsWidgetItem;
}

} // end namespace omviz
