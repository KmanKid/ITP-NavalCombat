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
    //Connect the editField to send content to server on Edit finished
    connect(ui->freundFunk, &QLineEdit::textChanged, this, &MainWindow::sendFunk);
    //Connect the editField to send content to server on Edit finished
    connect(ui->nameInput, &QLineEdit::textChanged, this, &MainWindow::sendName);
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
    if(mSplit[0] == "setFeindFunk")
    {
        //setzt den Text des Gegenspielers auf das soeben getippte
        this->ui->feindFunk->setText(mSplit[1]);
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
    //Anlegen des Pfades zu der icon ressource
    QString iconPath = "";
    //Abhängig des status wird das jeweilige Icon gesetzt
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

    //Nun wird entweder das Feld in der linken oder der rechten Seite gesetzt
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
    //Der Button sender wird zu einem GridCell Object gecastet
    GridCell* buttonSender = qobject_cast<GridCell*>(sender());
    //x und y Koordinaten sind die der Gridcell die ausgewählt wurde
    int x = buttonSender->x;
    int y = buttonSender->y;
    int shipSize = 0;
    //Für die Vorschau wird der status 7 gesetzt
    int previewStateNumber = 7;
    //Die ersten 4 Schiffe haben die Länge 1
    if (shipNumber <= 3)
    {
        shipSize = 1;
    }
    //Die nächsten 3 Schiffe haben die länge 2
    if (shipNumber > 3 && shipNumber <= 6)
    {
        shipSize = 2;
    }
    //Die nächsten 2 Schiffe haben die Länge 3
    if (shipNumber > 6 && shipNumber <= 8)
    {
        shipSize = 3;
    }
    //Das letzte Schiff hat die Länge 4
    if (shipNumber == 9)
    {
        shipSize = 4;
    }
    //Nur wenn es noch Schiffe zu platzieren gibt
    //und das ausgewählte Feld kein Vorschaufeld geht es weiter
    if (!(shipNumber > 9) && buttonSender->state != previewStateNumber)
    {
        //Wenn der Spieler noch kein Feld für den Schiffanfang ausgewählt hat
        if(!isSelctingShip)
        {
            //Speichern der x und y Koordinaten, also den Startpunkt des Schiffes
            shipTempX = x;
            shipTempY = y;
            //Nun ist der Spieler am auswählen des Schiffes
            isSelctingShip = true;
            this->setFieldState("left",x,y,previewStateNumber);
            //Wenn die Schiffsgröße 1 ist dann ist jetzt schon das Schiff platziert
            if(shipSize == 1)
            {
                isSelctingShip = false;
                //Sende das Schiff an den Server
                sendShip(x,y,shipSize,0,shipNumber);
                shipNumber++;
            }else
            {
                //Die Vorschaufelder müssen angezeigt werden
                togglePreview(x, y, shipSize, false,previewStateNumber);
            }
        }else
        {
            //Entweder auf der x oder y Achse die gleiche Koordinate
            bool valid = (shipTempX == x || shipTempY == y);
            //Entweder nach links oder Rechts auf dem in der Vorschau angezeigten Feld
            bool validOnXCoord = (x + shipSize-1 == shipTempX) || (x - shipSize+1 == shipTempX);
            //Entweder nach oben oder Unten auf dem in der Vorschau angezeigten Feld
            bool validOnYCoord = (y + shipSize-1 == shipTempY) || (y - shipSize+1 == shipTempY);

            //Wenn die Validierung erfolgreich war
            if(valid && (validOnXCoord || validOnYCoord))
            {
                //Die Vorschau wieder ausblenden
                togglePreview(shipTempX, shipTempY, shipSize, true,previewStateNumber);
                //Wenn das Schiff vertikal gesetzt werden soll
                if(shipTempX == x)
                {
                    //.. muss man in positiver y Richtung
                    if (shipTempY < y)
                    {
                        for (int i = 0; i < shipSize; i++)
                        {
                            //.. die Felder als Vorschau anzeigen
                            this->setFieldState("left",shipTempX,shipTempY+i,previewStateNumber);
                        }
                        //Das Schiff an den Server senden
                        sendShip(shipTempX,shipTempY,shipSize,1,shipNumber);
                    }
                    //.. analog in negativer y Richtung
                    else
                    {
                        for (int i = 0; i < shipSize; i++)
                        {
                            //.. die Felder als Vorschau anzeigen
                            this->setFieldState("left",shipTempX,shipTempY-i,previewStateNumber);
                        }
                        //Das Schiff an den Server senden
                        sendShip(x,y,shipSize,1,shipNumber);
                    }
                }
                //Wenn es hotizontal liegen soll
                if(shipTempY == y)
                {
                    //In positiver x Richtung
                    if (shipTempX < x)
                    {
                        for (int i = 0; i < shipSize; i++)
                        {
                            //Die Felder als Vorschau auffüllen
                            this->setFieldState("left",shipTempX+i,shipTempY,previewStateNumber);
                        }
                        //Das Schiff zum Server senden
                        sendShip(shipTempX,shipTempY,shipSize,0,shipNumber);
                    }
                    //In negativer x Richtung
                    else
                    {
                        for (int i = 0; i < shipSize; i++)
                        {
                            //Die Felder als Vorschau auffüllen
                            this->setFieldState("left",shipTempX-i,shipTempY,previewStateNumber);
                        }
                        //Das Schiff zu dem Server schicken
                        sendShip(x,y,shipSize,0,shipNumber);
                    }

                }
                //Das Schiff wurde platziert
                isSelctingShip = false;
                shipNumber++;
            }
        }
    }
}

//Zeigt beim setzten des Schiffes die Möglichen Felder an
void MainWindow::togglePreview(int x, int y, int size, bool remove,int pSNumber)
{
    //Ein blaues Schiff hat den Status 2
    int blueShip = 2;
    //Der Status der Vorschau beim setzten ist 6
    int state = 6;
    //Wenn die Vorschau gelöscht werden soll ist der Status der einer empty Grid-Cell
    if (remove)
    {
        state = 0;
    }
    //Die preview Koordinaten in Abhängigkeit der Achsen setzten
    int previewXPlus = x + size -1;
    int previewXMinus = x - size +1;
    int previewYPlus = y + size -1;
    int previewYMinus = y - size +1;

    //Nur wenn das positive x-Vorschaufeld innerhalb des Spielfeldes liegt
    if (previewXPlus >= 0 && previewXPlus < 10)
    {
        //..und das Feld kein SchiffsvorschauFeld und kein Schiff Feld ist
        if(gridLeft[previewXPlus][y]->state != pSNumber && gridLeft[previewXPlus][y]->state != blueShip)
        {
            //..setze den Preview state
            this->setFieldState("left",previewXPlus,y,state);
        }
    }
    //Nur wenn das negative x-Vorschaufeld innerhalb des Spielfeldes liegt
    if (previewXMinus >= 0 && previewXMinus < 10)
    {
        //..und das Feld kein SchiffsvorschauFeld und kein Schiff Feld ist
        if(gridLeft[previewXMinus][y]->state != pSNumber && gridLeft[previewXMinus][y]->state != blueShip)
        {
            //..setze den Preview state
            this->setFieldState("left",previewXMinus,y,state);
        }
    }
    //Nur wenn das positive y-Vorschaufeld innerhalb des Spielfeldes liegt
    if (previewYPlus >= 0 && previewYPlus < 10)
    {
        //..und das Feld kein SchiffsvorschauFeld und kein Schiff Feld ist
        if(gridLeft[x][previewYPlus]->state != pSNumber && gridLeft[x][previewYPlus]->state != blueShip)
        {
            //..setze den Preview state
            this->setFieldState("left",x,previewYPlus,state);
        }
    }
    //Nur wenn das negative y-Vorschaufeld innerhalb des Spielfeldes liegt
    if (previewYMinus >= 0 && previewYMinus < 10)
    {
        //..und das Feld kein SchiffsvorschauFeld und kein Schiff Feld ist
        if(gridLeft[x][previewYMinus]->state != pSNumber && gridLeft[x][previewYMinus]->state != blueShip)
        {
            //..setze den Preview state
            this->setFieldState("left",x,previewYMinus,state);
        }
    }
}

//Sendet das Schiff zu dem Server
void MainWindow::sendShip(int x, int y,int size, int orientation, int number)
{
    //Es wird eine Message gebaut die alle Informationen enthält
    QString message = QString("newShip-"+QString::number(x)
                             +"-"+QString::number(y)
                             +"-"+QString::number(size)
                             +"-"+QString::number(orientation)
                             +"-"+QString::number(number));
    client.sendTextMessage(message);
}

//Diese Funktion zeigt den eingegebenen String an
void MainWindow::showTextOnLabel(QString s)
{
    this->ui->manual->setText(s);
}

//Sende den Funk an den Server
void MainWindow::sendFunk(QString funk)
{
    client.sendTextMessage("funk-"+funk);
}

void MainWindow::sendName(QString name)
{
    client.sendTextMessage("name-"+name);
}

//Destruktor
MainWindow::~MainWindow()
{
    delete ui;
    client.close();
}
