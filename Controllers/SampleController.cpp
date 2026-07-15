#include "SampleController.h"

SampleController::SampleController(ISampleRepository& sampleRepository)
    : sampleRepository_(sampleRepository)
{
}

bool SampleController::RegisterSample(const std::string& id, const std::string& name,
    double avgProductionMinutesPerUnit, double yield)
{
    Sample sample;
    sample.id = id;
    sample.name = name;
    sample.avgProductionMinutesPerUnit = avgProductionMinutesPerUnit;
    sample.yield = yield;
    sample.stock = 0;

    return sampleRepository_.Add(sample);
}

std::vector<Sample> SampleController::GetAllSamples() const
{
    return sampleRepository_.GetAll();
}

std::vector<Sample> SampleController::SearchByName(const std::string& keyword) const
{
    return sampleRepository_.FindByNameContains(keyword);
}
