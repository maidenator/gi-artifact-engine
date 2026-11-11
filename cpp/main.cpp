#include <iostream>
#include <nlohmann/json.hpp>
#include <cstdlib>  // for std::system

void clearConsole() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}


int main() {
    clearConsole();
    std::cout << "GI-Artifact-Engine v" << GI_VERSION << "\n";
    std::cout << "=========================\n\n";
    std::cout << "C++ standard: " << __cplusplus << "\n";

    #if defined(__GNUC__) && !defined(__clang__)
        std::cout << "Compiler: g++ " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__ << "\n";
    #elif defined(__clang__)
        std::cout << "Compiler: clang++ " << __clang_major__ << "." << __clang_minor__ << "\n";
    #elif defined(_MSC_VER)
        std::cout << "Compiler: MSVC " << _MSC_VER << "\n";
    #endif

    std::cout << "CMake: " << CMAKE_VERSION_USED << "\n";
    std::cout << "Compiler (from CMake): " << COMPILER_VERSION_USED << "\n";

    std::cout << "nlohmann/json: "
              << NLOHMANN_JSON_VERSION_MAJOR << "."
              << NLOHMANN_JSON_VERSION_MINOR << "."
              << NLOHMANN_JSON_VERSION_PATCH << "\n";

    return 0;
}