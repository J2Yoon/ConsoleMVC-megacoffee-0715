#pragma once

#include <optional>
#include <string>
#include <vector>

#include "../Models/Order.h"
#include "../Repositories/IOrderRepository.h"
#include "../Repositories/ISampleRepository.h"
#include "ProductionController.h"

// 시료 주문(예약/승인/거절) 기능을 담당하는 Controller.
class OrderController
{
public:
    OrderController(IOrderRepository& orderRepository, ISampleRepository& sampleRepository,
        ProductionController& productionController);

    // 시료 예약 : 시료 ID, 고객명, 주문 수량을 받아 RESERVED 상태의 주문을 생성한다.
    // 존재하지 않는 시료 ID면 std::nullopt.
    std::optional<Order> PlaceOrder(const std::string& sampleId, const std::string& customerName, int quantity);

    // 접수된(RESERVED) 주문 목록을 조회한다.
    std::vector<Order> GetReservedOrders() const;

    // 접수된 주문을 승인한다.
    // 재고가 충분하면 즉시 CONFIRMED, 부족하면 생산 라인에 등록 후 PRODUCING으로 전환한다.
    bool ApproveOrder(const std::string& orderId);

    // 접수된 주문을 거절한다. 즉시 REJECTED로 전환한다.
    bool RejectOrder(const std::string& orderId);

private:
    IOrderRepository& orderRepository_;
    ISampleRepository& sampleRepository_;
    ProductionController& productionController_;

    int nextOrderSequence_ = 1;

    std::string GenerateOrderId();
};
