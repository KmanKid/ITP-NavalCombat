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
            connect(gridLeft[x][y], SIGNAL(clicked()),this, SLOT(selectShips()));

            //Right grid to shoot at enemy
            this->gridRight[x][y] = new GridCell(this,x,y);
            this->gridRight[x][y]->setGeometry(QRect(556 +(x*40),155+(y*40),40,40));
            this->gridRight[x][y]->setIcon(QIcon(QPixmap(":/assets/grid_cell_empty.png")));
            this->gridRight[x][y]->setIconSize(QSize(40,40));
            connect(gridRight[x][y], SIGNAL(clicked()),this, SLOT(shoot()));
        }
    }
}

void MainWindow::youHaveGotMail(QString message)
{
    qDebug() << message;
    // e.g sFS-left-0-0-2
    QStringList mSplit = message.split("-");
    if(mSplit[0] == "setFieldState")
    {
        this->setFieldState(mSplit[1],mSplit[2].toInt(),mSplit[3].toInt(),mSplit[4].toInt());
    }
}

void MainWindow::shoot()
{
    GridCell* buttonSender = qobject_cast<GridCell*>(sender());
    buttonSender->setIcon(QIcon(QPixmap(":/assets/grid_cell_miss.png")));
    //send the coordinates to the server
    buttonSender->state = 0;
    QString message = QString("shot-"+QString::number(buttonSender->x)
                             +"-"+QString::number(buttonSender->y));
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
        case 6:
            iconPath = ":/assets/grid_cell_on_hover.png";
            break;
        default:
            iconPath = ":/assets/grid_cell_empty.png";
    }

    if(side == "left")
    {
        gridLeft[x][y]->setIcon(QIcon(QPixmap(iconPath)));
        gridLeft[x][y]->state = state;

    }
    if(side == "right")
    {
        gridRight[x][y]->setIcon(QIcon(QPixmap(iconPath)));
        gridRight[x][y]->state = state;
    }

}

void MainWindow::selectShips()
{
    GridCell* buttonSender = qobject_cast<GridCell*>(sender());
    int x = buttonSender->x;
    int y = buttonSender->y;
    int shipSize = 0;
    if (shipNumber <= 3)
    {
        shipSize = 1;
    }
    if (shipNumber > 3 && shipNumber <= 6)
    {
        shipSize = 2;
    }
    if (shipNumber > 6 && shipNumber <= 8)
    {
        shipSize = 3;
    }
    if (shipNumber == 9)
    {
        shipSize = 4;
    }
    if (!(shipNumber > 9) && buttonSender->state != 2)
    {

        if(!isSelctingShip)
        {
            //Player wants to select a ship now --> store x and y for preview removal
            shipTempX = x;
            shipTempY = y;
            isSelctingShip = true;
            this->setFieldState("left",x,y,2);
            if(shipSize == 1)
            {
                isSelctingShip = false;
                sendShip(x,y,shipSize,0,shipNumber); //implementation needed
                shipNumber++;
            }else
            {
                togglePreview(x, y, shipSize, false);
            }
        }else
        {
            bool valid = (shipTempX == x || shipTempY == y); // further validate --> accurate koordinates not just axis
            bool validOnXCoord = (x + shipSize-1 == shipTempX) || (x - shipSize+1 == shipTempX);
            bool validOnYCoord = (y + shipSize-1 == shipTempY) || (y - shipSize+1 == shipTempY);
            qDebug() << "validX: " << validOnXCoord;
            qDebug() << "validY: " << validOnYCoord;

            if(valid && (validOnXCoord || validOnYCoord))
            {
                togglePreview(shipTempX, shipTempY, shipSize, true);
                qDebug() << x  << ":" << y;
                qDebug() << shipTempX << ":" << shipTempY;
                if(shipTempX == x)
                {
                    if (shipTempY < y)
                    {
                        for (int i = 0; i < shipSize; i++)
                        {
                            this->setFieldState("left",shipTempX,shipTempY+i,2);
                        }
                        sendShip(shipTempX,shipTempY,shipSize,1,shipNumber);
                    }else
                    {
                        for (int i = 0; i < shipSize; i++)
                        {
                            this->setFieldState("left",shipTempX,shipTempY-i,2);
                        }
                        sendShip(x,y,shipSize,1,shipNumber);
                    }
                }
                if(shipTempY == y)
                {
                    if (shipTempX < x)
                    {
                        for (int i = 0; i < shipSize; i++)
                        {
                            this->setFieldState("left",shipTempX+i,shipTempY,2);
                        }
                        sendShip(shipTempX,shipTempY,shipSize,0,shipNumber);
                    }else
                    {
                        for (int i = 0; i < shipSize; i++)
                        {
                            this->setFieldState("left",shipTempX-i,shipTempY,2);
                        }
                        sendShip(x,y,shipSize,0,shipNumber);
                    }

                }
                isSelctingShip = false;
                //implementation needed
                shipNumber++;
            }
        }
    }
    qDebug() << "Size:" << shipSize << "Number:" << shipNumber;
}

void MainWindow::togglePreview(int x, int y, int size, bool remove)
{
    int state = 6;
    if (remove)
    {
        state = 0;
    }
    int previewXPlus = x + size -1;
    int previewXMinus = x - size +1;
    int previewYPlus = y + size -1;
    int previewYMinus = y - size +1;
    if (previewXPlus >= 0 && previewXPlus < 10)
    {
        if(gridLeft[previewXPlus][y]->state != 2)
        {
            this->setFieldState("left",previewXPlus,y,state);
        }
    }
    if (previewXMinus >= 0 && previewXMinus < 10)
    {
        if(gridLeft[previewXMinus][y]->state != 2)
        {
            this->setFieldState("left",previewXMinus,y,state);
        }
    }
    if (previewYPlus >= 0 && previewYPlus < 10)
    {
        if(gridLeft[x][previewYPlus]->state != 2)
        {
            this->setFieldState("left",x,previewYPlus,state);
        }
    }
    if (previewYMinus >= 0 && previewYMinus < 10)
    {
        if(gridLeft[x][previewYMinus]->state != 2)
        {
            this->setFieldState("left",x,previewYMinus,state);
        }
    }
}

void MainWindow::sendShip(int x, int y,int size, int orientation, int number)
{
    //send ship to server, so that he can store it
    //server needs
    QString message = QString("newShip-"+QString::number(x)
                             +"-"+QString::number(y)
                             +"-"+QString::number(size)
                             +"-"+QString::number(orientation)
                             +"-"+QString::number(number));
    client.sendTextMessage(message);
}

MainWindow::~MainWindow()
{
    delete ui;
    client.close();
}
