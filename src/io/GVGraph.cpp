#include "GVGraph.hpp"
#include <sstream>
#include <boost/assign/list_of.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <base/Logging.hpp>
#include <graph_analysis/VertexTypeManager.hpp>

namespace graph_analysis {
namespace io {

/*! Dot uses a 72 DPI value for converting it's position coordinates from points to pixels
    while we display at 96 DPI on most operating systems. */
const double GVGraph::DotDefaultDPI=72.0;

std::set<std::string> GVGraph::msSupportedLayouts = boost::assign::list_of
    ("circo")
    ("dot")
    ("fdp")
    ("neato")
    ("nop")
    ("nop1")
    ("nop2")
    ("osage")
    ("patchwork")
    ("sfdp")
    ("twopi")
    ;

GVGraph::GVGraph(BaseGraph::Ptr baseGraph, const std::string& name, double node_size)
    : mpContext(gvContext())
    , mpGVGraph(_agopen(name, Agdirected)) // see also agstrictdirected graph -- one edgde between two nodes, see libgraph doc
    , mpBaseGraph(baseGraph)
    , mAppliedLayout(false)
{
    //Set graph attributes
    setGraphAttribute("overlap", "false");
    setGraphAttribute("splines", "true");
    setGraphAttribute("pad", "0,2");
    setGraphAttribute("dpi", "96,0");

    setGraphAttribute("concentrate", "false");

    // Don't use node sep here, since it prevents outputting the exact position
    // when using dot layout
    // setGraphAttribute("nodesep", "1");

    //Set default attributes for the future nodes
    setNodeAttribute("fixedsize", "false");
    setNodeAttribute("shape","box");
//    setNodeAttribute("regular", "true");

    //Divide the wanted width by the DPI to get the value in points
    std::string dpi = getGraphAttribute("dpi", "96,0");
    boost::replace_all(dpi, ",",".");
    try {
        mDPI = boost::lexical_cast<double>(dpi);
    } catch(const std::bad_cast& e)
    {
        LOG_WARN_S << e.what() << " dpi: " << dpi;
        mDPI = 96.0;
    }

    mScalingFactor = (1.0*mDPI) / (1.0*DotDefaultDPI);

    double nodePtsWidth = node_size/mDPI;
    std::stringstream ss;
    ss << nodePtsWidth;
    std::string nodePtsWidthTxt = ss.str();
    //GV uses , instead of . for the separator in floats
    boost::replace_all(nodePtsWidthTxt, ".",",");

    setNodeAttribute("width", nodePtsWidthTxt);

    // First you need to make the label 'known' or rather set a default value
    // this value should not be an empty string! Then you can use it via agset
    setEdgeAttribute("label", "o");
}

GVGraph::~GVGraph()
{
    //if(mAppliedLayout)
    //{
    //    gvFreeLayout(mpContext, mpGVGraph);
    //}

    //agclose(mpGVGraph);
    //gvFreeContext(mpContext);

    //mpGVGraph = NULL;
    //mpContext = NULL;
}

int GVGraph::setGraphAttribute(const std::string& name, const std::string& value)
{
    // Directly use agsafeset which always works, contrarily to agset
    return agsafeset(mpGVGraph, const_cast<char *>(name.c_str()),
                     const_cast<char *>(value.c_str()),
                     const_cast<char *>(value.c_str()));
}

std::string GVGraph::getGraphAttribute(const std::string& name, const std::string& defaultValue) const
{
    std::string value = agget(mpGVGraph, const_cast<char *>(name.c_str()));
    if(value.empty())
    {
        return defaultValue;
    }
    return value;
}

void GVGraph::setNodeAttribute(const std::string& name, const std::string& value)
{
    agattr(mpGVGraph, AGNODE, const_cast<char*>(name.c_str()), const_cast<char*>(value.c_str()));
}

void GVGraph::setEdgeAttribute(const std::string& name, const std::string& value)
{
    // first arg: edge
    // second arg: value
    agattr(mpGVGraph, AGEDGE, const_cast<char*>(name.c_str()), const_cast<char*>(value.c_str()));
}

bool GVGraph::setAttribute(const graph_analysis::Vertex::Ptr& vertex, const std::string& name, const std::string& value)
{
    std::map<GraphElementId, Agnode_t*>::const_iterator cit;
    cit = mNodes.find(mpBaseGraph->getVertexId(vertex));
    if(cit == mNodes.end())
    {
        throw std::invalid_argument("graph_analysis::io::GVGraph::setNodeAttribute: vertex '" + vertex->toString() + "' is not part of the graphviz graph");
    }

    Agnode_t* gvNode = cit->second;
    return agsafeset(gvNode, const_cast<char *>(name.c_str()),
                     const_cast<char *>(value.c_str()),
                     const_cast<char *>(value.c_str()));
}

bool GVGraph::setAttribute(const graph_analysis::Edge::Ptr& edge, const std::string& name, const std::string& value)
{
    std::map<GraphElementId, Agedge_t*>::const_iterator cit;
    cit = mEdges.find(mpBaseGraph->getEdgeId(edge));
    if(cit == mEdges.end())
    {
        throw std::invalid_argument("graph_analysis::io::GVGraph::setEdgeAttribute: edge '" + edge->toString() + "' is not part of the graphviz graph");
    }

    Agedge_t* gvEdge = cit->second;
    return agsafeset(gvEdge, const_cast<char *>(name.c_str()),
                     const_cast<char *>(value.c_str()),
                     const_cast<char *>(value.c_str()));
}

GraphElementId GVGraph::addNode(const graph_analysis::Vertex::Ptr& vertex)
{
    GraphElementId id;
    try {
        id = mpBaseGraph->getVertexId(vertex);
    } catch(const std::runtime_error& e)
    {
        throw std::invalid_argument("graph_analysis::io::GVGraph::addNode: vertex '" + vertex->toString() + "' is not part of the underlying graph");
    }

    if(mNodes.count(id))
    {
        removeNode(vertex);
    }

    Agnode_t* node = _agnode(mpGVGraph, getUniqueName(vertex));
    mNodes[id] = node;

    if(mNodes.size() == 1)
    {
        setRootNode(vertex);
    }

    return id;
}

void GVGraph::addNodes(const std::vector<Vertex::Ptr>& vertices)
{
    std::vector<Vertex::Ptr>::const_iterator cit = vertices.begin();
    for(; cit != vertices.end(); ++cit)
    {
        addNode(*cit);
    }
}

void GVGraph::removeNode(const Vertex::Ptr& vertex)
{
    GraphElementId id = mpBaseGraph->getVertexId(vertex);
    return removeNode(id);
}

void GVGraph::removeNode(GraphElementId id)
{
    std::map<GraphElementId, Agnode_t* >::iterator it = mNodes.find(id);
    if(it != mNodes.end())
    {
        std::vector<GraphElementId> edges;

        Agnode_t* node = it->second;
        for (Agedge_t* e = agfstedge(mpGVGraph, node); e; e = agnxtedge(mpGVGraph, e, node))
        {
            GraphElementId id = getId(e);
            edges.push_back(id);
        }

        std::vector<GraphElementId>::iterator eit = edges.begin();
        for(; eit != edges.end(); ++eit)
        {
            GraphElementId id = *eit;
            removeEdge(id);
        }

        agdelnode(mpGVGraph, node);
        mNodes.erase(it);
    }
}

void GVGraph::clearNodes()
{
    while(!mNodes.empty())
    {
        std::map<GraphElementId, Agnode_t*>::iterator it = mNodes.begin();
        removeNode(it->first);
    }
}

void GVGraph::setRootNode(Vertex::Ptr vertex)
{
    GraphElementId id = mpBaseGraph->getVertexId(vertex);
    if(mNodes.count(id))
    {
        setGraphAttribute("root", getUniqueName(vertex));
    }
}

GraphElementId GVGraph::addEdge(const Edge::Ptr& edge)
{
    GraphElementId sourceId = mpBaseGraph->getVertexId(edge->getSourceVertex());
    GraphElementId targetId = mpBaseGraph->getVertexId(edge->getTargetVertex());

    if(mNodes.count(sourceId) && mNodes.count(targetId))
    {
        bool create = true;
        Agedge_t* gvEdge = _agedge(mpGVGraph, mNodes[sourceId], mNodes[targetId], getUniqueName(edge), create);
        // This works after property has been made known via setEdgeAttribute
        agset(gvEdge, "label", const_cast<char*>(getUniqueName(edge).c_str()));

        GraphElementId id;

        try {
            id = mpBaseGraph->getEdgeId(edge);
        } catch(const std::runtime_error& e)
        {
            throw std::invalid_argument("graph_analysis::io::GVGraph::addEdge: edge '" + edge->toString() + "' is not part of the underlying graph");
        }

        mEdges[id] = gvEdge;
        return id;
    }
    throw std::runtime_error("graph_analysis::io::GVGraph::addEdge: failed to add edge, nodes or targets missing");
}

void GVGraph::removeEdge(const Edge::Ptr& edge)
{
    GraphElementId id = mpBaseGraph->getEdgeId(edge);
    return removeEdge(id);
}

void GVGraph::removeEdge(GraphElementId id)
{
    std::map<GraphElementId, Agedge_t*>::iterator it = mEdges.find(id);
    if(it != mEdges.end())
    {
        agdeledge(mpGVGraph, it->second);
        mEdges.erase(it);
        return;
    }
    throw std::invalid_argument("graph_analysis::io::GVGraph::removeEdge could not remove edge -- id not found");
}

void GVGraph::clearEdges()
{
    while(!mEdges.empty())
    {
        std::map<GraphElementId, Agedge_t*>::iterator it = mEdges.begin();
        removeEdge(it->first);
    }
}

void GVGraph::setFont(const std::string& fontname, uint16_t fontsizeInPt)
{
    std::stringstream ss;
    ss << fontsizeInPt;
    std::string fontsize = ss.str();

    setGraphAttribute("fontname", fontname);
    setGraphAttribute("fontsize", fontsize);

    setNodeAttribute("fontname", fontname);
    setNodeAttribute("fontsize", fontsize);

    setEdgeAttribute("fontname", fontname);
    setEdgeAttribute("fontsize", fontsize);
}

void GVGraph::applyLayout(const std::string& layout)
{
    if(mNodes.empty())
    {
        return;
    }

    if(mAppliedLayout)
    {
        gvFreeLayout(mpContext, mpGVGraph);
        mpContext = NULL;
    }

    LOG_INFO_S << "Apply layout: " << layout;
    gvLayout(mpContext, mpGVGraph, layout.c_str());
    mAppliedLayout = true;

}

void GVGraph::renderToFile(const std::string& filename, const std::string& layout, bool forced)
{
    if(forced || !mAppliedLayout)
    {
        applyLayout(layout);
    }
    int rc = gvRenderFilename(mpContext, mpGVGraph, layout.c_str(), filename.c_str());
    if(-1 == rc)
    {
        std::string error_msg = std::string("graph_analysis::io::GVGraph: failed to make graphviz apply layout ") + layout + " for graph rendering";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
}

boxf GVGraph::boundingRect() const
{
    boxf box = GD_bb(mpGVGraph);

    box.LL.x *= mScalingFactor;
    box.LL.y *= mScalingFactor;
    box.UR.x *= mScalingFactor;
    box.UR.y *= mScalingFactor;

    return box;
}

//QRectF GVGraph::boundingRect() const
//{
//    boxf box = GD_bb(mpGVGraph);
//
//    return QRectF(box.LL.x*mScalingFactor, box.LL.y*mScalingFactor,
//                  box.UR.x*mScalingFactor, box.UR.y*mScalingFactor);
//}

std::vector<GVNode> GVGraph::nodes() const
{
    std::vector<GVNode> gvNodes;

    std::map<GraphElementId, Agnode_t*>::const_iterator it = mNodes.begin();
    for(; it != mNodes.end(); ++it)
    {
        Agnode_t* node = it->second;

        //Set the name of the node
        //object.name = node->name;
        std::string name = agnameof(node);

        //Fetch the X coordinate, apply the DPI conversion rate (actual DPI / 72, used by dot)
        pointf point = ND_coord(node);
        int x = point.x*mScalingFactor;

        //Translate the Y coordinate from bottom-left to top-left corner
        boxf box = GD_bb(mpGVGraph);
        int y = (box.UR.y - point.y)*mScalingFactor;

        //Transform the width and height from inches to pixels
        double height = ND_height(node) * mDPI;
        double width = ND_width(node) * mDPI;

        GVNode gvNode( mpBaseGraph->getVertex(it->first),
                name, x, y, height, width );
        gvNodes.push_back(gvNode);
    }

    return gvNodes;
}

std::vector<GVEdge> GVGraph::edges() const
{
    std::vector<GVEdge> gvEdges;
    std::map<GraphElementId, Agedge_t*>::const_iterator it = mEdges.begin();
    for(; it != mEdges.end(); ++it)
    {
        Agedge_t* edge = it->second;

        GraphElementId id = getId(edge);
        GVEdge gvEdge( mpBaseGraph->getEdge(id) );

        //Calculate the path from the spline (only one spline, as the graph is strict. If it
        //wasn't, we would have to iterate over the first list too)
        //Calculate the path from the spline (only one as the graph is strict)

        //http://www.graphviz.org/content/edge-position-and-edge-label-position
        // use
        //  ND_pos ..
        //  ED_spl
        //  GD_..
        //
        //splines* spl = ED_spl(edge);
        //boxf bb = GD_bb(mpGVGraph);

        //if((spl->list!=0) && (spl->list->size%3 == 1))
        //{
        //    //If there is a starting point, draw a line from it to the first curve point
        //    if(spl->list->sflag)
        //    {
        //        object.path.moveTo(spl->list->sp.x*mScalingFactor,
        //                     (bb.UR.y - spl->list->sp.y)*mScalingFactor);
        //        object.path.lineTo(spl->list->list[0].x*mScalingFactor,
        //                (bb.UR.y - spl->list->list[0].y)*mScalingFactor);
        //    } else {
        //        object.path.moveTo(spl->list->list[0].x*mScalingFactor,
        //                (bb.UR.y - spl->list->list[0].y)*mScalingFactor);
        //    }

        //    //Loop over the curve points
        //    for(int i=1; i < spl->list->size; i+=3)
        //    {
        //        object.path.cubicTo(spl->list->list[i].x*mScalingFactor,
        //              (bb.UR.y - spl->list->list[i].y)*mScalingFactor,
        //              spl->list->list[i+1].x*mScalingFactor,
        //              (bb.UR.y - spl->list->list[i+1].y)*mScalingFactor,
        //              spl->list->list[i+2].x*mScalingFactor,
        //              (bb.UR.y - spl->list->list[i+2].y)*mScalingFactor);
        //    }

        //    //If there is an ending point, draw a line to it
        //    if(spl->list->eflag)
        //    {
        //        object.path.lineTo(spl->list->ep.x*mScalingFactor,
        //                     (bb.UR.y - spl->list->ep.y)*mScalingFactor);
        //    }
        //}

        gvEdges.push_back(gvEdge);
    }

    return gvEdges;
}

GraphElementId GVGraph::getId(Agnode_t* node) const
{
    std::map<GraphElementId, Agnode_t*>::const_iterator cit = mNodes.begin();
    for(; cit != mNodes.end(); ++cit)
    {
        if(agnameof(cit->second) == agnameof(node))
        {
            return cit->first;
        }
    }
    throw std::invalid_argument("graph_analysis::io::GVGraph::getId could not retrieve id for node" + std::string( agnameof(node) ) + "'");
}

GraphElementId GVGraph::getId(Agedge_t* edge) const
{
    std::map<GraphElementId, Agedge_t*>::const_iterator cit = mEdges.begin();
    for(; cit != mEdges.end(); ++cit)
    {
        if(agnameof(cit->second) == agnameof(edge))
        {
            return cit->first;
        }
    }
    throw std::invalid_argument("graph_analysis::io::GVGraph::getId could not retrieve id for edge '" + std::string( agnameof(edge) ) + "'");
}


std::string GVGraph::getUniqueName(Vertex::Ptr vertex) const
{
    std::stringstream ss;
    ss << vertex->toString() << " (v:" << mpBaseGraph->getVertexId(vertex) << ")";
    VertexTypeManager *vManager = VertexTypeManager::getInstance();

    std::list<std::string> members = vManager->getMembers(vertex->getClassName());
    for(std::list<std::string>::iterator members_it = members.begin(); members_it != members.end(); ++members_it)
    {
        VertexTypeManager::MemberCallbacks mc = vManager->getMemberCallbacks(vertex->getClassName(),*members_it);
        if(mc.printFunction)
        {
            ss << std::endl << *members_it << " " << (vertex.get()->*mc.printFunction)();
        }
    }
    return ss.str();
}

std::string GVGraph::getUniqueName(Edge::Ptr edge) const
{
    std::stringstream ss;
    ss << edge->toString() << " (e:" << mpBaseGraph->getEdgeId(edge) << ")";
    return ss.str();
}

} // end namespace io
} // end namespace graph_analysis
