#include "TaskGraphEditor.hpp"
#include "TaskGraphViewer.hpp"

// QT specific includes
#include "ui_TaskGraphEditor.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDirIterator>
#include <QProcessEnvironment>
#include <QtDebug>

// Rock specific includes
#include <base/Logging.hpp>

#include <graph_analysis/task_graph/TaskTemplateContainer.hpp>

namespace graph_analysis
{
namespace gui
{
TaskGraphEditor::TaskGraphEditor(graph_analysis::BaseGraph::Ptr graph,
                                 QWidget* parent)
    : QWidget(parent)
    , mpUi(new Ui::TaskGraphEditor)
    , mpGraph(graph)
    , mpTaskContainer(new task_graph::TaskTemplateContainer())
{
    mpUi->setupUi(this);
    mpTaskGraphViewer = new TaskGraphViewer(mpGraph);
    mpUi->placeHolder->addWidget(mpTaskGraphViewer);
    mpUi->splitter->setSizes(QList<int>() << 10 << 1000);

    mpRootItem = new QTreeWidgetItem(mpUi->taskTemplateTree);
    mpRootItem->setText(0, QString("Task Templates"));
    mpRootItem->setExpanded(true);

    connect(mpTaskGraphViewer, SIGNAL(baseGraphChanged()), this,
            SLOT(baseGraphChanged_internal()));
    connect(mpTaskGraphViewer, SIGNAL(currentStatus(QString, int)), this,
            SLOT(currentStatus_internal(QString, int)));

    // Get all system environment variables
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    // Check whether the environment variable to the INSTALL directory exists,
    // i.e. whether the env.sh has been sourced or not
    if(env.contains("AUTOPROJ_CURRENT_ROOT"))
    {
        // Expand the environment variable and add the folders to the yml models
        QString PathToYmlModels =
            qgetenv("AUTOPROJ_CURRENT_ROOT") + "/install/share/orogen/models";

        // Log the anomaly
        LOG_INFO("Checking for orogen yml files in: %s\n",
                 PathToYmlModels.toUtf8().constData());

        // Check if the folder to the yml model files exists
        if(QDir(PathToYmlModels).exists())
        {
            // Get an iterator over all elements in model folder
            QDirIterator it(PathToYmlModels);
            while(it.hasNext())
            {
                it.next();
                if(it.fileInfo().suffix() == "yml")
                {
                    addFile(it.fileInfo().absoluteFilePath());
                }
            }
        }
        else
        {
            // Log the anomaly
            LOG_WARN("The orogen yml model folder does not exist: %s\n",
                     PathToYmlModels.toUtf8().constData());

            // Notify the user
            QMessageBox* msgBox = new QMessageBox(this);
            msgBox->setAttribute(Qt::WA_DeleteOnClose);
            msgBox->setWindowTitle(tr("Folder not found"));
            msgBox->setText(tr("The orogen yml model folder does not exist!"));
            msgBox->setIcon(QMessageBox::Warning);
            msgBox->show();
        }
    }
    else
    {
        // Log the anomaly
        LOG_WARN("The 'AUTOPROJ_CURRENT_ROOT' has not been found - check if "
                 "'env.sh' has been sourced.\n");

        // Notify the user
        QMessageBox* msgBox = new QMessageBox(this);
        msgBox->setAttribute(Qt::WA_DeleteOnClose);
        msgBox->setWindowTitle(tr("Missing Environment Variable"));
        msgBox->setText(
            tr("The 'AUTOPROJ_CURRENT_ROOT' has not been found - check "
               "if 'env.sh' has been sourced."));
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->show();
    }
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
    emit currentStatus(msg, lvl);
}

void TaskGraphEditor::updateVisualization()
{
    mpTaskGraphViewer->updateVisualization();
}

void TaskGraphEditor::on_taskTemplateTree_itemDoubleClicked(
    QTreeWidgetItem* item, int column)
{
    if(!item)
        return;

    task_graph::TaskTemplate::Ptr templ =
        mpTaskContainer->find(item->text(1).toStdString());
    if(templ)
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
        addFile(filename);
    }
}

bool TaskGraphEditor::addFile(QString filename)
{
    // Ok, user has selected a file
    if(!mpTaskContainer->add(filename.toStdString()))
    {
        std::string msg = "Failed to import '" + filename.toStdString() + "'";
        QMessageBox::critical(this, tr("Model import failed"), msg.c_str());
        return false;
    }

    QTreeWidgetItem* child = new QTreeWidgetItem();
    task_graph::Task::Ptr task =
        mpTaskContainer->find(filename.toStdString())->rootVertex();
    child->setText(0, QString::fromStdString(task->toString()));
    child->setText(1, filename);
    mpRootItem->addChild(child);
    return true;
}

void TaskGraphEditor::on_removeButton_clicked()
{
    QTreeWidgetItem* item = mpUi->taskTemplateTree->currentItem();
    if(!item)
        return;

    task_graph::TaskTemplate::Ptr templ =
        mpTaskContainer->find(item->text(1).toStdString());
    if(templ)
    {
        item->parent()->removeChild(item);
        delete item;
        mpTaskContainer->remove(templ);
        // updateTreeWidget();
    }
}
}
}
