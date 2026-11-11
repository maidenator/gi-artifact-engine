cmake -G "MinGW Makefiles" -B build
cmake --build build --target sanity_check
# .\build\sanity_check.exe