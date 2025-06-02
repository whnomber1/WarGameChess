#ifndef SIMULATIONADVANCEDENGINE_H
#define SIMULATIONADVANCEDENGINE_H

#include <IGameEngine.h>

// 更高级的游戏循环实现：固定时间步长 + 可变渲染
class SimulationAdvancedEngine : public IGameEngine
{

public:
    SimulationAdvancedEngine();
    ~SimulationAdvancedEngine();
    virtual void engRun();
private:
    void runFixedTimestep();
    void updatePhysics(double dt);

    void render(double interpolation);

private:
    int m_targetFPS=60;
    double m_fixedTimestep; // 固定的物理更新步长
    double m_maxFrameFime = 0.25; // 最大帧时间，避免死亡螺旋

    bool m_IsRunning = true;
    double m_accumulator = 0.0;
};

#endif //SIMULATIONADVANCEDENGINE_H
