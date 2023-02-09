#ifndef CLIENT_H
#define CLIENT_H


#include <QString>
#include <QTcpSocket>
#include <QHostAddress>

class Client: public QObject
{
    Q_OBJECT
    public:
      Client(QObject* parent = 0);
      ~Client();
      void start( QString address, quint16 port );
      void shoot(int x,int y);
    public slots:
      void startTransfer();
      void startRead();
    private:
      QTcpSocket *client = new QTcpSocket;
};

#endif // CLIENT_H
