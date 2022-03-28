/*
 * Ultima VI - The False Prophet
 * Copyright (c) 1990 Origin Systems, Inc. and Richard Gariott
 * reverse-coded by Ergonomy Joe in 2013
 */
/*
	module REST/REPAIR
*/
#include "u6.h"

#undef abs
#include <math.h>

/*edible items*/
int D_2CCE[] = {
	OBJ_080,OBJ_081,
	OBJ_082,OBJ_083,
	OBJ_084,OBJ_085,
	OBJ_087,OBJ_05F,
	OBJ_0B8,OBJ_060,
	OBJ_109,OBJ_0B4,
	OBJ_0D1,OBJ_0D2
};

static /*C_3200_000E*/IsEdible(int objType) {
	int si;

	for(si = 0; si < 14; si ++) {
		if(D_2CCE[si] == objType)
			return 1;
	}
	return 0;
}

static /*C_3200_0035*/TryEatExclusive(int objNum) {
	int si, di;

	for(si = Link[objNum]; si >= 0 && GetCoordUse(si) != LOCXYZ && C_1184_1D2E(si, objNum); si = Link[si]) {
		di = GetType(si);
		if(IsEdible(di)) {
			TakeObj(objNum, di, 1);
			return 1;
		}
	}
	return 0;
}

static /*C_3200_0094*/TryEat(int partyNum) {
	int si;

	if(TryEatExclusive(Party[partyNum]))
		return 1;
	for(si = 0; si < PartySize; si ++) {
		if(TryEatExclusive(Party[si]))
			return 1;
	}
	return 0;
}

/*check wilderness*/
static C_3200_00D3() {
	int x, y, numTile;

	for(x = MapX - 5; x <= MapX + 5; x ++) {
		for(y = MapY - 5; y <= MapY + 5; y ++) {
			numTile = GetTileAtXYZ(x, y, MapZ);
			if(IsTerrainWall(numTile) && (TerrainType[numTile] & 0xf0))
				return 1;
		}
	}
	if(IsTerrainWet(GetTileAtXYZ(MapX, MapY, MapZ)) && !IN_VEHICLE)
		return 1;
	return 0;
}

/*check no foes near*/
static C_3200_0160() {
	int si, di;
	int bp_04, bp_02;

	if(MapZ)
		di = 0xff;
	else
		di = 0x3ff;
	bp_04 = (MapX - 0x10) & 0x3f8;
	bp_02 = (MapY - 0x10) & 0x3f8;
	for(si = 0; si < 0x100; si ++) {
		if(
			ObjShapeType[si] != 0 &&
			(GetAlignment(si) == EVIL || GetAlignment(si) == CHAOTIC) &&
			((GetX(si) - bp_04) & di) < 39 &&
			((GetY(si) - bp_02) & di) < 39 &&
			GetZ(si) == MapZ
		) return 1;
	}
	return 0;
}

/*check no one near*/
static C_3200_0226() {
	int si;

	for(si = 0; si < 0x100; si ++) {
		if(
			ObjShapeType[si] != 0 &&
			!IsPlrControl(si) &&
			CLOSE_ENOUGH(5, GetX(si), GetY(si), MapX, MapY) &&
			GetZ(si) == MapZ &&
			GetType(si) != OBJ_1AE
		) return 1;
	}
	return 0;
}

/*check enough room*/
static C_3200_02C4() {
	int si, di;

	for(si = 0; si < 3; si ++) {
		for(di = 0; di < 3; di ++) {
			if(!C_1E0F_000F(Party[Active], MapX + si - 1, MapY + di - 1))
				return 1;
		}
	}
	return 0;
}

static /*C_3200_0309*/HasNightEncounter() {
	int objNum, objType, numMonsters, i, tries;
	int pos, incr, x, y, ret;

	ret = 0;
	switch(OSI_rand(0, 3)) {
		case 0: objType = OBJ_15F; break;
		case 1: objType = OBJ_166; break;
		case 2: objType = OBJ_156; break;
		case 3: objType = OBJ_161; break;
	}
	numMonsters = OSI_rand(1, 8);
	for(i = 0; i < numMonsters; i ++) {
		objNum = EGG_generate(objType, 0, 0, MapZ);
		for(tries = 0; tries < 8; tries ++) {
			pos = OSI_rand(0, 3);
			incr = OSI_rand(0, 10);
			switch(pos) {
				case 0: x = MapX - 6;        y = MapY + incr - 6; break;/*left*/
				case 1: x = MapX + incr - 6; y = MapY - 7; break;/*up*/
				case 2: x = MapX + 7;        y = MapY + incr - 6; break;/*right*/
				case 3: x = MapX + incr - 6; y = MapY + 6; break;/*down*/
			}
			if(C_1E0F_000F(objNum, x, y))
				break;
		}
		if(tries != 8) {
			MoveObj(objNum, x, y, MapZ);
			SetAlignment(objNum, EVIL);
			NPCMode[objNum] = AI_ASSAULT;
			ret = 1;
		} else {
			DeleteObj(objNum);
		}
	}
	return ret;
}

/*restore health*/
C_3200_0467(int bp08, int bp06) {
	int si, di, bp_02;

	si = Party[bp08];
	di = MaxHP(si) - HitPoints[si];
	if(di) {
		if(di == 1)
			di = 2;
		bp_02 = OSI_rand(0, di / 2) + (di / 2);
		if(HitPoints[si] + bp_02 < MaxHP(si))
			HitPoints[si] += bp_02;
		else
			HitPoints[si] = MaxHP(si);
		if(bp06) {
			CON_printf(Names[bp08]);
			CON_printf(/*2D02*/" has healed.\n");
		}
	}
	MAGIC[si] = MaxMagic(si);
}

/*repair hull*/
static C_3200_0514() {
	int si, di;

	si = Party[Active];
	if(HitPoints[si] + 5 >= 100)
		HitPoints[si] = 100;
	else
		HitPoints[si] += 5;
	C_0A33_1355(5);
	CON_printf(/*2D10*/"\nHull Strength: %d%%\n", HitPoints[si]);
}

/*index/orientation
+---+---+---+
|5/0|1/0|6/6|
+---+---+---+
|3/2|   |2/6|
+---+---+---+
|7/2|0/4|4/4|
+---+---+---+*/
char D_2CEA[] = { 0, 0, 1,-1, 1,-1, 1,-1};
char D_2CF2[] = {-1, 1, 0, 0,-1, 1, 1,-1};
char D_2CFA[] = { 4, 0, 6, 2, 4, 0, 6, 2};

/*"rest"*/
C_3200_055D() {
	int i, di;
	struct coord xyz;
	int ch, sleepTime, campFire;
	int someoneCannotMove, cannotMove, bp_10, noMeal, guardNum;
	int bp_0a, guardMode, bp_06, wakeReason, bp_02;

	someoneCannotMove = 1;
	bp_10 = 20;
	guardNum = 0x100;
	wakeReason = 0;
	CON_printf(/*2D26*/"Rest");
	bp_02 = (char)SelectMode;
	if(InCombat) {
		CON_printf(/*2D2B*/"-Not while in Combat!\n");
		return;
	}
	if(IN_VEHICLE && GetType(Party[Active]) != OBJ_19C) {
		CON_printf(/*2D42*/"-Can not be repaired!\n");
		return;
	}
	if(C_3200_00D3()) {
		CON_printf(/*2D59*/"-Only in the wilderness!\n");
		return;
	}
	if(C_3200_0160()) {
		CON_printf(/*2D73*/"-Not while foes are near!\n");
		return;
	}
	if(C_3200_0226()) {
		CON_printf(/*2D8E*/"-Not while others are near!\n");
		return;
	}
	if(D_2CC3 != -1) {
		CON_printf(/*2DAB*/"-Not in solo mode!\n");
		return;
	}
	if(C_3200_02C4()) {
		CON_printf(/*2DBF*/"-Not enough room!\n");
		return;
	}
	if(IN_VEHICLE) {
		C_3200_0514();
		return;
	}
	for(i = 0; i < PartySize; i ++) {
		if(GetType(Party[i]) == OBJ_1AF) {
			CON_printf(/*2DD2*/"-Dismount first!\n");
			return;
		}
	}

	CON_printf(/*2DE4*/"\nHow many hours? ");
	ch = CON_getch();
	if(ch < '1' || ch > '9') {
		CON_printf(/*2DF6*/"0\n");
		return;
	}
	CON_printf(/*2DF9*/"%c\n", ch);
	sleepTime = ch - '0';

	if(PartySize != 1) {
		CON_printf(/*2DFD*/"Who will guard? ");
		MouseMode = 1;
		if(bp_02) {
			SelectRange = 7;
			C_0C9C_1AE5(0);
			SelectMode = 1;
		}
		ch = CON_getch();
		if(!bp_02 && ch >= '1' && ch < PartySize + '0') {
			Selection.obj = Party[ch - '1'];
			ch = CMD_8E;
			Selection.x = GetX(Selection.obj);
		}
		SelectMode = 0;
		OtherAnimations();
		if(ch != CMD_8E || Selection.obj == -1 || Selection.x == -1 || Selection.obj >= 0x100 || !IsPlrControl(Selection.obj)) {
			CON_printf(/*2E0E*/"none\n");
		} else {
			CON_printf((char *)GetObjectString(Selection.obj));
			CON_printf(/*2D0E*/"\n");
			guardNum = Selection.obj;
		}
	}

	bp_06 = CON_mouseOffAt(0, 0, 311, 191);

	SetCoordXYZ(xyz, MapX, MapY, MapZ);
	campFire = AddObj(0, xyz, OBJ_0FD, 1, 1);
	SetLocal(campFire);
	ShowObjects();

	while(someoneCannotMove && bp_10) {
		someoneCannotMove = 0;
		bp_10 --;
		for(i = 0; i < PartySize; i ++) {
			di = Party[i];
			cannotMove = TryMoveTo(di, MapX + D_2CEA[i], MapY + D_2CF2[i]);
			if(!cannotMove)
				C_1E0F_0664(di, D_2CFA[i]);
			if(cannotMove)
				someoneCannotMove = 1;
		}
		ShowObjects();
		OtherAnimations();
	}

	CON_printf(/*2E14*/"\nMealtime!\n");

	/*a bard plays a tune*/
	for(i = 0; i < PartySize; i ++) {
		di = Party[i];
		if(GetType(di) == OBJ_182) {
			SetType(di, OBJ_188);
			C_1E0F_0664(di, D_2CFA[i]);
			CON_printf(Names[i]);
			CON_printf(/*2E20*/" plays a tune.\n");
			break;
		}
	}
	ShowObjects();
	/*check food during melody*/
	i = 0;
	MUS_SongIndex = 0;
	noMeal = -1;
	for(; MUS_SongIndex || (MUS_SongIndex == 0 && i == 0); i ++) {
		if(!(i & 3)) {
			bp_0a = i / 4;
			if(bp_0a < PartySize) {
				CON_printf(Names[bp_0a]);
				if(TryEat(bp_0a)) {
					CON_printf(/*2E30*/" has food.\n");
				} else {
					CON_printf(/*2E3C*/" has no food.\n");
					if(noMeal == -1)
						noMeal = bp_0a;
				}
			}
		}
		D_033E = 1;
		OtherAnimations();
		PaletteAnimation();
		ServeMouse
	}
	if(noMeal == -1)
		noMeal = PartySize;

	if(StatusDisplay == CMD_92)
		C_155D_0CF5();

	/*the bard stops playing*/
	for(i = 0; i < PartySize; i ++) {
		di = Party[i];
		if(GetType(di) == OBJ_188) {
			SetType(di, OBJ_182);
			C_1E0F_0664(di, D_2CFA[i]);
			break;
		}
	}

	for(i = 0; i < PartySize; i ++) {
		if(Party[i] != guardNum) {
			SetTypeUnconscious(Party[i]);
		} else {
			CON_printf(/*2D0E*/"\n");
			CON_printf(Names[i]);
			CON_printf(/*2E4B*/" stands guard while the party rests.\n");
		}
	}
	ShowObjects();
	if(guardNum < 0x100) {
		guardMode = NPCMode[guardNum];
		NPCMode[guardNum] = AI_GRAZE;
	}

	/*night loop - 10 minutes per iteration*/
	for(i = 0; i < sleepTime * 6; i ++) {
		/*"guard"'s ai*/
		if(guardNum < 0x100)
			C_1E0F_3E6A(guardNum);

		C_0A33_1355(10);
		ShowObjects();
		D_033E = 1;
		OtherAnimations();
		PaletteAnimation();
		ServeMouse
		if(
			!(i & 5)/*strange*/ &&
			OSI_rand(1, 64) == 1 &&
			HasNightEncounter()
		) {
			if(guardNum < 0x100) {
				CON_printf(/*2E71*/"\nYou hear something!\n");
				wakeReason = 1;
			} else {
				wakeReason = 2;
			}
			break;
		}
	}

	SetFrame(campFire, 0);
	ShowObjects();

	/*everyone is waking?*/
	for(i = 0; i < PartySize; i ++) {
		if(wakeReason < 2) {
			if(Party[i] != guardNum)
				ObjShapeType[Party[i]] = OrigShapeType[Party[i]];
		} else {
			SetAsleep(Party[i]);
		}
		MovePts[Party[i]] = DEXTE[Party[i]];
	}

	if(guardNum < 0x100)
		NPCMode[guardNum] = guardMode;

	/*could rest?*/
	if(sleepTime >= 4 && NextSleep == 0 && wakeReason == 0) {
		for(i = 0; i < noMeal; i ++) {
			di = Party[i];
			if(di != guardNum)
				C_3200_0467(i, 1);
		}
		NextSleep = 12;
	}

	MapX = GetX(Party[Active]);
	MapY = GetY(Party[Active]);
	ShowObjects();
	C_101C_054C();
	OtherAnimations();
	CON_mouseOn(bp_06);
	if(StatusDisplay == CMD_91)
		StatusDirty ++;
}
