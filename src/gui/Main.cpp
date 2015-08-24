/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "GraphManager.hpp"

#include <string>
#include <sstream>
#include <QApplication>
#include <base/Logging.hpp>

// hardwired import filename for init; should have an extension
#define FILENAME  "../../resources/gexf/export_samples/props__and__operations_test.gexf"

graph_analysis::gui::GraphManager *graphManager;

// prints verbose help message regarding usage of 'cmd' to string stream 'ss'
void help_setup(std::stringstream& ss, const std::string& cmd)
{
    if(!graphManager)
    {
        LOG_ERROR_S << "graph_analysis::gui::Main::help_setup: cannot load the help message - the graph manager hasn't been initialized";
        exit(1);
    }
    graphManager->helpSetup(ss, cmd);
}

int main(int argc, char **argv)
{
    if(argc > 1)
    {
        if("--help" == std::string(argv[1]))
        {
            std::stringstream help_message;
            help_setup(help_message, std::string(argv[0]));
            std::cout << help_message.rdbuf();
            return 0;
        }
        else
        {
            std::cout << "usage: " << argv[0] << " [--help]\n";
            return 0;
        }
    }

    // setting up qt application
    QApplication app(argc, argv);

    graphManager = new graph_analysis::gui::GraphManager(
#ifdef FILENAME
                    FILENAME
#endif
                        );

    int return_code = app.exec();
    delete graphManager;
    return return_code;
}
