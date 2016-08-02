#include "AddVertexDialog.hpp"

#include "ui_AddVertexDialog.h"

#include <graph_analysis/VertexTypeManager.hpp>

namespace graph_analysis
{
namespace gui
{

AddVertexDialog::AddVertexDialog(QDialog* parent)
    : QDialog(parent)
    , mpUi(new ::Ui::AddVertexDialog())
{
    mpUi->setupUi(this);
    mpUi->lineEdit_className->setText("graph_analysis::TheVertexClass");
    mpUi->lineEdit_label->setText("TheLabel");

    VertexTypeManager* vertexManager = VertexTypeManager::getInstance();
    std::set<std::string> registeredTypes = vertexManager->getSupportedTypes();
    std::set<std::string>::const_iterator it = registeredTypes.begin();
    for(; it != registeredTypes.end(); it++)
    {
        mpUi->comboBox_className->insertItem(0,QString::fromStdString(*it));
    }
    mpUi->comboBox_className->setCurrentIndex(0);
}

AddVertexDialog::~AddVertexDialog()
{
    delete mpUi;
}

QString AddVertexDialog::getClassname() const
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

QString AddVertexDialog::getLabel() const
{
    return mpUi->lineEdit_label->text();
}

} // end namespace dialogs
} // end namespace graph_analysis
