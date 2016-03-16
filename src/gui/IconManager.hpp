#ifndef GRAPH_ANALYSIS_GUI_ICON_MANAGER_HPP
#define GRAPH_ANALYSIS_GUI_ICON_MANAGER_HPP

#include <base/Singleton.hpp>
#include <QIcon>
#include <map>
#include <string>

namespace graph_analysis {
namespace gui {

/**
 * \file IconManager.hpp
 * \class IconManager
 * \brief Singleton to load and store handles on all GUI icons
 * \details Facilitates access to uniquely loaded icons for all concerned widgets
 */
class IconManager : public base::Singleton<IconManager>
{
protected:
    /// constructor
    IconManager();
    friend class base::Singleton<IconManager>;
public:
    /// maps specific access keywords to loaded qt icons
    typedef std::map <std::string, QIcon> IconMap;

    /// getter method - retrieves a qt icon registered at the given keyword in the icons map
    const QIcon* getIcon(const std::string& key);

    static std::string getIconRootPath();

private:
    /// qt icons map (with string keywords identifiers)
    IconMap mIconMap;
    /// loads icon from image file 'file' to the given qt icon
    void loadIcon(QIcon& icon, const std::string& file);
};


} // end namespace gui
} // end namespace graph_analysis

#endif // GRAPH_ANALYSIS_GUI_ICON_MANAGER_HPP
