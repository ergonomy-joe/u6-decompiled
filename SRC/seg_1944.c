/*
 * Ultima VI - The False Prophet
 * Copyright (c) 1990 Origin Systems, Inc. and Richard Gariott
 * reverse-coded by Ergonomy Joe in 2013
 */
/*
	module spell/magic
*/
#include "u6.h"

#undef abs
#include <math.h>

/*0AF0*/unsigned char *SpellCode[] = {
	/*1st circle*/
	/*0EF8*/"IMY",
	/*0EFC*/"WO",
	/*0EFF*/"WJ",
	/*0F02*/"AJO",
	/*0F06*/"AF",
	/*0F09*/"AM",
	/*0F0C*/"IM",
	/*0F0F*/"KL",
	/*0F12*/"IF",
	/*0F15*/"IL",
	"","","","","","",
	/*2nd circle*/
	/*0F18*/"QL",
	/*0F1B*/"OJ",
	/*0F1E*/"INP",
	/*0F22*/"IY",
	/*0F25*/"IZ",
	/*0F28*/"OPY",
	/*0F2C*/"IJ",
	/*0F2F*/"EP",
	/*0F32*/"AJ",
	/*0F35*/"AY",
	"","","","","","",
	/*3rd circle*/
	/*0F38*/"AS",
	/*0F3B*/"AG",
	/*0F3E*/"PF",
	/*0F41*/"VL",
	/*0F44*/"AP",
	/*0F47*/"AVZ",
	/*0F47+1*/"VZ",
	/*0F4B*/"VWY",
	/*0F4F*/"IS",
	/*0F52*/"AXC",
	"","","","","","",
	/*4th circle*/
	/*0F56*/"OY",
	/*0F59*/"KX",
	/*0F5C*/"AVM",
	/*0F60*/"IFG",
	/*0F5C+1*/"VM",
	/*0F64*/"IW",
	/*0F67*/"VAJO",
	/*0F6C*/"ING",
	/*0F70*/"IZG",
	/*0F74*/"RH",
	"","","","","","",
	/*5th circle*/
	/*0F77*/"ISG",
	/*0F7B*/"VPF",
	/*0F7F*/"KBX",
	/*0F83*/"SL",
	/*0F86*/"OG",
	/*0F89*/"AXP",
	/*0F28+1*/"PY",
	/*0F8D*/"ASL",
	/*0F91*/"KMC",
	/*0F4B+1*/"WY",
	"","","","","","",
	/*6th circle*/
	/*0F95*/"AXE",
	/*0F99*/"IQX",
	/*0F9D*/"VQ",
	/*0FA0*/"FH",
	/*0FA3*/"KDY",
	/*0FA7*/"VIS",
	/*0FAB*/"AO",
	/*0FAE*/"NH",
	/*0FB1*/"IQY",
	/*0FB5*/"IDP",
	"","","","","","",
	/*7th circle*/
	/*0FB9*/"VOG",
	/*0FBD*/"IOY",
	/*0FC1*/"GH",
	/*0FC4*/"QC",
	/*0FC7*/"VRP",
	/*0FCB*/"IC",
	/*0FCE*/"VAS",
	/*0FD2*/"VSL",
	/*0FD6*/"KOX",
	/*0FDA*/"POW",
	"","","","","","",
	/*8th circle*/
	/*0FDE*/"VCBM",
	/*0FE3*/"CH",
	/*0FE6*/"VAL",
	/*0FEA*/"VAXE",
	/*0FEF*/"VC",
	/*0FF2*/"IMC",
	/*0FF6*/"VRX",
	/*0FFA*/"KXC",
	/*0FFE*/"AT",
	/*1001*/"VPY",
	"","","","","",""
};

/*Words of power*/
unsigned char *D_0BF0[] = {
	/*1005*/"An",
	/*1008*/"Bet",
	/*100C*/"Corp",
	/*1011*/"Des",
	/*1015*/"Ex",
	/*1018*/"Flam",
	/*101D*/"Grav",
	/*1022*/"Hur",
	/*1026*/"In",
	/*1029*/"Jux",
	/*102D*/"Kal",
	/*1031*/"Lor",
	/*1035*/"Mani",
	/*103A*/"Nox",
	/*103E*/"Ort",
	/*1042*/"Por",
	/*1046*/"Quas",
	/*104B*/"Rel",
	/*104F*/"Sanct",
	/*1055*/"Tym",
	/*1059*/"Uus",
	/*105D*/"Vas",
	/*1061*/"Wis",
	/*1065*/"Xen",
	/*1069*/"Ylem",
	/*106E*/"Zu"
};

/*0C24*/unsigned char *SpellName[] = {
	/*1st circle*/
	/*1071*/"Create Food",
	/*107D*/"Detect Magic",
	/*108A*/"Detect Trap",
	/*1096*/"Dispel Magic",
	/*10A3*/"Douse",
	/*10A9*/"Harm",
	/*10AE*/"Heal",
	/*10B3*/"Help",
	/*10B8*/"Ignite",
	/*10BF*/"Light",
	"","","","","","",
	/*2nd circle*/
	/*10C5*/"Infravision",
	/*10D1*/"Magic Arrow",
	/*10DD*/"Poison",
	/*10E4*/"Reappear",
	/*10ED*/"Sleep",
	/*10F3*/"Telekinesis",
	/*108A+6*/"Trap",
	/*10FF*/"Unlock Magic",
	/*110C*/"Untrap",
	/*1113*/"Vanish",
	"","","","","","",
	/*3rd circle*/
	/*111A*/"Curse",
	/*1120*/"Dispel Field",
	/*112D*/"Fireball",
	/*1136*/"Great Light",
	/*1142*/"Lock",
	/*1147*/"Mass Awaken",
	/*1153*/"Mass Sleep",
	/*115E*/"Peer",
	/*1163*/"Protection",
	/*116E*/"Repel Undead",
	"","","","","","",
	/*4th circle*/
	/*117B*/"Animate",
	/*1183*/"Conjure",
	/*118B*/"Disable",
	/*1193*/"Fire Field",
	/*119E*/"Great Heal",
	/*11A9*/"Locate",
	/*11B0*/"Mass Dispel",
	/*11BC*/"Poison Field",
	/*11C9*/"Sleep Field",
	/*11D5*/"Wind Change",
	"","","","","","",
	/*5th circle*/
	/*11E1*/"Energy Field",
	/*11EE*/"Explosion",
	/*11F8*/"Insect Swarm",
	/*1205*/"Invisibility",
	/*1212*/"Lightning",
	/*121C*/"Paralyze",
	/*1225*/"Pickpocket",
	/*1230*/"Reveal",
	/*1237*/"Seance",
	/*123E*/"X-ray",
	"","","","","","",
	/*6th circle*/
	/*1244*/"Charm",
	/*124A*/"Clone",
	/*1250*/"Confuse",
	/*1258*/"Flame Wind",
	/*1263*/"Hail Storm",
	/*126E*/"Mass Protect",
	/*127B*/"Negate Magic",
	/*1288*/"Poison Wind",
	/*1294*/"Replicate",
	/*129E*/"Web",
	"","","","","","",
	/*7th circle*/
	/*12A2*/"Chain Bolt",
	/*12AD*/"Enchant",
	/*12B5*/"Energy Wind",
	/*12C1*/"Fear",
	/*12C6*/"Gate Travel",
	/*12D2*/"Kill",
	/*12D7*/"Mass Curse",
	/*12E2*/"Mass Invis",
	/*12ED*/"Wing Strike",
	/*12F9*/"Wizard Eye",
	"","","","","","",
	/*8th circle*/
	/*1304*/"Armageddon",
	/*130F*/"Death Wind",
	/*131A*/"Eclipse",
	/*1322*/"Mass Charm",
	/*132D*/"Mass Kill",
	/*1337*/"Resurrect",
	/*1341*/"Slime",
	/*1347*/"Summon",
	/*134E*/"Time Stop",
	/*1358*/"Tremor",
	"","","","","","",
};

/*0D24*/char *Reagents_name[] = {
	/*135F*/"mandrake root",
	/*136D*/"nightshade",
	/*1378*/"black pearl",
	/*1384*/"blood moss",
	/*138F*/"spider silk",
	/*139B*/"garlic",
	/*13A2*/"ginseng",
	/*13AA*/"sulfurous ash"
};

/*0D34*/int ReagType[] = {OBJ_045,OBJ_046,OBJ_041,OBJ_042,OBJ_047,OBJ_043,OBJ_044,OBJ_048};

/*0D44*/unsigned char Reagents_needed[] = {
	/*1st circle*/
	REAGENT_GS|REAGENT_GA|REAGENT_MR,
	REAGENT_SA|REAGENT_NS,
	REAGENT_SA|REAGENT_NS,
	REAGENT_GS|REAGENT_GA,
	REAGENT_GA|REAGENT_BP,
	REAGENT_SS|REAGENT_NS,
	REAGENT_GS|REAGENT_SS,
	0,
	REAGENT_SA|REAGENT_BP,
	REAGENT_SA,
	0,0,0,0,0,0,
	/*2nd circle*/
	REAGENT_SA|REAGENT_NS,
	REAGENT_SA|REAGENT_BP,
	REAGENT_BM|REAGENT_BP|REAGENT_NS,
	REAGENT_SS|REAGENT_BM|REAGENT_BP,
	REAGENT_SS|REAGENT_BP|REAGENT_NS,
	REAGENT_BM|REAGENT_BP|REAGENT_MR,
	REAGENT_SS|REAGENT_NS,
	REAGENT_SA|REAGENT_BM,
	REAGENT_SA|REAGENT_BM,
	REAGENT_GA|REAGENT_BM|REAGENT_BP,
	0,0,0,0,0,0,
	/*3rd circle*/
	REAGENT_SA|REAGENT_GA|REAGENT_NS,
	REAGENT_SA|REAGENT_BP,
	REAGENT_SA|REAGENT_BP,
	REAGENT_SA|REAGENT_MR,
	REAGENT_SA|REAGENT_GA|REAGENT_BM,
	REAGENT_GS|REAGENT_GA,
	REAGENT_GS|REAGENT_SS|REAGENT_NS,
	REAGENT_NS|REAGENT_MR,
	REAGENT_SA|REAGENT_GS|REAGENT_GA,
	REAGENT_SA|REAGENT_GA,
	0,0,0,0,0,0,
	/*4th circle*/
	REAGENT_SA|REAGENT_BM|REAGENT_MR,
	REAGENT_SS|REAGENT_MR,
	REAGENT_SS|REAGENT_NS|REAGENT_MR,
	REAGENT_SA|REAGENT_SS|REAGENT_BP,
	REAGENT_GS|REAGENT_SS|REAGENT_MR,
	REAGENT_NS,
	REAGENT_SS|REAGENT_BP|REAGENT_NS,
	REAGENT_GS|REAGENT_SS|REAGENT_BP,
	REAGENT_GS|REAGENT_GA,
	REAGENT_SA|REAGENT_BM,
	0,0,0,0,0,0,
	/*5th circle*/
	REAGENT_SS|REAGENT_BP|REAGENT_MR,
	REAGENT_SA|REAGENT_BM|REAGENT_BP|REAGENT_MR,
	REAGENT_SA|REAGENT_SS|REAGENT_BM,
	REAGENT_BM|REAGENT_NS,
	REAGENT_SA|REAGENT_BP|REAGENT_MR,
	REAGENT_SA|REAGENT_SS|REAGENT_BP|REAGENT_NS,
	REAGENT_SS|REAGENT_BM|REAGENT_NS,
	REAGENT_SS|REAGENT_NS|REAGENT_MR,
	REAGENT_SA|REAGENT_SS|REAGENT_BM|REAGENT_NS|REAGENT_MR,
	REAGENT_SA|REAGENT_MR,
	0,0,0,0,0,0,
	/*6th circle*/
	REAGENT_SS|REAGENT_BP|REAGENT_NS,
	REAGENT_SA|REAGENT_GS|REAGENT_SS|REAGENT_BM|REAGENT_NS|REAGENT_MR,
	REAGENT_NS|REAGENT_MR,
	REAGENT_SA|REAGENT_BM|REAGENT_MR,
	REAGENT_BM|REAGENT_BP|REAGENT_MR,
	REAGENT_SA|REAGENT_GS|REAGENT_GA|REAGENT_MR,
	REAGENT_SA|REAGENT_GA|REAGENT_MR,
	REAGENT_SA|REAGENT_BM|REAGENT_NS,
	REAGENT_SA|REAGENT_GS|REAGENT_SS|REAGENT_BM|REAGENT_NS,
	REAGENT_SS,
	0,0,0,0,0,0,
	/*7th circle*/
	REAGENT_SA|REAGENT_BM|REAGENT_BP|REAGENT_MR,
	REAGENT_SA|REAGENT_SS|REAGENT_MR,
	REAGENT_SA|REAGENT_BM|REAGENT_NS|REAGENT_MR,
	REAGENT_GA|REAGENT_NS|REAGENT_MR,
	REAGENT_SA|REAGENT_BP|REAGENT_MR,
	REAGENT_SA|REAGENT_BP|REAGENT_NS,
	REAGENT_SA|REAGENT_GA|REAGENT_NS|REAGENT_MR,
	REAGENT_BM|REAGENT_BP|REAGENT_NS|REAGENT_MR,
	REAGENT_SA|REAGENT_SS|REAGENT_BM|REAGENT_MR,
	REAGENT_SA|REAGENT_SS|REAGENT_BM|REAGENT_BP|REAGENT_NS|REAGENT_MR,
	0,0,0,0,0,0,
	/*8th circle*/
	0,
	REAGENT_SA|REAGENT_BM|REAGENT_NS|REAGENT_MR,
	REAGENT_SA|REAGENT_GA|REAGENT_BM|REAGENT_NS|REAGENT_MR,
	REAGENT_SS|REAGENT_BP|REAGENT_NS|REAGENT_MR,
	REAGENT_SA|REAGENT_BP|REAGENT_NS|REAGENT_MR,
	REAGENT_SA|REAGENT_GS|REAGENT_GA|REAGENT_SS|REAGENT_BM|REAGENT_MR,
	REAGENT_BM|REAGENT_NS|REAGENT_MR,
	REAGENT_GA|REAGENT_SS|REAGENT_BM|REAGENT_MR,
	REAGENT_GA|REAGENT_BM|REAGENT_MR,
	REAGENT_SA|REAGENT_BM|REAGENT_MR,
	0,0,0,0,0,0
};

char *D_0DC4[] = {
	/*13B8*/"1ST",
	/*13BC*/"2ND",
	/*13C0*/"3RD",
	/*13C4*/"4TH",
	/*13C8*/"5TH",
	/*13CC*/"6TH",
	/*13D0*/"7TH",
	/*13D4*/"8TH"
};

char *__0DD4[] = {
	/*13D8*/"neutral",
	/*13E0*/"evil",
	/*13E5*/"good",
	/*13EA*/"chaotic"
};

char *D_0DDC[] = {
	/*13F2*/"Success",
	/*13FA*/"Failed",
	/*1401*/"No effect",
	/*140B*/"Out of range",
	/*1418*/"Blocked",
	/*1420*/"nothing",
	/*1428*/"Not possible",
	/*1435*/"Done",
	/*143A*/"On what: ",
	/*1444*/"On whom: ",
	/*144E*/"Location: ",
	/*1459*/"Not usable",
	/*1464*/"Magical energy prevents you from removing the amulet."
};

/*magical objects?*/
int D_0DF6[] = {
	TIL_207,TIL_20F,TIL_216,TIL_22F,TIL_232,TIL_235,TIL_238,TIL_239,TIL_23D,TIL_248,
	TIL_249,TIL_24A,TIL_24B,TIL_24C,TIL_24D,TIL_24E,TIL_24F,TIL_251,TIL_252,TIL_253,
	TIL_255,TIL_256,TIL_257,TIL_25A,TIL_25B,TIL_25C,TIL_25D,TIL_25F,TIL_26E,TIL_2B6,
	TIL_31E,TIL_31F,TIL_326,TIL_368,TIL_369,TIL_36A,TIL_36B,TIL_36C,TIL_36D,TIL_36E,
	TIL_36F,TIL_37D,TIL_37E,TIL_37F,TIL_3A8,TIL_3A9,TIL_3AA,TIL_3AB,TIL_3AC,TIL_3AD,
	TIL_3AE,TIL_3AF,TIL_3FD,TIL_3FE,TIL_40C,TIL_40D,TIL_40E,TIL_40F,TIL_41C,TIL_41D,
	TIL_41E,TIL_41F,TIL_42C,TIL_42D,TIL_42E,TIL_42F,TIL_43C,TIL_43D,TIL_43E,TIL_43F,
	TIL_484,TIL_485,TIL_48E,TIL_48F,TIL_6D0,TIL_6D1,TIL_6D2,TIL_6D3
};

/*C_1944_0006*/static IsMagical(int objNum) {
	int si, di;

	di = TILE_FRAME(objNum);
	for(si = 0; si < 78; si ++) {
		if(D_0DF6[si] == di)
			return 1;
	}
	return 0;
}

static C_1944_0054(int objTyp) {
	if(objTyp >= OBJ_1AA || objTyp == OBJ_19B || objTyp == OBJ_176 || objTyp == OBJ_19D || objTyp == OBJ_177)
		return 0;
	return 1;
}

int D_0E92[] = {0x226A,0x1E96,0x1B94,0x1996,0x173E,0x15C2,0x143C,0x12D4,0x1180};
int D_0EA4[] = { 2700, 3000, 1000,  100, 5000, 4000, 2500, 1000,    1};
int D_0EB6[] = {32700,31000,37000,45000,31000,34000,36500,39000,42000};
int D_0EC8[] = {    3,    2,    2,    2,    1,    1,    1,    1,    1};

/*magic "animation"*/
C_1944_0085(int circle) {
	int di;

	di = CON_mouseOffAt(0, 0, 311, 191);
	if(circle < 9) {
		OSI_playNoise(800, circle * 1600 + 8000, 700);
		GR_06(15);
		D_ECC4->_0f = 1;
		D_ECC4->_0c = 1;
		C_0A33_09CE(0);
		GR_12(8, 8, 168, 168);
		C_2FC1_0212(8, 8, 167, 167);
		GR_45(8, 8, 167, 167, 8, 8);
		OSI_playWavedNote(D_0E92[circle], 1, circle * 4000 + 10000, D_0EA4[circle], D_0EC8[circle]);
		OSI_playWavedNote(D_0E92[circle], 1, circle * 4000 + 10000, D_0EB6[circle], -D_0EC8[circle]);
		GR_12(8, 8, 168, 168);
		C_2FC1_0212(8, 8, 167, 167);
		GR_45(8, 8, 167, 167, 8, 8);
		D_ECC4->_0c = 0;
		D_ECC4->_0f = 0;
	} else {
		OSI_playWavedNote(0x1180, 1, (int)65000, 300, 1);
	}
	CON_mouseOn(di);
}

C_1944_01EC(int bp08, int bp06) {
	return ((GetStr(bp06) / 2) + 30 - GetStr(bp08)) / 2 > OSI_rand(1, 30);
}

static C_1944_0232(int bp08, int bp06) {
	return ((GetStr(bp06) / 2) + 30 - GetInt(bp08)) / 2 > OSI_rand(1, 30);
}

static C_1944_0278(int bp08, int bp06) {
	return ((GetDex(bp06) / 2) + 30 - GetInt(bp08)) / 2 > OSI_rand(1, 30);
}

static C_1944_02BE(int objNum, int objTyp, int objQual) {
	int si, quantity;

	quantity = 0;
	for(si = Link[objNum]; si >= 0 && GetCoordUse(si) != LOCXYZ; si = Link[si]) {
		if(GetType(si) == objTyp && GetQual(si) == objQual)
			quantity += GetQuan(si);
	}
	return quantity;
}

/*C_1944_0321*/SPELL_RefreshList(/*int bp08, bp06*/) {
	int si, di;
	int bp_0e, bp_0c, bp_0a,  bp_08, bp_06, bp_04, bp_02;

	/*if(Spellbook[0]) */{
		bp_0c = 0;
		bp_02 = 0;
		for(di = 0; di < 8; di ++)
			SpellList[di] = 0;
		for(bp_0e = FindInv(Spellbook[0]); bp_0e >= 0; bp_0e = NextInv()) {
			si = GetQual(bp_0e);
			if(si == 0xff) {
				bp_02 = 1;
				break;
			}
			if(!HAS_SPELL(si >> 4, si & 0xf)) {
				if(bp_0c == 0)
					bp_0c = si;
				SET_SPELL(si >> 4, si & 0xf);
			}
		}
		if(GetQual(Spellbook[0]) == 0 && FindInvType(Spellbook[0], OBJ_03A, 0) == -1)
			SetQual(Spellbook[0], bp_0c);
	}
	if(Spellbook[1]) {
		bp_0c = 0;

		for(bp_0e = FindInv(Spellbook[1]); bp_0e >= 0; bp_0e = NextInv()) {
			si = GetQual(bp_0e);
			if(si == 0xff) {
				bp_02 = 1;
				break;
			}
			if(!HAS_SPELL(si >> 4, si & 0xf)) {
				if(bp_0c == 0)
					bp_0c = si;
				SET_SPELL(si >> 4, si & 0xf);
			}
		}
		if(GetQual(Spellbook[1]) == 0 && FindInvType(Spellbook[1], OBJ_03A, 0) == -1)
			SetQual(Spellbook[1], bp_0c);
	}
	if(bp_02) {
		for(di = 0; di < 8; di ++)
			SpellList[di] = -1;
	}
	for(di = 0; di < 8; di ++)
		SpellList[di] &= 0x3ff;
	if(Spellbook[0])
		D_EBB3 = GetQual(Spellbook[0]);
	else
		D_EBB3 = GetQual(Spellbook[1]);
	D_EBB1 = -1;
	bp_06 = 0;
	bp_04 = D_EBB3 / 0x10;
	for(bp_0a = 0; bp_0a <= bp_04; bp_0a ++) {
		for(bp_08 = 0; bp_08 < 0x10; bp_08 ++) {
			if(HAS_SPELL(bp_0a, bp_08)) {
				if(bp_0a == bp_04) {
					if((char)D_EBB1 == -1)
						D_EBB1 = bp_06;
					if((bp_0a << 4) + bp_08 == D_EBB3) {
						if(D_EBB1 + 8 <= bp_06)
							D_EBB1 += 8;
						break;
					}
				}
				bp_06 ++;
			}
		}
	}
}

/*draw "magic parchment"*/
/*C_1944_05B1*/SPELL_DrawBG() {
	int si;

	si = CON_mouseOffAt(176, 6, 311, 103);
	GR_06(D_2A54);
	GR_12(168, 6, 311, 103);
	DISK_confirm(DISK_6);
	IsZFile = 1;
	LoadFile(D_0198[1], ScratchBuf);
	IsZFile = 0;
	GR_18(ScratchBuf, 168, 6);
	CON_mouseOn(si);
}

/*computes the maximum a spell can be casted(according to reagents)*/
/*C_1944_0639*/static PrintSpellQuantity(int objNum, int spellNum, int index) {
	int di;
	unsigned char reagentsMask;
	int bp_04,numMax;

	numMax = 99;
	reagentsMask = Reagents_needed[spellNum];
	if(reagentsMask == 0 && spellNum != SPELL_07 && spellNum != SPELL_70)
		return;
	for(bp_04 = 0; bp_04 < 8; bp_04 ++) {
		if((reagentsMask >> bp_04) & 1) {
			di = C_1944_02BE(objNum, ReagType[bp_04], 0);
			if(di < numMax)
				numMax = di;
		}
	}
	if(spellNum == SPELL_07 || spellNum == SPELL_70)
		numMax = 1;
	D_EBBE[index] = numMax;
	CON_printf(/*149A*/"%d", numMax);
}

C_1944_06CA(int spellNum) {
	int i;

	for(i = 0; i < 8; i ++) {
		if(D_EBB6[i] == spellNum)
			return i;
	}
	return -1;
}

static /*C_1944_06EF*/TakeReagents(int objNum, int spellNum, int index) {
	int si, di;
	unsigned char bp_03;
	int bp_02;

	bp_03 = Reagents_needed[spellNum];
	for(bp_02 = 0; bp_02 < 8; bp_02 ++) {
		if((bp_03 >> bp_02) & 1) {
			for(si = Link[objNum]; si >= 0; si = Link[si]) {
				if(ReagType[bp_02] == (di = GetType(si))) {
					TakeObj(objNum, di, 1);
					break;
				}
			}
		}
	}
	if(spellNum != SPELL_07)
		D_EBBE[index] --;
}

/*C_1944_0773*/SPELL_RefreshPage() {
	int si, di;
	int index, bp_14, bp_12, bp_10;
	int spellNum, circle, spellIdx;
	int bp_08, bp_06, bp_04, bp_02;

	di = -1;
	bp_06 = -1;
	bp_04 = -1;
	bp_02 = -1;
	HideMouse
	GR_06(D_2A54);
	GR_12(176, 14, 303, 79);
	CON_setClip(&D_B6B5[1]);
	bp_10 = Party[Active];
	for(si = 0; si < 4; si++)
		D_EBAD[si] = -1;
	bp_14 = 1;
	bp_12 = 1;
	index = 0;
	bp_08 = 0;
	for(circle = 0; circle < 8; circle ++) {
		for(spellIdx = 0; spellIdx < 16; spellIdx ++) {
			if(!HAS_SPELL(circle, spellIdx))
				continue;
			if(D_EBB1 > bp_08) {
				bp_08 ++;
				if(bp_02 == -1)
					bp_02 = circle;
				di = circle;
				bp_06 = spellIdx;
				continue;
			}
			if(bp_04 == -1)
				bp_04 = circle;
			if(index < 8 && circle == bp_04) {
				if(circle == di && spellIdx > bp_06 && index == 0)
					D_EBAD[1] = 2;
				if(circle > bp_02 && index == 0)
					D_EBAD[0] = di - ((D_EBAD[1] == 0xff)?0:D_EBAD[1]);
				if(D_EBAD[0] != 0xff && D_EBAD[1] == 0xff)
					D_EBAD[1] = 1;
				spellNum = (circle << 4) + spellIdx;
				D_EBB6[index] = spellNum;
				CON_gotoxy(bp_14,      bp_12); CON_printf(/*149D*/"%s", SpellName[spellNum]);
				CON_gotoxy(bp_14 + 13, bp_12); PrintSpellQuantity(bp_10, spellNum, index);
				/*selection arrow?*/
				if(D_EBB3 == spellNum) {
					CON_gotoxy(bp_14 - 1, bp_12);
					CON_putch0(0x1a);
					D_EBBE[index] |= 0x80;
				}
				di = circle;
				bp_06 = spellIdx;
				index ++;
				bp_12 ++;
				continue;
			}
			if(D_EBAD[2] == 0xff && circle == di && spellIdx > bp_06)
				D_EBAD[2] = 2;
			if(D_EBAD[2] == 0xff)
				D_EBAD[2] = 1;
			if(D_EBB0 == 0xff && circle > di)
				D_EBB0 = circle;
		}
	}
	D_EBAA = index;
	for(si = 1; si < 3; si ++) {
		if(D_EBAD[si] != 0xff)
			GR_2D(TIL_19B + si, ((si << 1) + 22) << 3, 88);
		else
			GR_2D(TIL_19B, ((si << 1) + 22) << 3, 88);
	}
	CON_gotoxy(11, 10); CON_printf(/*149D*/"%s", D_0DC4[bp_04]);
	CON_gotoxy(10, 11); CON_printf(/*14A0*/"level");
	CON_setClip(&D_B6B5[3]);
	ShowMouse
}

C_1944_0A43(int objNum, int objType, int objQual) {
	int si;
	for(
		si = Link[objNum];
		si >= 0 && GetCoordUse(si) != LOCXYZ && C_1184_1D2E(si, objNum);
		si = Link[si]
	) {
		if(GetType(si) == objType) {
			if(objQual == 0 || GetQual(si) == objQual)
				return si;
		}
	}
	return -1;
}

int D_0EDA[] = {OBJ_129,OBJ_12A,OBJ_12B,OBJ_12C};

/*IsDoor?*/
C_1944_0AA9(int objType) {
	int si;

	for(si = 0; si < 4; si ++) {
		if(D_0EDA[si] == objType)
			return 1;
	}
	return 0;
}

/*"sleep"*/
C_1944_0AD0(int bp08, int objNum) {
	int objCls;

	objCls = GetMonsterClass(GetType(objNum));
	if(objCls < 0 || !IsMonster_0001(objCls))  {
		if(COMBAT_TestIntel(bp08, objNum))
			return 1;
		COMBAT_showHit(objNum);
		SetAsleep(objNum);
		SetTypeUnconscious(objNum);
		return (unsigned char)-2;
	}
	return 2;
}

/*"poison"*/
static C_1944_0B32(int bp08, int si/*bp06*/) {
	if(!IsMonster_0004(GetMonsterClass(GetType(si)))) {
		if(C_1944_0232(bp08, si))
			return 1;
		SetPoisoned(si);
		COMBAT_showHit(si);
		return (unsigned char)-2;
	}
	return 2;
}

/*"magic arrow"*/
static C_1944_0B85(int di/*bp06*/) {
	int si;
	int bp_02;

	if(C_1944_0278(di, Selection.obj)) {
		SpellResult = 1;
		return;
	}
	si = OSI_rand(1, 10);
	bp_02 = LooseHP(Selection.obj, si);
	AddXP(di, bp_02);
	C_2337_1C1E(Selection.obj, di, si);
	CON_printf(/*14A6*/"\n");
	COMBAT_displaySeverity(Selection.obj);
	SpellResult = -1;
}

/*magical H.P. loss?*/
static C_1944_0BF4(int spellNum) {
	int si, di;
	int bp_02;

	di = Party[Active];
	if(COMBAT_TestIntel(di, Selection.obj)) {
		SpellResult = 1;
		return;
	}
	if(spellNum == SPELL_05)
		si = OSI_rand(1, 10);
	else if(spellNum == SPELL_62)
		si = OSI_rand(1, 30);
	else if(spellNum == SPELL_32)
		si = HitPoints[Selection.obj] - 1;
	CON_printf(/*14A6*/"\n");
	bp_02 = LooseHP(Selection.obj, si);
	AddXP(di, bp_02);
	C_2337_1C1E(Selection.obj, di, si);
	COMBAT_displaySeverity(Selection.obj);
	SpellResult = -1;
}

/*"charm"*/
static C_1944_0CA3(int bp08, int si/*bp06*/) {
	if(!COMBAT_TestIntel(bp08, si)) {
		if(!IsCharmed(si)) {
			SetBKAlignment(si, GetAlignment(si));
			SetCharmed(si);
			SetAlignment(si, GetAlignment(bp08));
			NPCMode[si] = AI_ASSAULT;
			COMBAT_showHit(si);
			CON_printf(/*14A6*/"\n");
			CON_printf((char *)GetObjectString(si));
			CON_printf(/*14A8*/" is charmed.\n");
			SpellResult = -2;
		} else if(IsCharmed(si)) {
			COMBAT_stopCharm(si);
		}
	} else {
		SpellResult = 1;
	}
}

/*"kill"*/
static C_1944_0D46(int bp08, int si/*bp06*/) {
	int objCls;
	int bp_02;

	objCls = GetMonsterClass(GetType(si));
	if(!IsDead(si) && (objCls < 0 || !IsMonster_0002(objCls))) {
		if(!COMBAT_TestIntel(bp08, si)) {
			bp_02 = LooseHP(si, HitPoints[si]);
			AddXP(bp08, bp_02);
			C_2337_1C1E(si, bp08, 1);
			COMBAT_displaySeverity(si);
			return (unsigned char)-2;
		}
		return 1;
	}
	return 2;
}

/*"douse"/"ignite"*/
static C_1944_0DCF(int objNum, int spellNum) {
	int di;
	int bp_02;

	di = GetType(objNum);
	bp_02 = GetFrame(objNum);
	if(
		(di != OBJ_05A && di != OBJ_0A4 && di != OBJ_07A && di != OBJ_0FD && di != OBJ_0CE && di != OBJ_091 && di != OBJ_0DF) ||
		(di == OBJ_0CE && bp_02 > 1) ||
		(spellNum == SPELL_04 && (bp_02 & 1) == 0) ||
		(spellNum == SPELL_08 && (bp_02 & 1))
	) {
		SpellResult = 2;
	} else if(di == OBJ_0DF && bp_02 == 0) {
		SetFrame(objNum, 1);
		D_2CA4 = 3;
		D_2CA6 = objNum;
	} else if(bp_02 & 1) {
		if(di == OBJ_05A) {
			DeleteObj(objNum);
			CON_printf(/*14A6*/"\n");
			CON_printf(BurnOutMsg);
		} else {
			SetFrame(objNum, bp_02 & 2);
			ClrLit(objNum);
		}
	} else {
		SetFrame(objNum, bp_02 | 1);
		SetLit(objNum);
	}
	C_1100_0306();
}

static C_1944_0F02(int bp06) {
	int si, di;
	int bp_0c, bp_0a, bp_08, bp_06, bp_04, bp_02;

	di = -1;
	bp_0c = 0x7fff;
	bp_0a = GetX(bp06);
	bp_08 = GetY(bp06);
	for(si = 0; si < 0x100; si ++) {
		if(ObjShapeType[si] && !IsDead(si) && si != bp06 && COMBAT_canSee(bp06, si)) {
			bp_06 = GetX(si);
			bp_04 = GetY(si);
			if(bp_06 > MapX - 5 && bp_06 < MapX + 5 && bp_04 > MapY - 5 && bp_04 < MapY + 5) {
				bp_02 = (bp_06 - bp_0a) * (bp_06 - bp_0a) + (bp_04 - bp_08) * (bp_04 - bp_08);
				if(bp_02 > 0 && bp_02 <= bp_0c) {
					if(bp_02 == bp_0c) {
						di = OSI_Rand(0, 1)?di:si;
					} else {
						bp_0c = bp_02;
						di = si;
					}
				}
			}
		}
	}
	return di;
}

/*"chain bolt"*/
static C_1944_1038(int si/*bp06*/) {
	int di;
	int bp_0a, bp_08, bp_06, bp_04, bp_02;

	bp_02 = LooseHP(si, OSI_rand(1, 30));
	CON_printf(/*14A6*/"\n");
	AddXP(Party[Active], bp_02);
	C_2337_1C1E(si, Party[Active], 1);
	COMBAT_displaySeverity(si);
	for(bp_0a = 0; bp_0a < 6; bp_0a ++) {
		di = C_1944_0F02(si);
		if(di == -1)
			break;
		bp_06 = GetX(di);
		bp_04 = GetY(di);
		bp_08 = COMBAT_Missile(si, &bp_06, &bp_04, 8, TIL_188);
		if(bp_08 == 0)
			break;
		si = di;
		bp_02 = LooseHP(si, OSI_rand(1, 30));
		AddXP(Party[Active], bp_02);
		CON_printf(/*14A6*/"\n");
		COMBAT_displaySeverity(si);
	}
}

/*"great heal"*/
C_1944_114D(int spellNum) {
	int bp_02;

	if(IsDead(Selection.obj)) {
		SpellResult = 2;
		return;
	}
	if(spellNum == SPELL_06) {
		bp_02 = OSI_rand(1, 30);
		if(MaxHP(Selection.obj) > HitPoints[Selection.obj] + bp_02) {
			HitPoints[Selection.obj] += bp_02;
			return;
		}
	} else if(spellNum != SPELL_34) {
		return;
	}
	HitPoints[Selection.obj] = MaxHP(Selection.obj);
}

/*"dispel"*/
static C_1944_11BB(int si/*bp06*/) {
	int di;

	di = 0;
	MagicalHalo(si, 1);
	if(IsAsleep(si)) {
		ClrAsleep(si);
		ObjShapeType[si] = OrigShapeType[si];
		di = 1;
	}
	if(IsPoisoned(si)) {
		ClrPoisoned(si);
		di = 1;
	}
	if(IsParalyzed(si)) {
		ClrParalyzed(si);
		di = 1;
	}
	if(IsCharmed(si)) {
		COMBAT_stopCharm(si);
		di = 1;
	}
	if(di)
		return 0;
	else
		return 1;
}

/*"fireball"*/
static C_1944_1232(int si/*bp06*/) {
	int objCls;
	int bp_02;

	objCls = GetMonsterClass(GetType(si));
	if(objCls < 0 || !IsMonster_0008(objCls)) {
		if(IsMonster_0010(objCls))
			bp_02 = LooseHP(si, OSI_rand(1, 20) << 1);
		else
			bp_02 = LooseHP(si, OSI_rand(1, 20));
		AddXP(Party[Active], bp_02);
		CON_printf(/*14A6*/"\n");
		C_2337_1C1E(si, Party[Active], 1);
		COMBAT_displaySeverity(si);
		return 1;
	}
	return 0;
}

C_1944_12FC(int spellnum, int projTile, int range, int objOrNpc, int aFlag) {
	int si, di;
	int bp_10, bp_0e, bp_0c, bp_0a, bp_08, bp_06;
	struct coord bp_04;

	di = Party[Active];
	if(IsPlrControl(Party[Active])) {
		if(objOrNpc == 1)
			CON_printf(/*149D*/"%s", D_0DDC[9]);
		else
			CON_printf(/*149D*/"%s", D_0DDC[8]);
	}
	if(!aFlag) {
		MouseMode = 1;
		if(SelectMode != 0)
			SelectRange = range;
		CON_getch();
	}
	if(Selection.obj == -1 || Selection.x == -1 || Selection.y == -1) {
		CON_printf(/*14B6*/"%s\n", D_0DDC[5]);
	} else {
		CON_printf((char *)GetObjectString(Selection.obj));
		CON_printf(/*14A6*/"\n");
	}
	MagicalHalo(Party[Active], 0);
	if(Selection.x != -1 && Selection.y != -1)
		bp_0e = COMBAT_Missile(di, &Selection.x, &Selection.y, range, projTile);
	if(Selection.obj == -1 || Selection.x == -1 || Selection.y == -1) {
		CON_printf(/*14BA*/"\n%s\n", D_0DDC[2]);
		MUS_0065(13, 0);
		return;
	}
	if(bp_0e) {
		if(
			(objOrNpc == 1 && Selection.obj >= 0x100) ||
			(objOrNpc == -1 && Selection.obj < 0x100 && spellnum != SPELL_21)
		) {
			CON_printf(/*14BA*/"\n%s\n", D_0DDC[2]);
			MUS_0065(13, 0);
			return;
		}
		Selection.obj = COMBAT_getHead(Selection.obj);
		bp_10 = GetType(Selection.obj);
		SpellResult = 0;
		switch(spellnum) {
			case SPELL_03:/*"dispel magic"*/
				if(C_1944_11BB(Selection.obj))
					SpellResult = 2;
			break;
			case SPELL_05:/*"harm"*/
				C_1944_0BF4(spellnum);
			break;
			case SPELL_06:/*"heal"*/
			case SPELL_34:/*"great heal"*/
				MagicalHalo(Selection.obj, 1);
				C_1944_114D(spellnum);
			break;
			case SPELL_04:/*"douse"*/
			case SPELL_08:/*"ignite"*/
				C_1944_0DCF(Selection.obj, spellnum);
			break;
			case SPELL_19:/*"vanish"*/
				if(IsTileGe(TILE_FRAME(Selection.obj))) {
					D_2C6C = ObjShapeType[Selection.obj];
					DeleteObj(Selection.obj);
					ScreenFade = 1;
					C_1100_0306();
					OtherAnimations();
				} else {
					SpellResult = 2;
				}
			break;
			case SPELL_14:/*"sleep"*/
				SpellResult = C_1944_0AD0(di, Selection.obj);
			break;
			case SPELL_11:/*"magic arrow"*/
				C_1944_0B85(di);
			break;
			case SPELL_44:/*"lightning"*/
				bp_06 = LooseHP(Selection.obj, OSI_rand(1, 30));
				AddXP(Party[Active], bp_06);
				CON_printf(/*14A6*/"\n");
				COMBAT_displaySeverity(Selection.obj);
				C_2337_1C1E(Selection.obj, Party[Active], 1);
				SpellResult = -1;
			break;
			case SPELL_22:/*"fireball"*/
				if(C_1944_1232(Selection.obj))
					SpellResult = -1;
				else
					SpellResult = 2;
			break;
			case SPELL_60:/*"chain bolt"*/
				C_1944_1038(Selection.obj);
				SpellResult = -1;
			break;
			case SPELL_12:/*"poison"*/
				SpellResult = C_1944_0B32(di, Selection.obj);
			break;
			case SPELL_50:/*"charm"*/
				C_1944_0CA3(di, Selection.obj);
			break;
			case SPELL_45:/*"paralyze"*/
				SetParalyzed(Selection.obj);
				COMBAT_showHit(Selection.obj);
				CON_printf(/*14A6*/"\n");
				CON_printf((char *)GetObjectString(Selection.obj));
				CON_printf(/*14BF*/" is paralyzed.\n");
			break;
			case SPELL_20:/*"curse"*/
				SetCursed(Selection.obj);
			break;
			case SPELL_28:/*"protection"*/
				SetProtected(Selection.obj);
			break;
			case SPELL_43:/*"invisibility"*/
				if(!IsInvisible(Selection.obj) && C_1944_0054(bp_10)) {
					SetInvisible(Selection.obj);
					MagicalHalo(Selection.obj, 1);
				} else {
					SpellResult = 2;
				}
			break;
			case SPELL_65:/*"kill"*/
				SpellResult = C_1944_0D46(di, Selection.obj);
			break;
			case SPELL_30:/*"animate"*/
				if(STAT_GetEquipSlot(Selection.obj) != -1) {
					bp_0e = EGG_generate(bp_10, GetX(Selection.obj), GetY(Selection.obj), MapZ);
					SetAlignment(bp_0e, GetAlignment(di));
					NPCMode[bp_0e] = AI_ASSAULT;
					DeleteObj(Selection.obj);
				} else {
					SpellResult = 2;
				}
			break;
			case SPELL_58:/*"replicate"*/
				if(IsTileGe(TILE_FRAME(Selection.obj))) {
					for(bp_0c = 0; bp_0c < 8; bp_0c ++) {
						bp_0a = OSI_rand(0, 10) + MapX - 5;
						bp_08 = OSI_rand(0, 10) + MapY - 5;
						SetCoordXYZ(bp_04, bp_0a, bp_08, MapZ);
						si = AddObj(0, bp_04, GetType(Selection.obj), GetFrame(Selection.obj), 1);
						SetOkToGet(si);
						if(C_1E0F_000F(si, bp_0a, bp_08)) {
							ScreenFade = 1;
							C_1100_0306();
							OtherAnimations();
							break;
						}
						DeleteObj(si);
					}
					if(bp_0c == 8)
						SpellResult = 1;
				} else {
					SpellResult = 1;
				}
			break;
			case SPELL_51:/*"clone"*/
				for(bp_0c = 0; bp_0c < 8; bp_0c ++) {
					bp_0a = OSI_rand(0, 10) + MapX - 5;
					bp_08 = OSI_rand(0, 10) + MapY - 5;
					if(C_1E0F_000F(Selection.obj, bp_0a, bp_08))
						break;
				}
				if(bp_0c == 8) {
					SpellResult = 2;
					break;
				}
				bp_0e = EGG_generate(GetType(Selection.obj), bp_0a, bp_08, MapZ);
				SetAlignment(bp_0e, GetAlignment(Selection.obj));
				NPCMode[bp_0e] = NPCMode[Selection.obj];
				ScreenFade = 1;
				C_1100_0306();
				OtherAnimations();
			break;
			case SPELL_21:/*"dispel field"*/
				for(si = FindLoc(Selection.x, Selection.y, MapZ); si >= 0; si = NextLoc()) {
					bp_10 = GetType(si);
					if(bp_10 == OBJ_13D || bp_10 == OBJ_13E || bp_10 == OBJ_13F || bp_10 == OBJ_140) {
						DeleteObj(si);
						COMBAT_showHit(si);
						C_1100_0306();
						break;
					}
				}
				if(si == -1)
					SpellResult = 2;
			break;
		}
		if(SpellResult != 0 && SpellResult != -1 && SpellResult != -2) {
			if(SpellResult == 1) {
				CON_printf(/*14BA*/"\n%s\n", D_0DDC[1]);
				MUS_0065(13, 0);
			} else if(SpellResult == 2) {
/*C_1984:*/
				CON_printf(/*14BA*/"\n%s\n", D_0DDC[2]);
				MUS_0065(13, 0);
			} else {
				MUS_0065(13, 0);
			}
		} else if(SpellResult == 0) {
			CON_printf(/*14BA*/"\n%s\n", D_0DDC[0]);
			MUS_0065(14, 0);
		}
		return;
	}
/*19c2*/
	if(!CLOSE_ENOUGH(range, Selection.x, Selection.y, GetX(di), GetY(di)))
		CON_printf(/*14CF*/"\n%s!\n", D_0DDC[3]);
	else
		CON_printf(/*14CF*/"\n%s!\n", D_0DDC[4]);
	MUS_0065(13, 0);
}

/*resurrection*/
C_1944_1A42(int bp06) {
	int si, di;
	int bp_08, bp_06, new_x, new_y;

	bp_08 = 0;
	si = C_1184_1C52(bp06);
	ClrDead(si);
	HitPoints[si] = 1;
	ObjShapeType[si] = OrigShapeType[si];
	MAGIC[si] = MaxMagic(si);
	Rstbis_001f(si);
	if(KARMA < 98)
		ExpPoints[si] = ((long)ExpPoints[si] * KARMA) / 100L;
	for(di = 1; di < 5; di ++) {
		for(bp_06 = 0; bp_06 < (di << 1) + 1; bp_06 ++) {
			new_x = GetX(si) - di + bp_06;
			new_y = GetY(si) - di;
			if(C_1E0F_000F(si, new_x, new_y)) {
				MoveObj(si, new_x, new_y, MapZ);
				bp_08 = 1;
				break;
			}
			new_x = GetX(si) - di;
			new_y = GetY(si) - di + bp_06;
			if(C_1E0F_000F(si, new_x, new_y)) {
				MoveObj(si, new_x, new_y, MapZ);
				bp_08 = 1;
				break;
			}
			new_x = GetX(si) + di - bp_06;
			new_y = GetY(si) + di;
			if(C_1E0F_000F(si, new_x, new_y)) {
				MoveObj(si, new_x, new_y, MapZ);
				bp_08 = 1;
				break;
			}
			new_x = GetX(si) + di;
			new_y = GetY(si) + di - bp_06;
			if(C_1E0F_000F(si, new_x, new_y)) {
				MoveObj(si, new_x, new_y, MapZ);
				bp_08 = 1;
				break;
			}
		}
		if(bp_08)
			break;
	}
	if(!bp_08)
		MoveObj(si, GetX(si), GetY(si), MapZ);
	if(IsPlrControl(si))
		JoinParty(si);
}

/*"resurrect"*/
static C_1944_1C51(int aFlag) {
	int si, di;

	CON_printf(/*149D*/"%s", D_0DDC[9]);
	if(!aFlag) {
		di = StatusDisplay;
		StatusDisplay = CMD_92;
		C_155D_1065(Party[Active]);
		MouseMode = 1;
		CON_getch();
		StatusDisplay = di;
	}
	if(Selection.obj == -1) {
		CON_printf(/*14B6*/"%s\n", D_0DDC[5]);
	} else {
		CON_printf((char *)GetObjectString(Selection.obj));
		CON_printf(/*14A6*/"\n");
	}
	MagicalHalo(Party[Active], 0);
	if(Selection.obj == -1 || IsArmageddon) {
		CON_printf(/*14BA*/"\n%s\n", D_0DDC[2]);
		MUS_0065(13, 0);
		return;
	}
	MagicalHalo(Selection.obj, 1);
	si = GetType(Selection.obj);
	if(
		(si != OBJ_153 && si != OBJ_162 && si != OBJ_155 && si != OBJ_154) ||
		GetQual(Selection.obj) >= 0xe0
	) {
		CON_printf(/*14BA*/"\n%s\n", D_0DDC[2]);
		MUS_0065(13, 0);
		return;
	}
	if(GetQual(Selection.obj) == 0) {
		CON_printf(/*14D5*/"\nThe corpse is too old!\n");
		return;
	}
	C_1944_1A42(Selection.obj);
	CON_printf(/*14BA*/"\n%s\n", D_0DDC[0]);
	MUS_0065(14, 0);
	SpellResult = 0;
}

static __1944_1DAB(int objTyp) {
	if(
		objTyp == OBJ_160 ||
		objTyp == OBJ_175 ||
		objTyp >= OBJ_1AA ||
		objTyp == OBJ_19B ||
		objTyp == OBJ_1A9 ||
		objTyp == OBJ_176 ||
		objTyp == OBJ_16E ||
		objTyp == OBJ_19D ||
		objTyp == OBJ_199
	)
		return 0;
	else
		return 1;
}

char *D_0EE2[] = {
	/*14EE*/"an awaken",
	/*14F8*/"a cure",
	/*14FF*/"a heal",
	/*1506*/"a poison",
	/*150F*/"a sleep",
	/*1517*/"a protection",
	/*1524*/"an invisibility",
	/*1534*/"an xray vision"
};

char D_0EF2[] = {1, -1};
char D_0EF4[] = {0x1a, 0x26};/*left wing*/
char D_0EF6[] = {0x22, 0x1e};/*right wing*/

static C_1944_1DF4(int spellnum, int bp0a, int bp08, int aFlag) {
	int di;
	int bp_06, bp_04, bp_02;

	bp_02 = 0;
	bp_06 = Party[Active];
	if(IsPlrControl(Party[Active])) {
		if(bp08 == 1)
			CON_printf(/*149D*/"%s", D_0DDC[9]);
		else
			CON_printf(/*149D*/"%s", D_0DDC[8]);
	}
	if(!aFlag) {
		MouseMode = 1;
		if(SelectMode != 0)
			SelectRange = bp0a;
		CON_getch();
	}
	if(Selection.obj == -1 || (Selection.x == -1 && spellnum != SPELL_01 && spellnum != SPELL_02)) {
		CON_printf(/*14B6*/"%s\n", D_0DDC[5]);
	} else{
		CON_printf((char *)GetObjectString(Selection.obj));
		CON_printf(/*14A6*/"\n");
	}
	MagicalHalo(Party[Active], 0);
	if(Selection.obj == -1) {
		goto C_22A9;/*TODO*/
	}
	if(Selection.x == -1 && spellnum != SPELL_01 && spellnum != SPELL_02) {
		goto C_22A9;/*TODO*/
	}
	if(!CLOSE_ENOUGH(bp0a, Selection.x, Selection.y, GetX(bp_06), GetY(bp_06))) {
		if(
			spellnum != SPELL_01 &&
			spellnum != SPELL_02 &&
			spellnum != SPELL_16 &&
			spellnum != SPELL_18 &&
			spellnum != SPELL_24 &&
			spellnum != SPELL_17
		) {
			CON_printf(/*14CF*/"\n%s!\n", D_0DDC[3]);
			MUS_0065(13, 0);
			return;
		}
	}
	if(bp08 == 1 && Selection.obj >= 0x100) {
		goto C_22A9;/*TODO*/
	}
	if(bp08 == -1 && Selection.obj < 0x100) {
		goto C_22A9;/*TODO*/
	}
	Selection.obj = COMBAT_getHead(Selection.obj);
	di = GetType(Selection.obj);
	SpellResult = 0;
	switch(spellnum) {
		case SPELL_32:/*"disable"*/ C_1944_0BF4(spellnum); break;
		case SPELL_24:/*"lock"*/
			if(C_1944_0AA9(di)) {
				C_27A1_2A44(Selection.obj, 0, 1, 0);
			} else if(di == OBJ_062) {
				C_27A1_2BBC(Selection.obj, 0, 1, 0);
				SpellResult = -1;
			} else {
				SpellResult = 2;
			}
		break;
		case SPELL_17:/*"unlock magic"*/
			if(C_1944_0AA9(di) && (GetFrame(Selection.obj) & 0xc) == 0xc) {
				C_27A1_2A44(Selection.obj, 0, 0, 1);
				SpellResult = -1;
			} else if(di == OBJ_062 && GetFrame(Selection.obj) == 3) {
				C_27A1_2BBC(Selection.obj, 0, 0, 1);
				SpellResult = -1;
			} else {
				SpellResult = 2;
			}
		break;
		case SPELL_18:/*"untrap"*/
			MagicalHalo(Selection.obj, 1);
			if(
				(di == OBJ_062 || C_1944_0AA9(di)) &&
				FindInvType(Selection.obj, OBJ_151, SPELL_16) != -1
			) {
				DeleteObj(C_1944_0A43(Selection.obj, OBJ_151, SPELL_16));
			} else {
				SpellResult = 2;
			}
		break;
		case SPELL_16:/*"trap"*/
			MagicalHalo(Selection.obj, 1);
			if(
				(di == OBJ_062 || C_1944_0AA9(di)) &&
				FindInvType(Selection.obj, OBJ_151, SPELL_16) == -1
			) {
				GiveObj(Selection.obj, OBJ_151, QualQuan(SPELL_16, 1));
			} else {
				SpellResult = 2;
			}
		break;
		case SPELL_02:
			MagicalHalo(Selection.obj, 1);
			if(OSI_rand(1, 30) < GetInt(bp_06)) {
				for(bp_04 = FindInv(Selection.obj); bp_04 >= 0; bp_04 = NextInv()) {
					if(GetType(bp_04) == OBJ_151) {
						CON_printf(/*1543*/"\nIt's trapped.\n");
						bp_02 = 1;
					}
				}
				if(bp_02) {
					SpellResult = -1;
					break;
				}
			}
			CON_printf(/*1553*/"\nNo trap\n");
			SpellResult = -1;
		break;
		case SPELL_01:
			MagicalHalo(Selection.obj, 1);
			if(IsMagical(Selection.obj)) {
				bp_02 = 1;
			} else {
				for(bp_04 = FindInv(Selection.obj); bp_04 >= 0; bp_04 = NextInv()) {
					if(GetType(bp_04) == OBJ_150) {
						CON_printf(/*155D*/"\nIt shows a charge of %s.\n", SpellName[GetQual(bp_04)]);
						bp_02 = 2;
						break;
					}
				}
			}
			if(bp_02 == 0) {
				CON_printf(/*1578*/"\nIt's not magical.\n");
			} else if(bp_02 == 1) {
				if(GetType(Selection.obj) == OBJ_113)
					CON_printf(/*158C*/"\nIt's %s potion.\n", D_0EE2[GetFrame(Selection.obj)]);
				else
					CON_printf(/*159E*/"\nIt's magical.\n");
			}
			SpellResult = -1;
		break;
	}
	if(SpellResult != 0 && SpellResult != -1) {
		if(SpellResult == 1)
			CON_printf(/*14BA*/"\n%s\n", D_0DDC[1]);
		else if(SpellResult == 2)
C_22A9:
			CON_printf(/*14BA*/"\n%s\n", D_0DDC[2]);
		MUS_0065(13, 0);
		return;
	}
	if(SpellResult == 0) {
		CON_printf(/*14BA*/"\n%s\n", D_0DDC[0]);
		MUS_0065(14, 0);
	}
}

/*"slime"*/
static C_1944_22F4() {
	int si, di;
	int bp_08, bp_06, bp_04, objCls;

	bp_08 = Party[Active];
	MagicalHalo(bp_08, 0);
	for(di = MapX - 5; di <= MapX + 5; di ++) {
		for(bp_06 = MapY - 5; bp_06 <= MapY + 5; bp_06 ++) {
			si = FindLoc(di, bp_06, MapZ);
			if(
				si != -1 && si < 0x100 &&
				GetAlignment(si) == EVIL &&
				!COMBAT_TestIntel(bp_08, si)
			) {
				objCls = GetMonsterClass(GetType(si));
				if(objCls < 0 || !IsMonster_0200(objCls) || IsMonster_AM(objCls)) {
					ObjShapeType[si] = TypeFrame(OBJ_177, 0);
					if(si >= 0xe0) {
						while((bp_04 = FindInv(si)) >= 0)
							DeleteObj(bp_04);
					}
				}
			}
		}
	}
	ScreenFade = 1;
	C_1100_0306();
	OtherAnimations();
}

/*"reappear"*/
static C_1944_23F2(int aFlag) {
	int si, di;
	struct coord xyz;

	di = Party[Active];
	if(IsPlrControl(Party[Active]))
		CON_printf(/*149D*/"%s", D_0DDC[10]);
	if(!aFlag) {
		MouseMode = 1;
		CON_getch();
	}
	CON_printf(/*14A6*/"\n");
	MagicalHalo(Party[Active], 0);
	if(Selection.x == -1 || Selection.y == -1 || D_2C6C == 0) {
		CON_printf(/*14BA*/"\n%s\n", D_0DDC[2]);
		MUS_0065(13, 0);
		return;
	}
	if(COMBAT_Missile(di, &Selection.x, &Selection.y, 8, TIL_17F)) {
		SetCoordXYZ(xyz, Selection.x, Selection.y, MapZ);
		si = AddObj(0, xyz, D_2C6C & 0x3ff, D_2C6C >> 10, 1);
		SetOkToGet(si);
		if(C_1E0F_000F(si, Selection.x, Selection.y)) {
			ScreenFade = 1;
			C_1100_0306();
			OtherAnimations();
			CON_printf(/*14BA*/"\n%s\n", D_0DDC[0]);
			MUS_0065(14, 0);
			D_2C6C = 0;
			return;
		}
		DeleteObj(si);
		CON_printf(/*14BA*/"\n%s\n", D_0DDC[2]);
		MUS_0065(13, 0);
	} else {
		CON_printf(/*14BA*/"\n%s\n", D_0DDC[4]);
		MUS_0065(13, 0);
	}
}

/*"mass ..." or "explosion"*/
static C_1944_256A(int spellNum, int bp0a, int bp08, int aFlag) {
	int si;
	int far *bp_04;
	
	si = Party[Active];
	if(IsPlrControl(Party[Active]))
		CON_printf(/*149D*/"%s", D_0DDC[10]);
	if(!aFlag) {
		MouseMode = 1;
		CON_getch();
	}
	CON_printf(/*14A6*/"\n");
	MagicalHalo(Party[Active], 0);
	if(Selection.x == -1 || Selection.y == -1) {
		CON_printf(/*14BA*/"\n%s\n", D_0DDC[2]);
		MUS_0065(13, 0);
		return;
	}
	if(COMBAT_Missile(si, &Selection.x, &Selection.y, 8, bp0a)) {
		for(bp_04 = Explosion(Selection.x, Selection.y, bp08); *bp_04 >= 0; bp_04 ++) {
			if(*bp_04 < 0x100) {
				switch(spellNum) {
					case SPELL_25:
						ClrAsleep(*bp_04);
						ObjShapeType[*bp_04] = OrigShapeType[*bp_04];
					break;
					case SPELL_55: SetProtected(*bp_04); break;
					case SPELL_66: SetCursed(*bp_04); break;
					case SPELL_26: C_1944_0AD0(si, *bp_04); break;
					case SPELL_36: C_1944_11BB(*bp_04); break;
					case SPELL_41: C_1944_1232(*bp_04); SpellResult = -1; break;
					case SPELL_73: C_1944_0CA3(si, *bp_04); break;
					case SPELL_74: C_1944_0D46(si, *bp_04); break;
					case SPELL_67:
						if(!IsInvisible(*bp_04) && C_1944_0054(GetType(*bp_04))) {
							SetInvisible(*bp_04);
							MagicalHalo(*bp_04, 1);
						}
					break;
				}
			}
		}
		if(spellNum == SPELL_41 || spellNum == SPELL_74)
			AfterExplosion(Selection.x, Selection.y);
		if(SpellResult != -1)
			CON_printf(/*14BA*/"\n%s\n", D_0DDC[0]);
	} else {
		CON_printf(/*14BA*/"\n%s\n", D_0DDC[4]);
		MUS_0065(13, 0);
	}
}

/*"{fire|poison|sleep|energy} field"*/
static C_1944_27AF(int spellnum, int bp08, int aFlag) {
	int si, di;
	struct coord bp_04;

	di = Party[Active];
	if(IsPlrControl(Party[Active]))
		CON_printf(/*149D*/"%s", D_0DDC[10]);
	if(!aFlag) {
		MouseMode = 1;
		CON_getch();
	}
	CON_printf(/*14A6*/"\n");
	MagicalHalo(Party[Active], 0);
	if(Selection.x == -1 || Selection.y == -1) {
		CON_printf(/*14BA*/"\n%s\n", D_0DDC[2]);
		MUS_0065(13, 0);
		return;
	}
	if(COMBAT_Missile(di, &Selection.x, &Selection.y, 8, bp08)) {
		SetCoordXYZ(bp_04, Selection.x, Selection.y, MapZ);
		switch(spellnum) {
			case SPELL_33: si = AddObj(0, bp_04, OBJ_13D, 0, 1); break;
			case SPELL_37: si = AddObj(0, bp_04, OBJ_13E, 0, 1); break;
			case SPELL_38: si = AddObj(0, bp_04, OBJ_140, 0, 1); break;
			case SPELL_40: si = AddObj(0, bp_04, OBJ_13F, 0, 1); break;
		}
		if(C_1E0F_000F(si, Selection.x, Selection.y)) {
			ScreenFade = 1;
			C_1100_0306();
			OtherAnimations();
			CON_printf(/*14BA*/"\n%s\n", D_0DDC[0]);
			return;
		}
		DeleteObj(si);
		CON_printf(/*14BA*/"\n%s\n", D_0DDC[6]);
		MUS_0065(13, 0);
	} else {
		CON_printf(/*14BA*/"\n%s\n", D_0DDC[4]);
		MUS_0065(13, 0);
	}
}

/*"fear"*/
static C_1944_29AA(int objNum) {
	int objCls;

	objCls = GetMonsterClass(GetType(objNum));
	if(objNum >= 0x100)
		return;
	if(GetAlignment(objNum) == GOOD)
		return;
	if(COMBAT_TestIntel(Party[Active], objNum))
		return;
	if(objCls < 0 || !IsMonster_0002(objCls))
		NPCMode[objNum] = AI_FEAR;
}

/*"..."*/
static C_1944_2A15(int spellnum) {
	register int objNum, x;
	int y, bp_04, bp_02;

	MagicalHalo(Party[Active], 0);
	bp_04 = Party[Active];
	for(x = MapX - 7; x <= MapX + 7; x ++) {
		for(y = MapY - 7; y <= MapY + 7; y ++) {
			if((objNum = FindLoc(x, y, MapZ)) >= 0) {
				switch(spellnum) {
					case SPELL_29:/*"repel undead"*/
						if(IsMonster_0080(GetMonsterClass(GetType(objNum))) && !COMBAT_TestIntel(bp_04, objNum))
							NPCMode[objNum] = AI_FEAR;
					break;
					case SPELL_47:/*"reveal"*/
						if(IsInvisible(objNum))
							ClrInvisible(objNum);
						bp_02 = FindInvType(objNum, OBJ_102, 0);
						if(bp_02 != -1 && GetCoordUse(bp_02) == EQUIP)
							SetCoordUse(bp_02, INVEN);
					break;
					case SPELL_63:/*"fear"*/
						C_1944_29AA(objNum);
					break;
					case SPELL_52:/*"confuse"*/
						if(objNum < 0x100 && GetAlignment(objNum) == EVIL && !COMBAT_TestIntel(bp_04, objNum)) {
							NPCMode[objNum] = AI_ASSAULT;
							SetAlignment(objNum, NEUTRAL);
						}
					break;
				}
			}
		}
	}
	CON_printf(/*14BA*/"\n%s\n", D_0DDC[0]);
	MUS_0065(14, 0);
}

/*"create food"*/
static C_1944_2B9A(int spellNum) {
	int si;

	C_1944_0085(MK_CIRCLE(spellNum));
	MagicalHalo(Party[Active], 0);
	si = OSI_rand(1, 10);
	GiveObj(Party[Active], OBJ_081, si);
	CON_printf(/*15AE*/"\nCreate %d food.\n", si);
}

/*"summon"*/
static C_1944_2BFD(int spellNum, int bp06) {
	int si, di;
	int bp_08, bp_06, bp_04, bp_02;

	di = OSI_rand(0, 15);
	if(di <= 3)
		bp_08 = OBJ_173;
	else if(di <= 6)
		bp_08 = OBJ_156;
	else if(di <= 9)
		bp_08 = OBJ_169;
	else if(di <= 12)
		bp_08 = OBJ_158;
	else
		bp_08 = OBJ_166;
	C_1944_0085(MK_CIRCLE(spellNum));
	MagicalHalo(bp06, 0);
	if(spellNum == SPELL_31)
		si = EGG_generate(bp_08, MapX, MapY, MapZ);
	else if(spellNum == SPELL_77)
		si = EGG_generate(OBJ_16F, MapX, MapY, MapZ);
	for(di = 0; di < 8; di ++) {
		bp_06 = OSI_rand(0, 10);
		bp_04 = OSI_rand(0, 10);
		if(C_1E0F_000F(si, MapX + bp_06 - 5, MapY + bp_04 - 5)) {
			MoveObj(si, MapX + bp_06 - 5, MapY + bp_04 - 5, MapZ);
			if(spellNum == SPELL_77 && COMBAT_TestIntel(bp06, si)) {
				bp_02 = CHAOTIC;
				CON_printf(/*15C0*/"\nHostile daemon summoned.\n");
				MUS_0065(13, 0);
			} else {
				bp_02 = GetAlignment(bp06);
				CON_printf(/*14BA*/"\n%s\n", D_0DDC[0]);
				MUS_0065(14, 0);
			}
			SetAlignment(si, bp_02);
			NPCMode[si] = AI_ASSAULT;
			ScreenFade = 1;
			C_1100_0306();
			OtherAnimations();
			return;
		}
	}
	DeleteObj(si);
	MUS_0065(13, 0);
}

/*"telekinesis"*/
static C_1944_2DA7(int aFlag) {
	int si, di;
	int bp_0a, bp_08, bp_06, bp_04, bp_02;

	bp_0a = Party[Active];
	if(IsPlrControl(Party[Active]))
		CON_printf(/*149D*/"%s", D_0DDC[8]);
	if(!aFlag) {
		MouseMode = 1;
		CON_getch();
	}
	if(Selection.obj == -1 || Selection.x == -1 || Selection.y == -1) {
		CON_printf(/*14B6*/"%s\n", D_0DDC[5]);
	} else {
		CON_printf((char *)GetObjectString(Selection.obj));
		CON_printf(/*14A6*/"\n");
	}
	MagicalHalo(Party[Active], 0);
	if(Selection.x != -1 && Selection.y != -1)
		bp_02 = COMBAT_Missile(bp_0a, &Selection.x, &Selection.y, 8, TIL_17F);
	if(Selection.obj == -1 || Selection.obj < 0x100 || Selection.x == -1 || Selection.y == -1) {
		CON_printf(/*14BA*/"\n%s\n", D_0DDC[2]);
		MUS_0065(13, 0);
		return;
	}
	if(bp_02) {
		si = GetType(Selection.obj);
		if(si == OBJ_120) {
			C_27A1_433D(Selection.obj);
			return;
		}
		if(si == OBJ_10C) {
			C_27A1_4479(Selection.obj);
			return;
		}
		if(TypeWeight[si] == 0 && Selection.obj >= 0x100) {
			CON_printf(/*15DB*/"\nNot Possible\n");
			return;
		}
		MagicalHalo(Selection.obj, 1);
		di = GetX(Selection.obj);
		bp_04 = GetY(Selection.obj);
		MouseMode = 0;
		D_04CA = ((di - MapX) << 4) + 88;
		D_04CB = ((bp_04 - MapY) << 4) + 88;
		CON_printf(/*15EA*/"Direction-");
		if(CON_getch() != CMD_80) {
			CON_printf(/*15F5*/"nowhere\n");
		} else {
			CON_printf(/*14B6*/"%s\n", DirNames[AdvanceDir]);
			bp_08 = DirIncrX[AdvanceDir] + di;
			bp_06 = DirIncrY[AdvanceDir] + bp_04;
			if(C_27A1_1DAB(Selection.obj, di, bp_04, AdvanceDir)) {
				CON_printf(/*14BA*/"\n%s\n", D_0DDC[4]);
			} else {
				MoveObj(Selection.obj, bp_08, bp_06, MapZ);
				if(si == OBJ_0DD)
					SetFrame(Selection.obj, (AdvanceDir >> 1) & 3);
				C_1100_0306();
			}
		}
		D_04CA = 88;
		D_04CB = 88;
		C_0C9C_01FB();
	} else {
		CON_printf(/*14BA*/"\n%s\n", D_0DDC[4]);
		MUS_0065(13, 0);
	}
}

/*"gate travel"*/
static C_1944_305A() {
	int phase;
	char bp_06[5];

	CON_printf(/*15FE*/"To phase ");
	CON_gets(bp_06, 4);
	CON_printf(/*14A6*/"\n");
	MagicalHalo(Party[Active], 0);
	phase = C_27A1_0205(bp_06, 0);
	if(
		phase == -1 ||
		phase == -2 ||
		phase == 0 ||
		phase > 8 ||
		(D_2C74[phase - 1][0] == 0 && D_2C74[phase - 1][1] == 0 && D_2C74[phase - 1][2] == 0)
	) {
		MUS_0065(13, 0);
		return;
	}
	GateTravel(phase - 1);
}

/*"x-ray"*/
C_1944_310C(int spellNum) {
	int si;

	C_1944_0085(MK_CIRCLE(spellNum));
	MagicalHalo(Party[Active], 0);
	D_05E8 = 1;
	C_1100_0306();
	for(si = 0; si < 100; si ++)
		OtherAnimations();
	D_05E8 = 0;
	C_1100_0306();
	OtherAnimations();
	CON_printf(/*14BA*/"\n%s\n", D_0DDC[7]);
}

/*TODO
extern unsigned char __NPC_1Hand;
extern unsigned char __ShowNPCInven;*/
#define __NPC_1Hand *(unsigned char *)(&NPC_1Hand)
#define __ShowNPCInven *(unsigned char *)(&ShowNPCInven)

/*"pickpocket"*/
static C_1944_3175(int aFlag) {
	int si, di;
	int bp_0e, bp_0c, bp_0a, bp_08, bp_06, bp_04, bp_02;

	bp_06 = 0;
	bp_04 = 0;
	si = Party[Active];
	if(IsPlrControl(Party[Active]))
		CON_printf(/*149D*/"%s", D_0DDC[9]);
	if(!aFlag) {
		MouseMode = 1;
		bp_06 = (char)SelectMode;
		CON_getch();
		SelectMode = bp_06;
	}
	if(Selection.obj == -1 || Selection.x == -1 || Selection.y == -1) {
		CON_printf(/*14B6*/"%s\n", D_0DDC[5]);
	} else {
		CON_printf((char *)GetObjectString(Selection.obj));
		CON_printf(/*14A6*/"\n");
	}
	MagicalHalo(Party[Active], 0);
	if(Selection.x != -1 && Selection.y != -1)
		bp_0e = COMBAT_Missile(si, &Selection.x, &Selection.y, 8, TIL_18B);
	if(Selection.obj == -1 || Selection.obj >= 0x100 || Selection.x == -1 || Selection.y == -1) {
		CON_printf(/*14BA*/"\n%s\n", D_0DDC[2]);
		MUS_0065(13, 0);
		return;
	}
	if(bp_0e) {
		D_EBB5 = 1;
		bp_0c = GetX(si);
		bp_0a = GetY(si);
		di = Selection.obj;
		__ShowNPCInven = Selection.obj;
		C_155D_1065(di);
		StatusDisplay = CMD_92;
		CON_printf(/*1608*/"Which object: ");
		if(SelectMode != 0) {
			SelectMode = 2;
			C_0C9C_1AE5(1);
		}
		bp_08 = CON_getch();
		D_EBB5 = 0;
		if(bp_08 != CMD_8E || Selection.obj == -1) {
			CON_printf(/*14B6*/"%s\n", D_0DDC[5]);
		} else {
			CON_printf((char *)GetObjectString(Selection.obj));
			CON_printf(/*14A6*/"\n");
		}
		if(Selection.obj == -1 || Selection.obj < 0x100 || GetCoordUse(Selection.obj) == LOCXYZ) {
			CON_printf(/*14BA*/"\n%s\n", D_0DDC[2]);
			MUS_0065(13, 0);
		} else {
			bp_02 = GetType(Selection.obj);
			bp_04 = C_155D_1666(Selection.obj, di);
			if(bp_04 == 0) {
				C_155D_0748(si);
				if(GetWeight(Selection.obj) + WeightInven + WeightEquip > STREN[si] * 20) {
					CON_printf(/*1617*/"\nCan't carry!\n");
				} else {
					COMBAT_Missile(di, &bp_0c, &bp_0a, 8, TILE_FRAME(Selection.obj));
					C_155D_16E7(Selection.obj, di);
					if(QuanType(GetType(Selection.obj))) {
						GiveObj(si, bp_02, Amount[Selection.obj]);
						TakeObj(di, bp_02, Amount[Selection.obj]);
					} else {
						InsertObj(Selection.obj, si, INVEN);
					}
					C_155D_1022(si);
					CON_printf(/*14BA*/"\n%s\n", D_0DDC[0]);
					KARMA -= 5;
				}
				D_04B3 = Active;
				D_07CE = 0;
				StatusDirty ++;
			}
		}
		__ShowNPCInven = 0;
		__NPC_1Hand = 0;
	} else {
		CON_printf(/*14BA*/"\n%s\n", D_0DDC[4]);
		MUS_0065(13, 0);
	}
}

/*"insect swarm"*/
static C_1944_34CF(int spellNum) {
	int si, di;
	int bp_06, bp_04, bp_02;

	C_1944_0085(MK_CIRCLE(spellNum));
	MagicalHalo(Party[Active], 0);
	for(bp_06 = 0; bp_06 < 8; bp_06 ++) {
		si = EGG_generate(OBJ_157, MapX, MapY, MapZ);
		for(di = 0; di < 8; di ++) {
			bp_04 = OSI_rand(0, 10);
			bp_02 = OSI_rand(0, 10);
			if(C_1E0F_000F(si, MapX + bp_04 - 5, MapY + bp_02 - 5)) {
				MoveObj(si, MapX + bp_04 - 5, MapY + bp_02 - 5, MapZ);
				SetAlignment(si, GetAlignment(Party[Active]));
				NPCMode[si] = AI_ASSAULT;
				break;
			}
		}
		if(di == 8)
			DeleteObj(si);
	}
	CON_printf(/*14BA*/"\n%s\n", D_0DDC[0]);
	MUS_0065(14, 0);
}

/*"web"*/
static C_1944_35EC(int aFlag) {
	int si, di;
	int bp_08, bp_06;
	struct coord xyz;

	if(IsPlrControl(Party[Active]))
		CON_printf(/*149D*/"%s", D_0DDC[10]);
	if(!aFlag) {
		MouseMode = 1;
		CON_getch();
	}
	CON_printf(/*14A6*/"\n");
	bp_08 = Party[Active];
	MagicalHalo(bp_08, 0);
	if(Selection.x == -1 || Selection.y == -1) {
		CON_printf(/*14BA*/"\n%s\n", D_0DDC[2]);
		MUS_0065(13, 0);
		return;
	}
	if(C_2FC1_131E(bp_08, Selection.x, Selection.y)) {
		for(si = Selection.x - 1; si <= Selection.x + 1; si ++) {
			for(di = Selection.y - 1; di <= Selection.y + 1; di ++) {
				SetCoordXYZ(xyz, 0, 0, MapZ);
				bp_06 = AddObj(0, xyz, OBJ_035, 0, 1);
				if(C_1E0F_000F(bp_06, si, di))
					MoveObj(bp_06, si, di, MapZ);
				else
					DeleteObj(bp_06);
			}
		}
		C_1100_0306();
		CON_printf(/*14BA*/"\n%s\n", D_0DDC[0]);
		MUS_0065(14, 0);
	} else {
		CON_printf(/*14BA*/"\n%s\n", D_0DDC[4]);
		MUS_0065(13, 0);
	}
}

/*"enchant"*/
static C_1944_3750(int objNum) {
	int j, i;
	unsigned char ch;
	int bp_0a, spellbook_0, spellbook_1, bp_04, spellNum;

	bp_0a = 0;
	spellbook_0 = 0;
	spellbook_1 = 0;
	StatusDisplay = CMD_92;
	C_155D_1065(Active);
	CON_printf(/*149D*/"%s", D_0DDC[8]);
	if(!objNum) {
		MouseMode = 1;
		CON_getch();
	}
	if(Selection.obj == -1) {
		CON_printf(/*14B6*/"%s\n", D_0DDC[5]);
	} else {
		CON_printf((char *)GetObjectString(Selection.obj));
		CON_printf(/*14A6*/"\n");
	}
	if(
		(Selection.obj == -1 && Selection.obj >= 0x100 && GetCoordUse(Selection.obj) != LOCXYZ) ||
		GetType(Selection.obj) != OBJ_04E
	) {
		CON_printf(/*14BA*/"\n%s\n", D_0DDC[2]);
		MUS_0065(13, 0);
		return;
	}
	bp_04 = Selection.obj;
	for(i = 0, j = FindInv(bp_04); j >= 0; i ++, j = NextInv());
	if(i == 10) {
		CON_printf(/*1626*/"\nIt's full\n");
		MUS_0065(13, 0);
		return;
	}
	for(j = FindInv(Party[Active]); j >= 0; j = NextInv()) {
		if(GetCoordUse(j) == EQUIP && GetType(j) == OBJ_039) {
			if(!bp_0a) {
				bp_0a = OBJ_039;
				spellbook_0 = j;
			} else {
				spellbook_1 = j;
			}
		}
	}
	Spellbook[0] = spellbook_0;
	Spellbook[1] = spellbook_1;
	SPELL_RefreshList();
	SPELL_DrawBG();
	SPELL_RefreshPage();
	StatusDisplay = CMD_A1;
	CON_printf(/*1632*/"With: ");
	ch = CON_getch();
	MagicalHalo(Party[Active], 0);
	if(ch != CMD_8E || Selection.obj == -1) {
		CON_printf(/*1639*/"none\n");
	} else {
		spellNum = D_EBB6[Selection.obj];
		i = 0;
		ch = SpellCode[spellNum][i++];
		CON_printf(/*149D*/"%s", D_0BF0[ch - 'A']);
		while(ch = SpellCode[spellNum][i++])
			CON_printf(/*149D*/" %s", D_0BF0[ch - 'A']);
		GiveObj(bp_04, OBJ_150, QualQuan(spellNum, 1));
	}
	CON_printf(/*14A6*/"\n");
}

/*"wizard eye"*/
C_1944_39A1(int x, int y, int aFlag) {
	int numTile, di;
	int bp_0c, sav_x, sav_y, bp_06;
	struct coord xyz;

	bp_0c = 40;
	numTile = 0;
	di = 0;
	if(aFlag) {
		C_1944_0085(6);
		MagicalHalo(Party[Active], 0);
	}
	D_17AE = 1;
	sav_x = MapX;
	sav_y = MapY;
	MapX = x;
	MapY = y;
	SetCoordXYZ(xyz, MapX, MapY, MapZ);
	bp_06 = AddObj(0, xyz, OBJ_034, 0, 1);
	D_05E8 = 1;
	ScreenFade = 1;
	ShowObjects();
	C_101C_054C();
	OtherAnimations();
	numTile = GetTileAtXYZ(MapX, MapY, MapZ);
	while(bp_0c >= 0 && di != 27 && numTile < TIL_0FC) {
		MouseMode = 0;
		D_17AE = 0;
		di = CON_getch();
		if(di == CMD_80) {
			MapX += DirIncrX[AdvanceDir];
			MapY += DirIncrY[AdvanceDir];
			MoveObj(bp_06, MapX, MapY, MapZ);
			ShowObjects();
			C_101C_054C();
			OtherAnimations();
			bp_0c --;
			numTile = GetTileAtXYZ(MapX, MapY, MapZ);
		}
	}
	DeleteObj(bp_06);
	D_05E8 = 0;
	MapX = sav_x;
	MapY = sav_y;
	ShowObjects();
	C_101C_054C();
	ScreenFade = 1;
	OtherAnimations();
	CON_printf(/*14BA*/"\n%s\n", D_0DDC[7]);
	if(StatusDisplay == CMD_92)
		C_155D_1022(Party[D_04B3]);
}

/*"tremor"*/
static C_1944_3B2D() {
	int si, di;
	int bp_04, bp_02;

	MagicalHalo(Party[Active], 0);
	ShakeScreen(20, 3);
	for(di = MapX - 5; di <= MapX + 5; di ++) {
		for(bp_04 = MapY - 5; bp_04 <= MapY + 5; bp_04 ++) {
			si = FindLoc(di, bp_04, MapZ);
			if(
				si >= 0 && si < 0x100 && GetAlignment(si) == EVIL &&
				!IsMonster_0100(GetMonsterClass(GetType(si)))
			) {
				bp_02 = LooseHP(si, OSI_rand(1, 30));
				AddXP(Party[Active], bp_02);
				CON_printf(/*14A6*/"\n");
				COMBAT_displaySeverity(si);
			}
		}
	}
	CON_printf(/*14BA*/"\n%s\n", D_0DDC[0]);
}

/*"seance"*/
static C_1944_3C30(int aFlag) {
	int si;

	CON_printf(/*149D*/"%s", D_0DDC[9]);
	if(!aFlag) {
		MouseMode = 1;
		CON_getch();
	}
	CON_printf(/*14A6*/"\n");
	MagicalHalo(Party[Active], 0);
	si = GetType(Selection.obj);
	if(Selection.obj == -1 || (si != OBJ_153 && si != OBJ_162 && si != OBJ_155 && si != OBJ_154)) {
		CON_printf(/*14BA*/"\n%s\n", D_0DDC[2]);
		MUS_0065(13, 0);
		return;
	}
	CON_printf((char *)GetObjectString(Selection.obj));
	CON_printf(/*14A6*/"\n");
	SeanceFlag = 1;
	TALK_initTalk(Active, GetQual(Selection.obj));
	SeanceFlag = 0;
}

/*"hail storm"*/
static C_1944_3D0F(int aFlag) {
	if(IsPlrControl(Party[Active]))
		CON_printf(/*149D*/"%s", D_0DDC[10]);
	if(!aFlag) {
		MouseMode = 1;
		CON_getch();
	}
	CON_printf(/*14A6*/"\n");
	MagicalHalo(Party[Active], 0);
	if(Selection.x == -1 || Selection.y == -1) {
		CON_printf(/*14BA*/"\n%s\n", D_0DDC[2]);
		MUS_0065(13, 0);
		return;
	}
	C_2FC1_13B0(Selection.x, Selection.y);
	CON_printf(/*14BA*/"\n%s\n", D_0DDC[0]);
}

/*"{flame|poison|energy|death} wind"*/
static C_1944_3DB8(int spellNum, int bp08, int aFlag) {
	int si, di;
	int far *bp_04;

	si = Party[Active];
	if(IsPlrControl(Party[Active]))
		CON_printf(/*149D*/"%s", D_0DDC[10]);
	if(!aFlag) {
		MouseMode = 1;
		CON_getch();
	}
	CON_printf(/*14A6*/"\n");
	MagicalHalo(Party[Active], 0);
	if(
		Selection.x == -1 || Selection.y == -1 ||
		(GetX(si) == Selection.x && GetY(si) == Selection.y)
	) {
		CON_printf(/*14BA*/"\n%s\n", D_0DDC[2]);
		MUS_0065(13, 0);
		return;
	}
	for(bp_04 = MagicWind(si, Selection.x, Selection.y, bp08); *bp_04 >= 0; bp_04 ++) {
		if(*bp_04 < 0x100) {
			switch(spellNum) {
				case SPELL_53: C_1944_1232(*bp_04); break;
				case SPELL_57: C_1944_0B32(si, *bp_04); break;
				case SPELL_62:/*"energy wind"*/
					di = LooseHP(*bp_04, OSI_rand(1, 30));
					AddXP(Party[Active], di);
					COMBAT_displaySeverity(*bp_04);
					C_2337_1C1E(*bp_04, Party[Active], 1);
				break;
				case SPELL_71: C_1944_0D46(si, *bp_04); break;
			}
		}
	}
}

/*"wing strike"*/
C_1944_3F6C() {
	int objNum, h_dir;
	int x, y;
	int deltax_1, deltax_2;
	int i;
	int dragon_0, dragon_1;
	int hitPts;
	struct coord xyz;
	int hasHit;

	hasHit = 0;
	MagicalHalo(Party[Active], 0);
	for(objNum = 0; objNum < 0x100; objNum ++) {
		if(
			ObjShapeType[objNum] && !IsDead(objNum) && !IsInvisible(objNum) &&
			(GetAlignment(objNum) == EVIL || GetAlignment(objNum) == CHAOTIC)
		) {
			x = GetX(objNum);
			y = GetY(objNum);
			if(
				MapX - 5 <= x && MapX + 5 >= x &&
				MapY - 5 <= y && MapY + 5 >= y
			) {
				deltax_1 = x - (MapX - 5);
				deltax_2 = (MapX + 5) - x;
				if(deltax_1 > deltax_2)
					h_dir = 0;
				else
					h_dir = 1;
				SetCoordXYZ(xyz, MapX - D_0EF2[h_dir] * 5, y - 1, MapZ);
				dragon_0 = AddObj(0, xyz, OBJ_19B, D_0EF4[h_dir], 1);
				SetCoordXYZ(xyz, MapX - D_0EF2[h_dir] * 5, y, MapZ);
				dragon_1 = AddObj(0, xyz, OBJ_19B, D_0EF6[h_dir], 1);
				C_1100_0306();
				OtherAnimations();
				for(i = 1; i < 11; i ++) {
					SetFrame(dragon_0, GetFrame(dragon_0) ^ 1);
					SetFrame(dragon_1, GetFrame(dragon_1) ^ 1);
					MoveObj(dragon_0, MapX - (5 - i) * D_0EF2[h_dir], y - 1, MapZ);
					MoveObj(dragon_1, MapX - (5 - i) * D_0EF2[h_dir], y, MapZ);
					C_1100_0306();
					OtherAnimations();
					if(MapX - (5 - i) * D_0EF2[h_dir] == x) {
						hitPts = LooseHP(objNum, OSI_rand(1, 20));
						AddXP(Party[Active], hitPts);
						COMBAT_displaySeverity(objNum);
					}
				}
				hasHit = 1;
				DeleteObj(dragon_0);
				DeleteObj(dragon_1);
			}
		}
	}
	if(!hasHit) {
		CON_printf(/*1643*/"\nNo foes.\n");
		MUS_0065(13, 0);
	}
}

/*some invisibility?*/
static C_1944_4260() {
	int si;

	si = Party[Active];
	if(!IsInvisible(si)) {
		SetInvisible(si);
		MagicalHalo(si, 1);
		CON_printf(/*14BA*/"\n%s\n", D_0DDC[0]);
	} else {
		CON_printf(/*14BA*/"\n%s\n", D_0DDC[2]);
	}
}

/*"locate"*/
C_1944_42AC() {
	int si, di;
	int bp_04, bp_02;

	si = MapX;
	di = MapY;
	if(MapZ) {
		si <<= 2;
		di <<= 2;
	}
	bp_04 = (di - 0x168) >> 3;
	bp_02 = (si - 0x130) >> 3;
	CON_printf(/*164E*/"\n%d{", abs(bp_04));
	if(bp_04 > 0)
		CON_printf(/*0F38 + 1*/"S");
	else
		CON_printf(/*1653*/"N");
	CON_printf(/*1655*/", %d{", abs(bp_02));
	if(bp_02 > 0)
		CON_printf(/*165B*/"E\n");
	else
		CON_printf(/*165E*/"W\n");
}

/*"wind change"*/
C_1944_435B() {
	CON_printf(/*15EA*/"Direction-");
	MouseMode = 0;
	if(CON_getch() != CMD_80)
		CON_printf(/*1661*/"nowhere.\n");
	else
		CON_printf(/*155D + 16h*/"%s.\n", DirNames[AdvanceDir]);
	WindDir = AdvanceDir;
}

/*"armageddon"*/
C_1944_43A3() {
	int si;

	for(si = 0; si < 0xff; si ++) {
		if(si != 1 && si != 5)
			DeleteObj(si);
	}
	C_1100_0306();
	OtherAnimations();
	IsArmageddon = 1;
	PartySize = 1;
}

C_1944_43D5(int spellNum, int aFlag) {
	CON_printf(/*14A6*/"\n");
	switch(spellNum) {
		case SPELL_04:/*"douse"*/ C_1944_12FC(spellNum, TIL_18B, 8, -1, aFlag); break;
		case SPELL_19:/*"vanish"*/
		case SPELL_21:/*"dispel field"*/
		case SPELL_30:/*"animate"*/
		case SPELL_58:/*"replicate"*/ C_1944_12FC(spellNum, TIL_17F, 8, -1, aFlag); break;
		case SPELL_08:/*"ignite"*/ C_1944_12FC(spellNum, TIL_18D, 8, -1, aFlag); break;
		case SPELL_32:/*"disable"*/ C_1944_1DF4(spellNum, 8, 1, aFlag); break;
		case SPELL_75:/*"resurrect"*/ C_1944_1C51(aFlag); break;
		case SPELL_09:/*"light"*/
			MagicalHalo(Party[Active], 0);
			C_1944_0085(MK_CIRCLE(spellNum));
			SpellFx[0] = 100;
			C_0A33_1355(0);
		break;
		case SPELL_07:/*"help"?*/
			C_1944_0085(MK_CIRCLE(spellNum));
			C_101C_0ACC();
		break;
		case SPELL_01:/*"detect magic"*/
		case SPELL_02:/*"detech trap"*/
		case SPELL_16:/*"trap"*/
		case SPELL_17:/*"unlock magic"*/
		case SPELL_18:/*"untrap"*/
		case SPELL_24:/*"lock"*/ C_1944_1DF4(spellNum, 8, -1, aFlag); break;
		case SPELL_76:/*"slime"*/ C_1944_22F4(); break;
		case SPELL_00:/*"create food"*/ C_1944_2B9A(spellNum); break;
		case SPELL_13:/*"reappear"*/ C_1944_23F2(aFlag); break;
		case SPELL_14:/*"sleep"*/ C_1944_12FC(spellNum, TIL_17D, 8, 1, aFlag); break;
		case SPELL_11:/*"magic arrow"*/ C_1944_12FC(spellNum, TIL_236, 8, 1, aFlag); break;
		case SPELL_22:/*"fireball"*/ C_1944_12FC(spellNum, TIL_17E, 8, 1, aFlag); break;
		case SPELL_44:/*"lightning"*/
		case SPELL_60:/*"chain bolt"*/ C_1944_12FC(spellNum, TIL_188, 8, 1, aFlag); break;
		case SPELL_15:/*"telekinesis"*/ C_1944_2DA7(aFlag); break;
		case SPELL_10:/*"infravision"*/
			MagicalHalo(Party[Active], 0);
			C_1944_0085(MK_CIRCLE(spellNum));
			SpellFx[1] = 20;
		break;
		case SPELL_25:/*"mass awaken"*/
		case SPELL_36:/*"mass dispel"*/
		case SPELL_55:/*"mass protect"*/
		case SPELL_66:/*"mass curse"*/ C_1944_256A(spellNum, TIL_18B, TIL_17F, aFlag); break;
		case SPELL_67:/*"mass invis"*/
		case SPELL_73:/*"mass charm"*/ C_1944_256A(spellNum, TIL_188, TIL_18A, aFlag); break;
		case SPELL_74:/*"mass kill"*/ C_1944_256A(spellNum, TIL_188, TIL_18C, aFlag); break;
		case SPELL_26:/*"mass sleep"*/ C_1944_256A(spellNum, TIL_18B, TIL_17D, aFlag); break;
		case SPELL_41:/*"explosion"*/ C_1944_256A(spellNum, TIL_189, TIL_17E, aFlag); break;
		case SPELL_29:/*"repel undead"*/
		case SPELL_47:/*"reveal"*/
		case SPELL_52:/*"confuse"*/
		case SPELL_63:/*"fear"*/ C_1944_2A15(spellNum); break;
		case SPELL_12:/*"poison"*/ C_1944_12FC(spellNum, TIL_17C, 8, 1, aFlag); break;
		case SPELL_23:/*"great light"*/
			MagicalHalo(Party[Active], 0);
			C_1944_0085(MK_CIRCLE(spellNum));
			SpellFx[0] = 255;
			C_0A33_1355(0);
		break;
		case SPELL_27:/*"peer"*/
			MagicalHalo(Party[Active], 0);
			C_1944_0085(MK_CIRCLE(spellNum));
			C_27A1_319F(0);
			CON_printf(/*14BA*/"\n%s\n", D_0DDC[7]);
		break;
		case SPELL_33:/*"fire field"*/ C_1944_27AF(spellNum, TIL_17E, aFlag); break;
		case SPELL_37:/*"poison field"*/ C_1944_27AF(spellNum, TIL_17C, aFlag); break;
		case SPELL_35:/*"locate"*/
			MagicalHalo(Party[Active], 0);
			C_1944_0085(MK_CIRCLE(spellNum));
			C_1944_42AC();
		break;
		case SPELL_38:/*"sleep field"*/ C_1944_27AF(spellNum, TIL_17D, aFlag); break;
		case SPELL_31:/*"conjure"*/
		case SPELL_77:/*"summon"*/ C_1944_2BFD(spellNum, Party[Active]); break;
		case SPELL_46:/*"pickpocket"*/ C_1944_3175(aFlag); break;
		case SPELL_39:/*"wind change"*/ C_1944_435B(); break;
		case SPELL_40:/*"energy field"*/ C_1944_27AF(spellNum, TIL_17F, aFlag); break;
		case SPELL_42:/*"insect swarm"*/ C_1944_34CF(spellNum); break;
		case SPELL_49:/*"x-ray"*/ C_1944_310C(spellNum); break;
		case SPELL_43:/*"invisibility"*/ C_1944_12FC(spellNum, TIL_18B, 8, 1, aFlag); break;
		case SPELL_03:/*"dispel magic"*/
		case SPELL_05:/*"harm"*/
		case SPELL_06:/*"heal"*/
		case SPELL_20:/*"curse"*/
		case SPELL_28:/*"protection"*/
		case SPELL_34:/*"great heal"*/
		case SPELL_45:/*"paralyze"*/
		case SPELL_50:/*"charm"*/
		case SPELL_51:/*"clone"*/ C_1944_12FC(spellNum, TIL_17F, 8, 1, aFlag); break;
		case SPELL_65:/*"kill"*/ C_1944_12FC(spellNum, TIL_18C, 8, 1, aFlag); break;
		case SPELL_53:/*"flame wind"*/ C_1944_3DB8(spellNum, TIL_18D, aFlag); break;
		case SPELL_57:/*"poison wind"*/ C_1944_3DB8(spellNum, TIL_17C, aFlag); break;
		case SPELL_62:/*"energy wind"*/ C_1944_3DB8(spellNum, TIL_188, aFlag); break;
		case SPELL_71:/*"death wind"*/ C_1944_3DB8(spellNum, TIL_18C, aFlag); break;
		case SPELL_59:/*"web"*/ C_1944_35EC(aFlag); break;
		case SPELL_56:/*"negate magic"*/
			MagicalHalo(Party[Active], 0);
			C_1944_0085(MK_CIRCLE(spellNum));
			SpellFx[13] = 20;
		break;
		case SPELL_61:/*"enchant"*/ C_1944_3750(aFlag); break;
		case SPELL_69:/*"wizard eye"*/ C_1944_39A1(MapX, MapY, 1); break;
		case SPELL_64:/*"gate travel"*/ C_1944_305A(); break;
		case SPELL_78:/*"time stop"*/
			MagicalHalo(Party[Active], 0);
			C_1944_0085(MK_CIRCLE(spellNum));
			SpellFx[14] = 10;
		break;
		case SPELL_72:/*"eclipse"*/
			MagicalHalo(Party[Active], 0);
			C_1944_0085(MK_CIRCLE(spellNum));
			SpellFx[15] = 20;
		break;
		case SPELL_79:/*"tremor"*/ C_1944_3B2D(); break;
		case SPELL_54:/*"hail storm"*/ C_1944_3D0F(aFlag); break;
		case SPELL_68:/*"wing strike"*/ C_1944_3F6C(); break;
		case SPELL_80:/*some invisibility?*/ C_1944_4260(); break;
		case SPELL_48:/*"seance"*/ C_1944_3C30(aFlag); break;
		case SPELL_70:/*"armageddon"*/
			C_1944_0085(MK_CIRCLE(spellNum));
			C_1944_43A3();
		break;
	}
	C_1100_0306();
	OtherAnimations();
}

static C_1944_4926(int bp08, int bp06) {
	int i, di;
	unsigned char ch;
	int spellNum, circle, aFlag, bp_02;

	aFlag = 0;
	bp_02 = 0;
	Spellbook[0] = bp08;
	Spellbook[1] = bp06;
	if(SpellFx[13]) {
		CON_printf(/*166B*/"No magic at this time!\n");
		return;
	}
	di = Party[Active];
	SPELL_RefreshList();
	for(i = 0; i < 8; i ++) {
		if(SpellList[i])
			break;
	}
	if(i == 8) {
		CON_printf(/*1683*/"No spells in the spellbook!\n");
		return;
	}
	SPELL_DrawBG();
	SPELL_RefreshPage();
	D_EBAB = StatusDisplay;
	StatusDisplay = CMD_A1;
	ch = CON_getch();
	if((ch != CMD_8E || Selection.obj == -1) && Selection.x == -1) {
		CON_printf(/*16A0*/"nothing\n");
		StatusDisplay = D_EBAB;
		StatusDirty ++;
		return;
	}
	if(Selection.x != -1 && Selection.y != -1) {
		spellNum = D_EBB3;
		aFlag = 1;
	} else {
		spellNum = D_EBB6[Selection.obj];
	}
	if(SelectMode != 0 && D_04B5) {
		CON_printf(/*16A9*/"(%s)", SpellName[spellNum]);
		D_04B5 = 0;
	} else {
		CON_printf(/*16AE*/"%s\n\"", SpellName[spellNum]);
		i = 0;
		ch = SpellCode[spellNum][i++];
		CON_printf(/*149D*/"%s", D_0BF0[ch - 'A']);
		while(ch = SpellCode[spellNum][i++])
			CON_printf(/*163F*/" %s", D_0BF0[ch - 'A']);
		CON_printf(/*16AE + 3*/"\"");
	}
	circle = MK_CIRCLE(spellNum);
	if(!aFlag) {
		CON_setClip(&D_B6B5[1]);
		for(i = 0; i < 8; i ++) {
			if(D_EBBE[i] & 0x80) {
				CON_gotoxy(0, i + 1);
				CON_putch0(' ');
			}
		}
		CON_gotoxy(0, Selection.obj + 1);
		CON_putch0(0x1a);
		CON_setClip(&D_B6B5[3]);
		if(Spellbook[0])
			SetQual(Spellbook[0], spellNum);
		if(Spellbook[1])
			SetQual(Spellbook[1], spellNum);
	}
	StatusDisplay = D_EBAB;
	StatusDirty ++;
	RefreshStatus();
	if(Level[di] < circle) {
		MUS_0065(13, 0);
		CON_printf(/*16B3*/"\n\nYour level is not high enough.\n");
	} else if(MAGIC[di] < circle) {
		MUS_0065(13, 0);
		CON_printf(/*16D5*/"\n\nNot enough magic points.\n");
	} else if((D_EBBE[C_1944_06CA(spellNum)] & 0x7f) == 0) {
		MUS_0065(13, 0);
		CON_printf(/*16F1*/"\n\nNo Reagents.\n");
	} else {
		TakeReagents(di, spellNum, Selection.obj);
		MAGIC[di] -= circle;
		SubMov(di, circle * 3 + 10);
		C_1944_43D5(spellNum, aFlag);
		bp_02 = 1;
	}
	if(bp_02) {
		StatusDisplay = D_EBAB;
		StatusDirty ++;
		RefreshStatus();
	}
}

/*"cast"?*/
C_1944_4C2F() {
	int si, type0, type1, objNum0, objNum1;

	type0 = 0;
	type1 = 0;
	objNum0 = 0;
	objNum1 = 0;
	for(si = FindInv(Party[Active]); si >= 0; si = NextInv()) {
		if(GetCoordUse(si) == EQUIP && GetType(si) == OBJ_039) {
			if(type0 == 0) {
				type0 = OBJ_039;
				objNum0 = si;
			} else {
				type1 = OBJ_039;
				objNum1 = si;
			}
		}
	}
	if(type0 == OBJ_039 || type1 == OBJ_039)
		C_1944_4926(objNum0, objNum1);
	else
		CON_printf(/*1701*/"No spellbook is readied.\n");
}
