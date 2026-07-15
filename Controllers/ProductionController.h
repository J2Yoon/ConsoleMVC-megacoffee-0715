#pragma once

#include <deque>
#include <string>
#include <vector>

#include "../Models/ProductionJob.h"
#include "../Repositories/IOrderRepository.h"
#include "../Repositories/ISampleRepository.h"

// 생산 라인(FIFO 생산 큐) 기능을 담당하는 Controller.
//
// PoC 범위 안내: 실제 생산 소요 시간의 경과 반영(실시간 진행, 재시작 후 복원 등)은
// 본 스켈레톤에서 다루지 않으며, 이후 프로젝트 개발 단계에서 구현한다.
// 여기서는 큐 등록/조회와, 담당자가 수동으로 트리거하는 "생산 완료 처리" 동작까지만 제공한다.
class ProductionController
{
public:
    ProductionController(ISampleRepository& sampleRepository, IOrderRepository& orderRepository);

    // 부족분(shortage)만큼 실 생산량을 계산해 FIFO 큐에 작업을 등록한다.
    // 실 생산량 = ceil(shortage / 수율)
    void Enqueue(const std::string& orderId, const std::string& sampleId, int shortage);

    // 대기 중인 생산 큐를 FIFO 순서로 조회한다.
    std::vector<ProductionJob> GetQueue() const;

    // 현재 큐 맨 앞(처리 중)인 작업을 조회한다. 큐가 비어 있으면 nullptr.
    const ProductionJob* GetCurrentJob() const;

    // [PoC 스텁] 큐 맨 앞 작업을 완료 처리한다.
    // 실 생산량만큼 재고를 늘리고, 해당 주문 상태를 PRODUCING -> CONFIRMED로 전환한다.
    bool CompleteCurrentJob();

private:
    ISampleRepository& sampleRepository_;
    IOrderRepository& orderRepository_;
    std::deque<ProductionJob> queue_;
};
