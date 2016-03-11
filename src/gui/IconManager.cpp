#include "IconManager.hpp"
#include <utilmm/configfile/pkgconfig.hh>
#include <stdexcept>
#include <QFileInfo>

namespace graph_analysis {
namespace gui {

std::string IconManager::getIconRootPath()
{
    try {
        utilmm::pkgconfig pkg("graph_analysis");
        return pkg.get("sharedir") + "/icons/";
    } catch(...)
    {
        throw std::runtime_error("graph_analysis::gui::IconManager: graph_analysis is not installed - failed to find icon dir");
    }
}

IconManager::IconManager()
{
    // setting up all icons
    std::string pathToIcons = IconManager::getIconRootPath();

    //        taken_from: www.softicons.com         //        commercial_usage: NOT allowed
    loadIcon(mIconMap["addNode"], pathToIcons + "addNode.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["import"], pathToIcons + "import.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["export"], pathToIcons + "export.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["refresh"], pathToIcons + "refresh.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["shuffle"], pathToIcons + "shuffle.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["reset"], pathToIcons + "reset.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["layout"], pathToIcons + "layout.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["connection_mode"], pathToIcons + "dragndrop.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["label"], pathToIcons + "label.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["featureLabel"], pathToIcons + "featureLabel.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["remove"], pathToIcons + "remove.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["removeAll"], pathToIcons + "removeAll.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["addFeature"], pathToIcons + "addFeature.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["move"], pathToIcons + "move.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["reload"], pathToIcons + "reload.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["swap"], pathToIcons + "swap.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["layers"], pathToIcons + "layers.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["properties"], pathToIcons + "properties.png");

    //        taken_from: www.softicons.com         //        commercial_usage: NOT allowed
    loadIcon(mIconMap["addNode_white"], pathToIcons + "addNode_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["import_white"], pathToIcons + "import_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["export_white"], pathToIcons + "export_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["refresh_white"], pathToIcons + "refresh_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["shuffle_white"], pathToIcons + "shuffle_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["reset_white"], pathToIcons + "reset_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["layout_white"], pathToIcons + "layout_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["connection_mode_white"], pathToIcons + "connectivity_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["move_mode_white"], pathToIcons + "move_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["edit_mode_white"], pathToIcons + "edit.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["label_white"], pathToIcons + "label_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["featureLabel_white"], pathToIcons + "featureLabel_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["remove_white"], pathToIcons + "remove_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["removeAll_white"], pathToIcons + "removeAll_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["addFeature_white"], pathToIcons + "addFeature_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["move_white"], pathToIcons + "move_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["reload_white"], pathToIcons + "reload_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["swap_white"], pathToIcons + "swap_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["layers_white"], pathToIcons + "layers_white.png");
    //        taken_from: www.softicons.com         //        commercial_usage: allowed
    loadIcon(mIconMap["properties_white"], pathToIcons + "properties_white.png");
}

const QIcon* IconManager::getIcon(const std::string& key)
{
    IconMap::const_iterator it = mIconMap.find(key);
    if(it == mIconMap.end())
    {
        throw std::invalid_argument("graph_analysis::gui::IconManager: Could not find a icon named for '" + key + "'");
    }

    return &it->second;
}

void IconManager::loadIcon(QIcon& icon, const std::string& file)
{
    QFileInfo iconFile(file.c_str());
    if(iconFile.exists())
    {
        icon.addFile(QString::fromUtf8(file.c_str()), QSize(), QIcon::Normal, QIcon::Off);
    } else {
        throw std::runtime_error("graph_analysis::gui::IconManager::loadIcon: failed to load icon from '" + file + "'");
    }
}

} // end namespace gui
} // end namespace graph_analysis
