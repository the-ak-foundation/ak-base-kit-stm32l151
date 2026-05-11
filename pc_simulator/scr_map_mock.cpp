#include "screens/scr_map.h"
#include <SDL2/SDL.h>
#include <stdio.h>

// Extern renderer từ main.cpp
extern SDL_Renderer* g_renderer;

// Font 3x5 đơn giản để vẽ số (0-9)
const uint8_t font3x5[10][5] = {
    {0b111, 0b101, 0b101, 0b101, 0b111}, // 0
    {0b010, 0b110, 0b010, 0b010, 0b111}, // 1
    {0b111, 0b001, 0b111, 0b100, 0b111}, // 2
    {0b111, 0b001, 0b111, 0b001, 0b111}, // 3
    {0b101, 0b101, 0b111, 0b001, 0b001}, // 4
    {0b111, 0b100, 0b111, 0b001, 0b111}, // 5
    {0b111, 0b100, 0b111, 0b101, 0b111}, // 6
    {0b111, 0b001, 0b010, 0b010, 0b010}, // 7
    {0b111, 0b101, 0b111, 0b101, 0b111}, // 8
    {0b111, 0b101, 0b111, 0b001, 0b111}  // 9
};

// Hàm vẽ 1 chữ số
void draw_digit(SDL_Renderer* r, int x, int y, int digit, int size) {
    if (digit < 0 || digit > 9) return;
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 3; col++) {
            if (font3x5[digit][row] & (1 << (2 - col))) {
                SDL_Rect px = { x + col * size, y + row * size, size, size };
                SDL_RenderFillRect(r, &px);
            }
        }
    }
}

// Hàm vẽ số có nhiều chữ số
void draw_number(SDL_Renderer* r, int x, int y, int number) {
    int size = 2; // pixel size multiplier
    int spacing = 3 * size + 2; // khoảng cách giữa các chữ số
    
    char buf[10];
    sprintf(buf, "%d", number);
    
    for (int i = 0; buf[i] != '\0'; i++) {
        draw_digit(r, x + i * spacing, y, buf[i] - '0', size);
    }
}

void scr_map_render(GameState_t* gs) {
    if (!g_renderer) return;

    // Xóa nền đen
    SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
    SDL_RenderClear(g_renderer);

    // Tỷ lệ scale: Vẽ đồ họa trên hệ tọa độ 128x64, nhưng phóng to x4 trên PC
    int scale = 4;

    // Vẽ các đường kết nối (Adj)
    for (uint8_t i = 0; i < MAX_NODES; i++) {
        for (uint8_t j = i + 1; j < MAX_NODES; j++) {
            if (gs->adj[i] & (1 << j)) {
                if (gs->cost[i][j] > 4) {
                    SDL_SetRenderDrawColor(g_renderer, 139, 69, 19, 255); // Màu nâu (SaddleBrown) cho đường núi
                } else {
                    SDL_SetRenderDrawColor(g_renderer, 100, 100, 100, 255); // Màu xám cho đường bằng
                }
                SDL_RenderDrawLine(g_renderer, 
                    gs->nodes[i].x * scale, gs->nodes[i].y * scale,
                    gs->nodes[j].x * scale, gs->nodes[j].y * scale);
            }
        }
    }

    // Vẽ các Node (Thành trì)
    for (uint8_t i = 0; i < MAX_NODES; i++) {
        Node_t* n = &gs->nodes[i];
        
        // Vẽ viền lớn hơn để làm nổi bật Node đang chọn
        SDL_Rect rect = { n->x * scale - 12, n->y * scale - 12, 24, 24 };

        // Màu sắc dựa trên phe sở hữu
        if (n->owner == OWNER_PLAYER) {
            SDL_SetRenderDrawColor(g_renderer, 0, 200, 0, 255); // Xanh lá - Player
        } else if (n->owner == OWNER_AI) {
            SDL_SetRenderDrawColor(g_renderer, 200, 0, 0, 255); // Đỏ - AI
        } else {
            SDL_SetRenderDrawColor(g_renderer, 150, 150, 150, 255); // Xám - Trung lập
        }

        SDL_RenderFillRect(g_renderer, &rect);

        // Highlight con trỏ (Yellow)
        if (i == gs->cursor) {
            SDL_SetRenderDrawColor(g_renderer, 255, 255, 0, 255);
            SDL_Rect outline = { rect.x - 2, rect.y - 2, rect.w + 4, rect.h + 4 };
            SDL_RenderDrawRect(g_renderer, &outline);
        }
        
        // Highlight thành đang được chọn làm nguồn xuất quân (Cyan)
        if (i == gs->selected_src) {
            SDL_SetRenderDrawColor(g_renderer, 0, 255, 255, 255);
            SDL_Rect outline = { rect.x - 4, rect.y - 4, rect.w + 8, rect.h + 8 };
            SDL_RenderDrawRect(g_renderer, &outline);
        }

        // Vẽ số lượng quân của Node (trắng)
        SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
        int offset_x = (n->troops >= 10) ? 7 : 3; // Căn giữa nhẹ
        draw_number(g_renderer, n->x * scale - offset_x, n->y * scale - 7, n->troops);

        // Vẽ biểu thị level (các chấm vàng nhỏ) bên dưới số quân
        SDL_SetRenderDrawColor(g_renderer, 255, 215, 0, 255); // Màu vàng gold
        int total_w = n->level * 6 - 2; // Khoảng cách 6px mỗi chấm
        int start_x = n->x * scale - total_w / 2;
        for (int l = 0; l < n->level; l++) {
            SDL_Rect star = { start_x + l * 6, n->y * scale + 5, 4, 4 };
            SDL_RenderFillRect(g_renderer, &star);
        }
    }

    // Vẽ các đội quân đang hành quân và đếm số đạo quân
    int player_marches = 0;
    int ai_marches = 0;

    for (int i = 0; i < MAX_MARCHES; i++) {
        if (gs->marches[i].troops > 0) {
            if (gs->marches[i].owner == OWNER_PLAYER) player_marches++;
            if (gs->marches[i].owner == OWNER_AI) ai_marches++;
            
            if (gs->marches[i].progress > 0) {
                Node_t* src = &gs->nodes[gs->marches[i].src];
                Node_t* dst = &gs->nodes[gs->marches[i].dst];
                
                // Nội suy tọa độ dựa trên progress
                int mx = src->x + (dst->x - src->x) * gs->marches[i].progress / 100;
                int my = src->y + (dst->y - src->y) * gs->marches[i].progress / 100;
                
                // Màu đạo quân theo phe
                if (gs->marches[i].owner == OWNER_PLAYER) {
                    SDL_SetRenderDrawColor(g_renderer, 0, 200, 0, 255); // Xanh lá
                } else {
                    SDL_SetRenderDrawColor(g_renderer, 200, 0, 0, 255); // Đỏ
                }
                SDL_Rect mrect = { mx * scale - 4, my * scale - 4, 8, 8 };
                SDL_RenderFillRect(g_renderer, &mrect);
            }
        }
    }

    // Vẽ UI hiển thị số lượng hành quân ở góc màn hình (X - 4)
    // Của người chơi (Góc trái)
    SDL_SetRenderDrawColor(g_renderer, 0, 200, 0, 255); // Hộp xanh lá
    SDL_Rect p_rect = { 5, 5, 10, 10 };
    SDL_RenderFillRect(g_renderer, &p_rect);
    SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
    draw_number(g_renderer, 20, 5, player_marches);
    SDL_Rect slash1 = { 30, 10, 4, 2 }; // Dấu gạch ngang biểu tượng phân số
    SDL_RenderFillRect(g_renderer, &slash1);
    draw_number(g_renderer, 38, 5, 4);

    // Của AI (Góc phải)
    SDL_SetRenderDrawColor(g_renderer, 200, 0, 0, 255); // Hộp đỏ
    SDL_Rect ai_rect = { 512 - 50, 5, 10, 10 }; // SCREEN_WIDTH = 512
    SDL_RenderFillRect(g_renderer, &ai_rect);
    SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
    draw_number(g_renderer, 512 - 35, 5, ai_marches);
    SDL_Rect slash2 = { 512 - 25, 10, 4, 2 };
    SDL_RenderFillRect(g_renderer, &slash2);
    draw_number(g_renderer, 512 - 17, 5, 4);

    // Đẩy hình ảnh ra màn hình SDL
    SDL_RenderPresent(g_renderer);
}
