# Dungeon Loop - Embedded Turn-Based Adventure

## Overview

Dungeon Loop is a lightweight turn-based adventure game designed for embedded systems using:

* STM32L151 MCU
* SSD1306 OLED Display
* Winbond W25Q80 External Flash

The game focuses on:

* modular architecture
* embedded runtime systems
* save/load mechanics
* event-driven gameplay
* low-resource rendering

The player travels through dangerous forest paths while collecting items, fighting monsters, and surviving through multiple stages.

---

# Gameplay Flow

# Dungeon Loop - Embedded Turn-Based Adventure

## Overview

Dungeon Loop is a lightweight turn-based adventure game designed for embedded systems using:

* STM32L151 MCU
* SSD1306 OLED Display
* Winbond W25Q80 External Flash

The game focuses on:

* modular architecture
* embedded runtime systems
* save/load mechanics
* event-driven gameplay
* low-resource rendering

The player travels through dangerous forest paths while collecting items, fighting monsters, and surviving through multiple stages.

---

# Gameplay Flow

```text
BOOT
 ↓
MAIN MENU
 ↓
SELECT LEVEL
 ↓
START STAGE
 ↓
AUTO MOVE
 ↓
EVENT TRIGGER
 ↓
BATTLE / ITEM / TRAP
 ↓
REWARD
 ↓
NEXT STAGE
 ↓
LEVEL COMPLETE
 ↓
SAVE GAME
```

---

# Main Menu

```text
> Continue
  New Game
  Select Level
  Settings
```

---

# Pause Menu

```text
> Resume
  Save Game
  Exit To Menu
```

---

# Core Gameplay

The player automatically travels through a forest path.

During the journey, random events may occur:

* Monster Encounter 👾
* Mystery Chest ❓
* Trap 💀
* Healing Shrine ❤️
* Boss Gate 🐉

The player must survive through all stages in a level.

---

# Level System

| Level | Mode | Stages | Number of Monster |
| :--- | :---: | :---: | :---: |
| 1 | Easy | 4 | 1 Slime - 1 Goblin - 1 Wolf - 1 Godzilla |
| 2 | Easy | 5 | 2 Slime - 1 Goblin - 1 Wolf - 1 Godzilla |
| 3 | Medium | 6 | 1 Slime - 2 Goblin - 1 Wolf - 1 Godzilla - 1 Dragon |
| 4 | Medium | 7 | 1 Slime - 2 Goblin - 2 Wolf - 1 Godzilla - 1 Dragon |
| 5 | Hard | 8 | 1 Slime - 2 Goblin - 1 Wolf - 2 Godzilla - 1 Dragon - 1 Eye Watcher |

---

# Player Stats

| Stat          | Description           |
| ------------- | --------------------- |
| HP            | Player Health         |
| ATK           | Attack Damage         |
| DEF           | Armor / Defense       |
| Inventory     | Item Storage          |
| Status Effect | Poison / Burn / Curse |

---

# Mystery Event System

When the player encounters a mystery event:

```text
You found a mysterious chest...
Choose one item:
```

Example:

```text
1. Sword (+5 ATK)
2. Shield (+5 DEF)
3. Healing Potion (+5 HP)
```

The player can only choose one item.

---

# Item System

| Item | Description | Base Effect | Level Scaling | Level Spawn |
| :--- | :---: |  :---: | :---: | :---: | 
| Poison | Deals damage over time to enemies | 5 Damage | +2 Damage / Level | 1 |
| Antidote | Cures all active poison status effects | Instant Cure | N/A | 1 |
| Purify | Removes Curse and restores 100% Potion healing| Bypass Reduces | N/A | 5 |
| Healing | Restores HP to the player (HP) | +5 HP | +2 HP / Level | 1 |
| Sword | Increases player Attack power | +5 Attack | +3 Attack / Level | 1 |
| Shield | Increases player Armor/Defense | +5 Armor | +4 Armor / Level | 1 |
| Bomb | Shreds and reduces enemy Armor | -3 Armor | +2 Armor Reduction / Level | 1 |

---

# Trap Event

Example:

```text
A hidden trap was triggered!
-10 HP
```

Or:

```text
You are poisoned!
Poison damage activated.
```

---

# Monster Encounter

When encountering a monster:

| Name | HP | DMG | Special Ability | Activation Turns | Scaling | Turn to Kill | Level Spawn |
| :--- | :---: | :---:| :---: | :---: | :---: | :---: | :---: |
| Slime | 30 | 5 | Healing (+5 HP) | 2, 4, 8, 12...  | +3 HP / Level | 10 (+2 turn/level) | 1 -> 5 |
| Goblin | 50 | 10 | Poison (5 DMG) | 2, 5, 8, 11... | +2 DMG / Level | 15 (+5 turn/level) | 1 -> 5 |
| Wolf | 70 | 30 | Dodge (100% Miss) | 3, 7, 11, 15... | N/A | 20 (+5 turn/level) | 1 -> 5 |
| Gorilla | 90 | 40 | Armor (+5 DEF) | 2, 5, 8, 11... | +5 DEF / Level | 25 (+5 turn/level) | 1 -> 5 |
| Dragon | 150 | 50 | Burn (10 Instant DMG + 5 Burn DMG Over Time (DOT)) | 3, 7, 11, 15... | +7 DEF / Level | 30 (+5 turn/level) | 3 -> 5 |
| Eye Watcher | 200 | 60 | Reduces Healing Potion effectiveness by 50% (-50% Healing Potion) | 3, 6, 9, 12... | N/A | 40 (+5 turn/level) | 5 -> 5 |

```text
A wild Goblin appeared!
```

The game enters battle mode.

---

# Battle System

## Battle Screen

```text
PLAYER
HP: 35
ATK: 12
DEF: 5

VS

GOBLIN
HP: 50
DMG: 10
```

---

# Player Actions

The player has 5 main actions:

| Action | Description            |
| ------ | ---------------------- |
| Attack | Standard attack        |
| Item   | Use item               |
| Defend | Reduce incoming damage |
| Skill  | Use special ability    |
| Escape | Attempt to flee        |

---

# Battle Flow

```text
Player Turn
 ↓
Enemy Turn
 ↓
Status Effect Update
 ↓
Next Turn
```

---

# Monster System

## Slime

Ability:

* Regenerates HP periodically

```text
Slime regenerates HP!
+5 HP
```

---

## Goblin

Ability:

* Applies poison

```text
Goblin poisoned the player!
```

---

## Wolf

Ability:

* Dodge attack

```text
Wolf dodged the attack!
MISS
```

---

## Godzilla

Ability:

* Increase armor

```text
Godzilla hardened its armor!
+5 DEF
```

---

## Dragon

Ability:

* Burn damage

```text
Dragon breathes fire!
Burn activated.
```

---

## Eye Watcher

Ability:

* Reduce healing effectiveness

```text
Eye Watcher cursed the player!
Healing reduced by 50%.
```

---

# Status Effects

| Effect      | Description          |
| ----------- | -------------------- |
| Poison      | Damage every turn    |
| Burn        | Instant + DOT damage |
| Curse       | Reduce healing       |
| Armor Break | Reduce DEF           |

---

# Win Condition

The player wins by defeating all monsters in the final stage.

```text
LEVEL COMPLETE
```

---

# Lose Condition

The player loses when HP reaches zero.

```text
GAME OVER
```

---

# Save System

The game stores:

* Current Level
* Current Stage
* Player HP
* Inventory
* Status Effects
* Score

Save data is stored inside External Flash memory.

---

# Continue System

When selecting Continue:

```text
Loading Save Data...
Resume Adventure
```

The game restores:

* player progress
* inventory
* stage state

---

# Embedded Design Philosophy

This project is designed specifically for embedded hardware limitations.

Focus Areas:

* low memory usage
* low rendering overhead
* event-driven architecture
* modular game runtime
* bare-metal execution

Avoided Features:

* heavy animation
* realtime physics
* high FPS rendering
* complex graphics pipeline

---

# Embedded Architecture Goals

The project demonstrates:

* External Flash management
* Bootloader system
* Metadata handling
* Save/Load partition
* Dynamic game loading
* Modular Game API
* Bare-metal game runtime

Target Platform:

* STM32L151
* SSD1306 OLED
* W25Q80 SPI Flash

