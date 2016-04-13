#ifndef GRAPH_ANALYSIS_TASK_GRAPH_TASK_TEMPLATE_HPP
#define GRAPH_ANALYSIS_TASK_GRAPH_TASK_TEMPLATE_HPP

#include <graph_analysis/Graph.hpp>
#include "Task.hpp"

namespace graph_analysis
{
    class BaseGraph;

namespace task_graph
{
    class TaskTemplate
    {
        public:
            typedef shared_ptr< TaskTemplate > Ptr;
            /**
             * The default constructor needs a filename to import a base graph model
             */
            TaskTemplate(const std::string &yamlFileName);

            /**
             * If called, a new instance of the stored BaseGraph is created.
             */
            BaseGraph::Ptr instantiateTask();

            /**
             * If called, creates a new instance and adds it to the given graph argument
             */
            void instantiateAndAddTask(BaseGraph::Ptr graph);

            std::string const &yamlFileName() const
            {
                return mYamlFileName;
            }

            const Task::Ptr rootVertex()
            {
                return mpRootVertex;
            }

        private:
            std::string     mLabel;
            std::string     mYamlFileName;

            int mInstance;
            BaseGraph::Ptr  mpBaseGraph;
            Task::Ptr       mpRootVertex;
    };

}
}
#endif
