#ifndef OMVIZ_IRINODE_HPP
#define OMVIZ_IRINODE_HPP

#include <graph_analysis/Vertex.hpp>
#include <owl_om/owlapi/model/OWLOntology.hpp>

#include <boost/shared_ptr.hpp>

namespace omviz {

class IRINode : public graph_analysis::Vertex
{
    owlapi::model::IRI mIri;
    owlapi::model::OWLOntology::Ptr mpOntology;

public:
    typedef boost::shared_ptr<IRINode> Ptr;

    IRINode(const owlapi::model::IRI& iri, owlapi::model::OWLOntology::Ptr ontology);

    std::string getClassName() const;

    std::string toString() const { return mIri.getFragment(); }

};

} // end namespace omviz
#endif // OMVIZ_IRINODE_HPP
