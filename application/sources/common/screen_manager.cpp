/**
 ******************************************************************************
 * @author: An Nguyen Khanh
 * @date:   31/11/2016
 ******************************************************************************
**/

#include "ak.h"
#include "message.h"

#include "screen_manager.h"

#include "sys_dbg.h"

static ak_msg_t screen_msg_entry;

static scr_mng_t* screen_manager = SCREEN_MANAGER_NULL;
static view_screen_t* view_screen = VIEW_SCREEN_NULL;

static uint8_t screen_none_update_mark = 0;

void scr_mng_ctor(scr_mng_t* scr_mng, screen_f init_scr, view_screen_t* scr_obj) {
	/* init entry message */
	screen_msg_entry.sig = SCREEN_ENTRY;

	view_screen = scr_obj;				/* point to current screen object */
	screen_manager = scr_mng;			/* init singleton screen manager */
	screen_manager->screen = init_scr;	/* assign init handler */
}

void scr_mng_dispatch(ak_msg_t* msg) {
	if (screen_manager == SCREEN_MANAGER_NULL) {
		FATAL("SCR_MNG", 0x01);
		return;
	}

	screen_none_update_mark = 1;

	screen_manager->screen(msg);

	if (screen_none_update_mark) {
		view_render_screen(view_screen);
	}
}

void scr_mng_tran(screen_f target,  view_screen_t* scr_obj) {
	if (screen_manager == SCREEN_MANAGER_NULL) {
		FATAL("SCR_MNG", 0x01);
		return;
	}

	/* change new screen */
	view_screen = scr_obj;
	screen_manager->screen = target;

	/* entry new screen */
	screen_manager->screen(&screen_msg_entry);
	view_render_screen(view_screen);
}

void scr_mng_contain_screen_none_update_mark() {
	screen_none_update_mark = 0;
}

screen_f scr_mng_get_current_screen() {
	return screen_manager->screen;
}
