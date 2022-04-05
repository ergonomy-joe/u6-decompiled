/*
 * Ultima VI - The False Prophet
 * Copyright (c) 1990 Origin Systems, Inc. and Richard Gariott
 * reverse-coded by Ergonomy Joe in 2013
 */
/*
	module combat?
*/
#include "u6.h"

#undef abs
#include <math.h>

int D_1968 = 1;
int D_196A = -1;

unsigned char D_196C[][8] = {
	0,1,2,3,4,5,6,7,
	1,1,2,3,4,5,6,7,
	2,2,2,3,4,5,6,7,
	3,3,3,4,5,6,7,7,
	4,4,4,5,6,7,7,8,
	5,5,5,6,7,7,8,8,
	6,6,6,7,7,8,8,8,
	7,7,7,7,8,8,8,8
};

/*for triple-crossbow & magic wind*/
unsigned char D_19AC[][11] = {
/*	0x42,0x52,0x52,0x52,0x52,0x62,0x63,0x63,0x63,0x63,0x64,
	0x41,0x42,0x52,0x52,0x52,0x62,0x63,0x63,0x63,0x64,0x74,
	0x41,0x41,0x42,0x52,0x52,0x62,0x63,0x63,0x64,0x74,0x74,
	0x41,0x41,0x41,0x42,0x52,0x62,0x63,0x64,0x74,0x74,0x74,
	0x41,0x41,0x41,0x41,0x42,0x62,0x64,0x74,0x74,0x74,0x74,
	0x40,0x40,0x40,0x40,0x40,0x00,0x04,0x04,0x04,0x04,0x04,
	0x30,0x30,0x30,0x30,0x20,0x26,0x06,0x05,0x05,0x05,0x05,
	0x30,0x30,0x30,0x20,0x27,0x26,0x16,0x06,0x05,0x05,0x05,
	0x30,0x30,0x20,0x27,0x27,0x26,0x16,0x16,0x06,0x05,0x05,
	0x30,0x20,0x27,0x27,0x27,0x26,0x16,0x16,0x16,0x06,0x05,
	0x20,0x27,0x27,0x27,0x27,0x26,0x16,0x16,0x16,0x16,0x06*/
#define MD(dir_0,dir_1) (((dir_0)<<4)|(dir_1))
	MD(4,2),MD(5,2),MD(5,2),MD(5,2),MD(5,2),MD(6,2),MD(6,3),MD(6,3),MD(6,3),MD(6,3),MD(6,4),
	MD(4,1),MD(4,2),MD(5,2),MD(5,2),MD(5,2),MD(6,2),MD(6,3),MD(6,3),MD(6,3),MD(6,4),MD(7,4),
	MD(4,1),MD(4,1),MD(4,2),MD(5,2),MD(5,2),MD(6,2),MD(6,3),MD(6,3),MD(6,4),MD(7,4),MD(7,4),
	MD(4,1),MD(4,1),MD(4,1),MD(4,2),MD(5,2),MD(6,2),MD(6,3),MD(6,4),MD(7,4),MD(7,4),MD(7,4),
	MD(4,1),MD(4,1),MD(4,1),MD(4,1),MD(4,2),MD(6,2),MD(6,4),MD(7,4),MD(7,4),MD(7,4),MD(7,4),
	MD(4,0),MD(4,0),MD(4,0),MD(4,0),MD(4,0),MD(0,0),MD(0,4),MD(0,4),MD(0,4),MD(0,4),MD(0,4),
	MD(3,0),MD(3,0),MD(3,0),MD(3,0),MD(2,0),MD(2,6),MD(0,6),MD(0,5),MD(0,5),MD(0,5),MD(0,5),
	MD(3,0),MD(3,0),MD(3,0),MD(2,0),MD(2,7),MD(2,6),MD(1,6),MD(0,6),MD(0,5),MD(0,5),MD(0,5),
	MD(3,0),MD(3,0),MD(2,0),MD(2,7),MD(2,7),MD(2,6),MD(1,6),MD(1,6),MD(0,6),MD(0,5),MD(0,5),
	MD(3,0),MD(2,0),MD(2,7),MD(2,7),MD(2,7),MD(2,6),MD(1,6),MD(1,6),MD(1,6),MD(0,6),MD(0,5),
	MD(2,0),MD(2,7),MD(2,7),MD(2,7),MD(2,7),MD(2,6),MD(1,6),MD(1,6),MD(1,6),MD(1,6),MD(0,6)
#undef MD
};

static int D_1A25 = 0;/*equiped weapons max_index*/
static int D_1A27 = 0;/*equiped weapons index*/

/*C_2337_000B*/COMBAT_getCathesus(int objNum, int x, int y) {
	int delta_x, delta_y;

	delta_x = abs(GetX(objNum) - x);
	delta_y = abs(GetY(objNum) - y);
	if(delta_x > delta_y)
		return delta_x;
	else
		return delta_y;
}

/*C_2337_005E*/COMBAT_stopCharm(int objNum) {
	ClrCharmed(objNum);
	SetAlignment(objNum, GetBKAlignment(objNum));
	if(IsPlrControl(objNum))
		SetAlignment(objNum, GOOD);
	if(InCombat)
		NPCMode[objNum] = NPCComMode[objNum];
	else if(D_2CC3 < 0)
		NPCMode[objNum] = AI_FOLLOW;
	else
		NPCMode[objNum] = AI_MOTIONLESS;
}

/*C_2337_00C8*/COMBAT_begin() {
	int si, di;

	InCombat = 1;
	for(si = 0; si < PartySize; si ++) {
		if(!IsCharmed(Party[si]))
			NPCMode[Party[si]] = NPCComMode[Party[si]];
		Leader[Party[si]] = Party[si];
	}
	if(D_2CC3 < 0) {
		for(
			si = 0;
			(Isbis_0016(Party[si]) || IsDraggedUnder(Party[si])) && si < PartySize;
			si ++
		);
	} else {
		si = D_2CC3;
	}
	D_2CC4 = NPCMode[Party[si]];
	NPCMode[Party[si]] = AI_COMMAND;
	di = CON_mouseOffAt(152, 176, 167, 191);
	GR_2D(TIL_19F, 152, 176);
	CON_mouseOn(di);
	MUS_091A(7);
}

/*C_2337_01BC*/COMBAT_breakOff() {
	int si;
	int bp_04, bp_02;

	InCombat =
	D_17B6 = 0;
	if(D_2CC3 < 0)
		bp_02 = AI_FOLLOW;
	else
		bp_02 = AI_MOTIONLESS;
	for(bp_04 = 0; bp_04 < PartySize; bp_04 ++) {
		if(IsDead(Party[bp_04])) {
			NPCMode[Party[bp_04]] = AI_MOTIONLESS;
		} else if(!IsCharmed(Party[bp_04])) {
			NPCMode[Party[bp_04]] = bp_02;
			if(bp_02 == AI_FOLLOW)
				Leader[Party[bp_04]] = bp_04;
		}
	}
	if(D_2CC3 < 0)
		bp_04 = 0;
	else
		bp_04 = D_2CC3;
	D_2CC4 = NPCMode[Party[bp_04]];
	NPCMode[Party[bp_04]] = AI_COMMAND;
	si = CON_mouseOffAt(152, 176, 167, 191);
	GR_2D(TIL_19E, 152, 176);
	CON_mouseOn(si);
	if(MUS_MIndex == 7)
		MUS_09A8();
}

/*C_2337_02DC*/COMBAT_TryTeleport(int objNum, int onScreenNotOk) {
	int bp_0a, x, y, z, new_x, new_y;

	if(objNum < 0)
		return 0;
	x = GetX(objNum);
	y = GetY(objNum);
	z = GetZ(objNum);
	for(bp_0a = 0; bp_0a < 8; bp_0a ++) {
		new_x = OSI_rand(1, 4) + OSI_rand(1, 4) + x - 5;
		new_y = OSI_rand(1, 4) + OSI_rand(1, 4) + y - 5;
		if(
			onScreenNotOk &&
			MapX - 5 <= new_x && new_x <= MapX + 5 &&
			MapY - 5 <= new_y && new_y <= MapY + 5
		) continue;
		if(C_1E0F_000F(objNum, new_x, new_y)) {
			MoveObj(objNum, new_x, new_y, z);
			return 1;
		}
	}
	return 0;
}

/*C_2337_03E7*/GetStr(int objNum) {
	int si = STREN[objNum];
	if(IsCursed(objNum)) {
		if((si -= 3) < 1)
			si = 1;
	}
	return si;
}

/*C_2337_0411*/GetDex(int objNum) {
	int si = DEXTE[objNum];
	if(IsCursed(objNum)) {
		if((si -= 3) < 1)
			si = 1;
	}
	if(SpellFx[14] && !IsPlrControl(objNum))
		si = 1;
	if(IsAsleep(objNum))
		si = 1;
	return si;
}

/*C_2337_0458*/GetInt(int objNum) {
	int si = INTEL[objNum];
	if(IsCursed(objNum)) {
		if((si -= 3) < 1)
			si = 1;
	}
	return si;
}

/*C_2337_0482*/COMBAT_TestIntel(int bp08, int bp06) {
	if(GetInt(bp06) / 2 + 15 - GetInt(bp08) > OSI_rand(1, 30))
		return 1;
	return 0;
}

/*uncalled*/
/*__2337_04BE*/ComputeLevel(int objNum) {
	unsigned lvl, exp;

	exp = ExpPoints[objNum];
	lvl = 1;
	while(exp > 99) {
		lvl ++;
		exp >>= 1;
	}
	return lvl;
}

/*C_2337_04E1*/GetLevel(int objNum) {
	return Level[objNum];
}

/*C_2337_04F4*/AddXP(int objNum, int xp) {
	if(ExpPoints[objNum] < 9999 - xp)
		ExpPoints[objNum] += xp;
	else
		ExpPoints[objNum] = 9999;
}

/*C_2337_0529*/MaxHP(int objNum) {
	int si;
	
	si = Level[objNum] * 30;
	if(si == 0)
		si = 1;
	if(si > 255)
		return 255;
	return si;
}

/*C_2337_0559*/MaxMagic(int objNum) {
	int si, di;

	di = 0;
	si = GetType(objNum);
	if(si == OBJ_19A)
		di = INTEL[objNum] << 1;
	if(si == OBJ_17A)
		di = INTEL[objNum];
	else if(si == OBJ_179 || si == OBJ_182)
		di = INTEL[objNum] >> 1;
	return di;
}

/*object type==>corpse type*/
/*1A29*/int DeadShape[] = {
	OBJ_1AE,TypeFrame(OBJ_086,1),
	OBJ_16F,TypeFrame(OBJ_153,0),
	OBJ_16B,TypeFrame(OBJ_153,0),
	OBJ_173,TypeFrame(OBJ_153,0),
	OBJ_172,TypeFrame(OBJ_153,1),
	OBJ_1A8,TypeFrame(OBJ_154,3),
	OBJ_17C,TypeFrame(OBJ_153,2),
	OBJ_17B,TypeFrame(OBJ_153,2),
	OBJ_183,TypeFrame(OBJ_153,3),
	OBJ_17A,TypeFrame(OBJ_153,3),
	OBJ_17E,TypeFrame(OBJ_153,4),
	OBJ_180,TypeFrame(OBJ_153,5),
	OBJ_179,TypeFrame(OBJ_153,5),
	OBJ_178,TypeFrame(OBJ_153,6),
	OBJ_19A,TypeFrame(OBJ_153,7),
	OBJ_182,TypeFrame(OBJ_153,7),
	OBJ_188,TypeFrame(OBJ_153,7),
	OBJ_17D,TypeFrame(OBJ_153,8),
	OBJ_181,TypeFrame(OBJ_153,8),
	OBJ_187,TypeFrame(OBJ_153,8),
	OBJ_17F,TypeFrame(OBJ_153,8),
	OBJ_170,TypeFrame(OBJ_153,9),
	OBJ_16A,TypeFrame(OBJ_155,3),
	-1
};

/*set to "unconscious" type*/
/*C_2337_05B4*/SetTypeUnconscious(int objNum) {
	int objType, objCls;
	int *pTypUnc;
	int newObj, isDead;

	objType = GetType(objNum);
	isDead = IsDead(objNum);
	if(objType == OBJ_1AF) {
		C_27A1_55F0(objNum);
		objType = GetType(objNum);
	} else if(objType == OBJ_19B || objType >= OBJ_1AA || objType == OBJ_19D || objType == OBJ_19C) {
		OrigShapeType[objNum] = ObjShapeType[objNum];
	}
	objCls = GetMonsterClass(objType);
	for(pTypUnc = DeadShape; *pTypUnc >= 0; pTypUnc ++) {
		if(*pTypUnc ++ == objType)
			break;
	}
	if(*pTypUnc >= 0 && !IsMonster_1000(objCls)) {
		if(objType == OBJ_1AE) {
			if(isDead) {
				DeleteObj(OBJ_getHead(objNum));
				ObjShapeType[objNum] = TypeFrame(*pTypUnc, 0);
			}
		} else {
			ObjShapeType[objNum] = TypeFrame(*pTypUnc, 0);
		}
	} else if(isDead) {
		if(objCls >= 0) {
			if(IsMonster_IN(objCls))
				EGG_generate(OBJ_157, GetX(objNum), GetY(objNum), MapZ);

			if(IsMonster_0800(objCls) || IsMonster_1000(objCls)) {
				DeleteObj(objNum);
				if(IsMonster_1000(objCls) && CLOSE_ENOUGH(5, GetX(objNum), GetY(objNum), MapX, MapY)) {
					C_0A33_09CE(1);
					ShowObjects();
					ScreenFade = 1;
					C_0A33_09CE(1);
				}
			}
		}
		if(objType == OBJ_177)
			C_1184_0BC1();
		if(objType == OBJ_19B)
			DeleteObj(objNum);
	}
	if(isDead) {
		if(
			(objCls >= 0 && IsMonster_2000(objCls)) ||
			objType == OBJ_187 || objType == OBJ_188
		) {
			newObj = AddMapObj(OBJ_152, LOCAL, 0, GetX(objNum), GetY(objNum), MapZ);
			if(newObj >= 0)
				SetFrame(newObj, OSI_rand(0, 2));
		}
	}
	D_17B0 = 1;
}

/*C_2337_0837*/COMBAT_canSee(int objNum_0, int objNum_1) {
	int isVisible_1;

	isVisible_1 = !IsInvisible(objNum_1);

	if(
		IsPlrControl(objNum_0) &&
		!isVisible_1 && objNum_1 < 0x100 && IsPlrControl(objNum_1)
	) isVisible_1 = 1;

	if(objNum_1 < 0x100 && IsDraggedUnder(objNum_1))
		isVisible_1 = 0;

	if(ObjShapeType[objNum_1] == TypeFrame(OBJ_165, 0))
		isVisible_1 = 0;

	if(IsTileIg(TILE_FRAME(objNum_1)))
		isVisible_1 = 0;

	if(GetZ(objNum_0) != GetZ(objNum_1))
		isVisible_1 = 0;

	return isVisible_1;
}

C_2337_08F1(int objNum_param, int x, int y) {
	int objNum_ret;
	int objNum_2, objNum_3;

	objNum_3 = -1;
	/*first "canSee" object*/
	for(objNum_ret = FindLoc(x, y, MapZ); objNum_ret >= 0; objNum_ret = NextLoc()) {
		if(COMBAT_canSee(objNum_param, objNum_ret))
			break;
		if(objNum_3 < 0)
			objNum_3 = objNum_ret;
	}
	/*first "canSee" notDead NPC*/
	for(objNum_2 = objNum_ret; objNum_2 >= 0; objNum_2 = NextLoc()) {
		if(COMBAT_canSee(objNum_param, objNum_2)) {
			if(objNum_2 < 0x100 && !IsDead(objNum_2))
				break;
			continue;
		}
		if(objNum_3 < 0)
			objNum_3 = objNum_2;
	}
	/* */
	if(objNum_2 >= 0)
		objNum_ret = objNum_2;
	else if(objNum_3 >= 0 && objNum_ret < 0)
		objNum_ret = objNum_3;

	return objNum_ret;
}

int D_1A87[] = {
	OBJ_001,OBJ_002,OBJ_003,OBJ_004,OBJ_005,OBJ_006,OBJ_007,OBJ_008,
	OBJ_009,OBJ_00A,OBJ_00B,OBJ_00C,OBJ_00D,OBJ_00E,OBJ_00F,OBJ_010,
	OBJ_011,OBJ_012,OBJ_013,OBJ_014,OBJ_015,OBJ_016,OBJ_017,OBJ_023,
	OBJ_018,OBJ_100
};
unsigned char D_1ABB[] = {
	1,2,3,3,2,2,3,5,
	2,3,3,3,2,2,4,5,
	1,2,3,4,5,7,10,1,
	2,5
};

/*C_2337_0983*/GetArmorPoints(int objNum) {
	int si, di;

	di = 0;
	for(si = 0; si < 26; si ++) {
		if(D_1A87[si] == GetType(objNum)) {
			di = D_1ABB[si];
			break;
		}
	}
	return di;
}

static /*C_2337_09BF*/__GetProtection(int objNum) {
	int si, di;

	si = 0;
	for(di = FindInv(objNum); di >= 0; di = NextInv()) {
		if(GetCoordUse(di) == EQUIP)
			si += GetArmorPoints(di);
	}
	if(IsCursed(objNum))
		si -= 3;
	if(IsProtected(objNum))
		si += 3;

	return si;
}

int D_1AD5[] = {
	OBJ_028,OBJ_02F,OBJ_026,OBJ_053,OBJ_021,OBJ_025,OBJ_024,OBJ_029,
	OBJ_02A,OBJ_039,OBJ_150,OBJ_031,OBJ_032,OBJ_19C,OBJ_05B,OBJ_036,
	OBJ_04F,OBJ_050
};
unsigned char D_1AF9[] = {
	2,2,3,5,4,3,4,5,
	7,7,5,5,7,5,4,7,
	7,7
};

static /*C_2337_0A14*/__GetWeaponRange(int objNum) {
	int si, di;

	di = 1;
	for(si = 0; si < 18; si ++) {
		if(GetType(objNum) == D_1AD5[si]) {
			di = D_1AF9[si];
			break;
		}
	}
	return di;
}

static C_2337_0A4E(int objNum) {
	int i;

	if(NPCMode[objNum] > AI_FOLLOW && NPCMode[objNum] < AI_10 && Leader[objNum] != objNum) {
		i = Leader[objNum];
		if(
			ObjShapeType[i] &&
			!IsDead(i) &&
			!IsAsleep(i) &&
			!IsParalyzed(i) &&
			!IsDraggedUnder(i) &&
			CLOSE_ENOUGH_S(2, GetX(objNum), GetY(objNum), GetX(i), GetY(i))
		) {
			if(SpellFx[14] == 0)
				return 1;
			if(IsPlrControl(i))
				return 1;
		}
	}
	return 0;
}

/*C_2337_0B3F*/COMBAT_Missile(int objNum, int *pX_dst, int *pY_dst, int max_range, int numTile) {
	int di;

	D_196A = objNum;
	di = Missiles(GetX(objNum), GetY(objNum), 1, pX_dst, pY_dst, max_range, numTile, 0);
	D_196A = -1;

	return di;
}

/*range weapon management?*/
static C_2337_0B99(int att_objNum, int *pX_dest, int *pY_dest, int weap_objNum) {
	int typ;
	int bp_14, bp_12, bp_10, range, bp_0c;
	int bp_0a, protTile, numBolts, bp_04, numTile;

	typ = GetType(weap_objNum);
	range = __GetWeaponRange(weap_objNum);

	if(typ == OBJ_029 || typ == OBJ_036)
		protTile = BaseTile[OBJ_037];
	else if(typ == OBJ_02A || typ == OBJ_032)
		protTile = BaseTile[OBJ_038];
	else if(typ == OBJ_04F)
		protTile = TIL_188;
	else if(typ == OBJ_050)
		protTile = TIL_189;
	else if(typ == OBJ_19C)
		protTile = TIL_18F;
	else if(typ == OBJ_021 || (range > 1 && att_objNum == weap_objNum))
		protTile = TIL_18E;
	else if(att_objNum != weap_objNum)
		protTile = TILE_FRAME(weap_objNum);
	else
		protTile = 0;

	if(typ == OBJ_032) {/*triple crossbow?*/
		bp_04 = D_19AC[*pY_dest - GetY(att_objNum) + 5][*pX_dest - GetX(att_objNum) + 5];
		D_EC02[0] = *pX_dest;
		D_EC02[1] = *pX_dest + DirIncrX[bp_04 >> 4];
		D_EC02[2] = *pX_dest + DirIncrX[bp_04 & 0xf];
		D_EC08[0] = *pY_dest;
		D_EC08[1] = *pY_dest + DirIncrY[bp_04 >> 4];
		D_EC08[2] = *pY_dest + DirIncrY[bp_04 & 0xf];
		numBolts = TestObj(att_objNum, OBJ_038);
		if(numBolts > 3)
			numBolts = 3;

		D_196A = att_objNum;
		Missiles(GetX(att_objNum), GetY(att_objNum), numBolts, D_EC02, D_EC08, range, protTile, 0);
		D_196A = -1;

		*pX_dest = D_EC02[0];
		*pY_dest = D_EC08[0];
	} else {
		COMBAT_Missile(att_objNum, pX_dest, pY_dest, range, protTile);
	}
	bp_10 = 0;
	switch(typ) {
		case OBJ_029:
		case OBJ_02A:
		case OBJ_032:
		case OBJ_036:
			if(typ == OBJ_029 || typ == OBJ_036)
				bp_12 = FindInvType(att_objNum, OBJ_037, -1);
			else
				bp_12 = FindInvType(att_objNum, OBJ_038, -1);
			if(typ == OBJ_032)
				numBolts = 3;
			else
				numBolts = 1;
			if(bp_12 >= 0) {
				if(GetQuan(bp_12) > numBolts)
					SubQuan(bp_12, numBolts);
				else
					DeleteObj(bp_12);
				bp_10 = 1;
			}
		break;
		case OBJ_024:
		case OBJ_025:
		case OBJ_026:
			bp_0c = *pX_dest;
			bp_0a = *pY_dest;
			bp_0c = abs(bp_0c - GetX(att_objNum));
			bp_0a = abs(bp_0a - GetY(att_objNum));
			if(bp_0c > 1 || bp_0a > 1) {
				bp_14 = 0;
				for(bp_12 = FindInv(att_objNum); bp_12 >= 0; bp_12 = NextInv()) {
					if(GetCoordUse(bp_12) == INVEN && GetType(bp_12) == typ) {
						bp_14 = 1;
						break;
					}
				}
				if(bp_14 == 0)
					bp_12 = weap_objNum;
				bp_10 = 1;
				if(D_EC9E >= 0 && D_EC9E < 0x100) {
					DeleteObj(bp_12);
				} else {
					numTile = GetTileAtXYZ(D_EBFE[0], D_EBFE[1], MapZ);
					typ = TerrainType[numTile];
					if(
						(
						((typ & TERRAIN_FLAG_01) && (typ & TERRAIN_FLAG_02)) || (numTile >= TIL_0FC && numTile <= TIL_0FF)
						) && FindLoc(D_EBFE[0], D_EBFE[1], MapZ) < 0
					) {
						DeleteObj(bp_12);
					} else {
						MoveObj(bp_12, D_EBFE[0], D_EBFE[1], MapZ);
						SetOwned(bp_12);
					}
					ShowObjects();
				}
			}
		break;
		case OBJ_053:
			bp_0c = *pX_dest;
			bp_0a = *pY_dest;
			bp_0c = abs(bp_0c - GetX(att_objNum));
			bp_0a = abs(bp_0a - GetY(att_objNum));
			bp_10 = 1;
			TakeObj(att_objNum, OBJ_053, 1);
			typ = TerrainType[GetTileAtXYZ(D_EBFE[0], D_EBFE[1], MapZ)];
			if(!(typ & TERRAIN_FLAG_01)) {
				AddMapObj(OBJ_13D, LOCAL, 0, D_EBFE[0], D_EBFE[1], MapZ);
				ShowObjects();
			}
		break;
		case OBJ_05B:
			TakeObj(att_objNum, OBJ_05B, 1);
			if(D_EC9E >= 0 && D_EC9E < 0x100)
				C_1944_0AD0(att_objNum, D_EC9E);
		break;
		case OBJ_04F:
		case OBJ_050:
			if(OSI_rand(1, 100) == 37) {
				DeleteObj(weap_objNum);
				CON_printf(RingVanishedMsg, /*1BC8*/"wand");
				PromptFlag = 1;
			}
		break;
	}
	if(bp_10 && StatusDisplay == CMD_92 && Party[D_04B3] == att_objNum)
		StatusDirty = 1;
	return 1;
}

int D_1B0B[] = {
	OBJ_004,OBJ_021,OBJ_022,OBJ_023,OBJ_024,OBJ_025,OBJ_026,OBJ_027,
	OBJ_028,OBJ_029,OBJ_02A,OBJ_02B,OBJ_02C,OBJ_02D,OBJ_02E,OBJ_02F,
	OBJ_031,OBJ_032,OBJ_030,OBJ_00D,OBJ_0DD,OBJ_053,OBJ_036,OBJ_06D,
	OBJ_064,OBJ_065,OBJ_067,OBJ_068,OBJ_069,OBJ_071,OBJ_072,OBJ_08D,
	OBJ_19C,OBJ_05B,OBJ_04E,OBJ_04F,OBJ_050
};
unsigned char D_1B55[] = {
	 4, 6,  8, 8,10,10, 6,15,
	15,10, 12,15,20,20,20,30,
	 8,12,255, 4,90, 4,20, 2,
	 6, 4,  4, 4, 4, 4, 4, 4,
	30, 0,  4,30,20
};

/*C_2337_10E4*/GetDamagePoints(int bp06) {
	int si, di;
	int objCls;

	di = -1;
	if(bp06 < 0x100)
		objCls = GetMonsterClass(GetType(bp06));
	else
		objCls = -1;
	if(objCls >= 0) {
		di = D_3522_0182[objCls];
	} else {
		for(si = 0; si < 37; si ++)
			if(GetType(bp06) == D_1B0B[si]) {
				di = D_1B55[si];
				break;
			}
	}
	return di;
}

/*C_2337_115F*/CloneObject(int src_objNum, int dst_objNum) {
	ObjStatus[dst_objNum] = ObjStatus[src_objNum];
	if(GetType(src_objNum) != OBJ_177)
		ObjShapeType[dst_objNum] = ObjShapeType[src_objNum];
	Amount[dst_objNum] = Amount[src_objNum];
	NPCStatus[dst_objNum] = NPCStatus[src_objNum];
	STREN[dst_objNum] = STREN[src_objNum];
	DEXTE[dst_objNum] = DEXTE[src_objNum];
	INTEL[dst_objNum] = INTEL[src_objNum];
	MAGIC[dst_objNum] = MAGIC[src_objNum];
	MovePts[dst_objNum] = 0;
	ExpPoints[dst_objNum] = ExpPoints[src_objNum];
	HitPoints[dst_objNum] = HitPoints[src_objNum];
	NPCMode[dst_objNum] = NPCMode[src_objNum];
	NPCComMode[dst_objNum] = NPCComMode[src_objNum];
	SchedIndex[dst_objNum] = SchedIndex[src_objNum];
	Leader[dst_objNum] = Leader[src_objNum];
	NPCFlag[dst_objNum] = NPCFlag[src_objNum];
}

/*C_2337_121F*/COMBAT_showHit(int objNum) {
	if(D_1968) {
		int x = GetX(objNum) - MapX + 5;
		int y = GetY(objNum) - MapY + 5;
		if(x >= 0 && x < 11 && y >= 0 && y < 11) {
			/*display hit?*/
			ShowObjects();
			ShowObject(TIL_101, x, y, 1);
			C_0A33_09CE(1);
			MUS_0065(4, 0);
			ShowObjects();
			D_17B0 = 1;
		}
		if(objNum < 0x100)
			SetSkipSomeTest(objNum);
	}
	if(IsAsleep(objNum)) {
		ClrAsleep(objNum);
		ObjShapeType[objNum] = OrigShapeType[objNum];
		D_17B0 = 1;
	}
	D_1968 = 1;
}

/*C_2337_12F5*/LooseHP(int objNum, int bp06) {
	int di;
	int new_x, new_y, bp_0c, objTyp;
	int bp_08, objCls, exp, bp_02;

	if(bp06 < 0)
		return 0;
	exp = 0;
	objTyp = GetType(objNum);
	if(objTyp == OBJ_1A7)
		return 0;

	bp_02 = TextOn; TextOn = 1;

	if(objNum < 0x100) {
		COMBAT_showHit(objNum);
		if(IsMonster_IM(GetMonsterClass(objTyp))) {
			HitPoints[objNum] = 255;
		} else if(objTyp == OBJ_19E || objTyp == OBJ_19F) {/*Skiff/Raft*/
			bp_0c = Party[0];
			if(PartySize > 1)
				bp_0c = Party[OSI_rand(0, PartySize - 1)];
			LooseHP(bp_0c, bp06);
			COMBAT_displaySeverity(bp_0c);
		} else if(HitPoints[objNum] <= bp06) {/*too many hits?*/
			if(D_04A7 == objNum)
				D_04A7 = -1;
			if(objTyp == OBJ_19C) {
				C_1184_116D(objNum, GetX(objNum), GetY(objNum), 1);
				ObjShapeType[objNum] = TypeFrame(OBJ_19F, 0);
				HitPoints[objNum] = 10;
				D_17B0 = 1;
			} else {
				if(HitPoints[objNum] != 0) {
					if(GetAlignment(objNum) == NEUTRAL)
						SubKarma(5);
					HitPoints[objNum] = 0;
					SetDead(objNum);
					if(Is_ATKPLR(objNum)) {
						objCls = GetMonsterClass(GetType(objNum));
						if(objCls >= 0) {
							exp = (
								/*STR*/D_3522_0082[objCls] +
								/*DEX*/D_3522_00C2[objCls] +
								/*INT*/D_3522_0102[objCls] +
								/*ARM*/D_3522_0142[objCls] +
								/*DMG*/D_3522_0182[objCls] +
								/*HIT*/D_3522_01C2[objCls]
							) >> 2;
						}
					}
					ClrInvisible(objNum);
					SetTypeUnconscious(objNum);
					NPCMode[objNum] = AI_MOTIONLESS;
					if(IsPlrControl(objNum) && Party[0] != objNum) {
						for(di = 1; di <= PartySize; di ++) {
							if(Party[di] == objNum) {
								if(D_2CC3 >= di)
									D_2CC3 --;
								if(Active == di)
									SetPartyMode();
								if(Active > di)
									Active --;
								for( ; di < PartySize; di ++) {
									Party[di] = Party[di + 1];
									strcpy(Names[di], Names[di + 1]);
								}
								PartySize --;
								StatusDirty = 1;
								break;
							}
						}
					}
					C_1184_1B3A(objNum);
					D_17B0 = 1;
				}
				if(Party[Active] == objNum)
					C_1E0F_4B6A();
			}
		} else {
			HitPoints[objNum] -= bp06;
			if(NPCMode[objNum] == AI_SHY)
				NPCMode[objNum] = AI_ASSAULT;
			if(IsMonster_SP(GetMonsterClass(objTyp))) {
				for(di = 0;di < 8; di ++) {
					new_x = GetX(objNum) + OSI_rand(0, 2) - 1;
					new_y = GetY(objNum) + OSI_rand(0, 2) - 1;
					if(C_1E0F_000F(objNum, new_x, new_y)) {
						bp_0c = AddMonster(GetType(objNum), new_x, new_y, MapZ);
						if(bp_0c >= 0) {
							CloneObject(objNum, bp_0c);
							CON_printf(SlimeDividesMsg);
							PromptFlag = 1;
							D_17B0 = 1;
						}
						break;
					}
				}
			}
		}
		if(IsPlrControl(objNum)) {
			if(StatusDisplay == CMD_90 && Party[D_04B3] == objNum) {
				StatusDirty = 1;
			} else if(StatusDisplay == CMD_91) {
				for(di = D_07CC; di < PartySize && di < D_07CC + 5; di ++) {
					if(Party[di] == objNum) {
						bp_08 = CON_mouseOffAt(176, 6, 311, 103);
						CON_setClip(&(D_B6B5[1]));
						CON_gotoxy(14, ((di - D_07CC) << 1) + 2);
						if(IsPoisoned(objNum))
							D_04DF->_fg = D_2A4C;
						else if(HitPoints[objNum] < 10)
							D_04DF->_fg = D_2A4A;
						CON_printf(/*1BCD*/"%3d\n", HitPoints[objNum]);
						D_04DF->_fg = D_2A56;
						CON_setClip(&(D_B6B5[3]));
						CON_mouseOn(bp_08);
					}
				}
			}
		}
	} else if(objTyp == OBJ_16E) {
		HitPoints[GetQual(objNum)] = 0;
		SetDead(GetQual(objNum));
		ClrInvisible(GetQual(objNum));
		NPCMode[GetQual(objNum)] = AI_MOTIONLESS;
		CON_printf(VineKilledMsg);
		PromptFlag = 1;
	} else if(!QuanType(ObjShapeType[objNum]) && GetQuan(objNum) != 0) {
		COMBAT_showHit(objNum);
		if(
			!(objTyp >= OBJ_129 && objTyp <= OBJ_12C && GetFrame(objNum) >= 12) &&
			!(objTyp == OBJ_062 && GetFrame(objNum) == 3)
		) {
			if(GetQuan(objNum) > bp06) {
				SubQuan(objNum, bp06);
			} else {
				CON_printf(BrokenMsg, GetObjectString(objNum));
				PromptFlag = 1;
				if(FindInvType(objNum, OBJ_151, SPELL_16) != -1)
					C_27A1_28A3(objNum);
				if(objTyp == OBJ_07B) {
					MUS_0065(15, 0);
					SetFrame(objNum, 2);
					D_17B0 = 1;
					SubKarma(10);
				} else {
					DeleteObj(objNum);
					D_17B0 = 1;
				}
			}
		}
	}

	TextOn = bp_02;

	return exp;
}

static /*C_2337_18CF*/__AttackTest(int objNum_att, int objNum_def, int objNum_weapon) {
	int objTyp, str_dex;
	int dex, objCls, strOverDex;

	if(objNum_def >= 0x100)
		return 1;
	objTyp = GetType(objNum_weapon);
	if(objTyp == OBJ_030)
		return 1;
	if(objNum_weapon == objNum_att) {
		objCls = GetMonsterClass(objTyp);
		strOverDex = IsMonster_0020(objCls);
	} else {
		strOverDex = (objTyp == OBJ_004 || objTyp == OBJ_022 || objTyp == OBJ_027 || objTyp == OBJ_02C);
	}
	if(strOverDex)
		str_dex = GetStr(objNum_att);
	else
		str_dex = GetDex(objNum_att);
	dex = GetDex(objNum_def);

	return OSI_rand(1, 30) >= (dex + 30 - str_dex) / 2;
}

/*C_2337_198F*/COMBAT_displaySeverity(int objNum) {
	int di;

	if(GetType(objNum) == OBJ_1A7 || HitPoints[objNum] == 0)
		return;
	di = (HitPoints[objNum] << 2) / MaxHP(objNum);
	if(di < 4) {
		CON_printf(/*1BD2*/"`%s ", GetObjectString(objNum));
		PromptFlag = 1;
	}
	if(di == 0) {/*critical*/
		CON_printf(CriticalMsg);
		if(
			GetInt(objNum) >= 5 &&
			NPCMode[objNum] != AI_BERSEK &&
			NPCMode[objNum] != AI_IMMOBILE &&
			NPCMode[objNum] != AI_COMMAND &&
			NPCMode[objNum] != AI_FRONT &&
			NPCMode[objNum] != AI_FOLLOW &&
			NPCMode[objNum] != AI_MOTIONLESS
		) NPCMode[objNum] = AI_RETREAT;
	} else if(di == 1) {/*heavily*/
		CON_printf(HeavilyMsg);
	} else if(di == 2) {/*lightly*/
		CON_printf(LightlyMsg);
	} else if(di == 3) {/*barely*/
		CON_printf(BarelyMsg);
	}
	if(di != 0 && di < 4)
		CON_printf(WoundedMsg);
}

/*dissolvable objects*/
int D_1B7A[] = {
	OBJ_002,OBJ_003,OBJ_004,OBJ_005,OBJ_007,OBJ_00A,OBJ_00B,OBJ_00C,
	OBJ_00D,OBJ_00F,OBJ_013,OBJ_014,OBJ_015,OBJ_016,OBJ_023,OBJ_026,
	OBJ_02B,OBJ_071,OBJ_072,
	0
};
/*robbable objects*/
int D_1BA2[] = {
	OBJ_080,OBJ_081,OBJ_082,OBJ_083,OBJ_084,OBJ_085,OBJ_087,OBJ_0D1,
	OBJ_0D2,OBJ_0D3,
	0
};

/*pick {dissolv/robb}able object?*/
static C_2337_1A99(int bp08, int bp06/*disolve/rob flag*/) {
	int si, di;
	int *bp_02;

	di = -1;
	for(si = FindInv(bp08); si >= 0; si = NextInv()) {
		bp_02 = bp06?D_1BA2:D_1B7A;
		while(*bp_02) {
			if(GetType(si) == *(bp_02++)) {
				if(di < 0 || OSI_rand(0, 1))
					di = si;
				break;
			}
		}
	}
	return di;
}

/*The avatar did a bad action and "everyone"
run after him*/
C_2337_1B0E() {
	int objNum;

	SubKarma(5);
	for(objNum = 0; objNum < 0x100; objNum ++) {
		if(!ObjShapeType[objNum])
			continue;
		if(GetZ(objNum) != MapZ)
			continue;
		if(IsDead(objNum) || IsPlrControl(objNum) || GetAlignment(objNum) != NEUTRAL)
			continue;
		/*guard*/
		if(GetType(objNum) == OBJ_17E) {
			if(COMBAT_getCathesus(objNum, MapX, MapY) < 32)
				NPCMode[objNum] = AI_ARREST;
			continue;
		}

		if(NPCMode[objNum] == AI_IMMOBILE || NPCMode[objNum] == AI_MOTIONLESS)
			continue;
		if(Isbis_0016(objNum) || IsDraggedUnder(objNum))
			continue;
		if(COMBAT_getCathesus(objNum, MapX, MapY) >= 6)
			continue;
		if(NPCMode[objNum] >= AI_SCHEDULE) {
			if(NPCMode[objNum] <= AI_86)
				continue;
			if(GetType(objNum) == OBJ_188)
				ObjShapeType[objNum] = OrigShapeType[objNum];
			NPCMode[objNum] = AI_VIGILANTE;
		} else {
			NPCMode[objNum] = AI_FEAR;
		}
	}
}

/*attack morality/alignement check?*/
C_2337_1C1E(int si/*victim?*/, int bp08/*attacker?*/, int bp06/*hit points?*/) {
	int objCls;

	if(si >= 0x100)
		return;
	if(GetAlignment(si) != NEUTRAL)
		return;
	if(IsPlrControl(si))
		return;
	objCls = GetMonsterClass(GetType(si));
	if(NPCMode[bp08] == AI_COMMAND && objCls >= 0 && IsMonster_4000(objCls) && bp06 > 0) {
		/*killed a human ? ...yells for help...*/
		CON_printf(D_356A_044A, GetObjectString(si));
		C_2337_1B0E();
		PromptFlag = 1;
	}
	if(NPCMode[si] >= AI_SCHEDULE) {
		if(NPCMode[bp08] == AI_COMMAND) {
			if(GetType(si) == OBJ_188)
				ObjShapeType[si] = OrigShapeType[si];
			NPCMode[si] = AI_VIGILANTE;
		}
		return;
	}
	if(GetAlignment(bp08) == GOOD)
		SetAlignment(si, CHAOTIC);
	else
		SetAlignment(si, GOOD);
	if(si >= 0xe0)
		NPCMode[si] = AI_ASSAULT;
}

static /*C_2337_1D20*/__DoHit(int objNum, int si/*bp08*/, int hitPts) {
	char *pObjStr;
	char bp_22[26];
	int armorPts, objCls, bp_04, bp_02;

#if 0
	CON_printf("%03x hits %03x (%03d)\n", objNum, si, hitPts);
#endif
	if(si < 0)
		return;
	pObjStr = GetObjectString(si);
	if(ObjShapeType[objNum] == TypeFrame(OBJ_165, 0)) {
		SetFrame(objNum, 1);
		D_17B0 = 1;
	}

	if(hitPts == -1)
		hitPts = 1;
	else if(hitPts > 1 && hitPts < 255)
		hitPts = OSI_rand(1, hitPts);

	if(si < 0x100)
		armorPts = __GetProtection(si);
	else
		armorPts = 0;
	if(armorPts > 0 && hitPts < 255)
		hitPts -= OSI_rand(1, armorPts);

	if(hitPts <= 0) {
		COMBAT_showHit(si);
		CON_printf(GrazedMsg, pObjStr);
		PromptFlag = 1;
	} else {
		if(
			si < 0x100 &&
			NPCMode[si] > AI_FOLLOW && NPCMode[si] < AI_10 &&
			CLOSE_ENOUGH_S(2, GetX(si), GetY(si), GetX(objNum), GetY(objNum))
		) Leader[si] = objNum;
		AddXP(objNum, LooseHP(si, hitPts));
	}
	if(si >= 0x100)
		return;

	if(si == Selection.obj)
		C_2337_1C1E(si, objNum, hitPts);
	bp_02 = GetType(si);
	if(
		HitPoints[si] == 0 &&
		bp_02 != OBJ_1A7 &&
		bp_02 != OBJ_19E &&
		bp_02 != OBJ_19F &&
		bp_02 != OBJ_19C
	) {
		CON_printf(KiledMsg, pObjStr);
		PromptFlag = 1;
		if(StatusDisplay == CMD_90 && Party[D_04B3] == objNum)
			StatusDirty = 1;
		return;
	}
	objCls = GetMonsterClass(GetType(objNum));
	if(IsMonster_DR(objCls)) {
		MUS_0065(9, 0);
		CON_printf(DraggedUnderMsg, pObjStr);
		SetDraggedUnder(si);
		PromptFlag = 1;
		D_17B0 = 1;
	}
	if(IsMonster_AC(objCls)) {
		bp_04 = C_2337_1A99(si, 0);
		if(bp_04 >= 0) {
			MUS_0065(11, 0);
			strcpy(bp_22, GetObjectString(si));
			CON_printf(DissolvedMsg, bp_22, GetObjectString(bp_04));
			PromptFlag = 1;
			DeleteObj(bp_04);
		}
	}
	if(IsMonster_SF(objCls)) {
		bp_04 = C_2337_1A99(si, 1);
		if(bp_04 >= 0) {
			MUS_0065(12, 0);
			CON_printf(StolenMsg, GetObjectString(si));
			PromptFlag = 1;
			DeleteObj(bp_04);
		}
	}
	if(IsMonster_0040(objCls) && OSI_rand(0, 3) == 0) {
		CON_printf(PoisonedMsg, pObjStr);
		PromptFlag = 1;
		SetPoisoned(si);
		if(
			StatusDisplay == CMD_91 ||
			(StatusDisplay == CMD_90 && Party[D_04B3] == si)
		) StatusDirty = 1;
	}
	if(hitPts > 0)
		COMBAT_displaySeverity(si);
}

/*not very proud of that name ...*/
/*C_2337_205C*/COMBAT_getHead(int objNum) {
	int di;
	int objType, bp_04, bp_02;

	if(objNum < 0)
		return -1;
	objType = GetType(objNum);
	if(objType >= OBJ_1AA || objType == OBJ_19B || objType == OBJ_19C || objType == OBJ_19D) {
		if(objNum >= 0x100)
			objNum = OBJ_getHead(objNum);
	} else if(objType == OBJ_1A9) {
		bp_04 = GetX(objNum) - DirIncrX[GetFrame(objNum) >> 2];
		bp_02 = GetY(objNum) - DirIncrY[GetFrame(objNum) >> 2];
		for(di = FindLoc(bp_04, bp_02, MapZ); di >= 0; di = NextLoc()) {
			if(di < 0x100 && GetType(di) == OBJ_176) {
				objNum = di;
				break;
			}
		}
	}
	return objNum;
}

static /*C_2337_214D*/__TryHit(int att_objNum, int x, int y, int weap_objNum) {
	int weap_typ, opp_objNum;
	int bp_16, bp_14, damage, didHit;
	int delta_x, delta_y, some_weapon_flag, numBolts, bp_06, bp_04, objTyp;

#if 0
	CON_printf("%04x att. %03x,%03x w.%04x\n", att_objNum, x, y, weap_objNum);
#endif
	if(COMBAT_range(x, y, GetX(att_objNum), GetY(att_objNum)) > __GetWeaponRange(weap_objNum))
		return 2;

	objTyp = GetType(weap_objNum);
	if((objTyp == OBJ_029 || objTyp == OBJ_036) && FindInvType(att_objNum, OBJ_037, -1) < 0)
		return 0;
	if((objTyp == OBJ_02A || objTyp == OBJ_032) && FindInvType(att_objNum, OBJ_038, -1) < 0)
		return 0;

	delta_x = x - GetX(att_objNum);
	delta_y = y - GetY(att_objNum);
	if(abs(delta_x) > abs(delta_y))
		bp_16 = (delta_x < 0)?6:2;
	else
		bp_16 = (delta_y < 0)?0:4;
	objTyp = GetType(att_objNum);
	if(objTyp == OBJ_19C) {
		if(((GetDirection(att_objNum) & 6) ^ bp_16) & 2)
			bp_16 = GetDirection(att_objNum);
		else
			bp_16 ^= 2;
	}
	SetDirection(att_objNum, bp_16);
	C_1E0F_0664(att_objNum, bp_16);
	if(objTyp >= OBJ_170 && objTyp <= OBJ_174) {
		bp_14 = (GetFrame(att_objNum) & 0xc) + 2;
		SetFrame(att_objNum, bp_14);
	}

	C_0A33_0D06();
	D_EC9E = Selection.obj;
	weap_typ = GetType(weap_objNum);
	some_weapon_flag = 0;
	if(weap_typ != OBJ_028 && weap_typ != OBJ_02F) {
		if(delta_x > 1 || delta_x < -1 || delta_y > 1 || delta_y < -1)
			some_weapon_flag = 1;
		if(
			weap_typ == OBJ_021 || weap_typ == OBJ_025 || weap_typ == OBJ_029 ||
			weap_typ == OBJ_02A || weap_typ == OBJ_031 || weap_typ == OBJ_053 ||
			weap_typ == OBJ_032 || weap_typ == OBJ_05B || weap_typ == OBJ_04F ||
			weap_typ == OBJ_050
		) some_weapon_flag = 1;
	}
	if(some_weapon_flag && C_2337_0A4E(att_objNum))
		return 1;
	if(weap_typ == OBJ_032) {
		numBolts = TestObj(att_objNum, OBJ_038);
		if(numBolts > 3)
			numBolts = 3;
	}

	damage = GetDamagePoints(weap_objNum);
	if(damage < 0)
		damage = 1;

	didHit = -1;
	opp_objNum = COMBAT_getHead(D_EC9E);
	/*spell*/
	if(weap_typ == OBJ_150 || weap_typ == OBJ_039) {
		if(GetQual(weap_objNum) == SPELL_81 && objTyp == OBJ_175) {
			x = GetX(att_objNum);
			y = GetY(att_objNum);
			MoveObj(att_objNum, MapX, MapY, MapZ);
			if(COMBAT_TryTeleport(att_objNum, 0)) {
				CON_printf(TeleportMsg);
				PromptFlag = 1;
			} else {
				MoveObj(att_objNum, x, y, MapZ);
			}
			return 0;
		}
		if(weap_typ == OBJ_039) {
			bp_04 = GetQual(weap_objNum) / 0x10 + 1;
			if(MAGIC[att_objNum] < bp_04 || Level[att_objNum] < bp_04)
				return 0;
			MAGIC[att_objNum] -= bp_04;
		}
		bp_06 = Active;
		Active = 0x10;
		Party[0x10] = att_objNum;
		Selection.obj = opp_objNum;
		Selection.x = GetX(opp_objNum);
		Selection.y = GetY(opp_objNum);
		TextOn = 0;
		C_1944_43D5(GetQual(weap_objNum), 1);/*SPELL_cast*/
		TextOn = 1;
		if(GetQual(weap_objNum) == SPELL_50 && SpellResult == -2)
			CON_printf(/*1BD7*/"%s is charmed.\n", GetObjectString(Selection.obj));
		else if(GetQual(weap_objNum) == SPELL_45 && SpellResult == 0)
			CON_printf(/*1BE7*/"%s is paralyzed.\n", GetObjectString(Selection.obj));
		Active = bp_06;
		return 0;
	}
	/* */
	if(D_EC9E >= 0 && opp_objNum < 0x100) {
		didHit = __AttackTest(att_objNum, opp_objNum, weap_objNum);
		if(!didHit && some_weapon_flag) {
			didHit = -1;
			x += OSI_rand(0, 2) - 1;
			y += OSI_rand(0, 2) - 1;
			D_EC9E = C_2337_08F1(att_objNum, x, y);
			opp_objNum = COMBAT_getHead(D_EC9E);
		}
	}
	if(some_weapon_flag && !C_2337_0B99(att_objNum, &x, &y, weap_objNum))
		didHit = -1;
	opp_objNum = COMBAT_getHead(D_EC9E);
	if(!some_weapon_flag && COMBAT_getCathesus(att_objNum, MapX, MapY) < 6)
		MUS_0065(17, 0);
	if(didHit == -1) {
		didHit = 1;
		if(D_EC9E >= 0 && opp_objNum < 0x100)
			didHit = __AttackTest(att_objNum, opp_objNum, weap_objNum);
	}
	/*triple crossbow*/
	if(weap_typ == OBJ_032) {
		for(bp_14 = 0; bp_14 < numBolts; bp_14 ++) {
			if(bp_14 > 0) {
				D_EC9E = C_2337_08F1(att_objNum, D_EC02[bp_14], D_EC08[bp_14]);
				opp_objNum = COMBAT_getHead(D_EC9E);
				didHit = __AttackTest(att_objNum, opp_objNum, weap_objNum);
			}
			if(didHit && D_EC9E >= 0 && opp_objNum != att_objNum)
				__DoHit(att_objNum, opp_objNum, damage);
		}
		return 0;
	}

	if(didHit && D_EC9E >= 0 && opp_objNum != att_objNum) {
		if(
			IsMonster_0080(GetMonsterClass(GetType(opp_objNum))) &&
			weap_typ != OBJ_030 && weap_typ != OBJ_032 && weap_typ != OBJ_036
		) damage = (damage + 1) >> 1;
		if(weap_typ != OBJ_05B)
			__DoHit(att_objNum, opp_objNum, damage);
		if(weap_typ == OBJ_030) {
			CON_printf(GlassSwordMsg);
			DeleteObj(weap_objNum);
			PromptFlag = 1;
		}
	}
	/*boomerang returns*/
	if(weap_typ == OBJ_031)
		Missiles(GetX(att_objNum), GetY(att_objNum), 1, &x, &y, 100, BaseTile[OBJ_031], 1);

	return 0;
}

/*reset equiped weapon list*/
static C_2337_27C5() {
	int si, di;

	di = Party[Active];
	D_1A25 =
	D_1A27 = 0;
	for(si = FindInv(di); si >= 0 && D_1A25 < 8; si = NextInv()) {
		if(GetCoordUse(si) == EQUIP && GetDamagePoints(si) >= 0) {
			D_EC0E[D_1A25] = si;
			D_1A25 ++;
		}
	}
	if(D_1A25 == 0) {
		if(GetType(di) == OBJ_19C)
			CON_printf(ShipCannonMsg);
		else
			CON_printf(BareHandsMsg);
	} else
		CON_printf(WithMsg, GetObjectString(D_EC0E[0]));
}

/*next equiped weapon list*/
static C_2337_2875() {
	int si, di;
	int bp_04, bp_02;

	si = Party[Active];
	if(D_1A25 == 0)
		bp_04 = si;
	else
		bp_04 = D_EC0E[D_1A27];
	di = GetType(bp_04);
	if((di == OBJ_029 || di == OBJ_036) && FindInvType(si, OBJ_037, -1) < 0) {
		CON_printf(OutOfArrowsMsg);
	} else if((di == OBJ_02A || di == OBJ_032) && FindInvType(si, OBJ_038, -1) < 0) {
		CON_printf(OutOfBoltsMsg);
	} else {
		bp_02 = 0;
		if(Selection.obj >= 0) {
			if(Selection.obj == si)
				CON_printf(D_356A_00A3);
			else
				CON_printf(/*1BF9*/"%s.\n", GetObjectString(Selection.obj));
		} else {
			if(D_B6DF)
				CON_printf(NothingMsg);
			else
				CON_printf(/*1BF9*/"%s.\n", GetTileString(GetTileAtXYZ(Selection.x, Selection.y, MapZ)));
		}
		if(Selection.obj != si) {
			CON_printf(/*1BCD+3*/"\n");
			D_04A9 = 1;
			if(Selection.obj < 0)
				Selection.obj = C_2337_08F1(si, Selection.x, Selection.y);
			bp_02 = __TryHit(si, Selection.x, Selection.y, bp_04);
			if(bp_02 == 1) {
				MUS_0065(12, 0);
				CON_printf(InterferenceMsg, GetObjectString(Leader[si]));
			} else if(bp_02 == 2) {
				CON_printf(OutOfRangeMsg);
				D_04A7 = -1;
				MUS_0065(12, 0);
			}
		}
	}
	D_1A27 ++;
	if(D_1A27 < D_1A25) {
		SelectMode = 1;
		SelectRange = __GetWeaponRange(D_EC0E[D_1A27]);
		if(COMBAT_range(AimX, AimY, 5, 5) > SelectRange) {
			AimX =
			AimY = 5;
		}
		CON_printf(AttackWithMsg, GetObjectString(D_EC0E[D_1A27]));
	} else {
		MouseMode = 0;
		SelectMode = 0;
		C_0C9C_01FB();
	}
}

/*"attack"*/
/*C_2337_2AA9*/COMBAT_attack() {
	int ch, weapon_objNum;

	CON_printf(AttackMsg);
	if(IN_VEHICLE && GetType(Party[Active]) != OBJ_19C) {
		CON_printf(/*1BFE*/"-");
		CON_printf(NotOnShipMsg);
		return;
	}
	CON_printf(/*1BD2+3*/" ");
	MouseMode = 1;
	D_04C2 = CMD_81;
	C_0C9C_01FB();
	C_2337_27C5();
	if(D_1A25 == 0)
		weapon_objNum = Party[Active];
	else
		weapon_objNum = D_EC0E[D_1A27];
	SelectRange = __GetWeaponRange(weapon_objNum);
	if(COMBAT_range(AimX, AimY, 5, 5) > SelectRange) {
		AimX =
		AimY = 5;
	}
	ch = CON_getch();
	D_04A7 = Selection.obj;
	if(ch == CMD_8E && (Selection.obj < 0 || GetCoordUse(Selection.obj) == LOCXYZ)) {
		do {
			C_2337_2875();
			if(MouseMode == 1) {
				RefreshStatus();
				ch = CON_getch();
				if(ch != CMD_8E) {
					CON_printf(WhatMsg);
					break;
				}
			}
		} while(MouseMode == 1);
	} else {
		CON_printf(WhatMsg);
	}
	SubMov(Party[Active], 10);
	MouseMode = 0;
	SelectMode = 0;
	C_0C9C_01FB();
}

/*C_2337_2BEE*/COMBAT_range(int bp0c, int bp0a, int bp08, int bp06) {
	int delta_x, delta_y;
	int bp_02;

	delta_x = abs(bp0c - bp08);
	delta_y = abs(bp0a - bp06);
	if(delta_x < 8 && delta_y < 8)
		bp_02 = D_196C[delta_x][delta_y];
	else
		bp_02 = 9;
	return bp_02;
}

/*C_2337_2C43*/static __PickWeapon(int att_objNum, int opp_objNum) {
	int ret, i;
	int opp_x, opp_y, att_x, att_y;
	int range, dmg, max_dmg, bp_02;

	ret = att_objNum;
	max_dmg = 0;
	opp_x = GetX(opp_objNum);
	opp_y = GetY(opp_objNum);
	att_x = GetX(ret);
	att_y = GetY(ret);
	range = COMBAT_range(opp_x, opp_y, att_x, att_y);
	for(i = FindInv(att_objNum); i >= 0; i = NextInv()) {
		bp_02 = GetType(i);
		if(GetCoordUse(i) == EQUIP) {
			if(bp_02 == OBJ_039 || bp_02 == OBJ_150) {
				if(
					OSI_rand(0, 3) == 0 &&
					SpellFx[13] == 0 &&
					OSI_rand(0, GetQual(i)) < 16
				) {
					ret = i;
					break;
				}
			}
		}
		if(GetCoordUse(i) == EQUIP) {
			dmg = GetDamagePoints(i);
			if(dmg > max_dmg && __GetWeaponRange(i) >= range) {
				max_dmg = dmg;
				ret = i;
			}
		}
	}
	return ret;
}

/*C_2337_2D6E*/COMBAT_pickOpponent(int objNum) {
	int i;
	int x, y, opp_x, opp_y, objAln;
	int dst2, ret, min_dst2;

	ret = -1;
	objAln = GetAlignment(objNum);
	x = GetX(objNum);
	y = GetY(objNum);
	min_dst2 = 0x7fff;
	for(i = 0; i < 0x100; i++) {
		if(ObjShapeType[i] != 0 && !IsDead(i) && i != objNum && COMBAT_canSee(objNum, i)) {
			if(
				NPCMode[objNum] != AI_SHY &&
				NPCMode[objNum] != AI_9A &&
				NPCMode[objNum] != AI_FEAR &&
				NPCMode[objNum] != AI_RETREAT &&
				NPCMode[objNum] != AI_BRAWL
			) {
				if(objAln == NEUTRAL && (NPCMode[objNum] != AI_VIGILANTE || GetAlignment(i) != GOOD))
					continue;
				if(objAln == CHAOTIC && GetAlignment(i) == CHAOTIC)
					continue;
				if(GetAlignment(i) == NEUTRAL)
					continue;
				if(objAln == GOOD && !Is_ATKPLR(i))
					continue;
				if(objAln == EVIL && !Is_ATKMON(i))
					continue;
			}
			opp_x = GetX(i);
			opp_y = GetY(i);
			if(COMBAT_getCathesus(objNum, opp_x, opp_y) <= 8) {
				if(NPCMode[i] != AI_RETREAT || CLOSE_ENOUGH(5, opp_x, opp_y, MapX, MapY)) {
						dst2 = (opp_x - x) * (opp_x - x) + (opp_y - y) * (opp_y - y);
						if(dst2 < min_dst2 || (dst2 == min_dst2 && OSI_rand(0, 1))) {
							min_dst2 = dst2;
							ret = i;
						}
				}
			}
		}
	}
	return ret;
}

/*C_2337_2F3E*/COMBAT_AI_Assault(int objNum) {
	int bp_0a, opp_x, opp_y, x, y, bp_02;

	Selection.obj = COMBAT_pickOpponent(objNum);
	bp_0a = __PickWeapon(objNum, Selection.obj);
	if(Selection.obj >= 0) {
		opp_x = GetX(Selection.obj);
		opp_y = GetY(Selection.obj);
		x = GetX(objNum);
		y = GetY(objNum);
		bp_02 = __GetWeaponRange(bp_0a);
		if(
			CLOSE_ENOUGH_S(8, opp_x, opp_y, x, y) &&
			D_196C[abs(opp_x - x)][abs(opp_y - y)] <= bp_02
		) {
			if(COMBAT_Missile(objNum, &opp_x, &opp_y, bp_02, 0)) {
				__TryHit(objNum, opp_x, opp_y, bp_0a);
				SubMov(objNum, 10);
				return;
			}
			if(OSI_rand(0, 1)) {
				opp_x = x - (GetY(Selection.obj) - y);
				opp_y = GetX(Selection.obj) - x + y;
			} else {
				opp_x = (GetY(Selection.obj) - y) + x;
				opp_y = y - (GetX(Selection.obj) - x);
			}
/*C_30D2:*/
			TryMoveTo(objNum, opp_x, opp_y);
			/*return;*/
		} else
/*C_30E0*/
		if(NPCMode[objNum] == AI_FRONT) {
/*C_30EA:*/
			SubMov(objNum, 5);
		} else {
			/*goto C_30D2;*/
			TryMoveTo(objNum, opp_x, opp_y);
		}
	} else {
/*C_30F6*/
		if(NPCMode[objNum] == AI_FRONT)
			/*goto C_30EA;*/
			SubMov(objNum, 5);
		else
/*C_3100*/
			C_1E0F_37DB(objNum);
	}
}

static C_2337_310E(int objNum) {
	int di;
	int bp_24, bp_22, bp_20;
	int bp_1e, bp_1c, bp_1a, bp_18, bp_16, bp_14, bp_12;
	int bp_10, bp_0e, x, y, bp_08, bp_06, bp_04, bp_02;

	if((EnemiesNum == 0 || D_EBF7 == 0) && !IsPlrControl(objNum)) {
		SubMov(objNum, 5);
		return 1;
	}
	x = GetX(objNum);
	y = GetY(objNum);
	if(IsPlrControl(objNum)) {
		bp_06 = GetX(Party[0]);
		bp_04 = GetY(Party[0]);
		di = EnemiesGravityX - bp_06;
		bp_1a = EnemiesGravityY - bp_04;
	} else {
		bp_06 = PartyGravityX;
		bp_04 = PartyGravityY;
		di = GetX(D_EBF7) - bp_06;
		bp_1a = GetY(D_EBF7) - bp_04;
	}
	bp_1e = (x - bp_06) * bp_1a - (y - bp_04) * di;
	bp_24 = (bp_1e > 0);
	if(bp_1e == 0)
		bp_24 = OSI_rand(0, 1);
	bp_14 = (bp_1a > 0)?1:-1;
	bp_12 = (di > 0)?-1:1;
	if(!bp_24) {
		bp_14 = -bp_14;
		bp_12 = -bp_12;
	}
	if(!IsPlrControl(objNum))
		bp_22 = D_EBF7;
	else
		bp_22 = Party[0];
	bp_20 = (GetX(bp_22) - bp_06) * di + (GetY(bp_22) - bp_04) * bp_1a;
	if(Party[0] == bp_22)
		bp_20 += abs(di) + abs(bp_1a);
	bp_1c = di * di + bp_1a * bp_1a;
	if(bp_1c == 0)
		bp_1c = 1;
	bp_18 = (bp_20 * di) / bp_1c + bp_06;
	bp_16 = (bp_20 * bp_1a) / bp_1c + bp_04;
	D_17B2 = 0;
	do {
		if(
			bp_18 < AreaX || AreaX + 39 < bp_18 ||
			bp_16 < AreaY || AreaY + 39 < bp_16 ||
			(x == bp_18 && y == bp_16)
		) {
			D_17B2 = 1;
			return 1;
		}
		bp_08 = (C_1184_0B3D(0, bp_18, bp_16) >= 0);
		if(bp_08) {
			bp_10 = (bp_18 + bp_14 - bp_06) * di + (bp_16 - bp_04) * bp_1a;
			bp_0e = (bp_18 - bp_06) * di + (bp_16 + bp_12 - bp_04) * bp_1a;
			if(abs(bp_10 - bp_20) < abs(bp_0e - bp_20))
				bp_18 += bp_14;
			else
				bp_16 += bp_12;
		}
	} while(bp_08);
	D_17B2 = 1;
	bp_02 = MovePts[objNum];
	if(!TryMoveTo(objNum, bp_18, bp_16)) {
		MovePts[objNum] = bp_02;
	} else {
		if(!IsPlrControl(objNum))
			return 0;
		if(GetX(objNum) == bp_18 && GetY(objNum) == bp_16)
			return 0;
		if(EnemiesNum == 0)
			MovePts[objNum] = bp_02;
		if(TryMoveTo(objNum, bp_18, bp_16))
			return 0;
	}
	COMBAT_AI_Assault(objNum);
	return 0;
}

/*C_2337_3464*/COMBAT_AI_Front(int si/*bp06*/) {
	if(!C_2337_310E(si))
		return;
	if(!IsPlrControl(si)) {
		if(COMBAT_getCathesus(si, PartyGravityX, PartyGravityY) < 8) {
			if(si == D_EBF7 && OSI_rand(0, 7) == 0) {
				TryMoveTo(si, PartyGravityX, PartyGravityY);
				return;
			}
		} else {
			SubMov(si, 5);
			return;
		}
	}
	COMBAT_AI_Assault(si);
}

/*C_2337_34C9*/COMBAT_AI_Rear(int si/*bp06*/) {
	int di;
	int bp_16, bp_14, bp_12, bp_10, bp_0e;
	int bp_0c, bp_0a, bp_08, bp_06, bp_04, bp_02;

	bp_0c = 0;
	if(IsPlrControl(si)) {
		if(EnemiesNum == 0) {
			TryMoveTo(si, MapX, MapY);
			return;
		}
		bp_04 = D_EBE9;
		bp_02 = D_EBF5;
		bp_08 = PartyGravityX;
		bp_06 = PartyGravityY;
		di = EnemiesGravityX - PartyGravityX;
		bp_14 = EnemiesGravityY - PartyGravityY;
	} else {
		if(EnemiesNum == 0) {
			SubMov(si, 5);
			return;
		}
		bp_04 = D_EBF9;
		bp_02 = D_EBF1;
		if(bp_04 < 0)
			bp_04 = si;
		if(bp_02 < 0)
			bp_02 = si;
		bp_08 = EnemiesGravityX;
		bp_06 = EnemiesGravityY;
		di = PartyGravityX - EnemiesGravityX;
		bp_14 = PartyGravityY - EnemiesGravityY;
	}
	bp_10 = 0x7fff;
	bp_0e = GetAlignment(si);
	for(bp_16 = 0; bp_16 < 0x100; bp_16 ++) {
		if(
			!IsDead(bp_16) &&
			NPCMode[bp_16] == AI_FRONT &&
			GetAlignment(bp_16) == bp_0e
		) {
			bp_12 = (GetX(bp_16) - bp_08) * di + (GetY(bp_16) - bp_06) * bp_14;
			if(bp_12 < bp_10)
				bp_10 = bp_12;
		}
	}
	bp_12 = (GetX(si) - bp_08) * di + (GetY(si) - bp_06) * bp_14;
	bp_0a = MovePts[si];
	if(IsPlrControl(si) && COMBAT_getCathesus(si, MapX, MapY) > 3) {
		bp_0c = !TryMoveTo(si, MapX, MapY);
	} else if(bp_12 >= bp_10) {
		bp_0c = !TryMoveTo(si, GetX(si) - di, GetY(si) - bp_14);
	} else {
		bp_12 = (GetX(si) - bp_08) * bp_14 - (GetY(si) - bp_06) * di;
		if((GetX(bp_04) - bp_08) * bp_14 - (GetY(bp_04) - bp_06) * di < bp_12) {
			bp_0c = !TryMoveTo(si, GetX(si) - bp_14, GetY(si) + di);
		} else if((GetX(bp_02) - bp_08) * bp_14 - (GetY(bp_02) - bp_06) * di > bp_12) {
			bp_0c = !TryMoveTo(si, GetX(si) + bp_14, GetY(si) - di);
		} else {
			bp_0c = 1;
		}
	}
	if(bp_0c) {
		MovePts[si] = bp_0a;
		COMBAT_AI_Assault(si);
	}
}

/*C_2337_37F2*/COMBAT_AI_Flank(int objNum) {
	int i;
	int bp_28, bp_26;
	int bp_24, bp_22, bp_20, bp_1e, bp_1c;
	int bp_1a, bp_18, bp_16, bp_14;
	int objAln, bp_10, bp_0e, bp_0c, bp_0a;
	int bp_08, delta_x, delta_y, bp_02;

	if(
		EnemiesNum == 0 ||
		(!IsPlrControl(objNum) && COMBAT_getCathesus(objNum, MapX, MapY) > 7)
	) {
		bp_02 = MovePts[objNum];
		if(IsPlrControl(objNum) && COMBAT_getCathesus(objNum, MapX, MapY) > 2)
			TryMoveTo(objNum, MapX, MapY);
		MovePts[objNum] = bp_02;
		SubMov(objNum, 5);
		return;
	}
	objAln = GetAlignment(objNum);
	bp_24 = GetX(objNum);
	bp_22 = GetY(objNum);
	bp_28 = EnemiesGravityX - PartyGravityX;
	bp_26 = EnemiesGravityY - PartyGravityY;
	bp_20 = (bp_24 - PartyGravityX) * bp_26 - (bp_22 - PartyGravityY) * bp_28;
	bp_1e = (bp_20 > 0);
	if(bp_20 == 0)
		bp_1e = OSI_rand(0, 1);
	if(bp_1e) {
		bp_10 = bp_26;
		bp_0e = - bp_28;
	} else {
		bp_10 = -bp_26;
		bp_0e = bp_28;
		bp_20 = -bp_20;
	}
	bp_1a = 0x8000;
	bp_18 = -1;
	for(i = 0; i < 0x100; i ++) {
		if(
			ObjShapeType[i] &&
			!IsDead(i) &&
			GetAlignment(i) != objAln &&
			COMBAT_canSee(objNum, i)
		) {
			if(objAln == GOOD && !Is_ATKPLR(i))
				continue;
			if(objAln == EVIL && !Is_ATKMON(i))
				continue;
			bp_16 = GetX(i);
			bp_14 = GetY(i);
			if(NPCMode[i] == AI_RETREAT || NPCMode[i] == AI_SHY || NPCMode[i] == AI_9A) {
				if(
					(MapX - 5) > bp_16 || (MapX + 5) < bp_16 ||
					(MapY - 5) > bp_14 || (MapY + 5) < bp_14
				) continue;
			}
			if(
				CLOSE_ENOUGH(7, bp_16, bp_14, MapX, MapY) ||
				CLOSE_ENOUGH(5, bp_16, bp_14, bp_24, bp_22)
			) {
				bp_1c = (bp_16 - PartyGravityX) * bp_26 - (bp_14 - PartyGravityY) * bp_28;
				if(bp_1e == 0)
					bp_1c = -bp_1c;
				if(bp_1c > bp_1a) {
					bp_1a = bp_1c;
					bp_18 = i;
				}
			}
		}
	}
	if(bp_18 >= 0) {
		Selection.obj = bp_18;
		bp_28 = GetX(bp_18);
		bp_26 = GetY(bp_18);
		bp_0c = 0;
		bp_0a = __PickWeapon(objNum, bp_18);
		delta_x = abs(bp_28 - bp_24);
		delta_y = abs(bp_26 - bp_22);
		bp_08 = __GetWeaponRange(bp_0a);
		if(delta_x < 8 && delta_y < 8 && D_196C[delta_x][delta_y] <= bp_08) {
			if(COMBAT_Missile(objNum, &bp_28, &bp_26, bp_08, 0)) {
				__TryHit(objNum, bp_28, bp_26, bp_0a);
				SubMov(objNum, 10);
			} else {
				if(OSI_rand(0, 1)) {
					bp_28 = bp_24 - (GetY(Selection.obj) - bp_22);
					bp_26 = bp_22 + (GetX(Selection.obj) - bp_24);
					bp_0c = 1;
				} else {
					bp_28 = bp_24 + (GetY(Selection.obj) - bp_22);
					bp_26 = bp_22 - (GetX(Selection.obj) - bp_24);
					bp_0c = 1;
				}
			}
		} else {
			if(bp_10 > 0)
				bp_28 ++;
			else if(bp_10 < 0)
				bp_28 --;
			if(bp_0e > 0)
				bp_26 ++;
			else if(bp_0e < 0)
				bp_26 --;
			bp_0c = 1;
		}
		if(bp_0c) {
			bp_02 = MovePts[objNum];
			if(!TryMoveTo(objNum, bp_28, bp_26)) {
				MovePts[objNum] = bp_02;
				COMBAT_AI_Assault(objNum);
			}
		}
	} else {
		C_1E0F_37DB(objNum);
	}
}

/*C_2337_3C3A*/COMBAT_AI_Bersek(int objNum) {
	int si;
	int bp_1c, bp_1a, bp_18, x, y, bp_12, bp_10;
	int bp_0e, bp_0c, bp_0a, delta_x, delta_y, bp_04, bp_02;

	bp_1a = -1;
	bp_04 = 0;
	bp_18 = GetAlignment(objNum);
	x = GetX(objNum);
	y = GetY(objNum);
	bp_10 = 0;
	for(si = 0; si < 0x100; si ++) {
		if(
			ObjShapeType[si] &&
			!IsDead(si) &&
			si != objNum &&
			COMBAT_getCathesus(si, x, y) <= 8 &&
			COMBAT_canSee(objNum, si) &&
			(bp_18 != GOOD || GetAlignment(si) == EVIL) &&
			(bp_18 != EVIL || GetAlignment(si) == GOOD) &&
			(bp_18 != CHAOTIC || (GetAlignment(si) != CHAOTIC && GetAlignment(si) != NEUTRAL))
		) {
			bp_12 = STREN[si] + DEXTE[si] + INTEL[si];
			if(bp_12 > bp_10) {
				bp_10 = bp_12;
				bp_1a = si;
			}
		}
	}
	if(bp_1a >= 0) {
		bp_0e = GetX(bp_1a);
		bp_0c = GetY(bp_1a);
		delta_x = abs(bp_0e - x);
		delta_y = abs(bp_0c - y);
		bp_1c = __PickWeapon(objNum, bp_1a);
		bp_0a = __GetWeaponRange(bp_1c);
		if(delta_x < 8 && delta_y < 8 && D_196C[delta_x][delta_y] <= bp_0a) {
			Selection.obj = bp_1a;
			if(COMBAT_Missile(objNum, &bp_0e, &bp_0c, bp_0a, 0)) {
				__TryHit(objNum, bp_0e, bp_0c, bp_1c);
				SubMov(objNum, 10);
			} else {
				if(OSI_rand(0, 1)) {
					bp_0e = x - (GetY(Selection.obj) - y);
					bp_0c = y + (GetX(Selection.obj) - x);
					bp_04 = 1;
				} else {
					bp_0e = x + (GetY(Selection.obj) - y);
					bp_0c = y - (GetX(Selection.obj) - x);
					bp_04 = 1;
				}
			}
		} else {
			bp_04 = 1;
		}
		if(bp_04) {
			bp_02 = MovePts[objNum];
			if(!TryMoveTo(objNum, bp_0e, bp_0c)) {
				MovePts[objNum] = bp_02;
				COMBAT_AI_Assault(objNum);
			}
		}
	} else {
		COMBAT_AI_Assault(objNum);
	}
	SubMov(objNum, 5);
}

/*C_2337_3ED2*/COMBAT_AI_Retreat(int objNum) {
	int oppObjNum;
	int delta_x, delta_y, bp_04, bp_02;

	if(COMBAT_getCathesus(objNum, MapX, MapY) > 7) {
		MovePts[objNum] = 0;
		return;
	}
	oppObjNum = COMBAT_pickOpponent(objNum);
	if(oppObjNum < 0) {
		C_1E0F_37DB(objNum);
		return;
	}
	delta_x = GetX(oppObjNum) - GetX(objNum);
	delta_y = GetY(oppObjNum) - GetY(objNum);
	bp_04 = (NPCMode[objNum] == AI_RETREAT)?8:3;
	if(CLOSE_ENOUGH_S(bp_04, delta_x, delta_y, 0, 0)) {
		bp_02 = MovePts[objNum];
		if(!TryMoveTo(objNum, GetX(objNum) - delta_x, GetY(objNum) - delta_y)) {
			MovePts[objNum] = bp_02;
			if(NPCMode[objNum] != AI_9A && NPCMode[objNum] != AI_SHY)
				COMBAT_AI_Assault(objNum);
		}
	} else if(GetAlignment(objNum) == GOOD && NPCMode[objNum] != AI_SHY && NPCMode[objNum] != AI_9A) {
		TryMoveTo(objNum, PartyGravityX, PartyGravityY);
	} else {
		C_1E0F_37DB(objNum);
	}
	if(NPCMode[objNum] == AI_RETREAT && OSI_rand(0, 3) == 0) {
		if(MaxHP(objNum) > HitPoints[objNum])
			HitPoints[objNum] ++;
		if(HitPoints[objNum] * 4 / MaxHP(objNum) > 0)
			NPCMode[objNum] = NPCComMode[objNum];
	}
}

/*C_2337_407F*/COMBAT_AI_Like(int objNum) {
	int i;
	int x, y, bp_02;

	bp_02 = 0;
	x = GetX(objNum);
	y = GetY(objNum);
	for(i = 0; i < PartySize; i ++) {
		if(CLOSE_ENOUGH_S(3, GetX(Party[i]), GetY(Party[i]), x, y)) {
			bp_02 = 1;
			break;
		}
	}
	if(bp_02 && OSI_rand(0, 1) == 0) {
		TryMoveTo(objNum, PartyGravityX, PartyGravityY);

		return;
	}
	if(OSI_rand(0, 1)) {
		if(OSI_rand(0, 1))
			x += OSI_rand(0, 1)?10:-10;
		else
			y += OSI_rand(0, 1)?10:-10;
		TryMoveTo(objNum, x, y);
		return;
	}
	SubMov(objNum, 5);
}

/*C_2337_41B7*/COMBAT_AI_Unfriendly(int objNum) {
	int i;
	int x, y, bp_02;

	bp_02 = 0;
	x = GetX(objNum);
	y = GetY(objNum);
	for(i = 0; i < PartySize; i ++) {
		if(CLOSE_ENOUGH_S(3, GetX(Party[i]), GetY(Party[i]), x, y)) {
			bp_02 = 1;
			break;
		}
	}
	if(bp_02 && OSI_rand(0, 7) == 0) {
		NPCMode[objNum] = AI_ASSAULT;
		COMBAT_AI_Assault(objNum);
		return;
	}
	if(OSI_rand(0, 1)) {
		if(OSI_rand(0, 1))
			x += OSI_rand(0, 1)?10:-10;
		else
			y += OSI_rand(0, 1)?10:-10;
		TryMoveTo(objNum, x, y);
		return;
	}
	SubMov(objNum, 5);
}

/*C_2337_42EE*/COMBAT_AI_Immobile(int objNum) {
	int i;
	int objAln, opp_x, opp_y;

	objAln = GetAlignment(objNum);
	if(objAln != NEUTRAL) {
		for(i = 0; i < 16; i ++) {
			opp_x = GetX(objNum) + OSI_rand(0, 8) - 4;
			opp_y = GetY(objNum) + OSI_rand(0, 8) - 4;
			Selection.obj = C_2337_08F1(objNum, opp_x, opp_y);
			if(
				Selection.obj >= 0 && Selection.obj < 0x100 && COMBAT_canSee(objNum, Selection.obj) &&
				GetAlignment(Selection.obj) != objAln && GetAlignment(Selection.obj) != NEUTRAL
			) {
				__TryHit(objNum, opp_x, opp_y, __PickWeapon(objNum, Selection.obj));
				SubMov(objNum, 10);
				return;
			}
		}
	}
	SubMov(objNum, 5);
}

unsigned char D_1BB8[][2] = {
	{0x01,0x03},
	{0x01,0x04},
	{0x05,0x00},
	{0x04,0x00},
	{0x01,0x02},
	{0x01,0x05},
	{0x02,0x00},
	{0x03,0x00}
};

static /*C_2337_43D2*/__TryGrowVine(int objNum, int dir) {
	int bp_0a, x, y, prev_dir, bp_02;

	dir &= 6;
	x = GetX(objNum);
	y = GetY(objNum);
	prev_dir = GetDirection(objNum);
	if((dir ^ prev_dir) == 4)
		return 0;
	bp_0a = TryStraightMove(objNum, dir, 1);
	if(bp_0a == 0)
		return 0;
	bp_02 = AddMapObj(OBJ_16E, LOCAL, QualQuan(objNum, 0), x, y, MapZ);
	if(bp_02 >= 0) {
		SetFrame(bp_02, D_1BB8[prev_dir][dir >> 1]);
		SetFrame(objNum, D_1BB8[dir][OSI_rand(0, 3)]);
		return 1;
	}
	MoveObj(objNum, x, y, MapZ);
	SetDirection(objNum, prev_dir);

	return 0;
}

/*C_2337_44F6*/COMBAT_AI_Tangle(int objNum) {
	int dir;
	int opp_x, opp_y, tangle_x, tangle_y;

	Selection.obj = COMBAT_pickOpponent(objNum);
	if(Selection.obj >= 0) {
		opp_x = GetX(Selection.obj);
		opp_y = GetY(Selection.obj);
		tangle_x = GetX(objNum);
		tangle_y = GetY(objNum);
		if(CLOSE_ENOUGH_S(2, opp_x, opp_y, tangle_x, tangle_y) && OSI_rand(0, 1)) {
			__TryHit(objNum, opp_x, opp_y, objNum);
			SubMov(objNum, 10);
			return;
		}
		if(CLOSE_ENOUGH_S(5, opp_x, opp_y, tangle_x, tangle_y) && OSI_rand(0, 3) == 0) {
			opp_x -= tangle_x;
			opp_y -= tangle_y;
			if(abs(opp_x) > abs(opp_y)) {
				dir = (opp_x > 0)?2:6;
				if(!__TryGrowVine(objNum, dir)) {
					dir = (opp_y > 0)?4:0;
					__TryGrowVine(objNum, dir);
					return;
				}
			} else {
				dir = (opp_y > 0)?4:0;
				if(!__TryGrowVine(objNum, dir)) {
					dir = (opp_x > 0)?2:6;
					__TryGrowVine(objNum, dir);
					return;
				}
			}
		}
	}
	if(OSI_rand(0, 3) == 0) {
		dir = OSI_rand(0, 3) << 1;
		if(dir == GetDirection(objNum))
			dir ^= 4;
		__TryGrowVine(objNum, dir);
	} else {
		MovePts[objNum] = 0;
	}
}
