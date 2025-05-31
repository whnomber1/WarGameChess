#ifndef DEDUCEMAIN_H
#define DEDUCEMAIN_H

#include <QThread>
#include <QElapsedTimer>

class DeduceMain : public QThread
{
    Q_OBJECT
    void run() override;
public:
    explicit DeduceMain(QObject *parent = nullptr);
    void setDeduceNeedQuit();
signals:
private:
    void step_simulation();
private:
    bool m_bNeedQuit=true;
    QElapsedTimer m_Elapsedtimer;
    qint64 m_preTickNum=0;
};

#endif // DEDUCEMAIN_H
