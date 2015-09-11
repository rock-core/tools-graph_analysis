#ifndef GRAPH_ANALYSIS_GUI_DIALOGS_EXPORT_FILE_HPP
#define GRAPH_ANALYSIS_GUI_DIALOGS_EXPORT_FILE_HPP

#include <QFileDialog>
#include <graph_analysis/GraphIO.hpp>

namespace graph_analysis {
namespace gui {
namespace dialogs {

class ExportFile : public QFileDialog
{
    Q_OBJECT;

public:
    ExportFile(const QString& filter, QWidget* parent = 0);
    virtual ~ExportFile() {}

    representation::Type getTypeName() const;
    QString getFilename() const { return selectedFiles().at(0); }

};

} // end namespace dialogs
} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_DIALOGS_EXPORT_FILE_HPP
