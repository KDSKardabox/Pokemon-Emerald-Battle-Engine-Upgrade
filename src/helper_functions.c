#include "types.h"
#include "defines.h"
#include "battle_locations.h"
#include "battle_structs.h"
#include "vanilla_functions.h"
#include "new_battle_struct.h"
#include "static_references.h"

u8 check_ability(u8 bank, u8 ability);
u8 get_airborne_state(u8 bank, u8 mode, u8 check_levitate);
u16 get_1_16_of_max_hp(u8 bank);
u8 item_battle_effects(u8 switchid, u8 bank, u8 moveturn);
u8 affected_by_substitute(u8 substitute_bank);
u8 change_stats(s8 arg1, s8 arg2, s8 arg3, void* battlescript_if_fails);
u8 cant_become_paralyzed(u8 bank, u8 self_inflicted);
u8 cant_poison(u8 bank, u8 self_inflicted);
u8 cant_become_burned(u8 bank, u8 self_inflicted);
u8 cant_become_freezed(u8 bank, u8 self_inflicted);
u8 cant_fall_asleep(u8 bank, u8 self_inflicted);
u8 berry_eaten(u8 howcall, u8 bank);
u32 percent_lose(u32 number, u16 percent);
u8 find_move_in_table(u16 move, u16 table_ptr[]);
u8 is_of_type(u8 bank, u8 type);
u8 set_type(u8 bank, u8 type);
u8 get_first_to_strike(u8 bank1, u8 bank2, u8 ignore_priority);
u16 get_poke_weight(u8 bank);
u8 get_attacking_move_type();
u16 type_effectiveness_calc(u16 move, u8 type, u8 atkbank, u8 defbank, u8 effectshandling);
void* get_move_battlescript_ptr(u16 move);
u8 get_target_of_move(u16 move, u8 target_given, u8 adjust);
void move_to_buffer(u16 move);
u8 check_mega_evo(u8 bank);
u8 is_bank_present(u8 bank);
u8 can_lose_item(u8 bank, u8 stickyhold_check, u8 sticky_message);
u8 canlose_megastone(u8 bank, u16 item);
extern u8 type_effectiveness_table[TYPE_FAIRY-0x4][TYPE_FAIRY-0x4];

u8 sample_text[] = {0xDD, 0xFF};
u8 snowwarning_text[] = {0xFD, 0x13, 0xB4, 0xE7, 0, 0xFD, 0x1A, 0xFE, 0xEB, 0xDC, 0xDD, 0xE4, 0xE4, 0xD9, 0xD8, 0x00, 0xE9, 0xE4, 0x00, 0xD5, 0x00, 0xDC, 0xD5, 0xDD, 0xE0, 0xE7, 0xE8, 0xE3, 0xE6, 0xE1, 0xAB, 0xFF};
u8 extreme_sun_activation_text[] = {0xCE, 0xDC, 0xD9, 0x00, 0xE7, 0xE9, 0xE2, 0xE0, 0xDD, 0xDB, 0xDC, 0xE8, 0x00, 0xE8, 0xE9, 0xE6, 0xE2, 0xD9, 0xD8, 0xFE, 0xD9, 0xEC, 0xE8, 0xE6, 0xD9, 0xE1, 0xD9, 0xE0, 0xED, 0x00, 0xDC, 0xD5, 0xE6, 0xE7, 0xDC, 0xAB, 0xFF};
u8 heavyrain_activation_text[] = {0xBB, 0x00, 0xDC, 0xD9, 0xD5, 0xEA, 0xED, 0x00, 0xE6, 0xD5, 0xDD, 0xE2, 0x00, 0xD6, 0xD9, 0xDB, 0xD5, 0xE2, 0x00, 0xE8, 0xE3, 0x00, 0xDA, 0xD5, 0xE0, 0xE0, 0xAB, 0xFF};
u8 mysticalaircurrent_activation_text[] = {0xBB, 0x00, 0xE1, 0xED, 0xE7, 0xE8, 0xD9, 0xE6, 0xDD, 0xE3, 0xE9, 0xE7, 0x00, 0xD5, 0xDD, 0xE6, 0x00, 0xD7, 0xE9, 0xE6, 0xE6, 0xD9, 0xE2, 0xE8, 0xFE, 0xDD, 0xE7, 0x00, 0xE4, 0xE6, 0xE3, 0xE8, 0xD9, 0xD7, 0xE8, 0xDD, 0xE2, 0xDB, 0x00, 0xC0, 0xE0, 0xED, 0xDD, 0xE2, 0xDB, 0xAE, 0xE8, 0xED, 0xE4, 0xD9, 0x00, 0xCA, 0xE3, 0xDF, 0x1B, 0xE1, 0xE3, 0xE2, 0xAB, 0xFF};
u8 forewarn_text[] = {0xFD, 0x13, 0xB4, 0xE7, 0, 0xFD, 0x1A, 0xFE, 0xD5, 0xE0, 0xD9, 0xE6, 0xE8, 0xD9, 0xD8, 0, 0xDD, 0xE8, 0, 0xE8, 0xE3, 0x0, 0xFD, 0, 0xAB, 0xFF};
u8 slowstart_text[] = {0xFD, 0x13, 0x0, 0xD7, 0xD5, 0xE2, 0xB4, 0xE8, 0x00, 0xDB, 0xD9, 0xE8, 0x00, 0xDD, 0xE8, 0x00, 0xDB, 0xE3, 0xDD, 0xE2, 0xDB, 0xAB, 0xFF};
u8 anticipation_text[] = {0xFD, 0x13, 0, 0xE7, 0xDC, 0xE9, 0xD8, 0xD8, 0xD9, 0xE6, 0xD9, 0xD8, 0x00, 0xEB, 0xDD, 0xE8, 0xDC, 0x00, 0xD5, 0xE2, 0xE8, 0xDD, 0xD7, 0xDD, 0xE4, 0xD5, 0xE8, 0xDD, 0xE3, 0xE2, 0xAB ,0xFF};
u8 dryskin_damage_text[] = {0xFD, 15, 0xB4, 0xE7, 0, 0xFD, 24, 0x0, 0xE1, 0xD5, 0xDF, 0xD9, 0xE7, 0x00, 0xDD, 0xE7, 0xFE, 0xEA, 0xE9, 0xE0, 0xE2, 0xD9, 0xE6, 0xD5, 0xD6, 0xE0, 0xD9, 0x00, 0xE8, 0xE3, 0x00, 0xE7, 0xE9, 0xE2, 0xAB, 0xFF};
u8 solarpower_text[] = {0xFD, 15, 0xB4, 0xE7, 0, 0xFD, 24, 0x0, 0xE1, 0xD5, 0xDF, 0xD9, 0xE7, 0x0, 0xDD, 0xE8, 0x0, 0xD8, 0xE6, 0xD5, 0xEB, 0x0, 0xE4, 0xE3, 0xEB, 0xD9, 0xE6, 0xFE, 0xDA, 0xE6, 0xE3, 0xE1, 0x0, 0xE8, 0xDC, 0xD9, 0x0, 0xE7, 0xE9, 0xE2, 0x0, 0xDD, 0xE2, 0x00, 0xD9, 0xEC, 0xD7, 0xDC, 0xD5, 0xE2, 0xDB, 0xD9, 0x00, 0xDA, 0xE3, 0xE6, 0x00, 0xE0, 0xDD, 0xDA, 0xD9, 0xAB, 0xFF};
u8 harvest_text[] = {0xFD, 15, 0, 0xDC, 0xD5, 0xE6, 0xEA, 0xD9, 0xE7, 0xE8, 0xD9, 0xD8, 0x00, 0xE3, 0xE2, 0xD9, 0, 0xFD, 22, 0xAB, 0xFF};
u8 healer_text[] = {0xFD, 15, 0, 0xB4, 0xE7, 0, 0xFD, 24, 0, 0xD7, 0xE9, 0xE6, 0xD9, 0xD8, 0x00, 0xDD, 0xE8, 0xE7, 0x00, 0xE4, 0xD5, 0xE6, 0xE8, 0xE2, 0xD9, 0xE6, 0xB4, 0xE7, 0xFE, 0xFD, 0x0, 0xAB, 0xFF};
u8 pickup_text[] = {0xFD, 15, 0, 0xDA, 0xE3, 0xE9, 0xE2, 0xD8, 0x00, 0xE3, 0xE2, 0xD9, 0x00, 0xFD, 22, 0xAB, 0xFF};
u8 moldbreaker_text[] = {0xFD, 0x13, 0, 0xD6, 0xE6, 0xD9, 0xD5, 0xDF, 0xE7, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xE1, 0xE3, 0xE0, 0xD8, 0xAB, 0xFF};
u8 turboblaze_text[] = {0xFD, 0x13, 0, 0xDD, 0xE7, 0x00, 0xE6, 0xD5, 0xD8, 0xDD, 0xD5, 0xE8, 0xDD, 0xE2, 0xDB, 0x00, 0xD5, 0x00, 0xD6, 0xE0, 0xD5, 0xEE, 0xDD, 0xE2, 0xDB, 0x00, 0xD5, 0xE9, 0xE6, 0xD5, 0xAB, 0xFF};
u8 terravolt_text[] = {0xFD, 0x13, 0, 0xDD, 0xE7, 0x00, 0xE6, 0xD5, 0xD8, 0xDD, 0xD5, 0xE8, 0xDD, 0xE2, 0xDB, 0x00, 0xD5, 0x00, 0xD6, 0xE9, 0xE6, 0xE7, 0xE8, 0xDD, 0xE2, 0xDB, 0x00, 0xD5, 0xE9, 0xE6, 0xD5, 0xAB, 0xFF};
u8 downloadatk_text[] = {0xFD, 0x13, 0xB4, 0xE7, 0x0, 0xFD, 0x1A, 0, 0xE6, 0xD5, 0xDD, 0xE7, 0xD9, 0xD8, 0x00, 0xDD, 0xE8, 0xE7, 0xFE, 0xBB, 0xE8, 0xE8, 0xD5, 0xD7, 0xDF, 0xAB, 0xFF};
u8 downloadspatk_text[] = {0xFD, 0x13, 0xB4, 0xE7, 0x0, 0xFD, 0x1A, 0, 0xE6, 0xD5, 0xDD, 0xE7, 0xD9, 0xD8, 0x00, 0xDD, 0xE8, 0xE7, 0xFE, 0xCD, 0xE4, 0xAD, 0, 0xBB, 0xE8, 0xE8, 0xD5, 0xD7, 0xDF, 0xAB, 0xFF};
u8 absorbabilityboost_text[] = {0xFD, 0x10, 0xB4, 0xE7, 0, 0xFD, 0x19, 0, 0xE6, 0xD5, 0xDD, 0xE7, 0xD9, 0xD8, 0x00, 0xDD, 0xE8, 0xE7, 0xFE, 0xFD, 0x0, 0xAB ,0xFF};
u8 absorbabilityimmune_text[] = {0xFD, 0x10, 0xB4, 0xE7, 0, 0xFD, 0x19, 0, 0xE1, 0xD5, 0xDF, 0xD9, 0xE7, 0x00, 0xDD, 0xE8, 0xFE, 0xDD, 0xE1, 0xE1, 0xE9, 0xE2, 0xD9, 0x00, 0xE8, 0xE3, 0x00, 0xFD, 0x00, 0xAE, 0xE8, 0xED, 0xE4, 0xD9, 0x00, 0xE1, 0xE3, 0xEA, 0xD9, 0xE7, 0xAB, 0xFF};
u8 userteam_text[] = {Y_, o_, u_, r_, 0, t_, e_, a_, m_, Termin};
u8 foeteam_text[] = {T_, h_, e_, 0, f_, o_, e_, Apos, s_, 0, t_, e_, a_, m_, Termin};
//u8 userteam_lc_text[] = {y_, o_, u_, r_, 0, t_, e_, a_, m_, Termin};
//u8 foeteam_lc_text[] = {t_, h_, e_, 0, f_, o_, e_, Apos, s_, 0, t_, e_, a_, m_, Termin};
u8 aftermath_text[] = {0xFD, 15, 0, 0xDD, 0xE7, 0x0, 0xDC, 0xE9, 0xE6, 0xE8, 0x00, 0xD6, 0xED, 0xFE, 0xFD, 16, 0xB4, 0xE7, 0, 0xFD, 25, 0xAB, 0xFF};
u8 pickpocket_text[] = {0xFD, 16, 0, 0xE7, 0xE8, 0xE3, 0xE0, 0xD9, 0, 0xFD, 15, 0xB4, 0xE7, 0xFE, 0xFD, 22, 0xAB, 0xFF};
u8 mummy_text[] = {0xFD, 15, 0xB4, 0xE7, 0, 0xD5, 0xD6, 0xDD, 0xE0, 0xDD, 0xE8, 0xED, 0xFE, 0xD6, 0xD9, 0xD7, 0xD5, 0xE1, 0xD9, 0, 0xFD, 24, 0xAB, 0xFF};
/*0x195*/ u8 target_ability[] = {0xFD, 16, 0xB4, 0xE7, 0, 0xFD, 25, 0xAB, 0xFF};
/*0x196*/ u8 cursedbody_text[] = {0xFD, 16, 0xB4, 0xE7, 0, 0xFD, 25, 0, 0xD8, 0xDD, 0xE7, 0xD5, 0xD6, 0xE0, 0xD9, 0xD8, 0xFE, 0xFD, 15, 0xB4, 0xE7, 0, 0xFD, 0, 0xAB, 0xFF};
/*0x197*/ u8 drastically_text[] = {0xD8, 0xE6, 0xD5, 0xE7, 0xE8, 0xDD, 0xD7, 0xD5, 0xE0, 0xE0, 0xED, 0x0, 0xFF};
/*0x198*/ u8 pressure_text[] = {BuffCharac, 0x13, Space, i_, s_, Space, e_, x_, e_, r_, t_, i_, n_, g_, JumpLine, i_, t_, s_, Space, P_, r_, e_, s_, s_, u_, r_, e_, Exclam, Termin};
/*0x199*/ u8 unnerve_text[] = {0xFD, 0x0, 0, i_, s_, 0, t_, o_, o_, 0, n_, e_, r_, v_, o_, u_, s_, 0xFE, t_, o_, 0, e_, a_, t_, 0, B_, e_, r_, r_, i_, e_, s_, Exclam, Termin};
/*0x19A*/ u8 aurabreak_text[] = {0xFD, 0x13, 0, r_, e_, v_, e_, r_, s_, e_, d_, 0, a_, l_, l_, 0, o_, t_, h_, e_, r_, 0xFE, P_, o_, k_, Poke_e, m_, o_, n_, Apos, s_, 0, a_, u_, r_, a_, s_, Exclam, 0xFF};
/*0x19B*/ u8 fairyaura_text[] = {BuffCharac, 0x13, Space, i_, s_, Space, r_, a_, d_, i_, a_, t_, i_, n_, g_, JumpLine, a_, Space, F_, a_, i_, r_, y_, Space, A_, u_, r_, a_, Exclam, Termin};
/*0x19C*/ u8 darkaura_text[] = {BuffCharac, 0x13, Space, i_, s_, Space, r_, a_, d_, i_, a_, t_, i_, n_, g_, JumpLine, a_, Space, D_, a_, r_, k_, Space, A_, u_, r_, a_, Exclam, Termin};
/*0x19D*/ u8 frisk_text[] = {BuffCharac, 0x13, Space, f_, r_, i_, s_, k_, e_, d_, Space, BuffCharac, 0x10, JumpLine, a_, n_, d_, Space,
                   f_, o_, u_, n_, d_, Space, i_, t_, s_, Space, BuffCharac, 0x16, Exclam, Termin};
/*0x19E*/ u8 hurtbyitem_text[] = {BuffCharac, 15, Space, i_,s_, Space, h_, u_, r_, t_, Space, b_, y_, Space, BuffCharac, 22, Exclam,0xFF};
/*0x19F*/ u8 got_burned_text[] = {BuffCharac, 15, Space, g_, o_, t_, Space, b_, u_, r_, n_, e_, d_, Exclam, 0xFF};
/*0x1A0*/ u8 got_badlypoisoned_text[] = {BuffCharac, 15, Space, g_, o_, t_, Space, b_, a_, d_, l_, y_, 0xFE, p_, o_, i_, s_, o_, n_, e_, d_, Exclam, 0xFF};
/*0x1A1*/ u8 airballoon_text[] = {BuffCharac, 18, Space, i_, s_, Space, f_, l_, o_, a_, t_, i_, n_, g_, Exclam, 0xFF};
/*0x1A2*/ u8 bad_dreams_text[] = {BuffCharac, 0x10, Space, i_, s_, Space, t_, o_, r_, m_, e_, n_, t_, e_, d_, Space, b_, y_, JumpLine, b_, a_, d_, Space, d_, r_, e_, a_, m_, s_, Exclam, Termin};
/*0x1A3*/ u8 item_text[] = {BuffCharac, 16, Apos, Space, BuffCharac, 22, Exclam, 0xFF};
/*0x1A4*/ u8 rockyhelmet_text[] = {BuffCharac, 15, Space, i_, s_, Space, h_, u_, r_, t_, Space, b_, y_, JumpLine, BuffCharac, 16, Apos, Space, BuffCharac, 22, Exclam, 0xFF};
/*0x1A5*/ u8 popped_text[] = {BuffCharac, 16, Apos, Space, BuffCharac, 22, Space, p_, o_, p_, p_, e_, d_, Exclam, 0xFF};
/*0x1A6*/ u8 fellinlove_text[] = {BuffCharac, 15, Space, f_,e_,l_,l_, Space, i_, n_, Space, l_, o_, v_, e_, JumpLine, w_, i_, t_, h_, Space, BuffCharac, 16, Exclam, 0xFF};
/*0x1A7*/ u8 healblockend_text[] = {H_, e_, a_, l_, Space, B_, l_, o_, c_, k_, Space, f_, o_, r_, BuffCharac, 18, JumpLine, h_, a_, s_, Space, e_, n_, d_, e_, d_, Exclam, 0xFF};
/*0x1A8*/ u8 magicbounce_text[] = {BuffCharac, 15, Apos, s_, Space, BuffCharac, 20, JumpLine, w_, a_, s_, Space, b_, o_, u_, n_, c_, e_, d_, Space, b_, a_, c_, k_, Space, b_, y_, Space, BuffCharac, 25, Exclam, 0xFF};
/*0x1A9*/ u8 angerpoint_text[] = {0xFD, 0x10, Apos, s_, 0, 0xFD, 0x19, 0 ,m_, a_, x_, e_, d_, 0, i_, t_, s_, 0xFE, a_, t_, t_, a_, c_, k_, Exclam, 0xFF};
/*0x1AA*/ u8 stealhrock_text[] = {P_, o_, i_, n_, t_, e_, d_, Space, s_, t_, o_, n_, e_, s_, Space, d_, u_, g_, Space, i_, n_, t_, o_, JumpLine, BuffCharac, 16, Exclam, 0xFF};
/*0x1AB*/ u8 stickyweb_text[] = {BuffCharac, 16, Space, w_, a_, s_, Space, c_, a_, u_, g_, h_, t_, Space, i_, n_, Space, a_, Space, S_, t_, i_, c_, k_, y_, Space, W_, e_, b_, Exclam, 0xFF};
/*0x1AC*/ u8 gotpoisoned_text[] = {BuffCharac, 16, Space, g_, o_, t_, Space, p_, o_, i_, s_, o_, n_, e_, d_, Exclam, 0xFF};
/*0x1AD*/ u8 absorbed_spikes_text[] = {T_, o_, x_, i_, c_, Space, S_, p_, i_, k_, e_, s_, Space, h_, a_, v_, e_, Space, b_, e_, e_, n_, JumpLine, a_, b_, s_, o_, r_, b_, e_, d_, Space, b_, y_, Space, BuffCharac, 16, Exclam, 0xFF};
/*0x1AE*/ u8 lost_some_hp_text[] = {BuffCharac, 15, Space, l_, o_, s_, t_, Space, s_, o_, m_, e_, Space, o_, f_, Space, i_, t_, s_, Space, H_, P_, Exclam, 0xFF};
/*0x1AF*/ u8 tauntended_text[] = {BuffCharac, 18, Space, i_, s_, Space, n_, o_, Space, l_, o_, n_, g_, e_, r_, Space, T_, a_, u_, n_, t_, e_, d_, Exclam, 0xFF};
/*0x1B0*/ u8 tormentended_text[] = {BuffCharac, 18, Space, i_, s_, Space, n_, o_, Space, l_, o_, n_, g_, e_, r_, Space, T_, o_, r_, m_, e_, n_, e_, d_, Exclam, 0xFF};
/*0x1B1*/ u8 healblockprevents_text[] = {H_, e_, a_, l_, Space, B_, l_, o_, c_, k_, Space, p_, r_, e_, v_, e_, n_, t_, s_, JumpLine, u_, s_, a_, g_, e_, Space, o_, f_, Space, BuffCharac, 0, Exclam, 0xFF};
/*0x1B2*/ u8 gravityprevents_text[] = {G_, r_, a_, v_, i_, t_, y_, Space, p_, r_, e_, v_, e_, n_, t_, s_, JumpLine, u_, s_, a_, g_, e_, Space, o_, f_, Space, BuffCharac, 0, Exclam, 0xFF};
/*0x1B3*/ u8 embargoprevents_text[] = {C_, a_, n_, Apos, t_, Space, u_, s_, e_, Space, BuffCharac, 0, Space, u_, n_, d_, e_, r_, JumpLine, t_, h_, e_, Space, e_, f_, f_, e_, c_, t_, s_, Space, o_, f_, Space, E_, m_, b_, a_, r_, g_, o_, Exclam, 0xFF};
/*0x1B4*/ u8 aromaveilprevents_text[] = {BuffCharac, 23, Space, p_, r_, e_, v_, e_, n_, t_, s_, BuffCharac, 16, f_, r_, o_, m_, BuffCharac, 0, Exclam, 0xFF};
/*0x1B5*/ u8 spikyshield_damage[] = {BuffCharac, 16, Apos, Space, S_, p_, i_, k_, y_, Space, S_, h_, i_, e_, l_, d_, JumpLine, h_, u_, r_, t_, s_, Space, BuffCharac, 15, Exclam, 0xFF};
/*0x1B6*/ u8 symbiosispassing_text[] = {BuffCharac, 16, Space, p_, a_, s_, s_, e_, d_, Space, i_, t_, s_, JumpLine, i_, t_, e_, m_, Space, t_, o_, Space, BuffCharac, 19, Exclam, 0xFF};
/*0x1B7*/ u8 restored_hp_text[] = {BuffCharac, 16, Space, r_, e_, s_, t_, o_, r_, e_, d_, Space, i_, t_, s_, Space, H_, P_, Exclam, 0xFF};
/*0x1B8*/ u8 replacement_healed_text[] = {T_, h_, e_, Space, r_, e_, p_, l_, a_, c_, e_, m_, e_, n_, t_, Space, w_, a_, s_, Space, f_, u_, l_, l_, y_, Space, h_, e_, a_, l_, e_, d_, Exclam, 0xFF};
/*0x1B9*/ u8 telekinesis_end_text[] = {BuffCharac, 15, Space, w_, a_, s_, Space, f_, r_, e_, e_, d_, Space, f_, r_, o_, m_, JumpLine, t_, h_, e_, Space, T_, e_, l_, e_, k_, i_, n_, e_, s_, i_, s_, Exclam, 0xFF};
/*0x1BA*/ u8 embargoend_text[] = {BuffCharac, 15, Space, c_, a_, n_, Space, u_, s_, e_, JumpLine, i_, t_, e_, m_, s_, Space, a_, g_, a_, i_, n_, Exclam, 0xFF};
/*0x1BB*/ u8 magnetriseend_text[] = {BuffCharac, 15, Apos, s_, Space, e_, l_, e_, c_, t_, r_, o_, m_, a_, g_, n_, e_, t_, i_, s_, m_, JumpLine, w_, o_, r_, e_, Space, o_, f_, f_, Exclam, 0xFF};
/*0x1BC*/ u8 wrapped_text[] = {BuffCharac, 16, Space, w_, a_, s_, Space, t_, r_, a_, p_, p_, e_, d_, JumpLine, b_, y_, Space, 0xFD, 0, Exclam, 0xFF};
/*0x1BD*/ u8 nofiremoves_text[] = {T_, h_, e_, Space, F_, i_, r_, e_, Dash, t_, y_,p_,e_, a_,t_,t_,a_,c_,k_,JumpLine, f_,i_,z_,z_,l_,e_,d_,Space, o_,u_,t_,Space, i_,n_, Space,t_,h_,e_, Space, h_,e_,a_,v_,y_,Space, r_,a_,i_,n_,Exclam,0xFF};
/*0x1BE*/ u8 nowatermoves_text[] = {T_, h_, e_, Space, W_, a_, t_, e_, r_, Dash, t_, y_,p_,e_, a_,t_,t_,a_,c_,k_,JumpLine, e_,v_,a_,p_,o_,r_,a_,t_,e_,d_, Space, i_,n_, Space, t_,h_,e_, Space, h_,a_,r_,s_,h_, Space, s_,u_,n_,l_,i_,g_,h_,t_, Exclam, 0xFF};
/*0x1BF*/ u8 trickroom_ends[] = {T_, h_, e_, Space, d_, i_, m_, e_, n_, s_, i_, o_, n_, s_, Space, r_, e_, t_, u_, r_, n_, e_, d_, Space, t_, o_, Space, n_, o_, r_, m_, a_, l_, Exclam, 0xFF};
/*0x1C0*/ u8 magicroom_ends[] = {M_, a_, g_, i_, c_, Space, R_, o_, o_, m_, Space, w_, o_, r_, e_, Space, o_, f_, f_, Space, a_, n_, d_, Space, h_, e_, l_, d_, JumpLine, i_, t_, e_, m_, s_, Apos, Space, e_, f_, f_, e_, c_, t_, s_, Space, r_, e_, t_, u_, r_, n_, e_, d_, Space, t_, o_, Space, n_, o_, r_, m_, a_, l_, Exclam, 0xFF};
/*0x1C1*/ u8 wonderoom_ends[] = {W_, o_, n_, d_, e_, r_, Space, R_, o_, o_, m_, Space, w_, o_, r_, e_, Space, o_, f_, f_, Space, a_, n_, d_, Space, D_, e_, f_, e_, n_, s_, e_, Space, a_, n_, d_, JumpLine,S_, p_, Dot, Space, D_, e_, f_, Space, s_, t_, a_, t_, s_, Space, r_, e_, t_, u_, r_, n_, e_, d_, Space, t_, o_, Space, n_, o_, r_, m_, a_, l_, Exclam, 0xFF};
/*0x1C2*/ u8 gravity_ends_text[] = {G_, r_, a_, v_, i_, t_, y_, Space, r_, e_, t_, u_, r_, n_, e_, d_, Space, t_, o_, Space, n_, o_, r_, m_, a_, l_, Exclam, 0xFF};
/*0x1C3*/ u8 grassyterainends_text[] = {T_, h_, e_, Space, g_, r_, a_, s_, s_, Space, d_, i_, s_, a_, p_, p_, e_, a_, r_, e_, d_, Space, f_, r_, o_, m_, JumpLine,t_, h_, e_, Space, b_, a_, t_, t_, l_, e_, f_, i_, e_, l_, d_, Exclam, 0xFF};
/*0x1C4*/ u8 mistyterrainends_text[] = {T_, h_, e_, Space, m_, i_, s_, t_,  Space, d_, i_, s_, a_, p_, p_, e_, a_, r_, e_, d_, Space, f_, r_, o_, m_, JumpLine,t_, h_, e_, Space, b_, a_, t_, t_, l_, e_, f_, i_, e_, l_, d_, Exclam, 0xFF};
/*0x1C5*/ u8 electrerrainends_text[] = {T_, h_, e_, Space, e_, l_, e_, c_, t_, r_, i_, c_, i_, t_, y_, Space, d_, i_, s_, a_, p_, p_, e_, a_, r_, e_, d_, Space, f_, r_, o_, m_, JumpLine,t_, h_, e_, Space, b_, a_, t_, t_, l_, e_, f_, i_, e_, l_, d_, Dot, 0xFF};
/*0x1C6*/ u8 grassyterrain_heal[] = {BuffCharac, 19, Space, r_, e_, s_, t_, o_, r_, e_, d_, Space, s_, o_, m_, e_, Space, o_, f_, JumpLine, i_, t_, s_, Space, H_, P_, Space, u_, s_, i_, n_, g_, Space, G_, r_, a_, s_, s_, y_, Space, T_, e_, r_, r_, a_, i_, n_, Exclam, 0xFF};
/*0x1C7*/ u8 fogcontinues_text[] = {T_, h_, e_, Space, f_, o_, g_, Space, i_, s_, Space, d_, e_, e_, p_, Dot, Dot, Dot, 0xFF};
/*0x1C8*/ u8 fogends_text[] = {T_, h_, e_, Space, f_, o_, g_, Space, l_, i_, f_, t_, e_, d_, Exclam, 0xFF};
/*0x1C9*/ u8 obtaineditem[] = {BuffCharac, 15, Space, o_, b_, t_, a_, i_, n_, e_, d_, Space, BuffCharac, 22, Exclam, 0xFF};
/*0x1CA*/ u8 tailwind_begins_text[] = {T_, h_, e_, 0, T_, a_, i_, l_, w_, i_, n_, d_, 0, b_, l_, e_, w_, 0, f_, r_, o_, m_, 0xFE, b_, e_, h_, i_, n_, d_, 0, 0xFD, 0xF, Apos, s_, 0, t_, e_, a_, m_, Exclam, 0xFF};
/*0x1CB*/ u8 luckychant_begins_text[] = {T_, h_, e_, 0, l_, u_, c_, k_, y_, 0, c_, h_, a_, n_, t_, 0, s_, h_, i_, e_, l_, d_, e_, d_, 0, 0xFD, 0xF, Apos, s_, 0xFE, t_, e_, a_, m_, 0, f_, r_, o_, m_, 0, c_, r_, i_, t_, i_, c_, a_, l_, 0, h_, i_, t_, s_, Exclam, 0xFF};
/*0x1CC*/ u8 magnetrise_begins_text[] = {0xFD, 0xF, 0, l_, e_, v_, i_, t_, a_, t_, e_, d_, 0xFE, w_, i_, t_, h_, 0, e_, l_, e_, c_, t_, r_, o_, m_, a_, g_, n_, e_, t_, i_, s_, m_, Exclam, 0xFF};
/*0x1CD*/ u8 magicroom_start_text[] = {I_, t_, 0, c_, r_, e_, a_, t_, e_, d_, 0, a_, 0, b_, i_, z_, a_, r_, r_, e_, 0, a_, r_, e_, a_, 0, i_, n_, 0, w_, h_, i_, c_, h_, 0xFE, P_, o_, k_, Poke_e, m_, o_, n_, Apos, s_, 0, h_, e_, l_, d_, 0, i_, t_, e_, m_, s_, 0, l_, o_, s_, e_, 0, t_, h_, e_, i_, r_, 0xFA, e_, f_, f_, e_, c_, t_, s_, Exclam, 0xFF};
/*0x1CE*/ u8 trickroom_start_text[] = {0xFD, 0xF, 0, t_, w_, i_, s_, t_, e_, d_, 0xFE, t_, h_, e_, 0, d_, i_, m_, e_, n_, s_, i_, o_, n_, s_, Exclam, 0xFF};
/*0x1CF*/ u8 wonderroom_start_text[] = {I_, t_, 0, c_, r_, e_, a_, t_, e_, d_, 0, a_, 0, b_, i_, z_, a_, r_, r_, e_, 0, a_, r_, e_, a_, 0, i_, n_, 0, w_, h_, i_, c_, h_, 0xFE, D_, e_, f_, e_, n_, s_, e_, 0, a_, n_, d_, 0, S_, p_, Dot, 0, D_, e_, f_, 0, s_, t_, a_, t_, s_, 0, a_, r_, e_, 0, s_, w_, a_, p_, p_, e_, d_, Exclam, 0xFF};
/*0x1D0*/ u8 gravitystarts_text[] = {G_, r_, a_, v_, i_, t_, y_, Space, i_, n_, t_, e_, n_, s_, i_, f_, i_, e_, d_, Exclam, 0xFF};
/*0x1D1*/ u8 telekinesis_start_text[] = {0xFD, 16, Space, w_, a_, s_, Space, h_, u_, r_, l_, e_, d_, JumpLine, i_, n_, t_, o_, Space, t_, h_, e_, Space, a_, i_, r_, Exclam, 0xFF};
/*0x1D2*/ u8 abilitychange_text[] = {0xFD, 16, Apos, Space, a_, b_, i_, l_, i_, t_, y_, JumpLine, b_, e_, c_, a_, m_, e_, Space, 0xFD, 0x19, Exclam, 0xFF};
/*0x1D3*/ u8 statswap_text[] = {0xFD, 15, Space, s_, w_, i_, t_, c_, h_, e_, d_, Space, a_, l_, l_, Space, s_, t_, a_, t_, JumpLine, c_, h_, a_, n_, g_, e_, s_, Space, t_, o_, Space, i_, t_, s_, Space, 0xFD, 0, Space, a_, n_, d_, Space, 0xFD, 1, 0xFA, w_, i_, t_, h_, Space, t_, h_, e_, Space, t_, a_, r_, g_, e_, t_, Exclam, 0xFF};
/*0x1D4*/ u8 heartswap_text[] = {0xFD, 15, Space, s_, w_, i_, t_, c_, h_, e_, d_, Space, s_, t_, a_, t_, JumpLine, c_, h_, a_, n_, g_, e_, s_, Space, w_, i_, t_, h_, Space, t_, h_, e_, Space, t_, a_, r_, g_, e_, t_, Exclam, 0xFF};
/*0x1D5*/ u8 bugbite_text[] = {0xFD, 15, Space, s_, t_, o_, l_, e_, Space, a_, n_, d_, Space, a_, t_, e_, JumpLine, 0xFD, 16, Apos, Space, 0xFD, 22, Exclam, 0xFF};
/*0x1D6*/ u8 incinerate_text[] = {0xFD, 16, Apos, s_, Space, 0xFD, 22, Space, w_, a_, s_, JumpLine, b_, u_, r_, n_, t_, Space, u_, p_, Exclam, 0xFF};
/*0x1D7*/ u8 gravitybringsdown_text[] = {0xFD, 15, Space, c_, o_, u_, l_, d_, Space, n_, o_, t_, Space, s_, t_, a_, y_, JumpLine, a_, i_, r_, b_, o_, r_, n_, e_, Space, b_, e_, c_, a_, u_, s_, e_, Space, o_, f_, Space, G_, r_, a_, v_, i_, t_, y_, Exclam, 0xFF};
/*0x1D8*/ u8 fellforfeint[] = {0xFD, 16, Space, f_, e_, l_, l_, Space, f_, o_, r_, Space, t_, h_, e_, Space, f_, e_, i_, n_, t_, Exclam, 0xFF};
/*0x1D9*/ u8 protection_broken[] = {0xFD, 15, Space, b_, r_, o_, k_, e_, Space, t_, h_, r_, o_, u_, g_, h_, JumpLine, 0xFD, 16, Space, p_, r_, o_, t_, e_, c_, t_, i_, o_, n_, Exclam, 0xFF};
/*0x1DA*/ u8 teamprotection[] = {0xFD, 0, Space, p_, r_, o_, t_, e_, c_, t_, s_, JumpLine, 0xFD, 15, Apos, Space, t_, e_, a_, m_, Exclam, 0xFF};
/*0x1DB*/ u8 becameatype[] = {0xFD, 0, Space, t_, y_, p_, e_, Space, w_, a_, s_, Space, a_, d_, d_, e_, d_, Space, t_, o_, JumpLine, 0xFD, 16, Exclam, 0xFF};
/*0x1DC*/ u8 targetsabilitybecameattacker[] = {0xFD, 16, Space, a_, c_, q_, u_, i_, r_, e_, d_, Space, 0xFD, 25, Exclam, 0xFF};
/*0x1DD*/ u8 gastro_text[] = {0xFD, 16, Apos, s_, Space, A_, b_, i_, l_, i_, t_, y_, Space, w_, a_, s_, Space, s_, u_, r_, p_, a_, s_, s_, e_, d_, Exclam, 0xFF};
/*0x1DE*/ u8 embargostart_text[] = {BuffCharac, 16, Space, c_, a_, n_, Apos, t_,Space, u_, s_, e_, JumpLine, i_, t_, e_, m_, s_, Space, a_, n_, y_, m_, o_, r_, e_, Exclam, 0xFF};
/*0x1DF*/ u8 afteryout_text[] = {BuffCharac, 16, Space, t_, o_, o_, k_, Space, t_, h_, e_, Space, k_, i_, n_, d_, Space, o_, f_, f_, e_, r_, Exclam, 0xFF};
/*0x1E0*/ u8 powder_text[] = {BuffCharac, 16, Space, i_, s_, JumpLine, c_, o_, v_, e_, r_, e_, d_, Space, i_, n_, Space, p_, o_, w_, d_, e_, r_, Exclam, 0xFF};
/*0x1E1*/ u8 powderdamage_text[] = {W_, h_, e_, n_, Space, t_, h_, e_, Space, f_, l_, a_, m_, e_, Space, t_, o_, u_, c_, h_, e_, d_, Space, t_, h_, e_, Space, p_, o_, w_, d_, e_, r_, JumpLine, o_, n_, Space, t_, h_, e_, Space, 0xFD, 15, Comma, Space, i_, t_, Space, e_, x_, p_, l_, o_, d_, e_, d_, Exclam, 0xFF};
/*0x1E2*/ u8 statchangesremoved_text[] = {0xFD, 17, Apos, s_, Space, s_, t_, a_, t_, s_, Space, c_,h_,a_,n_,g_,e_,s_, JumpLine, w_, e_, r_, e_, Space, r_, e_, m_, o_, v_, e_, d_, Exclam, 0xFF};
/*0x1E3*/ u8 electify_text[] = {0xFD, 16, Apos, s_, Space, m_, o_, v_, e_, s_, Space, h_, a_, v_, e_, Space, b_, e_, e_, n_, JumpLine, e_, l_, e_, c_, t_, r_, i_, f_, i_, e_, d_, Exclam, 0xFF};
/*0x1E4*/ u8 stealthrock2_text[] = {P_, o_, i_, n_, t_, e_, d_, Space, s_, t_, o_, n_, e_, s_, Space, f_, l_, o_, a_, t_, Space, a_, l_, l_, Space, a_, r_, o_, u_, n_, d_, JumpLine, t_, h_, e_, Space, o_, p_, p_, o_, n_, e_, n_, t_, Apos, s_, Space, s_, i_, d_, e_, Exclam, 0xFF};
/*0x1E5*/ u8 toxicspikes2_text[] = {P_, o_, i_, s_, o_, n_, Space, s_, p_, i_, k_, e_, s_, Space, w_, e_, r_, e_, Space, s_, c_, a_, t_, t_, e_, r_, e_, d_, JumpLine, a_, l_, l_, Space, a_, r_, o_, u_, n_, d_, Space, t_, h_, e_, Space, o_, p_, p_, o_, n_, e_, n_, t_, Apos, s_, Space, s_, i_, d_, e_, Exclam, 0xFF};
/*0x1E6*/ u8 stickyweb2_text[] = {A_, Space, s_, t_, i_, c_, k_, y_, Space, w_, e_, b_, Space, s_, p_, r_, e_, a_, d_, s_, Space, o_, u_, t_, Space, b_, e_, n_, e_, a_, t_, h_, JumpLine, t_, h_, e_, Space, o_, p_, p_, o_, n_, e_, n_, t_, Apos, s_, Space, s_, i_, d_, e_, Exclam, 0xFF};
/*0x1E7*/ u8 nimble_text[] = {0xFD, 15, Space, b_, e_, c_, a_, m_, e_, Space, n_, i_, m_, b_, l_, e_, Exclam, 0xFF};
/*0x1E8*/ u8 iondelugeset_text[] = {A_, Space, d_, e_, l_, u_, g_, e_, Space, o_, f_, Space, i_, o_, n_, s_, Space, s_, h_, o_, w_, e_, r_, s_, JumpLine, t_, h_, e_, Space, b_, a_, t_, t_, l_, e_, f_, i_, e_, l_, d_, Exclam, 0xFF};
/*0x1E9*/ u8 reflecttype_text[] = {0xFD, 15, Apos, s_, Space, t_, y_, p_, e_, Space, b_, e_, c_, a_, m_, e_, Space, t_, h_, e_, JumpLine, s_, a_, m_, e_, Space, a_, s_, Space, 0xFD, 16, Apos, s_, Space, t_, y_, p_, e_, Exclam, Termin};
/*0x1EA*/ u8 healblock_start_text[] = {0xFD, 16, Space, w_, a_, s_, Space, p_, r_, e_, v_, e_, n_, t_, e_, d_, JumpLine, f_, r_, o_, m_, Space, h_, e_, a_, l_, i_, n_, g_, Exclam, 0xFF};
/*0x1EB*/ u8 smackdown_text[] = {0xFD, 16, Space, f_, e_, l_, l_, Space, s_, t_, r_, a_, i_, g_, h_, t_, Space, d_, o_, w_, n_, Exclam, 0xFF};
/*0x1EC*/ u8 rapidspinontoxicspikes_text[] = {T_, h_, e_, Space, p_, o_, i_, s_, o_, n_, Space, s_, p_, i_, k_, e_, s_, Space, d_, i_, s_, a_, p_, p_, e_, a_, r_, e_, d_, JumpLine, f_, r_, o_, m_, Space, a_, r_, o_, u_, n_, d_, Space, 0xFD, 15, Apos, s_, Space, s_, i_, d_, e_, Exclam, 0xFF};
/*0x1ED*/ u8 rapidspinonstealthrock_text[] = {T_, h_, e_, Space, p_, o_, i_, n_, t_, e_, d_, Space, s_, t_, o_, n_, e_, s_, Space, d_, i_, s_, a_, p_, p_, e_, a_, r_, e_, d_, JumpLine, f_, r_, o_, m_, Space, a_, r_, o_, u_, n_, d_, Space, 0xFD, 15, Apos, s_, Space, s_, i_, d_, e_, Exclam, 0xFF};
/*0x1EE*/ u8 rapidspinonstickyweb_text[] = {T_, h_, e_, Space, s_, t_, i_, c_, k_, y_, Space, w_, e_, b_, Space, d_, i_, s_, a_, p_, p_, e_, a_, r_, e_, d_, JumpLine, f_, r_, o_, m_, Space, b_, e_, n_, e_, a_, t_, h_, Space, 0xFD, 15, Apos, s_, Space, s_, i_, d_, e_, Exclam, 0xFF};
/*0x1EF*/ u8 powertrick_text[] = {0xFD, 15, Space, s_, w_, i_, t_, c_, h_, e_, d_, JumpLine, i_, t_, s_, Space, A_, t_, t_, a_, c_, k_, Space, a_, n_, d_, Space, D_, e_, f_, e_, n_, s_, e_, Exclam, 0xFF};
/*0x1F0*/ u8 soak_text[] = {0xFD, 16, Space, t_, r_, a_, s_, f_, o_, r_, m_, e_, d_, JumpLine, i_, n_, t_, o_, Space, t_, h_, e_, Space, 0xFD, 0, Space, t_, y_, p_, e_, Exclam, 0xFF};
/*0x1F1*/ u8 defogspikes_text[] = {T_, h_, e_, Space, s_, p_, i_, k_, e_, s_, Space, d_, i_, s_, a_, p_, p_, e_, a_, r_, e_, d_, JumpLine, f_, r_, o_, m_, Space, a_, r_, o_, u_, n_, d_, Space, 0xFD, 15, Apos, s_, Space, s_, i_, d_, e_, Exclam, 0xFF};
/*0x1F2*/ u8 power_text[] = {p_, o_, w_, e_, r_, 0xFF};
/*0x1F3*/ u8 guard_text[] = {g_, u_, a_, r_, d_, 0xFF};
/*0x1F4*/ u8 psychosplit_text[] = {0xFD, 15, Space, s_, h_, a_, r_, e_, d_, Space, i_, t_, s_, JumpLine, 0xFD, 0, Space, w_, i_, t_, h_, Space, t_, h_, e_, Space, t_, a_, r_, g_, e_, t_, Exclam, 0xFF};
/*0x1F5*/ u8 stockpileend_text[] = {0xFD, 15, Apos, s_, Space, S_, t_, o_, c_, k_, p_, i_, l_, e_, JumpLine, e_, f_, f_, e_, c_, t_, Space, w_, o_, r_, e_, Space, o_, f_, f_, Exclam, 0xFF};
/*0x1F6*/ u8 geomancy_text[] = {0xFD, 15, Space, i_, s_, Space, a_, b_, o_, r_, b_, i_, n_,g_, Space, p_, o_, w_, e_, r_, Exclam, 0xFF};
/*0x1F7*/ u8 powerherb_text[] = {0xFD, 15, Space, b_, e_, c_, a_, m_, e_, Space, f_, u_, l_, l_, y_, Space, c_, h_, a_, r_, g_, e_, d_, JumpLine, d_, u_, e_, Space, t_, o_, Space, 0xFD, 22, Exclam, 0xFF};
/*0x1F8*/ u8 iceburn_text[] = {0xFD, 15, Space, b_, e_, c_, a_, m_, e_, Space, c_, l_, o_, a_, k_, e_, d_, JumpLine, i_, n_, Space ,f_, r_, e_, e_, z_, i_, n_, g_, Space, a_, i_, r_, Exclam, 0xFF};
/*0x1F9*/ u8 freezeshock_text[] = {0xFD, 15, Space, b_, e_, c_, a_, m_, e_, Space, c_, l_, o_, a_, k_, e_, d_, JumpLine, i_, n_, Space ,f_, r_, e_, e_, z_, i_, n_, g_, Space, l_, i_, g_, h_, t_, Exclam, 0xFF};\
/*0x1FA*/ u8 shadowforce_text[] = {0xFD, 15, Space, v_, a_, n_, i_, s_, h_, e_, d_, Space, i_, n_, s_, t_, a_, n_, t_, l_, y_, Exclam, 0xFF};
/*0x1FB*/ u8 mistyterrain_text[] = {M_, i_, s_, t_, Space, s_, w_, i_, r_, l_, e_, d_, Space, a_, b_, o_, u_, t_, Space, t_, h_, e_, Space, b_, a_, t_, t_, l_, e_, f_, i_, e_, l_, d_, Exclam, 0xFF};
/*0x1FC*/ u8 grassyterrain_text[] = {G_, r_, a_, s_, s_, Space, g_, r_, e_, w_, Space, t_, o_, Space, c_, o_, v_, e_, r_, Space, t_, h_, e_, Space, b_, a_, t_, t_, l_, e_, f_, i_, e_, l_, d_, Exclam, 0xFF};
/*0x1FD*/ u8 electricterrain_text[] = {A_, n_, Space, e_, l_, e_, c_, t_, r_, i_, c_, Space, c_, u_, r_, r_, e_, n_, t_, JumpLine, r_, u_, n_, s_, Space, a_, c_, r_, o_, s_, s_, Space, t_, h_, e_, Space, b_, a_, t_, t_, l_, e_, f_, i_, e_, l_, d_, Exclam, 0xFF};
/*0x1FE*/ u8 aquaring_text[] = {0xFD, 15, Space, s_, u_, r_, r_, o_, u_, n_, d_, e_, d_, Space, i_, t_, s_, e_, l_, f_, JumpLine, w_, i_, t_, h_, Space, a_, Space, v_, e_, i_, l_, Space, o_, f_, Space, w_, a_, t_, e_, r_,Exclam, 0xFF};
/*0x1FF*/ u8 aquaringheal_text[] = {A_, q_, u_, a_, Space, R_, i_, n_,g_, Space, r_, e_, s_, t_, o_, r_, e_, d_, Space, 0xFD, 15, Apos, s_, Space, H_, P_, Exclam, 0xFF};
/*0x200*/ u8 assaultvest_text[] = {T_, h_, e_, Space, e_, f_, f_, e_, c_, t_, s_, Space, o_, f_, Space, 0xFD, 22, JumpLine, p_, r_, e_, v_, e_, n_, t_, Space, u_, s_, i_, n_, g_, Space, s_, t_, a_, t_, u_, s_, Space, m_, o_, v_, e_, s_, Exclam, 0xFB, 0xFF};
/*0x201*/ u8 gravityprevents2_text[] = {0xFD, 18, Space, c_, a_, n_, Apos, t_, Space, u_, s_, e_, JumpLine, 0xFD, 20, Space, b_, e_, c_, a_, u_, s_, e_, Space, o_, f_, Space, g_, r_, a_, v_, i_, t_, y_, Exclam, 0xFB, 0xFF};
/*0x202*/ u8 healblockprevents2_text[] = {0xFD, 18, Space, c_, a_, n_, Apos, t_, Space, u_, s_, e_, JumpLine, 0xFD, 20, Space, b_, e_, c_, a_, u_, s_, e_, Space, o_, f_, Space, H_, e_, a_, l_, Space, B_, l_, o_, c_, k_, Exclam, 0xFB, 0xFF};
/*0x203*/ u8 let_it_move_first_text[] = {0xFD, 15, Apos, s_, Space, 0xFD, 0x16, Space, l_, e_, t_, s_, JumpLine, i_, t_, Space, m_, o_, v_, e_, Space, f_, i_, r_, s_, t_, Exclam, Termin };
/*0x204*/ u8 mega_evolved_text[] = {0xFD, 15, Space, h_, a_, s_, Space, M_, e_, g_, a_, Space, E_, v_, o_, l_, v_, e_, d_, JumpLine, i_, n_, t_, o_, Space, M_, e_, g_, a_, Space, 0xFD, 0, Exclam, 0xFB, 0xFF};
/*0x205*/ u8 mega_trigger_text[] = {0xFD, 15, Apos, s_, Space, 0xFD, 22, Space, i_, s_, Space, r_, e_, a_, c_, t_, i_, n_, g_, JumpLine, t_, o_, Space , 0xFD, 52, Apos, s_, Space, 0xFD, 1, Exclam, 0xFB, 0xFF};
/*0x206*/ u8 fervent_trigger_text[] = {BuffCharac, 52, Apos, s_, Space, f_, e_, r_, v_, e_, n_, t_, Space, w_, i_, s_, h_, JumpLine, r_, e_, a_, c_, h_, e_, d_, Space, BuffCharac, 0xF, Exclam, Termin};
/*0x207*/ u8 quash_text[] = {0xFD, 16, Apos, s_, Space, m_, o_, v_, e_, JumpLine, w_, a_, s_, Space, p_, o_, s_, t_, p_, o_, n_, e_, d_, Exclam, 0xFF};
/*0x208*/ u8 allyswitch_text[] = {0xFD, 19, Space, a_, n_, d_, Space, 0xFD, 15, JumpLine, s_, w_, i_, t_, c_, h_, e_, d_, Space, p_, l_, a_, c_, e_, s_, Exclam, 0xFF};
/*0x209*/ u8 topsyturvy_text[] = {0xFD, 16, Apos, s_, Space, s_, t_, a_, t_, Space, c_, h_, a_, n_, g_, e_, s_, JumpLine, w_, e_, r_, e_, Space, a_, l_, l_, Space, r_, e_, v_, e_, r_, s_, e_, d_, Exclam, 0xFF};
/*0x20A*/ u8 bestow_text[] = {0xFD, 16, Space, r_, e_, c_, e_, v_, i_, e_, d_, Space, o_, n_, e_, JumpLine, 0xFD, 22, Space, f_, r_, o_, m_, Space, 0xFD, 15, Exclam, 0xFF};
/*0x20B*/ u8 statushealpoison_text[] = {0xFD, 0, Space, w_, a_, s_, Space, c_, u_, r_, e_, d_, Space, o_, f_, JumpLine, i_, t_, s_, Space, p_, o_, i_, s_, o_, n_, i_, n_, g_, Exclam, 0xFF};
/*0x20C*/ u8 statushealburn_text[] = {0xFD, 0, Space, h_, e_, a_, l_, e_, d_, JumpLine, i_, t_, s_, Space, b_, u_,r_, n_, Exclam, 0xFF};
/*0x20D*/ u8 statushealpar_text[] = {0xFD, 0, Space, w_, a_, s_, Space, c_, u_, r_, e_, d_, Space, o_, f_, JumpLine, p_, a_, r_, a_, l_, y_, s_, i_, s_, Exclam, 0xFF};
/*0x20E*/ u8 statushealslp_text[] = {0xFD, 0, Space, w_, o_, k_, e_, Space, u_, p_, Exclam, 0xFF};
/*0x20F*/ u8 statushealfrz_text[] = {0xFD, 0, Space, t_, h_, a_, w_, e_, d_, Space, o_, u_, t_, Exclam, 0xFF};

void* new_strings_table[] = {&sample_text, &snowwarning_text, &extreme_sun_activation_text, &heavyrain_activation_text, &mysticalaircurrent_activation_text, &forewarn_text, &slowstart_text, &anticipation_text, &dryskin_damage_text, &solarpower_text, &harvest_text, &healer_text, &pickup_text, &moldbreaker_text, &turboblaze_text, &terravolt_text, &downloadatk_text,
&downloadspatk_text, &absorbabilityboost_text , &absorbabilityimmune_text, &userteam_text/*0x190*/, &foeteam_text/*0x191*/,
&aftermath_text, &pickpocket_text, &mummy_text, &target_ability, &cursedbody_text, &drastically_text, &pressure_text, &unnerve_text, &aurabreak_text, &fairyaura_text, &darkaura_text, &frisk_text, //0x19D
&hurtbyitem_text, &got_burned_text, &got_badlypoisoned_text, &airballoon_text, &bad_dreams_text, &item_text, &rockyhelmet_text, &popped_text, &fellinlove_text, &healblockend_text, &magicbounce_text,
&angerpoint_text, &stealhrock_text, &stickyweb_text, &gotpoisoned_text, &absorbed_spikes_text, &lost_some_hp_text, &tauntended_text, &tormentended_text,
&healblockprevents_text, &gravityprevents_text, &embargoprevents_text, &aromaveilprevents_text, &spikyshield_damage, &symbiosispassing_text,
&restored_hp_text, &replacement_healed_text, &telekinesis_end_text, &embargoend_text, &magnetriseend_text, &wrapped_text,
&nofiremoves_text, &nowatermoves_text, &trickroom_ends, &magicroom_ends, &wonderoom_ends, &gravity_ends_text, &grassyterainends_text,
&mistyterrainends_text, &electrerrainends_text, &grassyterrain_heal, &fogcontinues_text, &fogends_text, &obtaineditem,
&tailwind_begins_text, &luckychant_begins_text, &magnetrise_begins_text, &magicroom_start_text, &trickroom_start_text, &wonderroom_start_text,
&gravitystarts_text, &telekinesis_start_text, &abilitychange_text, &statswap_text, &heartswap_text, &bugbite_text, &incinerate_text,
&gravitybringsdown_text, &fellforfeint, &protection_broken, &teamprotection, &becameatype, &targetsabilitybecameattacker,
&gastro_text, &embargostart_text, &afteryout_text, &powder_text, &powderdamage_text, &statchangesremoved_text, &electify_text,
&stealthrock2_text, &toxicspikes2_text, &stickyweb2_text, &nimble_text, &iondelugeset_text, &reflecttype_text, &healblock_start_text, &smackdown_text,
&rapidspinontoxicspikes_text, &rapidspinonstealthrock_text, &rapidspinonstickyweb_text, &powertrick_text, &soak_text, &defogspikes_text,
&power_text, &guard_text, &psychosplit_text, &stockpileend_text, &geomancy_text, &powerherb_text, &iceburn_text, &freezeshock_text,
&shadowforce_text, &mistyterrain_text, &grassyterrain_text, &electricterrain_text, &aquaring_text, &aquaringheal_text, &assaultvest_text,
&gravityprevents2_text, &healblockprevents2_text , &let_it_move_first_text, &mega_evolved_text, &mega_trigger_text, &fervent_trigger_text,
&quash_text, &allyswitch_text, &topsyturvy_text, &bestow_text, &statushealpoison_text, &statushealburn_text, &statushealpar_text,
&statushealslp_text, &statushealfrz_text};

void battle_string_loader(u16 string_id)
{
    string_id -= 0x17C;
    display_string_in_battle(new_strings_table[string_id]);
    return;
}

u8 count_party_pokemon(u8 bank)
{
    struct pokemon* poke;
    if (is_bank_from_opponent_side(bank))
        poke = &party_opponent[0];
    else
        poke = &party_player[0];
    u8 usable_pokes = 0;
    for (u8 i = 0; i < 6; i ++)
    {
        if (get_attributes(&poke[i], ATTR_CURRENT_HP, 0) != 0 && get_attributes(&poke[i], ATTR_IS_EGG, 0) != 1 && get_attributes(&poke[i], ATTR_SPECIES, 0) != 0)
            usable_pokes++;
    }
    return usable_pokes;
}

extern void call_ability_effects();

void apply_burn_animation()
{
    u8 active = active_bank;
    active_bank = bank_attacker;
    new_battlestruct.ptr->various.var1 = 0x10;
    new_battlestruct.ptr->various.var2 = 0;
    prepare_status_animation(0, 0, new_battlestruct.ptr->various.var1);
    mark_buffer_bank_for_execution(bank_attacker);
    active_bank = active;
    return;
}

void change_attacker_item()
{
    active_bank = bank_attacker;
    prepare_setattributes_in_battle(0, 2, 0, 4, &battle_participants[bank_attacker].held_item);
    mark_buffer_bank_for_execution(bank_attacker);
}

void try_to_raise_spd()
{
    if (battle_participants[new_battlestruct.ptr->various.active_bank].spd_buff != 0xC)
    {
        battle_scripting.stat_changer = 0x13;
        battlescript_push();
        battlescripts_curr_instruction = &changetargetstat_bs;
    }
    return;
}

void try_to_lower_def()
{
    if (battle_participants[new_battlestruct.ptr->various.active_bank].def_buff > 0)
    {
        battle_scripting.stat_changer = 0x92;
        battlescript_push();
        battlescripts_curr_instruction = &changetargetstat_bs;
    }
    return;
}

void changestatvar1()
{
    if (new_battlestruct.ptr->various.var1)
    {
        battle_scripting.stat_changer = new_battlestruct.ptr->various.var1;
        battlescript_push();
        battlescripts_curr_instruction = &changetargetstat_bs;
    }
    return;
}

void changestatvar2()
{
    if (new_battlestruct.ptr->various.var2)
    {
        battle_scripting.stat_changer = new_battlestruct.ptr->various.var2;
        battlescript_push();
        battlescripts_curr_instruction = &changetargetstat_bs;
    }
    return;
}

void frisk_target_item()
{   u16 curr_item=battle_participants[bank_target].held_item;
    if (curr_item)
    {
        last_used_item=curr_item;
        b_std_message(0x19D,battle_scripting.active_bank);
        record_usage_of_item(bank_target, get_item_effect(bank_target, 0));
        battle_communication_struct.is_message_displayed=1;
    }
    else
        battle_communication_struct.is_message_displayed=0;

    return;
}

void set_stat_msg_buffer()
{
    u8 stat=battle_scripting.stat_changer&0xF;
    battle_text_buff1[0] = 0xFD;
    battle_text_buff1[1] = 5;
    battle_text_buff1[2] = stat;
    battle_text_buff1[3] = 0xFF;
}

void set_type_msg_buffer()
{
    u8 move_type=battle_stuff_ptr.ptr->dynamic_move_type&0x3F;
    if (!move_type)
        move_type = move_table[current_move].type;
    battle_text_buff1[0] = 0xFD;
    battle_text_buff1[1] = 3;
    battle_text_buff1[2] = move_type;
    battle_text_buff1[3] = 0xFF;
}

void set_team_msg_buffer()
{
    battle_text_buff1[0] = 0xFD;
    battle_text_buff1[1] = 0;
    if (is_bank_from_opponent_side(battle_scripting.active_bank))
        battle_text_buff1[2] = 0x90;
    else
        battle_text_buff1[2] = 0x91;
    battle_text_buff1[3] = 0x1;
    battle_text_buff1[4] = 0xFF;
}

u32 get_1_8_of_max_hp(u8 bank);

void bad_dreams_damage_calc()
{
    damage_loc=get_1_8_of_max_hp(bank_target);
}

void weaknesspolicy()
{
    if (battle_participants[battle_scripting.active_bank].sp_atk_buff != 0xC)
    {
        battlescript_push();
        battlescripts_curr_instruction = &weaknesspolicyspattack;
    }
    if (battle_participants[battle_scripting.active_bank].atk_buff != 0xC)
    {
        battlescript_push();
        battlescripts_curr_instruction = &weaknesspolicyattack;
    }
    return;
}

void mentalherb()
{
    u8 bank = battle_scripting.active_bank;
    if (disable_structs[bank].disable_timer)
    {
        disable_structs[bank].disable_timer = 0;
        disable_structs[bank].disabled_move = 0;
        battlescript_push();
        battlescripts_curr_instruction = &disable_end_bs;
    }
    if (disable_structs[bank].encore_timer)
    {
        disable_structs[bank].encore_timer = 0;
        disable_structs[bank].encored_move = 0;
        battlescript_push();
        battlescripts_curr_instruction = &encore_end_bs;
    }
    if (disable_structs[bank].taunt_timer)
    {
        disable_structs[bank].taunt_timer = 0;
        battlescript_push();
        battlescripts_curr_instruction = &taunt_end_bs;
    }
    if (new_battlestruct.ptr->bank_affecting[bank].heal_block)
    {
        new_battlestruct.ptr->bank_affecting[bank].heal_block = 0;
        battlescript_push();
        battlescripts_curr_instruction = &healblock_end_bs;
    }
    return;
}

void placeholder0x14()
{
    return;
}

void hazards_bank_switcher()
{
    new_battlestruct.ptr->various.active_bank = bank_target;
    bank_target = new_battlestruct.ptr->various.var1;
    return;
}

void hazards_bank_return()
{
    bank_target = new_battlestruct.ptr->various.active_bank;
    return;
}

u32 percent_boost(u32 number, u16 percent);

void leechseed_update()
{
    if (new_battlestruct.ptr->bank_affecting[bank_target].heal_block)
        damage_loc = 0;
    else if (get_item_effect(bank_target, 1) == ITEM_EFFECT_BIGROOT)
        damage_loc = percent_boost(damage_loc, 30);
}

void callasm_stat_change(u8 bank, void* battlescript_ptr)
{
    battle_scripting.stat_changer = read_byte(battlescripts_curr_instruction);
    battlescripts_curr_instruction++;
    battlescript_push();
    battlescripts_curr_instruction = battlescript_ptr;
    return;
}

void target_stat_change()
{
    callasm_stat_change(bank_target, &changetargetstat_bs);
    return;
}

void attacker_stat_change()
{
    callasm_stat_change(bank_attacker, &changeattackerstat_bs);
    return;
}

void moxie_stat_raise()
{
    if ((current_move == MOVE_BLOCK || check_ability(bank_attacker, ABILITY_MOXIE)) && bank_attacker != bank_target && DAMAGING_MOVE)
    {
        //check if that's the last opponent pokemon, if yes, then we'll skip the animation
        if (is_bank_from_opponent_side(bank_target))
        {
            for (u8 i = 0; i < 6; i++)
            {
                if (!(get_attributes(&party_opponent[i], ATTR_CURRENT_HP, 0) == 0 || get_attributes(&party_opponent[i], ATTR_IS_EGG, 0) || get_attributes(&party_opponent[i], ATTR_SPECIES, 0) == 0))
                    break;
                if (i == 5)
                    return;
            }
        }
        battlescript_push();
        battlescripts_curr_instruction = &changeattackerstat_bs;
        battle_scripting.stat_changer = 0x21;
    }
    return;
}

void grassyterrainn_heal()
{
    for (u8 i = 0; i < no_of_all_banks; i++)
    {
        if (!new_battlestruct.ptr->bank_affecting[i].grassyterrain_heal && get_airborne_state(i, 0, 1) <= 2 && battle_participants[i].current_hp < battle_participants[i].max_hp && !SEMI_INVULNERABLE(i))
        {
            new_battlestruct.ptr->bank_affecting[i].grassyterrain_heal = 1;
            battlescripts_curr_instruction -= 3;
            battlescript_push();
            battlescripts_curr_instruction = &grassyheal;
            battle_scripting.active_bank = i;
            damage_loc = get_1_16_of_max_hp(i) * -1;
            break;
        }
        if (i == no_of_all_banks)
            break;
    }
    return;
}

void callitemeffects()
{
    item_battle_effects(1, active_bank, 0);
    return;
}

void damagecalc2()
{
    u32 damage = 0;
    type_effectiveness_calc(current_move, get_attacking_move_type(), bank_attacker, bank_target, 1);
    move_outcome.super_effective = 0;
    move_outcome.not_very_effective = 0;
    switch (current_move)
    {
    case MOVE_DRAGON_RAGE:
        damage = 40;
        break;
    case MOVE_SONIC_BOOM:
        damage = 20;
        break;
    case MOVE_SEISMIC_TOSS:
    case MOVE_NIGHT_SHADE:
        damage = battle_participants[bank_attacker].level;
        break;
    case MOVE_SUPER_FANG:
        damage = battle_participants[bank_target].current_hp / 2;
        break;
    case MOVE_FINAL_GAMBIT:
        damage = battle_participants[bank_attacker].current_hp;
        break;
    case MOVE_PSYWAVE:
        damage = battle_participants[bank_attacker].level * 10 * (__umodsi3(rng(),16) + 5) / 100;
        break;
    case MOVE_ENDEAVOR:
        if (battle_participants[bank_attacker].current_hp > battle_participants[bank_target].current_hp)
        {
            battlescripts_curr_instruction = (void*) 0x082D9F1A;
            return;
        }
        damage = battle_participants[bank_target].current_hp - battle_participants[bank_attacker].current_hp;
        break;
    }
    if (damage < 1)
        damage = 1;
    damage_loc = damage;
    return;
}

void set_statchanger()
{
    battle_scripting.stat_changer = move_table[current_move].arg1;
}

void checksubstitute()
{
    if (affected_by_substitute(bank_target))
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
        battlescripts_curr_instruction += 4;
}

void statchangeeffect()
{
    u8 stat = move_table[current_move].arg1;
    if (stat & 0x80)
    {
        stat ^= 0x80;
        battle_communication_struct.move_effect = stat + 7;
    }
    else
        battle_communication_struct.move_effect = stat;
    return;
}

u8 checkifstatchangehaseffect()
{
    u8 stats_to_change = move_table[current_move].arg1;
    s8 by_how_much = move_table[current_move].arg2;
    u8 bank = bank_target;
    if (move_table[current_move].target == move_target_user)
        bank = bank_attacker;
    for (u8 i = 0; i < 7; i++)
    {
        if (stats_to_change & bits_table[i])
        {
            u8* stat_ptr = &battle_participants[bank].atk_buff + i;
            if (by_how_much < 0)
            {
                if (*stat_ptr != 0)
                    return 1;
            }
            else
            {
                if (*stat_ptr != 0xC)
                    return 1;
            }
        }
    }
    return 0;
}

void statcheck_return()
{
    new_battlestruct.ptr->various.var1 = 0;
    if (checkifstatchangehaseffect())
        return;
    battlescripts_curr_instruction = &return_bs;
}

void doublestatchange_check()
{
    new_battlestruct.ptr->various.var1 = 0;
    if (checkifstatchangehaseffect())
        battlescripts_curr_instruction += 4;
    else
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
}

u8 bit_to_stat(u8 value)
{
    if (value & 1)
        return 1;
    if (value & 2)
        return 2;
    if (value & 4)
        return 3;
    if (value & 8)
        return 4;
    if (value & 0x10)
        return 5;
    if (value & 0x20)
        return 6;
    if (value & 0x40)
        return 7;
    return 0;
}

void dostatchanges()
{
    u16* done_stats = &new_battlestruct.ptr->various.var1;
    u8 stats_to_change = move_table[current_move].arg1;
    s8 by_how_much = move_table[current_move].arg2;
    u8 affects_user = 0x40;
    bank_partner_def = bank_attacker;
    if (move_table[current_move].target != move_target_user && move_table[current_move].base_power == 0 && move_table[current_move].split == 2)
    {
        affects_user = 0;
        bank_partner_def = bank_target;
    }
    for (u8 i = 0; i < 7; i++)
    {
        if (stats_to_change & bits_table[i] && !(*done_stats & bits_table[i]))
        {
            void* stat_msg = 0;
            *done_stats |= bits_table[i];
            if (change_stats(by_how_much, bit_to_stat(bits_table[i]), affects_user, 0) == 0) //it worked
            {
                if (battle_scripting.stat_changer & 0x80)
                    stat_msg = &stat_lower;
                else
                    stat_msg = &stat_raise;
            }
            else
            {
                if (battle_scripting.stat_changer & 0x80)
                    stat_msg = &cant_lower_bs;
                else
                    stat_msg = &cant_raise_bs;
            }
            battlescripts_curr_instruction -= 3;
            battlescript_push();
            battlescripts_curr_instruction = stat_msg;
            break;
        }
    }
    return;
}

void checkifcanconfuse()
{
    void* failaddress = (void*) read_word(battlescripts_curr_instruction);
    u8 fail = 0;
    if (battle_participants[bank_target].status2.confusion)
    {
        u8 stat_to_change = move_table[current_move].arg1;
        if (stat_to_change == 0)
            fail = 1;

        u8* stat_ptr = &battle_participants[bank_target].hp_buff + (stat_to_change & 7);
        if (stat_to_change & 0x80)
        {
            if (*stat_ptr == 0)
                fail = 1;
        }
        else
        {
            if (*stat_ptr == 0xC)
                fail = 1;
        }
    }
    if (fail)
        battlescripts_curr_instruction = failaddress;
    else
        battlescripts_curr_instruction += 4;
    return;
}

void confuse_applystatchange()
{
    u8 stat_to_change = move_table[current_move].arg1;
    battle_scripting.stat_changer = stat_to_change;
    if (stat_to_change !=0 && change_stats(stat_to_change & 0xF0, stat_to_change & 7, 0, 0) == 0)
    {
         bank_partner_def = bank_target;
         battlescript_push();
         if (battle_scripting.stat_changer & 0x80)
             battlescripts_curr_instruction = &stat_lower;
         else
             battlescripts_curr_instruction = &stat_raise;
    }
    return;
}

void jumpifcantpoison()
{
    switch (cant_poison(bank_target, 0))
    {
    case 1: //already poisoned
        battlescripts_curr_instruction = (void*) 0x82D8F52;
        break;
    case 2: //other major condition
        battlescripts_curr_instruction = (void*) 0x82D9F1C;
        break;
    case 3: //type protection
        battlescripts_curr_instruction = (void*) 0x82D9F2E;
        break;
    case 4: //ability prevention
        last_used_ability = battle_participants[bank_target].ability_id;
        record_usage_of_ability(bank_target, last_used_ability);
        battlescripts_curr_instruction = (void*) 0x82D8F63;
        break;
    }
}

void jumpifcantparalyze()
{
    switch (cant_become_paralyzed(bank_target, 0))
    {
    case 1: //already paralyzed
        battlescripts_curr_instruction = (void*) 0x82D9351;
        break;
    case 2: //other major condition
        battlescripts_curr_instruction = (void*) 0x82D9F1C;
        break;
    case 3: //type protection
        battlescripts_curr_instruction = (void*) 0x82D9F2E;
        break;
    case 4: //ability prevention
        last_used_ability = battle_participants[bank_target].ability_id;
        record_usage_of_ability(bank_target, last_used_ability);
        battlescripts_curr_instruction = (void*) 0x82D9362;
        break;
    }
}

void jumpifcantburn()
{
    switch (cant_become_burned(bank_target, 0))
    {
    case 1:
        battlescripts_curr_instruction = (void*) 0x82DA0BB;
        break;
    case 2:
        battlescripts_curr_instruction = (void*) 0x82D9F1C;
        break;
    case 3:
        battlescripts_curr_instruction = (void*) 0x82D9F2E;
        break;
    }
}

void statustoeffect()
{
    u8 stat_to_change = move_table[current_move].arg1;
    if (stat_to_change & STATUS_BURN)
        battle_communication_struct.move_effect = 3;
    else if (stat_to_change & STATUS_FREEZE)
        battle_communication_struct.move_effect = 4;
    else if (stat_to_change & STATUS_SLEEP)
        battle_communication_struct.move_effect = 1;
    else if (stat_to_change & STATUS_PARALYSIS)
        battle_communication_struct.move_effect = 5;
    else if (stat_to_change & STATUS_POISON)
        battle_communication_struct.move_effect = 2;
    else if (stat_to_change & STATUS_TOXIC_POISON)
        battle_communication_struct.move_effect = 6;
    else
        battle_communication_struct.move_effect = 0;
    return;
}

void half_hp_damage()
{
    damage_loc = battle_participants[get_battle_bank(read_byte(battlescripts_curr_instruction))].max_hp >> 1;
    if (damage_loc == 0)
        damage_loc = 1;
    battlescripts_curr_instruction++;
}

void jumpifonlyonepokemon()
{
    if (count_party_pokemon(bank_attacker) <= 1)
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
        battlescripts_curr_instruction += 4;
}

void setlunardanceeffect()
{
    new_battlestruct.ptr->side_affecting[is_bank_from_opponent_side(bank_attacker)].lunardance = 1;
    damage_loc = battle_participants[bank_attacker].current_hp;
    hitmarker |= HITMARKER_IGNORE_SUBSTITUTE;
    return;
}

u8 moveargweather_check(u8 arg)
{
    if (weather_abilities_effect())
    {
        if (arg & 1 && battle_weather.int_bw & (weather_downpour | weather_heavy_rain | weather_permament_rain | weather_rain))
            return 1;
        if (arg & 2 && battle_weather.int_bw & (weather_harsh_sun | weather_permament_sun | weather_sun))
            return 1;
        if (arg & 4 && battle_weather.int_bw & (weather_permament_sandstorm | weather_sandstorm))
            return 1;
        if (arg & 8 && battle_weather.int_bw & (weather_hail | weather_permament_hail))
            return 1;
        if (arg & 0x10 && battle_weather.int_bw & (weather_fog | weather_permament_fog))
            return 1;
        if (arg & 0x20 && battle_weather.int_bw & weather_air_current)
            return 1;
    }
    return 0;
}

void weatherhpheal()
{
    u8 bank = get_battle_bank(read_byte(battlescripts_curr_instruction));
    battlescripts_curr_instruction++;
    if (moveargweather_check(move_table[current_move].arg1) || find_move_in_table(current_move, &megalauncher_moves_table[0]))
        damage_loc = (battle_participants[bank].max_hp * 2 / 3) *-1;
    else if (moveargweather_check(move_table[current_move].arg2))
        damage_loc = percent_lose(battle_participants[bank].max_hp, 75) *-1;
}

void checkifcantransfercondition()
{
    struct battle_participant* attacker_struct = &battle_participants[bank_attacker];
    struct battle_participant* target_struct = &battle_participants[bank_target];
    u8 fail = 1;
    if (attacker_struct->status.flags.burn && !cant_become_burned(bank_target, 0))
    {
        target_struct->status.flags.burn = 1;
        fail = 0;
    }
    else if (attacker_struct->status.flags.freeze && !cant_become_freezed(bank_target, 0))
    {
        target_struct->status.flags.freeze = 1;
        fail = 0;
    }
    else if (attacker_struct->status.flags.sleep && !cant_fall_asleep(bank_target, 0))
    {
        target_struct->status.flags.sleep = attacker_struct->status.flags.sleep;
        fail = 0;
    }
    else if (attacker_struct->status.flags.paralysis && !cant_become_paralyzed(bank_target, 0))
    {
        target_struct->status.flags.paralysis = 1;
        fail = 0;
    }
    else if ((attacker_struct->status.flags.poison || attacker_struct->status.flags.toxic_poison) && !cant_become_freezed(bank_target, 0))
    {
        target_struct->status.flags.poison = attacker_struct->status.flags.poison;
        target_struct->status.flags.toxic_poison = attacker_struct->status.flags.toxic_poison;
        fail = 0;
    }
    if (fail)
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
    {
        battlescripts_curr_instruction += 4;
        active_bank = bank_target;
        prepare_setattributes_in_battle(0, REQUEST_STATUS_BATTLE, 0, 4, &target_struct->status);
        mark_buffer_bank_for_execution(bank_target);
    }
    return;

}

void choosestatusinflictiontext()
{
    u32 target_status = battle_participants[bank_target].status.int_status;
    u8 position = 0;
    if (target_status & STATUS_BURN)
        position = 3;
    else if (target_status & STATUS_FREEZE)
        position = 4;
    else if (target_status & STATUS_SLEEP)
        position = 1;
    else if (target_status & STATUS_PARALYSIS)
        position = 5;
    else if (target_status & STATUS_POISON)
        position = 2;
    else if (target_status & STATUS_TOXIC_POISON)
        position = 6;
    if (position)
    {
        bank_partner_def = bank_target;
        battlescript_push();
        battlescripts_curr_instruction = battlescripts_for_moveeffects[position];
        battle_stuff_ptr.ptr->synchronize_effect_chooser = position;
    }
}

void roostactivation()
{
    u16 max_hp = battle_participants[bank_attacker].max_hp;
    if (max_hp == battle_participants[bank_attacker].current_hp)
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
    {
        u16 half_hp = max_hp / 2;
        if (half_hp * 2 < max_hp)
            half_hp++; //rounding up
        damage_loc = half_hp * (-1);
        battlescripts_curr_instruction += 4;
        if (is_of_type(bank_attacker, TYPE_FLYING))
        {
            u8 *type1 = &battle_participants[bank_attacker].type1;
            u8 *type2 = &battle_participants[bank_attacker].type2;
            u8 type3 = new_battlestruct.ptr->bank_affecting[bank_attacker].type3;
            if (*type1 == TYPE_FLYING && *type2 == TYPE_FLYING && (type3 == TYPE_EGG || type3 == TYPE_FLYING))
            {//pure flying type
                set_type(bank_attacker, TYPE_NORMAL);
                new_battlestruct.ptr->bank_affecting[bank_attacker].roost = 4; //pure type-flying, becomes normal type
            }
            else
            {
                if (*type1 == TYPE_FLYING)
                {
                    if (*type2 != TYPE_FLYING)
                        *type1 = *type2;
                    else
                        *type1 = type3;
                    new_battlestruct.ptr->bank_affecting[bank_attacker].roost = 1;
                }
                else if (*type2 == TYPE_FLYING)
                {
                    if (*type1 != TYPE_FLYING)
                        *type2 = *type1;
                    else
                        *type2 = type3;
                    new_battlestruct.ptr->bank_affecting[bank_attacker].roost = 2;
                }
                else
                {
                    if (*type1 != TYPE_FLYING)
                        new_battlestruct.ptr->bank_affecting[bank_attacker].type3 = *type1;
                    else
                        new_battlestruct.ptr->bank_affecting[bank_attacker].type3 = *type2;
                    new_battlestruct.ptr->bank_affecting[bank_attacker].roost = 3;
                }
            }
        }
    }
    return;
}

void gravitysetter()
{
    if (new_battlestruct.ptr->field_affecting.gravity)
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
    {
        battlescripts_curr_instruction += 4;
        new_battlestruct.ptr->field_affecting.gravity = 5;
        new_battlestruct.ptr->various.var1 = 0;
    }
    return;
}

void gravity_ender()
{
    u8 effect = 1;
    void* instruction = 0;
    if (new_battlestruct.ptr->various.var1 == 0)
        new_battlestruct.ptr->various.var2 = bank_attacker; //bank attacker conserving
    for (u8 i = 0; i < no_of_all_banks; i++)
    {
        if (new_battlestruct.ptr->bank_affecting[i].telekinesis)
        {
            new_battlestruct.ptr->bank_affecting[i].telekinesis = 0;
            bank_attacker = i;
            instruction = &telekinesis_end_return_bs;
            effect = 1;
        }
        else if (new_battlestruct.ptr->bank_affecting[i].magnet_rise)
        {
            new_battlestruct.ptr->bank_affecting[i].magnet_rise = 0;
            bank_attacker = i;
            effect = 1;
            instruction = &magnetrise_end_return_bs;
        }
        else if (status3[i].on_air)
        {
            status3[i].on_air = 0;
            battle_participants[i].status2.multiple_turn_move = 0;
            bank_attacker = i;
            instruction = &gravitybringsdown_bs;
            effect = 1;
        }
        else if (check_ability(i, ABILITY_LEVITATE))
        {
            bank_attacker = i;
            instruction = &gravitybringsdown_bs;
            effect = 1;
        }
    }
    if (effect && instruction)
    {
        new_battlestruct.ptr->various.var1++;
        battlescripts_curr_instruction -= 3;
        battlescript_push();
        battlescripts_curr_instruction = instruction;
    }
    else if (!effect)
        bank_attacker = new_battlestruct.ptr->various.var2;
    return;
}

void setidentifierbit()
{
    if (new_battlestruct.ptr->bank_affecting[bank_target].miracle_eyed || battle_participants[bank_target].status2.foresight)
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
    {
        battlescripts_curr_instruction += 4;
        if (move_table[current_move].arg1)
            new_battlestruct.ptr->bank_affecting[bank_target].miracle_eyed = 1;
        else
            battle_participants[bank_target].status2.foresight = 1;
    }
    return;
}

void breakprotection()
{
    u8 banks_side = is_bank_from_opponent_side(bank_target);
    u8 effect = 0;
    if (protect_structs[bank_target].flag0_protect)
    {
        protect_structs[bank_target].flag0_protect = 0;
        effect = 1;
    }
    if (new_battlestruct.ptr->bank_affecting[bank_target].spiky_shield)
    {
        new_battlestruct.ptr->bank_affecting[bank_target].spiky_shield = 0;
        effect = 1;
    }
    if (new_battlestruct.ptr->bank_affecting[bank_target].kings_shield)
    {
        new_battlestruct.ptr->bank_affecting[bank_target].kings_shield = 0;
        effect = 1;
    }
    if (new_battlestruct.ptr->side_affecting[banks_side].quick_guard)
    {
        new_battlestruct.ptr->side_affecting[banks_side].quick_guard = 0;
        effect = 1;
    }
    if (new_battlestruct.ptr->side_affecting[banks_side].mat_block)
    {
        new_battlestruct.ptr->side_affecting[banks_side].mat_block = 0;
        effect = 1;
    }
    if (new_battlestruct.ptr->side_affecting[banks_side].wide_guard)
    {
        new_battlestruct.ptr->side_affecting[banks_side].wide_guard = 0;
        effect = 1;
    }
    if (new_battlestruct.ptr->side_affecting[banks_side].crafty_shield)
    {
        new_battlestruct.ptr->side_affecting[banks_side].crafty_shield = 0;
        effect = 1;
    }
    if (effect)
    {
        u8 arg1 = move_table[current_move].arg1;
        battle_communication_struct.multistring_chooser = 0 + (arg1 & 1);
    }
    else
        battle_communication_struct.multistring_chooser = 2;
    return;
}

void suckerpunchchecker()
{
    if (move_table[battle_participants[bank_target].moves[battle_stuff_ptr.ptr->chosen_move_position[bank_target]]].split == 2 ||
        get_bank_turn_order(bank_attacker) > get_bank_turn_order(bank_target))
          battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
        battlescripts_curr_instruction += 4;
}

void oppositegenderscheck()
{
    struct battle_participant* target_struct = &battle_participants[bank_target];
    struct battle_participant* attacker_struct = &battle_participants[bank_attacker];
    u8 gender_atk = gender_from_pid(attacker_struct->poke_species, attacker_struct->pid);
    u8 gender_def = gender_from_pid(target_struct->poke_species, target_struct->pid);
    if (gender_atk != 0xFF && gender_def != 0xFF && gender_atk != gender_def)
        battlescripts_curr_instruction += 4;
    else
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
}

void setthirdtype()
{
    u8 type = move_table[current_move].arg1;
    if (is_of_type(bank_target, type))
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
    {
        battlescripts_curr_instruction += 4;
        new_battlestruct.ptr->bank_affecting[bank_target].type3 = type;
        battle_text_buff1[0] = 0xFD;
        battle_text_buff1[1] = 0x3;
        battle_text_buff1[2] = type;
        battle_text_buff1[3] = 0xFF;
    }
    return;
}

u8 forbidenabilitiestable1[] = {ABILITY_WONDER_GUARD, ABILITY_STANCE_CHANGE, ABILITY_ILLUSION, ABILITY_MULTITYPE, 0xFF};
u8 forbidenabilitiestable2[] = {ABILITY_MULTITYPE, ABILITY_STANCE_CHANGE, 0xFF}; //
u8 forbidenabilitiestable3[] = {ABILITY_WONDER_GUARD, ABILITY_STANCE_CHANGE, ABILITY_ILLUSION, ABILITY_MULTITYPE, ABILITY_FLOWER_GIFT, ABILITY_FORECAST, ABILITY_IMPOSTER, ABILITY_TRACE, ABILITY_ZEN_MODE, 0xFF};
u8 forbidenabilitiestable4[] = {ABILITY_MULTITYPE, ABILITY_TRUANT, ABILITY_STANCE_CHANGE, 0xFF};

u8 findability_in_table(u8 ability, u8 table[])
{
    for (u8 i = 0; table[i] != 0xFF; i++)
    {
        if (table[i] == ability)
            return 1;
    }
    return 0;
}

void ability_change()
{ //table goes Swapping/attackers change/targets change
    u8 fail = 0;
    u8* ability_atk = &battle_participants[bank_attacker].ability_id;
    u8* ability_def = &battle_participants[bank_target].ability_id;
    switch (move_table[current_move].arg1)
    {
        case 0: //swapping abilities
            if (findability_in_table(*ability_atk, &forbidenabilitiestable1[0]) || findability_in_table(*ability_def, &forbidenabilitiestable1[0]))
                fail = 1;
            else
            {
                u8 placeholder = *ability_atk;
                *ability_atk = *ability_def;
                *ability_def = placeholder;
                battle_communication_struct.multistring_chooser = 0;
            }
            break;
        case 1: //attacker's ability becomes target's
            if (*ability_atk == *ability_def || findability_in_table(*ability_atk, &forbidenabilitiestable2[0]) || findability_in_table(*ability_def, &forbidenabilitiestable3[0]))
                fail = 1;
            else
            {
                last_used_ability = *ability_atk = *ability_def;
                battle_communication_struct.multistring_chooser = 1;
            }
            break;
        case 2: //target's ability becomes attacker
            if (*ability_atk == *ability_def || findability_in_table(*ability_def, &forbidenabilitiestable4[0]) || findability_in_table(*ability_atk, &forbidenabilitiestable3[0]))
                fail = 1;
            else
            {
                last_used_ability = *ability_def = *ability_atk;
                battle_communication_struct.multistring_chooser = 2;
            }
            break;
        case 3: //target's ability becomes the one in arg2
            {
                u8 newability = move_table[current_move].arg2;
                if (*ability_def == newability || findability_in_table(newability, &forbidenabilitiestable4[0]))
                    fail = 1;
                else
                {
                    *ability_def = newability;
                    last_used_ability = newability;
                    battle_communication_struct.multistring_chooser = 2;
                }
            }
            break;
        default:
            fail = 1;
    }
    if (fail)
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
    {
        battlescripts_curr_instruction += 4;
        abilities_by_banks[bank_attacker] = *ability_atk;
        abilities_by_banks[bank_target] = *ability_def;
    }
    return;
}

void roomsetter()
{ //order of table trickoff, trickon, wonderoff, wonderon, magicoff, magicon
    switch (current_move)
    {
    case MOVE_TRICK_ROOM:
        if (new_battlestruct.ptr->field_affecting.trick_room)
        {
            battle_communication_struct.multistring_chooser = 0;
            new_battlestruct.ptr->field_affecting.trick_room = 0;
        }
        else
        {
            battle_communication_struct.multistring_chooser = 1;
            new_battlestruct.ptr->field_affecting.trick_room = 5;
        }
        break;
    case MOVE_WONDER_ROOM:
        if (new_battlestruct.ptr->field_affecting.wonder_room)
        {
            battle_communication_struct.multistring_chooser = 2;
            new_battlestruct.ptr->field_affecting.wonder_room = 0;
        }
        else
        {
            battle_communication_struct.multistring_chooser = 3;
            new_battlestruct.ptr->field_affecting.wonder_room = 5;
        }
        break;
    case MOVE_MAGIC_ROOM:
        if (new_battlestruct.ptr->field_affecting.magic_room)
        {
            battle_communication_struct.multistring_chooser = 4;
            new_battlestruct.ptr->field_affecting.magic_room = 0;
        }
        else
        {
            battle_communication_struct.multistring_chooser = 5;
            new_battlestruct.ptr->field_affecting.magic_room = 5;
        }
        break;
    }
    return;
}

void countercalc()
{
    u32 physical_damage = protect_structs[bank_attacker].physical_damage;
    u32 special_damage = protect_structs[bank_attacker].special_damage;
    u8 physical_bank = protect_structs[bank_attacker].counter_target;
    u8 special_target = protect_structs[bank_attacker].mirrorcoat_target;
    u8 attackers_side = is_bank_from_opponent_side(bank_attacker);
    u8 followme = 0;
    if (side_timers[attackers_side].followme_timer && battle_participants[side_timers[attackers_side].followme_target].current_hp)
        followme = 1;
    u32 damage = 0;
    u8 fail = 0;
    u8 damage_boots = 100;

    switch (current_move)
    {
    case MOVE_METAL_BURST:
        damage_boots = 50;
        if (special_damage)
            goto MIRRORCOAT;
        else if (physical_damage)
            goto COUNTER;
        else
            fail = 1;
        break;
    COUNTER:
    case MOVE_COUNTER:
        if (physical_damage)
        {
            if (followme)
                bank_target = side_timers[attackers_side].followme_target;
            else if (battle_participants[physical_bank].current_hp && is_bank_from_opponent_side(physical_bank) != attackers_side)
                bank_target = physical_bank;
            else
            {
                for (u8 i = 0; i < no_of_all_banks; i++)
                {
                    if (attackers_side != is_bank_from_opponent_side(i) && battle_participants[i].current_hp)
                        bank_target = i;
                    else if (i == no_of_all_banks)
                        fail = 1;
                }
            }
            damage = percent_boost(physical_damage, damage_boots);
        }
        else
            fail = 1;
        break;
    MIRRORCOAT:
    case MOVE_MIRROR_COAT:
        if (special_damage)
        {
            if (followme)
                bank_target = side_timers[attackers_side].followme_target;
            else if (battle_participants[special_target].current_hp && is_bank_from_opponent_side(special_target) != attackers_side)
                bank_target = special_target;
            else
            {
                for (u8 i = 0; i < no_of_all_banks; i++)
                {
                    if (attackers_side != is_bank_from_opponent_side(i) && battle_participants[i].current_hp)
                        bank_target = i;
                    else if (i == no_of_all_banks)
                        fail = 1;
                }
            }
            damage = percent_boost(special_damage, damage_boots);
        }
        else
            fail = 1;
        break;
    }
    if (fail)
    {
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
        special_statuses[bank_attacker].flag20 = 1;
    }
    else
    {
        battlescripts_curr_instruction += 4;
        damage_loc = damage;
    }
    return;
}

u16 get_average(u16 num1, u16 num2)
{
    return (num1 + num2) / 2;
}

void gastroacid()
{
    u16 targets_ability = battle_participants[bank_target].ability_id;
    if (targets_ability == ABILITY_MULTITYPE || targets_ability == ABILITY_STANCE_CHANGE)
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
    {
        battlescripts_curr_instruction += 4;
        new_battlestruct.ptr->bank_affecting[bank_target].gastro_acided = 1;
    }
    return;
}

void setembargo()
{
    if (new_battlestruct.ptr->bank_affecting[bank_target].embargo)
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
    {
        new_battlestruct.ptr->bank_affecting[bank_target].embargo = 5;
        battlescripts_curr_instruction += 4;
    }
}

void naturalgift()
{
    u16 item = battle_participants[bank_attacker].held_item;
    u8 effect = 1;
    if (battle_participants[bank_attacker].ability_id == ABILITY_KLUTZ || new_battlestruct.ptr->bank_affecting[bank_attacker].embargo || new_battlestruct.ptr->field_affecting.magic_room)
        effect = 0;
    if (!effect || !item || get_item_pocket_id(item) != 4)
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
        battlescripts_curr_instruction += 4;
}

void afteryou_check() //0 - first, 1 - second, 2 - third, 3 - fourth
{
    u8 target_turn = get_bank_turn_order(bank_target);
    u8 attacker_turn = get_bank_turn_order(bank_attacker);
    if (target_turn < attacker_turn || target_turn - 1 == attacker_turn)
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
    {
        battlescripts_curr_instruction += 4;
        if (attacker_turn == 0 && target_turn == 2)
        {
            turn_order[2] = turn_order[1];
            turn_order[1] = bank_target;
        }
        else if (attacker_turn == 0 && target_turn == 3)
        {
            turn_order[3] = turn_order[2];
            turn_order[2] = turn_order[1];
            turn_order[1] = bank_target;
        }
        else //attacker == 1 target == 3
        {
            turn_order[3] = turn_order[2];
            turn_order[2] = bank_target;
        }
    }
}

void powder_setter()
{
    new_battlestruct.ptr->bank_affecting[bank_target].powder = 1;
}
void jumpifnoally()
{
    u8 ally_bank = bank_attacker ^ 2;
    if (ally_bank < no_of_all_banks && battle_participants[ally_bank].current_hp)
    {
        battlescripts_curr_instruction += 4;
        bank_target = ally_bank;
    }
    else
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
}

void electrify()
{
    if (get_bank_turn_order(bank_attacker) > get_bank_turn_order(bank_target) || new_battlestruct.ptr->bank_affecting[bank_target].electrify)
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
    {
        new_battlestruct.ptr->bank_affecting[bank_target].electrify = 1;
        battlescripts_curr_instruction += 4;
    }
}

void set_entry_hazards()
{
    u8 fail = 0;
    u8 targets_side = is_bank_from_opponent_side(bank_target);
    struct side_affecting* target_struct = &new_battlestruct.ptr->side_affecting[targets_side];
    u8* string_chooser = &battle_communication_struct.multistring_chooser;
    switch (current_move)
    {
    case MOVE_SPIKES:
        if (side_timers[targets_side].spikes_amount >= 3)
            fail = 1;
        else
        {
            side_timers[targets_side].spikes_amount++;
            side_affecting_halfword[targets_side].spikes_on = 1;
            *string_chooser = 0;
        }
        break;
    case MOVE_STEALTH_ROCK:
        if (target_struct->stealthrock)
            fail = 1;
        else
        {
            target_struct->stealthrock = 1;
            *string_chooser = 1;
        }
        break;
    case MOVE_TOXIC_SPIKES:
        *string_chooser = 2;
        if (target_struct->toxic_spikes_psn && target_struct->toxic_spikes_badpsn)
            fail = 1;
        else if(target_struct->toxic_spikes_psn)
            target_struct->toxic_spikes_badpsn = 1;
        else
            target_struct->toxic_spikes_psn = 1;
        break;
    case MOVE_STICKY_WEB:
        if (target_struct->sticky_web)
            fail = 1;
        else
        {
            *string_chooser = 3;
            target_struct->sticky_web = 1;
        }
        break;
    }
    if (fail)
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
        battlescripts_curr_instruction += 4;
}

void jumpifnotdoublebattle()
{
    if (battle_flags.double_battle)
        battlescripts_curr_instruction += 4;
    else
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
}

void jumpifattackerandtargetdontsharetypes()
{
    u8 type1 = battle_participants[bank_attacker].type1;
    u8 type2 = battle_participants[bank_attacker].type2;
    u8 type3 = new_battlestruct.ptr->bank_affecting[bank_attacker].type3;
    if ((is_of_type(bank_target, type1) && type1 != TYPE_EGG) || (is_of_type(bank_target, type2) && type2 != TYPE_EGG) || (is_of_type(bank_target, type3) && type3 != TYPE_EGG))
        battlescripts_curr_instruction += 4;
    else
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
}

void try_autotonomize()
{
    u8* auto_uses = &new_battlestruct.ptr->bank_affecting[bank_attacker].autonomize_uses;
    if (get_poke_weight(bank_attacker) == 1)
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
    {
        *auto_uses += 1;
        battlescripts_curr_instruction += 4;
    }
    return;
}

void set_iondeluge()
{
    if (new_battlestruct.ptr->field_affecting.ion_deluge)
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
    {
        battlescripts_curr_instruction += 4;
        new_battlestruct.ptr->field_affecting.ion_deluge = 1;
    }
}

void reflecttypes()
{
    new_battlestruct.ptr->bank_affecting[bank_attacker].type3 = new_battlestruct.ptr->bank_affecting[bank_target].type3;
    battle_participants[bank_attacker].type1 = battle_participants[bank_target].type1;
    battle_participants[bank_attacker].type2 = battle_participants[bank_target].type2;
}

void sethealblock()
{
    if (new_battlestruct.ptr->bank_affecting[bank_target].heal_block)
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
    {
        new_battlestruct.ptr->bank_affecting[bank_target].heal_block = 5;
        battlescripts_curr_instruction += 4;
    }
}

void traptarget()
{
    battle_participants[bank_target].status2.cant_escape = 1;
}

u16 mefirstforbiddenmoves[] = {MOVE_COVET, MOVE_THIEF, MOVE_STRUGGLE, MOVE_CHATTER, MOVE_COUNTER, MOVE_METAL_BURST, MOVE_LIGHT_SCREEN, MOVE_ME_FIRST, MOVE_FOCUS_PUNCH, 0xFFFF};

void mefirst_check()
{
    u16 chosen_move = chosen_move_by_banks[bank_target];
    if (get_bank_turn_order(bank_target) > current_move_turn && move_table[chosen_move].base_power && !find_move_in_table(chosen_move, &mefirstforbiddenmoves[0]))
     {
         battlescripts_curr_instruction += 4;
         new_battlestruct.ptr->bank_affecting[bank_attacker].me_first = 1;
         new_battlestruct.ptr->various.var1 = chosen_move;
     }
    else
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    return;
}

void jump_to_move_bs()
{
    battle_scripting.field18 = 0;
    battle_scripting.field19 = 0;
    hitmarker &= 0xFFFFFBFF;
    hitmarker |= HITMARKER_NO_PPDEDUCT;
    u16 move = new_battlestruct.ptr->various.var1;
    battlescripts_curr_instruction = get_move_battlescript_ptr(move);
    current_move = move;
    bank_target = get_target_of_move(move, 0, 1);
}

void setluckychant()
{
    u8 side = is_bank_from_opponent_side(bank_attacker);
    if (new_battlestruct.ptr->side_affecting[side].lucky_chant)
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
    {
        new_battlestruct.ptr->side_affecting[side].lucky_chant = 5;
        new_battlestruct.ptr->side_affecting[side].lucky_chant_bank = bank_attacker;
        battlescripts_curr_instruction += 4;
    }
}

void settailwind()
{
    u8 side = is_bank_from_opponent_side(bank_attacker);
    if (new_battlestruct.ptr->side_affecting[side].tailwind)
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
    {
        new_battlestruct.ptr->side_affecting[side].tailwind = 5;
        new_battlestruct.ptr->side_affecting[side].tailwind_bank = bank_attacker;
        battlescripts_curr_instruction += 4;
    }
}

void cansetnightmare()
{
    if (battle_participants[bank_target].status.flags.sleep && battle_participants[bank_target].status2.nightmare == 0)
    {
        battle_participants[bank_target].status2.nightmare = 1;
        battlescripts_curr_instruction += 4;
    }
    else
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    return;
}

void bellydrum()
{
    u16 halvedmaxhp = battle_participants[bank_attacker].max_hp / 2;
    u8* buff = &battle_participants[bank_attacker].hp_buff + move_table[current_move].arg1;
    if (battle_participants[bank_attacker].current_hp > halvedmaxhp && *buff != 0xC)
    {
        *buff = 0xC;
        damage_loc = halvedmaxhp;
        battlescripts_curr_instruction += 4;
    }
    else
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
}

void setmagnetrise()
{
    struct bank_affecting* magnetrise = &new_battlestruct.ptr->bank_affecting[bank_attacker];
    if (magnetrise->magnet_rise || magnetrise->smacked_down || status3[bank_attacker].rooted)
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
    {
        battlescripts_curr_instruction += 4;
        magnetrise->magnet_rise = 5;
    }
    return;
}

void settelekinesis()
{
    struct bank_affecting* telekinesis = &new_battlestruct.ptr->bank_affecting[bank_target];
    if (telekinesis->telekinesis || telekinesis->smacked_down || status3[bank_target].rooted)
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
    {
        battlescripts_curr_instruction += 4;
        telekinesis->telekinesis = 5;
    }
    return;
}

void setpowertrick()
{
    u16* atk = &battle_participants[bank_attacker].atk;
    u16* def = &battle_participants[bank_attacker].def;
    u16 placeholder = *atk;
    *atk = *def;
    *def = placeholder;
    new_battlestruct.ptr->bank_affecting[bank_attacker].powertrick ^= 1;
}

void make_pokemon_one_type()
{
    u8 type = move_table[current_move].arg1 & 0x1F;
    if (battle_participants[bank_target].type1 == type && (battle_participants[bank_target].type2 == type || battle_participants[bank_target].type2 == TYPE_EGG) && (new_battlestruct.ptr->bank_affecting[bank_target].type3 == type || new_battlestruct.ptr->bank_affecting[bank_target].type3 == TYPE_EGG))
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
    {
        battlescripts_curr_instruction += 4;
        set_type(bank_target, type);
        battle_text_buff1[0] = 0xFD;
        battle_text_buff1[1] = 3;
        battle_text_buff1[2] = type;
        battle_text_buff1[3] = 0xFF;
    }
    return;
}

void defog_effect()
{
    bank_attacker = turn_order[current_move_turn];
    u8 attackers_side = is_bank_from_opponent_side(bank_attacker);
    u8 effect = 0;
    u8 targets_side = is_bank_from_opponent_side(bank_target);
    struct side_affecting_hword* target_side = &side_affecting_halfword[targets_side];
    struct side_timer* target_timer = &side_timers[targets_side];
    void* ptr_to_script = 0;
    if (target_side->reflect_on)
    {
        effect = 1;
        target_side->reflect_on = 0;
        target_timer->reflect_timer = 0;
        bank_attacker = target_timer->reflect_bank;
        ptr_to_script = &defogblows_bs;
        move_to_buffer(MOVE_REFLECT);
    }
    else if (target_side->light_screen_on)
    {
        effect = 1;
        target_side->light_screen_on = 0;
        target_timer->lightscreen_timer = 0;
        bank_attacker = target_timer->lightscreen_bank;
        ptr_to_script = &defogblows_bs;
        move_to_buffer(MOVE_LIGHT_SCREEN);
    }
    else if (target_side->mist_on)
    {
        effect = 1;
        target_side->mist_on = 0;
        target_timer->mist_timer = 0;
        bank_attacker = target_timer->mist_bank;
        ptr_to_script = &defogblows_bs;
        move_to_buffer(MOVE_MIST);
    }
    else if (target_side->safeguard_on)
    {
        effect = 1;
        target_side->safeguard_on = 0;
        target_timer->safeguard_timer = 0;
        bank_attacker = target_timer->safeguard_bank;
        ptr_to_script = &defogblows_bs;
        move_to_buffer(MOVE_SAFEGUARD);
    }
    else if (target_side->spikes_on)
    {
        effect = 1;
        target_side->spikes_on = 0;
        target_timer->spikes_amount = 0;
        bank_attacker = bank_target;
        ptr_to_script = (void*) 0x082DAFDD;
    }
    else if (new_battlestruct.ptr->side_affecting[targets_side].stealthrock)
    {
        effect = 1;
        new_battlestruct.ptr->side_affecting[targets_side].stealthrock = 0;
        bank_attacker = bank_target;
        ptr_to_script = &rapidspinonstealthrock_bs;
    }
    else if (new_battlestruct.ptr->side_affecting[targets_side].toxic_spikes_psn)
    {
        effect = 1;
        new_battlestruct.ptr->side_affecting[targets_side].toxic_spikes_psn = 0;
        new_battlestruct.ptr->side_affecting[targets_side].toxic_spikes_badpsn = 0;
        bank_attacker = bank_target;
        ptr_to_script = &rapidspinontoxicspikes_bs;
    }
    else if (new_battlestruct.ptr->side_affecting[targets_side].sticky_web)
    {
        effect = 1;
        new_battlestruct.ptr->side_affecting[targets_side].sticky_web = 0;
        bank_attacker = bank_target;
        ptr_to_script = &rapidspinonstickyweb_bs;
    }
    else if (side_affecting_halfword[attackers_side].spikes_on)
    {
        effect = 1;
        side_affecting_halfword[attackers_side].spikes_on = 0;
        side_timers[attackers_side].spikes_amount = 0;
        ptr_to_script = (void*) 0x082DAFDD;
    }
    else if (new_battlestruct.ptr->side_affecting[attackers_side].stealthrock)
    {
        effect = 1;
        new_battlestruct.ptr->side_affecting[attackers_side].stealthrock = 0;
        ptr_to_script = &rapidspinonstealthrock_bs;
    }
    else if (new_battlestruct.ptr->side_affecting[attackers_side].toxic_spikes_psn)
    {
        effect = 1;
        new_battlestruct.ptr->side_affecting[attackers_side].toxic_spikes_psn = 0;
        new_battlestruct.ptr->side_affecting[attackers_side].toxic_spikes_badpsn = 0;
        ptr_to_script = &rapidspinontoxicspikes_bs;
    }
    else if (new_battlestruct.ptr->side_affecting[attackers_side].sticky_web)
    {
        effect = 1;
        new_battlestruct.ptr->side_affecting[attackers_side].sticky_web = 0;
        ptr_to_script = &rapidspinonstickyweb_bs;
    }
    if (effect)
    {
        if (battle_communication_struct.multistring_chooser == 0)
                battle_communication_struct.multistring_chooser = 2;
        battlescripts_curr_instruction -= 3;
        battlescript_push();
        battlescripts_curr_instruction = ptr_to_script;
    }
    else if (effect == 0 && battle_communication_struct.multistring_chooser == 0)
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
    {
        bank_attacker = turn_order[current_move_turn];
        battlescripts_curr_instruction += 4;
    }
    return;
}

u16 copycat_forbidden_moves[] = {MOVE_ASSIST, MOVE_BELCH, MOVE_BESTOW, MOVE_CHATTER, MOVE_CIRCLE_THROW, MOVE_COPYCAT, MOVE_COUNTER, MOVE_COVET, MOVE_DESTINY_BOND, MOVE_DETECT, MOVE_DRAGON_TAIL, MOVE_ENDURE, MOVE_FEINT, MOVE_FOCUS_PUNCH, MOVE_FOLLOW_ME, MOVE_HELPING_HAND, MOVE_HOLD_HANDS, MOVE_KINGS_SHIELD, MOVE_MAT_BLOCK, MOVE_ME_FIRST, MOVE_METRONOME, MOVE_MIMIC, MOVE_MIRROR_COAT, MOVE_MIRROR_MOVE, MOVE_NATURE_POWER, MOVE_PROTECT, MOVE_RAGE_POWDER, MOVE_ROAR, MOVE_SKETCH, MOVE_SLEEP_TALK, MOVE_SNATCH, MOVE_SPIKY_SHIELD, MOVE_STRUGGLE, MOVE_SWITCHEROO, MOVE_THIEF, MOVE_TRANSFORM, MOVE_TRICK, MOVE_WHIRLWIND, 0x0, 0xFFFF};

void copycat_move()
{
    if (find_move_in_table(new_battlestruct.ptr->various.previous_move, &copycat_forbidden_moves[0]))
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
    {
        new_battlestruct.ptr->various.var1 = new_battlestruct.ptr->various.previous_move;
        battlescripts_curr_instruction += 4;
    }
}

void psycho_swaps()
{
    u8 stat1 = move_table[current_move].arg1;
    if (stat1)
        battle_communication_struct.multistring_chooser = 0;
    else
        battle_communication_struct.multistring_chooser = 1;
    u8 stat2 = move_table[current_move].arg2;
    for (u8 i = 0; i < 8; i++)
    {
        u8 *atk_stat = &battle_participants[bank_attacker].hp_buff + i;
        u8 *def_stat = &battle_participants[bank_target].hp_buff + i;
        u8 placeholder = *atk_stat;
        if (stat1 == 0)
        {
            *atk_stat = *def_stat;
            *def_stat = placeholder;
        }
        else if (i == stat1)
        {
            *atk_stat = *def_stat;
            *def_stat = placeholder;
            battle_text_buff1[0] = 0xFD;
            battle_text_buff1[1] = 5;
            battle_text_buff1[2] = i;
            battle_text_buff1[3] = 0xFF;
        }
        else if (i == stat2)
        {
            *atk_stat = *def_stat;
            *def_stat = placeholder;
            battle_text_buff2[0] = 0xFD;
            battle_text_buff2[1] = 5;
            battle_text_buff2[2] = i;
            battle_text_buff2[3] = 0xFF;
        }
    }
}

void psychosplits()
{
    switch (current_move)
    {
    case MOVE_POWER_SPLIT:
        {
            u16 *atk_atk = &battle_participants[bank_attacker].atk;
            u16 *atk_trg = &battle_participants[bank_target].atk;
            u16 average = (*atk_atk + *atk_trg) / 2;
            *atk_atk = average;
            *atk_trg = average;

            atk_atk = &battle_participants[bank_attacker].sp_atk;
            atk_trg = &battle_participants[bank_target].sp_atk;
            average = (*atk_atk + *atk_trg) / 2;
            *atk_atk = average;
            *atk_trg = average;

            battle_text_buff1[0] = 0xFD;
            battle_text_buff1[1] = 0;
            battle_text_buff1[2] = 0xF2;
            battle_text_buff1[3] = 1;
            battle_text_buff1[4] = 0xFF;
        }
        break;
    case MOVE_GUARD_SPLIT:
        {
            u16 *def_def = &battle_participants[bank_attacker].def;
            u16 *def_trg = &battle_participants[bank_target].def;
            u16 average = (*def_def + *def_trg) / 2;
            *def_def = average;
            *def_trg = average;

            def_def = &battle_participants[bank_attacker].sp_def;
            def_trg = &battle_participants[bank_target].sp_def;
            average = (*def_def + *def_trg) / 2;
            *def_def = average;
            *def_trg = average;

            battle_text_buff1[0] = 0xFD;
            battle_text_buff1[1] = 0;
            battle_text_buff1[2] = 0xF3;
            battle_text_buff1[3] = 1;
            battle_text_buff1[4] = 0xFF;
        }
        break;
    }
}

void stockpile_record()
{
    struct battle_participant* attacker_stats = &battle_participants[bank_attacker];
    u8* stockpile_def = (u8*)&(new_battlestruct.ptr->various.var2);
    u8* stockpile_spdef = (u8*)&(new_battlestruct.ptr->various.var2) + 1;
    struct bank_affecting* attacker_stockpile = &new_battlestruct.ptr->bank_affecting[bank_attacker];
    switch (read_byte(battlescripts_curr_instruction))
    {
    case 0:
        *stockpile_def = attacker_stats->def_buff;
        *stockpile_spdef = attacker_stats->sp_def_buff;
        break;
    case 1:
        attacker_stockpile->stockpile_def_changes += (attacker_stats->def_buff - *stockpile_def);
        attacker_stockpile->stockpile_sp_def_changes += (attacker_stats->sp_def_buff - *stockpile_spdef);
        break;
    case 2: //reset stat buffs
        {
            attacker_stats->def_buff -= attacker_stockpile->stockpile_def_changes;
            attacker_stats->sp_def_buff -= attacker_stockpile->stockpile_sp_def_changes;
            new_battlestruct.ptr->various.var2 = 0;
            attacker_stockpile->stockpile_def_changes = 0;
            attacker_stockpile->stockpile_sp_def_changes = 0;
            break;
        }
    }
    battlescripts_curr_instruction++;
}

void twoturn_moves()
{
    u8 adder;
    u8* stringchooser = &battle_communication_struct.multistring_chooser;
    if (move_table[current_move].split == 2)
        adder = 0;
    else
        adder = move_table[current_move].arg2;
    switch (current_move)
    {
    case MOVE_GEOMANCY:
        *stringchooser = 0;
        break;
    case MOVE_SKY_ATTACK:
        *stringchooser = 1;
        break;
    case MOVE_SKULL_BASH:
        *stringchooser = 2;
        break;
    case MOVE_SOLAR_BEAM:
        *stringchooser = 3;
        break;
    case MOVE_BOUNCE:
        *stringchooser = 4;
        break;
    case MOVE_ICE_BURN:
        *stringchooser = 5;
        break;
    case MOVE_FREEZE_SHOCK:
        *stringchooser = 6;
        break;
    case MOVE_RAZOR_WIND:
        *stringchooser = 7;
        break;
    case MOVE_PHANTOM_FORCE:
    case MOVE_SHADOW_FORCE:
        *stringchooser = 8;
        break;
    case MOVE_FLY:
        *stringchooser = 9;
        break;
    case MOVE_DIG:
        *stringchooser = 10;
        break;
    case MOVE_DIVE:
        *stringchooser = 11;
        break;
    }
    battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction + 4 * adder);
}

void powerherb_check()
{
    if (get_item_effect(bank_attacker, 1) == ITEM_EFFECT_POWERHERB)
    {
        last_used_item = battle_participants[bank_attacker].held_item;
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
        battlescript_push();
        battlescripts_curr_instruction = &powerherb_bs;
    }
    else
        battlescripts_curr_instruction += 4;
}

void set_terrain()
{
    u8 fail = 0;
    switch (current_move)
    {
    case MOVE_MISTY_TERRAIN:
        if (new_battlestruct.ptr->field_affecting.misty_terrain)
            fail = 1;
        else
        {
            new_battlestruct.ptr->field_affecting.misty_terrain = 5;
            battle_communication_struct.multistring_chooser = 0;
        }
        break;
    case MOVE_GRASSY_TERRAIN:
        if (new_battlestruct.ptr->field_affecting.grassy_terrain)
            fail = 1;
        else
        {
            new_battlestruct.ptr->field_affecting.grassy_terrain = 5;
            battle_communication_struct.multistring_chooser = 1;
        }
        break;
    case MOVE_ELECTRIC_TERRAIN:
        if (new_battlestruct.ptr->field_affecting.electic_terrain)
            fail = 1;
        else
        {
            new_battlestruct.ptr->field_affecting.electic_terrain = 5;
            battle_communication_struct.multistring_chooser = 2;
        }
        break;
    }
    if (fail)
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
        battlescripts_curr_instruction += 4;
}

void setaquaring()
{
    if (new_battlestruct.ptr->bank_affecting[bank_attacker].aqua_ring)
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
    {
        battlescripts_curr_instruction += 4;
        new_battlestruct.ptr->bank_affecting[bank_attacker].aqua_ring = 1;
    }
    return;
}

void get_trainer_name_for_mega()
{
    char* trainer_name;
    //to do multi battle checks
    if (is_bank_from_opponent_side(bank_attacker))
    {
        trainer_name = get_trainername_address(var_8015_trainer_opponent_A);
    }
    else
    {
        trainer_name = get_player_name_address();
    }
    strcpy_xFF_terminated_0(&battle_text_buff3, trainer_name);
}

void mega_evo_updatehpbar()
{
    struct pokemon* poke_address;
    u8 bank = bank_attacker;
    if (is_bank_from_opponent_side(bank))
        poke_address = &party_opponent[battle_team_id_by_side[bank]];
    else
        poke_address = &party_player[battle_team_id_by_side[bank]];
    u8 obj_ID = graphical_elements_pbs[bank];
    update_bank_graphical_elements(obj_ID, poke_address, 0); //0 updates all things; gender, exp, hp, etc.
}

void mega_evo_pursuit_check()
{
    battlescripts_curr_instruction -= 3;
    if (!check_mega_evo(bank_attacker))
        battlescripts_curr_instruction += 3;
    return;
}

void jumpifuserhasnoHP()
{
    if (battle_participants[bank_attacker].current_hp)
        battlescripts_curr_instruction += 4;
    else
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    return;
}

void quash_setter() //0 - first, 1 - second, 2 - third, 3 - fourth
{
    u8 target_turn = get_bank_turn_order(bank_target);
    u8 attacker_turn = get_bank_turn_order(bank_attacker);
    if (target_turn < attacker_turn)
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
    {
        new_battlestruct.ptr->bank_affecting[bank_attacker].quashed = 1;
        battlescripts_curr_instruction += 4;
        if (battle_flags.double_battle)
        {
            switch (target_turn)
            {
            case 1:
                turn_order[1] = turn_order[2];
            case 2:
                turn_order[2] = turn_order[3];
            case 3:
                turn_order[3] = bank_target;
            }
        }
    }
    return;
}

void beatup_getloopcounter()
{
    struct pokemon* poke;
    if (is_bank_from_opponent_side(bank_attacker))
        poke = &party_opponent[0];
    else
        poke = &party_player[0];
    u8 counter = 0;
    for (u8 i = 0; i < 6; i++)
    {
        u16 species = get_attributes(&poke[i], ATTR_SPECIES, 0);
        if (species != 0 && species != 412 && get_attributes(&poke[i], ATTR_CURRENT_HP, 0))
            counter++;
    }
    multihit_counter = counter;
}

void canuse_allyswitch()
{
    u8 ally_bank = bank_attacker ^ 2;
    if (is_bank_present(ally_bank) && battle_flags.double_battle)
    {
        battlescripts_curr_instruction += 4;
    }
    else
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
}

void allyswitch_dataswitch()
{
    u8 ally_bank = bank_attacker ^ 2;
    // paritcipant struct
    struct battle_participant attacker1 = battle_participants[bank_attacker];
    battle_participants[bank_attacker] = battle_participants[ally_bank];
    battle_participants[ally_bank] = attacker1;

    //disable struct
    struct disable_struct attacker2 = disable_structs[bank_attacker];
    disable_structs[bank_attacker] = disable_structs[ally_bank];
    disable_structs[ally_bank] = attacker2;

    //protect struct
    struct protect_struct attacker3 = protect_structs[bank_attacker];
    protect_structs[bank_attacker] = protect_structs[ally_bank];
    protect_structs[ally_bank] = attacker3;

    //status3
    struct status_3 attacker4 = status3[bank_attacker];
    status3[bank_attacker] = status3[ally_bank];
    status3[ally_bank] = attacker4;

    //special status
    struct special_status attacker5 = special_statuses[bank_attacker];
    special_statuses[bank_attacker] = special_statuses[ally_bank];
    special_statuses[ally_bank] = attacker5;

    //new battle struct
    struct bank_affecting attacker6 = new_battlestruct.ptr->bank_affecting[bank_attacker];
    new_battlestruct.ptr->bank_affecting[bank_attacker] = new_battlestruct.ptr->bank_affecting[ally_bank];
    new_battlestruct.ptr->bank_affecting[ally_bank] = attacker6;

    //tai move history
    struct used_moves attacker7 = battle_resources.ptr->battle_history->used_moves[bank_attacker];
    battle_resources.ptr->battle_history->used_moves[bank_attacker] = battle_resources.ptr->battle_history->used_moves[ally_bank];
    battle_resources.ptr->battle_history->used_moves[ally_bank] = attacker7;

    //abilities in RAM
    u8 attackerability = abilities_by_banks[bank_attacker];
    abilities_by_banks[bank_attacker] = abilities_by_banks[ally_bank];
    abilities_by_banks[ally_bank] = attackerability;

    //in-party position
    u16* partyAttacker = &battle_team_id_by_side[bank_attacker];
    u16* partyAlly = &battle_team_id_by_side[ally_bank];
    struct pokemon* poke;
    if (is_bank_from_opponent_side(bank_attacker))
        poke = &party_opponent[0];
    else
        poke = &party_player[0];

    struct pokemon attacker8 = poke[*partyAttacker];
    poke[*partyAttacker] = poke[*partyAlly];
    poke[*partyAlly] = attacker8;

    //selected move
    u16 attacker9 = chosen_move_by_banks[bank_attacker];
    chosen_move_by_banks[bank_attacker] = chosen_move_by_banks[ally_bank];
    chosen_move_by_banks[ally_bank] = attacker9;

    u8 attacker10 = battle_stuff_ptr.ptr->chosen_move_position[bank_attacker];
    battle_stuff_ptr.ptr->chosen_move_position[bank_attacker] = battle_stuff_ptr.ptr->chosen_move_position[ally_bank];
    battle_stuff_ptr.ptr->chosen_move_position[ally_bank] = attacker10;

    u8 attacker11 = battle_stuff_ptr.ptr->move_target[bank_attacker];
    battle_stuff_ptr.ptr->move_target[bank_attacker] = battle_stuff_ptr.ptr->move_target[ally_bank];
    battle_stuff_ptr.ptr->move_target[ally_bank] = attacker11;

    if (!is_bank_from_opponent_side(ally_bank))
    {
        u8 attacker13 = move_selection_cursor_pbs[bank_attacker];
        move_selection_cursor_pbs[bank_attacker] = move_selection_cursor_pbs[ally_bank];
        move_selection_cursor_pbs[ally_bank] = attacker13;
    }

    u16 attacker14 = last_used_moves[bank_attacker];
    last_used_moves[bank_attacker] = last_used_moves[ally_bank];
    last_used_moves[ally_bank] = attacker14;

    //objects
    u8* objCase = &battle_scripting.field21;
    battle_scripting.field22 = 0;

     *objCase = 21;
     c2_80777E8(); //fade screen

    *objCase = 3;
    while (*objCase < 21)
    {
        c2_80777E8(); //load everything
    }

    //turn order
    u16 atk_order = get_bank_turn_order(bank_attacker);
    u16 ally_order = get_bank_turn_order(ally_bank);
    turn_order[atk_order] = ally_bank;
    turn_order[ally_order] = bank_attacker;

    battle_scripting.active_bank = ally_bank;
}

u8 check_magneticflux_bank(u8 bank)
{
    if (is_bank_present(bank) && (check_ability(bank, ABILITY_PLUS) || check_ability(bank, ABILITY_MINUS)) && !(battle_participants[bank].def_buff == 0xC && battle_participants[bank].sp_def_buff == 0xC))
    {
        return 1;
    }
    return 0;
}

void can_magneticflux_work()
{
    u8 effect = 0;
    u8 side = is_bank_from_opponent_side(bank_attacker);
    for (u8 i = 0; i < no_of_all_banks; i++)
    {
        if (side == is_bank_from_opponent_side(i) && check_magneticflux_bank(i))
        {
            effect = 1;
            new_battlestruct.ptr->various.var1 = 0;
            new_battlestruct.ptr->various.var2 = 0;
            another_active_bank = bank_attacker;
            break;
        }
    }
    if (effect)
        battlescripts_curr_instruction += 4;
    else
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
}

void magnetic_flux_effect()
{
    u8 side = is_bank_from_opponent_side(bank_attacker);
    for (u8 i = 0; i < no_of_all_banks; i++)
    {
        if (side == is_bank_from_opponent_side(i) && ((new_battlestruct.ptr->various.var2 & bits_table[i]) == 0) && check_magneticflux_bank(i))
        {
            if (new_battlestruct.ptr->various.var1 == move_table[current_move].arg1)
            {
                new_battlestruct.ptr->various.var1 = 0;
                new_battlestruct.ptr->various.var2 |= bits_table[i];
                battlescripts_curr_instruction -= 3;
                break;
            }
            bank_attacker = i;
            dostatchanges();
            return;
        }
    }
    bank_attacker = another_active_bank;
}

u8 get_flowershieldbank()
{
    for (u8 i = 0; i < no_of_all_banks; i++)
    {
        u8* stat_ptr = &battle_participants[i].hp_buff + bit_to_stat(move_table[current_move].arg1);
        if (is_bank_present(i) && !(crit_loc & bits_table[i]) && is_of_type(i, TYPE_GRASS) && *stat_ptr != 0xC)
        {
            return i;
        }
    }
    return 0xFF;
}

void canuse_flowershield()
{
    if (get_flowershieldbank() == 0xFF)
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    else
    {
        battlescripts_curr_instruction += 4;
        new_battlestruct.ptr->various.var2 = bank_attacker;
    }
}

void flowershield_effect()
{
    u8 bank = get_flowershieldbank();
    if (bank != 0xFF)
    {
        crit_loc |= bits_table[bank];
        bank_attacker = bank;
        dostatchanges();
        return;
    }
    bank_attacker = new_battlestruct.ptr->various.var2;
    return;
}

void canuselastresort()
{
    s8 lastresort_pos = get_move_position(bank_attacker, current_move);
    if (lastresort_pos != -1)
    {
        for (u8 i = 0; i < 4; i++)
        {
            u16* move_ptr = &battle_participants[bank_attacker].moves[0];
            if (*move_ptr && i != lastresort_pos && (new_battlestruct.ptr->bank_affecting[bank_attacker].usedmoves & bits_table[i]))
            {
                battlescripts_curr_instruction += 4;
                return;
            }
        }
    }
    battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
}

void topsyturvy_effect()
{
    u8 reversed_stats = 0;
    for (u8 i = 0; i < 7; i++)
    {
        u8* stat = &battle_participants[bank_target].atk_buff + i;
        if (*stat != 6)
        {
            reversed_stats++;
            if (*stat < 6)
            {
                *stat += (6 - *stat) * 2;
            }
            else //*stat > 6
            {
                *stat -= (*stat - 6) * 2;
            }
        }
    }
    if (reversed_stats)
        battlescripts_curr_instruction += 4;
    else
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
}

void bestow_effect()
{
    u16* user_item = &battle_participants[bank_attacker].held_item;
    u16* target_item = &battle_participants[bank_target].held_item;
    if (*user_item && (can_lose_item(bank_attacker, 1, 0)) && !(*target_item) && canlose_megastone(bank_target, *user_item))
    {
        battlescripts_curr_instruction += 4;
        *target_item = *user_item;
        last_used_item = *user_item;
        *user_item = 0;
        active_bank = bank_attacker;
        prepare_setattributes_in_battle(0, REQUEST_HELDITEM_BATTLE, 0, 2, user_item);
        mark_buffer_bank_for_execution(active_bank);
        active_bank = bank_target;
        prepare_setattributes_in_battle(0, REQUEST_HELDITEM_BATTLE, 0, 2, target_item);
        mark_buffer_bank_for_execution(active_bank);
    }
    else
    {
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    }
}

void conversion_effect()
{
    u8 effect = 0;
    u8 type = 0;
    if (current_move == MOVE_CONVERSION)
    {
        type = move_table[battle_participants[bank_attacker].moves[0]].type;
        if (!is_of_type(bank_attacker, type))
            effect = 1;
    }
    else //current move == MOVE_CONVERSION2
    {
        u8 searched_moves = 0;
        u32 searched_moves_bitfield = 0;
        u8 last_type = new_battlestruct.ptr->bank_affecting[bank_target].lastmove_type;
        if (last_type)
        {
            last_type &= 0x3F;
            if (last_type >= TYPE_FAIRY)
                last_type -= 5;
            while (searched_moves < 18)
            {
                type = __umodsi3(rng(), 0x18);
                if (type != TYPE_EGG && (type <= 0x11 || type == TYPE_FAIRY) && ((searched_moves_bitfield & bits_table[type]) == 0))
                {
                    searched_moves_bitfield |= bits_table[type];
                    searched_moves++;
                    if (type >= TYPE_FAIRY)
                        type -= 5;
                    u8 effectiveness = type_effectiveness_table[last_type][type];
                    if (type == 0x12)
                        type = TYPE_FAIRY;
                    if (!is_of_type(bank_attacker, type) && (effectiveness == 0 || effectiveness == 5))
                    {
                        effect = 1;
                        break;
                    }
                }
            }
        }
    }
    if (effect)
    {
        battlescripts_curr_instruction += 4;
        battle_text_buff1[0] = 0xFD;
        battle_text_buff1[1] = 3;
        battle_text_buff1[2] = type;
        battle_text_buff1[3] = 0xFF;
        set_type(bank_attacker, type);
    }
    else
        battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
}

u8 is_poke_valid(struct pokemon* poke)
{
    u16 species = get_attributes(poke, ATTR_SPECIES, 0);
    if (species != 0 && species != 412)
        return 1;
    return 0;
}

u8 count_statused_pokes(struct pokemon* poke)
{
    u8 statused = 0;
    for (u8 i = 0; i < 6; i++)
    {
        if (is_poke_valid(poke) && get_attributes(poke, ATTR_STATUS_AILMENT, 0))
            statused++;
    }
    return statused;
}

struct pokemon* get_party_ptr(u8 bank)
{
    if (is_bank_from_opponent_side(bank))
        return &party_opponent[0];
    return &party_player[0];
}

//not functions delcarations, just addresses for BS
extern void PARTYHEAL_USER();
extern void PARTYHEAL_ALLY();
extern void PARTYHEAL_TEAM();

void party_heal()
{
    u8 ability_to_ignore = 0;
    u8 ignore_at_users = 0;
    if (current_move == MOVE_HEAL_BELL)
        ability_to_ignore = ABILITY_SOUNDPROOF;
    else if (current_move == MOVE_AROMATHERAPY)
    {
        ability_to_ignore = ABILITY_SAP_SIPPER;
        ignore_at_users = 1;
    }
    u8 effect = 0;
    u32* user_status = &battle_participants[bank_attacker].status.int_status;
    u32 condition = 0;
    u8 pos = 0;
    u8 ally = bank_attacker ^ 2;
    struct pokemon* poke = get_party_ptr(bank_attacker);
    void* instuction = 0;
    if (*user_status && (!check_ability(bank_attacker, ability_to_ignore) || ignore_at_users))
    {
        effect = 1;
        pos = battle_team_id_by_side[bank_attacker];
        condition = *user_status;
        *user_status = 0;
        active_bank = bank_attacker;
        prepare_setattributes_in_battle(0, REQUEST_STATUS_BATTLE, 0, 4, user_status);
        mark_buffer_bank_for_execution(active_bank);
        instuction = &PARTYHEAL_USER;
    }
    else if (is_bank_present(ally) && battle_participants[ally].status.int_status && !check_ability(ally, ability_to_ignore))
    {
        effect = 1;
        pos = battle_team_id_by_side[ally];
        condition = battle_participants[ally].status.int_status;
        battle_participants[ally].status.int_status = 0;
        active_bank = ally;
        battle_scripting.active_bank = ally;
        prepare_setattributes_in_battle(0, REQUEST_STATUS_BATTLE, 0, 4, &battle_participants[ally].status.int_status);
        mark_buffer_bank_for_execution(active_bank);
        instuction = &PARTYHEAL_ALLY;
    }
    else
    {
        for (u8 i = 0; i < 6; i++)
        {
            condition = get_attributes(&poke[i], ATTR_STATUS_AILMENT, 0);
            u8 ability = get_poke_ability(&poke[i]);
            if (is_poke_valid(&poke[i]) && condition && ability_to_ignore != ability)
            {
                effect = 1;
                pos = i;
                instuction = &PARTYHEAL_TEAM;
                u32 zero = 0;
                set_attributes(&poke[i], ATTR_STATUS_AILMENT, &zero);
                break;
            }
        }
    }
    if (effect)
    {
        battlescripts_curr_instruction -= 3;
        //PSN/BRN/PAR/SLP/FRZ
        u8* string = &battle_communication_struct.multistring_chooser;
        if ((condition & STATUS_POISON) || (condition & STATUS_TOXIC_POISON))
            *string = 0;
        else if (condition & STATUS_BURN)
            *string = 1;
        else if (condition & STATUS_PARALYSIS)
            *string = 2;
        else if (condition & STATUS_SLEEP)
            *string = 3;
        else if (condition & STATUS_FREEZE)
            *string = 4;
        u8* text_buff = &battle_text_buff1[0];
        if (is_bank_from_opponent_side(bank_attacker))
        {
            if (battle_flags.trainer)
                text_buff = strcpy_xFF_terminated_0(text_buff, (void*) 0x085CBD8B);
            else
                text_buff = strcpy_xFF_terminated_0(text_buff, (void*) 0x085CBD85);
        }
        get_attributes(&poke[pos], ATTR_NAME, text_buff);
        battlescript_push();
        battlescripts_curr_instruction = instuction;
    }
    return;
}

void accupressure_effect()
{
    u32 checked_stats_bitfield = 0;
    u8 checked_stats_int = 0;
    u8 stat;
    u8* statvalue = &battle_participants[bank_target].atk_buff;
    while (checked_stats_int < 7)
    {
        stat = __umodsi3(rng(), 7);
        if (!(checked_stats_bitfield & bits_table[stat]))
        {
            checked_stats_bitfield |= bits_table[stat];
            checked_stats_int++;
            if (*(statvalue + stat) != 0xC)
            {
                battle_scripting.stat_changer = 0x21 + stat;
                battlescripts_curr_instruction += 4;
                return;
            }
        }
    }
    battlescripts_curr_instruction = (void*) read_word(battlescripts_curr_instruction);
    return;
}

void* callasm_table[] = {&call_ability_effects /*0*/, &apply_burn_animation /*1*/, &change_attacker_item /*2*/, &try_to_lower_def /*3*/, &try_to_raise_spd /*4*/,
&changestatvar1 /*5*/, &changestatvar2 /*6*/, &frisk_target_item /*7*/, &set_stat_msg_buffer /*8*/, &set_type_msg_buffer /*9*/, &set_team_msg_buffer /*10*/, &bad_dreams_damage_calc /*11*/,
&weaknesspolicy /*12*/, &mentalherb /*13*/, &placeholder0x14 /*14*/, &hazards_bank_switcher /*15*/, &hazards_bank_return /*16*/, &leechseed_update /*17*/,
&target_stat_change /*18*/, &attacker_stat_change /*19*/, &moxie_stat_raise /*20*/, &grassyterrainn_heal /*21*/, &callitemeffects /*22*/,
&placeholder0x14 /*23*/, &damagecalc2 /*24*/, &set_statchanger /*25*/, &checksubstitute /*26*/, &statchangeeffect /*27*/,
&doublestatchange_check /*28*/, &dostatchanges /*29*/, &statcheck_return /*30*/, &checkifcanconfuse /*31*/, &confuse_applystatchange /*32*/,
&jumpifcantpoison /*33*/, &jumpifcantparalyze /*34*/, &jumpifcantburn /*35*/, &statustoeffect /*36*/, &half_hp_damage /*37*/,
&jumpifonlyonepokemon /*38*/, &setlunardanceeffect /*39*/, &weatherhpheal /*40*/, &checkifcantransfercondition /*41*/, &choosestatusinflictiontext /*42*/,
&roostactivation /*43*/, &gravitysetter /*44*/, &gravity_ender /*45*/, &setidentifierbit /*46*/, &breakprotection /*47*/,
&suckerpunchchecker /*48*/, &oppositegenderscheck /*49*/, &setthirdtype /*50*/, &ability_change /*51*/, &roomsetter /*52*/, &countercalc /*53*/,
&gastroacid /*54*/, &setembargo /*55*/, &naturalgift /*56*/, &afteryou_check /*57*/, &powder_setter /*58*/, &jumpifnoally /*59*/,
&electrify /*60*/, &set_entry_hazards /*61*/, &jumpifnotdoublebattle /*62*/, &jumpifattackerandtargetdontsharetypes /*63*/,
&try_autotonomize /*64*/, &set_iondeluge /*65*/, &reflecttypes /*66*/, &sethealblock /*67*/, &traptarget /*68*/, &mefirst_check /*69*/,
&jump_to_move_bs /*70*/, &setluckychant /*71*/, &settailwind /*72*/, &cansetnightmare /*73*/, &bellydrum /*74*/, &setmagnetrise /*75*/,
&settelekinesis /*76*/, &setpowertrick /*77*/, &make_pokemon_one_type /*78*/, &defog_effect /*79*/, &copycat_move /*80*/,
&psycho_swaps /*81*/, &psychosplits /*82*/, &stockpile_record /*83*/, &twoturn_moves /*84*/, &powerherb_check /*85*/, &set_terrain /*86*/,
&setaquaring /*87*/, &get_trainer_name_for_mega /*88*/, &mega_evo_updatehpbar /*89*/, &mega_evo_pursuit_check /*90*/,
&jumpifuserhasnoHP /*91*/, &quash_setter /*92*/, &beatup_getloopcounter /*93*/, &canuse_allyswitch /*94*/, &allyswitch_dataswitch /*95*/,
&can_magneticflux_work /*96*/, &magnetic_flux_effect /*97*/, &canuse_flowershield /*98*/, &flowershield_effect /*99*/, &canuselastresort /*100*/,
&topsyturvy_effect /*101*/, &bestow_effect /*102*/, &conversion_effect /*103*/, &party_heal /*104*/, &accupressure_effect /*105*/};

void callasm_cmd()
{
    void (*command) (void);
    u8 command_id_ls = read_byte(battlescripts_curr_instruction + 1);
    u8 command_id_ms = read_byte(battlescripts_curr_instruction + 2);
    battlescripts_curr_instruction += 3;
    command = callasm_table[(command_id_ms<<8)+command_id_ls];
    command();
}
