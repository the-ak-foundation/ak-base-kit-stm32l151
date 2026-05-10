#ifndef AK_MOCK_H
#define AK_MOCK_H

#include <stdint.h>

// Mock AK framework signals
#define SIG_BTN_UP     1
#define SIG_BTN_DOWN   2
#define SIG_BTN_OK     3
#define SIG_BTN_CANCEL 4
#define SIG_TICK       5

// Mock message struct
typedef struct {
    uint32_t sig;
} ak_msg_t;

#endif // AK_MOCK_H
