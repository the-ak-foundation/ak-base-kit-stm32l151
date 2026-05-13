/**
 ******************************************************************************
 * @author: An Nguyen Khanh
 * @date:   Start: 13/05/2026
 *          End:   13/05/2026
 ******************************************************************************
**/
#include "scr_leaderboard.h"

static dungeon_game_score_t dungeon_score_board;

static void view_scr_leaderboard();

view_dynamic_t dyn_view_item_leaderboard = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_leaderboard
};

view_screen_t scr_leaderboard = {
	&dyn_view_item_leaderboard,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

static void view_scr_leaderboard() {
	view_render.fillScreen(BLACK);
	view_render.drawRect(0, 0, LCD_WIDTH, LCD_HEIGHT, WHITE);
	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.setCursor(32, 4);
	view_render.print("LEADERBOARD");
	view_render.drawLine(6, 14, 121, 14, WHITE);

	view_render.setCursor(8, 20);
	view_render.print("Best progress");

	view_render.setTextSize(2);
	view_render.setCursor(24, 30);
	view_render.print("L");
	view_render.print(dungeon_score_board.best_level);
	view_render.print(" S");
	view_render.print(dungeon_score_board.best_stage);

	view_render.setTextSize(1);
	view_render.setCursor(8, 52);
	view_render.print("Best score:");
	view_render.print(dungeon_score_board.best_score);
	view_render.setCursor(8, 56);
	view_render.print("MODE = Back");
}

void scr_leaderboard_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case SCREEN_ENTRY: {
		APP_DBG_SIG("SCREEN_ENTRY\n");
		view_render.initialize();
		view_render_display_on();
		eeprom_read(EEPROM_DUNGEON_SCORE_ADDR, (uint8_t*)&dungeon_score_board, sizeof(dungeon_score_board));
		if (dungeon_score_board.best_level > 5) {
			dungeon_score_board.best_level = 0;
			dungeon_score_board.best_stage = 0;
		}
	}
		break;

	case AC_DISPLAY_BUTTON_MODE_RELEASED:
	case AC_DISPLAY_BUTTON_UP_RELEASED:
	case AC_DISPLAY_BUTTON_DOWN_RELEASED: {
		SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
		BUZZER_PlayTones(tones_cc);
	}
		break;

	default:
		break;
	}
}
