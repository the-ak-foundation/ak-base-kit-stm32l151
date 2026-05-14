/**
 ******************************************************************************
 * @author: An Nguyen Khanh
 * @date:   Start: 01/05/2026
 *          End:   02/05/2026
 ******************************************************************************
**/
#ifndef __SCR_CHARTS_GAME_H__
#define __SCR_CHARTS_GAME_H__

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "sys_ctrl.h"
#include "sys_dbg.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"
#include "view_render.h"

#include "buzzer.h"

#include "eeprom.h"
#include "app_eeprom.h"

#include "screens.h"
#include "screens_bitmap.h"

#define DUNGEON_LEVEL_VIEW_INFO		(0)
#define DUNGEON_LEVEL_VIEW_CREATOR	(1)

extern view_dynamic_t dyn_view_item_charts_game;
extern view_screen_t scr_charts_game;
extern void scr_charts_game_handle(ak_msg_t* msg);
extern void dungeon_set_level_view_mode(uint8_t mode);

#endif //__SCR_CHARTS_GAME_H__
