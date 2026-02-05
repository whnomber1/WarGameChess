//
// Created by 1 on 2026/2/4.
//

#include "DeduceWorker.h"

#include <iostream>

#include "DataDistributioner.h"
#include "SimulationEngine.h"

void DeduceWorker::run()
{
    SimulationEngine eng;
    DataDistributioner::GetInstance()->SetSimulationEngine(&eng);
    eng.setRunFPS(80);
    eng.engRun();
    std::cout << "所有测试完成！\n";
}
