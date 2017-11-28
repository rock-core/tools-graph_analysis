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
#include <QFileInfo>

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
    // https://joekuan.wordpress.com/2015/09/23/list-of-qt-icons/
    QAction *actionImport = comm.addAction("Import", SLOT(importGraph()), style->standardIcon(QStyle::SP_FileIcon)        , QKeySequence( QKeySequence::Open ), tr("Import graph from file"));
    QAction *actionExport = comm.addAction("Export", SLOT(exportGraph()), style->standardIcon(QStyle::SP_DialogSaveButton), QKeySequence( QKeySequence::SaveAs), tr("Export graph to file"));
    QAction *selectLayout = comm.addAction("Layout", SLOT(selectLayout()), style->standardIcon(QStyle::SP_FileDialogListView), QKeySequence( Qt::ControlModifier & Qt::Key_L), tr("Export graph to file"));
    QAction *saveScene = comm.addAction("Save Scene", SLOT(saveScene()), style->standardIcon(QStyle::SP_DialogSaveButton), QKeySequence(Qt::ControlModifier & Qt::Key_E), tr("Save scene"));
    fileMenu->addAction(actionImport);
    fileMenu->addAction(actionExport);
    fileMenu->addAction(selectLayout);
    fileMenu->addAction(saveScene);
    fileMenu->addSeparator();

    // Populate the recent files list
    for(int i = 0; i < MaxRecentFiles; ++i)
    {
        QAction* action = new QAction(this);
        action->setVisible(false);
        mpRecentFileActions.push_back(action);
        connect(action, SIGNAL(triggered()), this, SLOT(importRecentFile()));
    }

    // account only for the existing files
    int existingRecentFiles = qMin(mpRecentFileActions.size(), (int) MaxRecentFiles);
    for(int i = 0; i < existingRecentFiles; ++i)
    {
        fileMenu->addAction(mpRecentFileActions[i]);
    }
    fileMenu->addSeparator();

    QMenu *viewMenu = new QMenu(QObject::tr("&View"));
    QAction *playGraph = comm.addAction("Play", SLOT(playGraph()), style->standardIcon(QStyle::SP_MediaSeekForward), QKeySequence( Qt::ControlModifier & Qt::Key_P), tr("Play a graph"));
    QAction *reversePlayGraph = comm.addAction("Reverse Play", SLOT(reversePlayGraph()), style->standardIcon(QStyle::SP_MediaSeekBackward), QKeySequence( Qt::ControlModifier & Qt::Key_P), tr("Reverse play a graph"));
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

    updateRecentFileActions();
}

GraphAnalysisGui::~GraphAnalysisGui()
{
    delete mpUi;
}

void GraphAnalysisGui::importGraph()
{
    graph_analysis::BaseGraph::Ptr graph = graph_analysis::gui::dialogs::IODialog::importGraph(this);
    activateGraph(graph);

    updateRecentFileActions();
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

void GraphAnalysisGui::saveScene()
{
    if(mpUi->tabWidget->currentWidget() == mpBaseGraphView)
    {
        mpBaseGraphView->saveScene();
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

void GraphAnalysisGui::updateRecentFileActions()
{
    QSettings settings(QCoreApplication::organizationName(), "IO");
    QStringList files = settings.value("recentImportFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int) MaxRecentFiles);
    for(int i = 0; i < numRecentFiles; ++i)
    {
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        mpRecentFileActions[i]->setText(text);
        mpRecentFileActions[i]->setData(files[i]);
        mpRecentFileActions[i]->setVisible(true);
    }
    for(int j = numRecentFiles; j < MaxRecentFiles; ++j)
    {
        mpRecentFileActions[j]->setVisible(false);
    }
}

void GraphAnalysisGui::importRecentFile()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if(action)
    {
        qDebug() << "Importing file from: " << action->data().toString();
        graph_analysis::BaseGraph::Ptr graph = graph_analysis::gui::dialogs::IODialog::importGraph(this, action->data().toString());

        activateGraph(graph);
    }
}

void GraphAnalysisGui::activateGraph(graph_analysis::BaseGraph::Ptr& graph)
{
    if(graph)
    {
        mpBaseGraphView->clearVisualization();
        mpBaseGraph = graph;

        delete mpQBaseGraph;
        mpQBaseGraph = new QBaseGraph(mpBaseGraph);
        mpBaseGraphView->setGraph(mpBaseGraph);
        mpBaseGraphView->refresh();
        mpBaseGraphView->updateVisualization();
        mpBaseGraphView->applyLayout("dot");
        mpBaseGraphView->updateVisualization();
    }
}

QString GraphAnalysisGui::strippedName(const QString& fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

} // end namespace gui
} // end namespace graph_analysis
