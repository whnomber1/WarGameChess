#include <CJsonVariant.h>
#include <DataDistributionClient.h>
#include <QCoreApplication>
#include <QDateTime>
#include <SimulationEngine.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <windows.h>

#include "DataDistributioner.h"
#include "DeduceEvent.h"

SimulationEngine::SimulationEngine()
    : IGameEngine()
{
}

SimulationEngine::~SimulationEngine()
{

}

void SimulationEngine::engRun()
{
    // QCoreApplication::pr
    m_startDateTime = QDateTime::currentSecsSinceEpoch();

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
// #pragma comment(lib, "winmm.lib")
void SimulationEngine::runWithBusyWait()
{
#if 0
    auto startTime = std::chrono::steady_clock::now(); // 仅用于每秒统计重置
    int frameNum = 0;
    auto lastSimTime = std::chrono::steady_clock::now(); // 【关键新增】记录上一次updateSimTime结束时刻（连续时间，不重置）

    while (m_IsRunning)
    {
        auto currentTime = std::chrono::steady_clock::now();
        auto durtm = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();

        // ===== 核心：计算自上次模拟结束到现在的总真实流逝时间 =====
        double totalElapsedMs = std::chrono::duration<double, std::milli>(currentTime - lastSimTime).count();
        double idealFrameTime = 1000.0 / m_targetFPS; // 理想单帧耗时（ms）

        // 判断是否需要执行（含追赶逻辑）：当前累计时间 >= 应执行帧数 * 理想帧时间
        if (frameNum * idealFrameTime <= totalElapsedMs)
        {
            // 【关键修正】计算本次需追赶的帧数（避免负数/过大）
            int catchUpFrames = static_cast<int>(totalElapsedMs / idealFrameTime) - frameNum;
            if (catchUpFrames < 1) catchUpFrames = 1;
            const int MAX_CATCHUP = 5; // 防卡顿雪崩（根据业务调整）
            if (catchUpFrames > MAX_CATCHUP) catchUpFrames = MAX_CATCHUP;

            // 【精准步长】将本次需处理的总时间平均分配给追赶帧数
            double stepPerFrame = totalElapsedMs / catchUpFrames;

            // 连续执行catchUpFrames次（原逻辑扩展）
            for (int i = 0; i < catchUpFrames; ++i)
            {
                updateSimTime(stepPerFrame); // 传入精确平均步长
                frameNum++; // 原统计逻辑保留

                // 检查是否需重置统计（不影响模拟时间流）
                auto curtm = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - startTime).count();
                if (frameNum == m_targetFPS || curtm >= 1000)
                {
                    std::cerr << "\n average runtime: " << curtm << "ms, "
                        << "average FPS: " << double(frameNum) / double(curtm) * 1000
                        << std::endl;

                    frameNum = 0;
                    startTime = std::chrono::steady_clock::now();
                    // 注意：lastSimTime 不重置！保持连续时间流
                }
            }

            // 【关键】更新“上次模拟结束时刻” = 当前时刻（已处理完totalElapsedMs）
            lastSimTime = std::chrono::steady_clock::now();
        }
        else
        {
            // 休眠：计算到“理想下次执行点”的剩余时间
            double sleepMs = idealFrameTime - (totalElapsedMs - frameNum * idealFrameTime);
            if (sleepMs > 0.5) // 避免休眠0或负值
            {
                std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(sleepMs));
            }
            else
            {
                std::this_thread::yield(); // 时间太短则让出CPU
            }
        }
    }
#elif 0
    // 模拟步长（毫秒）：由目标FPS决定，确保每秒推进总时间为1000ms
    const double simStepMs = 1000.0 / m_targetFPS;
    double accumulator = 0.0; // 累积待处理的模拟时间（毫秒）
    auto lastTime = std::chrono::steady_clock::now();

    // 安全参数（防卡顿雪崩）
    const double maxAccumulator = 250.0;   // 最大累积时间（毫秒）
    const int maxStepsPerFrame = 10;       // 单帧最大更新次数
    int frameNum=0;
    auto startTime = std::chrono::steady_clock::now();

    while (m_IsRunning)
    {
        // 1. 计算真实经过时间
        auto currentTime = std::chrono::steady_clock::now();
        double deltaTime = std::chrono::duration<double, std::milli>(
            currentTime - lastTime).count();
        lastTime = currentTime;

        // 2. 限制异常大的时间差（如窗口最小化后恢复）
        if (deltaTime > maxAccumulator) deltaTime = maxAccumulator;
        accumulator += deltaTime;

        // 3. 限制总累积量，避免追赶雪崩
        if (accumulator > maxAccumulator) accumulator = maxAccumulator;

        // 4. 按固定步长追赶模拟时间（关键修改）
        int steps = 0;
        while (accumulator >= simStepMs && steps < maxStepsPerFrame)
        {
            updateSimTime(simStepMs); // 传入精确步长（毫秒）
            accumulator -= simStepMs;
            ++steps;
            frameNum++;
            auto curtm = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime).count();
            if (frameNum==m_targetFPS || curtm>=1000)
            {
                std::cerr << "\n average runtime: " << curtm << "ms, "
                          << "average FPS: " << double(frameNum)/double(curtm)*1000
                          << std::endl;

                frameNum=0;
                startTime=std::chrono::steady_clock::now();
            }
        }

        // 5. 休眠减少CPU占用（避免忙等待）
        if (accumulator < simStepMs)
        {
            // 休眠剩余时间（至少1ms，避免精度问题）
            double sleepMs = 100 / m_targetFPS;
            std::this_thread::sleep_for(
                std::chrono::duration<double, std::milli>(sleepMs));
        }
    }
#elif 1
    auto startTime = std::chrono::steady_clock::now();
    // 1s的时间
    m_CurrentSimTime=0;
    uint64_t curSimTim=0;
    frameCount=0;
    auto fpsbgtm = startTime;
    while (m_IsRunning)
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        if (m_updateFps)
        {
            m_updateFps=0;
            curSimTim=0;
            startTime=currentTime;
        }
        // if (POSamplingDataBeginTime+POSamplingCurNum*SAMPLINGINTERVAL<RuningInTime_counter)
        auto durtm = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime-startTime).count();
        if (curSimTim*1000/m_targetFPS <= durtm) // 执行逻辑
        {
            int shorttm = static_cast<double>(durtm)*static_cast<double>(m_targetFPS)/1000.0-curSimTim;
            shorttm++;
            // std::cerr << shorttm << "   ";
            updateSimTime(shorttm);
            m_CurrentSimTime+=shorttm;
            curSimTim+=shorttm;
            frameCount++;
            auto curtm = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - fpsbgtm).count();
            if (frameCount>=m_targetFPS||curtm>=1000)
            {
                // std::cerr << "average runtime: " << curtm << "ms, "
                //           << "average FPS: " << double(frameCount)/double(curtm)*1000.0
                //           << "  simTime (s): " << m_CurrentSimTime
                //           << "  realFrameNum: " << frameCount
                //           << std::endl;
                frameCount=0;
                fpsbgtm=std::chrono::steady_clock::now();
            }
        }
        else
        {
            int msnum = 100 / m_targetFPS;
            if (!msnum) msnum=1;
            std::this_thread::sleep_for(std::chrono::milliseconds(msnum));
            // std::this_thread::yield();
        }
    }
    // for (int i = 0; i < 10;)
    // {
    //     auto startTime = std::chrono::steady_clock::now();
    //     using namespace std::chrono_literals;
    //     std::this_thread::sleep_for(5ms);
    //     auto endtime = std::chrono::steady_clock::now();
    //     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endtime - startTime).count();
    //     std::cout << "耗时: " << duration << " 毫秒" << std::endl;
    // }
#endif
}

void SimulationEngine::runWithBusyWait1()
{
    std::cout << "开始忙等待循环 (60FPS) - 注意CPU占用较高\n";

    frameCount = 0;
    auto startTime = std::chrono::high_resolution_clock::now();
    auto nextFrameTime = startTime;

    using Clock = std::chrono::high_resolution_clock;
    using Duration = Clock::duration;
    using Period = Duration::period;
    std::cout << "period: " << Period::num << "/" << Period::den << " s" << std::endl;
    bool isUpdateFramtime=false;
    while (m_IsRunning)
    { // 运行5秒，因为CPU占用高
        auto currentTime = std::chrono::high_resolution_clock::now();
        static int iii=0;

        if (currentTime >= nextFrameTime)
        {
            // 游戏逻辑更新
            updateSimTime(m_curStepLen);
#if 0
            if(isFrameCycleNum(0.1))
            {
                iii++;
                auto tm1 = std::chrono::high_resolution_clock::now();
                int ii=75000000;
                while(ii)
                {
                    ii--;
                }
                auto tm2 = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(tm2 - tm1);
                std::cout << "---average runtime: " << duration.count() << "ms, "<< std::endl;
                // std::this_thread::sleep_for(std::chrono::duration<double>(m_targetFrameTime*2));
            }
#endif
            // 渲染
            render();
            frameCount++;
            m_currentDateTime = m_startDateTime + frameCount * m_curStepLen;

            if (canShowLogInfo())
            {
                auto endTime = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
                startTime = endTime;
                std::cerr << "average runtime: " << duration.count() << "ms, "
                          << "average FPS: " << ((m_targetFPS * m_showcont) * 1000.0 / duration.count())
                          << ", iii: " << iii << "\n"
                          << "currentTime: " << (nextFrameTime + std::chrono::microseconds(static_cast<long>(m_targetFrameTime * 1000000))).time_since_epoch().count()
                          << ", nextFrameTime: " << nextFrameTime.time_since_epoch().count()
                          << std::endl;
                iii=0;
            }

            // 设置下一帧时间
            if(isUpdateFramtime)
            {
                isUpdateFramtime = false;
                nextFrameTime = currentTime + std::chrono::microseconds(static_cast<long>(m_targetFrameTime * 1000000));
            }
            else
            {
                nextFrameTime += std::chrono::microseconds(static_cast<long>(m_targetFrameTime * 1000000));
            }
        }
        else
        {
            // if (canShowLogInfo())
            // {
                // std::cout << "111111111" << std::endl;
            // }
        }

        // 忙等待 - 不推荐在实际项目中使用，除非需要极高精度
        // std::this_thread::yield(); // 可以添加这行来减少CPU占用
        QCoreApplication::processEvents();
        auto frameEndTime = std::chrono::high_resolution_clock::now();
        auto frameProcessTime = std::chrono::duration<double>(frameEndTime - currentTime).count();
        if(m_targetFrameTime > frameProcessTime)
        {
            std::this_thread::sleep_for(std::chrono::duration<double>(m_targetFrameTime*0.05));
        }
        else
        {
            isUpdateFramtime = true;
        }
    }
}

bool SimulationEngine::canShowLogInfo(int num)
{
    if(!num) num = m_showcont;
    return m_showFpsInfo && (frameCount % (m_targetFPS * num) == 0);
}

bool SimulationEngine::isFrameCycleNum(float num)
{
    return (frameCount % static_cast<int>(m_targetFPS * num) == 0);
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
    // updateLongInfo(std::string("stepLen: ") + std::to_string(stepLen));
    // if(isFrameCycleNum(0.5))
    // {
    //     CJsonVariant var;
    //     var.setValue("simTime", m_currentDateTime);
    //     DataDistributionClient::GetInstance()->SendClientMessage(var.GetJsonString());
    // }
    if (!frameCount)
    {
        CJsonVariant var;
        var.setValue("eventType", EV_TimeUpdate);
        var.setValue("simTime", static_cast<qint64>(m_CurrentSimTime));
        // DataDistributionClient::GetInstance()->SendClientMessage(var.GetJsonString());
        DataDistributioner::GetInstance()->sendPublicMessageSig(var.GetJsonString());
    }
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
    m_targetFPS = fnum;
    m_updateFps=1;
}


