//
// Created by 1 on 2026/2/4.
//

#ifndef WARGAMECHESS_DATADISTRIBUTIONER_H
#define WARGAMECHESS_DATADISTRIBUTIONER_H
#include <QObject>

class IGameEngine;
class DataDistributioner : public QObject
{
    Q_OBJECT
public:
    static DataDistributioner *GetInstance();
    void SetSimulationEngine(IGameEngine *eng);
public slots:
    void sendPublicMessageSlt(const QString &s);
    void receiveDtMessageSlt(QString str);
signals:
      void sendPublicMessageSig(const QString &s);
private:
    IGameEngine *m_engine=nullptr;
};


#endif //WARGAMECHESS_DATADISTRIBUTIONER_H