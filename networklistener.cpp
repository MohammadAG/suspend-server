#include "networklistener.h"
#include <QProcess>
#include <QDateTime>
#include <QCoreApplication>
#ifdef Q_WS_X11
#include <QDBusInterface>
#include <QDBusConnection>
#include <QLatin1String>
#endif

NetworkListener::NetworkListener(QObject *parent) :
    QObject(parent), mPort(9095)
{
    mServer = new QTcpServer(this);
    connect(mServer, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

NetworkListener::~NetworkListener()
{
    mServer->close();
}

void NetworkListener::startListening() {
    if (!mServer->listen(QHostAddress::Any, mPort)) {
        qDebug() << "Failed to listen at port " + QString::number(mPort);
        qDebug() << "Error: " << mServer->serverError();
        mServer->close();
        qApp->exit(EXIT_FAILURE);
    } else {
        qDebug() << "Started listening on port " + QString::number(mPort);
    }
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
          "<h1>Suspending HTPC, wake it up by clicking <a href=http://mohammadag.xceleo.org/wol.php>here</a></h1>\n"
       << "\n";
    socket->close();
#ifdef Q_WS_X11
    QDBusInterface("org.freedesktop.UPower", "/org/freedesktop/UPower",
                   "org.freedesktop.UPower",
                   QDBusConnection::systemBus()).call(QLatin1String("Suspend"));
#else
    qDebug() << "Suspending PC...";
#endif
}
