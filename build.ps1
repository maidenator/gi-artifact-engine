Remove-Item -Recurse -Force build -ErrorAction SilentlyContinue

cmake -G "MinGW Makefiles" -B build
if ($LASTEXITCODE -ne 0) { exit 1 }

cmake --build build
if ($LASTEXITCODE -ne 0) { exit 1 }

.\build\gi_artifact_engine.exe