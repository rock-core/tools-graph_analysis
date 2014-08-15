#include <iostream>

#include <QApplication>
#include <omviz/MainWindow.hpp>

int main(int argc, char** argv)
{
    using namespace omviz;
    QApplication app(argc, argv);

    omviz::MainWindow mainWindow;

    return app.exec();
}
