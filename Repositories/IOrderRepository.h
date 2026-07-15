#pragma once

#include <optional>
#include <string>
#include <vector>

#include "../Models/Order.h"
#include "../Models/OrderStatus.h"

// 주문 데이터 접근을 담당하는 Repository 인터페이스.
// PoC 단계에서는 인메모리 구현체만 두고, 추후 별도 DataPersistence PoC의 결과물로 교체한다.
class IOrderRepository
{
public:
    virtual ~IOrderRepository() = default;

    virtual bool Add(const Order& order) = 0;
    virtual std::vector<Order> GetAll() const = 0;
    virtual std::vector<Order> FindByStatus(OrderStatus status) const = 0;
    virtual std::optional<Order> FindById(const std::string& orderId) const = 0;
    virtual bool UpdateStatus(const std::string& orderId, OrderStatus newStatus) = 0;
};
