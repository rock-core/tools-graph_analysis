#ifndef OMVIZ_PLANNINGWIDGET_PLANNING_PROBLEM_DIALOG_HPP
#define OMVIZ_PLANNINGWIDGET_PLANNING_PROBLEM_DIALOG_HPP

#include <QtGui/QWidget>
#include <QDialog>

#include <vector>
#include <pddl_planner/representation/Domain.hpp>

namespace Ui 
{
    class PlanningProblemDialog;
}

namespace omviz {

class PlanningProblemDialog : public QDialog
{
    Q_OBJECT;

public:
    PlanningProblemDialog(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    ~PlanningProblemDialog();

    void setTypeList(const pddl_planner::representation::TypeList& types);

    pddl_planner::representation::Constant getObject() { return mObject; }
    pddl_planner::representation::Expression getExpression() { return mStatus; }

public slots:
    void checkAndStoreValues();


private:
    Ui::PlanningProblemDialog* mUi;

    pddl_planner::representation::Constant mObject;
    pddl_planner::representation::Expression mStatus;
};

} // end namespace omviz
#endif // OMVIZ_PLANNINGWIDGET_PLANNING_PROBLEM_DIALOG_HPP
