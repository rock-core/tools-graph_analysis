#include "IODialog.hpp"

#include <QDir>
#include <QCoreApplication>
#include <QSettings>
#include <QFileInfo>
#include <QFileDialog>
#include <QTranslator>
#include <QMessageBox>
#include <QPainter>
#include <QPrinter>
#include <QPrintDialog>
#include <QSvgGenerator>

#include <sstream>
#include "../../GraphIO.hpp"

namespace graph_analysis {
namespace gui {
namespace dialogs {

QString IODialog::getGraphReaderSuffixFilter()
{
    // Constructing the writer suffix filter
    io::GraphIO::SuffixMap suffixMap = graph_analysis::io::GraphIO::getSuffixMap();
    io::GraphIO::ReaderMap readerMap = graph_analysis::io::GraphIO::getReaderMap();
    io::GraphIO::ReaderMap::const_iterator rit = readerMap.begin();

    std::stringstream ss;
    for(;;)
    {
        ss << representation::TypeTxt[rit->first] << " (";
        io::GraphIO::SuffixMap::const_iterator sit = suffixMap.begin();
        for(; sit != suffixMap.end(); ++sit)
        {
            if(sit->second == rit->first)
            {
                ss << "*." << sit->first << " ";
            }
        }
        ss << ")";

        ++rit;
        if(rit != readerMap.end())
        {
            ss << ";;";
        }
        else
        {
            break;
        }
    }

    return QString(ss.str().c_str());
}

QString IODialog::getGraphWriterSuffixFilter()
{
    // Constructing the writer suffix filter
    io::GraphIO::SuffixMap suffixMap = io::GraphIO::getSuffixMap();
    io::GraphIO::WriterMap writerMap = io::GraphIO::getWriterMap();
    io::GraphIO::WriterMap::const_iterator wit = writerMap.begin();

    std::stringstream ss;
    for(;;)
    {
        ss << representation::TypeTxt[ wit->first ] << " (";
        io::GraphIO::SuffixMap::const_iterator sit = suffixMap.begin();
        for(; sit != suffixMap.end(); ++sit)
        {
            if(sit->second == wit->first)
            {
                ss << "*." << sit->first << " ";
            }
        }
        ss << ")";

        ++wit;
        if(wit != writerMap.end())
        {
            ss << ";;";
        } else {
            break;
        }
    }
    // End constructing the writer suffix filter
    return QString(ss.str().c_str());
}

QString IODialog::getImportGraphFilename(QWidget* parent)
{
    QSettings settings(QCoreApplication::organizationName(),"IO");
    QString dir = QDir::currentPath();
    if(settings.contains("recentImportDir"))
    {
        dir = settings.value("recentImportDir").toString();
    }

    QString selectedFilter;
    QString filter = getGraphReaderSuffixFilter();
    QString filename = QFileDialog::getOpenFileName(
        parent, QObject::tr("Choose input file"), dir,
        filter, &selectedFilter);

    if(filename.isEmpty())
    {
        return filename;
    }

    QFileInfo fileinfo(filename);
    settings.setValue("recentDirImport", fileinfo.absolutePath());

    QStringList files = settings.value("recentImportFileList").toStringList();
    files.removeAll(filename);
    files.prepend(filename);
    while(files.size() > 10)
    {
        files.removeLast();
    }
    settings.setValue("recentImportFileList", files);

    return filename;
}

QString IODialog::getExportGraphFilename(QWidget* parent)
{
    QSettings settings;
    QString dir = QDir::currentPath();
    if(settings.contains("recentExportDir"))
    {
        dir = settings.value("recentExportDir").toString();
    }

    QString selectedFilter;
    QString filter = getGraphReaderSuffixFilter();

    QString filename;
    bool done = false;
    while(!done)
    {
        done = true;

        filename = QFileDialog::getSaveFileName(
            parent, QObject::tr("Choose input file"), dir,
            filter, &selectedFilter);

        if(filename.isEmpty())
        {
            return filename;
        } else {
            representation::Type intendedSuffix = io::GraphIO::getTypeFromFilename(filename.toStdString());
            representation::Type selectedSuffix = getTypeNameFromFilter(selectedFilter);
            if(intendedSuffix == representation::UNKNOWN)
            {
                filename += "." + QString( io::GraphIO::getSuffix( selectedSuffix ).c_str());
            } else if(intendedSuffix != selectedSuffix)
            {
                QString msg = "Extension is ambiguous: '" +
                    filename + QString("' has extension '") +
                    QString(io::GraphIO::getSuffix(intendedSuffix).c_str()) +
                    QString("', but selected filter for export was '") +
                    QString(io::GraphIO::getSuffix(selectedSuffix).c_str()) +
                    QString("'");

                QMessageBox::StandardButton button = QMessageBox::question(parent,
                    QObject::tr("Filename Extension Ambiguity"),
                    msg, QMessageBox::Retry | QMessageBox::Ignore);

                if(button == QMessageBox::Retry)
                {
                    done = false;
                }
            }
        }
    }

    QFileInfo fileinfo(filename);
    settings.setValue("recentExportDir", fileinfo.absolutePath());

    QStringList files = settings.value("recentExportFileList").toStringList();
    files.removeAll(filename);
    files.prepend(filename);
    while(files.size() > 10)
    {
        files.removeLast();
    }
    settings.setValue("recentExportFileList", files);

    return filename;
}

graph_analysis::representation::Type IODialog::getTypeNameFromFilter(QString filename)
{
    QString filterName = filename.trimmed();
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

BaseGraph::Ptr IODialog::importGraph(QWidget* parent, const QString& file)
{
    BaseGraph::Ptr graph = BaseGraph::getInstance();
    QString filename;
    if(file.isEmpty())
    {
        filename = getImportGraphFilename(parent);
    } else {
        filename = file;
    }

    if(filename.isEmpty())
    {
        return BaseGraph::Ptr();
    }

    try {
        io::GraphIO::read(filename.toStdString(), graph);
    } catch(const std::exception& e)
    {
        std::string msg = "Failed to import '" + filename.toStdString() + "': " + e.what();
        QMessageBox::critical(parent, QObject::tr("Graph Import Failed"), msg.c_str());
        return BaseGraph::Ptr();
    }
    return graph;

}

void IODialog::exportGraph(const BaseGraph::Ptr& graph, QWidget* parent)
{
    QString filename = getExportGraphFilename();
    if(!filename.isEmpty())
    {
        try {
            io::GraphIO::write(filename.toStdString(), graph);
        } catch(const std::exception& e)
        {
            std::string msg = "Export of graph to '" + filename.toStdString() + "' failed " + e.what();
            QMessageBox::critical(parent, QObject::tr("Graph Export Failed"), msg.c_str());
            return;
        }
    }
}

void IODialog::exportScene(QGraphicsScene* scene, QWidget* parent)
{
    QString filename = QFileDialog::getSaveFileName(parent, "Export scene", QCoreApplication::applicationDirPath(), "PDF Files (*.pdf);;SVG Files (*.svg)");
    if(!filename.isNull())
    {
        QFileInfo fileinfo(filename);
        QString suffix = fileinfo.completeSuffix();
        if(suffix == "pdf")
        {
            exportSceneAsPdf(scene, filename, parent);
        } else if(suffix == "svg")
        {
            exportSceneAsSvg(scene, filename);
        } else {
            std::string msg = "No exporter for " + suffix.toStdString() + " available.";
            QMessageBox::warning(parent, QObject::tr("Scene export failed"), msg.c_str());
        }
    }
}

void IODialog::exportSceneAsPdf(QGraphicsScene* scene, QString filename, QWidget* parent)
{
    if(filename.isNull())
    {
        return;
    }

    QRectF rect = scene->itemsBoundingRect();

    QPrinter printer(QPrinter::HighResolution);
    printer.setPaperSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Landscape);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filename);

    QPrintDialog printDialog(&printer, parent);
    if(printDialog.exec() == QDialog::Accepted)
    {
        QPainter painter(&printer);
        qreal scalingFactor = 20.0;
        scene->render(&painter, QRectF(0,0,rect.width()*scalingFactor, rect.height()*scalingFactor));
    }
}

void IODialog::exportSceneAsSvg(QGraphicsScene* scene, QString filename)
{
    if(filename.isNull())
    {
        return;
    }

    QRectF rect = scene->itemsBoundingRect();
    QSvgGenerator svgGen;
    svgGen.setFileName(filename);
    qreal scalingFactor = 0.2;
    svgGen.setSize(QSize(rect.width()*scalingFactor, rect.height()*scalingFactor));
    svgGen.setTitle("Graph Analysis SVG");
    svgGen.setDescription("SVG Drawing");
    QPainter svgPainter(&svgGen);
    scene->render(&svgPainter, rect);
}


} // end namespace dialogs
} // end namespace gui
} // end namespace graph_analysis
