#include "PropertiesWidget.hpp"
#include "ui_properties_widget.h"

namespace omviz
{

PropertiesWidget::PropertiesWidget(QWidget* parent)
    : QTabWidget(parent)
    , mUi(new Ui::PropertiesWidget)
{
    mUi->setupUi(this);
    this->show();

    connect(mUi->comboBox_Layout, SIGNAL(activated(QString)), this, SIGNAL(layoutChanged(QString)));
}

PropertiesWidget::~PropertiesWidget()
{
}

} // end namespace omviz
