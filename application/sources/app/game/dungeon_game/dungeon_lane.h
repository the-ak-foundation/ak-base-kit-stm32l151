/**
 ******************************************************************************
 * @author: An Nguyen Khanh
 * @date:   Start: 03/05/2026
 *          End:   03/05/2026
 ******************************************************************************
**/
#ifndef __DUNGEON_GAME_LANE_H__
#define __DUNGEON_GAME_LANE_H__

#include <stdio.h>

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"

#include "buzzer.h"
#include "scr_dungeon_game.h"

typedef struct {
	bool visible;
	uint32_t x, y;
	uint8_t action_image;
} dungeon_lane_t;

extern uint32_t dungeon_game_score;
extern dungeon_lane_t border;

extern uint8_t dungeon_get_current_frame();
extern uint8_t dungeon_get_total_frames();
extern uint8_t dungeon_get_throws_left();
extern bool dungeon_can_roll_ball();
extern void dungeon_begin_throw();
extern void dungeon_lane_handle(ak_msg_t* msg);

#endif //__DUNGEON_GAME_LANE_H__
