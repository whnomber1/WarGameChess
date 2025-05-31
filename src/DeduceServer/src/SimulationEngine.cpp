#include <SimulationEngine.h>
#include <iostream>
#include <chrono>
#include <thread>

SimulationEngine::SimulationEngine()
    : IGameEngine()
{
}

SimulationEngine::~SimulationEngine()
{

}

void SimulationEngine::engRun()
{
    runWithBusyWait();
}

void SimulationEngine::runWithSleepUntil()
{
    auto nextFrameTime = std::chrono::steady_clock::now();
    auto startTime = nextFrameTime;

    while (m_IsRunning)
    {
        updateSimTime();

        // 渲染
        render();

        frameCount++;
        if (canShowLogInfo())
        {
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            startTime = endTime;
            std::cout << "实际运行时间: " << duration.count() << "ms, "
                      << "平均FPS: " << (static_cast<double>(m_targetFPS * m_showcont) * 1000.0 / duration.count()) << std::endl;
        }

        // 计算下一帧的时间点
        nextFrameTime += std::chrono::microseconds(static_cast<long>(m_targetFrameTime * 1000000));

        // 精确等待到下一帧时间
        std::this_thread::sleep_until(nextFrameTime);
    }
}


void SimulationEngine::runWithDynamicTiming()
{
    std::cout << "开始动态调整循环 (60FPS)\n";

    frameCount = 0;
    auto startTime = std::chrono::high_resolution_clock::now();
    auto lastFrameTime = startTime;

    while (m_IsRunning)
    {
        auto currentTime = std::chrono::high_resolution_clock::now();

        // 计算上一帧的实际耗时
        auto deltaTime = std::chrono::duration<double>(currentTime - lastFrameTime).count();

        // 游戏逻辑更新（可以使用deltaTime进行时间相关的计算）
        updateSimTime(deltaTime);

        // 渲染
        render();
        lastFrameTime = currentTime;
        frameCount++;

        if (canShowLogInfo())
        {
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            startTime = endTime;
            std::cerr << "实际运行时间: " << duration.count() << "ms, "
                      << "平均FPS: " << (static_cast<double>(m_targetFPS * m_showcont) * 1000.0 / duration.count()) << std::endl;

        }


        // 计算需要等待的时间
        auto frameEndTime = std::chrono::high_resolution_clock::now();
        auto frameProcessTime = std::chrono::duration<double>(frameEndTime - currentTime).count();

        if (canShowLogInfo(1))
        {
            std::cout << "m_targetFrameTime: " << m_targetFrameTime
                      << "  detatime: " << (m_targetFrameTime - frameProcessTime)
                      << std::endl;
        }
        if (m_targetFrameTime > frameProcessTime)
        {
            std::this_thread::sleep_for(std::chrono::duration<double>(m_targetFrameTime - frameProcessTime));

        }
    }
}

void SimulationEngine::runWithBusyWait()
{
    std::cout << "开始忙等待循环 (60FPS) - 注意CPU占用较高\n";

    frameCount = 0;
    auto startTime = std::chrono::high_resolution_clock::now();
    auto nextFrameTime = startTime;

    while (m_IsRunning)
    { // 运行5秒，因为CPU占用高
        auto currentTime = std::chrono::high_resolution_clock::now();

        if (currentTime >= nextFrameTime)
        {
            // 游戏逻辑更新
            updateSimTime(m_curStepLen);

            // 渲染
            render();

            frameCount++;
            if (canShowLogInfo())
            {
                auto endTime = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
                startTime = endTime;
                std::cerr << "实际运行时间: " << duration.count() << "ms, "
                          << "平均FPS: " << ((m_targetFPS * m_showcont) * 1000.0 / duration.count()) << std::endl;
            }

            // 设置下一帧时间
            nextFrameTime += std::chrono::microseconds(static_cast<long>(m_targetFrameTime * 1000000));
        }

        // 忙等待 - 不推荐在实际项目中使用，除非需要极高精度
        // std::this_thread::yield(); // 可以添加这行来减少CPU占用

        auto frameEndTime = std::chrono::high_resolution_clock::now();
        auto frameProcessTime = std::chrono::duration<double>(frameEndTime - currentTime).count();
        if(m_targetFrameTime > frameProcessTime)
        {
            std::this_thread::sleep_for(std::chrono::duration<double>(m_targetFrameTime*0.05));
        }
    }
}

bool SimulationEngine::canShowLogInfo(int num)
{
    if(!num) num = m_showcont;
    return m_showFpsInfo && (frameCount % (m_targetFPS * num) == 0);
}

void SimulationEngine::updateLongInfo(std::string ilog)
{
    if (canShowLogInfo(1))
    {
        std::cerr << ilog << std::endl;
    }

}

// 游戏逻辑更新
void SimulationEngine::updateSimTime(double stepLen)
{
    // 这里放置游戏逻辑更新代码
    // 例如：物理计算、AI更新、输入处理等

    // 模拟一些处理时间
    // std::this_thread::sleep_for(std::chrono::microseconds(1000)); // 1ms的模拟处理时间
    updateLongInfo(std::string("stepLen: ") + std::to_string(stepLen));
}

void SimulationEngine::render()
{
    // 这里放置渲染代码
    // 例如：清屏、绘制对象、交换缓冲区等

    // 每100帧输出一次信息
    if (frameCount % 100 == 0) {
        // std::cout << "Frame: " << frameCount << std::endl;
    }
}

void SimulationEngine::stop()
{
    m_IsRunning = false;
}

void SimulationEngine::setRunFPS(int fnum)
{
    if(fnum > m_maxFpsNum)
    {
        m_curStepLen = static_cast<double>(fnum) / static_cast<double>(m_maxFpsNum);
        fnum = m_maxFpsNum;
    }
    m_targetFPS = fnum;
    m_targetFrameTime = 1.0 / m_targetFPS;
}


