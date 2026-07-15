#include "ShipmentController.h"

ShipmentController::ShipmentController(IOrderRepository& orderRepository)
    : orderRepository_(orderRepository)
{
}

std::vector<Order> ShipmentController::GetShippableOrders() const
{
    return orderRepository_.FindByStatus(OrderStatus::Confirmed);
}

bool ShipmentController::Ship(const std::string& orderId)
{
    auto order = orderRepository_.FindById(orderId);
    if (!order.has_value() || order->status != OrderStatus::Confirmed)
    {
        return false;
    }

    return orderRepository_.UpdateStatus(orderId, OrderStatus::Released);
}
