#ifndef GRAPH_ANALYSIS_GUI_BASEGRAPHVIEW_ADDVERTEXDIALOG_HPP
#define GRAPH_ANALYSIS_GUI_BASEGRAPHVIEW_ADDVERTEXDIALOG_HPP

#include <QDialog>

namespace Ui
{
class AddVertexDialog;
}

namespace graph_analysis
{
namespace gui
{

class AddVertexDialog : public QDialog
{
    Q_OBJECT

    ::Ui::AddVertexDialog* mpUi;

public:
    AddVertexDialog(QDialog* parent = NULL);
    virtual ~AddVertexDialog();

    QString getClassname() const;
    QString getLabel() const;
};

} // end namespace gui
} // end namespace graph_analysis
#endif
