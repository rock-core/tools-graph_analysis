#include "TaskGraphEditor.hpp"
#include "TaskGraphViewer.hpp"

#include "ui_TaskGraphEditor.h"
#include <QMessageBox>
#include <QFileDialog>

#include <graph_analysis/task_graph/TaskTemplateContainer.hpp>

namespace graph_analysis
{
    namespace gui
    {
        TaskGraphEditor::TaskGraphEditor(graph_analysis::BaseGraph::Ptr graph, QWidget *parent)
            : QWidget(parent)
            , mpUi(new Ui::TaskGraphEditor)
            , mpGraph(graph)
            , mpTaskContainer(new task_graph::TaskTemplateContainer())
        {
            mpUi->setupUi(this);
            mpTaskGraphViewer = new TaskGraphViewer(mpGraph);
            mpUi->placeHolder->addWidget(mpTaskGraphViewer);

            mpUi->taskTemplateTree->clear();
            mpRootItem = new QTreeWidgetItem(mpUi->taskTemplateTree);
            mpRootItem->setText(0, QString("Task Templates"));

            connect(mpTaskGraphViewer, SIGNAL(baseGraphChanged()),
                    this, SLOT(baseGraphChanged_internal()));
            connect(mpTaskGraphViewer, SIGNAL(currentStatus(QString,int)),
                    this, SLOT(currentStatus_internal(QString,int)));
        }

        TaskGraphEditor::~TaskGraphEditor()
        {
            delete mpTaskContainer;
            delete mpUi;
        }

        void TaskGraphEditor::baseGraphChanged_internal()
        {
            emit baseGraphChanged();
        }

        void TaskGraphEditor::currentStatus_internal(QString msg, int lvl)
        {
            emit currentStatus(msg,lvl);
        }

        void TaskGraphEditor::updateVisualization()
        {
            mpTaskGraphViewer->updateVisualization();
        }

        void TaskGraphEditor::on_taskTemplateTree_itemDoubleClicked(QTreeWidgetItem *item, int column)
        {
            if (!item)
                return;

            task_graph::TaskTemplate::Ptr templ = mpTaskContainer->find(item->text(1).toStdString());
            if (templ)
            {
                templ->instantiateAndAddTask(mpGraph);
                updateVisualization();
                baseGraphChanged_internal();
            }
        }

        void TaskGraphEditor::on_addButton_clicked()
        {
            QString filename = QFileDialog::getOpenFileName(
                this, tr("Choose orogen model file"), QDir::currentPath(),
                tr("Orogen Model Files (*.yml)"));

            if(!filename.isEmpty())
            {
                // Ok, user has selected a file
                if (!mpTaskContainer->add(filename.toStdString()))
                {
                    std::string msg = "Failed to import '" + filename.toStdString() + "'";
                    QMessageBox::critical(this, tr("Model import failed"), msg.c_str());
                    return;
                }

                QTreeWidgetItem *child = new QTreeWidgetItem();
                task_graph::Task::Ptr task = mpTaskContainer->find(filename.toStdString())->rootVertex();
                child->setText(0, QString::fromStdString(task->toString()));
                child->setText(1, filename);
                mpRootItem->addChild(child);

                //updateTreeWidget();
            }
        }

        void TaskGraphEditor::on_removeButton_clicked()
        {
            QTreeWidgetItem *item = mpUi->taskTemplateTree->currentItem();
            if (!item)
                return;

            task_graph::TaskTemplate::Ptr templ = mpTaskContainer->find(item->text(1).toStdString());
            if (templ)
            {
                item->parent()->removeChild(item);
                delete item;
                mpTaskContainer->remove(templ);
                //updateTreeWidget();
            }
        }
    }
}
