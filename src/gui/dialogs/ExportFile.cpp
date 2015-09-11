#include "ExportFile.hpp"

namespace graph_analysis {
namespace gui {
namespace dialogs {

ExportFile::ExportFile(const QString& filter, QWidget* parent)
    : QFileDialog(parent, "Export file")
{
    setNameFilter(filter);
    //setLabelText(QFileDialog::FileName,"Test label" );
    setAcceptMode(QFileDialog::AcceptSave);
    //setOption(QFileDialog::DontUseNativeDialog);
}

representation::Type ExportFile::getTypeName() const
{
    QString filterName = selectedNameFilter().trimmed();
    std::map<representation::Type, std::string>::const_iterator cit = representation::TypeTxt.begin();
    for(; cit != representation::TypeTxt.end(); ++cit)
    {
        if(filterName.startsWith(cit->second.c_str()))
        {
            return cit->first;
        }
    }
    return representation::UNKNOWN;
}

} // end namespace dialogs
} // end namespace gui
} // end namespace graph_analysis
