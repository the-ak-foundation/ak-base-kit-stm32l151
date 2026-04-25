#include "qrcode.h"
#include "scr_info.h"

static void view_scr_info();

view_dynamic_t dyn_view_info = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_info
};

view_screen_t scr_info = {
	&dyn_view_info,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

class line_segment {
public:
	int x0, x1;
	int distance() {
		return (x1 - x0);
	}
};

void view_scr_info() {
	/* link base kit */
	QRCode qrcode;
	uint8_t qrcodeData[qrcode_getBufferSize(8)];
	qrcode_initText(&qrcode, qrcodeData, 8, 0, "https://github.com/ak-embedded-software/ak-base-kit-stm32l151");

	for (uint8_t y = 0; y < qrcode.size; y++) {
		for (uint8_t x = 0; x < qrcode.size; x++) {
			view_render.drawPixel(x+40, y+2, qrcode_getModule(&qrcode, x, y) ? 1 : 0);
		}
	}
}

void scr_info_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case SCREEN_ENTRY: {
		APP_DBG_SIG("SCREEN_ENTRY\n");
		timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE, AC_DISPLAY_LOGO_INTERVAL, TIMER_ONE_SHOT);
	}
		break;

	case AC_DISPLAY_SHOW_IDLE: {
		APP_DBG_SIG("AC_DISPLAY_SHOW_IDLE\n");
		SCREEN_TRAN(scr_idle_handle, &scr_idle);
	}

	case AC_DISPLAY_BUTON_MODE_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTON_MODE_RELEASED\n");
		SCREEN_TRAN(scr_idle_handle, &scr_idle);
	}
		break;

	default:
		break;
	}
}
