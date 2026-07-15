#pragma once

#include <string>

// 생산 작업(ProductionJob) : 재고 부족으로 생산 라인에 등록된 작업 단위 (FIFO 큐로 관리)
struct ProductionJob
{
    std::string orderId;       // 대상 주문번호
    std::string sampleId;      // 생산 대상 시료 ID
    int shortage = 0;          // 부족분 (주문 수량 - 당시 재고)
    int actualQuantity = 0;    // 실 생산량 = ceil(부족분 / 수율)
};
