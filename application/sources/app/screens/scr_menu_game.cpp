/**
 ******************************************************************************
 * @author: An Nguyen Khanh
 * @date:   Start: 30/04/2026
 *          End:   30/04/2026
 ******************************************************************************
**/
#include "scr_menu_game.h"
/*****************************************************************************/
/* Instructions for adding new items to the menu*/
/*****************************************************************************/
/*  ** Để làm nhanh nhấn "Ctrl + F" để tìm theo từ khóa **
	Bước 1: Chuẩn bị icon cho item sẽ thêm vào kích thước 13-18 px*px
	Bước 2: Thêm 1 item vào menu
			- Tăng số lượng item: tăng "NUMBER_MENU_ITEMS" thêm 1
			- Thêm định danh: tăng "NUMBER_MENU_ITEMS" thêm 1 item
			- Thêm định dạng màu: tăng "MENU_ITEMS_ICON_COLOR" thêm 1 item
	Bước 3: Thêm tên cho item vào "menu_items_name[]"
	Bước 4: Thêm icon cho item:
			- Thêm icon: menu_items_icon[]
			- Thêm kich thước: menu_items_icon_size_w[], menu_items_icon_size_h[]
	Bước 5: Thêm địa chỉ cho item:
			- Thêm 1 item vào: "screen_tran_menu()" rồi điền địa chỉ muốn 
			chuyển tới.
	## Nếu icon không cân xứng vào "menu_items_icon_axis_y[3]" để điều chỉnh lại.
*/

/*****************************************************************************/
/* Variable and Struct Declaration - Menu game */
/*****************************************************************************/
// Screen 
#define STEP_MENU_CHOSSE				(22)
#define NUMBER_MENU_ITEMS				(6)
#define	SCREEN_MENU_H					(64)

enum {
	MENU_ITEM_CONTINUE = 0,
	MENU_ITEM_NEW_GAME,
	MENU_ITEM_LEVEL_INFO,
	MENU_ITEM_LEADERBOARD,
	MENU_ITEM_CREATOR_MODE,
	MENU_ITEM_SETTINGS,
};

// Menu items name
static char menu_items_name[NUMBER_MENU_ITEMS][20] = {
	"   Continue       ",		// item 1
	"   New Game       ",		// item 2
	"   Level Info     ",		// item 3
	"   LeaderBoard    ",		// item 4
	"   Creator Mode   ",		// item 5
	"   Settings       ",		// item 6
};

// Menu items icon
static const uint8_t *menu_items_icon[NUMBER_MENU_ITEMS] = {
	hero_icon,					// item 1
	item_sword,					// item 2
	chart_icon,					// item 3
	item_shield,				// item 4
	item_trap,					// item 5
	setting_icon,				// item 6
};

// Menu items size W
uint8_t menu_items_icon_size_w[NUMBER_MENU_ITEMS] = {
	24,							// item 1
	20,							// item 2
	16,							// item 3
	20,							// item 4
	15,							// item 5
	16,							// item 6
};

uint8_t menu_items_icon_size_h[NUMBER_MENU_ITEMS] = {
	17,							// item 1
	20,							// item 2
	16,							// item 3
	20,							// item 4
	20,							// item 5
	16,							// item 6
};

// Menu items color
uint8_t menu_items_icon_color[NUMBER_MENU_ITEMS];
// Menu items axis Y
uint8_t menu_items_icon_axis_y[3] = {
	2,							// icon frame 1
	24,							// icon frame 2
	46							// icon frame 3
};

// Screen and item location on the Menu
typedef struct {
	int screen;
	int location;
} screen_t;

// Scroll bar
typedef struct {
	uint8_t axis_x = 126;
	uint8_t axis_y = 0;
	uint8_t size_W = 3;
	uint8_t size_h = SCREEN_MENU_H / 3;
} scr_menu_scroll_bar_t;

// Frames
typedef struct {
	uint8_t axis_x = 0;
	uint8_t axis_y = 0;
	uint8_t size_w = 123;
	uint8_t size_h = 20;
	uint8_t size_r = 3;
} scr_menu_frames_t;

screen_t screen_menu;
scr_menu_scroll_bar_t scroll_bar;
scr_menu_frames_t frame_white;
scr_menu_frames_t frame[3];
static uint8_t menu_visible_list[NUMBER_MENU_ITEMS];
static uint8_t menu_visible_count;

static uint8_t menu_item_available(uint8_t item) {
	if (item == MENU_ITEM_CONTINUE) {
		return dungeon_has_save_data();
	}
	return 1;
}

static void menu_rebuild_visible_list() {
	menu_visible_count = 0;
	for (uint8_t item = 0; item < NUMBER_MENU_ITEMS; item++) {
		if (menu_item_available(item)) {
			menu_visible_list[menu_visible_count++] = item;
		}
	}

	if (menu_visible_count == 0) {
		menu_visible_list[0] = MENU_ITEM_NEW_GAME;
		menu_visible_count = 1;
	}
}

/*****************************************************************************/
/* View - Menu game */
/*****************************************************************************/
static void view_scr_menu_game();

view_dynamic_t dyn_view_menu = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_menu_game
};

view_screen_t scr_menu_game = {
	&dyn_view_menu,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

void view_scr_menu_game() {
#define DUNGEON_MENU_ICON_AXIS_X			(7)
#define DUNGEON_MENU_TEXT_AXIS_X			(32)
	// Scroll bar
	view_render.fillRect(	scroll_bar.axis_x - 1, \
							scroll_bar.axis_y, \
							scroll_bar.size_W, \
							scroll_bar.size_h, \
							WHITE);
	view_render.drawBitmap(	scroll_bar.axis_x, \
							0, \
							dot_icon, \
							1, \
							SCREEN_MENU_H, \
							WHITE);
	// Frame White
	view_render.fillRoundRect(	frame_white.axis_x, \
								frame_white.axis_y, \
								frame_white.size_w, \
								frame_white.size_h, \
								frame_white.size_r, \
								WHITE);
	for (uint8_t i = 0; i < 3; i++) {
		uint8_t visible_index = (uint8_t)(screen_menu.screen + i);
	// Frames
		view_render.drawRoundRect(	frame[i].axis_x, \
									frame[i].axis_y, \
									frame[i].size_w, \
									frame[i].size_h, \
									frame[i].size_r, \
									WHITE);
		if (visible_index >= menu_visible_count) {
			continue;
		}

		uint8_t item = menu_visible_list[visible_index];
	// Icon
		view_render.drawBitmap(	DUNGEON_MENU_ICON_AXIS_X, \
								menu_items_icon_axis_y[i], \
								menu_items_icon[item], \
								menu_items_icon_size_w[item], \
								menu_items_icon_size_h[item], \
								menu_items_icon_color[item]);
	}
	// Text Menu
	view_render.setTextSize(1);
	for (uint8_t i = 0; i < 3; i++) {
		uint8_t visible_index = (uint8_t)(screen_menu.screen + i);
		if (visible_index >= menu_visible_count) {
			continue;
		}
		uint8_t item = menu_visible_list[visible_index];
		view_render.setTextColor(menu_items_icon_color[item]);
		view_render.setCursor(DUNGEON_MENU_TEXT_AXIS_X, menu_items_icon_axis_y[i]+5);
		view_render.print(menu_items_name[item]);
	}
}

/*****************************************************************************/
/* Handle - Menu game */
/*****************************************************************************/
void update_menu_screen_chosse() {
	if (menu_visible_count == 0) {
		menu_rebuild_visible_list();
	}

	if (screen_menu.location >= menu_visible_count) {
		screen_menu.location = menu_visible_count - 1;
	}
	if (screen_menu.screen > screen_menu.location) {
		screen_menu.screen = screen_menu.location;
	}

	if (menu_visible_count > 3) {
		uint8_t max_screen = menu_visible_count - 3;
		if (screen_menu.screen > max_screen) {
			screen_menu.screen = max_screen;
		}
	}
	else {
		screen_menu.screen = 0;
	}

	// Frames location
	frame_white.axis_y =frame[screen_menu.location-screen_menu.screen].axis_y;
	frame[0].axis_y = 0;
	frame[1].axis_y = 22;
	frame[2].axis_y = 44;

	for (uint8_t i = 0; i < NUMBER_MENU_ITEMS; i++) {
		menu_items_icon_color[i] = WHITE;
	}
	menu_items_icon_color[menu_visible_list[screen_menu.location]] = BLACK;

	// update scroll bar
	scroll_bar.size_h = (uint8_t)(SCREEN_MENU_H / menu_visible_count);
	if (scroll_bar.size_h < 4) {
		scroll_bar.size_h = 4;
	}
	scroll_bar.axis_y = (uint8_t)(SCREEN_MENU_H*screen_menu.location / menu_visible_count);
}

void screen_tran_menu() {
	uint8_t item = menu_visible_list[screen_menu.location];
	switch (item) {
	case MENU_ITEM_CONTINUE:
			dungeon_prepare_continue();
			SCREEN_TRAN(scr_dungeon_how_to_play_handle, &scr_dungeon_how_to_play);
		break;
	case MENU_ITEM_NEW_GAME:
			dungeon_prepare_new_game();
			SCREEN_TRAN(scr_dungeon_how_to_play_handle,	&scr_dungeon_how_to_play	);
		break;
	case MENU_ITEM_LEVEL_INFO:
			dungeon_set_level_view_mode(DUNGEON_LEVEL_VIEW_INFO);
			SCREEN_TRAN(scr_charts_game_handle,		&scr_charts_game	);
		break;
	case MENU_ITEM_LEADERBOARD:
			SCREEN_TRAN(scr_leaderboard_handle, &scr_leaderboard);
		break;
	case MENU_ITEM_CREATOR_MODE:
			dungeon_set_level_view_mode(DUNGEON_LEVEL_VIEW_CREATOR);
			SCREEN_TRAN(scr_charts_game_handle,		&scr_charts_game	);
		break;
	case MENU_ITEM_SETTINGS:
			SCREEN_TRAN(scr_game_setting_handle,		&scr_game_setting	);
		break;
	
	default:
		break;
	}
}

void scr_menu_game_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case SCREEN_ENTRY: {
		APP_DBG_SIG("SCREEN_ENTRY\n");
		view_render.initialize();
		view_render_display_on();
		menu_rebuild_visible_list();
		screen_menu.screen = 0;
		screen_menu.location = 0;
		update_menu_screen_chosse();
		// timer switch to scr_idle
		timer_set(	AC_TASK_DISPLAY_ID, \
					AC_DISPLAY_SHOW_IDLE, \
					AC_DISPLAY_IDLE_INTERVAL, \
					TIMER_ONE_SHOT);
	}
		break;

	case AC_DISPLAY_SHOW_IDLE: {
		SCREEN_TRAN(scr_idle_handle,&scr_idle);
	}
		break;

	case AC_DISPLAY_BUTTON_MODE_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_MODE_RELEASED\n");
		screen_tran_menu();
	}
		break;

	case AC_DISPLAY_BUTTON_UP_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_UP_RELEASED\n");
		if (screen_menu.location > 0) {
			screen_menu.location--;
		}
		if (screen_menu.location < screen_menu.screen) {
			screen_menu.screen = screen_menu.location;
		}
		update_menu_screen_chosse();
		// Reset timer switch to scr_idle
		timer_set(	AC_TASK_DISPLAY_ID, \
					AC_DISPLAY_SHOW_IDLE, \
					AC_DISPLAY_IDLE_INTERVAL, \
					TIMER_ONE_SHOT);
	}
		BUZZER_PlayTones(tones_cc);
		break;

	case AC_DISPLAY_BUTTON_DOWN_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_DOWN_RELEASED\n");
		// 
		if (screen_menu.location < menu_visible_count-1) {
			screen_menu.location++;
		}
		if (screen_menu.location >= (screen_menu.screen + 3)) {
			screen_menu.screen = screen_menu.location - 2;
		}
		update_menu_screen_chosse();
		// Reset timer switch to scr_idle
		timer_set(	AC_TASK_DISPLAY_ID, \
					AC_DISPLAY_SHOW_IDLE, \
					AC_DISPLAY_IDLE_INTERVAL, \
					TIMER_ONE_SHOT);
	}
		BUZZER_PlayTones(tones_cc);
		break;

	default:
		break;
	}
}
