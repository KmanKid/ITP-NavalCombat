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
    void shoot();
    void youHaveGotMail(QString message);
    void setFieldState(QString side,int x, int y, int state);
    void selectShips();

private:
    Ui::MainWindow *ui;
    QWebSocket client;
    int shipNumber = 0;
    int shipTempX = 0;
    int shipTempY = 0;
    bool isSelctingShip = false;
    void sendShip(int x, int y,int size, int orientation, int shipNumber);
    void togglePreview(int x, int y, int size, bool remove = false,int pSNumber = 0);
};
#endif // MAINWINDOW_H
