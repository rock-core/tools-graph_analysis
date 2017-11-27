#include "GraphAnalysisGui.hpp"

#include "ui_GraphAnalysisGui.h"

#include "BaseGraphView/BaseGraphView.hpp"
#include "../VertexTypeManager.hpp"

#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QCommonStyle>
#include <QToolBar>
#include <QInputDialog>

#include <base-logging/Logging.hpp>

#include "GraphWidget.hpp"
#include "ActionCommander.hpp"
#include "dialogs/IODialog.hpp"
#include "Player.hpp"

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
    init();
}

GraphAnalysisGui::GraphAnalysisGui( BaseGraph::Ptr graph )
    : QMainWindow()
    , mpUi(new Ui::GraphAnalysisGui)
    , mpQBaseGraph(new QBaseGraph(graph, this))
    , mpBaseGraphView(new BaseGraphView(mpQBaseGraph->getBaseGraph(), this))
{
    init();
}

void GraphAnalysisGui::init()
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

    QMenu *viewMenu = new QMenu(QObject::tr("&View"));
    QAction *playGraph = comm.addAction("Play", SLOT(playGraph()), style->standardIcon(QStyle::SP_FileDialogListView), QKeySequence( Qt::ControlModifier & Qt::Key_P), tr("Play a graph"));
    QAction *reversePlayGraph = comm.addAction("Reverse Play", SLOT(reversePlayGraph()), style->standardIcon(QStyle::SP_FileDialogListView), QKeySequence( Qt::ControlModifier & Qt::Key_P), tr("Reverse play a graph"));
    viewMenu->addAction(playGraph);
    viewMenu->addAction(reversePlayGraph);

    QMenuBar* bar = menuBar();
    bar->addMenu(fileMenu);
    bar->addMenu(viewMenu);

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
    BaseGraph::Ptr graph = dialogs::IODialog::importGraph(this);
    if(!graph)
    {
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

void GraphAnalysisGui::exportGraph()
{
    dialogs::IODialog::exportGraph(mpQBaseGraph->getBaseGraph(), this);
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

void GraphAnalysisGui::reversePlayGraph()
{
    if(mpUi->tabWidget->currentWidget() == mpBaseGraphView)
    {
        PlayerConfiguration configuration;
        configuration.setShowItem(false);

        mpBaseGraphView->play(configuration);
    }
}

void GraphAnalysisGui::playGraph()
{
    if(mpUi->tabWidget->currentWidget() == mpBaseGraphView)
    {
        mpBaseGraphView->play( PlayerConfiguration() );
    }
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
