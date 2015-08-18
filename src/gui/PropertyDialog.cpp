
#include "ViewWidget.hpp"
#include "LayerWidget.hpp"
#include "PropertyDialog.hpp"

namespace graph_analysis {
namespace gui {

PropertyDialog::PropertyDialog(ViewWidget *viewWidget, LayerWidget *layerWidget, QMainWindow *mainWindow, QStackedWidget *stackedWidget, bool dragDropIsChecked, bool vertexFocused, bool edgeFocused)
: mpStackedWidget(stackedWidget)
, mpViewWidget(viewWidget)
, mpLayerWidget(layerWidget)
, mpMainWindow(mainWindow)
, mVertexFocused(vertexFocused)
, mEdgeFocused(edgeFocused)
{
    setupUi(&mDialog, dragDropIsChecked);
    // loads itself as dockable in the main window
    mpMainWindow->addDockWidget(Qt::RightDockWidgetArea, &mDialog, Qt::Horizontal);
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
    mpStackedWidget->setCurrentIndex(index);
}

inline void PropertyDialog::updateStatus(const std::string& message, int timeout)
{
    mpViewWidget->updateStatus(QString(message.c_str()), timeout);
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
    int currentIndexHorizontalLayoutWidget =  mpTabWidget->addTab(mpHorizontalLayoutWidget, *(mpViewWidget->getIcon("properties")), "&Properties");
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
        mpAddNodeButton->setIcon(*(mpViewWidget->getIcon("addNode")));
        mpAddNodeButton->setCheckable(false);
        mpAddNodeButton->setChecked(false);
        mpAddNodeButton->setToolTip(QString("adds a new node to the graph"));

        mpVerticalLayout->addWidget(mpAddNodeButton);
        addFrame(mpVerticalLayout);

        mpImportButton = new QPushButton(mpHorizontalLayoutWidget);
        mpImportButton->setObjectName(QString::fromUtf8("mpImportButton"));
        mpImportButton->setIcon(*(mpViewWidget->getIcon("import")));
        mpImportButton->setCheckable(false);
        mpImportButton->setChecked(false);
        mpImportButton->setToolTip(QString("opens an existing graph file"));

        mpVerticalLayout->addWidget(mpImportButton);

        mpExportButton = new QPushButton(mpHorizontalLayoutWidget);
        mpExportButton->setObjectName(QString::fromUtf8("mpExportButton"));
        mpExportButton->setIcon(*(mpViewWidget->getIcon("export")));
        mpExportButton->setCheckable(false);
        mpExportButton->setChecked(false);
        mpExportButton->setToolTip(QString("saves the graph to file"));

        mpVerticalLayout->addWidget(mpExportButton);
        addFrame(mpVerticalLayout);

        mpRefreshButton = new QPushButton(mpHorizontalLayoutWidget);
        mpRefreshButton->setObjectName(QString::fromUtf8("mpRefreshButton"));
        mpRefreshButton->setIcon(*(mpViewWidget->getIcon("refresh")));
        mpRefreshButton->setCheckable(false);
        mpRefreshButton->setChecked(false);
        mpRefreshButton->setToolTip(QString("reloads the graph view"));

        mpVerticalLayout->addWidget(mpRefreshButton);

        mpShuffleButton = new QPushButton(mpHorizontalLayoutWidget);
        mpShuffleButton->setObjectName(QString::fromUtf8("mpShuffleButton"));
        mpShuffleButton->setIcon(*(mpViewWidget->getIcon("shuffle")));
        mpShuffleButton->setCheckable(false);
        mpShuffleButton->setChecked(false);
        mpShuffleButton->setToolTip(QString("randomly spreads around existing vertices in this view"));

        mpVerticalLayout->addWidget(mpShuffleButton);

        mpResetButton = new QPushButton(mpHorizontalLayoutWidget);
        mpResetButton->setObjectName(QString::fromUtf8("mpResetButton"));
        mpResetButton->setIcon(*(mpViewWidget->getIcon("reset")));
        mpResetButton->setCheckable(false);
        mpResetButton->setChecked(false);
        mpResetButton->setToolTip(QString("completely deletes the graph"));

        mpVerticalLayout->addWidget(mpResetButton);

        mpLayoutButton = new QPushButton(mpHorizontalLayoutWidget);
        mpLayoutButton->setObjectName(QString::fromUtf8("mpLayoutButton"));
        mpLayoutButton->setIcon(*(mpViewWidget->getIcon("layout")));
        mpLayoutButton->setCheckable(false);
        mpLayoutButton->setChecked(false);
        mpLayoutButton->setToolTip(QString("applies a new layout to the graph"));

        mpVerticalLayout->addWidget(mpLayoutButton);
        addFrame(mpVerticalLayout);

        mpDragDropButton = new QPushButton(mpHorizontalLayoutWidget);
        mpDragDropButton->setObjectName(QString::fromUtf8("mpDragDropButton"));
        mpDragDropButton->setIcon(*(mpViewWidget->getIcon("dragndrop")));
        mpDragDropButton->setCheckable(true);
        mpDragDropButton->setChecked(dragDropIsChecked);
        mpDragDropButton->setToolTip(QString("toggles the drag-n-drop mode"));

        mpVerticalLayout->addWidget(mpDragDropButton);

        mpMoveAroundButton = new QPushButton(mpHorizontalLayoutWidget);
        mpMoveAroundButton->setObjectName(QString::fromUtf8("mpMoveAroundButton"));
        mpMoveAroundButton->setIcon(*(mpViewWidget->getIcon("move")));
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
        mpRenameNodeButton->setIcon(*(mpViewWidget->getIcon("label")));
        mpRenameNodeButton->setEnabled(mVertexFocused);
        mpRenameNodeButton->setCheckable(false);
        mpRenameNodeButton->setChecked(false);
        mpRenameNodeButton->setToolTip(QString("changes the label of the selected vertex"));

        mpVerticalLayoutFocus->addWidget(mpRenameNodeButton);

        mpAddFeatureButton = new QPushButton(mpHorizontalLayoutWidget);
        mpAddFeatureButton->setObjectName(QString::fromUtf8("mpAddFeatureButton"));
        mpAddFeatureButton->setIcon(*(mpViewWidget->getIcon("addFeature")));
        mpAddFeatureButton->setEnabled(mVertexFocused);
        mpAddFeatureButton->setCheckable(false);
        mpAddFeatureButton->setChecked(false);
        mpAddFeatureButton->setToolTip(QString("adds a feature to the selected vertex"));

        mpVerticalLayoutFocus->addWidget(mpAddFeatureButton);

        mpSwapFeaturesButton = new QPushButton(mpHorizontalLayoutWidget);
        mpSwapFeaturesButton->setObjectName(QString::fromUtf8("mpSwapFeaturesButton"));
        mpSwapFeaturesButton->setIcon(*(mpViewWidget->getIcon("swap")));
        mpSwapFeaturesButton->setEnabled(mVertexFocused);
        mpSwapFeaturesButton->setCheckable(false);
        mpSwapFeaturesButton->setChecked(false);
        mpSwapFeaturesButton->setToolTip(QString("interchanges the positions of two features of the selected vertex"));

        mpVerticalLayoutFocus->addWidget(mpSwapFeaturesButton);

        mpRenameFeatureButton = new QPushButton(mpHorizontalLayoutWidget);
        mpRenameFeatureButton->setObjectName(QString::fromUtf8("mpRenameFeatureButton"));
        mpRenameFeatureButton->setIcon(*(mpViewWidget->getIcon("featureLabel")));
        mpRenameFeatureButton->setEnabled(mVertexFocused);
        mpRenameFeatureButton->setCheckable(false);
        mpRenameFeatureButton->setChecked(false);
        mpRenameFeatureButton->setToolTip(QString("changes the label of a feature of the selected vertex"));

        mpVerticalLayoutFocus->addWidget(mpRenameFeatureButton);

        mpRemoveFeatureButton = new QPushButton(mpHorizontalLayoutWidget);
        mpRemoveFeatureButton->setObjectName(QString::fromUtf8("mpRemoveFeatureButton"));
        mpRemoveFeatureButton->setIcon(*(mpViewWidget->getIcon("remove")));
        mpRemoveFeatureButton->setEnabled(mVertexFocused);
        mpRemoveFeatureButton->setCheckable(false);
        mpRemoveFeatureButton->setChecked(false);
        mpRemoveFeatureButton->setToolTip(QString("removes a feature of the selected vertex"));

        mpVerticalLayoutFocus->addWidget(mpRemoveFeatureButton);

        mpRemoveFeaturesButton = new QPushButton(mpHorizontalLayoutWidget);
        mpRemoveFeaturesButton->setObjectName(QString::fromUtf8("mpRemoveFeaturesButton"));
        mpRemoveFeaturesButton->setIcon(*(mpViewWidget->getIcon("removeAll")));
        mpRemoveFeaturesButton->setEnabled(mVertexFocused);
        mpRemoveFeaturesButton->setCheckable(false);
        mpRemoveFeaturesButton->setChecked(false);
        mpRemoveFeaturesButton->setToolTip(QString("removes all features of the selected vertex"));

        mpVerticalLayoutFocus->addWidget(mpRemoveFeaturesButton);

        mpRemoveNodeButton = new QPushButton(mpHorizontalLayoutWidget);
        mpRemoveNodeButton->setObjectName(QString::fromUtf8("mpRemoveNodeButton"));
        mpRemoveNodeButton->setIcon(*(mpViewWidget->getIcon("remove")));
        mpRemoveNodeButton->setEnabled(mVertexFocused);
        mpRemoveNodeButton->setCheckable(false);
        mpRemoveNodeButton->setChecked(false);
        mpRemoveNodeButton->setToolTip(QString("removes the selected vertex from the graph"));

        mpVerticalLayoutFocus->addWidget(mpRemoveNodeButton);

        addFrame(mpVerticalLayoutFocus);

        mpRenameEdgeButton = new QPushButton(mpHorizontalLayoutWidget);
        mpRenameEdgeButton->setObjectName(QString::fromUtf8("mpRenameEdgeButton"));
        mpRenameEdgeButton->setIcon(*(mpViewWidget->getIcon("label")));
        mpRenameEdgeButton->setEnabled(mEdgeFocused);
        mpRenameEdgeButton->setCheckable(false);
        mpRenameEdgeButton->setChecked(false);
        mpRenameEdgeButton->setToolTip(QString("changes the label of the selected edge"));

        mpVerticalLayoutFocus->addWidget(mpRenameEdgeButton);

        mpRemoveEdgeButton = new QPushButton(mpHorizontalLayoutWidget);
        mpRemoveEdgeButton->setObjectName(QString::fromUtf8("mpRemoveEdgeButton"));
        mpRemoveEdgeButton->setIcon(*(mpViewWidget->getIcon("remove")));
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

    mpHorizontalLayoutLayerWidget = new QWidget();
    int currentIndexHorizontalLayoutLayerWidget = mpTabWidget->addTab(mpHorizontalLayoutLayerWidget, *(mpViewWidget->getIcon("layers")), "&Layers");
    mpHorizontalLayoutLayerWidget->setObjectName(QString::fromUtf8("mpHorizontalLayoutLayerWidget"));
    QString horizontalLayoutLayerWidgetTip("Command Properties for the Layers View");
    mpTabWidget->setTabToolTip(currentIndexHorizontalLayoutLayerWidget, horizontalLayoutLayerWidgetTip);
    mpTabWidget->setTabWhatsThis(currentIndexHorizontalLayoutLayerWidget, horizontalLayoutLayerWidgetTip);
    mpHorizontalLayoutLayerWidget->setToolTip(horizontalLayoutLayerWidgetTip);
    mpHorizontalLayoutLayer = new QHBoxLayout(mpHorizontalLayoutLayerWidget);
    mpHorizontalLayoutLayer->setContentsMargins(5, 0, 5, 0);
    mpVerticalLayoutLayer = new QVBoxLayout();
    mpHorizontalLayoutLayer->addLayout(mpVerticalLayoutLayer);
    mpVerticalLayoutLayer->setObjectName(QString::fromUtf8("mpVerticalLayoutLayer"));
    mpVerticalLayoutLayer->setContentsMargins(0, PADDING, 0, 0);

    // left hand-side in the layers view tab; general graph view specific available commands
    int verticalLayoutLayerLeftover;
    {

        mpImportLayerButton = new QPushButton(mpHorizontalLayoutLayerWidget);
        mpImportLayerButton->setObjectName(QString::fromUtf8("mpImportLayerButton"));
        mpImportLayerButton->setIcon(*(mpViewWidget->getIcon("import")));
        mpImportLayerButton->setCheckable(false);
        mpImportLayerButton->setChecked(false);
        mpImportLayerButton->setToolTip(QString("opens an existing graph file"));

        mpVerticalLayoutLayer->addWidget(mpImportLayerButton);

        mpExportLayerButton = new QPushButton(mpHorizontalLayoutWidget);
        mpExportLayerButton->setObjectName(QString::fromUtf8("mpExportLayerButton"));
        mpExportLayerButton->setIcon(*(mpViewWidget->getIcon("export")));
        mpExportLayerButton->setCheckable(false);
        mpExportLayerButton->setChecked(false);
        mpExportLayerButton->setToolTip(QString("saves the graph to file"));

        mpVerticalLayoutLayer->addWidget(mpExportLayerButton);
        addFrame(mpVerticalLayoutLayer);

        mpRefreshLayerButton = new QPushButton(mpHorizontalLayoutWidget);
        mpRefreshLayerButton->setObjectName(QString::fromUtf8("mpRefreshLayerButton"));
        mpRefreshLayerButton->setIcon(*(mpViewWidget->getIcon("refresh")));
        mpRefreshLayerButton->setCheckable(false);
        mpRefreshLayerButton->setChecked(false);
        mpRefreshLayerButton->setToolTip(QString("reloads this graph view"));

        mpVerticalLayoutLayer->addWidget(mpRefreshLayerButton);

        mpShuffleLayerButton = new QPushButton(mpHorizontalLayoutWidget);
        mpShuffleLayerButton->setObjectName(QString::fromUtf8("mpShuffleLayerButton"));
        mpShuffleLayerButton->setIcon(*(mpViewWidget->getIcon("shuffle")));
        mpShuffleLayerButton->setCheckable(false);
        mpShuffleLayerButton->setChecked(false);
        mpShuffleLayerButton->setToolTip(QString("randomly spreads around existing vertices"));

        mpVerticalLayoutLayer->addWidget(mpShuffleLayerButton);

        mpResetLayerButton = new QPushButton(mpHorizontalLayoutWidget);
        mpResetLayerButton->setObjectName(QString::fromUtf8("mpResetLayerButton"));
        mpResetLayerButton->setIcon(*(mpViewWidget->getIcon("reset")));
        mpResetLayerButton->setCheckable(false);
        mpResetLayerButton->setChecked(false);
        mpResetLayerButton->setToolTip(QString("completely deletes the graph"));

        mpVerticalLayoutLayer->addWidget(mpResetLayerButton);

        mpLayoutLayerButton = new QPushButton(mpHorizontalLayoutWidget);
        mpLayoutLayerButton->setObjectName(QString::fromUtf8("mpLayoutLayerButton"));
        mpLayoutLayerButton->setIcon(*(mpViewWidget->getIcon("layout")));
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

    // right hand-side in the layers view tab; filtering specific commands
    int verticalLayoutComponentsLayerLeftover;
    {
        mpLayersFilters = new QWidget();
        mpLayersFiltersLayout = new QVBoxLayout();
        mpLayersFiltersLayout->setSpacing(5);
        mpLayersFiltersLayout->setContentsMargins(0., 0., 0., 0.);
        mpLayersFilters->setLayout(mpLayersFiltersLayout);
        mpVerticalLayoutComponentsLayer->addWidget(mpLayersFilters);

        mpLayersLabel = new QLabel(mpHorizontalLayoutLayerWidget);
        mpLayersLabel->setObjectName(QString::fromUtf8("mpLayersLabel"));

        mpLayersFiltersLayout->addWidget(mpLayersLabel);

        mpToggleClusterLayerButton = new QPushButton(mpHorizontalLayoutLayerWidget);
        mpToggleClusterLayerButton->setObjectName(QString::fromUtf8("mpToggleClusterLayerButton"));
        mpToggleClusterLayerButton->setCheckable(true);
        mpToggleClusterLayerButton->setChecked(mpLayerWidget->getClusterLayerToggle());
        mpToggleClusterLayerButton->setToolTip(QString("toggles the visibility of the clusters layer"));

        mpLayersFiltersLayout->addWidget(mpToggleClusterLayerButton);

        mpToggleFeatureLayerButton = new QPushButton(mpHorizontalLayoutLayerWidget);
        mpToggleFeatureLayerButton->setObjectName(QString::fromUtf8("mpToggleFeatureLayerButton"));
        mpToggleFeatureLayerButton->setCheckable(true);
        mpToggleFeatureLayerButton->setChecked(mpLayerWidget->getFeatureLayerToggle());
        mpToggleFeatureLayerButton->setToolTip(QString("toggles the visibility of the features layer"));

        mpLayersFiltersLayout->addWidget(mpToggleFeatureLayerButton);

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

        mpFiltersLabel = new QLabel(mpHorizontalLayoutLayerWidget);
        mpFiltersLabel->setObjectName(QString::fromUtf8("mpFiltersLabel"));

        mpCustomFiltersLayout->addWidget(mpFiltersLabel);

        mpFiltersBox = new QWidget();
//        mpFiltersBox->setMinimumHeight(170);
        mpFiltersBoxLayout = new QHBoxLayout();
        mpFiltersBoxLayout->setSpacing(0);
        mpFiltersBoxLayout->setContentsMargins(0., 0., 0., 0.);
        mpFiltersBox->setLayout(mpFiltersBoxLayout);

        mpCheckBoxArea = new QScrollArea();
        mpCheckBoxColumn = new QWidget();
        mpCheckBoxArea->setWidget(mpCheckBoxColumn);

        mpCheckBoxColumnLayout = new QGridLayout(mpCheckBoxColumn);
        mpCheckBoxColumnLayout->setVerticalSpacing(0);
        mpCheckBoxColumnLayout->setContentsMargins(0., 0., 0., 0.);
        mpCheckBoxColumn->setLayout(mpCheckBoxColumnLayout);

        mpFiltersBoxLayout->addWidget(mpCheckBoxArea);

        mpFilterManager = new FilterManager(mpCheckBoxColumnLayout);
        mpFiltersBoxLayout->addWidget(mpFilterManager);

        mpCustomFiltersLayout->addWidget(mpFiltersBox);

        mpFilterButtonsWidget = new QWidget();
        mpFilterButtonsWidgetLayout = new QHBoxLayout();
        mpFilterButtonsWidgetLayout->setSpacing(0);
        mpFilterButtonsWidgetLayout->setContentsMargins(0., 0., 0., 0.);
        mpFilterButtonsWidget->setLayout(mpFilterButtonsWidgetLayout);

        mpCustomFiltersLayout->addWidget(mpFilterButtonsWidget);

        mpAddFilterButton = new QPushButton(mpHorizontalLayoutWidget);
        mpAddFilterButton->setObjectName(QString::fromUtf8("mpAddFilterButton"));
        mpAddFilterButton->setIcon(*(mpViewWidget->getIcon("addFeature")));
        mpAddFilterButton->setEnabled(true);
        mpAddFilterButton->setCheckable(false);
        mpAddFilterButton->setChecked(false);
        mpAddFilterButton->setToolTip(QString("adds a custom filter to the filters manager"));

        mpFilterButtonsWidgetLayout->addWidget(mpAddFilterButton);

        mpRenameFilterButton = new QPushButton(mpHorizontalLayoutWidget);
        mpRenameFilterButton->setObjectName(QString::fromUtf8("mpRenameFilterButton"));
        mpRenameFilterButton->setIcon(*(mpViewWidget->getIcon("featureLabel")));
        mpRenameFilterButton->setEnabled(true);
        mpRenameFilterButton->setCheckable(false);
        mpRenameFilterButton->setChecked(false);
        mpRenameFilterButton->setToolTip(QString("renames a custom filter in the filters manager"));

        mpFilterButtonsWidgetLayout->addWidget(mpRenameFilterButton);

        mpSwapFiltersButton = new QPushButton(mpHorizontalLayoutWidget);
        mpSwapFiltersButton->setObjectName(QString::fromUtf8("mpSwapFiltersButton"));
        mpSwapFiltersButton->setIcon(*(mpViewWidget->getIcon("swap")));
        mpSwapFiltersButton->setEnabled(true);
        mpSwapFiltersButton->setCheckable(false);
        mpSwapFiltersButton->setChecked(false);
        mpSwapFiltersButton->setToolTip(QString("swaps two custom filters in the filters manager"));

        mpFilterButtonsWidgetLayout->addWidget(mpSwapFiltersButton);

        mpRemoveFilterButton = new QPushButton(mpHorizontalLayoutWidget);
        mpRemoveFilterButton->setObjectName(QString::fromUtf8("mpRemoveFilterButton"));
        mpRemoveFilterButton->setIcon(*(mpViewWidget->getIcon("remove")));
        mpRemoveFilterButton->setEnabled(true);
        mpRemoveFilterButton->setCheckable(false);
        mpRemoveFilterButton->setChecked(false);
        mpRemoveFilterButton->setToolTip(QString("removes a custom filter from the filters manager"));

        mpFilterButtonsWidgetLayout->addWidget(mpRemoveFilterButton);

        mpRemoveFiltersButton = new QPushButton(mpHorizontalLayoutWidget);
        mpRemoveFiltersButton->setObjectName(QString::fromUtf8("mpRemoveFiltersButton"));
        mpRemoveFiltersButton->setIcon(*(mpViewWidget->getIcon("removeAll")));
        mpRemoveFiltersButton->setEnabled(true);
        mpRemoveFiltersButton->setCheckable(false);
        mpRemoveFiltersButton->setChecked(false);
        mpRemoveFiltersButton->setToolTip(QString("removess all custom filters from the filters manager"));

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
    PropertyDialog::connect(mpRefreshButton, SIGNAL(clicked()), mpViewWidget, SLOT(refresh()));
    PropertyDialog::connect(mpRefreshLayerButton, SIGNAL(clicked()), mpLayerWidget, SLOT(refresh()));
    PropertyDialog::connect(mpShuffleButton, SIGNAL(clicked()), mpViewWidget, SLOT(shuffle()));
    PropertyDialog::connect(mpShuffleLayerButton, SIGNAL(clicked()), mpLayerWidget, SLOT(shuffle()));
    PropertyDialog::connect(mpImportButton, SIGNAL(clicked()), mpViewWidget, SLOT(importGraph()));
    PropertyDialog::connect(mpImportLayerButton, SIGNAL(clicked()), mpViewWidget, SLOT(importGraph()));
    PropertyDialog::connect(mpExportButton, SIGNAL(clicked()), mpViewWidget, SLOT(exportGraph()));
    PropertyDialog::connect(mpExportLayerButton, SIGNAL(clicked()), mpViewWidget, SLOT(exportGraph()));
    PropertyDialog::connect(mpResetButton,  SIGNAL(clicked()), mpViewWidget, SLOT(resetGraph()));
    PropertyDialog::connect(mpResetLayerButton,  SIGNAL(clicked()), mpViewWidget, SLOT(resetGraph()));
    PropertyDialog::connect(mpLayoutButton,  SIGNAL(clicked()), mpViewWidget, SLOT(changeLayout()));
    PropertyDialog::connect(mpLayoutLayerButton,  SIGNAL(clicked()), mpLayerWidget, SLOT(changeLayout()));
    PropertyDialog::connect(mpAddNodeButton, SIGNAL(clicked()), mpViewWidget, SLOT(addNodeAdhoc()));
    PropertyDialog::connect(mpDragDropButton, SIGNAL(toggled(bool)), mpViewWidget, SLOT(updateDragDrop(bool)));
    PropertyDialog::connect(mpMoveAroundButton, SIGNAL(toggled(bool)), mpViewWidget, SLOT(updateMoveAround(bool)));
    PropertyDialog::connect(mpRenameNodeButton, SIGNAL(clicked()), mpViewWidget, SLOT(changeFocusedVertexLabel()));
    PropertyDialog::connect(mpAddFeatureButton, SIGNAL(clicked()), mpViewWidget, SLOT(addFeatureFocused()));
    PropertyDialog::connect(mpSwapFeaturesButton, SIGNAL(clicked()), mpViewWidget, SLOT(swapFeaturesFocused()));
    PropertyDialog::connect(mpRenameFeatureButton, SIGNAL(clicked()), mpViewWidget, SLOT(renameFeatureFocused()));
    PropertyDialog::connect(mpRemoveFeatureButton, SIGNAL(clicked()), mpViewWidget, SLOT(removeFeatureFocused()));
    PropertyDialog::connect(mpRemoveFeaturesButton, SIGNAL(clicked()), mpViewWidget, SLOT(removeFeaturesFocused()));
    PropertyDialog::connect(mpRemoveNodeButton, SIGNAL(clicked()), mpViewWidget, SLOT(removeFocusedVertex()));
    PropertyDialog::connect(mpRenameEdgeButton, SIGNAL(clicked()), mpViewWidget, SLOT(changeFocusedEdgeLabel()));
    PropertyDialog::connect(mpRemoveEdgeButton, SIGNAL(clicked()), mpViewWidget, SLOT(removeFocusedEdge()));
    PropertyDialog::connect(mpToggleFeatureLayerButton, SIGNAL(toggled(bool)), mpLayerWidget, SLOT(toggleFeatureLayer(bool)));
    PropertyDialog::connect(mpToggleClusterLayerButton, SIGNAL(toggled(bool)), mpLayerWidget, SLOT(toggleClusterLayer(bool)));
    PropertyDialog::connect(mpTabWidget, SIGNAL(currentChanged(int)), this, SLOT(updateMainWidget(int)));

    QMetaObject::connectSlotsByName(Dialog);

    // final re-sizing
    Dialog->setFixedSize(WIDTH + 20, 45 + mHeight - commonExtraPadding);
    mpTabWidget->setGeometry(QRect(5, 30, WIDTH + 10, 10 + mHeight - commonExtraPadding));
    mpTabWidget->setCurrentIndex(mpStackedWidget->currentIndex());
    mpHorizontalLayoutWidget->setGeometry(QRect(0, 0, WIDTH, mHeight - commonExtraPadding));
    mpHorizontalLayoutLayerWidget->setGeometry(QRect(0, 0, WIDTH / 2, mHeight - commonExtraPadding));
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

} // end namespace gui
} // end namespace graph_analysis

