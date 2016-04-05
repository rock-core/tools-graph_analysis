#include "TaskTemplateContainer.hpp"

namespace graph_analysis
{
namespace task_graph
{

bool TaskTemplateContainer::add(TaskTemplate::Ptr templ)
{
    if (storage.count(templ->yamlFileName()) > 0)
        return false;
    else
        storage[templ->yamlFileName()] = templ;
    return true;
}

bool TaskTemplateContainer::add(const std::string& yamlFileName)
{
    TaskTemplate::Ptr templ = TaskTemplate::Ptr(new TaskTemplate(yamlFileName));
    return add(templ);
}

void TaskTemplateContainer::remove(TaskTemplate::Ptr templ)
{
    storage.erase(templ->yamlFileName());
}

TaskTemplate::Ptr TaskTemplateContainer::find(const std::string& name)
{
    std::map<std::string, TaskTemplate::Ptr>::const_iterator it;
    if ((it = storage.find(name)) != storage.end())
        return it->second;
    else
        return TaskTemplate::Ptr();
}

void TaskTemplateContainer::clear()
{
    storage.clear();
}

}
}
