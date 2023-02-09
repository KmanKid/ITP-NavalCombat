#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gridcell.h"
#include "client.h"
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>

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
private:
    Client client;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
