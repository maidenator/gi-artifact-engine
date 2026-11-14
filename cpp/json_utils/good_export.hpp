#include <nlohmann/json.hpp>
#include "enum_convert.hpp"
#include "../artifact/types.hpp"

using json = nlohmann::json;

namespace goodexport {
/**
 * Converts an artifact object into a json object using the GOOD format
 * 
 * @param art The artifact object to be exported into the GOOD format
 * @param setName GOOD setKey, such as "GladiatorsFinale"
 * @return nlohmann::json The GOOD-formatted artifact
 */
inline json artifactToGood(const Artifact &art, const std::string &setName) {
    json artifact;
    artifact["setKey"] = setName; // Must be compliant of the GOOD setKey format
    artifact["slotKey"] = good::slotKeyFor(art.slot);
    
    artifact["level"] = art.level;
    artifact["rarity"] = art.rarity;

    artifact["mainStatKey"] = good::statKeyFor(art.mainStat.type);

    artifact["substats"] = json::array();
    for(std::size_t i = 0; i < art.substatCount; ++i) {
        artifact["substats"].push_back({
            {"key", good::statKeyFor(art.subStats[i].type)},
            {"value", art.subStats[i].value}
        });
    }

    artifact["location"] = ""; // Generated artifacts should not be equipped
    artifact["lock"] = false; // Generated artifacts should not be locked

    return artifact;
}

/**
 * Same as above, using a placeholder set for testing.
 */
inline json artifactToGood(const Artifact &art) {
    return artifactToGood(art, "GladiatorsFinale");
}
} // namespace goodexport