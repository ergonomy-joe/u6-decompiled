/*
 * Ultima VI - The False Prophet
 * Copyright (c) 1990 Origin Systems, Inc. and Richard Gariott
 * reverse-coded by Ergonomy Joe in 2013
 */
#include "D_2C4A.h"

/*2C4A*/unsigned char IsOnQuest = 0;
/*2C4B*/unsigned char NextSleep = 0;
/*2C4C*/unsigned char Time_M = 0;/*minutes*/
/*2C4D*/unsigned char Time_H = 8;/*hour of day*/
/*2C4E*/unsigned char Date_D = 4;/*day of month*/
/*2C4F*/unsigned char Date_M = 7;/*month*/
/*2C50*/unsigned Date_Y = 161;/*year*/
/*2C52*/unsigned char KARMA = 75;
/*2C53*/char WindDir = 0;
/*2C54*/unsigned char Active = 0;
/*2C55*/char D_2C55 = 7;
/*2C56*/int MapX = 0x133;
/*2C58*/int MapY = 0x160;
/*2C5A*/int MapZ = 0;
/*2C5C*/unsigned char SpellFx[16] = {0};
/*2C6C*/int D_2C6C = 0;
/*2C6E*/unsigned char D_2C6E = 0;
/*2C6F*/unsigned char CaughtFish = 0;
/*2C70*/unsigned char DrunkCounter = 0;
/*2C71*/unsigned char MustRingBell = 0;
/*2C72*/int SoundFlag = 1;
/*2C74*/int D_2C74[8][3] = {/*moonstones x, y, z*/
	0x3A7,0x106,0,
	0x1F7,0x166,0,
	0x09F,0x3AE,0,
	0x127,0x026,0,
	0x33F,0x0A6,0,
	0x147,0x336,0,
	0x017,0x016,1,
	0x397,0x3A6,0
};
/*2CA4*/int D_2CA4 = 0;
/*2CA6*/int D_2CA6 = 0;
/*2CA8*/int D_2CA8 = 0;/*SpeakGarglish*/
/*2CAA*/unsigned char PathObject[8] = {0};
/*2CB2*/unsigned char PathCounter[8] = {0};
/*2CBA*/unsigned char PathTries[8] = {0};
/*2CC2*/char InCombat = 1;
/*2CC3*/char D_2CC3 = -1;
/*2CC4*/char D_2CC4 = 0;
/*2CC5*/unsigned char DefaultCommand = 0xff;
/*2CC6*/unsigned char D_2CC6 = 0;/*moon 1 phase/?*/
/*2CC7*/unsigned char D_2CC7 = 0;/*moon 1 phase/?*/
/*2CC8*/unsigned char D_2CC8 = 0;/*moon 2 phase/?*/
/*2CC9*/unsigned char D_2CC9 = 0;/*moon 2 phase/?*/
/*2CCA*/unsigned char D_2CCA = 0;/*avatar's sex?*/
/*2CCB*/unsigned char D_2CCB = 0;
/*2CCC*/int D_2CCC = 0;/*end marker*/
