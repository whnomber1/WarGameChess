//
// Created by 1 on 2026/2/4.
//

#ifndef WARGAMECHESS_DEDUCEWORKER_H
#define WARGAMECHESS_DEDUCEWORKER_H
#include <QThread>

class DeduceWorker : public QThread
{
    Q_OBJECT
protected:
    void run();
signals:
      void sendPublicMessage(const QString &s);
};


#endif //WARGAMECHESS_DEDUCEWORKER_H