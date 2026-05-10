#include "game_logic.h"

void game_logic_init(GameState_t* gs) {
    // Khởi tạo 6 nút mặc định cho bản đồ
    gs->nodes[0] = {10, 10, 20, OWNER_PLAYER};  // Lưu Bị - HQ
    gs->nodes[1] = {50, 10,  5, OWNER_NEUTRAL};
    gs->nodes[2] = {90, 10,  5, OWNER_NEUTRAL};
    gs->nodes[3] = {10, 45,  5, OWNER_NEUTRAL};
    gs->nodes[4] = {50, 45,  5, OWNER_NEUTRAL};
    gs->nodes[5] = {90, 45, 20, OWNER_AI};      // Tào Tháo - HQ

    // Kết nối các cạnh (bitmask)
    gs->adj[0] = (1<<1)|(1<<3);
    gs->adj[1] = (1<<0)|(1<<2)|(1<<4);
    gs->adj[2] = (1<<1)|(1<<5);
    gs->adj[3] = (1<<0)|(1<<4);
    gs->adj[4] = (1<<1)|(1<<3)|(1<<5);
    gs->adj[5] = (1<<2)|(1<<4);

    gs->cursor       = 0;
    gs->selected_src = NO_NODE;
    gs->tick         = 0;
    gs->dirty        = 1;
}

void spawn_troops(GameState_t* gs) {
    for (uint8_t i = 0; i < MAX_NODES; i++) {
        if (gs->nodes[i].owner != OWNER_NEUTRAL
            && gs->nodes[i].troops < 99) {
            gs->nodes[i].troops++;
        }
    }
}

void resolve_combat(GameState_t* gs) {
    Node_t* dst = &gs->nodes[gs->march.dst];
    if (dst->owner == gs->nodes[gs->march.src].owner) {
        dst->troops += gs->march.troops;  // tiếp viện
    } else if (gs->march.troops > dst->troops) {
        dst->owner  = gs->nodes[gs->march.src].owner;
        dst->troops = gs->march.troops - dst->troops;
    } else {
        dst->troops -= gs->march.troops;
    }
}

uint8_t check_win(GameState_t* gs) {
    uint8_t ai_nodes = 0, player_nodes = 0;
    for (uint8_t i = 0; i < MAX_NODES; i++) {
        if (gs->nodes[i].owner == OWNER_AI)     ai_nodes++;
        if (gs->nodes[i].owner == OWNER_PLAYER) player_nodes++;
    }
    if (ai_nodes == 0)     return OWNER_PLAYER;
    if (player_nodes == 0) return OWNER_AI;
    return OWNER_NEUTRAL;
}