/**
 * @file game_logic.h
 * @brief Định nghĩa các cấu trúc dữ liệu và logic cốt lõi cho mini-game RTS.
 *        Bao gồm các trạng thái game, cấu trúc Node (thành trì), March (đội quân hành quân).
 */
#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <stdint.h>
#include <stdbool.h>

/* ── Enums ── */
/**
 * @brief Phe phái sở hữu các Node trong game
 */
typedef enum { 
    OWNER_NEUTRAL = 0, // Phe trung lập
    OWNER_PLAYER = 1,  // Người chơi (Lưu Bị)
    OWNER_AI = 2       // Máy (Tào Tháo)
} Owner_t;

/**
 * @brief Các trạng thái chính của Game State Machine
 */
typedef enum {
    STATE_INIT = 0,      // Khởi tạo game
    STATE_IDLE,          // Trạng thái chờ, người chơi di chuyển con trỏ
    STATE_SRC_SELECTED,  // Đã chọn thành trì nguồn, chờ chọn mục tiêu để xuất quân
    STATE_GAME_OVER      // Trò chơi kết thúc (Thắng/Thua)
} GameStateEnum_t;

/* ── Structs ── */
#define MAX_NODES 6      // Số lượng tối đa các thành trì trên bản đồ
#define NO_NODE   0xFF   // Giá trị biểu diễn không có thành trì nào được chọn

#define LEVEL_2_THRESHOLD 30
#define LEVEL_3_THRESHOLD 60

/**
 * @brief Cấu trúc dữ liệu của một Node (Thành trì)
 */
typedef struct {
    uint8_t x, y;        // Tọa độ trên màn hình để render
    uint8_t troops;      // Số lượng quân lính hiện tại
    uint8_t level;       // Cấp độ của thành trì (1, 2, 3...)
    uint8_t owner;       // Phe sở hữu (Owner_t)
} Node_t;

/**
 * @brief Cấu trúc dữ liệu cho một đạo quân đang hành quân
 */
typedef struct {
    uint8_t src, dst;    // Node nguồn và Node đích
    uint8_t troops;      // Số lượng quân lính tham gia hành quân
    uint8_t progress;    // Tiến trình di chuyển (0 - 100%)
    uint8_t owner;       // Phe sở hữu đội quân (Owner_t)
} March_t;

#define MAX_MARCHES 8

/**
 * @brief Trạng thái tổng thể của ván game
 */
typedef struct {
    Node_t  nodes[MAX_NODES];   // Danh sách các thành trì
    uint8_t adj[MAX_NODES];     // Ma trận kề (bitmask) lưu kết nối giữa các Node
    uint8_t cost[MAX_NODES][MAX_NODES]; // Ma trận trọng số của các cạnh (cost di chuyển)
    March_t marches[MAX_MARCHES]; // Mảng thông tin các đạo quân đang hành quân
    uint8_t cursor;             // Vị trí con trỏ hiện tại đang trỏ vào Node nào
    uint8_t selected_src;       // Node đang được chọn để xuất quân
    uint8_t tick;               // Bộ đếm thời gian (tick) trong game
    uint8_t dirty;              // Cờ đánh dấu cần render lại màn hình (1 = cần render)
} GameState_t;

/* ── Khai báo hàm ── */
void game_logic_init(GameState_t* gs);
void spawn_troops(GameState_t* gs);
void resolve_combat(GameState_t* gs, uint8_t march_idx);
uint8_t check_win(GameState_t* gs);
void ai_think(GameState_t* gs, bool* want_attack, uint8_t* src, uint8_t* dst);

#endif // GAME_LOGIC_H