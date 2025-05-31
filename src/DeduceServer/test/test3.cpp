#include <iostream>

int main() {
    // GameEngine engine;

    std::cout << "=== C++ 游戏引擎帧循环实现示例  ===\n\n" << std::endl;

    // 测试不同的循环方法
    // engine.runWithSleepUntil();

    // engine.runWithDynamicTiming();

    // engine.runWithBusyWait(); // 注释掉，因为CPU占用高

    // // 高级循环示例
    // AdvancedGameEngine advEngine;
    // advEngine.runFixedTimestep();

    // timeBeginPeriod(1); // 设置系统全局计时器分辨率为 1ms

    // const std::chrono::microseconds frameDuration(16000);
    // auto frameStart = std::chrono::steady_clock::now();
    // int frameCount=0;
    // auto startTime = std::chrono::steady_clock::now();
    // while (1)
    // {
    //     // QThread::usleep(16000);
    //     // auto frameEnd = std::chrono::steady_clock::now();
    //     // auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);
    //     frameCount++;
    //     if ((frameCount % 100) == 0)
    //     {
    //         auto endTime = std::chrono::high_resolution_clock::now();
    //         auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    //         startTime = endTime;
    //         std::cout<< "frameCount: " <<frameCount << ", 实际运行时间: " << duration.count() << "ms, "
    //                   << "平均FPS: " << (static_cast<double>(100) * 1000.0 / duration.count()) << std::endl;

    //     }

    //     std::this_thread::sleep_for(frameDuration);
    // }


    std::cout << "所有测试完成！\n";
    return 0;
}
