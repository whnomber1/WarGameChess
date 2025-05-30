#include "MainWindow.h"

#include <QApplication>
#include <QElapsedTimer>
#include <QDebug>
#include <QThread>
#include <thread>


#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#else
#include <time.h>
#include <unistd.h>
#endif

namespace sleep_utils {

// 微秒级 sleep 函数（跨平台）
inline void microSleep(unsigned int microseconds) {
#ifdef _WIN32
    // 使用高精度 WaitableTimer（兼容 Win2000+）
    HANDLE timer = CreateWaitableTimer(NULL, TRUE, NULL);
    if (!timer) return;

    LARGE_INTEGER li;
    li.QuadPart = -(static_cast<LONGLONG>(microseconds) * 10); // 100ns 单位，负数表示相对时间

    if (!SetWaitableTimer(timer, &li, 0, NULL, NULL, 0)) {
        CloseHandle(timer);
        return;
    }

    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
#else
    // POSIX 系统使用 nanosleep
    struct timespec ts;
    ts.tv_sec = microseconds / 1000000;
    ts.tv_nsec = (microseconds % 1000000) * 1000;
    nanosleep(&ts, nullptr);
#endif
}

}

#if 0
int main(int argc, char *argv[])
{
    QElapsedTimer timer;
    timer.start();
    while (1)
    {
        qDebug() << "time:" << timer.nsecsElapsed()/1000000;
        timer.restart();
        sleep_utils::microSleep(16000);
    }


    // QApplication a(argc, argv);
    // MainWindow w;
    // w.show();
    // return a.exec();
}
#elif 0
int main() {
    QElapsedTimer timer;

    const qint64 targetFrameMicroSec = 16666; // 16.666 ms
    const qint64 sleepThreshold = 1000;       // 提前1ms进入忙等待

    while (true) {
        timer.restart();

        // --- 模拟帧逻辑 ---
        qDebug() << "tick";

        // --- 睡到接近目标帧间隔 ---
        while (true) {
            qint64 elapsed = timer.nsecsElapsed() / 1000; // 微秒
            qint64 remaining = targetFrameMicroSec - elapsed;

            if (remaining > sleepThreshold) {
                sleep_utils::microSleep(remaining - sleepThreshold);
            } else if (remaining > 0) {
                // 忙等待最后1ms
                continue;
            } else {
                break;
            }
        }

        // 输出实际间隔
        qDebug() << "frame duration (ms):" << (timer.nsecsElapsed() / 1000000.0);
    }

    return 0;
}
#elif 0
#include <windows.h>
void enableHighPrecisionTimer() {
    timeBeginPeriod(1); // 设置系统全局计时器分辨率为 1ms
}

void disableHighPrecisionTimer() {
    timeEndPeriod(1);
}
int main() {
    enableHighPrecisionTimer();

    QElapsedTimer timer;
    const qint64 targetFrameMicroSec = 16666;
    const qint64 sleepThreshold = 1000;

    while (true) {
        timer.restart();
        qDebug() << "tick";

        while (true) {
            qint64 elapsed = timer.nsecsElapsed() / 1000;
            qint64 remaining = targetFrameMicroSec - elapsed;

            if (remaining > sleepThreshold) {
                sleep_utils::microSleep(remaining - sleepThreshold);
            } else if (remaining > 0) {
                continue; // busy-wait
            } else {
                break;
            }
        }

        qDebug() << "frame duration (ms):" << (timer.nsecsElapsed() / 1000000.0);
    }

    disableHighPrecisionTimer();
    return 0;
}
#elif 1
#include <windows.h>
void enableHighPrecisionTimer() {

}

void disableHighPrecisionTimer() {

}
int main(int argc, char *argv[])
{
    // timeBeginPeriod(1); // 设置系统全局计时器分辨率为 1ms

    // QElapsedTimer timer;
    // timer.start();
    // const std::chrono::microseconds frameDuration(16000);
    // while (1)
    // {
    //     qDebug() << "time:" << timer.nsecsElapsed()/1000000;
    //     timer.restart();
    //     // QThread::usleep(16000);
    //     // auto frameEnd = std::chrono::steady_clock::now();
    //     // auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);
    //     std::this_thread::sleep_for(frameDuration);
    // }


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
    timeEndPeriod(1);
    return 0;
}
#endif
