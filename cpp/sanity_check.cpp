#include "artifact/generator.hpp"
#include <iostream>
#include <set>

bool isValid(const Artifact &a) {
    // Must have 3 or 4 substats
    if (a.substatCount != 3 && a.substatCount != 4) return false;

    std::set<ArtifactSubstat> seen;
    for (size_t i = 0; i < a.substatCount; ++i) {
        // No duplicate substats
        if (!seen.insert(a.subStats[i].type).second) return false;

        // A substat can't match the main stat
        auto matching = distributions::mainStatToSubStat(a.mainStat.type);
        if (matching && *matching == a.subStats[i].type) return false;

        // Each substat must have at least one roll
        if (a.subStats[i].rolls < 1) return false;
    }
    return true;
}

int main() {
    rng::Xoshiro256 rng(12345);
    int fourLiners = 0;

    for (int i = 0; i < 100000; ++i) {
        Artifact a = generator::generateArtifact(rng);
        if (!isValid(a)) {
            std::cout << "Invalid artifact at generation " << i << "\n";
            return 1;
        }
        if (a.substatCount == 4) ++fourLiners;

        for (int u = 0; u < 5; ++u) {
            generator::upgradeArtifactOnce(a, rng);
            if (!isValid(a)) {
                std::cout << "Invalid after upgrade " << u + 1 << "\n";
                return 1;
            }
        }
        if (a.level != 20) {
            std::cout << "Level should be 20, got " << a.level << "\n";
            return 1;
        }
    }

    std::cout << "All rules passed\n";
    std::cout << "4-liner rate: " << fourLiners / 100000.0
              << " (expected " << distributions::FOUR_LINER_CHANCE << ")\n";
}