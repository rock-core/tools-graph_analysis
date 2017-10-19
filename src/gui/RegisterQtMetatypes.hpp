#ifndef REGISTERQTMETATYPES_H
#define REGISTERQTMETATYPES_H

#include <QMetaType>

#include "../Vertex.hpp"
#include "../Edge.hpp"
#include "../BaseGraph.hpp"

/**
 * doing so allows us to use the shared pointer inside "QVariant"
 */
Q_DECLARE_METATYPE(graph_analysis::Vertex::Ptr)
Q_DECLARE_METATYPE(graph_analysis::Edge::Ptr)
Q_DECLARE_METATYPE(graph_analysis::BaseGraph::Ptr)

#endif /*REGISTERQTMETATYPES_H*/
