#ifndef SIMULATIONENGINE_H
#define SIMULATIONENGINE_H

#include <IGameEngine.h>
#include <string>


class SimulationEngine: public IGameEngine
{
public:
    SimulationEngine();
    ~SimulationEngine();
    // 方法1: 使用 chrono 和 sleep_until 的精确计时
    virtual void engRun();

    // 游戏逻辑更新
    virtual void updateSimTime(double stepLen=1.0);

    // 渲染函数
    virtual void render();

    virtual void stop();

    virtual void setRunFPS(int fnum);
private:
    void runWithSleepUntil();
    // 方法2: 使用高精度计时和动态调整
    void runWithDynamicTiming();

    // 方法3: 使用忙等待的高精度循环（CPU密集型）
    void runWithBusyWait();

    bool canShowLogInfo(int num=0);

    void updateLongInfo(std::string ilog);

private:
    double m_curStepLen=1.0;
};

#endif //SIMULATIONENGINE_H
