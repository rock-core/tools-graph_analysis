
#include "FilterManager.hpp"
#include "ActionCommander.hpp"
#include "AddFilterDialog.hpp"

#include <QRectF>
#include <QMenu>
#include <QSizeF>
#include <QMessageBox>
#include <QInputDialog>
#include <base/Logging.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

namespace graph_analysis {
namespace gui {

FilterManager::FilterManager(ViewWidget *viewWidget, LayerWidget *layerWidget, QWidget *checkBoxGrid, QWidget *parent)
    : QGraphicsView(parent)
    , mpViewWidget(viewWidget)
    , mpLayerWidget(layerWidget)
    , mpCheckBoxGrid(checkBoxGrid)
    , mItemSelected(false)
{
    QGraphicsScene *custom_scene = new QGraphicsScene(this);
    custom_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(custom_scene);
    setToolTip(QString("Custom Filters Manager Box"));
    setCacheMode(CacheBackground);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setMinimumWidth(110);

    // setting up the context menu
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showContextMenu(const QPoint &)));

    // example filters
    addFilter("filter1");
    addFilter("filter2", true);
    addFilter("filter3");
    addFilter("filter4", true);
    addFilter("filter5");
    addFilter("filter6");
    addFilter("filter7", true);
    addFilter("filter8");
    addFilter("filter9");
}

FilterManager::~FilterManager()
{
}

void FilterManager::showContextMenu(const QPoint& pos)
{
    ActionCommander comm(this);
    QMenu contextMenu(tr("Context menu"), this);

    QAction *actionAddFilter = comm.addAction("Add Regexp Filter", SLOT(addFilter()), *(mpViewWidget->getIcon("addFeature")));
    QAction *actionRenameFilter = comm.addAction("Rename one Filter", SLOT(renameFilter()), *(mpViewWidget->getIcon("featureLabel")));
    QAction *actionRenameSelectedFilter = comm.addAction("Rename Selected Filter", SLOT(renameSelectedFilter()), *(mpViewWidget->getIcon("featureLabel")));
    QAction *actionRemoveFilter = comm.addAction("Remove one Filter", SLOT(removeFilter()), *(mpViewWidget->getIcon("remove")));
    QAction *actionRemoveSelectedFilter = comm.addAction("Remove Selected Filter", SLOT(removeSelectedFilter()), *(mpViewWidget->getIcon("remove")));
    QAction *actionRemoveFilters = comm.addAction("Remove All Filters", SLOT(removeFilters()), *(mpViewWidget->getIcon("removeAll")));

    if(mItemSelected)
    {
        contextMenu.addAction(actionRenameSelectedFilter);
        contextMenu.addAction(actionRemoveSelectedFilter);
        contextMenu.addSeparator();
    }

    contextMenu.addAction(actionAddFilter);
    contextMenu.addAction(actionRenameFilter);
    contextMenu.addAction(actionRemoveFilter);
    contextMenu.addAction(actionRemoveFilters);

    contextMenu.exec(mapToGlobal(pos));
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

void FilterManager::renameFilter(FilterItem::filter_index_t index, QString regexp)
{
    dieOnIndex(index, "renameFilter");
    mFilters[index]->setLabel(regexp);
}

void FilterManager::renameFilter(FilterItem *item, QString regexp)
{
    if(!item)
    {
        LOG_ERROR_S << "graph_analysis::gui::FilterManager::renameFilter: cannot rename regexp filter - an invalid (null) filter pointer was provided";
        QMessageBox::critical(this, tr("Cannot Rename Regexp Filter"), tr("An invalid (null) filter pointer was provided!"));
        return;
    }
    item->setLabel(regexp);
}

void FilterManager::removeFilter(FilterItem::filter_index_t index)
{
    dieOnIndex(index, "removeFilter");
    removeFilter(mFilters[index]);
}

void FilterManager::removeFilter(FilterItem *item)
{
    if(!item)
    {
        LOG_ERROR_S << "graph_analysis::gui::FilterManager::removeFilter: cannot remove regexp filter - an invalid (null) filter pointer was provided";
        QMessageBox::critical(this, tr("Cannot Remove Regexp Filter"), tr("An invalid (null) filter pointer was provided!"));
        return;
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

void FilterManager::removeFilters()
{
    if(!mFilters.size())
    {
        LOG_WARN_S << "graph_analysis::gui::FilterManager::removeFilters: cannot remove regexp filters - there are no filters to remove";
        QMessageBox::critical(this, tr("Cannot Remove Regexp Filters"), tr("The custom regexp filters manager holds no filter whatsoever!"));
        return;
    }

    QMessageBox::StandardButton button = QMessageBox::question(this, tr("Confirm Deletion of All Regexp Filters"),
                                            tr("All Custom Regexp Filters in the Filters Manager will be completely erased! Are you sure you want to continue?"),
                                            QMessageBox::Yes | QMessageBox::No
                                        );
    switch(button)
    {
        case QMessageBox::Yes:
            // resetting will be performed
        break;

        default:
            return;
        break;
    }

    foreach(FilterItem *item, mFilters)
    {
        if(item)
        {
            delete item;
        }
    }

    foreach(QCheckBox *checkBox, mCheckBoxes)
    {

        if(checkBox)
        {
            delete checkBox;
        }
    }

    mFilters.clear();
    mCheckBoxes.clear();
    mCheckBoxIndexMap.clear();
}

void FilterManager::renameFilter()
{

}

void FilterManager::removeFilter()
{

}

void FilterManager::renameSelectedFilter()
{
    if(!mItemSelected)
    {
        LOG_WARN_S << "graph_analysis::gui::FilterManager::renameSelectedFilter: cannot rename selected regexp filter - no filter is selected";
        QMessageBox::critical(this, tr("Cannot Rename Selected Regexp Filter"), tr("No custom regexp filter is selected!"));
        return;
    }

    bool ok;
    QString label = QInputDialog::getText(this, tr("Input Filter Regexp"),
                                         tr("New Regexp:"), QLineEdit::Normal,
                                         QString(mpSelectedItem->getLabel()), &ok);
    if (ok && !label.isEmpty())
    {
        renameFilter(mpSelectedItem, label);
    }
}

void FilterManager::removeSelectedFilter()
{
    removeFilter(mpSelectedItem);
}

} // end namespace gui
} // end namespace graph_analysis
