/**
 ******************************************************************************
 * @author: An Nguyen Khanh
 * @date:   Start: 04/05/2026
 *          End:   04/05/2026
 ******************************************************************************
**/
#include "dungeon_action.h"

dungeon_action_t arrow[MAX_NUM_ARROW];

static bool dungeon_action_finished;

bool dungeon_action_is_active() {
	return (arrow[0].visible == WHITE);
}

bool dungeon_action_consume_finished() {
	bool finished = dungeon_action_finished;
	dungeon_action_finished = false;
	return finished;
}

static void dungeon_action_setup() {
	arrow[0].visible = BLACK;
	arrow[0].x = 0;
	arrow[0].y = 0;
	arrow[0].vx = 0;
	arrow[0].power = 0;
	arrow[0].action_image = 0;
	dungeon_action_finished = false;
}

static void dungeon_action_finish_roll() {
	arrow[0].visible = BLACK;
	arrow[0].vx = 0;
	arrow[0].action_image = 0;
	dungeon_action_finished = true;
}

static void dungeon_action_run() {
	(void)arrow;
	if (arrow[0].visible == BLACK) {
		dungeon_action_finish_roll();
	}
}

static void dungeon_action_shoot() {
	if (dungeon_game_state != GAME_PLAY) {
		BUZZER_PlayTones(tones_3beep);
		return;
	}

	dungeon_confirm_action();
	dungeon_action_finished = false;
}

void dungeon_action_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case DUNGEON_ACTION_SETUP: {
		APP_DBG_SIG("DUNGEON_GAME_BALL_SETUP\n");
		dungeon_action_setup();
	}
		break;

	case DUNGEON_ACTION_RUN: {
		APP_DBG_SIG("DUNGEON_GAME_BALL_RUN\n");
		dungeon_action_run();
	}
		break;

	case DUNGEON_ACTION_SHOOT: {
		APP_DBG_SIG("DUNGEON_GAME_BALL_THROW\n");
		dungeon_action_shoot();
	}
		break;

	case DUNGEON_ACTION_RESET: {
		APP_DBG_SIG("DUNGEON_GAME_BALL_RESET\n");
		dungeon_action_setup();
	}
		break;

	default:
		break;
	}
}
