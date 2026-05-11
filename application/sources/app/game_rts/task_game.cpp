#include "task_game.h"
#include "game_logic.h"
#include "screens/scr_map.h"

// Biến toàn cục lưu trữ trạng thái game và state hiện tại
static GameState_t gs;
static uint8_t game_state = STATE_INIT;

// Khai báo ngoài để tìm Node kề (cần được implement thêm logic chọn mục tiêu)
extern uint8_t prev_adj(uint8_t current_cursor);
extern uint8_t next_adj(uint8_t current_cursor);

/**
 * @brief Tìm một khe trống để xuất quân, đồng thời đếm số quân đang đi của Owner
 * @return Index của march_slot hoặc -1 nếu đã đạt tối đa 4 đạo quân
 */
static int get_free_march_slot(GameState_t* gs_ptr, uint8_t owner) {
    int active_count = 0;
    int free_slot = -1;
    for (int i = 0; i < MAX_MARCHES; i++) {
        if (gs_ptr->marches[i].troops > 0) {
            if (gs_ptr->marches[i].owner == owner) active_count++;
        } else {
            if (free_slot == -1) free_slot = i;
        }
    }
    if (active_count >= 4) return -1;
    return free_slot;
}

/**
 * @brief Xử lý sự kiện Tick chung cho các state (cập nhật quân, hành quân, AI)
 */
static void handle_global_tick() {
    spawn_troops(&gs);   // +1 quân mỗi N tick
    if (check_win(&gs) != OWNER_NEUTRAL) {
        game_state = STATE_GAME_OVER;
        return;
    }

    // Cập nhật tất cả các đạo quân đang đi
    for (int i = 0; i < MAX_MARCHES; i++) {
        if (gs.marches[i].troops > 0) {
            uint8_t cost = gs.cost[gs.marches[i].src][gs.marches[i].dst];
            uint8_t speed = 100 / (cost > 0 ? cost : 4); // Mặc định 25 nếu cost=4
            gs.marches[i].progress += speed; // Tăng tốc độ hành quân theo địa hình
            if (gs.marches[i].progress >= 100) {
                resolve_combat(&gs, i);   
                // Xóa đạo quân sau khi giao tranh xong
                gs.marches[i].troops = 0;
                gs.marches[i].progress = 0;
            }
        }
    }
    
    // AI đưa ra quyết định nếu chưa đạt giới hạn 4 đạo quân
    int ai_slot = get_free_march_slot(&gs, OWNER_AI);
    if (ai_slot != -1) {
        bool want_attack = false;
        uint8_t ai_src, ai_dst;
        ai_think(&gs, &want_attack, &ai_src, &ai_dst);
        
        if (want_attack) {
            // Đưa quân đi đánh
            gs.marches[ai_slot] = (March_t){ ai_src, ai_dst, (uint8_t)(gs.nodes[ai_src].troops / 2), 0, OWNER_AI };
            gs.nodes[ai_src].troops /= 2;
        }
    }
    gs.dirty = 1;
}

/**
 * @brief Hàm xử lý các message gửi đến task game (State Machine)
 * @param msg Con trỏ chứa thông tin tín hiệu sự kiện (nút bấm, tick...)
 */
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
            handle_global_tick();
        }
        gs.dirty = 1; 
        break;

    case STATE_SRC_SELECTED:
        if (msg->sig == SIG_BTN_UP)   gs.cursor = prev_adj(gs.cursor);
        if (msg->sig == SIG_BTN_DOWN) gs.cursor = next_adj(gs.cursor);

        if (msg->sig == SIG_BTN_CANCEL) {
            gs.selected_src = NO_NODE;
            game_state = STATE_IDLE;
        }
        
        // Xuất quân nếu ấn OK ở thành kề
        if (msg->sig == SIG_BTN_OK
            && (gs.adj[gs.selected_src] & (1 << gs.cursor))) {
            
            // Chỉ xuất quân nếu chưa đạt giới hạn 4 đạo quân
            int player_slot = get_free_march_slot(&gs, OWNER_PLAYER);
            if (player_slot != -1) {
                gs.marches[player_slot] = (March_t){ gs.selected_src, gs.cursor,
                                      (uint8_t)(gs.nodes[gs.selected_src].troops / 2), 0, OWNER_PLAYER };
                gs.nodes[gs.selected_src].troops /= 2; 
            }
            // Trở về trạng thái IDLE để người chơi có thể tiếp tục thao tác khác
            gs.selected_src = NO_NODE;
            game_state = STATE_IDLE; 
        }

        if (msg->sig == SIG_TICK) {
            handle_global_tick();
        }
        gs.dirty = 1;
        break;

    case STATE_GAME_OVER:
        // TODO: Hiển thị kết quả
        break;
    }

    if (gs.dirty) {
        scr_map_render(&gs);
        gs.dirty = 0; 
    }
}