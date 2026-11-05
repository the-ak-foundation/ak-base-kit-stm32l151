/**
 ******************************************************************************
 * @author: GaoKong
 * @date:   05/09/2016
 ******************************************************************************
**/
#ifndef __SYS_DBG_H__
#define __SYS_DBG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#include "task.h"
#include "message.h"
#include "sys_ctrl.h"
#include "xprintf.h"

#if defined(SYS_DBG_EN)
#define SYS_DBG(fmt, ...)       xprintf((const char*)fmt, ##__VA_ARGS__)
#else
#define SYS_DBG(fmt, ...)
#endif

#if defined(SYS_PRINT_EN)
#define SYS_PRINT(fmt, ...)       xprintf((const char*)fmt, ##__VA_ARGS__)
#else
#define SYS_PRINT(fmt, ...)
#endif

typedef struct {
	uint32_t ipsr;
	uint32_t primask;
	uint32_t faultmask;
	uint32_t basepri;
	uint32_t control;
} m3_core_reg_t;

typedef struct {
	int8_t		string[10];
	uint8_t		code;
	task_t		current_task;
	ak_msg_t	current_active_object;
	m3_core_reg_t m3_core_reg;
	uint32_t	fatal_times;
	uint32_t	restart_times;
} fatal_log_t;

#define FATAL(s, c) \
	do { \
		DISABLE_INTERRUPTS(); \
		sys_ctrl_shell_sw_to_block(); \
		sys_dbg_fatal((const int8_t*)s, (uint8_t)c); \
	} while (0)

/*
 * Entry for fault/exception handlers (MUST be the only content of a naked function).
 * Pass the stack holding the hardware stacked frame (MSP or PSP, picked by
 * EXC_RETURN bit 2) to sys_dbg_fault_handler(sp, code), without touching the stack.
 * Uses only Thumb-1 instructions, so it builds on any Cortex-M (M0/M0+/M3/M4/M7/M23/M33).
 */
#define SYS_DBG_FAULT_HANDLER_ENTRY(code) \
	__asm volatile ( \
		"movs  r0, #4                   \n" \
		"mov   r1, lr                   \n" \
		"tst   r0, r1                   \n" \
		"beq   1f                       \n" \
		"mrs   r0, psp                  \n" \
		"b     2f                       \n" \
		"1:                             \n" \
		"mrs   r0, msp                  \n" \
		"2:                             \n" \
		"movs  r1, #" #code "           \n" \
		"bl    sys_dbg_fault_handler    \n" \
		"b     .                        \n" \
	)

#define FATAL_PARAM(expr, s, c) \
	do { \
		if (!expr) { \
			DISABLE_INTERRUPTS(); \
			sys_ctrl_shell_sw_to_block(); \
			sys_dbg_fatal((const int8_t*)s, (uint8_t)c); \
		} \
	} while (0)

extern void sys_dbg_fatal(const int8_t* s, uint8_t c);
extern void sys_dbg_fault_handler(uint32_t* sp, uint8_t c);
extern void sys_dbg_func_stack_dump(uint32_t*);
extern void sys_dbg_cpu_dump();
extern void sys_dbg_stack_space_dump();

#ifdef __cplusplus
}
#endif

#endif //__SYS_DBG_H__
