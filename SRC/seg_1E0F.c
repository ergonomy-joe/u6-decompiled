/*
 * Ultima VI - The False Prophet
 * Copyright (c) 1990 Origin Systems, Inc. and Richard Gariott
 * reverse-coded by Ergonomy Joe in 2013
 */
/*
	NPCTracker module?
*/
#include "u6.h"

/*red gate X*/
int D_171C[] = {
	0x383,0x3A7,0x1B3,0x1F7,0x093,
	0x397,0x044,0x133,0x0BC,0x09F,
	0x2E3,    0,    0,    0,0x0E3,
	0x017,0x080,0x06C,0x39B,0x127,
	0x04B,0x147,0x183,0x33F,0x29B
};
/*red gate Y*/
int D_174E[] = {
	0x1F3,0x106,0x18B,0x166,0x373,
	0x3A6,0x02D,0x160,0x02D,0x3AE,
	0x2BB,    0,    0,    0,0x083,
	0x016,0x056,0x0DD,0x36C,0x026,
	0x1FB,0x336,0x313,0x0A6,0x043
};
/*red gate Z*/
unsigned char D_1780[] = {
	    0,    0,    0,    0,    0,
		0,    5,    0,    5,    0,
	    0,    0,    0,    0,    0,
		1,    5,    5,    0,    0,
	    0,    0,    0,    0,    0
};

int D_1799[] = {OBJ_0FC,OBJ_0A3,OBJ_0E4,OBJ_1A3};
/*17A1*/int AllowNPCTeleport = 0;
int D_17A3 = 0;
static int D_17A5 = 0;
static int D_17A7 = 0;
int D_17A9 = 0;
int D_17AB = 0;
unsigned char D_17AD = 0;
int D_17AE = 0;
int D_17B0 = 0;
int D_17B2 = 1;
int D_17B4 = 0;
int D_17B6 = 0;

/*
 -3  -2  -1   0   1   2   3
+---+---+---+---+---+---+---+
|   |   |   | 0 |   |   |   | 0
+---+---+---+---+---+---+---+
|   |   | 1 |10 | 2 |   |   | 1
+---+---+---+---+---+---+---+
|   | 4 |   | 3 |   | 5 |   | 2
+---+---+---+---+---+---+---+
| 8 |   | 6 |   | 7 |   | 9 | 3
+---+---+---+---+---+---+---+
*/
char D_17B8[] = { 0,-1, 1, 0,-2, 2,-1, 1,-3, 3, 0};
char D_17C3[] = { 0, 1, 1, 2, 2, 2, 3, 3, 3, 3, 1};

/*is terrain praticable?*/
C_1E0F_000F(int objNum, int x, int y) {
	int i, di;
	int terrainFlags, bp_1a;
	int objTyp, retVal, flies, swims, bp_10, ethereal;
	int objCls, bp_0a, keepFind, skiffOrRaft, bp_04, is2x2;

	retVal = 0;
	D_17A9 = 0;
	is2x2 = ((TileFlag[TILE_FRAME(objNum)] & (TILE_FLAG1_80|TILE_FLAG1_40)) == (TILE_FLAG1_80|TILE_FLAG1_40));
	bp_04 = Party[Active];
	objTyp = GetType(objNum);
	objCls = GetMonsterClass(objTyp);
	flies = IsMonster_0100(objCls);
	skiffOrRaft = (objTyp == OBJ_19E || objTyp == OBJ_19F);
	swims = (IsMonster_0200(objCls) || skiffOrRaft);
	ethereal = IsMonster_0400(objCls);
	bp_10 = ((!swims && !flies) || IsMonster_AM(objCls));
	terrainFlags = TerrainType[GetTileAtXYZ(x, y, MapZ)];
	if(objTyp == OBJ_168) {
		retVal = terrainFlags & TERRAIN_FLAG_08;
	} else {
		if(
			(bp_10 && !(terrainFlags & TERRAIN_FLAG_02)) ||
			(swims && (terrainFlags & TERRAIN_FLAG_02) && (terrainFlags & TERRAIN_FLAG_01)) ||
			(skiffOrRaft && (terrainFlags & TERRAIN_FLAG_01)) ||
			(flies && !(terrainFlags & TERRAIN_FLAG_04)) ||
			ethereal
		) retVal = 1;
		if(terrainFlags & TERRAIN_FLAG_08) {
			D_17A9 = 1;
			if(D_17B4)
				return 0;
		}
	}
/*0198*/
	keepFind = 1;
	for(i = FindLoc(x, y, MapZ); i >= 0; i = NextLoc()) {
		if(i == objNum)
			continue;
		di = GetType(i);
		/*not on sacred quest?*/
		if(di == OBJ_1A0 && VarInt['Q' - 0x37] == 0) {
			if(D_17AD) {
				CON_printf(D_356A_0382);
				D_17AD = 0;
				PromptFlag = 1;
			}
			keepFind =
			retVal = 0;
			break;
		}
		/* */
		if(
			(objTyp >= OBJ_1AA || objTyp == OBJ_19C) &&
			i >= 0x100 && di == objTyp
		) {
			bp_1a = TerrainType[TILE_FRAME(objNum)];
			if(
				(bp_1a & TERRAIN_FLAG_80) && GetY(objNum) > y ||
				(bp_1a & TERRAIN_FLAG_40) && GetX(objNum) < x ||
				(bp_1a & TERRAIN_FLAG_20) && GetY(objNum) < y ||
				(bp_1a & TERRAIN_FLAG_10) && GetX(objNum) > x
			) continue;
		}

		if(
			objTyp == OBJ_162 &&
			(di == OBJ_0D5 || di == OBJ_11A || di == OBJ_136)
		) {
			retVal = 1;
			continue;
		}

		bp_0a = TILE_FRAME(i) - D_0658;
		if(is2x2 && D_0658 == 0 && (TileFlag[bp_0a] & (TILE_FLAG1_80|TILE_FLAG1_40)))
			return 0;
		if(bp_10 && IsTileBr(bp_0a)) {
			retVal = 1;
			if(!IsTileIg(bp_0a))
				break;
		}

		bp_1a = TerrainType[bp_0a];
		if(bp_1a & TERRAIN_FLAG_08) {
			D_17A9 = 1;
			if(D_17B4)
				return 0;
		}

		if(objTyp == OBJ_19D && GetType(i) == OBJ_19D) {
			if(GetQual(i) == GetQual(objNum) + 1) {
				keepFind =
				retVal = 0;
			}
			goto c_04ed;
		}
		if(bp_10) {
			if(!(bp_1a & TERRAIN_FLAG_02))
				goto c_04ed;
		}
		if(swims) {
			if((bp_1a & TERRAIN_FLAG_02) && (bp_1a & TERRAIN_FLAG_01))
				goto c_04ed;
			if(IsTileIg(bp_0a))
				goto c_04ed;
		}
		if(skiffOrRaft) {
			if(bp_1a & TERRAIN_FLAG_01)
				goto c_04ed;
			if(IsTileIg(bp_0a))
				goto c_04ed;
		}
		if(flies) {
			if(!(bp_1a & TERRAIN_FLAG_04)) {
				if(i >= 0x100)
					goto c_04ed;
				if(!IsMonster_0100(GetMonsterClass(di)))
					goto c_04ed;
			}
		}
		if(ethereal) {
			if(i >= 0x100)
				goto c_04ed;
		}

		if(IsPlrControl(objNum)) {
			if(
				i != bp_04 &&
				D_17B2 &&
				i < 0x100 &&
				IsPlrControl(i) &&
				!(Isbis_0016(i) || IsDraggedUnder(i))
			) continue;
		} else if(objNum < 0x100) {
			if(
				(di >= OBJ_129 && di <= OBJ_12C && GetFrame(i) < 8) ||
				di == OBJ_116 || di == OBJ_118
			) {
				if(IsMonster_4000(objCls))
					goto c_04ed;
			}
		}
/*c_04e5:*/
		keepFind =
		retVal = 0;

c_04ed:
		if(i < 0x100 && i) {
			if(
				(di != OBJ_164 && di != OBJ_162 && di != OBJ_157 && di != OBJ_167 && di != OBJ_165) ||
				di == objTyp
			) {
				keepFind =
				retVal = 0;
				break;
			}
		}
		if(bp_1a & TERRAIN_FLAG_04) {
			if(objTyp == OBJ_16A || objTyp == OBJ_1A8 || objTyp == OBJ_19B || objTyp == OBJ_1A9) {
				keepFind =
				retVal = 0;
			}
		}

		if(!keepFind)
			break;
	}/*for(... */

	return retVal;
}

static /*C_1E0F_056E*/IsOutOfArea(int objNum) {
	return !(
		GetZ(objNum) == MapZ &&
		((GetX(objNum) - AreaX) & D_05CA) <= (AREA_W-1) &&
		((GetY(objNum) - AreaY) & D_05CA) <= (AREA_H-1)
	);
}

C_1E0F_05D8(int bp08, int bp06) {
	int si;

	if(IsTerrainImpass(GetTileAtXYZ(bp08, bp06, MapZ)))
		return 1;
	for(si = FindLoc(bp08, bp06, MapZ); si >= 0; si = NextLoc()) {
		if(si >= 0x100 || !IsPlrControl(si)) {
			if(IsTerrainImpass(TILE_FRAME(si) - D_0658))
				return 1;
		}
	}
	return 0;
}

/*dragon related*/
unsigned char D_17CE[] = {
	0x08,0x20,0x10,0x18,
	0x1A,0x0A,0x22,0x12,
	0x14,0x1C,0x0C,0x24,
	0x26,0x16,0x1E,0x0E
};
unsigned char D_17DE[] = {TERRAIN_FLAG_20,TERRAIN_FLAG_10,TERRAIN_FLAG_80,TERRAIN_FLAG_40};

#define MACRO_A(dir,frame) {                        \
	if(!((dir) & 1))                                \
		(frame) = (dir) >> 1;                       \
	else if(((((dir) >> 1) - (frame) + 1) & 3) > 1) \
		(frame) = ((frame) + 2) & 3;                \
}

/*change object direction?*/
C_1E0F_0664(int objNum, int dir) {
	int objFrm, dirFram;
	int bp_0e, objTyp, tmpObj, isOnChair, bp_06;
	int cur_x, cur_y;

	isOnChair = 0;
	if(!IsOutOfArea(objNum)) {
		for(tmpObj = FindLoc(GetX(objNum), GetY(objNum), GetZ(objNum)); tmpObj >= 0; tmpObj = NextLoc()) {
			bp_06 = GetType(tmpObj);
			if(bp_06 == OBJ_0FC || (bp_06 == OBJ_147 && GetFrame(tmpObj) - D_0658 == 2)) {
				isOnChair = 1;
				break;
			}
		}
	}
	objTyp = GetType(objNum);
	objFrm = GetFrame(objNum);
	if(
		(objTyp >= OBJ_178 && objTyp <= OBJ_183) ||
		(objTyp >= OBJ_199 && objTyp <= OBJ_19A)
	) {
		if(isOnChair) {
			objFrm = 3;
			if(bp_06 == OBJ_0FC)
				dirFram = GetFrame(tmpObj);
			else
				dirFram = 2;
		} else {
			dirFram = objFrm >> 2;
			MACRO_A(dir, dirFram);
			switch(objFrm & 3) {
				case 0: ClrWalking(objNum); objFrm = 1; break;
				case 1: objFrm = IsWalking(objNum)?0:2; break;
				case 2: SetWalking(objNum); objFrm = 1; break;
				default: objFrm = 1;
			}
		}
		objFrm = objFrm + (dirFram << 2);
		SetFrame(objNum, objFrm);
	} else if(objTyp == OBJ_16A) {
		dirFram = objFrm / 12;
		MACRO_A(dir, dirFram);
		switch(objFrm % 12) {
			case 3/*0*4+3*/: ClrWalking(objNum); objFrm = 7; break;
			case 7/*1*4+3*/: objFrm = IsWalking(objNum)?3:11; break;
			case 11/*2*4+3*/: SetWalking(objNum); objFrm = 7; break;
			default: objFrm = 7;
		}
		objFrm = objFrm + dirFram * 12;
		SetFrame(objNum, objFrm);
	} else if(objTyp == OBJ_164) {
		objFrm = OSI_rand(0, 2);
		SetFrame(objNum, objFrm);
	} else if(objTyp == OBJ_16B) {
		dirFram = objFrm / 3;
		MACRO_A(dir, dirFram);
		switch(objFrm % 3) {
			case 0: ClrWalking(objNum); objFrm = 1; break;
			case 1: objFrm = IsWalking(objNum)?0:2; break;
			case 2: SetWalking(objNum); objFrm = 1; break;
			default: objFrm =1;
		}
		objFrm = objFrm + dirFram * 3;
		SetFrame(objNum, objFrm);
	} else if(
		objTyp == OBJ_15C ||
		objTyp == OBJ_15D ||
		objTyp == OBJ_15E ||
		objTyp == OBJ_156 ||
		objTyp == OBJ_166 ||
		objTyp == OBJ_169 ||
		objTyp >= OBJ_1AA ||
		objTyp == OBJ_15F ||
		objTyp == OBJ_15A ||
		(objTyp >= OBJ_16F && objTyp <= OBJ_174) ||
		objTyp == OBJ_188
	) {
		dirFram = objFrm >> 1;
		if(objTyp < OBJ_1AA || D_17A3) {
			if(dir & 1) {
				cur_x = GetX(objNum);
				cur_y = GetY(objNum);
				if(C_1E0F_05D8(cur_x, cur_y - DirIncrY[dir]))
					dir = (dir & 4) + 2;
				else if(C_1E0F_05D8(cur_x - DirIncrX[dir], cur_y))
					dir = (dir + 1) & 4;
			}
			MACRO_A(dir, dirFram);
			if(!(objFrm & 1))
				objFrm = 1;
			else
				objFrm = 0;
		}
		objFrm = (objFrm & 1) + (dirFram << 1);
		if(objTyp >= OBJ_1AA && D_17A3 && (tmpObj = OBJ_getHead(objNum)) >= 0) {
			if(GetFrame(tmpObj) & 8)
				SetFrame(tmpObj, (objFrm & 7) + 8);
			else
				SetFrame(tmpObj, (objFrm & 7) ^ 4);
			MoveObj(tmpObj, GetX(objNum) - DirIncrX[dirFram << 1], GetY(objNum) - DirIncrY[dirFram << 1], MapZ);
		}
		SetFrame(objNum, objFrm);
	} else if(objTyp == OBJ_19C && D_17A3) {
		dirFram = (objFrm & 6) >> 1;
		MACRO_A(dir, dirFram);
		objFrm = 9 + (dirFram << 1);
		cur_x = GetX(objNum);
		cur_y = GetY(objNum);
		dir = GetFrame(objNum) & 6;
		for(tmpObj = FindLoc(cur_x + DirIncrX[dir], cur_y + DirIncrY[dir], MapZ); tmpObj >= 0; tmpObj = NextLoc()) {
			if(
				tmpObj >= 0x100 &&
				GetType(tmpObj) == OBJ_19C &&
				GetFrame(tmpObj) < 8 &&
				TerrainType[TILE_FRAME(tmpObj)] & D_17DE[dir >> 1]
			) {
				SetFrame(tmpObj, objFrm - 8);
				MoveObj(tmpObj, cur_x + DirIncrX[dirFram << 1], cur_y + DirIncrY[dirFram << 1], MapZ);
				break;
			}
		}
		for(tmpObj = FindLoc(cur_x - DirIncrX[dir], cur_y - DirIncrY[dir], MapZ); tmpObj >= 0; tmpObj = NextLoc()) {
			if(
				tmpObj >= 0x100 &&
				GetType(tmpObj) == OBJ_19C &&
				GetFrame(tmpObj) > 0xf &&
				TerrainType[TILE_FRAME(tmpObj)] & D_17DE[(dir >> 1) ^ 2]
			) {
				SetFrame(tmpObj, objFrm + 8);
				MoveObj(tmpObj, cur_x - DirIncrX[dirFram << 1], cur_y - DirIncrY[dirFram << 1], MapZ);
				break;
			}
		}
		SetFrame(objNum, objFrm);
	} else if(objTyp == OBJ_162 || objTyp == OBJ_167 || objTyp == OBJ_19E || objTyp == OBJ_184) {
		dirFram = objFrm;
		MACRO_A(dir, dirFram);
		objFrm = dirFram;
		SetFrame(objNum, objFrm);
	} else if(objTyp == OBJ_1A8) {
		dirFram = objFrm >> 3;
		MACRO_A(dir, dirFram);
		if(!(objFrm & 4))
			objFrm = 7;
		else
			objFrm = 3;
		objFrm = objFrm + (dirFram << 3);
		SetFrame(objNum, objFrm);
		if(!IsOutOfArea(objNum))
			CyclopsFlag = OSI_rand(2, 4);
	} else if(objTyp == OBJ_19B && objFrm < 8) {
		cur_x = GetX(objNum);
		cur_y = GetY(objNum);
		for(bp_0e = 0; bp_0e < 4; bp_0e ++) {
			tmpObj = C_1184_10F1(cur_x, cur_y, MapZ, bp_0e << 1, objFrm);
			if(tmpObj >= 0)
				SetFrame(tmpObj, D_17CE[bp_0e + ((dir & 6) << 1)]);
		}
		SetFrame(objNum, dir & 6);
	}
	D_17A3 = 0;
}
#undef MACRO_A


/*C_1E0F_0F54*/SubMov(int objNum, int bp06) {
	if(GetType(objNum) == OBJ_1AF)
		bp06 >>= 1;
	if(SpellFx[10] && IsPlrControl(objNum))
		bp06 >>= 1;
	else if(SpellFx[7] && Is_ATKPLR(objNum))
		bp06 <<= 1;
	if(bp06 < 1)
		bp06 = 1;
	MovePts[objNum] -= bp06;
}

/*is dragged under[by corpser]?*/
static C_1E0F_0FA9(int si/*bp06*/) {
	if(IsDraggedUnder(si)) {
		if(NPCMode[si] == AI_COMMAND)
			CON_printf(D_356A_0181, (char *)GetObjectString(si));
		if(OSI_rand(1, 30) < GetStr(si)) {
			MUS_0065(10, 0);
			CON_printf((char *)GetObjectString(si));
			CON_printf(RegurgitatedMsg);
			ClrDraggedUnder(si);
			D_17B0 = 1;
		} else {
			LooseHP(si, OSI_rand(1, 15));
			MovePts[si] = 0;
			return 1;
		}
	}
	return 0;
}

/*AlreadyContiguous?*/
static C_1E0F_1056(int bp0a, int x, int y) {
	int delta_x, delta_y;
	int bp_0a, bp_08, bp_06, ret, isMouse;

	ret = 0;
	isMouse = (GetType(D_EBD5[bp0a]) == OBJ_162);
	for(bp_0a = 0; bp_0a < bp0a; bp_0a ++) {
		bp_08 = D_EBD5[bp_0a];
		bp_06 = GetType(bp_08);
		delta_x = GetX(bp_08) - x;
		delta_y = GetY(bp_08) - y;
		if(delta_x == 0 && delta_y == 0 && bp_06 != OBJ_162 && !isMouse)
			return 0;
		if(delta_x > -2 && delta_x < 2 && delta_y > -2 && delta_y < 2)
			ret = 1;
		if(GetType(bp_08) != OBJ_1AF)
			continue;
		delta_x -= DirIncrX[GetFrame(bp_08) & 6];
		delta_y -= DirIncrY[GetFrame(bp_08) & 6];
		if(delta_x == 0 && delta_y == 0 && bp_06 != OBJ_162 && !isMouse)
			return 0;
		if(delta_x > -2 && delta_x < 2 && delta_y > -2 && delta_y < 2)
			ret = 1;
	}
	return ret;
}

/*C_1E0F_1193*/MoveFollowers(int objNum, int aFlag) {
	register int si, follow_pos;
	int bp_26, try_dir, pass;
	int follower_x, follower_y;
	int try_x, try_y, eager, new_dir;
	int max_eager, bp_12, facing_dir, bp_0e, x, y, target_x, target_y;
	int bp_04, bp_02;

	if(IN_VEHICLE)
		return;
	D_EBD5[0] = objNum;
	x = GetX(objNum);
	y = GetY(objNum);
	follow_pos = 1;
	bp_0e = GetDirection(objNum);
	bp_04 = GetType(objNum);
	if((bp_04 >= OBJ_178 && bp_04 <= OBJ_19A) || bp_04 == OBJ_16B)
		facing_dir = (GetFrame(objNum) >> 1) & 6;
	else
		facing_dir = bp_0e & 6;
	/*two pass algorithm*/
	/*[see doc9.txt]*/
	for(pass = 0; pass < 2; pass ++) {
		follow_pos = 1;
		for(bp_26 = 0; bp_26 < PartySize; bp_26 ++) {
			si = Party[bp_26];
			D_EBD5[follow_pos] = si;
			target_x = x
				- DirIncrX[facing_dir] * D_17C3[follow_pos]
				- DirIncrY[facing_dir] * D_17B8[follow_pos]
			;
			target_y = y
				+ DirIncrX[facing_dir] * D_17B8[follow_pos]
				- DirIncrY[facing_dir] * D_17C3[follow_pos]
			;
			if(si == objNum)
				continue;
			if(GetZ(si) != MapZ)
				continue;
			if(NPCMode[si] != AI_FOLLOW)
				continue;
			if(C_1E0F_0FA9(si))
				continue;
			if(Isbis_0016(si) || IsDraggedUnder(si))
				continue;
			bp_12 = MovePts[si];
			follower_x = GetX(si);
			follower_y = GetY(si);
			if(
				!C_1E0F_1056(follow_pos, follower_x, follower_y)
				/*on second pass*/
				|| (pass == 1 && (
					(aFlag && !(target_x == follower_x && target_y == follower_y)) ||
					(!aFlag && (target_x - follower_x) * DirIncrX[facing_dir] + (target_y - follower_y) * DirIncrY[facing_dir] > 0)
				))
			) {
				bp_02 = C_1E0F_1056(follow_pos, follower_x, follower_y);
				new_dir = -1;
				max_eager = 0;
				for(try_dir = 0; try_dir < 8; try_dir ++) {
					try_x = follower_x + DirIncrX[try_dir];
					try_y = follower_y + DirIncrY[try_dir];
					if(C_1E0F_000F(si, try_x, try_y)) {
						if(D_17A9) {
							if(bp_02)
								continue;
							eager = 128;
						} else {
							eager = 256;
						}
						if(C_1E0F_1056(follow_pos, try_x, try_y))
							eager += 256;
						else if(bp_02)
							eager = 0;
						eager -= __abs__(try_x - target_x);
						eager -= __abs__(try_y - target_y);
						if(eager > max_eager) {
							max_eager = eager;
							new_dir = try_dir;
						}
					}
				}
				if(new_dir >= 0) {
					MoveObj(si, follower_x + DirIncrX[new_dir], follower_y + DirIncrY[new_dir], MapZ);
					D_17A3 = 1;
					C_1E0F_0664(si, new_dir);
				}
			}
			MovePts[si] = bp_12;
			SubMov(Party[bp_26], 5);
			follow_pos ++;
		}
	}
}

/*C_1E0F_14BE*/Board(int objNum) {
	int di;
	int new_objNum, bp_06, bp_04, bp_02;

	bp_02 = -1;
	if(GetType(objNum) == OBJ_19C) {
		bp_06 = TerrainType[TILE_FRAME(objNum)] & 0xf0;
		bp_04 = -1;
		if(bp_06 == TERRAIN_FLAG_80)
			bp_04 = 0;
		else if(bp_06 == TERRAIN_FLAG_40)
			bp_04 = 2;
		else if(bp_06 == TERRAIN_FLAG_20)
			bp_04 = 4;
		else if(bp_06 == TERRAIN_FLAG_10)
			bp_04 = 6;
		if(bp_04 >= 0) {
			for(objNum = FindLoc(GetX(objNum) + DirIncrX[bp_04], GetY(objNum) + DirIncrY[bp_04], MapZ); objNum >= 0; objNum = NextLoc()) {
				if(GetType(objNum) == OBJ_19C)
					break;
			}
		}
	}
	if(objNum < 0)
		return;
	COMBAT_breakOff();
	if(IN_VEHICLE) {
		bp_02 = Unboard(1);
		if(GetType(bp_02) != OBJ_19E && GetType(bp_02) != OBJ_19F)
			bp_02 = -1;
	}
	MapX = GetX(objNum);
	MapY = GetY(objNum);
	PartyEnter(2);
	new_objNum = 0;
	CloneObject(Party[0], new_objNum);
	ClrLocal(new_objNum);
	ClrInvisible(new_objNum);
	MovePts[new_objNum] = DEXTE[new_objNum];
	MoveObj(new_objNum, MapX, MapY, MapZ);
	ObjShapeType[new_objNum] = ObjShapeType[objNum];
	HitPoints[new_objNum] = GetQuan(objNum);
	SetAlignment(new_objNum, GOOD);
	SetPlrControl(new_objNum);
	NPCMode[new_objNum] = AI_COMMAND;
	ObjShapeType[objNum] = TypeFrame(OBJ_058, 0);/*why?*/
	DeleteObj(objNum);
	for(di = 0; di < PartySize; di ++)
		InsertObj(Party[di], new_objNum, INVEN);
	if(bp_02 >= 0)
		InsertObj(bp_02, new_objNum, INVEN);
	Active = PartySize;
	Party[Active] = new_objNum;
	for(di = 0; di < 14; di ++)
		Names[Active][di] = Names[0][di];
	ShowObjects();
	D_17B0 = 1;
	MUS_09A8();
}

/*C_1E0F_1745*/Unboard(int onLand) {
	int si, di;
	int bp_02;

	si = -1;
	di = Party[PartySize];
	bp_02 = AddMapObj(GetType(di), 1, HitPoints[di], MapX, MapY, MapZ);
	if(!onLand) {
		/*look for skiff or raft*/
		for(si = FindInv(di); si >= 0; si = NextInv()) {
			if(GetType(si) == OBJ_19E || GetType(si) == OBJ_19F) {
				MoveObj(si, MapX, MapY, MapZ);
				break;
			}
		}
	}
	ClrLocal(bp_02);
	if(bp_02 >= 0)
		ObjShapeType[bp_02] = ObjShapeType[di];
	ObjShapeType[di] = TypeFrame(OBJ_058, 0);
	NPCMode[di] = AI_MOTIONLESS;
	MoveObj(di, MapX, MapY, 14);
	Active = 0;
	PartyExit(MapX, MapY, MapZ, 2);
	if(si >= 0)
		Board(si);
	MUS_09A8();
	return bp_02;
}

char *D_17E2[] = {
	/*1872*/"Deceit",
	/*1879*/"Despise",
	/*1881*/"Destard",
	/*1889*/"Wrong",
	/*188F*/"Covetous",
	/*1898*/"Shame",
	/*189E*/"Hythloth",

	/*18A7*/"GSA",

	/*18AB*/"Control",
	/*18B3*/"Passion",
	/*18BB*/"Diligence",

	/*18C5*/"Tomb of Kings",
	/*18D3*/"Ant Mound",
	/*18DD*/"Swamp Cave",
	/*18E8*/"Spider Cave",
	/*18F4*/"Cyclops Cave",
	/*1901*/"Heftimus Cave",
	/*190F*/"Heroes' Hole",
	/*191C*/"Pirate Cave",
	/*1928*/"Buccaneer's Cave"
};

C_1E0F_184D() {
	int objNum, di;
	int bp_06, objType, bp_02;

	bp_02 = 0;
	if(InCombat)
		return;
	if(!IN_VEHICLE) {
		for(objNum = FindLoc(MapX, MapY, MapZ); objNum >= 0; objNum = NextLoc()) {
			if(Party[Active] == objNum)
				continue;

			objType = GetType(objNum);
			/*moongate*/
			if(objType == OBJ_055 && D_0658 == 0) {
				if(D_2CC3 != -1) {
					bp_02 = 1;
					break;
				}
				if(Time_H == 0 && Time_M < 10) {
					/*shrine of spirituality*/
					PartyTeleport(0x018, 0x01d, 1, 1);
					MUS_091A(8);
					C_0A33_1355(10);
					break;
				}
				bp_06 = __abs__(7 - D_2CC7) - __abs__(7 - D_2CC9);
				if(bp_06 < 0) {
c_1922:
					GateTravel(D_2CC6);
					break;
				}
				if(bp_06 <= 0 && (Time_M < 30) == (D_2CC7 < D_2CC9)) {
					goto c_1922;
				}
				{
					GateTravel(D_2CC8);
					break;
				}
			}
			/*red gate*/
			if(objType == OBJ_054 && D_0658 == 0) {
				if(D_2CC3 != -1) {
					bp_02 = 1;
					break;
				}
				di = GetQual(objNum);
				if(di) {
					di --;
					PartyTeleport(D_171C[di], D_174E[di], D_1780[di], 1);
					MUS_09A8();
					break;
				}
				PartyTeleport(MapX, MapY, MapZ, 1);
				break;
			}
			/*dungeon/cave*/
			if(objType == OBJ_146 || objType == OBJ_134) {
				if(SHAMINO_COMMENT && GetQual(objNum) != 0 && GetQual(objNum) < 21) {
					CON_printf(D_356A_045F);
					if(GetQual(objNum) < 8)
						CON_printf(/*1939*/"dungeon ");
					else if(GetQual(objNum) > 8 && GetQual(objNum) < 12)
						CON_printf(/*1942*/"shrine of ");
					CON_printf(D_17E2[GetQual(objNum) - 1]);
					CON_printf(/*194D*/".\"\n");
					PromptFlag = 1;
				}
				/* */
				if(D_2CC3 != -1) {
					bp_02 = 1;
					break;
				}
				C_101C_089E(objNum);
				break;
			}
		}
	}
	if(bp_02) {
		CON_printf(D_356A_0493);
		PromptFlag = 1;
	}
}

unsigned char D_180A[] = {10,5,3,4, 5,4,3,5};
unsigned char D_1812[] = { 3,4,5,7,10,7,5,4};

unsigned char D_181A[][8] = {
	{0,0,1,1,0,7,7,0},
	{1,1,1,2,2,1,0,0},
	{1,2,2,2,3,3,2,1},
	{2,2,3,3,3,4,4,3},
	{4,3,3,4,4,4,5,5},
	{6,5,4,4,5,5,5,6},
	{7,7,6,5,5,6,6,6},
	{7,0,0,7,6,6,7,7}
};

/*[advance]*/
C_1E0F_1B0E(int dir) {
	int si, tmp_dir;
	int bp_0a, new_x, new_y, bp_04, flow_dir;

	D_17AD = 1;
	si = Party[Active];
	MapX = GetX(si);
	MapY = GetY(si);
	D_17AE = 0;
	if(DrunkCounter > 3 && OSI_rand(0, 1)) {
		CON_printf(/*1951*/"Hic!\n");
		dir = OSI_rand(0, 7);
	}
	if(GetType(si) == OBJ_19F || GetType(si) == OBJ_1A7) {
		flow_dir = dir;
		dir = WindDir ^ 4;
		if(GetType(si) == OBJ_19F) {
			bp_04 = GetTileAtXYZ(MapX, MapY, MapZ);
			if(bp_04 >= TIL_008 && bp_04 < TIL_010)
				flow_dir = bp_04 - TIL_008;
			if(dir < 0)
				dir = flow_dir;
			else
				dir = D_181A[flow_dir][dir];
		}
		if(dir < 0) {
			CON_printf(NoWindMsg);
			PromptFlag = 1;
			MovePts[si] = 0;
			return;
		}
	}
	tmp_dir = dir;
	new_x = (MapX + DirIncrX[tmp_dir]) & 0x3ff;
	new_y = (MapY + DirIncrY[tmp_dir]) & 0x3ff;
	if(!C_1E0F_000F(si, new_x, new_y)) {
		D_17AE = 1;
		if(MousePress && DirIncrX[tmp_dir] && DirIncrY[tmp_dir]) {
			if(abs(MouseMapX - MapX) > abs(MouseMapY - MapY)) {
				if(C_1E0F_000F(si, new_x, MapY)) {
					D_17AE = 0;
					new_y = MapY;
				} else if(C_1E0F_000F(si, MapX, new_y)) {
					D_17AE = 0;
					new_x = MapX;
				}
			} else {
				if(C_1E0F_000F(si, MapX, new_y)) {
					D_17AE = 0;
					new_x = MapX;
				} else if(C_1E0F_000F(si, new_x, MapY)) {
					D_17AE = 0;
					new_y = MapY;
				}
			}
		}
	} else if(
		DirIncrX[tmp_dir] && DirIncrY[tmp_dir] &&
		!C_1E0F_000F(si, (MapX + DirIncrX[tmp_dir]) & 0x3ff, MapY) &&
		!C_1E0F_000F(si, MapX, (MapY + DirIncrY[tmp_dir]) & 0x3ff)
	) {
		D_17AE = 1;
	}
	if(D_17AE) {
		MUS_0065(1, 0x137);
		MapX = GetX(si);
		MapY = GetY(si);
		D_17AD = 0;
		return;
	}
	if(GetType(Party[Active]) != OBJ_162) {
		for(bp_0a = 0; bp_0a < PartySize; bp_0a ++) {
			if(Active != bp_0a) {
				si = Party[bp_0a];
				if(
					!IsDead(si) && GetType(si) != OBJ_162 &&
					GetX(si) == new_x && GetY(si) == new_y && GetZ(si) == MapZ
				) {
					if(NPCMode[si] != AI_FRONT || TryMoveTo(si, new_x + new_x - MapX, new_y + new_y - MapY) == 0) {
						MoveObj(si, MapX, MapY, MapZ);
						D_17A3 = 1;
						C_1E0F_0664(si, (tmp_dir + 4) & 7);
					}
					break;
				}
			}
		}
	}

	si = Party[Active];
	bp_04 = GetType(si);
	if(bp_04 == OBJ_19C && WindDir >= 0) {
		SubMov(si, D_180A[(tmp_dir - WindDir) & 7]);
	} else if(bp_04 == OBJ_19E) {
		flow_dir = tmp_dir;
		bp_04 = GetTileAtXYZ(MapX, MapY, MapZ);
		if(bp_04 >= TIL_008 && bp_04 < TIL_010)
			flow_dir = bp_04 - TIL_008;
		SubMov(si, D_1812[(tmp_dir - flow_dir) & 7]);
	} else if(bp_04 == OBJ_19F || bp_04 == OBJ_1A7) {
		SubMov(si, 5);
	} else if(DirIncrX[tmp_dir] && DirIncrY[tmp_dir]) {
		SubTerrainMov(si);
		SubMov(si, 5);
	} else {
		SubTerrainMov(si);
	}
	MapX = new_x;
	MapY = new_y;
	C_101C_054C();
	if(MoveObj(si, new_x, new_y, MapZ) >= 0) {
		SetDirection(si, tmp_dir);
		D_17A3 = 1;
		C_1E0F_0664(si, tmp_dir);
		D_17B0 = 1;
	} else {
		MapX = GetX(si);
		MapY = GetY(si);
		C_101C_054C();
		CON_printf(/*1957*/"Object mismatch\n");
	}
	D_17AD = 0;
	MoveFollowers(si, 0);
	C_1E0F_184D();
	D_17AD = 0;
}

C_1E0F_2001(int bp08, int bp06) {
	int si, di;
	int bp_04, objCls;

	objCls = GetMonsterClass(GetType(bp08));
	if(objCls < 0 || !IsMonster_4000(objCls) || IsPlrControl(bp08))
		return;
	for(si = FindLoc(GetX(bp08), GetY(bp08), GetZ(bp08)); si >= 0; si = NextLoc()) {
		di = GetType(si);
		if(di == OBJ_129 || di == OBJ_12A || di == OBJ_12B || di == OBJ_12C) {
			bp_04 = GetFrame(si);
			SetFrame(si, (bp06 << 2) + (bp_04 & 3));
			C_1100_0306();
			break;
		}
		if(di == OBJ_116 || di == OBJ_118) {
			TextOn = 0;
			C_27A1_5DF2(si);
			TextOn = 1;
		}
	}
}

/*look for plate around {x, y, z} in 4 directions*/
static C_1E0F_2125(int bp0a, int bp08, int bp06) {
	int si, di;

	for(si = 0; si < 8; si += 2) {
		for(di = FindLoc(DirIncrX[si] + bp0a, DirIncrY[si] + bp08, bp06); di >= 0; di = NextLoc()) {
			if(GetType(di) == OBJ_077)
				break;
		}
		if(di >= 0)
			break;
	}
	if(si < 8)
		return si + 1;
	return 0;
}

static C_1E0F_2184(int objNum, int bp06) {
	int si, di;
	int x, y, z;

	x = GetX(objNum);
	y = GetY(objNum);
	z = GetZ(objNum);
	di = D_1799[bp06];
	for(si = FindLoc(x, y, z); si >= 0; si = NextLoc()) {
		if(di == OBJ_0E4) {
			if(GetType(si) == OBJ_0FC && C_1E0F_2125(x, y, z))
				break;
		} else if(di == OBJ_0FC) {
			if(
				GetType(si) == OBJ_0FC ||
				(GetType(si) == OBJ_147 && GetFrame(si) - D_0658 == 2)
			) break;
		} else if(GetType(si) == di) {
			break;
		}
	}
	return si;
}

static /*C_1E0F_2276*/__AtDestination(int objNum) {
	int di;
	struct coord xyz;
	int action, isAtDest, bp_02;

	action = Schedule[SchedIndex[objNum] + SchedPointer[objNum]].action;
	xyz = Schedule[SchedIndex[objNum] + SchedPointer[objNum]].xyz;
	isAtDest = (
		xyz.raw[0] == ObjPos[objNum].raw[0] &&
		xyz.raw[1] == ObjPos[objNum].raw[1] &&
		xyz.raw[2] == ObjPos[objNum].raw[2]
	);
	NPCMode[objNum] = action;
	if(action == AI_SLEEP) {
		isAtDest = 1;
		SetAsleep(objNum);
		if(!IsOutOfArea(objNum)) {
			for(di = FindLoc(GetX(objNum), GetY(objNum), GetZ(objNum)); di >= 0; di = NextLoc()) {
				if(GetType(di) == OBJ_0A3 && (GetFrame(di) - D_0658 == 0 || GetFrame(di) - D_0658 == 6)) {
					ObjShapeType[objNum] = TypeFrame(OBJ_092, 0);
					if(GetFrame(di) - D_0658 != 0)
						SetFrame(objNum, 1);
					break;
				}
			}
			if(di < 0 || GetType(di) != OBJ_0A3)
				SetTypeUnconscious(objNum);
		} else {
			NPCMode[objNum] = AI_SCHEDULE;
		}
	} else {
		ClrAsleep(objNum);
		ObjShapeType[objNum] = OrigShapeType[objNum];
		if(action == AI_SIT || action == AI_PLAY) {
			isAtDest = 1;
			if(!IsOutOfArea(objNum)) {
				di = C_1E0F_2184(objNum, 0/*chair*/);
				if(di >= 0) {
					if(action == AI_PLAY) {
						SetFrame(di, 2);
						C_1E0F_0664(objNum, 4);
						ObjShapeType[objNum] = TypeFrame(OBJ_188, 0);
						SetFrame(objNum, 4);
					} else {
						C_1E0F_0664(objNum, 0);
					}
				}
			} else {
				NPCMode[objNum] = AI_SCHEDULE;
			}
		} else if(action == AI_EAT) {
			isAtDest = 1;
			if(!IsOutOfArea(objNum)) {
				di = C_1E0F_2184(objNum, 2/*table*/);
				if(di >= 0) {
					bp_02 = C_1E0F_2125(GetX(di), GetY(di), GetZ(di));
					SetFrame(di, bp_02 >> 1);
					C_1E0F_0664(objNum, bp_02 - 1);
				} else {
					C_1E0F_0664(objNum, 4);
				}
			} else {
				NPCMode[objNum] = AI_SCHEDULE;
			}
		} else if(action == AI_RINGBELL) {
			isAtDest = 1;
			if(!IsOutOfArea(objNum)) {
				di = C_1E0F_2184(objNum, 3/*pull chain*/);
			} else {
				NPCMode[objNum] = AI_SCHEDULE;
			}
		} else if(action >= AI_STAND_N && action <= AI_GUARD_W) {
			SetDirection(objNum, ((action - AI_STAND_N) & 3) << 1);
			C_1E0F_0664(objNum, ((action - AI_STAND_N) & 3) << 1);
		}
	}
	if(!isAtDest) {
		NPCMode[objNum] = AI_FINDPATH;
		if(GetType(objNum) == OBJ_188) {
			ObjShapeType[objNum] = TypeFrame(OBJ_182, 0);
			C_1E0F_0664(objNum, 4);
		}
	}
}

typedef struct {
	unsigned char /*_0000*/PTH_map[AREA_H][AREA_W];
	int /*_0640*/PTH_rank[0x100];
	int /*_0840*/PTH_link[0x100];
	unsigned char /*_0a40*/PTH_x[0x100],/*_0b40*/PTH_y[0x100];
	unsigned char /*_0c40*/PTH_resist[AREA_H][AREA_W];
} t40x40;

#define _ScratchBuf ((t40x40 far *)ScratchBuf)

char D_185A[] = { 0,1,0,-1};
char D_185E[] = {-1,0,1, 0};

static C_1E0F_25F9(int area_x, int area_y) {
	int i, dir;
	int bp_0e, some_min, dir_min, back_x, back_y, new_x, new_y;

	back_x = area_x;
	back_y = area_y;

	bp_0e =
	dir = 0;
	D_8C42->content[PTH_Index][0] = 0;

	while(_ScratchBuf->PTH_map[area_y][area_x]) {
		some_min = 0xff;
		for(i = 0; i < 4; i ++) {
			new_x = area_x + D_185A[i];
			new_y = area_y + D_185E[i];
			if(new_x >= 0 && new_x <= (AREA_W-1) && new_y >= 0 && new_y <= (AREA_H-1)) {
				if(

					_ScratchBuf->PTH_map[new_y][new_x] < some_min ||
					(_ScratchBuf->PTH_map[new_y][new_x] == some_min && dir == i)
				) {
					some_min = _ScratchBuf->PTH_map[new_y][new_x];
					dir_min = i;
				}
			}
		}
		dir = dir_min;
		area_x += D_185A[dir];
		area_y += D_185E[dir];


		if(bp_0e == 0 || (D_8C42->content[PTH_Index][0] & 3) != (dir ^ 2)) {
			if(bp_0e < 0x1f)
				bp_0e ++;
			for(i = bp_0e; i > 0; i --)
				D_8C42->content[PTH_Index][i] = D_8C42->content[PTH_Index][i - 1];
			D_8C42->content[PTH_Index][0] = (dir ^ 2) + 4;
		} else {
			D_8C42->content[PTH_Index][0] += 4;
		}
	}

	if(PTH_object < 0 && PTH_direct < 0) {
		area_x = back_x;
		area_y = back_y;

		_ScratchBuf->PTH_map[area_y][area_x] = 0xff;

		while(_ScratchBuf->PTH_map[area_y][area_x] > 0x80) {
			some_min = 0xff;
			for(i = 0; i < 4; i ++) {
				new_x = area_x + D_185A[i];
				new_y = area_y + D_185E[i];
				if(new_x >= 0 && new_x <= (AREA_W-1) && new_y >= 0 && new_y <= (AREA_H-1)) {
					if(
						_ScratchBuf->PTH_map[area_y + D_185E[i]][area_x + D_185A[i]] >= 0x80 && (
						_ScratchBuf->PTH_map[new_y][new_x] < some_min ||
						(_ScratchBuf->PTH_map[new_y][new_x] == some_min && dir == i)
					)) {
						some_min = _ScratchBuf->PTH_map[new_y][new_x];
						dir_min = i;
					}
				}
			}
			dir = dir_min;
			area_x += D_185A[dir];
			area_y += D_185E[dir];
			if(bp_0e >= 0x1f)
				break;
			if(bp_0e == 0 || (D_8C42->content[PTH_Index][bp_0e - 1] & 3) != dir) {
				D_8C42->content[PTH_Index][bp_0e] = dir + 4;
				bp_0e ++;
			} else {
				D_8C42->content[PTH_Index][bp_0e - 1] += 4;
			}
		}
	}
	D_8C42->content[PTH_Index][bp_0e] = 0;
}

static C_1E0F_291C(int objNum) {
	struct coord sched_xyz;
	int x, y, z;

	x = GetX(objNum);
	y = GetY(objNum);
	z = GetZ(objNum);
	if(
		!AllowNPCTeleport &&
		z == MapZ &&
		x >= MapX - 5 && x <= MapX + 5 &&
		y >= MapY - 5 && y <= MapY + 5
	) return;
	sched_xyz = Schedule[SchedIndex[objNum] + SchedPointer[objNum]].xyz;
	x = GetCoordX(sched_xyz);
	y = GetCoordY(sched_xyz);
	if(
		!AllowNPCTeleport &&
		z == MapZ &&
		x >= MapX - 5 && x <= MapX + 5 &&
		y >= MapY - 5 && y <= MapY + 5
	) return;
	if(NPCMode[objNum] >= AI_ONPATH && NPCMode[objNum] < AI_86)
		PathObject[Leader[objNum]] = 0;
	D_06E6 = 0;
	MoveObj(objNum, x, y, GetCoordZ(sched_xyz));
	D_06E6 = 1;
	__AtDestination(objNum);
	SubMov(objNum, 10);
	D_17A5 ++;
}

static C_1E0F_2A74(int resist, int x, int y, int flag_80) {
	int newTop, di;
	int bp_04, bp_02;

	if(
		PTH_Found ||
		resist > 0x7f ||
		!(x >= 0 && x <= (AREA_W-1) && y >= 0 && y <= (AREA_H-1))
	) return;

	if(_ScratchBuf->PTH_map[y][x] != 0xff && (_ScratchBuf->PTH_map[y][x] & 0x80) != flag_80) {
		PTH_Found = 1;
		C_1E0F_25F9(x, y);
		return;
	}

	if(PTH_object >= 0) {
		bp_04 = (MapX - 0x10) & 0x3f8;
		bp_02 = (MapY - 0x10) & 0x3f8;
		for(di = FindLoc(x + bp_04, y + bp_02, MapZ); di >= 0; di = NextLoc()) {
			if(
				(PTH_object == OBJ_0E4 && GetType(di) == OBJ_0FC && C_1E0F_2125(x + bp_04, y + bp_02, MapZ)) ||
				(PTH_object != OBJ_0E4 && GetType(di) == PTH_object)
			) {
				PTH_Found = 1;
				C_1E0F_25F9(x, y);
				return;
			}
		}
	}

	if(PTH_direct >= 0) {
		if(
			(x == 0 && PTH_direct == 3) || (x == (AREA_W-1) && PTH_direct == 1) ||
			(y == 0 && PTH_direct == 0) || (y == (AREA_H-1) && PTH_direct == 2)
		) {
			PTH_Found = 1;
			PTH_AnX = x;
			PTH_AnY = y;
			C_1E0F_25F9(x, y);
			return;
		}
	}
	if((_ScratchBuf->PTH_map[y][x] & 0x7f) <= resist)
		return;
	newTop = PTH_Top;
	if(newTop < 0)
		return;
	if(PTH_Min < resist)
		PTH_Min = resist;
	PTH_Top = _ScratchBuf->PTH_link[newTop];
	_ScratchBuf->PTH_link[newTop] = _ScratchBuf->PTH_rank[resist];
	_ScratchBuf->PTH_x[newTop] = x;
	_ScratchBuf->PTH_y[newTop] = y;
	_ScratchBuf->PTH_rank[resist] = newTop;
	_ScratchBuf->PTH_map[y][x] = resist | flag_80;
}

static /*C_1E0F_2C9D*/__ResetPath(int index) {
	int si, di;
	int bp_02;

	for(di = 0; di < 0x20; di ++) {
		if(D_8C42->content[index][di] == 0)
			break;
	}
	for(si = 0; si < (di + 1) / 2; si ++) {
		bp_02 = D_8C42->content[index][si] ^ 2;
		D_8C42->content[index][si] = D_8C42->content[index][di - 1 - si] ^ 2;
		D_8C42->content[index][di - 1 - si] = bp_02;
	}
}

static C_1E0F_2D37(int objNum, int x, int y) {
	int dst_x, dst_y, i, j, bp_0e;
	int area_x, area_y, bp_08, flag_80, tmp, bp_02;

	PTH_Found = bp_02 = 0;
	PTH_direct = -1;
	area_x = (MapX - 0x10) & 0x3f8;
	area_y = (MapY - 0x10) & 0x3f8;
	/*coord "in area"*/
	dst_x = GetX(objNum) - area_x;
	dst_y = GetY(objNum) - area_y;
	if(x >= 0) {
		/*coord "in area"*/
		x -= area_x;
		y -= area_y;
		PTH_object = -1;
		if(dst_x == x && dst_y == y) {
			__AtDestination(objNum);
			return;
		}
		if(!(
			GetZ(objNum) == MapZ &&
			dst_x >= 0 && dst_x <= (AREA_W-1) &&
			dst_y >= 0 && dst_y <= (AREA_H-1) &&
			x >= 0 && x <= (AREA_W-1) &&
			y >= 0 && y <= (AREA_H-1)
		)) {
			if(D_17A5 >= 3)
				return;
			C_1E0F_291C(objNum);
			if(NPCMode[objNum] != AI_FINDPATH)
				return;
			if(!(dst_x >= 0 && dst_x <= (AREA_W-1) && dst_y >= 0 && dst_y <= (AREA_H-1))) {
				tmp = x; x = dst_x; dst_x = tmp;
				tmp = y; y = dst_y; dst_y = tmp;
				bp_02 = 1;
			}
			/*favored direction?*/
			if(__abs__(x - 20) > __abs__(y - 20))
				PTH_direct = (x < 20)?3:1;
			else
				PTH_direct = (y < 20)?0:2;
		}
	} else {
		/*seeked object*/
		PTH_object = y;
	}

	D_17A7 ++;
	__ComputeResistance(objNum);

	for(i = 0; i < AREA_H; i ++) {
		for(j = 0; j < AREA_W; j ++)
			_ScratchBuf->PTH_map[i][j] = 0xff;
	}
	for(i = 0; i < 0x100; i ++)
		_ScratchBuf->PTH_link[i] = i + 1;
	for(i = 0; i < 0x100; i ++)
		_ScratchBuf->PTH_rank[i] = -1;
	_ScratchBuf->PTH_link[0xff] = -1;
	PTH_Top = PTH_Found = 0;
	PTH_Cur = PTH_Min = 0;

	C_1E0F_2A74(0, dst_x, dst_y, 0);
	if(PTH_object == -1 && PTH_direct == -1)
		C_1E0F_2A74(0, x, y, 0x80);

	while(_ScratchBuf->PTH_rank[PTH_Cur] >= 0 && !PTH_Found) {
		bp_0e = _ScratchBuf->PTH_rank[PTH_Cur];
		_ScratchBuf->PTH_rank[PTH_Cur] = _ScratchBuf->PTH_link[bp_0e];
		x = _ScratchBuf->PTH_x[bp_0e];
		y = _ScratchBuf->PTH_y[bp_0e];
		_ScratchBuf->PTH_link[bp_0e] = PTH_Top;
		PTH_Top = bp_0e;
		bp_08 = _ScratchBuf->PTH_map[y][x];
		flag_80 = bp_08 & 0x80;
		bp_08 &= 0x7f;
		C_1E0F_2A74(_ScratchBuf->PTH_resist[y][x - 1] + bp_08, x - 1, y, flag_80);
		C_1E0F_2A74(_ScratchBuf->PTH_resist[y][x + 1] + bp_08, x + 1, y, flag_80);
		C_1E0F_2A74(_ScratchBuf->PTH_resist[y - 1][x] + bp_08, x, y - 1, flag_80);
		C_1E0F_2A74(_ScratchBuf->PTH_resist[y + 1][x] + bp_08, x, y + 1, flag_80);
		if(_ScratchBuf->PTH_rank[PTH_Cur] < 0) {
			while(PTH_Cur < PTH_Min && _ScratchBuf->PTH_rank[PTH_Cur] < 0)
				PTH_Cur ++;
		}
	}
	if(PTH_Found) {
		if(bp_02) {
			D_06E6 = 0;
			MoveObj(objNum, PTH_AnX + area_x, PTH_AnY + area_y, GetZ(objNum));
			D_06E6 = 1;
			__ResetPath(PTH_Index);
		}
		NPCMode[objNum] = AI_ONPATH;
		Leader[objNum] = PTH_Index;
		PathObject[PTH_Index] = objNum;
		PathCounter[PTH_Index] = 0;
		PathTries[PTH_Index] = 0;
	} else if(NPCMode[objNum] == AI_FINDPATH) {
		NPCMode[objNum] = AI_86;
	} else if(NPCMode[objNum] == AI_SEEKOBJ) {
		NPCMode[objNum] = AI_SCHEDULE;
	}
}

static C_1E0F_31C7() {
	int si;

	for(si = 0; OSI_rand(0, 1); si ++);
	if(OSI_rand(0, 1))
		si = -si;
	return si;
}

/*consume movement points according
to terrain*/
static /*C_1E0F_31F7*/SubTerrainMov(int numObject) {
	int numTile, di;
	int x, y, z, pts;

	pts = 5;
	x = GetX(numObject);
	y = GetY(numObject);
	z = GetZ(numObject);
	numTile = GetTileAtXYZ(x, y, z);
	if(!IsTerrainImpass(numTile))
		pts += TerrainType[numTile] >> 4;
	for(di = FindLoc(x, y, z); di >= 0; di = NextLoc()) {
		numTile = TILE_FRAME(di);
		if(!IsTerrainImpass(numTile))
			pts += TerrainType[numTile] >> 4;
	}
	SubMov(numObject, pts);
}

/*C_1E0F_32D6*/TryStraightMove(int objNum, int dir, int bp06) {
	int new_x, new_y;

	dir &= 7;
	SubTerrainMov(objNum);
	new_x = GetX(objNum) + DirIncrX[dir];
	new_y = GetY(objNum) + DirIncrY[dir];
	if(
		!IsPlrControl(objNum) || !InCombat ||
		CLOSE_ENOUGH0(8, MapX, MapY, new_x, new_y)
	) {
		D_17B4 = bp06;
		if(C_1E0F_000F(objNum, new_x, new_y)) {
				D_17B4 = 0;
			C_1E0F_2001(objNum, 1);
			MoveObj(objNum, new_x, new_y, MapZ);
			SetDirection(objNum, dir);
			D_17A3 = 1;
			C_1E0F_0664(objNum, dir);
			C_1E0F_2001(objNum, 0);
			return 1;
		}
		D_17B4 = 0;
	}
	return 0;
}

static C_1E0F_33C4(int objNum, int x, int y) {
	int delta_x, delta_y, dir;

	if(OSI_rand(0, 7) == 0) {
		delta_x = GetX(objNum) - x;
		delta_y = GetY(objNum) - y;
		if(__abs__(delta_y) - __abs__(delta_x) < C_1E0F_31C7())
			dir = (C_1E0F_31C7() > delta_x)?2:6;
		else
			dir = (C_1E0F_31C7() > delta_y)?4:0;
		TryStraightMove(objNum, dir, 1);
	} else {
		SubMov(objNum, 5);
	}
}

static /*C_1E0F_3470*/__TryDiagMove(int objNum, int h_dir, int v_dir) {
	int dir;
	int bp_06, new_x, new_y;

	if(GetType(objNum) == OBJ_19D)
		return 0;

	if(v_dir == 0 && h_dir == 6)
		dir = 7;
	else
		dir = (v_dir + h_dir) >> 1;
	SubTerrainMov(objNum);
	new_x = GetX(objNum) + DirIncrX[dir];
	new_y = GetY(objNum) + DirIncrY[dir];
	D_17B2 = 1;
	D_17B4 = 1;
	bp_06 = C_1E0F_000F(objNum, new_x, GetY(objNum));
	if(!bp_06)
		bp_06 = C_1E0F_000F(objNum, GetX(objNum), new_y);
	D_17B2 = 0;
	if(bp_06 && C_1E0F_000F(objNum, new_x, new_y)) {
		D_17B4 = 0;
		C_1E0F_2001(objNum, 1);
		MoveObj(objNum, new_x, new_y, MapZ);
		SetDirection(objNum, dir);
		D_17A3 = 1;
		C_1E0F_0664(objNum, dir);
		C_1E0F_2001(objNum, 0);
		return 1;
	}
	D_17B4 = 0;

	return 0;
}

/*C_1E0F_35A7*/TryMoveTo(int objNum, int new_x, int new_y) {
	int delta_x, delta_y;
	int h_dir, v_dir, bp_06, isVMove, bp_02;

	bp_02 = !D_17AB;
	bp_06 = 1;
	delta_x = new_x - GetX(objNum);
	delta_y = new_y - GetY(objNum);
	if((delta_x == 0 && delta_y == 0) || NPCMode[objNum] == AI_IMMOBILE) {
		SubMov(objNum, 5);
		return 0;
	}
	if(delta_x == 0)
		h_dir = OSI_rand(0, 1)?2:6;
	else if(delta_x < 0)
		h_dir = 6;
	else
		h_dir = 2;
	if(delta_y == 0)
		v_dir = OSI_rand(0, 1)?0:4;
	else if(delta_y < 0)
		v_dir = 0;
	else
		v_dir = 4;
	D_17B2 = 0;
	if(__abs__(delta_x) < 4 && __abs__(delta_y) < 4) {
		isVMove = 
			(__abs__(delta_x) < __abs__(delta_y)) ||
			(__abs__(delta_x) == __abs__(delta_y) && OSI_rand(0, 1))
		;
	} else {
		isVMove = OSI_rand(1, __abs__(delta_x) + __abs__(delta_y)) > __abs__(delta_x);
	}
	if(isVMove) {
		if(
			!TryStraightMove(objNum, v_dir, bp_02) &&
			!__TryDiagMove(objNum, h_dir, v_dir) &&
			!TryStraightMove(objNum, h_dir, bp_02) &&
			(OSI_rand(0, 1) || !TryStraightMove(objNum, h_dir ^ 4, 1))
		) {
			SubTerrainMov(objNum);
			bp_06 = 0;
		}
	} else {
		if(
			!TryStraightMove(objNum, h_dir, bp_02) &&
			!__TryDiagMove(objNum, h_dir, v_dir) &&
			!TryStraightMove(objNum, v_dir, bp_02) &&
			(OSI_rand(0, 1) || !TryStraightMove(objNum, v_dir ^ 4, 1))
		) {
			SubTerrainMov(objNum);
			bp_06 = 0;
		}
	}
	D_17B2 = 1;

	return bp_06;
}

C_1E0F_37DB(int objNum) {
	if(NPCMode[objNum] != AI_IMMOBILE) {
		if(
			IsPlrControl(objNum) &&
			(GetX(objNum) != MapX || GetY(objNum) != MapY) &&
			OSI_rand(0, 3) == 0
		) {
			TryMoveTo(objNum, MapX, MapY);
			return;
		}
		if(OSI_rand(0, 7) == 0) {
			TryStraightMove(objNum, OSI_rand(0, 3) << 1, 1);
			return;
		}
	}
	SubMov(objNum, 5);
}

static /*C_1E0F_387D*/__DoOnPath(int objNum, int x, int y) {
	int index;
	int info;

	index = Leader[objNum];
	info = D_8C42->content[index][PathCounter[index]];
	if(info == 0) {
		PathObject[index] = 0;
		if(COMBAT_getCathesus(objNum, x, y) < 2)
			__AtDestination(objNum);
		else
			NPCMode[objNum] = AI_FINDPATH;
		SubMov(objNum, 5);
	} else if(TryStraightMove(objNum, (info & 3) << 1, 1)) {
		if((info >> 2) <= ++PathTries[index]) {
			PathCounter[index] ++;
			PathTries[index] = 0;
		}
		NPCMode[objNum] = AI_ONPATH;
	} else if(NPCMode[objNum] == AI_ONPATH) {
		NPCMode[objNum] = AI_84;
		MovePts[objNum] = 0;
	} else if(NPCMode[objNum] == AI_84) {
		NPCMode[objNum] = AI_85;
		MovePts[objNum] = 0;
	} else if(NPCMode[objNum] == AI_85) {
		NPCMode[objNum] = AI_86;
		MovePts[objNum] = 0;
		PathObject[index] = 0;
		if(GetX(objNum) + DirIncrX[info] == x && GetY(objNum) + DirIncrY[info] == y)
			__AtDestination(objNum);
	}
}

static C_1E0F_39B3(int objNum) {
	int si, di;
	int bp_0e, x, y, x_dest, y_dest, dist, dist_min;

	si = -1;
	x = GetX(objNum);
	y = GetY(objNum);
	dist_min = 50;
	for(bp_0e = 0; bp_0e < PartySize; bp_0e ++) {
		di = Party[bp_0e];
		if(ObjShapeType[di] && !IsDead(di) && COMBAT_canSee(objNum, di)) {
			x_dest = GetX(di);
			y_dest = GetY(di);
			dist = (x_dest - x) * (x_dest - x) + (y_dest - y) * (y_dest - y);
			if(
				(dist < dist_min) ||
				(dist == dist_min && OSI_rand(0, 1))
			) {
				dist_min = dist;
				si = di;
			}
		}
	}
	if(si < 0) {
		TryMoveTo(objNum, PartyGravityX, PartyGravityY);
	} else {
		TryMoveTo(objNum, GetX(si), GetY(si));
		if(!CLOSE_ENOUGH0(1, GetX(si), GetY(si), GetX(objNum), GetY(objNum)))
			si = -1;
	}
	return si;
}

/*caught by guard?*/
static C_1E0F_3B60(int bp06) {
	int si, di;
	int bp_06, bp_04, bp_02;

	if(!(TalkFlags[5] & 0x80)) {
		NPCMode[bp06] = AI_FINDPATH;
		return;
	}
	CON_printf(ArrestMsg);
	do
		bp_06 = CON_getch();
	while(bp_06 != 'N' && bp_06 != 'Y');
	NPCMode[bp06] = AI_FINDPATH;
	if(bp_06 == 'Y') {
		CON_printf(GotoJailMsg);
		C_2FC1_19A1();
		if(InCombat)
			COMBAT_breakOff();
		PartyTeleport(0x0e7, 0x0ba, 0, 0);
		while(Time_H != 8)
			C_0A33_1355(60);
		for(di = 0; di < PartySize; di ++) {
			for(bp_04 = Link[Party[di]]; bp_04 >= 0 && GetCoordUse(bp_04) != LOCXYZ && C_1184_1D2E(bp_04, Party[di]); bp_04 = bp_02) {
				bp_02 = Link[bp_04];
				if(GetType(bp_04) == OBJ_03F)
					DeleteObj(bp_04);
				if(GetType(bp_04) == OBJ_040 && GetQual(bp_04) == 9) {
					for(si = FindLoc(0x0eb, 0x0b7, 0); si >= 0 && GetType(si) != OBJ_0B1; si = NextLoc());
					if(si >= 0)
						InsertObj(bp_04, si, CONTAINED);
				}
				for(si = FindLoc(0x0e7, 0x0b8, 0); si >= 0 && GetType(si) != OBJ_12C; si = NextLoc());
				if(si >= 0)
					SetFrame(si, 9);
			}
		}
		ScreenFade = 1;
		ShowObjects();
		MUS_09A8();
		OtherAnimations();
	} else {
		CON_printf(D_356A_02D9);
		C_2337_1B0E();
		NPCMode[bp06] = AI_ASSAULT;
		SetAlignment(bp06, EVIL);
		for(di = 0; di < 0x100; di ++) {
			if(NPCMode[di] == AI_ARREST) {
				NPCMode[di] = AI_ASSAULT;
				SetAlignment(di, EVIL);
			}
		}
	}
	PromptFlag = 1;
}

/*steal gold from ...*/
static C_1E0F_3D8D(int objNum_0, int objNum_1) {
	/*bug, second parameter is a party index*/
	/*should add: objNum_1 = Party[objNum_1];*/
	if(!TestObj(objNum_1, OBJ_058))
		return;
	TakeObj(objNum_1, OBJ_058, OSI_rand(10, 30));
	if((GetInt(objNum_1) + 30 - GetDex(objNum_0)) / 2 <= OSI_rand(1, 30))
		CON_printf(StealMsg, GetObjectString(objNum_0));/*"The %s stole some gold!\n"*/
}

/*talk to ...*/
static C_1E0F_3E08(int bp08, int partyId) {
	TALK_talkTo(partyId, bp08, 0);
	MUS_09A8();
	PromptFlag = 1;
}

static /*C_1E0F_3E27*/__NewPathIndex() {
	int si, di;

	for(si = 0; si < 8; si ++) {
		if(PathObject[si]) {
			di = NPCMode[PathObject[si]];
			if(!(di >= AI_ONPATH  && di <= AI_86))
				PathObject[si] = 0;
		}
		if(PathObject[si] == 0)
			break;
	}
	return si;
}

/*A.I. engine?*/
C_1E0F_3E6A(int objNum) {
	int di;
	int dir, sched_x, sched_y, bp_08, bp_06;
	struct coord sched_xyz;

	if(IsPlrControl(objNum) && !IsCharmed(objNum))
		COMBAT_stopCharm(objNum);
	bp_06 = MovePts[objNum];
	if(SpellFx[14] || GetZ(objNum) != MapZ) {
		MovePts[objNum] = 0;
		return;
	}
	sched_xyz = Schedule[SchedIndex[objNum] + SchedPointer[objNum]].xyz;
	sched_x = GetCoordX(sched_xyz);
	sched_y = GetCoordY(sched_xyz);
	switch(NPCMode[objNum]) {
		case AI_FRONT: COMBAT_AI_Front(objNum); break;
		case AI_REAR: COMBAT_AI_Rear(objNum); break;
		case AI_FLANK: COMBAT_AI_Flank(objNum); break;
		case AI_BERSEK: COMBAT_AI_Bersek(objNum); break;
		case AI_ASSAULT:
		case AI_VIGILANTE: COMBAT_AI_Assault(objNum); break;
		case AI_IMMOBILE: COMBAT_AI_Immobile(objNum); break;
		case AI_TANGLE: COMBAT_AI_Tangle(objNum); break;
		case AI_RETREAT:
		case AI_SHY:
		case AI_FEAR:
		case AI_9A: COMBAT_AI_Retreat(objNum); break;
		case AI_LIKE: COMBAT_AI_Like(objNum); break;
		case AI_UNFRIENDLY: COMBAT_AI_Unfriendly(objNum); break;
		case AI_ARREST:
			if((di = C_1E0F_39B3(objNum)) >= 0)
				C_1E0F_3B60(objNum);
		break;
		case AI_BRAWL:
			if(OSI_rand(0, 3) == 0) {
				di = COMBAT_pickOpponent(objNum);
				if(di >= 0) {
					if(COMBAT_getCathesus(di, sched_x, sched_y) < 5) {
						if(COMBAT_getCathesus(objNum, GetX(di), GetY(di)) < 2) {
							COMBAT_showHit(di);
							TryMoveTo(objNum, sched_x, sched_y);
						} else {
							TryMoveTo(objNum, GetX(di), GetY(di));
						}
						break;
					}
				}
			}
			C_1E0F_33C4(objNum, sched_x, sched_y);
		break;
		case AI_CONVERSE:
		case AI_THIEF:
			if(CLOSE_ENOUGH0_S(5, sched_x, sched_y, MapX, MapY)) {
				if((di = C_1E0F_39B3(objNum)) >= 0) {
					NPCMode[objNum] = AI_LOITER;/*bug?*/
					if(NPCMode[objNum] == AI_THIEF)
						C_1E0F_3D8D(objNum, di);/*steal gold*/
					else
						C_1E0F_3E08(objNum, di);/*talk to*/
				}
			} else {
				C_1E0F_33C4(objNum, sched_x, sched_y);
			}
		break;
		case AI_ONPATH:
		case AI_84:
		case AI_85: __DoOnPath(objNum, sched_x, sched_y); break;
		case AI_SEEKOBJ:
			PTH_Index = __NewPathIndex();
			if(PTH_Index < 8) {
				C_1E0F_2D37(objNum, -1, D_1799[Leader[objNum]]);
				SubMov(objNum, 5);
			} else {
				MovePts[objNum] = 0;
			}
		break;
		case AI_GRAZE:
		case AI_WANDER: C_1E0F_37DB(objNum); break;
		case AI_LOITER:
		case AI_FARM: C_1E0F_33C4(objNum, sched_x, sched_y); break;
		case AI_0F:
		case AI_10:
		case AI_GUARD_N:
		case AI_GUARD_E:
		case AI_GUARD_S:
		case AI_GUARD_W:
			if(OSI_rand(0, 1) == 0) {
				SubMov(objNum, 5);
			} else {
				if(NPCMode[objNum] >= AI_GUARD_N && GetX(objNum) == sched_x && GetY(objNum) == sched_y)
					dir = (NPCMode[objNum] - AI_GUARD_N) << 1;
				else
					dir = GetDirection(objNum);
				bp_08 = MovePts[objNum];
				if(TryStraightMove(objNum, dir, 1) == 0) {
					dir ^= 4;
					MovePts[objNum] = bp_08;
					TryStraightMove(objNum, dir, 1);
				}
			}
		break;
		case AI_RINGBELL:
			if(MustRingBell) {
				MustRingBell = 0;
				MUS_Bell = Time_H % 12;
				if(MUS_Bell == 0)
					MUS_Bell = 12;
				SetTileAnimation(BaseTile[OBJ_0EC], 1);
			}
		default:
			SubMov(objNum, 5);
	}
	if(MovePts[objNum] == bp_06)
		SubMov(objNum, 10);
}

static C_1E0F_4265(int tile, int area_x, int area_y) {
	int bp_02;

	if(_ScratchBuf->PTH_resist[area_y][area_x] == 0xff) {
		if(IsTerrainWet(AreaTiles[area_y][area_x]) && !IsTerrainImpass(tile))
			_ScratchBuf->PTH_resist[area_y][area_x] = (TerrainType[tile] >> 4) + 1;
	} else if(IsTerrainImpass(tile)) {
		_ScratchBuf->PTH_resist[area_y][area_x] = 0xff;
	} else if(!IsTerrainWall(tile)) {
		bp_02 = _ScratchBuf->PTH_resist[area_y][area_x] + (TerrainType[tile] >> 4);
		if(bp_02 > 0xff)
			bp_02 = 0xff;
		_ScratchBuf->PTH_resist[area_y][area_x] = bp_02;
	}
}

static /*C_1E0F_436A*/__ComputeResistance(int __unused__) {
	int area_y, area_x;
	int bp_10, bp_0e, objNum, tile;
	int bp_08, bp_06, objTyp, bp_02;

	bp_08 = (MapX - 0x10) & 0x3f8;
	bp_06 = (MapY - 0x10) & 0x3f8;

	for(bp_10 = 0; bp_10 < AREA_H; bp_10 ++) {
		for(bp_0e = 0; bp_0e < AREA_W; bp_0e ++) {
			tile = AreaTiles[bp_10][bp_0e];
			if(IsTerrainImpass(tile))
				_ScratchBuf->PTH_resist[bp_10][bp_0e] = 0xff;
			else
				_ScratchBuf->PTH_resist[bp_10][bp_0e] = (TerrainType[tile] >> 4) + 1;
		}
	}

	for(objNum = SearchArea(bp_08, bp_06, bp_08 + (AREA_W-1), bp_06 + (AREA_H-1)); objNum >= 0; objNum = NextArea()) {
		area_x = GetX(objNum) - bp_08;
		area_y = GetY(objNum) - bp_06;
		objTyp = GetType(objNum);
		bp_02 = TileFlag[TILE_FRAME(objNum)];
		if(objTyp >= OBJ_129 && objTyp <= OBJ_12C) {/*doors*/
			if(GetFrame(objNum) < 8 && _ScratchBuf->PTH_resist[area_y][area_x] < 0xff)
				_ScratchBuf->PTH_resist[area_y][area_x] ++;
			else
				_ScratchBuf->PTH_resist[area_y][area_x] = 0xff;
			if(bp_02 & TILE_FLAG1_40) {
				if(area_y)
					_ScratchBuf->PTH_resist[area_y - 1][area_x] = 0xff;
			} else {
				if(area_x)
					_ScratchBuf->PTH_resist[area_y][area_x - 1] = 0xff;
			}
		} else if(objTyp == OBJ_116 || objTyp == OBJ_118) {/*passthrough*/
			if(_ScratchBuf->PTH_resist[area_y][area_x] < 0xff)
				_ScratchBuf->PTH_resist[area_y][area_x] ++;
		} else if(objNum >= 0x100) {
			tile = TILE_FRAME(objNum);
			C_1E0F_4265(tile, area_x, area_y);
			if(bp_02 & TILE_FLAG1_80) {
				if(area_x)
					C_1E0F_4265(tile - 1, area_x - 1, area_y);
				if(bp_02 & TILE_FLAG1_40) {
					if(area_y)
						C_1E0F_4265(tile - 2, area_x, area_y - 1);
					if(area_x && area_y)
						C_1E0F_4265(tile - 3, area_x - 1, area_y - 1);
				}
			} else if(bp_02 & TILE_FLAG1_40) {
				if(area_y)
					C_1E0F_4265(tile - 1, area_x, area_y - 1);
			}
		}
	}
}

C_1E0F_464A() {
	int objNum, di;
	int bp_06;
	struct coord xyz;

	di = 1;
	PTH_Index = __NewPathIndex();
	if(PTH_Index == 8)
		return;
	D_17A5 =
	D_17A7 = 0;
	for(objNum = 2; objNum < 0xe0; objNum ++) {
		if(NPCMode[objNum] == AI_FINDPATH) {
			if(D_17A5 < 3)
				C_1E0F_291C(objNum);
			if(NPCMode[objNum] == AI_FINDPATH && D_17A7 < 3) {
				di = 0;
				bp_06 = SchedIndex[objNum] + SchedPointer[objNum];
				xyz = Schedule[bp_06].xyz;
				C_1E0F_2D37(objNum, GetCoordX(xyz), GetCoordY(xyz));
				while(PathObject[PTH_Index] && PTH_Index < 8)
					PTH_Index ++;
				if(PTH_Index == 8)
					break;
			}
		}
	}
	if(di) {
		for(objNum = 2; objNum < 0xe0; objNum ++) {
			if(NPCMode[objNum] == AI_86) {
				NPCMode[objNum] = AI_FINDPATH;
				if(D_17A5 < 3)
					C_1E0F_291C(objNum);
			}
		}
	}
}

static C_1E0F_4746() {
	int i, num;
	int enemy_x, enemy_y, bp_16, bp_14, bp_12, bp_10, bp_0e, bp_0c, bp_0a;
	long sum_x, sum_y;

	/*compute party's gravity center?*/
	sum_x =
	sum_y =
	num = 0;
	for(i = 0; i < PartySize; i ++) {
		if(NPCMode[Party[i]] != AI_FLANK && NPCMode[Party[i]] != AI_BERSEK) {
			num ++;
			sum_x += GetX(Party[i]);
			sum_y += GetY(Party[i]);
		}
	}
	if(num) {
		PartyGravityX = ((PartySize >> 1) + sum_x) / num;
		PartyGravityY = ((PartySize >> 1) + sum_y) / num;
	} else {
		PartyGravityX = MapX;
		PartyGravityY = MapY;
	}
	/*compute enemies's gravity center?*/
	num =
	sum_x =
	sum_y = 0;
	for(i = 1; i < 0x100; i ++) {
		if(ObjShapeType[i] && !IsDead(i) && Is_ATKPLR(i)) {
			enemy_x = GetX(i);
			enemy_y = GetY(i);
			if(NPCMode[i] != AI_RETREAT || CLOSE_ENOUGH0(5, enemy_x, enemy_y, MapX, MapY)) {
				if(CLOSE_ENOUGH0_S(24, enemy_x, enemy_y, MapX, MapY)) {
					num ++;
					sum_x += enemy_x;
					sum_y += enemy_y;
				}
			}
		}
	}
	EnemiesNum = num;
	if(num) {
		EnemiesGravityX = ((num >> 1) + sum_x) / num;
		EnemiesGravityY = ((num >> 1) + sum_y) / num;
		enemy_x = EnemiesGravityX - PartyGravityX;
		enemy_y = EnemiesGravityY - PartyGravityY;
		bp_16 = 0x8000;

		D_EBF7 =
		D_EBF1 =
		D_EBF9 = -1;

		bp_12 =
		bp_14 =
		bp_0e =
		bp_10 = 0;
		for(i = 0; i < 0x100; i ++) {
			if(IsDead(i))
				continue;
			if(NPCMode[i] == AI_FRONT || NPCMode[i] == AI_COMMAND) {
				bp_0c = (GetX(i) - PartyGravityX) * enemy_x + (GetY(i) - PartyGravityY) * enemy_y;
				bp_0a = (GetX(i) - PartyGravityX) * enemy_y - (GetY(i) - PartyGravityY) * enemy_x;
				if(IsPlrControl(i)) {
					if(bp_0c > bp_16) {
						bp_16 = bp_0c;
						D_EBED = i;
					}
					if(bp_0a > bp_14) {
						bp_14 = bp_0a;
						D_EBE9 = i;
					}
					if(bp_0a < bp_12) {
						bp_12 = bp_0a;
						D_EBF5 = i;
					}
				} else {
					if(D_EBF7 < 0)
						D_EBF7 = i;
					if(bp_0a > bp_10) {
						bp_10 = bp_0a;
						D_EBF1 = i;
					}
					if(bp_0a < bp_0e) {
						bp_0e = bp_0a;
						D_EBF9 = i;
					}
				}
			}
		}
	} else {
		D_17B6 = 0;
		EnemiesGravityX = MapX + DirIncrX[GetDirection(Party[0])];
		EnemiesGravityY = MapY + DirIncrY[GetDirection(Party[0])];
	}
	/* */
	if(EnemiesGravityX == PartyGravityX && EnemiesGravityY == PartyGravityY) {
		EnemiesGravityX += DirIncrX[GetDirection(Party[0])];
		EnemiesGravityY += DirIncrY[GetDirection(Party[0])];
	}
}

/*requiem*/
int D_1862[] = {0x12C,0x119,0x12C,0x0FA,0x119,0x0DE,0x0FA,0x0FA};

/*death/revive?*/
static C_1E0F_4AFA() {
	int si;

	if(Active == 0x10)
		return;
	MUS_091A(0);
	CON_printf(D_356A_032D);
	for(si = 0; si < 8; si ++)
		OSI_playWavedNote(D_1862[si], 1, 20000, 1, 3);
	D_0340 = 1;
	C_2FC1_19A1();
	OSI_delay(30);
	CON_printf(D_356A_0354);
	C_101C_0ACC();
}

C_1E0F_4B6A() {
	int si;

	if(Active > PartySize)
		return;
	if(IN_VEHICLE) {
		Rst_0044(0);
		Rstbis_001e(0);
		ClrDraggedUnder(0);
		SetAlignment(0, GOOD);
	}
	if(IsDead(1))
		C_1E0F_4AFA();
	if(D_2CC3 >= 0) {
		if(Isbis_0016(Party[Active]) || IsDraggedUnder(Party[Active]) || IsCharmed(Party[Active]))
			SetPartyMode();
	}
	if(D_2CC3 >= 0)
		return;
	if(
		Isbis_0016(Party[Active]) ||
		IsDraggedUnder(Party[Active]) ||
		IsCharmed(Party[Active])
	) {
		for(si = 0; si < PartySize; si ++) {
			if(
				Isbis_0016(Party[si]) ||
				IsDraggedUnder(Party[si]) ||
				IsCharmed(Party[si])
			) continue;
			if(NPCMode[Party[si]] != AI_MOTIONLESS) {
				NPCMode[Party[Active]] = AI_FOLLOW;
				NPCMode[Party[si]] = AI_COMMAND;
				Active =
				D_EBF3 = si;
				PromptFlag = 1;
				MapX = GetX(Party[si]);
				MapY = GetY(Party[si]);
				C_101C_054C();
				break;
			}
		}
	}
	if(Active < PartySize && Active && D_2CC3 < 0) {
		if(
			Isbis_0016(Party[0]) ||
			IsDraggedUnder(Party[0]) ||
			IsCharmed(Party[0])
		) return;
		if(InCombat) {
			NPCMode[Party[Active]] = NPCComMode[Party[Active]];
		} else {
			NPCMode[Party[Active]] = AI_FOLLOW;
			Active =
			D_EBF3 = 0;
			PromptFlag = 1;
			MapX = GetX(Party[0]);
			MapY = GetY(Party[0]);
			C_101C_054C();
		}
		NPCMode[Party[0]] = AI_COMMAND;
	}
}

C_1E0F_4E0A() {
	int i, di;
	int area_x, area_y, bp_08, bp_06, bp_04, bp_02;
	
	C_1E0F_4B6A();
	C_1E0F_4746();
	C_1E0F_464A();
	bp_08 = Party[Active];
	if(NPCMode[bp_08] > AI_FOLLOW && NPCMode[bp_08] < AI_10)
		Leader[bp_08] = bp_08;
	if(MapZ)
		bp_02 = 0xff;
	else
		bp_02 = 0x3ff;

	do {
		bp_08 = -1;
		di = 0;
		bp_06 = 1;

		do {
			area_x = (MapX - 0x10) & 0x3f8;
			area_y = (MapY - 0x10) & 0x3f8;

			for(i = 1; i < PartySize; i ++) {
				if(NPCMode[Party[i]] == AI_FOLLOW && MovePts[Party[i]] <= 0)
					MovePts[Party[0]] = 0;
			}

			for(i = 0; i < 0x100; i ++) {
				if(
					ObjShapeType[i] &&
					NPCMode[i] != AI_MOTIONLESS &&
					!IsParalyzed(i) &&
					(!IsAsleep(i) || NPCMode[i] == AI_SCHEDULE) &&
					!IsDead(i) &&
					MovePts[i] > 0 &&
					GetCoordUse(i) == LOCXYZ && GetZ(i) == MapZ
				) {
					if(
						((GetX(i) - area_x) & bp_02) > (AREA_W-1) ||
						((GetY(i) - area_y) & bp_02) > (AREA_H-1)
					) {
						if(NPCMode[i] >= AI_ONPATH && NPCMode[i] <= AI_86)
							C_1E0F_291C(i);
						if(NPCMode[i] >= AI_ONPATH && NPCMode[i] < AI_86) {
							PathObject[Leader[i]] = 0;
							NPCMode[i] = AI_FINDPATH;
						}
						MovePts[i] = 0;
					} else if(NPCMode[i] != AI_FOLLOW) {
						if(NPCMode[i] == AI_SCHEDULE)
							__AtDestination(i);
						bp_04 = MovePts[i] * bp_06 - DEXTE[i] * di;
						if(
							MovePts[i] >= DEXTE[i] || bp_04 > 0 ||
							(bp_04 == 0 && DEXTE[i] > bp_06)
						) {
							bp_08 = i;
							di = MovePts[i];
							bp_06 = DEXTE[i];
						}
						if(MovePts[i] >= DEXTE[i])
							break;
					}
				}
			}

			if(di <= 0) {
				for(i = 0; i < 0x100; i ++) {
					MovePts[i] = (MovePts[i] < 0)?(MovePts[i] + DEXTE[i]):DEXTE[i];
				}
				C_0A33_1355(1);
			}

		} while(di <= 0);

		if(!C_1E0F_0FA9(bp_08)) {
			if(NPCMode[bp_08] != AI_COMMAND && NPCMode[bp_08] != AI_FOLLOW) {
				C_1E0F_3E6A(bp_08);
				C_1E0F_4B6A();
				if(NPCMode[bp_08] > AI_FOLLOW && NPCMode[bp_08] < AI_10)
					Leader[bp_08] = bp_08;
			}
			C_0A33_0D06();
		}

	} while(NPCMode[bp_08] != AI_COMMAND || IsDraggedUnder(bp_08));

	if(IN_VEHICLE)
		bp_08 = 0;
	if(Party[Active] != bp_08) {
		for(i = 0; i < PartySize; i ++) {
			if(Party[i] == bp_08) {
				SetActive(i);
				PromptFlag = 1;
				break;
			}
		}
	}
}

C_1E0F_512A() {
	int si;

	for(si = 0; si < 0x100; si ++) {
		if(DEXTE[si] == 0)
			DEXTE[si] = 1;
	}
	if(NPCMode[0] >= AI_SCHEDULE)
		NPCMode[0] = AI_MOTIONLESS;
	if(D_2CC3 < 0)
		D_EBF3 = Active;
	C_1E0F_4746();
}

/*NPC update*/
C_1E0F_5165() {
	int si, di;
	int bp_06, bp_04, bp_02;

	for(si = 2; si < 0xe0; si ++) {
		if(NPCMode[si] == AI_VIGILANTE)
			NPCMode[si] = AI_FINDPATH;
		if(ObjShapeType[si] == 0)
			continue;
		if(NPCMode[si] < AI_SCHEDULE && NPCMode[si] != AI_RETREAT)
			continue;
		if(IsPlrControl(si))
			continue;
		bp_06 = SchedPointer[si];
		bp_04 = SchedPointer[si + 1] - 1;
		if(bp_04 < bp_06)
			continue;
		for(di = bp_04; di >= bp_06; di --) {
			bp_02 = Schedule[di].time;
			if((bp_02 & 0x1f) == Time_H)
			if((bp_02 >> 5) == ((Date_D - 1) % 7 + 1) || (bp_02 & 0xe0) == 0) {
				SchedIndex[si] = di - bp_06;
				NPCMode[si] = AI_FINDPATH;
				if(GetType(si) == OBJ_188 && Schedule[di].action != AI_PLAY) {
					ObjShapeType[si] = OrigShapeType[si];
					C_1E0F_0664(si, 4);
				}
				break;
			}
		}
	}
}
