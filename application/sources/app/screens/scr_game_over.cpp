/**
 ******************************************************************************
 * @author: An Nguyen Khanh
 * @date:   Start: 01/05/2026
 *          End:   01/05/2026
 ******************************************************************************
**/
#include "scr_game_over.h"

/*****************************************************************************/
/* Variable Declaration - game over */
/*****************************************************************************/
static dungeon_game_score_t dungeon_score_board;
static uint32_t dungeon_score_now;

/*****************************************************************************/
/* View - game over */
/*****************************************************************************/
static void view_scr_game_over();

view_dynamic_t dyn_view_item_game_over = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_game_over
};

view_screen_t scr_game_over = {
	&dyn_view_item_game_over,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

void view_scr_game_over() {
	view_render.drawRect(0, 0, LCD_WIDTH, LCD_HEIGHT, WHITE);
	view_render.setTextSize(2);
	view_render.setTextColor(WHITE);
	view_render.setCursor(8, 8);
	if (dungeon_last_outcome == DUNGEON_OUTCOME_WIN) {
		view_render.print("LEVEL");
		view_render.setCursor(24, 28);
		view_render.print("CLEAR");
	}
	else {
		view_render.print("GAME");
		view_render.setCursor(24, 28);
		view_render.print("OVER");
	}

	view_render.setTextSize(1);
	view_render.setCursor(8, 48);
	view_render.print("Score:");
	view_render.print(dungeon_score_now);
	view_render.setCursor(8, 57);
	view_render.print("Best :");
	view_render.print(dungeon_score_board.best_score);
}

/*****************************************************************************/
/* Handle - game over */
/*****************************************************************************/
void rank_ranking() {
	if (dungeon_is_creator_mode()) {
		return;
	}

	if (dungeon_score_now > dungeon_score_board.best_score) {
		dungeon_score_board.best_score = dungeon_score_now;
		dungeon_score_board.best_level = dungeon_selected_level;
		dungeon_score_board.best_stage = dungeon_get_total_stages();
	}
}

void scr_game_over_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case SCREEN_ENTRY: {
		APP_DBG_SIG("SCREEN_ENTRY\n");
		view_render.initialize();
		view_render_display_on();
		eeprom_read(	EEPROM_DUNGEON_SCORE_ADDR, \
						(uint8_t*)&dungeon_score_board, \
						sizeof(dungeon_score_board));
		eeprom_read(	EEPROM_SCORE_PLAY_ADDR, \
						(uint8_t*)&dungeon_score_now, \
						sizeof(dungeon_score_now));
		if (dungeon_is_creator_mode()) {
			dungeon_score_now = 0;
		}
		rank_ranking();
	}
		break;

	case AC_DISPLAY_BUTTON_MODE_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_MODE_RELEASED\n");
		eeprom_write(	EEPROM_DUNGEON_SCORE_ADDR, \
						(uint8_t*)&dungeon_score_board, \
						sizeof(dungeon_score_board));
		SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
	}
		BUZZER_PlayTones(tones_cc);
		break;

	case AC_DISPLAY_BUTTON_UP_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_UP_RELEASED\n");
		eeprom_write(	EEPROM_DUNGEON_SCORE_ADDR, \
						(uint8_t*)&dungeon_score_board, \
						sizeof(dungeon_score_board));
		SCREEN_TRAN(scr_charts_game_handle, &scr_charts_game);
	}
		BUZZER_PlayTones(tones_cc);
		break;

	case AC_DISPLAY_BUTTON_DOWN_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_DOWN_RELEASED\n");
		eeprom_write(	EEPROM_DUNGEON_SCORE_ADDR, \
						(uint8_t*)&dungeon_score_board, \
						sizeof(dungeon_score_board));
		dungeon_prepare_level(dungeon_selected_level);
		SCREEN_TRAN(scr_dungeon_how_to_play_handle, &scr_dungeon_how_to_play );
	}	
		BUZZER_PlayTones(tones_cc);
		break;

	default:
		break;
	}
}