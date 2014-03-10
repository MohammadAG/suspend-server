#include <QCoreApplication>
#include <QStringList>

#include "networklistener.h"

void print_version()
{
    QTextStream stream(stdout, QIODevice::WriteOnly);
    stream << QCoreApplication::applicationName() + " " + QCoreApplication::applicationVersion() << endl;
    stream << "Copyright (C) 2014 Mohammad Abu-Garbeyyeh" << endl;
}

void print_usage()
{
    QTextStream stream(stdout, QIODevice::WriteOnly);

    stream << QCoreApplication::applicationName() + " " + QCoreApplication::applicationVersion() << endl;
    stream << "Copyright (C) 2014 Mohammad Abu-Garbeyyeh" << endl;
    stream << "Usage: " + QCoreApplication::applicationName() + " [option] [argument]" << endl;
    stream << "\n";
    stream << "-h   --help       Show this help screen" << endl;
    stream << "-v   --version    Print application name and version" << endl;
    stream << "-p   --port       Change port used by server (default: 9095)" << endl;
    stream << "-d   --delay      Delay in ms after which this machine should be suspended" << endl;
    stream << "\n";
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setApplicationName("suspend-server");
    a.setApplicationVersion("0.1");
    QStringList list = QCoreApplication::arguments();

    NetworkListener listener;
    for (int i = 0; i < list.size(); i++) {
        QString arg = list.at(i);
        if (arg == "-p" || arg == "--port") {
            if (list.size() >= i+2) {
                QString portNum = list.at(i+1);
                listener.setPort(portNum.toInt());
            } else {
                qDebug() << "Error: You must supply a port number";
                return EXIT_FAILURE;
            }
        } else if (arg == "-d" || arg == "--delay") {
            if (list.size() >= i+2) {
                QString delayMs = list.at(i+1);
                listener.setDelay(delayMs.toInt());
            } else {
                qDebug() << "Error: You must a delay in ms";
                return EXIT_FAILURE;
            }
        } else if (arg == "-h" || arg == "--help") {
            print_usage();
            return EXIT_SUCCESS;
        } else if (arg == "-v" || arg == "--version") {
            print_version();
            return EXIT_SUCCESS;
        }
    }

    print_version();
    QTextStream stream(stdout, QIODevice::WriteOnly);
    stream << "\n";

    if (listener.startListening())
        return a.exec();
    else
        return EXIT_FAILURE;
}
