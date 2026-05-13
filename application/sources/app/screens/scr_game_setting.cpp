/**
 ******************************************************************************
 * @author: An Nguyen Khanh
 * @date:   Start: 30/04/2026
 *          End:   01/05/2026
 ******************************************************************************
**/
#include "scr_game_setting.h"

/*****************************************************************************/
/* Variable Declaration - Setting game */
/*****************************************************************************/
dungeon_game_setting_t settingdata;
static uint8_t setting_location_chosse;

static void bw_sanitize_setting_local() {
	if ((settingdata.num_arrow == 0) || (settingdata.num_arrow > 5)) {
		settingdata.num_arrow = 3;
	}

	if ((settingdata.meteoroid_speed == 0) || (settingdata.meteoroid_speed > 5)) {
		settingdata.meteoroid_speed = 2;
	}

	if ((settingdata.arrow_speed == 0) || (settingdata.arrow_speed > 10)) {
		settingdata.arrow_speed = 4;
	}
}

/*****************************************************************************/
/* View - Setting game */
/*****************************************************************************/
static void view_scr_game_setting();

view_dynamic_t dyn_view_item_game_setting = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_game_setting
};

view_screen_t scr_game_setting = {
	&dyn_view_item_game_setting,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

void view_scr_game_setting() {
	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.drawBitmap(0, setting_location_chosse - DUNGEON_SETTING_CHOSSE_ICON_AXIS_Y, chosse_icon, DUNGEON_SETTING_CHOSSE_ICON_SIZE_W, DUNGEON_SETTING_CHOSSE_ICON_SIZE_H, WHITE);
	if (settingdata.silent == 0) {
		view_render.drawBitmap(109, DUNGEON_SETTING_FRAMES_AXIS_Y_1 + DUNGEON_SETTING_FRAMES_STEP * 3 - 12, speaker_1, 7, 7, WHITE);
	}
	else {
		view_render.drawBitmap(109, DUNGEON_SETTING_FRAMES_AXIS_Y_1 + DUNGEON_SETTING_FRAMES_STEP * 3 - 12, speaker_2, 7, 7, WHITE);
	}

	view_render.drawRoundRect(DUNGEON_SETTING_FRAMES_AXIS_X, DUNGEON_SETTING_FRAMES_AXIS_Y_1, DUNGEON_SETTING_FRAMES_SIZE_W, DUNGEON_SETTING_FRAMES_SIZE_H, DUNGEON_SETTING_FRAMES_SIZE_R, WHITE);
	view_render.drawRoundRect(DUNGEON_SETTING_FRAMES_AXIS_X, DUNGEON_SETTING_FRAMES_AXIS_Y_1 + DUNGEON_SETTING_FRAMES_STEP, DUNGEON_SETTING_FRAMES_SIZE_W, DUNGEON_SETTING_FRAMES_SIZE_H, DUNGEON_SETTING_FRAMES_SIZE_R, WHITE);
	view_render.drawRoundRect(DUNGEON_SETTING_FRAMES_AXIS_X, DUNGEON_SETTING_FRAMES_AXIS_Y_1 + DUNGEON_SETTING_FRAMES_STEP * 2, DUNGEON_SETTING_FRAMES_SIZE_W, DUNGEON_SETTING_FRAMES_SIZE_H, DUNGEON_SETTING_FRAMES_SIZE_R, WHITE);
	view_render.drawRoundRect(DUNGEON_SETTING_FRAMES_AXIS_X, DUNGEON_SETTING_FRAMES_AXIS_Y_1 + DUNGEON_SETTING_FRAMES_STEP * 3, DUNGEON_SETTING_FRAMES_SIZE_W, DUNGEON_SETTING_FRAMES_SIZE_H, DUNGEON_SETTING_FRAMES_SIZE_R, WHITE);

	view_render.setCursor(DUNGEON_SETTING_TEXT_AXIS_X, 5);
	view_render.print(" Frames       ( ) ");
	view_render.setCursor(DUNGEON_SETTING_NUMBER_AXIS_X, 5);
	view_render.print(settingdata.num_arrow);

	view_render.setCursor(DUNGEON_SETTING_TEXT_AXIS_X, 20);
	view_render.print(" Lane speed   ( ) ");
	view_render.setCursor(DUNGEON_SETTING_NUMBER_AXIS_X, 20);
	view_render.print(settingdata.meteoroid_speed);

	view_render.setCursor(DUNGEON_SETTING_TEXT_AXIS_X, 35);
	view_render.print(" Silent           ");

	view_render.setCursor(DUNGEON_SETTING_TEXT_AXIS_X + 32, 50);
	view_render.print(" EXIT ");
	view_render.update();
}

/*****************************************************************************/
/* Handle - Setting game */
/*****************************************************************************/
void scr_game_setting_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case SCREEN_ENTRY: {
		APP_DBG_SIG("SCREEN_ENTRY\n");
		view_render.clear();
		setting_location_chosse = SETTING_ITEM_ARRDESS_1;
		eeprom_read(EEPROM_SETTING_START_ADDR, (uint8_t*)&settingdata, sizeof(settingdata));
		bw_sanitize_setting_local();
	}
		break;

	case AC_DISPLAY_BUTTON_MODE_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_MODE_RELEASED\n");
		switch (setting_location_chosse) {
		case SETTING_ITEM_ARRDESS_1: {
			settingdata.num_arrow++;
			if (settingdata.num_arrow > 5) {
				settingdata.num_arrow = 1;
			}
		}
			break;

		case SETTING_ITEM_ARRDESS_2: {
			settingdata.meteoroid_speed++;
			if (settingdata.meteoroid_speed > 5) {
				settingdata.meteoroid_speed = 1;
			}
		}
			break;

		case SETTING_ITEM_ARRDESS_3: {
			settingdata.silent = !settingdata.silent;
			BUZZER_Sleep(settingdata.silent);
		}
			break;

		case SETTING_ITEM_ARRDESS_4: {
			settingdata.arrow_speed = 4;
			eeprom_write(EEPROM_SETTING_START_ADDR, (uint8_t*)&settingdata, sizeof(settingdata));
			SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
			BUZZER_PlayTones(tones_startup);
		}
			break;

		default:
			break;
		}
	}
		BUZZER_PlayTones(tones_cc);
		break;

	case AC_DISPLAY_BUTTON_UP_LONG_PRESSED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_UP_LONG_PRESSED\n");
		settingdata.num_arrow = 5;
		settingdata.meteoroid_speed = 5;
		settingdata.silent = 0;
	}
		BUZZER_Sleep(settingdata.silent);
		BUZZER_PlayTones(tones_cc);
		break;

	case AC_DISPLAY_BUTTON_UP_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_UP_RELEASED\n");
		setting_location_chosse -= STEP_SETTING_CHOSSE;
		if (setting_location_chosse == SETTING_ITEM_ARRDESS_0) {
			setting_location_chosse = SETTING_ITEM_ARRDESS_4;
		}
	}
		BUZZER_PlayTones(tones_cc);
		break;

	case AC_DISPLAY_BUTTON_DOWN_LONG_PRESSED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_DOWN_LONG_PRESSED\n");
		settingdata.num_arrow = 1;
		settingdata.meteoroid_speed = 1;
		settingdata.silent = 1;
	}
		BUZZER_Sleep(settingdata.silent);
		BUZZER_PlayTones(tones_cc);
		break;

	case AC_DISPLAY_BUTTON_DOWN_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_DOWN_RELEASED\n");
		setting_location_chosse += STEP_SETTING_CHOSSE;
		if (setting_location_chosse > SETTING_ITEM_ARRDESS_4) {
			setting_location_chosse = SETTING_ITEM_ARRDESS_1;
		}
	}
		BUZZER_PlayTones(tones_cc);
		break;

	default:
		break;
	}
}
