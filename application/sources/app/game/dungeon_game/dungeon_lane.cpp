/**
 ******************************************************************************
 * @author: An Nguyen Khanh
 * @date:   Start: 03/05/2026
 *          End:   03/05/2026
 ******************************************************************************
**/
#include "dungeon_lane.h"

dungeon_lane_t border;
uint32_t dungeon_game_score = 0;

uint8_t dungeon_get_current_frame() {
	return dungeon_get_current_stage();
}

uint8_t dungeon_get_total_frames() {
	return dungeon_get_total_stages();
}

uint8_t dungeon_get_throws_left() {
	return dungeon_get_level_value();
}

bool dungeon_can_roll_ball() {
	return (dungeon_game_state == GAME_PLAY);
}

void dungeon_begin_throw() {
	/* Dungeon mode handles turn resolution directly in confirm action. */
}

static void dungeon_lane_setup() {
	border.x = DUNGEON_LANE_X;
	border.y = DUNGEON_LANE_Y;
	border.visible = WHITE;
	border.action_image = 0;
	dungeon_setup_session();
}

static void dungeon_lane_update() {
	dungeon_tick();
}

static void dungeon_lane_reset() {
	border.visible = BLACK;
	border.action_image = 0;
	dungeon_reset_session();
}

void dungeon_lane_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case DUNGEON_LANE_SETUP: {
		APP_DBG_SIG("DUNGEON_GAME_SCORE_SETUP\n");
		dungeon_lane_setup();
	}
		break;

	case DUNGEON_LANE_LEVEL_UP: {
		APP_DBG_SIG("DUNGEON_GAME_LEVEL_UPDATE\n");
		dungeon_lane_update();
	}
		break;

	case DUNGEON_LANE_CHECK_GAME_OVER: {
		APP_DBG_SIG("DUNGEON_GAME_CHECK_PROGRESS\n");
	}
		break;

	case DUNGEON_LANE_RESET: {
		APP_DBG_SIG("DUNGEON_GAME_SCORE_RESET\n");
		dungeon_lane_reset();
	}
		break;

	default:
		break;
	}
}
