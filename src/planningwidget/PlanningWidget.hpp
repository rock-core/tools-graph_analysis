#ifndef OMVIZ_PLANNINGWIDGET_PLANNING_WIDGET_HPP
#define OMVIZ_PLANNINGWIDGET_PLANNING_WIDGET_HPP

#include <QtGui/QWidget>
#include <owl_om/OrganizationModel.hpp>
#include <owl_om/exporter/PDDLExporter.hpp>
#include <pddl_planner/Planning.hpp>

class QTreeWidget;
class QTreeWidgetItem;

namespace Ui
{
    class PlanningWidget;
}

namespace omviz
{

class PlanningWidget : public QWidget
{
    Q_OBJECT

public: 
    PlanningWidget(QWidget* parent = 0);
    ~PlanningWidget();

    void populate(owl_om::OrganizationModel::Ptr model);
    /**
     * Use internal status to refresh view
     */

public slots:
    void addProblemItem();
    void repopulate();
    void checkGoalExpression();
    void plan();

    void contextMenuDomain(const QPoint&);
    void contextMenuProblem(const QPoint&);
    void saveAllToDirectory(const QString& directory) const;

protected:

    void populateDomainView(QTreeWidget* domainView, const pddl_planner::representation::Domain& domain);
    void populateProblemView(QTreeWidget* problemView, const pddl_planner::representation::Problem& problem);
    void populateSolutionsView(QTreeWidget* solutionsView, const pddl_planner::PlanCandidates& planCandidates);


private:

    void saveToFile(const QString& filename, const QString& data) const;

    QTreeWidgetItem* createWidgetItem(const std::string& label);
    QTreeWidgetItem* createActionsWidgetItem(const pddl_planner::representation::ActionList& actions);
    QTreeWidgetItem* createActionWidgetItem(const pddl_planner::representation::Action& action);
    QTreeWidgetItem* createTypesWidgetItem(const pddl_planner::representation::TypeList& types, const std::string& label);
    QTreeWidgetItem* createTypedItemListWidgetItem(const pddl_planner::representation::TypedItemList& typedItems, const std::string& label);
    QTreeWidgetItem* createPredicateListWidgetItem(const pddl_planner::representation::PredicateList& predicates, const std::string& label);
    QTreeWidgetItem* createExpressionListWidgetItem(const pddl_planner::representation::ExpressionList& expressionList, const std::string& label);

    Ui::PlanningWidget* mUiPlanningWidget;
    owl_om::PDDLExporter mExporter;

    pddl_planner::representation::Domain mDomain;
    pddl_planner::representation::Problem mProblem;
    pddl_planner::representation::Expression mGoal;
    pddl_planner::PlanCandidates mCandidates;
    pddl_planner::Planning mPlanning;
    std::string mPlannerName;
};

} // end namespace omiz
#endif // OMVIZ_PLANNINGWIDGET_PLANNING_WIDGET_HPP
