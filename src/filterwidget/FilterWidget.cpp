#include "FilterWidget.hpp"
#include "ui_filter_widget.h"
#include "AddRegexFilterDialog.hpp"
#include <omviz/Utils.hpp>
#include <base/Logging.hpp>

namespace ga = graph_analysis;

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


std::vector< ga::Filter< ga::Edge::Ptr>::Ptr > FilterWidget::getEdgeFilters()
{
    ItemEdgeFilterList filters;
    ItemEdgeFilterMap::iterator cit = mEdgeFilters.begin();
    for(; cit != mEdgeFilters.end(); ++cit)
    {
        filters.push_back(cit->second);
    }
    return filters;
}

std::vector< ga::Filter< ga::Vertex::Ptr>::Ptr > FilterWidget::getNodeFilters()
{
    ItemNodeFilterList filters;
    ItemNodeFilterMap::iterator cit = mNodeFilters.begin();
    for(; cit != mNodeFilters.end(); ++cit)
    {
        filters.push_back(cit->second);
    }
    return filters;
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
        LOG_DEBUG_S << "Edge filter added: " << filter->toString();
       
        QTreeWidgetItem* filterItem = Utils::createTreeWidgetItem(filter->toString());
        mUi->treeWidget_Filters->insertTopLevelItem(0, filterItem);

        mEdgeFilters[filterItem] = filter;
        emit updated();
    }
}

void FilterWidget::removeEdgeFilter()
{
    QTreeWidget* treeWidget = mUi->treeWidget_Filters;

    QTreeWidgetItem* item = treeWidget->currentItem();
    ItemEdgeFilterMap::iterator it = mEdgeFilters.find(item);
    if(it != mEdgeFilters.end())
    {
        mEdgeFilters.erase(item);
        treeWidget->removeItemWidget(item, 0 /*column*/);
        delete item;

        emit updated();
    } else {
        LOG_DEBUG_S << "omviz::FilterWidget::removeEdgeFilter: could not remove filter -- item does not exist";
    }
}

} // end namespace omviz
