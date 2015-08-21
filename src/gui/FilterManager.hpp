#ifndef FILTERMANAGER_HPP
#define FILTERMANAGER_HPP

#include <vector>
#include <string>
#include <QCheckBox>
#include <QGraphicsView>

#include "FilterItem.hpp"
#include "ViewWidget.hpp"
#include "LayerWidget.hpp"

namespace graph_analysis {
namespace gui {

class FilterManager : public QGraphicsView
{
    Q_OBJECT
public:
    typedef std::vector<FilterItem*> Filters;
    typedef std::vector<QCheckBox*>  CheckBoxes;
    typedef std::map<QCheckBox*, FilterItem::filter_index_t> CheckBoxIndexMap;

    FilterManager(ViewWidget *viewWidget, LayerWidget *layerWidget, QWidget *checkBoxGrid, QWidget *parent = 0);
    ~FilterManager();

    bool getItemSelected(void) { return mItemSelected; }
    void setItemSelected(bool itemSelected) { mItemSelected = itemSelected; }

    FilterItem * getSelectedItem(void) { return mpSelectedItem; }
    void setSelectedItem(FilterItem * selectedItem) { mpSelectedItem = selectedItem; }

    int filterCount() { return mFilters.size(); }
    void addFilter(const std::string& label, bool enable);
    void addFilter(const std::string& label);
    unsigned int createFilter(const std::string& label);
    void swapFilters(FilterItem::filter_index_t left, FilterItem::filter_index_t right);
    void dieOnIndex(FilterItem::filter_index_t index, const std::string& caller = std::string());
    void updateToolTip(FilterItem::filter_index_t index, bool witness, bool refresh = true);
    void refreshToolTip(FilterItem::filter_index_t index, bool refresh = false);
    void renameFilter(FilterItem::filter_index_t index, QString regexp);
    void renameFilter(FilterItem *item, QString regexp);
    void removeFilter(FilterItem::filter_index_t index);
    void removeFilter(FilterItem *item);

public slots:
    void addFilter();
    void swapFilters();
    void renameFilter();
    void removeFilter();
    void removeFilters();
    void renameSelectedFilter();
    void removeSelectedFilter();
    void updateToolTip(int state);

    /// displays context menu (upon a right click on the scene)
    void showContextMenu(const QPoint& pos);

private:
    Filters mFilters;
    ViewWidget *mpViewWidget;
    LayerWidget *mpLayerWidget;
    QWidget *mpCheckBoxGrid;
    CheckBoxes mCheckBoxes;
    CheckBoxIndexMap mCheckBoxIndexMap;
    FilterItem *mpSelectedItem;
    bool mItemSelected;
};

} // end namespace gui
} // end namespace graph_analysis

#endif // FILTERMANAGER_HPP
