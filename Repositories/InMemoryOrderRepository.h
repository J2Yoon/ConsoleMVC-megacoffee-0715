#pragma once

#include <vector>

#include "IOrderRepository.h"

// IOrderRepository의 인메모리(std::vector 기반) 구현체.
class InMemoryOrderRepository : public IOrderRepository
{
public:
    bool Add(const Order& order) override;
    std::vector<Order> GetAll() const override;
    std::vector<Order> FindByStatus(OrderStatus status) const override;
    std::optional<Order> FindById(const std::string& orderId) const override;
    bool UpdateStatus(const std::string& orderId, OrderStatus newStatus) override;

private:
    std::vector<Order> orders_;
};
