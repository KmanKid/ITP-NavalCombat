#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    client.open(QUrl("ws://127.0.0.1:8888"));
    connect(&client, &QWebSocket::textMessageReceived,this, &MainWindow::youHaveGotMail);
    ui->setupUi(this);
    for (int x = 0; x < 10; x++)
    {
        for (int y = 0; y < 10; y ++)
        {
            //Left grid for Player to place ships and see where enemy shot at
            this->gridLeft[x][y] = new GridCell(this,x,y);
            this->gridLeft[x][y]->setGeometry(QRect(44+(x*40),155+(y*40),40,40));
            this->gridLeft[x][y]->setIcon(QIcon(QPixmap(":/assets/grid_cell_empty.png")));
            this->gridLeft[x][y]->setIconSize(QSize(40,40));

            //Right grid to shoot at enemy
            this->gridRight[x][y] = new GridCell(this,x,y);
            this->gridRight[x][y]->setGeometry(QRect(556 +(x*40),155+(y*40),40,40));
            this->gridRight[x][y]->setIcon(QIcon(QPixmap(":/assets/grid_cell_empty.png")));
            this->gridRight[x][y]->setIconSize(QSize(40,40));
            connect(gridRight[x][y], SIGNAL(clicked()),this, SLOT(showChangeOnHit()));
        }
    }
}

void MainWindow::youHaveGotMail(QString message)
{
    qDebug() << message;
}

void MainWindow::showChangeOnHit()
{
    GridCell* buttonSender = qobject_cast<GridCell*>(sender());
    buttonSender->setIcon(QIcon(QPixmap(":/assets/grid_cell_miss.png")));
    //send the coordinates to the server
    buttonSender->state = 0;
    QString message = QString("x:"+QString::number(buttonSender->x)
                             +"_y:"+QString::number(buttonSender->y));
    client.sendTextMessage(message);
}

void MainWindow::setFieldState(QString side,int x, int y, int state)
{
    QString iconPath = "";
    switch(state)
    {
        case 0:
            iconPath = ":/assets/grid_cell_empty.png";
            break;
        case 1:
            iconPath = ":/assets/grid_cell_miss.png";
            break;
        case 2:
            iconPath = ":/assets/gridcell_blue_full.png";
            break;
        case 3:
            iconPath = ":/assets/gridcell_blue_hit.png";
            break;
        case 4:
            iconPath = ":/assets/gridcell_red_hit.png";
            break;
        case 5:
            iconPath = ":/assets/gridcell_red_full.png";
            break;
        default:
            iconPath = ":/assets/grid_cell_empty.png";
    }

    if(side == "left")
    {
        gridLeft[x][y]->setIcon(QIcon(QPixmap(iconPath)));

    }
    if(side == "right")
    {
        gridRight[x][y]->setIcon(QIcon(QPixmap(iconPath)));
    }

}

MainWindow::~MainWindow()
{
    delete ui;
    client.close();
}
