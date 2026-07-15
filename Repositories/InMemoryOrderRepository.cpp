#include "InMemoryOrderRepository.h"

#include <algorithm>

bool InMemoryOrderRepository::Add(const Order& order)
{
    if (FindById(order.orderId).has_value())
    {
        return false; // 이미 존재하는 주문번호
    }

    orders_.push_back(order);
    return true;
}

std::vector<Order> InMemoryOrderRepository::GetAll() const
{
    return orders_;
}

std::vector<Order> InMemoryOrderRepository::FindByStatus(OrderStatus status) const
{
    std::vector<Order> result;
    for (const auto& order : orders_)
    {
        if (order.status == status)
        {
            result.push_back(order);
        }
    }
    return result;
}

std::optional<Order> InMemoryOrderRepository::FindById(const std::string& orderId) const
{
    auto it = std::find_if(orders_.begin(), orders_.end(),
        [&orderId](const Order& order) { return order.orderId == orderId; });

    if (it == orders_.end())
    {
        return std::nullopt;
    }
    return *it;
}

bool InMemoryOrderRepository::UpdateStatus(const std::string& orderId, OrderStatus newStatus)
{
    auto it = std::find_if(orders_.begin(), orders_.end(),
        [&orderId](const Order& order) { return order.orderId == orderId; });

    if (it == orders_.end())
    {
        return false;
    }

    it->status = newStatus;
    return true;
}
