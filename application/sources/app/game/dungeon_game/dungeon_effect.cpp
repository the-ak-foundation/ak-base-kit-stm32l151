/**
 ******************************************************************************
 * @author: An Nguyen Khanh
 * @date:   Start: 04/05/2026
 *          End:   04/05/2026
 ******************************************************************************
**/
#include "dungeon_effect.h"

dungeon_effect_t bang[NUM_BANG];

static void dungeon_effect_setup() {
	for (uint8_t i = 0; i < NUM_BANG; i++) {
		bang[i].x = 0;
		bang[i].y = 0;
		bang[i].life = 0;
		bang[i].visible = BLACK;
		bang[i].action_image = 0;
	}
}

void dungeon_spawn_hit_effect(int16_t x, int16_t y) {
	for (uint8_t i = 0; i < NUM_BANG; i++) {
		if (bang[i].visible == BLACK) {
			bang[i].x = x;
			bang[i].y = y;
			bang[i].life = 5;
			bang[i].visible = WHITE;
			bang[i].action_image = 1;
			break;
		}
	}
}

static void dungeon_effect_update() {
	for (uint8_t i = 0; i < NUM_BANG; i++) {
		if (bang[i].visible == WHITE) {
			if (bang[i].life > 0) {
				bang[i].life--;
				bang[i].action_image++;
				if (bang[i].action_image > 3) {
					bang[i].action_image = 1;
				}
			}
			else {
				bang[i].visible = BLACK;
				bang[i].action_image = 0;
			}
		}
	}
}

void dungeon_effect_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case DUNGEON_EFFECT_SETUP: {
		APP_DBG_SIG("DUNGEON_GAME_SPARK_SETUP\n");
		dungeon_effect_setup();
	}
		break;

	case DUNGEON_EFFECT_UPDATE: {
		APP_DBG_SIG("DUNGEON_GAME_SPARK_UPDATE\n");
		dungeon_effect_update();
	}
		break;

	case DUNGEON_EFFECT_RESET: {
		APP_DBG_SIG("DUNGEON_GAME_SPARK_RESET\n");
		dungeon_effect_setup();
	}
		break;

	default:
		break;
	}
}
