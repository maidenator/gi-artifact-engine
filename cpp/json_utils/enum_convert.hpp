#pragma once
#include "../artifact/types.hpp"
#include <string_view>

namespace good {
    constexpr std::string_view statKeyFor(ArtifactMainStat stat) {
        switch(stat) {
            case ArtifactMainStat::hpFlat:          return "hp"; 
            case ArtifactMainStat::hpPercent:       return "hp_";

            case ArtifactMainStat::atkFlat:         return "atk";
            case ArtifactMainStat::atkPercent:      return "atk_";

            case ArtifactMainStat::defFlat:         return "def";
            case ArtifactMainStat::defPercent:      return "def_";

            case ArtifactMainStat::elementalMastery:return "eleMas";
            case ArtifactMainStat::energyRecharge:  return "enerRech_";
            case ArtifactMainStat::healingBonus:    return "heal_";

            case ArtifactMainStat::critRate:        return "critRate_";
            case ArtifactMainStat::critDmg:         return "critDMG_";

            case ArtifactMainStat::physicalDmg:     return "physical_dmg_";
            case ArtifactMainStat::anemoDmg:        return "anemo_dmg_";
            case ArtifactMainStat::geoDmg:          return "geo_dmg_";
            case ArtifactMainStat::electroDmg:      return "electro_dmg_";
            case ArtifactMainStat::hydroDmg:        return "hydro_dmg_";
            case ArtifactMainStat::pyroDmg:         return "pyro_dmg_";
            case ArtifactMainStat::cryoDmg:         return "cryo_dmg_";
            case ArtifactMainStat::dendroDmg:       return "dendro_dmg_";
        }
        return "";
    }

    constexpr std::string_view statKeyFor(ArtifactSubstat stat) {
        switch(stat) {
            case ArtifactSubstat::hpFlat:           return "hp"; 
            case ArtifactSubstat::hpPercent:        return "hp_";
            
            case ArtifactSubstat::atkFlat:          return "atk";
            case ArtifactSubstat::atkPercent:       return "atk_";

            case ArtifactSubstat::defFlat:          return "def";
            case ArtifactSubstat::defPercent:       return "def_";

            case ArtifactSubstat::elementalMastery: return "eleMas";
            case ArtifactSubstat::energyRecharge:   return "enerRech_";

            case ArtifactSubstat::critRate:         return "critRate_";
            case ArtifactSubstat::critDmg:          return "critDMG_";
        }
        return "";
    }

    constexpr std::string_view slotKeyFor(ArtifactSlot slot) {
    switch (slot) {
        case ArtifactSlot::flower:                  return "flower";
        case ArtifactSlot::feather:                 return "plume";
        case ArtifactSlot::sands:                   return "sands";
        case ArtifactSlot::goblet:                  return "goblet";
        case ArtifactSlot::circlet:                 return "circlet";
    }
    return "";
}
static_assert(statKeyFor(ArtifactMainStat::critDmg) == "critDMG_");
static_assert(statKeyFor(ArtifactSubstat::critDmg) == "critDMG_");
static_assert(slotKeyFor(ArtifactSlot::feather) == "plume");
} // namespace good