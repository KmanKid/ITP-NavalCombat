#include "mainwindow.h"
#include "ui_mainwindow.h"

//MainWindow implementiert QMainWindow und Ui
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //Der Socket öffnet localhost:8888
    client.open(QUrl("ws://127.0.0.1:8888"));
    //Der Socket wird die youHaveGotMail Funktion aufrufen wenn er eine Nachricht bekommt
    connect(&client, &QWebSocket::textMessageReceived,this, &MainWindow::youHaveGotMail);
    //Das User Interface wird vorbereitet
    ui->setupUi(this);
    //Im folgenden werden die Beiden Spielfeld
    for (int x = 0; x < 10; x++)
    {
        for (int y = 0; y < 10; y ++)
        {
            //Das Linke Raster/Spielfeld wird erzeugt
            this->gridLeft[x][y] = new GridCell(this,x,y);
            //Die Werte für Größen etc. entstammen dem Mockup
            this->gridLeft[x][y]->setGeometry(QRect(44+(x*40),155+(y*40),40,40));
            this->gridLeft[x][y]->setIcon(QIcon(QPixmap(":/assets/grid_cell_empty.png")));
            this->gridLeft[x][y]->setIconSize(QSize(40,40));
            //Das linke Spielfeld wird bei klicken die selectShips Funktion aufrufen
            connect(gridLeft[x][y], SIGNAL(clicked()),this, SLOT(selectShips()));

            //Das Rechte Raster/Spielfeld wird erzeugt
            this->gridRight[x][y] = new GridCell(this,x,y);
            //Die Werte für Größen etc. entstammen dem Mockup
            this->gridRight[x][y]->setGeometry(QRect(556 +(x*40),155+(y*40),40,40));
            this->gridRight[x][y]->setIcon(QIcon(QPixmap(":/assets/grid_cell_empty.png")));
            this->gridRight[x][y]->setIconSize(QSize(40,40));
            //Das Rechte Spielfeld wird bei klicken die shoot Funktion aufrufen
            connect(gridRight[x][y], SIGNAL(clicked()),this, SLOT(shoot()));
        }
    }
}

//Diese Methode verarbeitet die vom Server geschickten Nachrichten
void MainWindow::youHaveGotMail(QString message)
{
    //Die Nachrichten werden an - getrennt, somit kann man verschiedene Parameter erkennen
    QStringList mSplit = message.split("-");

    //Hier könnte man auch switch case benutzen
    if(mSplit[0] == "setFieldState")
    {
        //setField bekommt als Eingabe Parameter die Seite, x und y Koordinaten und den status
        this->setFieldState(mSplit[1],mSplit[2].toInt(),mSplit[3].toInt(),mSplit[4].toInt());
    }
    if(mSplit[0] == "yourTurn")
    {
        //Dieser Client ist an der Reihe
        this->isMyTurn = true;
    }
    if(mSplit[0] == "notYourTurn")
    {
        //Dieser Client ist nicht mehr an der Reihe
        this->isMyTurn = false;
    }
    if(mSplit[0] == "setText")
    {
        //Der Text des Labels unterhalb des Logos wird auf den Wert nach dem - gesezt
        this->showTextOnLabel(mSplit[1]);
    }
}

//Diese Funktion vearbeitet den Klick auf ein rechtes Feld und schickt die Koordinaten zum Server
void MainWindow::shoot()
{
    // Wenn ich an der Reihe bin darf ich schießen
    if(isMyTurn)
    {
        GridCell* buttonSender = qobject_cast<GridCell*>(sender());
        buttonSender->state = 0;
        QString message = QString("shoot-"+QString::number(buttonSender->x)
                                 +"-"+QString::number(buttonSender->y));
        client.sendTextMessage(message);
    }

}

//Durch diese Funktion kann eine Gridcell den entsprechenden Status erhalten
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
        case 7:
            iconPath = ":/assets/gridcell_blue_full_preview.png";
            break;
        case 8:
            iconPath = ":/assets/ship_destroyed_horizontal.png";
            break;
        case 9:
            iconPath = ":/assets/ship_destroyed_vertical.png";
            break;
        case 10:
            iconPath = ":/assets/ship_destroyed_single.png";
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

//Die Schiffe auswählen
void MainWindow::selectShips()
{
    GridCell* buttonSender = qobject_cast<GridCell*>(sender());
    int x = buttonSender->x;
    int y = buttonSender->y;
    int shipSize = 0;
    int previewStateNumber = 7;
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
    if (!(shipNumber > 9) && buttonSender->state != previewStateNumber)
    {

        if(!isSelctingShip)
        {
            //Player wants to select a ship now --> store x and y for preview removal
            shipTempX = x;
            shipTempY = y;
            isSelctingShip = true;
            this->setFieldState("left",x,y,previewStateNumber);
            if(shipSize == 1)
            {
                isSelctingShip = false;
                sendShip(x,y,shipSize,0,shipNumber); //implementation needed
                shipNumber++;
            }else
            {
                togglePreview(x, y, shipSize, false,previewStateNumber);
            }
        }else
        {
            bool valid = (shipTempX == x || shipTempY == y); // further validate --> accurate koordinates not just axis
            bool validOnXCoord = (x + shipSize-1 == shipTempX) || (x - shipSize+1 == shipTempX);
            bool validOnYCoord = (y + shipSize-1 == shipTempY) || (y - shipSize+1 == shipTempY);

            if(valid && (validOnXCoord || validOnYCoord))
            {
                togglePreview(shipTempX, shipTempY, shipSize, true,previewStateNumber);
                qDebug() << x  << ":" << y;
                qDebug() << shipTempX << ":" << shipTempY;
                if(shipTempX == x)
                {
                    if (shipTempY < y)
                    {
                        for (int i = 0; i < shipSize; i++)
                        {
                            this->setFieldState("left",shipTempX,shipTempY+i,previewStateNumber);
                        }
                        sendShip(shipTempX,shipTempY,shipSize,1,shipNumber);
                    }else
                    {
                        for (int i = 0; i < shipSize; i++)
                        {
                            this->setFieldState("left",shipTempX,shipTempY-i,previewStateNumber);
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
                            this->setFieldState("left",shipTempX+i,shipTempY,previewStateNumber);
                        }
                        sendShip(shipTempX,shipTempY,shipSize,0,shipNumber);
                    }else
                    {
                        for (int i = 0; i < shipSize; i++)
                        {
                            this->setFieldState("left",shipTempX-i,shipTempY,previewStateNumber);
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
}

void MainWindow::togglePreview(int x, int y, int size, bool remove,int pSNumber)
{
    int blueShip = 2;
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
        if(gridLeft[previewXPlus][y]->state != pSNumber && gridLeft[previewXPlus][y]->state != blueShip)
        {
            this->setFieldState("left",previewXPlus,y,state);
        }
    }
    if (previewXMinus >= 0 && previewXMinus < 10)
    {
        if(gridLeft[previewXMinus][y]->state != pSNumber && gridLeft[previewXMinus][y]->state != blueShip)
        {
            this->setFieldState("left",previewXMinus,y,state);
        }
    }
    if (previewYPlus >= 0 && previewYPlus < 10)
    {
        if(gridLeft[x][previewYPlus]->state != pSNumber && gridLeft[x][previewYPlus]->state != blueShip)
        {
            this->setFieldState("left",x,previewYPlus,state);
        }
    }
    if (previewYMinus >= 0 && previewYMinus < 10)
    {
        if(gridLeft[x][previewYMinus]->state != pSNumber && gridLeft[x][previewYMinus]->state != blueShip)
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

void MainWindow::showTextOnLabel(QString s)
{
    this->ui->manual->setText(s);
}

MainWindow::~MainWindow()
{
    delete ui;
    client.close();
}
