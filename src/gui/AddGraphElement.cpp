#include "AddGraphElement.hpp"
#include "ui_add_graph_element.h"

namespace graph_analysis {
namespace gui {

AddGraphElement::AddGraphElement(const QStringList& types, QWidget* parent)
    : QDialog(parent)
    , mUi(new Ui::AddGraphElement)
{
    mUi->setupUi(this);
    this->show();

    mUi->typeComboBox->addItems(types);
    connect(mUi->typeComboBox, SIGNAL(activated(QString)),
            this, SLOT(typeChanged(QString)));
}

AddGraphElement::~AddGraphElement()
{}

QString AddGraphElement::getLabel() const { return mUi->labelLineEdit->text(); }

} // end namespace gui
} // end namespace graph_analysis
