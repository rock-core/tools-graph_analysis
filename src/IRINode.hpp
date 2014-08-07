#ifndef OMVIZ_IRINODE_HPP
#define OMVIZ_IRINODE_HPP

#include <graph_analysis/Vertex.hpp>
#include <owl_om/Ontology.hpp>

#include <boost/shared_ptr.hpp>

namespace omviz {

class IRINode : public graph_analysis::Vertex
{
    owl_om::IRI mIri;
    owl_om::Ontology::Ptr mpOntology;

public:
    typedef boost::shared_ptr<IRINode> Ptr;

    IRINode(const owl_om::IRI& iri, owl_om::Ontology::Ptr ontology);

    std::string getClassName() const;

    std::string toString() const { return mIri.getFragment(); }

};

} // end namespace omviz
#endif // OMVIZ_IRINODE_HPP
