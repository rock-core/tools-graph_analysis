#ifndef GRAPH_ANALYSIS_GUI_TASKGRAPHEDITOR_HPP
#define GRAPH_ANALYSIS_GUI_TASKGRAPHEDITOR_HPP

#include <graph_analysis/Graph.hpp>
#include <QWidget>
#include <QTreeWidget>

namespace Ui
{
    class TaskGraphEditor;
}

namespace graph_analysis
{
    namespace task_graph
    {
        class TaskTemplateContainer;
    }

    namespace gui
    {
        class TaskGraphViewer;

        class TaskGraphEditor : public QWidget
        {
            Q_OBJECT

            public:
                TaskGraphEditor(graph_analysis::BaseGraph::Ptr graph, QWidget *parent = NULL);
                ~TaskGraphEditor();

                QString getClassName() const
                { return "graph_analysis::gui::TaskGraphEditor"; }

            private:
                // GUI Elements
                Ui::TaskGraphEditor*                mpUi;

                // Graph
                graph_analysis::BaseGraph::Ptr      mpGraph;
                TaskGraphViewer*                    mpTaskGraphViewer;

                // Task Template stuff
                task_graph::TaskTemplateContainer*  mpTaskContainer;
                QTreeWidgetItem*                    mpRootItem;

                /** adding one new file into the task-container and the tree widget */
                bool addFile(QString filename);

            signals:
                // Will be triggered by the TaskGraphViewer
                void baseGraphChanged();
                void currentStatus(QString, int);

            private slots:
                // Needed to observe the TaskGraphViewer
                void baseGraphChanged_internal();
                void currentStatus_internal(QString, int);

            public slots:
                // Adding/Removing TaskTemplates from Container
                void on_addButton_clicked();
                void on_removeButton_clicked();

                // On double click we want to create new tasks :)
                void on_taskTemplateTree_itemDoubleClicked(QTreeWidgetItem* item, int column);

                // For updating the TaskGraphViewer
                void updateVisualization();
        };
    }
}

#endif
