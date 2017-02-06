#include "qtsinglecoreapplication.h"
#include "qtlocalpeer.h"


QtSingleCoreApplication::QtSingleCoreApplication(int &argc, char **argv)
    : QCoreApplication(argc, argv)
{
    peer = new QtLocalPeer(this);
    connect(peer, SIGNAL(messageReceived(const QString&)), SIGNAL(messageReceived(const QString&)));
}

QtSingleCoreApplication::QtSingleCoreApplication(const QString &appId, int &argc, char **argv)
    : QCoreApplication(argc, argv)
{
    peer = new QtLocalPeer(this, appId);
    connect(peer, SIGNAL(messageReceived(const QString&)), SIGNAL(messageReceived(const QString&)));
}

bool QtSingleCoreApplication::isRunning()
{
    return peer->isClient();
}

bool QtSingleCoreApplication::sendMessage(const QString &message, int timeout)
{
    return peer->sendMessage(message, timeout);
}

QString QtSingleCoreApplication::id() const
{
    return peer->applicationId();
}
