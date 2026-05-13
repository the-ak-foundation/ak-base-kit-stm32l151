#ifndef __BUZZER_H__
#define __BUZZER_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "io_cfg.h"

// Single tone definition
typedef struct {
	uint16_t frequency;
	uint8_t  duration;
} Tone_TypeDef;

static const Tone_TypeDef tones_cc[] = {
	{2000,2}, 
	{   0,0}, 
};

static const Tone_TypeDef tones_BUM[] = {
	{3000,3},
	{4500,6},
	{   0,0}
};

static const Tone_TypeDef tones_USB_con[] = {
	{ 400,4},
	{   0,1},
	{1600,2},
	{   0,0}
};

static const Tone_TypeDef tones_USB_dis[] = {
	{1600,4},
	{   0,1},
	{ 400,2},
	{   0,0}
};

static const Tone_TypeDef tones_Lets_go[] = {
	{ 262,100},
	{ 330,100},
	{ 392,100},
	{ 349,100},
	{ 330,100},
	{ 392,100},
	{  44,100},
	{   0,200},
	{   0,200},
	{   0,0}     // <-- tones end
};

static const Tone_TypeDef tones_startup[] = {
	{2000,3},
	{   0,3},
	{3000,3},
	{   0,3},
	{4000,3},
	{   0,3},
	{1200,4},
	{   0,6},
	{4500,6},
	{   0,0}     // <-- tones end
};

static const Tone_TypeDef tones_3beep[] = {
	{4000, 3},
	{   0,10},
	{1000, 6},
	{   0,10},
	{4000, 3},
	{   0, 0}
};

// "Super Mario bros." =)
static const Tone_TypeDef tones_SMB[] = {
	{2637,18}, // E7 x2
	{   0, 9}, // x3
	{2637, 9}, // E7
	{   0, 9}, // x3
	{2093, 9}, // C7
	{2637, 9}, // E7
	{   0, 9}, // x3
	{3136, 9}, // G7
	{   0,27}, // x3
	{1586, 9}, // G6
	{   0,27}, // x3

	{2093, 9}, // C7
	{   0,18}, // x2
	{1586, 9}, // G6
	{   0,18}, // x2
	{1319, 9}, // E6
	{   0,18}, // x2
	{1760, 9}, // A6
	{   0, 9}, // x1
	{1976, 9}, // B6
	{   0, 9}, // x1
	{1865, 9}, // AS6
	{1760, 9}, // A6
	{   0, 9}, // x1

	{1586,12}, // G6
	{2637,12}, // E7
	{3136,12}, // G7
	{3520, 9}, // A7
	{   0, 9}, // x1
	{2794, 9}, // F7
	{3136, 9}, // G7
	{   0, 9}, // x1
	{2637, 9}, // E7
	{   0, 9}, // x1
	{2093, 9}, // C7
	{2349, 9}, // D7
	{1976, 9}, // B6
	{   0,18}, // x2

	{2093, 9}, // C7
	{   0,18}, // x2
	{1586, 9}, // G6
	{   0,18}, // x2
	{1319, 9}, // E6
	{   0,18}, // x2
	{1760, 9}, // A6
	{   0, 9}, // x1
	{1976, 9}, // B6
	{   0, 9}, // x1
	{1865, 9}, // AS6
	{1760, 9}, // A6
	{   0, 9}, // x1

	{1586,12}, // G6
	{2637,12}, // E7
	{3136,12}, // G7
	{3520, 9}, // A7
	{   0, 9}, // x1
	{2794, 9}, // F7
	{3136, 9}, // G7
	{   0, 9}, // x1
	{2637, 9}, // E7
	{   0, 9}, // x1
	{2093, 9}, // C7
	{2349, 9}, // D7
	{1976, 9}, // B6

	{   0, 0}
};

// Merry Christmas
static const Tone_TypeDef tones_merryChristmas[] = {
	{2637, 9}, // E7
	{   0, 9}, // x1
	{2637, 9}, // E7
	{   0, 9}, // x1
	{2637, 9}, // E7
	{   0,18}, // x2

	{2637, 9}, // E7
	{   0, 9}, // x1
	{2637, 9}, // E7
	{   0, 9}, // x1
	{2637, 9}, // E7
	{   0,18}, // x2

	{2637, 9}, // E7
	{   0, 9}, // x1
	{3136, 9}, // G7
	{   0, 9}, // x1
	{2093, 9}, // C7
	{   0, 9}, // x1
	{2349, 9}, // D7
	{   0, 9}, // x1
	{2637, 9}, // E7
	{   0,24}, // x2

	{2794, 9}, // F7
	{   0, 9}, // x1
	{2794, 9}, // F7
	{   0, 9}, // x1
	{2794, 9}, // F7
	{   0, 9}, // x1
	{2794, 9}, // F7
	{   0, 9}, // x1
	{2794, 9}, // F7
	{   0, 9}, // x1
	{2637, 9}, // E7
	{   0, 9}, // x1
	{2637, 9}, // E7
	{   0, 9}, // x1
	{2637, 9}, // E7
	{   0, 9}, // x1
	{2637, 9}, // E7
	{   0, 9}, // x1
	{2637, 9}, // E7
	{   0, 9}, // x1
	{2349, 9}, // D7
	{   0, 9}, // x1
	{2349, 9}, // D7
	{   0, 9}, // x1
	{2637, 9}, // E7
	{   0, 9}, // x1
	{2349, 9}, // D7
	{   0, 9}, // x1
	{3136, 9}, // G7
	{   0, 0}  // <-- tones end
};

// Function prototypes
void BUZZER_Init(void);
void BUZZER_Enable(uint16_t freq, uint32_t duration);
void BUZZER_Disable(void);
void BUZZER_PlayTones(const Tone_TypeDef * melody);
void BUZZER_Sleep(bool sleep);

extern void buzzer_irq( void );

#ifdef __cplusplus
}
#endif

#endif // __BUZZER_H__
