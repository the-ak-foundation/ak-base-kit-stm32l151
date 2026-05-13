/**
 ******************************************************************************
 * @author: An Nguyen Khanh
 * @date:   Start: 28/04/2026
 *          End:   28/04/2026
 ******************************************************************************
**/
#include "button.h"

#include "sys_dbg.h"

#include "app.h"
#include "app_bsp.h"
#include "app_dbg.h"
#include "app_if.h"

#include "task_list.h"

#include "scr_dungeon_game.h"

button_t btn_mode;
button_t btn_up;
button_t btn_down;

void btn_mode_callback(void* b) {
	button_t* me_b = (button_t*)b;
	switch (me_b->state) {
	case BUTTON_SW_STATE_PRESSED: {
		APP_DBG("[btn_mode_callback] BUTTON_SW_STATE_PRESSED\n");
		// task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUTTON_MODE_PRESSED);
	}
		break;

	case BUTTON_SW_STATE_LONG_PRESSED: {
		APP_DBG("[btn_mode_callback] BUTTON_SW_STATE_LONG_PRESSED\n");
		task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUTTON_MODE_LONG_PRESSED);
	}
		break;

	case BUTTON_SW_STATE_RELEASED: {
		APP_DBG("[btn_mode_callback] BUTTON_SW_STATE_RELEASED\n");
		if (dungeon_game_state == GAME_PLAY) {
			task_post_pure_msg(DUNGEON_ACTION_ID, DUNGEON_ACTION_SHOOT);
		}
		else {
			task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUTTON_MODE_RELEASED);
		}
	}
		break;

	default:
		break;
	}
}

void btn_up_callback(void* b) {
	button_t* me_b = (button_t*)b;
	switch (me_b->state) {
	case BUTTON_SW_STATE_PRESSED: {
		APP_DBG("[btn_up_callback] BUTTON_SW_STATE_PRESSED\n");
		// task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUTTON_UP_PRESSED);
	}
		break;

	case BUTTON_SW_STATE_LONG_PRESSED: {
		APP_DBG("[btn_up_callback] BUTTON_SW_STATE_LONG_PRESSED\n");
		task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUTTON_UP_LONG_PRESSED);
	}
		break;

	case BUTTON_SW_STATE_RELEASED: {
		APP_DBG("[btn_up_callback] BUTTON_SW_STATE_RELEASED\n");
		if (dungeon_game_state == GAME_PLAY) {
			task_post_pure_msg(DUNGEON_CONTROL_ID, DUNGEON_CONTROL_UP);
		}
		else {
			task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUTTON_UP_RELEASED);
		}
	}
		break;

	default:
		break;
	}
}

void btn_down_callback(void* b) {
	button_t* me_b = (button_t*)b;
	switch (me_b->state) {
	case BUTTON_SW_STATE_PRESSED: {
		APP_DBG("[btn_down_callback] BUTTON_SW_STATE_PRESSED\n");
		// task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUTTON_DOWN_PRESSED);
	}
		break;

	case BUTTON_SW_STATE_LONG_PRESSED: {
		APP_DBG("[btn_down_callback] BUTTON_SW_STATE_LONG_PRESSED\n");
		task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUTTON_DOWN_LONG_PRESSED);
	}	
		break;

	case BUTTON_SW_STATE_RELEASED: {
		APP_DBG("[btn_down_callback] BUTTON_SW_STATE_RELEASED\n");
		if (dungeon_game_state == GAME_PLAY) {
			task_post_pure_msg(DUNGEON_CONTROL_ID, DUNGEON_CONTROL_DOWN);
		}
		else {
			task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUTTON_DOWN_RELEASED);
		}
	}
		break;

	default:
		break;
	}
}
