#ifndef OMVIZ_IRIEDGE_HPP
#define OMVIZ_IRIEDGE_HPP

#include <graph_analysis/Edge.hpp>
#include <owl_om/owlapi/model/OWLOntology.hpp>

namespace omviz {

class IRIEdge : public graph_analysis::Edge
{
    owlapi::model::IRI mIri;
    owlapi::model::OWLOntology::Ptr mpOntology;

public:
    typedef boost::shared_ptr<IRIEdge> Ptr;

    IRIEdge(const owlapi::model::IRI& iri, owlapi::model::OWLOntology::Ptr ontology);

    std::string getClassName() const;
    std::string toString() const { return mIri.getFragment(); }
};

} // end namespace omviz
#endif // OMVIZ_IRIEDGE_HPP
