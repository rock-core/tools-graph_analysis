#include "AddGraphElement.hpp"
#include "ui_add_graph_element.h"

namespace graph_analysis {
namespace gui {
namespace dialogs {

AddGraphElement::AddGraphElement(const QStringList& types, QWidget* parent)
    : QDialog(parent)
    , mUi(new Ui::AddGraphElement)
{
    mUi->setupUi(this);
    this->show();

    mUi->typeComboBox->addItems(types);
}

AddGraphElement::~AddGraphElement()
{}

QString AddGraphElement::getLabel() const
{
    return mUi->labelLineEdit->text();
}

QString AddGraphElement::getType() const
{
    return mUi->typeComboBox->currentText();
}



} // end namespace dialogs
} // end namespace gui
} // end namespace graph_analysis
