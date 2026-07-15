#include "ProductionController.h"

#include <cmath>

ProductionController::ProductionController(ISampleRepository& sampleRepository, IOrderRepository& orderRepository)
    : sampleRepository_(sampleRepository)
    , orderRepository_(orderRepository)
{
}

void ProductionController::Enqueue(const std::string& orderId, const std::string& sampleId, int shortage)
{
    auto sample = sampleRepository_.FindById(sampleId);
    double yield = (sample.has_value() && sample->yield > 0.0) ? sample->yield : 1.0;

    ProductionJob job;
    job.orderId = orderId;
    job.sampleId = sampleId;
    job.shortage = shortage;
    job.actualQuantity = static_cast<int>(std::ceil(shortage / yield));

    queue_.push_back(job);
}

std::vector<ProductionJob> ProductionController::GetQueue() const
{
    return std::vector<ProductionJob>(queue_.begin(), queue_.end());
}

const ProductionJob* ProductionController::GetCurrentJob() const
{
    if (queue_.empty())
    {
        return nullptr;
    }
    return &queue_.front();
}

bool ProductionController::CompleteCurrentJob()
{
    if (queue_.empty())
    {
        return false;
    }

    ProductionJob job = queue_.front();
    queue_.pop_front();

    // 실 생산량 중 부족분(shortage)은 해당 주문으로 바로 소비되고,
    // 초과분(surplus)만 재고로 반영한다.
    int surplus = job.actualQuantity - job.shortage;
    if (surplus > 0)
    {
        sampleRepository_.ChangeStock(job.sampleId, surplus);
    }
    orderRepository_.UpdateStatus(job.orderId, OrderStatus::Confirmed);
    return true;
}
