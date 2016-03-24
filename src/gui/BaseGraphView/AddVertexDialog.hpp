#ifndef GRAPH_ANALYSIS_GUI_BASEGRAPHVIEW_ADDVERTEXDIALOG_HPP
#define GRAPH_ANALYSIS_GUI_BASEGRAPHVIEW_ADDVERTEXDIALOG_HPP

#include <QDialog>

namespace graph_analysis
{
namespace gui
{

class AddVertexDialog : public QDialog
{
    Q_OBJECT

  public:
    AddVertexDialog(QWidget* parent = NULL);
    virtual ~AddVertexDialog(){};

    QString getClassname() const;
    QString getLabel() const;
};

} // end namespace gui
} // end namespace graph_analysis
#endif
