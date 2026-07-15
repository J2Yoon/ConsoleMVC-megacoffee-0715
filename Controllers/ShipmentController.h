#pragma once

#include <vector>

#include "../Models/Order.h"
#include "../Repositories/IOrderRepository.h"

// 출고 처리 기능을 담당하는 Controller.
class ShipmentController
{
public:
    explicit ShipmentController(IOrderRepository& orderRepository);

    // 출고 가능(CONFIRMED) 주문 목록을 조회한다.
    std::vector<Order> GetShippableOrders() const;

    // 특정 주문을 출고 처리한다. CONFIRMED -> RELEASED.
    bool Ship(const std::string& orderId);

private:
    IOrderRepository& orderRepository_;
};
