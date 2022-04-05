/*
 * Ultima VI - The False Prophet
 * Copyright (c) 1990 Origin Systems, Inc. and Richard Gariott
 * reverse-coded by Ergonomy Joe in 2013
 */
/*
	Status, equipment, inventory module
*/
#include  "u6.h"

unsigned char D_07CE = 0;

/*display fight mode*/
static char *D_07CF[] = {
	/*0831*/"COMMAND",
	/*0839*/"FRONT",
	/*083F*/"REAR",
	/*0844*/"FLANK",
	/*084A*/"BERSERK",
	/*0852*/"RETREAT",
	/*085A*/"ASSAULT"
};

/*display party info*/
C_155D_000C(int index) {
	int i;
	int bp_08, bp_06, bp_04, bp_02;

	bp_08 = CON_mouseOffAt(176, 6, 311, 103);
	GR_06(D_2A54);
	GR_12(168, 6, 312, 103);
	C_2FC1_19C5();
	CON_setClip(&D_B6B5[0]);
	/*up arrow*/
	if(index) {
		CON_gotoxy(21, 3);
		CON_putch(0x18);
	}
	/*down arrow*/
	if(index < 16 && PartySize > index + 5) {
		CON_gotoxy(21, 12);
		CON_putch(0x19);
	}
	CON_setClip(&D_B6B5[1]);
	for(i = index; i < PartySize && i < index + 5; i ++) {
		bp_02 = Party[i];
		bp_04 = OrigShapeType[bp_02] & 0x3ff;
		bp_06 = OBJ_MakeDirFrame(bp_04, 4) + BaseTile[bp_04];
		C_0C9C_0E57(bp_06, TIL_19B, 176, TIL2SCR(i - index) + 24);
		CON_gotoxy(14, ((i - index) << 1) + 2);
		if(IsPoisoned(bp_02))
			D_04DF->_fg = D_2A4C;
		else if(HitPoints[bp_02] < 10)
			D_04DF->_fg = D_2A4A;
		CON_printf(/*0862*/"%3d\n", HitPoints[bp_02]);
		D_04DF->_fg = D_2A56;
		CON_gotoxy(3, ((i - index) << 1) + 3);
		CON_printf(/*0867*/"%s\n", Names[i]);
	}
	CON_setClip(&D_B6B5[3]);
	CON_mouseOn(bp_08);
}

/*character combat mode*/
/*C_155D_01BC*/STAT_refreshComMode() {
	int si, di;
	int bp_04;
	struct tClipInfo *bp_02;

	bp_04 = CON_mouseOffAt(176, 6, 311, 103);
	bp_02 = D_04DF;
	CON_setClip(&D_B6B5[1]);
	GR_06(D_2A54);
	GR_12(256, 96, 311, 103);
	si = Party[D_04B3];
	di = strlen(D_07CF[NPCComMode[si] - AI_COMMAND]);
	CON_gotoxy((8 - di) / 2 + 10, 11);
	CON_printf(/*086B*/"%s", D_07CF[NPCComMode[si] - AI_COMMAND]);
	CON_setClip(bp_02);
	CON_mouseOn(bp_04);
}

/*character status view*/
C_155D_028A(int di/*bp06*/) {
	int objNum,bp_02;

	bp_02 = CON_mouseOffAt(176, 6, 311, 103);
	GR_06(D_2A54);
	GR_12(168, 6, 312, 103);
	CON_setClip(&D_B6B5[1]);
	CON_gotoxy((17 - strlen(Names[di])) / 2, 0);
	CON_printf(/*0867*/"%s\n", Names[di]);
	C_2FC1_1C19(Party[di], 176, 16);/*load portrait?*/
	objNum = Party[di];
	CON_gotoxy(10, 2); CON_printf(/*086E*/"STR:%d\n", STREN[objNum]);
	CON_gotoxy(10, 3); CON_printf(/*0876*/"DEX:%d\n", DEXTE[objNum]);
	CON_gotoxy(10, 4); CON_printf(/*087E*/"INT:%d\n", INTEL[objNum]);
	CON_gotoxy(10, 6); CON_printf(/*0886*/"Magic");
	CON_gotoxy(10, 7); CON_printf(/*088C*/"%2d%/%d", MAGIC[objNum], MaxMagic(objNum));
	CON_gotoxy(10, 8); CON_printf(/*0894*/"Health");
	CON_gotoxy(10, 9);
	if(IsPoisoned(objNum))
		D_04DF->_fg = D_2A4C;
	else if(HitPoints[objNum] < 10)
		D_04DF->_fg = D_2A4A;
	CON_printf(/*089B*/"%3d", HitPoints[objNum]);
	D_04DF->_fg = D_2A56;
	CON_printf(/*088F*/"%/%d", MaxHP(objNum));
	CON_gotoxy(10, 10); CON_printf(/*089F*/"Lev%/Exp");
	CON_gotoxy(10, 11); CON_printf(/*08A8*/"%d%/%d", GetLevel(objNum), ExpPoints[objNum]);
	if(D_04B3 > 0)
		GR_2D(TIL_183, 176, 88);
	GR_2D(TIL_180, 192, 88);
	GR_2D(TIL_182, 208, 88);
	if(PartySize - 1 > D_04B3)
		GR_2D(TIL_184, 224, 88);
	CON_setClip(&D_B6B5[3]);
	CON_mouseOn(bp_02);
}

int D_07DD[] = {
	TIL_220,TIL_221,TIL_223,TIL_224,TIL_225,TIL_226,TIL_227,TIL_22A,
	TIL_22F,TIL_230,TIL_238,TIL_254,TIL_256,TIL_255,TIL_259,TIL_262,
	TIL_263,TIL_264,TIL_270,TIL_271,TIL_272,TIL_273,TIL_274,TIL_275,
	TIL_279,TIL_27D,TIL_27E,TIL_27F,TIL_280,TIL_281,TIL_2A2,TIL_2A3,
	TIL_2B9
};

/*C_155D_0516*/STAT_GetEquipSlot(int objNum) {
	int si, di;

	si = TILE_FRAME(objNum);
	if(si == TIL_21A || si == TIL_21B)
		return SLOT_FEET;
	if(si == TIL_258 || (si >= TIL_37D && si <= TIL_37F))
		return SLOT_RING;
	if(si == TIL_219 || (si >= TIL_250 && si <= TIL_252) || si == TIL_217 || si == TIL_101)
		return SLOT_NECK;
	if(si >= TIL_200 && si <= TIL_207)
		return SLOT_HEAD;
	if((si >= TIL_210 && si <= TIL_216) || si == TIL_218 || si == TIL_219 || si == TIL_28C || si == TIL_28E || si == TIL_29D || si == TIL_257)
		return SLOT_CHST;
	if(si == TIL_228 || si == TIL_229 || si == TIL_231 || si == TIL_235 || (si >= TIL_22B && si <= TIL_22E))
		return SLOT_2HND;
	if((si >= TIL_208 && si <= TIL_20F) || si == TIL_222)
		return SLOT_LHND;
	for(di = 0; di < 33; di ++)
		if(D_07DD[di] == si)
			return SLOT_RHND;
	return -1;
}

int D_081F[] = {OBJ_058,OBJ_041,OBJ_042,OBJ_043,OBJ_044,OBJ_045,OBJ_046,OBJ_047,OBJ_048};

C_155D_063A(int objType) {
	int si;

	for(si = 0; si < 9; si ++) {
		if(D_081F[si] == objType)
			return 1;
	}
	return 0;
}

/*C_155D_0661*/GetWeight(int objNum) {
	int si, qty, weight, bp_02;

	si = GetType(objNum);
	bp_02 = QuanType(si);
	if(bp_02 == 4)
		qty = Amount[objNum];
	else if(bp_02 == 2 && (si != OBJ_05A || GetFrame(objNum) != 1))
		qty = GetQuan(objNum);
	else
		qty = 1;
	if(C_155D_063A(si))
		weight = (TypeWeight[si] * qty) / 10;
	else
		weight = TypeWeight[si] * qty;
	if(weight == 255)
		weight = 10000;
	return weight;
}

/*C_155D_0707*/Encumbrance(int objNum) {
	int si, weight;

	weight = 0;
	for(
		si = Link[objNum];
		si >= 0 && GetCoordUse(si) != LOCXYZ && C_1184_1D2E(si, objNum);
		si = Link[si]
	) weight += GetWeight(si);
	return weight;
}

C_155D_0748(int objNum) {
	int si, di;
	int objTyp;

	InvenSize = 0;
	WeightInven = 0;
	if(objNum < 0x100)
		WeightEquip = 0;
	for(di = 0; di < 8; di ++)
		if(Party[di] == objNum)
			break;
	for(si = FindInv(objNum); si >= 0; si = NextInv()) {
		objTyp = GetType(si);
		if(GetCoordUse(si) == EQUIP) {
			WeightEquip += TypeWeight[objTyp];
		} else {
			WeightInven += GetWeight(si) + Encumbrance(si);
			InvenSize ++;
		}
	}
}

C_155D_07E0(int objNum) {
	int slot, di;
	int bp_04, bp_02;

	InvenSize = 0;
	WeightInven = 0;
	if(objNum < 0x100)
		WeightEquip = 0;
	for(bp_02 = 0; bp_02 < 8; bp_02 ++) {
		if(Party[bp_02] == objNum)
			break;
	}
	for(di = FindInv(objNum); di >= 0; di = NextInv()) {
		bp_04 = GetType(di);
		if(GetCoordUse(di) == EQUIP) {
			WeightEquip += TypeWeight[bp_04];
			slot = STAT_GetEquipSlot(di);
			if(slot == SLOT_2HND) {
				slot = SLOT_RHND;
				Equipment[SLOT_LHND] = 1;
				if(!ShowNPCInven)
					Party_1Hand[bp_02] = 1;
				else
					NPC_1Hand = 1;
			} else if(slot == SLOT_RHND && Equipment[SLOT_RHND]) {
				slot = SLOT_LHND;
			} else if(slot == SLOT_LHND && Equipment[SLOT_LHND]) {
				slot = SLOT_RHND;
			} else if(slot == SLOT_RING) {
				if(Equipment[SLOT_RFNG])
					slot = SLOT_LFNG;
				else
					slot = SLOT_RFNG;
			}
			Equipment[slot] = di;
		} else {
			WeightInven += GetWeight(di) + Encumbrance(di);
			InvenSize ++;
		}
	}
}

/*draw equipment*/
C_155D_08F4(register int x, register int y) {
	int slot, objNum;

	for(slot = 0; slot < 8; slot ++) {
		objNum = Equipment[slot];
		if(objNum) {
			switch(slot) {
				case SLOT_HEAD: C_0C9C_0E57(TILE_FRAME(objNum), TIL_19B, x + 200, y + 16); break;
				case SLOT_NECK: C_0C9C_0E57(TILE_FRAME(objNum), TIL_19B, x + 176, y + 24); break;
				case SLOT_CHST: C_0C9C_0E57(TILE_FRAME(objNum), TIL_19B, x + 224, y + 24); break;
				case SLOT_RHND: C_0C9C_0E57(TILE_FRAME(objNum), TIL_19B, x + 176, y + 40); break;
				case SLOT_LHND:
					if((!ShowNPCInven && Party_1Hand[D_04B3]) || (ShowNPCInven && NPC_1Hand))
						GR_2D(TIL_185, x + 224, y + 40);
					else
						C_0C9C_0E57(TILE_FRAME(objNum), TIL_19B, x + 224, y + 40);
				break;
				case SLOT_RFNG: C_0C9C_0E57(TILE_FRAME(objNum), TIL_19B, x + 176, y + 56); break;
				case SLOT_LFNG: C_0C9C_0E57(TILE_FRAME(objNum), TIL_19B, x + 224, y + 56); break;
				case SLOT_FEET: C_0C9C_0E57(TILE_FRAME(objNum), TIL_19B, x + 200, y + 64); break;
			}
		} else {
			switch(slot) {
				case SLOT_HEAD: GR_2D(TIL_19A, x + 200, y + 16); break;
				case SLOT_NECK: GR_2D(TIL_19A, x + 176, y + 24); break;
				case SLOT_CHST: GR_2D(TIL_19A, x + 224, y + 24); break;
				case SLOT_RHND: GR_2D(TIL_19A, x + 176, y + 40); break;
				case SLOT_LHND:
					if((!ShowNPCInven && Party_1Hand[D_04B3]) || (ShowNPCInven && NPC_1Hand))
						GR_2D(TIL_185, x + 224, y + 40);
					else
						GR_2D(TIL_19A, x + 224, y + 40);
				break;
				case SLOT_RFNG: GR_2D(TIL_19A, x + 176, y + 56); break;
				case SLOT_LFNG: GR_2D(TIL_19A, x + 224, y + 56); break;
				case SLOT_FEET: GR_2D(TIL_19A, x + 200, y + 64); break;
			}
		}
	}
}

static C_155D_0CB6(int bp06) {
	int si, di;

	si = bp06 / 10;
	di = bp06 % 10;
	if(si)
		return si + (di >= 5);
	else
		return si + (di != 0);
}

C_155D_0CF5() {
	int si, di;
	int bp_0a, bp_08, bp_06, bp_04;
	struct tClipInfo *bp_02;

	if(StatusDisplay == CMD_91 && D_EBB5 == 0)
		return;
	bp_0a = CON_mouseOffAt(176, 6, 311, 103);
	bp_02 = D_04DF;
	CON_setClip(&D_B6B5[1]);
	GR_06(D_2A54);
	GR_12(248, 16, 311, 79);
	C_155D_08F4(0, 0);
	di = FindInv(D_E709);
	si = 0;
	if(D_07CE >= 4 && D_07CE + 8 >= InvenSize)
		D_07CE -= 4;
	while(D_07CE > si) {
		if(GetCoordUse(di) == INVEN || GetCoordUse(di) == CONTAINED)
			si ++;
		di = NextInv();
	}
	bp_08 = GetType(D_E709);
	if(D_E709 < 0x100) {
		bp_08 = OrigShapeType[D_E709] & 0x3ff;
		bp_06 = OBJ_MakeDirFrame(bp_08, 4) + BaseTile[bp_08];
		C_0C9C_0E57(bp_06, TIL_19B, 272, 16);
	} else {
		C_0C9C_0E57(TILE_FRAME(D_E709), TIL_19B, 272, 16);
	}
	for(si = 0; di >= 0 && si < 12; di = NextInv()) {
		if(GetCoordUse(di) == INVEN || GetCoordUse(di) == CONTAINED) {
			C_0C9C_0D31(di, 248 + TIL2SCR(si & 3), 32 + TIL2SCR(si >> 2));
			D_E70F[si++] = di;
		}
	}
	for( ;si < 12; si ++) {
		D_E70F[si] = 0;
		GR_2D(TIL_19A, 248 + TIL2SCR(si & 3), 32 + TIL2SCR(si >> 2));
	}
	if(InvenSize > D_07CE + 12) {
		CON_gotoxy(8, 8);
		CON_putch(0x19);
	} else {
		CON_gotoxy(8, 8);
		CON_putch(' ');
	}
	if(D_07CE == 0) {
		CON_gotoxy(8, 3);
		CON_putch(' ');
	} else {
		CON_gotoxy(8, 3);
		CON_putch(0x18);
	}
	bp_04 = D_E709;
	if(bp_04 >= 0x100) {
		do
			bp_04 = GetAssoc(bp_04);
		while(bp_04 >= 0x100);
	}
	/*display equiped weight*/
	CON_gotoxy(2, 9);
	CON_printf(/*08AF*/"      ");
	CON_gotoxy(2, 9);
	CON_printf(/*08B6*/"%d%/%ds", C_155D_0CB6(WeightEquip), STREN[bp_04]);
	/*display inventory weight*/
	CON_gotoxy(11, 9);
	CON_printf(/*08AF*/"      ");
	CON_gotoxy(11, 9);
	CON_printf(/*08B6*/"%d%/%ds", C_155D_0CB6(WeightInven + WeightEquip), STREN[bp_04] << 1);
	/* */
	CON_setClip(bp_02);
	CON_mouseOn(bp_0a);
}


C_155D_1022(int objNum) {
	int si, di;

	for(si = 0; si < 12; si ++)
		D_E70F[si] = 0;
	if(objNum < 0x100) {
		for(si = 0; si < 8; si ++)
			Equipment[si] = 0;
	}
	C_155D_07E0(objNum);
	C_155D_0CF5();
}

/*character equipment view*/
C_155D_1065(int __unused__) {
	int si;

	D_07CE = 0;
	si = CON_mouseOffAt(176, 6, 311, 103);
	GR_06(D_2A54);
	GR_12(168, 6, 312, 103);
	CON_setClip(&D_B6B5[1]);
	if(!ShowNPCInven) {
		CON_gotoxy((17 - strlen(Names[D_04B3])) / 2, 0);
		CON_printf(/*0867*/"%s\n", Names[D_04B3]);
	}
	C_2FC1_1EAF(192, 32);
	CON_gotoxy(0, 9); CON_printf(/*08BE*/"E:");
	CON_gotoxy(9, 9); CON_printf(/*08C1*/"I:");
	if(!ShowNPCInven) {
		if(D_04B3 > 0)
			GR_2D(TIL_183, 176, 88);
		GR_2D(TIL_180, 192, 88);
		GR_2D(TIL_181, 208, 88);
		if(PartySize - 1 > D_04B3)
			GR_2D(TIL_184, 224, 88);
		if(D_04B3)
			GR_2D(TIL_187, 240, 88);
	}
	if(!ShowNPCInven)
		D_E709 = Party[D_04B3];
	else
		D_E709 = ShowNPCInven;
	Party_1Hand[D_04B3] = 0;
	C_155D_1022(D_E709);
	STAT_refreshComMode();
	CON_setClip(&D_B6B5[3]);
	CON_mouseOn(si);
}

C_155D_120A(int __unused__) {
	CON_setClip(&D_B6B5[1]);
	if(D_07CE + 4 < InvenSize) {
		D_07CE += 4;
		C_155D_0CF5();
		CON_setClip(&D_B6B5[3]);
	}
}

C_155D_123E(int __unused__) {
	CON_setClip(&D_B6B5[1]);
	if(D_07CE >= 4) {
		D_07CE -= 4;
		C_155D_0CF5();
		CON_setClip(&D_B6B5[3]);
	}
}

C_155D_1267() {
	int si;
	if(
		PointerX >= 248 && PointerX < 312 &&
		PointerY >= 32 && PointerY < 80
	) {
		si = SCR2TIL(PointerX - 248);
		si += SCR2TIL(PointerY - 32) << 2;
		Selection.obj = D_E70F[si];
		if(Selection.obj == 0)
			Selection.obj = -1;
		Selection.x = si + 1;
		return 1;
	}
	if(PointerX >= 272 && PointerX < 288 && PointerY >= 16 && PointerY < 32) {
		if(D_E709 >= 0x100) {
			Selection.x = 0;
		} else {
			Selection.x = MapX;
			Selection.y = MapY;
		}
		Selection.obj = D_E709;
		return 1;
	}
	Selection.obj = -1;
	return 0;
}

C_155D_130E() {
	if(PointerX >= 200 && PointerX < 216 && PointerY >= 16 && PointerY < 32) {
		Selection.x = 0;
		Selection.obj = Equipment[SLOT_HEAD];
	} else if(PointerX >= 176 && PointerX < 192 && PointerY >= 24 && PointerY < 72) {
		Selection.x = SCR2TIL(PointerY - 24) + 1;
		Selection.obj = Equipment[Selection.x];
	} else if(PointerX >= 224 && PointerX < 240 && PointerY >= 24 && PointerY < 72) {
		Selection.x = SCR2TIL(PointerY - 24) + 4;
		Selection.obj = Equipment[Selection.x];
	} else if(PointerX >= 200 && PointerX < 216 && PointerY >= 64 && PointerY < 80) {
		Selection.x = 7;
		Selection.obj = Equipment[SLOT_FEET];
	} else {
		Selection.obj = -1;
		return 0;
	}
	if(Selection.obj == 0 || Selection.obj == 1)
		Selection.obj = -1;
	return 1;
}

C_155D_13F4(int objNum) {
	int i;

	for(i = 0; i < 8; i++) {
		if(Equipment[i] == objNum)
			return i;
	}

	return -1;
}

/*uncalled?*/
__155D_141B(int objTyp) {
	int i;

	for(i = 0; i < 8; i++) {
		if(GetType(Equipment[i]) == objTyp)
			return i;
	}

	return -1;
}

/*"Ready"*/
C_155D_144B() {
	int slot, di;
	int objTyp, strength, bp_02;

	bp_02 = Party[D_04B3];
	objTyp = GetType(Selection.obj);
	strength = STREN[bp_02];
	CON_printf(/*08C4*/"Ready-");
	if(objTyp == OBJ_05A)
		CON_printf(/*08CB*/"torch\n");
	else
		CON_printf(/*0867*/"%s\n", GetObjectString(Selection.obj));
	if((slot = STAT_GetEquipSlot(Selection.obj)) == -1) {
		CON_printf(/*08D2*/"\nCan't be readied!\n");
	} else if(TypeWeight[objTyp] + WeightEquip > strength * 10) {
		CON_printf(/*08E6*/"\nToo heavy!\n");
	} else {
		if(slot == SLOT_2HND) {
			if(Equipment[SLOT_RHND]) {
				slot = SLOT_RHND;
			} else if(Equipment[SLOT_LHND]) {
				slot = SLOT_LHND;
			} else {
				slot = SLOT_RHND;
				Equipment[SLOT_LHND] = 1;
				Party_1Hand[D_04B3] = 1;
			}
		} else if(slot == SLOT_RHND && Equipment[SLOT_RHND] && Equipment[SLOT_LHND] == 0) {
			slot = SLOT_LHND;
		} else if(slot == SLOT_LHND && Equipment[SLOT_LHND] && Equipment[SLOT_RHND] == 0) {
			slot = SLOT_RHND;
		} else if(slot == SLOT_RING) {
			if(Equipment[SLOT_RFNG])
				slot = SLOT_LFNG;
			else
				slot = SLOT_RFNG;
		}
		if(Equipment[slot]) {
			CON_printf(/*08F3*/"\nNo place to put!\n");
		} else {
			di = Selection.obj;
			do
				di = GetAssoc(di);
			while(di >= 0x100);
			if(objTyp == OBJ_05A) {
				if(SubQuan(Selection.obj, 1) == 0)
					DeleteObj(Selection.obj);
				Equipment[slot] = AddInvObj(OBJ_05A, EQUIP, 1, di);
			} else {
				Equipment[slot] = InsertObj(Selection.obj, di, EQUIP);
				InvenSize --;
			}
			C_155D_1022(Party[D_04B3]);
			if(GetType(Selection.obj) == OBJ_102) {
				SetInvisible(bp_02);
				MagicalHalo(bp_02, 1);
			} else if(GetType(Selection.obj) == OBJ_051) {
				SpellFx[13] = 20;
				MagicalHalo(bp_02, 1);
			}
		}
	}
	CON_printf(/*0906*/"\n%s:\n%>", Names[Active]);
}

C_155D_1666(int si/*bp08*/, int __unused__) {
	int di;

	if(GetCoordUse(si) == EQUIP) {
		di = GetType(si);
		if(di == OBJ_04C) {
			CON_printf(/*090E*/"\n%s\n", D_0DDC[12]);
			return 1;
		}
		if(di == OBJ_05A && GetFrame(si) == 1) {
			DeleteObj(si);
			CON_printf(/*0862 + 3*/"\n");
			CON_printf(BurnOutMsg);
			return 2;
		}
	}
	return 0;
}

/*stop equiped ring/cloak effect*/
/*(the return value is not used)*/
C_155D_16E7(int objNum, int di/*bp06*/) {
	int objTyp;

	if(GetCoordUse(objNum) == EQUIP) {
		objTyp = GetType(objNum);
		if(objTyp == OBJ_102) {
			ClrInvisible(di);
			MagicalHalo(di, 1);
			return 3;
		}
		if(objTyp == OBJ_051) {
			SpellFx[13] = 0;
			MagicalHalo(di, 1);
			return 3;
		}
	}
	return 0;
}

/*"Unready"*/
C_155D_1738() {
	int si, di;
	int objTyp;

	di = 0;
	if(Selection.obj == 1)
		return;
	CON_printf(/*0913*/"Unready-%s\n", GetObjectString(Selection.obj));
	objTyp = GetType(Selection.obj);
	si = Party[D_04B3];
	di = C_155D_1666(Selection.obj, si);
	if(di != 1) {
		Equipment[C_155D_13F4(Selection.obj)] = 0;
		if(di != 2) {
			C_155D_16E7(Selection.obj, si);
			if(STAT_GetEquipSlot(Selection.obj) == SLOT_2HND) {
				Party_1Hand[D_04B3] = 0;
				Equipment[SLOT_LHND] = 0;
			}
			if(objTyp == OBJ_05A && Is_0010(FindInvType(si, OBJ_05A, 0))) {
				DeleteObj(Selection.obj);
				GiveObj(si, OBJ_05A, 1);
			} else {
				SetCoordUse(Selection.obj, INVEN);
				InvenSize ++;
			}
		}
	}
	C_155D_1022(Party[D_04B3]);
	CON_printf(/*0906*/"\n%s:\n%>", Names[Active]);
}
