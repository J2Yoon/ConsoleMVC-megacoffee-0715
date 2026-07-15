#pragma once

#include <vector>

#include "ISampleRepository.h"

// ISampleRepository의 인메모리(std::vector 기반) 구현체.
class InMemorySampleRepository : public ISampleRepository
{
public:
    bool Add(const Sample& sample) override;
    std::vector<Sample> GetAll() const override;
    std::optional<Sample> FindById(const std::string& sampleId) const override;
    std::vector<Sample> FindByNameContains(const std::string& keyword) const override;
    bool ChangeStock(const std::string& sampleId, int delta) override;

private:
    std::vector<Sample> samples_;
};
