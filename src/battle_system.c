#include "types.h"
#include "defines.h"
#include "battle_locations.h"
#include "battle_structs.h"
#include "vanilla_functions.h"
#include "static_references.h"
#include "new_battle_struct.h"

u8 check_ability(u8 bank, u8 ability);
u8 change_stats(s8 arg1, s8 arg2, s8 arg3, void* battlescript_if_fails);
void statustoeffect();
u8 find_move_in_table(u16 move, u16 table_ptr[]);
u8 healblock_forbidden_moves(u16 move, u8 with_leechseed);
u8 gravity_forbidden_move(u16 move);
u8 embargo_forbidden_move(u16 move);

u8 get_attacking_move_type()
{
    u8 move_type=battle_stuff_ptr.ptr->dynamic_move_type&0x3F;
    if(!move_type)
    {
        if(check_ability(bank_attacker,ABILITY_NORMALIZE))
            move_type = TYPE_NORMAL;
        else
            move_type = move_table[current_move].type;
        if(move_type==TYPE_NORMAL && (new_battlestruct.ptr->field_affecting.ion_deluge || new_battlestruct.ptr->bank_affecting[bank_target].electrify))
            move_type = TYPE_ELECTRIC;
    }
    return move_type;
}

u8 get_item_effect(u8 bank, u8 check_negating_effects)
{
    if (check_negating_effects)
    {
        if (battle_participants[bank].ability_id == ABILITY_KLUTZ || new_battlestruct.ptr->field_affecting.magic_room || new_battlestruct.ptr->bank_affecting[bank].embargo)
            return ITEM_EFFECT_NOEFFECT;
    }
    if (battle_participants[bank].held_item == ITEM_ENIGMABERRY)
    {
        return enigma_berry_battle[bank].battle_effect_x12;
    }
    else
    {
        return get_item_x12_battle_function(battle_participants[bank].held_item);
    }
}

u8 is_of_type(u8 bank, u8 type)
{
    if (battle_participants[bank].type1 == type || battle_participants[bank].type2 == type || new_battlestruct.ptr->bank_affecting[bank].type3 == type)
        return true;
    else
        return false;
}

void set_type(u8 bank, u8 type)
{
    battle_participants[bank].type1 = type;
    battle_participants[bank].type2 = type;
    new_battlestruct.ptr->bank_affecting[bank].type3 = type;
    return;
}

u32 two_options_rand(u32 option1, u32 option2)
{
    if (rng() & 1)
        return option1;
    return option2;
}

u8 weather_abilities_effect()
{
    if (ability_battle_effects(19, 0, ABILITY_AIR_LOCK, 0, 0) || ability_battle_effects(19, 0, ABILITY_CLOUD_NINE, 0, 0))
        return false;
    else
        return true;
}

u32 get_1_16_of_max_hp(u8 bank)
{
    u32 num = (battle_participants[bank].max_hp) >> 4;
    if (num < 1)
    {
        num = 1;
    }
    return num;
}

u32 get_1_8_of_max_hp(u8 bank)
{
    u32 num = (battle_participants[bank].max_hp) >> 3;
    if (num < 1)
    {
        num = 1;
    }
    return num;
}

u32 get_1_4_of_max_hp(u8 bank)
{
    u32 num = (battle_participants[bank].max_hp) >> 2;
    if (num < 1)
    {
        num = 1;
    }
    return num;
}

u8 percent_chance(u8 percent)
{
    u8 num = __umodsi3(rng(), 100) + 1;
    if (num <= percent)
        return true;
    else
        return false;
}

s8 get_move_position(u8 bank, u16 move)
{
    for (u8 i = 0; i < 4; i++)
    {
        if (battle_participants[bank].moves[i] == move)
            return i;
    }
    return -1;
}

u8 should_cherrim_change(u8 bank)
{
    u8 effect = false;
    if (!(battle_participants[bank].ability_id == ABILITY_FLOWER_GIFT && battle_participants[bank].poke_species == POKE_CHERRIM))
    {
        return effect;
    }
    if ((battle_weather.flags.sun || battle_weather.flags.permament_sun || battle_weather.flags.harsh_sun) && weather_abilities_effect())
    {
        if (!new_battlestruct.ptr->bank_affecting[bank].sunshine_form)
        {
            effect = 2;
            new_battlestruct.ptr->bank_affecting[bank].sunshine_form = 1;
        }
    }
    else
    {
        if (new_battlestruct.ptr->bank_affecting[bank].sunshine_form)
        {
            effect = 1;
            new_battlestruct.ptr->bank_affecting[bank].sunshine_form = 0;
        }
    }
    return effect;
}

void copy_status_condition_text(u8 bank, u8 confusion)
{
    u32* status_flag;
    if (battle_participants[bank].status.flags.sleep)
    {
        status_flag = &sleep_status_flag;
    }
    else if (battle_participants[bank].status.flags.poison || battle_participants[bank].status.flags.toxic_poison)
    {
        status_flag = &poison_status_flag;
    }
    else if (battle_participants[bank].status.flags.freeze)
    {
        status_flag = &freeze_status_flag;
    }
    else if (battle_participants[bank].status.flags.paralysis)
    {
        status_flag = &paralysis_status_flag;
    }
    else if (battle_participants[bank].status.flags.burn)
    {
        status_flag = &burn_status_flag;
    }
    else if (confusion && battle_participants[bank].status2.confusion)
    {
        status_flag = (void*)0x0831BC98;
    }
    else
    {
        return;
    }
    strcpy_xFF_terminated_0(&battle_text_buff1, status_flag);
    return;
}

u8 type_effectiveness_table[TYPE_FAIRY-0x4][TYPE_FAIRY-0x4] = {
    {10,10,10,10,10,05,10,00,05,10,10,10,10,10,10,10,10,10,10}, //normal
    {20,10,05,05,10,20,05,00,20,10,10,10,10,10,05,20,10,20,05}, //fight
    {10,20,10,10,10,05,20,10,05,10,10,10,20,05,10,10,10,10,10}, //flying
    {10,10,10,05,05,05,10,05,00,10,10,10,20,10,10,10,10,10,20}, //poison
    {10,10,00,20,10,20,05,10,20,10,20,10,05,20,10,10,10,10,10}, //ground
    {10,05,20,10,05,10,20,10,05,10,20,10,10,10,10,20,10,10,10}, //rock
    {10,05,05,05,10,10,10,05,05,10,05,10,20,10,20,10,10,20,05}, //bug
    {00,10,10,10,10,10,10,20,10,10,10,10,10,10,20,10,10,05,10}, //ghost
    {10,10,10,10,10,20,10,10,05,10,05,05,10,05,10,20,10,10,20}, //steel
    {10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10}, //egg
    {10,10,10,10,10,05,20,10,20,10,05,05,20,10,10,20,05,10,10}, //fire
    {10,10,10,10,20,20,10,10,10,10,20,05,05,10,10,10,05,10,10}, //water
    {10,10,05,05,20,20,05,10,05,10,05,20,05,10,10,10,05,10,10}, //grass
    {10,10,20,10,00,10,10,10,10,10,10,20,05,05,10,10,05,10,10}, //electric
    {10,20,10,20,10,10,10,10,05,10,10,10,10,10,05,10,10,00,10}, //psychic
    {10,10,20,10,20,10,10,10,05,10,05,05,20,10,10,05,20,10,10}, //ice
    {10,10,10,10,10,10,10,10,05,10,10,10,10,10,10,10,20,10,00}, //dragon
    {10,05,10,10,10,10,10,20,10,10,10,10,10,10,20,10,10,05,05}, //dark
    {10,20,10,05,10,10,10,10,05,10,05,10,10,10,10,10,20,20,10}  //fairy
};

u16 get_airborne_state(u8 bank, u8 mode, u8 check_levitate)
{
    // return 0 grounded due to gravity
    // return 1 grounded due to iron ball or smack down or ingrain
    // return 2 for not grounded nor airborne
    // return 3 airborne due to flying type or telekinesis
    // return 4 airborne due to levitate
    // mode 1 for damage calc && 0 for hazards etc.
    if (new_battlestruct.ptr->field_affecting.gravity)
        return 0;
    if (get_item_effect(bank, true) == ITEM_EFFECT_IRONBALL
        || new_battlestruct.ptr->bank_affecting[bank].smacked_down
        || status3[bank].rooted)
        return 1;
    if (check_levitate && battle_participants[bank].ability_id == ABILITY_LEVITATE && has_ability_effect(bank,mode,1))
        return 4;
    if ((mode==0 && is_of_type(bank,TYPE_FLYING)) || get_item_effect(bank, true) == ITEM_EFFECT_AIRBALLOON ||
        new_battlestruct.ptr->bank_affecting[bank].magnet_rise || new_battlestruct.ptr->bank_affecting[bank].telekinesis)
        return 3;
    return 2;
}

void handle_type_immune_ability(u8 def_bank, u8 string_chooser, u8 ability)
{
    move_outcome.missed=1;
    move_outcome.failed=1;
    move_outcome.not_affected=1;
    last_used_ability=ability;
    move_hit_with_pbs[def_bank]=0;
    move_type_hit_with_pbs[def_bank]=0;
    record_usage_of_ability(def_bank,ability);
    battle_communication_struct.field6=string_chooser;
}

u16 damage_type_effectiveness_update(u8 attacking_type, u8 defending_type, u8 atk_bank, u8 def_bank, u16 chained_effect, u8 airstatus)
{
    u8 effect,atype=attacking_type,dtype=defending_type;
    if(!chained_effect || atype==TYPE_EGG || dtype==TYPE_EGG)
        return chained_effect;

    if(atype>=TYPE_FAIRY)
        atype=atype-5;

    if(dtype>=TYPE_FAIRY)
        dtype=dtype-5;

    effect=type_effectiveness_table[atype][dtype];

    if (new_battlestruct.ptr->various.inverse_battle)
    {
        if (effect == 20)
        {
            effect = 5;
        }
        else if (effect == 5 || effect == 0)
        {
            effect = 20;
        }
    }

    if ((((attacking_type == TYPE_NORMAL || attacking_type == TYPE_FIGHTING) && defending_type == TYPE_GHOST && ((battle_participants[def_bank].status2.foresight))) || battle_participants[atk_bank].ability_id == ABILITY_SCRAPPY) && effect == 0)
    {
        effect = 10;
    }

    else if ((attacking_type == TYPE_PSYCHIC) && defending_type == TYPE_DARK && new_battlestruct.ptr->bank_affecting[def_bank].miracle_eyed && effect == 0)
    {
        effect = 10;
    }

    else if (effect == 0 && (get_item_effect(def_bank, true) == ITEM_EFFECT_RINGTARGET))
    {
        effect = 10;
        }
    else if (battle_weather.flags.air_current && defending_type == TYPE_FLYING && effect == 20 && weather_abilities_effect())
    {
        effect = 10;
    }
    else if (attacking_type == TYPE_GROUND)
    {
        if(effect == 0 && (airstatus<2 || current_move==MOVE_THOUSAND_ARROWS)) // grounded pokemon
            effect = 10;
        else if(airstatus>2 && current_move!=MOVE_THOUSAND_ARROWS)
            effect = 0;
    }
    else if (current_move == MOVE_FREEZEDRY && defending_type == TYPE_WATER)
    {
        effect = 20;
    }
    switch(effect)
    {
        case 0:
            chained_effect = 0;
            break;
        case 5:
            chained_effect = chained_effect>>1;
            break;
        case 20:
            chained_effect = chained_effect<<1;
            break;
    }
    return chained_effect;
}

u16 apply_type_effectiveness(u16 chained_effect, u8 move_type, u8 target_bank, u8 atk_bank, u8 airstatus)
{
    u8 defender_type1 = battle_participants[target_bank].type1;
    u8 defender_type2 = battle_participants[target_bank].type2;
    u8 defender_type3 = new_battlestruct.ptr->bank_affecting[target_bank].type3;
    //set different types
    if (defender_type2 == defender_type1)
        defender_type2 = TYPE_EGG;
    if (defender_type3 == defender_type1 || defender_type3 == defender_type2)
        defender_type3 = TYPE_EGG;
    chained_effect = damage_type_effectiveness_update(move_type, defender_type1, atk_bank, target_bank, chained_effect, airstatus);
    chained_effect = damage_type_effectiveness_update(move_type, defender_type2, atk_bank, target_bank, chained_effect, airstatus);
    chained_effect = damage_type_effectiveness_update(move_type, defender_type3, atk_bank, target_bank, chained_effect, airstatus);
    return chained_effect;
}

u16 dual_type_moves[]={MOVE_FLYING_PRESS,0xFFFF};

u16 type_effectiveness_calc(u16 move, u8 move_type, u8 atk_bank, u8 def_bank, u8 effects_handling_and_recording)
{
    u8 airstatus = get_airborne_state(def_bank,1,effects_handling_and_recording);
    u16 chained_effect = 64;
    for (u8 i = 0; dual_type_moves[i]!= 0xFFFF; i++)
    {
        if (move == dual_type_moves[i])
        {
            chained_effect = apply_type_effectiveness(chained_effect, move_table[move].arg2,
                                                      def_bank, atk_bank, airstatus);
            break;
        }
    }
    chained_effect = apply_type_effectiveness(chained_effect, move_type, def_bank, atk_bank, airstatus);
    if(chained_effect>64)
        move_outcome.super_effective=1;
    else if (chained_effect==0)
        move_outcome.not_affected=1;
    else if (chained_effect<64)
        move_outcome.not_very_effective=1;
    if(effects_handling_and_recording)
    {
        if(!move_outcome.super_effective && has_ability_effect(def_bank,1,1) &&
            battle_participants[def_bank].ability_id==ABILITY_WONDER_GUARD)
        {
            chained_effect = 0;
            handle_type_immune_ability(def_bank,3,ABILITY_WONDER_GUARD);
        }
        else if(airstatus==4 && chained_effect==0)
            handle_type_immune_ability(def_bank,4,ABILITY_LEVITATE);
        if(move_outcome.not_affected)
            protect_structs[atk_bank].flag1_noteffective=1;
    }
    return chained_effect;
}

u16 get_forewarn_move(u8 bank)
{
    u16 most_powerful_move = 0;
    s16 best_power = - 1;
    u8 banks_side = is_bank_from_opponent_side(bank);
    for (u8 i2 = 0; i2 < no_of_all_banks; i2++)
    {
        if (banks_side != is_bank_from_opponent_side(i2) && battle_participants[i2].current_hp)
        {
            for (u8 i = 0; i < 4; i++)
            {
                u16 curr_move = battle_participants[i2].moves[i];
                if (move_table[curr_move].base_power > best_power)
                {
                    most_powerful_move = curr_move;
                    best_power = move_table[curr_move].base_power;
                }
                else if (move_table[curr_move].base_power == best_power)
                {
                    most_powerful_move = two_options_rand(most_powerful_move, curr_move);
                }
                else if (move_table[curr_move].base_power == 1)
                {
                        u16 curr_power;
                        if (move_table[curr_move].script_id == 38) //one hit ko moves
                            curr_power = 160;
                        else if (curr_move == MOVE_COUNTER || curr_move == MOVE_METAL_BURST || curr_move == MOVE_MIRROR_COAT)
                            curr_power = 120;
                        else
                        {
                            u8 j = 0;
                            while (true)
                            {
                                if (forewarn_moves[j] == curr_move)
                                {
                                    curr_power = 80;
                                    break;
                                }
                                else if (forewarn_moves[j] == 0xFFFF)
                                {
                                    curr_power = 1;
                                    break;
                                }
                                j++;
                            }
                        }
                        if (curr_power > best_power)
                        {
                            most_powerful_move = curr_move;
                            best_power = curr_power;
                        }
                        else if (curr_power == best_power)
                        {
                            most_powerful_move = two_options_rand(most_powerful_move, curr_move);
                        }
                    }
            }
        }
    }
    if (best_power == 0)
        return 0;
    else
        return most_powerful_move;
}

u8 anticipation_shudder(u8 bank)
{
    u8 shudder = false;
    u8 banks_side = is_bank_from_opponent_side(bank);
    for (u8 i2 = 0; i2 < no_of_all_banks; i2++)
    {
        if (banks_side != is_bank_from_opponent_side(i2) && battle_participants[i2].current_hp)
        {
            for (u8 i = 0; i < 4; i++)
            {
                if (move_table[battle_participants[i2].moves[i]].script_id == 38) //one hit ko move check
                {
                    shudder = true;
                    break;
                }
                else
                {
                    if (move_table[battle_participants[i2].moves[i]].base_power)
                    {
                        u16 move_now = current_move;
                        current_move = battle_participants[i2].moves[i];
                        u16 damage = type_effectiveness_calc(battle_participants[i2].moves[i],
                                    move_table[battle_participants[i2].moves[i]].type,bank,i2,0);
                        current_move = move_now;
                        if (damage > 64)
                        {
                            shudder = true;
                            break;
                        }
                    }
                }
            }
        }
    }
    return shudder;
}

u8 prepare_castform_switch(u8 effect, u8 bank)
{
    if (effect)
    {
        void* bs_castform = (void*) 0x082DB4A9;
        execute_battle_script(bs_castform);
        battle_scripting.active_bank = bank;
        battle_stuff_ptr.ptr->castform_switch_form = effect - 1;
    }
    return effect;
}

u8 has_ability_effect(u8 bank, u8 mold_breaker, u8 gastro)
{
    u8 attacker_ability;
    if (gastro && new_battlestruct.ptr->bank_affecting[bank].gastro_acided)
        return false;
    if (mold_breaker)
    {   attacker_ability=battle_participants[bank_attacker].ability_id;
        if(has_ability_effect(bank_attacker,0,1) && (attacker_ability==ABILITY_MOLD_BREAKER
               || attacker_ability==ABILITY_TERAVOLT || attacker_ability==ABILITY_TURBOBLAZE))
            return false;
    }
    return true;
}

u8 ability_battle_effects(u8 switch_id, u8 bank, u8 ability_to_check, u8 special_cases_argument, u16 move)
{
    u8 effect = false;
    u16 curr_move;
    if (move > 0 && move < 622)
        curr_move = move;
    else
        curr_move = current_move;
    u8 move_type;
    if (battle_stuff_ptr.ptr->dynamic_move_type)
        move_type = battle_stuff_ptr.ptr->dynamic_move_type & 0x3F;
    else
        move_type = move_table[curr_move].type;

    if (special_cases_argument)
        last_used_ability = special_cases_argument;
    else
        last_used_ability = battle_participants[bank].ability_id;

    u8 bank_side = is_bank_from_opponent_side(bank);
    u8 common_effect=0;
    void* script_ptr;
    u8 adder=0;
    u8 move_effect=0;
    u8 contact =move_table[curr_move].move_flags.flags.makes_contact;
    switch (switch_id)
    {
    case 0: //switch-in abilities
        if(status3[bank].switchinlock)
            break;
        u8 dontlock=0;
        switch (last_used_ability)
        {
        case ABILITY_MOLD_BREAKER:
            script_ptr = &moldbreaker_message_bs;
            common_effect=1;
            break;
        case ABILITY_TURBOBLAZE:
            script_ptr = &turboblaze_bs;
            common_effect=1;
            break;
        case ABILITY_TERAVOLT:
            script_ptr = &terravolt_bs;
            common_effect=1;
            break;
        case ABILITY_PRESSURE:
            script_ptr = &pressure_bs;
            common_effect=1;
            break;
        case ABILITY_UNNERVE:
            script_ptr = &unnerve_bs;
            common_effect=1;
            break;
        case ABILITY_AURA_BREAK:
            script_ptr = &aurabreak_bs;
            common_effect=1;
            break;
        case ABILITY_FAIRY_AURA:
            script_ptr = &fairyaura_bs;
            common_effect=1;
            break;
        case ABILITY_DARK_AURA:
            script_ptr = &darkaura_bs;
            common_effect=1;
            break;
        case ABILITY_DOWNLOAD:
            {
                u16 def_sum = 0;
                u16 spdef_sum = 0;
                u8 banks_side = is_bank_from_opponent_side(bank);
                for (u8 i = 0; i < no_of_all_banks; i++)
                {
                    if (i != bank && banks_side!= is_bank_from_opponent_side(i))
                    {
                        def_sum += battle_participants[i].def;
                        spdef_sum += battle_participants[i].sp_def;
                    }
                }
                if ((def_sum == spdef_sum && two_options_rand(0, 1)) || (spdef_sum < def_sum))
                {
                    battle_participants[bank].sp_atk_buff++;
                    script_ptr = &downloadspatk_bs;
                }
                else
                {
                    battle_participants[bank].atk_buff++;
                    script_ptr = &downloadatk_bs;
                }
                    effect = 1;
                    battle_scripting.active_bank = bank;
                    execute_battle_script(script_ptr);
                    break;
            }
        case ABILITY_DROUGHT:
            if (!(battle_weather.flags.sun || battle_weather.flags.permament_sun || battle_weather.flags.harsh_sun || battle_weather.flags.heavy_rain || battle_weather.flags.air_current))
            {
                effect = true;
                battle_weather.int_bw = weather_sun;

                if (get_item_effect(bank, true) == ITEM_EFFECT_HEATROCK)
                    battle_effects_duration.weather_dur = 8;
                else
                    battle_effects_duration.weather_dur = 5;

                script_ptr = (void*) 0x082DB52A;
                execute_battle_script(script_ptr);
                battle_scripting.active_bank = bank;
                dontlock=1;
            }
        break;
        case ABILITY_DRIZZLE:
            if (!(battle_weather.flags.rain || battle_weather.flags.permament_rain || battle_weather.flags.downpour || battle_weather.flags.harsh_sun || battle_weather.flags.heavy_rain || battle_weather.flags.air_current))
            {
                effect = true;
                battle_weather.int_bw = weather_rain;
                if (get_item_effect(bank, true) == ITEM_EFFECT_DAMPROCK)
                    battle_effects_duration.weather_dur = 8;
                else
                    battle_effects_duration.weather_dur = 5;

                script_ptr = (void*) 0x082DB430;
                execute_battle_script(script_ptr);
                battle_scripting.active_bank = bank;
                dontlock=1;
            }
            break;
        case ABILITY_SAND_STREAM:
            if (!(battle_weather.flags.sandstorm || battle_weather.flags.permament_sandstorm || battle_weather.flags.harsh_sun || battle_weather.flags.heavy_rain || battle_weather.flags.air_current))
            {
                effect = true;
                battle_weather.int_bw = weather_sandstorm;
                if (get_item_effect(bank, true) == ITEM_EFFECT_SMOOTHROCK)
                    battle_effects_duration.weather_dur = 8;
                else
                    battle_effects_duration.weather_dur = 5;

                script_ptr = (void*) 0x082DB470;
                execute_battle_script(script_ptr);
                battle_scripting.active_bank = bank;
                dontlock=1;
            }
            break;
        case ABILITY_SNOW_WARNING:
            if (!(battle_weather.flags.hail || battle_weather.flags.permament_hail || battle_weather.flags.harsh_sun || battle_weather.flags.heavy_rain || battle_weather.flags.air_current))
            {
                effect = true;
                battle_weather.int_bw = weather_hail;
                if (get_item_effect(bank, true) == ITEM_EFFECT_ICYROCK)
                    battle_effects_duration.weather_dur = 8;
                else
                    battle_effects_duration.weather_dur = 5;

                execute_battle_script(&snow_warning_bs);
                battle_scripting.active_bank = bank;
                dontlock=1;
            }
            break;
        case ABILITY_DESOLATE_LAND:
            if (!(battle_weather.flags.harsh_sun))
            {
                effect = true;
                battle_weather.int_bw = weather_harsh_sun | weather_permament_sun;
                execute_battle_script(&desolateland_bs);
                dontlock=1;
            }
            break;
        case ABILITY_PRIMORDIA_SEA:
            if (!(battle_weather.flags.heavy_rain))
            {
                effect = true;
                battle_weather.int_bw = weather_heavy_rain | weather_permament_rain;
                execute_battle_script(&primoridalsea_bs);
                battle_scripting.active_bank = bank;
                dontlock=1;
            }
            break;
        case ABILITY_DELTA_STREAM:
            if (!(battle_weather.flags.air_current))
            {
                effect = true;
                battle_weather.int_bw = weather_air_current;
                execute_battle_script(&deltastream_bs);
                dontlock=1;
            }
            break;
        case ABILITY_FRISK:
            {
                effect = true;
                battle_stuff_ptr.ptr->intimidate_user=bank;
                execute_battle_script(frisk_bs);
            }
            break;
        case ABILITY_FOREWARN:
            {
                u16 best_move = get_forewarn_move(bank);
                if (best_move)
                {
                    effect = true;
                    battle_scripting.active_bank = bank;
                    execute_battle_script(&forewarn_bs);
                    battle_text_buff1[0] = 0xFD;
                    battle_text_buff1[1] = 0x2;
                    battle_text_buff1[2] = best_move;
                    battle_text_buff1[3] = (best_move >> 8);
                    battle_text_buff1[4] = 0xFF;
                }
            }
            break;
        case ABILITY_ANTICIPATION:
            {
                if (anticipation_shudder(bank))
                {
                    effect = true;
                    battle_scripting.active_bank = bank;
                    execute_battle_script(&anticipation_bs);
                }
            }
            break;
        case ABILITY_IMPOSTER:
            if (battle_participants[(bank ^ 1)].current_hp && (bank ^ 1) < no_of_all_banks && !(battle_participants[bank].status2.transformed) && !(battle_participants[(bank ^1)].status2.transformed))
            {
                effect = true;
                execute_battle_script(&transform_bs);
                current_move = MOVE_TRANSFORM;
                bank_attacker = bank;
                bank_target = (bank ^ 1);
                battle_scripting.active_bank = bank;
                new_battlestruct.ptr->various.active_bank = bank;
                dontlock=1;
            }
            break;
        case ABILITY_INTIMIDATE:
            {
                script_ptr = (void*) 0x082DB4B8;
                execute_battle_script(script_ptr);
                battle_stuff_ptr.ptr->intimidate_user=bank;
                bank_attacker = bank;
                effect=true;
            }
            break;
        case ABILITY_TRACE:
            {
                active_bank=bank^1;
                u16 hp1=battle_participants[active_bank].current_hp;
                if (battle_flags.double_battle)
                {
                    u16 hp2=battle_participants[active_bank^2].current_hp;
                    if(hp1==0 && hp2==0)
                        break;
                    if(hp1==0 || (hp2!=0 && two_options_rand(0,1)))
                        active_bank=active_bank^2;
                }
                else if (hp1==0)
                        break;
                last_used_ability=battle_participants[active_bank].ability_id;
                if (!last_used_ability)
                    break;
                if (last_used_ability!=ABILITY_TRACE) //If trace copies another switch-in ability. This fragment enables to activate that ability immediately after tracing.
                {
                    battle_stuff_ptr.ptr->switch_in_ability_bank_counter--;
                    dontlock=1;
                }
                script_ptr = (void*) 0x082DB452;
                execute_battle_script(script_ptr);
                battle_participants[bank].ability_id=last_used_ability;
                battle_scripting.active_bank = bank;
                battle_stuff_ptr.ptr->intimidate_user=bank;
                battle_text_buff1[0]=0xFD;
                battle_text_buff1[1]=0x4;
                battle_text_buff1[2]=active_bank;
                battle_text_buff1[3]=battle_team_id_by_side[active_bank];
                battle_text_buff1[4]=0xFF;
                battle_text_buff2[0]=0xFD;
                battle_text_buff2[1]=0x9;
                battle_text_buff2[2]=last_used_ability;
                battle_text_buff2[3]=0xFF;
                effect=true;
            }
            break;
        case ABILITY_AIR_LOCK:
        case ABILITY_CLOUD_NINE:
            dontlock=1;
            for (u8 j = 0; j < no_of_all_banks; j++)
            {
                effect = castform_switch(j);
                effect = prepare_castform_switch(effect, j);
                if (effect)
                {
                    break;
                }
                effect = should_cherrim_change(j);
                if (effect)
                {
                    execute_battle_script(&cherrimswitch_bs);
                    battle_scripting.active_bank = j;
                    battle_stuff_ptr.ptr->castform_switch_form = effect - 1;
                    break;
                }
            }
            break;
        case ABILITY_FLOWER_GIFT:
            if (!(new_battlestruct.ptr->bank_affecting[bank].cherrim_transformed))
            {
                effect = should_cherrim_change(bank);
                if (effect)
                {
                    execute_battle_script(&cherrimswitch_bs);
                    battle_scripting.active_bank = bank;
                    battle_stuff_ptr.ptr->castform_switch_form = effect - 1;
                    new_battlestruct.ptr->bank_affecting[bank].cherrim_transformed = 1;
                    dontlock=1;
                }
            }
            break;
        case ABILITY_FORECAST:
            effect = castform_switch(bank);
            prepare_castform_switch(effect, bank);
            dontlock=1;
            break;
        case ABILITY_SLOW_START:
            {
                new_battlestruct.ptr->bank_affecting[bank].slowstart_duration = 5;
                effect = true;
                battle_scripting.active_bank = bank;
                execute_battle_script(&slowstart_bs);
                battle_scripting.active_bank = bank;
            }
            break;
        }
        if (common_effect)
        {
            effect = true;
            battle_scripting.active_bank = bank;
            execute_battle_script(script_ptr);
        }
        if(effect && !dontlock)
            status3[bank].switchinlock=1;
        break;
    case 1: //end turn abilities
        if (!(battle_participants[bank].current_hp && has_ability_effect(bank, 0, 1)))
            break;
        bank_attacker = bank;
        switch (last_used_ability)
        {
            case ABILITY_RAIN_DISH:
                if (weather_abilities_effect() && (battle_weather.flags.downpour || battle_weather.flags.rain || battle_weather.flags.permament_rain || battle_weather.flags.heavy_rain))
                {
                    if (battle_participants[bank].current_hp < battle_participants[bank].max_hp && !new_battlestruct.ptr->bank_affecting[bank].heal_block)
                    {
                        script_ptr = (void*) 0x082DB45C;
                        execute_battle_script(script_ptr);
                        damage_loc = get_1_16_of_max_hp(bank) * (-1);
                        effect = true;
                    }
                }
                break;
            case ABILITY_DRY_SKIN:
                if (weather_abilities_effect())
                {
                    if (battle_weather.flags.downpour || battle_weather.flags.rain || battle_weather.flags.permament_rain || battle_weather.flags.heavy_rain)
                    {
                        if (battle_participants[bank].current_hp < battle_participants[bank].max_hp && !new_battlestruct.ptr->bank_affecting[bank].heal_block)
                        {
                            script_ptr = (void*) 0x082DB45C;
                            execute_battle_script(script_ptr);
                            damage_loc = get_1_8_of_max_hp(bank) * (-1);
                            effect = true;
                        }
                    }
                    else if (battle_weather.flags.sun || battle_weather.flags.permament_sun || battle_weather.flags.harsh_sun)
                    {
                        damage_loc = get_1_8_of_max_hp(bank);
                        execute_battle_script(&dryskindamage_bs);
                        effect = true;
                    }
                }
                break;
            case ABILITY_SOLAR_POWER:
                if (weather_abilities_effect() && (battle_weather.flags.sun || battle_weather.flags.permament_sun || battle_weather.flags.harsh_sun))
                {
                    damage_loc = get_1_8_of_max_hp(bank);
                    execute_battle_script(&solarpower_bs);
                    effect = true;
                }
                break;
            case ABILITY_HARVEST:
                if (get_item_pocket_id(battle_stuff_ptr.ptr->used_held_items[bank]) == 4 && !battle_participants[bank].held_item)
                {
                    if ((weather_abilities_effect() && (battle_weather.flags.sun || battle_weather.flags.permament_sun || battle_weather.flags.harsh_sun)) || two_options_rand(0, 1) == 0)
                    {
                        effect = true;
                        execute_battle_script(&harvest_bs);
                    }
                }
                break;
            case ABILITY_HEALER:
                {
                    u8 friendly_bank = bank ^ 2;
                    if (friendly_bank < no_of_all_banks && battle_participants[friendly_bank].status.int_status & battle_participants[friendly_bank].current_hp)
                    {
                        if (__umodsi3(rng(), 100) + 1 <= 30)
                        {   battle_participants[friendly_bank].status2.nightmare = 0;
                            effect = true;
                            copy_status_condition_text(friendly_bank,0);
                            execute_battle_script(&healer_bs);
                            battle_scripting.active_bank = friendly_bank;
                            active_bank = friendly_bank;
                            battle_participants[friendly_bank].status.int_status = 0;
                            prepare_setattributes_in_battle(0, 0x28, 0, 4, &battle_participants[friendly_bank].status);
                            mark_buffer_bank_for_execution(friendly_bank);
                        }
                    }
                    break;
                }
            case ABILITY_HYDRATION:
                    if (((battle_weather.flags.downpour || battle_weather.flags.rain || battle_weather.flags.permament_rain || battle_weather.flags.heavy_rain) && weather_abilities_effect()))
                        common_effect=1;
                    break;
            case ABILITY_SHED_SKIN:
                    if (__umodsi3(rng(), 3) == 0)
                        common_effect=1;
                    break;
            case ABILITY_PICKUP:
                if (!battle_participants[bank].held_item)
                {
                    for (u8 i = 0; i < no_of_all_banks; i++)
                    {
                        if (i!=bank && battle_stuff_ptr.ptr->used_held_items[i]
                            && new_battlestruct.ptr->various.recently_used_item == battle_stuff_ptr.ptr->used_held_items[i])
                        {
                            effect = true;
                            battle_stuff_ptr.ptr->used_held_items[i] = 0;
                            execute_battle_script(&pickup_bs);
                            battle_participants[bank].held_item = new_battlestruct.ptr->various.recently_used_item;
                            new_battlestruct.ptr->various.recently_used_item = 0;
                            active_bank = bank;
                            prepare_setattributes_in_battle(0, 2, 0, 2,&battle_participants[bank].held_item);
                            mark_buffer_bank_for_execution(bank);
                            break;
                        }
                    }
                }
                break;
            case ABILITY_BAD_DREAMS:
                effect = true;
                battle_stuff_ptr.ptr->intimidate_user=bank;
                execute_battle_script(bad_dreams_bs);
                break;
            case ABILITY_SPEED_BOOST:
                if (battle_participants[bank].spd_buff != 0xC && disable_structs[bank].is_first_turn != 2)
                {
                    effect = 1;
                    battle_participants[bank].spd_buff++;
                    battle_scripting.active_bank = bank;
                    battle_scripting.field10 = 0x11;
                    battle_scripting.field11 = 0;
                    script_ptr = (void*) 0x082DB444;
                    execute_battle_script(script_ptr);
                }
                break;
            case ABILITY_TRUANT:
                disable_structs[bank].truant_counter ^= 1;
                break;
            case ABILITY_MOODY:
                {
                    u8 raiseable_stats = 0;
                    u8 lowerable_stats = 0;
                    u8* stat_ptr = &battle_participants[bank].atk_buff;
                    for (u8 i = 0; i < 7; i ++)
                    {
                        if (*(stat_ptr + i) != 0xC)
                        {
                            raiseable_stats |= bits_table[i];
                        }
                        if (*(stat_ptr + i) != 0)
                        {
                            lowerable_stats |= bits_table[i];
                        }
                    }
                    if (raiseable_stats || lowerable_stats)
                    {
                        effect = true;
                        execute_battle_script(&moody_bs);
                        new_battlestruct.ptr->various.var1 = 0;
                        new_battlestruct.ptr->various.var2 = 0;
                        battle_scripting.active_bank = bank;
                        bank_target = bank;
                        while (raiseable_stats != 0)
                        {
                            u8 rand_stat = __umodsi3(rng(), 7);
                            if (raiseable_stats & bits_table[rand_stat])
                            {
                                new_battlestruct.ptr->various.var1 = 0x21 + rand_stat;
                                if (lowerable_stats & bits_table[rand_stat])
                                    lowerable_stats ^= bits_table[rand_stat];
                                break;
                            }
                        }
                        while (lowerable_stats != 0)
                        {
                            u8 rand_stat = __umodsi3(rng(), 7);
                            if (lowerable_stats & bits_table[rand_stat])
                            {
                                new_battlestruct.ptr->various.var2 = 0x91 + rand_stat;
                                break;
                            }
                        }
                    }
                    break;
                }
            }
       if (common_effect && battle_participants[bank].status.int_status)
        {
            battle_participants[bank].status2.nightmare = 0;
            effect = true;
            copy_status_condition_text(bank,0);
            script_ptr = (void*) 0x082DB484;
            execute_battle_script(script_ptr);
            active_bank = bank;
            battle_scripting.active_bank = bank;
            battle_participants[bank].status.int_status = 0;
            prepare_setattributes_in_battle(0, 0x28, 0, 4, &battle_participants[bank].status);
            mark_buffer_bank_for_execution(bank);
        }
        break;
    case 2: //abilities that affect certain moves
        if (!has_ability_effect(bank, 1, 1))
            break;
        u16* table_ptr;
        switch (last_used_ability)
        {
        case ABILITY_SOUNDPROOF:
            table_ptr = &sound_moves[0];
            common_effect=1;
            break;
        case ABILITY_BULLETPROOF:
            table_ptr = &ball_bomb_moves[0];
            common_effect=1;
            break;
        case ABILITY_OVERCOAT:
            table_ptr = &powder_moves[0];
            common_effect=1;
            break;
        }
        if(common_effect)
        {
            for (u8 i = 0; table_ptr[i]!= 0xFFFF; i++)
            {
                if (current_move == table_ptr[i])
                {
                    effect = true;
                    battlescripts_curr_instruction = (void*) 0x082DB61F;
                    if (battle_participants[bank_attacker].status2.multiple_turn_move)
                    {
                        hitmarker |= 0x800;
                    }
                    break;
                }
            }
        }
        break;

    case 3: //abilities that affect type
            if (curr_move && has_ability_effect(bank, 1, 1))
            {
                if (protect_structs[bank_attacker].flag2_notfirststrike)
                    adder = 1;
                u8 stat=0;

                switch (last_used_ability)
                {
                case ABILITY_WATER_ABSORB:
                case ABILITY_DRY_SKIN:
                    if (move_type == TYPE_WATER)
                        common_effect=1;
                    break;
                case ABILITY_VOLT_ABSORB:
                    if (move_type == TYPE_ELECTRIC)
                        common_effect=1;
                    break;
                case ABILITY_FLASH_FIRE:
                    if (move_type == TYPE_FIRE)
                    {
                        effect = 1;
                        battle_resources_ptr.ptr->field0_ptr->ability_flags_ptr->flags_ability[bank].flag1_flashfire = 1;
                        script_ptr = (void*) 0x082DB5A7;
                        script_ptr += adder;
                        battlescripts_curr_instruction = script_ptr;
                    }
                    break;
                case ABILITY_LIGHTNING_ROD:
                    if (move_type == TYPE_ELECTRIC && (!is_of_type(bank,TYPE_GROUND) || get_item_effect(bank,1)==ITEM_EFFECT_RINGTARGET))
                    {

                        common_effect=2;
                        stat=4;
                    }
                    break;
                case ABILITY_STORM_DRAIN:
                    if (move_type == TYPE_WATER)
                    {
                        common_effect=2;
                        stat=4;
                    }
                    break;
                case ABILITY_SAP_SIPPER:
                    if (move_type == TYPE_GRASS)
                    {
                        common_effect=2;
                        stat=1;
                    }
                    break;
                case ABILITY_MOTOR_DRIVE:
                    if (move_type == TYPE_ELECTRIC)
                    {
                        common_effect=2;
                        stat=3;
                    }
                    break;
                }
                if(common_effect==1)
                {
                    if (battle_participants[bank].max_hp == battle_participants[bank].current_hp || new_battlestruct.ptr->bank_affecting[bank].heal_block)
                        script_ptr = (void*) 0x082DB591;
                    else
                    {
                        script_ptr = (void*) 0x082DB56E;
                        damage_loc = get_1_4_of_max_hp(bank) * (-1);
                    }
                    script_ptr += adder;
                    battlescripts_curr_instruction = script_ptr;
                    effect = true;
                }
                else if(common_effect==2)
                {
                    u8* stat_ptr = &battle_participants[bank].hp_buff;
                    stat_ptr += stat;

                    if((*stat_ptr)!=0xC)
                    {
                        battle_scripting.stat_changer=0x20+stat;
                        *stat_ptr +=1 ;
                        script_ptr = &absorb_ability_boost_bs;
                    }
                    else
                        script_ptr = &absorb_ability_immune_bs;
                    script_ptr += adder;
                    effect = true;
                    battlescripts_curr_instruction = script_ptr;
                }
            }

            break;
    case 4: //move end turn abilities
        {
            if (has_ability_effect(bank, 0, 1) && !(move_outcome.not_affected || move_outcome.failed || move_outcome.missed)
                && battle_participants[bank_attacker].current_hp && (special_statuses[bank_target].moveturn_losthp))
            {
                switch (last_used_ability)
                {
                case ABILITY_STATIC:
                    move_effect = 0x45;
                    common_effect=1;
                    break;
                case ABILITY_FLAME_BODY:
                    move_effect = 0x43;
                    common_effect=1;
                    break;
                case ABILITY_POISON_POINT:
                    move_effect = 0x42;
                    common_effect=1;
                    break;
                case ABILITY_EFFECT_SPORE:
                    if(!(has_ability_effect(bank_attacker, 0, 1) &&
                         (battle_participants[bank_attacker].ability_id==ABILITY_OVERCOAT))) //add safety goggles check
                    {
                        move_effect=(rng()&3);
                        if(move_effect==0)
                            move_effect=0x41;
                        else if(move_effect==1)
                            move_effect=0x42;
                        else
                            move_effect=0x45;
                        common_effect=1;
                    }
                    break;
                case ABILITY_AFTERMATH:
                    if (battle_participants[bank].current_hp == 0 && contact && !(ability_battle_effects(0x13, 0, ABILITY_DAMP, 0, 0)) && !(has_ability_effect(bank_attacker, 0, 1) && battle_participants[bank_attacker].ability_id == ABILITY_MAGIC_GUARD))
                    {
                        damage_loc = get_1_4_of_max_hp(bank_attacker);
                        effect = true;
                        battlescript_push();
                        battlescripts_curr_instruction = &aftermath_bs;
                    }
                    break;
                case ABILITY_PICKPOCKET:
                    if (!contact || battle_participants[bank].held_item
                        || (new_battlestruct.ptr->bank_affecting[bank_attacker].sheerforce_bonus)
                        || (multihit_counter != 1 && multihit_counter != 0)
                        || (battle_participants[bank_attacker].ability_id == ABILITY_STICKY_HOLD
                            && has_ability_effect(bank_attacker,0,1)))
                        break;
                    u16 *user_held_item=&battle_participants[bank_attacker].held_item;
                    if((*user_held_item)!=0 && (*user_held_item)!=0xAF)
                    {
                        last_used_item = *user_held_item;
                        battle_stuff_ptr.ptr->choiced_move[bank_attacker]=0;
                        battle_participants[bank].held_item = last_used_item;
                        *user_held_item = 0;
                        battlescript_push();
                        battlescripts_curr_instruction = &pickpocket_bs;
                        effect = true;
                        active_bank = bank;
                        prepare_setattributes_in_battle(0, 2, 0, 2, &battle_participants[active_bank].held_item);
                        mark_buffer_bank_for_execution(active_bank);
                        active_bank = bank_attacker;
                        prepare_setattributes_in_battle(0, 2, 0, 2, &battle_participants[active_bank].held_item);
                        mark_buffer_bank_for_execution(active_bank);
                    }
                    break;
                case ABILITY_MUMMY:
                    if (contact && battle_participants[bank_attacker].ability_id != ABILITY_MUMMY && battle_participants[bank_attacker].ability_id != ABILITY_STANCE_CHANGE && battle_participants[bank_attacker].ability_id != ABILITY_MULTITYPE)
                    {
                        effect = true;
                        battle_participants[bank_attacker].ability_id = battle_participants[bank].ability_id;
                        battlescript_push();
                        battlescripts_curr_instruction = &mummy_bs;
                    }
                    break;
                case ABILITY_GOOEY:
                    if (contact && battle_participants[bank_attacker].spd_buff > 0)
                    {
                        effect = true;
                        battle_scripting.stat_changer = 0x93;
                        battlescript_push();
                        battlescripts_curr_instruction = &gooey_bs;
                    }
                    break;
                case ABILITY_CURSED_BODY:
                    if (!disable_structs[bank_attacker].disabled_move && percent_chance(30) && !ability_battle_effects(12, bank_attacker, ABILITY_AROMA_VEIL, 1, 0))
                    {
                        disable_structs[bank_attacker].disabled_move = curr_move;
                        disable_structs[bank_attacker].disable_timer = 3;
                        battlescript_push();
                        battlescripts_curr_instruction = &cursedbody_bs;
                        effect = true;
                        battle_text_buff1[0] = 0xFD;
                        battle_text_buff1[1] = 0x2;
                        battle_text_buff1[2] = curr_move;
                        battle_text_buff1[3] = (curr_move >> 8);
                        battle_text_buff1[4] = 0xFF;
                    }
                    break;
                case ABILITY_ROUGH_SKIN:
                case ABILITY_IRON_BARBS:
                    if (contact && !(has_ability_effect(bank_attacker, 0, 1) && battle_participants[bank_attacker].ability_id == ABILITY_MAGIC_GUARD))
                    {
                        damage_loc = get_1_8_of_max_hp(bank_attacker);
                        effect = true;
                        battlescript_push();
                        void* roughskin_bs = (void*) 0x082DB654;
                        battlescripts_curr_instruction = roughskin_bs;
                    }
                    break;
                case ABILITY_RATTLED:
                    if (battle_participants[bank].spd_buff != 0xC && (move_type == TYPE_BUG || move_type == TYPE_DARK || move_type == TYPE_GHOST))
                    {
                        effect = true;
                        battlescript_push();
                        battlescripts_curr_instruction = &rattled_bs;
                        battle_scripting.stat_changer = 0x13;
                    }
                    break;
                case ABILITY_JUSTIFIED:
                    if (battle_participants[bank].atk_buff != 0xC && move_type == TYPE_DARK)
                    {
                        effect = true;
                        battlescript_push();
                        battlescripts_curr_instruction = &rattled_bs;
                        battle_scripting.stat_changer = 0x11;
                    }
                    break;
                case ABILITY_STEADFAST:
                    if (battle_participants[bank].spd_buff != 0xC && battle_participants[bank].status2.flinched)
                    {
                        effect = true;
                        battlescript_push();
                        battlescripts_curr_instruction = &rattled_bs;
                        battle_scripting.stat_changer = 0x13;
                    }
                    break;
                case ABILITY_ANGER_POINT:
                    if (crit_loc==2 && battle_participants[bank].atk_buff != 0xC)
                    {
                        effect = true;
                        battlescript_push();
                        battlescripts_curr_instruction = &angerpoint_bs;
                        battle_participants[bank].atk_buff=0xC;
                    }
                    break;
                case ABILITY_COLOR_CHANGE:
                    if (curr_move != MOVE_STRUGGLE && !new_battlestruct.ptr->bank_affecting[bank_attacker].sheerforce_bonus && !is_of_type(bank, move_type) && (multihit_counter == 1 || multihit_counter == 0))
                    {
                        battle_text_buff1[0] = 0xFD;
                        battle_text_buff1[1] = 0x3;
                        battle_text_buff1[2] = move_type;
                        battle_text_buff1[3] = 0xFF;
                        effect = true;
                        battlescript_push();
                        void* colorchange_bs = (void*) 0x082DB64D;
                        battlescripts_curr_instruction = colorchange_bs;
                        set_type(bank, move_type);
                    }
                    break;
                case ABILITY_WEAK_ARMOR:
                    if (battle_participants[bank].def_buff > 0 || battle_participants[bank].spd_buff != 0xC)
                    {
                        effect = true;
                        battlescript_push();
                        battlescripts_curr_instruction = &weakarmor_bs;
                        new_battlestruct.ptr->various.active_bank = bank;
                    }
                    break;
                case ABILITY_CUTE_CHARM:
                    if (battle_participants[bank_attacker].ability_id != ABILITY_OBLIVIOUS && percent_chance(30) && contact && !ability_battle_effects(12, bank_attacker, ABILITY_AROMA_VEIL, 1, 0))
                    {
                        u8 target_gender = gender_from_pid(battle_participants[bank].poke_species, battle_participants[bank].pid);
                        u8 attacker_gender = gender_from_pid(battle_participants[bank_attacker].poke_species, battle_participants[bank_attacker].pid);
                        if (target_gender != attacker_gender && target_gender != 0xFF && attacker_gender != 0xFF && ((battle_participants[bank_attacker].status2.in_love & bits_table[bank_target]) == 0))
                        {
                            effect = true;
                            battle_participants[bank_attacker].status2.in_love |= bits_table[bank];
                            battlescript_push();
                            battlescripts_curr_instruction = (void*) 0x082DB66F;
                        }
                    }
                    break;
                }
                if (common_effect && contact && percent_chance(30))
                {
                    battle_communication_struct.move_effect = move_effect;
                    battlescript_push();
                    script_ptr = (void*) 0x082DB67C;
                    battlescripts_curr_instruction = script_ptr;
                    hitmarker |= HITMAKRER_IGNORE_SAFEGUARD;
                    effect = true;
                }
                break;
            }

        }
    case 5: //status immunities
        for (u8 i = 0; i < no_of_all_banks; i++)
        {
            if (has_ability_effect(i, 0, 1))
            {
                switch (battle_participants[i].ability_id)
                {
                case ABILITY_LIMBER:
                    if (battle_participants[i].status.flags.paralysis)
                        common_effect=1;
                    break;
                case ABILITY_MAGMA_ARMOR:
                    if (battle_participants[i].status.flags.freeze)
                        common_effect=1;
                    break;
                case ABILITY_IMMUNITY:
                    if (battle_participants[i].status.flags.poison || battle_participants[i].status.flags.toxic_poison)
                        common_effect=1;
                    break;
                case ABILITY_WATER_VEIL:
                    if (battle_participants[i].status.flags.burn)
                        common_effect=1;
                    break;
                case ABILITY_INSOMNIA:
                case ABILITY_VITAL_SPIRIT:
                    if (battle_participants[i].status.flags.sleep)
                    {
                        common_effect=1;
                        battle_participants[i].status2.nightmare = 0;
                    }
                    break;
                case ABILITY_OBLIVIOUS:
                    if (battle_participants[i].status2.in_love)
                    {
                        battle_participants[i].status2.in_love = 0;
                        strcpy_xFF_terminated_0(&battle_text_buff1, (void*) 0x0831BCA0);
                        effect = true;
                    }
                    break;
                case ABILITY_OWN_TEMPO:
                    if (battle_participants[i].status2.confusion)
                    {
                        battle_participants[i].status2.confusion = 0;
                        strcpy_xFF_terminated_0(&battle_text_buff1, (void*) 0x0831BC98);
                        effect = true;
                    }
                    break;
                }
                if (common_effect==1)
                {
                    copy_status_condition_text(i,0);
                    battle_participants[i].status.int_status = 0;
                    effect=true;
                    battlescript_push();
                    battlescripts_curr_instruction = (void*) 0x082DB68C;
                    active_bank = i;
                    battle_scripting.active_bank = i;
                    prepare_setattributes_in_battle(0, 0x28, 0, 4, &battle_participants[i].status);
                    mark_buffer_bank_for_execution(active_bank);
                    break;
                }
                if (effect)
                    break;
            }
        }
        break;
    case 6: //check castform and cherrim
        {
            break;
        }
    case 7: //user's synchronize
        adder=0x40;
    case 8: //target's synchronize after static etc.
        if(battle_participants[bank].ability_id==ABILITY_SYNCHRONIZE && has_ability_effect(bank,0,1))
        {
            effect=true;
            hitmarker &= 0xFFFFBFFF;
            battle_stuff_ptr.ptr->synchronize_effect_chooser &= 0x3F;
            battle_communication_struct.move_effect=adder+battle_stuff_ptr.ptr->synchronize_effect_chooser;
            battle_scripting.active_bank=bank;
            battlescript_push();
            battlescripts_curr_instruction=(void *)(0x82DB67F);
            hitmarker |= 0x2000;
        }
        break;
    case 10:
        if (has_ability_effect(bank, 0, 1) && !(move_outcome.not_affected || move_outcome.failed || move_outcome.missed)
                && battle_participants[bank_attacker].current_hp && (special_statuses[bank_target].moveturn_losthp))
        {
            switch(last_used_ability)
            {
                case ABILITY_MAGICIAN:
                    if (battle_participants[bank].held_item || current_move==MOVE_DOOM_DESIRE || current_move==MOVE_FUTURE_SIGHT
                        || (battle_participants[bank_target].ability_id == ABILITY_STICKY_HOLD && has_ability_effect(bank_target,0,1)))
                        break;
                    u16 *target_held_item=&battle_participants[bank_target].held_item;
                    if((*target_held_item)!=0 && (*target_held_item)!=0xAF)
                    {
                        last_used_item = *target_held_item;
                        battle_stuff_ptr.ptr->choiced_move[bank_target]=0;
                        battle_participants[bank].held_item = last_used_item;
                        *target_held_item = 0;
                        battlescript_push();
                        battlescripts_curr_instruction = (void *)(0x82DB422); // to add flavour text
                        effect = true;
                        active_bank = bank;
                        prepare_setattributes_in_battle(0, 2, 0, 2, &battle_participants[active_bank].held_item);
                        mark_buffer_bank_for_execution(active_bank);
                        active_bank = bank_target;
                        prepare_setattributes_in_battle(0, 2, 0, 2, &battle_participants[active_bank].held_item);
                        mark_buffer_bank_for_execution(active_bank);
                    }
                    break;
                case ABILITY_POISON_TOUCH:
                    if(contact && percent_chance(30))
                    {
                        hitmarker|=HITMAKRER_IGNORE_SAFEGUARD;
                        battle_communication_struct.move_effect=2;
                        battlescript_push();
                        battlescripts_curr_instruction = (void *) 0x082DB67F;
                        effect=true;
                    }
                    break;
                case ABILITY_STENCH:
                    if(contact && percent_chance(10) && get_item_effect(bank,1)!=ITEM_EFFECT_KINGSROCK) //not working
                    {
                        hitmarker|=HITMAKRER_IGNORE_SAFEGUARD;
                        battle_communication_struct.move_effect=8;
                        battlescript_push();
                        battlescripts_curr_instruction = (void *) 0x082DB67F;
                        effect=true;
                    }
                    break;
            }
        }
        break;
    case 12: //check opposing field for ability
    {
        for (u8 i = 0; i < no_of_all_banks; i++)
        {
            if (is_bank_from_opponent_side(i) != bank_side && battle_participants[i].ability_id == ability_to_check && has_ability_effect(i, special_cases_argument, 1))
            {
                last_used_ability = ability_to_check;
                effect = i + 1;
                break;
            }
        }
        break;
    }
    case 13: //check bank field for ability
    {
        for (u8 i = 0; i < no_of_all_banks; i++)
        {
            if (is_bank_from_opponent_side(i) == bank_side && battle_participants[i].ability_id == ability_to_check && has_ability_effect(i, special_cases_argument, 1))
            {
                last_used_ability = ability_to_check;
                effect = i + 1;
                break;
            }
        }
        break;
    }
    case 14: //check field affecting; mud, water sport and whole field for ability
        {
            for (u8 i = 0; i < no_of_all_banks; i++)
            {
                if ((status3[i].mud_sport && special_cases_argument == 0xFD) || (status3[i].watersport && special_cases_argument == 0xFE))
                {
                    effect = i + 1;
                    break;
                }
                else if (battle_participants[i].ability_id == ability_to_check && special_cases_argument == 0xFF && has_ability_effect(i, special_cases_argument, 1))
                {
                    effect = i + 1;
                    last_used_ability = ability_to_check;
                    break;
                }
            }
            break;
        }
    case 15: //check field except the bank
        {
            for (u8 i = 0; i < no_of_all_banks; i++)
            {
                if (battle_participants[i].ability_id == ability_to_check && i != bank && has_ability_effect(i, special_cases_argument, 1))
                {
                    effect = i + 1;
                    last_used_ability = ability_to_check;
                    break;
                }
            }
            break;
        }
    case 16: //count instances of ability in the opponent field
        {
            for (u8 i = 0; i < no_of_all_banks; i++)
            {
                if (is_bank_from_opponent_side(i) != bank_side && battle_participants[i].ability_id == ability_to_check && has_ability_effect(i, special_cases_argument, 1))
                {
                    effect++;
                    last_used_ability = ability_to_check;
                }
            }
            break;
        }
    case 17: //count instances of ability in the banks field
        {
            for (u8 i = 0; i < no_of_all_banks; i++)
            {
                if (is_bank_from_opponent_side(i) == bank_side && battle_participants[i].ability_id == ability_to_check && has_ability_effect(i, special_cases_argument, 1))
                {
                    effect++;
                    last_used_ability = ability_to_check;
                }
            }
            break;
        }
    case 18: //count instances of ability except bank
        {
            for (u8 i = 0; i < no_of_all_banks; i++)
            {
                if ( i != bank_side && battle_participants[i].ability_id == ability_to_check && has_ability_effect(i, special_cases_argument, 1))
                {
                    effect++;
                    last_used_ability = ability_to_check;
                }
            }
            break;
        }
    case 19: //check whole field for ability with a hp check
        {
            for (u8 i = 0; i < no_of_all_banks; i ++)
            {
                if (battle_participants[i].ability_id == ability_to_check && battle_participants[i].current_hp && has_ability_effect(i, special_cases_argument, 1))
                {
                    effect = i + 1;
                    last_used_ability = ability_to_check;
                    break;
                }
            }
            break;
        }
    case 20: //check ally
        {
            u8 ally = bank ^ 2;
            for (u8 i = 0; i < no_of_all_banks; i ++)
            {
                if (battle_participants[i].ability_id == ability_to_check && i == ally && has_ability_effect(i, special_cases_argument, 1))
                {
                    effect = i + 1;
                    last_used_ability = ability_to_check;
                    break;
                }
            }
            break;
        }
    }

    if (effect && last_used_ability != 0xFF && switch_id < 0xB)
        record_usage_of_ability(bank, last_used_ability);
    return effect;
}

void call_ability_effects()
{
    ability_battle_effects(0, new_battlestruct.ptr->various.active_bank, 0, 0, 0);
    return;
}

u8 white_herb_effect(u8 bank)
{
    u8 effect = false;
    for (u8 i = 0; i < 7; i++)
    {
        if (*(&battle_participants[bank].atk_buff + i) < 6)
        {
            effect = 5;
            battle_scripting.active_bank = bank;
            active_bank = bank;
            bank_attacker = bank;
            call_bc_move_exec((void*) 0x082DB7AE);
        }
    }
    return effect;
}

u8 get_all_item_quality(u8 bank)
{
    if (battle_participants[bank].held_item == ITEM_ENIGMABERRY)
        return enigma_berry_battle[bank].quality;
    else
        return get_item_quality(battle_participants[bank].held_item);
}

u8 mental_herb_effect(u8 bank)
{
    u8 effect = false;
    if (disable_structs[bank].disable_timer || disable_structs[bank].encore_timer || disable_structs[bank].taunt_timer || battle_participants[bank].status2.tormented || battle_participants[bank].status2.in_love || new_battlestruct.ptr->bank_affecting[bank].heal_block)
    {
        effect = 2;
        call_bc_move_exec(&mentalherb_bs);
        battle_scripting.active_bank = bank;
        active_bank = bank;
        bank_attacker = bank;
    }
    return effect;
}

u8 cant_poison(u8 bank, u8 self_inflicted)
{   //0 == can poison
    //1 == is already poisoned
    //2 == has other major condition
    //3 == type doesn't allow it
    //4 == ability doesn't allow it
    //5 == safeguard protection
    //8 == misty terrain doesn't allow it
    if (battle_participants[bank].status.flags.poison || battle_participants[bank].status.flags.toxic_poison)
        return 1;
    if (battle_participants[bank].status.int_status)
        return 2;
    if (is_of_type(bank, TYPE_POISON) || is_of_type(bank, TYPE_STEEL))
        return 3;
    if ((has_ability_effect(bank, 0, 1) && (battle_participants[bank].ability_id == ABILITY_IMMUNITY || (battle_participants[bank].ability_id == ABILITY_LEAF_GUARD && (battle_weather.flags.sun || battle_weather.flags.permament_sun || battle_weather.flags.harsh_sun)))))
        return 4;
    if (side_affecting_halfword[is_bank_from_opponent_side(bank)].safeguard_on && !self_inflicted)
        return 5;
    if (new_battlestruct.ptr->field_affecting.misty_terrain && get_airborne_state(bank, 1, 1) <= 2 && !self_inflicted)
        return 8;
    return 0;
}

u8 cant_fall_asleep(u8 bank, u8 self_inflicted)
{
    //0 == can fall asleep
    //1 == can't because it's already asleep
    //2 == can't because it has other major status condition
    //3 == can't because type doesn't allow it, currently doesn't exist
    //4 == can't because an ability protects it
    //5 == can't because it's protected by safeguard
    //6 == can't because there's an uproar
    //8 == misty terrain doesn't allow it
    //9 == electric terrain doesn't allow it
    if (battle_participants[bank].status.flags.sleep)
        return 1;
    if (battle_participants[bank].status.int_status)
        return 2;
    if ((has_ability_effect(bank, 0, 1) && (battle_participants[bank].ability_id == ABILITY_VITAL_SPIRIT || battle_participants[bank].ability_id == ABILITY_INSOMNIA || (battle_participants[bank].ability_id == ABILITY_LEAF_GUARD && (battle_weather.flags.sun || battle_weather.flags.permament_sun || battle_weather.flags.harsh_sun)))))
        return 4;
    if (side_affecting_halfword[is_bank_from_opponent_side(bank)].safeguard_on && !self_inflicted)
        return 5;
    if (!check_ability(bank, ABILITY_SOUNDPROOF))
    {
        for (u8 i = 0; i < no_of_all_banks; i++)
        {
            if (battle_participants[bank].status2.uproar)
                return 6;
        }
    }
    if (new_battlestruct.ptr->field_affecting.misty_terrain && get_airborne_state(bank, 1, 1) <= 2 && !self_inflicted)
        return 8;
    if (new_battlestruct.ptr->field_affecting.electic_terrain && get_airborne_state(bank, 1, 1) <= 2 && !self_inflicted)
        return 9;
    return 0;
}

u8 cant_become_paralyzed(u8 bank, u8 self_inflicted)
{   //0 == can paralyze
    //1 == is already paralyzed
    //2 == has other major condition
    //3 == type doesn't allow it
    //4 == ability doesn't allow it
    //5 == safeguard protection
    //8 == misty terrain protection
    if (battle_participants[bank].status.flags.paralysis)
        return 1;
    if (battle_participants[bank].status.int_status)
        return 2;
    if (is_of_type(bank, TYPE_ELECTRIC))
        return 3;
    if ((has_ability_effect(bank, 0, 1) && (battle_participants[bank].ability_id == ABILITY_LIMBER || (battle_participants[bank].ability_id == ABILITY_LEAF_GUARD && (battle_weather.flags.sun || battle_weather.flags.permament_sun || battle_weather.flags.harsh_sun)))))
        return 4;
    if (side_affecting_halfword[is_bank_from_opponent_side(bank)].safeguard_on && !self_inflicted)
        return 5;
    if (new_battlestruct.ptr->field_affecting.misty_terrain && get_airborne_state(bank, 1, 1) <= 2 && !self_inflicted)
        return 8;
    return 0;
}

u8 cant_become_burned(u8 bank, u8 self_inflicted)
{   //0 == can burn
    //1 == is already burned
    //2 == has other major condition
    //3 == type doesn't allow it
    //4 == ability doesn't allow it
    //5 == safeguard protection
    //8 == misty terrain protection
    if (battle_participants[bank].status.flags.burn)
        return 1;
    if (battle_participants[bank].status.int_status)
        return 2;
    if (is_of_type(bank, TYPE_FIRE))
        return 3;
    if ((has_ability_effect(bank, 0, 1) && (battle_participants[bank].ability_id == ABILITY_WATER_VEIL || (battle_participants[bank].ability_id == ABILITY_LEAF_GUARD && (battle_weather.flags.sun || battle_weather.flags.permament_sun || battle_weather.flags.harsh_sun)))))
        return 4;
    if (side_affecting_halfword[is_bank_from_opponent_side(bank)].safeguard_on && !self_inflicted)
        return 5;
    if (new_battlestruct.ptr->field_affecting.misty_terrain && get_airborne_state(bank, 1, 1) <= 2 && !self_inflicted)
        return 8;
    return 0;
}

u8 cant_become_freezed(u8 bank, u8 self_inflicted)
{   //0 == can freeze
    //1 == is already freezed
    //2 == has other major condition
    //3 == type doesn't allow it
    //4 == ability doesn't allow it
    //5 == safeguard protection
    //7 == weather doesn't allow it
    //8 == misty terrain doesn't allow it
    if (battle_participants[bank].status.flags.freeze)
        return 1;
    if (battle_participants[bank].status.int_status)
        return 2;
    if (is_of_type(bank, TYPE_ICE))
        return 3;
    if ((has_ability_effect(bank, 0, 1) && (battle_participants[bank].ability_id == ABILITY_MAGMA_ARMOR || (battle_participants[bank].ability_id == ABILITY_LEAF_GUARD && (battle_weather.flags.sun || battle_weather.flags.permament_sun || battle_weather.flags.harsh_sun)))))
        return 4;
    if (side_affecting_halfword[is_bank_from_opponent_side(bank)].safeguard_on && !self_inflicted)
        return 5;
    if (weather_abilities_effect() && battle_weather.int_bw & (weather_harsh_sun || weather_permament_sun || weather_sun))
        return 7;
    if (new_battlestruct.ptr->field_affecting.misty_terrain && get_airborne_state(bank, 1, 1) <= 2 && !self_inflicted)
        return 8;
    return 0;
}

u8 hp_condition(u8 bank, u8 percent) //1 = 50 %, 2 = 25 %
{
    struct battle_participant* ptr_to_struct = &battle_participants[bank];
    if (ptr_to_struct->ability_id == ABILITY_GLUTTONY && has_ability_effect(bank, 0, 1) && percent > 1)
    {
        percent--;
        record_usage_of_ability(bank, ABILITY_GLUTTONY);
    }
    if (ptr_to_struct->current_hp && ptr_to_struct->current_hp <= (ptr_to_struct->max_hp >> percent))
        return 1;
    return 0;
}

u8 item_battle_effects(u8 switchid, u8 bank, u8 move_turn)
{
    u8 item_effect = get_item_effect(bank, 1);
    u8 effect = false;
    u8 eaten_berry = false;
    u8 quality = get_all_item_quality(bank);
    switch (switchid)
    {
    case 0: //switch in items
        switch (item_effect)
        {
        case ITEM_EFFECT_AMULETCOIN:
            if (!is_bank_from_opponent_side(bank))
            {
                battle_stuff_ptr.ptr->money_multiplier = 2;
                effect = true;
            }
            break;
        case ITEM_EFFECT_WHITEHERB:
            effect = white_herb_effect(bank);
            break;
        case ITEM_EFFECT_AIRBALLOON:
            if (!new_battlestruct.ptr->field_affecting.gravity)
            {
                effect = 1;
                active_bank = bank;
                call_bc_move_exec(&airballon_bs);
                record_usage_of_item(bank, ITEM_EFFECT_AIRBALLOON);
            }
        }
        break;
    case 1: //battle end turn
        switch (item_effect)
        {
            u8 flavour;
        case ITEM_EFFECT_ORANBERRY:
            if (hp_condition(bank, 1) && !move_turn && !new_battlestruct.ptr->bank_affecting[bank_target].heal_block)
            {
                effect = 4;
                if (quality > battle_participants[bank].max_hp - battle_participants[bank].current_hp)
                    quality = battle_participants[bank].max_hp - battle_participants[bank].current_hp;
                damage_loc = quality * -1;
                call_bc_move_exec((void*)0x082DB7C4);
                eaten_berry = 1;
            }
            break;
        case ITEM_EFFECT_SITRUSBERRY:
            if (hp_condition(bank, 1) && !move_turn && !new_battlestruct.ptr->bank_affecting[bank_target].heal_block)
            {
                effect = 4;
                s32 damage = battle_participants[bank].max_hp >> 2;
                if (damage > battle_participants[bank].max_hp - battle_participants[bank].current_hp)
                    damage = battle_participants[bank].max_hp - battle_participants[bank].current_hp;
                damage_loc = damage * -1;
                call_bc_move_exec((void*)0x082DB7C4);
                eaten_berry = 1;
            }
            break;
        case ITEM_EFFECT_CHERIBERRY:
            if (battle_participants[bank].status.flags.paralysis)
            {
                effect = 1;
                battle_participants[bank].status.flags.paralysis = 0;
                call_bc_move_exec((void*)0x082DB706);
                eaten_berry = 1;
            }
            break;
        case ITEM_EFFECT_CHESTOBERRY:
            if (battle_participants[bank].status.flags.sleep)
            {
                effect = 1;
                battle_participants[bank].status.flags.sleep = 0;
                call_bc_move_exec((void*)0x082DB766);
                eaten_berry = 1;
            }
            break;
        case ITEM_EFFECT_PECHABERRY:
            if (battle_participants[bank].status.flags.poison || battle_participants[bank].status.flags.toxic_poison)
            {
                effect = 1;
                battle_participants[bank].status.flags.poison = 0;
                battle_participants[bank].status.flags.toxic_poison = 0;
                call_bc_move_exec((void*)0x082DB71E);
                eaten_berry = 1;
            }
            break;
        case ITEM_EFFECT_RAWSTBERRY:
            if (battle_participants[bank].status.flags.burn)
            {
                effect = 1;
                battle_participants[bank].status.flags.burn = 0;
                call_bc_move_exec((void*)0x082DB736);
                eaten_berry = 1;
            }
            break;
        case ITEM_EFFECT_ASPEARBERRY:
            if (battle_participants[bank].status.flags.freeze)
            {
                effect = 1;
                battle_participants[bank].status.flags.freeze = 0;
                call_bc_move_exec((void*)0x082DB74E);
                eaten_berry = 1;
            }
            break;
        case ITEM_EFFECT_LEPPABERRY:
            if (!move_turn)
            {
                for (u8 i = 0; i < 4; i++)
                {
                    if (battle_participants[bank].moves[i] && battle_participants[bank].current_pp[i] == 0)
                    {
                        effect = 3;
                        eaten_berry = 1;
                        break;
                    }
                }
            }
            break;
        case ITEM_EFFECT_PERSIMBERRY:
            if (battle_participants[bank].status2.confusion)
            {
                battle_participants[bank].status2.confusion = 0;
                effect = 2;
                call_bc_move_exec((void*)0x82DB77E);
                eaten_berry = 1;
            }
            break;
        case ITEM_EFFECT_LUMBERRY:
            if (battle_participants[bank].status.int_status || battle_participants[bank].status2.confusion)
            {
                copy_status_condition_text(bank, 1);
                battle_participants[bank].status2.confusion = 0;
                battle_participants[bank].status.int_status = 0;
                eaten_berry = 1;
                effect = 1;
                call_bc_move_exec((void*)0x082DB794);
            }
            break;
        case ITEM_EFFECT_FIGYBERRY:
            flavour = SPICY_PREFERENCE;
            goto HEAL_CONFUSE;
        case ITEM_EFFECT_WIKIBERRY:
            flavour = DRY_PREFERENCE;
            goto HEAL_CONFUSE;
        case ITEM_EFFECT_MAGOBERRY:
            flavour = SWEET_PREFERENCE;
            goto HEAL_CONFUSE;
        case ITEM_EFFECT_AGUAVBERRY:
            flavour = BITTER_PREFERENCE;
            goto HEAL_CONFUSE;
        case ITEM_EFFECT_IAPAPABERRY:
            flavour = SOUR_PREFERENCE;
        HEAL_CONFUSE:
            if (hp_condition(bank, 1) && !move_turn && !new_battlestruct.ptr->bank_affecting[bank_target].heal_block)
            {
                s32 damage = (battle_participants[bank].max_hp / quality);
                if (damage == 0)
                    damage = 1;
                if (damage > battle_participants[bank].max_hp - battle_participants[bank].current_hp)
                    damage = battle_participants[bank].max_hp - battle_participants[bank].current_hp;
                damage_loc = damage * -1;
                effect = 4;
                eaten_berry = 1;
                if (get_poke_flavour_relation(battle_participants[bank].pid, flavour) == FLAVOUR_DISLIKED)
                {
                    call_bc_move_exec((void*)0x082DB824);
                }
                else
                {
                    call_bc_move_exec((void*)0x082DB7C4);
                }
                battle_text_buff1[0] = 0xFD;
                battle_text_buff1[1] = 8;
                battle_text_buff1[2] = 1;
                battle_text_buff1[3] = 0xFF;
            }
            break;
        case ITEM_EFFECT_LIECHIBERRY:
            flavour = 0;
            goto STAT_RAISE;
        case ITEM_EFFECT_GANLONBERRY:
            flavour = 1;
            goto STAT_RAISE;
        case ITEM_EFFECT_SALACBERRY:
            flavour = 2;
            goto STAT_RAISE;
        case ITEM_EFFECT_PETAYABERRY:
            flavour = 3;
            goto STAT_RAISE;
        case ITEM_EFFECT_APICOTBERRY:
            flavour = 4;
        STAT_RAISE:
            if (hp_condition(bank, 2) && !move_turn)
            {
                if (*(&battle_participants[bank].atk_buff + flavour) != 0xC)
                {
                    effect = 5;
                    eaten_berry = 1;
                    call_bc_move_exec((void*)0x82DB84E);
                    battle_text_buff1[0] = 0xFD;
                    battle_text_buff1[1] = 0x5;
                    battle_text_buff1[2] = flavour + 1;
                    battle_text_buff1[3] = 0xFF;
                    bank_partner_def = bank;
                    battle_scripting.stat_changer = 0x11 + flavour;
                    battle_scripting.field10 = 0xF + flavour;
                    battle_scripting.field11 = move_turn;
                }
            }
            break;
        case ITEM_EFFECT_LANSATBERRY:
            if (hp_condition(bank, 2) && !move_turn)
            {
                if (!(battle_participants[bank].status2.focus_energy))
                {
                    battle_participants[bank].status2.focus_energy = 1;
                    effect = 2;
                    call_bc_move_exec((void*)0x82DB869);
                    eaten_berry = 1;
                }
            }
            break;
        case ITEM_EFFECT_STARFBERRY:
            if (hp_condition(bank, 2) && !move_turn)
            {
                u8 doable = 0;
                for (u8 i = 0; i < 5; i++)
                {
                    if (*(&battle_participants[bank].atk_buff + i) != 0xC)
                        doable |= bits_table[i];
                }
                while (doable)
                {
                    u8 rand = __umodsi3(rng(), 5);
                    if (doable & bits_table[rand])
                    {
                        effect = 5;
                        eaten_berry = 1;
                        battle_scripting.stat_changer = 0x21 + rand;
                        call_bc_move_exec((void*)0x82DB84E);
                        battle_scripting.field10 = 0xF + rand;
                        battle_scripting.field11 = move_turn;
                        break;
                    }
                }
            }
            break;
        case ITEM_EFFECT_WHITEHERB:
            effect = white_herb_effect(bank);
            break;
        case ITEM_EFFECT_MENTALHERB:
            effect = mental_herb_effect(bank);
            break;
        case ITEM_EFFECT_STICKYBARB:
            STICKYBARB:
            if (battle_participants[bank].current_hp && !move_turn && !(has_ability_effect(bank, 0, 1) && battle_participants[bank].ability_id == ABILITY_MAGIC_GUARD))
            {
                effect = 4;
                damage_loc = battle_participants[bank].max_hp >> 3;
                call_bc_move_exec(&itemhurt_bs);
                record_usage_of_item(bank, get_item_effect(bank, 0));
            }
            break;
        case ITEM_EFFECT_BLACKSLUDGE:
            if (!is_of_type(bank, TYPE_POISON))
            {
                goto STICKYBARB;
            }
        case ITEM_EFFECT_LEFTOVERS:
            if (battle_participants[bank].max_hp != battle_participants[bank].current_hp && !move_turn && !new_battlestruct.ptr->bank_affecting[bank_target].heal_block)
            {
                effect = 4;
                s32 damage = battle_participants[bank].max_hp >> 4;
                if (damage == 0)
                    damage = 1;
                if (damage > battle_participants[bank].max_hp - battle_participants[bank].current_hp)
                    damage = battle_participants[bank].max_hp - battle_participants[bank].current_hp;
                damage_loc = damage * -1;
                call_bc_move_exec((void*)0x082DB7F1);
                record_usage_of_item(bank, get_item_effect(bank, 0));
            }
            break;
        }
        active_bank = bank;
        battle_scripting.active_bank = bank;
        bank_attacker = bank;
        break;
    case 2: //having effect on target items
        active_bank = bank;
        battle_scripting.active_bank = bank;
        u8 move_type;
        if (battle_stuff_ptr.ptr->dynamic_move_type)
            move_type = battle_stuff_ptr.ptr->dynamic_move_type & 0x3F;
        else
            move_type = move_table[current_move].type;

        switch (item_effect)
        {
        case ITEM_EFFECT_WEAKNESSPOLICY:
            if (move_outcome.super_effective && battle_participants[bank].current_hp)
            {
                if (battle_participants[bank].atk_buff != 0xC || battle_participants[bank].sp_atk_buff != 0xC)
                {
                    effect = 5;
                    battle_scripting.field11 = 0;
                    call_bc_move_exec(&weaknesspolicy_bs);
                }
            }
            break;
        case ITEM_EFFECT_REDCARD:
            battle_scripting.active_bank = bank_attacker;
        case ITEM_EFFECT_EJECTBUTTON:
            if ((special_statuses[bank].moveturn_losthp_physical || special_statuses[bank].moveturn_losthp_special) && battle_participants[bank].current_hp)
            {
                void* current_instruction = battlescripts_curr_instruction;
                battlescripts_curr_instruction = &can_switch_bs;
                jump_if_cannot_switch_atk4F();
                if ((u32)battlescripts_curr_instruction != 1)
                {
                    effect = 6;
                    battlescripts_curr_instruction = current_instruction;
                    call_bc_move_exec(&ejectbutton_bs);
                }
                else
                     battlescripts_curr_instruction = current_instruction;
            }
            break;
        case ITEM_EFFECT_ROCKYHELMET:
            if (move_table[current_move].move_flags.flags.makes_contact && (special_statuses[bank].moveturn_losthp_physical || special_statuses[bank].moveturn_losthp_special) && !(battle_participants[bank_attacker].status2.substitute) && battle_participants[bank_attacker].current_hp)
            {
                damage_loc = __udivsi3(battle_participants[bank_attacker].max_hp, 6);
                if (damage_loc == 0)
                    damage_loc = 1;
                effect = 7;
                call_bc_move_exec(&rockyhelmet_bs);
                record_usage_of_item(bank, ITEM_EFFECT_ROCKYHELMET);
            }
            break;
        case ITEM_EFFECT_STICKYBARB:
            if (move_table[current_move].move_flags.flags.makes_contact && TARGET_TURN_DAMAGED && !(battle_participants[bank_attacker].status2.substitute) && battle_participants[bank_attacker].current_hp && battle_participants[bank_attacker].held_item == 0)
            {
                effect = 2;
                u16* target_item = &battle_participants[bank].held_item;
                u16* attacker_item = &battle_participants[bank_attacker].held_item;
                *attacker_item = *target_item;
                *target_item = 0;
                active_bank = bank;
                prepare_setattributes_in_battle(0, REQUEST_HELDITEM_BATTLE, 0, 2, target_item);
                mark_buffer_bank_for_execution(bank);
                active_bank = bank_attacker;
                prepare_setattributes_in_battle(0, REQUEST_HELDITEM_BATTLE, 0, 2, attacker_item);
                mark_buffer_bank_for_execution(active_bank);
                call_bc_move_exec(&stickybarbswap);
                bank = bank_attacker;
            }
            break;
        case ITEM_EFFECT_AIRBALLOON:
            if (special_statuses[bank].moveturn_losthp_physical || special_statuses[bank].moveturn_losthp_special)
            {
                effect = 8;
                call_bc_move_exec(&balloonpops_bs);
            }
            break;
        case ITEM_EFFECT_DESTINYKNOT:
            if (battle_participants[bank].status2.in_love && battle_participants[bank].current_hp && !(has_ability_effect(bank_attacker, 0, 1) && battle_participants[bank_attacker].ability_id == ABILITY_OBLIVIOUS))
            {
                if (!(battle_participants[bank_attacker].status2.in_love & bits_table[bank]))
                {
                    battle_participants[bank_attacker].status2.in_love |= bits_table[bank];
                    effect = 9;
                    call_bc_move_exec(&destinyknot_bs);
                }
            }
            break;
        case ITEM_EFFECT_LUMINOUSMOSS:
            if (move_type == TYPE_WATER && battle_participants[bank].sp_def_buff != 0xC && !(move_outcome.missed || move_outcome.failed || move_outcome.not_affected) && battle_participants[bank].current_hp)
            {
                battle_scripting.stat_changer = 0x15;
                battle_scripting.field10 = 0xF + 4;
                goto STATRAISE;
            }
            break;
            STATRAISE:
                effect = 5;
                call_bc_move_exec(&statraise_bs);
                battle_scripting.field11 = 0;
            break;
        case ITEM_EFFECT_CELLBATTERY:
            if (move_type == TYPE_ELECTRIC && battle_participants[bank].atk_buff != 0xC && !(move_outcome.missed || move_outcome.failed || move_outcome.not_affected) && battle_participants[bank].current_hp)
            {
                battle_scripting.stat_changer = 0x11;
                battle_scripting.field10 = 0xF;
                goto STATRAISE;
            }
            break;
        case ITEM_EFFECT_SNOWBALL:
            if (move_type == TYPE_ICE && battle_participants[bank].atk_buff != 0xC && !(move_outcome.missed || move_outcome.failed || move_outcome.not_affected) && battle_participants[bank].current_hp)
            {
                battle_scripting.stat_changer = 0x11;
                battle_scripting.field10 = 0xF;
                goto STATRAISE;
            }
            break;
        case ITEM_EFFECT_ABSORBBULB:
            if (move_type == TYPE_WATER && battle_participants[bank].sp_atk_buff != 0xC && !(move_outcome.missed || move_outcome.failed || move_outcome.not_affected) && battle_participants[bank].current_hp)
            {
                battle_scripting.stat_changer = 0x14;
                battle_scripting.field10 = 0xF + 3;
                goto STATRAISE;
            }
            break;

        }
        break;
    case 3: //move turn
        for (u8 i = 0; i < no_of_all_banks; i++)
        {
            item_effect = get_item_effect(i, 1);
            bank = i;
            switch (item_effect)
            {
            case ITEM_EFFECT_CHERIBERRY:
                if (battle_participants[bank].status.flags.paralysis)
                {
                    battle_participants[bank].status.flags.paralysis = 0;
                    effect = 1;
                    battlescript_push();
                    battlescripts_curr_instruction = (void*) 0x82DB70C;
                    eaten_berry = 1;
                }
                break;
            case ITEM_EFFECT_CHESTOBERRY:
                if (battle_participants[bank].status.flags.sleep)
                {
                    battle_participants[bank].status.flags.sleep = 0;
                    effect = 1;
                    battlescript_push();
                    battlescripts_curr_instruction = (void*) 0x82DB76C;
                    eaten_berry = 1;
                }
                break;
            case ITEM_EFFECT_PECHABERRY:
                if (battle_participants[bank].status.flags.poison || battle_participants[bank].status.flags.toxic_poison)
                {
                    battle_participants[bank].status.flags.poison = 0;
                    battle_participants[bank].status.flags.toxic_poison = 0;
                    effect = 1;
                    battlescript_push();
                    battlescripts_curr_instruction = (void*) 0x82DB724;
                    eaten_berry = 1;
                }
                break;
            case ITEM_EFFECT_RAWSTBERRY:
                if (battle_participants[bank].status.flags.burn)
                {
                    effect = 1;
                    battle_participants[bank].status.flags.burn = 0;
                    battlescript_push();
                    battlescripts_curr_instruction = (void*) 0x82DB73C;
                    eaten_berry = 1;
                }
                break;
            case ITEM_EFFECT_ASPEARBERRY:
                if (battle_participants[bank].status.flags.freeze)
                {
                    effect = 1;
                    battle_participants[bank].status.flags.freeze = 0;
                    battlescript_push();
                    battlescripts_curr_instruction = (void*) 0x82DB754;
                    eaten_berry = 1;
                }
                break;
            case ITEM_EFFECT_PERSIMBERRY:
                if (battle_participants[bank].status2.confusion)
                {
                    battle_participants[bank].status2.confusion = 0;
                    effect = 2;
                    battlescript_push();
                    battlescripts_curr_instruction = (void*) 0x82DB784;
                    eaten_berry = 1;
                }
                break;
            case ITEM_EFFECT_LUMBERRY:
                if (battle_participants[bank].status.int_status || battle_participants[bank].status2.confusion)
                {
                    copy_status_condition_text(bank, 1);
                    battle_participants[bank].status2.confusion = 0;
                    battle_participants[bank].status.int_status = 0;
                    eaten_berry = 1;
                    effect = 1;
                    battlescript_push();
                    battlescripts_curr_instruction = (void*) 0x82DB79A;
                }
                break;
            case ITEM_EFFECT_WHITEHERB:
                effect = white_herb_effect(bank);
                break;
            case ITEM_EFFECT_MENTALHERB:
                effect = mental_herb_effect(bank);
                break;
            }
            if (effect)
            {
                bank_attacker = bank;
                active_bank = bank;
                battle_scripting.active_bank = bank;
                break;
            }
        }
        break;
    case 4:
        {
            quality = get_all_item_quality(bank_attacker);
            item_effect = get_item_effect(bank_attacker, 1);
            switch (item_effect)
            {
            case ITEM_EFFECT_KINGSROCK:
                if (MOVE_WORKED && special_statuses[bank_target].moveturn_losthp && percent_chance(quality) && move_table[current_move].move_flags.flags.affected_by_kingsrock)
                {
                    battle_participants[bank_target].status2.flinched = 1;
                    record_usage_of_item(bank_attacker, ITEM_EFFECT_KINGSROCK);
                }
                break;
            case ITEM_EFFECT_SHELLBELL:
                if (MOVE_WORKED && special_statuses[bank_target].moveturn_losthp && special_statuses[bank_target].moveturn_losthp != 0xFFFF && bank_target != bank_attacker && battle_participants[bank_attacker].current_hp && battle_participants[bank_attacker].current_hp <= battle_participants[bank_target].max_hp && !new_battlestruct.ptr->bank_affecting[bank_target].heal_block)
                {
                    effect = 2;
                    another_active_bank = bank_attacker;
                    bank = bank_attacker;
                    battle_scripting.active_bank = bank_attacker;
                    u32 damage = __udivsi3(special_statuses[bank_target].moveturn_losthp, quality);
                    if (damage == 0)
                        damage = 1;
                    damage_loc = damage * -1;
                    special_statuses[bank_target].moveturn_losthp = 0;
                    battlescript_push();
                    battlescripts_curr_instruction = (void*)0x82DB7F7;
                    record_usage_of_item(bank, ITEM_EFFECT_SHELLBELL);
                }
                break;
            }
        }
        break;
    case 5: //toxic and flame orb
        switch (item_effect)
        {
        case ITEM_EFFECT_FLAMEORB:
            if (!cant_become_burned(bank, 1))
            {
                effect = 1;
                battle_participants[bank].status.flags.burn = 1;
                call_bc_move_exec(&flameorb_bs);
                record_usage_of_item(bank, ITEM_EFFECT_FLAMEORB);
            }
            break;
        case ITEM_EFFECT_TOXICORB:
            if (!cant_poison(bank, 1))
            {
                effect = 1;
                battle_participants[bank].status.flags.toxic_poison = 1;
                call_bc_move_exec(&toxicorb_bs);
                record_usage_of_item(bank, ITEM_EFFECT_TOXICORB);
            }
            break;
        }
        break;
    }
    if (effect)
    {
        u16 item = battle_participants[bank].held_item;
        last_used_item = item;
        new_battlestruct.ptr->various.recently_used_item = item;
        new_battlestruct.ptr->bank_affecting[bank].item_used = 1;
        if (eaten_berry)
            new_battlestruct.ptr->bank_affecting[bank].eaten_berry = 1;
        if (effect == 1)
        {
            active_bank = bank;
            prepare_setattributes_in_battle(0, 0x28, 0, 4, &battle_participants[bank].status.flags);
            mark_buffer_bank_for_execution(bank);
        }
    }
    return effect;
}

u8 not_magicguard(u8 bank);

u8 berry_eaten(u8 how_to_call, u8 bank)
{
    u8 cheek_pouch = 0;
    if (new_battlestruct.ptr->bank_affecting[bank].eaten_berry)
    {
        new_battlestruct.ptr->bank_affecting[bank].eaten_berry = 0;
        if (check_ability(bank, ABILITY_CHEEK_POUCH) && battle_participants[bank].current_hp != battle_participants[bank].max_hp)
        {
            cheek_pouch = 1;
            s32 damage = __udivsi3(battle_participants[bank].max_hp, 3);
            if (damage == 0)
                damage = 1;
            damage_loc = damage * -1;
            if (how_to_call == 0)
            {
                battlescript_push();
                battlescripts_curr_instruction = &cheekpouch_bs;
                battle_scripting.active_bank = bank;
            }
            else
            {
                call_bc_move_exec(&cheekpouch2_bs);
            }
            record_usage_of_ability(bank, ABILITY_CHEEK_POUCH);
        }
    }
    return cheek_pouch;
}

u8 poison_heal_check()
{
    u8 effect = 0;
    if (check_ability(active_bank, ABILITY_POISON_HEAL))
    {
        effect = 1;
        if (!(new_battlestruct.ptr->bank_affecting[active_bank].heal_block || battle_participants[active_bank].current_hp == battle_participants[active_bank].max_hp))
        {
            damage_loc = get_1_8_of_max_hp(active_bank) * -1;
            call_bc_move_exec(&poisonheal_bs);
        }
    }
    return effect;
}

u8 check_if_move_failed(u8 bank)
{
    struct protect_struct* failed = &protect_structs[bank];
    if (failed->flag1_confusion_self_damage)
        return 1;
    if (failed->flag1_love_immobility)
        return 1;
    if (failed->flag1_used_disabled_move)
        return 1;
    if (failed->flag1_used_imprisoned_move)
        return 1;
    if (failed->flag2_used_taunted_move)
        return 1;
    if (failed->flag2_flinch_immobility)
        return 1;
    if (failed->flag1_noteffective)
        return 1;
    if (new_battlestruct.ptr->bank_affecting[bank].move_failed)
        return 1;
    return 0;
}

u8 affected_by_substitute(u8 substitute_bank);

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

void effect_stat_change(void* pointer)
{
    battle_scripting.stat_changer = battle_communication_struct.move_effect;
    battlescript_push();
    battlescripts_curr_instruction = pointer;
}

u8 can_lose_item(u8 bank, u8 stickyhold_check, u8 sticky_message)
{
    u8 can_lose = 1;
    u16 item = battle_participants[bank].held_item;
    if (stickyhold_check && battle_participants[bank].ability_id == ABILITY_STICKY_HOLD && has_ability_effect(bank, 1, 1))
    {
        can_lose = 0;
        if (sticky_message)
        {
            battlescript_push();
            battlescripts_curr_instruction = (void*) 0x82DB682;
            last_used_ability = ABILITY_STICKY_HOLD;
            record_usage_of_ability(bank_target, ABILITY_STICKY_HOLD);
        }
    }
    else if (item == ITEM_ENIGMABERRY || item_is_mail(item))
        can_lose = 0;
    //mega stones, drives for genesects and plates for Arceus else if ()
    return can_lose;
}

u8 calculate_effect_chance(u8 bank, u16 move)
{
    u8 effect = move_table[current_move].effect_chance;
    if (check_ability(bank, ABILITY_SERENE_GRACE))
        effect *= 2;
    if (effect >= 100)
        return 1;
    return percent_chance(effect);
}

void move_effect_setter(u8 primary, u8 certain)
{
    battlescripts_curr_instruction++;
    u8 *move_effect = &battle_communication_struct.move_effect;
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
    if (check_ability(bank_attacker, ABILITY_SHEER_FORCE) && bank_to_apply != bank_attacker && !primary && find_move_in_table(current_move, &sheerforce_moves_table[0]) && *move_effect < 0x30)
        *move_effect = 0; //move effects past 0x30 affect always the attacker

    struct battle_participant* applier_bank = &battle_participants[bank_to_apply];
    u16 current_hp = applier_bank->current_hp;
    u8 substitute = 0;
    if (affected_by_substitute(bank_to_apply) && !affects_user)
        substitute = 1;
    u8 shield_dust = 0;
    if (applier_bank->ability_id == ABILITY_SHIELD_DUST && has_ability_effect(bank_to_apply, 1, 1) && !primary)
        shield_dust = 1;

    u8 status_change = 8;
    u8 status_table = status_flags_for_move_effects[*move_effect];
    void* script_pointer = battlescripts_for_moveeffects[*move_effect];
    if (*move_effect == 9) //tri attack
        *move_effect = 3 + __umodsi3(rng(), 3);
    void* ability_prevention = 0;

    switch (*move_effect)
    {
        case 1: //put to sleep
            if (current_hp && !shield_dust && !substitute)
                status_change = cant_fall_asleep(bank_to_apply, 0);
            break;
        case 2: //poison
        case 6: //bad poison
            if (current_hp && !shield_dust && !substitute)
            {
                ability_prevention = (void*) 0x082DB5E9;
                status_change = cant_poison(bank_to_apply, 0);
            }
            break;
        case 3: //burn
            if (current_hp && !shield_dust && !substitute)
                status_change = cant_become_burned(bank_to_apply, 0);
            break;
        case 4: //freeze
            if (current_hp && !shield_dust && !substitute)
                status_change = cant_become_freezed(bank_to_apply, 0);
            break;
        case 5: //paralysis
            if (current_hp && !shield_dust && !substitute)
            {
                ability_prevention = (void*) 0x082DB5DD;
                status_change = cant_become_paralyzed(bank_to_apply, 0);
            }
            break;
        case 7: //confusion
            if (current_hp && !shield_dust && !substitute && !applier_bank->status2.confusion && !check_ability(bank_to_apply, ABILITY_OWN_TEMPO))
            {
                applier_bank->status2.confusion = 2 + __umodsi3(rng(), 4);
                battlescript_push();
                battlescripts_curr_instruction = script_pointer;
            }
            break;
        case 8: //flinch
            statustoeffect();
            if (*move_effect && current_hp && !shield_dust && !substitute && calculate_effect_chance(bank_attacker, current_move))
            {
                battlescripts_curr_instruction--;
                move_effect_setter(0, 0);
            }
            if (current_hp && !shield_dust && !substitute && calculate_effect_chance(bank_attacker, current_move))
            {
                if (check_ability(bank_to_apply, ABILITY_INNER_FOCUS))
                {
                    if (certain || primary)
                    {
                        last_used_ability = ABILITY_INNER_FOCUS;
                        record_usage_of_ability(bank_to_apply, ABILITY_INNER_FOCUS);
                        battlescripts_curr_instruction = (void*) 0x82DB603;
                    }
                }
                else
                {
                    if (get_bank_turn_order(bank_to_apply) > current_move_turn)
                        applier_bank->status2.flinched = 1;
                }
            }
            break;
        case 10: //heal target's status
            if (applier_bank->status.int_status & move_table[current_move].arg1 && current_hp)
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
            if (!applier_bank->status2.trapped_in_wrap && current_hp)
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
            {
                u16* targets_item = &applier_bank->held_item;
                u16* attackers_item = &battle_participants[bank_attacker].held_item;
                u8 stickymsg = 0;
                if (primary || certain)
                    stickymsg = 1;
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
            if (applier_bank->held_item && can_lose_item(bank_to_apply, 1, 1))
            {
                last_used_item = applier_bank->held_item;
                applier_bank->held_item = 0;
                battle_effects_duration.knocked_off_pokes[is_bank_from_opponent_side(bank_to_apply)] |= bits_table[battle_team_id_by_side[bank_to_apply]];
                battlescript_push();
                battlescripts_curr_instruction = (void*) 0x082DB168;
                battle_stuff_ptr.ptr->choiced_move[bank_to_apply] = 0;
            }
            break;
        case 0x11: //stat +1
        case 0x12:
        case 0x13:
        case 0x14:
        case 0x15:
        case 0x16:
        case 0x17:
            if (change_stats(0x10, *move_effect &7, affects_user, 0) == 0 && current_hp)
                effect_stat_change(&stat_raise);
            break;
        case 0x18: //atk - 1
        case 0x19: //def - 1
        case 0x1A: //spd - 1
        case 0x1B: //sp atk - 1
        case 0x1C: //sp def - 1
        case 0x1D: // evasion - 1
        case 0x1E: //accuracy - 1
            if (change_stats(0x90, (*move_effect - 7) &7, affects_user, 0) == 0 && current_hp)
                effect_stat_change(&stat_lower);
            break;
        case 0x21:
        case 0x22:
        case 0x23:
        case 0x24:
        case 0x25:
        case 0x26:
        case 0x27:
            if (change_stats(0x20, *move_effect &7, affects_user, 0) == 0 && current_hp)
                effect_stat_change(&stat_raise);
            break;
        case 0x28: //atk - 2
        case 0x29: //def - 2
        case 0x2A: //spd - 2
        case 0x2B: //sp atk - 2
        case 0x2C: //sp def - 2
        case 0x2D: // evasion - 2
        case 0x2E: //accuracy - 2
            if (change_stats(0xA0, (*move_effect - 7) &7 , affects_user, 0) == 0 && current_hp)
                effect_stat_change(&stat_lower);
            break;
        case 0x2F: //multiple stats change
            battlescript_push();
            battlescripts_curr_instruction = &attackingstatschange_bs;
            break;
        case 0x30: //recoil
            statustoeffect();
            if (*move_effect && current_hp && !shield_dust && !substitute && calculate_effect_chance(bank_attacker, current_move))
            {
                battlescripts_curr_instruction--;
                move_effect_setter(0, 0);
            }
            if (recoil_damage(bank_attacker))
            {
                battlescript_push();
                battlescripts_curr_instruction = (void*) 0x082DB3F4; //recoil battlescript
            }
            break;
        case 0x31: //recharge needed
            applier_bank->status2.recharge = 1;
            disable_structs[bank_to_apply].recharge_counter = 2;
            locked_move[bank_to_apply] = current_move;
            break;
        case 0x32: //rage setter
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
            if (applier_bank->status2.uproar == 0 && MOVE_WORKED)
            {
                applier_bank->status2.uproar = 3;
                applier_bank->status2.multiple_turn_move = 1;
                locked_move[bank_to_apply] = current_move;
                battlescript_push();
                battlescripts_curr_instruction = (void*) 0x82DB3C2;
            }
            break;
        case 0x35: //set thrash
            if (applier_bank->status2.locked_and_confuse == 0 && MOVE_WORKED)
            {
                applier_bank->status2.locked_and_confuse = 2 + (rng() & 1);
                applier_bank->status2.multiple_turn_move = 1;
                locked_move[bank_to_apply] = current_move;
            }
            break;
        case 0x36: //berry eating
            if (MOVE_WORKED && get_item_pocket_id(battle_participants[bank_to_apply].held_item == 4))
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
    if (status_change != 8)
    {
        switch (status_change)
        {
        case 0: //can apply condition
            battlescript_push();
            if (status_table == 7)
                applier_bank->status.flags.sleep = 2 + __umodsi3(rng(), 4);
            else
                applier_bank->status.int_status |= status_table;

            battlescripts_curr_instruction = script_pointer;
            active_bank = bank_to_apply;
            prepare_setattributes_in_battle(0, REQUEST_STATUS_BATTLE, 0, 4, &applier_bank->status);
            mark_buffer_bank_for_execution(active_bank);
            if (hitmarker & 0x2000)
            {
                battle_communication_struct.multistring_chooser = 1;
                hitmarker &= 0xFFFFDFFF;
            }
            else
                battle_communication_struct.multistring_chooser = 0;
            if (*move_effect == 1 || *move_effect == 4)
                reset_several_turns_stuff(bank_to_apply);
            else
                battle_stuff_ptr.ptr->synchronize_effect_chooser = *move_effect;
            break;
        case 4: //ability prevents it
            if (primary || certain)
            {
                last_used_ability = battle_participants[bank_to_apply].ability_id;
                record_usage_of_ability(bank_to_apply, last_used_ability);
                if (ability_prevention)
                {
                    battlescript_push();
                    battlescripts_curr_instruction = ability_prevention;
                }
            }
            break;
        }
    }
    battle_communication_struct.move_effect = 0;
    return;
}

u8 battle_turn_move_effects()
{
    u8 effect = 0;
    hitmarker |= 0x1000020;
    u8* bank = &battle_stuff_ptr.ptr->end_turn_checked_bank;
    u8* tracker = &battle_stuff_ptr.ptr->end_turn_statetracker1;
    while (*bank < no_of_all_banks)
    {
        active_bank = bank_attacker = turn_order[*bank];
        if (absent_bank_flags & bits_table[active_bank])
        {
            *bank +=1;
        }
        else
        {
            struct bank_affecting* ptr_to_struct = &new_battlestruct.ptr->bank_affecting[active_bank];
            u16 current_hp = battle_participants[active_bank].current_hp;
            struct battle_participant* attacker_struct = &battle_participants[active_bank];
            switch (*tracker)
            {
                case 0: //clear new turn stuff for banks
                    ptr_to_struct->electrify = 0;
                    ptr_to_struct->gem_boost = 0;
                    ptr_to_struct->just_switched_in = 0;
                    ptr_to_struct->kings_shield = 0;
                    ptr_to_struct->me_first = 0;
                    ptr_to_struct->powder = 0;
                    ptr_to_struct->quashed = 0;
                    ptr_to_struct->spiky_shield = 0;
                    break;
                case 1: //ingrain
                    if (current_hp && current_hp < attacker_struct->max_hp && status3[active_bank].rooted && !ptr_to_struct->heal_block)
                    {
                        damage_loc = get_1_16_of_max_hp(active_bank) * -1;
                        effect = 1;
                        call_bc_move_exec((void*) 0x082DB0EE);
                    }
                    break;
                case 2: //aqua ring
                    if (current_hp && current_hp < attacker_struct->max_hp && ptr_to_struct->aqua_ring && !ptr_to_struct->heal_block)
                    {
                        damage_loc = get_1_16_of_max_hp(active_bank) * -1;
                        effect = 1;
                        hitmarker |= HITMARKER_IGNORE_SUBSTITUTE;
                        call_bc_move_exec(&aquaring_bs);
                    }
                    break;
                case 3: //end turn ability effects
                    if (ability_battle_effects(1, active_bank, 0, 0, 0))
                        effect = 1;
                    break;
                case 4: //item effects
                    if (item_battle_effects(1, active_bank, 0))
                        effect = 1;
                    break;
                case 5: //cheeck pouch
                    if (berry_eaten(1, active_bank))
                        effect = 1;
                    break;
                case 6:
                    break;
                case 7: //check leech seed
                    if (not_magicguard(active_bank) && status3[active_bank].leech_seed && current_hp && battle_participants[status3[active_bank].leech_seed_hp_receiver].current_hp)
                    {
                        effect = 1;
                        bank_target = status3[active_bank].leech_seed_hp_receiver; //funny this bank is a 'target' of receiving hp and the one losing it is the 'attacker'
                        damage_loc = get_1_8_of_max_hp(bank_attacker);
                        battle_scripting.field10 = bank_target;
                        battle_scripting.field11 = bank_attacker;
                        call_bc_move_exec(&leechseed_updated_bs);
                    }
                    break;
                case 8: //poison
                    if (attacker_struct->status.flags.poison && current_hp && not_magicguard(active_bank))
                    {
                        if (!poison_heal_check())
                        {
                            damage_loc = get_1_8_of_max_hp(active_bank);
                            call_bc_move_exec((void*)0x82DB23D);
                        }
                        effect = 1;
                    }
                    break;
                case 9: //bad poison
                    if (attacker_struct->status.flags.toxic_poison && current_hp && not_magicguard(active_bank))
                    {
                        if (!poison_heal_check())
                        {
                            if (attacker_struct->status.flags.poison_counter != 15)
                                attacker_struct->status.flags.poison_counter++;
                            u32 damage = attacker_struct->max_hp >> 4;
                            if (damage == 0)
                                damage = 1;
                            damage_loc = damage * attacker_struct->status.flags.poison_counter;
                            call_bc_move_exec((void*)0x82DB23D);
                        }
                        effect = 1;
                    }
                    break;
                case 10: //burn
                    if (attacker_struct->status.flags.burn && current_hp && not_magicguard(active_bank))
                    {
                        effect = 1;
                        damage_loc = get_1_8_of_max_hp(active_bank);
                        call_bc_move_exec((void*)0x082DB25F);
                    }
                    break;
                case 11: //check nightmare
                    if (current_hp && attacker_struct->status2.nightmare && not_magicguard(active_bank))
                    {
                        if (attacker_struct->status.flags.sleep)
                        {
                            effect = 1;
                            damage_loc = get_1_4_of_max_hp(active_bank);
                            call_bc_move_exec((void*)0x082DB33F);
                        }
                        else
                        {
                            attacker_struct->status2.nightmare = 0;
                        }
                    }
                    break;
                case 12: //check if cursed
                    if (current_hp && attacker_struct->status2.cursed && not_magicguard(active_bank))
                    {
                        effect = 1;
                        damage_loc = get_1_4_of_max_hp(active_bank);
                        call_bc_move_exec((void*)0x82DB350);
                    }
                    break;
                case 13: //check being trapped in a multiturn attack like wrap or whirpool
                    if (current_hp && attacker_struct->status2.trapped_in_wrap)
                    {
                        u16 trapped_move = battle_stuff_ptr.ptr->trapped_move[active_bank];
                        battle_text_buff1[0] = 0xFD;
                        battle_text_buff1[1] = 2;
                        battle_text_buff1[2] = trapped_move;
                        battle_text_buff1[3] = trapped_move >> 8;
                        battle_text_buff1[4] = 0xFF;
                        attacker_struct->status2.trapped_in_wrap--;
                        if (attacker_struct->status2.trapped_in_wrap && not_magicguard(active_bank)) //we're doing damage
                        {
                            battle_scripting.field10 = trapped_move;
                            battle_scripting.field11 = trapped_move >> 8;
                            if (get_item_effect(new_battlestruct.ptr->bank_affecting[active_bank].wrap_bank, 1) == ITEM_EFFECT_BINDINGBAND)
                            {
                                u32 damage = __udivsi3(attacker_struct->max_hp, 6);
                                if (damage == 0)
                                    damage = 1;
                                damage_loc = damage;
                            }
                            else
                                damage_loc = get_1_8_of_max_hp(active_bank);

                            call_bc_move_exec((void*) 0x82DB30E);
                        }
                        else if (attacker_struct->status2.trapped_in_wrap == 0) //broke free
                        {
                            call_bc_move_exec((void*)0x82DB320);
                        }
                        effect = 1;
                    }
                    break;
                case 14: //uproar
                    if (attacker_struct->status2.uproar)
                    {
                        for (u8 i = 0; i < no_of_all_banks; i++)
                        {
                            bank_attacker = i;
                            if (battle_participants[i].status.flags.sleep && !(battle_participants[i].ability_id == ABILITY_SOUNDPROOF && has_ability_effect(i, 0, 1)))
                            {
                                battle_participants[i].status.flags.sleep = 0;
                                battle_participants[i].status2.nightmare = 0;
                                battle_communication_struct.multistring_chooser = 1;
                                call_bc_move_exec((void*)0x82DB234);
                                active_bank = i;
                                prepare_setattributes_in_battle(0, REQUEST_STATUS_BATTLE, 0, 4, &battle_participants[i].status);
                                mark_buffer_bank_for_execution(active_bank);
                                effect = 5;
                                break;
                            }
                        }
                        if (effect != 5)
                        {
                            bank_attacker = active_bank;
                            attacker_struct->status2.uproar--;
                            if (check_if_move_failed(active_bank) || attacker_struct->status2.uproar == 0)
                            {
                                reset_several_turns_stuff(active_bank);
                                battle_communication_struct.multistring_chooser = 1;
                            }
                            else
                            {
                                battle_communication_struct.multistring_chooser = 0;
                                attacker_struct->status2.multiple_turn_move = 1;
                            }
                            effect = 1;
                            call_bc_move_exec((void*)0x82DB2A6);
                        }
                    }
                    break;
                case 15: //thrash aka lock and confuse
                    if (attacker_struct->status2.locked_and_confuse)
                    {
                        attacker_struct->status2.locked_and_confuse--;
                        if (check_if_move_failed(active_bank))
                            reset_several_turns_stuff(active_bank);
                        else if (attacker_struct->status2.locked_and_confuse == 0 && attacker_struct->status2.multiple_turn_move)
                        {
                            attacker_struct->status2.multiple_turn_move = 0;
                            if (!attacker_struct->status2.confusion)
                            {
                                effect = 1;
                                battle_communication_struct.move_effect = 0x47;
                                set_move_effect(1, 0);
                                if (attacker_struct->status2.confusion)
                                    call_bc_move_exec((void*)0x82DB2AF);
                            }
                        }
                    }
                    break;
                case 16: //disable
                    if (current_hp && disable_structs[active_bank].disable_timer)
                    {
                        s8 position = get_move_position(active_bank, disable_structs[active_bank].disabled_move);
                        if (position == -1)
                        {
                            disable_structs[active_bank].disabled_move = 0;
                            disable_structs[active_bank].disable_timer = 0;
                        }
                        else
                        {
                            disable_structs[active_bank].disable_timer--;
                            if (disable_structs[active_bank].disable_timer == 0)
                            {
                                effect = 1;
                                disable_structs[active_bank].disabled_move = 0;
                                call_bc_move_exec((void*)0x082DAE23);
                            }
                        }
                    }
                    break;
                case 17: //encore
                    if (disable_structs[active_bank].encore_timer)
                    {
                        s8 position = get_move_position(active_bank, disable_structs[active_bank].encored_move);
                        if (position == -1)
                        {
                            disable_structs[active_bank].encored_move = 0;
                            disable_structs[active_bank].encore_timer = 0;
                        }
                        else
                        {
                            disable_structs[active_bank].encore_timer--;
                            if (disable_structs[active_bank].encore_timer == 0)
                            {
                                effect = 1;
                                disable_structs[active_bank].encored_move = 0;
                                call_bc_move_exec((void*)0x082DAE37);
                            }
                        }
                    }
                    break;
                case 18: //embargo
                    if (ptr_to_struct->embargo)
                    {
                        ptr_to_struct->embargo--;
                        if (ptr_to_struct->embargo == 0)
                        {
                            effect = 1;
                            call_bc_move_exec(&embargo_end_bs);
                        }
                    }
                    break;
                case 19: //telekinesis
                    if (ptr_to_struct->telekinesis)
                    {
                        ptr_to_struct->telekinesis--;
                        if (ptr_to_struct->telekinesis == 0)
                        {
                            effect = 1;
                            call_bc_move_exec(&telekinesis_end_bs);
                        }
                    }
                    break;
                case 20: //healblock
                    if (ptr_to_struct->heal_block)
                    {
                        ptr_to_struct->heal_block--;
                        if (ptr_to_struct->heal_block == 0)
                        {
                            effect = 1;

                        }
                    }
                    break;
                case 21: //magnet rise
                    if (ptr_to_struct->magnet_rise)
                    {
                        ptr_to_struct->magnet_rise--;
                        if (ptr_to_struct->magnet_rise == 0)
                        {
                            effect = 1;
                            call_bc_move_exec(&magnetrise_end_bs);
                        }
                    }
                    break;
                case 22: //clear always hits flag
                    if (status3[active_bank].always_hits)
                    {
                        status3[active_bank].always_hits--;
                    }
                    break;
                case 23: //charge timer decrement
                    if (disable_structs[active_bank].charge_timer)
                    {
                        disable_structs[active_bank].charge_timer--;
                        if (disable_structs[active_bank].charge_timer == 0)
                            status3[active_bank].charged_up = 0;
                    }
                    break;
                case 24: //taunt
                    if (disable_structs[active_bank].taunt_timer)
                    {
                        disable_structs[active_bank].taunt_timer--;
                        if (disable_structs[active_bank].taunt_timer == 0)
                        {
                            effect = 1;
                            call_bc_move_exec(&tauntend2_bs);
                        }
                    }
                    break;
                case 25: //yawn
                    if (status3[active_bank].yawn)
                    {
                        status3[active_bank].yawn--;
                        if (status3[active_bank].yawn == 0 && !cant_fall_asleep(active_bank,1) && uproar_wakeup_check(active_bank) == 0)
                        {
                            reset_several_turns_stuff(active_bank);
                            effect = 1;
                            battle_participants[active_bank].status.flags.sleep = 2 + __umodsi3(rng(), 4); //2 + 0/1/2/3
                            prepare_setattributes_in_battle(0, REQUEST_STATUS_BATTLE, 0, 4, &battle_participants[active_bank].status);
                            mark_buffer_bank_for_execution(active_bank);
                            bank_partner_def = active_bank;
                            call_bc_move_exec((void*) 0x082DB378);
                        }
                    }
                case 26: //item effects again
                    if (item_battle_effects(1, active_bank, 1))
                        effect = 1;
                    break;
                case 27: //cheeck pouch again
                    if (berry_eaten(1, active_bank))
                        effect = 1;
                    break;
                case 28: //toxic/flame orb
                    if (item_battle_effects(5, active_bank, 0))
                        effect = 1;
                    break;
                case 29: //change roost type
                    switch (new_battlestruct.ptr->bank_affecting[active_bank].roost)
                    {
                    case 1:
                        battle_participants[active_bank].type1 = TYPE_FLYING;
                        break;
                    case 2:
                        battle_participants[active_bank].type2 = TYPE_FLYING;
                        break;
                    case 3:
                        new_battlestruct.ptr->bank_affecting[active_bank].type3 = TYPE_FLYING;
                        break;
                    case 4:
                        set_type(active_bank, TYPE_FLYING);
                        break;
                    }
                    break;

            }
            if (effect != 5) //check for uproar
                 *tracker += 1;
            #define TRACKER_MAX 30
            if (*tracker == TRACKER_MAX)
            {
                *tracker = 0;
                *bank +=1;
            }
            if (effect)
                return effect;
        }
    }

    hitmarker &= 0xFEFFFFDF;

    return effect;
}

u8 get_first_to_strike(u8 bank1, u8 bank2, u8 ignore_priority);

void move_to_buffer(u16 move)
{
    battle_text_buff1[0] = 0xFD;
    battle_text_buff1[1] = 2;
    battle_text_buff1[2] = move;
    battle_text_buff1[3] = move >> 8;
    battle_text_buff1[4] = 0xFF;
    return;
}

u8 update_turn_counters()
{
    u8 effect = 0;
    bank_attacker = 0;
    bank_target = 0;
    for (; bank_attacker < no_of_all_banks; bank_attacker++)
    {
        if (!(absent_bank_flags & bits_table[bank_attacker]))
            break;
    }
    for (; bank_target < no_of_all_banks; bank_target++)
    {
        if (!(absent_bank_flags & bits_table[bank_target]))
            break;
    }
    u8* statetracker = &battle_stuff_ptr.ptr->end_turn_statetracker2;
    u8* sidebank = &battle_stuff_ptr.ptr->field_DB;
    while (effect == 0)
    {
        switch (*statetracker)
        {
        case 0:
            if (new_battlestruct.ptr->field_affecting.trick_room)
            {
                new_battlestruct.ptr->field_affecting.trick_room--;
                if (new_battlestruct.ptr->field_affecting.trick_room == 0)
                {
                    effect = 1;
                    call_bc_move_exec(&trickroomends_bs);
                }
            }
            *statetracker +=1;
            break;
        case 1:
            if (new_battlestruct.ptr->field_affecting.magic_room)
            {
                new_battlestruct.ptr->field_affecting.magic_room--;
                if (new_battlestruct.ptr->field_affecting.magic_room == 0)
                {
                    effect = 1;
                    call_bc_move_exec(&magicroomends_bs);
                }
            }
            *statetracker +=1;
            break;
        case 2:
            if (new_battlestruct.ptr->field_affecting.wonder_room)
            {
                new_battlestruct.ptr->field_affecting.wonder_room--;
                if (new_battlestruct.ptr->field_affecting.wonder_room == 0)
                {
                    effect = 1;
                    call_bc_move_exec(&wonderoomends_bs);
                }
            }
            *statetracker +=1;
            break;
        case 3:
            if (new_battlestruct.ptr->field_affecting.gravity)
            {
                new_battlestruct.ptr->field_affecting.gravity--;
                if (new_battlestruct.ptr->field_affecting.gravity == 0)
                {
                    effect = 1;
                    call_bc_move_exec(&gravityends_bs);
                }
            }
            *statetracker +=1;
            break;
        case 4:
            if (new_battlestruct.ptr->field_affecting.grassy_terrain)
            {
                new_battlestruct.ptr->field_affecting.grassy_terrain--;
                if (new_battlestruct.ptr->field_affecting.grassy_terrain == 0)
                {
                    effect = 1;
                    call_bc_move_exec(&grassyterrainends_bs);
                }
                else
                {
                    effect = 1;
                    call_bc_move_exec(&grassyterrain_hpheal);
                    for (u8 i = 0; i < no_of_all_banks; i++)
                    {
                        new_battlestruct.ptr->bank_affecting[i].grassyterrain_heal = 0;
                    }
                }
            }
            *statetracker +=1;
            break;
        case 5:
            if (new_battlestruct.ptr->field_affecting.misty_terrain)
            {
                new_battlestruct.ptr->field_affecting.misty_terrain--;
                if (new_battlestruct.ptr->field_affecting.misty_terrain == 0)
                {
                    effect = 1;
                    call_bc_move_exec(&mistyterrainends_bs);
                }

            }
            *statetracker +=1;
            break;
        case 6:
            if (new_battlestruct.ptr->field_affecting.electic_terrain)
            {
                new_battlestruct.ptr->field_affecting.electic_terrain--;
                if (new_battlestruct.ptr->field_affecting.electic_terrain == 0)
                {
                    effect = 1;
                    call_bc_move_exec(&electrterrainends_bs);
                }
            }
            *statetracker +=1;
            break;
        case 7:
            if (new_battlestruct.ptr->field_affecting.ion_deluge)
                new_battlestruct.ptr->field_affecting.ion_deluge--;
            *statetracker +=1;
            break;
        case 8:
            for (u8 i = 0; i < no_of_all_banks; i++)
            {
                turn_order[i] = i;
            }
            u8 all_banks = no_of_all_banks - 1;
            for (s8 i2 = 0; i2 > all_banks; all_banks--)
            {
                s8 i3 = i2 + 1;
                while (i3 < no_of_all_banks)
                {
                    if (get_first_to_strike(turn_order[i2], turn_order[i3], 0))
                    {
                        sub_803CEDC(i2, i3);
                    }
                    i3++;
                }
            }
            *statetracker +=1;
            *sidebank = 0;
        case 9: //reflect
            while (*sidebank <= 1 && effect == 0)
            {
                if (side_affecting_halfword[*sidebank].reflect_on)
                {
                    side_timers[*sidebank].reflect_timer--;
                    if (side_timers[*sidebank].reflect_timer == 0)
                    {
                        side_affecting_halfword[*sidebank].reflect_on = 0;
                        bank_attacker = side_timers[*sidebank].reflect_bank;
                        effect = 1;
                        call_bc_move_exec((void*)0x82DACFA);
                        move_to_buffer(MOVE_REFLECT);
                        break;
                    }
                }
                *sidebank+=1;
            }
            if (effect == 0)
            {
                *sidebank = 0;
                *statetracker +=1;
            }
            break;
        case 10: //light screen
            while (*sidebank <= 1 && effect == 0)
            {
                if (side_affecting_halfword[*sidebank].light_screen_on)
                {
                    side_timers[*sidebank].lightscreen_timer--;
                    if (side_timers[*sidebank].lightscreen_timer == 0)
                    {
                        side_affecting_halfword[*sidebank].light_screen_on = 0;
                        bank_attacker = side_timers[*sidebank].lightscreen_bank;
                        effect = 1;
                        call_bc_move_exec((void*)0x82DACFA);
                        battle_communication_struct.multistring_chooser = *sidebank;
                        move_to_buffer(MOVE_LIGHT_SCREEN);
                        break;
                    }
                }
                *sidebank+=1;
            }
            if (effect == 0)
            {
                *sidebank = 0;
                *statetracker +=1;
            }
            break;
        case 11: //mist
            while (*sidebank <= 1 && effect == 0)
            {
                if (side_affecting_halfword[*sidebank].mist_on)
                {
                    side_timers[*sidebank].mist_timer--;
                    if (side_timers[*sidebank].mist_timer == 0)
                    {
                        side_affecting_halfword[*sidebank].mist_on = 0;
                        bank_attacker = active_bank = side_timers[*sidebank].mist_bank;
                        effect = 1;
                        call_bc_move_exec((void*)0x82DACFA);
                        battle_communication_struct.multistring_chooser = *sidebank;
                        move_to_buffer(MOVE_MIST);
                        break;
                    }
                }
                *sidebank+=1;
            }
            if (effect == 0)
            {
                *sidebank = 0;
                *statetracker +=1;
            }
            break;
        case 12: //safeguard
            while (*sidebank <= 1 && effect == 0)
            {
                if (side_affecting_halfword[*sidebank].safeguard_on)
                {
                    side_timers[*sidebank].safeguard_timer--;
                    if (side_timers[*sidebank].safeguard_timer == 0)
                    {
                        side_affecting_halfword[*sidebank].safeguard_on = 0;
                        bank_attacker = active_bank = side_timers[*sidebank].safeguard_bank;
                        effect = 1;
                        call_bc_move_exec((void*)0x82DAD0B);
                        battle_communication_struct.multistring_chooser = *sidebank;
                        move_to_buffer(MOVE_SAFEGUARD);
                        break;
                    }
                }
                *sidebank+=1;
            }
            if (effect == 0)
            {
                *sidebank = 0;
                *statetracker +=1;
            }
            break;
        case 13: //lucky chant
            while (*sidebank <= 1 && effect == 0)
            {
                if (new_battlestruct.ptr->side_affecting[*sidebank].lucky_chant)
                {
                    new_battlestruct.ptr->side_affecting[*sidebank].lucky_chant--;
                    if (new_battlestruct.ptr->side_affecting[*sidebank].lucky_chant == 0)
                    {
                        bank_attacker = active_bank = new_battlestruct.ptr->side_affecting[*sidebank].lucky_chant_bank;
                        effect = 1;
                        call_bc_move_exec((void*)0x82DACFA);
                        battle_communication_struct.multistring_chooser = *sidebank;
                        move_to_buffer(MOVE_LUCKY_CHANT);
                        break;
                    }
                }
                *sidebank+=1;
            }
            if (effect == 0)
            {
                *sidebank = 0;
                *statetracker +=1;
            }
            break;
        case 14: //tailwind
            while (*sidebank <= 1 && effect == 0)
            {
                if (new_battlestruct.ptr->side_affecting[*sidebank].tailwind)
                {
                    new_battlestruct.ptr->side_affecting[*sidebank].tailwind--;
                    if (new_battlestruct.ptr->side_affecting[*sidebank].tailwind == 0)
                    {
                        bank_attacker = active_bank = new_battlestruct.ptr->side_affecting[*sidebank].tailwind_bank;
                        effect = 1;
                        call_bc_move_exec((void*)0x82DACFA);
                        battle_communication_struct.multistring_chooser = *sidebank;
                        move_to_buffer(MOVE_TAILWIND);
                        break;
                    }
                }
                *sidebank+=1;
            }
            if (effect == 0)
            {
                *sidebank = 0;
                *statetracker +=1;
            }
            break;
        case 15: //wish
            while (*sidebank < no_of_all_banks && effect == 0)
            {
                if (battle_effects_duration.wish_duration[*sidebank])
                {
                    battle_effects_duration.wish_duration[*sidebank]--;
                    if (battle_effects_duration.wish_duration[*sidebank] == 0 && battle_participants[*sidebank].current_hp)
                    {
                        effect = 1;
                        bank_target = *sidebank;
                        call_bc_move_exec((void*)0x82DB0B7);
                        break;
                    }
                }
                *sidebank+=1;
            }
            if (effect == 0)
            {
                *sidebank = 0;
                *statetracker +=1;
            }
            break;
        case 16: //rain
            if (battle_weather.flags.rain || battle_weather.flags.downpour || battle_weather.flags.heavy_rain || battle_weather.flags.permament_rain)
            {
                effect = 1;
                battle_effects_duration.weather_dur--;
                if (battle_weather.flags.heavy_rain || battle_weather.flags.permament_rain || battle_effects_duration.weather_dur)
                {
                    if (battle_weather.flags.downpour)
                        *sidebank = 1;
                    else
                        *sidebank = 0;
                }
                else
                {
                    *sidebank = 2;
                    battle_weather.flags.downpour = 0;
                    battle_weather.flags.rain = 0;
                }

                call_bc_move_exec((void*)0x082DAC2C);
                battle_communication_struct.multistring_chooser = *sidebank;
            }
            *sidebank = 0;
            *statetracker += 1;
            break;
        case 17: //sun
            if (battle_weather.flags.sun || battle_weather.flags.permament_sun || battle_weather.flags.harsh_sun)
            {
                effect = 1;
                battle_effects_duration.weather_dur--;
                if (battle_weather.flags.permament_sun || battle_weather.flags.harsh_sun || battle_effects_duration.weather_dur)
                    call_bc_move_exec((void*)0x82DACD2);
                else
                {
                    battle_weather.flags.sun = 0;
                    call_bc_move_exec((void*)0x82DACE0);
                }
            }
            *statetracker += 1;
            break;
        case 18: //darude
            if (battle_weather.flags.sandstorm || battle_weather.flags.permament_sandstorm)
            {
                effect = 1;
                battle_effects_duration.weather_dur--;
                if (battle_weather.flags.permament_sandstorm || battle_effects_duration.weather_dur)
                    call_bc_move_exec((void*)0x82DAC47);
                else
                {
                    battle_weather.flags.sandstorm = 0;
                    call_bc_move_exec((void*)0x82DACC9);
                }
                battle_scripting.field10 = 0xC;
                battle_communication_struct.multistring_chooser = 0;
            }
            *statetracker += 1;
            break;
        case 19: //hail
            if (battle_weather.flags.hail || battle_weather.flags.permament_hail)
            {
                effect = 1;
                battle_effects_duration.weather_dur--;
                if (battle_weather.flags.permament_hail || battle_effects_duration.weather_dur)
                    call_bc_move_exec((void*)0x82DAC47);
                else
                {
                    battle_weather.flags.hail = 0;
                    call_bc_move_exec((void*)0x82DACC9);
                }
                battle_scripting.field10 = 0xD;
                battle_communication_struct.multistring_chooser = 1;
            }
            *statetracker += 1;
            break;
        case 20: //fog
            if (battle_weather.flags.fog || battle_weather.flags.permament_fog)
            {
                effect = 1;
                battle_effects_duration.weather_dur--;
                if (battle_weather.flags.permament_fog || battle_effects_duration.weather_dur)
                {
                    call_bc_move_exec(&fogcontinues_bs);
                    current_move = MOVE_HAZE;
                }
                else
                {
                    battle_weather.flags.fog = 0;
                    call_bc_move_exec(&fogends_bs);
                }
            }
            *statetracker += 1;
            break;
        }
        if (*statetracker > 20 && effect == 0)
            break;
    }
    return effect;
}

struct move_limitation{
    u8 limitation_no_move : 1; //0x1
    u8 limitation_no_pp : 1; //0x2
    u8 limitation_disabled : 1; //0x4
    u8 limitation_x8 : 1; //0x8
    u8 limitation_taunt : 1; //0x10
    u8 limitation_imprision : 1; //0x20
};

u8 check_move_limitations(u8 bank, u8 not_usable_moves, struct move_limitation limitations)
{
    struct battle_participant* checking_bank = &battle_participants[bank];
    u8 item_effect = get_item_effect(bank, 1);
    for (u8 i = 0; i < 4; i++)
    {
        u16 move_to_check = checking_bank->moves[i];
        if (move_to_check == 0 && limitations.limitation_no_move)
            not_usable_moves |= bits_table[i];
        else if (checking_bank->current_pp[i] == 0 && limitations.limitation_no_pp)
            not_usable_moves |= bits_table[i];
        else if (disable_structs[bank].disabled_move == move_to_check && limitations.limitation_disabled)
            not_usable_moves |= bits_table[i];
        else if (disable_structs[bank].taunt_timer && move_table[move_to_check].base_power == 0 && limitations.limitation_taunt)
            not_usable_moves |= bits_table[i];
        else if (status3[bank].imprision && check_if_imprisioned(bank, move_to_check) && limitations.limitation_imprision)
            not_usable_moves |= bits_table[i];
        else if (CHOICE_ITEM(item_effect) && battle_stuff_ptr.ptr->choiced_move[bank] && battle_stuff_ptr.ptr->choiced_move[bank] != 0xFFFF && battle_stuff_ptr.ptr->choiced_move[bank] != move_to_check)
            not_usable_moves |= bits_table[i];
        else if (item_effect == ITEM_EFFECT_ASSAULTVEST && move_table[move_to_check].split == 2)
            not_usable_moves |= bits_table[i];
        else if (disable_structs[bank].encore_timer && disable_structs[bank].encored_move != move_to_check)
            not_usable_moves |= bits_table[i];
        else if (checking_bank->status2.tormented && last_used_moves[bank] == move_to_check)
            not_usable_moves |= bits_table[i];
        else if (new_battlestruct.ptr->field_affecting.gravity && gravity_forbidden_move(move_to_check))
            not_usable_moves |= bits_table[i];
        else if (new_battlestruct.ptr->bank_affecting[bank].heal_block && healblock_forbidden_moves(move_to_check, 0))
            not_usable_moves |= bits_table[i];
        else if (new_battlestruct.ptr->bank_affecting[bank].embargo && embargo_forbidden_move(move_to_check))
            not_usable_moves |= bits_table[i];
    }
    return not_usable_moves;
}

u8 message_cant_choose_move()
{
    u8 bank = another_active_bank = active_bank;
    struct battle_participant* checking_bank = &battle_participants[bank];
    u8 move_index = battle_bufferB[bank].args[1];
    u16 checking_move = checking_bank->moves[move_index];
    void** loc_to_store_bs = (void*) 0x2024220 + bank * 4;
    u8 cant = 0;
    u8 item_effect = get_item_effect(bank, 1);
    if (checking_bank->current_pp[move_index] == 0)
    {
        cant = 1;
        *loc_to_store_bs = (void*) 0x82DB076;
    }
    else if (CHOICE_ITEM(item_effect) && battle_stuff_ptr.ptr->choiced_move[bank] && battle_stuff_ptr.ptr->choiced_move[bank] != 0xFFFF && battle_stuff_ptr.ptr->choiced_move[bank] != checking_move)
    {
        current_move = battle_stuff_ptr.ptr->choiced_move[bank];
        cant = 1;
        *loc_to_store_bs = (void*) 0x82DB812;
        last_used_item = checking_bank->held_item;
    }
    else if (item_effect == ITEM_EFFECT_ASSAULTVEST && move_table[checking_move].split == 2)
    {
        cant = 1;
        last_used_item = checking_bank->held_item;
        *loc_to_store_bs = &assaultvest_bs;
    }
    else if (status3[bank].imprision && check_if_imprisioned(bank, checking_move))
    {
        cant = 1;
        *loc_to_store_bs = (void*) 0x82DB181;
    }
    else if (disable_structs[bank].taunt_timer && move_table[checking_move].base_power == 0)
    {
        cant = 1;
        *loc_to_store_bs = (void*) 0x82DB0A0;
    }
    else if (checking_bank->status2.tormented && last_used_moves[bank] == checking_move && checking_move != MOVE_STRUGGLE)
    {
        cant = 1;
        reset_several_turns_stuff(bank);
        *loc_to_store_bs = (void*) 0x82DB089;
    }
    else if (disable_structs[bank].disabled_move == checking_move && disable_structs[bank].disable_timer)
    {
        cant = 1;
        current_move = checking_move;
        *loc_to_store_bs = (void*) 0x82DAE1F;
    }
    else if (new_battlestruct.ptr->field_affecting.gravity && gravity_forbidden_move(checking_move))
    {
        cant = 1;
        current_move = checking_move;
        *loc_to_store_bs = &gravityprevents2_bs;
    }
    else if (new_battlestruct.ptr->bank_affecting[bank].heal_block && healblock_forbidden_moves(checking_move, 0))
    {
        cant = 1;
        current_move = checking_move;
        *loc_to_store_bs = &cantselecthealblock_bs;
    }
    return cant;
}
