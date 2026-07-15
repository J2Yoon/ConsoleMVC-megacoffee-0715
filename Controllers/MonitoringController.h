#pragma once

#include <map>
#include <vector>

#include "../Models/Sample.h"
#include "../Repositories/IOrderRepository.h"
#include "../Repositories/ISampleRepository.h"

// 재고 상태 : 주문 대비 재고 수량에 따라 표기되는 상태
enum class StockLevel
{
    Sufficient, // 여유 : 대기 중인 주문 수량 대비 재고 충분
    Low,        // 부족 : 대기 중인 주문 수량 대비 재고 부족
    Depleted    // 고갈 : 재고 수량이 0
};

struct StockStatus
{
    Sample sample;
    int pendingDemand = 0; // 해당 시료에 대한 미출고 주문(RESERVED/PRODUCING/CONFIRMED) 총 수량
    StockLevel level = StockLevel::Sufficient;
};

// 주문 현황 및 재고 현황을 확인하는 모니터링 Controller.
class MonitoringController
{
public:
    MonitoringController(IOrderRepository& orderRepository, ISampleRepository& sampleRepository);

    // 상태별 주문 건수를 조회한다. REJECTED는 유효한 주문이 아니므로 제외한다.
    std::map<OrderStatus, int> GetOrderCountsByStatus() const;

    // 시료별 재고 현황(재고 수량, 미출고 주문 대비 상태)을 조회한다.
    std::vector<StockStatus> GetStockStatuses() const;

private:
    IOrderRepository& orderRepository_;
    ISampleRepository& sampleRepository_;
};
