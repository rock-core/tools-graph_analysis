#include <string>
#include <iostream>

#include <QApplication>
#include <QTime>

#include "GraphAnalysisGui.hpp"

using namespace graph_analysis::gui;

int main(int argc, char **argv)
{
    // setting up qt application
    QApplication app(argc, argv);

    // provide seed for force-based layouting in the LayerViewWidget and
    // ComponentEditorWidget
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    GraphAnalysisGui w;
    w.show();

    app.setApplicationName(w.windowTitle());

    return app.exec();
}
