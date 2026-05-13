/**
 ******************************************************************************
 * @author: An Nguyen Khanh
 * @date:   Start: 01/05/2026
 *          End:   01/05/2026
 ******************************************************************************
**/
#ifndef __SCR_GAME_OVER_H__
#define __SCR_GAME_OVER_H__

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

extern view_dynamic_t dyn_view_item_game_over;
extern view_screen_t scr_game_over;
extern void scr_game_over_handle(ak_msg_t* msg);

#endif //__SCR_GAME_OVER_H__
