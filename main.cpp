#include <Windows.h>

#include "Controllers/MonitoringController.h"
#include "Controllers/OrderController.h"
#include "Controllers/ProductionController.h"
#include "Controllers/SampleController.h"
#include "Controllers/ShipmentController.h"
#include "Repositories/InMemoryOrderRepository.h"
#include "Repositories/InMemorySampleRepository.h"
#include "Views/MainMenuView.h"

// 반도체 시료 생산주문관리 시스템 - MVC 스켈레톤 PoC
//
// 계층 구성
//   Models      : Sample, Order, ProductionJob 등 순수 데이터
//   Repositories: 데이터 접근 인터페이스 + 인메모리 구현 (추후 DataPersistence PoC 결과물로 교체 예정)
//   Controllers : 시료/주문/생산/출고/모니터링 업무 로직
//   Views       : 콘솔 입출력 및 메뉴 흐름
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    InMemorySampleRepository sampleRepository;
    InMemoryOrderRepository orderRepository;

    SampleController sampleController(sampleRepository);
    ProductionController productionController(sampleRepository, orderRepository);
    OrderController orderController(orderRepository, sampleRepository, productionController);
    ShipmentController shipmentController(orderRepository);
    MonitoringController monitoringController(orderRepository, sampleRepository);

    MainMenuView mainMenuView(sampleController, orderController, productionController,
        shipmentController, monitoringController);
    mainMenuView.Run();

    return 0;
}
