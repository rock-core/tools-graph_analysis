#ifndef GRAPH_ANALYSIS_GUI_DIALOGS_IO_DIALOG_HPP
#define GRAPH_ANALYSIS_GUI_DIALOGS_IO_DIALOG_HPP

#include <QWidget>
#include <QString>
#include <QGraphicsScene>
#include "../../GraphIO.hpp"
#include "../../BaseGraph.hpp"

namespace graph_analysis {
namespace gui {
namespace dialogs {

class IODialog
{
public:
    static QString DefaultSettingsLabel;
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
    static QString getImportGraphFilename(QWidget* parent = NULL,
            const QString& settingsLabel = DefaultSettingsLabel);

    /**
     * Get the filename for exportingg a graph
     * \return filename or empty string on failure
     */
    static QString getExportGraphFilename(QWidget* parent = NULL,
            const QString& settingsLabel = DefaultSettingsLabel);

    /**
     * Get the representation type from a given filter selection
     */
    static graph_analysis::representation::Type getTypeNameFromFilter(QString filtername);

    /**
     * Retrieve the imported graph, i.e. opens a file dialog (filtered by
     * supported import types)
     * \param parent Parent of this dialog
     * \param filename of the file to be loaded (to skip showing the menu)
     * \param label for the QSetting for creating multiple recent filelists
     * \return the imported graph, or empty shared pointer, on failure
     */
    static BaseGraph::Ptr importGraph(QWidget* parent = NULL,
            const QString& filename = QString(),
            const QString& settingsLabel = DefaultSettingsLabel);

    /**
     * Export a given graph, i.e. opens a file dialog for selecting the target
     * and file format
     * \param graph Graph to export
     * \param parent Parent widget point
     * \param settingsLabel Label for the QSettings to create disjoint lists for
     *    recentExportDir, recentExportFileList, recentExportFile
     */
    static void exportGraph(const BaseGraph::Ptr& graph,
            QWidget* parent = NULL,
            const QString& settingsLabel = DefaultSettingsLabel);

    /**
     * Opens a file dialog to save a graphics scene
     * \param scene Scene to save
     * \param parent Parent of the dialog to be opened
     */
    static void exportScene(QGraphicsScene* scene,
            QWidget* parent = NULL);

    /**
     * Export a graphics scene as Pdf, will open a printer dialog with
     * preconfigure setting to save as pdf
     * \param scene Scene to save
     * \param filename Filename to use for saving
     * \param parent Parent of the dialog to be opened
     */
    static void exportSceneAsPdf(QGraphicsScene* scene,
            QString filename,
            QWidget* parent = NULL);

    /**
     * Export a graphics scene as svg
     * \param scene Scene to save
     * \param filename Filename to use for saving
     */
    static void exportSceneAsSvg(QGraphicsScene* scene,
            QString filename);
};

} // end namespace dialogs
} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_DIALOGS_IO_DIALOG_HPP
