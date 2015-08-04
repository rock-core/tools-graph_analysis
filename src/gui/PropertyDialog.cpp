
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
        mpAddPortButton    -> setEnabled(vertexFocused);
        mpRenamePortButton -> setEnabled(vertexFocused);
        mpRemovePortButton -> setEnabled(vertexFocused);
        mpRemovePortsButton-> setEnabled(vertexFocused);
        mpSwapPortsButton  -> setEnabled(vertexFocused);
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

        mpAddPortButton = new QPushButton(mpHorizontalLayoutWidget);
        mpAddPortButton->setObjectName(QString::fromUtf8("mpAddPortButton"));
        mpAddPortButton->setIcon(*(mpViewWidget->getIcon("addPort")));
        mpAddPortButton->setEnabled(mVertexFocused);
        mpAddPortButton->setCheckable(false);
        mpAddPortButton->setChecked(false);
        mpAddPortButton->setToolTip(QString("adds a port to the selected vertex"));

        mpVerticalLayoutFocus->addWidget(mpAddPortButton);

        mpSwapPortsButton = new QPushButton(mpHorizontalLayoutWidget);
        mpSwapPortsButton->setObjectName(QString::fromUtf8("mpSwapPortsButton"));
        mpSwapPortsButton->setIcon(*(mpViewWidget->getIcon("swap")));
        mpSwapPortsButton->setEnabled(mVertexFocused);
        mpSwapPortsButton->setCheckable(false);
        mpSwapPortsButton->setChecked(false);
        mpSwapPortsButton->setToolTip(QString("interchanges the positions of two ports of the selected vertex"));

        mpVerticalLayoutFocus->addWidget(mpSwapPortsButton);

        mpRenamePortButton = new QPushButton(mpHorizontalLayoutWidget);
        mpRenamePortButton->setObjectName(QString::fromUtf8("mpRenamePortButton"));
        mpRenamePortButton->setIcon(*(mpViewWidget->getIcon("portLabel")));
        mpRenamePortButton->setEnabled(mVertexFocused);
        mpRenamePortButton->setCheckable(false);
        mpRenamePortButton->setChecked(false);
        mpRenamePortButton->setToolTip(QString("changes the label of a port of the selected vertex"));

        mpVerticalLayoutFocus->addWidget(mpRenamePortButton);

        mpRemovePortButton = new QPushButton(mpHorizontalLayoutWidget);
        mpRemovePortButton->setObjectName(QString::fromUtf8("mpRemovePortButton"));
        mpRemovePortButton->setIcon(*(mpViewWidget->getIcon("remove")));
        mpRemovePortButton->setEnabled(mVertexFocused);
        mpRemovePortButton->setCheckable(false);
        mpRemovePortButton->setChecked(false);
        mpRemovePortButton->setToolTip(QString("removes a port of the selected vertex"));

        mpVerticalLayoutFocus->addWidget(mpRemovePortButton);

        mpRemovePortsButton = new QPushButton(mpHorizontalLayoutWidget);
        mpRemovePortsButton->setObjectName(QString::fromUtf8("mpRemovePortsButton"));
        mpRemovePortsButton->setIcon(*(mpViewWidget->getIcon("removeAll")));
        mpRemovePortsButton->setEnabled(mVertexFocused);
        mpRemovePortsButton->setCheckable(false);
        mpRemovePortsButton->setChecked(false);
        mpRemovePortsButton->setToolTip(QString("removes all ports of the selected vertex"));

        mpVerticalLayoutFocus->addWidget(mpRemovePortsButton);

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
        verticalLayoutFocusLeftover = 3 * linePoints + 0 * buttonPoints; // this vertical layout lacks 3 * frames and 0 buttons (out of the maximum no. of each)
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
        verticalLayoutLayerLeftover = 2 * linePoints + 3 * buttonPoints; // this vertical layout lacks 2 * frames and 3 buttons (out of the maximum no. of each)
        if(commonExtraPadding > verticalLayoutLayerLeftover)
        {
            commonExtraPadding = verticalLayoutLayerLeftover;
        }
    }

    addFrame(mpHorizontalLayoutLayer);

    mpVerticalLayoutComponentsLayer = new QVBoxLayout();
    mpHorizontalLayoutLayer->addLayout(mpVerticalLayoutComponentsLayer);
    mpVerticalLayoutComponentsLayer->setObjectName(QString::fromUtf8("mpVerticalLayoutComponentsLayer"));
    mpVerticalLayoutComponentsLayer->setContentsMargins(0, PADDING, 0, 0);

    // right hand-side in the layers view tab; filtering specific commands
    int verticalLayoutComponentsLayerLeftover;
    {
        mpLayersLabel = new QLabel(mpHorizontalLayoutLayerWidget);
        mpLayersLabel->setObjectName(QString::fromUtf8("mpComponentsLabel"));

        mpVerticalLayoutComponentsLayer->addWidget(mpLayersLabel);

        mpToggleClusterLayerButton = new QPushButton(mpHorizontalLayoutLayerWidget);
        mpToggleClusterLayerButton->setObjectName(QString::fromUtf8("mpToggleClusterLayerButton"));
        mpToggleClusterLayerButton->setCheckable(true);
        mpToggleClusterLayerButton->setChecked(mpLayerWidget->getClusterLayerToggle());
        mpToggleClusterLayerButton->setToolTip(QString("toggles the visibility of the clusters layer"));

        mpVerticalLayoutComponentsLayer->addWidget(mpToggleClusterLayerButton);

        mpTogglePortLayerButton = new QPushButton(mpHorizontalLayoutLayerWidget);
        mpTogglePortLayerButton->setObjectName(QString::fromUtf8("mpTogglePortLayerButton"));
        mpTogglePortLayerButton->setCheckable(true);
        mpTogglePortLayerButton->setChecked(mpLayerWidget->getPortLayerToggle());
        mpTogglePortLayerButton->setToolTip(QString("toggles the visibility of the ports layer"));

        mpVerticalLayoutComponentsLayer->addWidget(mpTogglePortLayerButton);

        // adding bottom padding
        verticalLayoutComponentsLayerLeftover = 3 * linePoints + 8 * buttonPoints; // this vertical layout lacks 3 * frames and 8 buttons (out of the maximum no. of each)
        if(commonExtraPadding > verticalLayoutComponentsLayerLeftover)
        {
            commonExtraPadding = verticalLayoutComponentsLayerLeftover;
        }
    }

    // filling in the gaps at the bottom of each vertical layout
    mpVerticalLayout->addSpacing(verticalLayoutLeftover - commonExtraPadding);
    mpVerticalLayoutFocus->addSpacing(verticalLayoutFocusLeftover - commonExtraPadding);
    mpVerticalLayoutLayer->addSpacing(verticalLayoutLayerLeftover - commonExtraPadding);
    mpVerticalLayoutComponentsLayer->addSpacing(verticalLayoutComponentsLayerLeftover - commonExtraPadding);

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
    PropertyDialog::connect(mpRenameNodeButton, SIGNAL(clicked()), mpViewWidget, SLOT(changeFocusedVertexLabel()));
    PropertyDialog::connect(mpAddPortButton, SIGNAL(clicked()), mpViewWidget, SLOT(addPortFocused()));
    PropertyDialog::connect(mpSwapPortsButton, SIGNAL(clicked()), mpViewWidget, SLOT(swapPortsFocused()));
    PropertyDialog::connect(mpRenamePortButton, SIGNAL(clicked()), mpViewWidget, SLOT(renamePortFocused()));
    PropertyDialog::connect(mpRemovePortButton, SIGNAL(clicked()), mpViewWidget, SLOT(removePortFocused()));
    PropertyDialog::connect(mpRemovePortsButton, SIGNAL(clicked()), mpViewWidget, SLOT(removePortsFocused()));
    PropertyDialog::connect(mpRemoveNodeButton, SIGNAL(clicked()), mpViewWidget, SLOT(removeFocusedVertex()));
    PropertyDialog::connect(mpRenameEdgeButton, SIGNAL(clicked()), mpViewWidget, SLOT(changeFocusedEdgeLabel()));
    PropertyDialog::connect(mpRemoveEdgeButton, SIGNAL(clicked()), mpViewWidget, SLOT(removeFocusedEdge()));
    PropertyDialog::connect(mpTogglePortLayerButton, SIGNAL(toggled(bool)), mpLayerWidget, SLOT(togglePortLayer(bool)));
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
    mpRenameNodeButton->setText(QApplication::translate("Dialog", "Rename Node", 0, QApplication::UnicodeUTF8));
    mpAddPortButton->setText(QApplication::translate("Dialog", "Add Port", 0, QApplication::UnicodeUTF8));
    mpRenamePortButton->setText(QApplication::translate("Dialog", "Rename a Port", 0, QApplication::UnicodeUTF8));
    mpRemovePortButton->setText(QApplication::translate("Dialog", "Remove a Port", 0, QApplication::UnicodeUTF8));
    mpSwapPortsButton->setText(QApplication::translate("Dialog", "Swap Ports", 0, QApplication::UnicodeUTF8));
    mpRemovePortsButton->setText(QApplication::translate("Dialog", "Remove Ports", 0, QApplication::UnicodeUTF8));
    mpRemoveNodeButton->setText(QApplication::translate("Dialog", "Remove Node", 0, QApplication::UnicodeUTF8));
    mpRenameEdgeButton->setText(QApplication::translate("Dialog", "Rename Edge", 0, QApplication::UnicodeUTF8));
    mpRemoveEdgeButton->setText(QApplication::translate("Dialog", "Remove Edge", 0, QApplication::UnicodeUTF8));
    mpLayersLabel->setText(QApplication::translate("Dialog", "Visible Layers:", 0, QApplication::UnicodeUTF8));
    mpTogglePortLayerButton->setText(QApplication::translate("Dialog", "Ports Layer", 0, QApplication::UnicodeUTF8));
    mpToggleClusterLayerButton->setText(QApplication::translate("Dialog", "Clusters Layer", 0, QApplication::UnicodeUTF8));
} // retranslateUi

} // end namespace gui
} // end namespace graph_analysis

