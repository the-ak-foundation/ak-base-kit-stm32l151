# 06 - Test, Flash, Debug Checklist

Goal: have a clear checklist before flashing to the board and merging.

## Build checklist

- [ ] `make -j4` passes
- [ ] No new serious warnings
- [ ] Menu -> dungeon -> game over flow still works

## Flash the board

From [application](../../application):

```bash
st-flash --reset write build_dungeon-game/dungeon-game.bin 0x08003000
```

## Runtime checklist on board

- [ ] MODE/UP/DOWN buttons respond correctly
- [ ] OLED renders stably
- [ ] Buzzer respects the silent setting
- [ ] Save/load works correctly after reboot

## Debugging common issues

- Screen transition bug: check the menu handler first
- Battle bug: check battle phases and tick timing
- Save bug: check save structure and default fallback

## Before pushing

- [ ] Build passes
- [ ] Run at least one full game loop
- [ ] Write a short changelog in the PR
