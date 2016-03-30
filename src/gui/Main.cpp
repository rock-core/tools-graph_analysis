#include "GraphWidgetManager.hpp"

#include <string>
#include <iostream>
#include <QApplication>
#include <QTime>

#include <graph_analysis/gui/BaseGraphView/BaseGraphView.hpp>
#include <graph_analysis/gui/ComponentGraphEditor/ComponentGraphEditor.hpp>

using namespace graph_analysis::gui;

int main(int argc, char **argv)
{
    // setting up qt application
    QApplication app(argc, argv);

    // provide seed for force-based layouting in the LayerViewWidget and
    // ComponentEditorWidget
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    GraphWidgetManager graphManager;

    if(argc > 1)
    {
        if("--help" == std::string(argv[1]))
        {
            std::stringstream help_message;
            graphManager.helpSetup(help_message, std::string(argv[0]));
            std::cout << help_message.rdbuf();
            exit(EXIT_FAILURE);
        }
        else
        {
            std::cout << "usage: " << argv[0] << " [--help]\n";
            exit(EXIT_FAILURE);
        }
    }

    graphManager.addGraphWidget(new BaseGraphView());
    graphManager.addGraphWidget(new ComponentGraphEditor());

    int return_code = app.exec();
    return return_code;
}
