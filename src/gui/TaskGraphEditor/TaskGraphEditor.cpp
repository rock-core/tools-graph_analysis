#include "TaskGraphEditor.hpp"
#include "TaskGraphViewer.hpp"

// QT specific includes
#include "ui_TaskGraphEditor.h"
#include <QDirIterator>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcessEnvironment>
#include <QtDebug>

// Rock specific includes
#include <base/Logging.hpp>

#include <graph_analysis/io/CndModelReader.hpp>
#include <graph_analysis/io/CndModelWriter.hpp>
#include <graph_analysis/task_graph/TaskTemplate.hpp>

namespace graph_analysis
{
namespace gui
{
TaskGraphEditor::TaskGraphEditor(graph_analysis::BaseGraph::Ptr graph,
                                 QWidget* parent)
    : QWidget(parent)
    , mpUi(new Ui::TaskGraphEditor)
    , mpGraph(graph)
{
    mpUi->setupUi(this);
    mpTaskGraphViewer = new TaskGraphViewer(mpGraph);
    mpUi->placeHolder->addWidget(mpTaskGraphViewer);
    mpUi->splitter->setSizes(QList<int>() << 10 << 1000);

    mpRootItem = new QTreeWidgetItem(mpUi->taskTemplateTree);
    mpRootItem->setText(0, QString("Task Templates"));
    mpRootItem->setExpanded(true);

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
    delete mpUi;
}

void TaskGraphEditor::currentStatus_internal(QString msg, int lvl)
{
    emit currentStatus(msg, lvl);
}

void TaskGraphEditor::updateVisualization()
{
    mpTaskGraphViewer->updateVisualization();
}

void TaskGraphEditor::on_loadButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
        this, tr("Choose component network description file"),
        QDir::currentPath(),
        tr("Component Network Definition File (*.yaml *.yml)"));

    if(!filename.isEmpty())
    {
        io::CndModelReader reader;
        reader.read(filename.toStdString(), mpGraph);
    }
}

void TaskGraphEditor::on_saveButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(
        this, tr("Choose component network description file"),
        QDir::currentPath(),
        tr("Component Network Definition File (*.yaml *.yml)"));

    if(!filename.isEmpty())
    {
        io::CndModelWriter writer;
        writer.write(filename.toStdString(), mpGraph);
    }
}

// FIXME: This should be a member of BaseGraph
template <typename T>
typename T::Ptr getVertexByLabel(const std::string& label, BaseGraph::Ptr graph)
{
    VertexIterator::Ptr it = graph->getVertexIterator();
    while(it->next())
    {
        if(it->current()->getClassName() == T::vertexType())
            if(it->current()->getLabel() == label)
                return dynamic_pointer_cast<T>(it->current());
    }
    return typename T::Ptr();
}

void TaskGraphEditor::on_taskTemplateTree_itemDoubleClicked(
    QTreeWidgetItem* item, int column)
{
    if(!item)
        return;

    task_graph::TaskTemplate::Ptr templ =
        getVertexByLabel<task_graph::TaskTemplate>(item->text(0).toStdString(),
                                                   mpGraph);
    if(templ)
    {
        templ->instantiateAndAddTo(mpGraph);
        updateVisualization();
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
    // At first we create a new template
    task_graph::TaskTemplate::Ptr newTemplate =
        task_graph::TaskTemplate::Ptr(new task_graph::TaskTemplate());
    newTemplate->loadFromFile(filename.toStdString());

    // Check if template already exists
    task_graph::TaskTemplate::Ptr templ =
        getVertexByLabel<task_graph::TaskTemplate>(newTemplate->getLabel(),
                                                   mpGraph);
    if(templ)
    {
        std::string msg = "Failed to import '" + filename.toStdString() + "'";
        QMessageBox::critical(this, tr("Model import failed"), msg.c_str());
        return false;
    }

    // It is a new template, so we are happy :)
    mpGraph->addVertex(newTemplate);
    QTreeWidgetItem* child = new QTreeWidgetItem();
    child->setText(0, QString::fromStdString(newTemplate->getLabel()));
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
        getVertexByLabel<task_graph::TaskTemplate>(item->text(0).toStdString(),
                                                   mpGraph);
    if(templ)
    {
        item->parent()->removeChild(item);
        delete item;
        mpGraph->removeVertex(templ);
    }
}
}
}
