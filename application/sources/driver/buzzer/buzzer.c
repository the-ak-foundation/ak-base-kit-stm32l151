#include <stm32l1xx_rcc.h>
#include <stm32l1xx_gpio.h>
#include <misc.h>
#include <buzzer.h>

volatile       uint32_t          _beep_duration;
volatile       bool              _tones_playing;
volatile const Tone_TypeDef     *_tones;
volatile       bool              _buzzer_sleep;

GPIO_InitTypeDef GPIO_InitStructure;

void buzzer_irq( void ) {
	if (BUZZER_TIM->SR & TIM_SR_UIF) {
		BUZZER_TIM->SR &= ~TIM_SR_UIF; // Clear the TIMx's interrupt pending bit

		_beep_duration--;
		if (_beep_duration == 0) {
			if (_tones_playing) {
				// Currently playing tones, take next tone
				_tones++;
				if (_tones->frequency == 0 && _tones->duration == 0) {
					// Last tone in sequence
					BUZZER_Disable();
					_tones_playing = false;
					_tones = NULL;
				} else {
					if (_tones->frequency == 0) {
						// Silence period
						BUZZER_TIM->ARR = SystemCoreClock / (100 * BUZZER_TIM->PSC) - 1;
						BUZZER_TIM->CCR3 = 0; // 0% duty cycle
						_beep_duration = _tones->duration + 1;
					} else {
						// Play next tone in sequence
						BUZZER_Enable(_tones->frequency,_tones->duration);
					}
				}
			} else {
				BUZZER_Disable();
			}
		}
	}
}

// Initialize buzzer output
void BUZZER_Init(void) {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStruct;

	RCC_APB1PeriphClockCmd(BUZZER_TIM_PERIPH, ENABLE);

	GPIO_InitStructure.GPIO_Pin = BUZZER_IO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(BUZZER_IO_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(BUZZER_IO_PORT, BUZZER_IO_SOURCE, BUZZER_IO_AF);
	GPIO_ResetBits(BUZZER_IO_PORT, BUZZER_IO_PIN);

	TIM_DeInit(BUZZER_TIM);
	TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock / 4000000;
	TIM_TimeBaseStructure.TIM_Period = 999;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(BUZZER_TIM, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = 499;

	TIM_OC3Init(BUZZER_TIM, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(BUZZER_TIM, TIM_OCPreload_Enable);

	NVIC_InitStruct.NVIC_IRQChannel = BUZZER_TIM_IRQ;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Init(&NVIC_InitStruct);

	TIM_ARRPreloadConfig(BUZZER_TIM, ENABLE);
	TIM_ITConfig(BUZZER_TIM, TIM_IT_Update, ENABLE);

	/* BUZZER TIM enable counter */
	TIM_Cmd(BUZZER_TIM, ENABLE);

	/* BUZZER disable */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(BUZZER_IO_PORT, &GPIO_InitStructure);
}

// Turn on buzzer with specified frequency
// input:
//   freq - PWM frequency for buzzer (Hz)
//   duration - duration of buzzer work (tens ms: 1 -> 10ms sound duration)
void BUZZER_Enable(uint16_t freq, uint32_t duration) {
	if (freq < 100 || freq > 8000 || duration == 0) {
		BUZZER_Disable();
	} else {
		_beep_duration = (freq / 100) * duration + 1;

		// Configure buzzer pin
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(BUZZER_IO_PORT, &GPIO_InitStructure);

		// Configure and enable PWM timer
		RCC->APB1ENR |= BUZZER_TIM_PERIPH; // Enable TIMx peripheral
		BUZZER_TIM->ARR = SystemCoreClock / (freq * BUZZER_TIM->PSC) - 1;
		BUZZER_TIM->CCR3 = BUZZER_TIM->ARR >> 1; // 50% duty cycle
		BUZZER_TIM->CR1 |= TIM_CR1_CEN; // Counter enable
	}
}

// Turn off buzzer
void BUZZER_Disable(void) {
	// Counter disable
	BUZZER_TIM->CR1 &= ~TIM_CR1_CEN;
	// Disable TIMx peripheral to conserve power
	RCC->APB1ENR &= ~BUZZER_TIM_PERIPH;
	// Configure buzzer pin as analog input without pullup to conserve power
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(BUZZER_IO_PORT, &GPIO_InitStructure);
}

// Start playing tones sequence
// input:
//   tones - pointer to tones array
void BUZZER_PlayTones(const Tone_TypeDef * tones) {
	if (_buzzer_sleep == 0) {
		if (_tones == NULL) {
			_tones = tones;
			_tones_playing = true;
			BUZZER_Enable(_tones->frequency,_tones->duration);
		}
	}
}

// Off Buzzer --- Buu
void BUZZER_Sleep(bool sleep) {
#define ON	(1)
#define OFF	(0)
	if (sleep == OFF) {
		_buzzer_sleep = OFF;
	}
	else {
		_buzzer_sleep = ON;
	}
}