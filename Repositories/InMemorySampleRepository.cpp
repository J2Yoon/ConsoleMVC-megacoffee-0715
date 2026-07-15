#include "InMemorySampleRepository.h"

#include <algorithm>

bool InMemorySampleRepository::Add(const Sample& sample)
{
    if (FindById(sample.id).has_value())
    {
        return false; // 이미 등록된 시료 ID
    }

    samples_.push_back(sample);
    return true;
}

std::vector<Sample> InMemorySampleRepository::GetAll() const
{
    return samples_;
}

std::optional<Sample> InMemorySampleRepository::FindById(const std::string& sampleId) const
{
    auto it = std::find_if(samples_.begin(), samples_.end(),
        [&sampleId](const Sample& sample) { return sample.id == sampleId; });

    if (it == samples_.end())
    {
        return std::nullopt;
    }
    return *it;
}

std::vector<Sample> InMemorySampleRepository::FindByNameContains(const std::string& keyword) const
{
    std::vector<Sample> result;
    for (const auto& sample : samples_)
    {
        if (sample.name.find(keyword) != std::string::npos)
        {
            result.push_back(sample);
        }
    }
    return result;
}

bool InMemorySampleRepository::ChangeStock(const std::string& sampleId, int delta)
{
    auto it = std::find_if(samples_.begin(), samples_.end(),
        [&sampleId](const Sample& sample) { return sample.id == sampleId; });

    if (it == samples_.end())
    {
        return false;
    }

    it->stock += delta;
    return true;
}
