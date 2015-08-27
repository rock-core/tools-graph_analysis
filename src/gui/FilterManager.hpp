#ifndef FILTERMANAGER_HPP
#define FILTERMANAGER_HPP

#include <vector>
#include <string>
#include <QCheckBox>
#include <QGraphicsView>

#include "FilterItem.hpp"
#include "LayerViewWidget.hpp"
#include "ComponentEditorWidget.hpp"

namespace graph_analysis {
namespace gui {

/**
 * \file FilterManager.hpp
 * \class FilterManager
 * \brief widget to manage all graphical custom regexp filter items in a common scene under behavioral restrictions
 * \details the item can only move vertically dropping in a snap-to-grid fashion and displace/push each other when
 *      they overlap too much (beyond a threshold)
 */
class FilterManager : public QGraphicsView
{
    Q_OBJECT
public:
    /// container datatype for an array of filter items
    typedef std::vector<FilterItem*> Filters;
    /// container datatype for an array of checkboxes handles
    typedef std::vector<QCheckBox*>  CheckBoxes;
    /// container datatype for mapping each registered check box to its registration index (i.e. offset in the array of checkboxes)
    typedef std::map<QCheckBox*, FilterItem::filter_index_t> CheckBoxIndexMap;

    /**
     * \brief constructor
     * \param checkBoxGrid widget to host the check-boxes at this widget's disposal
     * \param parent optional qt parent widget to assume ownership over this widget
     */
    FilterManager(QWidget *checkBoxGrid, QWidget *parent = 0);
    /// destructor
    ~FilterManager();

    /// getter method for the boolean item-selection witness; returns true when a filter item is being hover-over; false otherwise
    bool getItemSelected(void) { return mItemSelected; }
    /// setter method for the boolean item-selection witness;
    void setItemSelected(bool itemSelected) { mItemSelected = itemSelected; }

    /// getter method for the currently selected filter item (i.e. currently being hovered over)
    FilterItem* getSelectedItem(void) { return mpSelectedItem; }
    /// setter method for the currently selected filter item (i.e. currently being hovered over)
    void setSelectedItem(FilterItem *selectedItem) { mpSelectedItem = selectedItem; }

    /// returns the number of filters currently managed by the filter manager
    int filterCount() { return mFilters.size(); }

    /**
     * \brief introduces a new filter item
     * \param label the regex text of the filter
     * \param enable the state of the corresponding checkbox on init (checked/unchecked)
     */
    void addFilter(const std::string& label, bool enable);
    /// introduces a new filter item with the given 'label' as text regex
    void addFilter(const std::string& label);
    /// creates a new filter item with the given 'label' as text regex and its corresponding checkbox
    unsigned int createFilter(const std::string& label);

    /// swaps in-place the 2 filters of the given indices; NOTE: only left gets displaced (right is assumed to still be dragged by the mouse after the swapping)
    void swapFilters(FilterItem::filter_index_t left, FilterItem::filter_index_t right);
    /// error checking method: tests for the 'caller' whether the provided filter 'index' is out of bounds
    void dieOnIndex(FilterItem::filter_index_t index, const std::string& caller = std::string());
    /**
     * \brief synchronizes the GUI toolTip-s with the current text regex information held at index 'index'
     * \param index the offset of the concerned filter in the filters array
     * \param witness the boolean value of the checkbox to update to
     * \param refresh boolean flag dictating whether to order a layers view refresh; it generally does so
     */
    void updateToolTip(FilterItem::filter_index_t index, bool witness, bool refresh = true);
    /**
     * \brief synchronizes the GUI toolTip-s with the current text regex information held at index 'index'
     * \param index the offset of the concerned filter in the filters array
     * \param refresh boolean flag dictating whether to order a layers view refresh; it generally won't refresh
     */
    void refreshToolTip(FilterItem::filter_index_t index, bool refresh = false);
    /// changes regex of the filter at 'index' to 'regexp'
    void renameFilter(FilterItem::filter_index_t index, QString regexp);
    /// changes regex of the filter item 'filter' to 'regexp'
    void renameFilter(FilterItem *item, QString regexp);
    /// completely removes the filter item at 'index'
    void removeFilter(FilterItem::filter_index_t index);
    /// completely removes the given filter item
    void removeFilter(FilterItem *item);
    /// orders the layers view to refresh; when updatesStatusBar is unset, this takes place silently (the status bar won't get changed)
    void refreshView(bool updatesStatusBar = true);
    /// getter method for the array of filters
    Filters getFilters(void) { return mFilters; }

public slots:
    /// listener for the filter adding button in the property dialog
    void addFilter();
    /// listener for the filters swapping button in the property dialog
    void swapFilters();
    /// listener for the filter renaming button in the property dialog
    void renameFilter();
    /// listener for the filter removing button in the property dialog
    void removeFilter();
    /// listener for the all-filters removing button in the property dialog
    void removeFilters();
    /// listener for the selected filter renaming action in the context menu
    void renameSelectedFilter();
    /// listener for the selected filter removing action in the context menu
    void removeSelectedFilter();
    /// listener for the checkboxes user toggling
    void updateToolTip(int state);
    /// displays context menu (upon a right click in the scene)
    void showContextMenu(const QPoint& pos);

private:
    /// array of registered filters
    Filters mFilters;
    /// check boxes manager
    QWidget *mpCheckBoxGrid;
    /// array of checkboxes
    CheckBoxes mCheckBoxes;
    /// maps the checkboxes handles to their corresponding indices
    CheckBoxIndexMap mCheckBoxIndexMap;
    /// latest selected filter item
    FilterItem *mpSelectedItem;
    /// boolean filter-selected witness: true iff an item is currently hovered over
    bool mItemSelected;
};

} // end namespace gui
} // end namespace graph_analysis

#endif // FILTERMANAGER_HPP
