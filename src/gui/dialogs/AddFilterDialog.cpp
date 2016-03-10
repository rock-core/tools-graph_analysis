#include "AddFilterDialog.hpp"
#include "ui_add_filter_dialog_gui.h"

namespace graph_analysis {
namespace gui {
namespace dialogs {

AddFilter::AddFilter(QWidget* parent)
    : QDialog(parent)
    , mUi(new Ui::Dialog)
{
    mUi->setupUi(this);
    this->show();
}

AddFilter::~AddFilter()
{ delete mUi; }

QString AddFilter::getFilterRegexp() const
{
    return mUi->lineEdit->text();
}

bool AddFilter::isEnabled() const
{
    return mUi->checkBox->isEnabled();
}

} // end namespace dialogs
} // end namespace gui
} // end namespace graph_analysis
