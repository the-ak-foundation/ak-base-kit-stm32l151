/**
 ******************************************************************************
 * @author: An Nguyen Khanh
 * @date:   Start: 05/05/2026
 *          End:   05/05/2026
 ******************************************************************************
**/
#ifndef __DUNGEON_CONTROL_H__
#define __DUNGEON_CONTROL_H__

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

#define DUNGEON_LANE_X               (8)
#define DUNGEON_LANE_Y               (14)
#define DUNGEON_LANE_W               (112)
#define DUNGEON_LANE_H               (34)

#define DUNGEON_ACTION_START_X       (16)
#define DUNGEON_ACTION_RADIUS        (3)
#define DUNGEON_POWER_MIN            (1)
#define DUNGEON_POWER_MAX            (5)
#define DUNGEON_NUM_AIM_LANES        (5)

typedef struct {
	bool visible;
	int16_t x, y;
	uint8_t aim_index;
	uint8_t power;
	int8_t power_dir;
	uint8_t action_image;
} dungeon_control_t;

extern dungeon_control_t dungeon_control;

extern void dungeon_control_handle(ak_msg_t* msg);

#endif //__DUNGEON_CONTROL_H__
