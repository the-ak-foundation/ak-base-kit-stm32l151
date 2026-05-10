#include "task_game.h"
#include "game_logic.h"
#include "screens/scr_map.h"

static GameState_t gs;
static uint8_t game_state = STATE_INIT;

void task_game_handler(ak_msg_t* msg) {
    switch (game_state) {

    case STATE_INIT:
        game_logic_init(&gs);
        game_state = STATE_IDLE;
        break;

    case STATE_IDLE:
        if (msg->sig == SIG_BTN_UP)   gs.cursor = prev_adj(gs.cursor);
        if (msg->sig == SIG_BTN_DOWN) gs.cursor = next_adj(gs.cursor);

        if (msg->sig == SIG_BTN_OK
            && gs.nodes[gs.cursor].owner == OWNER_PLAYER) {
            gs.selected_src = gs.cursor;
            game_state = STATE_SRC_SELECTED;
        }
        if (msg->sig == SIG_TICK) {
            spawn_troops();   // +1 quân mỗi N tick cho nút của mình
            check_win();      // AI còn 0 nút → STATE_GAME_OVER
        }
        gs.dirty = 1;
        break;

    case STATE_SRC_SELECTED:
        if (msg->sig == SIG_BTN_CANCEL) {
            gs.selected_src = NO_NODE;
            game_state = STATE_IDLE;
        }
        if (msg->sig == SIG_BTN_OK
            && gs.adj[gs.selected_src] & (1 << gs.cursor)) {
            gs.march = (March_t){ gs.selected_src, gs.cursor,
                                  gs.nodes[gs.selected_src].troops / 2, 0 };
            gs.nodes[gs.selected_src].troops /= 2;
            game_state = STATE_COMBAT;
        }
        gs.dirty = 1;
        break;

    case STATE_COMBAT:
        if (msg->sig == SIG_TICK) {
            gs.march.progress += 10;   // tốc độ hành quân
            if (gs.march.progress >= 100) {
                resolve_combat();      // so sánh quân, đổi owner nếu thắng
                game_state = STATE_IDLE;
            }
        }
        gs.dirty = 1;
        break;

    case STATE_GAME_OVER:
        // ...
        break;
    }

    if (gs.dirty) {
        scr_map_render(&gs);
        gs.dirty = 0;
    }
}