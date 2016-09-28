#include "types.h"
#include "defines.h"
#include "battle_locations.h"
#include "battle_structs.h"
#include "vanilla_functions.h"
#include "new_battle_struct.h"
#include <string.h>
#include "static_references.h"

u8 check_ability(u8 bank, u8 ability);
u16 get_1_4_of_max_hp(u8 bank);
u16 get_1_8_of_max_hp(u8 bank);
u32 percent_lose(u32 number, u16 percent);
u32 percent_boost(u32 number, u16 percent);
u8 is_of_type(u8 bank, u8 type);
u16 get_airborne_state(u8 bank, u8 mode, u8 check_levitate);
u16 apply_type_effectiveness(u16 chained_effect, u8 move_type, u8 target_bank, u8 atk_bank, u8 airstatus);
u8 cant_poison(u8 bank, u8 self_inflicted);
u8 get_attacking_move_type();
u8 item_battle_effects(u8 switchid, u8 bank, u8 move_turn);
u8 percent_chance(u8 percent);
void damage_calc(u16 move, u8 move_type, u8 atk_bank, u8 def_bank);
u8 find_move_in_table(u16 move, u16 table_ptr[]);
u8 protect_affects(u16 move, u8 set);
u8 berry_eaten(u8 how_tocall, u8 bank);
u8 set_type(u8 bank, u8 type);
u8 get_target_of_move(u16 move, u8 target_given, u8 adjust);
void* get_move_battlescript_ptr(u16 move);
u8 affected_by_substitute(u8 bank);
u16 type_effectiveness_calc(u16 move, u8 move_type, u8 atk_bank, u8 def_bank, u8 effects_handling_and_recording);
void set_attacking_move_type();
void revert_mega_to_normalform(u8 teamID, u8 opponent_side);
u8 is_bank_present(u8 bank);

void atk7D_set_rain()
{
    battlescripts_curr_instruction++;
    if (battle_weather.flags.air_current || battle_weather.flags.harsh_sun)
    {
        battlescripts_curr_instruction = (void*) 0x082D9F1C; //but it failed script
    }
    else if (battle_weather.flags.downpour || battle_weather.flags.permament_rain || battle_weather.flags.rain || battle_weather.flags.heavy_rain)
    {
        move_outcome.missed = 1;
        battle_communication_struct.multistring_chooser = 2;
    }
    else
    {
        battle_weather.int_bw = weather_rain;
        battle_communication_struct.multistring_chooser = 1;
        if (get_item_effect(bank_attacker, 1) == ITEM_EFFECT_DAMPROCK)
            battle_effects_duration.weather_dur = 8;
        else
            battle_effects_duration.weather_dur = 5;
    }
    return;
}

void atk95_set_sandstorm()
{
    battlescripts_curr_instruction++;
    if (battle_weather.flags.air_current || battle_weather.flags.harsh_sun || battle_weather.flags.heavy_rain)
    {
        battlescripts_curr_instruction = (void*) 0x082D9F1C; //but it failed script
    }
    else if (battle_weather.flags.sandstorm || battle_weather.flags.permament_sandstorm)
    {
        move_outcome.missed = 1;
        battle_communication_struct.multistring_chooser = 2;
    }
    else
    {
        battle_weather.int_bw = weather_sandstorm;
        battle_communication_struct.multistring_chooser = 3;
        if (get_item_effect(bank_attacker, 1) == ITEM_EFFECT_SMOOTHROCK)
            battle_effects_duration.weather_dur = 8;
        else
            battle_effects_duration.weather_dur = 5;
    }
    return;
}

void atkBB_set_sunny()
{
    battlescripts_curr_instruction++;
    if (battle_weather.flags.air_current || battle_weather.flags.heavy_rain)
    {
        battlescripts_curr_instruction = (void*) 0x082D9F1C; //but it failed script
    }
    else if (battle_weather.flags.sun || battle_weather.flags.permament_sun || battle_weather.flags.harsh_sun)
    {
        move_outcome.missed = 1;
        battle_communication_struct.multistring_chooser = 2;
    }
    else
    {
        battle_weather.int_bw = weather_sun;
        battle_communication_struct.multistring_chooser = 4;
        if (get_item_effect(bank_attacker, 1) == ITEM_EFFECT_HEATROCK)
            battle_effects_duration.weather_dur = 8;
        else
            battle_effects_duration.weather_dur = 5;
    }
    return;
}

void atkC8_set_hail()
{
    battlescripts_curr_instruction++;
    if (battle_weather.flags.air_current || battle_weather.flags.harsh_sun || battle_weather.flags.heavy_rain)
    {
        battlescripts_curr_instruction = (void*) 0x082D9F1C; //but it failed script
    }
    else if (battle_weather.flags.hail || battle_weather.flags.permament_hail)
    {
        move_outcome.missed = 1;
        battle_communication_struct.multistring_chooser = 2;
    }
    else
    {
        battle_weather.int_bw = weather_hail;
        battle_communication_struct.multistring_chooser = 5;
        if (get_item_effect(bank_attacker, 1) == ITEM_EFFECT_ICYROCK)
            battle_effects_duration.weather_dur = 8;
        else
            battle_effects_duration.weather_dur = 5;
    }
    return;
}

void atk7E_set_reflect()
{
    battlescripts_curr_instruction++;
    u8 side = is_bank_from_opponent_side(bank_attacker);
    if (side_affecting_halfword[side].reflect_on)
    {
        move_outcome.missed = 1;
        battle_communication_struct.multistring_chooser = 0;
    }
    else
    {
        side_affecting_halfword[side].reflect_on = 1;
        if (get_item_effect(bank_attacker, 1) == ITEM_EFFECT_LIGHTCLAY)
            side_timers[side].reflect_timer = 8;
        else
            side_timers[side].reflect_timer = 5;
        side_timers[side].reflect_bank = bank_attacker;
        if (battle_flags.double_battle && count_alive_pokes_on_side(1) == 2)
            battle_communication_struct.multistring_chooser = 2;
        else
            battle_communication_struct.multistring_chooser = 1;
    }
    return;
}

void atk92_set_lightscreen()
{
    battlescripts_curr_instruction++;
    u8 side = is_bank_from_opponent_side(bank_attacker);
    if (side_affecting_halfword[side].light_screen_on)
    {
        move_outcome.missed = 1;
        battle_communication_struct.multistring_chooser = 0;
    }
    else
    {
        side_affecting_halfword[side].light_screen_on = 1;
        if (get_item_effect(bank_attacker, 1) == ITEM_EFFECT_LIGHTCLAY)
            side_timers[side].lightscreen_timer = 8;
        else
            side_timers[side].lightscreen_timer = 5;
        side_timers[side].lightscreen_bank = bank_attacker;
        if (battle_flags.double_battle && count_alive_pokes_on_side(1) == 2)
            battle_communication_struct.multistring_chooser = 4;
        else
            battle_communication_struct.multistring_chooser = 3;
    }
    return;
}

void atk96_weather_damage()
{
    battlescripts_curr_instruction++;
    s32 damage = 0;
    u8 ability = battle_participants[bank_attacker].ability_id;
    u8 ability_effect = has_ability_effect(bank_attacker, 0, 1);
    if (weather_abilities_effect() && !(get_item_effect(bank_attacker, 1) == ITEM_EFFECT_SAFETYGOOGLES) && !(ability_effect && ability == ABILITY_MAGIC_GUARD) &&!(absent_bank_flags & bits_table[bank_attacker]))
    {
        if (battle_weather.flags.sandstorm || battle_weather.flags.permament_sandstorm)
        {
            if (!(is_of_type(bank_attacker, TYPE_GROUND) || is_of_type(bank_attacker, TYPE_STEEL) || is_of_type(bank_attacker, TYPE_ROCK)) && !(ability_effect && (ability == ABILITY_SAND_FORCE || ability == ABILITY_SAND_RUSH)))
            {
                if (!(status3[bank_attacker].underground || status3[bank_attacker].underwater))
                {
                    damage = battle_participants[bank_attacker].max_hp >> 4;
                    if (damage == 0)
                        damage = 1;
                }
            }
        }
        else if (battle_weather.flags.hail || battle_weather.flags.permament_hail)
        {
            if (!(is_of_type(bank_attacker, TYPE_ICE)) && !(ability_effect && (ability == ABILITY_SNOW_CLOAK)))
            {
                if (!(status3[bank_attacker].underground || status3[bank_attacker].underwater))
                {
                    damage = battle_participants[bank_attacker].max_hp >> 4;
                    if (damage == 0)
                        damage = 1;

                    if (ability_effect && ability == ABILITY_ICE_BODY)
                        damage *= -1;
                }
            }
        }
    }
    damage_loc = damage;
    return;
}

void atkE2_switchout_abilities()
{
    active_bank = get_battle_bank(read_byte(battlescripts_curr_instruction + 1));
    if (has_ability_effect(active_bank, 0, 1))
    {
        u8 second_arg = bits_table[battle_stuff_ptr.ptr->field_58[active_bank]];
        switch (battle_participants[active_bank].ability_id)
        {
        case ABILITY_NATURAL_CURE:
            battle_participants[active_bank].status.int_status = 0;
            prepare_setattributes_in_battle(0, 0x28, second_arg, 4, &battle_participants[active_bank].status);
            mark_buffer_bank_for_execution(active_bank);
            break;
        case ABILITY_REGENERATOR:
            {
                u16 *current_hp = &battle_participants[active_bank].current_hp;
                u16 *max_hp = &battle_participants[active_bank].max_hp;
                if (current_hp)
                {
                    u8 added_hp = __udivsi3(*max_hp, 3);
                    if (*current_hp + added_hp > *max_hp)
                    {
                        added_hp = *max_hp - *current_hp;
                    }
                    *current_hp += added_hp;
                    prepare_setattributes_in_battle(0, 0x2A, second_arg, 4, current_hp);
                    mark_buffer_bank_for_execution(active_bank);
                }
                break;
            }
        }
    }
    battlescripts_curr_instruction += 2;
    return;
}

void atk8D_multihit_move_loop_counter()
{
    u8 arg_counter = read_byte(battlescripts_curr_instruction + 1);
    if (arg_counter != 0)
        multihit_counter = arg_counter;
    else
    {
        if (has_ability_effect(bank_attacker, 0, 1) && battle_participants[bank_attacker].ability_id == ABILITY_SKILL_LINK)
            multihit_counter = 5;
        else
        {
            u8 multihit = rng() & 3;
            if (multihit > 1)
                multihit = rng() & 3;
            multihit_counter = multihit + 2;
        }
    }
    battlescripts_curr_instruction += 2;
    return;
}

void atk42_jump_if_type()
{
    if (is_of_type(get_battle_bank(read_byte(battlescripts_curr_instruction + 1)), read_byte(battlescripts_curr_instruction + 2)))
        battlescripts_curr_instruction = (void*)read_word(battlescripts_curr_instruction + 3);
    else
        battlescripts_curr_instruction += 7;
    return;
}

void atk7F_set_leech_seed()
{
    if (move_outcome.missed || move_outcome.not_affected || move_outcome.failed || status3[bank_target].leech_seed)
    {
        move_outcome.missed = 1;
        battle_communication_struct.multistring_chooser = 1;
    }
    else if (is_of_type(bank_target, TYPE_GRASS))
    {
        move_outcome.missed = 1;
        battle_communication_struct.multistring_chooser = 2;
    }
    else
    {
        status3[bank_target].leech_seed_hp_receiver |= bank_attacker;
        status3[bank_target].leech_seed = 1;
        battle_communication_struct.multistring_chooser = 0;
    }
    battlescripts_curr_instruction++;
    return;
}

u8 entry_hazards_hook()
{
    u8 effect = 0;
    u8 banks_side = is_bank_from_opponent_side(active_bank);
    struct side_affecting* ptr_to_struct = &new_battlestruct.ptr->side_affecting[banks_side];
    if (ptr_to_struct->lunardance)
    {
        ptr_to_struct->lunardance = 0;
        damage_loc = (battle_participants[active_bank].max_hp - battle_participants[active_bank].current_hp) * -1;
        battle_participants[active_bank].status.int_status = 0;
        prepare_setattributes_in_battle(0, REQUEST_STATUS_BATTLE, 0, 4, &battle_participants[active_bank].status);
        mark_buffer_bank_for_execution(active_bank);
        //heal pp
        battlescript_push();
        battlescripts_curr_instruction = &lunardance_bs;
        effect = 1;
    }
    else if (side_affecting_halfword[banks_side].spikes_on && !(side_affecting_halfword[banks_side].spikes_damage_done) && get_airborne_state(active_bank, 0, 1) <= 2)
    {
        side_affecting_halfword[banks_side].spikes_damage_done = 1;
        if (has_ability_effect(active_bank, 0, 1) && battle_participants[active_bank].ability_id == ABILITY_MAGIC_GUARD)
        {
            damage_loc = 0;
        }
        else
        {
            u32 damage = __udivsi3(battle_participants[active_bank].max_hp, (5 - side_timers[banks_side].spikes_amount) * 2);
            if (damage == 0)
                damage = 1;
            damage_loc = damage;
        }
        battlescript_push();
        battlescripts_curr_instruction = &spikes_bs;
        effect = 1;
    }
    else if (ptr_to_struct->stealthrock && !ptr_to_struct->stealthrock_done)
    {
        ptr_to_struct->stealthrock_done = 1;
        if (has_ability_effect(active_bank, 0, 1) && battle_participants[active_bank].ability_id == ABILITY_MAGIC_GUARD)
        {
            damage_loc = 0;
        }
        else
        {
            u32 damage = 0;
            u16 max_hp = battle_participants[active_bank].max_hp;
            switch (type_effectiveness_calc(MOVE_STEALTH_ROCK, TYPE_ROCK, active_bank ^ 1, active_bank, 0) >> 4)
            {
            case 1:
                damage = max_hp >> 5;
                break;
            case 2:
                damage = max_hp >> 4;
                break;
            case 4:
                damage = max_hp >> 3;
                break;
            case 8:
                damage = max_hp >> 2;
                break;
            case 16:
                damage = max_hp >> 1;
                break;
            }
            if (damage == 0)
                damage = 1;
            damage_loc = damage;
        }
        battlescript_push();
        battlescripts_curr_instruction = &stealhrock_bs;
        effect = 1;
    }
    else if (ptr_to_struct->sticky_web && !ptr_to_struct->sticky_web_done && battle_participants[active_bank].spd_buff != 0 && get_airborne_state(active_bank, 0, 1) <= 2)
    {
        ptr_to_struct->sticky_web_done = 1;
        battlescript_push();
        battlescripts_curr_instruction = &stickyweb_bs;
        battle_scripting.stat_changer = 0x93;
        effect = 1;
    }
    else if (ptr_to_struct->toxic_spikes_psn && !ptr_to_struct->toxic_spikes_done && get_airborne_state(active_bank, 0, 1) <= 2)
    {
        ptr_to_struct->toxic_spikes_done = 1;
        if (is_of_type(active_bank, TYPE_POISON))
        {
            effect = 1;
            ptr_to_struct->toxic_spikes_psn = 0;
            battlescript_push();
            battlescripts_curr_instruction = &toxicpikes_absorbed;
        }
        else if (!cant_poison(active_bank, 0))
        {
            if (ptr_to_struct->toxic_spikes_badpsn)
                battle_participants[active_bank].status.flags.toxic_poison = 1;
            else
                battle_participants[active_bank].status.flags.poison = 1;
            battlescript_push();
            battlescripts_curr_instruction = &toxicspikes_bs;
            effect = 1;
            prepare_setattributes_in_battle(0, 0x28, 0, 4, &battle_participants[active_bank].status.flags);
            mark_buffer_bank_for_execution(active_bank);
        }
    }
    if (effect)
    {
       new_battlestruct.ptr->various.var1 = active_bank;
       battle_participants[active_bank].status2.destinny_bond = 0;
       hitmarker &= 0xFFFFFFBF; //same as above
    }
    return effect;
}

void switchin_newstruct_update()
{
    struct bank_affecting* ptr_to_struct = &new_battlestruct.ptr->bank_affecting[active_bank];
    struct bank_affecting previous_struct = new_battlestruct.ptr->bank_affecting[active_bank];
    memset(ptr_to_struct, 0, sizeof(struct bank_affecting));
    ptr_to_struct->type3 = TYPE_EGG;
    ptr_to_struct->just_switched_in = 1;
    ptr_to_struct->wish_hp = previous_struct.wish_hp;
    if (current_move == MOVE_BATON_PASS)
    {
        ptr_to_struct->aqua_ring = previous_struct.aqua_ring;
        ptr_to_struct->embargo = previous_struct.embargo;
        ptr_to_struct->powertrick = previous_struct.powertrick;
        ptr_to_struct->gastro_acided = previous_struct.gastro_acided;
        ptr_to_struct->heal_block = previous_struct.heal_block;
        if (previous_struct.powertrick)
        {
            u16* atk = &battle_participants[active_bank].atk;
            u16* def = &battle_participants[active_bank].def;
            u16 placeholder = *atk;
            *atk = *def;
            *def = placeholder;
        }
    }
    struct side_affecting* side_ptr = &new_battlestruct.ptr->side_affecting[is_bank_from_opponent_side(active_bank)];
    side_ptr->stealthrock_done = 0;
    side_ptr->sticky_web_done = 0;
    side_ptr->toxic_spikes_done = 0;
    side_ptr->lunardance_done = 0;
    return;
}

u8 not_magicguard(u8 bank)
{
    if (battle_participants[bank].ability_id == ABILITY_MAGIC_GUARD && has_ability_effect(bank, 0, 1))
        return 0;
    return 1;
}

u8 does_move_target_multiple()
{
    u8 retval=0;
    if(battle_flags.double_battle)
    {
        if(move_table[current_move].target==move_target_both)
        {
            retval=(count_alive_pokes_on_side(2)>=2);
        }
        else if(move_table[current_move].target==move_target_foes_and_ally)
        {
            active_bank=bank_attacker;
            retval=(count_alive_pokes_on_side(0)>=2);
        }
    }
    return retval;
}

u8 recoil_damage(u8 bank_to_apply)
{
    u8 can = 1;
    u16 recoil;
    if (current_move == MOVE_HEAD_SMASH || current_move == MOVE_LIGHT_OF_RUIN) //1/2 of damage dealt
        recoil = hp_dealt >> 1;
    else if (current_move == MOVE_STRUGGLE || current_move == MOVE_TAKE_DOWN || current_move == MOVE_WILD_CHARGE || current_move == MOVE_HEAD_CHARGE) //1/4 of damage dealt
        recoil = hp_dealt >> 2;
    else //1/3 of damage dealt hp
        recoil = __udivsi3(hp_dealt, 3);
    if (recoil == 0)
        recoil = 1;
    damage_loc = recoil;
    if ((check_ability(bank_to_apply, ABILITY_ROCK_HEAD) || check_ability(bank_to_apply, ABILITY_MAGIC_GUARD)) && current_move != MOVE_STRUGGLE)
    {
        record_usage_of_ability(bank_to_apply, battle_participants[bank_to_apply].ability_id);
        can = 0;
    }
    return can;
}

u8 can_lose_item(u8 bank, u8 stickyhold_check, u8 sticky_message);
void copy_status_condition_text(u8 bank, u8 confusion);
void status_to_effect();

u8 primary_effect_setter()
{
    u8 *move_effect = &new_battlestruct.ptr->various.move_primary_effect;
    u8 affects_user = 0;
    u8 bank_to_apply;
    if (*move_effect & 0x40)
    {
        bank_to_apply = bank_attacker;
        battle_scripting.active_bank = bank_target;
        affects_user = 0x40;
        *move_effect ^= 0x40;
    }
    else
    {
        bank_to_apply = bank_target;
        battle_scripting.active_bank = bank_attacker;
    }
    bank_partner_def = bank_to_apply;
    void *matcher = (void *)battlescripts_curr_instruction;
    if (!MOVE_WORKED)
        *move_effect = 0;

    struct battle_participant* applier_bank = &battle_participants[bank_to_apply];
    u16 current_hp = applier_bank->current_hp;
    u8 substitute = 0;
    if (affected_by_substitute(bank_to_apply) && !affects_user)
        substitute = 1;

    switch (*move_effect)
    {
        case 10: //heal target's status
            if (!substitute && applier_bank->status.int_status & move_table[current_move].arg1 && current_hp)
            {
                copy_status_condition_text(bank_to_apply, 0);
                applier_bank->status.int_status = 0;
                active_bank = bank_to_apply;
                prepare_setattributes_in_battle(0, REQUEST_STATUS_BATTLE, 0, 4, &applier_bank->status);
                mark_buffer_bank_for_execution(active_bank);
                battlescript_push();
                battlescripts_curr_instruction = (void*) 0x82DB361;
            }
            break;
        case 11: //wrap
            if (!substitute && !applier_bank->status2.trapped_in_wrap && current_hp)
            {
                if (get_item_effect(battle_scripting.active_bank, 1) == ITEM_EFFECT_GRIPCLAW)
                    applier_bank->status2.trapped_in_wrap = 7;
                else
                    applier_bank->status2.trapped_in_wrap = 4 + (rng() & 1);
                battle_stuff_ptr.ptr->trapped_move[bank_to_apply] = current_move;
                battle_stuff_ptr.ptr->trapper[bank_to_apply] = bank_attacker;
                new_battlestruct.ptr->bank_affecting[bank_to_apply].wrap_bank = battle_scripting.active_bank;
                battlescript_push();
                battlescripts_curr_instruction = &wrapped_bs;
                battle_text_buff1[0] = 0xFD;
                battle_text_buff1[1] = 2;
                battle_text_buff1[2] = current_move;
                battle_text_buff1[3] = current_move >> 8;
                battle_text_buff1[4] = 0xFF;
            }
            break;
         case 12: //clear stat changes
            {
                u8 effect = 0;
                u8* stat_ptr = &battle_participants[bank_to_apply].hp_buff;
                for (u8 i = 0; i < 8; i++)
                {
                    if (*(stat_ptr + i) != 6)
                    {
                        effect++;
                        *(stat_ptr + i) = 6;
                    }
                }
                if (effect && current_hp)
                {
                    battlescript_push();
                    battlescripts_curr_instruction = &clearstats_bs;
                }
            }
            break;
         case 13: //steal item
            if(!substitute && battle_participants[bank_attacker].current_hp)
            {
                u16* targets_item = &applier_bank->held_item;
                u16* attackers_item = &battle_participants[bank_attacker].held_item;
                u8 stickymsg = 0;
                //if (primary || certain)
                    //stickymsg = 1;
                if (*targets_item && *attackers_item == 0 && can_lose_item(bank_target, 1, stickymsg))
                {
                    battlescript_push();
                    battlescripts_curr_instruction = (void*) 0x82DB422;
                    last_used_item = *targets_item;
                    *attackers_item = *targets_item;
                    *targets_item = 0;
                    active_bank = bank_to_apply;
                    prepare_setattributes_in_battle(0, REQUEST_HELDITEM_BATTLE, 0, 2, targets_item);
                    mark_buffer_bank_for_execution(active_bank);
                    active_bank = bank_attacker;
                    prepare_setattributes_in_battle(0, REQUEST_HELDITEM_BATTLE, 0, 2, attackers_item);
                    mark_buffer_bank_for_execution(active_bank);
                    battle_stuff_ptr.ptr->choiced_move[bank_to_apply] = 0;
                }
            }
            break;
         case 14: //smack poke down
             if (current_hp && new_battlestruct.ptr->bank_affecting[bank_to_apply].smacked_down == 0 && (get_airborne_state(bank_to_apply, 1, 1) >= 3 || is_of_type(bank_to_apply, TYPE_FLYING) || status3[bank_to_apply].on_air || new_battlestruct.ptr->bank_affecting[bank_to_apply].sky_drop_attacker || new_battlestruct.ptr->bank_affecting[bank_to_apply].sky_drop_target))
             {
                 new_battlestruct.ptr->bank_affecting[bank_to_apply].smacked_down = 1;
                 new_battlestruct.ptr->bank_affecting[bank_to_apply].magnet_rise = 0;
                 new_battlestruct.ptr->bank_affecting[bank_to_apply].telekinesis = 0;
                 status3[bank_to_apply].on_air = 0;
                 battlescript_push();
                 battlescripts_curr_instruction = &smackdown_bs;
             }
            break;
        case 15: //rapid spin
            if (MOVE_WORKED && battle_participants[bank_attacker].current_hp)
            {
                battlescript_push();
                battlescripts_curr_instruction = (void*) 0x82DAFC3;
            }
            break;
        case 0x10: //knock off
            if (!substitute && applier_bank->held_item && can_lose_item(bank_to_apply, 1, 1) && battle_participants[bank_attacker].current_hp)
            {
                last_used_item = applier_bank->held_item;
                applier_bank->held_item = 0;
                battle_effects_duration.knocked_off_pokes[is_bank_from_opponent_side(bank_to_apply)] |= bits_table[battle_team_id_by_side[bank_to_apply]];
                battlescript_push();
                battlescripts_curr_instruction = (void*) 0x082DB168;
                battle_stuff_ptr.ptr->choiced_move[bank_to_apply] = 0;
            }
            break;
        case 0x30: //recoil
            if (current_hp && recoil_damage(bank_attacker))
            {
                battlescript_push();
                battlescripts_curr_instruction = (void*) 0x082DB3F4; //recoil battlescript
            }
            break;
        case 0x31: //recharge needed
            if (current_hp)
            {
                applier_bank->status2.recharge = 1;
                disable_structs[bank_to_apply].recharge_counter = 2;
                locked_move[bank_to_apply] = current_move;
            }
            break;
        case 0x32: //rage setter
            if(current_hp)
                applier_bank->status2.raged = 1;
            break;
        case 0x33: //payday
            if (!is_bank_from_opponent_side(bank_to_apply))
            {
                u16 to_give = 5 * applier_bank->level;
                if (payday_money + to_give > 0xFFFF)
                    payday_money = 0xFFFF;
                else
                {
                    payday_money += to_give;
                    battlescript_push();
                    battlescripts_curr_instruction = (void*) 0x082DB3D6;
                }
            }
            break;
        case 0x34: //uproar
            if (applier_bank->status2.uproar == 0 && MOVE_WORKED && current_hp)
            {
                applier_bank->status2.uproar = 3;
                applier_bank->status2.multiple_turn_move = 1;
                locked_move[bank_to_apply] = current_move;
                battlescript_push();
                battlescripts_curr_instruction = (void*) 0x82DB3C2;
            }
            break;
        case 0x35: //set thrash
            if (applier_bank->status2.locked_and_confuse == 0 && MOVE_WORKED && current_hp)
            {
                applier_bank->status2.locked_and_confuse = 2 + (rng() & 1);
                applier_bank->status2.multiple_turn_move = 1;
                locked_move[bank_to_apply] = current_move;
            }
            break;
        case 0x36: //berry eating
            if (!substitute && MOVE_WORKED && get_item_pocket_id(battle_participants[bank_to_apply].held_item == 4) && current_hp)
            {
                u8 arg = move_table[current_move].arg1 & 1;
                new_battlestruct.ptr->bank_affecting[bank_attacker].bugbite = arg;
                if (!arg)
                {
                    u16* berry = &battle_participants[bank_to_apply].held_item;
                    last_used_item = *berry;
                    *berry = 0;
                    battlescript_push();
                    battlescripts_curr_instruction = &incinerateberry_bs;
                    active_bank = bank_to_apply;
                    prepare_setattributes_in_battle(0, REQUEST_HELDITEM_BATTLE, 0, 2, berry);
                    mark_buffer_bank_for_execution(bank_to_apply);
                }
            }
            break;
        case 0x37: //set twoturn move
            battle_participants[bank_to_apply].status2.multiple_turn_move = 1;
            locked_move[bank_to_apply] = current_move;
            break;
    }
    *move_effect = 0;
    if(matcher==battlescripts_curr_instruction)
        return 0;
    else
        return 1;
}

#define INC_END_EVENTS battle_scripting.cmd49_state_tracker++;
#define case_max 34

void atk49_move_end_turn()
{
    u8 effect = 0;
    u16 last_move;
    if (last_used_move == 0xFFFF)
        last_move = 0;
    else
        last_move = last_used_move;
    u8 arg1 = read_byte(battlescripts_curr_instruction + 1);
    u8 arg2 = read_byte(battlescripts_curr_instruction + 2);
    u8 current_move_type = get_attacking_move_type();
    struct battle_participant* target_struct = &battle_participants[bank_target];
    struct battle_participant* attacker_struct = &battle_participants[bank_attacker];
    while (effect == 0)
    {
        switch (battle_scripting.cmd49_state_tracker)
        {
        case 0: //spiky shield recoil
            if (new_battlestruct.ptr->bank_affecting[bank_attacker].spikyshield_damage && attacker_struct->current_hp && not_magicguard(bank_attacker))
            {
                effect = 1;
                damage_loc = get_1_8_of_max_hp(bank_attacker);
                battlescript_push();
                battlescripts_curr_instruction = &spikyshield_dam_bs;
                new_battlestruct.ptr->bank_affecting[bank_attacker].spikyshield_damage = 0;
                move_outcome.missed = 0;
            }  // king shield recoil
            if (new_battlestruct.ptr->bank_affecting[bank_attacker].kingsshield_damage && attacker_struct->atk_buff != 0)
            {
                new_battlestruct.ptr->bank_affecting[bank_attacker].kingsshield_damage = 0;
                battle_scripting.stat_changer = 0xA1;
                move_outcome.missed = 0;
                battlescript_push();
                battlescripts_curr_instruction = &statloss_bs;
                effect = 1;
            }
            INC_END_EVENTS
            break;
        case 1: //rage checker
            if (target_struct->current_hp && target_struct->status2.raged && bank_target != bank_attacker &&
                is_bank_from_opponent_side(bank_target) != is_bank_from_opponent_side(bank_attacker) && MOVE_WORKED && TARGET_TURN_DAMAGED &&
                DAMAGING_MOVE && target_struct->atk_buff != 0xC)
            {
                target_struct->atk_buff++;
                battlescript_push();
                battlescripts_curr_instruction = (void*) 0x082DAE0D; //rage is building
                effect = 1;
            }
            INC_END_EVENTS
            break;
        case 2: //defrosting via fire type move
            if (target_struct->current_hp && target_struct->status.flags.freeze && TARGET_TURN_DAMAGED && MOVE_WORKED
                && (current_move==MOVE_SCALD || current_move_type == TYPE_FIRE))
            {
                target_struct->status.flags.freeze = 0;
                active_bank = bank_target;
                battlescript_push();
                battlescripts_curr_instruction = (void*) 0x082DB282; //defrosting
                effect = 1;
                prepare_setattributes_in_battle(0, REQUEST_STATUS_BATTLE, 0, 4, &target_struct->status);
                mark_buffer_bank_for_execution(active_bank);
            }
            INC_END_EVENTS
            break;
        case 3: // user ability activation on strike
            if(ability_battle_effects(10,bank_attacker,0,0,0))
                effect=1;
            INC_END_EVENTS
            break;
        case 4: // user items activation on strike (setting life orb bit or accumulating shell bell hp
            if(item_battle_effects(4,bank_attacker,0))
                effect=1;
            INC_END_EVENTS
            break;
        case 5: //target's synchronize activation
            if (ability_battle_effects(7, bank_target, 0, 0, 0))
                effect = 1;
            INC_END_EVENTS
            break;
        case 6: //target's ability activation on strike
            if (ability_battle_effects(4, bank_target, 0, 0, 0))
                effect = 1;
            INC_END_EVENTS
            break;
        case 7: //target's items activation on strike
            if (item_battle_effects(2, bank_target, 0))
                effect = 1;
            INC_END_EVENTS
            break;
        case 8: //attacker synchronize activation
            if (ability_battle_effects(8, bank_attacker, 0, 0, 0))
                effect = 1;
            INC_END_EVENTS
            break;
        case 9: //held items
            {
                for (u8 i = 0; i < no_of_all_banks; i++)
                {
                    u16* changed_item = &battle_stuff_ptr.ptr->changed_held_items[i];
                    if (*changed_item)
                    {
                        battle_participants[i].held_item = *changed_item;
                        *changed_item = 0;
                    }
                }
            }
            INC_END_EVENTS;
            break;
        case 10: //choice move update
        {
            u16 attacker_item = get_item_effect(bank_attacker, 1);
            u16* choice_move = &battle_stuff_ptr.ptr->choiced_move[bank_attacker];
            if (CHOICE_ITEM(attacker_item) && last_used_move != MOVE_STRUGGLE && (hitmarker & HITMARKER_OBEYS) && (*choice_move == 0 || *choice_move == 0xFFFF))
            {
                if ((last_used_move == MOVE_BATON_PASS || last_used_move == MOVE_VOLT_SWITCH || last_used_move == MOVE_UTURN) && move_outcome.failed)
                {
                    INC_END_EVENTS
                    break;
                }
                *choice_move = last_used_move;
            }
            if (get_move_position(bank_attacker, *choice_move) == -1)
                *choice_move = 0;
            INC_END_EVENTS
            break;
        }
        case 11: //hide user sprite in a semi invulnerable state
            if(SEMI_INVULNERABLE(bank_attacker) && (hitmarker & 80))
            {
                active_bank=bank_attacker;
                prepare_pokemon_battle_sprite_manipulation_buffer(0,1);
                mark_buffer_bank_for_execution(active_bank);
            }
            INC_END_EVENTS
            break;
        case 12: //refresh/restore user sprite in second turn of dive
            if(!MOVE_WORKED || !SEMI_INVULNERABLE(bank_attacker) || check_failure_due_to_status(bank_attacker))
            {
                active_bank=bank_attacker;
                prepare_pokemon_battle_sprite_manipulation_buffer(0,0);
                mark_buffer_bank_for_execution(active_bank);
                status3[bank_attacker].underground=0;
                status3[bank_attacker].underwater=0;
                status3[bank_attacker].on_air=0;
                status3[bank_attacker].phantomforce=0;
                special_statuses[bank_attacker].restored_bank_sprite=1;
            }
            INC_END_EVENTS
            break;
        case 13: //refresh/restore defender's sprite
            if(!special_statuses[bank_target].restored_bank_sprite && (bank_target<no_of_all_banks) &&
               !(SEMI_INVULNERABLE(bank_target)))
            {
                active_bank=bank_target;
                prepare_pokemon_battle_sprite_manipulation_buffer(0,0);
                mark_buffer_bank_for_execution(active_bank);
                status3[bank_target].underground=0;
                status3[bank_target].underwater=0;
                status3[bank_target].on_air=0;
                status3[bank_target].phantomforce=0;
            }
            INC_END_EVENTS
            new_battlestruct.ptr->various.active_bank = bank_attacker;
        case 14: //item move end turn
            if (multihit_counter!=0 && multihit_counter!=1 && item_battle_effects(3, 0, 0))
            {
                effect = 1;
                break;
            }
            INC_END_EVENTS
        case 15: //restore attacker_bank cheek pouch and berry adder
            if(multihit_counter!=0 && multihit_counter!=1)
            {
                for (u8 i = 0; i < no_of_all_banks; i++)
                {
                    if (berry_eaten(0, i))
                        effect = 1;
                }
                if(effect)
                    break;
            }
            INC_END_EVENTS
        case 16: //status immunities
            if (multihit_counter!=0 && multihit_counter!=1 && ability_battle_effects(5, 0, 0, 0, 0))
            {
                effect = 1;
                break;
            }
            INC_END_EVENTS
        case 17: // clear substitute bits
            if(multihit_counter!=0 || multihit_counter!=1)
            {
                for (u8 i = 0; i < no_of_all_banks; i++)
                {
                    if (disable_structs[i].substitute_hp == 0)
                    {
                        battle_participants[i].status2.substitute = 0;
                    }
                }
            }
            INC_END_EVENTS
        case 18: //magic bounce and coat eraser; avoids endless loops if magic bounce meets magic bounce or magic coat
            new_battlestruct.ptr->various.magicbounce = 0;
            battle_stuff_ptr.ptr->synchronize_effect_chooser = 0;
            INC_END_EVENTS
            break;
        case 19: //setup buffer for conversion, sketch etc.
            if(hitmarker & HITMARKER_PURSUIT_TRAP)
            {
                active_bank=bank_attacker;
                bank_attacker=bank_target;
                bank_target=active_bank;
                hitmarker &= 0xFFFFEFFF;
            }
            if(hitmarker & HITMARKER_NO_ATTACKSTRING2) //used by sketch
                previously_used_move[bank_attacker]=last_used_move;
            if (!(absent_bank_flags & bits_table[bank_attacker] & battle_stuff_ptr.ptr->absent_bank_flags_prev_turn) && last_move!=MOVE_BATON_PASS)
            {
                if((hitmarker & HITMARKER_OBEYS))
                {
                    current_move_used[bank_attacker]=current_move;
                    calling_move_used[bank_attacker]=last_used_move;
                }
                else
                {
                    current_move_used[bank_attacker]=0xFFFF;
                    calling_move_used[bank_attacker]=0xFFFF;
                }
                if(!(hitmarker & HITMARKER_FAINTED(bank_target)))
                    attacked_by[bank_target]=bank_attacker;
                if((hitmarker & HITMARKER_OBEYS) && MOVE_WORKED)
                {
                    move_hit_with_pbs[bank_target]=current_move;
                    move_type_hit_with_pbs[bank_target]=current_move_type;
                }
                else
                    move_hit_with_pbs[bank_target]=0xFFFF;
            }
            new_battlestruct.ptr->bank_affecting[bank_attacker].lastmove_type = current_move_type + 0x80;
            INC_END_EVENTS
            break;
        case 20: //setup mirror_move_buffers
            if (!(absent_bank_flags & bits_table[bank_attacker] & battle_stuff_ptr.ptr->absent_bank_flags_prev_turn) &&
                move_table[current_move].move_flags.flags.affected_by_mirrormove && (hitmarker & HITMARKER_OBEYS) &&
                bank_attacker!=bank_target && !(hitmarker&HITMARKER_FAINTED(bank_target)) && MOVE_WORKED)
                {
                    battle_stuff_ptr.ptr->mirror_moves_pbs[bank_target]=last_used_move;
                    battle_stuff_ptr.ptr->mirror_move_set_pbs[bank_target].moves_per_target[bank_attacker]=last_used_move;
                }
            INC_END_EVENTS
            break;
        case 21: //parental bond
            {
                if((new_battlestruct.ptr->various.parental_bond_mode==PBOND_PARENT) && MOVE_WORKED
                   && TARGET_TURN_DAMAGED && !(hitmarker&HITMARKER_IMMOBILE_DUE_TO_STATUS) &&
                    battle_participants[bank_target].current_hp && battle_participants[bank_attacker].current_hp)
                {
                    multihit_counter=1;
                    new_battlestruct.ptr->various.parental_bond_mode=PBOND_CHILD;
                    hitmarker|=HITMARKER_NO_PPDEDUCT;
                    attack_iteration_cleanup();
                    battle_scripting.cmd49_state_tracker=0;
                    battlescripts_curr_instruction = get_move_battlescript_ptr(current_move);
                    battlescript_push();
                    battlescripts_curr_instruction=(void *)(0x82DB87D);
                    effect=1;
                }
                else
                {
                    INC_END_EVENTS;
                }
                break;
            }
        case 22: //primary effects
            if(primary_effect_setter())
            {
                effect=1;
            }
            INC_END_EVENTS;
            break;
        case 23: //color change
            if (ability_battle_effects(21, bank_target, 0, 0, 0))
                effect = 1;
            INC_END_EVENTS
            break;
        case 24: //dual target moves
            if(!(hitmarker&HITMARKER_IMMOBILE_DUE_TO_STATUS) && move_table[current_move].target==move_target_both && battle_flags.double_battle &&
               !protect_structs[bank_attacker].flag0_onlystruggle && !(hitmarker&HITMARKER_NO_ATTACKSTRING)
               && battle_participants[bank_target^2].current_hp)
            {
                bank_target=bank_target^2;
                hitmarker|=HITMARKER_NO_ATTACKSTRING;
                attack_iteration_cleanup();
                battle_scripting.cmd49_state_tracker=0;
                battlescripts_curr_instruction = get_move_battlescript_ptr(current_move);
                battlescript_push();
                battlescripts_curr_instruction=(void *)(0x82DB87D);
                effect=1;
                break;
            }
            INC_END_EVENTS
            break;
        case 25: //triple target moves
            if(!(hitmarker&HITMARKER_IMMOBILE_DUE_TO_STATUS) && move_table[current_move].target==move_target_foes_and_ally &&
                battle_flags.double_battle &&!protect_structs[bank_attacker].flag0_onlystruggle)
            {
                u8 next_target=bank_target+1;
                while(next_target<no_of_all_banks && ((absent_bank_flags & bits_table[next_target]) || next_target==bank_attacker))
                    next_target++;
                if(next_target<no_of_all_banks && battle_participants[next_target].current_hp)
                {
                    bank_target=next_target;
                    hitmarker|=HITMARKER_NO_ATTACKSTRING;
                    attack_iteration_cleanup();
                    battle_scripting.cmd49_state_tracker=0;
                    battlescripts_curr_instruction = get_move_battlescript_ptr(current_move);
                    battlescript_push();
                    battlescripts_curr_instruction=(void *)(0x82DB87D);
                    effect=1;
                    break;
                }
            }
            INC_END_EVENTS
            break;
        case 26: //life orb damage
            if(attacker_struct->current_hp && !new_battlestruct.ptr->various.sheerforce_bonus)
            {
                if (new_battlestruct.ptr->various.life_orbed && not_magicguard(bank_attacker))
                {
                    u32 damage = __udivsi3(attacker_struct->max_hp, 10);
                    if (damage == 0)
                        damage = 1;
                    damage_loc = damage;
                    battlescript_push();
                    battlescripts_curr_instruction = &lifeorb_damage;
                    record_usage_of_item(bank_attacker, ITEM_EFFECT_LIFEORB);
                    effect = 1;
                }
                else if (new_battlestruct.ptr->various.accumulated_damage)
                {
                    u32 damage = new_battlestruct.ptr->various.accumulated_damage>>3;
                    if (damage == 0)
                        damage = 1;
                    damage_loc = damage*-1;
                    another_active_bank = bank_attacker;
                    battle_scripting.active_bank = bank_attacker;
                    battlescript_push();
                    battlescripts_curr_instruction = (void*)0x82DB7F7;
                    record_usage_of_item(bank_attacker, ITEM_EFFECT_SHELLBELL);
                    effect = 1;
                }
            }
            new_battlestruct.ptr->various.parental_bond_mode=0;
            new_battlestruct.ptr->various.gem_boost=0;
            new_battlestruct.ptr->various.life_orbed = 0;
            new_battlestruct.ptr->various.sheerforce_bonus = 0;
            new_battlestruct.ptr->various.accumulated_damage=0;
            INC_END_EVENTS
            break;
        case 27: //set this move as previous and mark move as used
            if (last_used_move)
                new_battlestruct.ptr->various.previous_move = last_used_move;
            else
                new_battlestruct.ptr->various.previous_move = current_move;
            s8 pos = get_move_position(bank_attacker, current_move);
            if (pos == -1)
                pos = get_move_position(bank_attacker, last_used_move);
            if (pos != -1)
                new_battlestruct.ptr->bank_affecting[bank_attacker].usedmoves |= bits_table[pos];
            new_battlestruct.ptr->various.active_bank = bank_attacker;
            INC_END_EVENTS
        case 28: //item move end turn
            if ((multihit_counter==0 || multihit_counter==1) && item_battle_effects(3, 0, 0))
            {
                effect = 1;
                break;
            }
            INC_END_EVENTS
        case 29: //bank cheek pouch and berry adder
            if (multihit_counter==0 || multihit_counter==1)
            {
                for (u8 i = 0; i < no_of_all_banks; i++)
                {
                    if (berry_eaten(MOVE_TURN, i))
                        effect = 1;
                }
                if(effect)
                    break;
            }
            INC_END_EVENTS
        case 30: //status immunities
            bank_attacker = new_battlestruct.ptr->various.active_bank;
            if ((multihit_counter==0 || multihit_counter==1) && ability_battle_effects(5, 0, 0, 0, 0))
            {
                effect = 1;
                break;
            }
            INC_END_EVENTS
        case 31: // clear substitute bits
            if (multihit_counter==0 || multihit_counter==1)
            {
                for (u8 i = 0; i < no_of_all_banks; i++)
                {
                    if (disable_structs[i].substitute_hp == 0)
                    {
                        battle_participants[i].status2.substitute = 0;
                    }
                }
            }
            INC_END_EVENTS
            break;
        case 32: //pickpocket ability activation on strike
            if (ability_battle_effects(22, bank_target, 0, 0, 0))
                effect = 1;
            INC_END_EVENTS
            break;
        default:
            battle_scripting.cmd49_state_tracker = case_max;
        }

        if (arg1 == 1 && effect == 0)
            battle_scripting.cmd49_state_tracker = case_max;
        else if (arg1 == 2 && arg2 == battle_scripting.cmd49_state_tracker)
            battle_scripting.cmd49_state_tracker = case_max;
        if (battle_scripting.cmd49_state_tracker >= case_max)
            break;
    }

    if (battle_scripting.cmd49_state_tracker >= case_max && effect == 0)
        battlescripts_curr_instruction += 3;
}

u8 move_thaws_user(u16 move)
{
    if (move == MOVE_FLAME_WHEEL || move == MOVE_SACRED_FIRE || move == MOVE_SCALD|| move == MOVE_FLARE_BLITZ || move == MOVE_FUSION_FLARE || move == MOVE_STEAM_ERUPTION)
        return 1;
    return 0;
}

u8 healblock_forbidden_moves(u16 move, u8 with_leechseed)
{
    if (move == MOVE_ABSORB || move == MOVE_MEGA_DRAIN || move == MOVE_GIGA_DRAIN|| move == MOVE_LEECH_LIFE || move == MOVE_DRAINING_KISS || move == MOVE_DRAIN_PUNCH || move == MOVE_DREAM_EATER || move == MOVE_HORN_LEECH || move == MOVE_OBLIVION_WING || move == MOVE_PARABOLIC_CHARGE || move == MOVE_MILK_DRINK || move == MOVE_HEAL_BLOCK || move == MOVE_REST || move == MOVE_RECOVER || move == MOVE_MOONLIGHT || move == MOVE_MORNING_SUN || move == MOVE_SLACK_OFF || move == MOVE_SYNTHESIS || move == MOVE_ROOST || move == MOVE_SOFTBOILED)
        return 1;
    else if (move == MOVE_LEECH_SEED && with_leechseed)
        return 1;
    return 0;
}

u8 gravity_forbidden_move(u16 move)
{
    if (move == MOVE_TELEKINESIS || move == MOVE_MAGNET_RISE || move == MOVE_SPLASH|| move == MOVE_BOUNCE || move == MOVE_FLY || move == MOVE_FLYING_PRESS || move == MOVE_JUMP_KICK || move == MOVE_HIGH_JUMP_KICK || move == MOVE_SKY_DROP)
        return 1;
    return 0;
}

u8 embargo_forbidden_move(u16 move)
{
    if (move == MOVE_FLING)
        return 1;
    return 0;
}

u8 aromaveil_affected_move(u16 move)
{
    if (current_move == MOVE_TORMENT || current_move == MOVE_TAUNT || current_move == MOVE_HEAL_BLOCK || current_move == MOVE_DISABLE || current_move == MOVE_ATTRACT || current_move == MOVE_ENCORE)
        return 1;
    return 0;
}

void check_and_set_parental_bond();

u8 check_if_cannot_attack()
{
    u8 effect = 0;
    check_and_set_parental_bond();
    struct battle_participant* attacker_struct = &battle_participants[bank_attacker];
    u8* state_tracker = &battle_stuff_ptr.ptr->atk_canceller_state_tracker;
    while(effect == 0)
    {
        switch (*state_tracker)
        {
        case 0: //flag clear
            attacker_struct->status2.destinny_bond = 0;
            status3[bank_attacker].grudge = 0;
            break;
        case 1: //check if asleep
            if (attacker_struct->status.flags.sleep)
            {
                if (uproar_wakeup_check(bank_attacker))
                {
                    effect = 2;
                    attacker_struct->status.flags.sleep = 0;
                    attacker_struct->status2.nightmare = 0;
                    battle_communication_struct.multistring_chooser = 1;
                    battlescript_push();
                    battlescripts_curr_instruction = (void*) 0x82DB220;
                }
                else
                {
                    if (check_ability(bank_attacker, ABILITY_EARLY_BIRD))
                        attacker_struct->status.flags.sleep -= 2;
                    else
                        attacker_struct->status.flags.sleep--;
                    if (attacker_struct->status.flags.sleep == 0)
                    {
                        effect = 2;
                        attacker_struct->status2.nightmare = 0;
                        battle_communication_struct.multistring_chooser = 0;
                        battlescript_push();
                        battlescripts_curr_instruction = (void*) 0x82DB220;
                    }
                    else if (current_move != MOVE_SLEEP_TALK && current_move != MOVE_SNORE)
                    {
                        effect = 1;
                        hitmarker |= 0x80000;
                        battlescripts_curr_instruction = (void*) 0x82DB213;
                    }
                }
            }
            break;
        case 2: //check if frozen
            if (attacker_struct->status.flags.freeze)
            {
                if (percent_chance(20))
                {
                    attacker_struct->status.flags.freeze = 0;
                    effect = 2;
                    battle_communication_struct.multistring_chooser = 0;
                    battlescript_push();
                    battlescripts_curr_instruction = (void*) 0x82DB277;
                }
                else if (!move_thaws_user(current_move))
                {
                    effect = 1;
                    battlescripts_curr_instruction = (void*) 0x82DB26A;
                    hitmarker |= HITMARKER_NO_ATTACKSTRING;
                }
            }
            break;
        case 3: //check truant
            if (check_ability(bank_attacker, ABILITY_TRUANT) && disable_structs[bank_attacker].truant_counter & 1)
            {
                battle_communication_struct.multistring_chooser = 0;
                battlescripts_curr_instruction = (void*) 0x082DB6AD;
                move_outcome.missed = 1;
                effect = 3;
            }
            break;
        case 4: //check recharge
            if (attacker_struct->status2.recharge)
            {
                attacker_struct->status2.recharge = 0;
                disable_structs[bank_attacker].recharge_counter = 0;
                battlescripts_curr_instruction = (void*) 0x82D9462;
                effect = 3;
            }
            break;
        case 5: //check flinched
            if (attacker_struct->status2.flinched)
            {
                attacker_struct->status2.flinched = 0;
                battlescripts_curr_instruction = (void*) 0x82DB29B;
                effect = 3;
                protect_structs[bank_attacker].flag2_flinch_immobility = 1;
            }
            break;
        case 6: //check if attack is disabled
            if (disable_structs[bank_attacker].disabled_move == current_move && current_move)
            {
                protect_structs[bank_attacker].flag1_used_disabled_move = 1;
                battle_scripting.active_bank = bank_attacker;
                battlescripts_curr_instruction = (void*) 0x82DAE14;
                effect = 3;
            }
            break;
        case 7: //check if can't use move due to taunt
            if (disable_structs[bank_attacker].taunt_timer && move_table[current_move].split == 2)
            {
                effect = 3;
                protect_structs[bank_attacker].flag2_used_taunted_move = 1;
                battlescripts_curr_instruction = (void*) 0x82DB0A4;
            }
            break;
        case 8: //check imprisioned
            if (check_if_imprisioned(bank_attacker, current_move))
            {
                effect = 3;
                protect_structs[bank_attacker].flag1_used_imprisoned_move = 1;
                battlescripts_curr_instruction = (void*) 0x82DB176;
            }
            break;
        case 9: //check heal block
            if (new_battlestruct.ptr->bank_affecting[bank_attacker].heal_block && healblock_forbidden_moves(current_move, 0))
            {
                effect = 4;
                battlescripts_curr_instruction = &healblock_prevents_usage_bs;
            }
            break;
        case 10: //check gravity
            if (new_battlestruct.ptr->field_affecting.gravity && gravity_forbidden_move(current_move))
            {
                effect = 4;
                battlescripts_curr_instruction = &gravity_prevents_usage_bs;
            }
            break;
        case 11: //check embargo
            if (new_battlestruct.ptr->bank_affecting[bank_attacker].embargo && embargo_forbidden_move(current_move))
            {
                effect = 4;
                battlescripts_curr_instruction = &embargoprevents_usage_bs;
            }
            break;
        case 12: //check confusion
            if (attacker_struct->status2.confusion)
            {
                attacker_struct->status2.confusion--;
                if (attacker_struct->status2.confusion == 0)
                {
                    battlescript_push();
                    battlescripts_curr_instruction = (void*) 0x82DB300; //snapped out of confusion
                }
               else if (percent_chance(50)) //manages to hit the target
                {
                    battlescript_push();
                    battlescripts_curr_instruction = (void*) 0x82DB2BD;
                    battle_communication_struct.multistring_chooser = 0;
                }
                else //hits itself
                {
                    battlescripts_curr_instruction = (void*) 0x82DB2BD;
                    battle_communication_struct.multistring_chooser = 1;
                    bank_target = bank_attacker;
                    damage_calc(MOVE_POUND, TYPE_EGG, bank_attacker, bank_attacker);
                    protect_structs[bank_attacker].flag1_confusion_self_damage = 1;
                    hitmarker |= 0x80000;
                }
                effect = 1;
            }
            break;
        case 13: //check paralysis
            if (attacker_struct->status.flags.paralysis && percent_chance(25))
            {
                effect = 1;
                protect_structs[bank_attacker].flag0_prlz_immobility = 1;
                battlescripts_curr_instruction = (void*) 0x82DB28B;
                hitmarker |= 0x80000;
            }
            break;
        case 14: //check infatuation
            if (attacker_struct->status2.in_love)
            {
                battle_scripting.active_bank = get_bank_in_love(attacker_struct->status2.in_love);
                if (percent_chance(50)) //in love cannot attack
                {
                    battlescript_custom_push((void*)0x82DB334);
                    battlescripts_curr_instruction = (void*) 0x82DB327;
                    protect_structs[bank_attacker].flag1_love_immobility = 1;
                    effect = 3;
                }
                else
                {
                    battlescript_push();
                    battlescripts_curr_instruction = (void*) 0x82DB327;
                    effect = 1;
                }
            }
            break;
        case 15: //check bide
            if (attacker_struct->status2.bide)
            {
                attacker_struct->status2.bide--;
                if (attacker_struct->status2.bide == 0) //unleashing energy
                {
                    if (taken_damage[bank_attacker] == 0) //got no damage
                        battlescripts_curr_instruction = (void*) 0x82DADC4;
                    else
                    {
                        current_move = MOVE_BIDE;
                        battle_scripting.bide_damage = taken_damage[bank_attacker] * 2;
                        battlescripts_curr_instruction = (void*) 0x82DAD7C;
                        bank_target = hurt_by[bank_attacker];
                        if (absent_bank_flags & bits_table[bank_target]) //poke's not longer on the battlefield
                            bank_target = get_target_of_move(MOVE_BIDE, 1, 1);
                    }
                }
                else //storing energy
                    battlescripts_curr_instruction = (void*) 0x82DAD71;
                effect = 1;
            }
            break;
        case 16: //aroma veil protection
            if (aromaveil_affected_move(current_move))
            {
                u8 aroma = ability_battle_effects(12, bank_target, ABILITY_AROMA_VEIL, 1, 0);
                if (aroma)
                {
                    effect = 4;
                    battlescripts_curr_instruction = &aromaveilprevents_bs;
                    last_used_ability = ABILITY_AROMA_VEIL;
                    record_usage_of_ability(aroma - 1, ABILITY_AROMA_VEIL);
                }
            }
            break;
        case 17: //check thawing effect
            if (attacker_struct->status.flags.freeze && move_thaws_user(current_move))
            {
                attacker_struct->status.flags.freeze = 0;
                battlescript_push();
                battlescripts_curr_instruction = (void*) 0x82DB277;
                battle_communication_struct.multistring_chooser = 1;
            }
            break;
        case 18://powder
            if (new_battlestruct.ptr->bank_affecting[bank_attacker].powder && get_attacking_move_type() == TYPE_FIRE)
            {
                effect = 1;
                damage_loc = get_1_4_of_max_hp(bank_attacker);
                battlescripts_curr_instruction = &powder_bs;
            }
            break;
        case 19: //weather prevents move usage
            if (battle_weather.flags.heavy_rain && get_attacking_move_type() == TYPE_FIRE)
            {
                effect = 1;
                battlescripts_curr_instruction = &heavyrain_prevents_bs;
            }
            else if (battle_weather.flags.harsh_sun && get_attacking_move_type() == TYPE_WATER)
            {
                effect = 1;
                battlescripts_curr_instruction = &harshsun_prevents_bs;
            }
            break;
        }
        if (effect == 2)
        {
            active_bank = bank_attacker;
            prepare_setattributes_in_battle(0, REQUEST_STATUS_BATTLE, 0, 4, &attacker_struct->status);
            mark_buffer_bank_for_execution(active_bank);
        }
        else if (effect >= 3)
        {
            hitmarker |= 0x80000;
            reset_several_turns_stuff(bank_attacker);
            if (effect == 4)
            {
                battle_text_buff1[0] = 0xFD;
                battle_text_buff1[1] = 2;
                battle_text_buff1[2] = current_move;
                battle_text_buff1[3] = (current_move >> 8);
                battle_text_buff1[4] = 0xFF;
            }
        }
        *state_tracker += 1;
        if (*state_tracker >= 19 && effect == 0)
            break;
    }
    return effect;
}

u8 immune_to_powder_moves(u8 def_bank, u16 move)
{
    u8 immune = 0;
    if (find_move_in_table(move, &powder_moves[0]))
    {
        if (is_of_type(def_bank, TYPE_GRASS))
        {
            immune = 1;
            battlescripts_curr_instruction = &no_effect_bs;
        }
        else if (get_item_effect(def_bank, 1) == ITEM_EFFECT_SAFETYGOOGLES)
        {
            immune = 1;
            record_usage_of_item(def_bank, ITEM_EFFECT_SAFETYGOOGLES);
            battlescripts_curr_instruction = &no_effect_bs;
        }
    }
    return immune;
}

u8 is_in_parental_bond_banlist()
{
    return 0;
}

void check_and_set_parental_bond()
{
    if(check_ability(bank_attacker,ABILITY_PARENTAL_BOND) && new_battlestruct.ptr->various.parental_bond_mode==0)
    {
        if(does_move_target_multiple() || is_in_parental_bond_banlist())
        {
            new_battlestruct.ptr->various.parental_bond_mode=PBOND_DONT_SET;
            multihit_counter = 2;
        }
        if(DAMAGING_MOVE)
            new_battlestruct.ptr->various.parental_bond_mode=PBOND_PARENT;
    }
}

void atk00_move_canceller()
{
    if (battle_outcome)
    {
        battle_state_mode = 0xC;
        return;
    }
    else if (battle_participants[bank_attacker].current_hp == 0 && !(hitmarker & HITMARKER_NO_ATTACKSTRING))
    {
        hitmarker |= 0x80000;
        battlescripts_curr_instruction = (void*) 0x082D8A4E; //bs_endturn and end
        return;
    }
    else if (check_if_cannot_attack())
        return;
    else if (ability_battle_effects(2, bank_target, 0, 0, 0))
        return;
    else if (immune_to_powder_moves(bank_target, current_move))
        return;
    else if (battle_participants[bank_attacker].current_pp[current_move_position] == 0 && current_move != MOVE_STRUGGLE && !battle_participants[bank_attacker].status2.multiple_turn_move && ((hitmarker & (HITMARKER_NO_ATTACKSTRING || 0x800000)) == 0) && new_battlestruct.ptr->various.magicbounce == 0)
    {
        move_outcome.missed = 1;
        battlescripts_curr_instruction = (void*) 0x082DB07A; //no pp bs
        return;
    }
    hitmarker &= 0xFF7FFFFF;
    if ((hitmarker & HITMARKER_OBEYS) == 0 && !battle_participants[bank_attacker].status2.multiple_turn_move)
    {
        u8 disobedient = is_poke_disobedient();
        if (disobedient == 2)
        {
            hitmarker |= HITMARKER_OBEYS;
            return;
        }
        else if (disobedient)
        {
            move_outcome.missed = 1;
            return;
        }
    }
    hitmarker |= HITMARKER_OBEYS;
    if (protect_structs[bank_target].flag0_bouncemove && move_table[current_move].move_flags.flags.affected_by_magic_coat && new_battlestruct.ptr->various.magicbounce == 0) //magic coat check
    {
        protect_structs[bank_target].flag0_bouncemove = 0;
        last_used_moves[bank_attacker] = current_move;
        pressure_pp_lose(bank_attacker, bank_target, MOVE_MAGIC_COAT);
        new_battlestruct.ptr->various.magicbounce = 1;
        battlescript_push();
        battlescripts_curr_instruction = (void*) 0x082DB194;
        return;
    }
    else if ((battle_participants[bank_target].ability_id == ABILITY_MAGIC_BOUNCE && has_ability_effect(bank_target, 1, 1)) && move_table[current_move].move_flags.flags.affected_by_magic_coat && new_battlestruct.ptr->various.magicbounce == 0 && !protect_affects(current_move, 0) && !SEMI_INVULNERABLE(bank_target))
    {
        new_battlestruct.ptr->various.magicbounce = 1;
        last_used_moves[bank_attacker] = current_move;
        battlescript_push();
        battlescripts_curr_instruction = &magicbounce_bs;
        return;
    }
    for (u8 i = 0; i < no_of_all_banks; i++)
    {
        if (protect_structs[turn_order[i]].flag0_stealmove && move_table[current_move].move_flags.flags.affected_by_snatch)
        {
            pressure_pp_lose(bank_attacker, bank_target, MOVE_SNATCH);
            protect_structs[turn_order[i]].flag0_stealmove = 0;
            battle_scripting.active_bank = turn_order[i];
            battlescript_push();
            battlescripts_curr_instruction = (void*) 0x82DB1AC;
            return;
        }
    }
    if (special_statuses[bank_target].lightning_rod_redirected)
    {
        special_statuses[bank_target].lightning_rod_redirected = 0;
        if (new_battlestruct.ptr->various.stormdrain)
        {
            new_battlestruct.ptr->various.stormdrain = 0;
            last_used_ability = ABILITY_STORM_DRAIN;
        }
        else
            last_used_ability = ABILITY_LIGHTNING_ROD;
        battlescript_push();
        battlescripts_curr_instruction = (void*) 0x082DB53E;
        record_usage_of_ability(bank_target, last_used_ability);
        return;
    }
    if (protect_affects(current_move, 0)
        && !(check_if_twoturns_move(current_move) && battle_participants[bank_attacker].status2.multiple_turn_move)
        && !(current_move == MOVE_CURSE && is_of_type(bank_attacker, TYPE_GHOST)))
    {
        battle_communication_struct.field6 = 1;
        move_outcome.missed = 1;
        reset_several_turns_stuff(bank_attacker);
        move_hit_with_pbs[bank_target] = 0;
        move_type_hit_with_pbs[bank_target] = 0;
    }

    battlescripts_curr_instruction++;
    return;
}

void atk88_drain_damage()
{
    s32 damage = 0;
    if (!(new_battlestruct.ptr->bank_affecting[bank_attacker].heal_block && healblock_forbidden_moves(current_move, 0)))
    {
        u8 percent_restore = move_table[current_move].arg1;
        u8 rounding = move_table[current_move].arg2;
        damage = percent_lose(hp_dealt, 100 - percent_restore);
        if (rounding && __umodsi3(hp_dealt, damage * ((100 * hp_dealt + 100 - percent_restore) / 100)) && damage != hp_dealt)
            damage++;
        if (get_item_effect(bank_attacker, 1) == ITEM_EFFECT_BIGROOT)
            damage = percent_boost(damage, 30);
        if (damage == 0)
            damage = 1;
    }
    damage_loc = damage * -1;
    battlescripts_curr_instruction++;
    return;
}

u8 symbiosis_effect(u8 bank)
{
    u8 symbiosis = 0;
    if (new_battlestruct.ptr->bank_affecting[bank].item_used)
    {
        new_battlestruct.ptr->bank_affecting[bank].item_used = 0;
        u8 ally_bank = bank ^ 2;
        if (ability_battle_effects(20, bank, ABILITY_STATIC, 0, 0) && battle_participants[bank].held_item == 0 && battle_participants[ally_bank].held_item)
        {
            symbiosis = 1;
            battle_participants[bank].held_item = battle_participants[ally_bank].held_item;
            battle_participants[ally_bank].held_item = 0;
            active_bank = bank;
            prepare_setattributes_in_battle(0, 2, 0, 2, &battle_participants[bank].held_item);
            mark_buffer_bank_for_execution(active_bank);
            active_bank = ally_bank;
            prepare_setattributes_in_battle(0, 2, 0, 2, &battle_participants[ally_bank].held_item);
            mark_buffer_bank_for_execution(active_bank);
            battlescript_push();
            battlescripts_curr_instruction = &symbiosispass_bs;
            battle_scripting.active_bank = bank;
            bank_target = ally_bank;
            record_usage_of_ability(ally_bank, ABILITY_SYMBIOSIS);
        }
    }
    return symbiosis;
}

void atk6A_remove_item()
{
    u8 bank = get_battle_bank(read_byte(battlescripts_curr_instruction + 1));
    battlescripts_curr_instruction += 2;
    u16* item = &battle_participants[bank].held_item;
    battle_stuff_ptr.ptr->used_held_items[bank] = *item;
    *item = 0;
    active_bank = bank;
    if (symbiosis_effect(bank) == 0)
    {
        prepare_setattributes_in_battle(0, REQUEST_HELDITEM_BATTLE, 0, 2, item);
        mark_buffer_bank_for_execution(bank);
    }
    return;
}

void atkA3_disable_move()
{
    s8 position = get_move_position(bank_target, last_used_moves[bank_target]);
    if (position != -1 && disable_structs[bank_target].disabled_move == 0 && battle_participants[bank_target].current_pp[position])
    {
        u16 move = battle_participants[bank_target].moves[position];
        battlescripts_curr_instruction += 5;
        battle_text_buff1[0] = 0xFD;
        battle_text_buff1[1] = 2;
        battle_text_buff1[2] = move;
        battle_text_buff1[3] = move >> 8;
        battle_text_buff1[4] = 0xFF;
        disable_structs[bank_target].disabled_move = move;
        disable_structs[bank_target].disable_timer = 4;
    }
    else
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction + 1);
    return;
}

void atkA4_encore_move()
{
    u16 move = last_used_moves[bank_target];
    s8 position = get_move_position(bank_target, move);
    if (position != -1 && disable_structs[bank_target].encored_move == 0 && battle_participants[bank_target].current_pp[position]
        && move != MOVE_STRUGGLE && move != MOVE_ENCORE && move != MOVE_MIRROR_MOVE && move != MOVE_MIMIC && move != MOVE_TRANSFORM && move != MOVE_SKETCH)
    {
        battlescripts_curr_instruction += 5;
        disable_structs[bank_target].encored_move = move;
        disable_structs[bank_target].encore_timer = 3;
        disable_structs[bank_target].encored_index = position;
    }
    else
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction + 1);
    return;
}

void atkD0_set_taunt()
{
    void* failjump = (void*) read_word(battlescripts_curr_instruction + 1);
    if (disable_structs[bank_target].taunt_timer || ability_battle_effects(17, bank_target, ABILITY_AROMA_VEIL, 1, 0))
        battlescripts_curr_instruction = failjump;
    else if (battle_participants[bank_target].ability_id == ABILITY_OBLIVIOUS && has_ability_effect(bank_target, 1, 1))
    {
        battlescripts_curr_instruction = failjump;
        record_usage_of_ability(bank_target, ABILITY_OBLIVIOUS);
    }
    else
    {
        disable_structs[bank_target].taunt_timer = 3;
        battlescripts_curr_instruction += 5;
    }
    return;
}

void atkB5_furycutter_calc()
{
    u8* cutter_timer = &disable_structs[bank_attacker].fury_cutter_timer;
    if (!MOVE_WORKED)
    {
        *cutter_timer = 0;
        battlescripts_curr_instruction = (void*)0x82D8A5E;
    }
    else
    {
        if (last_used_moves[bank_attacker] != MOVE_FURY_CUTTER)
            *cutter_timer = 0;

        if (*cutter_timer < 4)
            *cutter_timer += 1;
        dynamic_base_power = move_table[current_move].base_power * *cutter_timer;
        battlescripts_curr_instruction++;
    }
    return;
}

void atkEB_set_type_to_terrain()
{
    u8 terrain_type;
    if (new_battlestruct.ptr->field_affecting.grassy_terrain)
        terrain_type = TYPE_GRASS;
    else if (new_battlestruct.ptr->field_affecting.misty_terrain)
        terrain_type = TYPE_FAIRY;
    else if (new_battlestruct.ptr->field_affecting.electic_terrain)
        terrain_type = TYPE_ELECTRIC;
    else
        terrain_type = terrain_to_type_table[battle_background];
    if (is_of_type(bank_attacker, terrain_type))
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction + 1);
    else
    {
        battlescripts_curr_instruction += 5;
        set_type(bank_attacker, terrain_type);
        battle_text_buff1[0] = 0xFD;
        battle_text_buff1[1] = 3;
        battle_text_buff1[2] = terrain_type;
        battle_text_buff1[3] = 0xFF;
    }
    return;
}

void atk0F_resultmessage()
{
    if (battle_execution_buffer == 0)
    {
        battlescripts_curr_instruction++;
        u16 string_id = 0;
        if (move_outcome.missed && !(move_outcome.not_affected && battle_communication_struct.field6 <= 2))
            string_id = some_strings_table[battle_communication_struct.field6];
        else if (move_outcome.super_effective && MOVE_WORKED)
            string_id = 0xDE;
        else if (move_outcome.not_very_effective && MOVE_WORKED)
            string_id = 0xDD;
        else if (move_outcome.not_affected)
            string_id = 0x1B;
        else if (move_outcome.failed)
            string_id = 0xE5;
        else if (move_outcome.one_hit_ko && MOVE_WORKED)
            string_id = 0xDA;
        if (move_outcome.endured)
        {
            move_outcome.hanged_on_using_item = 0;
            move_outcome.sturdied = 0;
            battlescript_push();
            battlescripts_curr_instruction = (void*) 0x82DB1C7;
        }
        else if (move_outcome.hanged_on_using_item && MOVE_WORKED)
        {
            move_outcome.hanged_on_using_item = 0;
            move_outcome.sturdied = 0;
            if (!last_used_item)
                last_used_item = battle_participants[bank_target].held_item;
            battlescript_push();
            battlescripts_curr_instruction = (void*) 0x82DB816;
        }
        else if (move_outcome.sturdied && MOVE_WORKED)
        {
            move_outcome.sturdied = 0;
            battlescript_push();
            battlescripts_curr_instruction = (void*) 0x82DB1C7;
        }
        if (string_id)
        {
            battle_communication_struct.is_message_displayed = 1;
            b_std_message(string_id, bank_attacker);
        }
    }
    return;
}

void atk70_record_ability_of_bank()
{
    u8 bank = get_battle_bank(read_byte(battlescripts_curr_instruction + 1));
    record_usage_of_ability(bank, battle_participants[bank].ability_id);
    battlescripts_curr_instruction += 2;
}

void atk77_set_protect_stuff()
{
    //Detect, Endure, King's Shield, Protect, Quick Guard, Spiky Shield, or Wide Guard.
    u8 fail = 0;
    u8 protects_team = 0;
    u8 attacker_side = is_bank_from_opponent_side(bank_attacker);
    u8* activity = &disable_structs[bank_attacker].protect_endure_effect;
    u16 previous_move = last_used_moves[bank_attacker];
    if (move_table[previous_move].script_id != 34 || previous_move == MOVE_MAT_BLOCK)
        *activity = 0;
    if (current_move_turn == no_of_all_banks - 1)
        fail = 1;
    else if (current_move != MOVE_MAT_BLOCK) //mat block doesn't check it
    {
        if (protect_chance_table[*activity] < rng())
            fail = 1;
        else //at this point move should work
        {
            switch (current_move)
            {
            case MOVE_PROTECT:
            case MOVE_DETECT:
                protect_structs[bank_attacker].flag0_protect = 1;
                break;
            case MOVE_KINGS_SHIELD:
                new_battlestruct.ptr->bank_affecting[bank_attacker].kings_shield = 1;
                break;
            case MOVE_SPIKY_SHIELD:
                new_battlestruct.ptr->bank_affecting[bank_attacker].spiky_shield = 1;
                break;
            case MOVE_ENDURE:
                protect_structs[bank_attacker].flag0_endure = 1;
                break;
            case MOVE_WIDE_GUARD:
                protects_team = 1;
                if (new_battlestruct.ptr->side_affecting[attacker_side].wide_guard)
                    fail = 1;
                else
                    new_battlestruct.ptr->side_affecting[attacker_side].wide_guard = 1;
                break;
            case MOVE_QUICK_GUARD:
                protects_team = 1;
                if (new_battlestruct.ptr->side_affecting[attacker_side].quick_guard)
                    fail = 1;
                else
                    new_battlestruct.ptr->side_affecting[attacker_side].quick_guard = 1;
                break;
            }
        }
    }
    else
    {
        protects_team = 1;
        switch (current_move)
        {
        case MOVE_MAT_BLOCK:
            if (new_battlestruct.ptr->side_affecting[attacker_side].mat_block)
                fail = 1;
            else
                new_battlestruct.ptr->side_affecting[attacker_side].mat_block = 1;
            break;
        case MOVE_CRAFTY_SHIELD:
            if (new_battlestruct.ptr->side_affecting[attacker_side].crafty_shield)
                fail = 1;
            else
                new_battlestruct.ptr->side_affecting[attacker_side].crafty_shield = 1;
            break;
        }
    }
    if (fail)
    {
        move_outcome.failed = 1;
        battlescripts_curr_instruction = (void*) 0x082D9F1C;
        *activity = 0;
    }
    else
    {
        battlescripts_curr_instruction++;
        *activity += 1;
        if (protects_team)
        {
            battle_communication_struct.multistring_chooser = 2;
            battle_text_buff1[0] = 0xFD;
            battle_text_buff1[1] = 2;
            battle_text_buff1[2] = current_move;
            battle_text_buff1[3] = current_move >> 8;
            battle_text_buff1[4] = 0xFF;
        }
        else if (current_move == MOVE_ENDURE)
            battle_communication_struct.multistring_chooser = 0;
        else
            battle_communication_struct.multistring_chooser = 1;
    }
    return;
    //table looks like this: endure, protects bank, protects team
}

void atk0C_datahpupdate()
{
    if (battle_execution_buffer == 0)
    {
        u8 bank = get_battle_bank(read_byte(battlescripts_curr_instruction + 1));
        battlescripts_curr_instruction += 2;
        active_bank = bank;
        if (!MOVE_WORKED)
        {
            if (special_statuses[bank].moveturn_losthp == 0)
                special_statuses[bank].moveturn_losthp = 0xFFFF;
        }
        else
        {
            u8 split = move_table[current_move].split;
            if (affected_by_substitute(bank))
            {
                u8* substitute_hp = &disable_structs[bank].substitute_hp;
                if (damage_loc > *substitute_hp)
                {
                    hp_dealt = *substitute_hp;
                    *substitute_hp = 0;
                    if (special_statuses[bank].moveturn_losthp == 0)
                        special_statuses[bank].moveturn_losthp = hp_dealt;
                }
                else
                {
                    *substitute_hp -= damage_loc;
                    hp_dealt = damage_loc;
                    if (special_statuses[bank].moveturn_losthp == 0)
                        special_statuses[bank].moveturn_losthp = damage_loc;
                }
                if (*substitute_hp == 0)
                {
                    battlescript_push();
                    battlescripts_curr_instruction = (void*) 0x082DB6FB;
                }
            }
            else
            {
                hitmarker &= 0xFFFFFEFF;
                u16* current_hp = &battle_participants[bank].current_hp;
                if (damage_loc >= 0) //damage is subbed
                {
                    if (hitmarker & 0x20)
                        hitmarker ^= 0x20;
                    else
                    {
                        taken_damage[bank] += damage_loc;
                        if (bank == bank_target)
                            hurt_by[bank] = bank_attacker;
                        else
                            hurt_by[bank] = bank_target;
                    }
                    if (damage_loc > *current_hp)
                        hp_dealt = *current_hp;
                    else
                        hp_dealt = damage_loc;
                    *current_hp -= hp_dealt;
                    if (special_statuses[bank].moveturn_losthp == 0 && !(hitmarker & 0x100000))
                        special_statuses[bank].moveturn_losthp = hp_dealt;
                    if (split == 0 && !(hitmarker & 0x100000))
                    {
                        special_statuses[bank].moveturn_losthp_physical = hp_dealt;
                        protect_structs[bank].physical_damage = hp_dealt;
                        if (bank == bank_attacker)
                        {
                            special_statuses[bank].moveturn_physical_bank = bank_target;
                            protect_structs[bank].counter_target = bank_target;
                        }
                        else
                        {
                            special_statuses[bank].moveturn_physical_bank = bank_attacker;
                            protect_structs[bank].counter_target = bank_attacker;
                        }
                    }
                    else if (split == 1 && !(hitmarker & 0x100000))
                    {
                        special_statuses[bank].moveturn_losthp_special = hp_dealt;
                        protect_structs[bank].special_damage = hp_dealt;
                        if (bank == bank_attacker)
                        {
                            special_statuses[bank].moveturn_special_bank = bank_target;
                            protect_structs[bank].mirrorcoat_target = bank_target;
                        }
                        else
                        {
                            special_statuses[bank].moveturn_special_bank = bank_attacker;
                            protect_structs[bank].mirrorcoat_target = bank_attacker;
                        }
                    }

                }
                else //damage is added
                {
                    *current_hp = *current_hp + (damage_loc * (-1));
                    if (*current_hp > battle_participants[bank].max_hp)
                        *current_hp = battle_participants[bank].max_hp;
                }
                hitmarker &= 0xFFEFFFFF;
                active_bank = bank;
                prepare_setattributes_in_battle(0, 0x2A, 0, 2, current_hp);
                mark_buffer_bank_for_execution(active_bank);
            }
        }
    }
    return;
}

void atkD4_wish_effect()
{
    u8* wish_duration = &battle_effects_duration.wish_duration[bank_attacker];
    u8* wish_bank = &battle_effects_duration.id_of_wish_user[bank_attacker];
    u8 fail = 0;
    switch (read_byte(battlescripts_curr_instruction + 1))
    {
    case 0: //prepare a wish
        if (*wish_duration)
            fail = 1;
        else
        {
            *wish_duration = 2;
            *wish_bank = battle_team_id_by_side[bank_attacker];
            u16 wish_hp = battle_participants[bank_attacker].max_hp / 2;
            if (wish_hp * 2 < battle_participants[bank_attacker].max_hp)
                wish_hp += 1;
            new_battlestruct.ptr->bank_affecting[bank_attacker].wish_hp = wish_hp;
        }
        break;
    case 1: //get wish effect
        if (battle_participants[bank_target].max_hp == battle_participants[bank_target].current_hp || new_battlestruct.ptr->bank_affecting[bank_target].heal_block)
            fail = 1;
        else
        {
            battle_text_buff1[0] = 0xFD;
            battle_text_buff1[1] = 4;
            battle_text_buff1[2] = bank_target;
            battle_text_buff1[3] = *wish_bank;
            battle_text_buff1[4] = 0xFF;
            damage_loc = new_battlestruct.ptr->bank_affecting[bank_target].wish_hp * (-1);
        }
        break;
    }
    if (fail)
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction + 2);
    else
        battlescripts_curr_instruction += 6;
    return;
}

void atk93_ko_move()
{
    u8 atk_lvl = battle_participants[bank_attacker].level;
    u8 def_lvl = battle_participants[bank_target].level;
    u8 fail = 0;
    if (atk_lvl < def_lvl)
    {
        fail = 1;
        battle_communication_struct.multistring_chooser = 1;
    }
    else
    {
        if (has_ability_effect(bank_target, 1, 1) && battle_participants[bank_target].ability_id == ABILITY_STURDY)
        {
            move_outcome.missed = 1;
            battlescripts_curr_instruction = (void*) 0x082DB552;
            record_usage_of_ability(bank_target, ABILITY_STURDY);
            return;
        }
        u8 accuracy = atk_lvl - def_lvl + move_table[current_move].accuracy;
        if ((status3[bank_attacker].always_hits && disable_structs[bank_attacker].always_hits_bank == bank_target) || accuracy >= 100 || percent_chance(accuracy))
        {
            damage_loc = battle_participants[bank_target].current_hp;
            move_outcome.super_effective = 0;
            move_outcome.not_very_effective = 0;
            move_outcome.one_hit_ko = 1;
        }
        else
        {
            fail = 1;
            battle_communication_struct.multistring_chooser = 0;
        }
    }
    if (fail)
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction + 1);
    else
        battlescripts_curr_instruction += 5;
    return;
}

void atkBE_rapidspin_away()
{
    u8 banks_side = is_bank_from_opponent_side(bank_attacker);
    if (battle_participants[bank_attacker].status2.trapped_in_wrap)
    {
        battle_participants[bank_attacker].status2.trapped_in_wrap = 0;
        battle_scripting.active_bank = bank_target;
        bank_target = battle_stuff_ptr.ptr->trapper[bank_attacker];
        battle_text_buff1[0] = 0xFD;
        battle_text_buff1[1] = 2;
        battle_text_buff1[2] = battle_stuff_ptr.ptr->trapped_move[bank_attacker];
        battle_text_buff1[3] = battle_stuff_ptr.ptr->trapped_move[bank_attacker] >> 8;
        battle_text_buff1[4] = 0xFF;
        battlescript_push();
        battlescripts_curr_instruction = (void*) 0x82DAFC5;
    }
    else if (status3[bank_attacker].leech_seed)
    {
        status3[bank_attacker].leech_seed = 0;
        status3[bank_attacker].leech_seed_hp_receiver = 0;
        battlescript_push();
        battlescripts_curr_instruction = (void*) 0x82DAFD6;
    }
    else if (side_affecting_halfword[banks_side].spikes_on)
    {
        side_affecting_halfword[banks_side].spikes_on = 0;
        side_timers[banks_side].spikes_amount = 0;
        battlescript_push();
        battlescripts_curr_instruction = (void*) 0x82DAFDD;
    }
    else if (new_battlestruct.ptr->side_affecting[banks_side].toxic_spikes_psn)
    {
        new_battlestruct.ptr->side_affecting[banks_side].toxic_spikes_psn = 0;
        new_battlestruct.ptr->side_affecting[banks_side].toxic_spikes_badpsn = 0;
        battlescript_push();
        battlescripts_curr_instruction = &rapidspinontoxicspikes_bs;
    }
    else if (new_battlestruct.ptr->side_affecting[banks_side].stealthrock)
    {
        new_battlestruct.ptr->side_affecting[banks_side].stealthrock = 0;
        battlescript_push();
        battlescripts_curr_instruction = &rapidspinonstealthrock_bs;
    }
    else if (new_battlestruct.ptr->side_affecting[banks_side].sticky_web)
    {
        new_battlestruct.ptr->side_affecting[banks_side].sticky_web = 0;
        battlescript_push();
        battlescripts_curr_instruction = &rapidspinonstickyweb_bs;
    }
    else
        battlescripts_curr_instruction++;
    return;
}

u8 can_select_this_random_move(u16 move)
{
    u8 can = 1;
    u8 script_id = move_table[move].script_id;
    switch (move)
    {
    case MOVE_ASSIST:
        if (script_id == 34 || script_id == 73 || script_id == 72 || script_id == 123 || script_id == 99) //fly/dig/dive plus protect-like moves plus roar/whirlwind plus trick/switcheroo plus covet/thief
            can = 0;
        else if (move == MOVE_BESTOW || move == MOVE_DRAGON_TAIL || move == MOVE_CIRCLE_THROW || move == MOVE_FOCUS_PUNCH || move == MOVE_RAGE_POWDER || move == MOVE_SHADOW_FORCE || move == MOVE_FEINT || move == MOVE_PHANTOM_FORCE || move == MOVE_DESTINY_BOND || move == MOVE_HOLD_HANDS || move == MOVE_TRANSFORM)
            can = 0;
        else
            goto SHAREDWITHASSIST;
        break;
    case MOVE_SLEEP_TALK:
        if (script_id == 71) //two-turn moves
            can = 0;
        break;
        SHAREDWITHASSIST:
        if(move == MOVE_ASSIST || move == MOVE_SLEEP_TALK || move == MOVE_BELCH || move == MOVE_BIDE || move == MOVE_CHATTER || move == MOVE_COPYCAT || move == MOVE_HOLD_HANDS || move == MOVE_ME_FIRST || move == MOVE_METRONOME || move == MOVE_MIMIC || move == MOVE_MIRROR_MOVE || move == MOVE_NATURE_POWER || move == MOVE_SKETCH || move == MOVE_STRUGGLE)
            can = 0;
        break;
    }
    return can;
}

u16 metronome_forbidden_moves[] = {MOVE_AFTER_YOU, MOVE_ASSIST, MOVE_BELCH, MOVE_BESTOW, MOVE_CELEBRATE, MOVE_CHATTER, MOVE_COPYCAT, MOVE_COUNTER, MOVE_COVET, MOVE_CRAFTY_SHIELD, MOVE_DESTINY_BOND, MOVE_DETECT, MOVE_DIAMOND_STORM, MOVE_ENDURE, MOVE_FEINT, MOVE_FOCUS_PUNCH, MOVE_FOLLOW_ME, MOVE_FREEZE_SHOCK, MOVE_HAPPY_HOUR, MOVE_HELPING_HAND, MOVE_HOLD_HANDS, MOVE_HYPERSPACE_HOLE, MOVE_ICE_BURN, MOVE_KINGS_SHIELD, MOVE_LIGHT_OF_RUIN, MOVE_MAT_BLOCK, MOVE_ME_FIRST, MOVE_METRONOME, MOVE_MIMIC, MOVE_MIRROR_COAT, MOVE_MIRROR_MOVE, MOVE_NATURE_POWER, MOVE_PROTECT, MOVE_QUASH, MOVE_QUICK_GUARD, MOVE_RAGE_POWDER, MOVE_RELIC_SONG, MOVE_SECRET_SWORD, MOVE_SKETCH, MOVE_SLEEP_TALK, MOVE_SNARL, MOVE_SNATCH, MOVE_SNORE, MOVE_SPIKY_SHIELD, MOVE_STEAM_ERUPTION, MOVE_STRUGGLE, MOVE_SWITCHEROO, MOVE_TECHNO_BLAST, MOVE_THIEF, MOVE_THOUSAND_ARROWS, MOVE_THOUSAND_WAVES, MOVE_TRANSFORM, MOVE_TRICK, MOVE_VCREATE, MOVE_WIDE_GUARD, 0xFFFF};

void atk9E_metronome_chooser()
{
    #define MOVE_MAX 621
    do
    {
        current_move = __umodsi3(rng(), MOVE_MAX) + 1;
    } while(find_move_in_table(current_move, &metronome_forbidden_moves[0]));

    hitmarker &= 0xFFFFFBFF;
    battlescripts_curr_instruction = get_move_battlescript_ptr(current_move);
    bank_target = get_target_of_move(current_move, 0, 0);
    set_attacking_move_type();
}

void atkC5_hide_pre_attack()
{
    switch (current_move)
    {
    case MOVE_BOUNCE:
    case MOVE_FLY:
        status3[bank_attacker].on_air = 1;
        break;
    case MOVE_DIG:
        status3[bank_attacker].underground = 1;
        break;
    case MOVE_DIVE:
        status3[bank_attacker].underwater = 1;
        break;
    case MOVE_PHANTOM_FORCE:
    case MOVE_SHADOW_FORCE:
        status3[bank_attacker].phantomforce = 1;
        break;
    }
    battlescripts_curr_instruction++;
}

void atkC6_unhide_post_attack()
{
    status3[bank_attacker].on_air = 0;
    status3[bank_attacker].underwater = 0;
    status3[bank_attacker].underground = 0;
    status3[bank_attacker].phantomforce = 0;
    battlescripts_curr_instruction++;
    return;
}

u16 naturepower_table[] = {MOVE_SEED_BOMB /*GRASS*/, MOVE_RAZOR_LEAF /*LONG GRASS*/, MOVE_EARTHQUAKE /*SAND*/, MOVE_HYDRO_PUMP /*UNDERWATER*/, MOVE_SURF /*WATER*/, MOVE_BUBBLE_BEAM /*POND*/, MOVE_ROCK_SLIDE /*ROCK*/, MOVE_POWER_GEM /*CAVE*/, MOVE_TRI_ATTACK, MOVE_TRI_ATTACK};

void atkCC_nature_power()
{
    hitmarker &= 0xFFFFFBFF;
    if (new_battlestruct.ptr->field_affecting.grassy_terrain)
        current_move = MOVE_ENERGY_BALL;
    else if (new_battlestruct.ptr->field_affecting.misty_terrain)
        current_move = MOVE_MOONBLAST;
    else if (new_battlestruct.ptr->field_affecting.electic_terrain)
        current_move = MOVE_THUNDERBOLT;
    else
        current_move = naturepower_table[battle_background];
    bank_target = get_target_of_move(current_move, 0, 0);
    set_attacking_move_type();
    battlescript_custom_push(get_move_battlescript_ptr(current_move));
    battlescripts_curr_instruction++;
    return;
}

void atkB7_present_calc()
{
    u32 random_rumber = __umodsi3(rng() + 100, 101);
    if (random_rumber < 40)
        dynamic_base_power = 40;
    else if (random_rumber < 70)
        dynamic_base_power = 80;
    else if (random_rumber < 80)
        dynamic_base_power = 120;
    else
    {
        dynamic_base_power = 0;
        if (battle_participants[bank_target].current_hp >= battle_participants[bank_target].max_hp)
            battlescripts_curr_instruction = (void*) 0x082D9EFB;
        else if (new_battlestruct.ptr->bank_affecting[bank_target].heal_block)
            battlescripts_curr_instruction = &healblock_prevents_usage_bs;
        else
        {
            damage_loc = get_1_4_of_max_hp(bank_target) * (-1);
            battlescripts_curr_instruction = (void*) 0x082D9EE1;
        }

    }
    if (dynamic_base_power)
        battlescripts_curr_instruction = (void*) 0x082D8A30;
}

void atk13_printfromtable()
{
    if (battle_execution_buffer == 0)
    {
        u16* string_id = (u16*) read_word(battlescripts_curr_instruction + 1);
        string_id += battle_communication_struct.multistring_chooser;
        u8 lower = battle_scripting.stat_changer & 0x80;
        u16 string_to_read = *string_id;
        switch (*string_id)
        {
        case 0x3D:
            if (lower)
                string_to_read = 0x3E;
            break;
        case 0x3E:
            if (lower == 0)
                string_to_read = 0x3D;
            break;
        }
        battle_communication_struct.is_message_displayed = 1;
        b_std_message(string_to_read, bank_attacker);
        battlescripts_curr_instruction += 5;
        u8 bank = battle_scripting.active_bank;
        if (lower && new_battlestruct.ptr->bank_affecting[bank].stat_lowered)
        {
            new_battlestruct.ptr->bank_affecting[bank].stat_lowered = 0;
            if (check_ability(bank, ABILITY_DEFIANT) && battle_participants[bank].atk_buff != 0xC)
            {
                battle_scripting.stat_changer = 0x21;
                battlescript_push();
                battlescripts_curr_instruction = &defiant_bs;
                record_usage_of_ability(bank, ABILITY_DEFIANT);
            }
            else if (check_ability(bank, ABILITY_COMPETITIVE) && battle_participants[bank].sp_atk_buff != 0xC)
            {
                battle_scripting.stat_changer = 0x24;
                battlescript_push();
                battlescripts_curr_instruction = &defiant_bs;
                record_usage_of_ability(bank, ABILITY_COMPETITIVE);
            }
        }

    }
    return;
}

void atk56_prepare_fainting_cry()
{
    u8 bank = get_battle_bank(read_byte(battlescripts_curr_instruction + 1));
    battlescripts_curr_instruction += 2;
    active_bank = bank;
    prepare_fainting_cry(0);
    mark_buffer_bank_for_execution(bank);
    u8 side = is_bank_from_opponent_side(bank);
    u8 id_by_side=battle_team_id_by_side[bank];
    if ((side==0 && (new_battlestruct.ptr->mega_related.party_mega_check&bits_table[id_by_side]))
        || (side==1 && (new_battlestruct.ptr->mega_related.ai_party_mega_check&bits_table[id_by_side])))
    {
        revert_mega_to_normalform(id_by_side, side);
    }
    new_battlestruct.ptr->side_affecting[side].ally_fainted_last_turn = 2;
}

void atkE1_intimidate_loop()
{
    u8 bank = battle_stuff_ptr.ptr->intimidate_user;
    battle_scripting.active_bank = bank;
    u8 side = is_bank_from_opponent_side(bank);
    battle_text_buff1[0] = 0xFD;
    battle_text_buff1[1] = 9;
    battle_text_buff1[2] = battle_participants[bank].ability_id;
    battle_text_buff1[3] = 0xFF;
    while (bank_target < no_of_all_banks)
    {
        if (side != is_bank_from_opponent_side(bank_target) && is_bank_present(bank_target))
        {
            battlescripts_curr_instruction += 5;
            return;
        }
        bank_target++;
    }
    battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction + 1);
    return;
}

void atk47_graphical_statchange()
{
    battlescripts_curr_instruction++;
    battle_scripting.field11 = 0;
    u8 statchanger_value = battle_scripting.stat_changer & 0xF0;
    if (statchanger_value == 0x20) //double raise
        statchanger_value = 0x27;
    else if (statchanger_value == 0x10) //single raise
        statchanger_value = 0xF;
    else if (statchanger_value == 0x90) //single lower
        statchanger_value = 0x16;
    else if (statchanger_value == 0xA0) //double lower
        statchanger_value = 0x2E;
    else if (statchanger_value > 0xA0) //triple lower
        statchanger_value = 0x37;
    else //triple raise
    {
        battle_scripting.field10 = 0x38;
        return;
    }
    battle_scripting.field10 = (battle_scripting.stat_changer & 0xF) + statchanger_value - 1;
    return;
}
