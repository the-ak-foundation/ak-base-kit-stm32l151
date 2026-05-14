# 03 - First Successful Build
Target: Build project and it's passed at the first try.

## Build firmware

```bash
cd application
make -j4
```

Artifact build on [application/build_dungeon-game](../../application/build_dungeon-game).

## If it's failed

1. Check toolchain installed correctly
2. Check are you stand on correct application
3. Check file edit had any typo/include incorrect
