#include <DataDistributionClient.h>
#include <QApplication>
#include <iostream>
#include <qtextstream.h>
#include <windows.h>

#include "DataDistributioner.h"
#include "DeduceWorker.h"

// 自定义消息处理函数
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QTextStream out(stdout);  // 重定向到 stdout（标准输出）
    // QTextStream err(stderr);  // 如果想保持默认，用这个

    // 可选：根据类型加前缀（方便区分）
    switch (type) {
    case QtDebugMsg:
        out << "Debug: ";
        break;
    case QtInfoMsg:
        out << "Info: ";
        break;
    case QtWarningMsg:
        out << "Warning: ";
        break;
    case QtCriticalMsg:
        out << "Critical: ";
        break;
    case QtFatalMsg:
        out << "Fatal: ";
        break;
    }

    out << msg << Qt::endl;  // 输出消息
    out.flush();             // 立即刷新（调试时很重要）

    // 如果是 Fatal，直接退出（默认行为）
    if (type == QtFatalMsg) {
        abort();
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);
    QApplication app(argc, argv);
    DataDistributioner::GetInstance();
    DeduceWorker *workerThread = new DeduceWorker;
    workerThread->start();
    return app.exec();
}
