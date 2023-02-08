#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for (int x = 0; x < 10; x++)
    {
        for (int y = 0; y < 10; y ++)
        {
            this->gridLeft[x][y] = new QLabel(this);
            this->gridLeft[x][y]->setGeometry(QRect(44+(x*40),155+(y*40),40,40));
            this->gridLeft[x][y]->setPixmap(QPixmap(":/assets/grid_cell_empty.png"));
            connect(gridLeft[x][y], SIGNAL(),this, SLOT(showChangeOnHit(x,y)));
        }
    }
}

void MainWindow::showChangeOnHit(int x, int y)
{
    this->gridLeft[x][y]->setPixmap(QPixmap(":/assets/grid_cell_miss.png"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

