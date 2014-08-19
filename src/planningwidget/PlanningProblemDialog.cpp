#include "PlanningProblemDialog.hpp"
#include "ui_planning_problem_dialog.h"
#include <boost/foreach.hpp>

namespace omviz {

PlanningProblemDialog::PlanningProblemDialog(QWidget* parent, Qt::WindowFlags flags)
    : QDialog(parent, flags)
    , mUi(new Ui::PlanningProblemDialog)
{
    mUi->setupUi(this);

    connect(mUi->buttonBox, SIGNAL(accepted()), this, SLOT(checkAndStoreValues()));
}

PlanningProblemDialog::~PlanningProblemDialog()
{
    delete mUi;
}

void PlanningProblemDialog::setTypeList(const pddl_planner::representation::TypeList& types)
{
    BOOST_FOREACH(const pddl_planner::representation::Type& type, types)
    {
        mUi->comboBox_Types->addItem( QString( type.c_str() ));
    }
}

void PlanningProblemDialog::checkAndStoreValues()
{
    QString objectLabel = mUi->lineEdit_TypeLabel->text();
    QString objectType = mUi->comboBox_Types->currentText();
    if( objectLabel != QString("") && objectType != QString(""))
    {
        mObject = pddl_planner::representation::TypedItem(objectLabel.toStdString(), objectType.toStdString());
    }

    if(mUi->lineEdit_InitialStatus)
    {
        //m
    }
}

} // end namespace omviz
