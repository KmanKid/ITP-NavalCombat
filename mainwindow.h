#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QLabel* gridLeft[10][10];

public slots:
    void showChangeOnHit(int x, int y);
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
