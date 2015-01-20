#include "IRINode.hpp"
#include <owl_om/owlapi/model/OWLOntologyAsk.hpp>

using namespace owlapi::model;

namespace omviz {

IRINode::IRINode(const IRI& iri, OWLOntology::Ptr ontology)
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
        OWLOntologyAsk ask(mpOntology);
        IRI iri = ask.typeOf(mIri);
        return iri.toString();
    } catch(const std::runtime_error& e)
    {
        return mIri.toString();
    }

}

}
