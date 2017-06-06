#include "defines.h"
#include "static_references.h"

u8 get_airborne_state(u8 bank, u8 mode, u8 check_levitate);
u8 can_lose_item(u8 bank, u8 stickyholdcheck, u8 sticky_message);
u8 is_item_a_plate(u16 item);
u16 get_item_extra_param(u16 item);
u8 ability_battle_effects(u8 switch_id, u8 bank, u8 ability_to_check, u8 special_cases_argument, u16 move);
u8 get_item_effect(u8 bank, u8 check_negating_effects);
u8 has_ability_effect(u8 bank, u8 mold_breaker);
s8 get_move_position(u8 bank, u16 move);
u8 weather_abilities_effect();
u8 is_of_type(u8 bank, u8 type);
bool check_ability(u8 bank, u8 ability);
u8 is_bank_present(u8 bank);
void move_to_buff1(u16 move);
u8 get_bank_side(u8 bank);
void bs_push_current(void* now);

struct natural_gift{
    u8 move_power;
    u8 move_type;
};

const struct natural_gift natural_gift_table[] = { {0xFF, 0} ,{80, TYPE_FIRE}, {80, TYPE_WATER}, {80, TYPE_ELECTRIC}, {80, TYPE_GRASS}, {80, TYPE_ICE}, {80, TYPE_FIGHTING}, {80, TYPE_POISON}, {80, TYPE_GROUND}, {80, TYPE_FLYING}, {80, TYPE_PSYCHIC}, {80, TYPE_BUG}, {80, TYPE_ROCK}, {80, TYPE_GHOST}, {80, TYPE_DRAGON}, {80, TYPE_DARK}, {80, TYPE_STEEL}, {90, TYPE_FIRE}, {90, TYPE_WATER}, {90, TYPE_ELECTRIC}, {90, TYPE_GRASS}, {90, TYPE_ICE}, {90, TYPE_FIGHTING}, {90, TYPE_POISON}, {90, TYPE_GROUND}, {90, TYPE_FLYING}, {90, TYPE_PSYCHIC}, {90, TYPE_BUG}, {90, TYPE_ROCK}, {90, TYPE_GHOST}, {90, TYPE_DRAGON}, {90, TYPE_DARK}, {90, TYPE_STEEL}, {100, TYPE_FIRE}, {100, TYPE_WATER}, {100, TYPE_ELECTRIC}, {100, TYPE_FIGHTING}, {100, TYPE_POISON}, {100, TYPE_GROUND}, {100, TYPE_FLYING}, {100, TYPE_PSYCHIC}, {100, TYPE_BUG} };

struct fling{
    u16 item_id;
    u8 move_power;
    u8 move_effect;
};

const struct fling fling_table[] = { {ITEM_CHOICEBAND, 10, 0}, {ITEM_BRIGHTPOWDER, 10, 0}, {ITEM_FOCUSBAND, 10, 0}, {ITEM_LEFTOVERS, 10, 0}, {ITEM_MENTALHERB, 10, 0}, {ITEM_METALPOWDER, 10, 0}, {ITEM_SILKSCARF, 10, 0}, {ITEM_SILVERPOWDER, 10, 0}, {ITEM_SOFTSAND, 10, 0}, {ITEM_SOOTHEBELL, 10, 0}, {ITEM_WHITEHERB, 10, 0}, {ITEM_ANTIDOTE, 30, 0}, {ITEM_PARLYZHEAL, 30, 0}, {ITEM_AWAKENING, 30, 0}, {ITEM_BURNHEAL, 30, 0}, {ITEM_ICEHEAL, 30, 0}, {ITEM_FULLHEAL, 30, 0}, {ITEM_FULLRESTORE, 30, 0}, {ITEM_LAVACOOKIE, 30, 0}, {ITEM_POTION, 30, 0}, {ITEM_SUPERPOTION, 30, 0}, {ITEM_HYPERPOTION, 30, 0}, {ITEM_MAXPOTION, 30, 0}, {ITEM_HEALPOWDER, 30, 0}, {ITEM_ENERGYPOWDER, 30, 0}, {ITEM_ENERGYROOT, 30, 0}, {ITEM_REVIVALHERB, 30, 0}, {ITEM_REVIVE, 30, 0}, {ITEM_MAXREVIVE, 30, 0}, {ITEM_SODAPOP, 30, 0}, {ITEM_LEMONADE, 30, 0}, {ITEM_SODAPOP, 30, 0}, {ITEM_MOOMOOMILK, 30, 0}, {ITEM_BERRYJUICE, 30, 0}, {ITEM_ETHER, 30, 0}, {ITEM_MAXETHER, 30, 0}, {ITEM_ELIXIR, 30, 0}, {ITEM_MAXELIXIR, 30, 0}, {ITEM_REPEL, 30, 0}, {ITEM_SUPERREPEL, 30, 0}, {ITEM_MAXREPEL, 30, 0}, {ITEM_PROTEIN, 30, 0}, {ITEM_ZINC, 30, 0}, {ITEM_CALCIUM, 30, 0}, {ITEM_CARBOS, 30, 0}, {ITEM_HPUP, 30, 0}, {ITEM_PPUP, 30, 0}, {ITEM_PPMAX, 30, 0}, {ITEM_REDSHARD, 30, 0}, {ITEM_BLUESHARD, 30, 0}, {ITEM_YELLOWSHARD, 30, 0}, {ITEM_GREENSHARD, 30, 0}, {ITEM_XACCURACY, 30, 0}, {ITEM_XDEFEND, 30, 0}, {ITEM_XSPEED, 30, 0}, {ITEM_XATTACK, 30, 0}, {ITEM_XSPECIAL, 30, 0}, {ITEM_YELLOWFLUTE, 30, 0}, {ITEM_BLACKFLUTE, 30, 0}, {ITEM_WHITEFLUTE, 30, 0}, {ITEM_REDFLUTE, 30, 0}, {ITEM_BLUEFLUTE, 30, 0}, {ITEM_AMULETCOIN, 30, 0}, {ITEM_BIGMUSHROOM, 30, 0}, {ITEM_BIGPEARL, 30, 0}, {ITEM_BLACKBELT, 30, 0}, {ITEM_CHARCOAL, 30, 0}, {ITEM_CLEANSETAG, 30, 0}, {ITEM_DEEPSEASCALE, 30, 0}, {ITEM_DRAGONSCALE, 30, 0}, {ITEM_ESCAPEROPE, 30, 0}, {ITEM_EVERSTONE, 30, 0}, {ITEM_EXPSHARE, 30, 0}, {ITEM_FIRESTONE, 30, 0}, {ITEM_FLUFFYTAIL, 30, 0}, {ITEM_HEARTSCALE, 30, 0}, {ITEM_KINGSROCK, 30, 0}, {ITEM_LEAFSTONE, 30, 0}, {ITEM_LIGHTBALL, 30, 0}, {ITEM_LUCKYEGG, 30, 0}, {ITEM_MAGNET, 30, 0}, {ITEM_METALCOAT, 30, 0}, {ITEM_MIRACLESEED, 30, 0}, {ITEM_MOONSTONE, 30, 0}, {ITEM_MYSTICWATER, 30, 0}, {ITEM_NEVERMELTICE, 30, 0}, {ITEM_NUGGET, 30, 0}, {ITEM_PEARL, 30, 0}, {ITEM_POKEDOLL, 30, 0}, {ITEM_SACREDASH, 30, 0}, {ITEM_SCOPELENS, 30, 0}, {ITEM_SHELLBELL, 30, 0}, {ITEM_SHOALSALT, 30, 0}, {ITEM_SHOALSHELL, 30, 0}, {ITEM_SMOKEBALL, 30, 0}, {ITEM_SOULDEW, 30, 0}, {ITEM_SPELLTAG, 30, 0}, {ITEM_STARDUST, 30, 0}, {ITEM_STARPIECE, 30, 0}, {ITEM_SUNSTONE, 30, 0}, {ITEM_THUNDERSTONE, 30, 0}, {ITEM_TINYMUSHROOM, 30, 0}, {ITEM_TWISTEDSPOON, 30, 0}, {ITEM_UPGRADE, 30, 0}, {ITEM_WATERSTONE, 30, 0}, {ITEM_LUCKYPUNCH, 40, 0}, {ITEM_SHARPBEAK, 50, 0}, {ITEM_MACHOBRACE, 60, 0}, {ITEM_STICK, 60, 0}, {ITEM_DRAGONFANG, 70, 0}, {ITEM_POISONBARB, 70, 0}, {ITEM_QUICKCLAW, 80, 0}, {ITEM_DEEPSEATOOTH, 90, 0}, {ITEM_THICKCLUB, 90, 0}, {ITEM_DOMEFOSSIL, 100, 0}, {ITEM_HELIXFOSSIL, 100, 0}, {ITEM_OLDAMBER, 100, 0}, {ITEM_ROOTFOSSIL, 100, 0}, {ITEM_CLAWFOSSIL, 100, 0}, {0xFFFF, 0, 0} };

bool can_evolve(u16 species)
{
    const struct evolution_sub* evo = GET_EVO_TABLE(species);
    for (u8 i = 0; i < NUM_OF_EVOS; i++)
    {
        u8 method = evo[i].method;
        if (method != 0 && method < 0xFA)
            return true;
    }
    return false;
}

u32 percent_boost(u32 number, u16 percent)
{
    return number * (100 + percent) / 100;
}

u32 percent_lose(u32 number, u16 percent)
{
    return number * (100 - percent) / 100;
}

u16 chain_modifier(u16 curr_modifier, u16 new_modifier)
{
    u16 updated_modifier;
    if (curr_modifier == 0)
    {
        updated_modifier = new_modifier;
    }
    else
    {
        u32 calculations = (curr_modifier * new_modifier + 0x800) >> 12;
        updated_modifier = calculations;
    }
    return updated_modifier;
}

u32 apply_modifier(u16 modifier, u32 value)
{
    u32 multiplication = modifier * value;
    u32 anding_result = 0xFFF & multiplication;
    u32 new_value = multiplication >> 0xC;
    if (anding_result > 0x800)
    {
        new_value++;
    }
    return new_value;
}

u16 percent_to_modifier(u8 percent) //20 gives exactly 0x1333, 30 is short on 1
{
    return 0x1000 + (percent * 819 / 20);
}

u16 apply_statboost(u16 stat, u8 boost)
{
    static const struct stat_fractions stat_buffs[13] = { {2, 8}, {2, 7}, {2, 6}, {2, 5}, {2, 4}, {2, 3}, {2, 2}, {3, 2}, {4, 2}, {5, 2}, {6, 2}, {7, 2}, {8, 2} };
    return stat * stat_buffs[boost].dividend / stat_buffs[boost].divisor;
}

u16 get_poke_weight(u8 bank)
{
    u16 poke_weight = get_height_or_weight(species_to_national_dex(battle_participants[bank].species), 1);
    if (has_ability_effect(bank, 1))
    {
        switch (battle_participants[bank].ability_id)
        {
            case ABILITY_HEAVY_METAL:
                poke_weight *= 2;
                break;
            case ABILITY_LIGHT_METAL:
                poke_weight /= 2;
                break;
        }
    }
    if (get_item_effect(bank, 1) == ITEM_EFFECT_FLOATSTONE)
        poke_weight /= 2;

    s16 to_sub = 1000 * new_battlestruct->bank_affecting[bank].autonomize_uses;
    poke_weight -= to_sub;
    if (poke_weight < 1)
        poke_weight = 1;

    return poke_weight;
}

u8 count_stat_increases(u8 bank, u8 eva_acc)
{
    u8* atk_ptr = &battle_participants[bank].atk_buff;
    u8 increases = 0;
    for (u8 i = 0; i < 7; i++)
    {
        u8* stat_ptr = atk_ptr + i;
        if (*stat_ptr > 6)
        {
            if (i <= 5 || (i > 5 && eva_acc))
                increases += (*stat_ptr - 6);
        }
    }
    return increases;
}

u16 get_speed(u8 bank)
{
    u16 speed = battle_participants[bank].spd;
    //take items into account
    switch (get_item_effect(bank, 1))
    {
    case ITEM_EFFECT_IRONBALL:
        speed >>= 1;
        break;
    case ITEM_EFFECT_CHOICESCARF:
        speed = percent_boost(speed, 50);
        break;
     case ITEM_EFFECT_QUICKPOWDER:
        if (battle_participants[bank].species == POKE_DITTO && !battle_participants[bank].status2.transformed)
            speed <<= 1;
        break;
    }
    //take abilities into account
    if (has_ability_effect(bank, 0))
    {
        u8 weather_effect = weather_abilities_effect();
        switch (battle_participants[bank].ability_id)
        {
        case ABILITY_CHLOROPHYLL:
            if (weather_effect && (battle_weather.flags.harsh_sun || battle_weather.flags.permament_sun || battle_weather.flags.sun))
                speed *= 2;
            break;
        case ABILITY_SWIFT_SWIM:
            if (weather_effect && (battle_weather.flags.rain || battle_weather.flags.downpour || battle_weather.flags.permament_rain || battle_weather.flags.heavy_rain))
                speed *= 2;
            break;
        case ABILITY_SAND_RUSH:
            if (weather_effect && (battle_weather.flags.sandstorm || battle_weather.flags.permament_sandstorm))
                speed *= 2;
            break;
        case ABILITY_SLUSH_RUSH:
            if (weather_effect && (battle_weather.flags.hail || battle_weather.flags.permament_hail))
                speed *= 2;
            break;
        case ABILITY_SURGE_SURFER:
            if (new_battlestruct->field_affecting.electic_terrain)
                speed *= 2;
            break;
        case ABILITY_QUICK_FEET:
            if (battle_participants[bank].status.flags.poison || battle_participants[bank].status.flags.toxic_poison || battle_participants[bank].status.flags.burn)
                speed *= 2;
            else if (battle_participants[bank].status.flags.paralysis)
                speed *= 4;
            break;
        }
    }
    //tailwind
    if (new_battlestruct->side_affecting[get_bank_side(bank)].tailwind)
        speed *= 2;
    //unburden
    if (status3[bank].unburden)
        speed *= 2;
    //swamp
    if (new_battlestruct->side_affecting[get_bank_side(bank)].swamp_spd_reduce)
        speed /= 4;
    //paralysis
    if (battle_participants[bank].status.flags.paralysis)
	#if GENVII_PARALYSIS_SPEED_REDUCTION == true
        speed /= 2;
	#else
		speed /= 4;
	#endif
    speed = apply_statboost(speed, battle_participants[bank].spd_buff);

    return speed;
}

u16 get_base_power(u16 move, u8 atk_bank, u8 def_bank)
{
    u16 base_power = move_table[move].base_power;
    u8 atk_ally_bank = atk_bank ^ 2;
    switch (move)
    {
        case MOVE_GRASS_PLEDGE:
        case MOVE_FIRE_PLEDGE:
        case MOVE_WATER_PLEDGE:
            if (new_battlestruct->side_affecting[atk_bank&1].pledge_effect)
            {
                base_power = 150;
            }
            break;
        case MOVE_FLING: //make fling table; before calling damage calc should check if can use this, move effect is applied here
            {            //item deletion happens after damage calculation
                u8* effect = &battle_communication_struct.move_effect;
                u16 item = battle_participants[atk_bank].held_item;
                //check if it's a berry
                if (get_item_pocket_id(item) == 4)
                {
                    base_power = 10;
                    *effect = 0x38; //target gets the berry effect
                }
                //check if it's a plate
                else if (is_item_a_plate(item))
                    base_power = 90;
                else
                {
                    for (u16 i = 0; fling_table[i].item_id != 0xFFFF; i++)
                    {
                        if (fling_table[i].item_id == item)
                        {
                            base_power = fling_table[i].move_power;
                            *effect = fling_table[i].move_effect;
                            break;
                        }
                    }
                }
                break;
            }
        case MOVE_ERUPTION:
        case MOVE_WATER_SPOUT:
            base_power = base_power * battle_participants[atk_bank].current_hp / battle_participants[atk_bank].max_hp;
            break;
        case MOVE_FLAIL:
        case MOVE_REVERSAL:
            {
                u32 P = 48 * battle_participants[atk_bank].current_hp / battle_participants[atk_bank].max_hp;
                if (P <= 1)
                    base_power = 200;
                else if (P >= 2 && P <= 4)
                    base_power = 150;
                else if (5 <= P && P <= 9)
                    base_power = 100;
                else if (10 <= P && P <= 16)
                    base_power = 80;
                else if (17 <= P && P <= 32)
                    base_power = 40;
                else
                    base_power = 20;
            }
            break;
        case MOVE_RETURN:
            {
                u32 return_damage = battle_participants[atk_bank].happiness * 10 / 25;
                if (return_damage == 0)
                    return_damage = 1;
                base_power = return_damage;
            }
            break;
        case MOVE_FRUSTRATION:
            {
                u32 frustration_damage = (255 - battle_participants[atk_bank].happiness) * 10 / 25;
                if (frustration_damage == 0)
                    frustration_damage = 1;
                base_power = frustration_damage;
            }
            break;
        case MOVE_FURY_CUTTER:
        case MOVE_ROLLOUT:
        case MOVE_MAGNITUDE:
        case MOVE_PRESENT:
        case MOVE_TRIPLE_KICK:
            if (dynamic_base_power)
            {
                base_power = dynamic_base_power;
            }
            break;
        case MOVE_SPIT_UP:
            base_power = 100 * disable_structs[atk_bank].stockpile_counter;
            break;
        case MOVE_REVENGE:
        case MOVE_AVALANCHE:
            {
                struct protect_struct* protect_str = &protect_structs[bank_attacker];
                if ((protect_str->physical_damage && protect_structs->counter_target == bank_target) || (protect_str->special_damage && protect_structs->mirrorcoat_target == bank_target))
                    base_power *= 2;
            }
            break;
        case MOVE_WEATHER_BALL:
        case MOVE_PURSUIT:
            if (battle_scripting.damage_multiplier)
                base_power *= battle_scripting.damage_multiplier;
            break;
        case MOVE_NATURAL_GIFT: //checking for held item and the capability of using an item should happen before damage calculation
            {                   //dynamic type will be set here
                s8 berryID = itemid_to_berryid(battle_participants[atk_bank].held_item);
                base_power = natural_gift_table[berryID].move_power;
                battle_stuff_ptr->dynamic_move_type = natural_gift_table[berryID].move_type + 0x80;
                if ((new_battlestruct->field_affecting.ion_deluge || new_battlestruct->bank_affecting[atk_bank].electrify) && battle_stuff_ptr->dynamic_move_type == 0x80)
                    battle_stuff_ptr->dynamic_move_type = TYPE_ELECTRIC + 0x80;
            }
            break;
        case MOVE_WAKEUP_SLAP:
            if (battle_participants[def_bank].status.flags.sleep)
            {
                base_power *= 2;
            }
            break;
        case MOVE_SMELLING_SALTS:
            if (battle_participants[def_bank].status.flags.paralysis)
            {
                base_power *= 2;
            }
            break;
        case MOVE_WRING_OUT:
        case MOVE_CRUSH_GRIP:
            base_power = ATLEAST_ONE(120 * battle_participants[def_bank].current_hp / battle_participants[def_bank].max_hp);
            break;
        case MOVE_HEX:
            if (battle_participants[def_bank].status.int_status)
            {
                base_power *= 2;
            }
            break;
        case MOVE_ASSURANCE:
            if (special_statuses[def_bank].moveturn_losthp_physical || special_statuses[def_bank].moveturn_losthp_special)
            {
                base_power *= 2;
            }
            break;
        case MOVE_TRUMP_CARD:
            {
                s8 pp_slot = get_move_position(atk_bank, MOVE_TRUMP_CARD);
                u8 pp;
                if (pp_slot == -1)
                {
                    pp = 4;
                }
                else
                {
                    pp = battle_participants[atk_bank].current_pp[pp_slot];
                }
                switch (pp)
                {
                case 0:
                    base_power = 200;
                    break;
                case 1:
                    base_power = 80;
                    break;
                case 2:
                    base_power = 60;
                    break;
                case 3:
                    base_power = 50;
                    break;
                default:
                    base_power = 40;
                    break;
                }
                break;
            }
        case MOVE_ACROBATICS:
            if (!battle_participants[atk_bank].held_item)
            {
                base_power *= 2;
            }
            break;
        case MOVE_LOW_KICK:
        case MOVE_GRASS_KNOT:
            {
                u16 defender_weight = get_poke_weight(def_bank);
                if (defender_weight < 100)
                    base_power = 20;
                else if (defender_weight < 250)
                    base_power = 40;
                else if (defender_weight < 500)
                    base_power = 60;
                else if (defender_weight < 1000)
                    base_power = 80;
                else if (defender_weight < 2000)
                    base_power = 100;
                else
                    base_power = 120;
                break;
            }
        case MOVE_HEAT_CRASH:
        case MOVE_HEAVY_SLAM:
            {
                u16 weight_difference = get_poke_weight(atk_bank) / get_poke_weight(def_bank);

                if (weight_difference >= 5)
                    base_power = 120;
                else if (weight_difference == 4)
                    base_power = 100;
                else if (weight_difference == 3)
                    base_power = 80;
                else if (weight_difference == 2)
                    base_power = 60;
                else
                    base_power = 40;
                break;
            }
        case MOVE_PUNISHMENT:
            base_power = 60 + 20 * count_stat_increases(def_bank, 0);
            if (base_power > 200)
                base_power = 200;
            break;
        case MOVE_STORED_POWER:
            base_power = base_power + 20 * count_stat_increases(atk_bank, 1);
            break;
        case MOVE_ELECTRO_BALL:
            switch (get_speed(atk_bank) / get_speed(def_bank))
                {
                case 0:
                    base_power = 40;
                    break;
                case 1:
                    base_power = 60;
                    break;
                case 2:
                    base_power = 80;
                    break;
                case 3:
                    base_power = 120;
                    break;
                default:
                    base_power = 150;
                    break;
                }
            break;
        case MOVE_GYRO_BALL:
            base_power = (25 * get_speed(def_bank) / get_speed(atk_bank)) + 1;
            if (base_power > 150)
                base_power = 150;
            break;
        case MOVE_ECHOED_VOICE:
            base_power += 40 * new_battlestruct->field_affecting.echo_voice_counter;
            if (base_power > 200)
                base_power = 200;
            break;
        case MOVE_PAYBACK:
            if (get_bank_turn_order(def_bank) < turn_order[atk_bank])
            {
                base_power *= 2;
            }
            break;
        case MOVE_GUST:
        case MOVE_TWISTER:
            if (new_battlestruct->bank_affecting[def_bank].sky_drop_attacker || new_battlestruct->bank_affecting[def_bank].sky_drop_target || status3[def_bank].on_air)
            {
                base_power *= 2;
            }
            break;
        case MOVE_ROUND:
            if (chosen_move_by_banks[atk_ally_bank] == MOVE_ROUND)
            {
                base_power *= 2;
            }
            break;
        case MOVE_BEAT_UP:
            {
                struct pokemon* poke;
                if (get_bank_side(bank_attacker))
                    poke = &party_opponent[0];
                else
                    poke = &party_player[0];
                u8 poke_index = (u8) (0x02024480);
                if (get_attributes(&poke[poke_index], ATTR_CURRENT_HP, 0) == 0)
                {
                    for (u8 i = 0; i < 6; i ++)
                    {
                        if (i > poke_index && get_attributes(&poke[i], ATTR_CURRENT_HP, 0))
                            poke_index = i;
                    }
                }
                u16 species = get_attributes(&poke[poke_index], ATTR_SPECIES, 0);
                base_power = (*basestat_table)[species].base_atk / 10 + 5;
            }
            break;
    }
    curr_move_BP = base_power;
    return base_power;
}

bool find_move_in_table(u16 move, const u16* table_ptr)
{
    for (u32 i = 0; table_ptr[i] != 0xFFFF; i++)
    {
        if (table_ptr[i] == move) {return true;}
    }
    return false;
}

bool does_move_make_contact(u16 move, u8 atk_bank)
{
    if (move_table[move].move_flags.flags.makes_contact && !check_ability(atk_bank, ABILITY_LONG_REACH) && get_item_effect(atk_bank, 1) != ITEM_EFFECT_PROTECTIVEPADS)
        return 1;
    return 0;
}

u16 apply_base_power_modifiers(u16 move, u8 move_type, u8 atk_bank, u8 def_bank, u16 base_power)
{
    u16 modifier = 0x1000;
    u8 move_split = move_table[move].split;
    u16 quality_atk_modifier = percent_to_modifier(get_item_quality(battle_participants[atk_bank].held_item));
    if (has_ability_effect(atk_bank, 0))
    {
        switch (battle_participants[atk_bank].ability_id)
        {
        case ABILITY_TECHNICIAN:
            if (base_power <= 60)
            {
                modifier = chain_modifier(modifier, 0x1800);
            }
            break;
        case ABILITY_FLARE_BOOST:
            if (battle_participants[atk_bank].status.flags.burn && move_split == MOVE_SPECIAL)
            {
                modifier = chain_modifier(modifier, 0x1800);
            }
            break;
        case ABILITY_TOXIC_BOOST:
            if ((battle_participants[atk_bank].status.flags.toxic_poison || battle_participants[atk_bank].status.flags.poison) && move_split == MOVE_PHYSICAL)
            {
                modifier = chain_modifier(modifier, 0x1800);
            }
            break;
        case ABILITY_RECKLESS:
            if (find_move_in_table(move, reckless_moves_table))
            {
                modifier = chain_modifier(modifier, 0x1333);
            }
            break;
        case ABILITY_IRON_FIST:
            if (find_move_in_table(move, &ironfist_moves_table[0]))
            {
                modifier = chain_modifier(modifier, 0x1333);
            }
            break;
        case ABILITY_SHEER_FORCE:
            if (find_move_in_table(move, &sheerforce_moves_table[0]))
            {
                modifier = chain_modifier(modifier, 0x14CD);
                new_battlestruct->various.sheerforce_bonus = 1;
            }
            break;
        case ABILITY_SAND_FORCE:
            if (move_type == TYPE_STEEL || move_type == TYPE_ROCK || move_type == TYPE_GROUND)
            {
                modifier = chain_modifier(modifier, 0x14CD);
            }
            break;
        case ABILITY_RIVALRY:
            {
                u8 attacker_gender = gender_from_pid(battle_participants[atk_bank].species, battle_participants[atk_bank].pid);
                u8 target_gender = gender_from_pid(battle_participants[def_bank].species, battle_participants[def_bank].pid);
                if (attacker_gender != 0xFF && target_gender != 0xFF)
                {
                    if (attacker_gender == target_gender)
                    {
                        modifier = chain_modifier(modifier, 0x1400);
                    }
                    else
                    {
                        modifier = chain_modifier(modifier, 0xC00);
                    }
                }
                break;
            }
        case ABILITY_ANALYTIC:
            if (get_bank_turn_order(def_bank) < turn_order[atk_bank] && move != MOVE_FUTURE_SIGHT && move != MOVE_DOOM_DESIRE)
            {
                modifier = chain_modifier(modifier, 0x14CD);
            }
            break;
        case ABILITY_TOUGH_CLAWS:
            if (move_table[move].move_flags.flags.makes_contact)
            {
                modifier = chain_modifier(modifier, 0x14CD);
            }
            break;
        case ABILITY_STRONG_JAW:
            if (find_move_in_table(move, &biting_moves_table[0]))
            {
                modifier = chain_modifier(modifier, 0x1800);
            }
            break;
        case ABILITY_MEGA_LAUNCHER:
            if (find_move_in_table(move, &megalauncher_moves_table[0]))
            {
                modifier = chain_modifier(modifier, 0x1800);
            }
            break;
        case ABILITY_WATER_BUBBLE:
            if (move_type == TYPE_WATER)
            {
                modifier = chain_modifier(modifier, 0x2000);
            }
            break;
        case ABILITY_STEELWORKER:
            if (move_type == TYPE_STEEL)
            {
                modifier = chain_modifier(modifier, 0x1800);
            }
            break;
        }
    }

    if ((ability_battle_effects(19, 0, ABILITY_DARK_AURA, 0, 0) && move_type == TYPE_DARK) || (ability_battle_effects(19, 0, ABILITY_FAIRY_AURA, 0, 0) && move_type == TYPE_FAIRY))
    {
        if (ability_battle_effects(19, 0, ABILITY_AURA_BREAK, 0, 0))
        {
            modifier = chain_modifier(modifier, 0xC00);
        }
        else
        {
            modifier = chain_modifier(modifier, 0x1547);
        }
    }
    //check partner abilities
    u8 atk_partner = atk_bank ^ 2;
    if (is_bank_present(atk_partner) && has_ability_effect(atk_partner, 0))
    {
        switch (battle_participants[atk_partner].ability_id)
        {
        case ABILITY_BATTERY:
            if (move_split == MOVE_SPECIAL)
                modifier = chain_modifier(modifier, 0x14CD);
            break;
        }
    }

    //check target abilities
    if (has_ability_effect(def_bank, 1))
    {
        switch (battle_participants[def_bank].ability_id)
        {
        case ABILITY_HEATPROOF:
        case ABILITY_WATER_BUBBLE:
            if (move_type == TYPE_FIRE)
                modifier = chain_modifier(modifier, 0x800);
            break;
        case ABILITY_DRY_SKIN:
            if (move_type == TYPE_FIRE)
                modifier = chain_modifier(modifier, 0x1400);
            break;
        case ABILITY_FLUFFY:
            if (does_move_make_contact(move, atk_bank))
                modifier = chain_modifier(modifier, 0x800);
            if (move_type == TYPE_FIRE)
                modifier = chain_modifier(modifier, 0x2000);
            break;
        }
    }
    switch (get_item_effect(atk_bank, 1))
    {
    case ITEM_EFFECT_NOEFFECT:
        if (new_battlestruct->various.gem_boost)
        {
            new_battlestruct->various.gem_boost = 0;
            modifier = chain_modifier(modifier, 0x14CD);
        }
        break;
    case ITEM_EFFECT_MUSCLEBAND:
        if (move_split == MOVE_PHYSICAL)
        {
            modifier = chain_modifier(modifier, 0x1199);
        }
        break;
    case ITEM_EFFECT_WISEGLASSES:
        if (move_split == MOVE_SPECIAL)
        {
            modifier = chain_modifier(modifier, 0x1199);
        }
        break;
    case ITEM_EFFECT_LUSTROUSORB:
        if ((move_type == TYPE_WATER || move_type == TYPE_DRAGON) && battle_participants[atk_bank].species == POKE_PALKIA)
        {
            modifier = chain_modifier(modifier, 0x1333);
        }
        break;
    case ITEM_EFFECT_ADAMANTORB:
        if ((move_type == TYPE_STEEL || move_type == TYPE_DRAGON) && battle_participants[atk_bank].species == POKE_DIALGA)
        {
            modifier = chain_modifier(modifier, 0x1333);
        }
        break;
    case ITEM_EFFECT_GRISEOUSORB:
        if ((move_type == TYPE_GHOST || move_type == TYPE_DRAGON) && (battle_participants[atk_bank].species == POKE_GIRATINA))
        {
            modifier = chain_modifier(modifier, 0x1333);
        }
        break;
    case ITEM_EFFECT_PINKRIBBON:
        if (move_type == TYPE_FAIRY)
        {
            modifier = chain_modifier(modifier, quality_atk_modifier);
        }
        break;
    case ITEM_EFFECT_SILKSCARF:
        if (move_type == TYPE_NORMAL)
        {
            modifier = chain_modifier(modifier, quality_atk_modifier);
        }
        break;
    case ITEM_EFFECT_SHARPBEAK:
        if (move_type == TYPE_FLYING)
        {
            modifier = chain_modifier(modifier, quality_atk_modifier);
        }
        break;
    case ITEM_EFFECT_BLACKBELT:
        if (move_type == TYPE_FIGHTING)
        {
            modifier = chain_modifier(modifier, quality_atk_modifier);
        }
        break;
    case ITEM_EFFECT_SOFTSAND:
        if (move_type == TYPE_GROUND)
        {
            modifier = chain_modifier(modifier, quality_atk_modifier);
        }
        break;
    case ITEM_EFFECT_HARDSTONE:
        if (move_type == TYPE_ROCK)
        {
            modifier = chain_modifier(modifier, quality_atk_modifier);
        }
        break;
    case ITEM_EFFECT_MAGNET:
        if (move_type == TYPE_ELECTRIC)
        {
            modifier = chain_modifier(modifier, quality_atk_modifier);
        }
        break;
    case ITEM_EFFECT_NEVERMELTICE:
        if (move_type == TYPE_ICE)
        {
            modifier = chain_modifier(modifier, quality_atk_modifier);
        }
        break;
    case ITEM_EFFECT_BLACKGLASSES:
        if (move_type == TYPE_DARK)
        {
            modifier = chain_modifier(modifier, quality_atk_modifier);
        }
        break;
    case ITEM_EFFECT_SILVERPOWDER:
        if (move_type == TYPE_BUG)
        {
            modifier = chain_modifier(modifier, quality_atk_modifier);
        }
        break;
    case ITEM_EFFECT_SPELLTAG:
        if (move_type == TYPE_GHOST)
        {
            modifier = chain_modifier(modifier, quality_atk_modifier);
        }
        break;
    case ITEM_EFFECT_DRAGONFANG:
        if (move_type == TYPE_DRAGON)
        {
            modifier = chain_modifier(modifier, quality_atk_modifier);
        }
        break;
    case ITEM_EFFECT_MYSTICWATER:
        if (move_type == TYPE_WATER)
        {
            modifier = chain_modifier(modifier, quality_atk_modifier);
        }
        break;
    case ITEM_EFFECT_CHARCOAL:
        if (move_type == TYPE_FIRE)
        {
            modifier = chain_modifier(modifier, quality_atk_modifier);
        }
        break;
    case ITEM_EFFECT_MIRACLESEED:
        if (move_type == TYPE_GRASS)
        {
            modifier = chain_modifier(modifier, quality_atk_modifier);
        }
        break;
    case ITEM_EFFECT_TWISTEDSPOON:
        if (move_type == TYPE_PSYCHIC)
        {
            modifier = chain_modifier(modifier, quality_atk_modifier);
        }
        break;
    case ITEM_EFFECT_METALCOAT:
        if (move_type == TYPE_STEEL)
        {
            modifier = chain_modifier(modifier, quality_atk_modifier);
        }
        break;
    case ITEM_EFFECT_POISONBARB:
        if (move_type == TYPE_POISON)
        {
            modifier = chain_modifier(modifier, quality_atk_modifier);
        }
        break;
    case ITEM_EFFECT_PLATES:
        if(move_type == (u16)get_item_extra_param(battle_participants[bank_attacker].held_item))
        {
            modifier = chain_modifier(modifier, quality_atk_modifier);
        }
    }

    switch (move)
    {
    case MOVE_FACADE:
        if (battle_participants[atk_bank].status.flags.poison || battle_participants[atk_bank].status.flags.toxic_poison || battle_participants[atk_bank].status.flags.paralysis || battle_participants[atk_bank].status.flags.burn)
        {
            modifier = chain_modifier(modifier, 0x2000);
        }
        break;
    case MOVE_BRINE:
        if (battle_participants[def_bank].current_hp < (battle_participants[def_bank].max_hp >> 1))
        {
            modifier = chain_modifier(modifier, 0x2000);
        }
        break;
    case MOVE_VENOSHOCK:
        if (battle_participants[def_bank].status.flags.poison || battle_participants[def_bank].status.flags.toxic_poison)
        {
            modifier = chain_modifier(modifier, 0x2000);
        }
        break;
    case MOVE_RETALIATE:
        if (new_battlestruct->side_affecting[get_bank_side(atk_bank)].ally_fainted_last_turn)
        {
            modifier = chain_modifier(modifier, 0x2000);
        }
        break;
    case MOVE_SOLAR_BEAM:
        if (!(battle_weather.flags.sun || battle_weather.flags.permament_sun || battle_weather.flags.harsh_sun || battle_weather.int_bw == 0))
        {
            modifier = chain_modifier(modifier, 0x800);
        }
        break;
    case MOVE_EARTHQUAKE:
    case MOVE_MAGNITUDE:
    case MOVE_BULLDOZE:
        if (new_battlestruct->field_affecting.grassy_terrain && get_airborne_state(def_bank, 1, 1) <= 2)
        {
            modifier = chain_modifier(modifier, 0x800);
        }
        break;
    case MOVE_KNOCK_OFF:
        if (battle_participants[def_bank].held_item && can_lose_item(def_bank, 0, 0))
        {
            modifier = chain_modifier(modifier, 0x1800);
        }
        break;
    }

    if (protect_structs[atk_bank].flag0_helpinghand)
    {
        modifier = chain_modifier(modifier, 0x1800);
    }

    if (status3[atk_bank].charged_up && move_type == TYPE_ELECTRIC)
    {
        modifier = chain_modifier(modifier, 0x2000);
    }
    if ((move_type == TYPE_ELECTRIC && new_battlestruct->field_affecting.mudsport) || (move_type == TYPE_FIRE && new_battlestruct->field_affecting.watersport)) //mud and water sports
    {
        modifier = chain_modifier(modifier, 0x548);
    }
    if (new_battlestruct->bank_affecting[atk_bank].me_first)
    {
        modifier = chain_modifier(modifier, 0x1800);
    }
    if (new_battlestruct->field_affecting.grassy_terrain && get_airborne_state(atk_bank, 0, 1) <= 2 && move_type == TYPE_GRASS)
    {
        modifier = chain_modifier(modifier, 0x1800);
    }
    if (new_battlestruct->field_affecting.misty_terrain && get_airborne_state(def_bank, 1, 1) >= 2 && move_type == TYPE_DRAGON)
    {
        modifier = chain_modifier(modifier, 0x800);
    }
    if (new_battlestruct->field_affecting.electic_terrain && get_airborne_state(atk_bank, 0, 1) >= 2 && move_type == TYPE_ELECTRIC)
    {
        modifier = chain_modifier(modifier, 0x1800);
    }

    return apply_modifier(modifier, base_power);
}

u16 get_attack_stat(u16 move, u8 move_type, u8 atk_bank, u8 def_bank)
{
    u8 move_split = move_table[move].split;
    u8 stat_bank;
    if (move == MOVE_FOUL_PLAY)
    {
        stat_bank = def_bank;
    }
    else
    {
        stat_bank = atk_bank;
    }
    u16 attack_stat;
    u8 attack_boost;
    if (move_split == MOVE_PHYSICAL)
    {
        attack_stat = battle_participants[stat_bank].atk;
        attack_boost = battle_participants[stat_bank].atk_buff;
    }
    else
    {
        attack_stat = battle_participants[stat_bank].sp_atk;
        attack_boost = battle_participants[stat_bank].sp_atk_buff;
    }

    if (has_ability_effect(def_bank, 1) && battle_participants[def_bank].ability_id == ABILITY_UNAWARE)
    {
        attack_boost = 6;
    }
    else if (crit_loc == 2 && attack_boost < 6)
    {
        attack_boost = 6;
    }

    //apply stat boost to attack stat
    attack_stat = apply_statboost(attack_stat, attack_boost);

    //final modifications
    u16 modifier = 0x1000;
    if (has_ability_effect(atk_bank, 0))
    {
        u8 pinch_abilities;
        if (battle_participants[atk_bank].current_hp >= (battle_participants[atk_bank].max_hp / 3))
            pinch_abilities = false;
        else
            pinch_abilities = true;

        switch (battle_participants[atk_bank].ability_id)
        {
        case ABILITY_PURE_POWER:
        case ABILITY_HUGE_POWER:
            if (move_split == MOVE_PHYSICAL)
            {
                modifier = chain_modifier(modifier, 0x2000);
            }
            break;
        case ABILITY_SLOW_START:
            if (new_battlestruct->bank_affecting[atk_bank].slowstart_duration)
            {
                modifier = chain_modifier(modifier, 0x800);
            }
            break;
        case ABILITY_SOLAR_POWER:
            if (move_split == MOVE_SPECIAL && (battle_weather.flags.sun || battle_weather.flags.permament_sun || battle_weather.flags.harsh_sun))
            {
                modifier = chain_modifier(modifier, 0x1800);
            }
            break;
        case ABILITY_DEFEATIST:
            if (battle_participants[atk_bank].current_hp <= (battle_participants[atk_bank].max_hp >> 1))
            {
                modifier = chain_modifier(modifier, 0x800);
            }
            break;
        case ABILITY_FLASH_FIRE:
            if (move_type == TYPE_FIRE && battle_resources->ability_flags_ptr->flags_ability[atk_bank].flag1_flashfire)
            {
                modifier = chain_modifier(modifier, 0x1800);
            }
            break;
        case ABILITY_SWARM:
            if (move_type == TYPE_BUG && pinch_abilities)
            {
                modifier = chain_modifier(modifier, 0x1800);
            }
            break;
        case ABILITY_OVERGROW:
            if (move_type == TYPE_GRASS && pinch_abilities)
            {
                modifier = chain_modifier(modifier, 0x1800);
            }
            break;
        case ABILITY_TORRENT:
            if (move_type == TYPE_GRASS && pinch_abilities)
            {
                modifier = chain_modifier(modifier, 0x1800);
            }
            break;
        case ABILITY_BLAZE:
            if (move_type == TYPE_FIRE && pinch_abilities)
            {
                modifier = chain_modifier(modifier, 0x1800);
            }
            break;
        case ABILITY_THICK_FAT:
            if (move_type == TYPE_FIRE || move_type == TYPE_ICE)
            {
                modifier = chain_modifier(modifier, 0x800);
            }
            break;
        case ABILITY_PLUS:
        case ABILITY_MINUS:
            if (move_split == MOVE_SPECIAL && (ability_battle_effects(20, atk_bank, ABILITY_PLUS, 0, 0) || ability_battle_effects(20, atk_bank, ABILITY_MINUS, 0, 0)))
            {
                modifier = chain_modifier(modifier, 0x1800);
            }
            break;
        case ABILITY_HUSTLE:
            if (move_split == MOVE_PHYSICAL)
            {
                attack_stat = apply_modifier(0x1800, attack_stat);
            }
            break;
        }
    }

    if (battle_weather.flags.harsh_sun || battle_weather.flags.permament_sun || battle_weather.flags.sun)
    {
        u8 flower_gift_bank = ability_battle_effects(13, atk_bank, ABILITY_FLOWER_GIFT, 0, 0);
        if (flower_gift_bank && move_split == MOVE_PHYSICAL)
        {
            flower_gift_bank--;
            if (new_battlestruct->bank_affecting[flower_gift_bank].cherrim_form)
            {
                modifier = chain_modifier(modifier, 0x1800);
            }
        }
    }

    switch (get_item_effect(atk_bank, 1))
    {
    case ITEM_EFFECT_THICKCLUB:
        if (move_split == MOVE_PHYSICAL && (battle_participants[atk_bank].species == POKE_MAROWAK || battle_participants[atk_bank].species == POKE_CUBONE))
        {
            modifier = chain_modifier(modifier, 0x2000);
        }
        break;
    case ITEM_EFFECT_DEEPSEATOOTH:
        if (move_split == MOVE_SPECIAL && (battle_participants[atk_bank].species == POKE_CLAMPERL))
        {
            modifier = chain_modifier(modifier, 0x2000);
        }
        break;
    case ITEM_EFFECT_LIGHTBALL:
        if (battle_participants[atk_bank].species == POKE_PIKACHU)
        {
            modifier = chain_modifier(modifier, 0x2000);
        }
        break;
    case ITEM_EFFECT_SOULDEW:
        if (move_split == MOVE_SPECIAL && (battle_participants[atk_bank].species == POKE_LATIAS || battle_participants[atk_bank].species == POKE_LATIOS))
        {
            modifier = chain_modifier(modifier, 0x1800);
        }
        break;
    case ITEM_EFFECT_CHOICEBAND:
        if (move_split == MOVE_PHYSICAL)
        {
            modifier = chain_modifier(modifier, 0x1800);
        }
        break;
    case ITEM_EFFECT_CHOICESPECS:
        if (move_split == MOVE_SPECIAL)
        {
            modifier = chain_modifier(modifier, 0x1800);
        }
        break;
    }

    return apply_modifier(modifier, attack_stat);
}

u16 get_def_stat(u16 move, u8 atk_bank, u8 def_bank)
{
    u8 chosen_def; //0 = def, 1 = sp.def
    u8 move_split = move_table[move].split;
    if (move_split == MOVE_PHYSICAL || move == MOVE_PSYSTRIKE || move == MOVE_PSYSHOCK || move == MOVE_SECRET_SWORD)
        chosen_def = 0;
    else
        chosen_def = 1;

    if (new_battlestruct->field_affecting.wonder_room)
        chosen_def ^= 1;

    u16 def_stat;
    u8 def_boost;

    if (chosen_def) // sp.def
    {
        def_stat = battle_participants[def_bank].sp_def;
        def_boost = battle_participants[def_bank].sp_def_buff;
    }
    else //def
    {
        def_stat = battle_participants[def_bank].def;
        def_boost = battle_participants[def_bank].def_buff;
    }

    if (check_ability(atk_bank, ABILITY_UNAWARE) || find_move_in_table(move, ignore_targetstats_moves))
        def_boost = 6;
    else if (crit_loc == 2 && def_boost > 6)
        def_boost = 6;

    //apply def boost to the stat
    def_stat = apply_statboost(def_stat, def_boost);

    u16 modifier = 0x1000;

    if (move_split == MOVE_SPECIAL)
    {
        if ((battle_weather.flags.sandstorm || battle_weather.flags.permament_sandstorm) && is_of_type(def_bank, TYPE_ROCK) && weather_abilities_effect())
        {
            modifier = chain_modifier(modifier, 0x1800);
        }
        u8 flowergift_bank = ability_battle_effects(13, def_bank, ABILITY_FLOWER_GIFT, 0, 0);
        if (flowergift_bank)
        {
            if (has_ability_effect(flowergift_bank - 1, 1))
                modifier = chain_modifier(modifier, 0x1800);
        }
    }

    if (has_ability_effect(def_bank, 1) && battle_participants[def_bank].ability_id == ABILITY_MARVEL_SCALE && battle_participants[def_bank].status.int_status && chosen_def == 0)
    {
        modifier = chain_modifier(modifier, 0x1800);
    }
    else if (has_ability_effect(def_bank, 1) && battle_participants[def_bank].ability_id == ABILITY_FUR_COAT && chosen_def == 0)
    {
        modifier = chain_modifier(modifier, 0x2000);
    }
    else if (has_ability_effect(def_bank, 1) && battle_participants[def_bank].ability_id == ABILITY_GRASS_PELT && new_battlestruct->field_affecting.grassy_terrain && chosen_def == 0)
    {
        modifier = chain_modifier(modifier, 0x1800);
    }

    switch (get_item_effect(def_bank, 1))
    {
    case ITEM_EFFECT_DEEPSEASCALE:
        if (move_split == MOVE_SPECIAL && battle_participants[def_bank].species == POKE_CLAMPERL)
        {
            modifier = chain_modifier(modifier, 0x1800);
        }
        break;
    case ITEM_EFFECT_SOULDEW:
        if (move_split == MOVE_SPECIAL && (battle_participants[def_bank].species == POKE_LATIAS || battle_participants[def_bank].species == POKE_LATIOS))
        {
            modifier = chain_modifier(modifier, 0x1800);
        }
        break;
    case ITEM_EFFECT_ASSAULTVEST:
        if (chosen_def == 1)
        {
            modifier = chain_modifier(modifier, 0x1800);
        }
        break;
    case ITEM_EFFECT_EVIOLITE:
        if (can_evolve(battle_participants[def_bank].species))
        {
            modifier = chain_modifier(modifier, 0x1800);
        }
        break;
    case ITEM_EFFECT_METALPOWDER:
        if (battle_participants[def_bank].species == POKE_DITTO && move_split == MOVE_PHYSICAL && !(battle_participants[def_bank].status2.transformed))
        {
            modifier = chain_modifier(modifier, 0x2000);
        }
        break;
    }

    return apply_modifier(modifier, def_stat);
}

u16 type_effectiveness_calc(u16 move, u8 move_type, u8 atk_bank, u8 def_bank, u8 effects_handling_and_recording);
u8 does_move_target_multiple(void);
u8 count_alive_on_side(u8 bank);

u16 calc_reflect_modifier(u8 atk_bank, u8 def_bank, u16 final_modifier)
{
    if (crit_loc != 2 && !((check_ability(atk_bank, ABILITY_INFILTRATOR))))
    {
        if (count_alive_on_side(def_bank) == 2 && battle_flags.double_battle)
            final_modifier = chain_modifier(final_modifier, 0xA8F);
        else
            final_modifier = chain_modifier(final_modifier, 0x800);
    }
    return final_modifier;
}

void damage_calc(u16 move, u8 move_type, u8 atk_bank, u8 def_bank, u16 chained_effectiveness)
{
    damage_loc = 0;
    if (chained_effectiveness == 0) {return;} // avoid wastage of time in case of non effective moves
    u16 base_power = apply_base_power_modifiers(move, move_type, atk_bank, def_bank, get_base_power(move, atk_bank, def_bank));
    u16 atk_stat = get_attack_stat(move, move_type, atk_bank, def_bank);
    u16 def_stat = get_def_stat(move, atk_bank, def_bank);
    u32 damage = ((((2 * battle_participants[atk_bank].level) / 5 + 2) * base_power * atk_stat) / def_stat) / 50 + 2;

    //apply multi target modifier
    if (does_move_target_multiple())
    {
        damage = apply_modifier(0xC00, damage);
    }
    //weather modifier
    if (weather_abilities_effect())
    {
        if (battle_weather.flags.downpour || battle_weather.flags.rain || battle_weather.flags.permament_rain || battle_weather.flags.heavy_rain)
        {
            if (move_type == TYPE_FIRE)
                damage = apply_modifier(0x800, damage);
            else if (move_type == TYPE_WATER)
                damage = apply_modifier(0x1800, damage);
        }
        else if (battle_weather.flags.sun || battle_weather.flags.permament_sun || battle_weather.flags.harsh_sun)
        {
            if (move_type == TYPE_FIRE)
                damage = apply_modifier(0x1800, damage);
            else if (move_type == TYPE_WATER)
                damage = apply_modifier(0x800, damage);
        }
    }
    //crit modifier
    if (crit_loc == 2 && move != MOVE_CONFUSION_DMG)
    {
        damage = apply_modifier(0x1800, damage);
    }
    //rand modifier
    damage = (damage * (100 - (__umodsi3(rng(), 14) + 1))) / 100;

    u16 final_modifier = 0x1000;
    //stab modifier
    if (move_type != TYPE_EGG && is_of_type(atk_bank, move_type) && move != MOVE_STRUGGLE)
    {
        damage = apply_modifier(0x1800, damage);
        if (check_ability(atk_bank, ABILITY_ADAPTABILITY))
            final_modifier = 0x2000;
    }
    //type effectiveness
    damage=(damage*chained_effectiveness)>>6;

    //burn
    if (battle_participants[atk_bank].status.flags.burn && move_table[move].split == MOVE_PHYSICAL && move != MOVE_FACADE && !(check_ability(atk_bank, ABILITY_GUTS)))
    {
        damage /= 2;
    }
    //at least one check
    damage = ATLEAST_ONE(damage);
    //final modifiers
    u8 move_split = move_table[move].split;

    //check reflect/light screen
    u8 def_side = get_bank_side(def_bank);
    if ((side_affecting_halfword[def_side].reflect_on && move_split == MOVE_PHYSICAL) ||(side_affecting_halfword[def_side].light_screen_on && move_split == MOVE_SPECIAL))
        final_modifier = calc_reflect_modifier(atk_bank, def_bank, final_modifier);

    //check aurora veil
    if (new_battlestruct->side_affecting[def_side].aurora_veil)
        final_modifier = calc_reflect_modifier(atk_bank, def_bank, final_modifier);

    u8 atk_ability = battle_participants[atk_bank].ability_id;
    u8 def_ability = battle_participants[def_bank].ability_id;
    //halve damage if full HP
    if (FULL_HP(def_bank) && (def_ability == ABILITY_SHADOW_SHIELD || def_ability == ABILITY_MULTISCALE) && has_ability_effect(def_bank, 1))
    {
        final_modifier = chain_modifier(final_modifier, 0x800);
    }
    if (atk_ability == ABILITY_TINTED_LENS && move_outcome.not_very_effective && has_ability_effect(atk_bank, 0))
    {
        final_modifier = chain_modifier(final_modifier, 0x2000);
    }
    if (ability_battle_effects(20, def_bank, ABILITY_FRIEND_GUARD, 0, 1))
    {
        if((atk_ability != ABILITY_MOLD_BREAKER && atk_ability != ABILITY_TERAVOLT &&
            atk_ability != ABILITY_TURBOBLAZE) || !has_ability_effect(atk_bank,0))
            final_modifier = chain_modifier(final_modifier, 0xC00);
    }
    if (atk_ability == ABILITY_SNIPER && crit_loc == 2 && has_ability_effect(atk_bank, 0))
    {
        final_modifier = chain_modifier(final_modifier, 0x1800);
    }
    //reduces power of super effective moves
    if (move_outcome.super_effective && ((def_ability == ABILITY_FILTER || def_ability == ABILITY_SOLID_ROCK || def_ability == ABILITY_PRISM_ARMOR) && has_ability_effect(def_bank, 1)))
    {
        final_modifier = chain_modifier(final_modifier, 0xC00);
    }
    if (get_item_effect(atk_bank, 1) == ITEM_EFFECT_METRONOME)
    {
        if (new_battlestruct->bank_affecting[atk_bank].same_move_used > 4)
            final_modifier = chain_modifier(final_modifier, 0x2000);
        else
            final_modifier = chain_modifier(final_modifier, 0x1000 + new_battlestruct->bank_affecting[atk_bank].same_move_used * 0x333);
    }
    else if (get_item_effect(atk_bank, 1) == ITEM_EFFECT_EXPERTBELT && move_outcome.super_effective)
    {
        final_modifier = chain_modifier(final_modifier, 0x1333);
    }
    else if (get_item_effect(atk_bank, 1) == ITEM_EFFECT_LIFEORB)
    {
        final_modifier = chain_modifier(final_modifier, 0x14CD);
    }

    if(get_item_effect(def_bank,1)==ITEM_EFFECT_DAM_REDUX_BERRY)
    {
        u8 type_to_resist = get_item_extra_param(battle_participants[def_bank].held_item);
        if(type_to_resist==move_type && (move_outcome.super_effective || type_to_resist==TYPE_NORMAL))
        {
            final_modifier = chain_modifier(final_modifier, 0x800);
            new_battlestruct->various.berry_damage_redux=1;
        }
    }

    if ((move == MOVE_STEAMROLLER || move == MOVE_STOMP) && status3[def_bank].minimized)
    {
        final_modifier = chain_modifier(final_modifier, 0x2000);
    }
    else if (move == MOVE_EARTHQUAKE && status3[def_bank].underground)
    {
        final_modifier = chain_modifier(final_modifier, 0x2000);
    }
    else if (move == MOVE_SURF && status3[def_bank].underwater)
    {
        final_modifier = chain_modifier(final_modifier, 0x2000);
    }

    damage = apply_modifier(final_modifier, damage);
    damage_loc = ATLEAST_ONE(damage);
}

u8 get_attacking_move_type(void);

void atk05_dmg_calc(void)
{
    u8 move_type=get_attacking_move_type();
    u16 item = battle_participants[bank_attacker].held_item;
    u16 chained_effectiveness=type_effectiveness_calc(current_move, move_type, bank_attacker,bank_target,1);
    if (MOVE_WORKED && DAMAGING_MOVE(current_move) && item && get_item_effect(bank_attacker, 1) == ITEM_EFFECT_GEM && get_item_extra_param(item) == move_type && current_move != MOVE_STRUGGLE && current_move != MOVE_WATER_PLEDGE && current_move != MOVE_FIRE_PLEDGE && current_move != MOVE_GRASS_PLEDGE)
    {
        last_used_item = item;
        new_battlestruct->various.gem_boost = 1;
        move_to_buff1(current_move);
        bs_push_current(BS_GEM_MSG);
        return;
    }
    damage_calc(current_move, move_type, bank_attacker, bank_target, chained_effectiveness);
    if (new_battlestruct->various.parental_bond_mode == PBOND_CHILD)
        damage_loc /= 2;

    battlescripts_curr_instruction++;
}
