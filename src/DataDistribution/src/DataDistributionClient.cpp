#include "DataDistributionClient.h"

#include <qmqtt.h>
// #include <QtQmqtt/qmqtt_message.h>
#define EXAMPLE_TOPIC "qmqtt/exampletopic"
#include <CJsonVariant.h>
#include <QDateTime>
#include <QRandomGenerator>

static DataDistributionClient *S_DataDistInstance = nullptr;

DataDistributionClient *DataDistributionClient::GetInstance()
{
    if(!S_DataDistInstance)
    {
        S_DataDistInstance = new DataDistributionClient;
    }
    return S_DataDistInstance;
}

void DataDistributionClient::SendClientMessage(QString msg, quint16 mid)
{
    if(isDataDistClientConnected())
    {
        CJsonVariant var(msg);
        var.setValue("CurClientID", m_clientId);
        QMQTT::Message message(mid, EXAMPLE_TOPIC,
                               var.GetJsonString().toUtf8());
        m_mqttclient->publish(message);
    }
}

void DataDistributionClient::clientHasConnectedSlt()
{
    qDebug() << "Connected to broker";

    // 订阅主题
    m_mqttclient->subscribe(EXAMPLE_TOPIC);
    connect(m_mqttclient, &QMQTT::Client::received, [this](const QMQTT::Message& msg){
        // qDebug() << msg.id() << "Message topic: " << msg.topic() << "  Message received: " << msg.payload();
        ReceiveMessageSig(QString::fromUtf8(msg.payload()));
        // CJsonVariant var(msg.payload());
        // if(var.isValid("simTime"))
        // {
        //     qDebug() << QDateTime::fromSecsSinceEpoch(var.getValue("simTime").toInt()).toString("yyyy.MM.dd hh:mm:ss");
        // }
    });
}

void DataDistributionClient::clientDisConnectedSlt()
{
    qDebug() << "Disconnected from broker";
}

DataDistributionClient::DataDistributionClient()
    : QObject(nullptr)
{
    initDataDistribution();
}

void DataDistributionClient::initDataDistribution()
{
    if(!m_mqttclient)
    {
        m_mqttclient = new QMQTT::Client(QHostAddress("127.0.0.1"), 1883);

        // m_mqttclient->setHostName("127.0.0.1");
        // m_mqttclient->setPort(1883);
        m_clientId = "my_client_"+QString::number(QRandomGenerator::global()->generate());
        m_mqttclient->setClientId(m_clientId);
        m_mqttclient->setCleanSession(true);
        connect(m_mqttclient, SIGNAL(connected()), this,  SLOT(clientHasConnectedSlt()));
        connect(m_mqttclient, SIGNAL(disconnected()), this,  SLOT(clientHasConnectedSlt()));
        connect(m_mqttclient, &QMQTT::Client::error, [](const QMQTT::ClientError error){
            qDebug() << error;
        });

        // // 建立连接
        m_mqttclient->connectToHost();
    }
}

bool DataDistributionClient::isDataDistClientConnected()
{
    return m_mqttclient && m_mqttclient->isConnectedToHost();
}

