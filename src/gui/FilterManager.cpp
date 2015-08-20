
#include "FilterManager.hpp"
#include "AddFilterDialog.hpp"

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
    setToolTip(QString("Custom Filters Manager Box"));
    setCacheMode(CacheBackground);
//    setContextMenuPolicy(Qt::CustomContextMenu);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setMinimumWidth(110);
}

FilterManager::~FilterManager()
{
}

void FilterManager::updateToolTip(int state)
{
    bool witness = Qt::Unchecked == state ? false : true;
    QCheckBox *sender = (QCheckBox*)QObject::sender();
    FilterItem::filter_index_t index = mCheckBoxIndexMap[sender];
    dieOnIndex(index , "updateToolTip(int)");
    updateToolTip(index, witness);
}

void FilterManager::addFilter(const std::string& label)
{
    unsigned int index = createFilter(label);
    refreshToolTip(index);
}

void FilterManager::addFilter(const std::string& label, bool enable)
{
    unsigned int index = createFilter(label);
    mCheckBoxes[index]->setChecked(enable);
    updateToolTip(index, enable);
}

unsigned int FilterManager::createFilter(const std::string& label)
{
    // introducing a new filter item
    std::string regexp = label.empty() ? std::string("CustomRegexpFilter") : label;
    mFilters.push_back(new FilterItem(this, mFilters.size(), regexp));
    scene()->addItem(mFilters.back());
    mFilters.back()->setPos(0., FilterItem::sHeight * (qreal)(mFilters.size() - 1));
    // introducing its corresponding enabling checkbox
    QCheckBox *newCheckBox = new QCheckBox(mpCheckBoxGrid);
    unsigned int index = mCheckBoxes.size();
    mpCheckBoxGrid->setFixedHeight((index + 1) * FilterItem::sHeight);
    newCheckBox->setGeometry(0, index * FilterItem::sHeight, FilterItem::sHeight, FilterItem::sHeight);
    newCheckBox->show();
    mCheckBoxes.push_back(newCheckBox);
    mCheckBoxIndexMap[newCheckBox] = index;
    FilterManager::connect(newCheckBox, SIGNAL(stateChanged(int)), this, SLOT(updateToolTip(int)));
    return index;
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

    // swapping their checkboxes references too (much faster than toggling their values + updating tooltips)
//    refreshToolTip(left);
//    refreshToolTip(right);
    QCheckBox *cached_checkbox = mCheckBoxes[right];
    mCheckBoxes[right] = mCheckBoxes[left];
    mCheckBoxes[left] = cached_checkbox;

    // updating the checkbox indices map
    mCheckBoxIndexMap[mCheckBoxes[left ]] =  left;
    mCheckBoxIndexMap[mCheckBoxes[right]] = right;
    // updating the position on the screen
    mCheckBoxes[left ]->setGeometry(0, left  * FilterItem::sHeight, FilterItem::sHeight, FilterItem::sHeight);
    mCheckBoxes[right]->setGeometry(0, right * FilterItem::sHeight, FilterItem::sHeight, FilterItem::sHeight);
}

void FilterManager::refreshToolTip(FilterItem::filter_index_t index)
{
    dieOnIndex(index, "refreshToolTip");
    QCheckBox *current_checkbox = mCheckBoxes[index];
    bool witness = current_checkbox->isChecked();
    updateToolTip(index, witness);
}

void FilterManager::updateToolTip(FilterItem::filter_index_t index, bool witness)
{
    dieOnIndex(index, "updateToolTip");
    QCheckBox *current_checkbox = mCheckBoxes[index];
    FilterItem*current_filter   = mFilters   [index];

    QString filter_label = current_filter->getLabel();
    current_checkbox->setToolTip((witness ? QString("uncheck to disable filter '") : QString("check to enable filter '")) + filter_label + QString("'"));
//    current_checkbox->setToolTip(QString("(un)check to toggle filter '") + filter_label + QString("'"));
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

void FilterManager::addFilter()
{
    AddFilterDialog addFilterDialog;
    if(addFilterDialog.isValid())
    {
        std::string regexp = addFilterDialog.getFilterRegexp();
        bool enable = addFilterDialog.isEnabled();
        addFilter(regexp, enable);
    }
}

} // end namespace gui
} // end namespace graph_analysis
