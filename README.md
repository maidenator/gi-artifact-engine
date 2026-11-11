# gi-artifact-engine
A Standalone Genshin Impact Artifact Engine developed in C++

## Build and Run:
(These currently do nothing as im still working on the CLI, I'm not even sure if this should be a CLI) 
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