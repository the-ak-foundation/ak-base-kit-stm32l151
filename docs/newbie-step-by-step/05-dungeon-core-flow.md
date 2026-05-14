# 05 - Dungeon Core Flow

Goal: understand the core gameplay loop in the dungeon.

## Central gameplay file

- [application/sources/app/screens/scr_dungeon_game.cpp](../../application/sources/app/screens/scr_dungeon_game.cpp)

## Game loop

1. Setup session (new / continue / level select)
2. Travel view
3. Chest / message trigger
4. Battle view
5. Stage advance
6. Save progress
7. Finish game

## Battle phases

- Hero lunge
- Hero hit
- Hero apply damage
- Monster lunge
- Monster hit
- Monster apply damage

Splitting into phases ensures animations and damage are applied in the correct order.

## Dungeon support modules

- [application/sources/app/game/dungeon_game/dungeon_control.cpp](../../application/sources/app/game/dungeon_game/dungeon_control.cpp)
- [application/sources/app/game/dungeon_game/dungeon_action.cpp](../../application/sources/app/game/dungeon_game/dungeon_action.cpp)
- [application/sources/app/game/dungeon_game/dungeon_effect.cpp](../../application/sources/app/game/dungeon_game/dungeon_effect.cpp)
- [application/sources/app/game/dungeon_game/dungeon_lane.cpp](../../application/sources/app/game/dungeon_game/dungeon_lane.cpp)
- [application/sources/app/game/dungeon_game/dungeon_state.cpp](../../application/sources/app/game/dungeon_game/dungeon_state.cpp)
