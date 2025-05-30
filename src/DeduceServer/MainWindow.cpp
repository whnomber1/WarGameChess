#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <DeduceMain.h>
#include <QTimer>
#include <QDebug>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_workerThread = new DeduceMain(this);
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimerTimoutSlt()));
    // m_workerThread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
    if(m_workerThread)
    {
        m_workerThread->setDeduceNeedQuit();
        m_workerThread->quit();
        m_workerThread->wait();
        delete m_workerThread;
    }
}

void MainWindow::on_pushButton_clicked()
{
    m_timer->start(16);
}

void MainWindow::onTimerTimoutSlt()
{
    qint64 curmsec = QDateTime::currentMSecsSinceEpoch() - m_EStimer;
    double fpsnum = 1000.0/curmsec;
    qDebug() << fpsnum << curmsec;
    QThread::sleep(1);
    // int showNum=100000000;
    // while(showNum)
    // {
    //     showNum--;
    // }
    // ui->label_fps->setText(QString::number(fpsnum, 'f', 2));
    // if(showNum++ % 20 == 0)
    // {
    //     ui->label_fps->setText("fps: " + QString::number(fpsnum, 'f', 2));
    // }
    m_EStimer = QDateTime::currentMSecsSinceEpoch();
    qDebug() << "aaa";
}

