/*
 * Ultima VI - The False Prophet
 * Copyright (c) 1990 Origin Systems, Inc. and Richard Gariott
 * reverse-coded by Ergonomy Joe in 2013
 */
/*
	module "to compute only visible part of the area"
*/
#include "u6.h"

/*05E4*/int ViewX = 12;
/*05E6*/int ViewY = 12;
int D_05E8 = 0;/*XRay vision/see through walls?*/

static char D_05EA[][8] = {
	{0,1,2,3,4,5,6,7},
	{1,1,2,3,4,5,6,7},
	{2,2,3,4,5,6,6,7},
	{3,3,4,4,5,6,7,8},
	{4,4,5,5,6,7,7,8},
	{5,5,6,6,7,7,8,9},
	{6,6,6,7,7,8,8,9},
	{7,7,7,8,8,9,9,10}
};

/*062A*/static unsigned char visited_w = 0;
/*062B*/static unsigned char visited_r = 0;

static char D_062C[] = {      0, 1,      0,      0, -1, -1,       0,       0};/*delta_x*/
static char D_0634[] = {-     1, 0,      1,      1,  0,  0, -     1, -     1};/*delta_y*/
static char D_063C[] = {-AREA_W, 1, AREA_W, AREA_W, -1, -1, -AREA_W, -AREA_W};/*delta_y * AREA_W + delta_x*/

static unsigned char D_0644[] = {
	 0,/*0000*/
	 0,/*0001*/
	 1,/*0010*/
	10,/*0011*/
	 0,/*0100*/
	 0,/*0101*/
	 2,/*0110*/
	 2,/*0111*/
	 1,/*1000*/
	 5,/*1011*/
	 1,/*1010*/
	 1,/*1001*/
	11,/*1110*/
	 0,/*1111*/
	 2,/*1100*/
	 2 /*1101*/
};

static /*C_1100_000B*/__IsVisible(int x, int y) {
	int flag, tile;

	tile = AreaTiles[0][AreaOffset];
	flag = AreaFlags[0][AreaOffset];
	if(flag & 0x20)
		return 0;
	if(IsTileWin(tile) || (flag & 0x10)) {
		return
			(ViewX + 5 == x && abs(ViewY + 5 - y) < 2) ||
			(ViewY + 5 == y && abs(ViewX + 5 - x) < 2)
		;
	}
	if(!(flag & 0x04) && IsTileOpa(tile))
		return 0;
	return 1;
}

/*uncalled?*/
static __1100_009F(x, y) {
	return CLOSE_ENOUGH(5, x, y, MapX, MapY);
}

static /*C_1100_00CF*/__SetVisited(int x, int y) {
	if(x >= 0 && x < AREA_W && y >= 0 && y < AREA_H) {
		D_BBCC[visited_w] = x;
		D_BCCC[visited_w] = y;
		visited_w ++;
		if(visited_w == visited_r)
			visited_w --;
		else
			AreaFlags[y][x] |= 0x40;
	}
}

C_1100_0131(int x, int y, int dist, int bp06) {
	int some_x, some_y;
	int dir, light;

	AreaOffset = y << 3;
	AreaOffset += AreaOffset << 2;
	AreaOffset += x;
	if(__IsVisible(x, y)) {
		__SetVisited(x, y);
	} else {
		AreaOffset ++;
		if(__IsVisible(x + 1, y))
			__SetVisited(x + 1, y);
		AreaOffset += (AREA_W-1);
		if(__IsVisible(x, y + 1))
			__SetVisited(x, y + 1);
	}
	while(visited_w != visited_r) {
		some_x = D_BBCC[visited_r];
		some_y = D_BCCC[visited_r];
		visited_r ++;
		AreaOffset = some_y << 3;
		AreaOffset += AreaOffset << 2;
		AreaOffset += some_x;

		light = 4 - D_05EA[abs(some_x - x)][abs(some_y - y)] + dist;
		if(light < 0)
			light = 0;
		else if(light > 4)
			light = 4;
		AreaLight[0][AreaOffset] += light;

		if(bp06)
			AreaFlags[0][AreaOffset] |= 0x80;
		if(__IsVisible(some_x, some_y)) {
			for(dir = 0; dir < 8; dir ++) {
				some_x += D_062C[dir];
				some_y += D_0634[dir];
				AreaOffset += D_063C[dir];
				if(AreaFlags[0][AreaOffset] & 0x40)
					continue;
				if(bp06 == 1) {
					if(
						ViewX - 1 > some_x || ViewX + 11 < some_x ||
						ViewY - 1 > some_y || ViewY + 11 < some_y
					) continue;
					__SetVisited(some_x, some_y);
				} else if(bp06 == 2) {/*for peer*/
					__SetVisited(some_x, some_y);
				} else if(D_05EA[abs(some_x - x)][abs(some_y - y)] <= dist + 3) {
					__SetVisited(some_x, some_y);
				}
			}
		}
	}
}

C_1100_0306() {
	unsigned char *pTiles,*pFlags,*pLight;
	int *pView;
	int some_x,some_y,x,y;
	int tile,bp_10,bp_0e,bp_0c,imped,bp_08,bp_06,objNum;
	int light;

	if(D_05E8) {
		for(some_y = 0; some_y < 11; some_y ++) {
			for(some_x = 0; some_x < 11; some_x ++)
				Tile_11x11[some_y][some_x] = AreaTiles[some_y + ViewY][some_x + ViewX];
		}
		for(some_x = 8; some_x < 32; some_x ++) {
			for(some_y = 8; some_y < 32; some_y ++)
				AreaLight[some_x][some_y] = 4;
		}
		ShowObjects();
		return;
	}

	pFlags = &(AreaFlags[8][8]);
	pLight = &(AreaLight[8][8]);
	for(some_x = 8; some_x < 32; some_x ++) {
		for(some_y = 8; some_y < 32; some_y ++) {
			*pFlags++ =
			*pLight++ = 0;
		}
		pFlags += 16;
		pLight += 16;
	}
	some_x = (MapX - 0x10) & 0xfff8;
	some_y = (MapY - 0x10) & 0xfff8;
	for(
		objNum = SearchArea(some_x + 8, some_y + 8, some_x + 31, some_y + 31);
		objNum >= 0;
		objNum = NextArea()
	) {
		x = GetX(objNum) - some_x;
		y = GetY(objNum) - some_y;
		pFlags = &(AreaFlags[y][x]);
		tile = TILE_FRAME(objNum);

		if(IsTileWin(tile))
			*pFlags |= 0x10;
		else if(IsTileOpa(tile))
			*pFlags |= 0x20;
		else if(IsTileBa(tile))
			*pFlags |= 0x04;

		if(IsTileOpa(tile - 1)) {
			if(IsTileDoubleV(tile) && y)
				*(pFlags - AREA_W) |= 0x20;
			if(IsTileDoubleH(tile) && x)
				*(pFlags - 1) |= 0x20;
		}

		if(IsTerrainWall(tile)) {
			*pFlags |= 0x08;
			if(IsTileDoubleV(tile) && y)
				*(pFlags - AREA_W) |= 0x08;
			if(IsTileDoubleH(tile) && x)
				*(pFlags - 1) |= 0x08;
		}

		if(GetTileLight(tile)) {
			light = GetTileLight(tile);
			if((*pFlags & 0x03) < light)
				*pFlags = (*pFlags & ~0x03) | light;
		}
	}

	C_1100_0131(ViewX + 5, ViewY + 5, D_2C55, 1);

	/*light sources(if night)?*/
	if(D_2C55 < 7) {
		pFlags = &(AreaFlags[8][8]);
		pTiles = &(AreaTiles[8][8]);
		for(some_y = 8; some_y < 32; some_y ++) {
			for(some_x = 8; some_x < 32; some_x ++) {
				if((*pFlags & 0x03) && (*pFlags & 0x80)) {
					if(
						IsTerrainWall(*pTiles) &&
						!((*(pFlags + AREA_W) & 0x80) && (*(pFlags + AREA_W) & 0x80))/*bug?why twice?*/
					) continue;
					for(x = some_x - 3; x <= some_x + 3; x ++) {
						for(y = some_y - 3; y <= some_y + 3; y ++)
							AreaFlags[y][x] &= ~0x40;
					}
					C_1100_0131(some_x, some_y, (*pFlags & 0x03) - 4, 0);
				}
				pFlags ++;
				pTiles ++;
			}
			pFlags += 16;
			pTiles += 16;
		}
	}

	pView = &(Tile_11x11[0][0]);
	pLight = &(AreaLight[ViewY][ViewX]);
	pFlags = &(AreaFlags[ViewY][ViewX]);
	pTiles = &(AreaTiles[ViewY][ViewX]);
	for(some_y = 0; some_y < 11; some_y ++) {
		for(some_x = 0; some_x < 11; some_x ++) {
			x = some_x + ViewX;
			y = some_y + ViewY;
			if(!(*pFlags & 0x80)) {
				tile = TIL_0FF;/*hidden*/
			} else if(*pLight == 0) {
				tile = TIL_1BC;/*darkness*/
			} else {
				tile = *pTiles;
				/*"hide inside wall" patch?*/
				bp_10 = TerrainType[tile] & 0x0f;
				if(bp_10 == (TERRAIN_FLAG_04|TERRAIN_FLAG_02)) {
					bp_0e = tile & 0x0f0;
					if(bp_0e < TIL_090)
						bp_0e = TIL_090;
					bp_0c = 0;
					if(*(pFlags - AREA_W) & 0x80)
						bp_0c |= 8;
					if(*(pFlags + 1) & 0x80)
						bp_0c |= 4;
					if(*(pFlags + AREA_W) & 0x80)
						bp_0c |= 2;
					if(*(pFlags - 1) & 0x80)
						bp_0c |= 1;
					if(bp_0c != 0xf) {
						imped = TerrainType[tile] >> 4;
						if(imped & 4) {
							bp_08 = *(pTiles - AREA_W);
							bp_06 = TerrainType[bp_08] & 0x0f;
							if(
								!(*(pFlags - AREA_W) & 0x08) &&
								(!(bp_06 & TERRAIN_FLAG_04) || !(TerrainType[bp_08] & TERRAIN_FLAG_20))
							) imped &= ~8;
						}
						if(imped & 2) {
							bp_08 = *(pTiles - 1);
							bp_06 = TerrainType[bp_08] & 0x0f;
							if(
								!(*(pFlags - 1) & 0x08) &&
								(!(bp_06 & TERRAIN_FLAG_04) || !(TerrainType[bp_08] & TERRAIN_FLAG_40))
							) imped &= ~1;
						}
						if(imped == (4|2|1) || imped == (8|2|1) || imped > (8|4) || imped == bp_0c) {
							imped &= bp_0c;
							if(imped == (2|1) || imped == (8|4))
								tile = TIL_100 + (bp_0e >> 3) - (TIL_090 >> 3) + D_0644[imped];
							else
								tile = bp_0e + D_0644[imped];
						}
					}
				}

			}
			*pView++ = tile;
			pFlags ++;
			pTiles ++;
			pLight ++;
		}
		pFlags += 29;
		pTiles += 29;
		pLight += 29;
	}
	ShowObjects();
}
