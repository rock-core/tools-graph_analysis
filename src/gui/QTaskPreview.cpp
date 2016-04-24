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
        graph_analysis::Vertex::Ptr vertex =
            current->data(0, Qt::UserRole).value<graph_analysis::Vertex::Ptr>();

        mpUi->url->setText(
            QString("<a "
                    "href=\"http://rock-robotics.org/stable/tasks/"
                    "%1.html\">Browse Online</a>")
                .arg(QString::fromStdString(vertex->toString())));
    }
    else
    {
        mpUi->url->clear();
    }
}
