#ifndef GRAPH_ANALYSIS_GUI_GUI_DIALOGS_GRID_LAYOUT_HPP
#define GRAPH_ANALYSIS_GUI_GUI_DIALOGS_GRID_LAYOUT_HPP

#include <QDialog>
#include "../layouts/GridLayout.hpp"

namespace Ui
{
    class Dialog;
}

namespace graph_analysis {
namespace gui {
namespace dialogs {

class GridLayout : public QDialog
{
    Q_OBJECT

public:
    GridLayout(QWidget* parent = NULL);
    ~GridLayout();

    static int execute();

private:
    Ui::Dialog* mUi;

public slots:
    void updateColumnScalingFromSlider(int value);
    void updateRowScalingFromSlider(int value);

    layouts::GridLayout::Ptr getGridLayout() const;

};

} // end namespace dialogs
} // end namespace gui
} // end graph_analysis
#endif // GRAPH_ANALYSIS_GUI_GUI_DIALOGS_GRID_LAYOUT_HPP
