/*
 * Ultima VI - The False Prophet
 * Copyright (c) 1990 Origin Systems, Inc. and Richard Gariott
 * reverse-coded by Ergonomy Joe in 2013
 */
/*
	OBJECT.C module
*/
#include "u6.h"

static int D_0654 = 0;
int D_0656 = 0;
int D_0658 = 0;/*related to FindLoc*/
static int D_065A = 0;/*related to "Amulet of submission"*/
int D_065C[8*8 + 5] = {0};/*has tmp file?*/
int D_06E6 = 0;
int D_06E8[4] = {-1, -1, -1, -1};
int D_06F0 = -1;/*mask:__________yyyxxx?*/
int D_06F2 = -1;
static int D_06F4 = -1;
static int D_06F6 = 0;
static struct coord D_06F8 = {0, 0, 0};
int D_06FB = -1;
int D_06FD = -1;
int D_06FF = -1;
int D_0701 = -1;/*Quality*/
int D_0703 = -1;
int D_0705 = -1;
int D_0707 = -1;

/*0709*/static char __Sav_file[] = "savegame\\objblkaa";
/*071B*/static char __Tmp_file[] = "savegame\\objblkaa.tmp";
/*0731*/static char __Bak_file[] = "savegame\\objblkaa.bak";

unsigned char D_0747[] = {~TERRAIN_FLAG_80,0,~TERRAIN_FLAG_40,0,~TERRAIN_FLAG_20,0,~TERRAIN_FLAG_10};
unsigned char D_074E[] = {0x00,0x07,0x05,0x0C,0x03,0x09,0x0B,0x00,0x01,0x0D,0x08,0x00,0x0A,0x00,0x00,0x00};
unsigned char D_075E[] = {TERRAIN_FLAG_80,0,TERRAIN_FLAG_40,0,TERRAIN_FLAG_20,0,TERRAIN_FLAG_10};

/*uncalled?*/
__1184_000E(int x, int y, int z) {
	int si, di;
	int mapId;

	si = 0;
	if(z) {
		si = (z == MapZ);
	} else if(MapZ == 0) {
		mapId = (x >> 7) + ((y >> 4) & 0x38);
		for(di = 0; di < 4; di ++)
			if(D_06E8[di] == mapId) {
				si = 1;
				break;
			}
	}
	return si;
}

/*C_1184_0073*/OBJ_MakeDirFrame(int objTyp, int dir) {
	if(
		objTyp == OBJ_0FC || objTyp == OBJ_0DD ||
		objTyp == OBJ_162 || objTyp == OBJ_167 ||
		objTyp == OBJ_184
	) return dir >> 1;

	if(
		objTyp == OBJ_15C || objTyp == OBJ_15D || objTyp == OBJ_15E || objTyp == OBJ_15F ||
		objTyp == OBJ_166 || objTyp == OBJ_169 || objTyp >= OBJ_1AA || objTyp == OBJ_19D ||
		(objTyp >= OBJ_16F && objTyp <= OBJ_174) || objTyp == OBJ_156 || objTyp == OBJ_15A ||
		objTyp == OBJ_188
	) return dir;

	if(objTyp == OBJ_16A)
		return  ((dir << 1) + (dir << 2)) + 7;

	if(objTyp == OBJ_1A8)
		return (dir << 2) + 3;

	if(objTyp == OBJ_16B)
		return (dir >> 1) + dir + 1;

	if(
		(objTyp >= OBJ_178 && objTyp <= OBJ_183) ||
		(objTyp >= OBJ_199 && objTyp <= OBJ_19A)
	) return (dir << 1) + 1;

	return 0;
}

/*C_1184_0153*/OBJ_getHead(int objNum) {
	int i;
	int x, y, imped, imped_2;
	int objTyp, ret;

	if(objNum < 0)
		return -1;
	ret = objNum;
	objTyp = GetType(ret);
	if(objTyp >= OBJ_1AA || objTyp == OBJ_19B || objTyp == OBJ_19C) {
		x = GetX(objNum);
		y = GetY(objNum);
		imped = TerrainType[TILE_FRAME(objNum)] & 0xf0;
		if(imped == TERRAIN_FLAG_80) {
			y --;
			imped_2 = TERRAIN_FLAG_20;
		} else if(imped == TERRAIN_FLAG_40) {
			x ++;
			imped_2 = TERRAIN_FLAG_10;
		} else if(imped == TERRAIN_FLAG_20) {
			y ++;
			imped_2 = TERRAIN_FLAG_80;
		} else if(imped == TERRAIN_FLAG_10) {
			x --;
			imped_2 = TERRAIN_FLAG_40;
		} else {
			return objNum;
		}
		for(i = FindLoc(x, y, MapZ); i >= 0; i = NextLoc()) {
			if(GetType(i) == objTyp && (TerrainType[TILE_FRAME(i)] & imped_2)) {
				ret = i;
				break;
			}
		}
	} else if(
		objTyp == OBJ_19D &&
		objNum >= 0x100 &&
		GetType(GetQuan(objNum)) == OBJ_19D
	) {
		ret = GetQuan(objNum);
	}
	return ret;
}

/*C_1184_02E3*/static ClearSpace(int num) {
	if(D_E6E0 >= num)
		return D_D5DA;
	return -1;
}

static C_1184_02FA(int x, int y, int bp06) {
	int i, j;
	struct coord xyz;
	int bp_0c, bp_0a, bp_08, bp_06;
	int some_x, some_y;

	some_x = (x - AreaX) & 0x3ff;
	some_y = (y - AreaY) & 0x3ff;
	if(some_x < AREA_W && some_y < AREA_H) {
		j = AREA_W * some_y + some_x;
		if(bp06) {
			/*first non null object before j*/
			for(i = j - 1; i >= 0 && ((int *)MapObjPtr)[i] == -1; i --);
			/*first non null object after j*/
			for(; j < 0x63f && ((int *)MapObjPtr)[j] == -1; j ++);

			if(i >= 0)
				i = ((int *)MapObjPtr)[i];
			else
				i = 0x100;

			for(j = ((int *)MapObjPtr)[j]; (bp_08 = Link[i]) >= 0 && bp_08 != j; i = bp_08) {
				if(GetCoordUse(bp_08) == LOCXYZ) {
					bp_06 = GetY(bp_08);
					if(
						(bp_06 > y) ||
						((bp_06 == y) && (GetX(bp_08) >= x))
					) break;
				}
			}
		} else {
			i = ((int *)MapObjPtr)[j];
			if(i < 0) {
				for(; j < 0x63f && ((int *)MapObjPtr)[j] == -1; j ++);
				i = ((int *)MapObjPtr)[j];
			}
		}
	} else {
		SetCoordXYZ(xyz, x, y, 0);
		if(
			((AreaY + (AREA_H-1)) & 0x3ff) < y || (
			((AreaY + (AREA_W-1)) & 0x3ff) == y && ((AreaX + (AREA_W-1)) & 0x3ff) < x
		)) {
			for(i = 0x63f; i >= 0 && ((int *)MapObjPtr)[i] == -1; i --);
			if(i < 0)
				bp_0a = 0x100;
			else
				bp_0a = ((int *)MapObjPtr)[i];
		} else {
			bp_0a = 0x100;
		}
		for(bp_0c = Link[bp_0a]; bp_0c >= 0; bp_0a = bp_0c, bp_0c = Link[bp_0c]) {
			if(GetCoordUse(bp_0c) == LOCXYZ) {
				/*strange?!? considers X/Y as a 24bit integer?
				i.e if((ObjPos[bp_0c].24bit) & 0xfffff >= xyz.24bit) break;*/
				if((ObjPos[bp_0c].raw[2] & 0xf) > xyz.raw[2])
					break;
				if((ObjPos[bp_0c].raw[2] & 0xf) == xyz.raw[2]) {
					if(ObjPos[bp_0c].raw[1] > xyz.raw[1])
						break;
					if(ObjPos[bp_0c].raw[1] == xyz.raw[1] && ObjPos[bp_0c].raw[0] >= xyz.raw[0])
						break;
				}
			}
		}
		i = bp_0a;
		if(!bp06)
			i = Link[i];
	}
	return i;
}

/*C_1184_053F*/NextLoc() {
	struct coord *si;
	int di;
	struct coord loc_down, loc_right, loc_dn_rt;
	int bp_08, loc_x, loc_y, loc_z;
	
	di = -1;
	loc_x = GetCoordX(D_06F8);
	loc_y = GetCoordY(D_06F8);
	loc_z = GetCoordZ(D_06F8);

	loc_dn_rt =
	loc_down =
	loc_right = D_06F8;

	SetCoordX(loc_right, loc_x + 1);
	SetCoordY(loc_down, loc_y + 1);
	SetCoordX(loc_dn_rt, loc_x + 1);
	SetCoordY(loc_dn_rt, loc_y + 1);
	while(D_06F4 >= 0 && di < 0) {
		if(GetCoordUse(D_06F4) == LOCXYZ && GetZ(D_06F4) == loc_z) {
			si = &(ObjPos[D_06F4]);
			bp_08 = TILE_FRAME(D_06F4);
			if(
				si->raw[0] == D_06F8.raw[0] &&
				si->raw[1] == D_06F8.raw[1] &&
				si->raw[2] == D_06F8.raw[2]
			) {
				di = D_06F4;
			} else if(
				si->raw[0] == loc_right.raw[0] &&
				si->raw[1] == loc_right.raw[1] &&
				si->raw[2] == loc_right.raw[2]
			) {
				if(IsTileDoubleH(bp_08)) {
					di = D_06F4;
					D_0658 = 1;
				}
			} else if(
				si->raw[0] == loc_down.raw[0] &&
				si->raw[1] == loc_down.raw[1] &&
				si->raw[2] == loc_down.raw[2]
			) {
				if(IsTileDoubleV(bp_08)) {
					di = D_06F4;
					D_0658 = IsTileDoubleH(bp_08)?2:1;
				}
			} else if(
				si->raw[0] == loc_dn_rt.raw[0] &&
				si->raw[1] == loc_dn_rt.raw[1] &&
				si->raw[2] == loc_dn_rt.raw[2]
			) {
				if((TileFlag[bp_08] & (TILE_FLAG1_80|TILE_FLAG1_40)) == (TILE_FLAG1_80|TILE_FLAG1_40)) {
					di = D_06F4;
					D_0658 = 3;
				}
			} else if(!D_06F6) {
				D_06F4 = C_1184_02FA(loc_x, loc_y + 1, 0);
				D_06F6 = 1;
				continue;
			} else {
				D_06F4 = -1;
				break;
			}
		}
		D_06F4 = Link[D_06F4];
	}
	return di;
}

/*C_1184_075A*/FindLoc(int x, int y, int z) {
	if(z) {
		x &= 0xff;
		y &= 0xff;
	}
	D_0658 =
	D_06F6 = 0;
	D_06F4 = C_1184_02FA(x, y, 0);
	SetCoordXYZ(D_06F8, x, y, z);
	return NextLoc();
}

C_1184_07A7(int objTyp, int x, int y) {
	int si, di;

	di = -1;
	for(
		si = C_1184_02FA(x, y, 0);
		si >= 0 && (
			GetCoordUse(si) != LOCXYZ ||
			(GetX(si) == x && GetY(si) == y)
		);
		si = Link[si]
	) {
		if(GetCoordUse(si) == LOCXYZ && GetZ(si) == MapZ && GetType(si) == objTyp) {
			di = si;
			break;
		}
	}

	return di;
}

/*C_1184_0840*/FindInv(int objNum) {
	int si, di;

	di = -1;
	si = Link[objNum];
	if(GetCoordUse(si) != LOCXYZ && GetAssoc(si) == objNum) {
		di = si;
		D_06FB = Link[si];
		D_06FD = objNum;
		D_06FF = GetType(di);
		D_0701 = -1;
	}
	return di;
}

/*C_1184_0899*/NextInv() {
	int si;

	for(si = -1; D_06FB >= 0 && si < 0; D_06FB = Link[D_06FB]) {
		if(GetCoordUse(D_06FB) == LOCXYZ) {
			D_06FB = -1;
			break;
		}
		if(GetAssoc(D_06FB) == D_06FD) {
			if(D_0656 == 0 || GetCoordUse(D_06FB) == INVEN || GetCoordUse(D_06FB) == CONTAINED)
				si = D_06FB;
		}
	}
	return si;
}

/*C_1184_090D*/NextArea() {
	int si, di;
	int bp_04, bp_02;

	si = -1;
	if(D_0703 >= 0) {
		for(; D_0703 != D_0705 && D_0703 >= 0 && si < 0; D_0703 = Link[D_0703]) {
			if(GetCoordUse(D_0703) == LOCXYZ) {
				di = GetX(D_0703) & D_05CA;
				bp_04 = GetY(D_0703) & D_05CA;
				bp_02 = GetZ(D_0703);
				if(
					di >= D_BDCE[0] && di <= D_BDCE[2] &&
					bp_04 >= D_BDCE[1] && bp_04 <= D_BDCE[3] &&
					bp_02 == D_BDD8
				) si = D_0703;
			}
		}
		if(D_0703 == D_0705)
			D_0703 = D_0705 = -1;
	}
	return si;
}

/*C_1184_09DE*/SearchArea(int x1, int y1, int x2, int y2) {
	if(x1 < 0)
		x1 = 0;
	if(y1 < 0)
		y1 = 0;
	D_BDCE[0] = x1;
	D_BDCE[2] = x2;
	D_BDCE[1] = y1;
	D_BDCE[3] = y2;
	D_BDD8 = MapZ;
	D_0703 = C_1184_02FA(x1, y1, 0);
	D_0705 = C_1184_02FA(x2 + 1, y2, 0);
	return NextArea();
}

/*C_1184_0A37*/NextInvType() {
	int si;

	for(si = -1; D_06FB >= 0 && si < 0; D_06FB = Link[D_06FB]) {
		if(GetCoordUse(D_06FB) == LOCXYZ) {
			D_06FB = -1;
			break;
		}
		if(GetAssoc(D_06FB) == D_06FD && GetType(D_06FB) == D_06FF) {
			if(
				(D_0701 < 0 || GetQual(D_06FB) == D_0701) &&
				(D_0656 == 0 || GetCoordUse(D_06FB) == INVEN || GetCoordUse(D_06FB) == CONTAINED)
			) si = D_06FB;
		}
	}
	return si;
}


/*C_1184_0AE2*/FindInvType(int objNum, int objType, int quality) {
	int si, di;

	if(objNum < 0)
		return -1;
	di = -1;
	si = Link[objNum];
	if(GetCoordUse(si) != LOCXYZ && GetAssoc(si) == objNum) {
		di = si;
		D_06FB = si;
		D_06FD = objNum;
		D_06FF = objType;
		D_0701 = quality;
		di = NextInvType();
	}
	return di;
}

/*find a "living" object at x, y*/
C_1184_0B3D(int objNum, int x, int y) {
	int i, ret;

	ret = -1;
	if(objNum < 1)
		objNum = 1;
	for(i = objNum; i < 0x100; i ++) {
		if(
			ObjShapeType[i] &&
			!IsDead(i) &&
			GetX(i) == x &&
			GetY(i) == y &&
			GetZ(i) == MapZ
		) {
			ret = i;
			break;
		}
	}
	return ret;
}

/*SlimeRefreshFrame?*/
C_1184_0BC1() {
	int si, di;
	int bp_0e, x, y, new_x, new_y, bp_04, bp_02;

	for(si = 0xe0; si < 0x100; si ++) {
		if(GetType(si) == OBJ_177 && !IsDead(si)) {
			x = GetX(si);
			y = GetY(si);
			bp_02 = 1;
			bp_04 = 0;
			for(bp_0e = 0; bp_0e < 8; bp_02 <<= 1, bp_0e +=2) {
				new_x = x + DirIncrX[bp_0e];
				new_y = y + DirIncrY[bp_0e];
				for(
					di = C_1184_0B3D(0xe0, new_x, new_y);
					di >= 0;
					di = C_1184_0B3D(di + 1, new_x, new_y)
				) {
					if(GetType(di) == OBJ_177 && !IsDead(di)) {
						bp_04 |= bp_02;
						break;
					}
				}
			}
			SetFrame(si, bp_04);
		}
	}
}

/*silver serpent related?*/
static C_1184_0CBE(int objNum, int x, int y, int bp06) {
	int si;
	int bp_1a, bp_18, bp_16, bp_14, bp_12;
	int bp_10;
	int bp_0e, bp_0c, bp_0a, bp_08, bp_06;
	int bp_04, bp_02;

	bp_0c = bp_0e = objNum;
	bp_1a = x;
	bp_18 = y;
	bp_0a = TerrainType[TILE_FRAME(objNum)];
	if(bp_0a & TERRAIN_FLAG_80)
		bp_08 = 4;
	else if(bp_0a & TERRAIN_FLAG_40)
		bp_08 = 6;
	else if(bp_0a & TERRAIN_FLAG_20)
		bp_08 = 0;
	else if(bp_0a & TERRAIN_FLAG_10)
		bp_08 = 2;
	bp_06 = bp_08;
	bp_16 = GetX(objNum);
	bp_14 = GetY(objNum);
	bp_12 = GetZ(objNum);
	if(bp_16 > x) {
		bp_06 = 2;
		bp_0a |= TERRAIN_FLAG_40;
	} else if(bp_16 < x)  {
		bp_06 = 6;
		bp_0a |= TERRAIN_FLAG_10;
	} else if(bp_14 > y) {
		bp_06 = 4;
		bp_0a |= TERRAIN_FLAG_20;
	} else if(bp_14 < y) {
		bp_06 = 0;
		bp_0a |= TERRAIN_FLAG_80;
	}
	SetFrame(objNum, bp_06);
	bp_10 = GetQual(objNum) + 1;
	do {
		for(
			si = FindLoc(bp_1a - DirIncrX[bp_08], bp_18 - DirIncrY[bp_08], bp_12);
			si >= 0;
			si = NextLoc()
		) {
			if(
				si >= 0x100 &&
				GetType(si) == OBJ_19D &&
				GetQual(si) == bp_10 &&
				GetQuan(si) == objNum
			) {
				bp_0c = bp_0e;
				bp_0e = si;
				bp_02 = GetFrame(si);
				bp_10 ++;
				SetQual(si, bp_10);
				bp_1a = GetX(si);
				bp_18 = GetY(si);
				bp_06 = bp_08;
				bp_04 = TerrainType[TILE_FRAME(si)] & D_0747[bp_08];
				if(bp_04 & TERRAIN_FLAG_80)
					bp_08 = 4;
				else if(bp_04 & TERRAIN_FLAG_40)
					bp_08 = 6;
				else if(bp_04 & TERRAIN_FLAG_20)
					bp_08 = 0;
				else if(bp_04 & TERRAIN_FLAG_10)
					bp_08 = 2;
				if(bp06)
					DeleteObj(si);
				break;
			}
		}
	} while(si >= 0 && (bp_02 & 0xf) >= 8);
	if(bp06)
		return;

	if(bp_0c != objNum) {
		bp_04 = TerrainType[TILE_FRAME(bp_0c)];
		bp_04 &= D_0747[(GetFrame(bp_0e) & 0xf) ^ 5];
		if(bp_04 & TERRAIN_FLAG_80)
			SetFrame(bp_0c, 1);
		else if(bp_04 & TERRAIN_FLAG_40)
			SetFrame(bp_0c, 3);
		else if(bp_04 & TERRAIN_FLAG_20)
			SetFrame(bp_0c, 5);
		else if(bp_04 & TERRAIN_FLAG_10)
			SetFrame(bp_0c, 7);
	}
	if(bp_0e == objNum)
		return;
	if(bp_0c != objNum)
		SetFrame(bp_0e, D_074E[bp_0a >> 4]);
	else
		SetFrame(bp_0e, GetFrame(objNum) + 1);
	MoveObj(bp_0e, x, y, bp_12);
	SetQual(bp_0e, GetQual(objNum) + 1);
}

/*dragon related*/
C_1184_10F1(int x, int y, int z, int dir, int bp06) {
	int si;
	int bp_02;

	bp_02 = D_17CE[(dir >> 1) + ((bp06 & 6) << 1)];
	for(
		si = FindLoc(x + DirIncrX[dir], y + DirIncrY[dir], z);
		si >= 0;
		si = NextLoc()
	) {
		if(GetType(si) == OBJ_19B && (GetFrame(si) & 0x3e) == bp_02)
			return si;
	}
	return -1;
}

/*big creatures related*/
C_1184_116D(int objNum, int x, int y, int bp06) {
	int si, di;
	int bp_0e, bp_0c, bp_0a, bp_08, bp_06, bp_04, bp_02;

	bp_0e = GetX(objNum);
	bp_0c = GetY(objNum);
	bp_0a = GetZ(objNum);
	bp_06 = TerrainType[TILE_FRAME(objNum)];
	bp_08 = GetType(objNum);
	if(bp_08 == OBJ_176)
		bp_08 = OBJ_1A9;
	if(bp_08 == OBJ_19B)
		bp_02 = (GetFrame(objNum) & 6) >> 1;
	for(si = 0; si < 8; si ++) {
		if(
			bp_08 == OBJ_1A9 ||
			((si & 1) == 0 && (bp_06 & D_075E[si]))
		) {
			if(bp_08 == OBJ_19B) {
				di = C_1184_10F1(x, y, bp_0a, si, bp_02 << 1);
			} else {
				for(
					di = FindLoc(DirIncrX[si] + x, DirIncrY[si] + y, bp_0a);
					di >= 0;
					di = NextLoc()
				) {
					bp_04 = TerrainType[TILE_FRAME(di)];
					if(di >= 0x100 && GetType(di) == bp_08) {
						if(bp_08 == OBJ_1A9 && (GetFrame(di) >> 2) == si)
							break;
						if(bp_08 != OBJ_1A9 && (bp_04 & D_075E[si ^ 4]))
							break;
					}
				}
			}
			if(di < 0)
				continue;
			if(bp06)
				DeleteObj(di);
			else
				MoveObj(di, DirIncrX[si] + bp_0e, DirIncrY[si] + bp_0c, bp_0a);
		}
	}
}

/*C_1184_134C*/AddObj(int status, struct coord location, int shapeType, int shapeNum, int amount) {
	if((status & 0x18) == LOCXYZ)
		AddMapObj(shapeType | (shapeNum << 10), status, amount, GetCoordX(location), GetCoordY(location), GetCoordZ(location));
	else
		AddInvObj(shapeType | (shapeNum << 10), status, amount, GetCoordAssoc(location));
}

/*C_1184_13A7*/AddMapObj(int shapeType, int status, int amount, int x, int y, int z) {
	int objNum;
	int bp_04, bp_02;
	
	bp_02 = z?0xff:0x3ff;
	x &= bp_02;
	y &= bp_02;
	objNum = ClearSpace(1);
	if(objNum >= 0) {
		bp_04 = C_1184_02FA(x, y, 1);
		D_D5DA = Link[D_D5DA];
		D_E6E0 --;
		ObjStatus[objNum] = status | LOCAL;
		SetXYZ(objNum, x, y, z);
		ObjShapeType[objNum] = shapeType;
		Amount[objNum] = amount;
		Link[objNum] = Link[bp_04];
		Link[bp_04] = objNum;
		x = (x - AreaX) & 0x3ff;
		y = (y - AreaY) & 0x3ff;
		if(x < AREA_W && y < AREA_H)
			MapObjPtr[y][x] = objNum;
	}
	return objNum;
}

/*C_1184_14A8*/AddInvObj(int bp0c, int status, int amount, int di/*bp06*/) {
	int objNum;

	if(StatusDisplay == CMD_92 && Party[D_04B3] == di)
		StatusDirty = 1;
	objNum = ClearSpace(1);
	if(objNum >= 0) {
		D_D5DA = Link[D_D5DA];
		D_E6E0 --;
		ObjStatus[objNum] = status;
		if(bp0c != OBJ_18C)
			SetLocal(objNum);
		SetAssoc(objNum, di);
		ObjShapeType[objNum] = bp0c;
		Amount[objNum] = amount;
		Link[objNum] = Link[di];
		Link[di] = objNum;
	}
	return objNum;
}

/*C_1184_1540*/AddMonster(int objType, int x, int y, int z) {
	int objNum;
	int bp_04, bp_02;

	bp_02 = z?0xff:0x3ff;
	x &= bp_02;
	y &= bp_02;
	objNum = -1;
	if(D_E6E2)
		objNum = D_BDD6;
	if(objNum >= 0) {
		bp_04 = C_1184_02FA(x, y, 1);
		D_BDD6 = Link[D_BDD6];
		D_E6E2 --;
		ObjStatus[objNum] = LOCAL | OWNED;
		NPCStatus[objNum] = 0;
		NPCMode[objNum] = AI_ASSAULT;
		SetXYZ(objNum, x, y, z);
		ObjShapeType[objNum] =
		OrigShapeType[objNum] = TypeFrame(objType, 0);
		Amount[objNum] = 1;
		Link[objNum] = Link[bp_04];
		Link[bp_04] = objNum;
		x = (x - AreaX) & 0x3ff;
		y = (y - AreaY) & 0x3ff;
		if(x < AREA_W && y < AREA_H)
			MapObjPtr[y][x] = objNum;
		if(objType == OBJ_177)
			C_1184_0BC1();
	}
	return objNum;
}

static C_1184_1665(int objNum) {
	int ret;

	if(GetCoordUse(objNum) == LOCXYZ)
		ret = C_1184_02FA(GetX(objNum), GetY(objNum), 1);
	else
		ret = GetAssoc(objNum);
	for(; ret >= 0 && Link[ret] != objNum; ret = Link[ret]);

	return ret;
}

static C_1184_16D5(int x, int y, int z, int bp08, int bp06) {
	int si, di;
	int bp_02;

	if(GetCoordUse(bp08) == LOCXYZ) {
		bp_02 = z?0xff:0x3ff;
		si = (x - AreaX) & bp_02;
		di = (y - AreaY) & bp_02;
		if(si < AREA_W && di < AREA_H && MapObjPtr[di][si] == bp08) {
			if(GetCoordUse(bp06) == LOCXYZ && (GetX(bp06) != x || GetY(bp06) != y))
				MapObjPtr[di][si] = -1;
			else
				MapObjPtr[di][si] = bp06;
		}
	}
}

/*C_1184_17A5*/DeleteObj(int objNum) {
	int di;
	int bp_04, bp_02;

	if(objNum < 0)
		return;
	D_0707 = -1;
	if(StatusDisplay == CMD_92 && C_1184_1D2E(objNum, Party[D_04B3]))
		StatusDirty = 1;
	di = GetType(objNum);
	if(di == OBJ_19C)
		return;
	if(objNum < 0x100) {
		if(di == OBJ_176 || di == OBJ_19C || di >= OBJ_1AA || di == OBJ_19B)
			C_1184_116D(objNum, GetX(objNum), GetY(objNum), 1);
		else if(di == OBJ_19D)
			C_1184_0CBE(objNum, GetX(objNum), GetY(objNum), 1);
	}
	while((bp_04 = FindInv(objNum)) >= 0) {
		D_0707 = bp_04;
		if(GetCoordUse(objNum) == LOCXYZ)
			MoveObj(bp_04, GetX(objNum), GetY(objNum), GetZ(objNum));
		else
			InsertObj(bp_04, GetAssoc(objNum), GetCoordUse(objNum));
	}
	ObjShapeType[objNum] = 0;
	bp_02 = C_1184_1665(objNum);
	if(bp_02 >= 0) {
		Link[bp_02] = Link[objNum];
		if(objNum < 0x100) {
			Link[objNum] = D_BDD6;
			D_BDD6 = objNum;
			D_E6E2 ++;
		} else {
			Link[objNum] = D_D5DA;
			D_D5DA = objNum;
			D_E6E0 ++;
		}
	}
	C_1184_16D5(GetX(objNum), GetY(objNum), GetZ(objNum), objNum, Link[bp_02]);
	if(di == OBJ_177)
		C_1184_0BC1();
}

C_1184_19AA(int x, int y, int z) {
	int si;
	int bp_08, bp_06, obj_x, obj_y, obj_z;

	x &= D_05CA;
	y &= D_05CA;
	/*delete local objects*/
	for(si = 0xe0; si < 0x100; si ++) {
		if(ObjShapeType[si]) {
			obj_x = GetX(si);
			obj_y = GetY(si);
			obj_z = GetZ(si);
			if(
				x - 20 > obj_x || x + 20 <= obj_x ||
				y - 20 > obj_y || y + 20 <= obj_y ||
				obj_z != z
			) {
				if(IsLocal(si))
					DeleteObj(si);
			}
		}
	}
	/*reset eggs*/
	for(si = Link[0x100]; si >= 0; si = bp_06) {
		bp_06 = Link[si];
		if(GetCoordUse(si) == LOCXYZ) {
			obj_x = GetX(si);
			obj_y = GetY(si);
			obj_z = GetZ(si);
			if(
				x - 20 > obj_x || x + 20 <= obj_x ||
				y - 20 > obj_y || y + 20 <= obj_y ||
				obj_z != z
			) {
				bp_08 = GetType(si);
				if(IsLocal(si) || ObjShapeType[si] == 0) {
					DeleteObj(si);
					if(D_0707 >= 0)
						bp_06 = D_0707;
				} else if(bp_08 == OBJ_14F && IsHatched(si)) {
					ClrHatched(si);
				}
			}
		}
	}
}

/*in case of death?*/
C_1184_1B3A(int objNum) {
	int ret;
	int bp_0a, bp_08, x, y, z;

	ret = -1;
	if(objNum < 0)
		return -1;
	if(ObjShapeType[objNum]) {
		x = GetX(objNum);
		y = GetY(objNum);
		z = GetZ(objNum);
		bp_08 = FindLoc(x, y, z);

		D_065A = 1;
		ret = AddMapObj(ObjShapeType[objNum], ObjStatus[objNum] | OWNED, QualQuan(objNum, 0), x, y, z);
		if(ret >= 0) {
			if(objNum < 0xe0)
				ClrLocal(ret);
			while((bp_0a = FindInv(objNum)) >= 0) {
				InsertObj(bp_0a, ret, CONTAINED);
				SetOwned(bp_0a);
			}
			if(GetType(bp_08) == OBJ_152)
				MoveObj(bp_08, x, y, z);
			MoveObj(objNum, x, y, 15);
		}
		D_065A = 0;
	}

	return ret;
}

/*in case of revive?*/
C_1184_1C52(int objNum) {
	int si, di;
	int bp_02;

	if(objNum < 0)
		return -1;

	D_065A = 1;
	bp_02 = GetQual(objNum);
	for(si = objNum; GetCoordUse(si) != LOCXYZ; si = GetAssoc(si));
	MoveObj(bp_02, GetX(si), GetY(si), GetZ(si));
	for(di = FindInv(objNum); di >= 0; di = FindInv(objNum)) {
		if(GetType(di) == OBJ_04C)
			InsertObj(di, bp_02, EQUIP);
		else
			InsertObj(di, bp_02, INVEN);
	}
	DeleteObj(objNum);
	D_065A = 0;

	return bp_02;
}

/*contains ...?*/
C_1184_1D2E(int objNum, int bp06) {
	int ret;

	ret = 0;
	while(GetCoordUse(objNum) != LOCXYZ) {
		objNum = GetAssoc(objNum);
		if(objNum == bp06) {
			ret = 1;
			break;
		}
	}
	return ret;
}

static C_1184_1D60(int objNum) {
	int i, ret;

	if(objNum < 0)
		return -1;
	for(i = ret = objNum; Link[i] >= 0 && GetCoordUse(Link[i]) != LOCXYZ; ) {
		i = Link[i];
		if(C_1184_1D2E(i, objNum))
			ret = i;
	}
	return ret;
}

/*C_1184_1DAF*/MoveObj(int objNum, int to_x, int to_y, int to_z) {
	int cur_x;
	int bp_0e, bp_0c, bp_0a, cur_y, bp_06, bp_04, bp_02;

	if(objNum < 0)
		return -1;
	if(StatusDisplay == CMD_92 && C_1184_1D2E(objNum, Party[D_04B3]))
		StatusDirty = 1;
	bp_04 = GetType(objNum);
	if(bp_04 == OBJ_04C)
		return -1;
	cur_x = GetX(objNum) - to_x;
	cur_y = GetY(objNum) - to_y;
	if(bp_04 == OBJ_19D && objNum < 0x100) {
		bp_06 = GetFrame(objNum);
		if(cur_x && cur_y)
			return -1;
		if(cur_x == 0 && cur_y == 0)
			return -1;
		if(!CLOSE_ENOUGH(1, cur_x, cur_y, 0, 0))
			return -1;
		if(DirIncrX[bp_06] == cur_x && DirIncrY[bp_06] == cur_y)
			return -1;
	}

	bp_02 = to_z?0xff:0x3ff;
	to_x &= bp_02;
	to_y &= bp_02;
	cur_x = GetX(objNum);
	cur_y = GetY(objNum);
	bp_0c = C_1184_1665(objNum);
	if(bp_0c < 0)
		return -1;
	if(
		((MapX - 5) <= cur_x && (MapX + 5) >= cur_x &&
		(MapY - 5) <= cur_y && (MapY + 5) >= cur_y) ||

		((MapX - 5) <= to_x && (MapX + 5) >= to_x &&
		(MapY - 5) <= to_y && (MapY + 5) >= to_y)
	) D_17B0 = 1;

	bp_0a = C_1184_1D60(objNum);
	Link[bp_0c] = Link[bp_0a];
	C_1184_16D5(GetX(objNum), GetY(objNum), GetZ(objNum), objNum, Link[bp_0c]);
	bp_0e = C_1184_02FA(to_x, to_y, 1);
	Link[bp_0a] = Link[bp_0e];
	Link[bp_0e] = objNum;
	SetCoordUse(objNum, LOCXYZ);
	SetXYZ(objNum, to_x, to_y, to_z);
	to_x = (to_x - AreaX) & 0x3ff;
	to_y = (to_y - AreaY) & 0x3ff;
	if(to_x < AREA_W && to_y < AREA_H)
		MapObjPtr[to_y][to_x] = objNum;
	if(bp_04 == OBJ_177)
		C_1184_0BC1();
	else if(bp_04 == OBJ_19D && objNum < 0x100)
		C_1184_0CBE(objNum, cur_x, cur_y, 0);
	else if(objNum < 0x100 && (
		bp_04 == OBJ_176 || bp_04 == OBJ_19B ||bp_04 == OBJ_19C || bp_04 >= OBJ_1AA
	)) C_1184_116D(objNum, cur_x, cur_y, 0);
	else if(bp_04 == OBJ_165)
		SetFrame(objNum, 0);

	if(objNum < 0x100 && D_06E6) {
		ClrSkipSomeTest(objNum);
		C_0A33_0FEB(objNum);
	}

	return objNum;
}

/*C_1184_20DD*/InsertObj(int objNum, int destNum, int objLoc) {
	int bp_06, bp_04, bp_02;

	if(objNum < 0 || destNum < 0)
		return -1;

	bp_02 = GetType(objNum);
	if(bp_02 == OBJ_04C && !D_065A)
		return -1;

	if(objNum == destNum)
		return -1;
	if(C_1184_1D2E(destNum, objNum))
		return -1;
	bp_06 = C_1184_1665(objNum);
	bp_04 = C_1184_1D60(objNum);
	Link[bp_06] = Link[bp_04];
	Link[bp_04] = Link[destNum];
	Link[destNum] = objNum;
	C_1184_16D5(GetX(objNum), GetY(objNum), GetZ(objNum), objNum, Link[bp_06]);
	SetCoordUse(objNum, objLoc);
	SetAssoc(objNum, destNum);
	return objNum;
}

int D_0765[] = {
	OBJ_05A,OBJ_03F,OBJ_04D,OBJ_037,OBJ_038,OBJ_041,OBJ_042,
	OBJ_043,OBJ_044,OBJ_045,OBJ_046,OBJ_047,OBJ_048,OBJ_151,
	OBJ_080,OBJ_081,OBJ_053,OBJ_14F,OBJ_059,OBJ_05B,OBJ_05C
};

/*C_1184_21C7*/QuanType(int objTyp) {
	int si, di;

	di = 0;
	if(objTyp == OBJ_058) {
		di = 4;
	} else {
		for(si = 0; si < 21; si++)
			if(D_0765[si] == objTyp) {
				di = 2;
				break;
			}
	}
	return di;
}

static C_1184_21FD(int objNum) {
	int si;

	si = QuanType(ObjShapeType[objNum]);
	if(si == 4)
		return Amount[objNum];
	if(si == 2)
		return GetQuan(objNum);
	return 1;
}

/*C_1184_2241*/GiveObj(int owner, int objType, int amount) {
	int si;
	int bp_0c, bp_0a, bp_08, bp_06, bp_04, bp_02;

	if(owner < 0)
		return -1;
	bp_0a = amount >> 8;
	bp_08 = (unsigned char)amount;
	bp_06 = QuanType(objType);
	if(owner < 0x100) {
		bp_04 = INVEN;
		if(objType == OBJ_04C) {
			bp_04 = EQUIP;
			for(si = FindInv(owner); si >= 0; si = NextInv()) {
				if(GetCoordUse(si) == EQUIP) {
					bp_02 = STAT_GetEquipSlot(si);
					if(bp_02 == SLOT_NECK) {
						SetCoordUse(si, INVEN);
						break;
					}
				}
			}
		}
	} else {
		bp_04 = CONTAINED;
	}
	if(bp_06 == 4) {
		si = FindInvType(owner, objType, -1);
		if(si < 0)
			si = AddInvObj(objType, bp_04 | 1, 0, owner);
		if(si >= 0)
			Amount[si] += amount;
	} else if(bp_06 == 2) {
		D_0656 = 1;
		si = FindInvType(owner, objType, bp_0a);
		D_0656 = 0;
		while(bp_08) {
			if(si < 0)
				si = AddInvObj(objType, bp_04 | 1, QualQuan(bp_0a, 0), owner);
			if(si < 0)
				break;
			if(GetQuan(si) + bp_08 > 0x90) {
				bp_08 -= 0x90 - GetQuan(si);
				SetQuan(si, 0x90);
				si = -1;
			} else {
				AddQuan(si, bp_08);
				bp_08 = 0;
			}
		}
	} else {
		for(bp_0c = 0; bp_0c < bp_08; bp_0c ++) {
			si = AddInvObj(objType, bp_04 | 1, QualQuan(bp_0a, 0), owner);
			if(si < 0)
				break;
		}
	}
	return si;
}

/*C_1184_23F8*/TakeObj(int owner, int objType, int amount) {
	int si, di;
	int bp_0a, bp_08, bp_06, bp_04, bp_02;

	bp_02 = 0;
	if(owner < 0)
		return -1;
	bp_06 = amount >> 8;
	bp_04 = (unsigned char)amount;
	di = QuanType(objType);
	if(di != 4)
		amount = bp_04;
	si = FindInv(owner);
	while(si >= 0 && GetCoordUse(si) != LOCXYZ && C_1184_1D2E(si, owner)) {
		if(GetType(si) == objType) {
			if(di == 4 || bp_06 == 0 || GetQual(si) == bp_06)  {
				bp_0a = C_1184_21FD(si);
				if(amount >= bp_0a) {
					amount -= bp_0a;
					bp_08 = Link[si];
					DeleteObj(si);
					si = bp_08;
					bp_02 = 1;
					continue;
				}
				if(di == 4)
					Amount[si] -= amount;
				else if(di == 2)
					SubQuan(si, amount);
				amount = 0;
				bp_02 = 1;
				if(amount <= 0)
					break;
			}
		}
		si = Link[si];
	}
	if(bp_02 && StatusDisplay == CMD_92 && Party[D_04B3] == owner)
		StatusDirty = 1;
	return si;
}

/*C_1184_252D*/TestObj(int objNum, int objType) {
	int si, di;

	di = 0;
	si = FindInv(objNum);
	while(si >= 0 && GetCoordUse(si) != LOCXYZ && C_1184_1D2E(si, objNum)) {
		if(GetType(si) == objType)
			di += C_1184_21FD(si);
		si = Link[si];
	}
	return di;
}

/*uncalled?*/
/*C_1184_257D*/TransferObj(int oldOwner, int newOwner, int objType, int amount) {
	if(TakeObj(oldOwner, objType, amount) >= 0)
		GiveObj(newOwner, objType, amount);
}

/*C_1184_25A2*/static __ObjSerialize(int objNum, int index) {
	struct coord bp_06;
	int bp_02;

	if(GetCoordUse(objNum) != LOCXYZ) {
		if((int)GetAssoc(objNum) < 0x100) {
			if(GetCoordUse(objNum) == CONTAINED)
				SetCoordUse(objNum, INVEN);
		} else {
			SetCoordUse(objNum, CONTAINED);
		}
	}
	ScratchBuf->inner[index]._00 = ObjStatus[objNum];
	ScratchBuf->inner[index]._01 = ObjPos[objNum];
	ScratchBuf->inner[index]._04 = ObjShapeType[objNum];
	ScratchBuf->inner[index]._06 = Amount[objNum];
	if(GetCoordUse(objNum) == CONTAINED) {
		for(
			bp_02 = index;
			ScratchBuf->_6000[bp_02] != GetAssoc(objNum);
			bp_02 --
		);
		SetCoordAssoc(bp_06, bp_02);
		ScratchBuf->inner[index]._01 = bp_06;
	}
	ObjShapeType[objNum] = 0;
	if(index + 1 >= 0xc00)
		CON_printf(/*078F*/"Error-too many objects in region!\n");
}

/*DeleteTempFiles*/
C_1184_26E2() {
	int si;

	DISK_confirm(DISK_8);
	for(si = 0; si < (8*8 + 5); si ++) {
		if(D_065C[si]) {
			__Tmp_file[15] = (si & 7) + 'A';
			__Tmp_file[16] = (si >> 3) + 'A';
			OSI_delete(__Tmp_file);
		}
	}
}

/*write a objblk*/
static C_1184_2722(int num_obj, int area_id) {
	int si, di;
	char bp_04[2],bp_02[2];

	D_065C[area_id] = 1;
	DISK_confirm(DISK_8);
	di = OSI_create(__Tmp_file);
	OSI_write(di, 0, 2, &num_obj);
	if(D_2E9E != 'S' && D_2E9E != 'R')
		OSI_write(di, -1, (long)num_obj << 3, ScratchBuf);
	if(D_2E9E == 'S') {
		bp_04[0] = __Tmp_file[15];
		bp_04[1] = __Tmp_file[16];
		bp_02[0] = __Sav_file[15];
		bp_02[1] = __Sav_file[16];
		OSI_close(di);
		for(si = 0; si < (8*8 + 5); si ++) {
			if(D_065C[si]) {
				__Sav_file[15] = __Tmp_file[15] = (si & 7) + 'A';
				__Sav_file[16] = __Tmp_file[16] = (si >> 3) + 'A';
				OSI_delete(__Sav_file);
				if(si != area_id)
					OSI_rename(__Tmp_file, __Sav_file);
				D_065C[si] = 0;
			}
		}
		__Tmp_file[15] = __Sav_file[15] = bp_04[0];
		__Tmp_file[16] = __Sav_file[16] = bp_04[1];
		di = OSI_create(__Sav_file);
		OSI_write(di, 0, 2, &num_obj);
		OSI_write(di, -1, (long)num_obj << 3, ScratchBuf);
		__Sav_file[15] = bp_02[0];
		__Sav_file[16] = bp_02[1];
		D_2E9E = 'G';
		D_0340 = 1;
	}
	OSI_close(di);
}

/*bozo i -- leave an outside area*/
static C_1184_2888(int area_id) {
	int si, num_obj;
	int bp_08, bp_06, x, y;

	if(area_id < 0 || D_2E9E == 'R')
		return;
	num_obj = 0;
	x = (area_id & 7) << 7;
	y = (area_id & 0x38) << 4;
	bp_08 = 0x100;
	si = Link[0x100];
	while(num_obj < 0xc00 && si >= 0) {
		if(GetCoordUse(si) == LOCXYZ && GetZ(si) == 0 &&
			(GetX(si) & 0x380) == x && (GetY(si) & 0x380) == y
		) {
			if(si < 0x100) {
				bp_08 = si;
			} else {
				ScratchBuf->_6000[num_obj] = si;
				__ObjSerialize(si, num_obj++);
				D_E6E0 ++;
			}
			bp_06 = si;
			si = Link[si];
			while(si >= 0 && GetCoordUse(si) != LOCXYZ) {
				if(num_obj < 0xc00) {
					ScratchBuf->_6000[num_obj] = si;
					__ObjSerialize(si, num_obj++);
				}
				D_E6E0 ++;
				bp_06 = si;
				si = Link[si];
			}
			Link[bp_06] = D_D5DA;
			D_D5DA = Link[bp_08];
			Link[bp_08] = si;
		} else {
			bp_08 = si;
			si = Link[si];
		}
	}
	__Tmp_file[15] = (area_id & 7) + 'A';
	__Tmp_file[16] = (area_id >> 3) + 'A';
	C_1184_2722(num_obj, area_id);
}

static C_1184_29C4(int obj_0, int obj_1) {
	int assoc_0, assoc_1;
	int dist;

	if(obj_0 == -1)
		return 1;
	if(obj_1 == -1)
		return -1;
	assoc_0 = obj_0; assoc_1 = obj_1;
	while(GetCoordUse(assoc_0) == CONTAINED)
		assoc_0 = GetAssoc(assoc_0);
	while(GetCoordUse(assoc_1) == CONTAINED)
		assoc_1 = GetAssoc(assoc_1);
	if(GetCoordUse(assoc_0) != LOCXYZ && GetCoordUse(obj_1) == LOCXYZ)
		return -1;
	if(GetCoordUse(assoc_1) != LOCXYZ && GetCoordUse(obj_0) == LOCXYZ)
		return 1;
	if(Is_0010(assoc_0))
		assoc_0 = GetAssoc(assoc_0);
	if(Is_0010(assoc_1))
		assoc_1 = GetAssoc(assoc_1);
	dist = GetY(assoc_0) - GetY(assoc_1);
	if(dist == 0)
		dist = GetX(assoc_0) - GetX(assoc_1);
	if(dist == 0)
		dist = GetZ(assoc_1) - GetZ(assoc_0);
	if(assoc_0 == assoc_1) {
		if(GetCoordUse(assoc_0) == LOCXYZ)
			dist = -1;
		else if(GetCoordUse(assoc_1) == LOCXYZ)
			dist = 1;
	}
	return dist;
}

static C_1184_2B27() {
	int si, di;
	int bp_06, bp_04, bp_02;

	bp_06 = 0x100;
	while(Link[bp_06] >= 0) {
		si = Link[bp_06];
		if(Is_0010(si)) {
			bp_02 = GetAssoc(si);
			for(
				di = Link[bp_02];
				di >= 0 && GetCoordUse(di) != LOCXYZ && di != si;
				di = Link[di]
			);
			if(di != si) {
				bp_04 = C_1184_1D60(si);
				bp_02 = C_1184_1D60(bp_02);
				Link[bp_06] = Link[bp_04];
				Link[bp_04] = Link[bp_02];
				Link[bp_02] = si;
				continue;
			}
		}
		bp_06 = si;
	}
}

/*C_1184_2BD8*/static __ObjectsDeserialize(int num_obj) {
	int si, di;
	int bp_06, bp_04, bp_02;

	for(bp_06 = 0; bp_06 < num_obj; bp_06 ++) {
		if(D_E6E0 == 0) {
			ScratchBuf->_6000[bp_06] = -1;
			if(bp_06)
				Link[ScratchBuf->_6000[bp_06 - 1]] = -1;
			break;
		}
		si = D_D5DA;
		ScratchBuf->_6000[bp_06] = si;
		D_D5DA = Link[si];
		D_E6E0 --;
		ObjStatus[si] = ScratchBuf->inner[bp_06]._00;
		ObjPos[si] =  ScratchBuf->inner[bp_06]._01;
		ObjShapeType[si] = ScratchBuf->inner[bp_06]._04;
		Amount[si] = ScratchBuf->inner[bp_06]._06;
		if(GetCoordUse(si) == CONTAINED)
			GetAssoc(si) = ScratchBuf->_6000[GetAssoc(si)];
	}
	if(bp_06 == num_obj)
		Link[ScratchBuf->_6000[bp_06 - 1]] = -1;
	si = 0x100;
	di = ScratchBuf->_6000[0];
	bp_02 = 0;
	while(si >= 0 && di >= 0) {
		if(C_1184_29C4(Link[si], di) < 0) {
			si = Link[si];
		} else {
			bp_04 = Link[si];
			Link[si] = di;
			si = bp_04;
			while(Link[di] >= 0 && C_1184_29C4(si, Link[di]) > 0) {
				if(C_1184_29C4(di, Link[di]) <= 0) {
					di = Link[di];
				} else {
					bp_02 = 1;
					break;
				}
			}
			bp_04 = Link[di];
			Link[di] = si;
			di = bp_04;
			if(bp_02) {
				si = 0x100;
				bp_02 = 0;
				continue;
			}
		}
		if(Link[si] < 0) {
			Link[si] = di;
			si = -1;
		}
	}
}

/*read an objblk file*/
static C_1184_2DEF(int area_id) {
	int di;
	int bp_02;

	if(area_id < 0 || D_2E9E == 'R')
		return;
	area_id &= 0x3f;
	DISK_confirm(DISK_8);
	if(!D_065C[area_id]) {
		__Sav_file[15] = 'A' + (area_id & 7);
		__Sav_file[16] = 'A' + (area_id >> 3);
		di = OSI_open(__Sav_file);
	} else {
		__Tmp_file[15] = 'A' + (area_id & 7);
		__Tmp_file[16] = 'A' + (area_id >> 3);
		di = OSI_open(__Tmp_file);
	}
	OSI_read(di, 0, 2, (void far *)&bp_02);
	if(bp_02 > 0xc00)
		bp_02 = 0xc00;
	if(bp_02)
		OSI_read(di, -1, (long)bp_02 << 3, ScratchBuf);
	OSI_close(di);
	if(bp_02)
		__ObjectsDeserialize(bp_02);
}

C_1184_2ECC() {
	int si;
	int diff_x, diff_y;
	int __unused__;

	for(diff_y = 0; diff_y < AREA_H; diff_y ++) {
		for(diff_x = 0; diff_x < AREA_W; diff_x ++)
			MapObjPtr[diff_y][diff_x] = -1;
	}
	for(si = Link[0x100]; si >= 0; si = Link[si]) {
		if(GetCoordUse(si) == LOCXYZ) {
			__unused__ = GetZ(si)?0xff:0x3ff;
			diff_x = (GetX(si) - AreaX) & 0x3ff;/*__unused__ => bug?*/
			diff_y = (GetY(si) - AreaY) & 0x3ff;/*__unused__ => bug?*/
			if(diff_x < AREA_W && diff_y < AREA_H && MapObjPtr[diff_y][diff_x] == -1)
				MapObjPtr[diff_y][diff_x] = si;
		}
	}
}

/*bozo ii - leave a dungeon level?*/
static C_1184_2FB3(int z) {
	int si, num_obj;
	int bp_04, bp_02;

	if(z < 0 || z > 5 || D_2E9E == 'R')
		return;
	num_obj = 0;
	bp_04 = 0x100;


	si = Link[0x100];
	while(num_obj < 0xc00 && si >= 0) {
		if(GetCoordUse(si) == LOCXYZ && GetZ(si) == z) {


			if(si < 0x100) {
				bp_04 = si;
			} else {
				ScratchBuf->_6000[num_obj] = si;
				__ObjSerialize(si, num_obj++);
				D_E6E0 ++;
			}
			bp_02 = si;
			si = Link[si];
			while(si >= 0 && GetCoordUse(si) != LOCXYZ) {
				if(num_obj < 0xc00) {
					ScratchBuf->_6000[num_obj] = si;
					__ObjSerialize(si, num_obj++);
				}
				D_E6E0 ++;
				bp_02 = si;
				si = Link[si];
			}
			Link[bp_02] = D_D5DA;
			D_D5DA = Link[bp_04];
			Link[bp_04] = si;
		} else {
			bp_04 = si;
			si = Link[si];
		}
	}
	__Tmp_file[15] = z - 1 + 'A';
	__Tmp_file[16] = 'I';
	C_1184_2722(num_obj, z + 0x3f);
}

/*read a dungeon objblk file*/
static C_1184_30A6(int z) {
	int si;
	int bp_02;

	if(z < 0 || z > 5 || D_2E9E == 'R')
		return;
	DISK_confirm(DISK_8);
	if(!D_065C[8*8 + z - 1]) {
		__Sav_file[15] = 'A' + z - 1;
		__Sav_file[16] = 'I';
		si = OSI_open(__Sav_file);
	} else {
		__Tmp_file[15] = 'A' + z - 1;
		__Tmp_file[16] = 'I';
		si = OSI_open(__Tmp_file);
	}
	OSI_read(si, 0, 2, (void far *)&bp_02);
	if(bp_02 > 0xc00)
		bp_02 = 0xc00;
	if(bp_02)
		OSI_read(si, -1, (long)bp_02 << 3, ScratchBuf);
	OSI_close(si);
	if(bp_02)
		__ObjectsDeserialize(bp_02);
}

/*C_1184_3176*/LoadNewRegions(int x, int y, int z) {
	int si, di;
	int bp_14, bp_12, bp_10, bp_0e, bp_0c, bp_0a;
	int mapIds[4];

	bp_0c =
	bp_0a = 0;
	if(D_06F2 > 0 && D_06F2 != z) {
		C_1184_19AA(x, y, z);
		C_1184_2FB3(D_06F2);
		bp_0c ++;
	}
	if(z == 0) {
		if(D_06F0 == -1) {
			mapIds[0] = (((x - 0x40) >> 7) & 7) + (((y - 0x40) >> 4) & 0x38);
		} else {
			bp_14 = 0;
			bp_10 = x - ((D_06F0 & 7) << 7);
			for(; bp_10 < 5; bp_10 += 0x80, bp_14 --);
			for(; bp_10 >= 0xfb; bp_10 -= 0x80, bp_14 ++);
			bp_12 = 0;
			bp_0e = y - ((D_06F0 & 0x38) << 4);
			for(; bp_0e < 5; bp_0e += 0x80, bp_12 -= 8);
			for(; bp_0e >= 0xfb; bp_0e -= 0x80, bp_12 += 8);
			mapIds[0] = (((D_06F0 & 7) + bp_14) & 7) + (((D_06F0 & 0x38) + bp_12) & 0x38);
		}
		mapIds[1] = ((mapIds[0] + 1) & 7) + (mapIds[0] & 0x38);
		mapIds[2] = (mapIds[0] + 8) & 0x3f;
		mapIds[3] = (mapIds[1] + 8) & 0x3f;
		D_06F0 = mapIds[0];
	} else {
		for(si = 0; si < 4; si ++)
			mapIds[si] = -1;
		D_06F0 = -1;
	}
	if(D_06F2 == 0) {
		for(si = 0; si < 4; si ++) {
			for(di = 0; di < 4; di ++) {
				if(mapIds[di] == D_06E8[si])
					break;
			}
			if(di != 4 || D_06E8[si] == -1)
				continue;
			if(bp_0c == 0)
				C_1184_19AA(x, y, z);
			C_1184_2888(D_06E8[si]);
			bp_0c ++;
			D_06E8[si] = -1;
		}
	}
	if(z == 0) {
		for(di = 0; di < 4; di ++) {
			for(si = 0; si < 4; si++) {
				if(mapIds[di] == D_06E8[si])
					break;
			}
			if(si != 4)
				continue;
			for(si = 0; si < 4; si ++) {
				if(D_06E8[si] == -1)
					break;
			}
			C_1184_2DEF(mapIds[di]);
			bp_0a ++;
			D_06E8[si] = mapIds[di];
		}
	} else if(D_06F2 != z) {
		C_1184_30A6(z);
		bp_0a ++;
	}
	D_06F2 = z;
	if(bp_0a)
		C_1184_2B27();
}

/*"commit" obj files*/
C_1184_33CA() {
	int si;

	if(MapZ) {
		C_1184_2FB3(MapZ);
	} else {
		for(si = 0; si < 4; si ++)
			C_1184_2888(D_06E8[si]);
	}
	for(si = 0; si < (8*8 + 5); si ++) {
		if(D_065C[si]) {
			__Tmp_file[15] = __Sav_file[15] = __Bak_file[15] = (si & 7) + 'A';
			__Tmp_file[16] = __Sav_file[16] = __Bak_file[16] = (si >> 3) + 'A';
			OSI_delete(__Sav_file);
			OSI_rename(__Tmp_file, __Sav_file);
			D_065C[si] = 0;
		}
	}
	for(si = 0; si < 4; si ++)
		D_06E8[si] = -1;
	D_06F0 = D_06F2 = -1;
	LoadNewRegions(MapX, MapY, MapZ);
	C_1184_2ECC();
}

/*C_1184_347C*/ShowObject(int tile_frame, int x, int y, int bp06) {
	unsigned char *si;
	int tile;
	int bp_08, bp_06, bp_04;
	unsigned char *bp_02;

	tile = tile_frame & 0x7ff;
	if(BaseTile[OBJ_14F] == tile)
		return;
	bp_08 = Tile_11x11[y][x];
	if(bp_08 == TIL_0FF)
		return;
	if(bp_08 == TIL_1BC) {
		if(SpellFx[1] == 0 || !IsTileWa(tile))
			return;
	}
	if(IsTileBa(tile)) {
		Tile_11x11[y][x] = tile_frame;
		return;
	}
	bp_04 = D_E5E0[0];
	if(bp_04 == 0)
		return;
	D_E5E0[0] = D_E5E0[bp_04];
	D_D5DC[bp_04] = tile_frame;
	si = &(Obj_11x11[y][x]);
	if(IsTileFor(tile) || bp06 == 2) {
		bp_06 = 1;
		if(bp06 & 1) {
			if(SpellFx[1] && bp06 == 3) {
				bp_02 = si;
				for(; *si; si = D_E5E0 + *si) {
					if(!IsTileWa(D_D5DC[*si] & 0x7ff)) {
						bp_02 = D_E5E0 + *si;
						break;
					}
				}
				si = bp_02;
			} else {
				while(*si)
					si = D_E5E0 + *si;
			}
		} else {
			while(*si && !IsTileFor(D_D5DC[*si] & 0x7ff))
				si = D_E5E0 + *si;
		}
	}
	D_E5E0[bp_04] = *si;
	*si = bp_04;
}

static C_1184_35EA(int tile, int frame, int x, int y) {
	int bp_02;

	if(x < 11 && y < 11)
		ShowObject(tile + frame, x, y, 0);

	bp_02 = TileFlag[tile];
	if(bp_02 & TILE_FLAG1_80) {
		if(x != 0 && y < 11)
			ShowObject(tile + frame - 1, x - 1, y, 1);
		if(bp_02 & TILE_FLAG1_40) {
			if(x < 11 && y != 0)
				ShowObject(tile + frame - 2, x, y - 1, 1);
			if(x != 0 && y != 0)
				ShowObject(tile + frame - 3, x - 1, y - 1, 1);
		}
	} else if(x < 11 && y != 0 && (bp_02 & TILE_FLAG1_40)) {
		ShowObject(tile + frame - 1, x, y - 1, 1);
	}
}

/*C_1184_36C2*/ShowObjects() {
	int objNum;
	int i, j;
	int tile, obj_x, obj_y, light;
	int newFrame, objTyp, bp_06, bp_04, hasMusic;

	MUS_051E();
	hasMusic = 0;
	for(i = 0; i < 0x100; i ++)
		D_E5E0[i] = i + 1;

	for(i = 0; i < 11; i ++)
		for(j = 0; j < 11; j ++)
			Obj_11x11[i][j] = 0;

	for(
		objNum = SearchArea(MapX - 5, MapY - 5, MapX + 6, MapY + 6);
		objNum >= 0;
		objNum = NextArea()
	) {
		newFrame = 0;
		objTyp = GetType(objNum);
		/*possible eruption*/
		if(Erupting == 1)
			if(objTyp == OBJ_133 || objTyp == OBJ_0AB)
				Erupting = objNum;
		/*bard*/
		if(objTyp == OBJ_188)
			hasMusic = 1;
		/*ambiant sound*/
		MUS_043B(objTyp, COMBAT_getCathesus(objNum, MapX, MapY));
		/* */
		if(IsInvisible(objNum) && (objNum >= 0x100 || !IsPlrControl(objNum)))
			continue;
		if(objNum < 0x100 && IsDraggedUnder(objNum))
			continue;
		/*mirror reflection*/
		if(objTyp == OBJ_07B && GetFrame(objNum) < 2) {
			if(C_1184_0B3D(0, GetX(objNum), GetY(objNum) + 1) >= 0)
				SetFrame(objNum, 1);
			else
				SetFrame(objNum, 0);
		}
		/* */
		tile = TILE_FRAME(objNum);
		if(IsInvisible(objNum) && objTyp < OBJ_1AA)
			newFrame = (2 << 10);
		if(objNum < 0x100) {
			if(IsProtected(objNum))
				newFrame = (4 << 10);
			if(IsCursed(objNum))
				newFrame = (6 << 10);
		}
		obj_x = GetX(objNum) - MapX + 5;
		obj_y = GetY(objNum) - MapY + 5;
		if(newFrame)
			C_1184_35EA(tile, newFrame, obj_x, obj_y);
		if(!IsInvisible(objNum))
			C_1184_35EA(tile, 0, obj_x, obj_y);
		/*britannia lens in temple of singularity*/
		if(
			objTyp == OBJ_18A &&
			obj_x + MapX == 0x39e &&
			obj_y + MapY == 0x358 &&
			GetZ(objNum) == 0 &&
			obj_y < 11
		) {
			if(obj_x)
				ShowObject(TIL_1BA, obj_x + 1, obj_y, 1);
			if(obj_x < 10)
				ShowObject(TIL_1BB, obj_x - 1, obj_y, 1);
			continue;
		}
		/*gargoyle lens in temple of singularity*/
		if(
			objTyp == OBJ_18C &&
			obj_x + MapX == 0x3a2 &&
			obj_y + MapY == 0x358 &&
			GetZ(objNum) == 0 &&
			obj_y < 11
		) {
			if(obj_x)
				ShowObject(TIL_1B8, obj_x + 1, obj_y, 1);
			if(obj_x < 10)
				ShowObject(TIL_1B9, obj_x - 1, obj_y, 1);
		}
	}
	/* */
	if(!D_05E8) {
		for(i = 0; i < 11; i ++) {
			for(j = 0; j < 11; j ++) {
				if(IsTerrainWall(Tile_11x11[j][i])) {
					if(
						!(AreaFlags[j     + ViewY][i + 1 + ViewX] & 0x80) ||
						!(AreaFlags[j + 1 + ViewY][i     + ViewX] & 0x80)
					) {
						bp_06 = Obj_11x11[j][i];
						if(bp_06) {
							Obj_11x11[j][i] = 0;
							for(bp_04 = bp_06; D_E5E0[bp_04]; bp_04 = D_E5E0[bp_04]);
							D_E5E0[bp_04] = D_E5E0[0];
							D_E5E0[0] = bp_06;
						}
					}
				}
				/*draw obscurity*/
				light = AreaLight[j + ViewY][i + ViewX];
				if(light < 4 && light > 0) {
					tile = TIL_1BC + light;
					ShowObject(tile, i, j, 3);
				}
			}
		}
	}
	/*no eruption this time*/
	if(Erupting == 1)
		Erupting = 0;
	/* */
	if(hasMusic && !D_0654) {
		MUS_SongIndex = 0;
		MUS_Song = OSI_rand(0, 1);
	}
	D_0654 = hasMusic;
}

/*Initialization?*/
C_1184_3B1D() {
	int si;

	for(si = 0; si < 0xbff; si ++)
		Link[si] = si + 1;
	Link[0xbff] = -1;
	Link[0xff] = -1;
	D_BDD6 = 0xe0;
	D_E6E2 = 0x20;
	D_D5DA = 0x101;
	D_E6E0 = 0xaff;
	Link[0x100] = -1;
	for(si = 0; si < 4; si ++)
		D_06E8[si] = -1;
	D_06F0 = D_06F2 = -1;
}

/*called after a restore*/
C_1184_3B7D() {
	int si, di;
	int bp_02;

	bp_02 = 0x100;
	for(si = 0; si < 0x100; si ++) {
		if(ObjShapeType[si]) {
			if(si >= 0xe0) {
				if(bp_02 == 0x100)
					D_BDD6 = si + 1;
				else
					Link[bp_02] = si + 1;
				D_E6E2 --;
			}
			for(di = 0x100; Link[di] >= 0 && C_1184_29C4(Link[di], si) < 0; di = Link[di]);
			Link[si] = Link[di];
			Link[di] = si;
		} else {
			if(si >= 0xe0)
				bp_02 = si;
		}
	}
}

/*load tile description from "look.lzd"*/
/*C_1184_3C1B*/GetTileString(int numTile) {
	int si;
	char *di;
	int far *bp_04;

	DISK_confirm(DISK_5);
	IsZFile = 1;
	LoadFile(/*07B2*/"look.lzd", ScratchBuf);
	IsZFile = 0;
	bp_04 = ScratchBuf;
	si = *(bp_04++);
	while(si < numTile) {
		while(*(((char far *)bp_04)++));
		si = *(bp_04++);
	}
	di = D_D7DC;
	while(*di++ = *(((char far *)bp_04)++));
	return D_D7DC[0]?D_D7DC:/*07BB*/"nothing";
}

/*C_1184_3C9C*/GetObjectString(int objNum) {
	char *di;
	int bp_02;

	if(objNum < 0) {
		di = /*07C3*/"Nothing";
	} else if(IsPlrControl(objNum)) {
		di = GetTileString(TILE_FRAME(objNum));
		for(bp_02 = 0; bp_02 < PartySize; bp_02 ++) {
			if(Party[bp_02] == objNum) {
				strcpy(D_D7DC, Names[bp_02]);
				di = D_D7DC;
				break;
			}
		}
	} else if(objNum < 0x100 && Isbis_0014(objNum)) {
		di = GetTileString(BaseTile[OrigShapeType[objNum] & 0x3ff] + (OrigShapeType[objNum] >> 10));
	} else {
		di = GetTileString(TILE_FRAME(objNum));
	}
	return di;
}
