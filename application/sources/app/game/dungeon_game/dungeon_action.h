/**
 ******************************************************************************
 * @author: An Nguyen Khanh
 * @date:   Start: 04/05/2026
 *          End:   04/05/2026
 ******************************************************************************
**/
#ifndef __DUNGEON_ACTION_H__
#define __DUNGEON_ACTION_H__

#include <stdio.h>

#include "fsm.h"
#include "port.h"
#include "message.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"

#include "buzzer.h"
#include "scr_dungeon_game.h"

#define MAX_NUM_ARROW       (1)

typedef struct {
	bool visible;
	int16_t x, y;
	int16_t vx;
	uint8_t power;
	uint8_t action_image;
} dungeon_action_t;

extern dungeon_action_t arrow[MAX_NUM_ARROW];

extern bool dungeon_action_is_active();
extern bool dungeon_action_consume_finished();
extern void dungeon_action_handle(ak_msg_t* msg);

#endif //__DUNGEON_ACTION_H__
