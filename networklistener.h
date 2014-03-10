#ifndef NETWORKLISTENER_H
#define NETWORKLISTENER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class NetworkListener : public QObject
{
    Q_OBJECT
public:
    explicit NetworkListener(QObject *parent = 0);
    ~NetworkListener();

private:
    QTcpServer *mServer;
    qint32 mPort;

public:
    void setPort(int port, bool shouldStartListening = false);
    void startListening();
    void stopListening();

signals:

public slots:

private slots:
    void onNewConnection();
};

#endif // NETWORKLISTENER_H
