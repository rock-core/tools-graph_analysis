#ifndef GRAPH_ANALYIS_MAP_INITIALIZER_HPP
#define GRAPH_ANALYIS_MAP_INITIALIZER_HPP

#include <map>
namespace graph_analysis {

template <typename T, typename U>
class InitMap
{
private:
    std::map<T, U> mMap;
public:
    InitMap(const T& key, const U& val)
    {
        mMap[key] = val;
    }

    InitMap<T, U>& operator()(const T& key, const U& val)
    {
        mMap[key] = val;
        return *this;
    }

    operator std::map<T, U>()
    {
        return mMap;
    }
};

} // endif graph_analysis
#endif // GRAPH_ANALYIS_MAP_INITIALIZER_HPP
