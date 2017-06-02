#ifndef CONFIG_H
#define CONFIG_H

#include "types.h"

#define ALL_POKES       412     //number of all pokemon
#define ALL_ITEMS       377     //number of all items
#define NUM_OF_EVOS     5       //number of evolutions per pokemon
#define KEYSTONE        4     	//mega item that the player has to posses in order to mega evolve
#define ALL_MOVES		621		//number of all moves

#define BUILD_LEARNSETS true        //set to false if you modified the learnsets and dont want them to get changed
#define GEN4_LEARNSETS  false //set to true if you want to have learnsets for pokemon up to genIV
#define GEN5_LEARNSETS  false         //set to true if you want to have learnsets for pokemon up to genV
#define GEN6_LEARNSETS  false          //set to true if you want to have learnsets for pokemon up to genVI

#define EXP_CAPTURE         true    //set to false if you don't want to receive exp from catching pokes
#define STAT_RECALC         false   //set to true if you want all pokemon having their stats recalculated at the end of the battle
#define ITEM_STEAL          false   //if true player is able to steal other trainers' items, if false the stolen item will disappear at the end of the battle
#define ITEM_SWAP           false   //if true player's item will be that that was switched, if false the switched item that a pokemon originally had returns to it
#define MAX_LEVEL           100     //highest possible level
#define MAX_EVS             510     //maximum amount of EV points per pokemon
#define EXP_DIVIDE          true    //if true exp will be divided among pokemon(if two pokemon participate they'll get 50 % each), set to false if you want gen6-style exp
#define GENVI_EXPSHARE      false    //set to true if you want EXPSHARE to act like in gen6
#define EXPSHARE_FLAG       0x0   //flag that must be set for GENVI expshare to work
#define DISABLED_EXP_FLAG   0x0   //if that flag is set, receiving exp is disabled, if flag is 0 it has no effect
#define DISABLED_EVS_FLAG   0x0     //if that flag is set, receiving EVS points is disabled, if flag is 0 it has no effect; this and above flag can be the same
#define DOUBLE_WILD_BATTLES true    //set to false if you don't want have them in your hack at all
#define DOUBLE_WILD_TILES   4      //amount of tiles double wild battles are possible on
#define EXPANDED_POKEBALLS  false  //set to true if your hack uses pokeball expansion
#define INVERSE_FLAG        0x0   //if that flag is set, the battle is inverse
#define FISHING_FLAG        0x0   //if that flag is set, it's a battle against a hooked up pokemon
#define CANT_CATCH_FLAG     0x0   //if that flag is set, you can't catch any pokemon, if flag is 0 you always can catch any pokemon
#define ALLOW_LOSE_FLAG     0x0   //if that flag is set, you can lose a battle and the script will continue
#define FORCE_SET_FLAG      0x0   //if that flag is set, the player cannot switch a pokemon when opponent faints
#define NO_OF_SLIDING_MSG_TRAINERS 10 //number of trainers that say things in the middle of a battle
#define SCHOOLING_LEVEL 20

//form indexes
#define POKE_CHERRIM                1000
#define POKE_CHERRIM_SUNSHINE       1001
#define POKE_DARMANITAN             1002
#define POKE_ZEN_MODE               1003
#define POKE_AEGISLASH_BLADE        1004
#define POKE_AEGISLASH_SHIELD       1005
#define POKE_MELOETTA_ARIA          1006
#define POKE_MELOETTA_PIROUETTE     1007
#define POKE_BURMY_PLANT            1008
#define POKE_BURMY_SAND             1009
#define POKE_BURMY_TRASH            1010
#define POKE_GRENJA                 1011
#define POKE_ASH_GRENJA             1012
#define POKE_WISHIWASHI             1013
#define POKE_WISHIWASHI_SCHOOL      1014
#define POKE_ZYGARDE_10             1015
#define POKE_ZYGARDE_50             1016
#define POKE_ZYGARDE_100            1017
#define POKE_MINIOR_CORE            1018
#define POKE_MINIOR_METEOR          1019
#define POKE_MIMIKKYU               1020
#define POKE_MIMIKKYU_BUSTED        1021

//only change these if you want to adjust the position of mega icons or if you want to change the HP bars
#define SINGLES_HEALTHBOX_X 120
#define DBL_HB_0_X 120
#define DBL_HB_2_X 132
#define SINGLES_HEALTHBOX_Y 90
#define DBL_HB_0_Y 75
#define DBL_HB_2_Y 100

#endif /* CONFIG_H */
