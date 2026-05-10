#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <stdint.h>

/* ── Enums ── */
typedef enum { OWNER_NEUTRAL=0, OWNER_PLAYER=1, OWNER_AI=2 } Owner_t;

typedef enum {
    STATE_INIT = 0,
    STATE_IDLE,
    STATE_SRC_SELECTED,
    STATE_COMBAT,
    STATE_GAME_OVER
} GameStateEnum_t;

/* ── Structs ── */
#define MAX_NODES 8
#define NO_NODE   0xFF

typedef struct {
    uint8_t x, y;
    uint8_t troops;
    uint8_t owner;
} Node_t;

typedef struct {
    uint8_t src, dst;
    uint8_t troops;
    uint8_t progress;
} March_t;

typedef struct {
    Node_t  nodes[MAX_NODES];
    uint8_t adj[MAX_NODES];
    March_t march;
    uint8_t cursor;
    uint8_t selected_src;
    uint8_t tick;
    uint8_t dirty;
} GameState_t;

/* ── Khai báo hàm ── */
void game_logic_init(GameState_t* gs);
void spawn_troops(GameState_t* gs);
void start_march(GameState_t* gs, uint8_t src, uint8_t dst);
void resolve_combat(GameState_t* gs);
uint8_t check_win(GameState_t* gs);

#endif