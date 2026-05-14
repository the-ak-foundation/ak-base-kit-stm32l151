/**
 ******************************************************************************
 * @author: An Nguyen Khanh
 * @date:   Start: 02/05/2026
 *          End:   02/05/2026
 ******************************************************************************
**/
#ifndef __SCR_DUNGEON_HOW_TO_PLAY_H__
#define __SCR_DUNGEON_HOW_TO_PLAY_H__

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

#include "screens.h"

extern view_dynamic_t dyn_view_item_dungeon_how_to_play;
extern view_screen_t scr_dungeon_how_to_play;
extern void scr_dungeon_how_to_play_handle(ak_msg_t* msg);

#endif //__SCR_DUNGEON_HOW_TO_PLAY_H__
