/*
 * Ultima VI - The False Prophet
 * Copyright (c) 1990 Origin Systems, Inc. and Richard Gariott
 * reverse-coded by Ergonomy Joe in 2013
 */
/*

*/
#include "u6.h"

#undef abs
#include <math.h>

int D_0338 = 0;/*frameCounter?*/
/*033A*/unsigned char StatusDirty = 1;
/*033B*/unsigned char PromptFlag = 0;
/*033C*/unsigned char CyclopsFlag = 0;
/*033D*/unsigned char IsArmageddon = 0;
int D_033E = 0;
int D_0340 = 1;
/*0342*/int Erupting = 0;/*volcano/fumarole*/
/*same as D_075E, but unused?*/
unsigned char __0344[] = {TERRAIN_FLAG_80,0,TERRAIN_FLAG_40,0,TERRAIN_FLAG_20,0,TERRAIN_FLAG_10};

/*C_0A33_0008*/SubKarma(int bp06) {
	if(KARMA > bp06)
		KARMA -= bp06;
	else
		KARMA = 0;
}

/*never called?*/
/*C_0A33_0027*/AddKarma(bp06) {
	if(KARMA + bp06 < 100)
		KARMA += bp06;
	else
		KARMA = 99;
}

/*param bp06:
	0 = stop
	1 = normal
	2 = reverse*/
/*C_0A33_0049*/SetTileAnimation(int tileNum, int bp06) {
	int si;
	
	for(si = 0; si < NumAnimData; si ++) {
		if(D_67E4[si] == tileNum) {
			StateAnimData[si] = bp06;
			break;
		}
	}
}

/*animate tiles?*/
static C_0A33_0073() {
	int frm, i;
	int bp_0e, obj_x, obj_y, bp_08, mustRefresh, objTyp, bp_02;

	if(SpellFx[14])
		return;

	D_0338 ++;

	for(i = 0; i < NumAnimData; i++) {
		frm = D_9DF5[i];
		if(StateAnimData[i] == 1/*normal*/)
			frm += (D_0338 & D_9495[i]) >> D_B3F7[i];
		else if(StateAnimData[i] == 2/*reverse*/)
			frm += (~D_0338 & D_9495[i]) >> D_B3F7[i];
		GR_4B(D_67E4[i], frm);
	}

	if((D_0338 & 1) == 0)
		GR_3F();

	mustRefresh = 0;
	for(i = 0; i < 0x100; i ++) {
		if(
			ObjShapeType[i] == 0 ||
			GetZ(i) != MapZ ||
			!CLOSE_ENOUGH_S(6, GetX(i), GetY(i), MapX, MapY)
		) continue;

		objTyp = GetType(i);

		if(!D_033E)/*why test inside loop?*/
			continue;

		if(Isbis_0016(i))
			continue;

		frm = GetFrame(i);
		bp_08 = frm & 0xfc;
		if(objTyp >= OBJ_178 && objTyp <= OBJ_19A && objTyp != OBJ_184) {
			if(objTyp == OBJ_187) {
				if(OSI_rand(0, 3)) {
					if(OSI_rand(0, 15) == 0) {
						ObjShapeType[i] = TypeFrame(OBJ_181, 0);
						frm = 0;
						bp_08 = 8;
						mustRefresh = 1;
					}
				} else {
					frm ^= 1;
					mustRefresh = 1;
				}
			} else if(objTyp == OBJ_188) {
				if(OSI_rand(0, 1)) {
					frm ^= 1;
					mustRefresh = 1;
				}
			} else if(objTyp == OBJ_17F) {
				frm &= 3;
				if(OSI_rand(0, 1) && frm < 3/*!?!*/) {
					if(++frm > 2)
						frm = 0;
					mustRefresh = 1;
				}
			} else {
				switch(frm & 3) {
					case 0: if(OSI_rand(0, 31) == 0) {
						frm = 1;
						mustRefresh = 1;
					} break;
					case 1: if(OSI_rand(0, 63) == 0) {
						frm = OSI_rand(0, 2);
						mustRefresh = 1;
					} break;
					case 2: if(OSI_rand(0, 31) == 0) {
						frm = 1;
						mustRefresh = 1;
					} break;
				}
				if(objTyp == OBJ_181 && NPCMode[i] == AI_FARM && OSI_rand(0, 15) == 0) {
					ObjShapeType[i] = TypeFrame(OBJ_187, 0);
					frm = 0;
					bp_08 = frm;
					mustRefresh = 1;
				}
			}
		} else if(objTyp == OBJ_16B) {
			bp_08 = frm - (frm % 3);
			frm %= 3;
			switch(frm) {
				case 0: if(OSI_rand(0, 31) == 0) {
					frm = 1;
					mustRefresh = 1;
				} break;
				case 1: if(OSI_rand(0, 63) == 0) {
					frm = OSI_rand(0, 2);
					mustRefresh = 1;
				} break;
				case 2: if(OSI_rand(0, 31) == 0) {
					frm = 1;
					mustRefresh = 1;
				} break;
			}
		} else if(objTyp == OBJ_16A) {
			bp_08 = frm - (frm % 12);
			frm %= 12;
			switch(frm) {
				case 3:
					if(OSI_rand(0, 31) == 0) {
						frm = 7;
						mustRefresh = 1;
					}
				break;
				case 7:
					if(OSI_rand(0, 63) == 0) {
						frm = OSI_rand(0, 1)?3:11;
						mustRefresh = 1;
					}
				break;
				case 0xb:
					if(OSI_rand(0, 31) == 0) {
						frm = 7;
						mustRefresh = 1;
					}
				break;
			}
			bp_08 = bp_08 + frm;
			frm = 0;
		} else if(objTyp >= OBJ_16F && objTyp <= OBJ_174) {
			if(OSI_rand(0, 7) == 0) {
				frm ^= 1;
				mustRefresh = 1;
			}
		} else if(objTyp == OBJ_158 || objTyp == OBJ_16C) {
			if(OSI_rand(0, 1)) {
				if(IsWalking(i)) {
					if(++frm >= 3) {
						frm = 3;
						ClrWalking(i);
					}
				} else {
					if(--frm <= 0) {
						frm = 0;
						SetWalking(i);
					}
				}
				mustRefresh = 1;
			}
		} else if(objTyp == OBJ_163 || objTyp == OBJ_15B || objTyp == OBJ_168) {
			if(OSI_rand(0, 1)) {
				if(++frm > 3)
					frm = 0;
				mustRefresh = 1;
			}
		} else if(objTyp == OBJ_157 || objTyp == OBJ_160 || objTyp == OBJ_161) {
			if(objTyp == OBJ_157 || OSI_rand(0, 1)) {
				frm = OSI_rand(0, 3);
				mustRefresh = 1;
			}
		} else if(objTyp == OBJ_175 || objTyp == OBJ_159) {
			if(OSI_rand(0, 1)) {
				if(OSI_rand(0, 15) == 0)
					frm = 3;
				else
					frm = OSI_rand(0, 2);
				mustRefresh = 1;
			}
		} else if(objTyp == OBJ_165) {
			if(frm && OSI_rand(0, 1)) {
				IsWalking(i)?frm --:frm ++;
				if(frm > 3) {
					frm = 2;
					SetWalking(i);
				}
				if(frm < 1) {
					frm = 2;
					ClrWalking(i);
				}
				mustRefresh = 1;
			}
		} else if(objTyp == OBJ_164) {
			if(frm < 3) {
				if(OSI_rand(0, 31) == 0) {
					frm = 3;
					mustRefresh = 1;
				} else if(OSI_rand(0, 1)) {
					if(++frm > 2)
						frm = 0;
					mustRefresh = 1;
				}
			}
		} else if(objTyp == OBJ_16D) {
			if(OSI_rand(0, 7) == 0) {
				frm ^= 1;
				mustRefresh = 1;
			}
		} else if(objTyp == OBJ_176) {
			obj_x = GetX(i);
			obj_y = GetY(i);
			for(bp_0e = 0; bp_0e < 8; bp_0e ++) {
				for(
					bp_02 = FindLoc(DirIncrX[bp_0e] + obj_x, DirIncrY[bp_0e] + obj_y, MapZ);
					bp_02 >= 0;
					bp_02 = NextLoc()
				) {
					if(GetType(bp_02) == OBJ_1A9 && (GetFrame(bp_02) >> 2) == bp_0e) {
						if(OSI_rand(0, 1)) {
							frm = GetFrame(bp_02);
							SetFrame(bp_02, (frm & 0x1c) + ((frm + 1) & 3));
							mustRefresh = 1;
						}
						break;
					}
				}
			}
			frm = GetFrame(i);
			if(OSI_rand(0, 1)) {
				if(frm == 0 || (frm < 3 && OSI_rand(0, 4) == 0))
					SetWalking(i);
				if(frm == 3 || (frm > 0 && OSI_rand(0, 4) == 0))
					ClrWalking(i);
				if(IsWalking(i))
					frm ++;
				else
					frm --;
				mustRefresh = 1;
			}
		} else if(objTyp == OBJ_19B) {
			if(OSI_rand(0, 1)) {
				frm ^= 1;
				mustRefresh = 1;
			}
			obj_x = GetX(i);
			obj_y = GetY(i);
			for(bp_0e = 0; bp_0e < 8; bp_0e += 2) {
				bp_02 = C_1184_10F1(obj_x, obj_y, MapZ, bp_0e, GetFrame(i));
				if(bp_02 >= 0) {
					if(GetFrame(bp_02) >= 0x18) {
						SetFrame(bp_02, (GetFrame(bp_02) & 0x38) | (frm & 7));
					} else if(OSI_rand(0, 7) == 0) {
						SetFrame(bp_02, GetFrame(bp_02) ^ 1);
						mustRefresh = 1;
					}
				}
			}
		} else if(
			objTyp == OBJ_15C || objTyp == OBJ_15D || objTyp == OBJ_15E ||
			objTyp == OBJ_156 || objTyp == OBJ_166 || objTyp == OBJ_169 ||
			objTyp >= OBJ_1AA || objTyp == OBJ_15F || objTyp == OBJ_15A ||
			(objTyp >= OBJ_16F && objTyp <= OBJ_174)
		) {
			if(OSI_rand(0, 7) == 0) {
				if(objTyp != OBJ_1AC || OSI_rand(0, 3) == 0) {
					frm ^= 1;
					mustRefresh = 1;
					bp_02 = OBJ_getHead(i);
					if(bp_02 >= 0)
						SetFrame(bp_02, GetFrame(bp_02) ^ 1);
				}
			}
		}

		frm = (frm & 3) + bp_08;
		SetFrame(i, frm);
	}/*end for*/

	D_033E = 0;

	if(mustRefresh)
		ShowObjects();
}

unsigned char D_034B[] = {0,1,1,2,3,3,4,5,5,6,7,7};

/*display clock face?*/
static C_0A33_0931(int x, int y) {
	int si;

	x += 4;
	y += 14;
	GR_06(D_2A58);
	D_ECC4->_0f = 1;
	/*centre*/
	GR_09(x, y);
	/*short hand*/
	si = D_034B[Time_H % 12];
	GR_09(DirIncrX[si] + x, DirIncrY[si] + y);
	/*long hand*/
	si = (Time_M * 2) / 15;
	GR_09(DirIncrX[si] + x, DirIncrY[si] + y);
	D_ECC4->_0f = 0;
}

/*draw map area?*/
C_0A33_09CE(int bp06/*0:offScreen,1:onScreen*/) {
	int i, j;
	int bp_0a, scr_x, scr_y, bp_04, bp_02;

	GR_06(D_2A50);
	for(j = 0; j < 11; j++) {
		for(i = 0; i < 11; i++) {
			scr_x = TIL2SCR(i);
			scr_y = TIL2SCR(j);
			ServeMouse
			/*tiles*/
			GR_42(Tile_11x11[j][i], scr_x, scr_y);
			/*objects?*/
			bp_0a = Obj_11x11[j][i];
			while(bp_0a) {
				bp_04 = D_D5DC[bp_0a];
				GR_42(bp_04, scr_x, scr_y);
				if(BaseTile[OBJ_09F] == (bp_04 & 0x7ff))
					C_0A33_0931(scr_x, scr_y);
				bp_0a = D_E5E0[bp_0a];
			}
		}
	}
	/*negate magic/storm cloak effect*/
	if(SpellFx[13]) {
		D_ECC4->_0c = 1;
		D_ECC4->_0f = 1;
		GR_06(8);
		for(i = 0; i < 100; i++)
			GR_09(OSI_rand(8, 167), OSI_rand(8, 167));
		D_ECC4->_0c = 0;
		D_ECC4->_0f = 0;
	}
	/* */
	if(!bp06)
		return;
	if(MouseMode == 0 && !D_0497) {
		C_0C9C_1AE5(0);
		SelectMode = 0;
	}
	if(SelectMode == 1) {
		/*mouse cursor*/
		if(SelectRange < 0)
			bp_04 = TIL_16C;/*direction*/
		else
			bp_04 = TIL_16D;/*select*/
		GR_42(bp_04, AimX * 16, AimY * 16);
	}
	/*draw frame*/
	GR_42(TIL_1B0,   0,   0);
	GR_42(TIL_1B2, 160,   0);
	GR_42(TIL_1B3,   0, 160);
	GR_42(TIL_1B5, 160, 160);
	for(i = 1; i < 10; i ++) {
		GR_42(TIL_1B1, TIL2SCR(i),          0);
		GR_42(TIL_1B4, TIL2SCR(i),        160);
		GR_42(TIL_1B6,          0, TIL2SCR(i));
		GR_42(TIL_1B7,        160, TIL2SCR(i));
	}
	/* */
	if(MouseOn) {
		MouseArea.left = 8;
		MouseArea.right = 167;
		MouseArea.top = 8;
		MouseArea.bottom = 167;
		bp_02 = MOUSE_IN_AREA(&MouseArea);
		if(bp_02) {
			Mouse.f_0e = &D_9E3D;
			Mouse.f_02 -= 8;
			Mouse.f_04 -= 8;
			MOUSE_0183();
			MOUSE_015C();
			Mouse.f_02 += 8;
			Mouse.f_04 += 8;
			Mouse.f_0e = &Screen;
		}
	}
	/*display the map part*/
	if(ScreenFade) {
		GR_48(8, 8, 167, 167);
		ScreenFade = 0;
	} else {
		GR_45(8, 8, 167, 167, 8, 8);
	}
}

/*C_0A33_0CBC*/OtherAnimations() {
	int si;

	if(D_17B0) {
		C_1100_0306();
		D_17B0 = 0;
	}
	if(D_0340) {
		C_0A33_0073();
		C_0A33_09CE(1);
		MUS_0525();
	}
	/*cyclops ground shake*/
	if(CyclopsFlag) {
		si = CyclopsFlag;
		CyclopsFlag = 0;
		ShakeScreen(si, 1);
	}
}

C_0A33_0D06() {
	if(D_17B0) {
		C_1100_0306();
		D_17B0 = 0;
		OtherAnimations();
	}
}

/*C_0A33_0D1D*/TerrainDamage(int objNum, int tileNum) {
	int isImune;
	int bp_06, objCls, dmg;

	isImune = 0;
	SetSkipSomeTest(objNum);
	if(IsDead(objNum))
		return;
	objCls = GetMonsterClass(GetType(objNum));
	switch(tileNum) {
		case TIL_234:/*web*/
			if(IsProtected(objNum))
				break;
			if(GetStr(objNum) < OSI_rand(1, 30) && GetType(objNum) != OBJ_169) {
				SetParalyzed(objNum);
				CON_printf(D_356A_047B, GetObjectString(objNum));
				PromptFlag = 1;
			}
		break;
		case TIL_002: case TIL_003: case TIL_004: case TIL_005:/*swamps?*/
			for(bp_06 = FindInvType(objNum, OBJ_01C, -1); bp_06 >= 0; bp_06 = NextInvType()) {
				if(GetCoordUse(bp_06) == EQUIP)
					break;
			}
			if(bp_06 >= 0)
				break;
		case TIL_48D:/*poison field?*/
			if(IsProtected(objNum))
				isImune = 1;
			if(IsMonster_0004(objCls))
				isImune = 1;
			if(!isImune && !IsPoisoned(objNum)) {
				CON_printf(PoisonedMsg, GetObjectString(objNum));
				PromptFlag = 1;
				C_0A33_0D06();
				COMBAT_showHit(objNum);
				SetPoisoned(objNum);
				StatusDirty = 1;
			}
		break;
		case TIL_0DC: case TIL_0DD: case TIL_0DE: case TIL_0DF:
		case TIL_37A: case TIL_464: case TIL_465: case TIL_46A:
		case TIL_46B: case TIL_48C: case TIL_4A9:/*fire related?*/
			if(IsProtected(objNum))
				isImune = 1;
			if(IsMonster_0008(objCls))
				isImune = 1;
			if(!isImune) {
				if(tileNum == TIL_48C)
					dmg = OSI_rand(1, 30);
				else
					dmg = OSI_rand(1, 8);
				if(IsMonster_0010(objCls))
					dmg <<= 1;
				C_0A33_0D06();
				LooseHP(objNum, dmg);
			}
		break;
		case TIL_5B2: case TIL_5B3: case TIL_5B4:
		case TIL_5B5: case TIL_5B6: case TIL_5B7:
			if(GetType(objNum) == OBJ_16E)
				break;
		case TIL_232: case TIL_2DB:
			if(IsProtected(objNum))
				break;
			C_0A33_0D06();
			LooseHP(objNum, OSI_rand(1, 8));
		break;
		case TIL_2DC: case TIL_3F2:
			if(IsProtected(objNum))
				break;
			C_0A33_0D06();
			LooseHP(objNum, OSI_rand(1, 30));
		break;
		case TIL_48F:/*sleep field*/
			if(IsMonster_0001(objCls))
				isImune = 1;
			if(IsProtected(objNum))
				isImune = 1;
			if(!isImune && !IsAsleep(objNum)) {
				C_0A33_0D06();
				COMBAT_showHit(objNum);
				SetAsleep(objNum);
				SetTypeUnconscious(objNum);
			}
		break;
	}
}

/*do terrain effects?*/
/*the "skipable" test?*/
C_0A33_0FEB(int objNum) {
	int di;
	int tileNum, x, y, area_x, area_y, bp_06, objTyp, objCls;

	objTyp = GetType(objNum);
	objCls = GetMonsterClass(objTyp);
	if(
		objTyp == OBJ_1A7 ||
		objTyp == OBJ_19E ||
		objTyp == OBJ_19F ||
		objTyp == OBJ_19C
	) return;
	if(objCls >= 0 && IsMonster_0100(objCls))
		return;
	if(IsSkipSomeTest(objNum))
		return;
	if(GetZ(objNum) != MapZ)
		return;

	if(MapZ)
		bp_06 = 0xff;
	else
		bp_06 = 0x3ff;
	area_x = (MapX - 0x10) & 0x3f8;
	area_y = (MapY - 0x10) & 0x3f8;
	x = GetX(objNum);
	y = GetY(objNum);
	if(((x - area_x) & bp_06) < AREA_W && ((y - area_y) & bp_06) < AREA_H) {
		tileNum = GetTileAtXYZ(x, y, MapZ);
		if(IsTerrainDamage(tileNum)) {
			TerrainDamage(objNum, tileNum);
			return;
		}
		for(di = FindLoc(x, y, MapZ); di >= 0; di = NextLoc()) {
			if(di == objNum)
				continue;
			tileNum = TILE_FRAME(di);
			if(IsTerrainDamage(tileNum)) {
				ClrInvisible(di);
				TerrainDamage(objNum, tileNum);
				break;
			}
		}
	}
}

/*eruption management*/
static C_0A33_117A() {
	int objTyp, objNum;

	if(Erupting <= 1)
		return;
	objTyp = GetType(Erupting);
	if(objTyp == OBJ_133) {
		ShakeScreen(10, 2);
	} else if(objTyp == OBJ_0AB) {
		if(OSI_rand(0, 1)) {
			ShakeScreen(10, 2);
		} else {
			objNum = AddMapObj(OBJ_0AB, 0, 0, GetX(Erupting), GetY(Erupting), GetZ(Erupting));
			Detonate(objNum);
		}
	}
	Erupting = 0;
}

/*moongates management?*/
C_0A33_121A() {
	int i, x, y, z, objNum, bp_02;

	bp_02 = 0;
	for(i = 0; i < 8; i ++) {
		x = D_2C74[i][0];
		y = D_2C74[i][1];
		z = D_2C74[i][2];
		if(
			z == MapZ &&
			x >= AreaX && x < AreaX + AREA_W &&
			y >= AreaY && y < AreaY + AREA_H
		) {
			objNum = C_1184_07A7(OBJ_055, x, y);
			if(D_2CC7 < 15 || D_2CC9 < 15) {
				if(objNum < 0) {
					AddMapObj(TypeFrame(OBJ_055, 1), 0, 0, x, y, z);
					bp_02 = 1;
				}
			} else {
				if(objNum >= 0) {
					DeleteObj(objNum);
					bp_02 = 1;
				}
			}
		}
	}
	if(bp_02) {
		C_1100_0306();
		D_17B0 = 1;
	}
}

C_0A33_12F6(int noChange) {
	if(MapZ > 0 && MapZ < 5 && WindDir >= 0) {
		WindDir = -1;
		noChange = 1;
	}
	if(noChange)
		return;
	if(OSI_rand(0, 63))
		return;
	if(MapZ == 0 || MapZ == 5) {
		WindDir = OSI_rand(0, 8) - 1;
		noChange = 1;/*???*/
	}
}

/*directions?*/
char *D_0357[] = {
	/*03B6*/"C ",
	/*03B9*/"N ",
	/*03BC*/"NE",
	/*03BF*/"E ",
	/*03C2*/"SE",
	/*03C5*/"S ",
	/*03C8*/"SW",
	/*03CB*/"W ",
	/*03CE*/"NW"
};
unsigned char D_0369 = -1;
/*moons cycle per day? */
unsigned char D_036A[][2] = {
	{0,0},/*day 1*/
	{7,0},/*day 2*/
	{7,7},/*day 3*/
	{6,6},/*day 4*/
	{6,5},/*day 5*/
	{5,4},/*day 6*/
	{5,3},/*day 7*/
	{4,2},/*day 8*/
	{3,1},/*day 9*/
	{3,0},/*day 10*/
	{2,0},/*day 11*/
	{2,7},/*day 12*/
	{1,6},/*day 13*/
	{1,5},/*day 14*/
	{0,4},/*day 15*/
	{7,3},/*day 16*/
	{7,2},/*day 17*/
	{6,1},/*day 18*/
	{6,0},/*day 19*/
	{5,0},/*day 20*/
	{5,7},/*day 21*/
	{4,6},/*day 22*/
	{3,5},/*day 23*/
	{3,4},/*day 24*/
	{2,3},/*day 25*/
	{2,2},/*day 26*/
	{1,1},/*day 27*/
	{1,0} /*day 28*/
};

C_0A33_1355(int minutes) {
	int i, di;
	int bp_14, bp_12, bp_10, isEclipse;
	int bp_0c, objTyp, bp_08, bp_06, bp_04;
	char *bp_02;

	/*time stop counter*/
	if(SpellFx[14]) {
		if(SpellFx[14] <= minutes) {
			D_ECC4->_0e = 1;
			SpellFx[14] = 0;
		} else {
			D_ECC4->_0e = 0;
			SpellFx[14] -= minutes;
			return;
		}
	}
	/*other spells*/
	for(i = 0; i < 16; i ++) {
		if(SpellFx[i]) {
			if(SpellFx[i] <= minutes)
				SpellFx[i] = 0;
			else
				SpellFx[i] -= minutes;
		}
	}
	/*powder keg*/
	if(D_2CA4 > minutes) {
		D_2CA4 -= minutes;
	} else if(D_2CA4) {
		D_2CA4 = 0;
		Detonate(D_2CA6);
	}
	/*possible eruption*/
	if(OSI_rand(0, 7) == 0)
		Erupting = 1;
	/* */
	bp_0c =
	bp_04 = 0;
	for(i = 0; i < 0x100; i++) {
		if(ObjShapeType[i] == 0)
			continue;

		if(IsPlrControl(i)) {
			bp_08 =
			bp_06 = 0;
			bp_02 = /*03D1*/"ring";
			for(di = FindInv(i); di >= 0; di = NextInv()) {
				if(GetCoordUse(di) == INVEN)
					continue;
				/*torch*/
				if(GetType(di) == OBJ_05A && GetFrame(di) == 1) {
					if(OSI_rand(0, 1)) {
						if(GetQuan(di) <= minutes) {
							CON_printf(BurnOutMsg);
							DeleteObj(di);
							PromptFlag = 1;
						} else {
							bp_0c = 1;
							SubQuan(di, minutes);
						}
					} else {
						bp_0c = 1;
					}
				} else
				/*invisibility ring*/
				if(GetType(di) == OBJ_102) {
					if(OSI_rand(0, 1000) == 0x1ad/*429*/) {
						bp_06 = 1;
						ClrInvisible(i);
						ScreenFade = 1;
						C_0A33_09CE(1);
					} else {
						bp_08 = 1;
					}
				} else
				/*regeneration ring*/
				if(GetType(di) == OBJ_101) {
					if(StatusDisplay == CMD_91 || (StatusDisplay == CMD_90 && Party[D_04B3] == i)) {
						if(HitPoints[i] < MaxHP(i))
							StatusDirty ++;
					}
					if(HitPoints[i] + minutes < MaxHP(i)) {
						HitPoints[i] += minutes;
					} else {
						HitPoints[i] = MaxHP(i);
					}
					if(OSI_rand(0, 1000) == 0x2de/*734*/)
						bp_06 = 1;
				} else
				/*storm cloak*/
				if(GetType(di) == OBJ_051) {
					if(OSI_rand(0, 1000) == 0x24c/*588*/) {
						bp_02 = /*03D6*/"cloak";
						bp_06 = 1;
						SpellFx[13] = 0;
					} else {
						bp_04 = 1;
					}
				}
				/* */
				if(bp_06) {
					CON_printf(RingVanishedMsg, bp_02);
					MUS_0065(12, 0);
					DeleteObj(di);
					PromptFlag = 1;
					bp_06 = 0;
				}
			}
		}

		for(bp_14 = 0; bp_14 < minutes; bp_14 ++) {
			if(IsInvisible(i) && !bp_08 && OSI_rand(0, 63) == 0 && GetInt(i) <= OSI_rand(1, 30))
				ClrInvisible(i);
			if(IsProtected(i) && OSI_rand(0, 63) == 0 && GetInt(i) <= OSI_rand(1, 30))
				ClrProtected(i);
			if(IsCursed(i) && OSI_rand(0, 15) == 0 && GetInt(i) >= OSI_rand(1, 30))
				ClrCursed(i);
			if(IsCharmed(i) && OSI_rand(0, 7) == 0 && GetInt(i) >= OSI_rand(1, 30))
				COMBAT_stopCharm(i);
			if(IsParalyzed(i) && OSI_rand(0, 3) == 0 && GetStr(i) >= OSI_rand(1, 30))
				ClrParalyzed(i);
			if(IsAsleep(i) && NPCMode[i] != AI_SLEEP && OSI_rand(0, 15) == 0) {
				ClrAsleep(i);
				SetSkipSomeTest(i);
				ObjShapeType[i] = OrigShapeType[i];
			}
			if(IsPoisoned(i) && !IsProtected(i) && OSI_rand(0, 7) == 0)
				LooseHP(i, 1);
		}

		C_0A33_0FEB(i);
		ClrSkipSomeTest(i);
	}

	if(bp_04)
		SpellFx[13] = 1;
	/* */
	for(bp_12 = Time_M + minutes; bp_12 > 59; ) {
		bp_12 -= 60;
		/*-- +1 hour --*/
		if(++Time_H > 23) {
			Time_H = 0;
			/*-- +1 day --*/
			if(++Date_D > 28) {
				Date_D = 1;
				/*-- +1 month*/
				if(++Date_M > 12) {
					Date_M = 1;
					/*-- +1 year*/
					Date_Y ++;
				}
			}
		}

		if(NextSleep)
			NextSleep --;
		if(DrunkCounter)
			DrunkCounter --;

		C_1E0F_5165();

		for(i = 0; i < PartySize; i ++) {
			objTyp = GetType(Party[i]);
			if(objTyp == OBJ_19A || objTyp == OBJ_17A || objTyp == OBJ_179 || objTyp == OBJ_182) {
				MAGIC[Party[i]] += Level[Party[i]];
				if(MaxMagic(Party[i]) < MAGIC[Party[i]])
					MAGIC[Party[i]] = MaxMagic(Party[i]);
				StatusDirty ++;
			}
		}
	}
	/*hourly updates?*/
	if(D_0369 != Time_H) {
		/*some flags*/
		if(D_0369 != (unsigned char)-1) {
			D_2C6E = /*unused?*/
			CaughtFish = 0;
			MustRingBell = 1;
			MUS_Clock = Time_H % 12;
			if(MUS_Clock == 0)
				MUS_Clock = 12;
		}

		D_0369 = Time_H;
		/*sundial*/
		if(Time_H > 20 || Time_H < 5)
			bp_10 = TIL_13C;
		else
			bp_10 = TIL_148 + ((Time_H - 5) >> 1);
		GR_4B(BaseTile[OBJ_0EB], bp_10);
		/*moons*/
		D_2CC6 = D_036A[Date_D - 1][0];
		D_2CC7 = (D_2CC6 * 3 + 18 - Time_H) % 24;
		D_2CC8 = D_036A[Date_D - 1][1];
		D_2CC9 = (D_2CC8 * 3 + 20 - Time_H) % 24;
		C_2FC1_19C5();
		C_0A33_121A();
	}
	/*wind*/
	C_0A33_12F6(0);
	/*ambiant light?*/
	Time_M = bp_12;
	bp_10 = D_2C55;
	isEclipse = ((Date_D == 1 && (Date_M % 3) == 0) || SpellFx[15]);
	if(isEclipse || !(Time_H >= 5 && Time_H <= 19) || (MapZ > 0 && MapZ < 5))
		D_2C55 = 0;
	else if(Time_H == 5)
		D_2C55 = Time_M / 10 + 1;
	else if(Time_H == 19)
		D_2C55 = (59 - Time_M) / 10 + 1;
	else
		D_2C55 = 7;
	if(D_2C55 < 4 && (bp_0c || SpellFx[0]))
		D_2C55 = 4;
	if(D_2C55 != bp_10)
		C_1100_0306();
	/*display time/wind infos*/
	CON_setClip(&(D_B6B5[0]));
	CON_gotoxy(2, 21);
	CON_printf(DateMsg, Date_M, Date_D, Date_Y, D_0357[WindDir + 1]);
	CON_setClip(&(D_B6B5[3]));
}

/*C_0A33_1AB7*/RefreshStatus() {
	if(StatusDirty) {
		if(D_2CC3 >= 0)
			D_04B3 = D_2CC3;
		switch(StatusDisplay) {
			case CMD_91: C_155D_000C(D_07CC); break;
			case CMD_90: C_155D_028A(D_04B3); break;
			case CMD_92: C_155D_1065(D_04B3); break;
			case CMD_9E: C_27A1_02D9(Party[D_04B3]); break;
		}
		C_0C9C_1AE5(1);
	}
	StatusDirty = 0;
}

/*C_0A33_1B36*/SetActive(int bp06) {
	if(Active == bp06)
		return;
	Active = bp06;
	MapX = GetX(Party[bp06]);
	MapY = GetY(Party[bp06]);
	MapZ = GetZ(Party[bp06]);
	C_101C_054C();
	D_0340 = 1;
	PromptFlag = 1;
}

/*C_0A33_1BB2*/SetPartyMode() {
	int si, di;

	if(D_2CC3 < 0)
		NPCMode[Party[0]] = D_2CC4;
	else
		NPCMode[Party[D_2CC3]] = D_2CC4;
	D_2CC4 = NPCMode[Party[0]];
	NPCMode[Party[0]] = AI_COMMAND;
	for(si = 1; si < PartySize; si ++) {
		di = Party[si];
		if(!IsCharmed(di))
			NPCMode[di] = AI_FOLLOW;
	}
	SetActive(0);
	D_2CC3 = 0xff;
	C_0C9C_1AE5(0);
	D_EBF3 = 0;
}

static /*C_0A33_1C4C*/__GetHex(int len) {
	int si, di;

	if(len > 4)
		len = 4;
	CON_gets(D_B628, len);
	di = 0;
	for(si = 0; si < len; si++) {
		if(D_B628[si] == 0)
			break;
		di <<= 4;
		if(D_B628[si] >= '0' && D_B628[si] <= '9')
			di += D_B628[si] - '0';
		else
			di += toupper(D_B628[si]) - ('A' - 10);
	}
	return di;
}

/*commands*/
static char *D_03A2[] = {
	/*03DC*/"Attack",
	/*03E3*/"Cast",
	/*03E8*/"Talk",
	/*03ED*/"Look",
	/*03F2*/"Get",
	/*03F6*/"Drop",
	/*03FB*/"Move",
	/*0400*/"Use",
	/*0404*/"Rest",
	/*0409*/"Save"
};

/*game loop*/
C_0A33_1CB4() {
	int ch, di;
	int goto_x, goto_y, goto_z, bp_02;

	CON_printf(/*040E*/"%s:\n%>", Names[Active]);
	do {
		RefreshStatus();

		if(MouseMode == 0)
			AllowMouseMov = 1;
		ch = CON_getch();
		AllowMouseMov = 0;

		/*keyboard commands*/
		switch(ch) {
			case 'A'/*ttack*/:
				ch = CMD_81;
				C_0C9C_1AE5(0);
				D_0497 = 0;
				SelectMode = 1;
				AimX =
				AimY = 5;
				SelectRange = 7;
				if(D_04A7 >= 0) {
					AimX = (GetX(D_04A7) & 0xff) - MapX + 5;
					AimY = (GetY(D_04A7) & 0xff) - MapY + 5;
					if(AimX >= 0 && AimX <= 10 && AimY >= 0 && AimY <= 10)
						break;
					AimX =
					AimY = 5;
				}
			break;
			case 'C'/*ast*/:
				ch = CMD_82;
				C_0C9C_1AE5(0);
				D_0497 = 0;
				SelectMode = 3;
				AimX =
				AimY = 5;
				SelectRange = 7;
			break;
			case 'T'/*alk*/:
				ch = CMD_83;
				C_0C9C_1AE5(0);
				D_0497 = 0;
				SelectMode = 1;
				AimX =
				AimY = 5;
				SelectRange = 7;
			break;
			case 'L'/*ook*/:
				ch = CMD_84;
				C_0C9C_1AE5(0);
				D_0497 = 0;
				SelectMode = 1;
				AimX =
				AimY = 5;
				SelectRange = 7;
			break;
			case 'G'/*et*/:
				ch = CMD_85;
				C_0C9C_1AE5(0);
				D_0497 = 0;
				SelectMode = 1;
				AimX =
				AimY = 5;
				SelectRange = -1;
			break;
			case 'D'/*rop*/:
				ch = CMD_86;
				C_0C9C_1AE5(0);
				D_0497 = 0;
				SelectMode = 2;
				AimX =
				AimY = 5;
				SelectRange = 7;
				D_0499 = 3;
				D_049A = 0;
				if(!IN_VEHICLE && (StatusDisplay != CMD_92 || Active != D_04B3)) {
					StatusDisplay = CMD_92;
					D_04B3 = Active;
					StatusDirty = 1;
					RefreshStatus();
				}
				C_0C9C_1AE5(1);
			break;
			case 'M'/*ove*/:
				ch = CMD_87;
				C_0C9C_1AE5(0);
				D_0497 = 0;
				SelectMode = 1;
				AimX =
				AimY = 5;
				SelectRange = -1;
			break;
			case 'U'/*se*/:
				ch = CMD_88;
				C_0C9C_1AE5(0);
				D_0497 = 0;
				SelectMode = 1;
				AimX =
				AimY = 5;
				SelectRange = -1;
			break;
			case 'R'/*est*/:
				ch = CMD_89;
				C_0C9C_1AE5(0);
				D_0497 = 0;
				SelectMode = 1;
				AimX =
				AimY = 5;
				SelectRange = 7;
			break;
			case 19:/*<CTRL+S>*/
				ch = CMD_AA;
			break;
			case 'B'/*egin/break combat*/:
				ch = CMD_8A;
			break;
		}

		/*character switch*/
		if(ch >= '0' && ch <= '9' && ch < PartySize + '1') {
			if(ch == '0') {/*Party Mode*/
				if(IN_VEHICLE) {
					CON_printf(NotOnShipMsg);
					goto C_262D;
				}
				if(InCombat) {
					CON_printf(NotInCombatMsg);
					goto C_262D;
				}
				for(bp_02 = 0; bp_02 < PartySize; bp_02 ++) {
					if(!CLOSE_ENOUGH(8, MapX, MapY, GetX(Party[bp_02]), GetY(Party[bp_02]))) {
						CON_printf(NotAllHereMsg);
						break;
					}
				}
				if(PartySize != bp_02) {
					goto C_262D;
				}
				CON_printf(PartyModeMsg);
				SetPartyMode();
				MouseMode = 0;
			} else {/*Solo Mode*/
				if(Isbis_0016(Party[ch - '1']) || IsDraggedUnder(Party[ch - '1']) || IsCharmed(Party[ch - '1'])) {
					CON_printf(D_356A_00DF, Names[ch - '1']);
					goto C_262D;
				}
				if(IN_VEHICLE) {
					CON_printf(NotOnShipMsg);
					goto C_262D;
				}
				if(InCombat) {
					CON_printf(NotInCombatMsg);
					goto C_262D;
				}
				if(D_2CC3 < 0)
					NPCMode[Party[Active]] = D_2CC4;
				else
					NPCMode[Party[D_2CC3]] = D_2CC4;
				D_2CC4 = NPCMode[Party[ch - '1']];
				for(bp_02 = 0; bp_02 < PartySize; bp_02 ++) {
					if(NPCMode[Party[bp_02]] == AI_FOLLOW)
						NPCMode[Party[bp_02]] = AI_MOTIONLESS;
				}
				SetActive(ch - '1');
				NPCMode[Party[Active]] = AI_COMMAND;
				D_2CC3 = Active;
				if(StatusDisplay != CMD_91)
					StatusDirty = 1;
				CON_printf(SoloModeMsg);
				MouseMode = 0;
				C_0C9C_1AE5(0);
			}
			SelectMode = 0;
			D_04C2 = CMD_8F;
			C_0C9C_01FB();
			goto C_262D;
		}

		switch(ch) {
			case CMD_80:/*[advance]*/
				C_1E0F_1B0E(AdvanceDir);
			break;
			case 5: case 17: case 0x12D:/*<CTRL+E> <CTRL+Q> <ALT+X> --"exit to dos?"*/
				CON_printf(ExitToDOSMsg);
				while((di = CON_getch()) != 'Y' && di != 'N');
				CON_printf(/*0415*/"%c\n", di);
				if(di == 'N')
					ch = 0;
				else
					ch = 0x12D;
			break;
			case CMD_81:/*"attack"*/
				COMBAT_attack();
				D_04C2 = CMD_8F;
			break;
			case CMD_8A:/*begin/break combat*/
				if(D_2CC3 >= 0) {
					CON_printf(NotInSoloMsg);
				} else if(InCombat) {/*break off*/
					CON_printf(BreakOffMsg);
					COMBAT_breakOff();
				} else if(IN_VEHICLE) {
					CON_printf(NotOnShipMsg);
				} else {
					CON_printf(CombatMsg);
					COMBAT_begin();
				}
			break;
			case CMD_82:
			case CMD_85:
			case CMD_86:
			case CMD_87:
				if(ch == CMD_82) {
					D_04B6 =
					D_04B5 = 0;
				}
				if(IN_VEHICLE) {
					CON_printf(/*0419*/"%s-", D_03A2[ch - CMD_81]);
					CON_printf(NotOnShipMsg);
					break;
				}
			/*break;*/
			case CMD_83:
			case CMD_84:
			case CMD_88:
				MouseMode = 1;
				D_04C2 = ch;
				C_0C9C_01FB();
				CON_printf(/*0419*/"%s-", D_03A2[ch - CMD_81]);
				if(ch == CMD_82) {
					C_1944_4C2F();/*"cast"?*/
				} else {
					di = CON_getch();
					if(di == CMD_8E) {
						switch(D_04C2) {
							case CMD_84: C_27A1_0C67(); break;/*"look"*/
							case CMD_86: C_27A1_14DA(); break;/*"drop"*/
							case CMD_85: C_27A1_18F5(); break;/*"get"*/
							case CMD_87: C_27A1_1E8B(); break;/*"move"*/
							case CMD_88: C_27A1_6179(); break;/*"use"*/
							case CMD_83:/*"talk"*/
								TALK_talkTo(Active, Selection.obj, 1);
								MUS_09A8();
							break;
						}
					} else {
						CON_printf(WhatMsg);
					}
				}
				MouseMode = 0;

				D_04C2 = CMD_8F;
				C_0C9C_1AE5(0);
				SelectMode = 0;
				C_0C9C_01FB();
			break;
			case CMD_89:/*"rest"*/
				if(!IN_VEHICLE)
					MUS_091A(8);
				C_3200_055D();
				if(!IN_VEHICLE)
					MUS_09A8();
				MouseMode = 0;
				C_0C9C_1AE5(0);
				SelectMode = 0;
				C_0C9C_01FB();
			break;
			case CMD_AA:/*"save game"*/
				if(IsArmageddon)
					break;
				CON_printf(SaveMsg);
				while((di = CON_getch()) != 'Y' && di != 'N');
				CON_printf(/*0415*/"%c\n", di);
				if(di == 'Y') {
					C_0C9C_089F();
					C_0C9C_1AE5(0);
					MouseMode = 0;
					SelectMode = 0;
					C_0C9C_01FB();
				}
			break;
			case 18:/*<CTRL+R> "restore game"*/
				CON_printf(/*041D*/"Restore game? ");
				while((di = CON_getch()) != 'Y' && di != 'N');
				CON_printf(/*0415*/"%c\n", di);
				if(di == 'Y')
					C_0C9C_0397();
			break;
			case 27: case ' ': case CMD_8F:/*"pass"*/
				CON_printf(D_356A_0121);
				MovePts[Party[Active]] = 0;
				C_0C9C_1AE5(0);
				MouseMode = 0;
				SelectMode = 0;
				C_0C9C_01FB();
				if(
					(GetType(Party[Active]) == OBJ_19F || GetType(Party[Active]) == OBJ_1A7) &&
					WindDir >= 0
				) C_1E0F_1B0E(WindDir);
				MoveFollowers(Party[Active], 1);
			break;
			case 26:/*<CTRL+Z>*/
				if(SoundFlag) {
					MUS_091A(0);
					CON_printf(SoundOffMsg);
					SoundFlag = 0;
				} else {
					SoundFlag = 1;
					MUS_09A8();
					CON_printf(SoundOnMsg);
				}
			break;
			case 8:/*<CTRL+H>*/
				CON_printf(/*042C*/"Help ");
				D_04A1 = !D_04A1;
				CON_printf(D_04A1?/*0432*/"on":/*0435*/"off");
				CON_printf(/*0439*/"!\n");
			break;
			case 0x119:/*<ALT+P> palette related*/
				C_0903_0776();
			break;
			case 22:/*<CTRL+V> version string*/
				CON_printf(VersionMsg);
			break;
			case 213:/*<alt 2 1 3> peer+info*/
				CON_printf(/*043C*/"%02d%02d%02d%03x%03x%1x\n",
					KARMA, Time_H, Time_M, MapX, MapY, MapZ);
				C_2FC1_1FD6();/*peer?*/
			break;
			case 214:/*<alt 2 1 4> go to coordinate*/
				if(IN_VEHICLE) {
					CON_printf(/*0455*/"<nat uail abord wip!>\n");
					break;
				}
				CON_printf(/*046C*/"<gotu eks>: ");
				goto_x = __GetHex(3);
				CON_printf(/*0479*/"\n<uai>: ");
				goto_y = __GetHex(3);
				CON_printf(/*0482*/"\n<zi>: ");
				goto_z = __GetHex(1);
				if(goto_z) {
					goto_x &= 0xff;
					goto_y &= 0xff;
				} else {
					goto_x &= 0x3ff;
					goto_y &= 0x3ff;
				}
				if(goto_z > 5)
					goto_z = 5;
				PartyTeleport(goto_x, goto_y, goto_z, 0);
				MUS_09A8();
			break;
			case 215:/*<alt 2 1 5>time += 60 minutes*/
				C_0A33_1355(60);
				AllowNPCTeleport = 1;
				C_1E0F_464A();
				AllowNPCTeleport = 0;
			break;
			default:
				CON_printf(WhatMsg);
				C_0C9C_1AE5(0);
				MouseMode = 0;
				SelectMode = 0;
				C_0C9C_01FB();
		}

C_262D:
		if(ch != 0x12D) {
			if(D_2E9E == 'R')
				C_0C9C_0397();/*Restore game?*/
			else if(D_2E9E == 'G')
				C_0C9C_089F();/*Save game?*/
			C_1E0F_4E0A();
			C_0A33_117A();
			if(ch != CMD_80)
				PromptFlag = 1;
			if(PromptFlag) {
				CON_printf(/*048A*/"\n%s:\n%>", Names[Active]);
				PromptFlag = 0;
			}
		}
	} while(ch != 0x12D);
}
