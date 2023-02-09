#include "client.h"

Client::Client( QObject* parent ): QObject( parent ){
    connect( client, SIGNAL( connected() ), this, SLOT( startTransfer() ) );
    connect(client, SIGNAL(readyRead()), this, SLOT(startRead()));
}

Client::~Client(){
  client->close();
  client->deleteLater();
}

void Client::start( QString address, quint16 port )
{
    QHostAddress addr( address );
    client->connectToHost( addr, port );
}

void Client::startTransfer(){
    QString str("GET \r\n \r\n");
    QByteArray ba = str.toLocal8Bit();
    const char *c_str = ba.data();

    client->write( c_str, str.length()+1 );
}

void Client::shoot(int x,int y)
{
    client->write(QString::number(y).toUtf8());
    client->write(QString::number(x).toUtf8());
}
void Client::startRead(){

  char buffer[1024] = {0};
  QTcpSocket *sender = (QTcpSocket* ) QObject::sender();
  sender->read(buffer, sender->bytesAvailable());

  //Empfangenen String zum Debuggen ausgeben
  qDebug() << buffer;

  //Empfangenen String auswerten
  //ToDo
}
