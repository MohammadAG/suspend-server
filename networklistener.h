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
    qint32 mSuspendDelay;

public:
    void setPort(int port, bool shouldStartListening = false);
    bool startListening();
    void stopListening();
    void setDelay(int delay);

signals:

public slots:

private slots:
    void onNewConnection();
    void suspendPc();
};

#endif // NETWORKLISTENER_H
