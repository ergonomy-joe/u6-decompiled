/*
 * Ultima VI - The False Prophet
 * Copyright (c) 1990 Origin Systems, Inc. and Richard Gariott
 * reverse-coded by Ergonomy Joe in 2013
 */
/*
	Module GraphicFX, missile, peer, ...
*/
#include "u6.h"

#undef abs
#include <math.h>

char *D_2A34[] = {
	/*2C1F*/"U6MCGA.DRV",
	/*2C2A*/"U6EGA.DRV",
	/*2C34*/"U6TANDY.DRV",
	/*2C40*/"U6CGA.DRV",
	/*2C40*/"U6CGA.DRV"
};
int D_2A3E[] = {
	0x13,
	0xD,
	0x9,
	0xB800,
	0xB000
};
/*2A48*/int ScreenFade = 1;
/*colors ?*/
int D_2A4A = 12;
int D_2A4C = 10;
int D_2A4E = 9;
int D_2A50 = 13;
int __2A52 = 14;
int D_2A54 = 0x31;
int D_2A56 = 0x48;
int D_2A58 = 0;
int D_2A5A = 7;
/*====---- ----====*/

/*init gr?*/
C_2FC1_000A(si/*bp06*/) {
	/*-- load graphic driver*/
	IsZFile = 1;
	D_ECB8.p = LoadFile(D_2A34[si], 0L);
	IsZFile = 0;
	D_ECC4 = GR_00(D_2A3E[si] + D_01CE);
	D_ECC4->_10 = DISK_confirm;
	D_ECC4->_24 = LoadFileGr;
	D_ECC4->_28 = OSI_open;
	D_ECC4->_2c = OSI_read;
	D_ECC4->_30 = OSI_close;
	if(si) {
		D_2A54 = 0xf;
		D_2A56 = 6;
	}
}

/*C_2FC1_00B0*/MagicalHalo(int objNum, int bp06) {
	int delta_x,delta_y,bp_02;

	if(GetCoordUse(objNum) != LOCXYZ)
		return;
	delta_x = (GetX(objNum) - MapX + 5) & 0x3ff;
	delta_y = (GetY(objNum) - MapY + 5) & 0x3ff;
	bp_02 = TILE_FRAME(objNum) + (bp06?(6 << 10):(4 << 10));
	if(delta_x < 11 && delta_y < 11) {
		ShowObject(bp_02, delta_x, delta_y, 2);
		ScreenFade = 1;
		C_0A33_09CE(1);
		ShowObjects();
		ScreenFade = 1;
		C_0A33_09CE(1);
	}
}

/*redefineClipping*/
static C_2FC1_017F(int x, int y) {
	if(x < 16)
		D_ECBC[0] = 8;
	else if(x - 8 < D_ECBC[0])
		D_ECBC[0] = x - 8;

	if(y < 16)
		D_ECBC[2] = 8;
	else if(y - 8 < D_ECBC[2])
		D_ECBC[2] = y - 8;

	if(x > 159)
		D_ECBC[1] = 167;
	else if(x + 8 > D_ECBC[1])
		D_ECBC[1] = x + 8;

	if(y > 159)
		D_ECBC[3] = 167;
	else if(y + 8 > D_ECBC[3])
		D_ECBC[3] = y + 8;
}

/*redraw frame if dirty*/
C_2FC1_0212(int x0, int y0, int x1, int y1) {
	int si;

	if(x0 < 16 && y0 < 16)
		GR_42(TIL_1B0, 0, 0);
	if(x1 > 159 && y0 < 16)
		GR_42(TIL_1B2, 160, 0);
	if(x0 < 16 && y1 > 159)
		GR_42(TIL_1B3, 0, 160);
	if(x1 > 159 && y1 > 159)
		GR_42(TIL_1B5, 160, 160);
	for(si = 1; si < 10; si ++) {
		if(y0 < 16)
			GR_42(TIL_1B1, TIL2SCR(si), 0);
		if(y1 > 159)
			GR_42(TIL_1B4, TIL2SCR(si), 160);
		if(x0 < 16)
			GR_42(TIL_1B6, 0, TIL2SCR(si));
		if(x1 > 159)
			GR_42(TIL_1B7, 160, TIL2SCR(si));
	}
}

static C_2FC1_032B(int x0, int y0, int x1, int y1) {
	int si, di;
	int bp_04, bp_02;

	for(si = SCR2TIL(x0); si <= SCR2TIL(x1); si ++) {
		for(di = SCR2TIL(y0); di <= SCR2TIL(y1); di ++) {
			for(bp_04 = Obj_11x11[di][si]; bp_04; bp_04 = D_E5E0[bp_04]) {
				bp_02 = D_D5DC[bp_04];
				if(bp_02 && IsTileFor(bp_02 & 0x7ff))
					GR_42(bp_02 & 0x7ff, TIL2SCR(si), TIL2SCR(di));
			}
			if(Tile_11x11[di][si] == TIL_0FF || Tile_11x11[di][si] == TIL_1BC)
				GR_42(TIL_0FF, TIL2SCR(si), TIL2SCR(di));
		}
	}
	C_2FC1_0212(x0, y0, x1, y1);
}

/*distance.rotation angles map*/
unsigned char D_2A5C[][16] = {
	{0x30,0x2E,0x2D,0x2B,0x28,0x26,0x23,0x20,0x1D,0x1A,0x18,0x15,0x13,0x12,0x10,0x0F},
	{0x32,0x30,0x2E,0x2C,0x29,0x26,0x23,0x20,0x1D,0x1A,0x17,0x14,0x12,0x10,0x0E,0x0D},
	{0x34,0x32,0x30,0x2E,0x2B,0x27,0x24,0x20,0x1C,0x19,0x15,0x12,0x10,0x0E,0x0D,0x0B},
	{0x35,0x34,0x32,0x30,0x2D,0x29,0x25,0x20,0x1B,0x17,0x13,0x10,0x0E,0x0C,0x0B,0x09},
	{0x38,0x37,0x35,0x33,0x30,0x2C,0x26,0x20,0x1A,0x14,0x10,0x0D,0x0B,0x09,0x08,0x07},
	{0x3A,0x3A,0x39,0x37,0x34,0x30,0x29,0x20,0x17,0x10,0x0C,0x09,0x07,0x06,0x06,0x05},
	{0x3D,0x3D,0x3C,0x3B,0x3A,0x37,0x30,0x20,0x10,0x09,0x06,0x05,0x04,0x03,0x03,0x03},
	{0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x43,0x43,0x44,0x45,0x46,0x49,0x50,0x60,0x70,0x77,0x7A,0x7B,0x7C,0x7D,0x7D,0x7D},
	{0x46,0x46,0x47,0x49,0x4C,0x50,0x57,0x60,0x69,0x70,0x74,0x77,0x79,0x7A,0x7A,0x7B},
	{0x48,0x49,0x4B,0x4D,0x50,0x54,0x5A,0x60,0x66,0x6C,0x70,0x73,0x75,0x77,0x78,0x79},
	{0x4B,0x4C,0x4E,0x50,0x53,0x57,0x5B,0x60,0x65,0x69,0x6D,0x70,0x72,0x74,0x75,0x77},
	{0x4D,0x4E,0x50,0x52,0x55,0x59,0x5C,0x60,0x64,0x67,0x6B,0x6E,0x70,0x72,0x73,0x75},
	{0x4E,0x50,0x52,0x54,0x57,0x5A,0x5D,0x60,0x63,0x66,0x69,0x6C,0x6E,0x70,0x72,0x73},
	{0x50,0x52,0x53,0x55,0x58,0x5A,0x5D,0x60,0x63,0x66,0x68,0x6B,0x6D,0x6E,0x70,0x71},
	{0x51,0x53,0x55,0x57,0x59,0x5B,0x5D,0x60,0x63,0x65,0x67,0x69,0x6B,0x6D,0x6F,0x70}
};
/*missile tiles*/
unsigned D_2B5C[] = {
	0,
	TIL_223,TIL_224,TIL_225,TIL_230,TIL_236,
	TIL_237,TIL_188,TIL_189,TIL_235,TIL_18A,
	TIL_18B,TIL_18C,TIL_18D,TIL_17C,TIL_17D,
	TIL_17E,TIL_17F
};
/*angle phase?*/
unsigned char D_2B80[] = {0x40,0x10,0x00,0x10,0x00,0x00,0x00,0x20,0x20,0x00,0x20,0x20,0x20,0x20,0x00,0x00,0x00,0x00};
/*rotation increment?*/
unsigned char D_2B92[] = {0x00,0x00,0x0A,0x0A,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x0A,0x0A,0x0A};
/*start x?*/
unsigned char D_2BA4[] = {0x7F,0x75,0x7F,0x86,0x7A,0x71,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F};
/*start y?*/
unsigned char D_2BB6[] = {0x7F,0x7F,0x7F,0x8B,0x7F,0x46,0x3D,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F};
/*refresh rate?*/
unsigned char D_2BC8[] = {0x06,0x03,0x02,0x02,0x04,0x04,0x04,0x02,0x02,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02};

#define MAX_MISSILES 16

/*C_2FC1_042D*/Missiles(
	int src_x, int src_y,
	int num_dst, int *pX_dst, int *pY_dst,
	int max_range, int projTile,
	int attr/*1=reverse,2=has trail?,4=different sound?,8=no rotation?*/
) {
	int i;
	int dst;
	int bp_25c,refreshRate,temp,misType,delta_max;
	int tile_delta_x,tile_delta_y;
	int ret,whichDir;
	int spr_new_x,spr_new_y;
	int mustDisplay,numTile;
	int bp_242;
	int cur_x[MAX_MISSILES],cur_y[MAX_MISSILES];
	int delta_x[MAX_MISSILES],delta_y[MAX_MISSILES];
	int incr_x[MAX_MISSILES][2],incr_y[MAX_MISSILES][2];
	int b_error[MAX_MISSILES],berr_incr[MAX_MISSILES][2];

	int tile_x[MAX_MISSILES],tile_y[MAX_MISSILES];
	int spr_rot[MAX_MISSILES];
	int collision_mask[0x100/8];
	int spr_x[MAX_MISSILES],spr_y[MAX_MISSILES];

#if 0
	CON_printf("Missiles %03x,%03x\n", src_x, src_y);
#endif
	bp_242 = 0;/*unused?*/
	C_0A33_09CE(0);

	for(i = 0; i < 0x100/8; i ++)
		collision_mask[i] = 0;

	HideMouse
	ret = 1;

	D_EBFE[0] =
	tile_x[0] = src_x;
	D_EBFE[1] =
	tile_y[0] = src_y;

	if(attr & 1) {
		temp = tile_x[0]; tile_x[0] = pX_dst[0]; pX_dst[0] = temp;
		temp = tile_y[0]; tile_y[0] = pY_dst[0]; pY_dst[0] = temp;
	}

	cur_x[0] = TIL2SCR(tile_x[0]) + 8;
	cur_y[0] = TIL2SCR(tile_y[0]) + 8;

	misType = 0;
	for(i = 0; i < 18; i ++) {
		if(D_2B5C[i] == projTile) {
			misType = i;
			break;
		}
	}

	if(attr & 2)
		refreshRate = 6;
	else
		refreshRate = D_2BC8[misType];

	if(projTile) {
		if(attr & 8) {
			GR_33(15, projTile, 0, 0x7f, 0x7f);
		} else {
			for(dst = 0; dst < num_dst; dst ++) {
				spr_rot[dst] =
					D_2A5C[pY_dst[dst] - tile_y[0] + 7][pX_dst[dst] - tile_x[0] + 7] - D_2B80[misType];
				if(attr & 2)
					GR_33(dst, projTile, spr_rot[dst], D_2BA4[misType], D_2BB6[misType]);
				else
					GR_33(dst + 8, projTile, spr_rot[dst], D_2BA4[misType], D_2BB6[misType]);
			}
		}
	}

	delta_max = 0;
	for(dst = 0; dst < num_dst; dst ++) {
		cur_x[dst] = cur_x[0];
		cur_y[dst] = cur_y[0];
		tile_x[dst] = tile_x[0];
		tile_y[dst] = tile_y[0];
		delta_x[dst] = TIL2SCR(pX_dst[dst] - tile_x[0]);
		delta_y[dst] = TIL2SCR(pY_dst[dst] - tile_y[0]);
		incr_y[dst][0] = 0;
		if(delta_x[dst] < 0) {
			delta_x[dst] = -delta_x[dst];
			incr_x[dst][0] =
			incr_x[dst][1] = -1;
		} else {
			incr_x[dst][0] =
			incr_x[dst][1] = 1;
		}
		if(delta_y[dst] < 0) {
			delta_y[dst] = -delta_y[dst];

			incr_y[dst][1] = -1;
		} else {

			incr_y[dst][1] = 1;
		}
		if(delta_x[dst] < delta_y[dst]) {
			temp = delta_x[dst];
			delta_x[dst] = delta_y[dst];
			delta_y[dst] = temp;
			incr_y[dst][0] = incr_y[dst][1];
			incr_x[dst][1] = incr_x[dst][0];
			incr_x[dst][0] = 0;
		}
		if(delta_x[dst] > delta_max)
			delta_max = delta_x[dst];
		b_error[dst] = delta_y[dst] + delta_y[dst] - delta_x[dst];
		berr_incr[dst][0] = delta_y[dst] + delta_y[dst];
		berr_incr[dst][1] = (delta_y[dst] - delta_x[dst]) << 1;
	}

	for(i = 0; i < delta_max; i ++) {
		mustDisplay = 0;
		D_ECBC[0] = D_ECBC[2] = 168;
		D_ECBC[1] = D_ECBC[3] = 7;
		for(dst = 0; dst < num_dst; dst ++) {
			if(delta_x[dst] <= i) {
				if(i)
					C_2FC1_017F(spr_x[dst], spr_y[dst]);
				continue;
			}
			whichDir = (b_error[dst] >= 0);
			b_error[dst] += berr_incr[dst][whichDir];
			cur_x[dst] += incr_x[dst][whichDir];
			cur_y[dst] += incr_y[dst][whichDir];

			if(projTile && refreshRate && (i % refreshRate) == 0) {
				spr_new_x = cur_x[dst] - TIL2SCR(MapX - 5);
				spr_new_y = cur_y[dst] - TIL2SCR(MapY - 5);
				if(spr_new_x >= 8 && spr_new_x < 168 && spr_new_y >= 8 && spr_new_y < 168) {
					if(!mustDisplay) {
						if(dst == 0) {
							if(attr & 4)
								MUS_0065(16, i);
							else if(attr & 1)
								MUS_0065(5, delta_max - i);
							else
								MUS_0065(5, i);
						}
						mustDisplay = 1;
					}
					C_2FC1_017F(spr_new_x, spr_new_y);
					if(attr & 2) {
						GR_36(dst, spr_new_x, spr_new_y);
					} else {
						if(i)
							C_2FC1_017F(spr_x[dst], spr_y[dst]);
						spr_x[dst] = spr_new_x;
						spr_y[dst] = spr_new_y;
					}
				}
				if(D_2B92[misType]) {
					 spr_rot[dst] += D_2B92[misType];
					 if(!(attr & 2) && !(attr & 8))
						 GR_33(dst + 8, projTile, spr_rot[dst], D_2BA4[misType], D_2BB6[misType]);
				}
			}

			if(SCR2TIL(cur_x[dst]) == tile_x[dst] && SCR2TIL(cur_y[dst]) == tile_y[dst])
				continue;

			tile_delta_x = abs(SCR2TIL(cur_x[dst]) - src_x);
			tile_delta_y = abs(SCR2TIL(cur_y[dst]) - src_y);
			if(tile_delta_x <= 7 && tile_delta_y <= 7 && D_196C[tile_delta_x][tile_delta_y] <= max_range) {
				tile_x[dst] = SCR2TIL(cur_x[dst]);
				tile_y[dst] = SCR2TIL(cur_y[dst]);
				numTile = GetTileAtXYZ(tile_x[dst], tile_y[dst], MapZ);
				if(
					(tile_delta_x <= 1 && tile_delta_y <= 1 && IsTileWin(numTile)) ||
					!IsTileNos(numTile)
				) {
					for(bp_25c = FindLoc(tile_x[dst], tile_y[dst], MapZ); bp_25c >= 0; bp_25c = NextLoc()) {
						if(
							bp_25c < 0x100 &&
							(GetType(bp_25c) != OBJ_165 || GetFrame(bp_25c) != 0) &&
							!IsDraggedUnder(bp_25c)
						) collision_mask[bp_25c >> 3] |= (1 << (bp_25c & 7));
						if(IsTileNos(TILE_FRAME(bp_25c) - D_0658))
							break;
					}
					if(bp_25c < 0)
						goto c_0db6;
				}
			}
			delta_x[dst] = i;
			break;
c_0db6:
			if(dst == 0) {
				D_EBFE[0] = tile_x[0];
				D_EBFE[1] = tile_y[0];
			}
		}

		if(!mustDisplay)
			continue;

		if(!(i & 3))
			PaletteAnimation();

		if(!(attr & 2)) {
			for(dst = 0; dst < num_dst; dst ++) {
				if(delta_x[dst] > i || (attr & 8))
					GR_39(dst, spr_x[dst], spr_y[dst]);
			}
		}
		if(!(attr & 2)) {
			for(dst = 0; dst < num_dst; dst ++) {
				if(attr & 8)
					GR_36(15, spr_x[dst], spr_y[dst]);
				else if(delta_x[dst] > i)
					GR_36(dst + 8, spr_x[dst], spr_y[dst]);
			}
		}
		if(D_ECBC[0] <= D_ECBC[1] && D_ECBC[2] <= D_ECBC[3]) {
			C_2FC1_032B(D_ECBC[0], D_ECBC[2], D_ECBC[1], D_ECBC[3]);
			GR_45(D_ECBC[0], D_ECBC[2], D_ECBC[1], D_ECBC[3], D_ECBC[0], D_ECBC[2]);
		}
		if(!(attr & 2)) {
			for(dst = 0; dst < num_dst; dst ++) {
				if(delta_x[dst] > i || (attr & 8))
					GR_36(dst, spr_x[dst], spr_y[dst]);
			}
		}
	}

	MUS_0065(0, 0);
	if(projTile && !(attr & 2) && !(attr & 8))
		OtherAnimations();
	for(dst = 0; dst < num_dst; dst ++) {
		if(
			tile_x[dst] == pX_dst[dst] &&
			(tile_y[dst] == pY_dst[dst] || (attr & 1))
		) continue;
		ret = 0;
		pX_dst[dst] = tile_x[dst];
		pY_dst[dst] = tile_y[dst];
		if(dst == 0 && D_196A >= 0)
			D_EC9E = C_2337_08F1(D_196A, tile_x[0], tile_y[0]);
	}
	ShowMouse

	num_dst = 0;
	for(i = 0; i < 0x100/8; i ++) {
		for(dst = 0; dst < 8; dst ++) {
			if(collision_mask[i] & (1 << dst)) {
				D_EC1E[num_dst] = (i << 3) + dst;
				num_dst ++;
			}
			if(num_dst > 0x3e)
				break;
		}
		if(num_dst > 0x3e)
			break;
	}
	D_EC1E[num_dst] = -1;
	return ret;
}

/* explosion pattern
  +---+---+---+---+---+
  |   | 4 | 8 | 1 |   |
  +---+---+---+---+---+
  | b | 7 |   | e | c |
  +---+---+---+---+---+
  | 6 |   | o |   | d |
  +---+---+---+---+---+
  | a | 5 |   | 3 | 0 |
  +---+---+---+---+---+
  |   | 9 | 2 | f |   |
  +---+---+---+---+---+
*/
char D_2BDA[] = { 2, 1, 0, 1,-1,-1,-2,-1, 0,-1,-2,-2, 2, 2, 1, 1};
char D_2BEA[] = {-1, 2,-2,-1, 2,-1, 0, 1, 2,-2,-1, 1, 1, 0, 1,-2};

int far /*C_2FC1_10D2*/*Explosion(int x, int y, int bp06) {
	int si, di;
	int x_dst[MAX_MISSILES], y_dst[MAX_MISSILES];

	for(si = 0; si < MAX_MISSILES; si ++) {
		x_dst[si] = D_2BDA[si] + x;
		y_dst[si] = D_2BEA[si] + y;
	}
	Missiles(x, y, MAX_MISSILES, x_dst, y_dst, 3, bp06, 4|2);
	for(si = 0; si < 0x3e; si ++)
		if(D_EC1E[si] < 0)
			break;
	for(di = FindLoc(x, y, MapZ); si < 0x3f && di >= 0; di = NextLoc()) {
		if(
			di < 0x100 &&
			(GetType(di) != OBJ_165 || GetFrame(di) != 0) &&
			!IsDraggedUnder(di)
		) D_EC1E[si ++] = di;
		if(IsTileNos(TILE_FRAME(di) - D_0658))
			break;
	}
	D_EC1E[si] = -1;
	return D_EC1E;
}

int far /*C_2FC1_11EB*/*MagicWind(int objNum, int x, int y, int bp06) {
	int si;
	int x_dst[5], y_dst[5];

	si = D_19AC[y - GetY(objNum) + 5][x - GetX(objNum) + 5];
	x_dst[0] = x;
	x_dst[1] = x + DirIncrX[si >> 4];
	x_dst[2] = x + DirIncrX[si & 0xf];
	x_dst[3] = x_dst[1] + DirIncrX[si >> 4];
	x_dst[4] = x_dst[2] + DirIncrX[si & 0xf];
	y_dst[0] = y;
	y_dst[1] = y + DirIncrY[si >> 4];
	y_dst[2] = y + DirIncrY[si & 0xf];
	y_dst[3] = y_dst[1] + DirIncrY[si >> 4];
	y_dst[4] = y_dst[2] + DirIncrY[si & 0xf];
	Missiles(GetX(objNum), GetY(objNum), 5, x_dst, y_dst, 7, bp06, 4|2);
	return D_EC1E;
}

/*"Web" pattern
	+---+---+---+
	| 7 | 0 | 1 |
	+---+---+---+
	| 6 | 8 | 2 |
	+---+---+---+
	| 5 | 4 | 3 |
	+---+---+---+*/
C_2FC1_131E(int objNum, int x, int y) {
	int i;
	int dst_x[9], dst_y[9];

	for(i = 0; i < 8; i ++) {
		dst_x[i] = x + DirIncrX[i];
		dst_y[i] = y + DirIncrY[i];
	}
	dst_x[8] = x;
	dst_y[8] = y;
	return Missiles(GetX(objNum), GetY(objNum), 9, dst_x, dst_y, 7, BaseTile[OBJ_035], 8|4);
}

/*"hail storm"?*/
C_2FC1_13B0(int x, int y) {
	int si;
	int bp_3e;
	int tab_x0[6], tab_y0[6];
	int tab_x1[6], tab_y1[6];
	int some_x, some_y;
	int bp_0a, bp_08, bp_06, bp_04, bp_02;

	bp_08 = 0;
	bp_06 = 0;
	bp_02 = 0;
	for(si = 0; si < 6; si ++) {
		tab_x0[si] =
		tab_y0[si] =
		tab_x1[si] =
		tab_y1[si] = -1;
	}
	bp_3e = OSI_rand(10, 30);
	GR_33(7, TIL_18E, 0, 0x7f, 0x7f);
	D_ECBC[0] = D_ECBC[2] = 168;
	D_ECBC[1] = D_ECBC[3] = 7;
	while(bp_3e || bp_08) {
		bp_04 = 0;
		for(si = 0; si < 6; si ++)
			if(tab_x0[si] < 0)
				break;
		if(bp_3e && si < 6 && OSI_rand(0, 7) == 0) {
			tab_x1[si] = OSI_rand(1, 30) + TIL2SCR(x) + OSI_rand(1, 30) - 23;
			tab_y1[si] = OSI_rand(1, 30) + TIL2SCR(y) + OSI_rand(1, 30) - 23;
			tab_x0[si] = tab_x1[si] - 50;
			tab_y0[si] = tab_y1[si] - 50;
			bp_3e --;
		}
		bp_08 = 0;
		if(++bp_06 >= 4) {
			for(si = 0; si < 6; si ++) {
				if(tab_x0[si] >= 0) {
					some_x = tab_x0[si] - TIL2SCR(MapX - 5);
					some_y = tab_y0[si] - TIL2SCR(MapY - 5);
					if(some_x > 15 && some_x < 152 && some_y > 15 && some_y < 152) {
						GR_39(si, some_x, some_y);
						C_2FC1_017F(some_x, some_y);
					}
				}
			}
			for(si = 0; si < 6; si ++) {
				if(tab_x0[si] >= 0) {
					some_x = tab_x0[si] - TIL2SCR(MapX - 5);
					some_y = tab_y0[si] - TIL2SCR(MapY - 5);
					if(some_x > 15 && some_x < 152 && some_y > 15 && some_y < 152) {
						GR_36(7, some_x, some_y);
						C_2FC1_017F(some_x, some_y);
					}
				}
			}
			if(D_ECBC[0] <= D_ECBC[1] && D_ECBC[2] <= D_ECBC[3])
				GR_45(D_ECBC[0], D_ECBC[2], D_ECBC[1], D_ECBC[3], D_ECBC[0], D_ECBC[2]);
			D_ECBC[0] = D_ECBC[2] = 168;
			D_ECBC[1] = D_ECBC[3] = 7;
			for(si = 0; si < 6; si ++) {
				if(tab_x0[si] >= 0) {
					some_x = tab_x0[si] - TIL2SCR(MapX - 5);
					some_y = tab_y0[si] - TIL2SCR(MapY - 5);
					if(some_x > 15 && some_x < 152 && some_y > 15 && some_y < 152) {
						GR_36(si, some_x, some_y);
						C_2FC1_017F(some_x, some_y);
					}
				}
			}
			bp_06 = 0;
		}
		for(si = 0; si < 6; si ++) {
			if(tab_x0[si] >= 0) {
				tab_x0[si] ++;
				tab_y0[si] ++;
				if(tab_x1[si] == tab_x0[si] && tab_y1[si] == tab_y0[si]) {
					tab_x0[si] = -1;
					bp_04 = OSI_rand(400, 500);
					some_x = SCR2TIL(tab_x1[si]);
					some_y = SCR2TIL(tab_y1[si]);
					for(bp_0a = FindLoc(some_x, some_y, MapZ); bp_0a >= 0; bp_0a = NextLoc()) {
						if(bp_0a >= 0x100)
							continue;
						D_1968 = 0;
						bp_02 += LooseHP(bp_0a, 1);
						bp_04 = -1;
						some_x = TIL2SCR(some_x - (MapX - 5)) + 8;
						some_y = TIL2SCR(some_y - (MapY - 5)) + 8;
						if(some_x >= 8 && some_x < 168 && some_y >= 8 && some_y < 168) {
							GR_39(6, some_x, some_y);
							GR_42(TIL_101, some_x - 8, some_y - 8);
							GR_45(some_x - 8, some_y - 8, some_x + 7, some_y + 7, some_x - 8, some_y - 8);
							GR_36(6, some_x, some_y);
							C_2FC1_017F(some_x, some_y);
						}
					}
				} else {
					bp_08 = 1;
				}
			}
		}
		MUS_0065(7, bp_04);
	}
	OtherAnimations();
	return bp_02;
}

/*C_2FC1_18BD*/ShakeScreen(int duration, int ampl) {
	int di;
	int bp_04, bp_02;

	C_0A33_09CE(0);
	for(di = 0; di < duration; di ++) {
		bp_04 = OSI_rand(16 - ampl, ampl + 16);
		bp_02 = OSI_rand(16 - ampl, ampl + 16);
		GR_45(bp_04, bp_02, bp_04 + 144, bp_02 + 144, 16, 16);
		MUS_0065(8, 0);
	}
	OtherAnimations();
}

static C_2FC1_193C() {
	int si, di;

	for(si = 0; si < 11; si ++) {
		for(di = 0; di < 11; di ++)
			Tile_11x11[si][di] = TIL_0FF;
	}
	for(si = 0; si < 0x100; si++)
		D_E5E0[si] = si + 1;
	for(si = 0; si < 11; si ++) {
		for(di = 0; di < 11; di ++)
			Obj_11x11[si][di] = 0;
	}
}

C_2FC1_19A1() {
	C_2FC1_193C();
	D_17B0 = 0;
	ScreenFade = 1;
	C_0A33_09CE(1);
	C_101C_0306();
	C_1100_0306();
}

/*sun, moons'y*/
unsigned char D_2BFA[] = {10,7,5,3,2,1,0,0,0,1,2,3,5,7,10};

/*display sky info*/
C_2FC1_19C5() {
	int si, di;
	int isEclipse;

	if(StatusDisplay != CMD_91 && StatusDisplay != CMD_9E)
		return;
	isEclipse = ((Date_D == 1 && (Date_M % 3) == 0) || SpellFx[15]);
	for(si = 0; si < 9; si ++)
		GR_42(TIL_19B, TIL2SCR(si), 4);
	if(MapZ == 0 || MapZ == 5) {
		/*outside: brittania or gargoyles'realm*/
		/*the sun*/
		if(Time_H > 4 && Time_H < 20) {
			if(isEclipse)
				di = TIL_16B;
			else if(Time_H == 5 || Time_H == 19)
				di = TIL_169;
			else
				di = TIL_16A;
			GR_42(di, (19 - Time_H) << 3, D_2BFA[19 - Time_H]);
		}
		/*the moons*/
		if(!isEclipse) {
			if(D_2CC7 >= 0 && D_2CC7 <= 14)
				GR_42(BaseTile[OBJ_049] + D_2CC6, D_2CC7 << 3, D_2BFA[D_2CC7]);
			if(D_2CC9 >= 0 && D_2CC9 <= 14)
				GR_42(BaseTile[OBJ_049] + D_2CC8, D_2CC9 << 3, D_2BFA[D_2CC9]);
		}
		/*the mountain*/
		for(si = 0; si < 9; si ++)
			GR_42(TIL_160 + si, TIL2SCR(si), 4);
	} else {
		/*in a cave*/
		GR_42(TIL_174, 0, 4);
		for(si = 1; si < 8; si ++)
			GR_42(TIL_175, TIL2SCR(si), 4);
		GR_42(TIL_176, 128, 4);
	}
	GR_45(0, 4, 135, 19, 176, 6);
	C_0A33_12F6(1);
}

C_2FC1_1BCE() {
	C_0C9C_1AE5(0);
	SelectMode = 0;
	HideMouse
	C_0A33_09CE(0);
	GR_4E();
	C_2FC1_19A1();
	ScreenFade = 1;
	C_0A33_09CE(1);
	ShowMouse
}

char D_2C09[] = "portrait.a";

/*load portrait?*/
C_2FC1_1C19(int objNum, int x, int y) {
	int di;
	int bp_06;
	long bp_04;

	bp_06 = GetType(objNum);
	if(objNum >= 0xe0) {
		if(bp_06 == OBJ_175) {
			DISK_confirm(DISK_7);
			objNum = 0xc0;
		} else if(bp_06 == OBJ_17E) {
			DISK_confirm(DISK_7);
			objNum = 0xc1;
		} else if(bp_06 == OBJ_16B) {
			DISK_confirm(DISK_7);
			objNum = 0xc2;
		}
	}
	if(objNum == 1) {
		DISK_confirm(DISK_6);
		D_2C09[9] = 'z';
		objNum = D_2CCB - 1;
		if(objNum < 0)
			return;
	} else {
		if(objNum)
			objNum --;
		if(objNum >= 0x62) {
			DISK_confirm(DISK_7);
			D_2C09[9] = 'b';
			objNum -= 0x62;
		} else {
			DISK_confirm(DISK_6);
			D_2C09[9] = 'a';
		}
	}
	di = OSI_open(D_2C09);
	if(di < 0)
		return;
	OSI_read(di, (long)(objNum << 2), 4, &bp_04);
	decompress(di, bp_04, ((struct tPixmap far *)ScratchBuf)->data, FP_SEG(ScratchBuf) + 0x100);
	OSI_close(di);
	((struct tPixmap far *)ScratchBuf)->width = 56;
	((struct tPixmap far *)ScratchBuf)->height = 64;
	GR_18(ScratchBuf, x, y);
}

char D_2C14[] = "converse.a";

/*C_2FC1_1D40*/LoadConversation(int di/*bp0a*/, void far *bp06) {
	int si;
	long bp_0c, bp_08;
	int bp_04, bp_02;

	bp_04 = GetType(di);
	if(di >= 0xe0) {
		/*generic conversations?*/
		if(bp_04 == OBJ_175) {/*Wisp*/
			DISK_confirm(DISK_7);
			bp_02 = 0x66;
			D_2C14[9] = 'b';
		} else if(bp_04 == OBJ_17E) {/*Guard*/
			DISK_confirm(DISK_7);
			bp_02 = 0x67;
			D_2C14[9] = 'b';
		} else if(bp_04 == OBJ_16B) {/*Gargoyle*/
			DISK_confirm(DISK_7);
			bp_02 = 0x68;
			D_2C14[9] = 'b';
		} else {
			return 0;
		}
	} else if(di > 0x62) {
		DISK_confirm(DISK_7);
		D_2C14[9] = 'b';
		bp_02 = di - 0x63;
	} else {
		DISK_confirm(DISK_6);
		D_2C14[9] = 'a';
		bp_02 = di;
	}
	si = OSI_open(D_2C14);
	if(si >= 0) {
		/*read the NPC's data offset*/
		OSI_read(si, (long)(bp_02 << 2), 4, &bp_0c);
		if(bp_0c) {
			if(bp06 == TalkBuf)
				MUS_091A(0);
			/*read the inflated data size*/
			OSI_read(si, bp_0c, 4, &bp_08);
			if(bp_08 && bp_08 < 0x2800L)
				decompress(si, bp_0c, bp06, FP_SEG(ScratchBuf) + 0x400);
			else
				OSI_read(si, bp_0c + 4, 0x2800L, bp06);
		} else {
			di = 0;
		}
		OSI_close(si);
	}
	return di;
}

/*display equipment bg chara*/
C_2FC1_1EAF(int si/*bp08*/, int di/*bp06*/) {
	GR_2D(TIL_170, si,      di);
	GR_2D(TIL_171, si + 16, di);
	GR_2D(TIL_172, si,      di + 16);
	GR_2D(TIL_173, si + 16, di + 16);
}

static /*C_2FC1_1F14*/peer_tile(int tileNum, int x, int y) {
	int color;
	int i, j, pattern;

	pattern = 1;
	if(tileNum < 0) {
		color = 15;
		pattern = 0;
	} else if(tileNum == TIL_0FF) {
		color = 0;
	} else if(IsTerrainDamage(tileNum)) {
		color = D_2A4A;
	} else if(IsTerrainImpass(tileNum)) {
		if(IsTerrainWet(tileNum))
			color = D_2A4E;
		else
			color = D_2A5A;
	} else {
		color = -1;
	}
	if(color < 0 && tileNum >= TIL_100)
		return;
	if(color < 0)
		color = D_2A4C;
	GR_06(color);
	for(i = 0; i < 4; i ++) {
		for(j = 0; j < 4; j ++) {
			if(((i + j) & 1) == pattern)
				GR_09(i + x, j + y);
		}
	}
}

/*peer*/
C_2FC1_1FD6() {
	int si, di;
	int bp_10, bp_0e, bp_0c, bp_0a;
	int bp_08, bp_06, bp_04, isUnderground;

	isUnderground = (MapZ > 0 && MapZ < 5);
	for(si = 0; si < AREA_H; si ++) {
		for(di = 0; di < AREA_W; di ++)
			AreaFlags[si][di] =
			AreaLight[si][di] = 0;
	}
	bp_10 = bp_0c = 0;
	bp_0e = bp_0a = 40;
	if(isUnderground) {
		C_1100_0131(ViewX + 5, ViewY + 5, 40, 2);
	} else if(MapZ == 0) {
		if(AreaX == 0x3f0) {
			bp_0c = 8;
		} else if(AreaX == 0x3e0) {
			bp_0a = 0x20;
		} else if(AreaY >= 0x3f0) {
			bp_10 = 0x400 - AreaY;
		} else if(AreaY == 0x3e0 || AreaY == 0x3e8) {
			bp_0e = 0x400 - AreaY;
		}
	}
	D_ECC4->_0f = 1;
	GR_06(0);
	GR_12(8, 8, 167, 167);
	for(si = bp_10; si < bp_0e; si ++) {
		for(di = bp_0c; di < bp_0a; di ++) {
			bp_08 = AreaTiles[si][di];
			if(isUnderground && !(AreaFlags[si][di] & 0x80))
				bp_08 = TIL_0FF;
			peer_tile(bp_08, (di << 2) + 8, (si << 2) + 8);
		}
	}
	for(bp_06 = SearchArea(AreaX, AreaY, AreaX + (AREA_W-1), AreaY + (AREA_H-1)); bp_06 >= 0; bp_06 = NextArea()) {
		if(bp_06 == 0 && !IsPlrControl(bp_06))
			continue;
		di = GetX(bp_06) - AreaX;
		si = GetY(bp_06) - AreaY;
		if(isUnderground && !(AreaFlags[si][di] & 0x80))
			continue;
		if(bp_06 < 0x100) {
			peer_tile(-1, (di << 2) + 8, (si << 2) + 8);
		} else {
			bp_08 = TILE_FRAME(bp_06);
			peer_tile(bp_08, (di << 2) + 8, (si << 2) + 8);
			if(IsTileDoubleH(bp_08) && di) {
				if(IsTileDoubleV(bp_08) && si) {
					peer_tile(bp_08 - 2, (di << 2) + 8, (si << 2) + 4);
					peer_tile(bp_08 - 3, (di << 2) + 4, (si << 2) + 4);
				}
				peer_tile(bp_08 - 1, (di << 2) + 4, (si << 2) + 8);
			} else if(IsTileDoubleV(bp_08) && si) {
				peer_tile(bp_08 - 1, (di << 2) + 8, (si << 2) + 4);
			}
		}
	}
	GR_06(15);
	GR_12((ViewX << 2) + 28, (ViewY << 2) + 28, (ViewX << 2) + 31, (ViewY << 2) + 31);
	GR_06(0);
	D_ECC4->_0f = 0;
	C_2FC1_0212(8, 8, 167, 167);
	bp_04 = CON_mouseOffAt(8, 8, 167, 167);
	GR_45(8, 8, 167, 167, 8, 8);
	CON_mouseOn(bp_04);
	D_049C = 0;
	CON_getch();
	D_049C = 1;
	C_1100_0306();
}
