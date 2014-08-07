#include "GVGraph.hpp"
#include <QString>

namespace omviz {
/*! Dot uses a 72 DPI value for converting it's position coordinates from points to pixels
    while we display at 96 DPI on most operating systems. */
const qreal GVGraph::DotDefaultDPI=72.0;

GVGraph::GVGraph(QString name, QFont font, qreal node_size) :
        _context(gvContext()),
        _graph(_agopen(name, AGDIGRAPHSTRICT)) // Strict directed graph, see libgraph doc
{
    //Set graph attributes
    setGraphAttribute("overlap", "prism");
    setGraphAttribute("splines", "true");
    setGraphAttribute("pad", "0,2");
    setGraphAttribute("dpi", "96,0");
    setGraphAttribute("nodesep", "0,4");

    //Set default attributes for the future nodes
    setNodeAttribute("fixedsize", "true");
    setNodeAttribute("label", "");
    setNodeAttribute("regular", "true");

    //Divide the wanted width by the DPI to get the value in points
    QString nodePtsWidth("%1");
    double d = _agget(_graph, "dpi", "96,0").toDouble();
    nodePtsWidth.arg(node_size/d);

    //GV uses , instead of . for the separator in floats
    setNodeAttribute("width", nodePtsWidth.replace('.', ",").toStdString());

    setFont(font);
}

GVGraph::~GVGraph()
{
    gvFreeLayout(_context, _graph);
    agclose(_graph);
    gvFreeContext(_context);
}

int GVGraph::setGraphAttribute(const std::string& name, const std::string& value)
{
    // Directly use agsafeset which always works, contrarily to agset
    return agsafeset(_graph, const_cast<char *>(name.c_str()),
                     const_cast<char *>(value.c_str()),
                     const_cast<char *>(value.c_str()));
}

std::string GVGraph::getGraphAttribute(const std::string& name, const std::string& defaultValue) const
{
    std::string value = agget(_graph, const_cast<char *>(name.c_str()));
    if(value.empty())
    {
        return defaultValue;
    }
    return value;
}

void GVGraph::setNodeAttribute(const std::string& name, const std::string& value)
{
    agnodeattr(_graph, const_cast<char*>(name.c_str()), const_cast<char*>(value.c_str()));
}

void GVGraph::setEdgeAttribute(const std::string& name, const std::string& value)
{
    agedgeattr(_graph, const_cast<char*>(name.c_str()), const_cast<char*>(value.c_str()));
}

void GVGraph::addNode(const QString& name)
{
    if(_nodes.contains(name))
        removeNode(name);

    _nodes.insert(name, _agnode(_graph, name));

    if(_nodes.size() == 1)
    {
        setRootNode(name);
    }
}

void GVGraph::addNodes(const QStringList& names)
{
    for(int i=0; i<names.size(); ++i)
        addNode(names.at(i));
}

void GVGraph::removeNode(const QString& name)
{
    if(_nodes.contains(name))
    {
        agdelete(_graph, _nodes[name]);
        _nodes.remove(name);

        QList<QPair<QString, QString> >keys=_edges.keys();
        for(int i=0; i<keys.size(); ++i)
            if(keys.at(i).first==name || keys.at(i).second==name)
                removeEdge(keys.at(i));
    }
}

void GVGraph::clearNodes()
{
    QList<QString> keys=_nodes.keys();

    for(int i=0; i<keys.size(); ++i)
        removeNode(keys.at(i));
}

void GVGraph::setRootNode(const QString& name)
{
    if(_nodes.contains(name))
        setGraphAttribute("root", name.toStdString());
}

void GVGraph::addEdge(const QString &source, const QString &target)
{
    if(_nodes.contains(source) && _nodes.contains(target))
    {
        QPair<QString, QString> key(source, target);
        if(!_edges.contains(key))
            _edges.insert(key, agedge(_graph, _nodes[source], _nodes[target]));
    }
}

void GVGraph::removeEdge(const QString &source, const QString &target)
{
    removeEdge(QPair<QString, QString>(source, target));
}

void GVGraph::removeEdge(const QPair<QString, QString>& key)
{
    if(_edges.contains(key))
    {
        agdelete(_graph, _edges[key]);
        _edges.remove(key);
    }
}

void GVGraph::setFont(QFont font)
{
    _font=font;

    setGraphAttribute("fontname", font.family().toStdString());
    setGraphAttribute("fontsize", QString("%1").arg(font.pointSizeF()).toStdString());

    setNodeAttribute("fontname", font.family().toStdString());
    setNodeAttribute("fontsize", QString("%1").arg(font.pointSizeF()).toStdString());

    setEdgeAttribute("fontname", font.family().toStdString());
    setEdgeAttribute("fontsize", QString("%1").arg(font.pointSizeF()).toStdString());
}

void GVGraph::applyLayout()
{
    gvFreeLayout(_context, _graph);
    gvLayout(_context, _graph, "dot");

    gvRenderFilename(_context, _graph,"dot","/tmp/dotfile.txt");
}

QRectF GVGraph::boundingRect() const
{
    qreal dpi= getQGraphAttribute("dpi", "96,0").toDouble();
    return QRectF(_graph->u.bb.LL.x*(dpi/DotDefaultDPI), _graph->u.bb.LL.y*(dpi/DotDefaultDPI),
                  _graph->u.bb.UR.x*(dpi/DotDefaultDPI), _graph->u.bb.UR.y*(dpi/DotDefaultDPI));
}

QList<GVNode> GVGraph::nodes() const
{
    QList<GVNode> list;
    qreal dpi= getQGraphAttribute("dpi", "96,0").toDouble();

    for(QMap<QString, Agnode_t*>::const_iterator it=_nodes.begin(); it!=_nodes.end();++it)
    {
        Agnode_t *node=it.value();
        GVNode object;

        //Set the name of the node
        object.name=node->name;

        qDebug("X/Y => %.3f/%.3f",node->u.coord.x, node->u.coord.y);

        //Fetch the X coordinate, apply the DPI conversion rate (actual DPI / 72, used by dot)
        qreal x=node->u.coord.x; //*(dpi/DotDefaultDPI);

        //Translate the Y coordinate from bottom-left to top-left corner
        qreal y=(_graph->u.bb.UR.y - node->u.coord.y); //*(dpi/DotDefaultDPI);
        object.centerPos=QPoint(x*5, y*5);

        //Transform the width and height from inches to pixels
        object.height=node->u.height*dpi;
        object.width=node->u.width*dpi;

        list << object;
    }

    return list;
}

QList<GVEdge> GVGraph::edges() const
{
    QList<GVEdge> list;
    qreal dpi= getQGraphAttribute("dpi", "96,0").toDouble();

    for(QMap<QPair<QString, QString>, Agedge_t*>::const_iterator it=_edges.begin();
        it!=_edges.end();++it)
    {
        Agedge_t *edge=it.value();
        GVEdge object;

        //Fill the source and target node names
        object.source=edge->tail->name;
        object.target=edge->head->name;

        //Calculate the path from the spline (only one spline, as the graph is strict. If it
        //wasn't, we would have to iterate over the first list too)
        //Calculate the path from the spline (only one as the graph is strict)
        if((edge->u.spl->list!=0) && (edge->u.spl->list->size%3 == 1))
        {
            //If there is a starting point, draw a line from it to the first curve point
            if(edge->u.spl->list->sflag)
            {
                object.path.moveTo(edge->u.spl->list->sp.x*(dpi/DotDefaultDPI),
                             (_graph->u.bb.UR.y - edge->u.spl->list->sp.y)*(dpi/DotDefaultDPI));
                object.path.lineTo(edge->u.spl->list->list[0].x*(dpi/DotDefaultDPI),
                        (_graph->u.bb.UR.y - edge->u.spl->list->list[0].y)*(dpi/DotDefaultDPI));
            }
            else
                object.path.moveTo(edge->u.spl->list->list[0].x*(dpi/DotDefaultDPI),
                        (_graph->u.bb.UR.y - edge->u.spl->list->list[0].y)*(dpi/DotDefaultDPI));

            //Loop over the curve points
            for(int i=1; i<edge->u.spl->list->size; i+=3)
                object.path.cubicTo(edge->u.spl->list->list[i].x*(dpi/DotDefaultDPI), 
                      (_graph->u.bb.UR.y - edge->u.spl->list->list[i].y)*(dpi/DotDefaultDPI),
                      edge->u.spl->list->list[i+1].x*(dpi/DotDefaultDPI),
                      (_graph->u.bb.UR.y - edge->u.spl->list->list[i+1].y)*(dpi/DotDefaultDPI),
                      edge->u.spl->list->list[i+2].x*(dpi/DotDefaultDPI),
                      (_graph->u.bb.UR.y - edge->u.spl->list->list[i+2].y)*(dpi/DotDefaultDPI));

            //If there is an ending point, draw a line to it
            if(edge->u.spl->list->eflag)
                object.path.lineTo(edge->u.spl->list->ep.x*(dpi/DotDefaultDPI),
                             (_graph->u.bb.UR.y - edge->u.spl->list->ep.y)*(dpi/DotDefaultDPI));
        }

        list << object;
    }

    return list;
}


}
