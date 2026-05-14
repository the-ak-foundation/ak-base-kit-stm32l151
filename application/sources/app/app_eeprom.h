/**
 ******************************************************************************
 * @author: An Nguyen Khanh
 * @date:   Start: 28/04/2026
 *          End:   28/04/2026
 ******************************************************************************
**/
#ifndef __APP_EEPROM_H__
#define __APP_EEPROM_H__

#include <stdint.h>
#include "app.h"

/**
  *****************************************************************************
  * EEPROM define address.
  *
  *****************************************************************************
  */
#define EEPROM_START_ADDR           (0X0000)
#define EEPROM_END_ADDR             (0X1000)

#define EEPROM_SCORE_START_ADDR     (0X0010)
#define EEPROM_SCORE_PLAY_ADDR      (0X00FA)

#define EEPROM_SETTING_START_ADDR   (0X0100)
#define EEPROM_DUNGEON_SCORE_ADDR   (0X0120)
#define EEPROM_DUNGEON_SAVE_ADDR    (0X0140)

#define DUNGEON_SAVE_MAGIC          (0x44554E47UL)

/******************************************************************************/
/* Dungeon game */
/******************************************************************************/
typedef struct {
  /* score data */
  uint32_t score_now;
  uint32_t score_1st;
  uint32_t score_2nd;
  uint32_t score_3rd;
} dungeon_game_history_t;

typedef struct {
  /* setting data */
  bool silent;
  uint8_t num_arrow;
  uint8_t arrow_speed;
  uint8_t meteoroid_speed;
} dungeon_game_setting_t;

typedef struct {
  uint32_t best_score;
  uint8_t best_level;
  uint8_t best_stage;
  uint8_t reserved[3];
} dungeon_game_score_t;

typedef struct {
  uint32_t magic;
  uint8_t valid;
  uint8_t level;
  uint8_t stage;
  uint8_t total_stages;
  uint16_t score;
  int16_t player_hp;
  int16_t player_max_hp;
  int16_t player_atk;
  int16_t player_def;
  int16_t monster_hp;
  int16_t monster_max_hp;
  int16_t monster_dmg;
  int16_t monster_armor;
  uint8_t current_view;
  uint8_t current_monster;
  uint8_t selected_action;
  uint8_t selected_support_item;
  uint8_t poison_turns;
  uint8_t burn_turns;
  uint8_t curse_turns;
  uint8_t enemy_poison_turns;
  uint8_t enemy_poison_damage;
  uint8_t monster_dodge_ready;
  uint8_t defend_active;
  uint8_t support_event;
  uint8_t support_pending;
  uint8_t battle_turn;
  uint8_t travel_progress;
  uint8_t inventory[7];
  uint8_t chest_options[3];
} dungeon_game_save_t;

#endif //__APP_EEPROM_H__
