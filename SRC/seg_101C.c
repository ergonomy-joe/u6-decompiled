/*
 * Ultima VI - The False Prophet
 * Copyright (c) 1990 Origin Systems, Inc. and Richard Gariott
 * reverse-coded by Ergonomy Joe in 2013
 */
/*
	map module
*/
#include "u6.h"

int D_05CA = 0x3FF;
int D_05CC = -1;/*file "chunks" handler*/
int D_05CE = -1;/*file "map" handler*/
int D_05D0 = -1;
int D_05D2[4] = {-1, -1, -1, -1};/*currently loaded map portions?*/
int D_05DA = -1;/*prevX?*/
int D_05DC = -1;/*prevY?*/
int D_05DE = -1;/*prevZ?*/

#define MK_MAP_ID(x,y) (((x) >> 7) + (((y) >> 4) & 0x38))

static /*C_101C_000F*/cacheChunk(int chunkIdx) {
	int i, cacheIdx;
	int oldestIdx;

	if(D_05CC < 0)
		return 0;
	cacheIdx = -1;
	oldestIdx = 0;
	/*lookup chunk in cache*/
	for(i = 0; i < CHUNKS_CACHE_MAX; i ++) {
		if(D_B7A8[i] == chunkIdx) {
			cacheIdx = i;
			break;
		}
		if(D_B784[i] > D_B784[oldestIdx])
			oldestIdx = i;
	}
	/*load if not cached*/
	if(cacheIdx < 0) {
		cacheIdx = oldestIdx;
		D_B7A8[cacheIdx] = chunkIdx;
		DISK_confirm(DISK_5);
		OSI_read(D_05CC, (long)chunkIdx << 6, 0x40, D_B7C8[cacheIdx]);
	}
	/*update chunks "age"*/
	for(i = 0; i < CHUNKS_CACHE_MAX; i ++) {
		if(D_B784[i] < D_B784[cacheIdx])
			D_B784[i] ++;
	}
	D_B784[cacheIdx] = 0;

	return cacheIdx;
}

/*map
level 0 is divided in 60 16x16x(1.5) parts
+--+--+--+--+--+--+--+--+
|00|01|02|03|04|05|06|07|
+--+--+--+--+--+--+--+--+
|10|11|12|13|14|15|16|17|
+--+--+--+--+--+--+--+--+
|20|21|22|23|24|25|26|27|
+--+--+--+--+--+--+--+--+
|30|31|32|33|34|35|36|37|
+--+--+--+--+--+--+--+--+
|40|41|42|43|44|45|46|47|
+--+--+--+--+--+--+--+--+
|50|51|52|53|54|55|56|57|
+--+--+--+--+--+--+--+--+
|60|61|62|63|64|65|66|67|
+--+--+--+--+--+--+--+--+
|70|71|72|73|
+--+--+--+--+

levels 1~5 are in 4 16x16x(1.5) parts
*/
C_101C_00D7(int x, int y, int z) {
	int mapId, di;
	int mustLoad, bp_0e, bp_0c, bp_0a;
	long bp_08;
	unsigned char far *bp_04;

	mustLoad = (D_05D0 != z);
	if(!mustLoad && z == 0) {
		mapId = MK_MAP_ID(x, y);
		mustLoad = 1;
		for(di = 0; di < 4; di ++) {
			if(D_05D2[di] == mapId) {
				mustLoad = 0;
				break;
			}
		}
	}
	if(!mustLoad)
		return;
	if(z != 0) {
		DISK_confirm(DISK_8);
		D_05CE = OSI_open(/*05E0*/"map");
		OSI_read(D_05CE, (long)((z + z + z) << 9) + 0x5a00, 4 * 0x180, D_B7A4);
		OSI_close(D_05CE);
		D_05D0 = z;
		for(di = 0; di < 4; di ++)
			D_05D2[di] = -1;
	} else {
		mapId = MK_MAP_ID(x, y);
		if(D_05D2[0] >= 0) {
			bp_0e = (mapId - D_05D2[0]) & 7;
			bp_0c = (mapId - D_05D2[0]) & 0x38;
			mapId = D_05D2[0];
			if(bp_0e > 1 && bp_0e < 5)
				mapId = (mapId & 0x38) + ((mapId + bp_0e - 1) & 7);
			else if(bp_0e > 4)
				mapId = (mapId & 0x38) + ((mapId + bp_0e) & 7);
			if(bp_0c > 0x08 && bp_0c < 0x28)
				mapId = (mapId & 7) + ((mapId + bp_0c - 1) & 0x38);
			else if(bp_0c > 0x20)
				mapId = (mapId & 7) + ((mapId + bp_0c) & 0x38);
		}
		if(mapId != D_05D2[0]) {
			D_05D2[0] = mapId;
			D_05D2[1] = (mapId + 1) & 0x3f;
			D_05D2[2] = (mapId + 8) & 0x3f;
			D_05D2[3] = (mapId + 9) & 0x3f;
			DISK_confirm(DISK_8);
			D_05CE = OSI_open(/*05E0*/"map");
			for(di = 0; di < 4; di ++) {
				bp_08 = (long)D_05D2[di] << 7; bp_08 += bp_08 << 1;
				bp_0a = di << 7; bp_0a += bp_0a << 1;
				bp_04 = D_B7A4 + bp_0a;
				OSI_read(D_05CE, bp_08, 0x180, bp_04);
			}
			OSI_close(D_05CE);
		}
		D_05D0 = 0;
	}
}

/*refresh tile map?*/
C_101C_0306() {
	unsigned char *pMap, *pChunk;
	int i, j;
	int anX, anY, cacheIdx;

	AreaX = anX = (MapX - 0x10) & 0x3f8;
	ViewX = (MapX - anX - 5) & 0x1f;
	AreaY = anY = (MapY - 0x10) & 0x3f8;
	ViewY = (MapY - anY - 5) & 0x1f;
	if(MapZ)
		D_05CA = 0xff;
	else
		D_05CA = 0x3ff;
	/*redundant?*/
	anX = AreaX;
	anY = AreaY;
	/* */
	if(anX != D_05DA || anY != D_05DC || MapZ != D_05DE) {
		C_1184_2ECC();
		D_06E6 = 0;
		for(i = 0; i < 0x100; i ++) {
			anX = (GetX(i) - AreaX) & D_05CA;
			anY = (GetY(i) - AreaY) & D_05CA;
			if(ObjShapeType[i])
			if(GetCoordUse(i) == LOCXYZ)
			if(GetZ(i) == MapZ)
			if(anX < 40 && anY < 40)
			if(MapObjPtr[anY][anX] != i)
				MoveObj(i, GetX(i), GetY(i), MapZ);
		}
		D_06E6 = 1;

		anX = AreaX;
		anY = AreaY;
		C_1184_19AA(anX + 20, AreaY + 20, MapZ);

		D_05DA = anX;
		D_05DC = anY;
		D_05DE = MapZ;
		for(i = 0; i < 40; i += 8) {
			for(j = 0; j < 40; j += 8) {
				cacheIdx = cacheChunk(getChunkIdxAt((D_05DA + i) & 0x3ff, (D_05DC + j) & 0x3ff, MapZ));
				pChunk = D_B7C8[cacheIdx];
				pMap = &(AreaTiles[j][i]);
				for(anY = 0; anY < 8; anY ++) {
					for(anX = 0; anX < 8; anX ++)
						*pMap++ = *pChunk++;
					pMap += 32;
				}
			}
		}
		EGG_hatchArea();
	}
	C_0A33_121A();
	D_17B0 = 1;
}

C_101C_052D() {
	int si;

	for(si = 0; si < 0x10; si++) {
		D_B7A8[si] = -1;
		D_B784[si] = si;
	}
}

C_101C_054C() {
	LoadNewRegions(MapX, MapY, MapZ);
	C_101C_00D7(MapX, MapY, MapZ);
	C_101C_0306();
}

/*C_101C_0572*/PartyEnter(int withAnimation) {
	int i;

	/*dismount horses*/
	for(i = 0; i < PartySize; i ++) {
		if(GetType(Party[i]) == OBJ_1AF)
			C_27A1_55F0(Party[i]);
	}

	C_101C_054C();

	if(withAnimation) {
		int j, stopAnim;
		for(j = 0; j < 30; j ++) {
			stopAnim = 1;
			for(i = 0; i < PartySize; i ++) {
				int objNum = Party[i];
				if(IsAsleep(objNum))
					ObjShapeType[objNum] = OrigShapeType[objNum];
				Rstbis_0006(objNum);
				if(GetZ(objNum) == MapZ && !(Isbis_0016(objNum) || IsDraggedUnder(objNum))) {
					if(GetX(objNum) == MapX && GetY(objNum) == MapY) {
						if(withAnimation) {
							ShowObjects();
							OtherAnimations();
						}
						D_06E6 = 0;
						MoveObj(objNum, MapX, MapY, 15);
						D_06E6 = 1;
						if(withAnimation == 1) {
							ScreenFade = 1;
							ShowObjects();
							OtherAnimations();
						}
					} else if(!TryMoveTo(objNum, MapX, MapY)) {
						if(COMBAT_getCathesus(objNum, MapX, MapY) < 2)
							MoveObj(objNum, MapX, MapY, MapZ);
						else
							TryStraightMove(objNum, OSI_rand(0, 3) << 1, 1);
					} else {
						stopAnim = 0;
					}
				}
				MovePts[objNum] = DEXTE[objNum];
			}
			if(withAnimation) {
				ShowObjects();
				OtherAnimations();
			}
			if(stopAnim)
				break;
		}
	}

	D_06E6 = 0;
	for(i = 0; i < PartySize; i ++) {
		if(GetZ(Party[i]) != 15)
			MoveObj(Party[i], MapX, MapY, 15);
	}
	D_06E6 = 1;
}

/*C_101C_077E*/PartyExit(int x, int y, int z, int withAnimation) {
	int i;

	D_06E6 = 0;
	for(i = 0; i < PartySize; i ++)
		MoveObj(Party[i], x, y, z);
	D_06E6 = 1;

	SetPartyMode();

	if(withAnimation == 1) {
		ScreenFade = 1;
		ShowObjects();
		OtherAnimations();
	}

	for(i = 0; i < 5; i ++) {
		MoveFollowers(Party[Active], 1);
		if(withAnimation) {
			ShowObjects();
			OtherAnimations();
		}
	}
	for(i = 0; i < PartySize; i ++)
		MovePts[Party[i]] = DEXTE[Party[i]];
}

/*C_101C_0828*/PartyTeleport(int x, int y, int z, int withAnimation) {
	int objNum;

	PartyEnter(withAnimation);

	objNum = C_1184_07A7(OBJ_054, MapX, MapY);
	if(objNum >= 0)
		DeleteObj(objNum);

	MapX = x;
	MapY = y;
	MapZ = z;
	C_101C_054C();

	ForceHatching = 1;
	EGG_hatchArea();
	ForceHatching = 0;

	C_1100_0306();
	C_2FC1_19C5();
	C_0A33_1355(0);
	PartyExit(x, y, z, withAnimation);
}

/*enter dungeon/use ladder*/
C_101C_089E(int objNum) {
	int z_incr;

	if(InCombat)
		COMBAT_breakOff();
	if(MapZ && (MapZ == 5 || ObjShapeType[objNum] == TypeFrame(OBJ_131, 1)))
		z_incr = -1;
	else
		z_incr = 1;
	MapX = GetX(objNum);
	MapY = GetY(objNum);
	PartyEnter(2);
	MapZ += z_incr;
	if(MapZ == 1 && z_incr == 1) {
		MapX = ((MapX >> 2) & 0xf8) + (MapX & 7);
		MapY = ((MapY >> 2) & 0xf8) + (MapY & 7);
	} else if(MapZ == 0 && z_incr == -1) {
		MapX = ((MapX << 2) & 0x3e0) + (MapX & 7);
		MapY = ((MapY << 2) & 0x3e0) + (MapY & 7);
		if(GetQual(objNum) & 1)
			MapX += 8;
		if(GetQual(objNum) & 2)
			MapX += 0x10;
		if(GetQual(objNum) & 4)
			MapY += 8;
		if(GetQual(objNum) & 8)
			MapY += 0x10;
	}
	C_101C_054C();
	C_1100_0306();
	if(
		MapZ == 0 || MapZ == 5 ||
		(MapZ == 1 && z_incr == 1) ||
		(MapZ == 4 && z_incr == -1)
	) {
		C_2FC1_19C5();
		C_0A33_1355(0);
	}
	PartyExit(MapX, MapY, MapZ, 2);
	MUS_09A8();
}

/*C_101C_0A3A*/GateTravel(int si/*bp06*/) {
	if(InCombat)
		COMBAT_breakOff();
	if(D_2C74[si][0] || D_2C74[si][1] || D_2C74[si][2])
		PartyTeleport(D_2C74[si][0], D_2C74[si][1], D_2C74[si][2], 1);
	else
		PartyTeleport(MapX, MapY, MapZ, 1);
	MUS_09A8();
}

/*revive/help*/
C_101C_0ACC() {
	int si, di;
	int bp_02;

	Active = -1;
	for(si = 0; si < 0xe0; si ++) {
		if(IsPlrControl(si) && IsDead(si)) {
			for(bp_02 = Link[0x100]; bp_02 >= 0; bp_02 = Link[bp_02]) {
				if(GetType(bp_02) == OBJ_153 || GetType(bp_02) == OBJ_162)
				if(GetQual(bp_02) == si) {
					Selection.obj = bp_02;
					if(si == 1) {
						C_1184_1C52(bp_02);
						ClrDead(1);
						ObjShapeType[1] = OrigShapeType[1];
						MAGIC[1] = INTEL[1];
						Rstbis_001f(1);
						if(KARMA < 98)
							ExpPoints[1] = ((long)ExpPoints[1] * KARMA) / 100L;
					} else {
						D_06E6 = 0;
						C_1944_1A42(bp_02);
					}
					break;
				}
			}
		}
		if(NPCMode[si] == AI_VIGILANTE)
			NPCMode[si] = AI_FINDPATH;
	}
	for(si = 0; si < PartySize; si ++) {
		di = Party[si];
		if(IsAsleep(di)) {
			ClrAsleep(di);
			ObjShapeType[di] = OrigShapeType[di];
		}
		SetAlignment(di, GOOD);
		Rst_0046(di);
		Rstbis_001e(di);
		HitPoints[di] = MaxHP(di);
		MovePts[di] = DEXTE[di];
	}
	for(si = 0xe0; si < 0x100; si ++) {
		if(GetType(si) == OBJ_17E)
			DeleteObj(si);
	}
	for(si = 0; si < 16; si ++)
		SpellFx[si] = 0;
	for(si = 0; si < 0x40; si ++)
		D_EC1E[si] = -1;
	SetPartyMode();
	COMBAT_breakOff();
	PartyTeleport(0x133, 0x160, 0, 0);
	AllowNPCTeleport = 1;
	while(
		Time_H < 8 ||
		(Time_H > 11 && Time_H < 14) ||
		Time_H > 17
	) {
		C_0A33_1355(15);
		C_1E0F_464A();
	}
	AllowNPCTeleport = 0;
	ScreenFade = 1;
	ShowObjects();
	MUS_0065(21, 0);
	OtherAnimations();
	MUS_09A8();
	StatusDirty = 1;
	D_06E6 = 1;
}

static /*C_101C_0CF7*/getChunkIdxAt(int x, int y, int z) {
	int si;
	int bp_06, bp_04, bp_02;

	x &= 0x3ff;
	y &= 0x3ff;
	C_101C_00D7(x, y, z);
	if(z != 0) {
		si = (x >> 3) & 0x1f;
		si += (y << 2) & 0x3e0;
		si += si >> 1;
		if(x & 8)
			bp_06 = *((int far *)(D_B7A4 + si)) >> 4;
		else
			bp_06 = *((int far *)(D_B7A4 + si)) & 0xfff;
	} else {
		bp_04 = MK_MAP_ID(x, y);
		bp_02 = 0;
		if((bp_04 - D_05D2[0]) & 1)
			bp_02 = 0x100;
		if((bp_04 - D_05D2[0]) & 8)
			bp_02 += 0x200;
		si = ((x >> 3) & 0xf) + bp_02;
		si += (y << 1) & 0xf0;
		si += si >> 1;
		if(x & 8)
			bp_06 = *((int far *)(D_B7A4 + si)) >> 4;
		else
			bp_06 = *((int far *)(D_B7A4 + si)) & 0xfff;
	}

	return bp_06;
}

unsigned char /*C_101C_0DC9*/GetTileAtXYZ(int x, int y, int z) {
	int cacheIdx;
	int bp_04, bp_02;

	x &= D_05CA;
	y &= D_05CA;
	bp_04 = (x - AreaX) & D_05CA;
	bp_02 = (y - AreaY) & D_05CA;
	if(bp_04 < 40 && bp_02 < 40)
		return AreaTiles[bp_02][bp_04];

	cacheIdx = cacheChunk(getChunkIdxAt(x, y, z));
	return D_B7C8[cacheIdx][y & 7][x & 7];
}
