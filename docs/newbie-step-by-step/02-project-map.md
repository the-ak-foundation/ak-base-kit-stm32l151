# 02 - Project Map

Goal: understand how the project is structured.

## Top-level folders

- [application](../../application): Main firmware
- [boot](../../boot): Bootloader
- [hardware](../../hardware): Schematics and board assets
- [resources](../../resources): Images and other resources

## Inside application/

- [application/sources/app](../../application/sources/app): App core
- [application/sources/app/screens](../../application/sources/app/screens): All screens
- [application/sources/app/game/dungeon_game](../../application/sources/app/game/dungeon_game): Gameplay module
- [application/build_dungeon-game](../../application/build_dungeon-game): Build output

## Runtime entry files

- [application/sources/app/app.cpp](../../application/sources/app/app.cpp)
- [application/sources/app/task_list.cpp](../../application/sources/app/task_list.cpp)
