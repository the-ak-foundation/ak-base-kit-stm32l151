/**
 ******************************************************************************
 * @author: An Nguyen Khanh
 * @date:   Start: 04/05/2026
 *          End:   04/05/2026
 ******************************************************************************
**/
#ifndef __DUNGEON_EFFECT_H__
#define __DUNGEON_EFFECT_H__

#include <stdio.h>

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"

#include "scr_dungeon_game.h"

#define NUM_BANG                 (5)

typedef struct {
	bool visible;
	int16_t x, y;
	uint8_t life;
	uint8_t action_image;
} dungeon_effect_t;

extern dungeon_effect_t bang[NUM_BANG];

extern void dungeon_spawn_hit_effect(int16_t x, int16_t y);
extern void dungeon_effect_handle(ak_msg_t* msg);

#endif //__DUNGEON_EFFECT_H__
