#include "networklistener.h"
#include <QProcess>
#include <QDateTime>
#include <QCoreApplication>
#include <QTimer>
#ifdef Q_WS_X11
#include <QDBusInterface>
#include <QDBusConnection>
#include <QLatin1String>
#endif

NetworkListener::NetworkListener(QObject *parent) :
    QObject(parent), mPort(9095), mSuspendDelay(1000)
{
    mServer = new QTcpServer(this);
    connect(mServer, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

NetworkListener::~NetworkListener()
{
    mServer->close();
}

bool NetworkListener::startListening() {
    bool success = mServer->listen(QHostAddress::Any, mPort);
    if (!success) {
        qDebug() << "Failed to listen at port " + QString::number(mPort);
        qDebug() << "Error: " << mServer->serverError();
        mServer->close();
        qApp->exit(EXIT_FAILURE);
    } else {
        qDebug() << "Started listening on port " + QString::number(mPort);
    }

    return success;
}

void NetworkListener::stopListening()
{
    if (mServer->isListening())
        mServer->close();
}

void NetworkListener::setPort(int port, bool shouldStartListening)
{
    mPort = port;

    if (shouldStartListening) {
        stopListening();
        startListening();
    }
}

void NetworkListener::onNewConnection()
{
    QTcpSocket *socket = mServer->nextPendingConnection();
    QTextStream os(socket);
    os.setAutoDetectUnicode(true);
    os << "HTTP/1.0 200 Ok\r\n"
          "Content-Type: text/html; charset=\"utf-8\"\r\n"
          "\r\n"
          "<h1>Suspending HTPC..."
       << "\n";
    socket->flush();
    socket->close();
    /* Delay suspending so there's time for the HTML page to show */
    QTimer::singleShot(mSuspendDelay, this, SLOT(suspendPc()));
}

void NetworkListener::setDelay(int delay)
{
    mSuspendDelay = delay;
}

void NetworkListener::suspendPc()
{
#ifdef Q_WS_X11

    QDBusInterface("org.freedesktop.UPower", "/org/freedesktop/UPower",
                   "org.freedesktop.UPower",
                   QDBusConnection::systemBus()).call(QLatin1String("Suspend"));
#else
    qDebug() << "Suspending PC...";
#endif
}
