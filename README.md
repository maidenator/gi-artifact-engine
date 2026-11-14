# gi-artifact-engine
A Standalone Genshin Impact Artifact Engine developed in C++

## Build and Run:
### Windows: g++ via MSYS2
```powershell
.\winscripts\build.ps1
.\winscripts\sanitycheck.ps1
```

### Linux: g++ via pacman
```bash
./linuxscripts/build.sh
./linuxscripts/sanitycheck.sh
```

## JSON format
Artifacts are exported in the [GOOD format](https://frzyc.github.io/genshin-optimizer/#/doc), so they can be imported into Genshin Optimizer and similar tools.

Each artifact has these required fields:

- `setKey`: artifact set name, such as `"GladiatorsFinale"`
- `slotKey`: `"flower"`, `"plume"`, `"sands"`, `"goblet"`, or `"circlet"`
- `level`: 0 to 20
- `rarity`: 5
- `mainStatKey`: main stat key, such as `"hp"` or `"critDMG_"`
- `location`: `""` (not equipped)
- `lock`: `false`
- `substats`: list of `{ "key", "value" }`

Example:
```json
{
  "setKey": "GladiatorsFinale",
  "slotKey": "plume",
  "level": 20,
  "rarity": 5,
  "mainStatKey": "atk",
  "location": "",
  "lock": false,
  "substats": [
    { "key": "critRate_", "value": 3.9 },
    { "key": "critDMG_", "value": 7.8 },
    { "key": "enerRech_", "value": 5.8 },
    { "key": "hp_", "value": 4.7 }
  ]
}
```

## CLI
The engine runs a simulation and prints the results. Run it from the project root after building.

```bash
./build/gi_artifact_engine [options]
```

On Windows, use `.\build\gi_artifact_engine.exe`.

| Option | Meaning | Default |
|---|---|---|
| `--seed N` | RNG seed, for reproducible runs | 42 |
| `--resin N` | Resin to spend, 20 per run (minimum 20) | 160 |
| `--topk N` | Number of artifacts to show (must be above 0) | 10 |
| `--metric NAME` | `cv`, `critrate`, `critdmg`, `er`, `em`, `atk`, `hp` | `cv` |
| `--set KEY` | GOOD set key | `GladiatorsFinale` |
| `--out FILE` | Write the shown artifacts as GOOD JSON | none |
| `--luck` | Show the 4-liner z-score | off |
| `--verbose` | Print each shown artifact as JSON | off |
| `--no-clear` | Don't clear the console at start | off |
| `--help` | Show the options | |

Examples:

```bash
./build/gi_artifact_engine --resin 16000 --topk 5 --luck
./build/gi_artifact_engine --seed 7 --resin 16000 --topk 5 --out top.json
```

Notes:

- Only `cv` scores are implemented so far. Other metrics currently score 0.
- Top-K selection isn't finished, so the artifacts shown are not yet the best by the chosen metric.