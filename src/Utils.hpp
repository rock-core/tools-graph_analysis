#ifndef OMVIZ_UTILS_HPP
#define OMVIZ_UTILS_HPP

#include <string>

class QTreeWidgetItem;

namespace omviz
{

class Utils
{
public:
    static QTreeWidgetItem* createTreeWidgetItem(const std::string& label, const std::string& data = "");

    /**
     *  Simple escaping for a string value
     */
    static std::string escapeString(const std::string& value);
};

} // end namespace omviz
#endif // OMVIZ_UTILS_HPP
