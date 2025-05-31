#include <SimulationAdvancedEngine.h>
#include <iostream>
#include <chrono>
#include <thread>

SimulationAdvancedEngine::SimulationAdvancedEngine()
{
    m_fixedTimestep = 1 / m_targetFPS;
}

SimulationAdvancedEngine::~SimulationAdvancedEngine()
{
}

void SimulationAdvancedEngine::engRun()
{

}

void SimulationAdvancedEngine::runFixedTimestep() {
    std::cout << "开始固定时间步长循环\n";

    auto currentTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;

    while (m_IsRunning && frameCount < 600) {
        auto newTime = std::chrono::high_resolution_clock::now();
        auto frameTime = std::chrono::duration<double>(newTime - currentTime).count();

        // 防止死亡螺旋
        if (frameTime > m_fixedTimestep) {
            frameTime = m_fixedTimestep;
        }

        currentTime = newTime;
        m_accumulator += frameTime;

        // 固定时间步长的物理更新
        while (m_accumulator >= m_fixedTimestep) {
            updatePhysics(m_fixedTimestep);
            m_accumulator -= m_fixedTimestep;
        }

        // 插值因子，用于平滑渲染
        double interpolation = m_accumulator / m_fixedTimestep;

        // 渲染（可以使用插值进行平滑）
        render(interpolation);

        frameCount++;

        // 控制帧率
        std::this_thread::sleep_for(std::chrono::microseconds(8000)); // 约120FPS的渲染上限
    }

    std::cout << "固定时间步长循环结束\n";
}

void SimulationAdvancedEngine::updatePhysics(double dt) {
    // 固定时间步长的物理更新
    // 确保物理计算的一致性和可预测性
}

void SimulationAdvancedEngine::render(double interpolation) {
    // 使用插值进行平滑渲染
    // interpolation 值在 0.0 到 1.0 之间
    static int frameCount = 0;
    if (++frameCount % 120 == 0) {
        std::cout << "Render frame with interpolation: " << interpolation << std::endl;
    }
}


