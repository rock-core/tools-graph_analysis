#include "PropertyDialog.hpp"

#include <graph_analysis/gui/IconManager.hpp>
#include <graph_analysis/gui/GraphWidgetManager.hpp>
#include <graph_analysis/gui/WidgetManager.hpp>
#include <graph_analysis/gui/component_editor/ComponentEditorWidget.hpp>
#include <graph_analysis/gui/layer_view/LayerViewWidget.hpp>

#include <QScrollBar>

namespace graph_analysis {
namespace gui {
namespace dialogs {

PropertyDialog::PropertyDialog(bool dragDropIsChecked, bool vertexFocused, bool edgeFocused)
    : mVertexFocused(vertexFocused)
    , mEdgeFocused(edgeFocused)
{
    setupUi(&mDialog, dragDropIsChecked);
    // loads itself as dockable in the main window
    WidgetManager::getInstance()->getMainWindow()->addDockWidget(Qt::RightDockWidgetArea, &mDialog, Qt::Horizontal);
}

PropertyDialog::~PropertyDialog()
{
}

void PropertyDialog::addFrame(QVBoxLayout* verticalLayout)
{
    mFrames.push_back(new QFrame());
    mFrames.back()->setFrameShape(QFrame::HLine);
    verticalLayout->addWidget(mFrames.back());
}

void PropertyDialog::addFrame(QHBoxLayout* horizontalLayout)
{
    mFrames.push_back(new QFrame());
    mFrames.back()->setFrameShape(QFrame::VLine);
    horizontalLayout->addWidget(mFrames.back());
}

void PropertyDialog::setVertexFocused(bool vertexFocused)
{
    if(vertexFocused != mVertexFocused)
    {
        // enabling/disabling concerned focus-oriented node specific buttons
        mpRenameNodeButton -> setEnabled(vertexFocused);
        mpAddFeatureButton    -> setEnabled(vertexFocused);
        mpRenameFeatureButton -> setEnabled(vertexFocused);
        mpRemoveFeatureButton -> setEnabled(vertexFocused);
        mpRemoveFeaturesButton-> setEnabled(vertexFocused);
        mpSwapFeaturesButton  -> setEnabled(vertexFocused);
        mpRemoveNodeButton -> setEnabled(vertexFocused);
    }
    mVertexFocused = vertexFocused;
}

void PropertyDialog::setEdgeFocused(bool edgeFocused)
{
    if(edgeFocused != mEdgeFocused)
    {
        // enabling/disabling concerned focus-oriented edge specific buttons
        mpRenameEdgeButton->setEnabled(edgeFocused);
        mpRemoveEdgeButton->setEnabled(edgeFocused);
    }
    mEdgeFocused = edgeFocused;
}

void PropertyDialog::updateMainWidget(int index)
{
}

inline void PropertyDialog::updateStatus(const std::string& message, int timeout)
{
    WidgetManager::getInstance()->getGraphWidgetManager()->updateStatus(message, timeout);
}

void PropertyDialog::setupUi(CustomDialog *Dialog, bool dragDropIsChecked)
{
    int commonExtraPadding; // used to determine the least common extra space (and have it subtracted from all layouts heights in the end)
    int nframes = DEFAULT_NFRAMES; // known number of maximum frames in a single vertical layout
    int nbuttons = DEFAULT_NBUTTONS; // known number of maximum buttons in a single vertical layout
    int linePoints  = DEFAULT_LINE_POINTS; // known number of points per frame line height
    int buttonPoints  = DEFAULT_BUTTON_POINTS; // known number of points per button height
    mHeight = 31 + linePoints * nframes + buttonPoints * nbuttons; // total over-estimated panel height
    commonExtraPadding = mHeight;
    if (Dialog->objectName().isEmpty())
    {
        Dialog->setObjectName(QString::fromUtf8("Dialog"));
    }
    mpTabWidget = new QTabWidget(Dialog);
    mpHorizontalLayoutWidget = new QWidget();
    int currentIndexHorizontalLayoutWidget =  mpTabWidget->addTab(mpHorizontalLayoutWidget, *(IconManager::getInstance()->getIcon("properties")), "&Properties");
    mpHorizontalLayoutWidget->setObjectName(QString::fromUtf8("mpHorizontalLayoutWidget"));
    QString horizontalLayoutWidgetTip("Command Properties for the Graph View, Focused Node (if any) and Focused Edge (if any)");
    mpTabWidget->setTabToolTip(currentIndexHorizontalLayoutWidget, horizontalLayoutWidgetTip);
    mpTabWidget->setTabWhatsThis(currentIndexHorizontalLayoutWidget, horizontalLayoutWidgetTip);
    mpHorizontalLayoutWidget->setToolTip(horizontalLayoutWidgetTip);
    mpHorizontalLayout = new QHBoxLayout(mpHorizontalLayoutWidget);
    mpHorizontalLayout->setContentsMargins(5, 0, 5, 0);

    // main properties; left hand-side of the diagram view tab
    int verticalLayoutLeftover;
    {
        mpVerticalLayout = new QVBoxLayout();
        mpHorizontalLayout->addLayout(mpVerticalLayout);
        mpVerticalLayout->setObjectName(QString::fromUtf8("mpVerticalLayout"));
        mpVerticalLayout->setContentsMargins(0, PADDING, 0, 0);
//            addFrame(mpVerticalLayout);

        mpAddNodeButton = new QPushButton(mpHorizontalLayoutWidget);
        mpAddNodeButton->setObjectName(QString::fromUtf8("mpAddNodeButton"));
        mpAddNodeButton->setIcon(*(IconManager::getInstance()->getIcon("addNode")));
        mpAddNodeButton->setCheckable(false);
        mpAddNodeButton->setChecked(false);
        mpAddNodeButton->setToolTip(QString("adds a new node to the graph"));

        mpVerticalLayout->addWidget(mpAddNodeButton);
        addFrame(mpVerticalLayout);

        mpImportButton = new QPushButton(mpHorizontalLayoutWidget);
        mpImportButton->setObjectName(QString::fromUtf8("mpImportButton"));
        mpImportButton->setIcon(*(IconManager::getInstance()->getIcon("import")));
        mpImportButton->setCheckable(false);
        mpImportButton->setChecked(false);
        mpImportButton->setToolTip(QString("opens an existing graph file"));

        mpVerticalLayout->addWidget(mpImportButton);

        mpExportButton = new QPushButton(mpHorizontalLayoutWidget);
        mpExportButton->setObjectName(QString::fromUtf8("mpExportButton"));
        mpExportButton->setIcon(*(IconManager::getInstance()->getIcon("export")));
        mpExportButton->setCheckable(false);
        mpExportButton->setChecked(false);
        mpExportButton->setToolTip(QString("saves the graph to file"));

        mpVerticalLayout->addWidget(mpExportButton);
        addFrame(mpVerticalLayout);

        mpRefreshButton = new QPushButton(mpHorizontalLayoutWidget);
        mpRefreshButton->setObjectName(QString::fromUtf8("mpRefreshButton"));
        mpRefreshButton->setIcon(*(IconManager::getInstance()->getIcon("refresh")));
        mpRefreshButton->setCheckable(false);
        mpRefreshButton->setChecked(false);
        mpRefreshButton->setToolTip(QString("reloads the graph view"));

        mpVerticalLayout->addWidget(mpRefreshButton);

        mpShuffleButton = new QPushButton(mpHorizontalLayoutWidget);
        mpShuffleButton->setObjectName(QString::fromUtf8("mpShuffleButton"));
        mpShuffleButton->setIcon(*(IconManager::getInstance()->getIcon("shuffle")));
        mpShuffleButton->setCheckable(false);
        mpShuffleButton->setChecked(false);
        mpShuffleButton->setToolTip(QString("randomly spreads around existing vertices in this view"));

        mpVerticalLayout->addWidget(mpShuffleButton);

        mpResetButton = new QPushButton(mpHorizontalLayoutWidget);
        mpResetButton->setObjectName(QString::fromUtf8("mpResetButton"));
        mpResetButton->setIcon(*(IconManager::getInstance()->getIcon("reset")));
        mpResetButton->setCheckable(false);
        mpResetButton->setChecked(false);
        mpResetButton->setToolTip(QString("completely deletes the graph"));

        mpVerticalLayout->addWidget(mpResetButton);

        mpLayoutButton = new QPushButton(mpHorizontalLayoutWidget);
        mpLayoutButton->setObjectName(QString::fromUtf8("mpLayoutButton"));
        mpLayoutButton->setIcon(*(IconManager::getInstance()->getIcon("layout")));
        mpLayoutButton->setCheckable(false);
        mpLayoutButton->setChecked(false);
        mpLayoutButton->setToolTip(QString("applies a new layout to the graph"));

        mpVerticalLayout->addWidget(mpLayoutButton);
        addFrame(mpVerticalLayout);

        mpDragDropButton = new QPushButton(mpHorizontalLayoutWidget);
        mpDragDropButton->setObjectName(QString::fromUtf8("mpDragDropButton"));
        mpDragDropButton->setIcon(*(IconManager::getInstance()->getIcon("dragndrop")));
        mpDragDropButton->setCheckable(true);
        mpDragDropButton->setChecked(dragDropIsChecked);
        mpDragDropButton->setToolTip(QString("toggles the drag-n-drop mode"));

        mpVerticalLayout->addWidget(mpDragDropButton);

        mpMoveAroundButton = new QPushButton(mpHorizontalLayoutWidget);
        mpMoveAroundButton->setObjectName(QString::fromUtf8("mpMoveAroundButton"));
        mpMoveAroundButton->setIcon(*(IconManager::getInstance()->getIcon("move")));
        mpMoveAroundButton->setCheckable(true);
        mpMoveAroundButton->setChecked(!dragDropIsChecked);
        mpMoveAroundButton->setToolTip(QString("toggles the move-around mode"));

        mpVerticalLayout->addWidget(mpMoveAroundButton);

        // adding bottom padding
        verticalLayoutLeftover = 0 * linePoints + 1 * buttonPoints; // this vertical layout lacks 0 * frames and 1 buttons (out of the maximum no. of each)
        if(commonExtraPadding > verticalLayoutLeftover)
        {
            commonExtraPadding = verticalLayoutLeftover;
        }
    }

//        mpHorizontalLayout->addSpacing(5);
    addFrame(mpHorizontalLayout);
//        mpHorizontalLayout->addSpacing(5);

    // right hand-side in the diagram view tab; focus-based node and edge specific commands
    int verticalLayoutFocusLeftover;
    {
        // focus-based properties
        mpVerticalLayoutFocus = new QVBoxLayout();
        mpHorizontalLayout->addLayout(mpVerticalLayoutFocus);
        mpVerticalLayoutFocus->setObjectName(QString::fromUtf8("mpVerticalLayoutFocus"));
        mpVerticalLayoutFocus->setContentsMargins(0, PADDING, 0, 0);
//            addFrame(mpVerticalLayoutFocus);

        mpRenameNodeButton = new QPushButton(mpHorizontalLayoutWidget);
        mpRenameNodeButton->setObjectName(QString::fromUtf8("mpRenameNodeButton"));
        mpRenameNodeButton->setIcon(*(IconManager::getInstance()->getIcon("label")));
        mpRenameNodeButton->setEnabled(mVertexFocused);
        mpRenameNodeButton->setCheckable(false);
        mpRenameNodeButton->setChecked(false);
        mpRenameNodeButton->setToolTip(QString("changes the label of the selected vertex"));

        mpVerticalLayoutFocus->addWidget(mpRenameNodeButton);

        mpAddFeatureButton = new QPushButton(mpHorizontalLayoutWidget);
        mpAddFeatureButton->setObjectName(QString::fromUtf8("mpAddFeatureButton"));
        mpAddFeatureButton->setIcon(*(IconManager::getInstance()->getIcon("addFeature")));
        mpAddFeatureButton->setEnabled(mVertexFocused);
        mpAddFeatureButton->setCheckable(false);
        mpAddFeatureButton->setChecked(false);
        mpAddFeatureButton->setToolTip(QString("adds a feature to the selected vertex"));

        mpVerticalLayoutFocus->addWidget(mpAddFeatureButton);

        mpSwapFeaturesButton = new QPushButton(mpHorizontalLayoutWidget);
        mpSwapFeaturesButton->setObjectName(QString::fromUtf8("mpSwapFeaturesButton"));
        mpSwapFeaturesButton->setIcon(*(IconManager::getInstance()->getIcon("swap")));
        mpSwapFeaturesButton->setEnabled(mVertexFocused);
        mpSwapFeaturesButton->setCheckable(false);
        mpSwapFeaturesButton->setChecked(false);
        mpSwapFeaturesButton->setToolTip(QString("interchanges the positions of two features of the selected vertex"));

        mpVerticalLayoutFocus->addWidget(mpSwapFeaturesButton);

        mpRenameFeatureButton = new QPushButton(mpHorizontalLayoutWidget);
        mpRenameFeatureButton->setObjectName(QString::fromUtf8("mpRenameFeatureButton"));
        mpRenameFeatureButton->setIcon(*(IconManager::getInstance()->getIcon("featureLabel")));
        mpRenameFeatureButton->setEnabled(mVertexFocused);
        mpRenameFeatureButton->setCheckable(false);
        mpRenameFeatureButton->setChecked(false);
        mpRenameFeatureButton->setToolTip(QString("changes the label of a feature of the selected vertex"));

        mpVerticalLayoutFocus->addWidget(mpRenameFeatureButton);

        mpRemoveFeatureButton = new QPushButton(mpHorizontalLayoutWidget);
        mpRemoveFeatureButton->setObjectName(QString::fromUtf8("mpRemoveFeatureButton"));
        mpRemoveFeatureButton->setIcon(*(IconManager::getInstance()->getIcon("remove")));
        mpRemoveFeatureButton->setEnabled(mVertexFocused);
        mpRemoveFeatureButton->setCheckable(false);
        mpRemoveFeatureButton->setChecked(false);
        mpRemoveFeatureButton->setToolTip(QString("removes a feature of the selected vertex"));

        mpVerticalLayoutFocus->addWidget(mpRemoveFeatureButton);

        mpRemoveFeaturesButton = new QPushButton(mpHorizontalLayoutWidget);
        mpRemoveFeaturesButton->setObjectName(QString::fromUtf8("mpRemoveFeaturesButton"));
        mpRemoveFeaturesButton->setIcon(*(IconManager::getInstance()->getIcon("removeAll")));
        mpRemoveFeaturesButton->setEnabled(mVertexFocused);
        mpRemoveFeaturesButton->setCheckable(false);
        mpRemoveFeaturesButton->setChecked(false);
        mpRemoveFeaturesButton->setToolTip(QString("removes all features of the selected vertex"));

        mpVerticalLayoutFocus->addWidget(mpRemoveFeaturesButton);

        mpRemoveNodeButton = new QPushButton(mpHorizontalLayoutWidget);
        mpRemoveNodeButton->setObjectName(QString::fromUtf8("mpRemoveNodeButton"));
        mpRemoveNodeButton->setIcon(*(IconManager::getInstance()->getIcon("remove")));
        mpRemoveNodeButton->setEnabled(mVertexFocused);
        mpRemoveNodeButton->setCheckable(false);
        mpRemoveNodeButton->setChecked(false);
        mpRemoveNodeButton->setToolTip(QString("removes the selected vertex from the graph"));

        mpVerticalLayoutFocus->addWidget(mpRemoveNodeButton);

        addFrame(mpVerticalLayoutFocus);

        mpRenameEdgeButton = new QPushButton(mpHorizontalLayoutWidget);
        mpRenameEdgeButton->setObjectName(QString::fromUtf8("mpRenameEdgeButton"));
        mpRenameEdgeButton->setIcon(*(IconManager::getInstance()->getIcon("label")));
        mpRenameEdgeButton->setEnabled(mEdgeFocused);
        mpRenameEdgeButton->setCheckable(false);
        mpRenameEdgeButton->setChecked(false);
        mpRenameEdgeButton->setToolTip(QString("changes the label of the selected edge"));

        mpVerticalLayoutFocus->addWidget(mpRenameEdgeButton);

        mpRemoveEdgeButton = new QPushButton(mpHorizontalLayoutWidget);
        mpRemoveEdgeButton->setObjectName(QString::fromUtf8("mpRemoveEdgeButton"));
        mpRemoveEdgeButton->setIcon(*(IconManager::getInstance()->getIcon("remove")));
        mpRemoveEdgeButton->setEnabled(mEdgeFocused);
        mpRemoveEdgeButton->setCheckable(false);
        mpRemoveEdgeButton->setChecked(false);
        mpRemoveEdgeButton->setToolTip(QString("removes the selected edge from the graph"));

        mpVerticalLayoutFocus->addWidget(mpRemoveEdgeButton);

        // adding bottom padding
        verticalLayoutFocusLeftover = 3 * linePoints + 1 * buttonPoints; // this vertical layout lacks 3 * frames and 1 buttons (out of the maximum no. of each)
        if(commonExtraPadding > verticalLayoutFocusLeftover)
        {
            commonExtraPadding = verticalLayoutFocusLeftover;
        }
    }

    mpHorizontalLayoutLayerViewWidget = new QWidget();
    int currentIndexHorizontalLayoutLayerViewWidget = mpTabWidget->addTab(mpHorizontalLayoutLayerViewWidget, *(IconManager::getInstance()->getIcon("layers")), "&Layers");
    mpHorizontalLayoutLayerViewWidget->setObjectName(QString::fromUtf8("mpHorizontalLayoutLayerViewWidget"));
    QString horizontalLayoutLayerViewWidgetTip("Command Properties for the Layers View");
    mpTabWidget->setTabToolTip(currentIndexHorizontalLayoutLayerViewWidget, horizontalLayoutLayerViewWidgetTip);
    mpTabWidget->setTabWhatsThis(currentIndexHorizontalLayoutLayerViewWidget, horizontalLayoutLayerViewWidgetTip);
    mpHorizontalLayoutLayerViewWidget->setToolTip(horizontalLayoutLayerViewWidgetTip);
    mpHorizontalLayoutLayer = new QHBoxLayout(mpHorizontalLayoutLayerViewWidget);
    mpHorizontalLayoutLayer->setContentsMargins(5, 0, 5, 0);
    mpVerticalLayoutLayer = new QVBoxLayout();
    mpHorizontalLayoutLayer->addLayout(mpVerticalLayoutLayer);
    mpVerticalLayoutLayer->setObjectName(QString::fromUtf8("mpVerticalLayoutLayer"));
    mpVerticalLayoutLayer->setContentsMargins(0, PADDING, 0, 0);

    // left hand-side in the layers view tab; general graph view specific available commands
    int verticalLayoutLayerLeftover;
    {

        mpImportLayerButton = new QPushButton(mpHorizontalLayoutLayerViewWidget);
        mpImportLayerButton->setObjectName(QString::fromUtf8("mpImportLayerButton"));
        mpImportLayerButton->setIcon(*(IconManager::getInstance()->getIcon("import")));
        mpImportLayerButton->setCheckable(false);
        mpImportLayerButton->setChecked(false);
        mpImportLayerButton->setToolTip(QString("opens an existing graph file"));

        mpVerticalLayoutLayer->addWidget(mpImportLayerButton);

        mpExportLayerButton = new QPushButton(mpHorizontalLayoutWidget);
        mpExportLayerButton->setObjectName(QString::fromUtf8("mpExportLayerButton"));
        mpExportLayerButton->setIcon(*(IconManager::getInstance()->getIcon("export")));
        mpExportLayerButton->setCheckable(false);
        mpExportLayerButton->setChecked(false);
        mpExportLayerButton->setToolTip(QString("saves the graph to file"));

        mpVerticalLayoutLayer->addWidget(mpExportLayerButton);
        addFrame(mpVerticalLayoutLayer);

        mpRefreshLayerButton = new QPushButton(mpHorizontalLayoutWidget);
        mpRefreshLayerButton->setObjectName(QString::fromUtf8("mpRefreshLayerButton"));
        mpRefreshLayerButton->setIcon(*(IconManager::getInstance()->getIcon("refresh")));
        mpRefreshLayerButton->setCheckable(false);
        mpRefreshLayerButton->setChecked(false);
        mpRefreshLayerButton->setToolTip(QString("reloads this graph view"));

        mpVerticalLayoutLayer->addWidget(mpRefreshLayerButton);

        mpShuffleLayerButton = new QPushButton(mpHorizontalLayoutWidget);
        mpShuffleLayerButton->setObjectName(QString::fromUtf8("mpShuffleLayerButton"));
        mpShuffleLayerButton->setIcon(*(IconManager::getInstance()->getIcon("shuffle")));
        mpShuffleLayerButton->setCheckable(false);
        mpShuffleLayerButton->setChecked(false);
        mpShuffleLayerButton->setToolTip(QString("randomly spreads around existing vertices"));

        mpVerticalLayoutLayer->addWidget(mpShuffleLayerButton);

        mpResetLayerButton = new QPushButton(mpHorizontalLayoutWidget);
        mpResetLayerButton->setObjectName(QString::fromUtf8("mpResetLayerButton"));
        mpResetLayerButton->setIcon(*(IconManager::getInstance()->getIcon("reset")));
        mpResetLayerButton->setCheckable(false);
        mpResetLayerButton->setChecked(false);
        mpResetLayerButton->setToolTip(QString("completely deletes the graph"));

        mpVerticalLayoutLayer->addWidget(mpResetLayerButton);

        mpLayoutLayerButton = new QPushButton(mpHorizontalLayoutWidget);
        mpLayoutLayerButton->setObjectName(QString::fromUtf8("mpLayoutLayerButton"));
        mpLayoutLayerButton->setIcon(*(IconManager::getInstance()->getIcon("layout")));
        mpLayoutLayerButton->setCheckable(false);
        mpLayoutLayerButton->setChecked(false);
        mpLayoutLayerButton->setToolTip(QString("applies a new layout to the graph"));

        mpVerticalLayoutLayer->addWidget(mpLayoutLayerButton);

        // adding bottom padding
        verticalLayoutLayerLeftover = 2 * linePoints + 4 * buttonPoints; // this vertical layout lacks 2 * frames and 4 buttons (out of the maximum no. of each)
        if(commonExtraPadding > verticalLayoutLayerLeftover)
        {
            commonExtraPadding = verticalLayoutLayerLeftover;
        }
    }

    addFrame(mpHorizontalLayoutLayer);

    mpVerticalLayoutComponentsLayer = new QVBoxLayout();
    mpHorizontalLayoutLayer->addLayout(mpVerticalLayoutComponentsLayer);
    mpVerticalLayoutComponentsLayer->setObjectName(QString::fromUtf8("mpVerticalLayoutComponentsLayer"));
    mpVerticalLayoutComponentsLayer->setContentsMargins(0, PADDING, 0, PADDING);

    // the 2 handles are to be needed throughout the rest of this method
    LayerViewWidget *layerWidget = WidgetManager::getInstance() -> getLayerViewWidget();
    ComponentEditorWidget  * viewWidget = WidgetManager::getInstance() -> getComponentEditorWidget();

    // right hand-side in the layers view tab; filtering specific commands
    int verticalLayoutComponentsLayerLeftover;
    {
        mpLayersFilters = new QWidget();
        mpLayersFiltersLayout = new QVBoxLayout();
        mpLayersFiltersLayout->setSpacing(5);
        mpLayersFiltersLayout->setContentsMargins(0., 0., 0., 0.);
        mpLayersFilters->setLayout(mpLayersFiltersLayout);
        mpVerticalLayoutComponentsLayer->addWidget(mpLayersFilters);

        mpLayersLabel = new QLabel(mpHorizontalLayoutLayerViewWidget);
        mpLayersLabel->setObjectName(QString::fromUtf8("mpLayersLabel"));

        mpLayersFiltersLayout->addWidget(mpLayersLabel);

        // adding bottom padding
        verticalLayoutComponentsLayerLeftover = 3 * linePoints + 9 * buttonPoints; // this vertical layout lacks 3 * frames and 9 buttons (out of the maximum no. of each)
        if(commonExtraPadding > verticalLayoutComponentsLayerLeftover)
        {
            commonExtraPadding = verticalLayoutComponentsLayerLeftover;
        }

        mpCustomFilters = new QWidget();
        mpCustomFiltersLayout = new QVBoxLayout();
        mpCustomFiltersLayout->setSpacing(5);
        mpCustomFiltersLayout->setContentsMargins(0., 0., 0., 0.);
        mpCustomFilters->setLayout(mpCustomFiltersLayout);
        mpVerticalLayoutComponentsLayer->addWidget(mpCustomFilters);

        mpFiltersLabel = new QLabel(mpHorizontalLayoutLayerViewWidget);
        mpFiltersLabel->setObjectName(QString::fromUtf8("mpFiltersLabel"));

        mpCustomFiltersLayout->addWidget(mpFiltersLabel);

        mpFiltersBox = new QWidget();
        mpFiltersBox->setToolTip(QString("Custom Filters Manager Box"));

        int checkBoxWidth = 45; // pre-estimated size needed by the check boxes grid

        mpCheckBoxArea = new QScrollArea(mpFiltersBox);
//        QScrollBar *checkBoxesHorizontalScrollBar = mpCheckBoxArea->verticalScrollBar();
        int checkBoxesHorizontalScrollBarHeight = 12; // checkBoxesHorizontalScrollBar->size().height() is surprisingly off (takes an extra frame on top)
        QSize size(186, 150 + checkBoxesHorizontalScrollBarHeight);
        mpCheckBoxArea->setGeometry(0, 0, checkBoxWidth, size.height() - checkBoxesHorizontalScrollBarHeight);

        mpFiltersBox->resize(size.width(), size.height());
        mpFiltersBox->setFixedSize(size.width(), size.height());

        mpCheckBoxArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        mpCheckBoxArea->setToolTip(QString("Custom Filters Enabling Panel"));
        mpCheckBoxColumn = new QWidget();
        mpCheckBoxArea->setWidget(mpCheckBoxColumn);
        mpCheckBoxArea->setMaximumWidth(checkBoxWidth);
        mpCheckBoxColumn->setToolTip(QString("Custom Filters Enabling Panel"));
        mpCheckBoxColumn->setGeometry(0, 0, 20, 0);

        mpFilterManager = new FilterManager(mpCheckBoxColumn, mpFiltersBox);
        mpFilterManager->setGeometry(checkBoxWidth, 0, size.width() - checkBoxWidth, size.height());
        mpFilterManager->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        // synchronizing the 2 scroll bars on the check-boxes grid and the scene
        QScrollBar *checkBoxesScrollBar     =  mpCheckBoxArea->verticalScrollBar();
        QScrollBar *filterManagerScrollBar  = mpFilterManager->verticalScrollBar();
        PropertyDialog::connect(checkBoxesScrollBar, SIGNAL(valueChanged(int)), filterManagerScrollBar, SLOT(setValue(int)));
        PropertyDialog::connect(filterManagerScrollBar, SIGNAL(valueChanged(int)), checkBoxesScrollBar, SLOT(setValue(int)));

        mpCustomFiltersLayout->addWidget(mpFiltersBox);

        mpFilterButtonsWidget = new QWidget();
        mpFilterButtonsWidgetLayout = new QHBoxLayout();
        mpFilterButtonsWidgetLayout->setSpacing(0);
        mpFilterButtonsWidgetLayout->setContentsMargins(0., 0., 0., 0.);
        mpFilterButtonsWidget->setLayout(mpFilterButtonsWidgetLayout);

        mpCustomFiltersLayout->addWidget(mpFilterButtonsWidget);

        mpAddFilterButton = new QPushButton(mpHorizontalLayoutWidget);
        mpAddFilterButton->setObjectName(QString::fromUtf8("mpAddFilterButton"));
        mpAddFilterButton->setIcon(*(IconManager::getInstance()->getIcon("addFeature")));
        mpAddFilterButton->setEnabled(true);
        mpAddFilterButton->setCheckable(false);
        mpAddFilterButton->setChecked(false);
        mpAddFilterButton->setToolTip(QString("adds a custom regexp filter to the filters manager"));

        mpFilterButtonsWidgetLayout->addWidget(mpAddFilterButton);

        mpRenameFilterButton = new QPushButton(mpHorizontalLayoutWidget);
        mpRenameFilterButton->setObjectName(QString::fromUtf8("mpRenameFilterButton"));
        mpRenameFilterButton->setIcon(*(IconManager::getInstance()->getIcon("featureLabel")));
        mpRenameFilterButton->setEnabled(true);
        mpRenameFilterButton->setCheckable(false);
        mpRenameFilterButton->setChecked(false);
        mpRenameFilterButton->setToolTip(QString("renames a custom regexp filter in the filters manager"));

        mpFilterButtonsWidgetLayout->addWidget(mpRenameFilterButton);

        mpSwapFiltersButton = new QPushButton(mpHorizontalLayoutWidget);
        mpSwapFiltersButton->setObjectName(QString::fromUtf8("mpSwapFiltersButton"));
        mpSwapFiltersButton->setIcon(*(IconManager::getInstance()->getIcon("swap")));
        mpSwapFiltersButton->setEnabled(true);
        mpSwapFiltersButton->setCheckable(false);
        mpSwapFiltersButton->setChecked(false);
        mpSwapFiltersButton->setToolTip(QString("swaps two custom regexp filters in the filters manager"));

        mpFilterButtonsWidgetLayout->addWidget(mpSwapFiltersButton);

        mpRemoveFilterButton = new QPushButton(mpHorizontalLayoutWidget);
        mpRemoveFilterButton->setObjectName(QString::fromUtf8("mpRemoveFilterButton"));
        mpRemoveFilterButton->setIcon(*(IconManager::getInstance()->getIcon("remove")));
        mpRemoveFilterButton->setEnabled(true);
        mpRemoveFilterButton->setCheckable(false);
        mpRemoveFilterButton->setChecked(false);
        mpRemoveFilterButton->setToolTip(QString("removes a custom regexp filter from the filters manager"));

        mpFilterButtonsWidgetLayout->addWidget(mpRemoveFilterButton);

        mpRemoveFiltersButton = new QPushButton(mpHorizontalLayoutWidget);
        mpRemoveFiltersButton->setObjectName(QString::fromUtf8("mpRemoveFiltersButton"));
        mpRemoveFiltersButton->setIcon(*(IconManager::getInstance()->getIcon("removeAll")));
        mpRemoveFiltersButton->setEnabled(true);
        mpRemoveFiltersButton->setCheckable(false);
        mpRemoveFiltersButton->setChecked(false);
        mpRemoveFiltersButton->setToolTip(QString("removes all custom regexp filters from the filters manager"));

        mpFilterButtonsWidgetLayout->addWidget(mpRemoveFiltersButton);
    }

    // filling in the gaps at the bottom of each vertical layout
    mpVerticalLayout->addSpacing(verticalLayoutLeftover - commonExtraPadding);
    mpVerticalLayoutFocus->addSpacing(verticalLayoutFocusLeftover - commonExtraPadding);
    mpVerticalLayoutLayer->addSpacing(verticalLayoutLayerLeftover - commonExtraPadding);
//    mpVerticalLayoutComponentsLayer->addSpacing(verticalLayoutComponentsLayerLeftover - commonExtraPadding);

    // setting the proper titles to all gui components
    retranslateUi(Dialog);

    // establishing signal-slot connections
    PropertyDialog::connect(mpRefreshButton, SIGNAL(clicked()), viewWidget, SLOT(refresh()));
    PropertyDialog::connect(mpRefreshLayerButton, SIGNAL(clicked()), layerWidget, SLOT(refresh()));
    PropertyDialog::connect(mpShuffleButton, SIGNAL(clicked()), viewWidget, SLOT(shuffle()));
    PropertyDialog::connect(mpShuffleLayerButton, SIGNAL(clicked()), layerWidget, SLOT(shuffle()));
    PropertyDialog::connect(mpImportButton, SIGNAL(clicked()), viewWidget, SLOT(importGraph()));
    PropertyDialog::connect(mpImportLayerButton, SIGNAL(clicked()), viewWidget, SLOT(importGraph()));
    PropertyDialog::connect(mpExportButton, SIGNAL(clicked()), viewWidget, SLOT(exportGraph()));
    PropertyDialog::connect(mpExportLayerButton, SIGNAL(clicked()), viewWidget, SLOT(exportGraph()));
    PropertyDialog::connect(mpResetButton,  SIGNAL(clicked()), viewWidget, SLOT(resetGraph()));
    PropertyDialog::connect(mpResetLayerButton,  SIGNAL(clicked()), viewWidget, SLOT(resetGraph()));
    PropertyDialog::connect(mpLayoutButton,  SIGNAL(clicked()), viewWidget, SLOT(changeLayout()));
    PropertyDialog::connect(mpLayoutLayerButton,  SIGNAL(clicked()), layerWidget, SLOT(changeLayout()));
    PropertyDialog::connect(mpAddNodeButton, SIGNAL(clicked()), viewWidget, SLOT(addNodeAdhoc()));
    PropertyDialog::connect(mpDragDropButton, SIGNAL(toggled(bool)), viewWidget, SLOT(updateDragDrop(bool)));
    PropertyDialog::connect(mpMoveAroundButton, SIGNAL(toggled(bool)), viewWidget, SLOT(updateMoveAround(bool)));
    PropertyDialog::connect(mpRenameNodeButton, SIGNAL(clicked()), viewWidget, SLOT(changeSelectedVertexLabel()));
    PropertyDialog::connect(mpAddFeatureButton, SIGNAL(clicked()), viewWidget, SLOT(addFeatureFocused()));
    PropertyDialog::connect(mpSwapFeaturesButton, SIGNAL(clicked()), viewWidget, SLOT(swapFeaturesFocused()));
    PropertyDialog::connect(mpRenameFeatureButton, SIGNAL(clicked()), viewWidget, SLOT(renameFeatureFocused()));
    PropertyDialog::connect(mpRemoveFeatureButton, SIGNAL(clicked()), viewWidget, SLOT(removeFeatureFocused()));
    PropertyDialog::connect(mpRemoveFeaturesButton, SIGNAL(clicked()), viewWidget, SLOT(removeFeaturesFocused()));
    PropertyDialog::connect(mpRemoveNodeButton, SIGNAL(clicked()), viewWidget, SLOT(removeFocusedVertex()));
    PropertyDialog::connect(mpRenameEdgeButton, SIGNAL(clicked()), viewWidget, SLOT(changeSelectedEdgeLabel()));
    PropertyDialog::connect(mpRemoveEdgeButton, SIGNAL(clicked()), viewWidget, SLOT(removeSelectedEdge()));
    PropertyDialog::connect(mpToggleFeatureLayerButton, SIGNAL(toggled(bool)), layerWidget, SLOT(toggleFeatureLayer(bool)));
    PropertyDialog::connect(mpToggleClusterLayerButton, SIGNAL(toggled(bool)), layerWidget, SLOT(toggleClusterLayer(bool)));
    PropertyDialog::connect(mpTabWidget, SIGNAL(currentChanged(int)), this, SLOT(updateMainWidget(int)));
    PropertyDialog::connect(mpAddFilterButton, SIGNAL(clicked()), mpFilterManager, SLOT(addFilter()));
    PropertyDialog::connect(mpRenameFilterButton, SIGNAL(clicked()), mpFilterManager, SLOT(renameFilter()));
    PropertyDialog::connect(mpSwapFiltersButton, SIGNAL(clicked()), mpFilterManager, SLOT(swapFilters()));
    PropertyDialog::connect(mpRemoveFilterButton, SIGNAL(clicked()), mpFilterManager, SLOT(removeFilter()));
    PropertyDialog::connect(mpRemoveFiltersButton, SIGNAL(clicked()), mpFilterManager, SLOT(removeFilters()));

    QMetaObject::connectSlotsByName(Dialog);

    // final re-sizing
    Dialog->setFixedSize(WIDTH + 20, 45 + mHeight - commonExtraPadding);
    mpTabWidget->setGeometry(QRect(5, 30, WIDTH + 10, 10 + mHeight - commonExtraPadding));

    // TODO: switch properties with panel if needed
    //mpTabWidget->setCurrentIndex(WidgetManager::getInstance()->getStackedWidget()->currentIndex());

    mpHorizontalLayoutWidget->setGeometry(QRect(0, 0, WIDTH, mHeight - commonExtraPadding));
    mpHorizontalLayoutLayerViewWidget->setGeometry(QRect(0, 0, WIDTH / 2, mHeight - commonExtraPadding));
} // setupUi

void PropertyDialog::retranslateUi(CustomDialog *Dialog)
{
    Dialog->setWindowTitle(QApplication::translate("Dialog", " Command Panel", 0, QApplication::UnicodeUTF8));
    mpAddNodeButton->setText(QApplication::translate("Dialog", "Add Node", 0, QApplication::UnicodeUTF8));
    mpRefreshButton->setText(QApplication::translate("Dialog", "Refresh", 0, QApplication::UnicodeUTF8));
    mpRefreshLayerButton->setText(QApplication::translate("Dialog", "Refresh", 0, QApplication::UnicodeUTF8));
    mpShuffleButton->setText(QApplication::translate("Dialog", "Shuffle", 0, QApplication::UnicodeUTF8));
    mpShuffleLayerButton->setText(QApplication::translate("Dialog", "Shuffle", 0, QApplication::UnicodeUTF8));
    mpImportButton->setText(QApplication::translate("Dialog", "Import", 0, QApplication::UnicodeUTF8));
    mpImportLayerButton->setText(QApplication::translate("Dialog", "Import", 0, QApplication::UnicodeUTF8));
    mpExportButton->setText(QApplication::translate("Dialog", "Export", 0, QApplication::UnicodeUTF8));
    mpExportLayerButton->setText(QApplication::translate("Dialog", "Export", 0, QApplication::UnicodeUTF8));
    mpResetButton->setText(QApplication::translate("Dialog", "Reset", 0, QApplication::UnicodeUTF8));
    mpResetLayerButton->setText(QApplication::translate("Dialog", "Reset", 0, QApplication::UnicodeUTF8));
    mpLayoutButton->setText(QApplication::translate("Dialog", "Layout", 0, QApplication::UnicodeUTF8));
    mpLayoutLayerButton->setText(QApplication::translate("Dialog", "Layout", 0, QApplication::UnicodeUTF8));
    mpDragDropButton->setText(QApplication::translate("Dialog", "Drag'n'Drop", 0, QApplication::UnicodeUTF8));
    mpMoveAroundButton->setText(QApplication::translate("Dialog", "Move-around", 0, QApplication::UnicodeUTF8));
    mpRenameNodeButton->setText(QApplication::translate("Dialog", "Rename Node", 0, QApplication::UnicodeUTF8));
    mpAddFeatureButton->setText(QApplication::translate("Dialog", "Add Feature", 0, QApplication::UnicodeUTF8));
    mpRenameFeatureButton->setText(QApplication::translate("Dialog", "Rename a Feature", 0, QApplication::UnicodeUTF8));
    mpRemoveFeatureButton->setText(QApplication::translate("Dialog", "Remove a Feature", 0, QApplication::UnicodeUTF8));
    mpSwapFeaturesButton->setText(QApplication::translate("Dialog", "Swap Features", 0, QApplication::UnicodeUTF8));
    mpRemoveFeaturesButton->setText(QApplication::translate("Dialog", "Remove Features", 0, QApplication::UnicodeUTF8));
    mpRemoveNodeButton->setText(QApplication::translate("Dialog", "Remove Node", 0, QApplication::UnicodeUTF8));
    mpRenameEdgeButton->setText(QApplication::translate("Dialog", "Rename Edge", 0, QApplication::UnicodeUTF8));
    mpRemoveEdgeButton->setText(QApplication::translate("Dialog", "Remove Edge", 0, QApplication::UnicodeUTF8));
    mpLayersLabel->setText(QApplication::translate("Dialog", "Visible Layers:", 0, QApplication::UnicodeUTF8));
    mpFiltersLabel->setText(QApplication::translate("Dialog", "Custom Filters:", 0, QApplication::UnicodeUTF8));
    mpToggleFeatureLayerButton->setText(QApplication::translate("Dialog", "Features Layer", 0, QApplication::UnicodeUTF8));
    mpToggleClusterLayerButton->setText(QApplication::translate("Dialog", "Clusters Layer", 0, QApplication::UnicodeUTF8));
} // retranslateUi

} // end namespace dialogs
} // end namespace gui
} // end namespace graph_analysis

