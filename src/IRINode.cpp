#include "IRINode.hpp"

namespace omviz {

IRINode::IRINode(const owl_om::IRI& iri, owl_om::Ontology::Ptr ontology)
    : mIri(iri)
    , mpOntology(ontology)
{
    if(!ontology)
    {
        throw std::runtime_error("omviz::IRINode: construction requires a valid pointer to an ontology");
    }
}

std::string IRINode::getClassName() const
{
    try {
        owl_om::IRI iri = mpOntology->typeOf(mIri);
        return iri.toString();
    } catch(const std::runtime_error& e)
    {
        return mIri.toString();
    }

}

}
