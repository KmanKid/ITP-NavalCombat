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
            this->gridLeft[x][y] = new GridCell(this,x,y);
            this->gridLeft[x][y]->setGeometry(QRect(44+(x*40),155+(y*40),40,40));
            this->gridLeft[x][y]->setIcon(QIcon(QPixmap(":/assets/grid_cell_empty.png")));
            this->gridLeft[x][y]->setIconSize(QSize(40,40));
            connect(gridLeft[x][y], SIGNAL(clicked()),this, SLOT(showChangeOnHit()));
        }
    }
}

void MainWindow::showChangeOnHit()
{
    GridCell* buttonSender = qobject_cast<GridCell*>(sender());
    buttonSender->setIcon(QIcon(QPixmap(":/assets/grid_cell_miss.png")));
    qDebug() << "X" << buttonSender->x+1;
    qDebug() << "Y" << buttonSender->y+1;
}

MainWindow::~MainWindow()
{
    delete ui;
}
