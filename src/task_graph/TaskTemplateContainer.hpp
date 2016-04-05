#ifndef GRAPH_ANALYSIS_TASK_GRAPH_TASK_TEMPLATE_CONTAINER_HPP
#define GRAPH_ANALYSIS_TASK_GRAPH_TASK_TEMPLATE_CONTAINER_HPP

#include "TaskTemplate.hpp"

namespace graph_analysis
{
namespace task_graph
{
    class TaskTemplateContainer
    {
        public:

            bool add(TaskTemplate::Ptr templ);

            void remove(TaskTemplate::Ptr templ);

            TaskTemplate::Ptr find(const std::string& name);

            void clear();

        private:

            std::map<std::string, TaskTemplate::Ptr> storage;
    };

}
}

#endif
