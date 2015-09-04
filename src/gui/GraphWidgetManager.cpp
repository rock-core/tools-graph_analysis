#include "GraphWidgetManager.hpp"
#include "IconManager.hpp"
#include "WidgetManager.hpp"
#include "ActionCommander.hpp"

#include <QTime>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QTranslator>
#include <QInputDialog>
#include <QFileDialog>
#include <QToolBar>
#include <base/Logging.hpp>

#include <graph_analysis/filters/RegexFilters.hpp>
#include <graph_analysis/GraphIO.hpp>
#include <graph_analysis/io/YamlWriter.hpp>
#include <graph_analysis/io/GexfWriter.hpp>
#include <graph_analysis/io/GexfReader.hpp>
#include <graph_analysis/io/YamlReader.hpp>
#include <graph_analysis/io/GraphvizWriter.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>

namespace graph_analysis {
namespace gui {

const int GraphWidgetManager::TIMEOUT = 6900; // miliseconds

GraphWidgetManager::GraphWidgetManager()
    : mpMainWindow(new QMainWindow())
    , mpTabWidget(new QTabWidget())
    , mpStatus(mpMainWindow->statusBar())
    , mLayout("dot") // other possible layouts: circo, dot, fdp, neato, osage, sfdp, twopi
    , mMode(MOVE_MODE)
{
    resetGraph(false);

    // setting up the Reader and WriterMaps
    io::YamlWriter *yamlWriter = new io::YamlWriter();
    mWriterMap["yaml"]  = yamlWriter;
    mWriterMap["yml"]   = yamlWriter;
    io::YamlReader *yamlReader = new io::YamlReader();
    mReaderMap["yaml"]  = yamlReader;
    mReaderMap["yml"]   = yamlReader;
    io::GexfWriter *gexfWriter = new io::GexfWriter();
    mWriterMap["gexf"]  = gexfWriter;
    mWriterMap["xml"]   = gexfWriter;
    io::GexfReader *gexfReader = new io::GexfReader();
    mReaderMap["gexf"]  = gexfReader;
    mReaderMap["xml"]   = gexfReader;
    io::GraphvizWriter *gvWriter = new io::GraphvizWriter();
    mWriterMap["dot"]  = gvWriter;

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    WidgetManager *widgetManager = WidgetManager::getInstance();
    widgetManager->setGraphWidgetManager(this);
    widgetManager->setMainWindow(mpMainWindow);
    mpMainWindow->setMinimumSize(850, 400);
    mpTabWidget->setCurrentIndex(0); 
    mpMainWindow->setCentralWidget(mpTabWidget);

    // Handle tab changes
    connect(mpTabWidget, SIGNAL( currentChanged(int) ), SLOT( tabChanged(int)) );

    //mpPropertyDialog = new PropertyDialog();
    //widgetManager->setPropertyDialog(mpPropertyDialog);

    // setting up the Menus ToolBar
    ActionCommander comm(this);
    QMenuBar *bar = mpMainWindow->menuBar();

    // File menu
    QMenu *fileMenu = new QMenu(QObject::tr("&File"));
    QAction *actionImport = comm.addAction("Import", SLOT(importGraph()), *(IconManager::getInstance()->getIcon("import_white")), QKeySequence( QKeySequence::Open ), tr("Import graph from file"));

    QAction *actionExport = comm.addAction("Export", SLOT(exportGraph()), *(IconManager::getInstance()->getIcon("export_white")), QKeySequence( QKeySequence::SaveAs), tr("Export graph to file"));

    fileMenu->addAction(actionImport);
    fileMenu->addAction(actionExport);
    fileMenu->addSeparator();

    QToolBar* toolBar = new QToolBar("Toolbar");
    toolBar->addAction(actionImport);
    toolBar->addAction(actionExport);
    toolBar->setFloatable(true);
    mpMainWindow->addToolBar(toolBar);


    // Edit Menu
    QMenu *editMenu = new QMenu(QObject::tr("&Edit"));

    QMenu *modesMenu = editMenu->addMenu(QObject::tr("&Mode"));
    QActionGroup *actionModeGroup = new QActionGroup(this);
    QAction *actionEditMode = comm.addAction("Edit", SLOT(setEditMode()), *(IconManager::getInstance()->getIcon("edit_mode_white")));
    actionEditMode->setCheckable(true);
    QAction *actionConnectMode = comm.addAction("Connect", SLOT(setConnectMode()), *(IconManager::getInstance()->getIcon("connection_mode_white")));
    actionConnectMode->setCheckable(true);
    QAction *actionMoveMode = comm.addAction("Move", SLOT(setMoveMode()), *(IconManager::getInstance()->getIcon("move_mode_white")));
    actionMoveMode->setCheckable(true);

    actionModeGroup->addAction(actionEditMode);
    actionModeGroup->addAction(actionConnectMode);
    actionModeGroup->addAction(actionMoveMode);

    modesMenu->addAction(actionEditMode);
    modesMenu->addAction(actionConnectMode);
    modesMenu->addAction(actionMoveMode);

    QAction *actionAddNode = comm.addAction("Add vertex", SLOT(addVertex()), *(IconManager::getInstance()->getIcon("addNode_white")));
    QAction *actionRenameSelection = comm.addAction("Rename selection", SLOT(renameSelection()), *(IconManager::getInstance()->getIcon("label_white")));
    QAction *actionRemoveSelection  = comm.addAction("Remove selection", SLOT(removeSelection()), *(IconManager::getInstance()->getIcon("remove_white")));
    QAction *actionAddFeature     = comm.addAction("Add feature", SLOT(addFeature()), *(IconManager::getInstance()->getIcon("addFeature_white")));
    QAction *actionSwapFeatures   = comm.addAction("Swap features", SLOT(swapFeatures()), *(IconManager::getInstance()->getIcon("swap_white")));

    editMenu->addAction(actionAddNode);
    editMenu->addSeparator();
    //editMenu->addAction(actionDragDrop);
    //editMenu->addAction(actionMoveAround);
    //editMenu->addSeparator();
    //
    editMenu->addAction(actionRenameSelection);
    editMenu->addAction(actionRemoveSelection);
    editMenu->addAction(actionAddFeature);
    editMenu->addAction(actionSwapFeatures);

    // View Menu
    QMenu *viewMenu = new QMenu(QObject::tr("&View"));
    QAction *actionRefresh = comm.addAction("Refresh", SLOT(refresh()), *(IconManager::getInstance()->getIcon("refresh_white")));
    QAction *actionShuffle = comm.addAction("Shuffle", SLOT(shuffle()), *(IconManager::getInstance()->getIcon("shuffle_white")));
    //QAction *actionReloadPropertyDialog = comm.addAction("Reload Properties", SLOT(reloadPropertyDialogMainWindow()), *(IconManager::getInstance()->getIcon("reload_white")));

//    QAction *actionReset  = comm.addAction("Reset", SLOT(resetGraph()), *(IconManager::getInstance()->getIcon("reset_white")), mpGraphWidgetManager);
    QAction *actionSelectLayout = comm.addAction("Layout", SLOT(selectLayout()), *(IconManager::getInstance()->getIcon("layout_white")));

    viewMenu->addAction(actionRefresh);
    viewMenu->addAction(actionShuffle);
    //viewMenu->addAction(actionReset);
    viewMenu->addAction(actionSelectLayout);
    viewMenu->addSeparator();
    
    // loading different actions in different menus
    //viewMenu->addAction(actionReloadPropertyDialog);

    // loading menus in the bar
    bar->addMenu(fileMenu);
    bar->addMenu(editMenu);
    bar->addMenu(viewMenu);

    mpMainWindow->setWindowTitle(QObject::tr("Graph Analysis"));
    mpStatus->addWidget(new QLabel("Ready"));

    // setting up graph regex filtering
    filters::VertexRegexFilter sourceNodeFilter(".*");
    filters::VertexRegexFilter targetNodeFilter(".*");
    filters::EdgeRegexFilter edgeFilter(".*");

    filters::EdgeContextFilter::Ptr filter(new filters::CombinedEdgeRegexFilter(sourceNodeFilter, edgeFilter, targetNodeFilter));
    std::vector< Filter< graph_analysis::Edge::Ptr >::Ptr > edgeFilters;
    edgeFilters.push_back(filter);

    std::vector<GraphWidget*>::iterator it = mGraphWidgets.begin();
    for(; it != mGraphWidgets.end(); ++it)
    {
        GraphWidget* graphWidget = *it;
        graphWidget->setEdgeFilters(edgeFilters);
        graphWidget->setGraphLayout(mLayout);
    }

    mpMainWindow->show();
}

GraphWidgetManager::~GraphWidgetManager()
{
    WriterMap::iterator it_writer = mWriterMap.begin();
    for(; mWriterMap.end() != it_writer; ++it_writer)
    {
        delete it_writer->second;
        it_writer->second = NULL;
    }

    ReaderMap::iterator it_reader = mReaderMap.begin();
    for(; mReaderMap.end() != it_reader; ++it_reader)
    {
       delete it_reader->second;
       it_reader->second = NULL;
    }
}

void GraphWidgetManager::reloadPropertyDialog(void)
{
    updateStatus(std::string("reloading command panel..."));
    if(mpPropertyDialog)
    {
        delete mpPropertyDialog;
        mpPropertyDialog = NULL;
    }
    //mpPropertyDialog = new PropertyDialog(mpGraphWidgetManager->getDragDrop(), mpGraphWidgetManager->getVertexFocused(), mpGraphWidgetManager->getEdgeFocused());
    //
    WidgetManager::getInstance()->setPropertyDialog(mpPropertyDialog); // IMPORTANT!!! - now updating the corresponding field in the WidgetManager
    updateStatus(std::string("Reloaded command panel!"), GraphWidgetManager::TIMEOUT);
}

void GraphWidgetManager::helpSetup(std::stringstream& ss, const std::string& cmd)
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


void GraphWidgetManager::renameElementDialog(GraphElement::Ptr element)
{
    updateStatus("Renaming '" + element->toString() + "'");

    bool ok;
    QString label = QInputDialog::getText(currentGraphWidget(), tr("Change label"),
                                         tr("New label:"), QLineEdit::Normal,
                                         QString(element->getLabel().c_str()), &ok);
    if (ok && !label.isEmpty())
    {
        std::string old_label = element->toString();
        std::string new_label = label.toStdString();

        currentGraphWidget()->renameElement(element, new_label);
        updateStatus("Renamed from '" + old_label + "' to '" + new_label + "'", GraphWidgetManager::TIMEOUT);
        refresh();
    }
    else
    {
        updateStatus(std::string("Failed to rename focused node: aborted by user!"), GraphWidgetManager::TIMEOUT);
    }
}

void GraphWidgetManager::renameSelection()
{
    std::vector<GraphElement::Ptr> elements = currentGraphWidget()->getElementSelection();
    std::vector<GraphElement::Ptr>::iterator it = elements.begin();
    for(; it != elements.end(); ++it)
    {
        GraphElement::Ptr element = *it;
        renameElementDialog(element);
    }
//    if(mpGraphWidgetManager->getVertexFocused())
//    {
//        mpGraphWidgetManager->changeFocusedVertexLabel();
//    }
//    else
//    {
//        QMessageBox::information(mpStackedWidget, tr("Cannot Rename Focused Node"), tr("Cannot Rename Focused Node: no node is focused on!"));
//    }
}

void GraphWidgetManager::removeSelection()
{
//    if(mpGraphWidgetManager->getVertexFocused())
//    {
//        mpGraphWidgetManager->removeFocusedVertex();
//    }
//    else
//    {
//        QMessageBox::information(mpStackedWidget, tr("Cannot Remove Focused Node"), tr("Cannot Remove Focused Node: no node is focused on!"));
//    }
}

void GraphWidgetManager::addFeature()
{
//    if(mpGraphWidgetManager->getVertexFocused())
//    {
//        mpGraphWidgetManager->addFeatureFocused();
//    }
//    else
//    {
//        QMessageBox::information(mpStackedWidget, tr("Cannot Add a Feature to the Focused Node"), tr("Cannot Add a Feature to the Focused Node: no node is focused on!"));
//    }
}

void GraphWidgetManager::swapFeatures()
{
//    if(mpGraphWidgetManager->getVertexFocused())
//    {
//        mpGraphWidgetManager->swapFeaturesFocused();
//    }
//    else
//    {
//        QMessageBox::information(mpStackedWidget, tr("Cannot Swap Features of the Focused Node"), tr("Cannot Swap Features of the Focused Node: no node is focused on!"));
//    }
}

//void GraphWidgetManager::renameSelection()
//{
////    if(mpGraphWidgetManager->getVertexFocused())
////    {
////        mpGraphWidgetManager->renameFeatureFocused();
////    }
////    else
////    {
////        QMessageBox::information(mpStackedWidget, tr("Cannot Rename a Feature of the Focused Node"), tr("Cannot Rename a Feature of the Focused Node: no node is focused on!"));
////    }
//}
//
//void GraphWidgetManager::removeSelection()
//{
////    if(mpGraphWidgetManager->getVertexFocused())
////    {
////        mpGraphWidgetManager->removeFeaturesFocused();
////    }
////    else
////    {
////        QMessageBox::information(mpStackedWidget, tr("Cannot Remove the Features of the Focused Node"), tr("Cannot Remove the Features of the Focused Node: no node is focused on!"));
////    }
//}

void GraphWidgetManager::addVertex()
{
    currentGraphWidget()->addVertexDialog();
}

void GraphWidgetManager::refresh()
{
    LOG_DEBUG_S << "Refresh current widget: " << currentGraphWidget()->getClassName().toStdString();
    currentGraphWidget()->refresh();
}

void GraphWidgetManager::shuffle()
{
    LOG_DEBUG_S << "Shuffle current widget: " << currentGraphWidget()->getClassName().toStdString();
    currentGraphWidget()->shuffle();
}

void GraphWidgetManager::selectLayout()
{
    GraphWidget* widget = currentGraphWidget();

    updateStatus(std::string("Selecting graph layout..."));

    bool ok;

    QString desiredLayout = QInputDialog::getItem(widget, tr("Select Layout"),
                                         tr("select a layout:"), widget->getSupportedLayouts(),
                                         0, false, &ok);
    if (ok && !desiredLayout.isEmpty())
    {
        widget->refresh(true /*keep all data*/);
        widget->setGraphLayout(desiredLayout);

        updateStatus("Changed graph layout to '" + desiredLayout.toStdString() + "'", GraphWidgetManager::TIMEOUT);
    }
    else
    {
        updateStatus("Changing graph layout aborted by user", GraphWidgetManager::TIMEOUT);
    }
}

void GraphWidgetManager::reloadPropertyDialogMainWindow()
{
    if(!mpPropertyDialog->isRunning())
    {
        reloadPropertyDialog();
    }
    else
    {
        QMessageBox::information(mpTabWidget, tr("Cannot Reload Property Dialog"), tr("Cannot Reload Property Dialog: the dialog is still active! It needs closing first."));
    }
}

int GraphWidgetManager::addGraphWidget(GraphWidget* graphWidget)
{
    if( mGraphWidgets.end() == std::find(mGraphWidgets.begin(), mGraphWidgets.end(), graphWidget))
    {
        graphWidget->setGraph(mpGraph);
        graphWidget->setGraphWidgetManager(this);
        mGraphWidgets.push_back(graphWidget);
        WidgetManager *widgetManager = WidgetManager::getInstance();
        mpTabWidget->addTab(dynamic_cast<QWidget*>(graphWidget)/*,QIcon*/, graphWidget->getClassName());
        widgetManager->setWidget(graphWidget);
        return mGraphWidgets.size() - 1;
    } else {
        throw std::runtime_error("graph_analysis::gui::GraphWidgetManager::addGraphWidget: cannot add widget since it has already been registered");
    }
}


void GraphWidgetManager::resetGraph(bool keepData)
{
    if(!keepData)
    {
        mpGraph = BaseGraph::getInstance();
    }

    std::vector<GraphWidget*>::iterator it = mGraphWidgets.begin();
    for(; it != mGraphWidgets.end(); ++it)
    {
        GraphWidget* graphWidget = *it;
        graphWidget->setGraph(mpGraph);
        graphWidget->clear();
    }
}

void GraphWidgetManager::importGraph()
{
    updateStatus("importing graph...");
    QString selectedFilter;
    QString filename =  QFileDialog::getOpenFileName(currentGraphWidget(), tr("Choose input file"), QDir::currentPath(), tr("GEXF (*.gexf *.xml);;YAML/YML (*.yaml *.yml)"), &selectedFilter);

    if (!filename.isEmpty())
    {
        fromFile(filename, selectedFilter.startsWith("GEXF"));
        refresh();
    }
    else
    {
        updateStatus("Failed to import graph: aborted by user - an empty input filename was provided", GraphWidgetManager::TIMEOUT);
    }
}

void GraphWidgetManager::exportGraph()
{
    updateStatus("exporting graph...");
    QString selectedFilter;
    QString label =  QFileDialog::getSaveFileName(currentGraphWidget(), tr("Choose Export File"), QDir::currentPath(), tr("GEXF (*.gexf *.xml);;YAML/YML (*.yaml *.yml);;DOT (*.dot)"), &selectedFilter);

    if (!label.isEmpty())
    {
        // removing trailing whitespaces in the filename
        label = label.trimmed();
        if(label.contains('.'))
        {
            if(label.endsWith(QString(".gexf")) || label.endsWith(QString(".xml")))
            {
                toXmlFile(label.toStdString());
            }
            else if(label.endsWith(QString(".yml")) || label.endsWith(QString(".yaml")))
            {
                toYmlFile(label.toStdString());
            }
            else if(label.endsWith(QString(".dot")))
            {
                toDotFile(label.toStdString());
            }
            else
            {
                QMessageBox::critical(currentGraphWidget(), tr("Graph Export Failed"), "Unsupported file format for file '" + label + "'");
                updateStatus("Failed to export graph: to output file '" + label + "': unsupported file format", GraphWidgetManager::TIMEOUT);
                return;
            }
        }
        else
        {
            if(selectedFilter.startsWith("GEXF"))
            {
                toXmlFile(label.toStdString() + ".gexf");
            }
            else if(selectedFilter.startsWith("YAML"))
            {
                toYmlFile(label.toStdString() + ".yml");
            }
            else
            {
                toDotFile(label.toStdString() + ".dot");
            }
        }
        updateStatus("Exported graph to output file '" + label.toStdString() + "'", GraphWidgetManager::TIMEOUT);
    }
    else
    {
        updateStatus("Failed to export graph: aborted by user - an empty output filename was provided", GraphWidgetManager::TIMEOUT);
    }
}

void GraphWidgetManager::toYmlFile(const std::string& filename)
{
    try
    {
        mWriterMap["yaml"]->write(filename, mpGraph);
    }
    catch(std::runtime_error e)
    {
        LOG_ERROR_S << "graph_analysis::gui::GraphWidgetManager::toYmlFile: export failed: " << e.what();
        QMessageBox::critical(currentGraphWidget(), tr("Graph export Failed"), e.what());
        updateStatus("Yaml Graph export failed: " + std::string(e.what()), GraphWidgetManager::TIMEOUT);
    }
}

void GraphWidgetManager::toDotFile(const std::string& filename)
{
    try
    {
        mWriterMap["dot"]->write(filename, mpGraph);
    }
    catch(std::runtime_error e)
    {
        LOG_ERROR_S << "graph_analysis::gui::GraphWidgetManager::toDotFile: export via graphviz failed: " << e.what();
        QMessageBox::critical(currentGraphWidget(), tr("Graph Export via GraphViz Failed"), e.what());
        updateStatus("Dot Graph export failed: " + std::string(e.what()), GraphWidgetManager::TIMEOUT);
    }
}

void GraphWidgetManager::gvRender(const std::string& filename)
{
    currentGraphWidget()->gvRender(filename);
}

void GraphWidgetManager::toXmlFile(const std::string& filename)
{
    try
    {
        mWriterMap["gexf"]->write(filename, mpGraph);
    }
    catch(const std::runtime_error& e)
    {
        LOG_ERROR_S << "graph_analysis::gui::GraphWidgetManager::toXmlFile: export to .gexf failed: " << e.what();
        QMessageBox::critical(currentGraphWidget(), tr("Graph Export to .gexf Failed"), e.what());
        updateStatus("Gexf Graph export failed: " + std::string(e.what()), GraphWidgetManager::TIMEOUT);
    }
}

int GraphWidgetManager::fromFile(const std::string& filename, const std::string& format)
{
    graph_analysis::BaseGraph::Ptr graph = BaseGraph::getInstance();
    try
    {
        mReaderMap[format]->read(filename, graph);
    }
    catch(const std::runtime_error& e)
    {
        LOG_WARN_S << "graph_analysis::gui::GraphWidgetManager::fromFile: graph import failed: " << e.what();
        QMessageBox::critical(currentGraphWidget(), tr("Graph Import Failed"), e.what());
        updateStatus("Graph import failed: " + std::string(e.what()), GraphWidgetManager::TIMEOUT);
        return 1;
    }

    mpGraph = graph;
    notifyAll();
    return 0;
}

void GraphWidgetManager::notifyModeChange(Mode mode)
{
    std::vector<GraphWidget*>::iterator it = mGraphWidgets.end();
    for(; it != mGraphWidgets.end(); ++it)
    {
        GraphWidget* graphWidget = *it;
        graphWidget->modeChanged(mode);
    }
}

void GraphWidgetManager::notifyAll()
{
    std::vector<GraphWidget*>::iterator it = mGraphWidgets.end();
    for(; it != mGraphWidgets.end(); ++it)
    {
        GraphWidget* graphWidget = *it;
        graphWidget->setGraphLayout(mLayout);
        graphWidget->refresh(false);
    }
}

void GraphWidgetManager::fromFile(const QString& file, bool prefers_gexf, bool status)
{
    int return_code;
    // removing trailing whitespaces in the filename
    QString filename(file.trimmed());
    if(filename.contains('.'))
    {
        if(filename.endsWith(QString(".gexf")) || filename.endsWith(QString(".xml")))
        {
            return_code = fromFile(filename.toStdString(), "gexf");
        }
        else if(filename.endsWith(QString(".yml")) || filename.endsWith(QString(".yaml")))
        {
            return_code = fromFile(filename.toStdString(), "yml");
        }
        else
        {
            QMessageBox::critical(currentGraphWidget(), tr("Graph Import Failed"), "Unsupported file format for file '" + filename + "'");
            updateStatus("Failed to import graph from file '" + filename.toStdString() + "': unsupported file format", GraphWidgetManager::TIMEOUT);
            return;
        }
    }
    else
    {
        if(prefers_gexf)
        {
            return_code = fromFile(filename.toStdString() + ".gexf", "gexf");
        }
        else
        {
            return_code = fromFile(filename.toStdString() + ".yml", "yml");
        }
    }

    if(!return_code)
    {
        if(status)
        {
            updateStatus("Imported graph from file '" + filename.toStdString() + "'", GraphWidgetManager::TIMEOUT);
        }
    }
}

GraphWidget* GraphWidgetManager::currentGraphWidget()
{
    size_t currentIndex = mpTabWidget->currentIndex();
    if(mGraphWidgets.size() > currentIndex)
    {
        return mGraphWidgets[currentIndex];
    }
    throw std::runtime_error("graph_analysis::gui::currentGraphWidget: index out of bounds -- no graph widget available for this operation");
}

} // end namespace gui
} // end namespace graph_analysis
