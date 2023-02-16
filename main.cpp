#include <iostream>
#include "ui/ffpemg_player.h"
#include "Headers/mainwindow.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc,argv);
    Mainwindow main_ui;
    main_ui.show();
    return a.exec();
}