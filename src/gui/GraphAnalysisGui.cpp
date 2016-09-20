#include "GraphAnalysisGui.hpp"

#include "ui_GraphAnalysisGui.h"

#include <graph_analysis/gui/BaseGraphView/BaseGraphView.hpp>
#include <graph_analysis/VertexTypeManager.hpp>

#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QCommonStyle>
#include <QToolBar>
#include <QInputDialog>

#include <base-logging/Logging.hpp>

#include <graph_analysis/GraphIO.hpp>
#include <graph_analysis/io/YamlWriter.hpp>
#include <graph_analysis/io/GexfWriter.hpp>
#include <graph_analysis/io/GexfReader.hpp>
#include <graph_analysis/io/YamlReader.hpp>
#include <graph_analysis/io/GraphvizWriter.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>
#include <graph_analysis/gui/dialogs/ExportFile.hpp>
#include <graph_analysis/gui/ActionCommander.hpp>

namespace graph_analysis
{
namespace gui
{

GraphAnalysisGui::GraphAnalysisGui()
    : QMainWindow()
    , mpUi(new Ui::GraphAnalysisGui)
    , mpQBaseGraph(new QBaseGraph(this))
    , mpBaseGraphView(new BaseGraphView(mpQBaseGraph->getBaseGraph(), this))
{
    mpUi->setupUi(this);
    mpUi->tabWidget->clear();
    mpUi->tabWidget->addTab(mpBaseGraphView, mpBaseGraphView->getClassName());

    // and show both' widgets status-messages on the statusbar. this simply
    // assumes that only the one in the front is sending updates. otherwise
    // they would interleave...
    connect(mpBaseGraphView, SIGNAL(currentStatus(QString, int)),
            mpUi->statusbar, SLOT(showMessage(QString, int)));

    connect(mpQBaseGraph, SIGNAL(graphChanged()),
            this, SLOT(updateVisualization()));

    ActionCommander comm(this);
    QMenu *fileMenu = new QMenu(QObject::tr("&File"));
    QStyle* style = new QCommonStyle();
    QAction *actionImport = comm.addAction("Import", SLOT(importGraph()), style->standardIcon(QStyle::SP_FileIcon)        , QKeySequence( QKeySequence::Open ), tr("Import graph from file"));
    QAction *actionExport = comm.addAction("Export", SLOT(exportGraph()), style->standardIcon(QStyle::SP_DialogSaveButton), QKeySequence( QKeySequence::SaveAs), tr("Export graph to file"));
    QAction *selectLayout = comm.addAction("Layout", SLOT(selectLayout()), style->standardIcon(QStyle::SP_FileDialogListView), QKeySequence( Qt::ControlModifier & Qt::Key_L), tr("Export graph to file"));

    fileMenu->addAction(actionImport);
    fileMenu->addAction(actionExport);
    fileMenu->addAction(selectLayout);
    fileMenu->addSeparator();

    QToolBar* toolBar = new QToolBar("Toolbar");
    toolBar->addAction(actionImport);
    toolBar->addAction(actionExport);
    toolBar->addAction(selectLayout);
    toolBar->setFloatable(true);
    addToolBar(toolBar);
}

GraphAnalysisGui::~GraphAnalysisGui()
{
    delete mpUi;
}

void GraphAnalysisGui::importGraph()
{
    // updateStatus("importing graph...");
    QString selectedFilter;
    // Constructing the writer suffix filter
    io::GraphIO::SuffixMap suffixMap = io::GraphIO::getSuffixMap();
    io::GraphIO::ReaderMap readerMap = io::GraphIO::getReaderMap();
    io::GraphIO::ReaderMap::const_iterator rit = readerMap.begin();

    std::stringstream ss;
    for(;;)
    {
        ss << representation::TypeTxt[rit->first] << " (";
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
        }
        else
        {
            break;
        }
    }
    // End constructing the writer suffix filter

    QString filename = QFileDialog::getOpenFileName(
        this, tr("Choose input file"), QDir::currentPath(),
        tr(ss.str().c_str()), &selectedFilter);

    if(!filename.isEmpty())
    {
        fromFile(filename.toStdString());
    }
    else
    {
        /* updateStatus("Failed to import graph: aborted by user - an empty
         * input filename was provided"); */
    }
}

void GraphAnalysisGui::exportGraph()
{
    /* updateStatus("Exporting graph..."); */
    if(mpQBaseGraph->getBaseGraph()->empty())
    {
        QMessageBox::critical(this, tr("Graph Export Failed"),
                              "Graph is empty");
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
        ss << representation::TypeTxt[wit->first] << " (";
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
        }
        else
        {
            break;
        }
    }
    // End constructing the writer suffix filter

    dialogs::ExportFile dialog(ss.str().c_str());
    if(dialog.exec() == QFileDialog::Accepted)
    {
        try
        {
            io::GraphIO::write(dialog.getFilename().toStdString(), mpQBaseGraph->getBaseGraph(),
                               dialog.getTypeName());
            /* updateStatus("Exported graph to output file '" +
             * dialog.getFilename().toStdString() + "'"); */
        }
        catch(const std::exception& e)
        {
            std::string msg = "Export of graph to '" +
                              dialog.getFilename().toStdString() + "' failed " +
                              e.what();
            QMessageBox::critical(this, tr("Graph Export Failed"), msg.c_str());
            return;
        }
    }
    else
    {
        /* updateStatus("Exporting graph aborted by user"); */
    }
}

void GraphAnalysisGui::fromFile(const std::string& filename)
{
    graph_analysis::BaseGraph::Ptr graph = graph_analysis::BaseGraph::getInstance();
    try
    {
        io::GraphIO::read(filename, graph);
    }
    catch(const std::exception& e)
    {
        std::string msg = "Failed to import '" + filename + "': " + e.what();
        QMessageBox::critical(this, tr("Graph Import Failed"), msg.c_str());
        return;
    }

    mpBaseGraph = graph;

    delete mpQBaseGraph;
    mpQBaseGraph = new QBaseGraph(mpBaseGraph);

    mpBaseGraphView->setGraph(mpBaseGraph);
    mpBaseGraphView->clearVisualization();
    mpBaseGraphView->refresh();
    mpBaseGraphView->updateVisualization();

}

void GraphAnalysisGui::selectLayout()
{
    if(mpUi->tabWidget->currentWidget() == mpBaseGraphView)
    {
        bool ok;
        QString desiredLayout = QInputDialog::getItem(this, tr("Select Layout"),
                                    tr("select a layout:"), mpBaseGraphView->getSupportedLayouts(),
                                    0, false, &ok);
        if(ok)
        {
            mpBaseGraphView->applyLayout(desiredLayout.toStdString());
        }
    }
    updateVisualization();
}

void GraphAnalysisGui::on_tabWidget_currentChanged(int index)
{
    // When the tab changed, we want to update the widget
    this->updateVisualization();
}

void GraphAnalysisGui::updateVisualization()
{
    // Call the current tab widget's update function
    if (mpUi->tabWidget->currentWidget() == mpBaseGraphView)
    {
        mpBaseGraphView->updateVisualization();
    }
}

} // end namespace gui
} // end namespace graph_analysis
