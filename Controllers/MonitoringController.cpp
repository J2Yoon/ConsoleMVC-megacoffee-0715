#include "MonitoringController.h"

MonitoringController::MonitoringController(IOrderRepository& orderRepository, ISampleRepository& sampleRepository)
    : orderRepository_(orderRepository)
    , sampleRepository_(sampleRepository)
{
}

std::map<OrderStatus, int> MonitoringController::GetOrderCountsByStatus() const
{
    std::map<OrderStatus, int> counts;
    counts[OrderStatus::Reserved] = 0;
    counts[OrderStatus::Producing] = 0;
    counts[OrderStatus::Confirmed] = 0;
    counts[OrderStatus::Released] = 0;

    for (const auto& order : orderRepository_.GetAll())
    {
        if (order.status == OrderStatus::Rejected)
        {
            continue; // REJECTED는 모니터링에서 제외
        }
        counts[order.status]++;
    }

    return counts;
}

std::vector<StockStatus> MonitoringController::GetStockStatuses() const
{
    const auto orders = orderRepository_.GetAll();

    std::vector<StockStatus> result;
    for (const auto& sample : sampleRepository_.GetAll())
    {
        StockStatus status;
        status.sample = sample;

        int pendingDemand = 0;
        for (const auto& order : orders)
        {
            if (order.sampleId != sample.id)
            {
                continue;
            }
            if (order.status == OrderStatus::Reserved || order.status == OrderStatus::Producing
                || order.status == OrderStatus::Confirmed)
            {
                pendingDemand += order.quantity;
            }
        }
        status.pendingDemand = pendingDemand;

        if (sample.stock <= 0)
        {
            status.level = StockLevel::Depleted;
        }
        else if (sample.stock < pendingDemand)
        {
            status.level = StockLevel::Low;
        }
        else
        {
            status.level = StockLevel::Sufficient;
        }

        result.push_back(status);
    }

    return result;
}
