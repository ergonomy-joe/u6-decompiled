/*
 * Ultima VI - The False Prophet
 * Copyright (c) 1990 Origin Systems, Inc. and Richard Gariott
 * reverse-coded by Ergonomy Joe in 2013
 */
/*#include "u6.h"*/

#include "seg_3522.h"

#include "obj.h"
#include "spells.h"

/*classes*/
int D_3522_0000[] = {
/*0x00*/	OBJ_16C,OBJ_1AD,OBJ_1AB,OBJ_19A,OBJ_158,OBJ_180,OBJ_164,OBJ_17D,
/*0x08*/	OBJ_165,OBJ_1AC,OBJ_1A8,OBJ_16F,OBJ_15E,OBJ_15D,OBJ_19B,OBJ_171,
/*0x10*/	OBJ_183,OBJ_181,OBJ_178,OBJ_16A,OBJ_16B,OBJ_163,OBJ_160,OBJ_161,
/*0x18*/	OBJ_17E,OBJ_172,OBJ_1AE,OBJ_1AF,OBJ_176,OBJ_157,OBJ_17F,OBJ_199,
/*0x20*/	OBJ_182,OBJ_17A,OBJ_17C,OBJ_062,OBJ_174,OBJ_162,OBJ_167,OBJ_156,
/*0x28*/	OBJ_15B,OBJ_168,OBJ_1AA,OBJ_15A,OBJ_15C,OBJ_19C,OBJ_19D,OBJ_170,
/*0x30*/	OBJ_177,OBJ_166,OBJ_169,OBJ_159,OBJ_179,OBJ_16E,OBJ_173,OBJ_17B,
/*0x38*/	OBJ_175,OBJ_15F,OBJ_16D,OBJ_19E,OBJ_19F,OBJ_1A7,OBJ_184,OBJ_107,
	0
};
/*STR/strength*/
unsigned char D_3522_0082[] = {
	  5, 20, 16, 25,  5,  8,  1,  8, 17, 14, 27, 35, 11,  5, 40, 22,
	 10, 15, 20, 15, 25,  2,  1,  2, 30, 19, 22, 22, 26,  1, 14, 30,
	 12, 10, 12, 22, 20,  2,  3,  5, 21,  3, 13, 17,  5, 30, 60, 10,
	  6,  3,  9, 24, 15, 20, 18, 11,  1,  8, 20, 20, 10, 10,  4, 11
};
/*DEX/dexterity*/
unsigned char D_3522_00C2[] = {
	  5, 10, 15, 25, 25,  8, 28,  8,  6,  7,  8, 26, 18, 16, 35, 22,
	 14, 15, 17, 32, 19, 10, 10, 26, 30, 13, 27, 27, 11, 40, 16, 30,
	 16, 14, 12,  9, 27, 25,  9, 13, 20, 17, 13, 17, 10, 30, 21, 14,
	  6,  7, 11, 20, 18, 25, 14, 12, 40, 16,  5, 20, 10, 10, 18, 12
};
/*INT/intelligence*/
unsigned char D_3522_0102[] = {
	  2,  3,  2, 25,  4,  9,  4,  9,  3,  5,  7, 31,  5,  8, 27, 13,
	 15, 10, 11, 33,  8, 21,  9,  7,  9,  8,  7,  7,  2,  1, 12, 30,
	 14, 22, 18,  8, 13,  3,  3,  4, 16,  2,  3,  9,  4, 30,  8,  7,
	  2,  6,  4,  8, 12,  1,  9, 12, 20,  8,  1, 20, 10, 10,  8, 12
};
/*ARM/armor*/
unsigned char D_3522_0142[] = {
	  0,  6,  3,  0,  0,  0,  0,  0,  0,  0,  4, 10,  0,  0, 12,  4,
	  0,  0,  0,  5, 10,  0,  0,  0,  0,  2,  0,  0,  0,  0,  0, 30,
	  0,  0,  0,  5,  4,  0,  0,  0,  4,  0,  3,  2,  0, 30, 15,  0,
	  0,  1,  0,  0,  0,  4,  4,  0,  0,  0,  4, 20, 10, 10,  0,  0
};
/*DMG/damage*/
unsigned char D_3522_0182[] = {
	  1, 10,  8,  4,  6,  3,  1,  2, 15,  3, 20, 20,  2,  3, 30, 10,
	  4,  4,  6,  6, 15,  1, 12,  1,  8, 12,  3,  3, 15,  1,  4,255,
	  4,  4,  4, 15, 20,  2,  2,  6, 20,  2,  8, 30,  2, 30, 60,  6,
	  4,  2,  8, 20,  4,  1,  6,  4,  1,  4,  1, 20, 10, 10,  2,  4
};
/*HP/hit points*/
unsigned char D_3522_01C2[] = {
	 10, 30, 10, 30,  5,  6,  3,  6, 40, 10, 40,100,  8,  6,150, 50,
	  8,  8, 20, 50, 30, 20, 20,  5,100, 20, 15, 15, 50,  1,  8,255,
	  8, 30,  8, 30, 30,  5,  5,  8, 30,  5, 20, 70,  6, 30,200, 20,
	 15,  5, 10, 50, 15, 30, 15,  8, 40,  8, 30, 20, 10, 10,  6,  8
};

/*TODO move from here*/
#define NEUTRAL 0
#define EVIL 0x20
#define GOOD 0x40
#define CHAOTIC 0x60

/*ALN/Alignement*/
unsigned char D_3522_0202[] = {
	CHAOTIC,CHAOTIC,CHAOTIC,   GOOD,CHAOTIC,CHAOTIC,NEUTRAL,CHAOTIC,
	CHAOTIC,NEUTRAL,   EVIL,   EVIL,NEUTRAL,NEUTRAL,   EVIL,   EVIL,
	NEUTRAL,NEUTRAL,CHAOTIC,   EVIL,   EVIL,   EVIL,   EVIL,CHAOTIC,
	   EVIL,   EVIL,NEUTRAL,NEUTRAL,CHAOTIC,CHAOTIC,CHAOTIC,   GOOD,
	CHAOTIC,   EVIL,   EVIL,CHAOTIC,   EVIL,NEUTRAL,NEUTRAL,CHAOTIC,
	CHAOTIC,CHAOTIC,CHAOTIC,   EVIL,NEUTRAL,CHAOTIC,CHAOTIC,   EVIL,
	CHAOTIC,CHAOTIC,CHAOTIC,   EVIL,CHAOTIC,CHAOTIC,   EVIL,CHAOTIC,
	CHAOTIC,CHAOTIC,CHAOTIC,CHAOTIC,CHAOTIC,CHAOTIC,NEUTRAL,CHAOTIC
};
/*SPECIAL?*/
int D_3522_0242[] = {
/*0x00*/
	MONSTER_1000|MONSTER_DF|MONSTER_IS,
	MONSTER_2000|MONSTER_0800|MONSTER_0020|MONSTER_IP,
	MONSTER_2000|MONSTER_0800|MONSTER_0020|MONSTER_IS,
	MONSTER_4000|MONSTER_2000|MONSTER_0020,
	MONSTER_2000|MONSTER_0800|MONSTER_FL,
	MONSTER_4000|MONSTER_2000|MONSTER_0020,
	MONSTER_2000|MONSTER_0800|MONSTER_FL,
	MONSTER_4000|MONSTER_2000|MONSTER_0020,
	MONSTER_0800|MONSTER_DF|MONSTER_IS,
	MONSTER_2000|MONSTER_0800|MONSTER_0020,
	MONSTER_2000|MONSTER_0020,
	MONSTER_2000|MONSTER_FL|MONSTER_UN|MONSTER_0020|MONSTER_IF|MONSTER_ID|MONSTER_IS,
	MONSTER_2000,
	MONSTER_2000,
	MONSTER_1000|MONSTER_FL|MONSTER_IF,
	MONSTER_2000|MONSTER_FL|MONSTER_IF,
/*0x10*/
	MONSTER_4000|MONSTER_2000|MONSTER_0020,
	MONSTER_4000|MONSTER_2000|MONSTER_0020,
	MONSTER_4000|MONSTER_2000|MONSTER_0020,
	MONSTER_2000|MONSTER_FL|MONSTER_0020|MONSTER_IS,
	MONSTER_2000|MONSTER_0020,
	MONSTER_1000|MONSTER_FL|MONSTER_0020|MONSTER_IS,
	MONSTER_1000|MONSTER_ET|MONSTER_UN|MONSTER_IF|MONSTER_IP|MONSTER_ID|MONSTER_IS,
	MONSTER_0800,
	MONSTER_4000|MONSTER_2000|MONSTER_0020,
	MONSTER_2000|MONSTER_0020,
	MONSTER_2000|MONSTER_0800|MONSTER_0020,
	MONSTER_2000|MONSTER_0020,
	MONSTER_1000|MONSTER_0020|MONSTER_IF|MONSTER_IS,
	MONSTER_0800|MONSTER_FL|MONSTER_IS,
	MONSTER_4000|MONSTER_2000|MONSTER_0020,
	MONSTER_4000|MONSTER_2000|MONSTER_0020,
/*0x20*/
	MONSTER_4000|MONSTER_2000|MONSTER_0020,
	MONSTER_4000|MONSTER_2000|MONSTER_0020,
	MONSTER_4000|MONSTER_2000|MONSTER_0020,
	MONSTER_0020|MONSTER_IP|MONSTER_IS,
	MONSTER_2000|MONSTER_FL,
	MONSTER_2000,
	MONSTER_2000|MONSTER_0800,
	MONSTER_2000|MONSTER_0800|MONSTER_0040|MONSTER_IP,
	MONSTER_0020|MONSTER_DF|MONSTER_IS,
	MONSTER_0800|MONSTER_0040|MONSTER_DF|MONSTER_IP|MONSTER_IS,
	MONSTER_2000|MONSTER_0800|MONSTER_0040|MONSTER_IS,
	MONSTER_0800|MONSTER_SW,
	MONSTER_2000,
	MONSTER_SW,
	MONSTER_1000|MONSTER_0040,
	MONSTER_UN|MONSTER_0020|MONSTER_IF|MONSTER_IP|MONSTER_ID|MONSTER_IS,
/*0x30*/
	MONSTER_1000|MONSTER_0040|MONSTER_DF|MONSTER_IS,
	MONSTER_2000|MONSTER_0040,
	MONSTER_2000|MONSTER_0040,
	MONSTER_0800|MONSTER_SW|MONSTER_0040|MONSTER_IS,
	MONSTER_4000|MONSTER_2000|MONSTER_0020,
	MONSTER_0020|MONSTER_DF|MONSTER_IS,
	MONSTER_2000|MONSTER_0020,
	MONSTER_4000|MONSTER_2000|MONSTER_0020,
	MONSTER_1000|MONSTER_ET|MONSTER_0020|MONSTER_IF|MONSTER_ID|MONSTER_IS,
	MONSTER_2000,
	MONSTER_0020|MONSTER_DF|MONSTER_IS,
	MONSTER_SW,
	MONSTER_SW,
	MONSTER_FL,
	MONSTER_2000,
	MONSTER_0800
};
/*spells/weapons/armors/treasures?*/
/*indexes in D_3522_0342*/
int D_3522_02C2[] = {
	0x000,0x000,0x000,0x000,0x000,0x004,0x000,0x000,
	0x000,0x00B,0x010,0x01B,0x023,0x000,0x028,0x034,
	0x03C,0x043,0x04B,0x057,0x065,0x070,0x077,0x000,
	0x07D,0x087,0x000,0x000,0x090,0x000,0x098,0x000,
	0x0A0,0x0AC,0x0BE,0x0C7,0x0CF,0x000,0x000,0x000,
	0x0DC,0x000,0x000,0x0E6,0x0ED,0x000,0x000,0x0F2,
	0x000,0x000,0x0FE,0x104,0x10A,0x000,0x116,0x126,
	0x12E,0x000,0x135,0x000,0x000,0x000,0x000,0x000
};
/*
spells 0
weapons 0
armors 0
treasures 0*/
unsigned char D_3522_0342[] = {
/*0x000*/
	0,
	0,
	0,
	0,
/*0x004 -- 0x05*/
	0,
	OBJ_026,OBJ_022,0,
	OBJ_0B9,0,
	0,
/*0x00b -- 0x09*/
	0,
	0,
	0,
	OBJ_081,0,
/*0x010 -- 0x0a*/
	0,
	OBJ_02C,OBJ_021,OBJ_022,0,
	OBJ_014,OBJ_003,OBJ_00D,0,
	OBJ_062,0,
/*0x01b -- 0x0b*/
	SPELL_11,SPELL_80,SPELL_22,SPELL_50,0,
	0,
	0,
	0,
/*0x023 -- 0x0c*/
	0,
	0,
	0,
	OBJ_081,0,
/*0x028 -- 0x0e*/
	SPELL_22,SPELL_53,SPELL_44,SPELL_60,SPELL_77,0,
	0,
	0,
	OBJ_062,OBJ_058,OBJ_058,0,
/*0x034 -- 0x0f*/
	SPELL_22,SPELL_44,0,
	0,
	0,
	OBJ_058,OBJ_058,0,
/*0x03c -- 0x10*/
	0,
	OBJ_026,0,
	OBJ_0B9,0,
	OBJ_058,0,
/*0x043 -- 0x11*/
	0,
	OBJ_069,OBJ_065,0,
	OBJ_011,0,
	OBJ_058,0,
/*0x04b -- 0x12*/
	0,
	OBJ_02B,OBJ_02A,0,
	OBJ_015,OBJ_00A,OBJ_003,OBJ_016,0,
	OBJ_058,OBJ_058,0,
/*0x057 -- 0x13*/
	SPELL_44,SPELL_41,SPELL_45,0,
	OBJ_02E,OBJ_029,OBJ_02F,OBJ_031,0,
	OBJ_012,OBJ_001,0,
	OBJ_03A,0,
/*0x065 -- 0x14*/
	0,
	OBJ_022,OBJ_031,OBJ_025,0,
	OBJ_001,OBJ_009,OBJ_012,OBJ_014,0,
	0,
/*0x070 -- 0x15*/
	SPELL_14,SPELL_44,SPELL_50,0,
	0,
	0,
	0,
/*0x077 -- 0x16*/
	SPELL_80,SPELL_63,0,
	0,
	0,
	0,
/*0x07d -- 0x18*/
	0,
	OBJ_02A,OBJ_02F,0,
	OBJ_016,OBJ_00A,OBJ_003,0,
	OBJ_058,0,
/*0x087 -- 0x19*/
	0,
	OBJ_022,OBJ_024,0,
	OBJ_011,OBJ_009,0,
	OBJ_058,0,
/*0x090 -- 0x1c*/
	SPELL_45,SPELL_60,SPELL_62,SPELL_44,0,
	0,
	0,
	0,
/*0x098 -- 0x1e*/
	0,
	OBJ_026,OBJ_021,0,
	OBJ_011,0,
	OBJ_058,0,
/*0x0a0 -- 0x20*/
	0,
	OBJ_029,OBJ_021,OBJ_02B,0,
	OBJ_013,OBJ_001,OBJ_009,0,
	OBJ_058,OBJ_09E,0,
/*0x0ac -- 0x21*/
	SPELL_11,SPELL_22,SPELL_14,SPELL_31,SPELL_44,SPELL_50,SPELL_45,SPELL_53,0,
	OBJ_026,0,
	OBJ_011,OBJ_001,0,
	OBJ_03A,OBJ_062,OBJ_058,0,
/*0x0be -- 0x22*/
	0,
	OBJ_026,OBJ_02B,0,
	OBJ_011,OBJ_001,0,
	OBJ_058,0,
/*0x0c7 -- 0x23*/
	SPELL_12,SPELL_50,0,
	0,
	0,
	OBJ_062,OBJ_062,0,
/*0x0cf -- 0x24*/
	0,
	OBJ_026,OBJ_02B,OBJ_024,OBJ_025,0,
	OBJ_009,OBJ_011,OBJ_012,0,
	OBJ_062,OBJ_058,0,
/*0x0dc -- 0x28*/
	SPELL_11,SPELL_50,SPELL_42,SPELL_44,0,
	0,
	0,
	OBJ_03A,OBJ_058,0,
/*0x0e6 -- 0x2b*/
	SPELL_22,SPELL_53,SPELL_33,0,
	0,
	0,
	0,
/*0x0ed -- 0x2c*/
	0,
	0,
	0,
	OBJ_081,0,
/*0x0f2 -- 0x2f*/
	0,
	OBJ_02B,OBJ_024,OBJ_025,OBJ_029,OBJ_026,0,
	OBJ_009,OBJ_001,0,
	OBJ_058,0,
/*0x0fe -- 0x32*/
	SPELL_59,0,
	0,
	0,
	OBJ_081,0,
/*0x104 -- 0x33*/
	SPELL_11,SPELL_44,0,
	0,
	0,
	0,
/*0x10a -- 0x34*/
	0,
	OBJ_02B,OBJ_029,OBJ_021,0,
	OBJ_013,OBJ_001,OBJ_009,0,
	OBJ_058,OBJ_058,0,
/*0x116 -- 0x36*/
	0,
	OBJ_025,OBJ_024,OBJ_02B,OBJ_029,OBJ_022,0,
	OBJ_009,OBJ_012,OBJ_001,OBJ_013,0,
	OBJ_062,OBJ_058,OBJ_058,0,
/*0x126 -- 0x37*/
	0,
	OBJ_02B,OBJ_021,0,
	OBJ_011,0,
	OBJ_058,0,
/*0x12e -- 0x38*/
	SPELL_14,SPELL_45,SPELL_81,0,
	0,
	0,
	0,
/*0x135 -- 0x3a*/
	SPELL_14,0,
	0,
	0,
	0
/*0x13a --*/
#if 1
/*3522:047C*/,
	0x00,0x00,0x00,0x00/*padding or data?*/
#endif
};
