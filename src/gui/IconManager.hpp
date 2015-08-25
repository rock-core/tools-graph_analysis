#ifndef GRAPH_ANALYSIS_GUI_ICON_MANAGER_HPP
#define GRAPH_ANALYSIS_GUI_ICON_MANAGER_HPP

#include <map>
#include <QIcon>
#include <string>
#include <base/Singleton.hpp>

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
public:
    /// maps specific access keywords to loaded qt icons
    typedef std::map <std::string, QIcon> IconMap;

    /// hard-wired path to icons folder (relative to the runnable directory)
    static std::string DEFAULT_PATH_TO_ICONS;

    /// constructor
    IconManager();
    /// destructor
    ~IconManager();

    /// getter method - retrieves a qt icon registered at the given keyword in the icons map
    QIcon* getIcon(std::string key) { return &(mIconMap[key]); }

private:
    /// qt icons map (with string keywords identifiers)
    IconMap mIconMap;
    /// loads icon from image file 'file' to the given qt icon
    void loadIcon(QIcon& icon, std::string file);
};


} // end namespace gui
} // end namespace graph_analysis

#endif // GRAPH_ANALYSIS_GUI_ICON_MANAGER_HPP
