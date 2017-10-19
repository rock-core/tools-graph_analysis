#include "AddEdgeDialog.hpp"

#include "ui_AddEdgeDialog.h"

#include "../../EdgeTypeManager.hpp"

namespace graph_analysis
{
namespace gui
{

AddEdgeDialog::AddEdgeDialog(QDialog* parent)
    : QDialog(parent)
    , mpUi(new ::Ui::AddEdgeDialog())
{
    mpUi->setupUi(this);
    mpUi->lineEdit_className->setText("graph_analysis::TheEdgeClass");
    mpUi->lineEdit_label->setText("TheLabel");

    EdgeTypeManager* edgeManager = EdgeTypeManager::getInstance();
    std::set<std::string> registeredTypes = edgeManager->getSupportedTypes();
    std::set<std::string>::const_iterator it = registeredTypes.begin();
    for(; it != registeredTypes.end(); it++)
    {
        mpUi->comboBox_className->insertItem(0,QString::fromStdString(*it));
    }
    mpUi->comboBox_className->setCurrentIndex(0);
}

AddEdgeDialog::~AddEdgeDialog()
{
    delete mpUi;
}

QString AddEdgeDialog::getClassname() const
{
    if(mpUi->tabWidget->currentIndex() == 0)
    {
        return mpUi->comboBox_className->currentText();
    }
    else
    {
        return mpUi->lineEdit_className->text();
    }
}

QString AddEdgeDialog::getLabel() const
{
    return mpUi->lineEdit_label->text();
}

} // end namespace dialogs
} // end namespace graph_analysis
