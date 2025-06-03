#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <numeric>
#include <iomanip>

class GameEngineWithFPSMonitor {
private:
    static constexpr double PHYSICS_TIMESTEP = 1.0 / 60.0;  // 60Hz 物理更新
    static constexpr double MAX_FRAME_TIME = 0.25;

    bool running = true;
    double accumulator = 0.0;

    // FPS监控相关
    std::vector<double> frameTimes;
    std::chrono::steady_clock::time_point lastFPSReport;
    int totalFrames = 0;
    int physicsUpdates = 0;

    // 游戏对象状态（用于演示插值）
    struct GameObject {
        double x = 0.0, y = 0.0;           // 当前位置
        double prevX = 0.0, prevY = 0.0;   // 上一帧位置
        double velX = 50.0, velY = 30.0;   // 速度 (像素/秒)
    } player;

public:
    void runWithInterpolation() {
        std::cout << "=== 游戏循环：插值渲染 + FPS监控 ===\n";
        std::cout << "物理更新: 60Hz 固定频率\n";
        std::cout << "渲染频率: 可变（目标约120-144Hz）\n\n";

        auto currentTime = std::chrono::high_resolution_clock::now();
        lastFPSReport = currentTime;

        while (running /*&& totalFrames < 1000*/) {  // 运行1000渲染帧
            auto newTime = std::chrono::high_resolution_clock::now();
            auto frameTime = std::chrono::duration<double>(newTime - currentTime).count();

            // 防止死亡螺旋
            if (frameTime > MAX_FRAME_TIME) {
                frameTime = MAX_FRAME_TIME;
            }

            currentTime = newTime;
            accumulator += frameTime;

            // 固定频率的物理更新 (60Hz)
            while (accumulator >= PHYSICS_TIMESTEP) {
                updatePhysics(PHYSICS_TIMESTEP);
                accumulator -= PHYSICS_TIMESTEP;
                physicsUpdates++;
            }

            // 计算插值因子
            double interpolation = accumulator / PHYSICS_TIMESTEP;

            // 渲染（使用插值进行平滑）
            render(interpolation, frameTime);

            totalFrames++;

            // 记录帧时间用于FPS计算
            frameTimes.push_back(frameTime);
            if (frameTimes.size() > 60) {  // 只保留最近60帧的数据
                frameTimes.erase(frameTimes.begin());
            }

            // 每秒报告一次FPS
            reportFPSIfNeeded(currentTime);

            // 控制最大渲染帧率（避免过度消耗CPU）
            std::this_thread::sleep_for(std::chrono::microseconds(1000)); // 约166Hz上限
        }

        // 最终统计
        std::cout << "\n=== 最终统计 ===\n";
        std::cout << "总渲染帧数: " << totalFrames << "\n";
        std::cout << "总物理更新数: " << physicsUpdates << "\n";
        std::cout << "物理/渲染比: " << (double)physicsUpdates / totalFrames << "\n";
    }

private:
    void updatePhysics(double dt) {
        // 保存上一帧位置（用于插值）
        player.prevX = player.x;
        player.prevY = player.y;

        // 更新位置
        player.x += player.velX * dt;
        player.y += player.velY * dt;

        // 简单的边界反弹
        if (player.x > 800 || player.x < 0) {
            player.velX = -player.velX;
        }
        if (player.y > 600 || player.y < 0) {
            player.velY = -player.velY;
        }
    }

    void render(double interpolation, double frameTime) {
        // 使用插值计算平滑的渲染位置
        double renderX = player.prevX + (player.x - player.prevX) * interpolation;
        double renderY = player.prevY + (player.y - player.prevY) * interpolation;

        // 每50帧输出一次详细信息
        if (totalFrames % 50 == 0) {
            std::cout << "帧 " << totalFrames << std::endl;
            std::cout << "  插值值: " << std::fixed << std::setprecision(3) << interpolation << "\n";
            std::cout << "  物理位置: (" << player.x << ", " << player.y << ")\n";
            std::cout << "  渲染位置: (" << renderX << ", " << renderY << ")\n";
            std::cout << "  帧时间: " << (frameTime * 1000) << "ms\n";

            if (!frameTimes.empty()) {
                double avgFrameTime = std::accumulate(frameTimes.begin(), frameTimes.end(), 0.0) / frameTimes.size();
                double currentFPS = 1.0 / avgFrameTime;
                std::cout << "  当前FPS: " << std::fixed << std::setprecision(1) << currentFPS << "\n";
            }
            std::cout << std::endl;
        }
    }

    void reportFPSIfNeeded(std::chrono::steady_clock::time_point currentTime) {
        auto timeSinceLastReport = std::chrono::duration<double>(currentTime - lastFPSReport).count();

        if (timeSinceLastReport >= 1.0) {  // 每秒报告一次
            if (!frameTimes.empty()) {
                // 计算平均FPS
                double avgFrameTime = std::accumulate(frameTimes.begin(), frameTimes.end(), 0.0) / frameTimes.size();
                double avgFPS = 1.0 / avgFrameTime;

                // 计算最小和最大FPS
                double minFrameTime = *std::min_element(frameTimes.begin(), frameTimes.end());
                double maxFrameTime = *std::max_element(frameTimes.begin(), frameTimes.end());
                double maxFPS = 1.0 / minFrameTime;
                double minFPS = 1.0 / maxFrameTime;

                std::cout << "[FPS报告] 平均: " << std::fixed << std::setprecision(1) << avgFPS
                          << " | 范围: " << minFPS << " - " << maxFPS << std::endl;
            }

            lastFPSReport = currentTime;
        }
    }
};

// 简化版本：专门用于理解插值概念
class InterpolationDemo {
public:
    void demonstrateInterpolation() {
        std::cout << "\n=== 插值概念演示 ===\n";
        std::cout << "假设物理更新间隔: 16.67ms (60Hz)\n";
        std::cout << "渲染间隔: 8.33ms (120Hz)\n\n";

        double physicsInterval = 1.0 / 60.0;  // 16.67ms
        double renderInterval = 1.0 / 120.0;  // 8.33ms

        double accumulator = 0.0;
        double totalTime = 0.0;

        // 模拟游戏对象
        double physicsPos = 0.0;      // 物理位置
        double prevPhysicsPos = 0.0;  // 前一次物理位置
        double velocity = 100.0;      // 100 像素/秒

        std::cout << "时间轴演示（前几帧）：\n";
        std::cout << "时间(ms) | 事件类型 | 插值值 | 物理位置 | 渲染位置\n";
        std::cout << "---------|----------|--------|----------|----------\n";

        for (int frame = 0; frame < 15; frame++) {
            totalTime += renderInterval;
            accumulator += renderInterval;

            bool physicsUpdate = false;

            // 检查是否需要物理更新
            if (accumulator >= physicsInterval) {
                prevPhysicsPos = physicsPos;
                physicsPos += velocity * physicsInterval;
                accumulator -= physicsInterval;
                physicsUpdate = true;
            }

            // 计算插值
            double interpolation = accumulator / physicsInterval;
            double renderPos = prevPhysicsPos + (physicsPos - prevPhysicsPos) * interpolation;

            // 输出信息
            std::cout << std::fixed << std::setprecision(1);
            std::cout << std::setw(8) << (totalTime * 1000) << " | ";

            if (physicsUpdate) {
                std::cout << "物理+渲染" << " | ";
            } else {
                std::cout << "仅渲染  " << " | ";
            }

            std::cout << std::setprecision(3) << std::setw(6) << interpolation << " | ";
            std::cout << std::setprecision(1) << std::setw(8) << physicsPos << " | ";
            std::cout << std::setw(8) << renderPos << "\n";
        }

        std::cout << "\n插值值解释：\n" << std::endl;
        std::cout << "- 0.0: 刚完成物理更新，渲染位置 = 物理位置\n"<< std::endl;
        std::cout << "- 0.5: 在两次物理更新中间，渲染位置在两者之间\n"<< std::endl;
        std::cout << "- 接近1.0: 快要进行下次物理更新，预测下一帧位置\n"<< std::endl;
    }
};

int main() {
    // 演示插值概念
    InterpolationDemo demo;
    demo.demonstrateInterpolation();

    // 运行完整的游戏循环
    GameEngineWithFPSMonitor engine;
    engine.runWithInterpolation();

    return 0;
}
