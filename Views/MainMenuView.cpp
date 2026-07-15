#include "MainMenuView.h"

#include <iomanip>
#include <iostream>

#include "ConsoleView.h"

namespace
{
    const char* ToKorean(StockLevel level)
    {
        switch (level)
        {
        case StockLevel::Sufficient: return "여유";
        case StockLevel::Low:        return "부족";
        case StockLevel::Depleted:   return "고갈";
        }
        return "알수없음";
    }
}

MainMenuView::MainMenuView(SampleController& sampleController, OrderController& orderController,
    ProductionController& productionController, ShipmentController& shipmentController,
    MonitoringController& monitoringController)
    : sampleController_(sampleController)
    , orderController_(orderController)
    , productionController_(productionController)
    , shipmentController_(shipmentController)
    , monitoringController_(monitoringController)
{
}

void MainMenuView::Run()
{
    while (true)
    {
        ShowMainMenu();
        int choice = ConsoleView::ReadInt("선택 > ");

        switch (choice)
        {
        case 1: RunSampleMenu(); break;
        case 2: RunOrderMenu(); break;
        case 3: RunApprovalMenu(); break;
        case 4: RunMonitoringMenu(); break;
        case 5: RunProductionMenu(); break;
        case 6: RunShipmentMenu(); break;
        case 0: return;
        default: ConsoleView::PrintError("올바른 번호를 선택해주세요."); break;
        }
    }
}

void MainMenuView::ShowMainMenu() const
{
    const auto samples = sampleController_.GetAllSamples();
    const auto counts = monitoringController_.GetOrderCountsByStatus();

    int totalStock = 0;
    for (const auto& sample : samples)
    {
        totalStock += sample.stock;
    }

    int totalOrders = 0;
    for (const auto& [status, count] : counts)
    {
        totalOrders += count;
    }

    ConsoleView::PrintTitle("반도체 시료 생산주문관리 시스템");
    std::cout << "등록 시료 " << samples.size() << "종   총 재고 " << totalStock << " ea   전체 주문 "
        << totalOrders << "건   생산라인 대기 " << productionController_.GetQueue().size() << "건\n";
    ConsoleView::PrintDivider();
    ConsoleView::PrintLine("[1] 시료 관리        [2] 시료 주문");
    ConsoleView::PrintLine("[3] 주문 승인/거절   [4] 모니터링");
    ConsoleView::PrintLine("[5] 생산 라인 조회   [6] 출고 처리");
    ConsoleView::PrintLine("[0] 종료");
    ConsoleView::PrintDivider();
}

void MainMenuView::RunSampleMenu()
{
    ConsoleView::PrintTitle("[1] 시료 관리");
    ConsoleView::PrintLine("[1] 시료 등록   [2] 시료 목록   [3] 시료 검색   [0] 뒤로");
    int choice = ConsoleView::ReadInt("선택 > ");

    if (choice == 1)
    {
        std::string id = ConsoleView::ReadLine("시료 ID   > ");
        std::string name = ConsoleView::ReadLine("시료명    > ");
        double avgMinutes = ConsoleView::ReadDouble("평균 생산시간(분/ea) > ");
        double yield = ConsoleView::ReadDouble("수율(0.0~1.0)        > ");

        if (sampleController_.RegisterSample(id, name, avgMinutes, yield))
        {
            ConsoleView::PrintLine("시료 등록 완료.");
        }
        else
        {
            ConsoleView::PrintError("이미 등록된 시료 ID 입니다.");
        }
    }
    else if (choice == 2)
    {
        const auto samples = sampleController_.GetAllSamples();
        ConsoleView::PrintLine("등록 시료 목록 (총 " + std::to_string(samples.size()) + "종)");
        std::cout << std::left << std::setw(10) << "ID" << std::setw(20) << "시료명"
            << std::setw(16) << "평균생산시간" << std::setw(10) << "수율" << "재고\n";
        for (const auto& sample : samples)
        {
            std::cout << std::left << std::setw(10) << sample.id << std::setw(20) << sample.name
                << std::setw(16) << sample.avgProductionMinutesPerUnit << std::setw(10) << sample.yield
                << sample.stock << " ea\n";
        }
    }
    else if (choice == 3)
    {
        std::string keyword = ConsoleView::ReadLine("검색어(이름 포함) > ");
        const auto samples = sampleController_.SearchByName(keyword);
        ConsoleView::PrintLine("검색 결과 " + std::to_string(samples.size()) + "건");
        for (const auto& sample : samples)
        {
            std::cout << sample.id << " | " << sample.name << " | 재고 " << sample.stock << " ea\n";
        }
    }
}

void MainMenuView::RunOrderMenu()
{
    ConsoleView::PrintTitle("[2] 시료 주문");
    std::string sampleId = ConsoleView::ReadLine("시료 ID   > ");
    std::string customerName = ConsoleView::ReadLine("고객명    > ");
    int quantity = ConsoleView::ReadInt("주문 수량 > ");

    auto order = orderController_.PlaceOrder(sampleId, customerName, quantity);
    if (!order.has_value())
    {
        ConsoleView::PrintError("등록되지 않은 시료 ID 입니다.");
        return;
    }

    ConsoleView::PrintLine("예약 접수 완료.");
    std::cout << "주문번호 " << order->orderId << " | 현재 상태 " << ToString(order->status) << "\n";
}

void MainMenuView::RunApprovalMenu()
{
    ConsoleView::PrintTitle("[3] 주문 승인/거절");
    const auto reserved = orderController_.GetReservedOrders();

    if (reserved.empty())
    {
        ConsoleView::PrintLine("승인 대기 중인 주문이 없습니다.");
        return;
    }

    ConsoleView::PrintLine("승인 대기 중인 예약 목록 (RESERVED)");
    for (size_t i = 0; i < reserved.size(); ++i)
    {
        const auto& order = reserved[i];
        std::cout << "[" << (i + 1) << "] " << order.orderId << " | " << order.customerName
            << " | " << order.sampleId << " | " << order.quantity << " ea\n";
    }

    int index = ConsoleView::ReadInt("승인/거절할 번호 > ");
    if (index < 1 || static_cast<size_t>(index) > reserved.size())
    {
        ConsoleView::PrintError("올바른 번호를 선택해주세요.");
        return;
    }

    const std::string orderId = reserved[static_cast<size_t>(index) - 1].orderId;
    std::string decision = ConsoleView::ReadLine("[Y] 승인   [N] 거절 > ");

    if (decision == "Y" || decision == "y")
    {
        orderController_.ApproveOrder(orderId);
        ConsoleView::PrintLine("승인 처리 완료. (재고 상황에 따라 CONFIRMED 또는 PRODUCING 으로 전환)");
    }
    else
    {
        orderController_.RejectOrder(orderId);
        ConsoleView::PrintLine("거절 처리 완료. (REJECTED)");
    }
}

void MainMenuView::RunMonitoringMenu() const
{
    ConsoleView::PrintTitle("[4] 모니터링");
    ConsoleView::PrintLine("[1] 주문량 확인   [2] 재고량 확인   [0] 뒤로");
    int choice = ConsoleView::ReadInt("선택 > ");

    if (choice == 1)
    {
        const auto counts = monitoringController_.GetOrderCountsByStatus();
        ConsoleView::PrintLine("상태별 주문 현황 (REJECTED 제외)");
        for (const auto& [status, count] : counts)
        {
            std::cout << std::left << std::setw(12) << ToString(status) << count << "건\n";
        }
    }
    else if (choice == 2)
    {
        const auto statuses = monitoringController_.GetStockStatuses();
        ConsoleView::PrintLine("시료별 재고 현황");
        std::cout << std::left << std::setw(20) << "시료명" << std::setw(10) << "재고"
            << std::setw(10) << "미출고주문" << "상태\n";
        for (const auto& status : statuses)
        {
            std::cout << std::left << std::setw(20) << status.sample.name << std::setw(10) << status.sample.stock
                << std::setw(10) << status.pendingDemand << ToKorean(status.level) << "\n";
        }
    }
}

void MainMenuView::RunProductionMenu()
{
    ConsoleView::PrintTitle("[5] 생산 라인 조회 (FIFO)");

    const auto* current = productionController_.GetCurrentJob();
    if (current == nullptr)
    {
        ConsoleView::PrintLine("현재 처리 중인 생산 작업이 없습니다.");
        return;
    }

    std::cout << "현재 처리 중  | 주문번호 " << current->orderId << " | 시료 " << current->sampleId
        << " | 부족분 " << current->shortage << " ea -> 실생산량 " << current->actualQuantity << " ea\n";

    const auto queue = productionController_.GetQueue();
    ConsoleView::PrintLine("대기 중인 주문 (FIFO 순)");
    for (size_t i = 1; i < queue.size(); ++i)
    {
        const auto& job = queue[i];
        std::cout << "[" << i << "] " << job.orderId << " | " << job.sampleId
            << " | 부족분 " << job.shortage << " ea -> 실생산량 " << job.actualQuantity << " ea\n";
    }

    ConsoleView::PrintLine("");
    ConsoleView::PrintLine("* PoC 스켈레톤 범위: 실제 생산 소요 시간의 경과 반영은 본 단계에서는 다루지 않으며,");
    ConsoleView::PrintLine("  담당자가 아래 메뉴로 수동 완료를 트리거하는 것으로 대체한다.");
    std::string decision = ConsoleView::ReadLine("[Y] 현재 작업 완료 처리   [N] 뒤로 > ");
    if (decision == "Y" || decision == "y")
    {
        if (productionController_.CompleteCurrentJob())
        {
            ConsoleView::PrintLine("생산 완료 처리됨. 해당 주문 상태가 CONFIRMED로 전환되었습니다.");
        }
    }
}

void MainMenuView::RunShipmentMenu()
{
    ConsoleView::PrintTitle("[6] 출고 처리");
    const auto shippable = shipmentController_.GetShippableOrders();

    if (shippable.empty())
    {
        ConsoleView::PrintLine("출고 가능한(CONFIRMED) 주문이 없습니다.");
        return;
    }

    ConsoleView::PrintLine("출고 가능 주문 (CONFIRMED)");
    for (size_t i = 0; i < shippable.size(); ++i)
    {
        const auto& order = shippable[i];
        std::cout << "[" << (i + 1) << "] " << order.orderId << " | " << order.customerName
            << " | " << order.sampleId << " | " << order.quantity << " ea\n";
    }

    int index = ConsoleView::ReadInt("출고할 번호 > ");
    if (index < 1 || static_cast<size_t>(index) > shippable.size())
    {
        ConsoleView::PrintError("올바른 번호를 선택해주세요.");
        return;
    }

    const std::string orderId = shippable[static_cast<size_t>(index) - 1].orderId;
    if (shipmentController_.Ship(orderId))
    {
        ConsoleView::PrintLine("출고 처리 완료. (RELEASED)");
    }
    else
    {
        ConsoleView::PrintError("출고 처리에 실패했습니다.");
    }
}
