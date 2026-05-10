#ifndef TASK_GAME_H
#define TASK_GAME_H

// Nếu chạy trên giả lập PC, include thư viện mock
#ifdef PC_SIMULATOR
#include "ak_mock.h"
#else
// Chạy trên mạch thật thì include framework AK gốc
// (Cập nhật đường dẫn file nếu cần)
//#include "ak.h"
//#include "message.h"
#endif

// Hàm xử lý logic chính
void task_game_handler(ak_msg_t* msg);

#endif // TASK_GAME_H
