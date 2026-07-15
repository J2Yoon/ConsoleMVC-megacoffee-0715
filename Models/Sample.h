#pragma once

#include <string>

// 시료(Sample) : 이 시스템의 가장 기본이 되는 단위
struct Sample
{
    std::string id;                              // 시료 ID (예: S-001)
    std::string name;                             // 시료명
    double avgProductionMinutesPerUnit = 0.0;     // 개당 평균 생산 시간(분)
    double yield = 1.0;                           // 수율 (정상 생산 시료 수 / 총 생산 시료 수)
    int stock = 0;                                // 현재 재고 수량
};
