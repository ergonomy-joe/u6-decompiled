/*
 * Ultima VI - The False Prophet
 * Copyright (c) 1990 Origin Systems, Inc. and Richard Gariott
 * reverse-coded by Ergonomy Joe in 2013
 */
#ifndef __D_2C4A_H__
#define __D_2C4A_H__

/*__2C4A*/extern unsigned char IsOnQuest;
/*__2C4B*/extern unsigned char NextSleep;
/*__2C4C*/extern unsigned char Time_M;
/*__2C4D*/extern unsigned char Time_H;
/*__2C4E*/extern unsigned char Date_D;
/*__2C4F*/extern unsigned char Date_M;
/*__2C50*/extern unsigned Date_Y;
/*__2C52*/extern unsigned char KARMA;
/*__2C53*/extern char WindDir;
/*__2C54*/extern unsigned char Active;
/*__2C55*/extern char D_2C55;/*AmbiantLight?*/
/*__2C56*/extern int MapX;
/*__2C58*/extern int MapY;
/*__2C5A*/extern int MapZ;
/*__2C5C*/extern unsigned char SpellFx[16];
/*__2C6C*/extern int D_2C6C;/*"Vanish"ed object*/
/*__2C6E*/extern unsigned char D_2C6E;/*unused?*/
/*__2C6F*/extern unsigned char CaughtFish;
/*__2C70*/extern unsigned char DrunkCounter;
/*__2C71*/extern unsigned char MustRingBell;
/*__2C72*/extern int SoundFlag;
/*__2C74*/extern int D_2C74[8][3];/*moonstones x y z*/
/*__2CA4*/extern int D_2CA4;/*minutes to explosion?*/
/*__2CA6*/extern int D_2CA6;/*explosing object*/
/*__2CA8*/extern int D_2CA8;/*SpeakGarglish*/
/*__2CAA*/extern unsigned char PathObject[8];/*related to seg_1E0F/NPCTracker*/
/*__2CB2*/extern unsigned char PathCounter[8];/*related to seg_1E0F/NPCTracker*/
/*__2CBA*/extern unsigned char PathTries[8];/*related to seg_1E0F/NPCTracker*/
/*__2CC2*/extern char InCombat;
/*__2CC3*/extern char D_2CC3;
/*__2CC4*/extern char D_2CC4;
/*__2CC5*/extern unsigned char DefaultCommand;
/*__2CC6*/extern unsigned char D_2CC6;/*moon 1 phase/?*/
/*__2CC7*/extern unsigned char D_2CC7;/*moon 1 phase/?*/
/*__2CC8*/extern unsigned char D_2CC8;/*moon 2 phase/?*/
/*__2CC9*/extern unsigned char D_2CC9;/*moon 2 phase/?*/
/*__2CCA*/extern unsigned char D_2CCA;/*avatar's sex?*/
/*__2CCB*/extern unsigned char D_2CCB;
/*__2CCC*/extern int D_2CCC;/*end marker*/

#define obj_2C4A IsOnQuest

#endif
