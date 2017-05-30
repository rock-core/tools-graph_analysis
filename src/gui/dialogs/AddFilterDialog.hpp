#ifndef GRAPH_ANALYSIS_GUI_UI_ADD_FILTER_HPP
#define GRAPH_ANALYSIS_GUI_UI_ADD_FILTER_HPP

#include <QDialog>

namespace Ui
{
    class Dialog;
}

namespace graph_analysis {
namespace gui {
namespace dialogs {

class AddFilter : public QDialog
{
    Q_OBJECT

public:
    AddFilter(QWidget* parent = NULL);
    ~AddFilter();

    QString getFilterRegexp() const;
    bool isEnabled() const;

private:
    Ui::Dialog* mUi;
};

} // end namespace dialogs
} // end namespace gui
} // end graph_analysis
#endif // GRAPH_ANALYSIS_GUI_ADD_FILTER_HPP
