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
};

} // end namespace omviz
#endif // OMVIZ_UTILS_HPP
