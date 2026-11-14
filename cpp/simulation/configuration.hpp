#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "../artifact/types.hpp"
#include "../artifact/random_utils.hpp"
#include "../artifact/distributions.hpp"

namespace configuration {
enum class Metric {
    CritValue,
    CritRate,
    CritDamage,
    EnergyRecharge,
    ElementalMastery,
    Atk,
    Hp,
};

struct SimulationConfig {
    uint64_t seed = 42;
    int resin = 160;  // 160 resin = 8 runs
    std::string setKey = "GladiatorsFinale"; // Default value

    int topK = 10;
    Metric metric = Metric::CritValue;

    bool computeLuck = false;
    bool verbose = false;

    int runs() const {
        return resin / distributions::RESIN_PER_RUN;
    }
};

struct SimulationResult {
    uint64_t seed = 0;
    int runs = 0;

    std::vector<Artifact> topArtifacts;
    std::vector<double> topScores;

    int totalArtifacts = 0;
    int fourLiners = 0;
};
} // namespace configuration