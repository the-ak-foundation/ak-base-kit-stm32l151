/**
 ******************************************************************************
 * @author: An Nguyen Khanh
 * @date:   Start: 05/05/2026
 *          End:   05/05/2026
 ******************************************************************************
**/
#include "dungeon_control.h"

dungeon_control_t dungeon_control;

static void dungeon_control_setup() {
	dungeon_control.visible = WHITE;
	dungeon_control.aim_index = 0;
	dungeon_control.power = DUNGEON_POWER_MIN;
	dungeon_control.power_dir = 1;
	dungeon_control.action_image = 1;
	dungeon_control.x = 8;
	dungeon_control.y = 8;
}

static void dungeon_control_update() {
	if (dungeon_game_state != GAME_PLAY) {
		return;
	}

	dungeon_control.action_image++;
	if (dungeon_control.action_image > 3) {
		dungeon_control.action_image = 1;
	}
}

static void dungeon_control_move_up() {
	if (dungeon_game_state != GAME_PLAY) {
		return;
	}
	dungeon_move_selection(-1);
}

static void dungeon_control_move_down() {
	if (dungeon_game_state != GAME_PLAY) {
		return;
	}
	dungeon_move_selection(1);
}

static void dungeon_control_reset() {
	dungeon_control.visible = BLACK;
	dungeon_control.aim_index = 0;
	dungeon_control.power = DUNGEON_POWER_MIN;
	dungeon_control.power_dir = 1;
	dungeon_control.action_image = 0;
	dungeon_control.x = 0;
	dungeon_control.y = 0;
}

void dungeon_control_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case DUNGEON_CONTROL_SETUP: {
		APP_DBG_SIG("DUNGEON_GAME_LAUNCHER_SETUP\n");
		dungeon_control_setup();
	}
		break;

	case DUNGEON_CONTROL_UPDATE: {
		APP_DBG_SIG("DUNGEON_GAME_LAUNCHER_UPDATE\n");
		dungeon_control_update();
	}
		break;

	case DUNGEON_CONTROL_UP: {
		APP_DBG_SIG("DUNGEON_GAME_LAUNCHER_UP\n");
		dungeon_control_move_up();
	}
		break;

	case DUNGEON_CONTROL_DOWN: {
		APP_DBG_SIG("DUNGEON_GAME_LAUNCHER_DOWN\n");
		dungeon_control_move_down();
	}
		break;

	case DUNGEON_CONTROL_RESET: {
		APP_DBG_SIG("DUNGEON_GAME_LAUNCHER_RESET\n");
		dungeon_control_reset();
	}
		break;

	default:
		break;
	}
}
