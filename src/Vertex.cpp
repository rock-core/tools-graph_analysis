#include "Vertex.hpp"
#include "BaseGraph.hpp"

namespace graph_analysis {


Vertex::Vertex(const std::string& label)
    : GraphElement(label)
{}

Vertex::Ptr Vertex::clone() const
{
    Vertex::Ptr vertex( getClone());
    vertex->disassociateFromAll();
    return vertex;
}

Vertex::Ptr Vertex::getSharedPointerFromGraph(const shared_ptr<BaseGraph> &pGraph) const
{
    return const_pointer_cast<Vertex>( getPtr() );
}

Vertex::Ptr Vertex::getPtr()
{
    return dynamic_pointer_cast<Vertex>(shared_from_this());
}

Vertex::ConstPtr Vertex::getPtr() const
{
    return dynamic_pointer_cast<const Vertex>(shared_from_this());
}

std::string Vertex::toString() const
{
    return toString(0);
}

std::string Vertex::toString(uint32_t indent) const
{
    std::string hspace(indent,' ');
    if(!mLabel.empty())
    {
        return hspace + mLabel;
    }
    else
    {
        return hspace + getClassName();
    }
}

} // end namespace graph_analysis
