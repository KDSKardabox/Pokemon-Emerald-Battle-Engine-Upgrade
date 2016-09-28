#ifndef NEW_STRUCT
#define NEW_STRUCT

#include "types.h"

struct bank_affecting{
    u8 embargo : 3;
    u8 electrify : 1;
    u8 telekinesis: 3;
    u8 magnet_rise : 3;
    u8 heal_block : 3;
    u8 aqua_ring : 1;
    u8 powder : 1;
    u8 smacked_down : 1;
    u8 quashed : 1;
    u8 just_switched_in : 1;
    u8 miracle_eyed : 1;
    u8 eaten_berry : 1;
    u8 sunshine_form : 1;
    u8 cherrim_transformed : 1;
    u8 gastro_acided : 1;
    u8 spiky_shield : 1;
    u8 kings_shield : 1;
    u8 sky_drop_attacker : 1;
    u8 sky_drop_target : 1;
    u8 me_first : 1;
    u8 weakness2 : 1;
    u8 entry_message : 1;
    u8 powertrick : 1;
    u8 kingsshield_damage : 1;
    u8 spikyshield_damage : 1;
    u8 grassyterrain_heal : 1;
    u8 item_used : 1;
    u8 slowstart_duration : 3;
    u8 aegislash_form : 2; //0 = not aegi, 1 = defensive, 2 = attacking
    u8 killer_bank : 2;
    u8 same_move_used : 3;
    u8 wrap_bank : 2;
    u8 move_failed : 1;
    u8 type3 : 5;
    u8 stat_lowered : 1;
    u8 roost : 3;
    u8 bugbite : 1;
    u8 usedmoves : 4;
    s8 stockpile_def_changes;
    s8 stockpile_sp_def_changes;
    u8 autonomize_uses;
    u8 lastmove_type;
    u16 wish_hp;
};

struct side_affecting{
    u8 sticky_web : 1;
    u8 sticky_web_done : 1;
    u8 toxic_spikes_psn : 1;
    u8 toxic_spikes_badpsn : 1;
    u8 toxic_spikes_done : 1;
    u8 stealthrock : 1;
    u8 stealthrock_done : 1;
    u8 lucky_chant : 3;
    u8 lucky_chant_bank : 2;
    u8 tailwind : 3;
    u8 tailwind_bank : 2;
    u8 wide_guard : 1;
    u8 quick_guard : 1;
    u8 crafty_shield : 1;
    u8 combo_waterpledge : 1;
    u8 combo_firepledge : 1;
    u8 combo_grasspledge : 1;
    u8 swamp_spd_halving : 3;
    u8 sea_of_fire : 3;
    u8 rainbow : 3;
    u8 mat_block : 1;
    u8 lunardance : 1;
    u8 lunardance_done : 1;
    u8 echo_voice_counter : 3;
    u8 ally_fainted_last_turn : 2;
};

struct field_affecting{
    u8 trick_room : 3;
    u8 wonder_room : 3;
    u8 magic_room : 3;
    u8 grassy_terrain : 3;
    u8 electic_terrain : 3;
    u8 misty_terrain : 3;
    u8 ion_deluge : 3;
    u8 gravity : 3;
    u8 round_chosen : 1;
    u8 fairy_lock : 1;
};

struct various{
    u8 eaten_berry_player : 6;
    u8 eaten_berry_opponent : 6;
    u8 active_bank : 2;
    u8 inverse_battle : 1;
    u8 magicbounce : 1;
    u8 stormdrain : 1;
    u16 var1;
    u16 var2;
    u16 recently_used_item;
    u16 previous_move;
    u8 move_primary_effect;
    u8 parental_bond_mode: 2;
    u8 life_orbed : 1;
    u8 sheerforce_bonus : 1;
    u8 ate_bonus : 1;
    u8 gem_boost : 1;
    u16 accumulated_damage;
};

#define PBOND_PARENT 2
#define PBOND_CHILD 1
#define PBOND_DONT_SET 3

struct mega_related
{
    u8 user_trigger: 2; // 0x0 for not set, //0x1 for stone, //0x2 for wish
    u8 ally_trigger: 2;
    u8 evo_happened_pbs: 4;
    u8 party_mega_check: 6;
    u8 ai_party_mega_check: 6;
    u8 trigger_id;
    u8 indicator_id_pbs[4];
};

struct new_battle_struct{
    struct bank_affecting bank_affecting[4];
    struct side_affecting side_affecting[2];
    struct field_affecting field_affecting;
    struct various various;
    struct mega_related mega_related;
};

struct new_battlestruct_ptr{
    struct new_battle_struct *ptr;
};

extern struct new_battlestruct_ptr new_battlestruct;

//functions used by multiple files
u8 ability_battle_effects(u8 switch_id, u8 bank, u8 ability_to_check, u8 special_cases_argument, u16 move);
u8 get_item_effect(u8 bank, u8 check_negating_effects);
u8 has_ability_effect(u8 bank, u8 mold_breaker, u8 gastro);
s8 get_move_position(u8 bank, u16 move);
u8 weather_abilities_effect();

#endif /* NEW_STRUCT */
