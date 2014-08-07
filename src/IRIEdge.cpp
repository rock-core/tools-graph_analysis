#include "IRIEdge.hpp"

namespace omviz {

IRIEdge::IRIEdge(const owl_om::IRI& iri, owl_om::Ontology::Ptr ontology)
    : mIri(iri)
    , mpOntology(ontology)
{
    if(!ontology)
    {
        throw std::runtime_error("omviz::IRINode: construction requires a valid pointer to an ontology");
    }
}

std::string IRIEdge::getClassName() const
{
    return mIri.toString();
}

} // end namespace omviz
