#include "PlanningProblemDialog.hpp"
#include "ui_planning_problem_dialog.h"
#include <boost/foreach.hpp>
#include <base/Logging.hpp>
#include <QPalette>

namespace omviz {

ExpressionValidator::ExpressionValidator(QObject* parent)
    : QValidator(parent)
{}

ExpressionValidator::~ExpressionValidator()
{}

QValidator::State ExpressionValidator::validate(QString& input, int& pos) const
{
    try {
        pddl_planner::representation::Expression::fromString(input.toStdString());
        return QValidator::Acceptable;
    } catch(const std::exception& e)
    {
        return QValidator::Invalid;
    }
}

PlanningProblemDialog::PlanningProblemDialog(QWidget* parent, Qt::WindowFlags flags)
    : QDialog(parent, flags)
    , mUi(new Ui::PlanningProblemDialog)
{
    mUi->setupUi(this);

    connect(mUi->buttonBox, SIGNAL(accepted()), this, SLOT(checkAndStoreValues()));
    connect(mUi->lineEdit_InitialStatus, SIGNAL(textChanged(const QString&)), this, SLOT(checkStatusExpression(const QString&)));
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
}

void PlanningProblemDialog::checkStatusExpression(const QString& input)
{
    QLineEdit* lineEdit = mUi->lineEdit_InitialStatus;

    LOG_DEBUG_S << "Check status expression for " << input.toStdString();
    try {
        pddl_planner::representation::Expression::fromString( input.toStdString());
        mStatus = input.toStdString();
        QPalette pallete = lineEdit->palette();
        pallete.setBrush(QPalette::Text, QBrush(Qt::black));
        lineEdit->setPalette(pallete);

    } catch(const std::exception& e)
    {
        QPalette pallete = lineEdit->palette();
        pallete.setBrush(QPalette::Text, QBrush(Qt::red));
        lineEdit->setPalette(pallete);
        lineEdit->setToolTip(e.what());
    }
    mUi->lineEdit_InitialStatus->update();
}

} // end namespace omviz
