#ifndef OMVIZ_IRIEDGE_HPP
#define OMVIZ_IRIEDGE_HPP

#include <graph_analysis/Edge.hpp>
#include <owl_om/Ontology.hpp>

namespace omviz {

class IRIEdge : public graph_analysis::Edge
{
    owl_om::IRI mIri;
    owl_om::Ontology::Ptr mpOntology;

public:
    typedef boost::shared_ptr<IRIEdge> Ptr;

    IRIEdge(const owl_om::IRI& iri, owl_om::Ontology::Ptr ontology);

    std::string getClassName() const;
    std::string toString() const { return mIri.getFragment(); }
};

} // end namespace omviz
#endif // OMVIZ_IRIEDGE_HPP
