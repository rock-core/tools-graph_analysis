#include "FilterWidget.hpp"
#include "ui_filter_widget.h"
#include "AddRegexFilterDialog.hpp"
#include <omviz/Utils.hpp>
#include <base/Logging.hpp>

namespace omviz 
{

FilterWidget::FilterWidget(QWidget* parent)
    : QWidget(parent)
    , mUi(new Ui::FilterWidget)
{
    mUi->setupUi(this);
    this->show();

    connect(mUi->pushButton_addNodeFilter, SIGNAL(clicked()), this, SLOT(addNodeFilter()));
    connect(mUi->pushButton_removeNodeFilter, SIGNAL(clicked()), this, SLOT(removeNodeFilter()));

    connect(mUi->pushButton_addEdgeFilter, SIGNAL(clicked()), this, SLOT(addEdgeFilter()));
    connect(mUi->pushButton_removeEdgeFilter, SIGNAL(clicked()), this, SLOT(removeEdgeFilter()));
}

FilterWidget::~FilterWidget()
{
    delete mUi;
}

void FilterWidget::addNodeFilter()
{
}

void FilterWidget::removeNodeFilter()
{
}

void FilterWidget::addEdgeFilter()
{
    using namespace graph_analysis;

    AddRegexFilterDialog dialog(this);
    if( dialog.exec() )
    {
        Filter<graph_analysis::Edge::Ptr>::Ptr filter = dialog.getEdgeFilter();
        mEdgeFilters.push_back(filter);
        LOG_DEBUG_S << "Edge filter added";
       
        QTreeWidgetItem* filterItem = Utils::createTreeWidgetItem(filter->toString());
        mUi->treeWidget_Filters->insertTopLevelItem(0, filterItem);

        emit updated();
    }
}

void FilterWidget::removeEdgeFilter()
{
}

} // end namespace omviz
