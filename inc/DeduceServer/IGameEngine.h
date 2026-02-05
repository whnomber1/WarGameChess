#ifndef IGAMEENGINE_H
#define IGAMEENGINE_H

class IGameEngine {
public:

    IGameEngine()
    {
        m_targetFrameTime = 1.0 / m_targetFPS; // 16.67ms
    }
    //引擎需要实现的循环
    virtual void engRun()=0;

    // 游戏逻辑更新
    virtual void updateSimTime(double stepLen){};

    // 渲染函数
    virtual void render(){}

    virtual void start()
    {
        m_IsRunning = true;
    }

    //停止运行
    virtual void stop()
    {
        m_IsRunning = false;
    }

    virtual void setRunFPS(int fnum)
    {
        m_targetFPS = fnum;
        m_targetFrameTime = 1.0 / m_targetFPS;
    }

    virtual void setEnableFpsInfoLog(bool bLog)
    {
        m_showFpsInfo = bLog;
    }

    virtual void setFpsInfoLogStatu(int ct)
    {
        m_showcont=ct;
    }
protected:
    //fps
    int m_targetFPS = 4;
    int m_maxFpsNum = 1000;
    //每帧的运行时间
    double m_targetFrameTime;
    bool m_IsRunning = true;

    //日志输出
    bool m_showFpsInfo=true;
    int m_showcont = 3;
    int frameCount = 0;
};

#endif //IGAMEENGINE_H
