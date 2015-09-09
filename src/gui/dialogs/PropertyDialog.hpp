/********************************************************************************
** Form generated from reading UI file 'property_dialog_gui.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef PROPERTYDIALOG_H
#define PROPERTYDIALOG_H

#include <graph_analysis/gui/dialogs/CustomDialog.hpp>
#include <graph_analysis/gui/FilterManager.hpp>

#include <vector>
#include <QLabel>
#include <QObject>
#include <QGridLayout>
#include <QCheckBox>
#include <QCloseEvent>
#include <QScrollArea>
#include <QMainWindow>
#include <QtGui/QAction>
#include <QtGui/QDialog>
#include <QtGui/QWidget>
#include <QtCore/QVariant>
#include <QtGui/QTabWidget>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QStackedWidget>

// constant panel raw width
#define WIDTH 306
// general border width (for several frames/layouts within the panel)
#define PADDING 7
// maximum no. of separators   in a vertical layout out of the total 4 such vertical layouts in the 2 tabs
#define DEFAULT_NFRAMES 3
// maximum no. of push-buttons in a vertical layout out of the total 4 such vertical layouts in the 2 tabs
#define DEFAULT_NBUTTONS 10
// approximated hard-wired height (in points) accounted for a single separator (a.k.a. degenerate frame) including the border area around
#define DEFAULT_LINE_POINTS 15
// approximated hard-wired height (in points) accounted for a push-button including the border area around
#define DEFAULT_BUTTON_POINTS 31

namespace graph_analysis {
namespace gui {
namespace dialogs {

/**
 * \file PropertyDialog.hpp
 * \class PropertyDialog
 * \brief dockable multi-tab command panel for the entire GUI
 * \details accommodates options/properties for both the diagram view and also the layered conceptual graph view
 */
class PropertyDialog : public QObject
{
    Q_OBJECT
public:
    /// keeps track of both vertical and horizontal separators in the scene
    typedef std::vector<QFrame *> QFrames;

    /**
     * \brief constructor
     * \param dragDropIsChecked current drag-n-drop mode boolean witness
     * \param vertexFocused boolean witness: true when there is a node currently focused on; false otherwise
     * \param edgeFocused boolean witness:  true when there is an edge currently focused on; false otherwise
     */
    PropertyDialog(bool dragDropIsChecked = false, bool vertexFocused = false, bool edgeFocused = false);
    /// destructor
    ~PropertyDialog();

    /// getter method for retrieving running status; returns true when the main panel is still in the scene; false otherwise
    bool isRunning() { return (&mDialog)->isVisible(); }
    /// setter method for drag-n-drop button boolean state - used by the viewWidget to sync the drag-n-drop mode with its button switch here in the command panel
    void setDragDrop(bool toggle)
    {
        mpDragDropButton->setChecked(toggle);
        mpMoveAroundButton->setChecked(!toggle);
    }
    /// releases the mpDragDropButton
    void uncheckDragDrop(bool toggle)
    {
        mpDragDropButton->setChecked(!toggle);
    }

    /// releases the mpMoveAroundButton
    void uncheckMoveAround(bool toggle)
    {
        mpMoveAroundButton->setChecked(!toggle);
    }

    /// adds a horizontal separator
    void addFrame(QVBoxLayout* verticalLayout);
    /// adds a vertical separator
    void addFrame(QHBoxLayout* horizontalLayout);

    /// sets up all GUI components to the provided Dialog instance
    void setupUi(CustomDialog *Dialog, bool dragDropIsChecked = false);
    /// attributes meaningful titles to be displayed for all GUI components (e.g. buttons labels)
    void retranslateUi(CustomDialog *Dialog);

    /// setter method for the node-being-focused boolean witness
    void setVertexFocused(bool vertexFocused);
    /// setter method for the edge-being-focused boolean witness
    void setEdgeFocused  (bool edgeFocused  );

    /// getter method for the node-being-focused boolean witness
    bool getVertexFocused() { return mVertexFocused; }
    /// getter method for the edge-being-focused boolean witness
    bool getEdgeFocused  () { return mEdgeFocused;   }

    /// updates the temporary text currently being displayed on the Status Bar in the main viewWidget
    inline void updateStatus(const std::string& message = std::string(), int timeout = 0);

    /// getter method for the filters manager member field
    FilterManager* getFilterManager(void) { return mpFilterManager; }

public slots:
    /// switches the central widget to the widget of the given index: 0 for the ComponentEditorWidget; 1 for the LayerViewWidget
    void updateMainWidget(int index);

private:
    /// main qt dialog to be used
    CustomDialog mDialog;
    /// collection of tabs of the entire panel
    QTabWidget *mpTabWidget;
    /// the diagram view tab
    QWidget *mpHorizontalLayoutWidget;
    /// the layers view tab
    QWidget *mpHorizontalLayoutLayerViewWidget;
    /// main layout of the diagram view tab
    QHBoxLayout *mpHorizontalLayout;
    /// main layout of the layers view tab
    QHBoxLayout *mpHorizontalLayoutLayer;
    /// left hand-side vertical layout of the diagram view tab (hosts the general graph commands)
    QVBoxLayout *mpVerticalLayout;
    /// right hand-side vertical layout of the diagram view tab (hosts the focus-oriented node and edge specific commands)
    QVBoxLayout *mpVerticalLayoutFocus;
    /// left hand-side vertical layout of the layers view tab (hosts the general graph commands available on the layers view)
    QVBoxLayout *mpVerticalLayoutLayer;
    /// right hand-side vertical layout of the layers view tab (hosts the (layers) filtering specific commands)
    QVBoxLayout *mpVerticalLayoutComponentsLayer;
    /// button for spawning a new node
    QPushButton *mpAddNodeButton;
    /// button for refreshing the graph view in the diagram view tab
    QPushButton *mpRefreshButton;
    /// button for refreshing the graph view in the layers view tab
    QPushButton *mpRefreshLayerButton;
    /// button for shuffling all the nodes in the graph view in the diagram view tab
    QPushButton *mpShuffleButton;
    /// button for shuffling all the nodes in the graph view in the layers view tab
    QPushButton *mpShuffleLayerButton;
    /// button for loading a graph from file from the diagram view tab
    QPushButton *mpImportButton;
    /// button for loading a graph from file from the layers view tab
    QPushButton *mpImportLayerButton;
    /// button for saving a graph to file from the diagram view tab
    QPushButton *mpExportButton;
    /// button for saving a graph to file from the layers view tab
    QPushButton *mpExportLayerButton;
    /// button for resetting the graph from the diagram view tab
    QPushButton *mpResetButton;
    /// button for resetting the graph to file from the layers view tab
    QPushButton *mpResetLayerButton;
    /// button for chaging the layout of the diagram graph view
    QPushButton *mpLayoutButton;
    /// button for chaging the layout of the layers graph view
    QPushButton *mpLayoutLayerButton;
    /// button for toggling the drag-n-drop mode in the diagram view tab
    QPushButton *mpDragDropButton;
    /// button for toggling the move-around mode in the diagram view tab
    QPushButton *mpMoveAroundButton;
    /// button for renaming the focused node in the diagram view tab
    QPushButton *mpRenameNodeButton;
    /// button for adding a feature to the focused node in the diagram view tab
    QPushButton *mpAddFeatureButton;
    /// button for renaming a feature of the focused node in the diagram view tab
    QPushButton *mpRenameFeatureButton;
    /// button for removing a feature of the focused node in the diagram view tab
    QPushButton *mpRemoveFeatureButton;
    /// button for removing all features of the focused node in the diagram view tab
    QPushButton *mpRemoveFeaturesButton;
    /// button for removing the focused node in the diagram view tab
    QPushButton *mpRemoveNodeButton;
    /// button for renaming the focused edge in the diagram view tab
    QPushButton *mpRenameEdgeButton;
    /// button for removing the focused edge in the diagram view tab
    QPushButton *mpRemoveEdgeButton;
    /// button for swapping two features of the focused node in the diagram view tab
    QPushButton *mpSwapFeaturesButton;
    /// widget for grouping together the type-based layer filtering components
    QWidget *mpLayersFilters;
    /// vertical layout for the mpLayersFilters
    QVBoxLayout *mpLayersFiltersLayout;
    /// widget for grouping together the custom label-based filtering components
    QWidget *mpCustomFilters;
    /// vertical layout for the mpCustomFilters
    QVBoxLayout *mpCustomFiltersLayout;
    /// GUI label displaying "Visible Layers:" in the layers view tab on top of the layer filtering commands
    QLabel *mpLayersLabel;
    /// button for toggling visibility of feature vertices in the layers view tab
    QPushButton *mpToggleFeatureLayerButton;
    /// button for toggling visibility of cluster vertices in the layers view tab
    QPushButton *mpToggleClusterLayerButton;
    /// GUI label displaying "Custom Filters:" in the layers view tab on top of the FilterItems widget and of filtering buttons bar
    QLabel *mpFiltersLabel;
    /// widget with FilterItems on the right and their corresponding checkboxes on the left
    QWidget *mpFiltersBox;
    /// scrollable frame to wrap the check-boxes widget
    QScrollArea *mpCheckBoxArea;
    /// the check boxes vertical panel
    QWidget *mpCheckBoxColumn;
    /// widget for the custom regexp filters view (holds the FilterItem-s)
    FilterManager *mpFilterManager;
    /// buttons bar for FilterItems
    QWidget *mpFilterButtonsWidget;
    /// horizontal layout for the filter buttons bar
    QHBoxLayout *mpFilterButtonsWidgetLayout;
    /// button for adding a custom regexp FilterItem to the filters view in the layers tab
    QPushButton *mpAddFilterButton;
    /// button for renaming a custom regexp FilterItem from the filters view in the layers tab
    QPushButton *mpRenameFilterButton;
    /// button for swapping two custom regexp FilterItems in the filters view in the layers tab
    QPushButton *mpSwapFiltersButton;
    /// button for removing a custom regexp FilterItem from the filters view in the layers tab
    QPushButton *mpRemoveFilterButton;
    /// button for removing all custom regexp FilterItems from the filters view in the layers tab
    QPushButton *mpRemoveFiltersButton;
    /// set of separators in all layouts used
    QFrames mFrames;
    /// boolean witness: true when a node is being focused on; false otherwise
    bool mVertexFocused;
    /// boolean witness: true when an edge is being focused on; false otherwise
    bool mEdgeFocused;
    /// current height of the panel in the scene
    int mHeight;
};

} // end namespace dialogs
} // end namespace gui
} // end namespace graph_analysis

#endif // PROPERTYDIALOG_H
