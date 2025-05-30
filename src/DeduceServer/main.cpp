#include <iostream>
#include <chrono>
#include <thread>

class GameEngine {
private:
    static constexpr int TARGET_FPS = 60;
    static constexpr double TARGET_FRAME_TIME = 1.0 / TARGET_FPS; // 16.67ms

    bool running = true;
    int frameCount = 0;

public:
    // 方法1: 使用 chrono 和 sleep_until 的精确计时
    void runWithSleepUntil() {
        std::cout << "开始精确计时循环  (60FPS)\n";

        auto nextFrameTime = std::chrono::steady_clock::now();
        auto startTime = nextFrameTime;

        while (running && frameCount < 600) { // 运行10秒
            // 游戏逻辑更新
            update();

            // 渲染
            render();

            frameCount++;

            // 计算下一帧的时间点
            nextFrameTime += std::chrono::microseconds(static_cast<long>(TARGET_FRAME_TIME * 1000000));

            // 精确等待到下一帧时间
            std::this_thread::sleep_until(nextFrameTime);
        }

        auto endTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

        std::cout << "实际运行时间: " << duration.count() << "ms\n";
        std::cout << "平均FPS: " << (frameCount * 1000.0 / duration.count()) << "\n\n";
    }

    // 方法2: 使用高精度计时和动态调整
    void runWithDynamicTiming() {
        std::cout << "开始动态调整循环 (60FPS)\n";

        frameCount = 0;
        auto startTime = std::chrono::high_resolution_clock::now();
        auto lastFrameTime = startTime;

        while (running && frameCount < 600) {
            auto currentTime = std::chrono::high_resolution_clock::now();

            // 计算上一帧的实际耗时
            auto deltaTime = std::chrono::duration<double>(currentTime - lastFrameTime).count();

            // 游戏逻辑更新（可以使用deltaTime进行时间相关的计算）
            update(deltaTime);

            // 渲染
            render();

            frameCount++;
            lastFrameTime = currentTime;

            // 计算需要等待的时间
            auto frameEndTime = std::chrono::high_resolution_clock::now();
            auto frameProcessTime = std::chrono::duration<double>(frameEndTime - currentTime).count();

            double sleepTime = TARGET_FRAME_TIME - frameProcessTime;
            if (sleepTime > 0) {
                std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
            }
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

        std::cout << "实际运行时间: " << duration.count() << "ms\n";
        std::cout << "平均FPS: " << (frameCount * 1000.0 / duration.count()) << "\n\n";
    }

    // 方法3: 使用忙等待的高精度循环（CPU密集型）
    void runWithBusyWait() {
        std::cout << "开始忙等待循环 (60FPS) - 注意CPU占用较高\n";

        frameCount = 0;
        auto startTime = std::chrono::high_resolution_clock::now();
        auto nextFrameTime = startTime;

        while (running && frameCount < 600) { // 运行5秒，因为CPU占用高
            auto currentTime = std::chrono::high_resolution_clock::now();

            if (currentTime >= nextFrameTime) {
                // 游戏逻辑更新
                update();

                // 渲染
                render();

                frameCount++;

                // 设置下一帧时间
                nextFrameTime += std::chrono::microseconds(static_cast<long>(TARGET_FRAME_TIME * 1000000));
            }

            // 忙等待 - 不推荐在实际项目中使用，除非需要极高精度
            // std::this_thread::yield(); // 可以添加这行来减少CPU占用
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

        std::cout << "实际运行时间: " << duration.count() << "ms\n";
        std::cout << "平均FPS: " << (frameCount * 1000.0 / duration.count()) << "\n\n";
    }

    // 游戏逻辑更新
    void update(double deltaTime = TARGET_FRAME_TIME) {
        // 这里放置游戏逻辑更新代码
        // 例如：物理计算、AI更新、输入处理等

        // 模拟一些处理时间
        std::this_thread::sleep_for(std::chrono::microseconds(1000)); // 1ms的模拟处理时间
    }

    // 渲染函数
    void render() {
        // 这里放置渲染代码
        // 例如：清屏、绘制对象、交换缓冲区等

        // 每100帧输出一次信息
        if (frameCount % 100 == 0) {
            std::cout << "Frame: " << frameCount << std::endl;
        }
    }

    void stop() {
        running = false;
    }
};

// 更高级的游戏循环实现：固定时间步长 + 可变渲染
class AdvancedGameEngine {
private:
    static constexpr double FIXED_TIMESTEP = 1.0 / 60.0; // 固定的物理更新步长
    static constexpr double MAX_FRAME_TIME = 0.25; // 最大帧时间，避免死亡螺旋

    bool running = true;
    double accumulator = 0.0;

public:
    void runFixedTimestep() {
        std::cout << "开始固定时间步长循环\n";

        auto currentTime = std::chrono::high_resolution_clock::now();
        int frameCount = 0;

        while (running && frameCount < 600) {
            auto newTime = std::chrono::high_resolution_clock::now();
            auto frameTime = std::chrono::duration<double>(newTime - currentTime).count();

            // 防止死亡螺旋
            if (frameTime > MAX_FRAME_TIME) {
                frameTime = MAX_FRAME_TIME;
            }

            currentTime = newTime;
            accumulator += frameTime;

            // 固定时间步长的物理更新
            while (accumulator >= FIXED_TIMESTEP) {
                updatePhysics(FIXED_TIMESTEP);
                accumulator -= FIXED_TIMESTEP;
            }

            // 插值因子，用于平滑渲染
            double interpolation = accumulator / FIXED_TIMESTEP;

            // 渲染（可以使用插值进行平滑）
            render(interpolation);

            frameCount++;

            // 控制帧率
            std::this_thread::sleep_for(std::chrono::microseconds(8000)); // 约120FPS的渲染上限
        }

        std::cout << "固定时间步长循环结束\n";
    }

private:
    void updatePhysics(double dt) {
        // 固定时间步长的物理更新
        // 确保物理计算的一致性和可预测性
    }

    void render(double interpolation) {
        // 使用插值进行平滑渲染
        // interpolation 值在 0.0 到 1.0 之间
        static int frameCount = 0;
        if (++frameCount % 120 == 0) {
            std::cout << "Render frame with interpolation: " << interpolation << std::endl;
        }
    }
};

int main() {
    GameEngine engine;

    std::cout << "=== C++ 游戏引擎帧循环实现示例  ===\n\n" << std::endl;

    // 测试不同的循环方法
    engine.runWithSleepUntil();

    engine.runWithDynamicTiming();

    engine.runWithBusyWait(); // 注释掉，因为CPU占用高

    // 高级循环示例
    AdvancedGameEngine advEngine;
    advEngine.runFixedTimestep();

    std::cout << "所有测试完成！\n";
    return 0;
}
