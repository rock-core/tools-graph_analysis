#ifndef OMVIZ_PLANNINGWIDGET_PLANNING_PROBLEM_DIALOG_HPP
#define OMVIZ_PLANNINGWIDGET_PLANNING_PROBLEM_DIALOG_HPP

#include <QtGui/QWidget>
#include <QDialog>
#include <QValidator>

#include <vector>
#include <pddl_planner/representation/Domain.hpp>

namespace Ui 
{
    class PlanningProblemDialog;
}

namespace omviz {

class ExpressionValidator : public QValidator
{
    Q_OBJECT;

public:
    ExpressionValidator(QObject* parent = 0);
    virtual ~ExpressionValidator();

    QValidator::State validate(QString& input, int& pos) const;
};

class PlanningProblemDialog : public QDialog
{
    Q_OBJECT;

public:
    PlanningProblemDialog(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    ~PlanningProblemDialog();

    void setTypeList(const pddl_planner::representation::TypeList& types);

    pddl_planner::representation::Constant getObject() const { return mObject; }

    // FIXME: returns string here, since segfaulting when using expression
    std::string getStatusExpression() const { return mStatus; }

public slots:
    void checkAndStoreValues();
    void checkStatusExpression(const QString& input);


private:
    Ui::PlanningProblemDialog* mUi;

    pddl_planner::representation::Constant mObject;
    std::string mStatus;
};

} // end namespace omviz
#endif // OMVIZ_PLANNINGWIDGET_PLANNING_PROBLEM_DIALOG_HPP
