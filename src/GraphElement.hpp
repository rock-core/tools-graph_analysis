#ifndef GRAPH_ANALYSIS_GRAPH_ELEMENT_HPP
#define GRAPH_ANALYSIS_GRAPH_ELEMENT_HPP

#include <stdint.h>
#include <vector>
#include <map>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include "SharedPtr.hpp"

namespace graph_analysis {

typedef boost::uuids::uuid GraphElementUuid;
typedef uint32_t GraphElementId;
typedef uint32_t GraphId;
typedef std::map<GraphId, GraphElementId> GraphElementMap;
typedef std::vector<GraphId> GraphIdList;

/**
 * GraphElement is the base class for all element in a graph, i.e. for
 * edges and vertices
 *
 * It provides labeling functionality as well as the handling of ids
 * One GraphElement can be associated in multiple graphs
 *
 */
class GraphElement : public enable_shared_from_this<GraphElement>
{
public:
    /**
     * \brief Default constructor
     */
    GraphElement(const std::string& label = std::string());

    virtual ~GraphElement();

    typedef shared_ptr< GraphElement > Ptr;
    typedef weak_ptr< GraphElement > WeakPtr;
    typedef std::vector<Ptr> PtrList;

    /**
     * Set a label
     */
    virtual void setLabel(const std::string& label) { mLabel = label; }

    /**
     * Get label
     */
    virtual const std::string& getLabel() const { return mLabel; }

    // Get class name
    // \return class name
    virtual std::string getClassName() const { return "graph_analysis::GraphElement"; }

    /**
     * Convert element to string and adding and indentation (in spaces) to the
     * string
     */
    virtual std::string toString(uint32_t indent) const { return std::string(indent,' ') + getClassName(); }

    /**
     * Convert element to string
     */
    virtual std::string toString() const { return toString(0); }

    /**
     * Test whether this element has been associated with
     * an edge or node
     */
    bool associated(GraphId graph) const { return mGraphElementMap.count(graph); }

    /**
     * Add the element to a corresponding graph
     * This allows reverse mapping from the element to the graphs it belongs to
     */
    void associate(GraphId graph, GraphElementId elementId) { mGraphElementMap[graph] = elementId; }

    /**
     * Remove the edge that corresponds to a given graph
     */
    void disassociate(GraphId graph) { mGraphElementMap.erase(graph); }

    /**
     * Get id of this element within a given graph
     */
    GraphElementId getId(GraphId graph) const;

    /**
     * Get a universally unique id of this GraphElement
     * \deprecated use getUuid instead
     */
    GraphElementUuid getUid() const { return mUuid; }

    /**
     * Get a universally unique id of this GraphElement
     * \return uuid
     */
    GraphElementUuid getUuid() const { return mUuid; }

    /**
      * Get pointer to existing GraphElement from Uuid
      * \return pointer to GraphElement
      */
    static GraphElement::Ptr fromUuid(const GraphElementUuid& uuid);

    /**
     * Get list of graph associations this element has
     */
    GraphIdList getGraphAssociations() const;

    /**
     * Convert to a string with the element id as prefix
     */
    std::string toPrefixedString(GraphId graph) const;

protected:
    /**
     * Add local method to shared from this to allow using bind
     * \return pointer to existing GraphElement
     */
    GraphElement::Ptr getSharedFromThis() { return shared_from_this(); }

    void disassociateFromAll() { mGraphElementMap.clear(); }
    GraphElementMap mGraphElementMap;

    GraphElementUuid mUuid;
    static boost::uuids::random_generator msUuidGenerator;
    static std::map<GraphElementUuid, function<GraphElement::Ptr()> > msGraphElements;
    std::string mLabel;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GRAPH_ELEMENT_HPP
