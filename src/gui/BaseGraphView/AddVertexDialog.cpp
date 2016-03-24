#include "AddVertexDialog.hpp"

#include "ui_AddVertexDialog.h"

namespace graph_analysis
{
namespace gui
{

AddVertexDialog::AddVertexDialog(QDialog* parent)
    : QDialog(parent)
    , mpUi(new ::Ui::AddVertexDialog())
{
    mpUi->setupUi(this);
    mpUi->lineedit_classname->setText("graph_analysis::Component");
    mpUi->lineedit_label->setText("someName");
}

AddVertexDialog::~AddVertexDialog()
{
    delete mpUi;
}

QString AddVertexDialog::getClassname() const
{
    return mpUi->lineedit_classname->text();
}

QString AddVertexDialog::getLabel() const
{
    return mpUi->lineedit_label->text();
}

} // end namespace dialogs
} // end namespace graph_analysis
