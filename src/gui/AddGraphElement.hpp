#ifndef GRAPH_ANALYSIS_GUI_UI_ADD_GRAPH_ELEMENT_HPP
#define GRAPH_ANALYSIS_GUI_UI_ADD_GRAPH_ELEMENT_HPP

#include <QDialog>

namespace Ui
{
    class AddGraphElement;
};

namespace graph_analysis {
namespace gui {

class AddGraphElement : public QDialog
{
    Q_OBJECT;

public:
    AddGraphElement(const QStringList& types, QWidget* parent = 0);
    ~AddGraphElement();

    QString getLabel() const;
    QString getType() const;


private:
    QString mType;
    Ui::AddGraphElement* mUi;
};

} // end namespace gui
} // end graph_analysis
#endif // GRAPH_ANALYSIS_GUI_ADD_GRAPH_ELEMENT_HPP
