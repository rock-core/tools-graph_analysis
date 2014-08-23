#ifndef OMVIZ_FILTERWIDGET_REGEX_FILTER_DIALOG_HPP
#define OMVIZ_FILTERWIDGET_REGEX_FILTER_DIALOG_HPP

#include <QDialog>
#include <graph_analysis/Filter.hpp>

class QLineEdit;
class QComboBox;
class QCheckBox;

namespace Ui
{
    class AddRegexFilterDialog;
}

namespace omviz {

struct RegexFilterData
{
    std::string regex;
    graph_analysis::filters::Type type;
    bool invert;
};

class AddRegexFilterDialog : public QDialog
{
    Q_OBJECT

public:
    AddRegexFilterDialog(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    ~AddRegexFilterDialog();


private slots:
    void checkAndStoreValues();
    RegexFilterData getFilterData(QLineEdit* lineEdit, QComboBox* comboBox, QCheckBox* checkBox);

private:
    Ui::AddRegexFilterDialog* mUi;
    graph_analysis::Filter<graph_analysis::Edge::Ptr>::Ptr mpFilter;
};

} // end namespace omviz
#endif // OMVIZ_FILTERWIDGET_ADD_REGEX_FILTER_DIALOG_HPP
