/**
 ******************************************************************************
 * @author: An Nguyen Khanh
 * @date:   Start: 05/05/2026
 *          End:   06/05/2026
 ******************************************************************************
**/
#ifndef __SCR_DUNGEON_GAME_H__
#define __SCR_DUNGEON_GAME_H__

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

#include "eeprom.h"
#include "app_eeprom.h"

#include "buzzer.h"

#include "screens.h"
#include "screens_bitmap.h"

#include "dungeon_control.h"
#include "dungeon_action.h"
#include "dungeon_effect.h"
#include "dungeon_lane.h"
#include "dungeon_state.h"

#define GAME_OFF		(0)
#define GAME_PLAY		(1)
#define GAME_OVER		(2)

#define DUNGEON_START_CONTINUE	(0)
#define DUNGEON_START_NEW_GAME	(1)
#define DUNGEON_START_LEVEL		(2)
#define DUNGEON_START_CREATOR	(3)

#define DUNGEON_OUTCOME_NONE	(0)
#define DUNGEON_OUTCOME_WIN		(1)
#define DUNGEON_OUTCOME_LOSE	(2)

extern uint8_t dungeon_game_state;
extern dungeon_game_setting_t settingsetup;
extern uint8_t dungeon_start_mode;
extern uint8_t dungeon_selected_level;
extern uint8_t dungeon_last_outcome;

extern view_dynamic_t dyn_view_item_dungeon_game;
extern view_screen_t scr_dungeon_game;
extern void scr_dungeon_game_handle(ak_msg_t* msg);

extern void dungeon_prepare_continue();
extern void dungeon_prepare_new_game();
extern void dungeon_prepare_level(uint8_t level);
extern void dungeon_prepare_creator_mode(uint8_t level);
extern uint8_t dungeon_has_save_data();
extern uint8_t dungeon_is_creator_mode();

extern void dungeon_move_selection(int8_t delta);
extern void dungeon_confirm_action();
extern void dungeon_tick();
extern void dungeon_setup_session();
extern void dungeon_reset_session();

extern uint8_t dungeon_get_current_stage();
extern uint8_t dungeon_get_total_stages();
extern uint8_t dungeon_get_level_value();
extern uint32_t dungeon_get_score_value();

#endif //__SCR_DUNGEON_GAME_H__
