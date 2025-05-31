#include "DeduceMain.h"
#include <QDebug>
#include <QDateTime>
#include <QElapsedTimer>

// 伪结构体，真实情况用你收到的状态结构体
struct WorldState {
    int tick;
    // ... 其他仿真状态字段
};

// 假设你这里从DDS收取服务器最新状态
WorldState fetch_server_state() {
    // TODO: 调用DDS接口，同步等待或轮询最新状态
    // 伪代码（你自己塞真正的DDS处理代码）
    WorldState ws;
    ws.tick = 0; //替换成真正的
    // ...
    return ws;
}


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

// 本地仿真推进逻辑
void DeduceMain::step_simulation()
{
    if(m_preTickNum > 0)
    {
        qint64 curmsec = QDateTime::currentMSecsSinceEpoch();
        // qDebug() << "deta: " << curmsec - m_preTickNum;
        qDebug() << "fps: " << (1000 / (curmsec - m_preTickNum));
    }
    m_preTickNum = QDateTime::currentMSecsSinceEpoch();
    // msleep(50);
}

void DeduceMain::run()
{
    using namespace std::chrono;

    const int fpsnum = 60; // 每步50ms（=20帧/秒），你可自定义
    const qint64 targetFrameTime = 1000000 / fpsnum;
    QElapsedTimer timer;
    timer.start();
    while (m_bNeedQuit)
    {
        // if(m_preTickNum > 0)
        // {
        //     qint64 curmsec = QDateTime::currentMSecsSinceEpoch();
        //     // qDebug() << "deta: " << curmsec - m_preTickNum;
        //     qDebug() << "fps: " << (1000 / (curmsec - m_preTickNum));
        // }
        // m_preTickNum = QDateTime::currentMSecsSinceEpoch();
        // qDebug() << QDateTime::currentMSecsSinceEpoch();
        qDebug() << timer.nsecsElapsed()/1000000;
        timer.restart();
#if 0
        m_Elapsedtimer.restart();
        // 2. 本地推进仿真
        step_simulation();

        qint64 nes = m_Elapsedtimer.nsecsElapsed() / 1000;// 转换为微秒
        // qDebug() << nes;
        // qint64 sleepTime = static_cast<qint64>(1.0 / fpsnum * 1e6) - nes;
        // if (sleepTime > 0) {
        //     QThread::usleep(static_cast<quint64>(sleepTime));
        // }
        // QThread::usleep(static_cast<quint64>(16000));
        sleep_utils::microSleep(16000);
#elif 1
        qint64 start = m_Elapsedtimer.nsecsElapsed();

        // 模拟渲染或任务
        // qDebug() << "Tick";

        qint64 elapsed = (m_Elapsedtimer.nsecsElapsed() - start) / 1000; // 微秒
        qint64 sleepTime = targetFrameTime - elapsed;
        if (sleepTime > 0) {
            // QThread::usleep(12000);
            // qDebug() << sleepTime;
            sleep_utils::microSleep(13000);
        }

        // qint64 frameTotalTime = (m_Elapsedtimer.nsecsElapsed() - start);
        // double fps = 1000000000.0 / frameTotalTime;
        // qDebug() << "FPS:" << fps;
#endif
    }
}

DeduceMain::DeduceMain(QObject *parent)
    : QThread{parent}
{
    m_Elapsedtimer.start();
}

void DeduceMain::setDeduceNeedQuit()
{
    m_bNeedQuit = false;
}
