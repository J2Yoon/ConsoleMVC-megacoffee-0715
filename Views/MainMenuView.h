#pragma once

#include "../Controllers/MonitoringController.h"
#include "../Controllers/OrderController.h"
#include "../Controllers/ProductionController.h"
#include "../Controllers/SampleController.h"
#include "../Controllers/ShipmentController.h"

// 메인 메뉴 화면 흐름을 담당하는 View.
// 사용자 입력을 받아 각 Controller에 위임하고, 그 결과를 콘솔에 표시한다.
class MainMenuView
{
public:
    MainMenuView(SampleController& sampleController, OrderController& orderController,
        ProductionController& productionController, ShipmentController& shipmentController,
        MonitoringController& monitoringController);

    // 메인 메뉴 루프를 실행한다. 사용자가 종료를 선택하면 반환한다.
    void Run();

private:
    SampleController& sampleController_;
    OrderController& orderController_;
    ProductionController& productionController_;
    ShipmentController& shipmentController_;
    MonitoringController& monitoringController_;

    void ShowMainMenu() const;
    void RunSampleMenu();
    void RunOrderMenu();
    void RunApprovalMenu();
    void RunMonitoringMenu() const;
    void RunProductionMenu();
    void RunShipmentMenu();
};
