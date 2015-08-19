
#include "FilterManager.hpp"

#include <QRectF>
#include <QSizeF>
#include <base/Logging.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

namespace graph_analysis {
namespace gui {

FilterManager::FilterManager(QWidget *checkBoxGrid, QWidget *parent)
    : QGraphicsView(parent)
    , mpCheckBoxGrid(checkBoxGrid)
{
    QGraphicsScene *custom_scene = new QGraphicsScene(this);
    custom_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(custom_scene);

    setCacheMode(CacheBackground);
//    setContextMenuPolicy(Qt::CustomContextMenu);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
//    scale(qreal(0.8), qreal(0.8));
    setMinimumWidth(110);


    // populating the filter board with 3 example filters: filter1, filter2 and filter3
    addFilter("filter1");
    addFilter("filter2");
    addFilter("filter3");
    addFilter("filter4");
    addFilter("filter5");
}

FilterManager::~FilterManager()
{
}

void FilterManager::addFilter(const std::string& label)
{
    // introducing a new filter item
    mFilters.push_back(new FilterItem(this, mFilters.size(), label));
    scene()->addItem(mFilters.back());
    mFilters.back()->setPos(0.,  FilterItem::sHeight * (qreal)(mFilters.size() - 1));
    // introducing its corresponding enabling checkbox
    QCheckBox *newCheckBox = new QCheckBox(mpCheckBoxGrid);
    mpCheckBoxGrid->setFixedHeight((mCheckBoxes.size() + 1) * FilterItem::sHeight);
    newCheckBox->setGeometry(0, mCheckBoxes.size() * FilterItem::sHeight, FilterItem::sHeight, FilterItem::sHeight);
    mCheckBoxes.push_back(newCheckBox);
}

void FilterManager::swapFilters(FilterItem::filter_index_t left, FilterItem::filter_index_t right)
{
    dieOnIndex(left , "swapFilters");
    dieOnIndex(right, "swapFilters");

    FilterItem* cached_filter = mFilters[right];
    mFilters[right] = mFilters[left];
    mFilters[left ] = cached_filter;

    mFilters[right]->setIndex(right);
    cached_filter->setIndex(left);
    cached_filter->updatePos();
}

void FilterManager::dieOnIndex(FilterItem::filter_index_t index, const std::string& caller)
{
    if(index < 0 || index >= mFilters.size())
    {
        std::string method = ("" == caller) ? "dieOnIndex" : caller;
        std::string error_msg = std::string("graph_analysis::gui::FilterManager::") + method + ": the supplied index: "
                                        + boost::lexical_cast<std::string>(index)
                                        + " is out of bounds [0, "
                                        + boost::lexical_cast<std::string>(mFilters.size()) + "]";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
}

} // end namespace gui
} // end namespace graph_analysis
