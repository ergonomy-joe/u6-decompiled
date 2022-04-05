/*
 * Ultima VI - The False Prophet
 * Copyright (c) 1990 Origin Systems, Inc. and Richard Gariott
 * reverse-coded by Ergonomy Joe in 2013
 */
/*
	music module?
*/
#include <ctype.h>

#include "u6.h"

/*28BA*/unsigned char MUS_SongIndex = 0;
/*28BB*/unsigned char MUS_Song = 0;
/*28BC*/unsigned char MUS_Clock = 0;/*clocks ring counter*/
/*28BD*/unsigned char MUS_Bell = 0;/*bells ring counter*/
int D_28BE = 1;/*max sounds/channels?*/
/*28C0*/int MUS_MIndex = 0;
/*scale frequencies*/
int D_28C2[] = {0xC2C,0xDA9,0xF56,0x103F,0x1136,0x123C,0x1478,0x16FA,0x1857,0x19CA,0x1B53,0x1EAB};
/*melody I*/
unsigned char D_28DA[] = {
	2,6, 6, 0, 0,2,7, 7, 0,0,6,11,8,11,9,6,8,7,
	2,6, 0, 0, 0,2,7, 0, 0,0,6,11,8, 7,8,6,0,0,
	0,7,10,10,11,7,8,10,11,7,6, 8,7, 6,5,3,2,
0xFF};
/*melody II*/
unsigned char D_2910[] = {
	3,4, 6,0,12,0,0,0,9,9,11,9,8,9,7,0,
	2,3, 4,0,11,0,0,0,8,8, 9,8,7,8,6,0,
	3,4, 6,0,12,0,9,0,9,0,11,9,8,9,7,7,
	8,9,11,0, 7,0,8,0,6,0, 9,0,0,0,0,0,
0xFF};
unsigned char *D_2951[2] = {D_28DA, D_2910};


static MUS_0008(int bp0c, int bp0a, int bp08, int bp06) {
	int si, di;
	int bp_04, bp_02;

	if(bp06 > 0) {
		bp_02 = bp08;
		si = (bp0a - bp0c) * bp_02;
		si /= bp06;
		bp_04 = bp0c;
		for(di = 0; di < bp06; bp_04 += si, di += bp08) {
			OSI_sound(bp_04);
			OSI_sDelay(1, bp08);
		}
	}
	OSI_nosound();
}

/*play a speaker sound?*/
MUS_0065(int bp08, int di/*bp06*/) {
	int si;
	int bp_02;

	if(!SoundFlag)
		return;
	switch(bp08) {
		case 0: OSI_nosound(); break;
		case 1: OSI_playNote(di, 10); break;
		case 4: OSI_playNoise(1, 800, 10000); break;
		case 5:
			di = 2000 - (di << 4);
			for(si = 0; si < 40; si++) {
				if(OSI_rand(0, 3) == 0) {
					bp_02 = OSI_rand(0, 200) + di - 100;
					if(bp_02 > 19)
						OSI_sound(bp_02);
				} else {
					OSI_nosound();
				}
				OSI_sDelay(8, 1);
			}
			OSI_nosound();
		break;
		case 16:
			di = 1000 - (di << 3);
			for(si = 0; si < 40; si++) {
				if(OSI_rand(0, 1) && di > 20) {
					OSI_sound(OSI_rand(19, OSI_rand(20, di)));
				} else {
					OSI_nosound();
				}
				OSI_sDelay(8, 1);
			}
			OSI_nosound();
		break;
		case 6:
			di = 1000 - (di << 3);
			for(si = 0; si < 40; si++) {
				if(OSI_rand(0, 7) == 0) {
					bp_02 = OSI_rand(0, di << 1) + di;
					if(bp_02 > 19)
						OSI_sound(bp_02);
				} else {
					OSI_nosound();
				}
				OSI_sDelay(8, 1);
			}
			OSI_nosound();
		break;
		case 7:
			if(di < 0) {
				OSI_playNoise(1, 800, 3000);
				return;
			}
			for(si = 0; si < 40; si++) {
				if(di > 19) {
					if(OSI_rand(0, 7) == 0) {
						OSI_sound(OSI_rand(0, 40) + di);
					} else {
						OSI_nosound();
					}
				}
				OSI_sDelay(8, 1);
			}
			OSI_nosound();
		break;
		case 8:
			for(si = 0; si < 40; si++) {
				OSI_sound(OSI_rand(19, 200));
				OSI_sDelay(8, 1);
			}
			OSI_nosound();
		break;
		case 9: MUS_0008(1200, 2000, 1, 40); break;
		case 10: OSI_playNoise(1, 7000, 600); break;
		case 11:
			for(si = 0; si < 20; si++)
				OSI_playNoise(1, 350, OSI_rand(600, 4800));
		break;
		case 12:
		case 13: MUS_0008(800, 2000, 1, 50); break;
		case 14: OSI_playNote(2000, 10); break;
		case 15:
			for(si = 2000; si < 20000; si += 1000)
				OSI_playNoise(40, 120, si);
		break;
		case 17: MUS_0008(400, 750, 5, 150); break;
		case 20:
			OSI_playNote(di, 6);
			OSI_playWavedNote(di, 1, 700, 30000, -25);
		break;
		case 2: OSI_playWavedNote(di >> 1, 1, 6000, 30000, -5); break;
		case 3: OSI_playWavedNote(di, 1, 4000, 15000, -3); break;
		case 18: OSI_playWavedNote(di >> 1, 1, 2000, 20000, -20); break;
		case 19: OSI_playNote(di >> 2, 200); break;
		case 21:
			for(si = 0; si < 50; si++)
				OSI_playWavedNote((si << 4) + 800, 1, 1000, 10000 - (si << 6), (50 - si) << 2);
			OSI_playWavedNote(1600, 1, 8000, 0, 8);
		break;
		case 22: MUS_0008(5000, 8000, 1, 100); break;
		default: OSI_playNote(di, 100);
	}
}

/*push sound?*/
MUS_043B(int typeObject, int distance) {
	int di;
	int bp_02;

	di = 0;
	if(
		typeObject != OBJ_09F &&
		typeObject != OBJ_0EA &&
		typeObject != OBJ_0EC &&
		typeObject != OBJ_0A4 &&
		typeObject != OBJ_0A5 &&
		typeObject != OBJ_0C9 &&
		typeObject != OBJ_11F &&
		typeObject != OBJ_121 &&
		typeObject != OBJ_122 &&
		typeObject != OBJ_130 &&
		typeObject != OBJ_13D &&
		typeObject != OBJ_13F &&
		typeObject != OBJ_133 &&
		typeObject != OBJ_188
	) {
		return;
	}
	for(; D_ECAC[di] <= distance && di < D_ECAA; di ++);
	if(di >= 4)
		return;
	for(bp_02 = D_ECAA; bp_02 > di; bp_02 --) {
		if(bp_02 < 4) {
			D_ECA2[bp_02] = D_ECA2[bp_02 - 1];
			D_ECAC[bp_02] = D_ECAC[bp_02 - 1];
		}
	}
	if(D_ECAA < 4)
		D_ECAA ++;
	D_ECA2[di] = typeObject;
	D_ECAC[di] = distance;
}

MUS_051E() {
	D_ECAA = 0;
}

/*sound sequencer?*/
MUS_0525() {
	int numSounds, di;
	int i, bp_02;

	numSounds = 0;
	if(SpellFx[14] == 0) {
		for(i = numSounds; i < D_ECAA; i ++) {
			di = D_ECAC[i];
			switch(D_ECA2[i]) {
				case OBJ_09F:/*clock*/
					if(MUS_Clock && (D_0338 & 3) == 0) {
						OSI_playWavedNote(0xc2c, 1, 2000, 20000, -10);
						numSounds ++;
					} else if((D_0338 & 0xf) == 0) {/*tic*/
						OSI_playNote(3000, 3 - (di >> 2));
						numSounds ++;
					} else if((D_0338 & 0xf) == 8) {/*tac*/
						OSI_playNote(2000, 3 - (di >> 2));
						numSounds ++;
					}
				break;
				case OBJ_0EC:/*bell*/
					if(MUS_Bell && (D_0338 & 0xf) == 0) {
						OSI_playWavedNote(0xc2c, 1, 2000, 20000, -10);
						numSounds ++;
					}
				break;
				/*Fireplace, Fire (from forge), Cook Fire, Fire Field*/
				case OBJ_0A4: case OBJ_0C9: case OBJ_130: case OBJ_13D:
					if(OSI_rand(0, 3) == 0) {
						numSounds ++;
						for(bp_02 = 0; bp_02 < 5; bp_02 ++) {
							if(OSI_rand(0, di + 7) == 0)
								OSI_sound(OSI_rand(2000, 15000));
							else
								OSI_nosound();
							OSI_sDelay(8, 1);
						}
						OSI_nosound();
					}
				break;
				case OBJ_13F:/*protection field*/
					if(OSI_rand(0, 1) == 0) {
						numSounds ++;
						for(bp_02 = 0; bp_02 < 80; bp_02 ++) {
							if(OSI_rand(0, 15) == 0)
								OSI_sound(OSI_rand(200, 1500 - (di << 8)));
							else
								OSI_nosound();
							OSI_sDelay(8, 1);
						}
						OSI_nosound();
					}
				break;
				case OBJ_0EA:/*fountain*/
					OSI_playNoise(10, 30 - (di << 2), 25000 - (di << 11));
					numSounds ++;
				break;
				case OBJ_11F:/*water wheel*/
					OSI_playNoise(20, 60 - (di << 2), 10000 - (di << 10));
					numSounds ++;
				break;
				case OBJ_188:/*musician (short cape)*/
					if(D_2951[MUS_Song][MUS_SongIndex] && D_01CA == 0) {
						MUS_0065(18, D_28C2[D_2951[MUS_Song][MUS_SongIndex] - 1]);
						numSounds ++;
					}
					if(D_2951[MUS_Song][++MUS_SongIndex] == 0xff)
						MUS_SongIndex = 0;
				break;
			}
			if(numSounds >= D_28BE)
				break;
		}
	}
	if(MUS_Clock && (D_0338 & 3) == 0)
		MUS_Clock --;
	if(MUS_Bell && (D_0338 & 0xf) == 0) {
		if(--MUS_Bell == 0)
			SetTileAnimation(BaseTile[OBJ_0EC], 0);
	}
	OSI_delay(1);
}

static MUS_07F6() {
	int si, di;

	while(1) {
		si = TalkBuf[D_ECA0];
		if(si == ')')
			return -1;
		if(isdigit(si))
			break;
		D_ECA0 ++;
	}
	di = 0;
	while(1) {
		si = TalkBuf[D_ECA0];
		if(!isdigit(si))
			break;
		di = di * 10;
		di += si - '0';
		D_ECA0 ++;
	}
	return di;
}

static MUS_0851(int bp06) {
	int si, di;
	int bp_08, bp_06, bp_04, bp_02;

	DISK_confirm(DISK_2);
	si = OSI_open(/*2979*/"midi.dat");
	if(si >= 0) {/*parse midi.dat*/
		OSI_read(si, 0, 0x400, TalkBuf);
		bp_08 = 0;
		for(D_ECA0 = 0; D_ECA0 < 0x400; D_ECA0 ++) {
			if(bp_08 > bp06) {
				for(bp_06 = 0; (bp_04 = MUS_07F6()) >= 0; bp_06 ++) {
					bp_02 = MUS_07F6();
					D_ECB4.iii.ofs = 0xc; (*D_ECB4.f)(bp_06, bp_04, bp_02);
				}
				break;
			}
			if(TalkBuf[D_ECA0] == '(')
				bp_08 ++;
		}
		OSI_close(si);
	} else {/*no midi.dat*/
		for(di = 0; di < 16; di++) {
			D_ECB4.iii.ofs = 0xc; (*D_ECB4.f)(di, 2, 0);
		}
	}
}

char *D_2955[] = {
	/*2982*/"ultima.m",
	/*298B*/"bootup.m",
	/*2994*/"intro.m",
	/*299C*/"create.m",
	/*29A5*/"forest.m",
	/*29AE*/"hornpipe.m",
	/*29B9*/"engage.m",
	/*29C2*/"stones.m",
	/*28CB*/"dungeon.m",
	/*29D5*/"brit.m",
	/*29DC*/"gargoyle.m",
	/*29E7*/"end.m"
};

/*play a song?*/
MUS_091A(int si/*bp06*/) {
	if(D_01CA == 0)
		return;
	if(si == MUS_MIndex)
		return;
	if(MUS_MIndex != 0) {
		D_ECB4.iii.ofs = 9; (*D_ECB4.f)();
		MUS_MIndex = 0;
	}
	if(si == 0)
		return;
	if(!SoundFlag)
		return;
	DISK_confirm(DISK_2);
	if(D_01CA == 4)
		MUS_0851(si - 1);
	IsZFile = 1;
	LoadFile(D_2955[si - 1], TalkBuf);
	IsZFile = 0;
	D_ECB4.iii.ofs = 6; (*D_ECB4.f)(TalkBuf);
	MUS_MIndex = si;
}

/*play an "adequat" song*/
MUS_09A8() {
	int si, di;

	if(!SoundFlag)
		return;
	if(MapZ == 5) {
		si = 11;/*"gargoyle.m"*/
	} else if(MapZ == 0) {
		switch(OSI_rand(0, 3)) {
			case 0: si = 8; /*"stones.m"*/break;
			case 1: si = 1; /*"ultima.m"*/break;
			case 2: si = 5; /*"forest.m"*/break;
			case 3: si = 10;/*"brit.m"*/break;
		}
	} else {
		si = 9;/*"dungeon.m"*/
	}
	di = GetType(Party[Active]);
	if(di == OBJ_19C || di == OBJ_19E)
		si = 6;/*"hornpipe.m"*/
	if(InCombat)
		si = 7;/*"engage.m"*/
	MUS_091A(si);
}

char *D_296D[] = {
	/*29ED*/"U6ADLIB.DRV",
	/*29F9*/"U6CMS.DRV",
	/*2A03*/"U6TMUS.DRV",
	/*2A0E*/"U6ROLAND.DRV",
	/*2A1B*/"U6COVOX.DRV",
	/*2A27*/"U6INNOVA.DRV"
};

/*init music?*/
MUS_0A3A(int bp08, int bp06) {
	if(bp08 == 0)
		return;
	/*-- load music driver*/
	IsZFile = 1;
	D_ECB4.p = LoadFile(D_296D[bp08 - 1], 0L);
	IsZFile = 0;
	D_ECB4.iii.ofs = 0; (*D_ECB4.f)(bp06);
}
