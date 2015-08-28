#include "GraphManager.hpp"

#include <string>
#include <sstream>
#include <QApplication>

// prints verbose help message regarding usage of 'cmd' to string stream 'ss'
void help_setup(std::stringstream& ss, const std::string& cmd, graph_analysis::gui::GraphManager& graphManager)
{
    graphManager.helpSetup(ss, cmd);
}

int main(int argc, char **argv)
{
    // setting up qt application
    QApplication app(argc, argv);
    graph_analysis::gui::GraphManager graphManager;

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

    int return_code = app.exec();
    return return_code;
}
