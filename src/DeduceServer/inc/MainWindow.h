#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
class DeduceMain;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void onTimerTimoutSlt();
private:
    Ui::MainWindow *ui;
    DeduceMain *m_workerThread=nullptr;
    QTimer *m_timer;
    qint64 m_EStimer;
};
#endif // MAINWINDOW_H
