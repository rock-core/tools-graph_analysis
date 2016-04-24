#include "QTaskPreview.hpp"

#include "ui_QTaskPreview.h"

#include <QDebug>
#include <QTreeWidgetItem>

#include <graph_analysis/gui/RegisterQtMetatypes.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>
#include <graph_analysis/task_graph/TaskTemplate.hpp>
#include <graph_analysis/task_graph/Task.hpp>

#include <base/Logging.hpp>

QTaskPreview::QTaskPreview(QWidget* parent)
    : QWidget(parent)
    , mpUi(new Ui::QTaskPreview)
{
    mpUi->setupUi(this);

    mpUi->url->clear();
    mpUi->url->setTextFormat(Qt::RichText);
    mpUi->url->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mpUi->url->setOpenExternalLinks(true);
}

QTaskPreview::~QTaskPreview()
{
}

void QTaskPreview::updatePreview(QTreeWidgetItem* current, QTreeWidgetItem*)
{
    if(current)
    {
        graph_analysis::task_graph::TaskTemplate::Ptr templ =
            graph_analysis::dynamic_pointer_cast<
                graph_analysis::task_graph::TaskTemplate>(
                current->data(0, Qt::UserRole)
                    .value<graph_analysis::Vertex::Ptr>());

        mpUi->url->setText(
            QString("<a "
                    "href=\"http://rock-robotics.org/stable/tasks/"
                    "%1.html\">%2</a>")
                .arg(QString::fromStdString(templ->toString()))
                .arg(QString::fromStdString(templ->toString())));

        graph_analysis::task_graph::Task::Ptr task;
        graph_analysis::VertexIterator::Ptr it =
            templ->getInternalBaseGraph()->getVertexIterator();
        while(it->next())
        {
            if(it->current()->getClassName() ==
               graph_analysis::task_graph::Task::vertexType())
            {
                task = graph_analysis::dynamic_pointer_cast<
                    graph_analysis::task_graph::Task>(it->current());
                break;
            }
        }
        mpUi->outputPorts->setText(QString("%1").arg(
            task->getOutputPorts(templ->getInternalBaseGraph()).size()));
        mpUi->inputPorts->setText(QString("%1").arg(
            task->getInputPorts(templ->getInternalBaseGraph()).size()));
    }
    else
    {
        mpUi->url->clear();
        mpUi->inputPorts->clear();
        mpUi->outputPorts->clear();
    }
}
