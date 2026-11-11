cmake_minimum_required(VERSION 3.14)
project(gi_artifact_engine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# nlohmann dependency:
include(FetchContent)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

# main program
add_executable(gi_artifact_engine
    cpp/main.cpp
)

# record Artifact Engine, CMake and compiler versions
target_compile_definitions(gi_artifact_engine PRIVATE
    GI_VERSION="${PROJECT_VERSION}"
    CMAKE_VERSION_USED="${CMAKE_VERSION}"
    COMPILER_VERSION_USED="${CMAKE_CXX_COMPILER_VERSION}"
)

# warnings
if(MSVC)
    target_compile_options(gi_artifact_engine PRIVATE /W4)
else()
    target_compile_options(gi_artifact_engine PRIVATE -Wall -Wextra -Wpedantic)
endif()

# #include "artifact/..."
target_include_directories(gi_artifact_engine PRIVATE ${CMAKE_SOURCE_DIR}/cpp)

# link nlohmann/json
target_link_libraries(gi_artifact_engine PRIVATE nlohmann_json::nlohmann_json)

# header check (not built by default)
add_library(check_headers STATIC EXCLUDE_FROM_ALL cpp/check_headers.cpp)
target_include_directories(check_headers PRIVATE ${CMAKE_SOURCE_DIR}/cpp)
target_link_libraries(check_headers PRIVATE nlohmann_json::nlohmann_json)
if(MSVC)
    target_compile_options(check_headers PRIVATE /W4)
else()
    target_compile_options(check_headers PRIVATE -Wall -Wextra -Wpedantic)
endif()