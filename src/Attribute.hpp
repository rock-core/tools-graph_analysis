#ifndef  GRAPH_ANALYSIS_ATTRIBUTE_HPP
#define GRAPH_ANALYSIS_ATTRIBUTE_HPP

#include <string>

namespace graph_analysis {

class Attribute
{
public:
    typedef std::string Id;
    Attribute();

    Attribute(const Id& id,
            const std::string name,
            const std::string& type = "STRING");

    Id getId() const { return mId; }
    std::string getName() const { return mName; }
    std::string getType() const { return mType; }

    std::string toString() const;

private:
    Id mId;
    std::string mName;
    std::string mType;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ATTRIBUTE_HPP
