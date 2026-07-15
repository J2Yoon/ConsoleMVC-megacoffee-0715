#pragma once

#include <optional>
#include <string>
#include <vector>

#include "../Models/Sample.h"

// 시료 데이터 접근을 담당하는 Repository 인터페이스.
// PoC 단계에서는 인메모리 구현체만 두고, 추후 별도 DataPersistence PoC의 결과물로 교체한다.
class ISampleRepository
{
public:
    virtual ~ISampleRepository() = default;

    virtual bool Add(const Sample& sample) = 0;
    virtual std::vector<Sample> GetAll() const = 0;
    virtual std::optional<Sample> FindById(const std::string& sampleId) const = 0;
    virtual std::vector<Sample> FindByNameContains(const std::string& keyword) const = 0;

    // 재고 수량을 delta(증감분)만큼 변경한다.
    virtual bool ChangeStock(const std::string& sampleId, int delta) = 0;
};
