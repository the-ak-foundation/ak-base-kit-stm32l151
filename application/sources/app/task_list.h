/**
 ******************************************************************************
 * @author: An Nguyen Khanh
 * @date:   Start: 27/04/2026
 *          End:   27/04/2026
 ******************************************************************************
**/
#ifndef __TASK_LIST_H__
#define __TASK_LIST_H__

#include "ak.h"
#include "task.h"

extern const task_t app_task_table[];
extern const task_polling_t app_task_polling_table[];

/*****************************************************************************/
/*  DECLARE: Internal Task ID
 *  Note: Task id MUST be increasing order.
 */
/*****************************************************************************/
enum {
	/* SYSTEM TASKS */
	TASK_TIMER_TICK_ID,

	/* APP TASKS */
	AC_TASK_FW_ID,
	AC_TASK_SHELL_ID,
	AC_TASK_LIFE_ID,
	AC_TASK_IF_ID,
	AC_TASK_UART_IF_ID,
	AC_TASK_DISPLAY_ID,

	/* DUNGEON GAME ID */
	DUNGEON_STATE_ID,
	DUNGEON_LANE_ID,
	DUNGEON_CONTROL_ID,
	DUNGEON_ACTION_ID,
	DUNGEON_EFFECT_ID,
	DUNGEON_SCREEN_ID,

	/* LINK */
#if defined (IF_LINK_UART_EN)
	AC_LINK_PHY_ID,
	AC_LINK_MAC_ID,
	AC_LINK_ID,
#endif

	/* EOT task ID */
	AK_TASK_EOT_ID,
};

/*****************************************************************************/
/*  DECLARE: Internal Polling Task ID
 *  Note: Task id MUST be increasing order.
 */
/*****************************************************************************/
enum {
	/* APP TASKS */
	AC_TASK_POLLING_CONSOLE_ID,

	/* EOT polling task ID */
	AK_TASK_POLLING_EOT_ID,
};

/*****************************************************************************/
/*  DECLARE: Task entry point
 */
/*****************************************************************************/
/* APP TASKS */
extern void task_fw(ak_msg_t*);
extern void task_shell(ak_msg_t*);
extern void task_life(ak_msg_t*);
extern void task_if(ak_msg_t*);
extern void task_uart_if(ak_msg_t*);
extern void task_display(ak_msg_t*);

/* DUNGEON GAME HANDLE */
extern void dungeon_control_handle(ak_msg_t*);
extern void dungeon_action_handle(ak_msg_t*);
extern void dungeon_state_handle(ak_msg_t*);
extern void dungeon_lane_handle(ak_msg_t*);
extern void dungeon_effect_handle(ak_msg_t*);
extern void scr_dungeon_game_handle(ak_msg_t*);

/* LINK TASK */
extern void task_link_phy(ak_msg_t*);
extern void task_link_mac(ak_msg_t*);
extern void task_link(ak_msg_t*);

/*****************************************************************************/
/*  DECLARE: Task polling
 */
/*****************************************************************************/
extern void task_polling_console();

#endif //__TASK_LIST_H__
