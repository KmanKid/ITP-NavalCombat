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
    void showChangeOnHit();
    void youHaveGotMail(QString message);
    void setFieldState(QString side,int x, int y, int state);

private:
    Ui::MainWindow *ui;
    QWebSocket client;
};
#endif // MAINWINDOW_H
