#include "GraphWidgetManager.hpp"

#include <string>
#include <sstream>
#include <QApplication>
#include <graph_analysis/gui/component_editor/ComponentEditorWidget.hpp>
#include <graph_analysis/gui/layer_view/LayerViewWidget.hpp>

// prints verbose help message regarding usage of 'cmd' to string stream 'ss'
void help_setup(std::stringstream& ss, const std::string& cmd, graph_analysis::gui::GraphWidgetManager& graphManager)
{
    graphManager.helpSetup(ss, cmd);
}

int main(int argc, char **argv)
{
    // setting up qt application
    QApplication app(argc, argv);

    using namespace graph_analysis::gui;
    GraphWidgetManager graphManager;

    if(argc > 1)
    {
        if("--help" == std::string(argv[1]))
        {
            std::stringstream help_message;
            help_setup(help_message, std::string(argv[0]), graphManager);
            std::cout << help_message.rdbuf();
            return 0;
        }
        else
        {
            std::cout << "usage: " << argv[0] << " [--help]\n";
            return 0;
        }
    }

    graphManager.addGraphWidget(new ComponentEditorWidget());
    graphManager.addGraphWidget(new LayerViewWidget());

    int return_code = app.exec();
    return return_code;
}
