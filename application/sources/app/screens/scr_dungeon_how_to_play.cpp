/**
 ******************************************************************************
 * @author: An Nguyen Khanh
 * @date:   Start: 02/05/2026
 *          End:   02/05/2026
 ******************************************************************************
**/
#include "scr_dungeon_how_to_play.h"

static void view_scr_dungeon_how_to_play();

view_dynamic_t dyn_view_item_dungeon_how_to_play = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_dungeon_how_to_play
};

view_screen_t scr_dungeon_how_to_play = {
	&dyn_view_item_dungeon_how_to_play,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

void view_scr_dungeon_how_to_play() {
	view_render.drawRect(0, 0, LCD_WIDTH, LCD_HEIGHT, WHITE);
	view_render.setTextColor(WHITE);
	view_render.setTextSize(1);

	view_render.setCursor(31, 3);
	view_render.print("DUNGEON");

	view_render.drawLine(4, 13, 123, 13, WHITE);

	view_render.setCursor(4, 17);
	view_render.print("UP   : MOVE SELECT");
	view_render.setCursor(4, 27);
	view_render.print("DOWN : MOVE SELECT");
	view_render.setCursor(4, 37);
	view_render.print("MODE : CONFIRM");

	view_render.setCursor(4, 49);
	if (dungeon_is_creator_mode()) {
		view_render.print("CREATOR TEST (NO SAVE)");
	}
	else {
		view_render.print("TRAVEL, BATTLE, LOOT");
	}
	view_render.setCursor(17, 57);
	view_render.print("MODE TO BEGIN");
}

void scr_dungeon_how_to_play_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case SCREEN_ENTRY: {
		APP_DBG_SIG("SCREEN_ENTRY\n");
		dungeon_game_state = GAME_OFF;
		view_render.initialize();
		view_render_display_on();
	}
		break;

	case AC_DISPLAY_BUTTON_MODE_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_MODE_RELEASED\n");
		if ((dungeon_start_mode == DUNGEON_START_CONTINUE) && (dungeon_has_save_data() == 0)) {
			dungeon_prepare_new_game();
		}
		SCREEN_TRAN(scr_dungeon_game_handle, &scr_dungeon_game);
	}
		BUZZER_PlayTones(tones_cc);
		break;

	case AC_DISPLAY_BUTTON_UP_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_UP_RELEASED\n");
		SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
	}
		BUZZER_PlayTones(tones_cc);
		break;

	case AC_DISPLAY_BUTTON_DOWN_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_DOWN_RELEASED\n");
		SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
	}
		BUZZER_PlayTones(tones_cc);
		break;

	default:
		break;
	}
}
