#include "GraphManager.hpp"
#include "IconManager.hpp"
#include "WidgetManager.hpp"
#include "ActionCommander.hpp"

#include <QTime>
#include <QMenu>
#include <QMenuBar>
#include <QTranslator>
#include <base/Logging.hpp>

#include <graph_analysis/filters/RegexFilters.hpp>

namespace graph_analysis {
namespace gui {

GraphManager::GraphManager(const QString& filename)
    : mpMainWindow(new QMainWindow())
    , mpStackedWidget(new QStackedWidget())
    , mpStatus(mpMainWindow->statusBar())
    , mLayout("dot") // other possible layouts: circo, dot, fdp, neato, osage, sfdp, twopi
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    WidgetManager *widgetManager = WidgetManager::getInstance();
    widgetManager->setGraphManager(this);
    widgetManager->setMainWindow(mpMainWindow);
    widgetManager->setStackedWidget(mpStackedWidget);

    mpMainWindow->setMinimumSize(850, 400);

    mpLayerWidget = new LayerWidget();
    widgetManager->setLayerWidget(mpLayerWidget);

    mpViewWidget = new ViewWidget(mpMainWindow);
    widgetManager->setViewWidget(mpViewWidget);
//    mpViewWidget->reset();

    mpStackedWidget->addWidget((QWidget *) mpViewWidget);
    mpStackedWidget->addWidget((QWidget *) mpLayerWidget);
    mpStackedWidget->setCurrentIndex(0); // showing mpViewWidget on init
    mpMainWindow->setCentralWidget(mpStackedWidget);

    mpPropertyDialog = new PropertyDialog(mpViewWidget, mpLayerWidget, mpMainWindow, mpStackedWidget);
    widgetManager->setPropertyDialog(mpPropertyDialog);

    // setting up the Menus ToolBar
    ActionCommander comm(mpViewWidget);
    QMenuBar *bar = mpMainWindow->menuBar();

    // needed menus
    QMenu *MainMenu = new QMenu(QObject::tr("&Graph"));
    QMenu *NodeMenu = new QMenu(QObject::tr("&Node"));
    QMenu *EdgeMenu = new QMenu(QObject::tr("&Edge"));

    // needed actions
    QAction *actionChangeEdgeLabel = comm.addAction("Rename Edge", SLOT(changeFocusedEdgeLabelMainWindow()), *(IconManager::getInstance()->getIcon("label_white")));
    QAction *actionRemoveEdge  = comm.addAction("Remove Edge", SLOT(removeFocusedEdgeMainWindow()), *(IconManager::getInstance()->getIcon("remove_white")));
    QAction *actionChangeLabel = comm.addAction("Rename Node", SLOT(changeFocusedVertexLabelMainWindow()), *(IconManager::getInstance()->getIcon("label_white")));
    QAction *actionRemoveNode  = comm.addAction("Remove Node", SLOT(removeFocusedVertexMainWindow()), *(IconManager::getInstance()->getIcon("remove_white")));
    QAction *actionAddFeature     = comm.addAction("Add Feature", SLOT(addFeatureFocusedMainWindow()), *(IconManager::getInstance()->getIcon("addFeature_white")));
    QAction *actionSwapFeatures   = comm.addAction("Swap Features", SLOT(swapFeaturesFocusedMainWindow()), *(IconManager::getInstance()->getIcon("swap_white")));
    QAction *actionRenameFeature  = comm.addAction("Rename a Feature", SLOT(renameFeatureFocusedMainWindow()), *(IconManager::getInstance()->getIcon("featureLabel_white")));
    QAction *actionRemoveFeature  = comm.addAction("Remove a Feature", SLOT(removeFeatureFocusedMainWindow()), *(IconManager::getInstance()->getIcon("remove_white")));
    QAction *actionRemoveFeatures = comm.addAction("Remove Features", SLOT(removeFeaturesFocusedMainWindow()), *(IconManager::getInstance()->getIcon("removeAll_white")));
    QAction *actionAddNode = comm.addAction("Add Node", SLOT(addNodeAdhocMainWindow()), *(IconManager::getInstance()->getIcon("addNode_white")));
    QAction *actionRefresh = comm.addAction("Refresh", SLOT(refreshMainWindow()), *(IconManager::getInstance()->getIcon("refresh_white")));
    QAction *actionShuffle = comm.addAction("Shuffle", SLOT(shuffleMainWindow()), *(IconManager::getInstance()->getIcon("shuffle_white")));
    QAction *actionImport = comm.addAction("Import", SLOT(importGraph()), *(IconManager::getInstance()->getIcon("import_white")));
    QAction *actionExport = comm.addAction("Export", SLOT(exportGraph()), *(IconManager::getInstance()->getIcon("export_white")));
    QAction *actionReset  = comm.addAction("Reset", SLOT(resetGraph()), *(IconManager::getInstance()->getIcon("reset_white")));
    QAction *actionLayout = comm.addAction("Layout", SLOT(changeLayoutMainWindow()), *(IconManager::getInstance()->getIcon("layout_white")));
    QAction *actionDragDrop = comm.addAction("Drag-n-Drop Mode", SLOT(setDragDrop()), *(IconManager::getInstance()->getIcon("dragndrop_white")));
    QAction *actionMoveAround = comm.addAction("Move-around Mode", SLOT(unsetDragDrop()), *(IconManager::getInstance()->getIcon("move_white")));
    QAction *actionReloadPropertyDialog = comm.addAction("Reload Properties", SLOT(reloadPropertyDialogMainWindow()), *(IconManager::getInstance()->getIcon("reload_white")));

    // loading different actions in different menus
    MainMenu->addAction(actionAddNode);
    MainMenu->addSeparator();
    MainMenu->addAction(actionImport);
    MainMenu->addAction(actionExport);
    MainMenu->addSeparator();
    MainMenu->addAction(actionRefresh);
    MainMenu->addAction(actionShuffle);
    MainMenu->addAction(actionReset);
    MainMenu->addAction(actionLayout);
    MainMenu->addSeparator();
    MainMenu->addAction(actionDragDrop);
    MainMenu->addAction(actionMoveAround);
    MainMenu->addSeparator();
    MainMenu->addAction(actionReloadPropertyDialog);

    NodeMenu->addAction(actionChangeLabel);
    NodeMenu->addAction(actionAddFeature);
    NodeMenu->addAction(actionSwapFeatures);
    NodeMenu->addAction(actionRenameFeature);
    NodeMenu->addAction(actionRemoveFeature);
    NodeMenu->addAction(actionRemoveFeatures);
    NodeMenu->addAction(actionRemoveNode);

    EdgeMenu->addAction(actionChangeEdgeLabel);
    EdgeMenu->addAction(actionRemoveEdge);

    // loading menus in the bar
    bar->addMenu(MainMenu);
    bar->addMenu(NodeMenu);
    bar->addMenu(EdgeMenu);

    mpMainWindow->setWindowTitle(QObject::tr("Graph Analysis"));
    mpStatus->addWidget(new QLabel("Ready!"));

    // setting up graph regex filtering
    filters::VertexRegexFilter sourceNodeFilter(".*");
    filters::VertexRegexFilter targetNodeFilter(".*");
    filters::EdgeRegexFilter edgeFilter(".*");

    filters::EdgeContextFilter::Ptr filter(new filters::CombinedEdgeRegexFilter(sourceNodeFilter, edgeFilter, targetNodeFilter));
    std::vector< Filter< graph_analysis::Edge::Ptr >::Ptr > edgeFilters;
    edgeFilters.push_back(filter);
    mpViewWidget->setEdgeFilters(edgeFilters);
    mpViewWidget->setLayout(mLayout);
    if(!filename.isEmpty())
    {
        // loading graph on init from the given filename
        mpViewWidget->fromFile(filename, true, false);
    }
    mpMainWindow->show();
}

GraphManager::~GraphManager()
{
    if(mpMainWindow)
    {
        // this instance assumed ownership of the rest of the Qt components instances (it will delete them recursively)
        delete mpMainWindow;
    }
}

void GraphManager::reloadPropertyDialog(void)
{
    updateStatus(std::string("reloading command panel..."));
    if(mpPropertyDialog)
    {
        delete mpPropertyDialog;
    }
    mpPropertyDialog = new PropertyDialog(mpViewWidget, mpLayerWidget, mpMainWindow, mpStackedWidget, mpViewWidget->getDragDrop());
    updateStatus(std::string("Reloaded command panel!"), DEFAULT_TIMEOUT);
}

void GraphManager::helpSetup(std::stringstream& ss, const std::string& cmd)
{
    ss << "usage: " << cmd << " [--help]" << std::endl;
    ss << std::endl;
    ss << "Options:" << std::endl;
    ss << "-------"  << std::endl;
    ss << "    --help " << "displays this informative message" << std::endl;
    ss << std::endl;
    ss << "Details:" << std::endl;
    ss << "-------"  << std::endl;
    ss << " The current graph GUI implementation illustrates the Rock Orogen Components diagrams use-case."; // << std::endl;
    ss << " The Orogen Components are modeled as cluster nodes in an internal base graph, while their input/output "; // << std::endl;
    ss << "ports correspond to port vertices through which clusters form edges."; // << std::endl;
    ss << std::endl;
    ss << std::endl;
    ss << "Features:" << std::endl;
    ss << "--------"  << std::endl;
    ss << "The GUI contains a main windnow representing the Orogen Components Diagram (the view of the graph) and a dockable panel with button-commands." << std::endl;
    ss << "The commands panel is accompanied by homologous commands in the context menu of the main window. "; // << std::endl;
    ss << "The commands panel can be reloaded (if previously closed) via the extra command 'Reload Properties' made available in the context menu "; // << std::endl;
    ss << "or by the keyboard short-cut CTRL+P (CTRL+p)" << std::endl;
    ss << std::endl;
    ss << "In the so-called 'move-around' mode, the user can re-arranged and closely manipulate displayed components (a.k.a. node items)." << std::endl;
    ss << "The re-arranging is performed by dragging the components with the mouse while the manipulation includes the following commands "; // << std:endl;
    ss << "made available in both the commands panel and the main window context menu:" << std::endl;
    ss << "    At all times, the following commands are available:" << std::endl;
    ss << "        'Add Node' - adds a new node to the graph"       << std::endl;
    ss << "        'Import'   - opens an existing graph file"       << std::endl;
    ss << "        'Export'   - saves the graph to file"            << std::endl;
    ss << "        'Refresh'  - reloads the graph view"             << std::endl;
    ss << "        'Shuffle'  - randomly spreads vertices around"   << std::endl;
    ss << "        'Reset'    - completely deletes the graph"       << std::endl;
    ss << "        'Layout'   - applies a new layout to the graph"  << std::endl;
    ss << "        Drag'n'Drop- toggles the drag-n-drop mode"       << std::endl;
    ss << "    When a component has been focused on - i.e. the user double-clicked it";     // << std::endl;
    ss << " the following commands are made available in the commands panel:"               << std::endl;
    ss << "    (or in the context menu when the component has been right-clicked instead)"  << std::endl;
    ss << "        'Rename Node'    - changes the label of the selected vertex"             << std::endl;
    ss << "        'Add Port'       - adds a port to the selected vertex"                   << std::endl;
    ss << "        'Swap Ports'     - swaps the positions of two ports of that vertex"      << std::endl;
    ss << "        'Rename a Port'  - changes the label of a port of the selected vertex"   << std::endl;
    ss << "        'Remove a Port'  - removes a port of the selected vertex"                << std::endl;
    ss << "        'Remove Ports'   - removes ALL ports of the selected vertex"             << std::endl;
    ss << "        'Remove Node'    - removes the selected vertex from the graph"           << std::endl;
    ss << "    When an edge has been focused on - i.e. the user double-clicked its label";  // << std::endl;
    ss << " the following commands are made available in the commands panel:"               << std::endl;
    ss << "    (or in the context menu when the component has been right-clicked instead)"  << std::endl;
    ss << "        'Rename Edge'    - changes the label of the selected edge"               << std::endl;
    ss << "        'Remove Edge'    - removes the selected edge from the graph"             << std::endl;
    ss << "The view can generally be changed at all times by zooming in or out with the";   // << std::endl;
    ss << " mouse wheel or by 2D translations via dragging with the middle mouse button."   << std::endl;
    ss << std::endl;
    ss << "In the 'drag-n-drop' mode, the user can spawn new edges from output ports of one component to input ports of another component "; // << std::endl;
    ss << "and swap same-type ports of the same component." << std::endl;
    ss << "The user can toggle in between the 2 modes either by using the <Drag'n'Drop> button command in the dockable panel, "; // << std::endl;
    ss << "or the homologous one in the main window context menu, or by employing one of the 2 keyboard short-cuts: CTRL+CapsLock or CTRL+D (CTRL+d)." << std::endl;
    ss << std::endl;
    ss << "Keys Combinations:" << std::endl;
    ss << "---- ------------"  << std::endl;
    ss << "    Minus    (-)     -*  zoom-out the view" << std::endl;
    ss << "    Plus     (+)     -*  zoom-in  the view" << std::endl;
    ss << "    Space    (space) -*  refresh the graph view" << std::endl;
    ss << "    R        (r)     -*  refresh the graph view" << std::endl;
    ss << "    S        (s)     -*  shuffle the graph nodes" << std::endl;
    ss << "    L        (l)     -*  change graph layout" << std::endl;
    ss << std::endl;
    ss << "    Escape   (ESC)   -   clear focus (if any node and/or edge is being focused on)" << std::endl;
    ss << std::endl;
    ss << "    CTRL+Minus(CTRL-)-   zoom-out the view" << std::endl;
    ss << "    CTRL+Plus (CTRL+)-   zoom-in  the view" << std::endl;
    ss << "    CTRL+Q   (CTRL+q)-   quit" << std::endl;
    ss << "    CTRL+W   (CTRL+w)-   quit" << std::endl;
    ss << "    CTRL+R   (CTRL+r)-   reset graph" << std::endl;
    ss << "    CTRL+E   (CTRL+e)-   save (export graph)" << std::endl;
    ss << "    CTRL+S   (CTRL+s)-   save (export graph)" << std::endl;
    ss << "    CTRL+A   (CTRL+a)-   add new node to graph" << std::endl;
    ss << "    CTRL+I   (CTRL+i)-   open (import graph)" << std::endl;
    ss << "    CTRL+O   (CTRL+o)-   open (import graph)" << std::endl;
    ss << "    CTRL+L   (CTRL+l)-   open (import graph)" << std::endl;
    ss << "    CTRL+P   (CTRL+p)-   reload buttons panel (if closed)" << std::endl;
    ss << "    CTRL+Left        -   rotate view anti-clockwise" << std::endl;
    ss << "    CTRL+Right       -   rotate view clockwise" << std::endl;
    ss << "    CTRL+D   (CTRL+d)-   toggle in between the two modes" << std::endl;
    ss << "    CTRL+CapsLock    -   toggle in between the two modes" << std::endl;
    ss << std::endl;
    ss << "    SHIFT+Up         -** shift up the port that is being edited" << std::endl;
    ss << "    SHIFT+Down       -** shift down the port that is being edited" << std::endl;
    ss << std::endl;
    ss << "Notes:" << std::endl;
    ss << "    -*  available in move-around mode only" << std::endl;
    ss << "    -** available in drag-n-drop mode only" << std::endl;
    ss << std::endl;
}

} // end namespace gui
} // end namespace graph_analysis
