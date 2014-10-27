#include "mainwindow.h"
#include "randlib.h"
#include <ctime>
#include <QApplication>

int main(int argc, char *argv[])
{
    initialize_random_generator(time(0));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
