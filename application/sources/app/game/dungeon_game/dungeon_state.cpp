/**
 ******************************************************************************
 * @author: An Nguyen Khanh
 * @date:   Start: 03/05/2026
 *          End:   04/05/2026
 ******************************************************************************
**/
#include "dungeon_state.h"

#include "dungeon_effect.h"

dungeon_state_t meteoroid[NUM_METEOROIDS];

static void dungeon_state_setup() {
	for (uint8_t i = 0; i < NUM_METEOROIDS; i++) {
		meteoroid[i].x = 0;
		meteoroid[i].y = 0;
		meteoroid[i].visible = BLACK;
		meteoroid[i].action_image = 0;
	}
}

uint8_t dungeon_count_active_meteoroids() {
	return 0;
}

void dungeon_state_reset_meteoroids() {
	dungeon_state_setup();
}

static void dungeon_state_clear_index(uint8_t index) {
	(void)index;
}

void dungeon_process_action_hit(int16_t action_x, int16_t action_y, uint8_t power) {
	(void)action_x;
	(void)action_y;
	(void)power;
}

static void dungeon_state_run() {
	/* Rendered directly from dungeon state. */
}

void dungeon_state_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case DUNGEON_STATE_SETUP: {
		APP_DBG_SIG("DUNGEON_GAME_PINS_SETUP\n");
		dungeon_state_setup();
	}
		break;

	case DUNGEON_STATE_RUN: {
		APP_DBG_SIG("DUNGEON_GAME_PINS_RUN\n");
		dungeon_state_run();
	}
		break;

	case DUNGEON_STATE_DETONATOR: {
		/* Collision is handled by the rolling ball task. */
	}
		break;

	case DUNGEON_STATE_RESET: {
		APP_DBG_SIG("DUNGEON_GAME_PINS_RESET\n");
		dungeon_state_setup();
	}
		break;

	default:
		break;
	}
}
