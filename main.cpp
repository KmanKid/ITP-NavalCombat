#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //Deklarieren QApplication
    QApplication a(argc, argv);
    //Deklarieren Main Window
    MainWindow w;
    //Anzeige Main Window
    w.show(); 
    //Favicon Anzeigen
    a.setWindowIcon(QIcon(":/assets/nc_icon.png"));
    //Application ausführen
    return a.exec();
}
