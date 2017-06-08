#ifndef GRAPH_ANALYSIS_GUI_DIALOGS_IO_DIALOG_HPP
#define GRAPH_ANALYSIS_GUI_DIALOGS_IO_DIALOG_HPP

#include <QWidget>
#include <QString>
#include "../../GraphIO.hpp"
#include "../../BaseGraph.hpp"

namespace graph_analysis {
namespace gui {
namespace dialogs {

class IODialog
{
public:
    /**
     * Get the suffix filter for all supported graph format that can be imported
     * \return The string filter that can be passed to a QFileDialog
     */
    static QString getGraphReaderSuffixFilter();

    /**
     * Get the suffix filter for all supported graph format that can be exported
     * \return The string filter that can be passed to a QFileDialog
     */
    static QString getGraphWriterSuffixFilter();

    /**
     * Get the filename for importing a graph
     * \return filename or empty string on failure
     */
    static QString getImportGraphFilename(QWidget* parent = NULL);

    /**
     * Get the filename for exportingg a graph
     * \return filename or empty string on failure
     */
    static QString getExportGraphFilename(QWidget* parent = NULL);

    /**
     * Get the representation type from a given filter selection
     */
    static graph_analysis::representation::Type getTypeNameFromFilter(QString filtername);

    /**
     * Retrieve the imported graph, i.e. opens a file dialog (filtered by
     * supported import types)
     * \return the imported graph, or empty shared pointer, on failure
     */
    static BaseGraph::Ptr importGraph(QWidget* parent = NULL);

    /**
     * Export a given graph, i.e. opens a file dialog for selecting the target
     * and file format
     */
    static void exportGraph(const BaseGraph::Ptr& graph, QWidget* parent = NULL);
};

} // end namespace dialogs
} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_DIALOGS_IO_DIALOG_HPP
