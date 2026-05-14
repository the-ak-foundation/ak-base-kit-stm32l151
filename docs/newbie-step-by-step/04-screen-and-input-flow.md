# 04 - Screen and Input Flow

Target: Understand how button on project work.

## Summary Flow

BOOT -> MENU -> HOW TO PLAY -> DUNGEON -> GAME OVER -> LEADERBOARD

## Folder main for control screen

- [application/sources/app/screens](../../application/sources/app/screens)
- [application/sources/app/screen_manager.cpp](../../application/sources/app/screen_manager.cpp)

## Folder main for control input

- callback button on app-level
- event have send to task/screen handler

Doc handler at:

- [application/sources/app/screens/scr_menu_game.cpp](../../application/sources/app/screens/scr_menu_game.cpp)
- [application/sources/app/screens/scr_dungeon_game.cpp](../../application/sources/app/screens/scr_dungeon_game.cpp)

## Way to edit for safety

- Edit UI first to test pipeline
- Then Edit logic change screen
- Which each time change handler, build again immediately