#pragma once

#include <string>
#include <vector>

#include "../Models/Sample.h"
#include "../Repositories/ISampleRepository.h"

// 시료 관리(등록/조회/검색) 기능을 담당하는 Controller.
class SampleController
{
public:
    explicit SampleController(ISampleRepository& sampleRepository);

    // 새 시료를 등록한다. 이미 존재하는 ID면 false 반환.
    bool RegisterSample(const std::string& id, const std::string& name,
        double avgProductionMinutesPerUnit, double yield);

    // 등록된 모든 시료 목록(현재 재고 포함)을 조회한다.
    std::vector<Sample> GetAllSamples() const;

    // 이름에 keyword가 포함된 시료를 검색한다.
    std::vector<Sample> SearchByName(const std::string& keyword) const;

private:
    ISampleRepository& sampleRepository_;
};
