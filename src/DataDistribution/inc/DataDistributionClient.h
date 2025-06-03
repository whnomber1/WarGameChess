#ifndef DATADISTRIBUTIONCLIENT_H
#define DATADISTRIBUTIONCLIENT_H

#include <DataDistribution_export.h>
#include <QObject>

namespace QMQTT {
class Client;
}

class DATADISTRIBUTION_API DataDistributionClient : public QObject
{
    Q_OBJECT
public:
    static DataDistributionClient *GetInstance();

    void SendClientMessage(QString msg, quint16 mid=0);
public slots:
    void clientHasConnectedSlt();
    void clientDisConnectedSlt();

private:
    DataDistributionClient();
    void initDataDistribution();
    bool isDataDistClientConnected();
private:
    QMQTT::Client *m_mqttclient=nullptr;
};

#endif // DATADISTRIBUTIONCLIENT_H
