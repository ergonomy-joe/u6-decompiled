/*
 * Ultima VI - The False Prophet
 * Copyright (c) 1990 Origin Systems, Inc. and Richard Gariott
 * reverse-coded by Ergonomy Joe in 2013
 */
#ifndef __SEG_3522_H__
#define __SEG_3522_H__

extern int far D_3522_0000[];

extern unsigned char far D_3522_0082[];
extern unsigned char far D_3522_00C2[];
extern unsigned char far D_3522_0102[];
extern unsigned char far D_3522_0142[];
extern unsigned char far D_3522_0182[];
extern unsigned char far D_3522_01C2[];

extern unsigned char far D_3522_0202[];

extern int far D_3522_0242[];

#define MONSTER_IS 0x0001
#define MONSTER_ID 0x0002
#define MONSTER_IP 0x0004
#define MONSTER_IF 0x0008
#define MONSTER_DF 0x0010
#define MONSTER_0020 0x0020
#define MONSTER_0040 0x0040
#define MONSTER_UN 0x0080
#define MONSTER_FL 0x0100
#define MONSTER_SW 0x0200
#define MONSTER_ET 0x0400
#define MONSTER_0800 0x0800
#define MONSTER_1000 0x1000
#define MONSTER_2000 0x2000
#define MONSTER_4000 0x4000
#define MONSTER_8000 0x8000

/*[IS]imune to sleep/zu*/
#define IsMonster_0001(cls) (D_3522_0242[cls] & MONSTER_IS)
/*[ID]imune to death spells/corp*/
#define IsMonster_0002(cls) (D_3522_0242[cls] & MONSTER_ID)
/*[IP]imune to poison/nox*/
#define IsMonster_0004(cls) (D_3522_0242[cls] & MONSTER_IP)
/*[IF]imune to fire/flam*/
#define IsMonster_0008(cls) (D_3522_0242[cls] & MONSTER_IF)
/*[DF]take double damage from fire*/
#define IsMonster_0010(cls) (D_3522_0242[cls] & MONSTER_DF)
/*[??]strength over dexterity (in combat)?*/
#define IsMonster_0020(cls) (D_3522_0242[cls] & MONSTER_0020)
/*[??]is poisonous*/
#define IsMonster_0040(cls) (D_3522_0242[cls] & MONSTER_0040)
/*[UN]undead nature*/
#define IsMonster_0080(cls) (D_3522_0242[cls] & MONSTER_UN)
/*[FL]flies; also immune to tremor spells*/
#define IsMonster_0100(cls) (D_3522_0242[cls] & MONSTER_FL)
/*[SW]swims*/
#define IsMonster_0200(cls) (D_3522_0242[cls] & MONSTER_SW)
/*[ET]ethereal (walk through walls)*/
#define IsMonster_0400(cls) (D_3522_0242[cls] & MONSTER_ET)
/*[??]no corpse?*/
#define IsMonster_0800(cls) (D_3522_0242[cls] & MONSTER_0800)
/*[??]no corpse[fade out]?*/
#define IsMonster_1000(cls) (D_3522_0242[cls] & MONSTER_1000)
/*[??]leaves blood?*/
#define IsMonster_2000(cls) (D_3522_0242[cls] & MONSTER_2000)
/*[??]human?*/
#define IsMonster_4000(cls) (D_3522_0242[cls] & MONSTER_4000)

/*[AC]acid (dissolves inventory item){Slug}*/
#define IsMonster_AC(cls) (cls == 0x00)
/*[AM]amphibious{Alligator}*/
#define IsMonster_AM(cls) (cls == 0x01)
/*[DR]drags victims underground{Corpser}*/
#define IsMonster_DR(cls) (cls == 0x08)
/*[IM]immortal{Lord British}*/
#define IsMonster_IM(cls) (cls == 0x1f)
/*[IN]changes to insects when dead*/
#define IsMonster_IN(cls) (cls == 0x15)
/*[SP]splits in two{Slime}*/
#define IsMonster_SP(cls) (cls == 0x30)
/*[SF]steals food{Gremlin}*/
#define IsMonster_SF(cls) (cls == 0x17)
/*[??]{winged gargoyle}*/
#define IsMonster_13(cls) (cls == 0x13)


extern int far D_3522_02C2[];

extern unsigned char far D_3522_0342[];


#endif
