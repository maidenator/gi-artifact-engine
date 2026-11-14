#pragma  once
#include "artifact/random_utils.hpp"
#include "../artifact/generator.hpp"
#include "../artifact/random_utils.hpp"
#include "artifact/types.hpp"
#include "configuration.hpp"

namespace simulator {
    inline configuration::SimulationResult runSimulation(const configuration::SimulationConfig &cfg) {
        rng::Xoshiro256 rng (cfg.seed);
        configuration::SimulationResult result;

        result.seed = cfg.seed;
        result.runs = cfg.runs();
        
        for(int i = 0; i < cfg.runs(); ++i) {
            Artifact art = generator::generateArtifact(rng);
            if(art.substatCount == 4) 
                ++result.fourLiners;
            
            generator::upgradeToMax(art, rng);
            ++result.totalArtifacts;
            
            // TODO: score artifacts by cfg.metric and keep only the top K
            result.topArtifacts.emplace_back(art);
        }

        return result;
    }
} // namespace simulator