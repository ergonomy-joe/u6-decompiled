/*
 * Ultima VI - The False Prophet
 * Copyright (c) 1990 Origin Systems, Inc. and Richard Gariott
 * reverse-coded by Ergonomy Joe in 2013
 */
/*
	"look" "drop" "get" "move" "use" module
*/
#include "u6.h"

#undef abs
#include <math.h>

static C_27A1_0008(int bp06) {
	if(IN_VEHICLE)
		return Party[0];
	else
		return Party[bp06];
}

static C_27A1_0028(int bp06) {
	if(IN_VEHICLE)
		return GetX(0);
	else
		return GetX(bp06);
}

static C_27A1_0051(int bp06) {
	if(IN_VEHICLE)
		return GetY(0);
	else
		return GetY(bp06);
}

int D_1C00[] = {
	OBJ_062,OBJ_063,OBJ_039,
	OBJ_0BA,OBJ_0BC,OBJ_0BF,
	OBJ_0C0,OBJ_153,OBJ_03E
};

/*IsReadyable?*/
C_27A1_0082(int typeObject) {
	int si;

	for(si = 0; si < 9; si ++)
		if(D_1C00[si] == typeObject)
			return 1;
	return 0;
}

C_27A1_00A9(int objNum, int bp06) {
	int si;

	if(objNum != -1) {
		si = GetType(objNum);
		if(
			C_27A1_0082(si) &&
			bp06 >= 0x100 &&
			si != OBJ_039 && si != OBJ_153
		) {
			if((si != OBJ_062 && si != OBJ_0BA && si != OBJ_0C0) || GetFrame(objNum) == 0) {
				si = GetType(bp06);
				if(si != OBJ_062 && si != OBJ_0BA && si != OBJ_0C0)
					return 1;
			}
		}
	}
	return 0;
}

int D_1C12[] = {
	OBJ_053,OBJ_05C,OBJ_09B,OBJ_07B,OBJ_073,
	OBJ_074,OBJ_075,OBJ_076,OBJ_077,OBJ_078,
	OBJ_0B7,OBJ_0B8,OBJ_113,OBJ_0A1,OBJ_1A1
};

/*IsBreakable?*/
static C_27A1_012C(int typeObject) {
	int si;

	for(si = 0; si < 15; si ++) {
		if(D_1C12[si] == typeObject)
			return 1;
	}
	return 0;
}

static /*C_27A1_0153*/IsOkToGet(int objNum) {
	int si;

	if(!IsOwned(objNum))
		return 0;
	for(si = Link[objNum]; si >= 0 && GetCoordUse(si) != LOCXYZ && C_1184_1D2E(si, objNum); si = Link[si]) {
		if(!IsOwned(si))
			return 0;
	}
	return 1;
}

/*C_27A1_019E*/SetOkToGet(int objNum) {
	int si;

	SetOwned(objNum);
	for(si = Link[objNum]; si >= 0 && GetCoordUse(si) != LOCXYZ && C_1184_1D2E(si, objNum); si = Link[si])
		SetOwned(si);
}

int D_1C30[] = {
	OBJ_075,OBJ_1A7,OBJ_10E,OBJ_0BA,OBJ_0EC,OBJ_0B6,OBJ_0CE,OBJ_080,OBJ_0B4,OBJ_060,
	OBJ_083,OBJ_0FD,OBJ_07A,OBJ_091,OBJ_0DD,OBJ_12B,OBJ_084,OBJ_062,OBJ_087,OBJ_0B5,
	OBJ_1AC,OBJ_120,OBJ_0C0,OBJ_09B,OBJ_1A4,OBJ_0D4,OBJ_0A4,OBJ_109,OBJ_108,OBJ_0EA,
	OBJ_061,OBJ_04D,OBJ_05F,OBJ_085,OBJ_09D,OBJ_09C,OBJ_1AE,OBJ_1AF,OBJ_0B8,OBJ_03F,
	OBJ_131,OBJ_10C,OBJ_09E,OBJ_074,OBJ_0D1,OBJ_049,OBJ_081,OBJ_129,OBJ_057,OBJ_099,
	OBJ_116,OBJ_118,OBJ_067,OBJ_113,OBJ_0DF,OBJ_1A3,OBJ_19F,OBJ_0D2,OBJ_082,OBJ_0A9,
	OBJ_0F2,OBJ_0F3,OBJ_0F4,OBJ_0F5,OBJ_0F6,OBJ_0F7,OBJ_0F8,OBJ_0F9,OBJ_14E,OBJ_05D,
	OBJ_19C,OBJ_068,OBJ_139,OBJ_19E,OBJ_04E,OBJ_12C,OBJ_0AE,OBJ_09A,OBJ_05A,OBJ_040,
	OBJ_03E,OBJ_0E9,OBJ_12A,OBJ_073,OBJ_128
};

/*usable objects?*/
C_27A1_01DE(int typeObject) {
	int si;

	for(si = 0; si < 85; si ++)
		if(D_1C30[si] == typeObject)
			return 1;
	return 0;
}

/*string to int*/
C_27A1_0205(unsigned char *bp08, int bp06) {
	int si, di;

	di = 0;
	for(si = 0; si < strlen(bp08); si ++) {
		if(bp08[si] >= '0' && bp08[si] <= '9') {
			di = di * 10 + (bp08[si] - '0');
			continue;
		}
		if(bp06 == 1) {/*latitude*/
			if(toupper(bp08[si]) == 'S')
				return di;
			else if(toupper(bp08[si]) == 'N')
				return -di;
			else
				return 100;
		}
		if(bp06 == 2) {/*longitude*/
			if(toupper(bp08[si]) == 'E')
				return di;
			else if(toupper(bp08[si]) == 'W')
				return -di;
			else
				return 100;
		}
		return -2;
	}
	if(bp06)
		return 100;
	if(si == 0 && D_04AD == 0)
		return -1;
	if(si == 0 && D_04AD)
		return -2;
	return di;
}

/*display character's portrait/inventory*/
C_27A1_02D9(int objNum) {
	int si;
	int bp_12, bp_10, bp_0e, shrineTyp, isGeneric, bp_08, showInven, bp_04;
	char *bp_02;

	shrineTyp = 0;
	isGeneric = 0;
	showInven = 0;
	HideMouse
	bp_04 = StatusDisplay;
	NPC_1Hand = 0;
	if(IsPlrControl(objNum)) {
		ShowNPCInven = 0;
		for(bp_10 = 0; bp_10 < 0x10; bp_10 ++) {
			if(Party[bp_10] == objNum) {
				D_04B3 = bp_0e = bp_10;
				break;
			}
		}
	} else {
		ShowNPCInven = objNum;
	}

	for(bp_10 = 0; bp_10 < 8; bp_10 ++)
		Equipment[bp_10] = 0;
	for(bp_12 = FindInv(objNum); bp_12 >= 0; bp_12 = NextInv()) {
		if(GetCoordUse(bp_12) == EQUIP && GetType(bp_12) != OBJ_151 && GetType(bp_12) != OBJ_150) {
			showInven = 1;
			si = STAT_GetEquipSlot(bp_12);
			if(si == SLOT_2HND) {
				si = SLOT_RHND;
				Equipment[SLOT_LHND] = 1;
				if(IsPlrControl(objNum))
					Party_1Hand[bp_0e] = 1;
				else
					NPC_1Hand = 1;
			} else if(si == SLOT_RHND && Equipment[SLOT_RHND]) {
				si = SLOT_LHND;
			} else if(si == SLOT_LHND && Equipment[SLOT_LHND]) {
				si = SLOT_RHND;
			} else if(si == SLOT_RING) {
				if(Equipment[SLOT_RFNG])
					si = SLOT_LFNG;
				else
					si = SLOT_RFNG;
			}
			Equipment[si] = bp_12;
		}
	}

	bp_08 = D_04B3;
	si = GetType(objNum);
	if(si == OBJ_189 || si == OBJ_18D || si == OBJ_18E || si == OBJ_18F)
		shrineTyp = GetQual(objNum);
	else if(si == OBJ_175 || si == OBJ_17E || si == OBJ_16B)
		isGeneric = 1;
	if(showInven || objNum < 0xe0 || shrineTyp || isGeneric) {
		GR_06(D_2A54);
		GR_12(168, 6, 311, 103);
		CON_setClip(&D_B6B5[1]);
		C_2FC1_19C5();
	}
	if(showInven && (objNum < 0xe0 || isGeneric)) {
		C_2FC1_1C19(objNum, 248, 24);/*draw portrait*/
		C_2FC1_1EAF(192, 40);/*draw equipment*/
		C_155D_08F4(0, 8);/*draw inventory*/
		StatusDirty ++;
	} else if(showInven) {
		C_2FC1_1EAF(224, 40);/*draw equipment*/
		C_155D_08F4(32, 8);/*draw inventory*/
		StatusDirty ++;
	} else if(objNum < 0xe0 || isGeneric) {
		C_2FC1_1C19(objNum, 216, 24);/*draw portrait*/
		StatusDirty ++;
	} else if(shrineTyp) {
		C_2FC1_1C19(shrineTyp, 216, 24);/*draw portrait*/
		StatusDirty ++;
	}
	D_04B3 = bp_08;
	if(showInven || objNum < 0xe0 || shrineTyp || isGeneric) {
		if(objNum == 0)
			objNum = 0x82;/*pushme/pullyu?*/
		if(shrineTyp || (!TalkFlags[objNum] & 1)/*bug?*/)
			bp_02 = GetObjectString(objNum);
		else
			bp_02 = C_1703_0116(objNum);
		CON_gotoxy((17 - strlen(bp_02)) / 2, 10);
		CON_printf(/*1D6D*/"%s\n", bp_02);
		StatusDisplay = CMD_9E;
		if(!IsInConversation)
			CON_getch();
	}
	CON_setClip(&D_B6B5[3]);
	ShowMouse
	ShowNPCInven = 0;
	NPC_1Hand = 0;
	StatusDisplay = bp_04;
}

C_27A1_061E(int numTile) {
	switch(D_B3EF[numTile] & TILE_FLAG2_C0) {
		case 0x40: CON_printf(/*1D71*/"a "); break;
		case 0x80: CON_printf(/*1D74*/"an "); break;
		case 0xc0: CON_printf(/*1D78*/"the "); break;
	}
}

int D_1CDA[] = {
	OBJ_097,
	OBJ_03D,
	OBJ_098,
	OBJ_10E,
	OBJ_03B
};

/*IsReadableBook?*/
C_27A1_066D(int objNum) {
	int si, di;

	di = GetType(objNum);
	for(si = 0; si < 5; si ++) {
		if(D_1CDA[si] == di)
			return 1;
	}
	return 0;
}

int D_1CE4[] = {
	OBJ_14C,
	OBJ_14D,
	OBJ_08F,
	OBJ_0FE,
	OBJ_0FF
};

/*IsReadableSign?*/
C_27A1_06A2(int objNum) {
	int si, di;

	di = GetType(objNum);
	for(si = 0; si < 5; si ++) {
		if(D_1CE4[si] == di)
			return 1;
	}
	return 0;
}

/*CanRead?*/
C_27A1_06D7(int si/*bp06*/) {
	int x, y;

	x = GetX(Party[Active]);
	y = GetY(Party[Active]);
	if(C_27A1_066D(si)) {
		if(GetCoordUse(Selection.obj) != LOCXYZ)
			return 1;
		if(CLOSE_ENOUGH(1, GetX(si), GetY(si), x, y))
			return 1;
		else
			return 0;
	}
	if(C_27A1_06A2(si))
		return 1;
	return 0;
}

/*read book or sign?*/
C_27A1_078F(int bp06) {
	int si, di;
	int bp_04, bp_02;

	MUS_091A(0);
	bp_04 = 0;
	di = GetQual(bp06);
	CON_printf(/*1D7D*/"\n\n");
	DISK_confirm(DISK_6);
	si = OSI_open(/*1D80*/"BOOK.DAT");
	OSI_read(si, (long)(bp_04 + di - 1) << 1, 2, &bp_02);
	OSI_read(si, (long)bp_02, 0x2800, TalkBuf);
	D_04DF->_attr |= 0x10;
	CON_printf(TalkBuf);
	D_049F = 1;
	OSI_close(si);
	MUS_09A8();
}

/*display object number?*/
C_27A1_0841(int objNum) {
	int si;
	int bp_02;

	bp_02 = QuanType(GetType(objNum));
	if(bp_02 == 4) {
		si = Amount[objNum];
		CON_printf(/*1D89*/"%d ", si);
	} else if(bp_02 == 2 && !IsLit(objNum)) {
		si = GetQuan(objNum);
		CON_printf(/*1D89*/"%d ", si);
	} else {
		si = 1;
		C_27A1_061E(TILE_FRAME(objNum));
	}
	return si;
}

C_27A1_08D3(int objNum) {
	int si, di;

	si = GetType(objNum);
	di = GetFrame(objNum);
	if(si == OBJ_062 || si == OBJ_0C0 || si == OBJ_0BA) {
		if(di)
			return 1;
	}
	if(si == OBJ_039)
		return 1;
	return 0;
}

C_27A1_0919(int objNum) {
	int si;

	for(si = FindLoc(GetX(objNum), GetY(objNum), MapZ); si >= 0; si = NextLoc()) {
		if(si == objNum)
			continue;
		if(!IsTileIg(TILE_FRAME(si))) {
			if(si > 0xff)
				break;
		}
	}
	return si;
}

/*searching ...*/
C_27A1_09A1(int objNum, int x, int y) {
	int di;
	int bp_0e, bp_0c, bp_0a, objTyp, objFrm;
	int found, bp_02;

	found = 0;
	bp_0e = Party[Active];
	bp_0c = GetX(bp_0e);
	bp_0a = GetY(bp_0e);
	objTyp = GetType(objNum);
	objFrm = GetFrame(objNum);
	if(!CLOSE_ENOUGH(1, x, y, bp_0c, bp_0a))
		return;
	if(objNum < 0x100)
		return;
	if(IsTerrainDamage(TILE_FRAME(objNum)))
		return;
	if(objTyp == OBJ_039)
		return;
	CON_printf(/*1D8D*/"\nSearching here, you find ");
	if(objTyp == OBJ_14E) {
		if(!(objFrm & 1)) {
			CON_printf(/*1DA8*/"a secret door");
			SetFrame(objNum, objFrm ^ 1);
			C_1100_0306();
			OtherAnimations();
			found = 1;
		}
	} else {
		if(IsTileIg(TILE_FRAME(objNum))) {
			objNum = C_27A1_0919(objNum);
			objTyp = GetType(objNum);
			objFrm = GetFrame(objNum);
		}
		if(!C_27A1_08D3(objNum)) {
			di = FindInv(objNum);
			while(di >= 0) {
				objTyp = GetType(di);
				if(objTyp != OBJ_150 && objTyp != OBJ_151) {
					MoveObj(di, GetX(objNum), GetY(objNum), MapZ);
					MoveObj(bp_0e, bp_0c, bp_0a, MapZ);
					C_1100_0306();
					OtherAnimations();
					for(
						bp_02 = FindInv(objNum);
						bp_02 >= 0 && (GetType(bp_02) == OBJ_150 || GetType(bp_02) == OBJ_151);
						bp_02 = NextInv()
					);
					if(found && bp_02 >= 0)
						CON_printf(/*1DB6*/", ");
					else if(found && bp_02 == -1)
						CON_printf(/*1DB9*/" and ");
					C_27A1_0841(di);
					CON_printf((char *)GetObjectString(di));
					found = 1;
					di = bp_02;
					continue;
				}
				di = NextInv();
			}
		}
	}
	if(!found) {
		CON_printf(/*1DBF*/"nothing.\n");
		return;
	}
	CON_printf(/*1DC6*/".\n");
}

/*"look"*/
C_27A1_0C67() {
	int objTyp, weight;
	unsigned char numTile, ch;
	int x, y, i, bp_16;
	int bp_14, bp_12, bp_10, spellNum, bp_0c;
	int bp_0a;
	unsigned char bp_07;
	int bp_06, bp_04, bp_02;

	bp_04 = 0;
	D_04DF->_attr |= 0x10;
	CON_printf(/*1DC9*/"Thou dost see ");
	bp_16 = MkDirection(PointerX, PointerY);
	if(bp_16 != -1) {
		C_1E0F_0664(Party[Active], bp_16);
		C_1100_0306();
	}
	if(D_B6DF) {
		CON_printf(/*1DD8*/"darkness.\n");
		D_049F = 1;
		return;
	}
	x = GetX(Party[Active]);
	y = GetY(Party[Active]);
	if(Selection.obj == -1 || IsInvisible(Selection.obj)) {
		if(PointerX < 168 && PointerY < 168) {
			Selection.x = MapX - 5 + SCR2TIL(PointerX);
			Selection.y = MapY - 5 + SCR2TIL(PointerY);
			numTile = GetTileAtXYZ(Selection.x, Selection.y, MapZ);
			C_27A1_061E(numTile);
			CON_printf((char *)GetTileString(numTile));
		} else {
			CON_printf(/*1DE3*/"nothing");
		}
		CON_printf(/*1DC6*/".\n");
		if(CLOSE_ENOUGH(1, Selection.x, Selection.y, x, y))
			CON_printf(/*1DEB*/"\nSearching here, you find nothing.\n");
		D_049F = 1;
		return;
	}
	objTyp = GetType(Selection.obj);
	if(Selection.obj < 0x100 || objTyp == OBJ_1AE)
		Selection.obj = COMBAT_getHead(Selection.obj);
	if(objTyp == OBJ_18D || objTyp == OBJ_18E || objTyp == OBJ_18F || Selection.obj < 0x100) {
		if(!IsPlrControl(Selection.obj) || IN_VEHICLE)
			C_27A1_0841(Selection.obj);
		CON_printf((char *)GetObjectString(Selection.obj));
		CON_putch('\n');
		if(!IN_VEHICLE)
			C_27A1_02D9(Selection.obj);
	} else {
		bp_02 = TILE_FRAME(Selection.obj);
		bp_14 = C_27A1_0841(Selection.obj);
		if(bp_02 == TIL_4DC)
			Selection.obj = FindLoc(Selection.x, Selection.y - 1, MapZ);
		else if(bp_02 == TIL_4DD)
			Selection.obj = FindLoc(Selection.x - 1, Selection.y, MapZ);
		CON_printf((char *)GetObjectString(Selection.obj));
		weight = GetWeight(Selection.obj) + Encumbrance(Selection.obj);
		if(weight != 0 && weight < 255) {
			if(bp_14 == 1)
				CON_printf(/*1E0F*/". It weighs");
			else
				CON_printf(/*1E1B*/". They weigh");
			CON_printf(/*1E28*/" %d.%d stones", weight / 10, weight % 10);
		}
		bp_14 = GetDamagePoints(Selection.obj);
		bp_12 = GetArmorPoints(Selection.obj);
		if(bp_14 > 0) {
			if(!(weight != 0 && weight < 255))
				CON_printf(/*1E36*/". It");
			else if(bp_12)
				CON_printf(/*1E3B*/",");
			else
				CON_printf(/*1E3D*/" and");
			CON_printf(/*1E42*/" can do %d point\\s of damage", bp_14);
		}
		if(bp_12 > 0)
			CON_printf(/*1E5F*/" and can absorb %d point\\s of damage", bp_12);
		if(C_27A1_06D7(Selection.obj)) {
			if(GetCoordUse(Selection.obj) != LOCXYZ) {
				if(GetQual(Selection.obj)) {
					if(!bp_04)
						CON_printf(/*1E84*/":");
					C_27A1_078F(Selection.obj);
					bp_04 = 1;
				}
			} else {
				for(bp_06 = FindLoc(Selection.x, Selection.y, MapZ); bp_06 >= 0; bp_06 = NextLoc()) {
					if(GetQual(bp_06) == 0)
						continue;
					if(!bp_04)
						CON_printf(/*1E84*/":");
					C_27A1_078F(bp_06);
					bp_04 = 1;
				}
			}
			CON_putch('\n');
		} else if(
			objTyp == OBJ_039 && FindInv(Selection.obj) >= 0 &&
			(
				CLOSE_ENOUGH(1, Selection.x, Selection.y, x, y) ||
				GetCoordUse(Selection.obj) != LOCXYZ
			)
		) {
			Spellbook[0] = Selection.obj;
			Spellbook[1] = 0;
			bp_0c = Active;
			bp_0a = GetAssoc(Selection.obj);
			for(i = 0; i < 8; i ++) {
				if(Party[i] == bp_0a) {
					Active = i;
					break;
				}
			}
			SPELL_RefreshList();
			SPELL_DrawBG();
			SPELL_RefreshPage();
			bp_10 = StatusDisplay;
			StatusDisplay = CMD_A1;
			ch = CON_getch();
			if(ch == CMD_8E && Selection.obj != -1) {
				spellNum = D_EBB6[Selection.obj];
				CON_printf(/*1D7D*/"\n\n");
				CON_printf(/*1E86*/"%s-", SpellName[spellNum]);
				i = 0;
				ch = SpellCode[spellNum][i++];
				CON_printf(/*1E8A*/"%s", D_0BF0[ch - 'A']);
				while(ch = SpellCode[spellNum][i++])
					CON_printf(/*1E8D*/" %s", D_0BF0[ch - 'A']);
				CON_printf(/*1E91*/"\nIngredients: \n");
				bp_07 = Reagents_needed[spellNum];
				if(bp_07 == 0)
					CON_printf(/*1EA1*/"None\n");
				for(i = 0; i < 8; i ++) {
					if((bp_07 >> i) & 1)
						CON_printf(/*1EA7*/" %s\n", Reagents_name[i]);
				}
			} else {
				CON_printf(/*1DC6*/".\n");
			}
			Active = bp_0c;
			StatusDisplay = bp_10;
			D_04B3 = 0;
			StatusDirty = 1;
		} else if(
			objTyp == OBJ_09F ||
			(objTyp == OBJ_0EB && Time_H >= 5 && Time_H < 20)
		) {
			CON_printf(/*1EAC*/"\n\nThe time is ");
			if(Time_H == 0)
				CON_printf(/*1EBB*/"12:%02d A.M.\n", Time_M);
			else if(Time_H < 12)
				CON_printf(/*1EC9*/"%d:%02d A.M.\n", Time_H, Time_M);
			else if(Time_H == 12)
				CON_printf(/*1ED7*/"12:%02d P.M.\n", Time_M);
			else
				CON_printf(/*1EE5*/"%d:%02d P.M.\n", Time_H - 12, Time_M);
		} else {
			CON_printf(/*1DC6*/".\n");
		}
	}
	if(!bp_04)
		C_27A1_09A1(Selection.obj, Selection.x, Selection.y);
	D_049F = 1;
}

static C_27A1_1330(int x, int y) {
	int objNum;

	for(objNum = FindLoc(x, y, MapZ); objNum >= 0; objNum = NextLoc()) {
		if(IsTileSu(TILE_FRAME(objNum)))
			return 1;
		if(IsTerrainImpass(TILE_FRAME(objNum)))
			return 0;
	}
	return 0;
}

C_27A1_13BE(int objNum) {
	int si, di;
	char bp_0e[5];
	int bp_08, bp_06, bp_04, bp_02;

	bp_08 = QuanType(GetType(objNum));
	if(bp_08 && GetCoordUse(objNum) != EQUIP) {
		if(bp_08 == 2)
			di = GetQuan(objNum);
		else
			di = Amount[objNum];
		if(di != 1) {
			bp_04 = Selection.obj;
			bp_06 = D_04DF->_cury;
			CON_printf(/*1EF3*/"How many? ");
			bp_02 = (char)SelectMode;
			C_0C9C_1AE5(0);
			SelectMode = 0;
			CON_gets(bp_0e, 4);
			SelectMode = bp_02;
			Selection.obj = bp_04;
			si = C_27A1_0205(&bp_0e, 0);
			if(si == -1) {
				CON_gotoxy(10, bp_06);
				CON_printf(/*1EFE*/"%d\n", di);
				return di;
			}
			if(si > di || si == 0 || si == -2) {
				CON_gotoxy(10, bp_06);
				CON_printf(/*1F02*/"0   \n");
				return 0;
			}
			CON_putch('\n');
			return si;
		}
	}
	si = 1;
	return si;
}

/*"drop"*/
C_27A1_14DA() {
	int si, di;
	int bp_16, bp_14, bp_12, bp_10, bp_0e;
	struct coord xyz;
	int bp_08, bp_06, bp_04, bp_02;

	if(Selection.obj == -1 || GetCoordUse(Selection.obj) == LOCXYZ || Selection.obj == D_E709) {
		CON_printf(/*1D6D*/"%s\n", D_0DDC[5]);
		return;
	}
	CON_printf((char *)GetObjectString(Selection.obj));
	CON_putch('\n');
	bp_16 = GetType(Selection.obj);
	bp_04 = QuanType(bp_16);
	di = Party[D_04B3];
	si = Selection.obj;
	bp_02 = C_155D_1666(si, di);
	if(bp_02 == 0) {
		bp_06 = C_27A1_13BE(Selection.obj);
		if(bp_06 == 0)
			return;
		CON_printf(/*1F08*/"Location: ");
		MouseMode = 1;
		if(SelectMode != 0) {
			C_0C9C_1AE5(0);
			SelectMode = 1;
		}
		CON_getch();
		CON_putch('\n');
		bp_0e = FindLoc(Selection.x, Selection.y, MapZ);
		if(Selection.x == -1 || Selection.y == -1) {
			CON_printf(/*1F13*/"\n%s\n", D_0DDC[6]);
			return;
		}
		if(!C_1E0F_000F(si, Selection.x, Selection.y) && !C_27A1_1330(Selection.x, Selection.y)) {
			if(
				bp_0e == -1 ||
				(bp_0e != -1 && !C_27A1_00A9(bp_0e, si))
			) {
				CON_printf(/*1F13*/"\n%s\n", D_0DDC[6]);
				return;
			}
		}
		if(!COMBAT_Missile(di, &Selection.x, &Selection.y, 8, 0)) {
			CON_printf(/*1F13*/"\n%s\n", D_0DDC[6]);
			return;
		}
		C_155D_16E7(si, di);
		if(GetCoordUse(si) == EQUIP) {
			Equipment[C_155D_13F4(si)] = 0;
			if(STAT_GetEquipSlot(si) == SLOT_2HND) {
				Party_1Hand[D_04B3] = 0;
				Equipment[SLOT_LHND] = 0;
			}
		}
		C_155D_0748(di);
		SubMov(di, 3);
		bp_14 = Selection.x;
		bp_12 = Selection.y;
		COMBAT_Missile(di, &bp_14, &bp_12, 8, TILE_FRAME(si));
		if(
			!CLOSE_ENOUGH(1, Selection.x, Selection.y, GetX(di), GetY(di)) &&
			C_27A1_012C(bp_16)
		) {
			if(bp_04 != 0) {
				if(bp_04 == 2)
					bp_08 = QualQuan(GetQual(si), bp_06);
				else
					bp_08 = bp_06;
				TakeObj(di, bp_16, bp_08);
			} else {
				if(bp_16 == OBJ_1A1) {
					SetFrame(si, 1);
					MoveObj(si, bp_14, bp_12, MapZ);
					SetOkToGet(si);
				} else if(bp_16 == OBJ_07B) {
					SetFrame(si, 2);
					MoveObj(si, bp_14, bp_12, MapZ);
					SetOkToGet(si);
				} else {
					DeleteObj(si);
				}
			}
			MUS_0065(15, 0);
			CON_printf(/*1F18*/"\nIt broke!\n");
		} else {
			if(C_27A1_00A9(bp_0e, si) && !C_27A1_1330(Selection.x, Selection.y)) {
				if(bp_04) {
					if(bp_04 == 2)
						bp_08 = QualQuan(GetQual(si), bp_06);
					else
						bp_08 = bp_06;
					bp_10 = GiveObj(bp_0e, bp_16, bp_08);
					if(bp_10 != -1)
						SetOkToGet(bp_10);
					TakeObj(di, bp_16, bp_08);
				} else {
					InsertObj(si, bp_0e, CONTAINED);
					SetOkToGet(si);
				}
			} else {
				if(bp_04) {
					if(bp_04 == 2)
						bp_08 = QualQuan(GetQual(si), bp_06);
					else
						bp_08 = bp_06;
					SetCoordXYZ(xyz, bp_14, bp_12, MapZ);
					bp_10 = AddObj(0, xyz, bp_16, 0, bp_08);
					SetOkToGet(bp_10);
					TakeObj(di, bp_16, bp_08);
				} else {
					MoveObj(si, bp_14, bp_12, MapZ);
					SetOkToGet(si);
				}
			}
		}
		C_1100_0306();
		StatusDirty = 0;
	}
	C_155D_1022(D_E709);
}

/*"get"*/
C_27A1_18F5() {
	int si, di;
	int weight, bp_0e, bp_0c, bp_0a, bp_08;
	int bp_06, tileNum, bp_02;

	bp_02 = 0;
	di = Party[Active];
	if(
		(Selection.obj <= 0xff && Selection.x != -1) ||
		(IsTileIg(TILE_FRAME(Selection.obj)))
	) Selection.obj = C_27A1_0919(Selection.obj);
	if(Selection.obj == -1 || GetCoordUse(Selection.obj) != LOCXYZ) {
		CON_printf(/*1D6D*/"%s\n", D_0DDC[5]);
		return;
	}
	bp_08 = MkDirection(PointerX, PointerY);
	if(bp_08 != -1) {
		C_1E0F_0664(di, bp_08);
		C_1100_0306();
	}
	C_27A1_0841(Selection.obj);
	CON_printf((char *)GetObjectString(Selection.obj));
	CON_putch('\n');
	bp_0c = GetX(di);
	bp_0a = GetY(di);
	if(!CLOSE_ENOUGH(1, Selection.x, Selection.y, bp_0c, bp_0a)) {
		CON_printf(/*1F24*/"\n%s!\n", D_0DDC[3]);
		return;
	}
	tileNum = TILE_FRAME(Selection.obj);
	if(IsTerrainDamage(tileNum)) {
		CON_printf(/*1F2A*/"\n%s", D_0DDC[6]);
		TerrainDamage(di, tileNum);
		CON_putch('\n');
		COMBAT_displaySeverity(di);
		return;
	}
	weight = GetWeight(Selection.obj) + Encumbrance(Selection.obj);
	bp_0e = STREN[Party[Active]];
	if(Active != D_04B3 || StatusDisplay != CMD_92)
		C_155D_0748(Party[Active]);
	si = GetType(Selection.obj);
	if(
		(weight == 0 && !C_155D_063A(si)) ||
		weight >= 10000 || Selection.obj < 0x100 || si == OBJ_19B
	) {
		CON_printf(/*1F13*/"\n%s\n", D_0DDC[6]);
	} else if(bp_0e * 20 < weight + WeightInven + WeightEquip) {
		CON_printf(/*1F2E*/"\nThe total is too heavy.\n");
	} else {
		SubMov(di, 3);
		if(QuanType(si) && (si != OBJ_05A || GetFrame(Selection.obj) == 0)) {
			if(GiveObj(di, si, Amount[Selection.obj]) >= 0)
				DeleteObj(Selection.obj);
		} else if(si == OBJ_05A && GetFrame(Selection.obj) == 1) {
			if(Active == D_04B3) {
				if(Equipment[SLOT_RHND] == 0) {
					Equipment[SLOT_RHND] = Selection.obj;
				} else if(Equipment[SLOT_LHND] == 0) {
					Equipment[SLOT_LHND] = Selection.obj;
				} else {
					CON_printf(/*1F48*/"No free hand\n");
					return;
				}
			}
			InsertObj(Selection.obj, di, EQUIP);
		} else {
			InsertObj(Selection.obj, di, INVEN);
			if(si == OBJ_07A || si == OBJ_091) {
				SetFrame(Selection.obj, 0);
			} else if(si == OBJ_062 && GetFrame(Selection.obj) == 0) {
				SetFrame(Selection.obj, 1);
			} else if(si == OBJ_049) {
				bp_06 = GetFrame(Selection.obj);
				D_2C74[bp_06][0] = 0;
				D_2C74[bp_06][1] = 0;
				D_2C74[bp_06][2] = 0;
			} else if(si == OBJ_1A7) {
				SetType(Selection.obj, OBJ_1A4);
			}
		}
		if(Active == D_04B3 && StatusDisplay == CMD_92)
			C_155D_1022(Party[D_04B3]);
		if(!IsOkToGet(Selection.obj) && MapZ == 0) {
			SetOkToGet(Selection.obj);
			SubKarma(1);
			MUS_0065(13, 0);
			for(bp_06 = 1; bp_06 < 0xe0; bp_06 ++) {
				if(
					GetAlignment(bp_06) == NEUTRAL &&
					C_1703_0153(bp_06) &&
					!IsAsleep(bp_06)
				) {
					CON_printf(/*1F56*/"\n\"Stop Thief!!!\"\n");
					C_2337_1B0E();
					bp_02 = 1;
					break;
				}
			}
			if(!bp_02)
				CON_printf(/*1F68*/"\nStealing!!!\n");
		}
		C_1100_0306();
	}
	if(Active != D_04B3)
		C_155D_0748(Party[D_04B3]);
	MouseMode = 0;
}

C_27A1_1DAB(int bp0c, int bp0a, int bp08, int si/*bp06*/) {
	int di;
	int bp_0a, bp_08, bp_06, bp_04, bp_02;

	di = DirIncrX[si] + bp0a;
	bp_0a = DirIncrY[si] + bp08;
	if(!C_1E0F_000F(bp0c, di, bp_0a) && !C_27A1_1330(di, bp_0a))
		return 1;
	if(!(si & 1))
		return 0;
	bp_08 = DirIncrX[si-1] + bp0a;
	bp_06 = DirIncrY[si-1] + bp08;
	bp_04 = DirIncrX[si+1] + bp0a;
	bp_02 = DirIncrY[si+1] + bp08;
	if(C_1E0F_000F(bp0c, bp_08, bp_06) || C_27A1_1330(bp_08, bp_06))
		return 0;
	if(C_1E0F_000F(bp0c, bp_04, bp_02) || C_27A1_1330(bp_04, bp_02))
		return 0;
	return 1;
}

C_27A1_1E69(int objNum) {
	do
		objNum = GetAssoc(objNum);
	while(GetCoordUse(objNum) != LOCXYZ);
	return objNum;
}

/*"move"*/
C_27A1_1E8B() {
	/*1c*/
	int si, di;
	unsigned char bp_1b;
	int bp_1a, bp_18, bp_16, bp_14, bp_12, bp_10;
	int bp_0e, bp_0c, bp_0a, bp_08, bp_06, bp_04, bp_02;

	if(Selection.obj == -1 || Selection.obj == D_E709) {
		CON_printf(/*1D6D*/"%s\n", D_0DDC[5]);
		return;
	}
	CON_printf((char *)GetObjectString(Selection.obj));
	CON_putch('\n');
	di = GetType(Selection.obj);
	bp_1a = QuanType(di);
	if(GetCoordUse(Selection.obj) == LOCXYZ) {
		if(!CLOSE_ENOUGH(1, Selection.x, Selection.y, MapX, MapY)) {
			CON_printf(/*1F24*/"\n%s!\n", D_0DDC[3]);
			return;
		}
		bp_10 = MkDirection(PointerX, PointerY);
		if(bp_10 != -1) {
			C_1E0F_0664(Party[Active], bp_10);
			C_1100_0306();
		}
		bp_14 = GetX(Selection.obj);
		bp_12 = GetY(Selection.obj);
		MouseMode = 0;
		D_04CA = 88 + TIL2SCR(bp_14 - MapX);
		D_04CB = 88 + TIL2SCR(bp_12 - MapY);
		CON_printf(/*1F76*/"To ");
		if(CON_getch() != CMD_80) {
			CON_printf(/*1F7A*/"nowhere.\n");
			MouseMode = 0;
			D_04CA = 88;
			D_04CB = 88;
			C_0C9C_01FB();
			return;
		}
		CON_printf(/*1F84*/"%s.\n", DirNames[AdvanceDir]);
		bp_04 = Party[Active];
		bp_0e = GetWeight(Selection.obj) + Encumbrance(Selection.obj);
		if(TypeWeight[GetType(Selection.obj)] == 0 || GetType(Selection.obj) == OBJ_19B) {
			CON_printf(/*1F13*/"\n%s\n", D_0DDC[6]);
			goto c_2237;
		}
		if(Selection.obj < 0x100 && (C_1944_01EC(bp_04, Selection.obj) || bp_04 == Selection.obj ||
			((GetFrame(Selection.obj) & 3) == 3 && GetType(Selection.obj) != OBJ_153))) {
			CON_printf(/*1F13*/"\n%s\n", D_0DDC[1]);
			MouseMode = 0;
			D_04CA = 88;
			D_04CB = 88;
			C_0C9C_01FB();
			return;
		}
		SubMov(Party[Active], 5);
		bp_18 = DirIncrX[AdvanceDir] + bp_14;
		bp_16 = DirIncrY[AdvanceDir] + bp_12;
		bp_06 = FindLoc(bp_18, bp_16, MapZ);
		if(bp_06 != -1 && C_27A1_00A9(bp_06, Selection.obj) && bp_0e < 0xff) {
			InsertObj(Selection.obj, bp_06, CONTAINED);
		} else {
			if(C_27A1_1DAB(Selection.obj, bp_14, bp_12, AdvanceDir)) {
				CON_printf(/*1F13*/"\n%s\n", D_0DDC[4]);
				if(GetType(Selection.obj) == OBJ_0DD &&
					((GetFrame(Selection.obj) != (AdvanceDir >> 1)) & 3)/*parenthesis problem:bug?*/
					) {
					SetFrame(Selection.obj, (AdvanceDir >> 1) & 3);
				}
/*c_21a8:*/
				MouseMode = 0;
				D_04CA = 88;
				D_04CB = 88;
				C_0C9C_01FB();
				return;
			}
/*21c0*/
			if(GetType(Selection.obj) == OBJ_0DD &&
				((GetFrame(Selection.obj) != (AdvanceDir >> 1)) & 3)/*parenthesis problem:bug?*/
				) {
				SetFrame(Selection.obj, (AdvanceDir >> 1) & 3);
			} else {
				MoveObj(Selection.obj, bp_18, bp_16, MapZ);
			}
		}
/*c_2232:*/
		C_1100_0306();
c_2237:
		D_04CA = 88;
		D_04CB = 88;
		C_0C9C_01FB();
	} else {
		bp_02 = C_155D_1666(Selection.obj, Party[D_04B3]);
		if(bp_02) {
			C_155D_1022(D_E709);
			return;
		}
		if(!C_1703_0153(Party[D_04B3])) {
			CON_printf(/*1F89*/"\nThis person is not around!\n");
			return;
		}
		bp_0c = C_27A1_13BE(Selection.obj);
		if(!bp_0c)
			return;
		CON_printf(/*1F76*/"To ");
		si = Selection.obj;
		bp_08 = D_04B3;
		if(di == OBJ_04C) {
			CON_printf(/*1FA6*/"\n\n%s\n", D_0DDC[12]);
			return;
		}
		if(SelectMode != 0) {
			SelectRange = 7;
			C_0C9C_1AE5(0);
			SelectMode = 1;
		}
		bp_1b = CON_getch();
		if(bp_1b != CMD_8E || Selection.obj == -1) {
			CON_printf(/*1FAC*/"nobody.\n");
			return;
		}
		if(GetCoordUse(Selection.obj) == LOCXYZ && (
			(Selection.obj < 0x100 && !C_1703_0153(Selection.obj)) ||
			(Selection.obj >= 0x100 && !CLOSE_ENOUGH(1, Selection.x, Selection.y, MapX, MapY)))) {
			CON_printf(/*1FB5*/"\n\n%s!\n", D_0DDC[3]);
			return;
		}
		if(Selection.obj < 0x100) {
			if(Party[bp_08] != Selection.obj) {
				CON_printf((char *)GetObjectString(Selection.obj));
				CON_printf(/*1DC6*/".\n");
				if(!C_1703_0153(Selection.obj)) {
					CON_printf(/*1F89*/"\nThis person is not around!\n");
					return;
				}
				if(!IsPlrControl(Selection.obj)) {
					CON_printf(/*1FBC*/"\nOnly within the party!\n");
					return;
				}
				C_155D_0748(Selection.obj);
				if(bp_1a) {
					if(C_155D_063A(di))
						bp_0e = TypeWeight[di] * bp_0c / 10;
					else
						bp_0e = TypeWeight[di] * bp_0c;
				} else {
					bp_0e = GetWeight(si) + Encumbrance(si);
				}
				if(STREN[Selection.obj] * 20 < bp_0e + WeightInven + WeightEquip) {
					CON_printf(/*1FD5*/"\nCan't carry!\n");
					C_155D_0748(Party[D_04B3]);
					return;
				}
				C_155D_16E7(si, Party[bp_08]);
				if(GetCoordUse(si) == EQUIP && Party[bp_08] != Selection.obj) {
					Equipment[C_155D_13F4(si)] = 0;
					if(STAT_GetEquipSlot(si) == SLOT_2HND) {
						Party_1Hand[bp_08] = 0;
						Equipment[SLOT_LHND] = 0;
					}
				}
				if(bp_1a) {
					if(bp_1a == 2)
						bp_0a = QualQuan(GetQual(si), bp_0c);
					else
						bp_0a = bp_0c;
					GiveObj(Selection.obj, di, bp_0a);
					TakeObj(GetAssoc(si), di, bp_0a);
				} else {
					InsertObj(si, Selection.obj, INVEN);
				}
				C_155D_0748(Party[D_04B3]);
			} else {
				CON_printf(/*1FE4*/"yourself.\n");
			}
			bp_10 = MkDirection(PointerX, PointerY);
			if(bp_10 != -1) {
				C_1E0F_0664(Party[Active], bp_10);
				C_1100_0306();
			}
			SubMov(Party[Active], 5);
		} else {
			if(GetCoordUse(Selection.obj) == LOCXYZ) {
				CON_printf(/*1FAC*/"nobody.\n");
				return;
			}
			if(!C_27A1_0082(GetType(Selection.obj))) {
				CON_printf(/*1FEF*/"\n\nnot a container\n");
				return;
			}
			if(C_27A1_1E69(Selection.obj) != C_27A1_1E69(si)) {
				CON_printf(/*2002*/"\n\nCan't move to another person's bag.\n");
				return;
			}
			if(si < 0x100) {
				CON_printf(/*2029*/"\n\nIt's too big to fit inside a container.\n");
				return;
			}
			if(GetCoordUse(si) == EQUIP) {
				CON_printf(/*2054*/"\n\nYou have to unready it first.\n");
				return;
			}
			if(Selection.obj == si) {
				CON_printf(/*2075*/"\n\nHow can a container go into itself!\n");
				return;
			}
			if(GetType(Selection.obj) == OBJ_039 && GetType(si) != OBJ_03A) {
				CON_printf(/*209C*/"\n\nOnly spells can go into the spellbook.\n");
				return;
			}
			if(GetType(Selection.obj) == OBJ_03E && GetType(si) != OBJ_049) {
				CON_printf(/*20C6*/"\n\nOnly moonstones can go into the vortex cube.\n");
				return;
			}
			if(GetAssoc(si) == Selection.obj) {
				if(GetType(Selection.obj) == OBJ_039 && GetType(si) == OBJ_03A) {
					CON_printf(/*20F6*/"\n\nSpells are not suppose to be taken out from the Spellbook.\n");
					return;
				}
				CON_printf(/*2134*/"out of ");
				SubMov(Party[Active], 5);
				if(bp_1a) {
					if(bp_1a == 2) {
						if(GetQuan(si) == bp_0c)
							DeleteObj(si);
						else
							SubQuan(si, bp_0c);
						bp_0a = QualQuan(GetQual(si), bp_0c);
					} else {
						if(Amount[si] == bp_0c)
							DeleteObj(si);
						else
							Amount[si] -= bp_0c;
						bp_0a = bp_0c;
					}
					GiveObj(GetAssoc(Selection.obj), di, bp_0a);
				} else {
					InsertObj(si, GetAssoc(Selection.obj), CONTAINED);
				}
			} else {
				CON_printf(/*213C*/"into ");
				SubMov(Party[Active], 5);
				if(bp_1a) {
					if(bp_1a == 2) {
						if(GetQuan(si) == bp_0c)
							DeleteObj(si);
						else
							SubQuan(si, bp_0c);
						bp_0a = QualQuan(GetQual(si), bp_0c);
					} else {
						if(Amount[si] == bp_0c)
							DeleteObj(si);
						else
							Amount[si] -= bp_0c;
						bp_0a = bp_0c;
					}
					GiveObj(Selection.obj, di, bp_0a);
				} else {
					InsertObj(si, Selection.obj, CONTAINED);
				}
			}
			InvenSize --;
			CON_printf((char *)GetObjectString(Selection.obj));
			CON_putch('\n');
		}
		if(StatusDisplay == CMD_92)
			C_155D_1022(D_E709);
	}
/*c_2892*/
	StatusDirty = 0;
	MouseMode = 0;
}

/*trapped object(door, chest, ...)*/
C_27A1_28A3(int objNum) {
	int si;
	int bp_08, x, y;
	int far *bp_04;

	if(GetCoordUse(objNum) == LOCXYZ) {
		si = C_27A1_0008(Active);
		x = C_27A1_0028(objNum);
		y = C_27A1_0051(objNum);
	} else {
		si = C_27A1_0008(D_04B3);
		x = C_27A1_0028(si);
		y = C_27A1_0051(si);
	}
	bp_08 = OSI_rand(0, 3);
	switch(bp_08) {
		case 0:
			CON_printf(/*2142*/"Acid!\n");
			LooseHP(si, OSI_rand(1, 20));
			CON_putch('\n');
			COMBAT_displaySeverity(si);
		break;
		case 1:
			CON_printf(/*2149*/"Poison!\n");
			SetPoisoned(si);
			COMBAT_showHit(si);
		break;
		case 2:
			CON_printf(/*2152*/"Bomb!\n");
			for(bp_04 = Explosion(x, y, TIL_17E); *bp_04 >= 0; bp_04 ++) {
				if(*bp_04 < 0x100) {
					LooseHP(*bp_04, OSI_rand(1, 20));
					CON_putch('\n');
					COMBAT_displaySeverity(*bp_04);
				}
			}
		break;
		case 3:
			CON_printf(/*2159*/"Gas!\n");
			for(bp_04 = Explosion(x, y, TIL_17C); *bp_04 >= 0; bp_04 ++) {
				if(*bp_04 < 0x100) {
					SetPoisoned(*bp_04);
					COMBAT_showHit(*bp_04);
				}
			}
		break;
	}
	DeleteObj(C_1944_0A43(objNum, OBJ_151, SPELL_16));
	if(StatusDisplay == CMD_91 || StatusDisplay == CMD_90)
		StatusDirty ++;
}

/*use door*/
C_27A1_2A44(int objNum, int bp0a, int bp08, int bp06) {
	int di;

	CON_putch('\n');
	di = GetFrame(objNum);
	if(di < 4) {
		if(!bp0a && !bp08 && !bp06) {
			CON_printf(/*215F*/"closed!\n");
			SetFrame(objNum, di ^ 4);
		} else {
			CON_printf(/*2168*/"Can't (Un)lock an opened door\n");
		}
	} else if(di >= 4 && di < 8) {
		if(bp0a) {
			CON_printf(/*2187*/"locked!\n");
			SetFrame(objNum, di ^ 0xc);
		} else if(bp08) {
			CON_printf(/*2190*/"magically locked!\n");
			SetFrame(objNum, di ^ 8);
		} else {
			if(FindInvType(objNum, OBJ_151, SPELL_16) != -1)
				C_27A1_28A3(objNum);
			else
				CON_printf(/*21A3*/"opened!\n");
			SetFrame(objNum, di ^ 4);
		}
	} else if(di >= 8 && di < 0xc) {
		if(bp0a) {
			CON_printf(/*21AC*/"unlocked!\n");
			SetFrame(objNum, di ^ 0xc);
		} else if(bp08) {
			CON_printf(/*2190*/"magically locked!\n");
			SetFrame(objNum, di ^ 4);
		} else {
			CON_printf(/*21B7*/"locked\n");
		}
	} else {
		if(bp06) {
			CON_printf(/*21AC*/"unlocked!\n");
			SetFrame(objNum, di ^ 8);
		} else {
			CON_printf(/*2190*/"magically locked!\n");
		}
	}
}

/*use chest?*/
C_27A1_2BBC(int objNum, int bp0a, int bp08, int bp06) {
	int di;

	CON_putch('\n');
	di = GetFrame(objNum);
	if(di == 0) {
		if(!bp0a && !bp08 && !bp06) {
			CON_printf(/*215F*/"closed!\n");
			SetFrame(objNum, 1);
		} else {
			CON_printf(/*1D6D*/"%s\n", D_0DDC[2]);
		}
	} else if(di == 1) {
		if(!bp0a && !bp08 && !bp06) {
			if(FindInvType(objNum, OBJ_151, SPELL_16) != -1)
				C_27A1_28A3(objNum);
			else
				CON_printf(/*21A3*/"opened!\n");
			SetFrame(objNum, 0);
			C_27A1_09A1(objNum, GetX(objNum), GetY(objNum));
		} else if(bp0a) {
			CON_printf(/*2187*/"locked!\n");
			SetFrame(objNum, 2);
		} else if(bp08) {
			CON_printf(/*2190*/"magically locked!\n");
			SetFrame(objNum, 3);
		} else {
			CON_printf(/*1D6D*/"%s\n", D_0DDC[2]);
		}
	} else if(di == 2) {
		if(bp0a) {
			CON_printf(/*21AC*/"unlocked!\n");
			SetFrame(objNum, 1);
		} else
		if(bp08) {
			CON_printf(/*2190*/"magically locked!\n");
			SetFrame(objNum, 3);
		} else {
			CON_printf(/*1D6D*/"%s\n", D_0DDC[2]);
		}
	} else {
		if(bp06) {
			CON_printf(/*21AC*/"unlocked!\n");
			SetFrame(objNum, 1);
		} else {
			CON_printf(/*1D6D*/"%s\n", D_0DDC[2]);
		}
	}
	if(GetCoordUse(objNum) != LOCXYZ)
		C_155D_0CF5();
}

/*dexterity test?*/
C_27A1_2D34(int bp06) {
	return OSI_rand(1, 30) < ((45 - bp06) / 2);
}

/*intelligence test?*/
C_27A1_2D61(int bp06) {
	return OSI_rand(1, 30) < ((45 - bp06) / 2);
}

/*use lockpick/key*/
C_27A1_2D8E(int objNum) {
	int di;
	int bp_0a, x, y, bp_04, bp_02;

	bp_02 = Party[Active];
	bp_04 = GetType(objNum);
	CON_printf(/*21BF*/"On ");
	if(SelectMode != 0) {
		C_0C9C_1AE5(0);
		SelectMode = 1;
	}
	bp_0a = CON_getch();
	if(bp_0a == CMD_8E && Selection.obj != -1) {
		CON_printf((char *)GetObjectString(Selection.obj));
		CON_putch('\n');
		di = GetType(Selection.obj);
		x = GetX(bp_02);
		y = GetY(bp_02);
		if(GetCoordUse(Selection.obj) == LOCXYZ && !CLOSE_ENOUGH(1, Selection.x, Selection.y, x, y)) {
			CON_printf(/*1F25*/"%s!\n", D_0DDC[3]);
		} else if((di >= OBJ_129 && di <= OBJ_12C) || di == OBJ_062) {
			if(bp_04 == OBJ_03F && C_27A1_2D34(GetDex(bp_02))) {
				MUS_0065(13, 0);
				CON_printf(/*21C3*/"\nKey broke.\n");
				if(GetQuan(objNum) == 1)
					DeleteObj(objNum);
				else
					SetQuan(objNum, GetQuan(objNum) - 1);
				if(StatusDisplay == CMD_92)
					C_155D_0CF5();
			} else if(
				(bp_04 == OBJ_040 && GetQual(Selection.obj) && GetQual(objNum) == GetQual(Selection.obj)) ||
				(bp_04 == OBJ_03F && GetQual(Selection.obj) == 0)
			) {
				if(di == OBJ_062)
					C_27A1_2BBC(Selection.obj, 1, 0, 0);
				else
					C_27A1_2A44(Selection.obj, 1, 0, 0);
			} else {
				CON_printf(/*1F13*/"\n%s\n", D_0DDC[2]);
			}
		} else {
			CON_printf(/*1D6D*/"%s\n", D_0DDC[2]);
		}
	} else {
		CON_printf(/*1D6D*/"%s\n", D_0DDC[5]);
	}
}

/*use torch?*/
C_27A1_2FD1(int objNum) {
	int di;
	int bp_06;
	struct coord xyz;

	bp_06 = GetQuan(objNum);
	if(IsLit(objNum)) {
		DeleteObj(objNum);
		CON_putch('\n');
		CON_printf(BurnOutMsg);
		return;
	}
	if(Equipment[SLOT_RHND] && Equipment[SLOT_LHND]) {
		CON_printf(/*21D0*/"\nNo free hand to hold the torch.\n");
	} else {
		if(bp_06 > 1) {
			di = objNum;
			if(GetCoordUse(objNum) == LOCXYZ) {
				SetCoordXYZ(xyz, GetX(di), GetY(di), MapZ);
				objNum = AddObj(GetCoordUse(di), xyz, OBJ_05A, GetFrame(di), QualQuan(GetQual(di), 1));
			} else {
				objNum = AddInvObj(OBJ_05A, EQUIP, 1, Party[D_04B3]);
			}
			SetQuan(di, bp_06 - 1);
		}
		if(GetCoordUse(objNum) != LOCXYZ) {
			if(GetCoordUse(objNum) == INVEN || GetCoordUse(objNum) == CONTAINED) {
				if(Equipment[SLOT_RHND] == 0)
					Equipment[SLOT_RHND] = objNum;
				else
					Equipment[SLOT_LHND] = objNum;
				SetCoordUse(objNum, EQUIP);
				C_155D_0748(Party[D_04B3]);
			}
		}
		SetLit(objNum);
		SetFrame(objNum, 1);
		SetQuan(objNum, 200);
		C_0A33_1355(0);
		CON_printf(/*21F2*/"\nTorch is lit.\n");
	}
	if(StatusDisplay == CMD_92)
		C_155D_0CF5();
}

C_27A1_319F(int objNum) {
	int di;

	C_2FC1_1FD6();/*peer?*/
	if(objNum) {
		if((di = GetQuan(objNum)) > 1)
			SetQuan(objNum, di - 1);
		else
			DeleteObj(objNum);
	}
	if(StatusDisplay == CMD_92)
		C_155D_0CF5();
}

C_27A1_31F6(int objNum) {
	int di;
	int bp_02;

	bp_02 = GetType(objNum);
	di = GetFrame(objNum);
	CON_putch('\n');
	switch(bp_02) {
		case OBJ_0A4: CON_printf(/*2202*/"fireplace"); break;
		case OBJ_0CE: CON_printf(/*220C*/"brazier"); break;
		case OBJ_091: CON_printf(/*2214*/"candelabra"); break;
		case OBJ_0FD: CON_printf(/*221F*/"campfire"); break;
		case OBJ_07A: CON_printf(/*2228*/"candle"); break;
	}
	if(di & 1) {
		SetFrame(objNum, di & 2);
		CON_printf(/*222F*/" is doused.\n");
	} else {
		SetFrame(objNum, di | 1);
		CON_printf(/*21F8*/" is lit.\n");
	}
}

C_27A1_32FA(int objNum) {
	int di;

	di = GetFrame(objNum);
	if(di & 1)
		SetFrame(objNum, di & 2);
	else
		SetFrame(objNum, di | 1);
}

/*scale frequencies*/
int D_1CEE[] = {0x1EAB,0x0C2C,0x0DA9,0x0F56,0x103F,0x123C,0x1478,0x16FA,0x1857,0x1B53};

/*"music instrument" interface?*/
static C_27A1_335A(int bp06) {
	int si;

	SelectMode = 0;
	while(isdigit(si = CON_getch()))
		MUS_0065(bp06, D_1CEE[si - '0']);
}

/*use bell/pull chain*/
static C_27A1_338D() {
	int si, di;

	for(si = 0; si < NumAnimData; si ++) {
		if(D_67E4[si] == BaseTile[OBJ_0EC]) {
			for(di = 0; di < 0x10; di ++) {
				GR_4B(D_67E4[si], D_9DF5[si] + (di >> 2));
				C_0A33_09CE(1);
				PaletteAnimation();
			}
			GR_4B(D_67E4[si], D_9DF5[si]);
			break;
		}
	}
	OtherAnimations();
	OSI_playWavedNote(2000, 1, 16000, 20000, -1);
}

/*use moonstone?*/
C_27A1_3425(int objNum) {
	int x, y;
	int numTile, bp_02;

	x = GetX(Party[Active]);
	y = GetY(Party[Active]);
	numTile = GetTileAtXYZ(x, y, MapZ);
	if(
		(numTile >= TIL_001 && numTile <= TIL_007) ||
		(numTile >= TIL_010 && numTile <= TIL_06F)
	) {
		CON_printf(/*223C*/"buried.\n");
		bp_02 = GetFrame(objNum);
		D_2C74[bp_02][0] = x;
		D_2C74[bp_02][1] = y;
		D_2C74[bp_02][2] = MapZ;
		C_155D_0748(Party[D_04B3]);
		MoveObj(objNum, x, y, MapZ);
		MoveObj(Party[Active], x, y, MapZ);
		if(StatusDisplay == CMD_92)
			C_155D_0CF5();
	} else {
		CON_printf(/*2245*/"Cannot be buried here!\n");
	}
}

static C_27A1_3537(int objNum) {
	int spellNum, di;
	int bp_04;
	unsigned char bp_01;

	bp_04 = FindInv(objNum);
	if(bp_04  == -1) {
		CON_printf(/*1F13*/"\n%s\n", D_0DDC[2]);
		return;
	}
	spellNum = GetQual(bp_04);
	CON_printf(/*225D*/"With ");
	di = 0;
	bp_01 = SpellCode[spellNum][di++];
	CON_printf(/*1E8A*/"%s", D_0BF0[bp_01 - 'A']);
	while(bp_01 = SpellCode[spellNum][di++])
		CON_printf(/*1E8D*/" %s", D_0BF0[bp_01 - 'A']);
	if(SelectMode != 0)
		SelectRange = 7;
	C_1944_43D5(spellNum, 0);
	TakeObj(objNum, OBJ_150, QualQuan(spellNum, 1));
}

/*uncalled?*/
__27A1_3617() {
	if(SelectMode != 0)
		SelectRange = 7;
	C_1944_12FC(SPELL_22, TIL_17E, 8, 1, 0);/*fireball?*/
}

/*uncalled?*/
__27A1_363C() {
	if(SelectMode != 0)
		SelectRange = 7;
	C_1944_12FC(SPELL_44, TIL_188, 8, 1, 0);/*lightning?*/
}

C_27A1_3661() {
	int si, di;

	si = C_27A1_0008(Active);
	di = FindInvType(si, OBJ_0B2, 0);
	if(di == -1) {
		CON_printf(/*2263*/"\nYou need a bucket.\n");
		return;
	}
	C_155D_0748(si);
	if(STREN[si] * 20 < (WeightInven + WeightEquip + 32)) {
		CON_printf(/*2278*/"\nThe bucket of water is too heavy for you.\n");
		return;
	}
	DeleteObj(di);
	GiveObj(si, OBJ_0B3, 1);/*bucket of water*/
	CON_printf(/*1F13*/"\n%s\n", D_0DDC[7]);
}

/*use cow?*/
C_27A1_36E7() {
	int si, di;

	si = C_27A1_0008(Active);
	di = FindInvType(si, OBJ_0B2, 0);
	if(di == -1) {
		CON_printf(/*2263*/"\nYou need a bucket.\n");
		return;
	}
	C_155D_0748(si);
	if(STREN[si] * 20 < (WeightInven + WeightEquip + 32)) {
		CON_printf(/*22A4*/"\nThe bucket of milk is too heavy for you.\n");
		return;
	}
	DeleteObj(di);
	GiveObj(si, OBJ_0B4, 1);
	CON_printf(/*1F13*/"\n%s\n", D_0DDC[7]);
}

/*use churn?*/
C_27A1_376D() {
	int si, di;

	si = C_27A1_0008(Active);
	di = FindInvType(si, OBJ_0B4, 0);
	if(di == -1) {
		CON_printf(/*22CF*/"\nYou need some milk.\n");
		return;
	}
	DeleteObj(di);
	GiveObj(si, OBJ_060, 1);/*butter*/
	GiveObj(si, OBJ_0B2, 1);
	CON_printf(/*1F13*/"\n%s\n", D_0DDC[7]);
}

/*use beehive?*/
C_27A1_37D3() {
	int si, di;

	di = C_27A1_0008(Active);
	si = FindInvType(di, OBJ_0B7, 0);
	if(si == -1) {
		CON_printf(/*22E5*/"\nYou need a honey jar.\n");
		return;
	}
	SetType(si, OBJ_0B8);/*full honey jar*/
	CON_printf(/*1F13*/"\n%s\n", D_0DDC[7]);
	C_155D_0CF5();
}

/*use potion?*/
C_27A1_3832(int objNum) {
	int si;

	CON_printf(/*1E8A*/"%s", D_0DDC[9]);
	MouseMode = 1;
	if(SelectMode != 0) {
		C_0C9C_1AE5(0);
		SelectMode = 1;
		SelectRange = 7;
	}
	CON_getch();
	if(Selection.obj == -1 || Selection.x == -1 || Selection.y == -1) {
		CON_printf(/*1D6D*/"%s\n", D_0DDC[5]);
		return;
	}
	CON_printf((char *)GetObjectString(Selection.obj));
	CON_putch('\n');
	if(!IsPlrControl(Selection.obj)) {
		CON_printf(/*1D6D*/"%s\n", D_0DDC[2]);
		return;
	}
	si = GetFrame(objNum);
	switch(si) {
		case 0:/*orange - awaken*/
			if(IsAsleep(Selection.obj)) {
				ClrAsleep(Selection.obj);
				ObjShapeType[Selection.obj] = OrigShapeType[Selection.obj];
			} else {
				CON_printf(/*1F13*/"\n%s\n", D_0DDC[2]);
			}
		break;
		case 1:/*red - cure*/
			if(IsPoisoned(Selection.obj)) {
				ClrPoisoned(Selection.obj);
			} else {
				CON_printf(/*1F13*/"\n%s\n", D_0DDC[2]);
			}
		break;
		case 2:/*yellow - heal*/
			C_1944_114D(SPELL_06);
		break;
		case 3:/*green - poison*/
			if(!IsMonster_0004(GetMonsterClass(GetType(Selection.obj)))) {
				SetPoisoned(Selection.obj);
				COMBAT_showHit(Selection.obj);
			} else {
				CON_printf(/*1F13*/"\n%s\n", D_0DDC[2]);
			}
		break;
		case 4:/*blue - sleep*/
			/*bug?should be IsMonster_0001?*/
			if(!IsMonster_0080(GetMonsterClass(GetType(Selection.obj)))) {
				COMBAT_showHit(Selection.obj);
				SetAsleep(Selection.obj);
				SetTypeUnconscious(Selection.obj);
			} else {
				CON_printf(/*1F13*/"\n%s\n", D_0DDC[2]);
			}
		break;
		case 5:/*purple - protection*/
			if(!IsProtected(Selection.obj)) {
				SetProtected(Selection.obj);
				MagicalHalo(Selection.obj, 1);
			} else {
				CON_printf(/*1F13*/"\n%s\n", D_0DDC[2]);
			}
		break;
		case 6:/*black - invisibility*/
			if(!IsInvisible(Selection.obj)) {
				SetInvisible(Selection.obj);
				MagicalHalo(Selection.obj, 1);
			} else {
				CON_printf(/*1F13*/"\n%s\n", D_0DDC[2]);
			}
		break;
		case 7:/*white - xray vision*/
			C_1944_310C(SPELL_49);
		break;
	}
	DeleteObj(objNum);
	if(StatusDisplay == CMD_91 || StatusDisplay == CMD_90)
		StatusDirty ++;
}

char D_1D02[] = { 0, 1, 0, -1};
char D_1D06[] = {-1, 0, 1,  0};

C_27A1_3A35(int objNum) {
	int si, di;
	int x, y, bp_04, bp_02;

	CON_printf(/*22FD*/"\nFire!\n");
	si = GetFrame(objNum);
	x = GetX(objNum);
	y = GetY(objNum);
	for(di = 1; di < 6; di ++) {
		bp_04 = FindLoc(D_1D02[si] * di + x, D_1D06[si] * di + y, MapZ);
		if(bp_04 >= 0 && bp_04 < 0x100)
			break;
	}
	x += D_1D02[si] * di;
	y += D_1D06[si] * di;
	bp_02 = COMBAT_Missile(objNum, &x, &y, 5, TIL_18F);
	if(!bp_02) {
		D_EBFE[0] += D_1D02[si];
		D_EBFE[1] += D_1D06[si];
		Explosion(D_EBFE[0], D_EBFE[1], TIL_189);
	}
	bp_04 = FindLoc(D_EBFE[0], D_EBFE[1], MapZ);
	if(bp_04 >= 0 && !QuanType(GetType(bp_04)))
		LooseHP(bp_04, OSI_rand(1, 60));
}

/*use powder keg*/
static C_27A1_3B59(int objNum) {
	if(GetFrame(objNum) == 0) {
		if(D_2CA4) {
			CON_printf(/*2305*/"\nNot now\n");
			return;
		}
		SetFrame(objNum, 1);
		D_2CA4 = 3;
		D_2CA6 = objNum;
		CON_printf(/*230F*/"\nPowder lit!\n");
		if(GetCoordUse(objNum) == LOCXYZ) {
			C_1100_0306();
			OtherAnimations();
		} else {
			C_155D_0CF5();
		}
	} else {
		CON_printf(/*1F13*/"\n%s\n", D_0DDC[2]);
	}
}

/*C_27A1_3BE1*/AfterExplosion(int x, int y) {
	int objNum;
	int i, j;

	for(i = x - 2; i <= x + 2; i ++) {
		for(j = y - 2; j < y + 2; j ++) {
			/*detonate other kegs*/
			objNum = __SearchTypeAt(i, j, MapZ, OBJ_0DF);
			if(objNum >= 0) {
				SetFrame(objNum, 1);
				C_1100_0306();
				OtherAnimations();
				Detonate(objNum);
			}
			/*blow the doors*/
			objNum = __SearchTypeAt(i, j, MapZ, OBJ_12C);
			if(objNum == -1 || (GetFrame(objNum) & 0xc) == 0xc)
				objNum = __SearchTypeAt(i, j, MapZ, OBJ_129);
			if(objNum == -1 || (GetFrame(objNum) & 0xc) == 0xc)
				objNum = __SearchTypeAt(i, j, MapZ, OBJ_12A);
			if(objNum == -1 || (GetFrame(objNum) & 0xc) == 0xc)
				objNum = __SearchTypeAt(i, j, MapZ, OBJ_12B);
			if(objNum >= 0 && (GetFrame(objNum) & 0xc) != 0xc) {
				DeleteObj(objNum);
				C_1100_0306();
				OtherAnimations();
				CON_printf(/*231D*/"\nThe door is blown up!\n");
			}
		}
	}
}

/*C_27A1_3D2B*/Detonate(int objNum) {
	int si;
	int x, y;
	int far *bp_04;

	if(GetCoordUse(objNum) == LOCXYZ) {
		x = GetX(objNum);
		y = GetY(objNum);
	} else {
		si = objNum;
		do
			si = GetAssoc(si);
		while(GetCoordUse(si) != LOCXYZ);
		x = GetX(si);
		y = GetY(si);
	}
	DeleteObj(objNum);
	for(bp_04 = Explosion(x, y, TIL_189); *bp_04 >= 0; bp_04 ++) {
		if(!QuanType(GetType(*bp_04)))
			LooseHP(*bp_04, OSI_rand(1, 60));
	}
	AfterExplosion(x, y);
	C_1100_0306();
	OtherAnimations();
}

static /*C_27A1_3E21*/__SearchTypeAt(int x, int y, int z, int objTyp) {
	int si;

	for(si = FindLoc(x, y, z); si >= 0 && GetType(si) != objTyp; si = NextLoc());
	return si;
}

/*drawbridge related*/
static C_27A1_3E59(int x, int y, int z) {
	int objNum;
	
	for(objNum = FindLoc(x, y, z); objNum >= 0 && GetType(objNum) == OBJ_10D; objNum = NextLoc());
	if(objNum == -1)
		return 0;
	return objNum;
}

C_27A1_3E9A(int x, int y, int z) {
	int si;
	
	for(si = FindLoc(x, y, z); si >= 0 && si >= 0x100; si = NextLoc());
	if(si == -1)
		return 0;
	return si;
}

unsigned char D_1D0A[] = {TIL_010,TIL_01C,TIL_020,TIL_02C,TIL_0D6};

C_27A1_3ED1(int bp06) {
	int si;

	for(si = 0; si < 5; si ++) {
		if(D_1D0A[si] == bp06)
			return 1;
	}
	return 0;
}

/*add permanent part: bridge related*/
static C_27A1_3EF6(int x, int y, int typeObject, int shapeNum) {
	int numObject;
	struct coord xyz;

	SetCoordXYZ(xyz, x, y, MapZ);
	numObject = AddObj(0, xyz, typeObject, shapeNum, 0);
	ClrLocal(numObject);

	return numObject;
}

/*try to activate drawbrige*/
static C_27A1_3F47(int objNum) {
	register int tmp_x, tmp_y;
	int obj_x, obj_y, bp_0e, objQual, tmp_num, objFrm, numTile, bp_04, bp_02;

	objQual = GetQual(objNum);
	objFrm = GetFrame(objNum);

	tmp_num = objNum;
	obj_x = GetX(objNum);
	obj_y = GetY(objNum);
	tmp_x = obj_x;
	tmp_y = obj_y;
	/***********/
	/*try close*/
	/***********/
	if(objFrm == 3) {
		/*is there something on the bridge?*/
		while(tmp_num >= 0) {
			while(tmp_num >= 0) {
				if(C_27A1_3E59(tmp_x, tmp_y, MapZ))
					return 3;/*can't close*/
				tmp_num = __SearchTypeAt(++tmp_x, tmp_y, MapZ, OBJ_10D);
			}
			tmp_x = obj_x;
			tmp_num = __SearchTypeAt(tmp_x, ++tmp_y, MapZ, OBJ_10D);
		}
		/*close it*/
		tmp_num = objNum;
		tmp_x = obj_x;
		tmp_y = obj_y;
		objNum = C_27A1_3EF6(tmp_x - 1, tmp_y - 1, OBJ_10D, 6);
		SetQual(objNum, objQual);
		while(tmp_num >= 0) {
			DeleteObj(tmp_num);
			bp_02 = C_27A1_3EF6(tmp_x ++, tmp_y - 1, OBJ_10D, 7);
			tmp_num = __SearchTypeAt(tmp_x, tmp_y, MapZ, OBJ_10D);
		}
		DeleteObj(bp_02);
		C_27A1_3EF6(tmp_x - 1, tmp_y - 1, OBJ_10D, 8);
		tmp_x = obj_x;
		tmp_y = obj_y + 1;
		tmp_num = __SearchTypeAt(tmp_x, tmp_y, MapZ, OBJ_10D);
		while(tmp_num >= 0) {
			while(tmp_num >= 0) {
				DeleteObj(tmp_num);
				tmp_num = __SearchTypeAt(++tmp_x, tmp_y, MapZ, OBJ_10D);
			}
			tmp_x = obj_x;
			tmp_num = __SearchTypeAt(tmp_x, ++tmp_y, MapZ, OBJ_10D);
		}
		C_1100_0306();
		return 1;/*can close*/
	}
	/**********/
	/*try open*/
	/**********/
	if(objFrm == 6) {
		bp_04 = 1;
		while(tmp_num >= 0) {
			bp_04 ++;
			tmp_num = __SearchTypeAt(++tmp_x, tmp_y, MapZ, OBJ_10D);
		}
		tmp_x = obj_x + 1;
		tmp_y = obj_y + 1;
		while(!C_27A1_3ED1(numTile = GetTileAtXYZ(tmp_x, tmp_y, MapZ))) {
			for(bp_0e = 0; bp_0e < bp_04 - 4; bp_0e ++) {
				if(C_27A1_3E59(tmp_x, tmp_y, MapZ))
					return 4;/*can't open*/
				tmp_x ++;
			}
			tmp_x = obj_x + 1;
			tmp_y ++;
		}
		/*open it*/
		tmp_num = objNum;
		tmp_x = obj_x;
		tmp_y = obj_y;
		while(tmp_num >= 0) {
			DeleteObj(tmp_num);
			tmp_num = __SearchTypeAt(++tmp_x, tmp_y, MapZ, OBJ_10D);
		}
		tmp_x = obj_x + 1;
		tmp_y = obj_y + 1;
		while(!C_27A1_3ED1(numTile = GetTileAtXYZ(tmp_x, tmp_y, MapZ))) {
			objNum = C_27A1_3EF6(tmp_x++, tmp_y, OBJ_10D, 3);
			SetQual(objNum, objQual);
			for(bp_0e = 0; bp_0e < bp_04 - 4; bp_0e ++)
				C_27A1_3EF6(tmp_x ++, tmp_y, OBJ_10D, 4);
			C_27A1_3EF6(tmp_x, tmp_y, OBJ_10D, 5);
			tmp_x = obj_x + 1;
			tmp_y ++;
		}
		C_27A1_3EF6(tmp_x ++, tmp_y, OBJ_10D, 0);
		tmp_num = C_27A1_3E59(tmp_x - 1, tmp_y, MapZ);
		if(tmp_num)
			MoveObj(tmp_num, GetX(tmp_num), GetY(tmp_num), MapZ);
		for(bp_0e = 0; bp_0e < bp_04 - 4; bp_0e ++) {
			C_27A1_3EF6(tmp_x ++, tmp_y, OBJ_10D, 1);
			tmp_num = C_27A1_3E59(tmp_x - 1, tmp_y, MapZ);
			if(tmp_num)
				MoveObj(tmp_num, GetX(tmp_num), GetY(tmp_num), MapZ);
		}
		C_27A1_3EF6(tmp_x, tmp_y, OBJ_10D, 2);
		tmp_num = C_27A1_3E59(tmp_x, tmp_y, MapZ);
		if(tmp_num)
			MoveObj(tmp_num, GetX(tmp_num), GetY(tmp_num), MapZ);
		C_1100_0306();
		return 2;/*can open*/
	}

	return 0;/*nothine happens*/
}

/*use crank*/
C_27A1_433D(int objNum) {
	int si, di;
	int bp_06, bp_04, bp_02;

	bp_06 = GetQual(objNum);
	for(
		si = SearchArea(0, 0, 0x3ff, 0x3ff);
		si >= 0 && (GetType(si) != OBJ_10D || GetQual(si) != bp_06);
		si = NextArea()
	);
	if(si == -1) {
		CON_printf(/*2335*/"\nNo correspondent drawbridge\n");
		return;
	}
	bp_04 = GetFrame(si);
	if(bp_04 == 3)
		di = 2;
	else
		di = 1;
	SetTileAnimation(TIL_3F1, di);
	SetTileAnimation(TIL_3FC, di);
	for(bp_02 = 0; bp_02 < 20; bp_02 ++)
		OtherAnimations();
	di = C_27A1_3F47(si);
	switch(di) {
		case 0: CON_printf(/*2353*/"\nStrange, nothing happened.\n"); break;
		case 1: CON_printf(/*2370*/"\nClose the drawbridge.\n"); break;
		case 2: CON_printf(/*2388*/"\nOpen the drawbridge.\n"); break;
		case 3: CON_printf(/*239F*/"\nCan't close the drawbridge.\n"); break;
		case 4: CON_printf(/*23BD*/"\nCan't open the drawbridge.\n"); break;
	}
	SetTileAnimation(TIL_3F1, 0);
	SetTileAnimation(TIL_3FC, 0);
}

/*use lever*/
C_27A1_4479(int objNum) {
	int si, di;
	int objQual, bp_0a, bp_08, bp_06;
	struct coord xyz;

	bp_06 = 0;
	CON_printf(/*23DA*/"\nSwitch the lever, ");
	SetFrame(objNum, GetFrame(objNum) ^ 1);
	objQual = GetQual(objNum);
	if(objQual) {
		si = SearchArea(0, 0, 0x3ff, 0x3ff);
		while(si >= 0) {
			while(si >= 0 && (GetType(si) != OBJ_12D || GetQual(si) != objQual))
				si = NextArea();
			if(si < 0)
				continue;
			bp_0a = GetX(si);
			bp_08 = GetY(si);
			di = __SearchTypeAt(bp_0a, bp_08, MapZ, OBJ_136);
			if(di == -1) {
				if(C_27A1_3E9A(bp_0a, bp_08, MapZ)) {
					bp_06 = 2;
					break;
				}
				if(GetFrame(si) & 2) {
					SetCoordXYZ(xyz, bp_0a, bp_08, MapZ);
					di = AddObj(0, xyz, OBJ_136, 3, 0);
				} else {
					SetCoordXYZ(xyz, bp_0a, bp_08, MapZ);
					di = AddObj(0, xyz, OBJ_136, 1, 0);
				}
				ClrLocal(di);
			} else {
				DeleteObj(di);
			}
			bp_06 = 1;
			si = NextArea();
		}
	}
	if(bp_06 == 0)
		CON_printf(/*23EE*/"strange, nothing happened.\n");
	else if(bp_06 == 1)
		CON_printf(/*240A*/"you hear a noise.\n");
	else if(bp_06 == 2)
		CON_printf(/*241D*/"can't close the portcullis.\n");
}

/*use switch*/
C_27A1_4672(int objNum) {
	int si, di;
	int objQual, bp_0a, bp_08, bp_06;
	struct coord xyz;

	bp_06 = 0;
	CON_printf(/*243A*/"\nOperate the switch, ");
	SetFrame(objNum, GetFrame(objNum) ^ 1);
	objQual = GetQual(objNum);
	if(objQual) {
		si = SearchArea(0, 0, 0x3ff, 0x3ff);
		while(si >= 0) {
			while(si >= 0 && (GetType(si) != OBJ_12D || GetQual(si) != objQual))
				si = NextArea();
			if(si < 0)
				continue;
			bp_0a = GetX(si);
			bp_08 = GetY(si);
			di = __SearchTypeAt(bp_0a, bp_08, MapZ, OBJ_0AF);
			if(di == -1) {
				SetCoordXYZ(xyz, bp_0a, bp_08, MapZ);
				di = AddObj(0, xyz, OBJ_0AF, 0, 0);
				ClrLocal(di);
			} else {
				DeleteObj(di);
			}
			bp_06 = 1;
			si = NextArea();
		}
	}
	if(bp_06 == 0)
		CON_printf(/*23EE*/"strange, nothing happened.\n");
	else
		CON_printf(/*240A*/"you hear a noise.\n");
}

C_27A1_47E0() {
	CON_printf(/*2450*/"\nYou study the scroll!\n");
	D_2CA8 = 1;
}

/*use ballon plans*/
C_27A1_47F3() {
	int si, di;
	int bp_10[4];
	int bp_08, bp_06;
	struct coord xyz;

	si = 0;
	CON_putch('\n');
	for(bp_08 = 0; bp_08 < 4; bp_08 ++)
		bp_10[bp_08] = 0;

	bp_10[0] = C_1703_04C6(OBJ_1A5, 0);
	if(bp_10[0] == -1) {
		CON_printf(/*2468*/"Missing a mammoth silk bag.\n");
		si = 1;
	}
	bp_10[1] = C_1703_04C6(OBJ_1A6, 0);
	if(bp_10[1] == -1) {
		CON_printf(/*2485*/"Missing a balloon basket.\n");
		si = 1;
	}
	bp_10[2] = C_1703_04C6(OBJ_093, 0);
	if(bp_10[2] == -1) {
		CON_printf(/*24A0*/"Missing a cauldron.\n");
		si = 1;
	}
	bp_10[3] = C_1703_04C6(OBJ_11C, 0);
	if(bp_10[3] == -1) {
		CON_printf(/*24A0*/"Missing a rope.\n");
		si = 1;
	}
	if(si)
		return;
	DeleteObj(C_1944_0A43(bp_10[0], OBJ_1A5, 0));
	DeleteObj(C_1944_0A43(bp_10[1], OBJ_1A6, 0));
	DeleteObj(C_1944_0A43(bp_10[2], OBJ_093, 0));
	DeleteObj(C_1944_0A43(bp_10[3], OBJ_11C, 0));
	di = Party[Active];
	SetCoordXYZ(xyz, GetX(di), GetY(di), MapZ);
	bp_06 = AddObj(0, xyz, OBJ_1A4, 0, 0);
	SetOkToGet(bp_06);
	ClrLocal(bp_06);
	MoveObj(di, GetX(di), GetY(di), MapZ);
	CON_printf(/*24C6*/"Done!\n");
}

/*use deflated balloon*/
C_27A1_49C3(int bp06) {
	int si, di;
	struct coord xyz, bp_0c/*unused?*/;
	int bp_0a, bp_08, bp_06, bp_04, bp_02;

	bp_02 = 0;
	DeleteObj(bp06);
	bp_08 = Party[Active];
	bp_06 = GetX(bp_08);
	bp_04 = GetY(bp_08);
	SetCoordXYZ(xyz, GetX(bp_08), GetY(bp_08), MapZ);
	bp_0a = AddObj(0, xyz, OBJ_1A7, 3, 0);
	ClrLocal(bp_0a);
	for(si = 0; si < 3; si ++) {
		for(di = 0; di < 3; di ++) {
			if(!si && !di)
				continue;
			if(C_1E0F_000F(bp_0a, bp_06 + si - 1, bp_04 + di - 1)) {
				MoveObj(bp_0a, bp_06 + si - 1, bp_04 + di - 1, MapZ);
				CON_printf(/*24CD*/"\nDone!\n");
				bp_02 = 1;
				break;
			}
		}
		if(bp_02)
			break;
	}
	if(!bp_02)
		CON_printf(/*24D5*/"\nNo room!\n");
	if(StatusDisplay == CMD_92)
		C_155D_1022(D_E709);
}

/*destroy gargoyles eggs*/
static C_27A1_4B0B(int x, int y) {
	int si, di;

	for(si = SearchArea(x - 20, y - 20, x + 20, y + 20); si >= 0; si = NextArea()) {
		if(GetType(si) == OBJ_14F) {
			di = FindInv(si);
			if(GetType(di) == OBJ_16A || GetType(di) == OBJ_16B) {
				while(di >= 0) {
					DeleteObj(di);
					di = FindInv(si);
				}
				DeleteObj(si);
			}
		}
	}
}

char *D_1D0F[] = {
	/*24E0*/"Ahm",
	/*24E4*/"Mu",
	/*24E7*/"Ra",
	/*24EA*/"Beh",
	/*24EE*/"Cah",
	/*24F2*/"Summ",
	/*24F7*/"Om",
	/*24FA*/"Lum"
};
/*shrines coords?*/
unsigned __1D1F[] = {0x3A4,0x200,0x090,0x124,0x334,0x144,0x000,0x39C};
unsigned __1D2F[] = {0x108,0x170,0x394,0x02C,0x064,0x33C,0x000,0x360};

/*use rune?*/
static C_27A1_4B98(int objNum) {
	int i, j;
	char mantra[5];
	int virtue, x, y;
	int moonstone, forcefield, bp_02;

	bp_02 = 0;
	virtue = GetType(objNum) - OBJ_0F2;
	CON_printf(/*24FE*/"Mantra: ");
	CON_gets(mantra, 4);
	CON_putch('\n');
	for(i = 0; i < 8; i ++)
		if(stricmp(mantra, D_1D0F[i]) == 0)
			break;
	if(i == 8 || i != virtue) {
		CON_printf(/*2507*/"\nWrong mantra!\n");
		return;
	}
	virtue = i;/*???*/
	x = GetX(Party[Active]);
	y = GetY(Party[Active]);
	for(i = 0; i < 3; i ++) {
		for(j = 0; j < 3; j ++) {
			if(i != 1 || j != 1) {
				moonstone = __SearchTypeAt(x + i - 1, y + j - 1, MapZ, OBJ_049);
				if(moonstone >= 0 && GetFrame(moonstone) == virtue) {
					/*delete forcefield*/
					forcefield = __SearchTypeAt(x + i - 1, y + j - 1, MapZ, OBJ_033);
					if(forcefield >= 0)
						DeleteObj(forcefield);
					/*destroy gargoyles eggs*/
					C_27A1_4B0B(GetX(moonstone), GetY(moonstone));

					bp_02 = 1;
					break;
				}
			}
		}
	}
	if(bp_02) {
		C_1944_0085(1);
		CON_printf(/*1F13*/"\n%s\n", D_0DDC[7]);
	} else {
		CON_printf(/*1F13*/"\n%s\n", D_0DDC[2]);
	}
}

/*use fishing pole*/
C_27A1_4D46() {
	int si, di;
	int bp_08, bp_06, bp_04, bp_02;

	bp_02 = 0;
	di = C_27A1_0008(Active);
	bp_06 = C_27A1_0028(di);
	bp_04 = C_27A1_0051(di);
	for(si = 0; si < 3; si ++) {
		for(bp_08 = 0; bp_08 < 3; bp_08 ++) {
			if(si == 1 && bp_08 == 1)
				continue;
			if(IsTerrainWet(GetTileAtXYZ(bp_06 + si - 1, bp_04 + si - 1, MapZ))) {
				if(!CaughtFish && OSI_rand(0, 4) == 0) {
					C_155D_0748(di);
					if(STREN[di] * 20 < WeightInven + WeightEquip + 5) {
						CON_printf(/*2517*/"\nGot it, but can't carry it.\n");
					} else {
						CON_printf(/*2535*/"\nGot it!\n");
						AddInvObj(OBJ_109, INVEN, 1, di);
						CaughtFish = 1;
					}
				} else {
					CON_printf(/*253F*/"\nDidn't get a fish.\n");
				}
				bp_02 = 1;
				break;
			}
		}
		if(bp_02)
			break;
	}
	if(bp_02 == 0)
		CON_printf(/*2554*/"\nYou need to stand next to water.\n");
	if(StatusDisplay == CMD_92)
		C_155D_0CF5();
}

/*"use" alcool*/
C_27A1_4E6F(int objNum) {
	DeleteObj(objNum);
	DrunkCounter ++;
	CON_printf(/*2577*/"\nYou drink it.\n");
	if(StatusDisplay == CMD_92)
		C_155D_0CF5();
}

char *D_1D3F[] = {
	/*2587*/"Food",
	/*258C*/"Mutton",
	/*2593*/"Wine",
	/*2598*/"Fruit",
	/*259E*/"Meat"
};

/*use fountain*/
C_27A1_4E9B() {
	int si, di;
	int bp_0e, bp_0c;
	char bp_0a[10];

	bp_0c = 0;
	di = C_27A1_0008(Active);
	CON_printf(/*25A3*/"Make a wish?");
	bp_0e = CON_getch();
	if(bp_0e == 'Y') {
		CON_printf(/*25B0*/"\nWish for: ");
		CON_gets(bp_0a, 9);
		for(si = 0; si < 10/*bug?*/; si ++) {
			if(stricmp(bp_0a, D_1D3F[si]) == 0) {
				if(OSI_rand(0, 4) == 0) {
					CON_printf(/*25BC*/"\n\nYou got food");
					C_155D_0748(di);
					if(STREN[di] * 20 < WeightInven + WeightEquip + 2) {
						CON_printf(/*25CB*/", but you can't carry it.\n");
					} else {
						GiveObj(di, OBJ_081, 1);
						CON_printf(/*1DC6*/".\n");
					}
				} else {
					CON_printf(/*1FA6*/"\n\n%s\n", D_0DDC[1]);
				}
				bp_0c = 1;
				break;
			}
		}
		if(!bp_0c)
			CON_printf(/*1FA6*/"\n\n%s\n", D_0DDC[2]);
	} else {
		CON_printf(/*1D6F*/"\n");
	}
	if(StatusDisplay == CMD_92)
		C_155D_0CF5();
}

C_27A1_4FC6() {
	int dig_x, dig_y;
	int tile, bp_06;
	struct coord xyz;

	CON_printf(/*25E6*/"Direction:");
	MouseMode = 0;
	if(SelectMode != 0) {
		C_0C9C_1AE5(0);
		SelectMode = 1;
	}
	if(CON_getch() != CMD_80) {
		CON_printf(/*1F7A*/"nowhere.\n");
		return;
	}
	CON_printf(/*1F84*/"%s.\n", DirNames[AdvanceDir]);
	dig_x = DirIncrX[AdvanceDir] + MapX;
	dig_y = DirIncrY[AdvanceDir] + MapY;
	tile = GetTileAtXYZ(dig_x, dig_y, MapZ);
	if(MapZ == 0 && dig_x == 0x2c3 && dig_y == 0x343 && __SearchTypeAt(dig_x, dig_y, MapZ, OBJ_134) == -1) {
		/*Pirate Cave?*/
		SetCoordXYZ(xyz, dig_x, dig_y, MapZ);
		bp_06 = AddObj(0, xyz, OBJ_134, 0, 0);
		SetQual(bp_06, 0x13);
		CON_printf(/*25F1*/"\nYou dig a hole.\n");
	} else if(MapZ >= 1 && MapZ <= 4 && tile >= TIL_06C && tile <= TIL_06F) {
		if(OSI_rand(0, 31) == 0) {
			SetCoordXYZ(xyz, dig_x, dig_y, MapZ);
			AddObj(0, xyz, OBJ_0EA, 0, 0);
			CON_printf(/*2603*/"\nYou find a water fountain.\n");
		} else if(OSI_rand(0, 9) == 0) {
			SetCoordXYZ(xyz, dig_x, dig_y, MapZ);
			AddObj(0, xyz, OBJ_059, 0, 1);
			CON_printf(/*2620*/"\nYou find a gold nugget.\n");
		} else {
			CON_printf(/*1F13*/"\n%s\n", D_0DDC[1]);
		}
	} else {
		CON_printf(/*1F13*/"\n%s\n", D_0DDC[2]);
	}
}

char D_1D49[] = { 0, 1, 1, 1, 0,-1,-1,-1};
char D_1D51[] = {-1,-1, 0, 1, 1, 1, 0,-1};

static C_27A1_51C4(int bp0a, int bp08, int bp06) {
	int si, di;

	if(GetType(bp06) == OBJ_19C) {
		di = GetFrame(bp06) & 2;
		if(C_1E0F_000F(Party[0], D_1D49[(di + 2) & 7] + bp0a, D_1D51[(di + 2) & 7] + bp08))
			return 1;
		if(C_1E0F_000F(Party[0], D_1D49[(di + 6) & 7] + bp0a, D_1D51[(di + 6) & 7] + bp08))
			return 1;
	} else {
		for(si = 0; si < 8; si ++) {
			if(C_1E0F_000F(Party[0], D_1D49[si] + bp0a, D_1D51[si] + bp08))
				return 1;
		}
	}
	return 0;
}

/*use ship/.../balloon?*/
static C_27A1_5289(int objNum) {
	int di, x, y, bp_04, bp_02;

	objNum = COMBAT_getHead(objNum);
	di = GetType(objNum);
	bp_04 = Party[Active];
	bp_02 = GetType(bp_04);
	x = GetX(objNum);
	y = GetY(objNum);
	if(D_2CC3 != -1) {
		CON_printf(/*263A*/"Not in solo mode!\n");
		return;
	}
	if(IN_VEHICLE && bp_04 == objNum) {
		/*try to unboard*/
		if(C_27A1_51C4(MapX, MapY, objNum)) {
			/*on land*/
			objNum = Unboard(1);
			if(di == OBJ_1A7) {
				ObjShapeType[objNum] = TypeFrame(OBJ_1A4, 0);
				C_155D_1022(D_E709);
			}
		} else if(di == OBJ_19C && FindInvType(objNum, OBJ_19E, 0) != -1) {
			/*at sea on skiff*/
			objNum = Unboard(0);
		} else {
			CON_printf(/*264D*/"\nOnly next to land.\n");
		}
	} else if(
		(di == OBJ_19C || di == OBJ_19E) &&
		GetQual(objNum) != 0 && C_1703_04C6(OBJ_095, GetQual(objNum)) == -1
	) {
		CON_printf(/*2662*/"\nA deed is required.\n");
	} else if(!CLOSE_ENOUGH(1, MapX, MapY, x, y)) {
		CON_printf(/*1F24*/"\n%s!\n", D_0DDC[3]);
	} else {
		if(di == OBJ_19C && bp_02 == OBJ_19E)
			InsertObj(bp_04, objNum, INVEN);
		Board(objNum);
	}
}

C_27A1_5431(int objNum, int x, int y, int bp08, int bp06) {
	int bp_08, bp_06;
	struct coord xyz;

	MoveObj(objNum, x, y, MapZ);
	ObjShapeType[objNum] = OBJ_1AF | (bp08 << 10);
	MapX = x;
	MapY = y;
	bp_08 = bp08 & 6;
	x -= DirIncrX[bp_08];
	y -= DirIncrY[bp_08];
	if(bp06) {
		bp_06 = FindLoc(x, y, MapZ);
		bp08 = GetFrame(bp_06);
		ObjShapeType[bp_06] = OBJ_1AF | (bp08 << 10);
	} else {
		SetCoordXYZ(xyz, x, y, MapZ);
		AddObj(0, xyz, OBJ_1AF, bp08 + 8, 0);
	}
	C_101C_054C();
	OtherAnimations();
}

/*use un-mounted horse*/
static C_27A1_5503(int si/*bp06*/) {
	int di;
	int bp_06, bp_04, bp_02;

	si = COMBAT_getHead(si);
	if(si < 0xe0) {
		CON_printf(/*2678*/"\nNot boardable!\n");
		return;
	}
	di = Party[Active];
	if(di == /*NPC_*/0x09) {
		CON_printf(/*2689*/"\nSherry says: \"Eeek!!! I'm afraid of horses!\"\n");
		return;
	}
	if(di == /*NPC_*/0xA4) {
		CON_printf(/*26B8*/"\nBehLem says: \"Horses are for food!\"\n");
		return;
	}
	if(GetType(di) == OBJ_1AF) {
		CON_printf(/*26DE*/"\nYou're already on a horse!\n");
		return;
	}
	bp_06 = GetX(si);
	bp_04 = GetY(si);
	bp_02 = GetFrame(si);
	SetType(si, OBJ_058);
	DeleteObj(si);
	C_27A1_5431(di, bp_06, bp_04, bp_02, 1);
}

/*use mounted-horse*/
C_27A1_55F0(int objNum) {
	int di;
	int bp_16, objTyp, cur_x, cur_y, bp_0e, bp_0c;
	int dir, bp_08, i, j, bp_02;

	bp_02 = 0;
	objNum = COMBAT_getHead(objNum);
	cur_x = GetX(objNum);
	cur_y = GetY(objNum);
	dir = GetFrame(objNum) & 6;
	objTyp = OrigShapeType[objNum] & 0x3ff;
	bp_16 = OBJ_MakeDirFrame(objTyp, GetDirection(objNum));
	ObjShapeType[objNum] = objTyp | (bp_16 << 10);

	bp_0e = cur_x - DirIncrX[dir];
	bp_0c = cur_y - DirIncrY[dir];
	for(di = FindLoc(bp_0e, bp_0c, MapZ); di >= 0 && GetType(di) != OBJ_1AF; di = NextLoc());
	if(di >= 0)
		DeleteObj(di);
	else
		CON_printf(/*26FB*/"\nRear horse???\n");

	bp_08 = EGG_generate(OBJ_1AE, 0, 0, MapZ);
	for(i = 0; i < 3; i ++) {
		for(j = 0; j < 3; j ++) {
			if(i == 1 && j == 1)
				continue;
			if(C_1E0F_000F(bp_08, cur_x + i - 1, cur_y + j - 1)) {
				MoveObj(bp_08, cur_x + i - 1, cur_y + j - 1, MapZ);
				bp_02 = 1;
				break;
			}
		}
		if(bp_02)/*unnecessary?*/
			break;
	}
	if(!bp_02) {
		CON_printf(/*24D5*/"\nNo room!\n");
		DeleteObj(bp_08);
	}
	OtherAnimations();
}

C_27A1_5789() {
	int si, di;
	struct coord xyz;

	if(!((TalkFlags[5] >> 5) & 1)) {
		CON_printf(/*270B*/"\nYou can't figure out how to use it.\n");
		return;
	}
	CON_printf(/*2731*/"Where: ");
	MouseMode = 1;
	if(SelectMode != 0) {
		C_0C9C_1AE5(0);
		SelectMode = 1;
		SelectRange = 7;
	}
	CON_getch();
	if(
		MapX - 2 <= Selection.x && MapX + 2 >= Selection.x &&
		MapY - 2 <= Selection.y && MapY + 2 >= Selection.y
	) {
		if(Selection.y == MapY && MapX - 1 <= Selection.x && MapX + 1 >= Selection.x)
			di = 0;
		else
			di = (Selection.y - MapY + 2) * 5 + (Selection.x - MapX + 3);
		SetCoordXYZ(xyz, 0, 0, MapZ);
		si = AddObj(0, xyz, OBJ_054, 1, QualQuan(di, 0));
		if(
			C_1E0F_000F(si, Selection.x, Selection.y) &&
			__SearchTypeAt(Selection.x, Selection.y, MapZ, OBJ_12D) == -1 &&
			__SearchTypeAt(Selection.x, Selection.y, MapZ, OBJ_12E) == -1
		) {
			CON_printf(/*2739*/"a red moon gate appears.\n");
			MoveObj(si, Selection.x, Selection.y, MapZ);
			ScreenFade = 1;
			C_1100_0306();
			OtherAnimations();
		} else {
			CON_printf(/*1D6D*/"%s\n", D_0DDC[1]);
			DeleteObj(si);
		}
	} else {
		CON_printf(/*1D6D*/"%s\n", D_0DDC[1]);
	}
}

/*use crystal ball*/
C_27A1_5935() {
	int si, di;
	char bp_0e[4];
	int bp_0a, bp_08, bp_06, snd_flag, bp_02;

	bp_02 = C_27A1_0008(Active);
	if(C_27A1_2D61(INTEL[bp_02])) {
		LooseHP(bp_02, OSI_rand(1, 10));
		CON_putch('\n');
		COMBAT_displaySeverity(bp_02);
		return;
	}
	if(SelectMode != 0) {
		C_0C9C_1AE5(0);
		SelectMode = 0;
	}
	CON_printf(/*2753*/"\nEnter degrees followed by N, S, E or W.\n");
	CON_printf(/*277D*/"\nAt latitude=");
	bp_06 = D_04DF->_cury;
	CON_gets(bp_0e, 4);
	si = C_27A1_0205(bp_0e, 1);
	if(si > 80 || si < -44) {
		CON_gotoxy(12, bp_06);
		CON_printf(/*1F02*/"0   \n");
		return;
	}
	CON_printf(/*278B*/"\n  longitude=");
	bp_06 = D_04DF->_cury;
	CON_gets(bp_0e, 4);
	di = C_27A1_0205(bp_0e, 2);
	if(di > 88 || di < -37) {
		CON_gotoxy(12, bp_06);
		CON_printf(/*1F02*/"0   \n");
		return;
	}
	CON_putch('\n');
	snd_flag = SoundFlag;
	SoundFlag = 0;
	bp_0a = 0x130 + (di << 3);
	bp_08 = 0x168 + (si << 3);
	if(MapZ != 0) {
		bp_0a >>= 2;
		bp_08 >>= 2;
	}
	C_1944_39A1(bp_0a, bp_08, 0);
	SoundFlag = snd_flag;
}

/*use telescope*/
C_27A1_5A97(int bp06) {
	int si, di;
	int bp_04, bp_02;

	si = 40;
	di = 0;
	bp_04 = MapX;
	bp_02 = MapY;
	MapX = GetX(bp06);
	MapY = GetY(bp06);
	ShowObjects();
	C_101C_054C();
	OtherAnimations();
	CON_printf(/*2799*/"Direction-");
	MouseMode = 0;
	if(CON_getch() != CMD_80) {
		CON_printf(/*1F7A*/"nowhere.\n");
		return;
	}
	CON_printf(/*1F84*/"%s.\n", DirNames[AdvanceDir]);
	D_05E8 = 1;
	ScreenFade = 1;
	ShowObjects();
	C_101C_054C();
	OtherAnimations();
	while(si >= 0 && di < TIL_0FC) {
		MapX += DirIncrX[AdvanceDir];
		MapY += DirIncrY[AdvanceDir];
		ShowObjects();
		C_101C_054C();
		OtherAnimations();
		si --;
		di = GetTileAtXYZ(MapX, MapY, MapZ);
	}
	D_05E8 = 0;
	MapX = bp_04;
	MapY = bp_02;
	ShowObjects();
	C_101C_054C();
	ScreenFade = 1;
	OtherAnimations();
	CON_printf(/*1F13*/"\n%s\n", D_0DDC[7]);
}

/*use silver horn?*/
C_27A1_5BCF() {
	int x, y;
	int bp_12, bp_10, bp_0e, bp_0c, bp_0a, egg_numObject, bp_06;
	struct coord xyz;

	bp_06 = 0;
	for(bp_12 = 0xe0; bp_12 < 0x100; bp_12 ++) {
		if(ObjShapeType[bp_12] != 0 && GetType(bp_12) == OBJ_19D) {
			CON_printf(/*27A4*/"\nNot now!\n");
			return;
		}
	}
	SetCoordXYZ(xyz, 0, 0, MapZ);
	bp_0a = AddObj(0, xyz, OBJ_19D, 0, 1);
	for(bp_0c = 0; bp_0c < 3; bp_0c ++) {
		for(bp_0e = 0; bp_0e < 8; bp_0e ++) {
			bp_12 = OSI_rand(0, 3);
			bp_10 = OSI_rand(0, 10);
			switch(bp_12) {
				case 0: x = MapX - 6; y = MapY + bp_10 - 6; break;
				case 1: x = MapX + bp_10 - 6; y = MapY - 7; break;
				case 2: x = MapX + 7; y = MapY + bp_10 - 6; break;
				case 3: x = MapX + bp_10 - 6; y = MapY + 6; break;
			}
			if(
				C_1E0F_000F(bp_0a, x,     y) &&
				C_1E0F_000F(bp_0a, x - 1, y) &&
				C_1E0F_000F(bp_0a, x,     y + 1) &&
				C_1E0F_000F(bp_0a, x - 1, y + 1) &&
				C_1E0F_000F(bp_0a, x + 1, y) &&
				C_1E0F_000F(bp_0a, x,     y - 1)
			) break;
		}
		if(bp_0e == 8)
			continue;
		SetCoordXYZ(xyz, x, y, MapZ);
		egg_numObject = AddObj(0, xyz, OBJ_14F, 0, QualQuan(4, 100));
		AddInvObj(OBJ_19D, CONTAINED, QualQuan(8, OSI_rand(4, 8)), egg_numObject);
		EGG_hatches(egg_numObject);
		bp_06 ++;
	}
	DeleteObj(bp_0a);
	if(!bp_06) {
		CON_printf(/*27AF*/"\nNone generated!\n");
		return;
	}
	CON_printf(/*27C1*/"\nSilver snakes are generated!\n");
}

C_27A1_5DF2(int si/*bp06*/) {
	int di;
	int bp_08, bp_06, bp_04, bp_02;

	bp_06 = GetType(si);
	bp_08 = GetFrame(si);
	di = GetX(si);
	bp_04 = GetY(si);
	if(bp_08 == 0) {
		if(bp_06 == OBJ_116)
			MoveObj(si, di, bp_04 - 1, MapZ);
		else
			MoveObj(si, di - 1, bp_04, MapZ);
		SetFrame(si, bp_08 | 2);
		CON_printf(/*27E0*/"\nOpen the passthrough.\n");
	} else {
		if(bp_06 == OBJ_116) {
			bp_02 = FindLoc(di, bp_04 + 1, MapZ);
			if(bp_02 != -1 && bp_02 < 0x100) {
				CON_printf(/*27A4*/"\nNot now!\n");
				return;
			}
			MoveObj(si, di, bp_04 + 1, MapZ);
		} else {
			bp_02 = FindLoc(di + 1, bp_04, MapZ);
			if(bp_02 != -1 && bp_02 < 0x100) {
				CON_printf(/*27A4*/"\nNot now!\n");
				return;
			}
			MoveObj(si, di + 1, bp_04, MapZ);
		}
		SetFrame(si, bp_08 & 1);
		CON_printf(/*27F8*/"\nClose the passthrough.\n");
	}
}

C_27A1_5F43(int si/*bp06*/) {
	int di;

	di = 0;
	CON_printf(/*2811*/"\nYou eat the food.\n");
	if(GetCoordUse(si) != LOCXYZ)
		di = 1;
	if(QuanType(GetType(si)) && GetQuan(si) > 1)
		SubQuan(si, 1);
	else
		DeleteObj(si);
	if(di)
		C_155D_0CF5();
}

/*Use vortex cube?*/
static C_27A1_5FAC(int objNum) {
	int si, di;
	int bp_12[8];
	int codex_objNum;

	/*all moonstones in the vortex cube?*/
	for(si = 0; si < 8; si ++)
		bp_12[si] = 0;
	for(di = FindInv(objNum); di >= 0; di = NextInv())
		bp_12[GetFrame(di)] = 1;
	for(si = 0; si < 8; si ++) {
		if(!bp_12[si])
			return 0;
	}
	/*Britannia lens?*/
	if(__SearchTypeAt(0x399, 0x353, 0, OBJ_18A) == -1)
		return 0;
	/*Gargoyle lens?*/
	if(__SearchTypeAt(0x39d, 0x353, 0, OBJ_18C) == -1)
		return 0;
	/*codex?*/
	codex_objNum = __SearchTypeAt(0x39b, 0x353, 0, OBJ_03B);
	if(codex_objNum == -1)
		return 0;
	if(
		MapX - 5 > 0x39b || MapX + 5 < 0x39b ||
		MapY - 5 > 0x353 || MapY + 5 < 0x353
	) return 0;
	/*all conditions are OK*/
	DeleteObj(codex_objNum);
	C_2FC1_1BCE();
	CON_printf(/*2825*/"\nThe Codex has vanished!\n");
	CON_getch();
	C_0903_0A1E();/*go to ending?*/

	return 1;
}

C_27A1_60BD() {
	CON_printf(/*283F*/"\nSqueak!\n");
	MUS_0065(22, 0);
}

C_27A1_60D6() {
	WindDir = (WindDir + 1) & 7;
	CON_printf(/*2849*/"\nYou feel a breeze.\n");
	C_0A33_1355(0);
}

unsigned D_1D59[] = {OBJ_1A7,OBJ_10E,OBJ_0DD,OBJ_1A4,OBJ_1AE,OBJ_131,OBJ_19F,OBJ_19C,OBJ_19E,OBJ_04E};

C_27A1_60F5(int bp06) {
		int si, di;
		int bp_04, bp_02;
		bp_04 = 0;
		di = GetType(bp06);
		for(si = 0; si < 10; si ++) {
			if(D_1D59[si] == di) {
				bp_04 = 1;
				break;
			}
		}
		if(bp_04 == 0)
			return 0;
		bp_02 = Party[Active];
		if(
			(IN_VEHICLE && bp_02 == bp06) ||
			(GetType(bp_02) == OBJ_19E && di == OBJ_19C)
		) return 0;
		return 1;
}

/*"use"*/
C_27A1_6179() {
	int si, di;
	int bp_0a, bp_08, bp_06, bp_04, bp_02;

	Selection.obj = COMBAT_getHead(Selection.obj);
	si = GetType(Selection.obj);
	if(
		(Selection.obj <= 0xff && Selection.x != -1 &&
		si != OBJ_1AE && si != OBJ_1AF && si != OBJ_1AC &&
		!IN_VEHICLE) ||
		(IsTileIg(TILE_FRAME(Selection.obj)))
	) Selection.obj = C_27A1_0919(Selection.obj);
	if(Selection.obj == -1) {
		CON_printf(/*1D6D*/"%s\n", D_0DDC[5]);
		return;
	}
	Selection.obj = COMBAT_getHead(Selection.obj);
	if(si == OBJ_1AF) {
		CON_printf(/*285E*/"horse\n");
	} else {
		CON_printf((char *)GetObjectString(Selection.obj));
		CON_putch('\n');
	}
	si = GetType(Selection.obj);
	if(!C_27A1_01DE(si)) {
		CON_printf(/*1F13*/"\n%s\n", D_0DDC[11]);
		return;
	}
	if(IN_VEHICLE && C_27A1_60F5(Selection.obj)) {
		CON_printf(/*1D6F*/"\n");
		CON_printf(NotOnShipMsg);
		return;
	}
	if(GetCoordUse(Selection.obj) == LOCXYZ) {
		bp_02 = MkDirection(PointerX, PointerY);
		if(bp_02 != -1) {
			C_1E0F_0664(Party[Active], bp_02);
			C_1100_0306();
		}
		bp_08 = GetX(Party[Active]);
		bp_06 = GetY(Party[Active]);
		if(!CLOSE_ENOUGH(1, Selection.x, Selection.y, bp_08, bp_06)) {
			CON_printf(/*1F24*/"\n%s!\n", D_0DDC[3]);
			return;
		}
	}
	di = FindLoc(GetX(Selection.obj), GetY(Selection.obj), MapZ);
	bp_04 = GetType(di);
	if(
		(C_1944_0AA9(si) && !C_1944_0AA9(bp_04) && !IsTileIg(TILE_FRAME(di))) ||
		(si == OBJ_14E && bp_04 != OBJ_14E)
	) {
		CON_printf(/*27A4*/"\nNot now!\n");
		return;
	}
	if(Active != D_04B3 && GetCoordUse(Selection.obj) == EQUIP && si != OBJ_05A) {
		CON_printf(/*2865*/"\nMust be in your hand.\n");
		return;
	}
	bp_0a = GetFrame(Selection.obj);
	switch(si) {
		case OBJ_073:
		case OBJ_074:
		case OBJ_075: C_27A1_4E6F(Selection.obj); break;
		case OBJ_19C:
		case OBJ_19E:
		case OBJ_19F:
		case OBJ_1A7:
			if(GetCoordUse(Selection.obj) != LOCXYZ)
				CON_printf(/*1F13*/"\n%s\n", D_0DDC[11]);
			else
				C_27A1_5289(Selection.obj);
		break;
		case OBJ_10E: C_27A1_47F3(); break;
		case OBJ_0BA:
		case OBJ_0C0:
			if(bp_0a == 1)
				SetFrame(Selection.obj, 0);
			else
				CON_printf(/*1F13*/"\n%s\n", D_0DDC[11]);
			C_27A1_09A1(Selection.obj, Selection.x, Selection.y);
		break;
		case OBJ_0EC:
		case OBJ_1A3: C_27A1_338D(); break;
		case OBJ_0B6: C_27A1_37D3(); break;
		case OBJ_07A:
		case OBJ_091:
		case OBJ_0A4:
		case OBJ_0CE:
		case OBJ_0FD:
			if(si == OBJ_0CE && (bp_0a & 2))
				CON_printf(/*1F13*/"\n%s\n", D_0DDC[11]);
			else
				C_27A1_31F6(Selection.obj);
		break;
		case OBJ_05F:
		case OBJ_060:
		case OBJ_080:
		case OBJ_081:
		case OBJ_082:
		case OBJ_083:
		case OBJ_084:
		case OBJ_085:
		case OBJ_087:
		case OBJ_0B4:
		case OBJ_0B8:
		case OBJ_0D1:
		case OBJ_0D2:
		case OBJ_109: C_27A1_5F43(Selection.obj); break;
		case OBJ_0DD: C_27A1_3A35(Selection.obj); break;
		case OBJ_129:
		case OBJ_12A:
		case OBJ_12B:
		case OBJ_12C: C_27A1_2A44(Selection.obj, 0, 0, 0); break;
		case OBJ_062:
			if(GetCoordUse(Selection.obj) != LOCXYZ)
				CON_printf(/*1F13*/"\n%s\n", D_0DDC[11]);
			else
				C_27A1_2BBC(Selection.obj, 0, 0, 0);
		break;
		case OBJ_0B5: C_27A1_376D(); break;
		case OBJ_1AC: C_27A1_36E7(); break;
		case OBJ_120: C_27A1_433D(Selection.obj); break;
		case OBJ_09B: C_27A1_5935(); break;
		case OBJ_1A4:
			if(MapZ != 0 && MapZ != 5)
				CON_printf(/*1F13*/"\n%s\n", D_0DDC[11]);
			else
				C_27A1_49C3(Selection.obj);
		break;
		case OBJ_0D4: C_27A1_60D6(); break;
		case OBJ_108: C_27A1_4D46(); break;
		case OBJ_0EA: C_27A1_4E9B(); break;
		case OBJ_061: C_27A1_47E0(); break;
		case OBJ_04D: C_27A1_319F(Selection.obj); break;
		case OBJ_09D: C_27A1_335A(2); break;/*"music instrument" interface?*/
		case OBJ_09C: C_27A1_335A(3); break;/*"music instrument" interface?*/
		case OBJ_1AE: C_27A1_5503(Selection.obj); break;
		case OBJ_1AF: C_27A1_55F0(Selection.obj); break;
		case OBJ_03F:
		case OBJ_040: C_27A1_2D8E(Selection.obj); break;
		case OBJ_131:
			if(D_2CC3 == -1)
				C_101C_089E(Selection.obj);
			else
				CON_printf(/*287D*/"\nNot in solo mode.\n");
		break;
		case OBJ_10C: C_27A1_4479(Selection.obj); break;
		case OBJ_09E: C_27A1_335A(0x12); break;/*"music instrument" interface?*/
		case OBJ_049: C_27A1_3425(Selection.obj); break;
		case OBJ_057:
			if(GetCoordUse(Selection.obj) == LOCXYZ)
				CON_printf(/*1F13*/"\n%s\n", D_0DDC[11]);
			else
				C_27A1_5789();
		break;
		case OBJ_099: C_27A1_335A(0x13); break;/*"music instrument" interface?*/
		case OBJ_116:
		case OBJ_118: C_27A1_5DF2(Selection.obj); break;
		case OBJ_067:
		case OBJ_068:
			if(GetCoordUse(Selection.obj) == EQUIP)
				C_27A1_4FC6();
			else
				CON_printf(/*2891*/"\nNot readied.\n");
		break;
		case OBJ_113: C_27A1_3832(Selection.obj); break;
		case OBJ_0DF: C_27A1_3B59(Selection.obj); break;
		case OBJ_0A9: C_27A1_60BD(); break;
		case OBJ_0F2:
		case OBJ_0F3:
		case OBJ_0F4:
		case OBJ_0F5:
		case OBJ_0F6:
		case OBJ_0F7:
		case OBJ_0F8:
		case OBJ_0F9: C_27A1_4B98(Selection.obj); break;
		case OBJ_14E: C_27A1_32FA(Selection.obj); break;
		case OBJ_05D:
			if(MapZ != 0 && MapZ != 5)
				CON_printf(/*1F13*/"\n%s\n", D_0DDC[11]);
			else
				C_1944_42AC();
		break;
		case OBJ_139: C_27A1_5BCF(); break;
		case OBJ_04E:
			if(GetCoordUse(Selection.obj) == LOCXYZ)
				CON_printf(/*1F13*/"\n%s\n", D_0DDC[11]);
			else if(GetCoordUse(Selection.obj) == EQUIP)
				C_27A1_3537(Selection.obj);
			else
				CON_printf(/*2891*/"\nNot readied.\n");
		break;
		case OBJ_0AE: C_27A1_4672(Selection.obj); break;
		case OBJ_09A: C_27A1_5A97(Selection.obj); break;
		case OBJ_05A: C_27A1_2FD1(Selection.obj); break;
		case OBJ_03E:/*vortex cube*/
			if(IsArmageddon || !C_27A1_5FAC(Selection.obj))
				CON_printf(/*1F13*/"\n%s\n", D_0DDC[2]);
		break;
		case OBJ_0E9: C_27A1_3661(); break;
		case OBJ_128: C_27A1_335A(0x14); break;/*"music instrument" interface?*/
	}
	SubMov(Party[Active], 5);
	C_1100_0306();
	OtherAnimations();
}
