# Dungeon Loop — Embedded Game Runtime Roadmap

# 1. Project Goal

Build a modular bare-metal embedded game runtime on:

- STM32L151
- SSD1306 OLED
- Winbond W25Q80 SPI Flash

Main Features:

- Bootloader
- External Flash Partitions
- Save/Load System
- Metadata System
- Multiple Game Support
- Turn-Based Dungeon Gameplay
- Event-Driven Architecture

---

# 2. Recommended Folder Structure

```txt
Core/
│
├── bootloader/
├── runtime/
├── drivers/
├── system/
├── ui/
├── api/
├── save/
├── metadata/
├── assets/
│
Games/
│
├── dungeon_loop/
│   ├── game.c
│   ├── battle.c
│   ├── event.c
│   ├── inventory.c
│   ├── map.c
│   ├── render.c
│   ├── save.c
│   └── assets/
│
└── snake/
```

---

# 3. External Flash Partition Layout
```txt
0x000000 - Bootloader Metadata
0x001000 - Game Metadata Table
0x010000 - Game Binary A
0x030000 - Game Binary B
0x050000 - Save Data
0x070000 - Assets
```

---

# 4. STEP-BY-STEP DEVELOPMENT PLAN

# STEP 1 — SSD1306 Display Driver

Goal:
- Display text
- Draw bitmap
- Build menu system

Files:

```txt
drivers/ssd1306.c
drivers/ssd1306.h
```

Basic API:

```c
void SSD1306_Init(void);
void SSD1306_Clear(void);
void SSD1306_UpdateScreen(void);
void SSD1306_DrawString(uint8_t x, uint8_t y, char* str);
void SSD1306_DrawBitmap(uint8_t x, uint8_t y, const uint8_t* bitmap);
```

Example:

```c
SSD1306_Clear();
SSD1306_DrawString(0,0,"Dungeon Loop");
SSD1306_UpdateScreen();
```

---

# STEP 2 — Button Input System

Goal:
- Read navigation buttons
- Menu control

Buttons:
- UP
- DOWN
- SELECT
- BACK

Files:

```txt
drivers/input.c
drivers/input.h
```

Example:

```c
typedef enum
{
    BTN_NONE,
    BTN_UP,
    BTN_DOWN,
    BTN_SELECT,
    BTN_BACK
} Button_t;
```

Read Input:

```c
Button_t Input_Read(void);
```

---

# STEP 3 — Main Menu System

Goal:
- Continue
- New Game
- Settings

Files:

```txt
ui/menu.c
ui/menu.h
```

Menu State:

```c
typedef enum
{
    MENU_MAIN,
    MENU_SETTINGS,
    MENU_PAUSE
} MenuState_t;
```

Main Loop:

```c
while(1)
{
    Input_Update();
    Menu_Update();
    Render_Update();
}
```

---

# STEP 4 — W25Q80 Flash Driver

Goal:
- Read/Write external flash

Files:

```txt
drivers/w25q80.c
drivers/w25q80.h
```

Functions:

```c
void W25Q80_Init(void);

void W25Q80_Read(
    uint32_t addr,
    uint8_t* buffer,
    uint32_t size
);

void W25Q80_Write(
    uint32_t addr,
    uint8_t* buffer,
    uint32_t size
);
```

---

# STEP 5 — Metadata System

Goal:
- Detect installed games
- Read game information

Files:

```txt
metadata/game_metadata.c
metadata/game_metadata.h
```

Metadata Structure:

```c
typedef struct
{
    char name[32];

    uint32_t gameAddr;
    uint32_t gameSize;

    uint32_t saveAddr;

    uint32_t iconAddr;

} GameMetadata_t;
```

Example:

```c
GameMetadata_t meta;
W25Q80_Read(0x001000, (uint8_t*)&meta, sizeof(meta));
```

---

# STEP 6 — Save System

Goal:
- Save progress
- Continue game

Files:

```txt
save/save.c
save/save.h
```

Save Structure:

```c
typedef struct
{
    uint8_t level;
    uint8_t stage;

    uint16_t hp;
    uint16_t atk;
    uint16_t def;

    uint32_t score;

    uint8_t inventory[8];

} SaveData_t;
```

Save Example:

```c
W25Q80_Write(
    SAVE_ADDR,
    (uint8_t*)&saveData,
    sizeof(saveData)
);
```

---

# STEP 7 — Game State Machine

Goal:
- Control gameplay flow

Files:

```txt
runtime/game_state.c
runtime/game_state.h
```

States:

```c
typedef enum
{
    GAME_MENU,
    GAME_MAP,
    GAME_EVENT,
    GAME_BATTLE,
    GAME_REWARD,
    GAME_PAUSE,
    GAME_OVER
} GameState_t;
```

Update:

```c
void Game_Update(void)
{
    switch(gameState)
    {
        case GAME_MENU:
            Menu_Update();
            break;

        case GAME_MAP:
            Map_Update();
            break;

        case GAME_BATTLE:
            Battle_Update();
            break;
    }
}
```

---

# STEP 8 — Event System

Goal:
- Trigger random encounters

Files:

```txt
game/event.c
game/event.h
```

Event Types:

```c
typedef enum
{
    EVENT_MONSTER,
    EVENT_ITEM,
    EVENT_TRAP,
    EVENT_HEAL,
    EVENT_BOSS
} EventType_t;
```

Random Event:

```c
EventType_t Event_Generate(void)
{
    return rand() % 5;
}
```

---

# STEP 9 — Monster System

Goal:
- Define enemy data

Files:

```txt
game/monster.c
game/monster.h
```

Monster Structure:

```c
typedef struct
{
    char name[32];

    uint16_t hp;
    uint16_t dmg;

    uint8_t specialAbility;

} Monster_t;
```

Example:

```c
Monster_t slime =
{
    "Slime",
    30,
    5,
    ABILITY_HEAL
};
```

---

# STEP 10 — Item System

Goal:
- Inventory
- Effects

Files:

```txt
game/item.c
game/item.h
```

Item Structure:

```c
typedef struct
{
    char name[32];

    uint8_t effectType;

    int16_t value;

} Item_t;
```

Example:

```c
Item_t sword =
{
    "Sword",
    EFFECT_ATTACK,
    5
};
```

---

# STEP 11 — Battle System

Goal:
- Turn-based combat

Files:

```txt
game/battle.c
game/battle.h
```

Battle Actions:

```c
typedef enum
{
    ACTION_ATTACK,
    ACTION_ITEM,
    ACTION_DEFEND,
    ACTION_SKILL,
    ACTION_ESCAPE
} BattleAction_t;
```

Damage:

```c
damage =
    player.atk -
    enemy.def;
```

Battle Flow:

```txt
Player Turn
↓
Enemy Turn
↓
Status Update
↓
Next Turn
```

---

# STEP 12 — Status Effect System

Goal:
- Poison
- Burn
- Curse

Structure:

```c
typedef struct
{
    uint8_t poison;
    uint8_t burn;
    uint8_t curse;

} StatusEffect_t;
```

Update:

```c
void Status_Update(void)
{
    if(status.poison)
    {
        player.hp -= 5;
    }
}
```

---

# STEP 13 — Node Map System

Goal:
- Lightweight exploration

Map Example:

```txt
START
  |
MONSTER
 /   \
ITEM TRAP
 \   /
BOSS
```

Files:

```txt
game/map.c
game/map.h
```

Node:

```c
typedef struct
{
    uint8_t eventType;

    uint8_t nextA;
    uint8_t nextB;

} MapNode_t;
```

---

# STEP 14 — Bitmap Asset System

Goal:
- Render icons

Folder:

```txt
assets/
```

Recommended Assets:

```txt
slime.png
goblin.png
wolf.png
godzilla.png
dragon.png
eye_watcher.png

sword.png
shield.png
healing.png
poison.png
bomb.png
```

Convert PNG to Bitmap:

```txt
Use:
- image2cpp
- LCD Assistant
```

Generated:

```c
const uint8_t slime_bitmap[] = {
};
```

---

# STEP 15 — Pause Menu

Goal:
- Save
- Resume
- Exit

Pause Actions:

```txt
Resume
Save Game
Exit
```

---

# STEP 16 — Continue System

Goal:
- Load previous progress

Example:

```c
W25Q80_Read(
    SAVE_ADDR,
    (uint8_t*)&saveData,
    sizeof(saveData)
);
```

---

# STEP 17 — Bootloader System

Goal:
- Load game binaries

Flow:

```txt
System Boot
↓
Read Metadata
↓
Find Game Binary
↓
Jump To Game
```

Jump Function:

```c
typedef void (*GameEntry_t)(void);

GameEntry_t game;

game = (GameEntry_t)(GAME_ADDR);

game();
```

---

# STEP 18 — Runtime API

Goal:
- Shared API between loader and game

Files:

```txt
api/game_api.h
```

API:

```c
typedef struct
{
    void (*DrawText)(uint8_t x, uint8_t y, char* str);

    void (*DrawBitmap)(
        uint8_t x,
        uint8_t y,
        const uint8_t* bmp
    );

    uint32_t (*GetTick)(void);

} EngineAPI_t;
```

---

# STEP 19 — Main Game Loop

```c
while(1)
{
    Input_Update();

    Event_Update();

    Game_Update();

    Render_Update();
}
```

---

# STEP 20 — Recommended Development Order

```txt
1. SSD1306
2. Input
3. Menu
4. W25Q80
5. Save System
6. Event System
7. Battle System
8. Inventory
9. Map System
10. Pause Menu
11. Metadata
12. Multi-game Loader
13. Bootloader
```

---

# FINAL GOAL

Build a:

"Modular Bare-Metal Embedded Game Runtime"

instead of only:

"Simple STM32 Game"

