#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <nlohmann/json.hpp>

#include "simulation/simulator.hpp"
#include "json_utils/good_export.hpp"
#include "artifact/distributions.hpp"

// ---------- Console helpers ----------

void clearConsole() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

// Runs until the jthread is asked to stop
void spinner(std::stop_token stop) {
    const char *frames[] = {"|", "/", "-", "\\"};
    for (int i = 0; !stop.stop_requested(); ++i) {
        std::cout << "\rRunning " << frames[i % 4] << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
    }
    std::cout << "\r" << std::string(20, ' ') << "\r" << std::flush;
}

// ---------- Metric names ----------

struct MetricName {
    std::string_view name;
    configuration::Metric metric;
};

constexpr MetricName METRICS[] = {
    {"cv",       configuration::Metric::CritValue},
    {"critrate", configuration::Metric::CritRate},
    {"critdmg",  configuration::Metric::CritDamage},
    {"er",       configuration::Metric::EnergyRecharge},
    {"em",       configuration::Metric::ElementalMastery},
    {"atk",      configuration::Metric::Atk},
    {"hp",       configuration::Metric::Hp},
};

std::string_view metricName(configuration::Metric m) {
    for (const auto &entry : METRICS)
        if (entry.metric == m) return entry.name;
    return "unknown";
}

bool parseMetric(std::string_view s, configuration::Metric &out) {
    for (const auto &entry : METRICS) {
        if (entry.name == s) {
            out = entry.metric;
            return true;
        }
    }
    return false;
}

// ---------- Usage ----------

void printUsage() {
    std::cout << "Usage: gi_artifact_engine [options]\n\n"
              << "  --seed N        RNG seed (default 42)\n"
              << "  --resin N       Resin to spend, 20 per run (default 160)\n"
              << "  --topk N        Number of artifacts to show (default 10)\n"
              << "  --metric NAME   cv, critrate, critdmg, er, em, atk, hp (default cv)\n"
              << "  --set KEY       GOOD set key (default GladiatorsFinale)\n"
              << "  --out FILE      Write the artifacts as GOOD JSON\n"
              << "  --luck          Show the 4-liner z-score\n"
              << "  --verbose       Print each artifact as JSON\n"
              << "  --no-clear      Don't clear the console at start\n"
              << "  --help          Show this message\n";
}

void printConfig(const configuration::SimulationConfig &cfg) {
    std::cout << "GI Artifact Engine v" << GI_VERSION << "\n";
    std::cout << "=========================\n";
    std::cout << "Seed:    " << cfg.seed << "\n";
    std::cout << "Resin:   " << cfg.resin << " (" << cfg.runs() << " runs)\n";
    std::cout << "Set:     " << cfg.setKey << "\n";
    std::cout << "Top K:   " << cfg.topK << " by " << metricName(cfg.metric) << "\n\n";
}

// ---------- Main ----------

int main(int argc, char *argv[]) {
    configuration::SimulationConfig cfg;
    std::string outPath;
    bool verbose = false;
    bool noClear = false;

    try {
        for (int i = 1; i < argc; ++i) {
            std::string_view arg = argv[i];

            // Returns the next argument as the value for the current option
            auto value = [&]() -> std::string {
                if (i + 1 >= argc)
                    throw std::invalid_argument(std::string(arg) + " needs a value");
                return argv[++i];
            };

            if (arg == "--help" || arg == "-h") {
                printUsage();
                return 0;
            } else if (arg == "--seed") {
                cfg.seed = std::stoull(value());
            } else if (arg == "--resin") {
                cfg.resin = std::stoi(value());
            } else if (arg == "--topk") {
                cfg.topK = std::stoi(value());
            } else if (arg == "--set") {
                cfg.setKey = value();
            } else if (arg == "--metric") {
                std::string name = value();
                if (!parseMetric(name, cfg.metric))
                    throw std::invalid_argument("unknown metric: " + name);
            } else if (arg == "--out") {
                outPath = value();
            } else if (arg == "--luck") {
                cfg.computeLuck = true;
            } else if (arg == "--verbose") {
                verbose = true;
            } else if (arg == "--no-clear") {
                noClear = true;
            } else {
                throw std::invalid_argument("unknown option: " + std::string(arg));
            }
        }

        if (cfg.topK <= 0)
            throw std::invalid_argument("--topk must be greater than 0");
        if (cfg.resin < distributions::RESIN_PER_RUN)
            throw std::invalid_argument("--resin must be at least " +
                                        std::to_string(distributions::RESIN_PER_RUN));
        if (cfg.setKey.empty())
            throw std::invalid_argument("--set cannot be empty");
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n\n";
        printUsage();
        return 1;
    }

    if (!noClear) clearConsole();
    printConfig(cfg);

    configuration::SimulationResult result;
    auto start = std::chrono::steady_clock::now();
    {
        std::jthread spin(spinner);
        result = simulator::runSimulation(cfg);
    }
    double elapsed = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();

    // Summary
    const double n = result.totalArtifacts;
    const double k = result.fourLiners;
    const double p = distributions::FOUR_LINER_CHANCE;

    std::cout << "Simulated " << result.totalArtifacts << " artifacts in "
              << std::fixed << std::setprecision(3) << elapsed << " s\n";
    std::cout << "4-liner rate: " << std::setprecision(4) << k / n
              << " (expected " << p << ")\n";

    if (cfg.computeLuck) {
        const double expected = n * p;
        const double sd = std::sqrt(n * p * (1.0 - p));
        std::cout << "4-liner z-score: " << std::setprecision(2) << (k - expected) / sd << "\n";
    }

    // Show at most topK artifacts from the list the simulation returned
    const std::size_t shown = std::min(result.topArtifacts.size(),
                                       static_cast<std::size_t>(cfg.topK));

    std::cout << "\nShowing " << shown << " artifacts:\n";
    for (std::size_t i = 0; i < shown; ++i) {
        const Artifact &art = result.topArtifacts[i];
        std::cout << std::setw(3) << i + 1 << ". "
                  << good::slotKeyFor(art.slot) << " "
                  << good::statKeyFor(art.mainStat.type) << "  |";

        for (std::size_t j = 0; j < art.substatCount; ++j) {
            std::cout << " " << good::statKeyFor(art.subStats[j].type)
                      << "=" << art.subStats[j].value;
        }
        std::cout << "\n";

        if (verbose) {
            std::cout << goodexport::artifactToGood(art, cfg.setKey).dump(2) << "\n";
        }
    }

    // Optional file output
    if (!outPath.empty()) {
        nlohmann::json out = nlohmann::json::array();
        for (std::size_t i = 0; i < shown; ++i)
            out.push_back(goodexport::artifactToGood(result.topArtifacts[i], cfg.setKey));

        std::ofstream file(outPath);
        if (!file) {
            std::cerr << "Could not open " << outPath << "\n";
            return 1;
        }
        file << out.dump(2) << "\n";
        std::cout << "\nWrote " << out.size() << " artifacts to " << outPath << "\n";
    }

    return 0;
}