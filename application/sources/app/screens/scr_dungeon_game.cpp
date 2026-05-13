/**
 ******************************************************************************
 * @author: An Nguyen Khanh
 * @date:   Start: 05/05/2026
 *          End:   13/05/2026
 ******************************************************************************
**/
#include "scr_dungeon_game.h"

#include <stdio.h>
#include <string.h>

#include "screens.h"

uint8_t dungeon_game_state;
dungeon_game_setting_t settingsetup;
uint8_t dungeon_start_mode = DUNGEON_START_NEW_GAME;
uint8_t dungeon_selected_level = 1;
uint8_t dungeon_last_outcome = DUNGEON_OUTCOME_NONE;
static uint8_t dungeon_persist_enabled = 1;

enum {
	DUNGEON_VIEW_TRAVEL = 0,
	DUNGEON_VIEW_CHEST,
	DUNGEON_VIEW_MESSAGE,
	DUNGEON_VIEW_BATTLE,
};

enum {
	DUNGEON_SUPPORT_NONE = 0,
	DUNGEON_SUPPORT_CHEST,
	DUNGEON_SUPPORT_TRAP,
	DUNGEON_SUPPORT_SHRINE,
};

enum {
	DUNGEON_ACTION_ATTACK = 0,
	DUNGEON_ACTION_ITEM,
	DUNGEON_ACTION_DEFEND,
	DUNGEON_ACTION_SKILL,
	DUNGEON_ACTION_ESCAPE,
	DUNGEON_ACTION_COUNT,
};

enum {
	DUNGEON_MONSTER_SLIME = 0,
	DUNGEON_MONSTER_GOBLIN,
	DUNGEON_MONSTER_WOLF,
	DUNGEON_MONSTER_GORILLA,
	DUNGEON_MONSTER_DRAGON,
	DUNGEON_MONSTER_EYE,
};

enum {
	DUNGEON_ITEM_SWORD = 0,
	DUNGEON_ITEM_SHIELD,
	DUNGEON_ITEM_HEAL,
	DUNGEON_ITEM_BOMB,
	DUNGEON_ITEM_ANTIDOTE,
	DUNGEON_ITEM_PURIFY,
	DUNGEON_ITEM_POISON,
	DUNGEON_ITEM_COUNT,
};

enum {
	DUNGEON_NEXT_NONE = 0,
	DUNGEON_NEXT_BATTLE,
	DUNGEON_NEXT_STAGE,
	DUNGEON_NEXT_WIN,
	DUNGEON_NEXT_LOSE,
	DUNGEON_NEXT_TRAVEL,
	DUNGEON_NEXT_RETURN,
};

enum {
	DUNGEON_BATTLE_PHASE_INPUT = 0,
	DUNGEON_BATTLE_PHASE_HERO_WAIT,
	DUNGEON_BATTLE_PHASE_MONSTER_WAIT,
};

#define DUNGEON_BATTLE_WAIT_TICKS	(10)

typedef struct {
	const uint8_t* data;
	uint8_t width;
	uint8_t height;
} dungeon_bitmap_t;

typedef struct {
	uint8_t level;
	uint8_t stage;
	uint8_t total_stages;
	uint8_t current_view;
	uint8_t current_monster;
	uint8_t support_event;
	uint8_t support_pending;
	uint8_t battle_turn;
	uint8_t battle_phase;
	uint8_t battle_wait_ticks;
	uint8_t defend_icon_active;
	uint8_t travel_progress;
	uint8_t selected_action;
	uint8_t selected_support_item;
	uint8_t defend_active;
	uint8_t poison_turns;
	uint8_t burn_turns;
	uint8_t curse_turns;
	uint8_t enemy_poison_turns;
	uint8_t enemy_poison_damage;
	uint8_t monster_dodge_ready;
	uint8_t inventory[DUNGEON_ITEM_COUNT];
	uint8_t chest_options[3];
	int16_t player_hp;
	int16_t player_max_hp;
	int16_t player_atk;
	int16_t player_def;
	int16_t monster_hp;
	int16_t monster_max_hp;
	int16_t monster_dmg;
	int16_t monster_armor;
	char line_1[22];
	char line_2[22];
	char line_3[22];
} dungeon_runtime_t;

static dungeon_runtime_t dungeon_runtime;
static uint8_t dungeon_message_next;

static const uint8_t dungeon_stage_counts[5] = {4, 5, 6, 7, 8};
static const uint8_t dungeon_monster_table[5][8] = {
	{DUNGEON_MONSTER_SLIME, DUNGEON_MONSTER_GOBLIN, DUNGEON_MONSTER_WOLF, DUNGEON_MONSTER_GORILLA, DUNGEON_MONSTER_GORILLA, DUNGEON_MONSTER_GORILLA, DUNGEON_MONSTER_GORILLA, DUNGEON_MONSTER_GORILLA},
	{DUNGEON_MONSTER_SLIME, DUNGEON_MONSTER_SLIME, DUNGEON_MONSTER_GOBLIN, DUNGEON_MONSTER_WOLF, DUNGEON_MONSTER_GORILLA, DUNGEON_MONSTER_GORILLA, DUNGEON_MONSTER_GORILLA, DUNGEON_MONSTER_GORILLA},
	{DUNGEON_MONSTER_SLIME, DUNGEON_MONSTER_GOBLIN, DUNGEON_MONSTER_GOBLIN, DUNGEON_MONSTER_WOLF, DUNGEON_MONSTER_GORILLA, DUNGEON_MONSTER_DRAGON, DUNGEON_MONSTER_DRAGON, DUNGEON_MONSTER_DRAGON},
	{DUNGEON_MONSTER_SLIME, DUNGEON_MONSTER_GOBLIN, DUNGEON_MONSTER_GOBLIN, DUNGEON_MONSTER_WOLF, DUNGEON_MONSTER_WOLF, DUNGEON_MONSTER_GORILLA, DUNGEON_MONSTER_DRAGON, DUNGEON_MONSTER_DRAGON},
	{DUNGEON_MONSTER_SLIME, DUNGEON_MONSTER_GOBLIN, DUNGEON_MONSTER_GOBLIN, DUNGEON_MONSTER_WOLF, DUNGEON_MONSTER_GORILLA, DUNGEON_MONSTER_GORILLA, DUNGEON_MONSTER_DRAGON, DUNGEON_MONSTER_EYE},
};

static const char* dungeon_monster_name[DUNGEON_MONSTER_EYE + 1] = {
	"SLIME",
	"GOBLIN",
	"WOLF",
	"GORILLA",
	"DRAGON",
	"EYE WATCHER",
};

static const char* dungeon_action_name[DUNGEON_ACTION_COUNT] = {
	"ATK",
	"ITEM",
	"DEF",
	"SKL",
	"ESC",
};

static const char* dungeon_item_name[DUNGEON_ITEM_COUNT] = {
	"Sword",
	"Shield",
	"Healing",
	"Bomb",
	"Antidote",
	"Purify",
	"Poison",
};

static void view_scr_dungeon_game();
static void dungeon_set_message(const char* line_1, const char* line_2, const char* line_3, uint8_t next_state);
static void dungeon_prepare_stage();
static void dungeon_start_battle();
static void dungeon_advance_stage();
static void dungeon_finish_game(uint8_t outcome);
static void dungeon_save_progress();
static void dungeon_clear_save();
static void dungeon_update_best_progress();
static void dungeon_queue_enemy_turn();
static void dungeon_finish_monster_turn();

view_dynamic_t dyn_view_item_dungeon_game = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_dungeon_game
};

view_screen_t scr_dungeon_game = {
	&dyn_view_item_dungeon_game,
	ITEM_NULL,
	ITEM_NULL,
	.focus_item = 0,
};

static uint8_t dungeon_clamp_level(uint8_t level) {
	if (level < 1) {
		return 1;
	}
	if (level > 5) {
		return 5;
	}
	return level;
}

static int16_t dungeon_max_int16(int16_t a, int16_t b) {
	return (a > b) ? a : b;
}

static int16_t dungeon_min_int16(int16_t a, int16_t b) {
	return (a < b) ? a : b;
}

static uint8_t dungeon_effect_damage() {
	uint8_t value = 0;
	if (dungeon_runtime.poison_turns > 0) {
		value += 5;
	}
	if (dungeon_runtime.burn_turns > 0) {
		value += 5;
	}
	return value;
}

static void bw_sanitize_setting() {
	if ((settingsetup.num_arrow == 0) || (settingsetup.num_arrow > 5)) {
		settingsetup.num_arrow = 3;
	}

	if ((settingsetup.meteoroid_speed == 0) || (settingsetup.meteoroid_speed > 5)) {
		settingsetup.meteoroid_speed = 2;
	}

	if ((settingsetup.arrow_speed == 0) || (settingsetup.arrow_speed > 10)) {
		settingsetup.arrow_speed = 4;
	}
}

static void ar_game_level_setup() {
	eeprom_read(EEPROM_SETTING_START_ADDR, (uint8_t*)&settingsetup, sizeof(settingsetup));
	bw_sanitize_setting();
}

static void ar_game_time_tick_setup() {
	timer_set(AC_TASK_DISPLAY_ID, DUNGEON_TIME_TICK, DUNGEON_TIME_TICK_INTERVAL, TIMER_PERIODIC);
}

static void ar_game_save_and_reset_score() {
	if (dungeon_persist_enabled) {
		eeprom_write(EEPROM_SCORE_PLAY_ADDR, (uint8_t*)&dungeon_game_score, sizeof(dungeon_game_score));
	}
	dungeon_game_score = 0;
}

static void bw_reset_objects() {
	task_post_pure_msg(DUNGEON_CONTROL_ID, DUNGEON_CONTROL_RESET);
	task_post_pure_msg(DUNGEON_ACTION_ID, DUNGEON_ACTION_RESET);
	task_post_pure_msg(DUNGEON_STATE_ID, DUNGEON_STATE_RESET);
	task_post_pure_msg(DUNGEON_EFFECT_ID, DUNGEON_EFFECT_RESET);
	task_post_pure_msg(DUNGEON_LANE_ID, DUNGEON_LANE_RESET);
}

static uint8_t dungeon_monster_for_stage(uint8_t level, uint8_t stage) {
	return dungeon_monster_table[level - 1][stage - 1];
}

static dungeon_bitmap_t dungeon_monster_bitmap(uint8_t monster) {
	switch (monster) {
	case DUNGEON_MONSTER_SLIME:
		return {monster_slime, 31, 30};
	case DUNGEON_MONSTER_GOBLIN:
		return {monster_goblin, 31, 30};
	case DUNGEON_MONSTER_WOLF:
		return {monster_wolf, 30, 30};
	case DUNGEON_MONSTER_GORILLA:
		return {monster_gorilla, 30, 30};
	case DUNGEON_MONSTER_DRAGON:
		return {monster_dragon, 31, 30};
	default:
		return {monster_eye, 30, 30};
	}
}

static dungeon_bitmap_t dungeon_item_bitmap(uint8_t item) {
	switch (item) {
	case DUNGEON_ITEM_SWORD:
		return {item_sword, 20, 20};
	case DUNGEON_ITEM_SHIELD:
		return {item_shield, 20, 20};
	case DUNGEON_ITEM_HEAL:
		return {item_heal, 20, 20};
	case DUNGEON_ITEM_BOMB:
		return {item_bomb, 20, 25};
	case DUNGEON_ITEM_ANTIDOTE:
		return {item_shrine, 20, 20};
	case DUNGEON_ITEM_PURIFY:
		return {item_heal, 20, 20};
	default:
		return {item_trap, 15, 20};
	}
}

static void dungeon_pick_chest_options() {
	uint8_t pool[DUNGEON_ITEM_COUNT];
	uint8_t pool_len = 0;
	uint8_t seed = (uint8_t)(dungeon_runtime.level + dungeon_runtime.stage);

	for (uint8_t item = 0; item < DUNGEON_ITEM_COUNT; item++) {
		if ((item == DUNGEON_ITEM_PURIFY) && (dungeon_runtime.level < 5)) {
			continue;
		}
		pool[pool_len++] = item;
	}

	for (uint8_t index = 0; index < 3; index++) {
		dungeon_runtime.chest_options[index] = pool[(seed + (index * 2)) % pool_len];
		for (uint8_t prev = 0; prev < index; prev++) {
			if (dungeon_runtime.chest_options[index] == dungeon_runtime.chest_options[prev]) {
				dungeon_runtime.chest_options[index] = pool[(seed + index + prev + 1) % pool_len];
			}
		}
	}

	dungeon_runtime.selected_support_item = 0;
}

static void dungeon_init_player(uint8_t level) {
	memset(&dungeon_runtime, 0, sizeof(dungeon_runtime));
	dungeon_runtime.level = dungeon_clamp_level(level);
	dungeon_runtime.stage = 1;
	dungeon_runtime.total_stages = dungeon_stage_counts[dungeon_runtime.level - 1];
	dungeon_runtime.player_max_hp = 35 + (int16_t)dungeon_runtime.level * 5;
	dungeon_runtime.player_hp = dungeon_runtime.player_max_hp;
	dungeon_runtime.player_atk = 10 + (int16_t)dungeon_runtime.level * 2;
	dungeon_runtime.player_def = 4 + dungeon_runtime.level;
	dungeon_runtime.selected_action = 0;
	dungeon_game_score = 0;
	dungeon_prepare_stage();
}

static void dungeon_load_message_defaults() {
	snprintf(dungeon_runtime.line_1, sizeof(dungeon_runtime.line_1), "%s", "Forest path ahead");
	snprintf(dungeon_runtime.line_2, sizeof(dungeon_runtime.line_2), "%s", "MODE to continue");
	dungeon_runtime.line_3[0] = '\0';
}

static void dungeon_update_best_progress() {
	dungeon_game_score_t score_data;
	eeprom_read(EEPROM_DUNGEON_SCORE_ADDR, (uint8_t*)&score_data, sizeof(score_data));

	if (score_data.best_level < 1 || score_data.best_level > 5) {
		score_data.best_level = 0;
		score_data.best_stage = 0;
	}

	if ((dungeon_runtime.level > score_data.best_level) ||
		((dungeon_runtime.level == score_data.best_level) && (dungeon_runtime.stage > score_data.best_stage))) {
		score_data.best_level = dungeon_runtime.level;
		score_data.best_stage = dungeon_runtime.stage;
	}

	if (dungeon_game_score > score_data.best_score) {
		score_data.best_score = dungeon_game_score;
	}

	eeprom_write(EEPROM_DUNGEON_SCORE_ADDR, (uint8_t*)&score_data, sizeof(score_data));
}

static void dungeon_save_progress() {
	if (dungeon_persist_enabled == 0) {
		return;
	}

	dungeon_game_save_t save_data;
	memset(&save_data, 0, sizeof(save_data));
	save_data.magic = DUNGEON_SAVE_MAGIC;
	save_data.valid = 1;
	save_data.level = dungeon_runtime.level;
	save_data.stage = dungeon_runtime.stage;
	save_data.total_stages = dungeon_runtime.total_stages;
	save_data.score = (uint16_t)dungeon_game_score;
	save_data.player_hp = dungeon_runtime.player_hp;
	save_data.player_max_hp = dungeon_runtime.player_max_hp;
	save_data.player_atk = dungeon_runtime.player_atk;
	save_data.player_def = dungeon_runtime.player_def;
	save_data.monster_hp = dungeon_runtime.monster_hp;
	save_data.monster_max_hp = dungeon_runtime.monster_max_hp;
	save_data.monster_dmg = dungeon_runtime.monster_dmg;
	save_data.monster_armor = dungeon_runtime.monster_armor;
	save_data.current_view = dungeon_runtime.current_view;
	save_data.current_monster = dungeon_runtime.current_monster;
	save_data.selected_action = dungeon_runtime.selected_action;
	save_data.selected_support_item = dungeon_runtime.selected_support_item;
	save_data.poison_turns = dungeon_runtime.poison_turns;
	save_data.burn_turns = dungeon_runtime.burn_turns;
	save_data.curse_turns = dungeon_runtime.curse_turns;
	save_data.enemy_poison_turns = dungeon_runtime.enemy_poison_turns;
	save_data.enemy_poison_damage = dungeon_runtime.enemy_poison_damage;
	save_data.monster_dodge_ready = dungeon_runtime.monster_dodge_ready;
	save_data.defend_active = dungeon_runtime.defend_active;
	save_data.support_event = dungeon_runtime.support_event;
	save_data.support_pending = dungeon_runtime.support_pending;
	save_data.battle_turn = dungeon_runtime.battle_turn;
	save_data.travel_progress = dungeon_runtime.travel_progress;
	memcpy(save_data.inventory, dungeon_runtime.inventory, sizeof(save_data.inventory));
	memcpy(save_data.chest_options, dungeon_runtime.chest_options, sizeof(save_data.chest_options));
	eeprom_write(EEPROM_DUNGEON_SAVE_ADDR, (uint8_t*)&save_data, sizeof(save_data));
	dungeon_update_best_progress();
}

static void dungeon_clear_save() {
	dungeon_game_save_t save_data;
	memset(&save_data, 0, sizeof(save_data));
	eeprom_write(EEPROM_DUNGEON_SAVE_ADDR, (uint8_t*)&save_data, sizeof(save_data));
}

uint8_t dungeon_has_save_data() {
	dungeon_game_save_t save_data;
	eeprom_read(EEPROM_DUNGEON_SAVE_ADDR, (uint8_t*)&save_data, sizeof(save_data));
	return ((save_data.magic == DUNGEON_SAVE_MAGIC) && (save_data.valid == 1));
}

static uint8_t dungeon_restore_save() {
	dungeon_game_save_t save_data;
	eeprom_read(EEPROM_DUNGEON_SAVE_ADDR, (uint8_t*)&save_data, sizeof(save_data));
	if ((save_data.magic != DUNGEON_SAVE_MAGIC) || (save_data.valid != 1)) {
		return 0;
	}

	memset(&dungeon_runtime, 0, sizeof(dungeon_runtime));
	dungeon_runtime.level = dungeon_clamp_level(save_data.level);
	dungeon_runtime.stage = save_data.stage;
	dungeon_runtime.total_stages = save_data.total_stages;
	dungeon_runtime.current_view = save_data.current_view;
	dungeon_runtime.current_monster = save_data.current_monster;
	dungeon_runtime.support_event = save_data.support_event;
	dungeon_runtime.support_pending = save_data.support_pending;
	dungeon_runtime.battle_turn = save_data.battle_turn;
	dungeon_runtime.travel_progress = save_data.travel_progress;
	dungeon_runtime.selected_action = save_data.selected_action;
	dungeon_runtime.selected_support_item = save_data.selected_support_item;
	dungeon_runtime.defend_active = save_data.defend_active;
	dungeon_runtime.poison_turns = save_data.poison_turns;
	dungeon_runtime.burn_turns = save_data.burn_turns;
	dungeon_runtime.curse_turns = save_data.curse_turns;
	dungeon_runtime.enemy_poison_turns = save_data.enemy_poison_turns;
	dungeon_runtime.enemy_poison_damage = save_data.enemy_poison_damage;
	dungeon_runtime.monster_dodge_ready = save_data.monster_dodge_ready;
	dungeon_runtime.player_hp = save_data.player_hp;
	dungeon_runtime.player_max_hp = save_data.player_max_hp;
	dungeon_runtime.player_atk = save_data.player_atk;
	dungeon_runtime.player_def = save_data.player_def;
	dungeon_runtime.monster_hp = save_data.monster_hp;
	dungeon_runtime.monster_max_hp = save_data.monster_max_hp;
	dungeon_runtime.monster_dmg = save_data.monster_dmg;
	dungeon_runtime.monster_armor = save_data.monster_armor;
	memcpy(dungeon_runtime.inventory, save_data.inventory, sizeof(dungeon_runtime.inventory));
	memcpy(dungeon_runtime.chest_options, save_data.chest_options, sizeof(dungeon_runtime.chest_options));
	dungeon_game_score = save_data.score;
	dungeon_selected_level = dungeon_runtime.level;
	dungeon_load_message_defaults();
	if (dungeon_runtime.current_view > DUNGEON_VIEW_BATTLE) {
		dungeon_runtime.current_view = DUNGEON_VIEW_TRAVEL;
	}
	return 1;
}

void dungeon_prepare_continue() {
	dungeon_start_mode = DUNGEON_START_CONTINUE;
}

void dungeon_prepare_new_game() {
	dungeon_start_mode = DUNGEON_START_NEW_GAME;
	dungeon_selected_level = 1;
	dungeon_clear_save();
}

void dungeon_prepare_level(uint8_t level) {
	dungeon_start_mode = DUNGEON_START_LEVEL;
	dungeon_selected_level = dungeon_clamp_level(level);
}

void dungeon_prepare_creator_mode(uint8_t level) {
	dungeon_start_mode = DUNGEON_START_CREATOR;
	dungeon_selected_level = dungeon_clamp_level(level);
}

uint8_t dungeon_is_creator_mode() {
	return (dungeon_start_mode == DUNGEON_START_CREATOR);
}

static void dungeon_set_message(const char* line_1, const char* line_2, const char* line_3, uint8_t next_state) {
	snprintf(dungeon_runtime.line_1, sizeof(dungeon_runtime.line_1), "%s", (line_1 != 0) ? line_1 : "");
	snprintf(dungeon_runtime.line_2, sizeof(dungeon_runtime.line_2), "%s", (line_2 != 0) ? line_2 : "");
	snprintf(dungeon_runtime.line_3, sizeof(dungeon_runtime.line_3), "%s", (line_3 != 0) ? line_3 : "");
	dungeon_runtime.current_view = DUNGEON_VIEW_MESSAGE;
	dungeon_message_next = next_state;
}

static void dungeon_set_monster_stats(uint8_t monster) {
	dungeon_runtime.current_monster = monster;
	dungeon_runtime.battle_turn = 1;
	dungeon_runtime.battle_phase = DUNGEON_BATTLE_PHASE_INPUT;
	dungeon_runtime.battle_wait_ticks = 0;
	dungeon_runtime.defend_icon_active = 0;
	dungeon_runtime.defend_active = 0;
	dungeon_runtime.enemy_poison_turns = 0;
	dungeon_runtime.enemy_poison_damage = 0;
	dungeon_runtime.monster_dodge_ready = 0;

	switch (monster) {
	case DUNGEON_MONSTER_SLIME:
		dungeon_runtime.monster_max_hp = 30 + (dungeon_runtime.level - 1) * 3;
		dungeon_runtime.monster_dmg = 5;
		dungeon_runtime.monster_armor = 1;
		break;
	case DUNGEON_MONSTER_GOBLIN:
		dungeon_runtime.monster_max_hp = 50;
		dungeon_runtime.monster_dmg = 10 + (dungeon_runtime.level - 1) * 2;
		dungeon_runtime.monster_armor = 2;
		break;
	case DUNGEON_MONSTER_WOLF:
		dungeon_runtime.monster_max_hp = 70;
		dungeon_runtime.monster_dmg = 30;
		dungeon_runtime.monster_armor = 1;
		break;
	case DUNGEON_MONSTER_GORILLA:
		dungeon_runtime.monster_max_hp = 90;
		dungeon_runtime.monster_dmg = 40;
		dungeon_runtime.monster_armor = 4 + dungeon_runtime.level * 2;
		break;
	case DUNGEON_MONSTER_DRAGON:
		dungeon_runtime.monster_max_hp = 150;
		dungeon_runtime.monster_dmg = 50;
		dungeon_runtime.monster_armor = (dungeon_runtime.level >= 3) ? (dungeon_runtime.level - 2) * 7 : 7;
		break;
	default:
		dungeon_runtime.monster_max_hp = 200;
		dungeon_runtime.monster_dmg = 60;
		dungeon_runtime.monster_armor = 12;
		break;
	}

	dungeon_runtime.monster_hp = dungeon_runtime.monster_max_hp;
	dungeon_runtime.selected_action = 0;
}

static void dungeon_prepare_stage() {
	dungeon_runtime.travel_progress = 0;
	dungeon_runtime.current_view = DUNGEON_VIEW_TRAVEL;
	/* Every stage meets a chest before battle, boss stage gets 2 chests. */
	dungeon_runtime.support_event = (dungeon_runtime.stage >= dungeon_runtime.total_stages) ? 2 : 1;
	dungeon_runtime.support_pending = 1;
	dungeon_runtime.current_monster = dungeon_monster_for_stage(dungeon_runtime.level, dungeon_runtime.stage);
	dungeon_load_message_defaults();
	dungeon_save_progress();
}

static void dungeon_apply_chest_item(uint8_t item) {
	int16_t level_bonus = dungeon_runtime.level - 1;
	switch (item) {
	case DUNGEON_ITEM_SWORD:
		dungeon_runtime.player_atk += 5 + (level_bonus * 3);
		break;
	case DUNGEON_ITEM_SHIELD:
		dungeon_runtime.player_def += 5 + (level_bonus * 4);
		break;
	default:
		dungeon_runtime.inventory[item]++;
		break;
	}

	if (dungeon_runtime.support_event > 0) {
		dungeon_runtime.support_event--;
	}

	dungeon_game_score += 5;
	dungeon_runtime.travel_progress = 0;
	dungeon_runtime.support_pending = 1;
	dungeon_runtime.current_view = DUNGEON_VIEW_TRAVEL;
	dungeon_set_message("You picked item", dungeon_item_name[item], "Keep moving", DUNGEON_NEXT_TRAVEL);
	dungeon_save_progress();
}

static void dungeon_start_battle() {
	dungeon_set_monster_stats(dungeon_runtime.current_monster);
	dungeon_runtime.current_view = DUNGEON_VIEW_BATTLE;
	dungeon_runtime.selected_action = 0;
	dungeon_save_progress();
}

static void dungeon_trigger_support() {
	if (dungeon_runtime.support_event > 0) {
		dungeon_pick_chest_options();
		dungeon_runtime.current_view = DUNGEON_VIEW_CHEST;
	}
	else {
		dungeon_start_battle();
	}
	dungeon_save_progress();
}

static int16_t dungeon_player_damage(uint8_t skill) {
	int16_t damage = dungeon_runtime.player_atk + (skill ? (4 + dungeon_runtime.level * 2) : 0);
	damage -= skill ? (dungeon_runtime.monster_armor / 3) : (dungeon_runtime.monster_armor / 2);
	return dungeon_max_int16(damage, 1);
}

static void dungeon_enemy_take_damage(int16_t damage) {
	dungeon_runtime.monster_hp -= damage;
	if (dungeon_runtime.monster_hp < 0) {
		dungeon_runtime.monster_hp = 0;
	}
}

static uint8_t dungeon_use_best_item() {
	int16_t heal_amount;

	if ((dungeon_runtime.curse_turns > 0) && (dungeon_runtime.inventory[DUNGEON_ITEM_PURIFY] > 0)) {
		dungeon_runtime.inventory[DUNGEON_ITEM_PURIFY]--;
		dungeon_runtime.curse_turns = 0;
		return 1;
	}

	if (((dungeon_runtime.poison_turns > 0) || (dungeon_runtime.burn_turns > 0)) && (dungeon_runtime.inventory[DUNGEON_ITEM_ANTIDOTE] > 0)) {
		dungeon_runtime.inventory[DUNGEON_ITEM_ANTIDOTE]--;
		dungeon_runtime.poison_turns = 0;
		dungeon_runtime.burn_turns = 0;
		return 1;
	}

	heal_amount = 5 + ((dungeon_runtime.level - 1) * 2);
	if ((dungeon_runtime.inventory[DUNGEON_ITEM_HEAL] > 0) && (dungeon_runtime.player_hp < dungeon_runtime.player_max_hp)) {
		if (dungeon_runtime.curse_turns > 0) {
			heal_amount /= 2;
		}
		dungeon_runtime.inventory[DUNGEON_ITEM_HEAL]--;
		dungeon_runtime.player_hp = dungeon_min_int16(dungeon_runtime.player_hp + heal_amount, dungeon_runtime.player_max_hp);
		return 1;
	}

	if ((dungeon_runtime.inventory[DUNGEON_ITEM_POISON] > 0) && (dungeon_runtime.enemy_poison_turns == 0)) {
		dungeon_runtime.inventory[DUNGEON_ITEM_POISON]--;
		dungeon_runtime.enemy_poison_turns = 3;
		dungeon_runtime.enemy_poison_damage = 5 + ((dungeon_runtime.level - 1) * 2);
		return 1;
	}

	if (dungeon_runtime.inventory[DUNGEON_ITEM_BOMB] > 0) {
		dungeon_runtime.inventory[DUNGEON_ITEM_BOMB]--;
		dungeon_runtime.monster_armor -= 3 + ((dungeon_runtime.level - 1) * 2);
		if (dungeon_runtime.monster_armor < 0) {
			dungeon_runtime.monster_armor = 0;
		}
		return 1;
	}

	return 0;
}

static uint8_t dungeon_turn_matches(uint8_t turn, uint8_t first, uint8_t step) {
	return ((turn >= first) && (((turn - first) % step) == 0));
}

static void dungeon_enemy_action() {
	int16_t damage = dungeon_runtime.monster_dmg - (dungeon_runtime.player_def / 2);
	if (dungeon_runtime.defend_active) {
		damage /= 2;
		dungeon_runtime.defend_active = 0;
	}
	damage = dungeon_max_int16(damage, 1);

	if ((dungeon_runtime.current_monster == DUNGEON_MONSTER_SLIME) && ((dungeon_runtime.battle_turn % 2) == 0)) {
		dungeon_runtime.monster_hp = dungeon_min_int16(dungeon_runtime.monster_hp + 5, dungeon_runtime.monster_max_hp);
	}
	else if ((dungeon_runtime.current_monster == DUNGEON_MONSTER_GOBLIN) && dungeon_turn_matches(dungeon_runtime.battle_turn, 2, 3)) {
		dungeon_runtime.poison_turns = 3;
	}
	else if ((dungeon_runtime.current_monster == DUNGEON_MONSTER_WOLF) && dungeon_turn_matches(dungeon_runtime.battle_turn, 3, 4)) {
		dungeon_runtime.monster_dodge_ready = 1;
	}
	else if ((dungeon_runtime.current_monster == DUNGEON_MONSTER_GORILLA) && dungeon_turn_matches(dungeon_runtime.battle_turn, 2, 3)) {
		dungeon_runtime.monster_armor += 5;
	}
	else if ((dungeon_runtime.current_monster == DUNGEON_MONSTER_DRAGON) && dungeon_turn_matches(dungeon_runtime.battle_turn, 3, 4)) {
		dungeon_runtime.player_hp -= 10;
		dungeon_runtime.burn_turns = 3;
	}
	else if ((dungeon_runtime.current_monster == DUNGEON_MONSTER_EYE) && dungeon_turn_matches(dungeon_runtime.battle_turn, 3, 3)) {
		dungeon_runtime.curse_turns = 3;
	}

	dungeon_runtime.player_hp -= damage;
	if (dungeon_runtime.player_hp < 0) {
		dungeon_runtime.player_hp = 0;
	}
}

static void dungeon_status_tick() {
	if (dungeon_runtime.poison_turns > 0) {
		dungeon_runtime.player_hp -= 5;
		dungeon_runtime.poison_turns--;
	}
	if (dungeon_runtime.burn_turns > 0) {
		dungeon_runtime.player_hp -= 5;
		dungeon_runtime.burn_turns--;
	}
	if (dungeon_runtime.curse_turns > 0) {
		dungeon_runtime.curse_turns--;
	}
	if (dungeon_runtime.enemy_poison_turns > 0) {
		dungeon_runtime.monster_hp -= dungeon_runtime.enemy_poison_damage;
		dungeon_runtime.enemy_poison_turns--;
	}

	if (dungeon_runtime.player_hp < 0) {
		dungeon_runtime.player_hp = 0;
	}
	if (dungeon_runtime.monster_hp < 0) {
		dungeon_runtime.monster_hp = 0;
	}
}

static void dungeon_after_battle_win() {
	dungeon_runtime.battle_phase = DUNGEON_BATTLE_PHASE_INPUT;
	dungeon_runtime.battle_wait_ticks = 0;
	dungeon_runtime.defend_icon_active = 0;
	dungeon_game_score += 20 + (dungeon_runtime.level * 10);
	if (dungeon_runtime.stage >= dungeon_runtime.total_stages) {
		dungeon_set_message("LEVEL COMPLETE", "All monsters down", "MODE TO FINISH", DUNGEON_NEXT_WIN);
	}
	else {
		dungeon_set_message("Monster defeated", "Stage cleared", "MODE NEXT STAGE", DUNGEON_NEXT_STAGE);
	}
	dungeon_save_progress();
}

static void dungeon_queue_enemy_turn() {
	dungeon_runtime.battle_phase = DUNGEON_BATTLE_PHASE_HERO_WAIT;
	dungeon_runtime.battle_wait_ticks = DUNGEON_BATTLE_WAIT_TICKS;
}

static void dungeon_finish_monster_turn() {
	dungeon_runtime.battle_phase = DUNGEON_BATTLE_PHASE_INPUT;
	dungeon_runtime.battle_wait_ticks = 0;
	if (dungeon_runtime.defend_active == 0) {
		dungeon_runtime.defend_icon_active = 0;
	}
	dungeon_runtime.current_view = DUNGEON_VIEW_BATTLE;
	dungeon_save_progress();
}

static void dungeon_finish_game(uint8_t outcome) {
	dungeon_last_outcome = outcome;
	if (dungeon_persist_enabled) {
		dungeon_clear_save();
	}
	if (outcome == DUNGEON_OUTCOME_WIN) {
		task_post_pure_msg(DUNGEON_SCREEN_ID, DUNGEON_LAND_SUCCESS);
	}
	else {
		task_post_pure_msg(DUNGEON_SCREEN_ID, DUNGEON_RESET);
	}
}

static void dungeon_advance_stage() {
	dungeon_runtime.stage++;
	if (dungeon_runtime.stage > dungeon_runtime.total_stages) {
		dungeon_finish_game(DUNGEON_OUTCOME_WIN);
		return;
	}
	dungeon_prepare_stage();
}

void dungeon_move_selection(int8_t delta) {
	if (dungeon_game_state != GAME_PLAY) {
		return;
	}

	if (dungeon_runtime.current_view == DUNGEON_VIEW_CHEST) {
		int16_t next_index = (int16_t)dungeon_runtime.selected_support_item + delta;
		if (next_index < 0) {
			next_index = 0;
		}
		if (next_index > 2) {
			next_index = 2;
		}
		dungeon_runtime.selected_support_item = (uint8_t)next_index;
	}
	else if (dungeon_runtime.current_view == DUNGEON_VIEW_BATTLE) {
		int16_t next_index = (int16_t)dungeon_runtime.selected_action + delta;
		if (next_index < 0) {
			next_index = 0;
		}
		if (next_index >= DUNGEON_ACTION_COUNT) {
			next_index = DUNGEON_ACTION_COUNT - 1;
		}
		dungeon_runtime.selected_action = (uint8_t)next_index;
	}
}

void dungeon_confirm_action() {
	if (dungeon_game_state != GAME_PLAY) {
		return;
	}

	if (dungeon_runtime.current_view == DUNGEON_VIEW_MESSAGE) {
		if (dungeon_message_next == DUNGEON_NEXT_BATTLE) {
			dungeon_start_battle();
		}
		else if (dungeon_message_next == DUNGEON_NEXT_STAGE) {
			dungeon_advance_stage();
		}
		else if (dungeon_message_next == DUNGEON_NEXT_WIN) {
			dungeon_finish_game(DUNGEON_OUTCOME_WIN);
		}
		else if (dungeon_message_next == DUNGEON_NEXT_LOSE) {
			dungeon_finish_game(DUNGEON_OUTCOME_LOSE);
		}
		else if (dungeon_message_next == DUNGEON_NEXT_TRAVEL) {
			dungeon_runtime.current_view = DUNGEON_VIEW_TRAVEL;
		}
		else if (dungeon_message_next == DUNGEON_NEXT_RETURN) {
			dungeon_runtime.current_view = DUNGEON_VIEW_BATTLE;
		}
		BUZZER_PlayTones(tones_cc);
		return;
	}

	if (dungeon_runtime.current_view == DUNGEON_VIEW_CHEST) {
		dungeon_apply_chest_item(dungeon_runtime.chest_options[dungeon_runtime.selected_support_item]);
		BUZZER_PlayTones(tones_cc);
		return;
	}

	if (dungeon_runtime.current_view != DUNGEON_VIEW_BATTLE) {
		BUZZER_PlayTones(tones_3beep);
		return;
	}

	if (dungeon_runtime.battle_phase != DUNGEON_BATTLE_PHASE_INPUT) {
		BUZZER_PlayTones(tones_3beep);
		return;
	}

	switch (dungeon_runtime.selected_action) {
	case DUNGEON_ACTION_ATTACK:
		dungeon_runtime.defend_icon_active = 0;
		if ((dungeon_runtime.current_monster == DUNGEON_MONSTER_WOLF) && (dungeon_runtime.monster_dodge_ready != 0)) {
			dungeon_runtime.monster_dodge_ready = 0;
		}
		else {
			dungeon_enemy_take_damage(dungeon_player_damage(0));
		}
		break;

	case DUNGEON_ACTION_ITEM:
		dungeon_runtime.defend_icon_active = 0;
		if (dungeon_use_best_item() == 0) {
			dungeon_set_message("No item ready", "Try another action", "MODE TO RETURN", DUNGEON_NEXT_RETURN);
			BUZZER_PlayTones(tones_3beep);
			return;
		}
		break;

	case DUNGEON_ACTION_DEFEND:
		dungeon_runtime.defend_active = 1;
		dungeon_runtime.defend_icon_active = 1;
		break;

	case DUNGEON_ACTION_SKILL:
		dungeon_runtime.defend_icon_active = 0;
		dungeon_enemy_take_damage(dungeon_player_damage(1));
		break;

	default:
		dungeon_runtime.defend_icon_active = 0;
		if ((dungeon_runtime.current_monster == DUNGEON_MONSTER_DRAGON) || (dungeon_runtime.current_monster == DUNGEON_MONSTER_EYE)) {
			dungeon_set_message("Boss blocks path", "Escape failed", "Enemy turn", DUNGEON_NEXT_NONE);
		}
		else if (((dungeon_runtime.level + dungeon_runtime.stage + dungeon_runtime.battle_turn) % 3) != 0) {
			dungeon_game_score += 5;
			dungeon_set_message("Escape succeeded", "Stage skipped", "MODE NEXT STAGE", DUNGEON_NEXT_STAGE);
			dungeon_save_progress();
			BUZZER_PlayTones(tones_cc);
			return;
		}
		else {
			dungeon_set_message("Escape failed", "Enemy attacks", "", DUNGEON_NEXT_NONE);
		}
		break;
	}

	if (dungeon_runtime.monster_hp <= 0) {
		dungeon_after_battle_win();
		BUZZER_PlayTones(tones_startup);
		return;
	}

	dungeon_queue_enemy_turn();
	dungeon_save_progress();
	BUZZER_PlayTones(tones_cc);
}

void dungeon_tick() {
	if (dungeon_game_state != GAME_PLAY) {
		return;
	}

	if (dungeon_runtime.current_view == DUNGEON_VIEW_BATTLE) {
		if ((dungeon_runtime.battle_phase == DUNGEON_BATTLE_PHASE_HERO_WAIT) || (dungeon_runtime.battle_phase == DUNGEON_BATTLE_PHASE_MONSTER_WAIT)) {
			if (dungeon_runtime.battle_wait_ticks > 0) {
				dungeon_runtime.battle_wait_ticks--;
			}

			if (dungeon_runtime.battle_wait_ticks == 0) {
				if (dungeon_runtime.battle_phase == DUNGEON_BATTLE_PHASE_HERO_WAIT) {
					dungeon_enemy_action();
					dungeon_status_tick();
					dungeon_runtime.battle_turn++;

					if (dungeon_runtime.monster_hp <= 0) {
						dungeon_after_battle_win();
						BUZZER_PlayTones(tones_startup);
						return;
					}

					if (dungeon_runtime.player_hp <= 0) {
						dungeon_runtime.battle_phase = DUNGEON_BATTLE_PHASE_INPUT;
						dungeon_runtime.battle_wait_ticks = 0;
						dungeon_runtime.defend_icon_active = 0;
						dungeon_set_message("Hero has fallen", "GAME OVER", "MODE TO EXIT", DUNGEON_NEXT_LOSE);
						BUZZER_PlayTones(tones_3beep);
						return;
					}

					dungeon_runtime.battle_phase = DUNGEON_BATTLE_PHASE_MONSTER_WAIT;
					dungeon_runtime.battle_wait_ticks = DUNGEON_BATTLE_WAIT_TICKS;
					dungeon_save_progress();
				}
				else {
					dungeon_finish_monster_turn();
				}
			}
		}
		return;
	}

	if (dungeon_runtime.current_view != DUNGEON_VIEW_TRAVEL) {
		return;
	}

	/* Slow the hero down a bit so the path animation feels less rushed. */
	uint8_t travel_step = (uint8_t)(3 + (settingsetup.arrow_speed / 2));
	dungeon_runtime.travel_progress += travel_step;
	if (dungeon_runtime.travel_progress >= 100) {
		dungeon_runtime.travel_progress = 100;
		if (dungeon_runtime.support_pending) {
			dungeon_runtime.support_pending = 0;
			dungeon_trigger_support();
		}
	}
}

void dungeon_setup_session() {
	dungeon_last_outcome = DUNGEON_OUTCOME_NONE;
	ar_game_level_setup();
	dungeon_persist_enabled = 1;

	if ((dungeon_start_mode == DUNGEON_START_CONTINUE) && dungeon_restore_save()) {
		dungeon_game_score = dungeon_get_score_value();
		return;
	}

	if (dungeon_start_mode == DUNGEON_START_CREATOR) {
		dungeon_persist_enabled = 0;
		dungeon_init_player(dungeon_selected_level);
	}
	else if (dungeon_start_mode == DUNGEON_START_LEVEL) {
		dungeon_init_player(dungeon_selected_level);
	}
	else {
		dungeon_init_player(1);
		dungeon_selected_level = 1;
	}
}

void dungeon_reset_session() {
	memset(&dungeon_runtime, 0, sizeof(dungeon_runtime));
	dungeon_message_next = DUNGEON_NEXT_NONE;
}

uint8_t dungeon_get_current_stage() {
	return dungeon_runtime.stage;
}

uint8_t dungeon_get_total_stages() {
	return dungeon_runtime.total_stages;
}

uint8_t dungeon_get_level_value() {
	return dungeon_runtime.level;
}

uint32_t dungeon_get_score_value() {
	return dungeon_game_score;
}

static void dungeon_draw_stats_line() {
	view_render.drawRect(1, 1, LCD_WIDTH - 2, 9, WHITE);
	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.setCursor(4, 3);
	view_render.print("HP:");
	view_render.print(dungeon_runtime.player_hp);
	view_render.setCursor(40, 3);
	view_render.print("D:");
	view_render.print(dungeon_runtime.player_atk);
	view_render.setCursor(70, 3);
	view_render.print("S:");
	view_render.print(dungeon_runtime.player_def);
	view_render.setCursor(100, 3);
	view_render.print("E:");
	view_render.print(dungeon_effect_damage());
	view_render.drawLine(1, 11, LCD_WIDTH - 2, 11, WHITE);
}

static void dungeon_draw_travel() {
	int16_t hero_x = 8 + ((int16_t)dungeon_runtime.travel_progress * 84) / 100;
	int16_t hero_y = 0;
	uint8_t phase = (uint8_t)((dungeon_control.action_image * 5) + (dungeon_runtime.travel_progress / 3));

	view_render.fillScreen(BLACK);

	for (int16_t x = 2; x < (LCD_WIDTH - 2); x++) {
		int16_t wave_a = (x + phase) % 32;
		int16_t wave_b = (x + (phase * 2)) % 28;
		if (wave_a > 16) {
			wave_a = 32 - wave_a;
		}
		if (wave_b > 14) {
			wave_b = 28 - wave_b;
		}

		int16_t top = 24 + (wave_a / 2) - (wave_b / 3);
		int16_t bottom = 43 - (wave_b / 2) + (wave_a / 3);

		if (top < 14) {
			top = 14;
		}
		if (bottom > 51) {
			bottom = 51;
		}
		if (bottom <= top) {
			bottom = top + 1;
		}

		if ((x >= hero_x) && (x <= (hero_x + 24))) {
			hero_y = top + ((bottom - top) / 2) - 8;
		}

		view_render.drawFastVLine(x, top, bottom - top + 1, WHITE);
	}

	view_render.drawRect(0, 0, LCD_WIDTH, LCD_HEIGHT, WHITE);
	dungeon_draw_stats_line();
	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.setCursor(2, 13);
	view_render.print("L");
	view_render.print(dungeon_runtime.level);
	view_render.print(" S:");
	view_render.print(dungeon_runtime.stage);
	view_render.print("/");
	view_render.print(dungeon_runtime.total_stages);
	view_render.setCursor(92, 13);
	view_render.print("SC:");
	view_render.print(dungeon_game_score);

	view_render.drawBitmap(hero_x, hero_y, hero_icon, 24, 17, BLACK);

	if (dungeon_runtime.support_event > 0) {
		view_render.drawRect(108, 24, 14, 14, BLACK);
		view_render.setCursor(112, 27);
		view_render.setTextColor(BLACK);
		view_render.print("?");
		view_render.setTextColor(WHITE);
	}

	view_render.setCursor(36, 44);
	if (dungeon_runtime.support_event > 0) {
		view_render.print("Move to chest");
	}
	else {
		if (dungeon_runtime.stage >= dungeon_runtime.total_stages) {
			view_render.print("Boss ahead");
		}
		else {
			view_render.print("Enemy ahead");
		}
	}
}

static void dungeon_draw_message() {
	view_render.drawRect(0, 0, LCD_WIDTH, LCD_HEIGHT, WHITE);
	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.setCursor(10, 10);
	view_render.print(dungeon_runtime.line_1);
	view_render.setCursor(10, 26);
	view_render.print(dungeon_runtime.line_2);
	view_render.setCursor(10, 42);
	view_render.print(dungeon_runtime.line_3);
	view_render.setCursor(30, 56);
	view_render.print("MODE TO NEXT");
}

static void dungeon_draw_chest() {
	view_render.drawRect(0, 0, LCD_WIDTH, LCD_HEIGHT, WHITE);
	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.setCursor(24, 4);
	view_render.print("MYSTERY CHEST");

	/* Triangle cursor above selected box. */
	int16_t tri_x = 24 + (dungeon_runtime.selected_support_item * 40);
	view_render.fillTriangle(tri_x, 12, tri_x - 4, 8, tri_x + 4, 8, WHITE);

	for (uint8_t index = 0; index < 3; index++) {
		dungeon_bitmap_t bitmap = dungeon_item_bitmap(dungeon_runtime.chest_options[index]);
		int16_t axis_x = 6 + (index * 40);
		int16_t axis_y = 14;
		view_render.drawRect(axis_x, axis_y, 32, 46, WHITE);

		view_render.setCursor(axis_x + 4, axis_y + 4);
		if (dungeon_runtime.chest_options[index] == DUNGEON_ITEM_SWORD) {
			view_render.print("SWORD");
		}
		else if (dungeon_runtime.chest_options[index] == DUNGEON_ITEM_SHIELD) {
			view_render.print("SHIELD");
		}
		else if (dungeon_runtime.chest_options[index] == DUNGEON_ITEM_HEAL) {
			view_render.print("HEART");
		}
		else if (dungeon_runtime.chest_options[index] == DUNGEON_ITEM_BOMB) {
			view_render.print("BOMB");
		}
		else if (dungeon_runtime.chest_options[index] == DUNGEON_ITEM_ANTIDOTE) {
			view_render.print("ANTI");
		}
		else if (dungeon_runtime.chest_options[index] == DUNGEON_ITEM_PURIFY) {
			view_render.print("PURE");
		}
		else {
			view_render.print("POISON");
		}

		if (index == dungeon_runtime.selected_support_item) {
			view_render.drawBitmap(axis_x + 6, axis_y + 18, bitmap.data, bitmap.width, bitmap.height, WHITE);
		}
		else {
			view_render.drawBitmap(axis_x + 6, axis_y + 18, bitmap.data, bitmap.width, bitmap.height, WHITE);
		}
	}
	view_render.setCursor(30, 55);
	view_render.print("UP/DOWN + MODE");
}

static void dungeon_draw_battle() {
	dungeon_bitmap_t monster = dungeon_monster_bitmap(dungeon_runtime.current_monster);
	int8_t hero_dx = 0;
	int8_t hero_dy = 0;
	int8_t monster_dx = 0;
	int8_t monster_dy = 0;

	if (dungeon_runtime.battle_phase == DUNGEON_BATTLE_PHASE_HERO_WAIT) {
		hero_dx = ((dungeon_runtime.battle_wait_ticks & 0x01) == 0) ? 2 : 0;
		hero_dy = ((dungeon_runtime.battle_wait_ticks & 0x01) == 0) ? -1 : 0;
		monster_dx = -1;
	}
	else if (dungeon_runtime.battle_phase == DUNGEON_BATTLE_PHASE_MONSTER_WAIT) {
		monster_dx = ((dungeon_runtime.battle_wait_ticks & 0x01) == 0) ? -2 : 0;
		monster_dy = ((dungeon_runtime.battle_wait_ticks & 0x01) == 0) ? -1 : 0;
		hero_dx = 1;
	}
	view_render.drawRect(0, 0, LCD_WIDTH, LCD_HEIGHT, WHITE);
	dungeon_draw_stats_line();
	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.setCursor(2, 13);
	view_render.print("L");
	view_render.print(dungeon_runtime.level);
	view_render.print(" S");
	view_render.print(dungeon_runtime.stage);
	view_render.print("/");
	view_render.print(dungeon_runtime.total_stages);
	view_render.setCursor(82, 13);
	view_render.print("SC:");
	view_render.print(dungeon_game_score);

	view_render.drawBitmap(4 + hero_dx, 20 + hero_dy, hero_icon, 24, 17, WHITE);
	if (dungeon_runtime.defend_icon_active) {
		int16_t shield_x = 28 + hero_dx;
		int16_t shield_y = 22 + hero_dy;
		view_render.drawPixel(shield_x, shield_y + 0, WHITE);
		view_render.drawPixel(shield_x + 1, shield_y - 1, WHITE);
		view_render.drawPixel(shield_x + 2, shield_y - 2, WHITE);
		view_render.drawPixel(shield_x + 3, shield_y - 3, WHITE);
		view_render.drawPixel(shield_x + 4, shield_y - 3, WHITE);
		view_render.drawPixel(shield_x + 5, shield_y - 2, WHITE);
		view_render.drawPixel(shield_x + 6, shield_y - 1, WHITE);
		view_render.drawPixel(shield_x + 7, shield_y + 0, WHITE);
		view_render.drawPixel(shield_x + 7, shield_y + 1, WHITE);
		view_render.drawPixel(shield_x + 6, shield_y + 2, WHITE);
		view_render.drawPixel(shield_x + 5, shield_y + 3, WHITE);
		view_render.drawPixel(shield_x + 4, shield_y + 4, WHITE);
		view_render.drawPixel(shield_x + 3, shield_y + 4, WHITE);
		view_render.drawPixel(shield_x + 2, shield_y + 3, WHITE);
		view_render.drawPixel(shield_x + 1, shield_y + 2, WHITE);
	}

	view_render.drawBitmap(92 + monster_dx, 18 + monster_dy, monster.data, monster.width, monster.height, WHITE);
	view_render.setCursor(54, 24);
	view_render.print(dungeon_monster_name[dungeon_runtime.current_monster]);
	view_render.setCursor(54, 34);
	view_render.print("MHP:");
	view_render.print(dungeon_runtime.monster_hp);
	view_render.setCursor(54, 42);
	view_render.print("ATK:");
	view_render.print(dungeon_runtime.monster_dmg);
	view_render.setCursor(54, 50);
	view_render.print("DEF:");
	if (dungeon_runtime.battle_phase == DUNGEON_BATTLE_PHASE_INPUT) {
		view_render.print(dungeon_runtime.monster_armor);
	}
	else {
		view_render.print("...");
	}

	for (uint8_t index = 0; index < DUNGEON_ACTION_COUNT; index++) {
		int16_t axis_x = 2 + (index * 25);
		view_render.drawRoundRect(axis_x, 54, 22, 8, 2, WHITE);
		if (index == dungeon_runtime.selected_action) {
			view_render.fillRoundRect(axis_x, 54, 22, 8, 2, WHITE);
			view_render.setTextColor(BLACK);
			view_render.setCursor(axis_x + 2, 55);
			view_render.print(dungeon_action_name[index]);
			view_render.setTextColor(WHITE);
		}
		else {
			view_render.setCursor(axis_x + 2, 55);
			view_render.print(dungeon_action_name[index]);
		}
	}
}

static void view_scr_dungeon_game() {
	view_render.clear();
	if (dungeon_game_state == GAME_PLAY) {
		if (dungeon_runtime.current_view == DUNGEON_VIEW_TRAVEL) {
			dungeon_draw_travel();
		}
		else if (dungeon_runtime.current_view == DUNGEON_VIEW_CHEST) {
			dungeon_draw_chest();
		}
		else if (dungeon_runtime.current_view == DUNGEON_VIEW_BATTLE) {
			dungeon_draw_battle();
		}
		else {
			dungeon_draw_message();
		}
	}
	else if (dungeon_game_state == GAME_OVER) {
		view_render.drawRect(0, 0, LCD_WIDTH, LCD_HEIGHT, WHITE);
		dungeon_draw_stats_line();
		view_render.setTextSize(2);
		view_render.setTextColor(WHITE);
		view_render.setCursor(10, 18);
		if (dungeon_last_outcome == DUNGEON_OUTCOME_WIN) {
			view_render.print("LEVEL");
			view_render.setCursor(18, 38);
			view_render.print("DONE");
		}
		else {
			view_render.print("GAME");
			view_render.setCursor(18, 38);
			view_render.print("OVER");
		}
		view_render.setTextSize(1);
		view_render.setCursor(8, 57);
		view_render.print("MODE: Continue");
	}
}

void scr_dungeon_game_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case SCREEN_ENTRY: {
		APP_DBG_SIG("SCREEN_ENTRY\n");
		view_render.initialize();
		view_render_display_on();
		dungeon_game_state = GAME_PLAY;
		dungeon_setup_session();
		task_post_pure_msg(DUNGEON_CONTROL_ID, DUNGEON_CONTROL_SETUP);
		task_post_pure_msg(DUNGEON_ACTION_ID, DUNGEON_ACTION_SETUP);
		task_post_pure_msg(DUNGEON_EFFECT_ID, DUNGEON_EFFECT_SETUP);
		task_post_pure_msg(DUNGEON_LANE_ID, DUNGEON_LANE_SETUP);
		task_post_pure_msg(DUNGEON_STATE_ID, DUNGEON_STATE_SETUP);
		ar_game_time_tick_setup();
	}
		break;

	case DUNGEON_TIME_TICK: {
		APP_DBG_SIG("DUNGEON_GAME_TIME_TICK\n");
		task_post_pure_msg(DUNGEON_CONTROL_ID, DUNGEON_CONTROL_UPDATE);
		task_post_pure_msg(DUNGEON_ACTION_ID, DUNGEON_ACTION_RUN);
		task_post_pure_msg(DUNGEON_EFFECT_ID, DUNGEON_EFFECT_UPDATE);
		task_post_pure_msg(DUNGEON_LANE_ID, DUNGEON_LANE_LEVEL_UP);
		dungeon_tick();
	}
		break;

	case DUNGEON_LAND_SUCCESS: {
		APP_DBG_SIG("DUNGEON_GAME_FINISH\n");
		timer_remove_attr(AC_TASK_DISPLAY_ID, DUNGEON_TIME_TICK);
		bw_reset_objects();
		timer_set(AC_TASK_DISPLAY_ID, DUNGEON_EXIT_GAME, DUNGEON_TIME_EXIT_INTERVAL, TIMER_ONE_SHOT);
		ar_game_save_and_reset_score();
		dungeon_game_state = GAME_OVER;
	}
		BUZZER_PlayTones(tones_startup);
		break;

	case DUNGEON_RESET: {
		APP_DBG_SIG("DUNGEON_GAME_RESET\n");
		timer_remove_attr(AC_TASK_DISPLAY_ID, DUNGEON_TIME_TICK);
		bw_reset_objects();
		timer_set(AC_TASK_DISPLAY_ID, DUNGEON_EXIT_GAME, DUNGEON_TIME_EXIT_INTERVAL, TIMER_ONE_SHOT);
		ar_game_save_and_reset_score();
		dungeon_game_state = GAME_OVER;
	}
		BUZZER_PlayTones(tones_3beep);
		break;

	case DUNGEON_EXIT_GAME: {
		APP_DBG_SIG("DUNGEON_GAME_EXIT\n");
		dungeon_game_state = GAME_OFF;
		SCREEN_TRAN(scr_game_over_handle, &scr_game_over);
	}
		break;

	case AC_DISPLAY_BUTTON_MODE_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_MODE_RELEASED\n");
		if (dungeon_game_state == GAME_OVER) {
			SCREEN_TRAN(scr_game_over_handle, &scr_game_over);
			BUZZER_PlayTones(tones_cc);
		}
	}
		break;

	case AC_DISPLAY_BUTTON_UP_RELEASED:
	case AC_DISPLAY_BUTTON_DOWN_RELEASED:
		break;

	default:
		break;
	}
}
