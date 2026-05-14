/**
 ******************************************************************************
 * @author: An Nguyen Khanh
 * @date:   Start: 29/04/2026
 *          End:   30/04/2026
 ******************************************************************************
**/
#ifndef __SCR_STARTUP_H__
#define __SCR_STARTUP_H__

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

extern view_dynamic_t dyn_view_startup;
extern view_screen_t scr_startup;
extern void scr_startup_handle(ak_msg_t* msg);

#endif //__SCR_STARTUP_H__
