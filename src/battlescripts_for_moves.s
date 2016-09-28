.text
.thumb
.align 2

.include "asm_defines.s"
.include "defines/bs_commands.s"

.equ MOVE_FAILED, 0x082D9F1A
.equ MOVE_MISSED, 0x082D8A5E
.equ ENDTURN, 0x82D8A4E
.equ EndTurnTracker, 0x2024488
.equ CURRENT_MOVE, 0x20241EA
.equ EffectChooser, 0x2024335
.equ MultiStringChooser, 0x2024337
.equ MoveOutcome, 0x202427C
.equ HitMarker, 0x2024280
.equ OutcomeMissed, 1
.equ OutcomeNotaffected, 8
.equ OutcomeFailed, 0x20
.equ OutcomeSturdied, 0x100
.equ HitMarkerNoAnimations, 0x80
.equ HitMarkerHitsSubstitute, 0x100
.equ HitMarkerHitsOnAir, 0x10000
.equ StatChanger, 0x0202448E

.macro jumpifmove jumpifmove_move jumpifmove_address
jumpifhalfword 0x0 0x020241EA \jumpifmove_move \jumpifmove_address
.endm

.macro jumpifnotmove jumpifnotmove_move jumpifnotmove_address
jumpifhalfword 0x1 0x020241EA \jumpifnotmove_move \jumpifnotmove_address
.endm

@@@macros for callasms
.macro jumpifsubstituteaffects jumpifsubstituteaffects_address
.byte 0x83
.hword 26
.word \jumpifsubstituteaffects_address
.endm

.macro jumpifcantchangetwostats jumpifcantchangetwostats_address
.byte 0x83
.hword 28
.word \jumpifcantchangetwostats_address
.endm

.macro doublestatchange
.byte 0x83
.hword 29
.endm

.macro jumpifcantconfuseandchangestats jumpifcantconfuseandchangestats_address
.byte 0x83
.hword 31
.word \jumpifcantconfuseandchangestats_address
.endm

.macro jumpifonlyonepokemoninteam jumpifonlyonepokemoninteam_address
.byte 0x83
.hword 38
.word \jumpifonlyonepokemoninteam_address
.endm

.macro weatherhpheal weatherhphealbank
.byte 0x83
.hword 40
.byte \weatherhphealbank
.endm

.macro jumpifnoally jumpifnoally_address
.byte 0x83
.hword 59
.word \jumpifnoally_address
.endm

.macro jumpifuserhasnoHP jumpifuserhasnoHP_address
.byte 0x83
.hword 91
.word \jumpifuserhasnoHP_address
.endm

battlescripts_table:
.word ATTACKING_MOVE	@ 0	Just attacks target(Tackle)
.word FIXED_DAMAGE		@1 Sonicboom, Dragon Rage, etc.
.word ONE_STAT_USER		@2 Swords Dance, Double Team, etc. ;arg1 = statchanger value
.word ONE_STAT_TARGET	@3 stuff like tail whip, growl and such ;arg1 = statchanger value
.word ATTACK_USERSTAT_CHANCE	@4 Metal Claw, Steel Wing, etc. ;arg1 = statchanger value
.word ATTACK_TARGETSTAT_CHANCE	@5 Mud Slap, Mud Shot and such ;arg1 = statchanger values
.word MULTIPLE_STAT_CHANGE_USER	@6 Dragon Dance, Bulk Up, etc. ;arg1 is a bitfield with which stats to raise, arg2 is a signed value for how much to add
.word MULTIPLE_STAT_CHANGE_TARGET	@7 Tickle, etc. ;arg1 is a bitfield with which stats to raise, arg2 is a signed value for how much to add
.word ATTACK_MULTIPLESTAT_CHANCE_USER	@8 Ancientpower, Close Combat, etc. ;arg1 is a bitfield with which stats to raise, arg2 is a signed value for how much to add
.word CONFUSE_TARGET	@9 Confuse Ray, Swagger; arg1 = statchanger value, if 0 only confuses
.word ATTACK_CONFUSION_CHANCE	@10 simply can confuse the target after the move
.word ATTACK_FLINCH_CHANCE		@11 simply can flinch the target after the move; arg1 is a status flag that can be applied, similarly to elemental fangs effect
.word PUT_TARGET_TO_SLEEP	@12	Sing, etc. tries to puts the target to sleep
.word POISON_TARGET		@13 poison gas, poisonspore, etc.
.word BADLY_POISON_TARGET	@14 Toxic is the only move that can do that
.word PARALYZE_TARGET	@15 Thunder Wave, Glare
.word BURN_TARGET		@16 Will-o-wisp
.word FREEZE_TARGET		@17 currently no move can do that
.word ATTACK_STATUS_CHANCE	@18 arg1 is status(poison, burn, etc.) flag
.word RECOIL_ATTACK		@19 arg1 is status(poison, burn, etc.) flag that can be applied; default recoil is 1/3
.word CRASH_ATTACK		@20 Jump Kick and such
.word FAINT_HEAL	@21 user heals, replacement is healed
.word FAINTSTATCHANGE	@22 user faints, but lowers target's stats; arg1 is bitfield for stats, arg2 is value for raising/lowering
.word EXPLODE			@23 Selfdestruct and explosion
.word 0x082D8AEA		@24 moves that drain damage, arg1 is percent of the damage to be applied to HP(note: don't try values higher than 100), if arg2 isn't 0 we round up, else round down
.word HPHEAL_user		@25 heals user by 50% of its HP; arg1 is weather that gets 2/3 of HP, arg2 is weather that gets 1/4 of HP
.word HPHEAL_target		@26 heals target by 50% of its HP; bitfield for weather: 1 - rain, 2 - sun, 4 - sandstorm, 8 - hail, 0x10 - fog, 0x20 - air current
.word CHARGE			@27 arg1 is statvalue to be applied
.word TRANSFER_CONDITION	@28 Psycho Shift
.word ROOST				@29 Roost, heals 1/2 rounded up, loses flying type for remainder of the turn
.word GRAVITY			@30 sets Gravity, brings airborne Pokemon down
.word IDENTIFIER		@31 if arg1 is 0 = Foresight/Odor Sleught, else MiracleEye
.word ATTACK_HEAL_TARGET_STATUS	@32 arg1 is status flag, BP doubled is done in damage calc function
.word ATTACK_PROTECTION_BREAK	@33 if move is succesfull, it breaks protection for the turn; arg1 if 0 = broke through protect message, else feint message
.word PROTECT_LIKE_MOVE		@34 protect, shields, endure, all that stuff goes here
.word ATTACK_SWITCH		@35 Volt Switch/U-Turn basically; arg1 is a status flag that can be applied coz why not
.word ATTACK_ONLYIF_TARGET_ATTACKS	@36 Sucker Punch; arg1 is a status flag that can be applied
.word ATTRACT			@37 just attract
.word LOWERSTAT_IFOPPOSITEGENDER	@38 Captivate basically; arg1 is statvalue
.word RECHARGE_NEEDED	@39 hyper beam, giga impacts, shit like that
.word ATTACK_AND_TRAP	@40 Wrap and stuff
.word ADD_THIRD_TYPE	@41 sets a third type to target; fails if user's already that type; arg1 is type to be changed into
.word ABILITY_CHANGE	@42 arg1 if 0 = we swap abilities, if 1 attacker's ability becomes target, if 2 target's ability becomes attacker, if 3 target's ability becomes ability in arg2
.word ROOMS				@43 no args, moves are read from IDs
.word COUNTER			@44 Counter, Mirror Coat and Metal Burst, moves read from IDs
.word GASTROACID		@45 makes target's ability ineffective
.word EMBARGO			@46 makes target's item useless
.word NATURALGIFT		@47 natural gift
.word AFTERYOU			@48 target moves right after the user
.word POWDER			@49 set powder bit
.word CHANGEALLYSTAT	@50 Aromatic-Mist like arg1 is statvalue
.word ATTACKCLEARSTATCHANGES	@51 Clears target's stat changes
.word ELECTRIFY_MOVE	@52 Electify
.word ENTRYHAZARDS		@53 Read from moves IDs
.word FOLLOWME			@54 Follow Me, Rage Powder
.word ATTACK_ONLYIFSHARETYPE	@55 Synchronoise
.word WEIGHTLOSE_STATCHANGE		@56 Autonomize basically; arg1 is statvalue
.word IONDELUGE			@57 Ion Deluge
.word CAMOUFLAGE		@58 Camouflage
.word REFLECTTYPE		@59 changes all types to match the target's
.word FOCUSENERGY		@60 focus energy
.word TAUNTT			@61 taunt
.word TORMENTT			@62 Torment
.word HEALBLOCK			@63 Heal Block
.word ESCAPEPREVENTION	@64 Spider's Web, Mean look, etc.
.word PERISHSONG		@65 Perish Song
.word HITS_MULTIPLE_TIMES @66 Bullet Seed, Rock Blast, etc. ;arg1 is status flag to be applied
.word HITS_TWO_TIMES 	@67 Double Kick, Gear Grind, etc. ;arg1 is status flag to be applied
.word 0x82D8D86			@68 Bide
.word PAYDAYY			@69 PayDay
.word ONEHITKO			@70 Sheer Cold, Fissure, etc.
.word TWOTURNMOVE		@71 if it's a status move it acts like multiple stat change user; else based on arg2: if 1 flinch + status chance(Sky Attack); if 2 single stat raise in first turn(Skull Bash); if 3 performs in one turn in weather in arg1(Solarbeam); if 4 status chance(Bounce); if 5 protection break(Phantom Force )
.word FORCETOSWITCH		@72 Roar, Whirlwind, etc.
.word ALLYSWITCH		@73 Ally Switch
.word 0x082D8D9A 		@74 Thrash, Outrage, Petal Dance
.word SETDISABLE		@75 Disable
.word SETENCOREE		@76 Encore
.word 0x082D9179 		@77 Mist
.word SETLEECHSEED		@78 Leech Seed
.word RAGEATTACK		@79 Rage
.word 0x082D9D7A 		@80 Teleport
.word MIMIC				@81 Mimic
.word MINIMIZESTATCHANGE		@82 arg1 is statvalue
.word DEFENSECURLSTATCHANGE		@83 arg1 is statvalue
.word 0x082D9288		@84 Reflect
.word 0x082D8F88 		@85	Light Screen
.word 0x082D8D75 		@86 Haze
.word 0x082D94BA		@87 Metronome
.word 0x082D8C62 		@88 Mirror Move
.word 0x082D8BF9 		@89 Dream Eater
.word NIGHTMARE			@90 Nightmare
.word 0x082D9249		@91 Transform
.word 0x082D94F5		@92 Splash
.word 0x082D8F9C		@93 Rest
.word CONVERSIONS		@94 Conversion and Conversion2, based on move IDs
.word TRIATTACK 		@95 Tri Attack
.word 0x082D9409		@96 Substitute
.word 0x082D962D		@97 Sketch
.word HITS_THREE_TIMES	@98 Triple Kick; Note: You can still use this script ID for moves that always hit 3 times without Triple Kick effect as it's read from its ID; plus arg1 is status flag that could be applied(like Twineedle)
.word ATTACK_STEALITEM	@99 thief, covet
.word NEXTHITWILLHIT	@100 Lock-ON, Mind Reader
.word 0x082D96A5 		@101 Spite
.word BELLYDRUMLIKE		@102 Belly Drum
.word 0x082D968E 		@103 destiny bond
.word 0x082D986D		@104 Curse
.word ROLLOUT         	@105 Rollout, Ice Ball
.word FURYCUTTER     	@106 fury cutter
.word 0x082D964C		@107 Sleep Talk
.word PARTYHEAL			@108 Heal Bell/Aromatherapy
.word PRESENTT          @109 Present
.word 0x082D9A8E		@110 Safeguard
.word 0x082D9585 		@111 Pain Split
.word 0x082DA2B6 		@112 Magic Coat
.word RAPID_SPIN        @113 Rapid Spin
.word 0x082D9BA1        @114 Psych Up
.word 0x082D9CC4		@115 Future Sight
.word BEAT_UP           @116 Beat Up
.word UPROAR            @117 Uproar
.word STOCKPILE_STUFF   @118 Stockpile, Swallow, Spit Up
.word MAGNITUDE			@119 Magnitude
.word 0x082DA189 		@120 Focus Punch
.word 0x082DA1D6		@121 Nature Power
.word 0x082DA210		@122 Helping Hand
.word ITEMSWAP			@123 Trick/Switcheroo
.word 0x082DA26F 		@124 Wish
.word 0x082DA27F		@125 Assist
.word 0x082DA296		@126 Ingrain
.word 0x082D9AB5 		@127 Baton Pass
.word 0x082DA2CB		@128 Recycle
.word SNORE				@129 Snore ;arg1 status flag to be applied
.word BRICKBREAK		@130 Brick Break
.word SETYAWN			@131 Yawn
.word KNOCKOFF			@132 Knock Off
.word 0x082DA3F8 		@133 Imprision
.word 0x082DA40D 		@134 Refresh
.word 0x082DA424		@135 Grudge
.word 0x082DA439		@136 Snatch
.word ATTACKING_MOVE @	".word SECRETPOWER			"	137
.word 0x082DA529		@138 Water/Mud Sport
.word BERRYDESTROY		@139 Incinerate, Pluck, Bug Bite; arg1 is 1 if berry is stolen and eaten, 0 if only destroyed
.word SETTAILWIND		@140 Tailwind
.word ACUPRESSURE		@141 Acupressure
.word ATTACKING_MOVE @	".word FLINGG				"	142
.word POWERTRICK		@143 Power Trick
.word SETLUCKYCHANT		@144 Lucky Chant
.word PSYCHIC_SWAPS		@145 arg1 stat to swap, arg2 stat to swap, if arg1 is 0 it acts like hearts swap
.word ME_FIRST       	@146 me first
.word COPYCAT           @147 Copycat
.word AQUA_RING         @148 Aqua Ring
.word MAGNET_RISE  		@149 Magnet Rise 
.word DEFOG             @150 Defog; arg1 is statchanger value
.word TERRAINCHANGE 	@151 Misty/Grassy/Electric Terrain
.word PSYCHIC_SPLITS	@152 Power/Guard Split; no args, read from ID
.word TELEKINESIS      	@153 Telekinesis
.word SMACK_DOWN 		@154 Smack Down
.word CHANGE_TARGET_TYPE_TO	@155 @arg1 is type to change target into
.word SHELLSMASH		@156 Shell Smash
.word ATTACKING_MOVE @	".word SKYDROP				"	157
.word SHIFTGEAR			@158 Shift Gear; arg1 stats to raise + 2; arg2 stats to raise + 1
.word QUASHH			@159 Quash
.word FAKEOUT			@160 Fake Out
.word 0x082D99B7		@161 Sandstorm
.word 0x082D9B3D		@162 Rain Dance
.word 0x082D9B55		@163 Sunny Day
.word 0x082D9FD2		@164 Hail
.word MAGNETICFLUX		@165 Magnetic Flux @arg1 bitfield for stats to raise; arg 2 by how much
.word VENOMDRENCH		@166 Venom Drench
.word FLOWERSHIELD		@167 Flower Shield @arg1 bitfield for stats to raise; arg 2 by how much
.word LASTRESORT		@168 Last Resort
.word TOPSYTURVY		@169 Topsy Turvy
.word BESTOW			@170 Bestow
.word TARGETSTATSWITCH	@171 Parting Shot; @arg1 bitfield for stats to raise; arg 2 by how much

ACUPRESSURE:
	attackcanceler
	attackstring
	ppreduce
	callasm_cmd 105
	.word MOVE_FAILED + 2
	statbuffchange 0x1 0x82D8D60
	goto_cmd 0x82D8D4E

PARTYHEAL:
	attackcanceler
	attackstring
	ppreduce
	attackanimation
	waitanimation
	call PARTYHEAL_STRING
	printfromtable 0x85CC904
	waitmessage 0x40
	setbyte 0x02024211 0x0
	callasm_cmd 104
	goto_cmd ENDTURN
	
.global PARTYHEAL_USER
PARTYHEAL_USER:
	printfromtable statushealstrings
	waitmessage 0x40
	statusiconeupdate bank_attacker
	return_cmd
	
.global PARTYHEAL_ALLY
PARTYHEAL_ALLY:
	printfromtable statushealstrings
	waitmessage 0x40
	statusiconeupdate 10
	return_cmd
	
.global PARTYHEAL_TEAM
PARTYHEAL_TEAM:
	printfromtable statushealstrings
	waitmessage 0x40
	return_cmd
	
PARTYHEAL_STRING:
	jumpifmove MOVE_HEAL_BELL STRING_SETZERO
	setbyte MultiStringChooser 4
	return_cmd
STRING_SETZERO:
	setbyte MultiStringChooser 0x0
	return_cmd

CONVERSIONS:
	attackcanceler
	accuracycheck MOVE_FAILED 0xFFFF
	attackstring
	ppreduce
	callasm_cmd 103
	.word MOVE_FAILED + 2
	goto_cmd 0x82DA79D

TARGETSTATSWITCH:
	attackcanceler
	jumpifsubstituteaffects MOVE_FAILED
	jumpifcantchangetwostats MOVE_FAILED
	accuracycheck MOVE_MISSED 0x0
	attackstring
	ppreduce
	attackanimation
	waitanimation
	callasm_cmd 29 @do all stat changes that are possible
	setbyte EndTurnTracker 0x0
	cmd49 0x0 0x0
	goto_cmd MOVE_TRY_SWITCHING

BESTOW:
	attackcanceler
	callasm_cmd 102
	.word MOVE_FAILED
	accuracycheck MOVE_FAILED 0xFFFF
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printstring 0x20A
	waitmessage 0x40
	goto_cmd ENDTURN

TOPSYTURVY:
	attackcanceler
	callasm_cmd 101
	.word MOVE_FAILED
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printstring 0x209
	waitmessage 0x40
	goto_cmd ENDTURN

LASTRESORT:
	attackcanceler
	callasm_cmd 100
	.word MOVE_FAILED
	goto_cmd ATTACKING_MOVE + 1

FLOWERSHIELD:
	attackcanceler
	setbyte 0x02024211 0x0
	callasm_cmd 98
	.word MOVE_FAILED
	attackstring
	ppreduce
	attackanimation
	waitanimation
	callasm_cmd 99
	goto_cmd ENDTURN

VENOMDRENCH:
	jumpifstatus bank_target STATUS_POISON | STATUS_BAD_POISON MULTIPLE_STAT_CHANGE_TARGET
	attackcanceler
	goto_cmd MOVE_FAILED	

MAGNETICFLUX:
	attackcanceler
	callasm_cmd 96
	.word MOVE_FAILED
	attackstring
	ppreduce
	attackanimation
	waitanimation
	callasm_cmd 97
	goto_cmd ENDTURN

ALLYSWITCH:
	attackcanceler
	callasm_cmd 94
	.word MOVE_FAILED
	attackstring
	ppreduce
	attackanimation
	callasm_cmd 95
	waitanimation
	printstring 0x208
	waitmessage 0x40
	goto_cmd ENDTURN

BEAT_UP:
	attackcanceler
	accuracycheck MOVE_MISSED 0x0
	attackstring
	ppreduce
	callasm_cmd 93 @sets multihit counter
	preparemultihit
	setbyte 0x202448A 0x0
	goto_cmd MULTIHIT_LOOP_START

QUASHH:
	attackcanceler
	callasm_cmd 92
	.word MOVE_FAILED
	accuracycheck MOVE_MISSED 0x0
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printstring 0x207
	waitmessage 0x40
	goto_cmd ENDTURN

PRESENTT:
	attackcanceler
	accuracycheck MOVE_MISSED 0x0
	jumpiftypenotaffected MOVE_FAILED
	attackstring
	ppreduce
	presentdamagecalculation

SHELLSMASH:
	attackcanceler
	attackstring
	ppreduce
	jumpifstat bank_attacker 0x1 0x2 0x0 SHELLSMASHDEF
	jumpifstat bank_attacker 0x1 0x5 0x0 SHELLSMASHDEF
	jumpifstat bank_attacker 0x1 0x1 0xC SHELLSMASHDEF
	jumpifstat bank_attacker 0x1 0x3 0xC SHELLSMASHDEF
	jumpifstat bank_attacker 0x0 0x4 0xC MOVE_FAILED + 2
SHELLSMASHDEF:
	attackanimation
	waitanimation
	setbyte StatChanger 0x92
	statbuffchange 0x41 SHELLSMASHSPDEF
	jumpifbyte 0x1 0x2024337 0x2 SHELLSMASHDEFCHANGE
	pause_cmd 0x10
	call STATBUFF_CHANGE_NEGATIVE_MSG
	goto_cmd SHELLSMASHSPDEF
SHELLSMASHDEFCHANGE:
	call STATBUFF_CHANGE_NEGATIVE
SHELLSMASHSPDEF:
	setbyte StatChanger 0x95
	statbuffchange 0x41 SHELLSMASHATK
	jumpifbyte 0x1 0x2024337 0x2 SHELLSMASHSPDEF_CHANGE
	pause_cmd 0x10
	call STATBUFF_CHANGE_NEGATIVE_MSG
	goto_cmd SHELLSMASHATK
SHELLSMASHSPDEF_CHANGE:
	call STATBUFF_CHANGE_NEGATIVE
SHELLSMASHATK:
	setbyte StatChanger 0x21
	statbuffchange 0x41 SHELLSMASHSPATK
	jumpifbyte 0x1 0x2024337 0x2 SHELLSMASHATK_CHANGE
	pause_cmd 0x10
	call STATBUFF_CHANGE_POSITIVE_MSG
	goto_cmd SHELLSMASHSPATK
SHELLSMASHATK_CHANGE:
	call STATBUFF_CHANGE_POSITIVE
SHELLSMASHSPATK:
	setbyte StatChanger 0x24
	statbuffchange 0x41 SHELLSMASHSPD
	jumpifbyte 0x1 0x2024337 0x2 SHELLSMASHSPATK_CHANGE
	pause_cmd 0x10
	call STATBUFF_CHANGE_POSITIVE_MSG
	goto_cmd SHELLSMASHSPD
SHELLSMASHSPATK_CHANGE:
	call STATBUFF_CHANGE_POSITIVE
SHELLSMASHSPD:
	setbyte StatChanger 0x23
	statbuffchange 0x41 ENDTURN
	jumpifbyte 0x1 0x2024337 0x2 SHELLSMASHSPD_CHANGE
	pause_cmd 0x10
	call STATBUFF_CHANGE_POSITIVE_MSG
	goto_cmd ENDTURN
SHELLSMASHSPD_CHANGE:
	call STATBUFF_CHANGE_POSITIVE
	goto_cmd ENDTURN
	
STATBUFF_CHANGE_POSITIVE:
	cmd47
	playanimation 0x1 0x1 0x2024484
STATBUFF_CHANGE_POSITIVE_MSG:
	printfromtable 0x85CC89C
	waitmessage 0x40
	return_cmd
	
STATBUFF_CHANGE_NEGATIVE:
	cmd47
	playanimation 0x1 0x1 0x2024484
STATBUFF_CHANGE_NEGATIVE_MSG:
	printfromtable 0x85CC8A8
	waitmessage 0x40
	return_cmd

SHIFTGEAR:
	attackcanceler
	callasm_cmd 28
	.word MOVE_FAILED
	attackstring
	ppreduce
	attackanimation
	waitanimation
	setbyte StatChanger 0x23
	statbuffchange bank_attacker | Override SHIFTGEARATTACK
	jumpifbyte 0x1 0x2024337 0x2 SHIFTGEARSPEED
	pause_cmd 0x10
	goto_cmd SHIFTGEARSPEEDMSG
SHIFTGEARSPEED:
	call STATBUFF_CHANGE_POSITIVE
	goto_cmd SHIFTGEARATTACK
SHIFTGEARSPEEDMSG:
	call STATBUFF_CHANGE_POSITIVE_MSG
SHIFTGEARATTACK:
	setbyte StatChanger 0x11
	statbuffchange bank_attacker | Override SHIFTGEAREND
	jumpifbyte 0x1 0x2024337 0x2 SHIFTGEARATTACKBOOST
	pause_cmd 0x10
	goto_cmd SHIFTGEARATTACKBOOST_MSG
SHIFTGEARATTACKBOOST:
	call STATBUFF_CHANGE_POSITIVE
	goto_cmd SHIFTGEAREND
SHIFTGEARATTACKBOOST_MSG:
	call STATBUFF_CHANGE_POSITIVE_MSG
SHIFTGEAREND:
	goto_cmd ENDTURN
	

AQUA_RING:
	attackcanceler
	callasm_cmd 87
	.word MOVE_FAILED
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printstring 0x1FE
	waitmessage 0x40
	goto_cmd ENDTURN

TERRAINCHANGE:
	attackcanceler
	callasm_cmd 86
	.word MOVE_FAILED
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printfromtable terrainstrings
	waitmessage 0x40
	goto_cmd ENDTURN

MIMIC:
	attackcanceler
	jumpifsubstituteaffects MOVE_FAILED
	accuracycheck MOVE_FAILED 0xFFFF
	attackstring
	ppreduce
	copyattack 0x82D9F1C
	attackanimation
	waitanimation
	printstring 0x16
	waitmessage 0x40
	goto_cmd ENDTURN

STOCKPILE_STUFF:
	jumpifmove 0xFE STOCKPILE
	jumpifmove 0x100 SWALLOW
SPIT_UP:
	attackcanceler
	attackstring
	ppreduce
	stockpiletobasedamage 0x82D9F94
	accuracycheck SPITUP_FAIL 0x0
	jumpiftypenotaffected SPITUP_FAIL
	critcalc
	damagecalc
	damageadjustment
	attackanimation
	waitanimation
	effectiveness_sound
	hitanim bank_target
	waitstate
	graphicalhpupdate bank_target
	datahpupdate bank_target
	critmessage
	waitmessage 0x40
	resultmessage
	waitmessage 0x40
	faintpokemon bank_target 0x0 0x0
	callasm_cmd 83 @stockpile record stat buffs reset
	.byte 0x2
	printstring 0x1F5
	waitmessage 0x40
	goto_cmd ENDTURN
SPITUP_FAIL:
	callasm_cmd 83 @stockpile record stat buffs reset
	.byte 0x2
	resultmessage
	waitmessage 0x40
	goto_cmd ENDTURN
STOCKPILE:
	attackcanceler
	attackstring
	ppreduce
	stockpile
	jumpifbyte 0x0 MultiStringChooser 0x1 STOCKPILE_NOMORE
	attackanimation
	waitanimation
	printstring 0x73
	waitmessage 0x40
	jumpifcantchangetwostats ENDTURN
	callasm_cmd 83 @stockpile record
	.byte 0x0
	callasm_cmd 29 @do all stat changes that are possible
	callasm_cmd 83 @stockpile record
	.byte 0x1
	goto_cmd ENDTURN
STOCKPILE_NOMORE:
	printstring 0x74
	waitmessage 0x40
	goto_cmd ENDTURN
SWALLOW:
	attackcanceler
	attackstring
	ppreduce
	stockpiletohprecovery 0x82D9FC2
	attackanimation
	waitanimation
	orword 0x2024280 0x100
	graphicalhpupdate 0x0
	datahpupdate 0x0
	printstring 0x4B
	waitmessage 0x40
	callasm_cmd 83 @stockpile record stat buffs reset
	.byte 0x2
	printstring 0x1F5
	waitmessage 0x40
	goto_cmd ENDTURN

PSYCHIC_SPLITS:
	attackcanceler
	accuracycheck MOVE_FAILED 0xFFFF
	attackstring
	ppreduce
	attackanimation
	waitanimation
	callasm_cmd 82
	printstring 0x1F4
	waitmessage 0x40
	goto_cmd ENDTURN

PSYCHIC_SWAPS:
	attackcanceler
	accuracycheck MOVE_FAILED 0xFFFF
	attackstring
	ppreduce
	attackanimation
	waitanimation
	callasm_cmd 81
	printfromtable swaps_strings
	waitmessage 0x40
	goto_cmd ENDTURN

BERRYDESTROY:
	setbyte EffectChooser 0x36
	goto_cmd ATTACKING_MOVE

COPYCAT:
	attackcanceler
	callasm_cmd 80 @check if copycat is usable
	.word MOVE_FAILED
	attackstring
	ppreduce
	attackanimation
	waitanimation
	callasm_cmd 70

DEFOG:
	attackcanceler
	setbyte MultiStringChooser 0x0
	jumpifsubstituteaffects DEFOG_HAZARD_REMOVAL
	accuracycheck DEFOG_HAZARD_REMOVAL 0xFFFF
	attackstring
	ppreduce
	callasm_cmd 25 @sets stat to change based on arg1
	statbuffchange 0x1 DEFOG_HAZARD_REMOVAL
	jumpifbyte 0x3 0x2024337 0x2 DEFOG_STAT_LOWER
	jumpifbyte 0x0 0x2024337 0x3 DEFOG_HAZARD_REMOVAL
	pause_cmd 0x20
	goto_cmd DEFOG_STAT_MSG
DEFOG_STAT_LOWER:
	attackanimation
	waitanimation
	cmd47
	playanimation 0x0 0x1 0x2024484
DEFOG_STAT_MSG:
	setbyte MultiStringChooser 1
	printfromtable 0x85CC8A8
	waitmessage 0x40
DEFOG_HAZARD_REMOVAL:
	callasm_cmd 79
	.word MOVE_FAILED
	jumpifbyte 0x0 MultiStringChooser 0x1 DEFOG_AFTERANIMATION
	attackanimation
	waitanimation
DEFOG_AFTERANIMATION:
	goto_cmd ENDTURN

ITEMSWAP:
	attackcanceler
	jumpifsubstituteaffects MOVE_FAILED
	accuracycheck MOVE_FAILED 0x0
	attackstring
	ppreduce
	itemswap MOVE_FAILED + 0x2
	goto_cmd 0x082DA23E

CHANGE_TARGET_TYPE_TO:
	attackcanceler
	jumpifsubstituteaffects MOVE_FAILED
	jumpifability bank_target 0x79 MOVE_FAILED @Multitype check
	accuracycheck MOVE_MISSED 0x0
	attackstring
	ppreduce
	attackanimation
	waitanimation
	callasm_cmd 78
	.word MOVE_FAILED
	printstring 0x1F0
	waitmessage 0x40
	goto_cmd ENDTURN

POWERTRICK:
	attackcanceler
	callasm_cmd 77
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printstring 0x1EF
	waitmessage 0x40
	goto_cmd ENDTURN

TELEKINESIS:
	attackcanceler
	jumpifsubstituteaffects MOVE_FAILED
	accuracycheck MOVE_FAILED 0xFFFF
	callasm_cmd 76
	.word MOVE_FAILED
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printstring 0x1D1
	waitmessage 0x40
	goto_cmd ENDTURN

MAGNET_RISE:
	attackcanceler
	callasm_cmd 75
	.word MOVE_FAILED
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printstring 0x1CC
	waitmessage 0x40
	goto_cmd ENDTURN

TWOTURNMOVE: @if it's a status move it acts like multiple stat change user; else based on arg2: if 1 flinch + status chance(Sky Attack); if 2 single stat raise in first turn(Skull Bash); if 3 performs in one turn in sunny weather(Solarbeam); if 4 status chance(Bounce); if 5 protection break(Phantom Force )
	attackcanceler
	callasm_cmd 84
	.word TWOTURNSTATCHANGE
	.word TWOTURNFLINCHCHANCE
	.word TWOTURNSTATRAISE_THENATTACK
	.word TWOTURN_MAYBEONETURNIFWEATHER
	.word TWOTURN_STATUSCHANCE
	.word TWOTURN_PROTECTIONBREAK
TWOTURNSTATCHANGE:
	jumpifsecondarystatus bank_attacker 0x1000 TWOTURNSTATCHANGE_ACTUALMOVE
	call TWOTURN_LOADINGTURN
	callasm_cmd 85 @power herb check
	.word TWOTURNSTATCHANGE_ACTUALMOVE
	call SETTWOTURN
	goto_cmd ENDTURN
TWOTURNSTATCHANGE_ACTUALMOVE:
	call CLEAR_TWOTURN
	goto_cmd MULTIPLE_STAT_CHANGE_USER + 1
	
TWOTURN_LOADINGTURN:
	ppreduce
	printstring 0x130 @this string is empty, it's here to change textbox
	attackanimation
	waitanimation
	printfromtable twoturnstrings
	waitmessage 0x40
	return_cmd
	
CLEAR_TWOTURN:
	setbyte 0x202448C 0x1
	setbyte 0x2024335 0xC
	clearstatus bank_attacker
	orword 0x2024280 0x800
	unhidepostattack
	return_cmd
SETTWOTURN:
	hidepreattack
	setbyte EffectChooser 0x37 | 0x40
	seteffectprimary
	return_cmd
	
TWOTURNFLINCHCHANCE:
	jumpifsecondarystatus bank_attacker 0x1000 TWOTURNFLINCHCHANCE_ACTUALMOVE
	call TWOTURN_LOADINGTURN
	callasm_cmd 85 @power herb check
	.word TWOTURNFLINCHCHANCE_ACTUALMOVE
	call SETTWOTURN
	goto_cmd ENDTURN
	
TWOTURNFLINCHCHANCE_ACTUALMOVE:
	call CLEAR_TWOTURN
	goto_cmd ATTACK_FLINCH_CHANCE
	
TWOTURNSTATRAISE_THENATTACK:
	jumpifsecondarystatus bank_attacker 0x1000 TWOTURNSTATRAISE_ACTUALMOVE
	call TWOTURN_LOADINGTURN
	callasm_cmd 25 @sets stat to change based on arg1
	statbuffchange 0x41 TWOTURNSTATDIDNTWORK @won't grow any higher
	jumpifbyte 0x1 0x2024337 0x2 TWOTURNSTAT_WORKED @worked
	pause_cmd 0x20
TWOTURNSTATDIDNTWORK:
	printfromtable 0x85CC89C
	waitmessage 0x40
	goto_cmd TWOTURNSTATRAISE_THENATTACK__
TWOTURNSTAT_WORKED:
	cmd47
	playanimation 0x1 0x1 0x2024484
	printfromtable 0x85CC89C
	waitmessage 0x40
TWOTURNSTATRAISE_THENATTACK__:
	callasm_cmd 85 @power herb check
	.word TWOTURNSTATRAISE_ACTUALMOVE
	call SETTWOTURN
	goto_cmd ENDTURN
TWOTURNSTATRAISE_ACTUALMOVE:
	call CLEAR_TWOTURN
	goto_cmd ATTACKING_MOVE + 1
	
TWOTURN_MAYBEONETURNIFWEATHER:
	jumpifsecondarystatus bank_attacker 0x1000 TWOTURN_MAYBEONETURNIFWEATHER_ACTUALMOVE
	call TWOTURN_LOADINGTURN
	callasm_cmd 85 @power herb check
	.word TWOTURNFLINCHCHANCE_ACTUALMOVE
	jumpifabilitypresent 0xD NOWEATHERJUMP
	jumpifabilitypresent 0x4D NOWEATHERJUMP
	jumpifhalfword 0x4 0x20243CC 0x860 TWOTURN_MAYBEONETURNIFWEATHER_ACTUALMOVE
NOWEATHERJUMP:
	call SETTWOTURN
	goto_cmd ENDTURN
	
TWOTURN_MAYBEONETURNIFWEATHER_ACTUALMOVE:
	call CLEAR_TWOTURN
	goto_cmd ATTACKING_MOVE + 1
TWOTURN_STATUSCHANCE:
	jumpifsecondarystatus bank_attacker 0x1000 TWOTURN_STATUSCHANCE_ACTUALMOVE
	call TWOTURN_LOADINGTURN
	callasm_cmd 85 @power herb check
	.word TWOTURN_STATUSCHANCE_ACTUALMOVE
	call SETTWOTURN
	goto_cmd ENDTURN
TWOTURN_STATUSCHANCE_ACTUALMOVE:
	call CLEAR_TWOTURN
	goto_cmd ATTACK_STATUS_CHANCE
	
TWOTURN_PROTECTIONBREAK:
	jumpifsecondarystatus bank_attacker 0x1000 TWOTURN_PROTECTIONBREAK_ACTUALMOVE
	call TWOTURN_LOADINGTURN
	callasm_cmd 85 @power herb check
	.word TWOTURN_PROTECTIONBREAK_ACTUALMOVE
	call SETTWOTURN
	goto_cmd ENDTURN
TWOTURN_PROTECTIONBREAK_ACTUALMOVE:
	call CLEAR_TWOTURN
	goto_cmd ATTACK_PROTECTION_BREAK
	
SNORE:
	attackcanceler
	jumpifstatus bank_attacker 0x7 SNORE_WORKED
	goto_cmd MOVE_FAILED
SNORE_WORKED:
	jumpifhalfword 0x0 0x20241EC 0xD6 SNORE_GOTO
	printstring 0x6B
	waitmessage 0x40
	statusanimation bank_attacker
SNORE_GOTO:
	goto_cmd ATTACK_FLINCH_CHANCE_CANCELLER_DONE

KNOCKOFF:
	setbyte EffectChooser 0x10
	goto_cmd ATTACKING_MOVE

UPROAR:
	attackcanceler
	accuracycheck MOVE_MISSED 0x0
	setbyte EffectChooser 0x34 | 0x40
	attackstring
	jumpifsecondarystatus bank_target 0x1000 0x82D8A30
	ppreduce
	goto_cmd 0x82D8A30

RAPID_SPIN:
	setbyte EffectChooser 15
	goto_cmd ATTACKING_MOVE

SMACK_DOWN:
	setbyte EffectChooser 14
	orword HitMarker HitMarkerHitsOnAir
	goto_cmd ATTACKING_MOVE

TRIATTACK:
	setbyte EffectChooser 0x9
	goto_cmd ATTACKING_MOVE

ROLLOUT:
	attackcanceler
	attackstring
	jumpifsecondarystatus 0x1 0x1000 NOPPREDUCE
	ppreduce
NOPPREDUCE:
	accuracycheck ROLLOUT_CALC 0x0
	jumpiftypenotaffected ROLLOUT_CALC
ROLLOUT_CALC:
	rolloutdamagecalculation
	critcalc
	damagecalc
	damageadjustment
	goto_cmd ATTACK_ANIM

NEXTHITWILLHIT:
	attackcanceler
	jumpifsubstituteaffects MOVE_FAILED
	jumpifspecialstatus3 bank_attacker 0x18 0x0 MOVE_FAILED
	accuracycheck MOVE_FAILED 0xFFFF
	attackstring
	ppreduce
	goto_cmd 0x082D961F

BELLYDRUMLIKE:
	attackcanceler
	callasm_cmd 74
	.word MOVE_FAILED
	attackstring
	ppreduce
	orword 0x2024280 0x100
	graphicalhpupdate bank_attacker
	datahpupdate bank_attacker
	printstring 0x9B
	waitmessage 0x40
	goto_cmd ENDTURN

NIGHTMARE:
	attackcanceler
	jumpifsubstituteaffects MOVE_FAILED
	callasm_cmd 73
	.word MOVE_FAILED
	accuracycheck MOVE_FAILED 0xFFFF
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printstring 0x90
	waitmessage 0x40
	goto_cmd ENDTURN
	
HITS_THREE_TIMES:
	attackcanceler
	jumpifmove 0xA7 PREPARE_TRIPLE_KICK
	accuracycheck MOVE_MISSED 0x0
NOACCURACYTRIPLEKICKYET:
	attackstring
	ppreduce
	setloopcounter 0x3
	preparemultihit
	setbyte 0x202448A 0x0
	goto_cmd MULTIHIT_LOOP_START
PREPARE_TRIPLE_KICK:
	bichalfword 0x02024400 0xFFFF @dynamic base power equals 0
	goto_cmd NOACCURACYTRIPLEKICKYET

HITS_MULTIPLE_TIMES:
	attackcanceler
	accuracycheck MOVE_MISSED 0x0
	attackstring
	ppreduce
	setloopcounter 0x0
	preparemultihit
	setbyte 0x202448A 0x0
	goto_cmd MULTIHIT_LOOP_START

HITS_TWO_TIMES:
	attackcanceler
	accuracycheck MOVE_MISSED 0x0
	attackstring
	ppreduce
	setloopcounter 0x2
	preparemultihit
	setbyte 0x202448A 0x0
MULTIHIT_LOOP_START:
	jumpiffainted bank_attacker MULTIHIT_END
	jumpiffainted bank_target MULTIHIT_RESULTMESSAGE
	jumpifhalfword 0x0 0x20241EC 0xD6 MULTIHIT_LOOP
	jumpifstatus bank_attacker 0x7 MULTIHIT_RESULTMESSAGE
MULTIHIT_LOOP:
	jumpifnotmove 0xA7 AFTER_TRIPLEKICKEFFECTS
	accuracycheck TRIPLEKICK_SKILLLINKCHECK 0x0
TRIPLEKICK_DAMAGE:
	addbyte 0x02024400 0xA @dynamic base power
AFTER_TRIPLEKICKEFFECTS:
	cmd25
	copyarray 0x2024335 0x202448A 0x1
	critcalc
	damagecalc
	jumpifbyte 0x4 MoveOutcome OutcomeFailed | OutcomeMissed | OutcomeNotaffected MULTIHIT_RESULTMESSAGE
	damageadjustment
	attackanimation
	waitanimation
	effectiveness_sound
	hitanim bank_target
	waitstate
	graphicalhpupdate bank_target
	datahpupdate bank_target
	critmessage
	waitmessage 0x40
	call STURDY_MESSAGE
	printstring 0x130
	waitmessage 0x1
	addbyte 0x2024480 0x1
	callasm_cmd 36 @changes status flag in arg1 to correct effect chooser value
	seteffectwithchance
	setbyte EndTurnTracker 0x0
	cmd49 0x2 20
	jumpifbyte 0x4 0x202427C 0x40 MULTIHIT_RESULTMESSAGE
	cmd27 MULTIHIT_LOOP_START
MULTIHIT_RESULTMESSAGE:
	resultmessage
	waitmessage 0x40
	jumpifbyte 0x0 0x2024480 0x0 MULTIHIT_END
	jumpifbyte 0x4 0x202427C 0x29 MULTIHIT_END
	copyarray 0x2022F58 0x202447C 0x6
	printstring 0x22
	waitmessage 0x40
MULTIHIT_END:
	faintpokemon bank_target 0x0 0x0
	setbyte EndTurnTracker 20
	cmd49 0x0 0x0
	end_cmd
STURDY_MESSAGE:
	jumpifhalfword 0x4 MoveOutcome OutcomeSturdied STURDYMSG
	jumpifhalfword 0x4 MoveOutcome 0x80 STURDYMSG
	return_cmd
STURDYMSG:
	copyarray MoveOutcome EndTurnTracker 1
	bicbyte MoveOutcome 0x7F
	resultmessage
	waitmessage 0x40
	copyarray EndTurnTracker MoveOutcome 1
	bichalfword MoveOutcome OutcomeSturdied | 0x80
	return_cmd
TRIPLEKICK_SKILLLINKCHECK:
	jumpifability bank_attacker 0x5C SKILLLINK_ALWAYS_THREETIMES
	goto_cmd TRIPLEKICK_MISS
SKILLLINK_ALWAYS_THREETIMES:
	jumpifbyte 0x1 0x2024480 0x0 TRIPLEKICK_DAMAGE
TRIPLEKICK_MISS:
	resultmessage
	waitmessage 0x40
	bicbyte MoveOutcome OutcomeMissed
	goto_cmd MULTIHIT_RESULTMESSAGE
	
ONEHITKO:
	attackcanceler
	accuracycheck MOVE_FAILED 0xFFFF
	jumpiftypenotaffected MOVE_FAILED
	attackstring
	ppreduce
	koplussomethings 0x82D9008
	damageadjustment
	cmdab
	goto_cmd ATTACK_ANIM

FORCETOSWITCH:
	attackcanceler
	attackstring
	ppreduce
	jumpifability bank_target 0x15 SUCTIONCUPSNOSWITCH
	jumpifspecialstatus3 bank_target 0x400 0x0 0x82DB109
	accuracycheck MOVE_FAILED + 2 0xFFFF
	jumpifword 0x4 0x2022FEC 0x40000 MOVE_FAILED + 2
	forcerandomswitch MOVE_FAILED + 2

SUCTIONCUPSNOSWITCH:
	recordability bank_target
	goto_cmd 0x082DB5B9
	
BRICKBREAK:
	attackcanceler
	accuracycheck MOVE_MISSED 0x0
	jumpiftypenotaffected MOVE_FAILED
	attackstring
	ppreduce
	removereflectlightscreen
	attackanimation
	waitanimation
	critcalc
	damagecalc
	damageadjustment
	goto_cmd 0x082DA31B

SETTAILWIND:
	attackcanceler
	callasm_cmd 72
	.word MOVE_FAILED
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printstring 0x1CA
	waitmessage 0x40
	goto_cmd ENDTURN

SETLUCKYCHANT:
	attackcanceler
	callasm_cmd 71
	.word MOVE_FAILED
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printstring 0x1CB
	waitmessage 0x40
	goto_cmd ENDTURN

MAGNITUDE:
	attackcanceler
	jumpifword 0x4 HitMarker 0x200 MAGNITUDEACC
	attackstring
	ppreduce
	magnitudedamagecalculation
	pause_cmd 0x20
	printstring 0x9A
	waitmessage 0x40
MAGNITUDEACC:
	accuracycheck MOVE_MISSED + 2 0x0
	critcalc
	damagecalc
	damageadjustment
	goto_cmd ATTACK_ANIM

ME_FIRST:
	attackcanceler
	callasm_cmd 69 @check if me first is usable
	.word MOVE_FAILED
	attackstring
	ppreduce
	attackanimation
	waitanimation
	callasm_cmd 70

ATTACK_STEALITEM:
	setbyte EffectChooser 13
	goto_cmd ATTACKING_MOVE

DEFENSECURLSTATCHANGE:
	attackcanceler
	setcurled
	goto_cmd ONE_STAT_USER_WORKED

MINIMIZESTATCHANGE:
	attackcanceler
	setminimize
	goto_cmd ONE_STAT_USER_WORKED

PAYDAYY:
	setbyte EffectChooser 0x33 + 0x40
	goto_cmd ATTACKING_MOVE

RAGEATTACK:
	setbyte EffectChooser 0x32 + 0x40
	goto_cmd ATTACKING_MOVE

FAKEOUT:
	attackcanceler
	jumpifnotfirstturn MOVE_FAILED
	accuracycheck MOVE_MISSED 0x0
	setbyte EffectChooser 0x88
	critcalc
	damagecalc
	goto_cmd SUCCESS_MOVE_ATTACK
	
SETYAWN:
	goto_cmd 0x082DA337

FURYCUTTER:
	attackcanceler
	accuracycheck FURYCUTTERMISS 0x0
	jumpiftypenotaffected FURYCUTTERNOTAFFECTED
	furycutterdamagecalculation
	critcalc
	damagecalc
	goto_cmd SUCCESS_MOVE_ATTACK
FURYCUTTERMISS:
	orbyte MoveOutcome OutcomeMissed
FURYCUTTERNOTAFFECTED:
	furycutterdamagecalculation
	
SETDISABLE:
	attackcanceler
	accuracycheck MOVE_MISSED 0x0
	attackstring
	ppreduce
	disablelastusedattack 0x82D9F1C
	attackanimation
	waitanimation
	printstring 0x84
	waitmessage 0x40
	goto_cmd ENDTURN

SETENCOREE:
	attackcanceler
	accuracycheck MOVE_MISSED 0x0
	setencore MOVE_FAILED
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printstring 0x7B + 0xC
	waitmessage 0x40
	goto_cmd ENDTURN

SETLEECHSEED:
	attackcanceler
	jumpifsubstituteaffects MOVE_FAILED
	accuracycheck MOVE_MISSED 0x0
	setleechseed
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printfromtable 0x85CC878
	waitmessage 0x40
	goto_cmd ENDTURN

PERISHSONG:
	attackcanceler
	setperishsong MOVE_FAILED
	attackstring
	ppreduce
	goto_cmd 0x82D997E

ESCAPEPREVENTION:
	attackcanceler
	jumpifsubstituteaffects MOVE_FAILED
	jumpifsecondarystatus 0x0 0x4000000 MOVE_FAILED
	accuracycheck MOVE_FAILED 0xFFFF
	attackstring
	ppreduce
	attackanimation
	waitanimation
	callasm_cmd 68 @sets the trapped bit
	printstring 0x8F
	waitmessage 0x40
	goto_cmd ENDTURN

HEALBLOCK:
	attackcanceler
	callasm_cmd 67
	.word MOVE_FAILED
	accuracycheck MOVE_MISSED 0x0
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printstring 0x1EA
	waitmessage 0x40
	goto_cmd ENDTURN

TORMENTT:
	attackcanceler
	settorment MOVE_FAILED
	accuracycheck MOVE_MISSED 0x0
	attackstring
	ppreduce
	goto_cmd 0x082D9FEA

TAUNTT:
	attackcanceler
	settaunt MOVE_FAILED
	accuracycheck MOVE_MISSED 0x0
	attackstring
	ppreduce
	goto_cmd 0x082DA203

FOCUSENERGY:
	attackcanceler
	jumpifsecondarystatus bank_attacker 0x100000 MOVE_FAILED
	setincreasedcriticalchance
	attackstring
	ppreduce
	goto_cmd 0x82D919A

REFLECTTYPE:
	attackcanceler
	callasm_cmd 66
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printstring 0x1E9
	waitmessage 0x40
	goto_cmd ENDTURN

CAMOUFLAGE:
	attackcanceler
	attackstring
	ppreduce
	settypetoterrain MOVE_FAILED + 2
	goto_cmd 0x82DA79D
	
IONDELUGE:
	attackcanceler
	callasm_cmd 65 @sets Ion Deluge
	.word MOVE_FAILED
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printstring 0x1E8
	waitmessage 0x40
	goto_cmd ENDTURN

WEIGHTLOSE_STATCHANGE:
	attackcanceler
	attackstring
	ppreduce
	callasm_cmd 25 @sets stat to change based on arg1
	statbuffchange 0x41 STATDIDNTWORK @won't grow any higher
	jumpifbyte 0x1 0x2024337 0x2 STAT_WORKED @worked
	pause_cmd 0x20
STATDIDNTWORK:
	bicbyte MoveOutcome OutcomeFailed | OutcomeMissed | OutcomeNotaffected
	callasm_cmd 64 @do autonomize 
	.word MOVE_FAILED
	attackanimation
	waitanimation
	call PRINTWEIGHT
	goto_cmd 0x82D8CC7
PRINTWEIGHT:
	printstring 0x1E7
	waitmessage 0x40
	return_cmd
STAT_WORKED:
	callasm_cmd 64 @do autonomize
	.word 0x82D8CBD
	attackanimation
	waitanimation
	call PRINTWEIGHT
	goto_cmd 0x82D8CBF
	
ATTACK_ONLYIFSHARETYPE:
	attackcanceler
	callasm_cmd 63 @jump if user and target don't share type
	.word MOVE_FAILED
	accuracycheck MOVE_MISSED 0x0
	critcalc
	damagecalc
	goto_cmd SUCCESS_MOVE_ATTACK

FOLLOWME:
	attackcanceler
	callasm_cmd 62 @jump if not double battle
	.word MOVE_FAILED
	goto_cmd 0x82DA1C6

ENTRYHAZARDS:
	attackcanceler
	callasm_cmd 61 @sets hazards
	.word MOVE_FAILED
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printfromtable hazards_strings
	waitmessage 0x40
	goto_cmd ENDTURN

ELECTRIFY_MOVE:
	attackcanceler
	accuracycheck MOVE_FAILED 0xFFFF
	callasm_cmd 60
	.word MOVE_FAILED
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printstring 0x1E3
	waitmessage 0x40
	goto_cmd ENDTURN

ATTACKCLEARSTATCHANGES:
	setbyte EffectChooser 12
	attackcanceler
	accuracycheck MOVE_FAILED 0xFFFF
	critcalc
	damagecalc
	goto_cmd SUCCESS_MOVE_ATTACK

CHANGEALLYSTAT:
	attackcanceler
	jumpifnoally MOVE_FAILED
	jumpifsubstituteaffects MOVE_FAILED
	accuracycheck MOVE_FAILED 0xFFFF
	goto_cmd ONE_STAT_TARGET_WORKED

POWDER:
	attackcanceler
	accuracycheck MOVE_MISSED 0x0
	callasm_cmd 58 @sets powder
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printstring 0x1E0
	waitmessage 0x40
	goto_cmd ENDTURN

AFTERYOU:
	attackcanceler
	accuracycheck MOVE_FAILED 0xFFFF
	callasm_cmd 57 @after you check
	.word MOVE_FAILED
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printstring 0x1DF
	waitmessage 0x40
	goto_cmd ENDTURN

NATURALGIFT:
	attackcanceler
	accuracycheck MOVE_FAILED 0x0
	callasm_cmd 56 @checks if can attack
	.word MOVE_FAILED
	critcalc
	damagecalc
	removeitem bank_attacker
	goto_cmd SUCCESS_MOVE_ATTACK
	
EMBARGO:
	attackcanceler
	accuracycheck MOVE_FAILED 0x0
	callasm_cmd 55 @sets the bit
	.word MOVE_FAILED
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printstring 0x1DE
	waitmessage 0x40
	goto_cmd ENDTURN

GASTROACID:
	attackcanceler
	accuracycheck MOVE_FAILED 0x0
	callasm_cmd 54 @sets the bit
	.word MOVE_FAILED
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printstring 0x1DD
	waitmessage 0x38
	goto_cmd ENDTURN

COUNTER:
	attackcanceler
	callasm_cmd 53 @sets damage
	.word MOVE_FAILED
	accuracycheck MOVE_FAILED 0x0
	goto_cmd SUCCESS_MOVE_ATTACK

ROOMS:
	attackcanceler
	attackstring
	ppreduce
	attackanimation
	waitanimation
	callasm_cmd 52 @sets bit, chooses string
	printfromtable rooms_strings
	waitmessage 0x40
	goto_cmd ENDTURN

ABILITY_CHANGE:
	attackcanceler
	callasm_cmd 51 @changes abilities
	.word MOVE_FAILED
	accuracycheck MOVE_MISSED 0xFFFF
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printfromtable abilitychange_strings
	waitmessage 0x40
	goto_cmd ENDTURN

ADD_THIRD_TYPE:
	attackcanceler
	jumpifsubstituteaffects MOVE_FAILED
	attackstring
	ppreduce
	callasm_cmd 50 @sets type
	.word (MOVE_FAILED + 2)
	accuracycheck MOVE_MISSED + 2 0x0
	attackanimation
	waitanimation
	printstring 0x1DB
	waitmessage 0x40
	goto_cmd ENDTURN

ATTACK_AND_TRAP:
	setbyte EffectChooser 11
	goto_cmd ATTACKING_MOVE

RECHARGE_NEEDED:
	setbyte EffectChooser 0x31 | 0x40 | 0x80
	goto_cmd ATTACKING_MOVE

LOWERSTAT_IFOPPOSITEGENDER:
	attackcanceler
	jumpifsubstituteaffects MOVE_FAILED
	accuracycheck MOVE_MISSED 0x0
	callasm_cmd 49 @chcecks if opposite gender
	.word MOVE_FAILED
	goto_cmd ONE_STAT_TARGET_WORKED

ATTRACT:
	attackcanceler
	accuracycheck MOVE_FAILED 0xFFFF
	tryinfatuatetarget MOVE_FAILED
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printstring 0x45
	waitmessage 0x40
	goto_cmd ENDTURN

ATTACK_ONLYIF_TARGET_ATTACKS:
	attackcanceler
	callasm_cmd 36 @changes status flag in arg1 to correct effect chooser value
	callasm_cmd 48 @checks if target chose an attacking move
	.word MOVE_FAILED
	accuracycheck MOVE_MISSED 0x0
	attackstring
	ppreduce
	critcalc
	damagecalc
	goto_cmd SUCCESS_MOVE_ATTACK

ATTACK_SWITCH:
	attackcanceler
	callasm_cmd 36 @changes status flag in arg1 to correct effect chooser value
	accuracycheck MOVE_MISSED 0x0
	attackstring
	ppreduce
	critcalc
	damagecalc
	damageadjustment
	attackanimation
	waitanimation
	effectiveness_sound
	hitanim bank_target
	waitstate
	graphicalhpupdate bank_target
	datahpupdate bank_target
	critmessage
	waitmessage 0x40
	resultmessage
	waitmessage 0x40
	seteffectwithchance
	faintpokemon bank_target 0x0 0x0
	setbyte EndTurnTracker 0x0
	cmd49 0x0 0x0
MOVE_TRY_SWITCHING:
	jumpifonlyonepokemoninteam JUSTEND
	openpartyscreen bank_attacker JUSTEND
	swithchoutabilities bank_attacker
	waitstate
	switch_handle_order bank_attacker 0x2
	cmd58 0x1
	switch1 0x1
	switch2 0x1
	cmd73 0x1
	printstring 0x3
	switch3 0x1 0x1
	waitstate
	switchineffects bank_attacker
	end_cmd

PROTECT_LIKE_MOVE:
	attackcanceler
	attackstring
	ppreduce
	setprotect
	attackanimation
	waitanimation
	printfromtable protect_moves_strings
	waitmessage 0x40
	goto_cmd ENDTURN

ATTACK_PROTECTION_BREAK:
	attackcanceler
	accuracycheck MOVE_MISSED 0x0
	attackstring
	ppreduce
	callasm_cmd 47 @breaks protection
	call PROTECTION_BROKEN_MESSAGE
	critcalc
	damagecalc
	damageadjustment
	goto_cmd SUCCESS_MOVE_ATTACK
	
PROTECTION_BROKEN_MESSAGE:
	jumpifbyte Equals MultiStringChooser 0x2 JUSTRETURN
	pause_cmd 0x30
	printfromtable protection_broken_strings
	waitmessage 0x40
JUSTRETURN:
	return_cmd

ATTACK_HEAL_TARGET_STATUS:
	setbyte EffectChooser 10
	goto_cmd ATTACKING_MOVE

IDENTIFIER:
	attackcanceler
	accuracycheck MOVE_MISSED 0x0
	callasm_cmd 46 @sets identyfier bits
	.word MOVE_FAILED
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printstring 0x96
	waitmessage 0x40
	goto_cmd ENDTURN

GRAVITY:
	attackcanceler
	callasm_cmd 44 @sets gravity
	.word MOVE_FAILED
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printstring 0x1D0
	waitmessage 0x40
	callasm_cmd 45 @bring everyone down
	goto_cmd ENDTURN
	
ROOST:
	attackcanceler
	callasm_cmd 43 @prepare roost
	.word MOVE_FAILED
	attackstring
	ppreduce
	goto_cmd 0x82D8ED7 @hp healing script for attacker

TRANSFER_CONDITION:
	attackcanceler
	callasm_cmd 41 @jump if can't transfer condition
	.word MOVE_FAILED
	accuracycheck MOVE_MISSED 0x0
	attackstring
	ppreduce
	attackanimation
	waitanimation
	callasm_cmd 42 @choose correct text: poke became paralyzed, etc.
	setbyte EndTurnTracker 0x0
	cmd49 0x0 0x0
	cureifburnedparalysedorpoisoned JUSTEND
	statusiconeupdate bank_attacker
	printstring 0xA7
	waitmessage 0x40
JUSTEND:
	end_cmd
	
CHARGE:
	attackcanceler
	attackstring
	ppreduce
	setcharge
	attackanimation
	waitanimation
	printstring 0xA5
	waitmessage 0x40
	callasm_cmd 25 @sets stat to change based on arg1
	statbuffchange 0x41 0x82D8CC7 @won't grow any higher
	jumpifbyte 0x1 0x2024337 0x2 0x82D8CBF @worked
	goto_cmd 0x82D8CC7

HPHEAL_target:
	attackcanceler
	setdamageasrestorehalfmaxhp ALREADY_FULLHP bank_target
	accuracycheck MOVE_MISSED 0x0
	attackstring
	ppreduce
	weatherhpheal bank_target
	goto_cmd 0x82D9EE1 @hp healing script for target

HPHEAL_user:
	attackcanceler
	setdamageasrestorehalfmaxhp ALREADY_FULLHP bank_attacker
	attackstring
	ppreduce
	weatherhpheal bank_attacker
	goto_cmd 0x82D8ED7 @hp healing script for attacker
	
ALREADY_FULLHP:
	orbyte MoveOutcome OutcomeFailed
	attackstring
	ppreduce
	goto_cmd 0x82D9EFB @already has full hp

EXPLODE:
	attackcanceler
	attackstring
	ppreduce
	jumpifuserhasnoHP EXPLODE_DAMAGE
	faintifabilitynotdamp
	setuserhptozero
	waitstate
EXPLODE_DAMAGE:
	accuracycheck MOVE_MISSED 0x0
	critcalc
	damagecalc
	damageadjustment
	attackanimation
	waitanimation
	effectiveness_sound
	hitanim bank_target
	waitstate
	graphicalhpupdate bank_target
	datahpupdate bank_target
	critmessage
	waitmessage 0x40
	resultmessage
	waitmessage 0x40
	seteffectwithchance
	faintpokemon bank_target 0x0 0x0 @faint target
	setbyte EndTurnTracker 0x0
	cmd49 0x0 0x0
	faintpokemon bank_attacker 0x0 0x0
	end_cmd

FAINTSTATCHANGE:
	attackcanceler
	jumpifsubstituteaffects MOVE_FAILED
	attackstring
	ppreduce
	accuracycheck MOVE_MISSED 0x0
	setuserhptozero
	attackanimation
	waitanimation
	datahpupdate bank_attacker
	graphicalhpupdate bank_attacker
	doublestatchange
	faintpokemon bank_attacker 0x0 0x0
	goto_cmd ENDTURN

FAINT_HEAL:
	attackcanceler
	jumpifonlyonepokemoninteam MOVE_FAILED
	callasm_cmd 39 @sets lunar dance effect
	attackstring
	ppreduce
	attackanimation
	waitanimation
	graphicalhpupdate bank_attacker
	datahpupdate bank_attacker
	faintpokemon 0x1 0x0 0x0
	goto_cmd ENDTURN

CRASH_ATTACK:
	attackcanceler
	accuracycheck CRASHED 0x0
	critcalc
	damagecalc
	jumpiftypenotaffected CRASHED
	goto_cmd SUCCESS_MOVE_ATTACK
	
CRASHED:
	orbyte MoveOutcome OutcomeFailed
	attackstring
	ppreduce
	pause_cmd 0x20
	resultmessage
	waitmessage 0x40
	printstring 0x60
	waitmessage 0x40
	callasm_cmd 37 @half hp damage getter, takes one argument for bank
	.byte bank_attacker
	bicword MoveOutcome OutcomeFailed | OutcomeMissed | OutcomeNotaffected
	graphicalhpupdate bank_attacker
	datahpupdate bank_attacker
	faintpokemon bank_attacker 0x0 0x0
	orbyte MoveOutcome OutcomeMissed
	goto_cmd ENDTURN

ATTACK_FLINCH_CHANCE:
	attackcanceler
ATTACK_FLINCH_CHANCE_CANCELLER_DONE:
	accuracycheck MOVE_MISSED 0x0
	setbyte EffectChooser 8
	critcalc
	damagecalc
	damageadjustment
	attackstring
	ppreduce
	attackanimation
	waitanimation
	effectiveness_sound
	hitanim bank_target
	waitstate
	graphicalhpupdate bank_target
	datahpupdate bank_target
	critmessage
	waitmessage 0x40
	resultmessage
	waitmessage 0x40
	seteffectsecondary @calculation for both flinch and status effect are done in the function
	faintpokemon 0x0 0x0 0x0 @faint target
	goto_cmd ENDTURN

RECOIL_ATTACK:
	setbyte EffectChooser 0x30 | 0x80
	goto_cmd ATTACKING_MOVE

ATTACK_STATUS_CHANCE:
	callasm_cmd 36 @changes status flag in arg1 to correct effect chooser value
	goto_cmd ATTACKING_MOVE

FREEZE_TARGET:
	goto_cmd ENDTURN

BURN_TARGET:
	attackcanceler
	jumpifsubstituteaffects MOVE_FAILED
	attackstring
	ppreduce
	callasm_cmd 35 @jumpifcantburn
	accuracycheck MOVE_MISSED 0x0
	jumpifhalverset 0x0 0x20 0x82DAD01
	attackanimation
	waitanimation
	setbyte EffectChooser 3
	seteffectprimary
	goto_cmd ENDTURN

PARALYZE_TARGET:
	attackcanceler
	jumpifsubstituteaffects MOVE_FAILED
	attackstring
	ppreduce
	jumpiftypenotaffected MOVE_FAILED
	callasm_cmd 34 @similar to jumpifcannotsleep
	accuracycheck MOVE_MISSED 0x0
	jumpifhalverset 0x0 0x20 0x82DAD01
	attackanimation
	waitanimation
	setbyte EffectChooser 5
	seteffectprimary
	goto_cmd ENDTURN

BADLY_POISON_TARGET:
	setbyte EffectChooser 6
	goto_cmd TRY_POISONING
POISON_TARGET:
	setbyte EffectChooser 2
TRY_POISONING:
	attackcanceler
	jumpifsubstituteaffects MOVE_FAILED
	attackstring
	ppreduce
	callasm_cmd 33 @similar to jumpifcannot sleep
	accuracycheck MOVE_MISSED 0x0
	jumpifhalverset 0x0 0x20 0x82DAD01
	attackanimation
	waitanimation
	seteffectprimary
	goto_cmd ENDTURN

PUT_TARGET_TO_SLEEP:
	attackcanceler
	jumpifsubstituteaffects MOVE_FAILED
	attackstring
	ppreduce
	jumpifstatus 0x0 0x7 0x82D8AB0 @already asleep
	jumpifcannotsleep 0x82D8ACF @uproar, abiliy
	jumpifstatus 0x0 0xFF MOVE_FAILED
	accuracycheck MOVE_FAILED 0x0
	jumpifhalverset 0x0 0x20 0x82DAD01
	attackanimation
	waitanimation
	setbyte EffectChooser 0x1
	seteffectprimary
	goto_cmd ENDTURN

ATTACK_CONFUSION_CHANCE:
	setbyte EffectChooser 7
	goto_cmd ATTACKING_MOVE

CONFUSE_TARGET:
	attackcanceler
	jumpifsubstituteaffects MOVE_FAILED
	jumpifcantconfuseandchangestats MOVE_FAILED
	accuracycheck MOVE_MISSED 0x0
	attackstring
	ppreduce
	attackanimation
	waitanimation
	callasm_cmd 32 @this will try to change stats
	jumpifsecondarystatus bank_target 7 CANTCONFUSE
	jumpifhalverset 0x0 0x20 CANTCONFUSE
	jumpifability bank_target ABILITY_OWN_TEMPO CANT_CONFUSE_DUETOABILITY
	setbyte EffectChooser 7
	seteffectprimary
	goto_cmd ENDTURN
	
CANT_CONFUSE_DUETOABILITY:
	recordability bank_target
	printstring 0xCA
	waitmessage 0x40
	goto_cmd ENDTURN
	
CANTCONFUSE:
	printstring 0x44
	waitmessage 0x40
	goto_cmd ENDTURN

ATTACK_MULTIPLESTAT_CHANCE_USER:
	setbyte EffectChooser 0x2F
	orbyte EffectChooser 0x40
	goto_cmd ATTACKING_MOVE

MULTIPLE_STAT_CHANGE_TARGET:
	attackcanceler
	jumpifsubstituteaffects MOVE_FAILED
	jumpifcantchangetwostats MOVE_FAILED
	accuracycheck MOVE_MISSED 0x0
	attackstring
	ppreduce
	attackanimation
	waitanimation
	callasm_cmd 29 @do all stat changes that are possible
	goto_cmd ENDTURN

MULTIPLE_STAT_CHANGE_USER:
	attackcanceler
	jumpifcantchangetwostats MOVE_FAILED
	attackstring
	ppreduce
	attackanimation
	waitanimation
	callasm_cmd 29 @do all stat changes that are possible
	goto_cmd ENDTURN

ATTACK_TARGETSTAT_CHANCE:
	callasm_cmd 27
	goto_cmd ATTACKING_MOVE

ATTACK_USERSTAT_CHANCE:
	callasm_cmd 27 @converts arg1 stat to correct effect
	orbyte EffectChooser 0x40
	goto_cmd ATTACKING_MOVE

ONE_STAT_TARGET:
	attackcanceler
	jumpifsubstituteaffects MOVE_FAILED
	accuracycheck MOVE_MISSED 0x0
ONE_STAT_TARGET_WORKED:
	attackstring
	ppreduce
	callasm_cmd 25 @sets stat to change based on arg1
	statbuffchange 0x1 0x82D8D60
	jumpifbyte 0x3 0x2024337 0x2 0x82D8D4E
	jumpifbyte 0x0 0x2024337 0x3 0x82D8D60
	pause_cmd 0x20
	goto_cmd 0x82D8D58

ONE_STAT_USER:
	attackcanceler
ONE_STAT_USER_WORKED:
	attackstring
	ppreduce
	callasm_cmd 25 @sets stat to change based on arg1
	statbuffchange 0x41 0x82D8CC7 @won't grow any higher
	jumpifbyte 0x1 0x2024337 0x2 0x82D8CBD @worked
	pause_cmd 0x20
	goto_cmd 0x82D8CC7 @failed

FIXED_DAMAGE:
	attackcanceler
	accuracycheck MOVE_MISSED 0x0
	callasm_cmd 24 @damage calc2
	goto_cmd SUCCESS_MOVE_ATTACK

ATTACKING_MOVE:
	attackcanceler
	accuracycheck MOVE_MISSED 0x0
SUCCESS_MOVE_ATTACK_WITH_CALC:
	critcalc
	damagecalc
SUCCESS_MOVE_ATTACK:
	damageadjustment
	attackstring
	ppreduce
ATTACK_ANIM:
	attackanimation
	waitanimation
	effectiveness_sound
	hitanim bank_target
	waitstate
	graphicalhpupdate bank_target
	datahpupdate bank_target
	critmessage
	waitmessage 0x40
	resultmessage
	waitmessage 0x40
	seteffectwithchance
	faintpokemon bank_target 0x0 0x0 @faint target
	goto_cmd ENDTURN
