/**
 ******************************************************************************
 * @author: An Nguyen Khanh
 * @date:   Start: 30/04/2026
 *          End:   01/05/2026
 ******************************************************************************
**/
#ifndef __SCR_GAME_SETTING_H__
#define __SCR_GAME_SETTING_H__

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

// chosse
#define STEP_SETTING_CHOSSE 					(15)
// items
#define SETTING_ITEM_ARRDESS_0					(0)
#define SETTING_ITEM_ARRDESS_1					(STEP_SETTING_CHOSSE)
#define SETTING_ITEM_ARRDESS_2					(STEP_SETTING_CHOSSE*2)
#define SETTING_ITEM_ARRDESS_3					(STEP_SETTING_CHOSSE*3)
#define SETTING_ITEM_ARRDESS_4					(STEP_SETTING_CHOSSE*4)
// Text and Number
#define DUNGEON_SETTING_TEXT_AXIS_X 			(20)
#define DUNGEON_SETTING_NUMBER_AXIS_X			(110)
// Chosse icon	
#define DUNGEON_SETTING_CHOSSE_ICON_AXIS_Y		(17)
#define DUNGEON_SETTING_CHOSSE_ICON_SIZE_W		(20)
#define DUNGEON_SETTING_CHOSSE_ICON_SIZE_H		(20)
// Frames	
#define DUNGEON_SETTING_FRAMES_AXIS_X			(20)
#define DUNGEON_SETTING_FRAMES_AXIS_Y_1			(2)
#define DUNGEON_SETTING_FRAMES_STEP 			(15)
#define DUNGEON_SETTING_FRAMES_SIZE_W			(103)
#define DUNGEON_SETTING_FRAMES_SIZE_H			(13)
#define DUNGEON_SETTING_FRAMES_SIZE_R			(3)

extern dungeon_game_setting_t settingdata;

extern view_dynamic_t dyn_view_item_game_setting;
extern view_screen_t scr_game_setting;
extern void scr_game_setting_handle(ak_msg_t* msg);

#endif //__SCR_GAME_SETTING_H__
