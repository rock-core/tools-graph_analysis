#ifndef GRAPH_ANALYSIS_GUI_BASEGRAPHVIEW_ADDEDGEDIALOG_HPP
#define GRAPH_ANALYSIS_GUI_BASEGRAPHVIEW_ADDEDGEDIALOG_HPP

#include <QDialog>

namespace Ui
{
class AddEdgeDialog;
}

namespace graph_analysis
{
namespace gui
{

class AddEdgeDialog : public QDialog
{
    Q_OBJECT

    ::Ui::AddEdgeDialog* mpUi;

public:
    AddEdgeDialog(QDialog* parent = NULL);
    virtual ~AddEdgeDialog();

    QString getClassname() const;
    QString getLabel() const;
};

} // end namespace gui
} // end namespace graph_analysis
#endif
