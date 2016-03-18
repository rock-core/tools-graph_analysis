#include "GraphWidgetManager.hpp"
#include "IconManager.hpp"
#include "WidgetManager.hpp"
#include "ActionCommander.hpp"

#include <QTime>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QTranslator>
#include <QInputDialog>
#include <QFileDialog>
#include <QToolBar>
#include <base/Logging.hpp>

#include <graph_analysis/GraphIO.hpp>
#include <graph_analysis/io/YamlWriter.hpp>
#include <graph_analysis/io/GexfWriter.hpp>
#include <graph_analysis/io/GexfReader.hpp>
#include <graph_analysis/io/YamlReader.hpp>
#include <graph_analysis/io/GraphvizWriter.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>
#include <graph_analysis/gui/dialogs/PropertyDialog.hpp>
#include <graph_analysis/gui/dialogs/ExportFile.hpp>

namespace graph_analysis {
namespace gui {

const int GraphWidgetManager::TIMEOUT = 6900; // miliseconds

GraphWidgetManager::GraphWidgetManager()
    : mpMainWindow(new QMainWindow())
    , mpTabWidget(new QTabWidget(mpMainWindow))
    , mpStatus(mpMainWindow->statusBar())
    , mLayout("dot") // other possible layouts: circo, dot, fdp, neato, osage, sfdp, twopi
    , mMode(MOVE_MODE)
{
    resetGraph(false);

    WidgetManager::getInstance()->setGraphWidgetManager(this);
    WidgetManager::getInstance()->setMainWindow(mpMainWindow);
    mpMainWindow->setMinimumSize(850, 400);
    mpTabWidget->setCurrentIndex(0);
    mpMainWindow->setCentralWidget(mpTabWidget);

    // Handle tab changes
    connect(mpTabWidget, SIGNAL( currentChanged(int) ), SLOT( tabChanged(int)) );

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

    // View Menu
    QMenu *viewMenu = new QMenu(QObject::tr("&View"));
    QAction *actionRefresh = comm.addAction("Refresh", SLOT(refresh()), *(IconManager::getInstance()->getIcon("refresh_white")));
    QAction *actionShuffle = comm.addAction("Shuffle", SLOT(shuffle()), *(IconManager::getInstance()->getIcon("shuffle_white")));

    viewMenu->addAction(actionRefresh);
    viewMenu->addAction(actionShuffle);

    // loading menus in the bar
    bar->addMenu(fileMenu);
    bar->addMenu(editMenu);
    bar->addMenu(viewMenu);

    mpMainWindow->setWindowTitle(QObject::tr("Graph Analysis"));

    mpMainWindow->show();
    updateStatus("Ready");
}

GraphWidgetManager::~GraphWidgetManager()
{
    delete mpTabWidget;
    delete mpMainWindow;
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

void GraphWidgetManager::addGraphWidget(GraphWidget* graphWidget)
{
    mpTabWidget->insertTab(mpTabWidget->count(), graphWidget, graphWidget->getClassName());
}


void GraphWidgetManager::resetGraph(bool keepData)
{
    if(!keepData)
    {
        // overwrite the existing shared to pointer to the BaseGraph with a
        // new, empty, instance.
        mpGraph = BaseGraph::getInstance();
    }

    for(int index = 0; index < mpTabWidget->count(); index++)
    {
        GraphWidget* graphWidget = dynamic_cast<GraphWidget*>(mpTabWidget->widget(index));
        graphWidget->setGraph(mpGraph);
        graphWidget->clearVisualization();
    }
}

void GraphWidgetManager::importGraph()
{
    updateStatus("importing graph...");
    QString selectedFilter;
    // Constructing the writer suffix filter
    io::GraphIO::SuffixMap suffixMap = io::GraphIO::getSuffixMap();
    io::GraphIO::ReaderMap readerMap = io::GraphIO::getReaderMap();
    io::GraphIO::ReaderMap::const_iterator rit = readerMap.begin();

    std::stringstream ss;
    for(;;)
    {
        ss << representation::TypeTxt[ rit->first ] << " (";
        io::GraphIO::SuffixMap::const_iterator sit = suffixMap.begin();
        for(; sit != suffixMap.end(); ++sit)
        {
            if(sit->second == rit->first)
            {
                ss << "*." << sit->first << " ";
            }
        }
        ss << ")";

        ++rit;
        if(rit != readerMap.end())
        {
            ss << ";;";
        } else {
            break;
        }
    }
    // End constructing the writer suffix filter

    QString filename =  QFileDialog::getOpenFileName(currentGraphWidget(), tr("Choose input file"), QDir::currentPath(), tr(ss.str().c_str()), &selectedFilter);

    if (!filename.isEmpty())
    {
        fromFile(filename.toStdString());
        refresh();
    } else
    {
        updateStatus("Failed to import graph: aborted by user - an empty input filename was provided");
    }
}

void GraphWidgetManager::exportGraph()
{
    updateStatus("Exporting graph...");
    if(mpGraph->empty())
    {
        QMessageBox::critical(currentGraphWidget(), tr("Graph Export Failed"), "Graph is empty");
        return;
    }

    QString selectedFilter;

    // Constructing the writer suffix filter
    io::GraphIO::SuffixMap suffixMap = io::GraphIO::getSuffixMap();
    io::GraphIO::WriterMap writerMap = io::GraphIO::getWriterMap();
    io::GraphIO::WriterMap::const_iterator wit = writerMap.begin();

    std::stringstream ss;
    for(;;)
    {
        ss << representation::TypeTxt[ wit->first ] << " (";
        io::GraphIO::SuffixMap::const_iterator sit = suffixMap.begin();
        for(; sit != suffixMap.end(); ++sit)
        {
            if(sit->second == wit->first)
            {
                ss << "*." << sit->first << " ";
            }
        }
        ss << ")";

        ++wit;
        if(wit != writerMap.end())
        {
            ss << ";;";
        } else {
            break;
        }
    }
    // End constructing the writer suffix filter

    dialogs::ExportFile dialog(ss.str().c_str());
    if( dialog.exec() == QFileDialog::Accepted)
    {
        try {
            io::GraphIO::write(dialog.getFilename().toStdString(), mpGraph, dialog.getTypeName());
            updateStatus("Exported graph to output file '" + dialog.getFilename().toStdString() + "'");
        } catch(const std::exception& e)
        {
            std::string msg = "Export of graph to '" + dialog.getFilename().toStdString() + "' failed " + e.what();
            QMessageBox::critical(currentGraphWidget(), tr("Graph Export Failed"), msg.c_str());
            return;
        }
    } else
    {
        updateStatus("Exporting graph aborted by user");
    }
}

void GraphWidgetManager::fromFile(const std::string& filename)
{
    graph_analysis::BaseGraph::Ptr graph = BaseGraph::getInstance();

    try {
        io::GraphIO::read(filename, graph);
    } catch(const std::exception& e)
    {
        std::string msg = "Failed to import '" + filename + "': " + e.what();
        QMessageBox::critical(currentGraphWidget(), tr("Graph Import Failed"), msg.c_str());
        return;
    }

    mpGraph = graph;
    notifyAll();
}

void GraphWidgetManager::notifyAll()
{
    for(int index = 0; index < mpTabWidget->count(); index++)
    {
        GraphWidget *graphWidget =
            dynamic_cast<GraphWidget *>(mpTabWidget->widget(index));
        graphWidget->refresh(false);
    }
}

GraphWidget* GraphWidgetManager::currentGraphWidget()
{
    return dynamic_cast<GraphWidget*>(mpTabWidget->currentWidget());
}

} // end namespace gui
} // end namespace graph_analysis
