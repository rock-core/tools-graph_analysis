#ifndef GRAPH_ANALYSIS_GUI_PLAYER_CONFIGURATION_HPP
#define GRAPH_ANALYSIS_GUI_PLAYER_CONFIGURATION_HPP

#include <cstddef>

namespace graph_analysis {
namespace gui {

class PlayerConfiguration
{
public:
    PlayerConfiguration();

    void setItemShowtimeDelayInMs(size_t v) { mItemShowtimeDelayInMs = v; }
    size_t getItemShowtimeDelayInMs() const { return mItemShowtimeDelayInMs; }

    void setShowItem(bool show) { mShowItem = show; }
    bool getShowItem() const { return mShowItem; }
private:
    size_t mItemShowtimeDelayInMs;
    bool mShowItem;

};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_PLAYER_CONFIGURATION_HPP
