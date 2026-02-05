//
// Created by 1 on 2026/2/4.
//

#include "DataDistributioner.h"
#include <QDebug>

#include "CJsonVariant.h"
#include "DataDistributionClient.h"
#include "DeduceEvent.h"
#include "IGameEngine.h"

static DataDistributioner *S_DataDistInstance = nullptr;

DataDistributioner *DataDistributioner::GetInstance()
{
    if(!S_DataDistInstance)
    {
        DataDistributionClient::GetInstance();
        S_DataDistInstance = new DataDistributioner;
        connect(S_DataDistInstance, SIGNAL(sendPublicMessageSig(const QString &)),
               S_DataDistInstance,  SLOT(sendPublicMessageSlt(const QString &)), Qt::BlockingQueuedConnection);
        connect(DataDistributionClient::GetInstance(), SIGNAL(ReceiveMessageSig(QString)),
           S_DataDistInstance,  SLOT(receiveDtMessageSlt(QString)));

    }
    return S_DataDistInstance;
}

void DataDistributioner::SetSimulationEngine(IGameEngine* eng)
{
    m_engine=eng;
}

void DataDistributioner::sendPublicMessageSlt(const QString& str)
{
    // qDebug() << str;
    DataDistributionClient::GetInstance()->SendClientMessage(str);
}

void DataDistributioner::receiveDtMessageSlt(QString str)
{
    CJsonVariant var(str);
    if(var.isValid("eventType"))
    {
        DeduceEventType evtp = static_cast<DeduceEventType>(var.getValue("eventType").toInt());
        switch (evtp)
        {
            case EV_UpdateSpeed:
            {
                if (m_engine)
                {
                    int val = var.getValue("speedVal").toInt();
                    m_engine->setRunFPS(val);
                }
                break;
            }
            default: break;
        }
    }

}
