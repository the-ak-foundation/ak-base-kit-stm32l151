# Node-RTS: Three Kingdoms Conquest

This project is a graph-based Real-Time Strategy (RTS) game developed for the **STM32L151** microcontroller using the **AK Base Kit** framework. It was created as part of the **FPT Telecom Embedded Bootcamp**.

## Project Overview
The game, inspired by the **Three Kingdoms** era, challenges players to command troops and capture strategic nodes on a graph. Players must manage resources (troops), secure supply lines, and outmaneuver AI-controlled rival warlords.

## Key Features
- **Graph-Based Gameplay:** Nodes represent strategic locations (cities/passes) connected by edges (supply routes).
- **Three Kingdoms Theme:** Command factions (e.g., Liu Bei vs. Cao Cao) with unique tactical focuses.
- **Resource Management:** Automatic troop generation at owned nodes with caps and scaling based on node level.
- **Dynamic AI:** Opponents with distinct personalities (Aggressive, Turtle, Opportunistic).
- **Hero System:** Powerful generals with unique skills (e.g., "Fire Attack") that can turn the tide of battle.

## Development Roadmap
The project follows a 3-week development cycle:

### Week 1: Core Foundation
- Basic graph rendering (5-8 nodes).
- Troop spawning and movement logic.
- Local Player vs. Player (PvP) mechanics.
- **Milestone:** A playable core where nodes can be captured.

### Week 2: Tactical Depth
- **Supply Lines:** Isolating nodes leads to "starvation" (troop loss).
- **AI Integration:** Rival generals with specific behavioral patterns.
- **Fog of War:** Strategic visibility limited to controlled nodes and paths.

### Week 3: Replayability & Polish
- **Procedural Maps:** Randomly generated node layouts for every match.
- **Hero System:** Integration of special skills and troop division (25/50/75/100%).
- **Random Events:** Environmental and political events saved to Flash memory.

## Technical Stack
- **Microcontroller:** STM32L151 (ARM Cortex-M3).
- **Framework:** AK Base Kit (Active Object / Task-based Architecture).
- **Language:** C/C++.
- **Peripheral Support:** OLED Display, Button Inputs, Internal Flash (for scores).

## Acknowledgments
- **FPT Telecom:** For organizing the Embedded Bootcamp.
- **AK Foundation:** For the AK Base Kit framework.

---
*Developed by Hồ Chí Công.*