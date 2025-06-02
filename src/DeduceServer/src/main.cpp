#include <iostream>
#include <SimulationEngine.h>
#include <DataDistributionClient.h>
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    DataDistributionClient::GetInstance();
    SimulationEngine eng;
    eng.setRunFPS(20);
    eng.engRun();
    std::cout << "所有测试完成！\n";

    return app.exec();
}
