/**
 ******************************************************************************
 * @author: An Nguyen Khanh
 * @date:   Start: 03/05/2026
 *          End:   04/05/2026
 ******************************************************************************
**/
#ifndef __DUNGEON_STATE_H__
#define __DUNGEON_STATE_H__

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

#define NUM_METEOROIDS              (5)

typedef struct {
	bool visible;
	int16_t x, y;
	uint8_t action_image;
} dungeon_state_t;

extern dungeon_state_t meteoroid[NUM_METEOROIDS];

extern uint8_t dungeon_count_active_meteoroids();
extern void dungeon_state_reset_meteoroids();
extern void dungeon_process_action_hit(int16_t action_x, int16_t action_y, uint8_t power);
extern void dungeon_state_handle(ak_msg_t* msg);

#endif //__DUNGEON_STATE_H__
