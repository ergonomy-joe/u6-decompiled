/*
 * Ultima VI - The False Prophet
 * Copyright (c) 1990 Origin Systems, Inc. and Richard Gariott
 * reverse-coded by Ergonomy Joe in 2013
 */
/*
	creature generation "EGGs" module
*/
#include "u6.h"

#undef abs
#include <math.h>

/*28A0*/int ForceHatching = 0;
static unsigned char D_28A2 = 0;

__2E2D_0003(int bp0c, int bp0a, int bp08, int bp06) {
	int delta_x, delta_y;

	delta_x = abs(bp08 - bp0c);
	delta_y = abs(bp06 - bp0a);
	if(delta_x > 7 || delta_y > 7)
		return 8;
	return D_196C[delta_x][delta_y];
}

/*C_2E2D_004B*/GetMonsterClass(int objType) {
	int si, di;

	di = -1;
	for(si = 0; D_3522_0000[si]; si ++) {
		if(D_3522_0000[si] == objType) {
			di = si;
			break;
		}
	}
	return di;
}

static /*C_2E2D_0088*/mkRandom(int val) {
	int half = val >> 1;
	if(half)
		val = OSI_rand(0, half) + (val - half) + OSI_rand(0, half);
	return val;
}

static unsigned D_28A3[] = {OBJ_081,OBJ_085,OBJ_080};

/*generate monster posessions?*/
static C_2E2D_00BE(int objNum, int objClass) {
	int i, j;
	int objNum_1, numWeapons;
	int objNum_2;
	unsigned char valu8;
	unsigned char far *ptr;

	ptr = &(D_3522_0342[D_3522_02C2[objClass]]);
	/*spells*/
	while(valu8 = *ptr++) {
		objNum_1 = AddInvObj(OBJ_150, LOCAL | EQUIP, QualQuan(valu8, 1), objNum);
		SetInvisible(objNum_1);
	}
	/*weapons*/
	for(i = 2; valu8 = *ptr++; i <<= 1) {
		if(OSI_rand(1, i) == 1) {
			if(valu8  == OBJ_024 || valu8 == OBJ_025 || valu8 == OBJ_026)
				numWeapons = OSI_rand(6, 12);
			else
				numWeapons = 1;
			for(j = 0; j < numWeapons; j ++) {
				objNum_1 = AddInvObj(valu8, LOCAL | EQUIP | OWNED, 0, objNum);
				if(objNum_1 >= 0) {
					if(valu8 == OBJ_029 || valu8 == OBJ_036)
						AddInvObj(OBJ_037, LOCAL | INVEN | OWNED, OSI_rand(12, 24), objNum);
					if(valu8 == OBJ_02A || valu8 == OBJ_032)
						AddInvObj(OBJ_038, LOCAL | INVEN | OWNED, OSI_rand(12, 24), objNum);
				}
			}
		}
	}
	/*armors*/
	for(i = 2; valu8 = *ptr++; i <<= 1) {
		if(OSI_rand(1, i) == 1)
			AddInvObj(valu8, LOCAL | EQUIP | OWNED, 1, objNum);
	}
	/*treasures*/
	for(i = 2; valu8 = *ptr++; i <<= 1) {
		if(OSI_rand(1, i) == 1) {
			if(valu8 == OBJ_062) {
				objNum_2 = AddInvObj(valu8, LOCAL | CONTAINED | OWNED, OSI_rand(1, 100), objNum);
				if(objNum_2 >= 0) {
					SetFrame(objNum_2, 1);
					if(OSI_rand(0, 1) != 0 && !IsMonster_AC(objClass))
						AddInvObj(OBJ_151, LOCAL | CONTAINED | OWNED, QualQuan(SPELL_16, 0), objNum_2);
					if(OSI_rand(0, 3) != 0)
						AddInvObj(OBJ_058, LOCAL | CONTAINED | OWNED, OSI_rand(1, 100), objNum_2);
					if(OSI_rand(0, 3) != 0)
						AddInvObj(D_28A3[OSI_rand(0, 2)], LOCAL | CONTAINED | OWNED, OSI_rand(1, 10), objNum_2);
					if(OSI_rand(0, 3) == 0)
						AddInvObj(OBJ_05A, LOCAL | CONTAINED | OWNED, OSI_rand(1, 6), objNum_2);
					if(OSI_rand(0, 10) == 0)
						AddInvObj(OBJ_04D, LOCAL | CONTAINED | OWNED, OSI_rand(1, 4), objNum_2);
				}
			} else if(valu8 == OBJ_03A) {
				if(OSI_rand(0, 3) != 0 && !IsMonster_13(objClass))
					AddInvObj(OBJ_058, LOCAL | INVEN | OWNED, OSI_rand(1, 100), objNum);
				if(OSI_rand(0, 3) == 0)
					AddInvObj(D_28A3[OSI_rand(0, 2)], LOCAL | INVEN | OWNED, OSI_rand(1, 10), objNum);
				if(OSI_rand(0, 3) == 0)
					AddInvObj(TypeFrame(OBJ_113, OSI_rand(0, 7)), LOCAL | INVEN | OWNED, 1, objNum);
			} else if(valu8 == OBJ_058) {
				AddInvObj(valu8, LOCAL | INVEN | OWNED, OSI_rand(1, 30) + OSI_rand(1, 30), objNum);
			} else {
				AddInvObj(valu8, LOCAL | INVEN | OWNED, 1, objNum);
			}
		}
	}
}

/*C_2E2D_0499*/EGG_generate(int objType, int x, int y, int z) {
	int objNum, di;
	int objClass, bp_04, isMutant;

	isMutant = 0;
	if(objType & 0x8000) {
		objType &= 0x7fff;
		isMutant = 1;
	}
	objNum = AddMonster(objType, x, y, z);
	if(objNum < 0)
		return objNum;
	objClass = GetMonsterClass(objType);
	if(objClass >= 0) {
		STREN[objNum] = mkRandom(D_3522_0082[objClass]);
		DEXTE[objNum] = mkRandom(D_3522_00C2[objClass]);
		INTEL[objNum] = mkRandom(D_3522_0102[objClass]);
		HitPoints[objNum] = mkRandom(D_3522_01C2[objClass]);
		Level[objNum] = (HitPoints[objNum] + 29) / 30;
		NPCStatus[objNum] = D_3522_0202[objClass];
	} else {
		STREN[objNum] =
		DEXTE[objNum] =
		INTEL[objNum] = 10;
		HitPoints[objNum] = 30;
		Level[objNum] = 1;
		NPCStatus[objNum] = EVIL;
	}
	MAGIC[objNum] = MaxMagic(objNum);
	MovePts[objNum] = DEXTE[objNum];
	ExpPoints[objNum] = 100;
	NPCMode[objNum] =
	NPCComMode[objNum] = AI_ASSAULT;

	if(objType == OBJ_16A || objType == OBJ_1A8) {
		SetFrame(objNum, 0x13);
	} else if(objType >= OBJ_1AA) {
		SetFrame(objNum, 6);
		if(isMutant)
			di = AddMapObj(TypeFrame(objType, 2), 1, 0, x + 1, y, z);
		else
			di = AddMapObj(TypeFrame(objType, 0xe), 1, 0, x + 1, y, z);
		if(di >= 0) 
			ClrLocal(di);
	} else if(objType == OBJ_19B) {
		/*body*/
		SetFrame(objNum, 0);
		/*head*/
		di = AddMapObj(TypeFrame(OBJ_19B, 8), 1, 0, x, y - 1, z);
		if(di >= 0)
			ClrLocal(di);
		/*tail*/
		di = AddMapObj(TypeFrame(OBJ_19B, 0x10), 1, 0, x, y + 1, z);
		if(di >= 0) 
			ClrLocal(di);
		/*left wing*/
		di = AddMapObj(TypeFrame(OBJ_19B, 0x18), 1, 0, x - 1, y, z);
		if(di >= 0) 
			ClrLocal(di);
		/*right wing*/
		di = AddMapObj(TypeFrame(OBJ_19B, 0x20), 1, 0, x + 1, y, z);
		if(di >= 0) 
			ClrLocal(di);
	} else if(objType == OBJ_176) {
		for(bp_04 = 0; bp_04 < 8; bp_04 ++) {
			di = AddMapObj(OBJ_1A9 + (bp_04 << 12), 1, 0, x + DirIncrX[bp_04], y + DirIncrY[bp_04], z);
			if(di >= 0) 
				ClrLocal(di);
		}
	}

	C_2E2D_00BE(objNum, objClass);

	return objNum;
}

/*silver serpent related*/
static char D_28A9[] = {0xD,0xA,0xB,0xC};/*curves*/
static char D_28AD[] = {0x1,0x3,0x5,0x7};/*tails*/
static char D_28B1[] = {0, -1, -1, 0};
static char D_28B5[] = {1,  1,  0, 0};
/*__28B9	db 0*/

/*C_2E2D_0760*/EGG_hatches(int egg_objNum) {
	int si;
	int bp_24, bp_22, bp_20, objTyp, typParam, num_monsters;
	int bp_18, bp_16, x, y, embryo_objNum, align_l5;
	int bp_0c, bp_0a, isFirstBorn, npcMod, forceGarglGraze, onScreenNotOk;
	
	bp_16 = 0;
	npcMod = AI_MOTIONLESS;
	forceGarglGraze = 0;

	if(IsArmageddon)
		return;
	/*Check DAY egg*/
	if(
		(GetQual(egg_objNum) / 10 == 1/*DAY*/) &&
		!(Time_H >= 6 && Time_H <= 18)
	) return;
	/*Check NIGHT egg*/
	if(
		(GetQual(egg_objNum) / 10 == 2/*NIGHT*/) &&
		(Time_H > 5 && Time_H < 19)
	) return;
	/*???*/
	if(IsLocal(egg_objNum))
		onScreenNotOk = 0;
	else
		onScreenNotOk = !ForceHatching;
	/*Gargoyles/amulet of submission*/
	for(bp_24 = 0; bp_24 < PartySize; bp_24 ++) {
		bp_16 = GetType(Party[bp_24]);
		if(bp_16 == OBJ_16B || bp_16 == OBJ_16A) {
			forceGarglGraze = 1;
			break;
		}
		if(FindInvType(1, OBJ_04C, -1) >= 0)
			forceGarglGraze = 1;
	}
	/* */
	if(!IsHatched(egg_objNum)) {
		/*Quality of egg: alignment of monsters*/
		align_l5 = ((GetQual(egg_objNum) % 10) - 1) << 5;
		/*Quantity of egg: percent chance of hatching*/
		if(OSI_rand(1, 100) <= GetQuan(egg_objNum)) {
			bp_0a = 0;
			isFirstBorn = 1;
			/*Contents of egg: "embryos"*/
			for(embryo_objNum = FindInv(egg_objNum); embryo_objNum >= 0; embryo_objNum = NextInv()) {
				objTyp =
				typParam = GetType(embryo_objNum);
				/*two-headed?*/
				if(IsMutant(embryo_objNum))
					typParam |= 0x8000;
				/*Quantity of embryo: number of monsters in party*/
				num_monsters = GetQuan(embryo_objNum);
				if(GetQuan(egg_objNum) != 100)
					num_monsters = OSI_rand(1, num_monsters);
				if(objTyp == OBJ_19D)
					num_monsters = 1;
				/* */
				if((objTyp == OBJ_16B || objTyp == OBJ_16A) && forceGarglGraze)
					npcMod = AI_GRAZE;
				/* */
				for(bp_24 = 0; bp_24 < num_monsters; bp_24 ++) {
					if(objTyp < OBJ_156 && objTyp != OBJ_062) {
						if(QuanType(objTyp))
							bp_20 = 1;
						else
							bp_20 = 0;
						si = AddMapObj(objTyp, 1, bp_20, GetX(egg_objNum), GetY(egg_objNum), MapZ);
					} else {
						si = EGG_generate(typParam, GetX(egg_objNum), GetY(egg_objNum), MapZ);
						if(si >= 0) {
							if(GetQual(egg_objNum) % 10)
								SetAlignment(si, align_l5);
							if(Is_ATKPLR(si) && !IsLocal(egg_objNum))
								bp_0a = 1;
						}
					}
					if(si < 0)
						continue;
					/**/
					if(!isFirstBorn) {
						if(!COMBAT_TryTeleport(si, onScreenNotOk))
							DeleteObj(si);
					} else {
						isFirstBorn = 0;
					}
					/* */
					if(objTyp == OBJ_19D) {/*Silver serpent*/
						D_28A2 = 0;
						SetFrame(si, 0);/*head*/
						SetQual(si, D_28A2 ++);
						x = GetX(si);
						y = GetY(si);
						for(bp_22 = 0; bp_22 <= GetQuan(embryo_objNum); bp_22 ++) {
							bp_18 = AddMapObj(
								TypeFrame(OBJ_19D, D_28A9[bp_22 & 3]),
								1,
								QualQuan(D_28A2 ++, si),
								D_28B1[bp_22 & 3] + x, D_28B5[bp_22 & 3] + y, MapZ
							);
							if(bp_18 < 0) {
								if(bp_22)
									SetFrame(bp_16, D_28AD[(bp_22 - 1) & 3]);
								else
									DeleteObj(si);
								break;
							}
							ClrLocal(bp_18);
							bp_16 = bp_18;
							if(GetQuan(embryo_objNum) == bp_22)
								SetFrame(bp_18, D_28AD[bp_22 & 3]);
						}
					} else if(objTyp == OBJ_16D) {/*Tangle vine(vine)*/
						SetFrame(si, 0);
						x = GetX(si);
						y = GetY(si);
						for(bp_22 = 0; bp_22 < 8; bp_22 += 2) {
							bp_18 = EGG_generate(OBJ_16E, DirIncrX[bp_22] + x, DirIncrY[bp_22] + y, MapZ);
							if(bp_18 < 0)
								continue;
							SetDirection(bp_18, bp_22);
							SetFrame(bp_18, (bp_22 & 2)?0:1);
							SetAlignment(bp_18, GetAlignment(si));
							NPCComMode[bp_18] = 
							NPCMode[bp_18] = AI_TANGLE;
						}
					}
					if(si >= 0x100)
						continue;
					/*Quality of embryo: combat strategy for monsters*/
					if(npcMod == AI_MOTIONLESS)
						npcMod = GetQual(embryo_objNum);
					NPCComMode[si] = 
					NPCMode[si] = npcMod;

					if(npcMod == AI_0F)
						SetDirection(si, 2);
					else if(npcMod == AI_10)
						SetDirection(si, 0);

					if((objTyp == OBJ_16B || objTyp == OBJ_16A) && NPCMode[si] == AI_BERSEK) {
						STREN[si] <<= 1;
						DEXTE[si] <<= 1;
						INTEL[si] <<= 1;
					}
				}
			}
			if(!D_17B6 && bp_0a) {
				x = GetX(egg_objNum) - MapX;
				y = GetY(egg_objNum) - MapY;
				bp_0c = MkDirection((x << 4) + D_04CA, (y << 4) + D_04CB);
				if(OSI_rand(0, 3) && SHAMINO_COMMENT) {
					CON_printf(D_356A_0128, DirNames[bp_0c]);
					PromptFlag = 1;
				}
				D_17B6 = 1;
			}
		}
	}
	SetHatched(egg_objNum);
	SetInvisible(egg_objNum);
}

/*C_2E2D_0DFE*/EGG_hatchArea() {
	int objNum, x, y, bp_02;

	if(IsArmageddon)
		return;
	D_28A2 = 0;
	x = AreaX;
	y = AreaY;
	for(objNum = SearchArea(x, y, x + 39, y + 39); objNum >= 0; objNum = NextArea()) {
		x = GetX(objNum);
		y = GetY(objNum);
		bp_02 = GetType(objNum);
		if(bp_02 != OBJ_14F || GetZ(objNum) != MapZ)
			continue;
		if(ForceHatching || !CLOSE_ENOUGH(8, x, y, MapX, MapY) || IsLocal(objNum))
			EGG_hatches(objNum);
	}
}
