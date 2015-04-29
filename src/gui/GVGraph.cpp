#include "GVGraph.hpp"
#include <QString>

namespace graph_analysis {
namespace gui {

/*! Dot uses a 72 DPI value for converting it's position coordinates from points to pixels
    while we display at 96 DPI on most operating systems. */
const qreal GVGraph::DotDefaultDPI=72.0;
std::set<std::string> GVGraph::msRegisteredLayouts;

void GVGraph::registerLayouts()
{
    //Registering "circo", "dot", "fdp", "neato", "nop", "nop1", "nop2", "osage", "patchwork", "sfdp", "twopi"
    GVGraph::msRegisteredLayouts.insert("circo");
    GVGraph::msRegisteredLayouts.insert("dot");
    GVGraph::msRegisteredLayouts.insert("fdp");
    GVGraph::msRegisteredLayouts.insert("neato");
    GVGraph::msRegisteredLayouts.insert("nop");
    GVGraph::msRegisteredLayouts.insert("nop1");
    GVGraph::msRegisteredLayouts.insert("nop2");
    GVGraph::msRegisteredLayouts.insert("osage");
    GVGraph::msRegisteredLayouts.insert("patchwork");
    GVGraph::msRegisteredLayouts.insert("sfdp");
    GVGraph::msRegisteredLayouts.insert("twopi");
}

GVGraph::GVGraph(QString name, QFont font, double node_size)
    : mpContext(gvContext())
    , mpGraph(_agopen(name, Agstrictdirected)) // Strict directed graph -- one egdeg between two nodes, see libgraph doc
    , mAppliedLayout(false)

{
    //Set graph attributes
    setGraphAttribute("overlap", "false");
    //setGraphAttribute("splines", "true");
    setGraphAttribute("pad", "0,2");
    setGraphAttribute("dpi", "96,0");

    // Don't use node sep here, since it prevents outputting the exact position
    // when using dot layout
    // setGraphAttribute("nodesep", "1");

    //Set default attributes for the future nodes
    setNodeAttribute("fixedsize", "false");
    setNodeAttribute("shape","box");
//    setNodeAttribute("regular", "true");

    //Divide the wanted width by the DPI to get the value in points
    mDPI = getQGraphAttribute("dpi", "96,0").replace(',',".").toDouble();
    mScalingFactor = (1.0*mDPI) / (1.0*DotDefaultDPI);

    // format string %1
    // http://qt-project.org/doc/qt-4.8/qstring.html#arg-20
    QString nodePtsWidth = QString("%1").arg( node_size/mDPI);

    //GV uses , instead of . for the separator in floats
    setNodeAttribute("width", nodePtsWidth.replace('.', ",").toStdString());

    setFont(font);
    if(GVGraph::msRegisteredLayouts.empty())
    {
        GVGraph::registerLayouts();
    }
}

GVGraph::~GVGraph()
{
    //if(mAppliedLayout)
    //{
    //    gvFreeLayout(mpContext, mpGraph);
    //}

    //agclose(mpGraph);
    //gvFreeContext(mpContext);

    //mpGraph = NULL;
    //mpContext = NULL;
}

int GVGraph::setGraphAttribute(const std::string& name, const std::string& value)
{
    // Directly use agsafeset which always works, contrarily to agset
    return agsafeset(mpGraph, const_cast<char *>(name.c_str()),
                     const_cast<char *>(value.c_str()),
                     const_cast<char *>(value.c_str()));
}

std::string GVGraph::getGraphAttribute(const std::string& name, const std::string& defaultValue) const
{
    std::string value = agget(mpGraph, const_cast<char *>(name.c_str()));
    if(value.empty())
    {
        return defaultValue;
    }
    return value;
}

void GVGraph::setNodeAttribute(const std::string& name, const std::string& value)
{
    agattr(mpGraph, AGNODE, const_cast<char*>(name.c_str()), const_cast<char*>(value.c_str()));
}

void GVGraph::setEdgeAttribute(const std::string& name, const std::string& value)
{
    // first arg: edge
    // second arg: value
    agattr(mpGraph, AGEDGE, const_cast<char*>(name.c_str()), const_cast<char*>(value.c_str()));
}

GraphElementId GVGraph::addNode(const QString& name)
{
    if(mNodes.contains(name))
        removeNode(name);

    Agnode_t* node = _agnode(mpGraph, name);
    mNodes.insert(name, node);

    if(mNodes.size() == 1)
    {
        setRootNode(name);
    }

    return AGID(node);
}

void GVGraph::addNodes(const QStringList& names)
{
    for(int i=0; i<names.size(); ++i)
    {
        addNode(names.at(i));
    }
}

void GVGraph::removeNode(const QString& name)
{
    if(mNodes.contains(name))
    {
        QList<QPair<QString, QString> > keys = mEdges.keys();
        for(int i=0; i<keys.size(); ++i)
        {
            QString sourceNodeName = keys.at(i).first;
            QString targetNodeName = keys.at(i).second;

            if( sourceNodeName == name || targetNodeName == name)
            {
                removeEdge(keys.at(i));
            }
        }

        agdelnode(mpGraph, mNodes[name]);
        mNodes.remove(name);

    }
}

void GVGraph::clearNodes()
{
    QList<QString> keys=mNodes.keys();

    for(int i=0; i<keys.size(); ++i)
    {
        removeNode(keys.at(i));
    }
}

void GVGraph::setRootNode(const QString& name)
{
    if(mNodes.contains(name))
    {
        setGraphAttribute("root", name.toStdString());
    }
}

GraphElementId GVGraph::addEdge(const QString& source, const QString& target, const QString& label)
{
    if(mNodes.contains(source) && mNodes.contains(target))
    {
        QPair<QString, QString> key(source, target);
        if(!mEdges.contains(key))
        {
            bool create = true;
            Agedge_t* edge = _agedge(mpGraph, mNodes[source], mNodes[target], label.toStdString().c_str(), create);
            mEdges.insert(key, edge);
            return AGID(edge);
        }
    }
    throw std::runtime_error("graph_analysis::gui::GVGraph::addEdge: failed to add edge, nodes or targets missing");
}

void GVGraph::removeEdge(const QString &source, const QString &target)
{
    removeEdge(QPair<QString, QString>(source, target));
}

void GVGraph::removeEdge(const QPair<QString, QString>& key)
{
    if(mEdges.contains(key))
    {
        agdeledge(mpGraph, mEdges[key]);
        mEdges.remove(key);
    }
}

void GVGraph::clearEdges()
{
    QList< QPair<QString, QString> > keys=mEdges.keys();

    for(int i=0; i < keys.size(); ++i)
    {
        removeEdge(keys.at(i));
    }
}

void GVGraph::setFont(QFont font)
{
    mFont=font;

    setGraphAttribute("fontname", font.family().toStdString());
    setGraphAttribute("fontsize", QString("%1").arg(font.pointSizeF()).toStdString());

    setNodeAttribute("fontname", font.family().toStdString());
    setNodeAttribute("fontsize", QString("%1").arg(font.pointSizeF()).toStdString());

    setEdgeAttribute("fontname", font.family().toStdString());
    setEdgeAttribute("fontsize", QString("%1").arg(font.pointSizeF()).toStdString());
}

void GVGraph::applyLayout(const std::string& layout)
{
    if(mNodes.empty())
    {
        return;
    }

    if(mAppliedLayout)
    {
        gvFreeLayout(mpContext, mpGraph);
    }

    gvLayout(mpContext, mpGraph, layout.c_str());
    mAppliedLayout = true;

}

void GVGraph::renderToFile(const std::string& filename, const std::string& layout)
{
    if(!mAppliedLayout)
    {
        applyLayout();
    }
    gvRenderFilename(mpContext, mpGraph, layout.c_str(), filename.c_str());
}

QRectF GVGraph::boundingRect() const
{
    boxf box = GD_bb(mpGraph);
    return QRectF(box.LL.x*mScalingFactor, box.LL.y*mScalingFactor,
                  box.UR.x*mScalingFactor, box.UR.y*mScalingFactor);
}

QList<GVNode> GVGraph::nodes() const
{
    QList<GVNode> list;

    QMap<QString, Agnode_t*>::const_iterator it = mNodes.begin();
    for(; it != mNodes.end(); ++it)
    {
        Agnode_t *node=it.value();
        GVNode object;

        //Set the name of the node
        //object.name = node->name;
        object.name = agnameof(node);

        //Fetch the X coordinate, apply the DPI conversion rate (actual DPI / 72, used by dot)
        pointf point = ND_coord(node);
        qreal x = point.x*mScalingFactor;

        //Translate the Y coordinate from bottom-left to top-left corner
        boxf box = GD_bb(mpGraph);
        qreal y = (box.UR.y - point.y)*mScalingFactor;
        object.centerPos=QPoint(x, y);

        //Transform the width and height from inches to pixels
        double height = ND_height(node);
        object.height=height*mDPI;

        double width = ND_width(node);
        object.width=width*mDPI;

        list << object;
    }

    return list;
}

QList<GVEdge> GVGraph::edges() const
{
    QList<GVEdge> list;
    QMap<QPair<QString, QString>, Agedge_t*>::const_iterator it = mEdges.begin();
    for(; it != mEdges.end(); ++it)
    {
        Agedge_t* edge = it.value();
        GVEdge object;

        //Fill the source and target node names
        //object.source = edge->tail->name;
        //object.target = edge->head->name;
        object.source = agnameof( agtail(edge) );
        object.target = agnameof( aghead(edge) );

        //Calculate the path from the spline (only one spline, as the graph is strict. If it
        //wasn't, we would have to iterate over the first list too)
        //Calculate the path from the spline (only one as the graph is strict)

        //http://www.graphviz.org/content/edge-position-and-edge-label-position
        // use 
        //  ND_pos ..
        //  ED_spl
        //  GD_..
        splines* spl = ED_spl(edge);
        boxf bb = GD_bb(mpGraph);

        if((spl->list!=0) && (spl->list->size%3 == 1))
        {
            //If there is a starting point, draw a line from it to the first curve point
            if(spl->list->sflag)
            {
                object.path.moveTo(spl->list->sp.x*mScalingFactor,
                             (bb.UR.y - spl->list->sp.y)*mScalingFactor);
                object.path.lineTo(spl->list->list[0].x*mScalingFactor,
                        (bb.UR.y - spl->list->list[0].y)*mScalingFactor);
            } else {
                object.path.moveTo(spl->list->list[0].x*mScalingFactor,
                        (bb.UR.y - spl->list->list[0].y)*mScalingFactor);
            }

            //Loop over the curve points
            for(int i=1; i < spl->list->size; i+=3)
            {
                object.path.cubicTo(spl->list->list[i].x*mScalingFactor,
                      (bb.UR.y - spl->list->list[i].y)*mScalingFactor,
                      spl->list->list[i+1].x*mScalingFactor,
                      (bb.UR.y - spl->list->list[i+1].y)*mScalingFactor,
                      spl->list->list[i+2].x*mScalingFactor,
                      (bb.UR.y - spl->list->list[i+2].y)*mScalingFactor);
            }

            //If there is an ending point, draw a line to it
            if(spl->list->eflag)
            {
                object.path.lineTo(spl->list->ep.x*mScalingFactor,
                             (bb.UR.y - spl->list->ep.y)*mScalingFactor);
            }
        }

        list << object;
    }

    return list;
}

} // end namespace gui
} // end namespace graph_analysis
