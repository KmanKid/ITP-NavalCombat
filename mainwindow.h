#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gridcell.h"
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QWebSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    GridCell* gridLeft[10][10];
    GridCell* gridRight[10][10];

public slots:
    //Schussfunktion
    void shoot();
    //Message-Handling von erhaltenen Nachrichten
    void youHaveGotMail(QString message);
    //Status und icon der Gridcell wird gesetzt
    void setFieldState(QString side,int x, int y, int state);
    //Funktion zum wählen der Schiffpositionen
    void selectShips();
    //Sende den Funk zu dem Server
    void sendFunk(QString message);

private:
    //Das Fenster
    Ui::MainWindow *ui;
    //Der client socket für die Kommunikation mit dem Server
    QWebSocket client;
    //Wie viele Schiffe schon Platziert wurden
    int shipNumber = 0;
    //Temporäres speichern der Koordinaten
    int shipTempX = 0;
    int shipTempY = 0;
    //Ist der Spieler immernoch am Schiffe platzieren
    bool isSelctingShip = false;
    //Ist es gerade mein Zug?
    bool isMyTurn = false;
    //Sende das platzierte Schiff an den Server
    void sendShip(int x, int y,int size, int orientation, int shipNumber);
    //Hilfsfunktion zur anzeige der Schiffsplatzierungsmarker
    void togglePreview(int x, int y, int size, bool remove = false,int pSNumber = 0);
    //Dient der Anzeige des Textes
    void showTextOnLabel(QString s);
};
#endif // MAINWINDOW_H
