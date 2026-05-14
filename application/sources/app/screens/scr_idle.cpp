/**
 ******************************************************************************
 * @author: An Nguyen Khanh
 * @date:   Start: 29/04/2026
 *          End:   29/04/2026
 ******************************************************************************
**/
#include "scr_idle.h"

/*****************************************************************************/
/* Variable and Class Declaration - idle */
/*****************************************************************************/
using namespace std;

#define MAX_BALL_DISPLAY	(16)

class ball {
	// rand from a to b
	// (rand() % (b - a + 1)) + a
public:
	static int total;
	int id, x, y, slope, axis_x, axis_y, radius;

	ball() {
		axis_x = 1;
		axis_y = 1;
		slope = (rand() % (31)) - 15;
		radius = (rand() % (7)) + 6;
		x = rand() % (LCD_WIDTH - radius);
		y = rand() % (LCD_HEIGHT - radius);
	}

	int distance(ball& __ball) {
		uint8_t dx, dy;
		dx = abs(x - __ball.x);
		dy = abs(y - __ball.y);
		return sqrt(dx*dx + dy*dy);
	}

	bool is_hit_to_other(ball& __ball) {
		if ((radius + __ball.radius) <= distance(__ball)) {
			return true;
		}
		else {
			return false;
		}
	}

	void moving() {
		if( axis_x > 0) {
			x = x + 2;
		}
		else {
			x = x - 2;
		}

		if (axis_y > 0) {
			y += 2 * atan(slope);
		}
		else {
			y -= 2 * atan(slope);
		}

		if (x > (LCD_WIDTH - radius) || x < radius) {
			axis_x = -axis_x;
			if (x < radius) {
				x = radius;
			}
		}

		if (y > (LCD_HEIGHT - radius) || y < radius ) {
			axis_y = -axis_y;
			if (y < radius) {
				y = radius;
			}
		}
	}
};

/*****************************************************************************/
/* View - idle */
/*****************************************************************************/
static void view_scr_idle();

view_dynamic_t dyn_view_idle = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_idle
};

view_screen_t scr_idle = {
	&dyn_view_idle,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

vector<ball> v_idle_ball;
int ball::total;

void view_scr_idle() {
	for(ball _ball : v_idle_ball) {
		view_render.drawCircle(_ball.x, _ball.y, _ball.radius, 144);
	}
}

/*****************************************************************************/
/* Handle - idle */
/*****************************************************************************/
void scr_idle_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case SCREEN_ENTRY: {
		APP_DBG_SIG("SCREEN_ENTRY\n");
		if (v_idle_ball.empty()) {
			ball new_ball;
			new_ball.id = ball::total++;
			v_idle_ball.push_back(new_ball);
		}

		timer_set(AC_TASK_DISPLAY_ID, \
				AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE, \
				AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE_INTERAL, \
				TIMER_PERIODIC);
	}
		break;

	case AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE: {
		for (unsigned int i = 0; i < v_idle_ball.size(); i++) {
			v_idle_ball[i].moving();
		}
	}
		break;

	case AC_DISPLAY_BUTTON_MODE_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_MODE_RELEASED\n");
		timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE);
		SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
	}
		break;

	case AC_DISPLAY_BUTTON_UP_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_UP_RELEASED\n");
		ball new_ball;
		new_ball.id = ball::total++;

		if (v_idle_ball.empty()) {
			timer_set(AC_TASK_DISPLAY_ID, \
					AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE, \
					AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE_INTERAL, \
					TIMER_PERIODIC);
		}

		if (v_idle_ball.size() < MAX_BALL_DISPLAY) {
			v_idle_ball.push_back(new_ball);
		}
		else {
			BUZZER_PlayTones(tones_3beep);
		}
	}
		break;

	case AC_DISPLAY_BUTTON_DOWN_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_DOWN_RELEASED\n");
		if (v_idle_ball.size()) {
			ball::total--;
			v_idle_ball.pop_back();
		}

		if (v_idle_ball.empty()) {
			timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE);
			SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
		}
	}
		break;

	default:
		break;
	}
}
