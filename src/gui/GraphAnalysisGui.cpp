#include "GraphAnalysisGui.hpp"

#include "ui_GraphAnalysisGui.h"

#include <graph_analysis/gui/BaseGraphView/BaseGraphView.hpp>
#include <graph_analysis/gui/ComponentGraphEditor/ComponentGraphEditor.hpp>
#include <graph_analysis/VertexTypeManager.hpp>

#include <QDebug>

namespace graph_analysis
{
namespace gui
{

GraphAnalysisGui::GraphAnalysisGui()
    : QMainWindow()
    , mpUi(new Ui::GraphAnalysisGui)
    , mpBaseGraphView(new BaseGraphView(this))
    , mpComponentGraphEditor(new ComponentGraphEditor(this))
{

    mpGraph = BaseGraph::getInstance();

    mpBaseGraphView->setGraph(mpGraph);
    mpComponentGraphEditor->setGraph(mpGraph);

    mpUi->setupUi(this);
    mpUi->tabWidget->clear();
    mpUi->tabWidget->addTab(mpBaseGraphView, mpBaseGraphView->getClassName());
    mpUi->tabWidget->addTab(mpComponentGraphEditor,
                            mpComponentGraphEditor->getClassName());

    // bidrectional connection of "change->update" cycle between the two widgets
    connect(mpBaseGraphView, SIGNAL(baseGraphChanged()), mpComponentGraphEditor,
            SLOT(updateVisualization()));
    connect(mpComponentGraphEditor, SIGNAL(baseGraphChanged()), mpBaseGraphView,
            SLOT(updateVisualization()));

    // and show both' widgets status-messages on the statusbar. this simply
    // assumes that only the one in the front is sending updates. otherwise
    // they would interleave...
    connect(mpBaseGraphView, SIGNAL(currentStatus(QString, int)),
            mpUi->statusbar, SLOT(showMessage(QString, int)));
    connect(mpComponentGraphEditor, SIGNAL(currentStatus(QString, int)),
            mpUi->statusbar, SLOT(showMessage(QString, int)));
}

GraphAnalysisGui::~GraphAnalysisGui()
{
    delete mpUi;
}

} // end namespace gui
} // end namespace graph_analysis
