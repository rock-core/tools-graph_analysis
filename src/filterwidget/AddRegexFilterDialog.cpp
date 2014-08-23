#include "AddRegexFilterDialog.hpp"
#include "ui_add_regex_filter_dialog.h"

#include <base/Logging.hpp>
#include <graph_analysis/filters/RegexFilters.hpp>
#include <QPalette>

namespace omviz
{

AddRegexFilterDialog::AddRegexFilterDialog(QWidget* parent, Qt::WindowFlags flags)
    : QDialog(parent, flags)
    , mUi( new Ui::AddRegexFilterDialog())
{
    mUi->setupUi(this);

    connect(mUi->lineEdit_Regex_SourceNode, SIGNAL(textChanged(const QString&)), this, SIGNAL(checkAndStoreValues));
    connect(mUi->lineEdit_Regex_TargetNode, SIGNAL(textChanged(const QString&)), this, SIGNAL(checkAndStoreValues));
    connect(mUi->lineEdit_Regex_Edge, SIGNAL(textChanged(const QString&)), this, SIGNAL(checkAndStoreValues));

    connect(mUi->buttonBox, SIGNAL(accepted()), this, SIGNAL(checkAndStoreValues()));
}

AddRegexFilterDialog::~AddRegexFilterDialog()
{
    delete mUi;
}

RegexFilterData AddRegexFilterDialog::getFilterData(QLineEdit* lineEdit, QComboBox* comboBox, QCheckBox* checkBox)
{
    RegexFilterData data;
    QPalette palette = lineEdit->palette();
    try {
        data.regex = lineEdit->text().toStdString();
        data.type = graph_analysis::filters::TxtType[ comboBox->currentText().toStdString() ];

        data.invert = false;
        if(checkBox->checkState() == Qt::Checked)
        {
            data.invert = true;
        }
        palette.setColor(QPalette::Text, Qt::black);
        lineEdit->setPalette(palette);
        lineEdit->setToolTip(QString("Valid filter expression"));
    } catch(const boost::regex_error& e)
    {
        palette.setColor(QPalette::Text, Qt::red);
        lineEdit->setPalette(palette);
        lineEdit->setToolTip(QString(e.what()));
        throw std::invalid_argument(e.what());
    }
    return data;

}
void AddRegexFilterDialog::checkAndStoreValues()
{
    using namespace graph_analysis;
    bool invalidExpressionEntered = false;
    RegexFilterData dataSourceNode;
    RegexFilterData dataTargetNode;
    RegexFilterData dataEdge;

    try {
        dataSourceNode = getFilterData(
                mUi->lineEdit_Regex_SourceNode,
                mUi->comboBox_Type_SourceNode,
                mUi->checkBox_Invert_SourceNode);
    } catch(const std::invalid_argument& e)
    {
        LOG_INFO_S << "Source Node: " << e.what();
        invalidExpressionEntered = true;
    }

    try {
        dataTargetNode = getFilterData(
                mUi->lineEdit_Regex_TargetNode,
                mUi->comboBox_Type_TargetNode,
                mUi->checkBox_Invert_TargetNode);
    } catch(const std::invalid_argument& e)
    {
        LOG_INFO_S << "Target Node: " << e.what();
        invalidExpressionEntered = true;
    }

    try {
        dataEdge = getFilterData(
                mUi->lineEdit_Regex_Edge,
                mUi->comboBox_Type_Edge,
                mUi->checkBox_Invert_Edge);
    } catch(const std::invalid_argument& e)
    {
        LOG_INFO_S << "Edge: " << e.what();
        invalidExpressionEntered = true;
    }

    if(invalidExpressionEntered)
    {
        mpFilter.reset();
        return;
    }
       
    filters::VertexRegexFilter sourceNodeFilter(dataSourceNode.regex
                , dataSourceNode.type
                , dataSourceNode.invert);

    filters::VertexRegexFilter targetNodeFilter(dataTargetNode.regex
                , dataTargetNode.type
                , dataTargetNode.invert);

    filters::EdgeRegexFilter edgeFilter(dataEdge.regex
                , dataEdge.type
                , dataEdge.invert);

    mpFilter = Filter<Edge::Ptr>::Ptr( new filters::CombinedEdgeRegexFilter(targetNodeFilter, edgeFilter, sourceNodeFilter));
}

} // end namespace omviz
