#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "task_game.h"
#include "game_logic.h"

SDL_Window* g_window = NULL;
SDL_Renderer* g_renderer = NULL;

const int SCREEN_WIDTH = 128 * 4; // 512 pixels
const int SCREEN_HEIGHT = 64 * 4; // 256 pixels

// Mock missing logic functions for adj (Di chuyển con trỏ)
uint8_t prev_adj(uint8_t current_cursor) {
    if (current_cursor == 0) return MAX_NODES - 1;
    return current_cursor - 1;
}

uint8_t next_adj(uint8_t current_cursor) {
    if (current_cursor == MAX_NODES - 1) return 0;
    return current_cursor + 1;
}

int main(int argc, char* args[]) {
    // Khởi tạo SDL Video
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Tạo cửa sổ kích thước 512x256 (Tỷ lệ 4x so với màn OLED 128x64)
    g_window = SDL_CreateWindow("Three Kingdoms RTS Simulator", 
                                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                                SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!g_window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
    if (!g_renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Khởi tạo State Machine của Game
    ak_msg_t init_msg = {0};
    task_game_handler(&init_msg); // Gọi state STATE_INIT

    bool quit = false;
    SDL_Event e;
    
    // Giả lập hệ thống Tick (1 giây = 1 tick)
    uint32_t last_tick_time = SDL_GetTicks();
    const uint32_t TICK_INTERVAL_MS = 1000; 

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                ak_msg_t msg;
                bool send_msg = false;
                
                // Map phím tắt trên PC vào các event của AK Framework
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                    case SDLK_LEFT:
                        msg.sig = SIG_BTN_UP;
                        send_msg = true;
                        break;
                    case SDLK_DOWN:
                    case SDLK_RIGHT:
                        msg.sig = SIG_BTN_DOWN;
                        send_msg = true;
                        break;
                    case SDLK_RETURN:
                    case SDLK_SPACE:
                        msg.sig = SIG_BTN_OK;
                        send_msg = true;
                        break;
                    case SDLK_ESCAPE:
                        msg.sig = SIG_BTN_CANCEL;
                        send_msg = true;
                        break;
                }
                
                if (send_msg) {
                    task_game_handler(&msg);
                }
            }
        }

        // Tạo sự kiện Tick
        uint32_t current_time = SDL_GetTicks();
        if (current_time - last_tick_time >= TICK_INTERVAL_MS) {
            last_tick_time = current_time;
            ak_msg_t msg = { SIG_TICK };
            task_game_handler(&msg);
        }

        SDL_Delay(16); // Delay ~60 FPS
    }

    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    SDL_Quit();

    return 0;
}
