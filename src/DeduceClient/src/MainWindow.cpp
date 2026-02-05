#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>

#include "CJsonVariant.h"
#include "DataDistributionClient.h"
#include "DeduceEvent.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(DataDistributionClient::GetInstance(), SIGNAL(ReceiveMessageSig(QString)),
       this,  SLOT(receiveMessageSlt(QString)));
    connect(ui->horizontalSlider_spd, SIGNAL(sliderReleased()),
       this,  SLOT(spdValueChangedSlt()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
}

void MainWindow::receiveMessageSlt(QString str)
{
    CJsonVariant var(str);
    if(var.isValid("simTime"))
    {
        // qDebug() << QDateTime::fromSecsSinceEpoch(var.getValue("simTime").toInt()).toString("yyyy.MM.dd hh:mm:ss");
        ui->label_tm->setText(QString::number(var.getValue("simTime").toInt()));
    }

}

void MainWindow::spdValueChangedSlt()
{
    int value = ui->horizontalSlider_spd->value();
    ui->label_spdval->setText(QString::number(value));
    CJsonVariant var;
    var.setValue("eventType", EV_UpdateSpeed);
    var.setValue("speedVal", value);
    DataDistributionClient::GetInstance()->SendClientMessage(var.GetJsonString());
}

