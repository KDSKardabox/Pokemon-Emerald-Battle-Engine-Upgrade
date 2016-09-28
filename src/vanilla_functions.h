#ifndef VANILLA_FUNCTIONS
#define VANILLA_FUNCTIONS

#include "types.h"
#include "battle_structs.h"

u8 get_item_x12_battle_function(u16 item_id);
u8 get_item_quality(u16 item_id);
void execute_battle_script(void* ptr_to_battlescript);
void record_usage_of_ability(u8 bank, u8 ability);
u8 castform_switch(u8 bank);
u8 is_bank_from_opponent_side(u8 bank);
void* display_string_in_battle(void*);
u8 read_byte(void*);
s16 read_hword(void*);
u32 read_word(void*);
u16 rng();
void damage_modulate_by_type_effectivity(u8 effect);
void prepare_status_animation(u8 buffer, u8 unkown, u32 status);
void mark_buffer_bank_for_execution(u8 bank);
u8 get_item_pocket_id(u16 itemID);
void* strcpy_xFF_terminated_0(void* destination, void* text_to_copy);
void prepare_setattributes_in_battle(u8 buffer, u8 data_request, u8 unkown, u8 data_to_add, void* ptr_to_attribute);
void battlescript_push();
void battlescript_pop();
void battlescript_custom_push(void* ptr_to_push);
void call_bc_move_exec(void* bs_ptr);
void b_std_message(u16 stringID, u8 bank);
u8 gender_from_pid(u16 species, u32 PID);
u8 get_battle_side(u8 bank);
s8 itemid_to_berryid(u16 itemID);
u16 species_to_national_dex(u16 species);
u16 get_height_or_weight(u16 national_dex_index, u8 which_one);
u8 get_bank_turn_order(u8 bank);
u8 count_alive_pokes_on_side(u8 side);
s8 get_poke_flavour_relation(u32 pid, u8 flavour);
void jump_if_cannot_switch_atk4F();
void set_move_effect(u8 a1, u8 a2);
u8 accuracy_helper_function(u16 move);
void jump_if_move_has_no_effect(u8 howmuchtoadd, u16 move);
void check_wonderguard_levitate_damage();
void record_usage_of_item(u8 bank, u8 item_effect);
u8 get_battle_bank(u8 bank_getter);
void pressure_pp_lose(u8 bank_atk, u8 bank_def, u16 move);
u8 is_poke_disobedient();
u8 uproar_wakeup_check(u8 bank);
void reset_several_turns_stuff(u8 bank);
u8 check_if_imprisioned(u8 bank, u16 move);
u8 get_bank_in_love(u8 arg);
u8 check_if_twoturns_move(u16 move);
void attack_iteration_cleanup();
void prepare_pokemon_battle_sprite_manipulation_buffer(u8 unknown, u8 unknown2);
u8 check_failure_due_to_status(u8 bank);
void sub_803CEDC(u8 bank1, u8 bank2);
u8 get_attributes(struct pokemon* poke_address, u8 request, void* destination);
void set_attributes(struct pokemon* poke_address, u8 request, void* new_value);
u8 item_is_mail(u8 itemID);
u8 get_lvl_from_exp(struct pokemon*);
u16 teach_move_in_available_slot(struct pokemon* , u16 move);
void new_move_for_the_first(struct pokemon*, u16 move);
u8 get_ai_bank(u8 arg);
void clear_atk_up_if_hit_flag_unless_enraged();
void bc_bs_executer();
void calculate_stats_pokekmon(struct pokemon* poke);
void prepare_battle_animation(u8 bufferID, u8 unkown, u8 special_arg);
void refresh_graphics(u8 bank, u8 arg2, u8 objID);
void task_delete(u8 taskID);
void move_anim_task_del(u8 taskID);
void prepare_health_bar_update_buffer(u8 bufferID, s16 hp_change);
char* get_player_name_address();
s8 compare_two_strings(char* string1, char* string2);
char* get_trainername_address(u16 trainerID);
void decompress_battle_healthbar_and_font();
void update_bank_graphical_elements(u8 objID, struct pokemon* poke, u8 element);
void play_sound(u16 songID);
u32 get_item_lastword(u16 itemID);
u8 checkitem(u16 itemID, u16 quantity);
void prepare_fainting_cry(u8 bufferID);
void bx_move_chosing_menu();
s16 get_pingpong(s16 phase, s16 scale);
u8 gpu_pal_tags_index_of(u16 tag);
void set_callback2(void* ptr);
void set_obj_pos(u8 ID, u16 x, u16 y);
void c2_80777E8();
void obj_delete(struct object*);
u8 b_load_sprite(u8 bank);
void update_bank_sprite(u8 bank);
void bank_sprite_sth(u8 bank);
u8 get_poke_ability(struct pokemon* poke);

u32 __udivsi3(u32 numerator, u32 denumerator);
u32 __umodsi3(u32 numerator, u32 denumerator);
u32 bits_table[32];

#endif /* VANILLA_FUNCTIONS */
