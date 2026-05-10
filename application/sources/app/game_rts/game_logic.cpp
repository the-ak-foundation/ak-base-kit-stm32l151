/**
 * @file game_logic.cpp
 * @brief Cài đặt các hàm xử lý logic của trò chơi như khởi tạo, 
 *        tăng quân số, xử lý giao tranh và kiểm tra thắng/thua.
 */
#include "game_logic.h"

/**
 * @brief Khởi tạo trạng thái ban đầu của game
 * @param gs Con trỏ tới GameState
 */
void game_logic_init(GameState_t* gs) {
    // Khởi tạo 6 nút mặc định cho bản đồ với tọa độ, số quân và chủ sở hữu
    gs->nodes[0] = {10, 10, 20, OWNER_PLAYER};  // Lưu Bị - HQ (Trụ sở chính)
    gs->nodes[1] = {50, 10,  5, OWNER_NEUTRAL}; // Các thành trì trung lập
    gs->nodes[2] = {90, 10,  5, OWNER_NEUTRAL};
    gs->nodes[3] = {10, 45,  5, OWNER_NEUTRAL};
    gs->nodes[4] = {50, 45,  5, OWNER_NEUTRAL};
    gs->nodes[5] = {90, 45, 20, OWNER_AI};      // Tào Tháo - HQ (Trụ sở chính AI)

    // Kết nối các cạnh (đường đi giữa các thành trì) dùng bitmask
    // Ví dụ: Nút 0 kết nối với Nút 1 và Nút 3
    gs->adj[0] = (1<<1)|(1<<3);
    gs->adj[1] = (1<<0)|(1<<2)|(1<<4);
    gs->adj[2] = (1<<1)|(1<<5);
    gs->adj[3] = (1<<0)|(1<<4);
    gs->adj[4] = (1<<1)|(1<<3)|(1<<5);
    gs->adj[5] = (1<<2)|(1<<4);

    // Khởi tạo các mảng hành quân trống
    for (uint8_t i = 0; i < MAX_MARCHES; i++) {
        gs->marches[i].troops = 0;
        gs->marches[i].progress = 0;
    }

    // Đặt lại các biến trạng thái
    gs->cursor       = 0;         // Đặt con trỏ ở HQ của người chơi
    gs->selected_src = NO_NODE;   // Chưa chọn thành trì nào
    gs->tick         = 0;         // Reset thời gian
    gs->dirty        = 1;         // Yêu cầu vẽ lại màn hình lần đầu
}

/**
 * @brief Sinh thêm quân lính cho các thành trì đã có chủ theo thời gian
 * @param gs Con trỏ tới GameState
 */
void spawn_troops(GameState_t* gs) {
    for (uint8_t i = 0; i < MAX_NODES; i++) {
        // Chỉ tăng quân cho người chơi và AI, tối đa 99 quân
        if (gs->nodes[i].owner != OWNER_NEUTRAL
            && gs->nodes[i].troops < 99) {
            gs->nodes[i].troops++;
        }
    }
}

/**
 * @brief Xử lý kết quả khi đạo quân đến đích (Giao tranh hoặc Tiếp viện)
 * @param gs Con trỏ tới GameState
 * @param march_idx Index của đạo quân trong mảng marches
 */
void resolve_combat(GameState_t* gs, uint8_t march_idx) {
    March_t* m = &gs->marches[march_idx];
    Node_t* dst = &gs->nodes[m->dst];
    
    // Nếu thành đích có cùng chủ sở hữu -> Tiếp viện
    if (dst->owner == m->owner) {
        dst->troops += m->troops;  // Cộng thêm quân lính
        if (dst->troops > 99) dst->troops = 99; // Giới hạn tối đa 99 quân
    } 
    // Nếu đạo quân lớn hơn quân thủ của đối phương -> Chiếm thành
    else if (m->troops > dst->troops) {
        dst->owner  = m->owner; // Đổi phe sở hữu
        dst->troops = m->troops - dst->troops; // Trừ đi quân tổn thất
    } 
    // Nếu đạo quân nhỏ hơn hoặc bằng -> Công thành thất bại
    else {
        dst->troops -= m->troops; // Quân thủ bị hao hụt
    }
}

/**
 * @brief Kiểm tra điều kiện thắng thua của trò chơi
 * @param gs Con trỏ tới GameState
 * @return Phe chiến thắng (OWNER_PLAYER, OWNER_AI) hoặc OWNER_NEUTRAL nếu chưa ai thắng
 */
uint8_t check_win(GameState_t* gs) {
    uint8_t ai_nodes = 0, player_nodes = 0;
    for (uint8_t i = 0; i < MAX_NODES; i++) {
        if (gs->nodes[i].owner == OWNER_AI)     ai_nodes++;
        if (gs->nodes[i].owner == OWNER_PLAYER) player_nodes++;
    }
    if (ai_nodes == 0)     return OWNER_PLAYER; // Người chơi chiếm toàn bộ thành của AI
    if (player_nodes == 0) return OWNER_AI;     // AI chiếm toàn bộ thành của người chơi
    return OWNER_NEUTRAL;                       // Game vẫn đang tiếp diễn
}

/**
 * @brief AI đánh giá và đưa ra quyết định hành quân
 * @param gs Con trỏ tới GameState
 * @param want_attack AI có muốn tấn công hay không (true/false)
 * @param src Node nguồn AI chọn
 * @param dst Node đích AI muốn đánh
 */
void ai_think(GameState_t* gs, bool* want_attack, uint8_t* src, uint8_t* dst) {
    *want_attack = false;
    // Tìm một node của AI có nhiều quân để làm điểm xuất phát
    for (uint8_t i = 0; i < MAX_NODES; i++) {
        if (gs->nodes[i].owner == OWNER_AI && gs->nodes[i].troops > 15) {
            // Duyệt các node kề cạnh
            for (uint8_t j = 0; j < MAX_NODES; j++) {
                if (gs->adj[i] & (1 << j)) {
                    // Ưu tiên chiếm node trung lập hoặc node người chơi nếu quân AI nhiều hơn gấp rưỡi
                    if (gs->nodes[j].owner != OWNER_AI && gs->nodes[j].troops < gs->nodes[i].troops * 2 / 3) {
                        *src = i;
                        *dst = j;
                        *want_attack = true;
                        return; // Quyết định đánh ngay
                    }
                }
            }
        }
    }
}