#include "OrderController.h"

#include <iomanip>
#include <sstream>

OrderController::OrderController(IOrderRepository& orderRepository, ISampleRepository& sampleRepository,
    ProductionController& productionController)
    : orderRepository_(orderRepository)
    , sampleRepository_(sampleRepository)
    , productionController_(productionController)
{
}

std::string OrderController::GenerateOrderId()
{
    std::ostringstream oss;
    oss << "ORD-" << std::setw(4) << std::setfill('0') << nextOrderSequence_++;
    return oss.str();
}

std::optional<Order> OrderController::PlaceOrder(const std::string& sampleId, const std::string& customerName, int quantity)
{
    if (!sampleRepository_.FindById(sampleId).has_value())
    {
        return std::nullopt; // 등록되지 않은 시료
    }

    Order order;
    order.orderId = GenerateOrderId();
    order.sampleId = sampleId;
    order.customerName = customerName;
    order.quantity = quantity;
    order.status = OrderStatus::Reserved;

    orderRepository_.Add(order);
    return order;
}

std::vector<Order> OrderController::GetReservedOrders() const
{
    return orderRepository_.FindByStatus(OrderStatus::Reserved);
}

bool OrderController::ApproveOrder(const std::string& orderId)
{
    auto order = orderRepository_.FindById(orderId);
    if (!order.has_value() || order->status != OrderStatus::Reserved)
    {
        return false;
    }

    auto sample = sampleRepository_.FindById(order->sampleId);
    if (!sample.has_value())
    {
        return false;
    }

    if (sample->stock >= order->quantity)
    {
        // 재고 충분 -> 즉시 출고 대기 상태로 전환하고 해당 수량만큼 재고를 예약(차감)한다.
        sampleRepository_.ChangeStock(sample->id, -order->quantity);
        orderRepository_.UpdateStatus(orderId, OrderStatus::Confirmed);
    }
    else
    {
        // 재고 부족 -> 가용 재고 전량을 이 주문에 예약(차감)하고, 부족분만큼 생산 라인에 등록한다.
        int shortage = order->quantity - sample->stock;
        if (sample->stock > 0)
        {
            sampleRepository_.ChangeStock(sample->id, -sample->stock);
        }
        productionController_.Enqueue(orderId, sample->id, shortage);
        orderRepository_.UpdateStatus(orderId, OrderStatus::Producing);
    }

    return true;
}

bool OrderController::RejectOrder(const std::string& orderId)
{
    auto order = orderRepository_.FindById(orderId);
    if (!order.has_value() || order->status != OrderStatus::Reserved)
    {
        return false;
    }

    return orderRepository_.UpdateStatus(orderId, OrderStatus::Rejected);
}
