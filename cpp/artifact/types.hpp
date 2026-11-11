#pragma once
#include <string>
#include <array>
#include <vector>
#include <optional>

enum class ArtifactSlot {
    flower,
    feather,
    sands,
    goblet,
    circlet,
};

enum class ArtifactSubstat {
    critDmg,
    critRate,
    elementalMastery,
    energyRecharge,
    atkPercent,
    atkFlat,
    hpPercent,
    hpFlat,
    defPercent,
    defFlat,
};

enum class ArtifactMainStat {
    // Artifact Substats are valid Main Stats
    critDmg,
    critRate,
    elementalMastery,
    energyRecharge,
    atkPercent,
    atkFlat,
    hpPercent,
    hpFlat,
    defPercent,
    defFlat,

    // Main stats that can't appear as Substats
    healingBonus,
    pyroDmg,
    hydroDmg,
    electroDmg,
    cryoDmg,
    anemoDmg,
    geoDmg,
    dendroDmg,
    physicalDmg,
};

struct SubstatRoll {
    ArtifactSubstat type;
    double value = 0.0;
    int rolls = 1;
};

struct MainStat {
    ArtifactMainStat type;
    double value = 0.0;
};

struct Artifact {
    std::string id;
    std::string setKey; // Supplied by the front end, not used during generation.
    ArtifactSlot slot;

    int rarity;
    int level = 0;

    MainStat mainStat;
    std::array<SubstatRoll, 4> subStats;
    size_t substatCount = 0;
};