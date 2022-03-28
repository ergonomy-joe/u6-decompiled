/*
 * Ultima VI - The False Prophet
 * Copyright (c) 1990 Origin Systems, Inc. and Richard Gariott
 * reverse-coded by Ergonomy Joe in 2013
 */
/*
	talkdr:conversation engine module
*/
#include "u6.h"

#define OP_SUP            0x81
#define OP_SUPE           0x82
#define OP_INF            0x83
#define OP_INFE           0x84
#define OP_DIF            0x85
#define OP_EQU            0x86

#define OP_ADD            0x90
#define OP_SUB            0x91
#define OP_MUL            0x92
#define OP_DIV            0x93
#define OP_OR             0x94
#define OP_AND            0x95

#define OP_CANCARRY       0x9a
#define OP_WEIGHT         0x9b
#define OP_GETHORSE       0x9c
#define OP_HORSED         0x9d
#define OP_REST           0x9e
#define OP_OWNS           0x9f

#define OP_RND            0xa0
#define OP_IF             0xa1
#define OP_ENDIF          0xa2
#define OP_ELSE           0xa3
#define OP_SET            0xa4
#define OP_CLR            0xa5
#define OP_LET            0xa6
#define OP_END_OF_FACTOR  0xa7
#define OP_LET_VALUE      0xa8
#define OP_TST            0xab

#define OP_GOTO           0xb0
#define OP_CALL           0xb1
#define OP_VARINT         0xb2
#define OP_VARSTR         0xb3
/*read 16 bit value from array?*/
#define OP__B4            0xb4
#define OP_PRINTSTR       0xb5
#define OP_LEAVE          0xb6
#define OP_STRSEARCH      0xb7
#define OP_ENDSEARCH      0xb8
#define OP_GIVEOBJ        0xb9
#define OP_TAKEOBJ        0xba
#define OP_TEST_OBJ       0xbb
/*push constant 100?*/
#define OP__BC            0xbc
/*push constant 100?*/
#define OP__BD            0xbd
#define OP_SHOW_INVENTORY 0xbe
#define OP_SHOW_CONVERSE  0xbf
#define OP_SELECT_OBJECT  0xc0
#define OP_OWNER          0xc1
#define OP_OBJTYPE        0xc2
#define OP_VALSEARCH      0xc3
#define OP_ADDKARMA       0xc4
#define OP_SUBKARMA       0xc5
#define OP_ISINPARTY      0xc6
#define OP_WHOSGOT        0xc7
#define OP_MOVEOBJ        0xc8
#define OP_TRANSFEROBJ    0xc9
#define OP_JOIN           0xca
#define OP_WAIT           0xcb
#define OP_LEAVEPARTY     0xcc
#define OP_SETMODE        0xcd

#define OP_DELAY          0xd0

#define OP_ADDRESS        0xd2
#define OP_BYTE           0xd3
#define OP_WORD           0xd4
/*a parameter to OP_PRINTSTR; followed by VarStr*/
#define OP__D5            0xd5
#define OP_RESURRECT      0xd6
#define OP_ISONSCREEN     0xd7
/*$Y := ...?*/
#define OP__D8            0xd8
#define OP_HEAL           0xd9
#define OP_WOUNDED        0xda
#define OP_CURE           0xdb
#define OP_POISONNED      0xdc
/*get party member npc num?*/
#define OP__DD            0xdd

/*$Y := ...?"You"*/
#define OP__DF            0xdf
#define OP_ADDEXP         0xe0
#define OP_ADDLVL         0xe1
#define OP_ADDSTR         0xe2
#define OP_ADDINT         0xe3
#define OP_ADDDEX         0xe4

#define OP_NPC            0xeb

#define OP_ENDRES         0xee
#define OP_KEY            0xef
/*OP__F0: start of input commands?*/
#define OP__F0            0xf0
#define OP_DESC           0xf1
#define OP_MAIN           0xf2
#define OP_RES            0xf6
#define OP_ASKTOP         0xf7
#define OP_GET            0xf8
#define OP_GETSTR         0xf9
#define OP_GETCHR         0xfa
#define OP_GETINT         0xfb
#define OP_GETDIGIT       0xfc
#define OP__FF            0xff


/*====---- ----====*/
unsigned char __0988 = 0;/*in this module?*/
unsigned char __0989 = 0;/*in this module?*/

/*098A*/static unsigned char mustLeave = 0;

/*098B*/unsigned char IsInConversation = 0;
/*====---- ----====*/

/*C_1703_0006*/static str_i_compare(unsigned char *bp0a, unsigned char *bp08, int bp06) {
	unsigned char *di;
	int si;
	unsigned char bp_64[100];

	strcpy(bp_64, bp08);
	for(di = strtok(bp_64, /*098C*/" "); di; di = strtok(0, /*098C*/" ")) {
		for(si = 0; si < bp06; si ++) {
			if(tolower(bp0a[si]) != tolower(di[si]) && bp0a[si] != '?')
				break;
		}
		if(si == bp06)
			return 1;
	}
	return 0;
}

/*C_1703_008A*/static mk_npcnum(int bp06) {
	if(bp06 == OP_NPC)
		return D_E796[0];
	return bp06;
}

/*get NPC name?*/
static char *C_1703_00A0(int objNum) {
	int i;

	if(LoadConversation(objNum, ScratchBuf)) {
		for(i = 0; i < 50; i ++)
			D_E764[i] = ((unsigned char far *)ScratchBuf + i)[2];
	} else {
		D_E764[0] = 0;
	}
	i = 0;
	while(!(D_E764[i] & 0x80) && D_E764[i++]);
	if(i == 0)
		CON_printf(/*098E*/"\nUndefined.\n");
	else if(objNum == 0x6e)/*patch for "Dr_ Cat"*/
		D_E764[2] = '.';
	D_E764[i] = 0;

	return D_E764;
}

char *C_1703_0116(int objNum) {
	int i;

	if(IsPlrControl(objNum)) {
		for(i = 0; i < 8; i ++)
			if(Party[i] == objNum)
				break;
		return Names[i];
	}
	C_1703_00A0(objNum);

	return D_E764;
}

/*is object onscreen?*/
C_1703_0153(int objNum) {
	int x, y, tile;

	if(IN_VEHICLE && IsPlrControl(objNum))
		return 1;
	if(
		GetX(objNum) > MapX - 5 && GetX(objNum) < MapX + 5 &&
		GetY(objNum) > MapY - 5 && GetY(objNum) < MapY + 5
	) {
		x = GetX(objNum) - (MapX - 5);
		y = GetY(objNum) - (MapY - 5);
		if(Obj_11x11[y][x] || Tile_11x11[y][x] >= TIL_200)
			return 1;
		tile = Tile_11x11[y][x];
		return (tile != TIL_0FF && tile != TIL_1BC);
	}
	return 0;
}

/*set #N -- number of nearby party members*/
static C_1703_028B() {
	int si, num;

	num = 0;
	for(si = 0; si < PartySize; si ++) {
		if(C_1703_0153(Party[si]))
			num ++;
	}
	VarInt['N' - 0x37] = num - 1;
}

/*C_1703_02B6*/JoinParty(int objNum) {
	unsigned char *si;
	int di;

	if(IN_VEHICLE)
		return 1;
	if(PartySize == 8)
		return 2;
	PartySize ++;
	Party[PartySize - 1] = objNum;
	si = C_1703_00A0(objNum);
	di = strlen(si);
	if(di >= 14)
		strncpy(Names[PartySize - 1], si, 13);
	else
		strcpy(Names[PartySize - 1], si);
	SetPlrControl(objNum);
	if(GetAlignment(objNum) == NEUTRAL)
		SetAlignment(objNum, GOOD);
	NPCMode[objNum] = AI_FOLLOW;
	NPCComMode[objNum] = AI_FRONT;
	if(GetType(objNum) == OBJ_188)
		SetType(objNum, OBJ_182);
	return 0;
}

/*C_1703_0398*/static LeaveParty(int objNum) {
	int di;
	int bp_04, bp_02;

	if(IN_VEHICLE)
		return 1;
	for(bp_04 = 0; bp_04 < PartySize; bp_04 ++)
		if(Party[bp_04] == objNum)
			break;
	if(PartySize == bp_04)
		return 2;
	for(di = bp_04; di < PartySize; di ++) {
		Party[di] = Party[di + 1];
		strcpy(Names[di], Names[di + 1]);
	}
	PartySize --;
	ClrPlrControl(objNum);
	NPCMode[objNum] = AI_WANDER;
	if(GetAlignment(objNum) == GOOD)
		SetAlignment(objNum, NEUTRAL);
	for(bp_02 = FindInv(objNum); bp_02 >= 0; bp_02 = FindInv(objNum)) {
		MoveObj(bp_02, GetX(objNum), GetY(objNum), MapZ);
		SetOkToGet(bp_02);
	}
	MoveObj(objNum, GetX(objNum), GetY(objNum), MapZ);
	return 0;
}

/*is object owned by party onscreen?*/
C_1703_04C6(int bp08, int bp06) {
	int si;

	for(si = 0; si < PartySize; si ++) {
		if(C_1703_0153(Party[si]) && C_1944_0A43(Party[si], bp08, bp06) != -1)
			return Party[si];
	}
	return -1;
}

/*this macro creates an "offset" into the current
conversation file. though, D_E7A1 is always 0
(initialized during the startup)*/
#define MK_0000(i) (int)(i) - D_E7A1

#define PARSE_U8 (TalkBuf[Talk_PC ++])
#define READ_U16 (*(unsigned far *)(TalkBuf + Talk_PC))
#define READ_S16 (*(int far *)(TalkBuf + Talk_PC))
#define READ_U32 (*(unsigned long far *)(TalkBuf + Talk_PC))

/*C_1703_050E*/static parse_factor() {
	int sidx;/*stack index*/
	long lstack[10];/*local "stack"?*/
	long bp_6a;
	int i, j, opcode, bp_60;
	unsigned char cparam0, cparam1, bp_5b;
	int bp_5a, bp_58;
	int iparam0, iparam1, iparam2;
	unsigned char scratch[80];
	int bp_02;

	sidx = 0;
	i = 0;
	j = 0;
	bp_58 = 0;
	/*reset local stack*/
	for(; sidx < 10; sidx ++)
		lstack[sidx] = 0L;
	sidx = 0;
	opcode = PARSE_U8;
	while(opcode != OP_END_OF_FACTOR && opcode != OP_LET_VALUE) {
		bp_5a = 0;
		switch(opcode) {
			case OP_ADDRESS: lstack[sidx] = READ_U32; Talk_PC += 4; break;
			case OP_BYTE: lstack[sidx] = PARSE_U8; break;
			case OP_WORD: lstack[sidx] = READ_U16; Talk_PC += 2; break;
			default:
				bp_5a = 1;
				lstack[sidx] = opcode;
		}
		if(bp_5a) {
			switch(lstack[sidx]) {
				case OP_RND:
					cparam0 = lstack[sidx - 2];
					cparam1 = lstack[sidx - 1];
					lstack[sidx - 2] = OSI_rand(cparam0, cparam1);
					sidx -= 2;
				break;
				case OP_TEST_OBJ:
					iparam0 = mk_npcnum(lstack[sidx - 2]);
					iparam1 = lstack[sidx - 1];
					lstack[sidx - 2] = TestObj(iparam0, iparam1);
					sidx -= 2;
				break;
				case OP_OWNS:
					iparam0 = mk_npcnum(lstack[sidx - 3]);
					iparam1 = lstack[sidx - 2];
					iparam2 = lstack[sidx - 1];
					if(C_1944_0A43(iparam0, iparam1, iparam2) != -1)
						lstack[sidx - 3] = 1;
					else
						lstack[sidx - 3] = 0;
					sidx -= 3;
				break;
				case OP_WEIGHT:
					iparam0 = lstack[sidx - 1];/*quantity*/
					iparam1 = lstack[sidx - 2];/*obj num*/
					iparam2 = TypeWeight[iparam1];
					if(C_155D_063A(iparam1))
						lstack[sidx - 2] = iparam0 * iparam2;
					else
						lstack[sidx - 2] = iparam0 * iparam2 * 10;
					sidx -= 2;
				break;
				case OP_JOIN:
					iparam0 = mk_npcnum(lstack[sidx - 1]);
					lstack[sidx - 1] = JoinParty(iparam0);
					sidx --;
				break;
				case OP_CANCARRY:
					iparam0 = mk_npcnum(lstack[sidx - 1]);
					C_155D_07E0(iparam0);
					iparam1 = STREN[iparam0] * 200;
					iparam2 = (WeightEquip + WeightInven) * 10;
					if(iparam1 > iparam2)
						lstack[sidx - 1] = iparam1 - iparam2;
					else
						lstack[sidx - 1] = 0;
					sidx --;
				break;
				case OP_LEAVEPARTY:
					iparam0 = mk_npcnum(lstack[sidx - 1]);
					lstack[sidx - 1] = LeaveParty(iparam0);
					sidx --;
				break;
				case OP_TST:
					iparam0 = mk_npcnum(lstack[sidx - 2]);
					iparam1 = lstack[sidx - 1];
					lstack[sidx - 2] = (TalkFlags[iparam0] >> iparam1) & 1;
					sidx -= 2;
				break;
				case OP_SELECT_OBJECT:
					CON_printf(/*099B*/"\n\nSelect item: ");
					MouseMode = 1;
					StatusDisplay = CMD_92;
					CON_getch();
					if(Selection.obj != -1 && Selection.x != -1)
						lstack[sidx] = Selection.obj;
					else
						lstack[sidx] = 0x8001;
				break;
				case OP_OWNER:
					lstack[sidx - 1] = (int)GetAssoc(lstack[sidx - 1]);
					TalkBuf[Talk_PC + 1] = mk_npcnum(TalkBuf[Talk_PC + 1]);
					sidx --;
				break;
				case OP_WOUNDED:
					iparam0 = mk_npcnum(lstack[sidx - 1]);
					if(MaxHP(iparam0) > HitPoints[iparam0])
						lstack[sidx - 1] = 1;
					else
						lstack[sidx - 1] = 0;
					sidx --;
				break;
				case OP_POISONNED:
					iparam0 = mk_npcnum(lstack[sidx - 1]);
					lstack[sidx - 1] = IsPoisoned(iparam0);
					sidx --;
				break;
				case OP_ADDEXP:
					iparam0 = mk_npcnum(lstack[sidx - 2]);
					iparam1 = lstack[sidx - 1];
					ExpPoints[iparam0] += iparam1;
					if(ExpPoints[iparam0] > 9999)
						ExpPoints[iparam0] = 9999;
					lstack[sidx - 2] = ExpPoints[iparam0];
					sidx -= 2;
				break;
				case OP_ADDLVL:
					iparam0 = mk_npcnum(lstack[sidx - 2]);
					iparam1 = lstack[sidx - 1];
					Level[iparam0] += iparam1;
					lstack[sidx - 2] = GetLevel(iparam0);
					sidx -= 2;
				break;
				case OP_ADDSTR:
					iparam0 = mk_npcnum(lstack[sidx - 2]);
					iparam1 = lstack[sidx - 1];
					STREN[iparam0] += iparam1;
					if(STREN[iparam0] > 30)
						STREN[iparam0] = 30;
					lstack[sidx - 2] = STREN[iparam0];
					sidx -= 2;
				break;
				case OP_ADDINT:
					iparam0 = mk_npcnum(lstack[sidx - 2]);
					iparam1 = lstack[sidx - 1];
					INTEL[iparam0] += iparam1;
					if(INTEL[iparam0] > 30)
						INTEL[iparam0] = 30;
					lstack[sidx - 2] = INTEL[iparam0];
					sidx -= 2;
				break;
				case OP_ADDDEX:
					iparam0 = mk_npcnum(lstack[sidx - 2]);
					iparam1 = lstack[sidx - 1];
					DEXTE[iparam0] += iparam1;
					if(DEXTE[iparam0] > 30)
						DEXTE[iparam0] = 30;
					lstack[sidx - 2] = DEXTE[iparam0];
					sidx -= 2;
				break;
				case OP__DD:/*get party member npc num?*/
					iparam0 = lstack[sidx - 2];/*party member index*/
					iparam1 = lstack[sidx - 1];/*if 0, membert must be onScreen*/
					if(iparam0 < PartySize) {
						if(iparam1) {
							lstack[sidx - 2] = Party[iparam0];
							sidx -= 2;
							break;
						}
						/*party's onscreen members list*/
						j = 0;
						for(i = 0; i < PartySize; i ++) {
							if(C_1703_0153(Party[i]))
								scratch[j++] = i;
						}
						for(cparam0 = j; cparam0 < PartySize; cparam0 ++)
							scratch[cparam0] = 0xff;

						if(scratch[iparam0] != 0xff) {
							lstack[sidx - 2] = Party[scratch[iparam0]];
							sidx -= 2;
							break;
						}
					}
					lstack[sidx - 2] = 0;
					sidx -= 2;
				break;
				case OP_ISONSCREEN:
					iparam0 = mk_npcnum(lstack[sidx - 1]);
					lstack[sidx - 1] = C_1703_0153(iparam0);
					sidx --;
				break;
				case OP_WHOSGOT:
					iparam0 = lstack[sidx - 2];
					iparam1 = lstack[sidx - 1];
					i = C_1703_04C6(iparam0, iparam1);
					if(i == -1)
						lstack[sidx - 2] = 0x8001;
					else
						lstack[sidx - 2] = i;
					sidx -= 2;
				break;
				case OP_ISINPARTY:
					iparam0 = mk_npcnum(lstack[sidx - 1]);
					if(
						IsPlrControl(iparam0) &&
						C_1703_0153(iparam0) &&
						!IsDead(iparam0) &&
						!IsAsleep(iparam0) &&
						!IsParalyzed(iparam0)
					)
						lstack[sidx - 1] = 1;
					else
						lstack[sidx - 1] = 0;
					sidx --;
				break;
				case OP_HORSED:
					iparam0 = mk_npcnum(lstack[sidx - 1]);
					if(GetType(iparam0) == OBJ_1AF)
						lstack[sidx - 1] = 1;
					else
						lstack[sidx - 1] = 0;
					sidx --;
				break;
				case OP_OBJTYPE:
					lstack[sidx - 1] = GetType(lstack[sidx - 1]);
					sidx --;
				break;
				case OP__BC:/*constant 100?*/
				case OP__BD:/*constant 100?*/
					lstack[sidx - 2] = 100;
					sidx -= 2;
				break;
				case OP__B4:/*read a 16bit value from array?*/
					bp_6a = lstack[sidx - 2];/*integer variable*/
					bp_5b = lstack[sidx - 1];/*array index*/
					D_E7A7 = Talk_PC;
					Talk_PC = (bp_5b << 1) + MK_0000(bp_6a);
					lstack[sidx - 2] = READ_S16;
					Talk_PC = D_E7A7;
					sidx -= 2;
				break;
				case OP_VARINT:/*# var*/
					bp_5b = lstack[sidx - 1];
					lstack[sidx - 1] = (unsigned)VarInt[bp_5b];
					sidx --;
				break;
				case OP_VARSTR:/*$ string*/
					bp_5b = lstack[sidx - 1];
					lstack[sidx - 1] = (int)VarStr[bp_5b];
					bp_58 = 1;
					sidx --;
				break;
				case OP_ADD:
					lstack[sidx - 2] = lstack[sidx - 1] + lstack[sidx - 2];
					sidx -= 2;
				break;
				case OP_SUB:
					lstack[sidx - 2] = lstack[sidx - 2] - lstack[sidx - 1];
					sidx -= 2;
				break;
				case OP_MUL:
					lstack[sidx - 2] = lstack[sidx - 1] * lstack[sidx - 2];
					sidx -= 2;
				break;
				case OP_DIV:
					if(lstack[sidx - 1])
						lstack[sidx - 2] = (unsigned long)lstack[sidx - 2] / (unsigned long)lstack[sidx - 1];
					else
						CON_printf(/*09AB*/"\nDivide error from talkdr.\n");
					sidx -= 2;
				break;
				case OP_AND:
					if(lstack[sidx - 1] && lstack[sidx - 2])
						lstack[sidx - 2] = 1;
					else
						lstack[sidx - 2] = 0;
					sidx -= 2;
				break;
				case OP_OR:
					if(lstack[sidx - 1] || lstack[sidx - 2])
						lstack[sidx - 2] = 1;
					else
						lstack[sidx - 2] = 0;
					sidx -= 2;
				break;
				case OP_EQU:
					if(bp_58) {
						if(stricmp((char *)lstack[sidx - 1], (char *)lstack[sidx - 2]) == 0)
							lstack[sidx - 2] = 1;
						else
							lstack[sidx - 2] = 0;
					} else {
						if(lstack[sidx - 1] == lstack[sidx - 2])
							lstack[sidx - 2] = 1;
						else
							lstack[sidx - 2] = 0;
					}
					sidx -= 2;
				break;
				case OP_SUP:
					if((unsigned long)lstack[sidx - 2] > (unsigned long)lstack[sidx - 1])
						lstack[sidx - 2] = 1;
					else
						lstack[sidx - 2] = 0;
					sidx -= 2;
				break;
				case OP_SUPE:
					if((unsigned long)lstack[sidx - 2] >= (unsigned long)lstack[sidx - 1])
						lstack[sidx - 2] = 1;
					else
						lstack[sidx - 2] = 0;
					sidx -= 2;
				break;
				case OP_INF:
					if((unsigned long)lstack[sidx - 2] < (unsigned long)lstack[sidx - 1])
						lstack[sidx - 2] = 1;
					else
						lstack[sidx - 2] = 0;
					sidx -= 2;
				break;
				case OP_INFE:
					if((unsigned long)lstack[sidx - 2] <= (unsigned long)lstack[sidx - 1])
						lstack[sidx - 2] = 1;
					else
						lstack[sidx - 2] = 0;
					sidx -= 2;
				break;
				case OP_DIF:
					if(lstack[sidx - 1] != lstack[sidx - 2])
						lstack[sidx - 2] = 1;
					else
						lstack[sidx - 2] = 0;
					sidx -= 2;
				break;
				case OP_STRSEARCH:
					bp_5b = lstack[sidx - 1];/*string variable*/
					bp_6a = lstack[sidx - 2];/*array variable*/
					bp_02 = VarStr[bp_5b];
					D_E7A7 = Talk_PC;
					Talk_PC = MK_0000(bp_6a);
					for(i = 0; (opcode = PARSE_U8) != OP_ENDSEARCH; ) {
						Talk_PC --;
						i ++;
						for(j = 0; opcode = PARSE_U8; j ++)
							scratch[j] = opcode;
						scratch[j] = 0;
						if(strlen(scratch) >= 4)
							bp_60 = 4;
						else
							bp_60 = strlen(scratch);
						if(str_i_compare(scratch, bp_02, bp_60))
							break;
					}
					if(opcode == OP_ENDSEARCH)
						lstack[sidx - 2] = i;
					else
						lstack[sidx - 2] = i - 1;
					Talk_PC = D_E7A7;
					sidx -= 2;
				break;
				case OP_VALSEARCH:
					bp_5b = lstack[sidx - 1];/*integer variable*/
					bp_6a = lstack[sidx - 2];/*array variable*/
					iparam1 = VarInt[bp_5b];
					D_E7A7 = Talk_PC;
					Talk_PC = MK_0000(bp_6a);
					for(i = 0; (opcode = TalkBuf[Talk_PC]) != OP_ENDSEARCH; Talk_PC += 2) {
						i ++;
						iparam0 = READ_S16;
						if(iparam0 == iparam1)
							break;
					}
					if(opcode == OP_ENDSEARCH)
						lstack[sidx - 2] = 0x8001;
					else
						lstack[sidx - 2] = i - 1;
					Talk_PC = D_E7A7;
					sidx -= 2;
				break;
			}
		}
		sidx ++;
		opcode = PARSE_U8;
	}
	if(sidx > 1)
		CON_printf(/*09C7*/"Error occured.\n");
	return lstack[0];
}

/*read U32 address?long goto?*/
static unsigned long C_1703_1494(int bp06) {
	int si;
	unsigned long bp_08, bp_04;

	bp_04 = READ_U32;
	Talk_PC += 4;
	if(PARSE_U8 != OP_CALL) {
		Talk_PC --;
		si = parse_factor();
		bp_08 = Talk_PC;
		Talk_PC = MK_0000(bp_04);
		if(bp06) {
			while(si > 0)
				if(PARSE_U8 == 0)
					si --;
		} else {
			Talk_PC += si << 1;
			bp_08 -= 1;
		}
	} else {
		bp_08 = Talk_PC;
		Talk_PC = MK_0000(bp_04);
	}
	return bp_08;
}

/*C_1703_153B*/static execute_op(unsigned char opcode, int objNum) {
	int si, di;
	unsigned long bp_1a;
	int bp_16, bp_14;
	int *bp_12;
	int bp_10, bp_0e, bp_0c, bp_0a, bp_08;
	int far *bp_06;
	char *bp_02;

	switch(opcode) {
		case OP_ENDIF:
		case OP_END_OF_FACTOR: break;
		case OP_LEAVE:
			mustLeave = 1;
		break;
		case OP_PRINTSTR:
			bp_16 = PARSE_U8;
			if(bp_16 == OP_ADDRESS) {/*constant string[address]*/
				D_E7A7 = C_1703_1494(1);
				while(bp_16 = PARSE_U8)
					CON_putch(bp_16);
				Talk_PC = D_E7A7;
			} else if(bp_16 != OP__D5) {/*VarStr*/
				di = PARSE_U8;
				bp_14 = PARSE_U8;
				if(bp_14 != OP_VARSTR)
					return 1;
				CON_printf(VarStr[di]);
				Talk_PC ++;
			}
		break;
		case OP_LET:
			di = PARSE_U8;
			if(di == OP_ADDRESS) {
				D_E7A7 = C_1703_1494(0);
				bp_06 = (int far *)(TalkBuf + Talk_PC);
				Talk_PC = D_E7A7;
				bp_14 = 0;
			} else {
				bp_14 = PARSE_U8;
			}
			if(PARSE_U8 != OP_LET_VALUE)
				return 1;
			if(bp_14 == 0) {
				*bp_06 = parse_factor();
			} else if(bp_14 == OP_VARINT) {
				bp_12 = VarInt + di;
				*bp_12 = parse_factor();
			} else {/*String affectation*/
				bp_02 = VarStr[di];
				if((bp_16 = PARSE_U8) == OP_ADDRESS) {
					D_E7A7 = C_1703_1494(1);
					for(si = 0; bp_16 = PARSE_U8; si ++)
						bp_02[si] = bp_16;
					bp_02[si] = 0;
					Talk_PC = D_E7A7;
				} else {
					di = bp_16;
					bp_14 = PARSE_U8;
					if(bp_14 != OP_VARSTR)
						return 1;
					strcpy(bp_02, VarStr[di]);
				}
			}
		break;
		case OP_IF:
			si = parse_factor();
			if(si == -1)
				return 1;
			if(si == 0) {
				for(bp_16 = PARSE_U8; bp_16 != OP_ELSE && bp_16 != OP_ENDIF; bp_16 = PARSE_U8) {
					if(bp_16 == OP_GOTO)
						Talk_PC +=4;
					else if(bp_16 == OP_BYTE)
						Talk_PC ++;
					else if(bp_16 == OP_WORD)
						Talk_PC += 2;
				}
			}
		break;
		case OP_ELSE:
				for(bp_16 = PARSE_U8; bp_16 != OP_ENDIF; bp_16 = PARSE_U8) {
					if(bp_16 == OP_GOTO)
						Talk_PC +=4;
					else if(bp_16 == OP_BYTE)
						Talk_PC ++;
					else if(bp_16 == OP_WORD)
						Talk_PC += 2;
				}
		break;
		case OP_GOTO:
			bp_1a = READ_U32;
			Talk_PC = MK_0000(bp_1a);
		break;
		case OP_GIVEOBJ: case OP_TAKEOBJ:
			bp_10 = mk_npcnum(parse_factor());
			bp_0e = parse_factor();
			bp_0c = parse_factor();
			bp_0a = parse_factor();
			if(opcode == OP_TAKEOBJ)
				TakeObj(bp_10, bp_0e, QualQuan(bp_0c, bp_0a));
			else
				GiveObj(bp_10, bp_0e, QualQuan(bp_0c, bp_0a));
		break;
		case OP_WAIT:
			PromptCh = 1;
			CON_getch();
			PromptCh = 5;
		break;
		case OP_SETMODE:
			bp_10 = mk_npcnum(parse_factor());
			bp_0e = parse_factor();
			NPCMode[bp_10] = bp_0e;
			if(bp_0e == AI_SLEEP) {
				SetAsleep(bp_10);
				SetTypeUnconscious(bp_10);
			}
			if(bp_0e != AI_PLAY && GetType(bp_10) == OBJ_188)
				SetType(bp_10, OBJ_182);
		break;
		case OP_MOVEOBJ:
			bp_10 = mk_npcnum(parse_factor());
			bp_0e = parse_factor();
			InsertObj(bp_10, bp_0e, INVEN);
		break;
		case OP_TRANSFEROBJ:
			bp_10 = parse_factor();
			bp_0e = parse_factor();
			bp_0c = mk_npcnum(parse_factor());
			bp_0a = mk_npcnum(parse_factor());
			InsertObj(FindInvType(bp_0c, bp_10, bp_0e), bp_0a, INVEN);
		break;
		case OP_ADDKARMA:
			bp_10 = parse_factor();
			KARMA += bp_10;
			if(KARMA > 99)
				KARMA = 99;
		break;
		case OP_SUBKARMA:
			bp_10 = parse_factor();
			if(KARMA > bp_10)
				KARMA -= bp_10;
			else
				KARMA = 0;
		break;
		case OP_DELAY:
			bp_10 = parse_factor();
			for(bp_08 = 0; bp_08 < bp_10 * 20; bp_08 ++)
				OtherAnimations();
		break;
		case OP_SET: case OP_CLR:
			bp_10 = mk_npcnum(parse_factor());
			bp_0e = parse_factor();
			if(opcode == OP_SET)
				TalkFlags[bp_10] |= 1 << bp_0e;
			else
				TalkFlags[bp_10] &= ~(1 << bp_0e);
		break;
		case OP_RESURRECT:
			bp_10 = mk_npcnum(parse_factor());
			for(bp_0e = Link[bp_10]; bp_0e >= 0; bp_0e = Link[bp_0e])
				if(GetType(bp_0e) == OBJ_153)
					break;
			C_1944_1A42(bp_0e);
		break;
		case OP_HEAL:
			bp_10 = mk_npcnum(parse_factor());
			HitPoints[bp_10] = MaxHP(bp_10);
		break;
		case OP_CURE:
			bp_10 = mk_npcnum(parse_factor());
			ClrPoisoned(bp_10);
		break;
		case OP__D8:/*set var $Y to ...?*/
			bp_10 = mk_npcnum(parse_factor());
			VarStr['Y' - 0x37] = C_1703_0116(bp_10);
		break;
		case OP__DF:/*set var $Y to ...?*/
			bp_10 = mk_npcnum(parse_factor());
			if(bp_10 == 1)
				VarStr['Y' - 0x37] = /*09D7*/"you";
			else
				VarStr['Y' - 0x37] = C_1703_0116(bp_10);
		break;
		case OP_REST:
			for(bp_08 = 0; bp_08 < PartySize; bp_08 ++)
				if(C_1703_0153(Party[bp_08]))
					C_3200_0467(bp_08, 0);
			C_2FC1_19A1();
			AllowNPCTeleport = 1;
			if(Time_H == 5)
				C_0A33_1355(60);
			while(Time_H != 5)
				C_0A33_1355(60);
			AllowNPCTeleport = 0;
			ScreenFade = 1;
			C_1100_0306();
			OtherAnimations();
		break;
		case OP_SHOW_INVENTORY:
			bp_10 = parse_factor();
			ShowNPCInven = 0;
			NPC_1Hand = 0;
			if(bp_10 == 1) {
				D_04B3 = Active;
			} else if(IsPlrControl(objNum)) {
				for(si = 0; si < 0x10; si ++)
					if(Party[si] == objNum) {
						D_04B3 = si;
						break;
					}
			} else {
				ShowNPCInven = objNum;
			}
			C_155D_1065(objNum);
		break;
		case OP_SHOW_CONVERSE:
			bp_10 = mk_npcnum(parse_factor());
			C_27A1_02D9(bp_10);/*display character's portrait*/
		break;
		case OP_GETHORSE:
			bp_10 = mk_npcnum(parse_factor());
			C_27A1_5431(bp_10, MapX, MapY, OBJ_MakeDirFrame(OBJ_1AF, GetDirection(bp_10)), 0);
		break;
		default:
			CON_printf(/*09DB*/"Unknown command.\n");
			return -1;
	}
	return 0;
}

static /*C_1703_1C45*/parse_statement(int objNum) {
	unsigned char opcode;

	opcode = PARSE_U8;
	while(
		opcode != OP_ASKTOP &&
		opcode < OP__F0 && opcode != 0 &&
		opcode != OP_GETSTR && opcode != OP_GET &&
		opcode != OP_ENDRES
	) {
		if(opcode < 0x80) {
			if(opcode >= ' ' || opcode == '\n') {
				CON_putch(opcode);
			} else {
				CON_printf(/*09ED*/"\nTry to print out a control char\n");
				break;
			}
		} else if(opcode == OP_KEY) {
			if(D_E79C == 0) {
				C_1703_1D01(objNum);
			} else {
				while(opcode != OP_ENDRES)
					opcode = PARSE_U8;
			}
		} else {
			execute_op(opcode, objNum);
		}
		if(mustLeave)
			break;
		opcode = PARSE_U8;
	}
	Talk_PC --;
}

/*KEY/RES*/
static C_1703_1D01(int objNum) {
	int si, di;
	int bp_02;

	bp_02 = 1;
	if(D_E732[0] == 0)
		strcpy(D_E732, /*0A0F*/"bye");
	while(bp_02) {
		bp_02 = 0;
		for(di = 0; (D_E728[di] = PARSE_U8) != ',' && D_E728[di] != OP_RES; di ++);
		if(D_E728[di] == ',')
			bp_02 = 1;
		else
			Talk_PC --;
		D_E728[di] = 0;
		if(str_i_compare(D_E728, D_E732, strlen(D_E728)) || D_E728[0] == '*') {
			while(PARSE_U8 != OP_RES);
			CON_printf(/*098E + 0Bh*/"\n");
			D_E79C = 1;
			parse_statement(objNum);
			return;
		}
	}
	do {
		si = PARSE_U8;
		if(si == OP_GOTO)
			Talk_PC += 4;
		else if(si == OP_BYTE)
			Talk_PC += 1;
		else if(si == OP_WORD)
			Talk_PC += 2;
	} while(si != OP_KEY && si != OP_ENDRES && si != OP__FF);
	if(si == OP_KEY || si == OP__FF)
		Talk_PC --;
}

/*C_1703_1E23*/TalkDriver(int objNum_0, int objNum_1) {
	int si, di;
	int opcode, dir, bp_06, bp_04, objTyp;

	D_E796[1] = objNum_0;
	D_E796[0] = objNum_1;
	if(IsPlrControl(D_E796[0]) && !C_1703_0153(D_E796[0])) {
		CON_printf(/*0A13*/"\nNot on screen.\n");
		return;
	}
	if(D_2CC3 != -1 && D_2CC3 != 0) {
		CON_printf(/*0A24*/"\nNot in solo mode.\n");
		return;
	}
	objTyp = GetType(objNum_1);
	/*status or shrine*/
	if(objTyp == OBJ_18D || objTyp == OBJ_18E || objTyp == OBJ_18F || objTyp == OBJ_189)
		D_E796[0] = GetQual(objNum_1);

	if(D_E796[0] == 0 || D_E796[0] >= 0xe0) {
		if(IsDead(D_E796[0]) && SeanceFlag) {
			CON_printf(/*0A38*/"\nYou hear a deep moan.\n");
			return;
		}
	}

	if(D_E796[0] == objNum_1) {
		if(
			(IsDead(D_E796[0]) && !SeanceFlag) ||
			IsAsleep(D_E796[0]) || IsParalyzed(D_E796[0]) ||
			NPCMode[D_E796[0]] == AI_VIGILANTE ||
			NPCMode[D_E796[0]] == AI_FEAR ||
			NPCMode[D_E796[0]] == AI_RETREAT ||
			NPCMode[D_E796[0]] == AI_ARREST ||
			GetAlignment(D_E796[0]) == EVIL ||
			GetAlignment(D_E796[0]) == CHAOTIC
		) {
			CON_printf(/*0A50*/"\n\nNo response\n");
			return;
		}
	}
	if(IsArmageddon) {
		CON_printf(/*0A50+1*/"\nNo response\n");
		return;
	}
	if(D_E796[1] == D_E796[0]) {
		CON_printf(/*0A5F*/"\nTalking to yourself?\n");
		return;
	}

	dir = MkDirection(PointerX, PointerY);
	if(dir != -1) {
		C_1E0F_0664(Party[Active], dir);
		C_1100_0306();
	}

	if(
		D_E796[0] == 0 ||
		(D_E796[0] >= 0xe0 && objTyp != OBJ_17E && objTyp != OBJ_175 && objTyp != OBJ_16B && objTyp != OBJ_16A) ||
		!LoadConversation(D_E796[0], TalkBuf)
	) {
		CON_printf(/*0A76*/"\nFunny, no response.\n");
		return;
	}
	C_1703_028B();/*compute #N*/
	Talk_PC = 0;
	/*skip OP__FF ncpnum*/
	si = 2;
	/*get the npc name: $N*/
	while((opcode = TalkBuf[si++]) != OP_DESC)
		VarStr['N' - 0x37][si - 3] = opcode;
	VarStr['N' - 0x37][si - 3] = 0;
	/* */
	IsInConversation = 1;
	bp_06 = StatusDisplay;
	bp_04 = D_04B3;
	if(!SeanceFlag) {
		if(GetType(objNum_1) != OBJ_189)
			C_27A1_02D9(objNum_1);/*display character's portrait*/
		CON_printf(/*0A8C*/"\n\nYou see ");
		while(PARSE_U8 != OP_DESC);
		D_E79D = Talk_PC;
		parse_statement(D_E796[0]);
	}
	CON_printf(/*098E + 0Bh*/"\n");
	while(PARSE_U8 != OP_MAIN);

	opcode = PARSE_U8;
	if(opcode != OP_ASKTOP) {
		Talk_PC --;
		parse_statement(D_E796[0]);
	} else {
		Talk_PC --;
	}

	while(!mustLeave) {
		D_E7A7 = Talk_PC;
		opcode = PARSE_U8;
		if(opcode == OP_ASKTOP) {
			CON_printf(/*0A97*/"\nyou say:");
			CON_gets(D_E732, 50);
			CON_printf(/*098E + 0Bh*/"\n");
			D_E79C = 0;
		} else if(opcode == OP_GETSTR) {
			CON_gets(D_E732, 50);
			si = PARSE_U8;
			if(PARSE_U8 != OP_VARSTR)
				break;
			strcpy(VarStr[si], D_E732);
			CON_printf(/*0AA1*/"\n\n");
			D_E79C = 0;
		} else if(opcode == OP_GETINT) {
			CON_gets(D_E732, 50);
			si = PARSE_U8;
			if(PARSE_U8 != OP_VARINT)
				break;
			di = C_27A1_0205(D_E732, 0);
			VarInt[si] = (di == -1 || di == -2)?0:di;
			CON_printf(/*0AA1*/"\n\n");
			D_E79C = 0;
		} else if(opcode == OP_GETDIGIT) {
			do {
				D_E732[0] = CON_getch();
				D_E732[1] = 0;
				di = C_27A1_0205(D_E732, 0);
				if(D_E732[0] == '\r' || D_E732[0] == 27)
					di = 0;
			} while(di == -1 || di == -2);
			si = PARSE_U8;
			if(PARSE_U8 != OP_VARINT)
				break;
			VarInt[si] = di;
			CON_printf(/*0AA4*/"%d\n\n", di);
			D_E79C = 0;
		} else if(opcode == OP_GETCHR) {
			D_E732[0] = CON_getch();
			D_E732[1] = 0;
			CON_printf(/*0AA9*/"%s\n\n", D_E732);
			D_E79C = 0;
		} else if(opcode == OP_WAIT) {
			CON_getch();
		} else if(opcode == OP_GET) {/*OP_GET c0 .. cn OP_KEY -- GET [...]*/
			for(si = 0; (opcode = PARSE_U8) != OP_KEY; si ++)
				D_E728[si] = opcode;
			Talk_PC --;
			D_E728[si] = 0;
			do {
				opcode = CON_getch();
				for(si = 0; D_E728[si] && toupper(D_E728[si]) != opcode; si++);
			} while(D_E728[si] == 0);
			if(opcode >= ' ')
				CON_printf(/*0AAE*/"%c", opcode);
			D_E732[0] = opcode;
			D_E732[1] = 0;
			CON_printf(/*098E + 0Bh*/"\n");
			D_E79C = 0;
		} else if(opcode == OP_KEY) {
			CON_printf(/*0AB1*/"\nCommand for input key expected.");
			break;
		} else if(opcode == OP__FF) {
			break;
		} else if(opcode == OP_ENDRES) {
			/**/
		} else {
			CON_printf(/*0AD2*/"\nLogical error occured.");
			break;
		}
		/*$Z is the last player's input*/
		VarStr['Z' - 0x37] = D_E732;

		if(stricmp(D_E732, /*0AEA*/"look") == 0) {
			CON_printf(/*0A8C + 2*/"You see ");
			Talk_PC = D_E79D;
			parse_statement(D_E796[0]);
			Talk_PC = D_E7A7;
		} else {
			parse_statement(D_E796[0]);
		}
	}
	mustLeave = 0;
	IsInConversation = 0;
	if(PartySize == D_04B3) {
		D_04B3 = 0;
		D_07CC = 0;
		StatusDisplay = CMD_91;
	} else {
		StatusDisplay = bp_06;
		D_04B3 = bp_04;
	}
	StatusDirty ++;
}
