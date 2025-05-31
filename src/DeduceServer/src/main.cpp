#include <iostream>
#include <SimulationEngine.h>

int main()
{

    SimulationEngine eng;
    eng.setRunFPS(25);
    eng.engRun();
    std::cout << "所有测试完成！\n";
    return 0;
}
