/*
 * Ultima VI - The False Prophet
 * Copyright (c) 1990 Origin Systems, Inc. and Richard Gariott
 * reverse-coded by Ergonomy Joe in 2013
 */
/*
	console module?
*/
#include "u6.h"

/*0492*/unsigned char SelectMode = 0;
/*0493*/char AimX = 0;
/*0494*/char AimY = 0;
/*0495*/char SelectRange = 0;
static unsigned char D_0496 = 0;/*force some refresh*/
unsigned char D_0497 = 0;
static unsigned char D_0498 = 0;/*some flag for text control '`'*/
char D_0499 = 3;
char D_049A = 0;
static unsigned char D_049B = 0;/*string input flag*/
unsigned char D_049C = 1;
int D_049D = 1;
int D_049F = 0;
int D_04A1 = 1;/*help flag*/
int D_04A3 = 0;
/*04A5*/int TextOn = 1;
int D_04A7 = -1;
int D_04A9 = 0;
static int D_04AB = 1;/*kbd input related*/
int D_04AD = 0;
int D_04AF = 1;
int D_04B1 = 1;
int D_04B3 = 0;/*currently displayed character info #*/
unsigned char D_04B5 = 0;/*current incantation index*/
unsigned long D_04B6 = 0;/*spell mask*/
/*04BA*/int MousePress = 0;
/*04BC*/int MouseTrigger = 0;
/*04BE*/int MouseMode = 0;
/*04C0*/int StatusDisplay = CMD_91;
int D_04C2 = CMD_8F;
/*04C4*/int AllowMouseMov = 0;
/*04C6*/int MouseMapX = -1;
/*04C8*/int MouseMapY = -1;
unsigned char D_04CA = 88;
unsigned char D_04CB = 88;
void C_0C9C_0286(int ch, int x, int y);/*"put char" callback*/
void C_0C9C_02F1(int x, int y, int w, int h);/*"scroll up" callback*/
void (*D_04CC)(int, int, int) = C_0C9C_0286;/*"put char" callback pointer*/
void (*D_04D0)(int, int, int, int) = C_0C9C_02F1;/*"scroll up" callback pointer*/
/*04D4*/int PromptCh = 5;
struct tClipInfo D_04D6 = {
	0, 0, 40, 25,
	0, 0,
	0xf, 0,
	0
};
struct tClipInfo *D_04DF = &D_04D6;/*current clip*/
int D_04E1 = 1;/*original attributes*/
/*-- 3x5 font*/
unsigned D_04E3[] = {
	0xF6DE,/*'0'*/
	0x4924,/*'1'*/
	0xE7CE,/*'2'*/
	0xE59E,/*'3'*/
	0xB792,/*'4'*/
	0xF39E,/*'5'*/
	0xF3DE,/*'6'*/
	0xE4A4,/*'7'*/
	0xF7DE,/*'8'*/
	0xF79E,/*'9'*/
	0xF7DA,/*'A'*/
	0xD75C,/*'B'*/
	0xF24E,/*'C'*/
	0xD6DC,/*'D'*/
	0xF34E,/*'E'*/
	0xF348,/*'F'*/
	0xF2DE,/*'G'*/
	0xB7DA,/*'H'*/
	0xE92E,/*'I'*/
	0x24DE,/*'J'*/
	0xB75A,/*'K'*/
	0x924E,/*'L'*/
	0xBEDA,/*'M'*/
	0x03DA,/*'N'*/
	0x03DE,/*'O'*/
	0xF7C8 /*'P'*/
};
unsigned char D_0517[][4] = {
	{ 7,0,0,0},/*1 digit*/
	{ 9,4,0,0},/*2 digit*/
	{11,7,3,0},/*3 digit*/
	{13,9,5,1} /*4 digit*/
};
int D_0527 = -1;
int D_0529 = -1;
int D_052B[][4] = {
	{  8,  8,167,167},/*map view?*/
	{  8,176,167,191},/*icons?*/
	{168,  8,311,103},/*party/portrait/equip view?*/
	{  0,  0,319,199} /*all screen*/
};
/*"slots" x & y*/
int D_054B[] = {176,200,224,248,264,280,296};
unsigned char D_0559[][7] = {
	{24,16,24,32,32,32,32},
	{40,40,40,48,48,48,48},
	{56,64,56,64,64,64,64}
};
/*some icons*/
int D_056E[] = {TIL_183,TIL_180,TIL_181,TIL_184,TIL_187};
int D_0578 = 0;
unsigned char D_057A = 0;
unsigned char D_057B = 0;
unsigned char D_057C = 1;
unsigned char D_057D = 0;

#define LEFT_CLICK (D_B72B & 1)
#define RIGHT_CLICK (D_B72B & 2)

/*HideMouseInArea?*/
/*C_0C9C_0007*/CON_mouseOffAt(int x0, int y0, int x1, int y1) {
	int si;

	if(MouseOn) {
		MouseArea.left = x0;
		MouseArea.top = y0;
		MouseArea.right = x1;
		MouseArea.bottom = y1;
		si = MOUSE_IN_AREA(&MouseArea);
		if(MOUSE_IN_AREA(&MouseArea))
			MOUSE_HIDE();
	}
	return si;
}

/*mouse on if bp06*/
/*C_0C9C_0050*/CON_mouseOn(int bp06) {
	if(MouseOn && bp06)
		MOUSE_SHOW();
}

/*get mouse "direction"*/
/*C_0C9C_0069*/MkDirection(int x, int y) {
	int dir;
	int delta_x, delta_y;

	dir = -1;
	delta_x = abs(x - D_04CA);
	delta_y = abs(y - D_04CB);
	if(
		D_04CA - 24 <= x && D_04CA + 24 > x &&
		D_04CB - 24 <= y && D_04CB + 24 > y
	) {
		if(D_04CA - 8 > x) {
			if(D_04CB - 8 > y)
				dir = 7;
			else if(D_04CB + 8 > y)
				dir = 6;
			else
				dir = 5;
		} else if(D_04CA + 8 > x) {
			if(D_04CB - 8 > y)
				dir = 0;
			else if(D_04CB + 8 > y)
				dir = -1;
			else
				dir = 4;
		} else {
			if(D_04CB - 8 > y)
				dir = 1;
			else if(D_04CB + 8 > y)
				dir = 2;
			else
				dir = 3;
		}
	} else {
		if(delta_x > delta_y) {
			if(delta_y == 0 || (delta_x / delta_y) >= 2) {
				dir = (D_04CA <= x)?2:6;
			}
		} else {
			if(delta_x == 0 || (delta_y / delta_x) >= 2) {
				dir = (D_04CB <= y)?4:0;
			}
		}
		if(dir == -1) {
			if(D_04CA <= x) {
				dir = (D_04CB <= y)?3:1;
			} else {
				dir = (D_04CB <= y)?5:7;
			}
		}
	}
	return dir;
}

/*mouse callback:
update mouse pointer type according to position*/
C_0C9C_01FB() {
	int si;

	if(!MouseOn)
		return;
	if(Mouse.f_02 >= 8 && Mouse.f_02 < 168 && Mouse.f_04 >= 8 && Mouse.f_04 < 168) {
		if(MouseMode == 0) {
			si = MkDirection(Mouse.f_02, Mouse.f_04);
			if(si >= 0) {
				MOUSE_POINTER(D_B623, si + 2);
				return;
			}
		}
	} else {
		if(MouseMode == 0) {
			MOUSE_POINTER(D_B623, 0);
			return;
		}
	}
	MOUSE_POINTER(D_B623, 1);
}

/*"put char" callback*/
void C_0C9C_0286(int ch, int x, int y) {
	int bp_02;

	x <<= 3;
	y <<= 3;
	bp_02 = CON_mouseOffAt(x, y, x + 7, y + 7);
	D_ECC4->_0b = D_04DF->_bg;
	D_ECC4->_0a = D_04DF->_fg;
	GR_1E(ch, x, y);
	CON_mouseOn(bp_02);
}

/*"scroll up" callback*/
void C_0C9C_02F1(int x, int y, int w, int h) {
	int bp_02;

	x <<= 3;
	y <<= 3;
	w <<= 3;
	h <<= 3;
	bp_02 = CON_mouseOffAt(x, y, x + w - 1, y + h - 1);
	GR_15(x, y, x + w - 1, y + h - 1, -8);
	GR_06(D_04DF->_bg);
	GR_12(x, y + h - 8, x + w - 1, y + h - 1);
	CON_mouseOn(bp_02);
}

/*Restore game?*/
C_0C9C_0397() {
	int si;

	IsArmageddon = 0;
	D_0340 = 1;
	SelectMode = 0;
	StatusDisplay = CMD_91;
	D_04B3 = 0;
	StatusDirty = 1;
	D_2E9E = ' ';
	C_1184_26E2();/*delete "savegame\\objblk??.tmp" files?*/
	for(si = 0; si < 0x45; si++)
		D_065C[si] = 0;
	C_1184_3B1D();
	C_101C_052D();
	setDefaultCommand(0xff);
	StatusDirty = 1;
	OSI_close(D_05CC);
	C_0C9C_042A();
	C_1E0F_512A();
	MUS_09A8();
	if(InCombat)
		COMBAT_begin();
	else
		COMBAT_breakOff();
	C_0A33_1355(0);
	PromptFlag = 1;
}

C_0C9C_042A() {
	int si, di;

	D_05DA = D_05DC = D_05DE = -1;

	DISK_confirm(DISK_4);
	si = OSI_open(/*057E*/"schedule");
	OSI_read(si, 0, (0x100 + 1) * sizeof(int), SchedPointer);
	OSI_read(si, -1, (long)SchedPointer[0x100] * sizeof(struct tSchedule), Schedule);
	OSI_close(si);

	DISK_confirm(DISK_4);
	si = OSI_open(/*0587*/"basetile");
	OSI_Read(si, 0L, 0x800L, BaseTile);
	OSI_close(si);

	DISK_confirm(DISK_8);
	si = OSI_open(/*0590*/"savegame\\objlist");
	OSI_read(si, 0, 0x100, ObjStatus);
	OSI_read(si, -1, 0x100 * sizeof(struct coord), ObjPos);
	OSI_read(si, -1, 0x200, ObjShapeType);
	OSI_read(si, -1, 0x200, Amount);
	OSI_read(si, -1, 0x100, NPCStatus);
	OSI_read(si, -1, 0x100, STREN);
	OSI_read(si, -1, 0x100, DEXTE);
	OSI_read(si, -1, 0x100, INTEL);
	OSI_read(si, -1, 0x200, ExpPoints);
	OSI_read(si, -1, 0x100, HitPoints);
	OSI_read(si, -1, 16 * 14, Names);
	OSI_read(si, -1, 0x10, Party);
	OSI_read(si, -1, 1, &PartySize);
	OSI_read(si, -1, 0x100, Level);
	OSI_read(si, -1, 0x100, SchedIndex);
	OSI_read(si, -1, 0x100, NPCMode);
	OSI_read(si, -1, 0x100, NPCComMode);
	OSI_read(si, -1, 0x100, MAGIC);
	OSI_read(si, -1, 0x100, MovePts);
	OSI_read(si, -1, 0x100 * sizeof(int), OrigShapeType);
	OSI_read(si, -1, 0x100, TalkFlags);
	OSI_read(si, -1, 0x100, Leader);
	OSI_read(si, -1, 0x100, NPCFlag);
	OSI_read(si, -1, 0x100, D_8C42);
	OSI_read(si, -1, (long)((char *)&D_2CCC - (char *)&obj_2C4A), &obj_2C4A);
	OSI_close(si);

	C_1184_3B7D();
	VarInt['Q' - 0x37] = IsOnQuest;
	MapX = GetX(Party[Active]);
	MapY = GetY(Party[Active]);
	MapZ = GetZ(Party[Active]);

	for(di = 0; di < NumAnimData; di ++) {
		if(
			D_67E4[di] == BaseTile[OBJ_125] ||
			D_67E4[di] == BaseTile[OBJ_120] ||
			D_67E4[di] == BaseTile[OBJ_0EC]
		)
			StateAnimData[di] = 0;
		else
			StateAnimData[di] = 1;
	}

	setDefaultCommand(DefaultCommand);
	LoadNewRegions(MapX, MapY, MapZ);
	C_101C_00D7(MapX, MapY, MapZ);

	DISK_confirm(DISK_5);
	D_05CC = OSI_open(/*05A1*/"chunks");
	D_06E6 = 1;
	C_101C_054C();
	if(IN_VEHICLE) {
		for(di = 0; di < PartySize; di++)
			InsertObj(Party[di], 0, INVEN);
	}
	C_1100_0306();
}

/*Save game?*/
C_0C9C_089F() {
	int si, di;

	D_2E9E = ' ';
	D_05DA = D_05DC = D_05DE = -1;
	if(IN_VEHICLE) {
		for(di = 0; di < PartySize; di++)
			MoveObj(Party[di], MapX, MapY, MapZ);
	}
	DISK_confirm(DISK_8);
	C_1184_33CA();
	IsOnQuest = VarInt['Q' - 0x37];

	DISK_confirm(DISK_8);
	si = OSI_create(/*0590*/"savegame\\objlist");
	OSI_write(si, 0, 0x100, ObjStatus);
	OSI_write(si, -1, 0x100 * sizeof(struct coord), ObjPos);
	OSI_write(si, -1, 0x200, ObjShapeType);
	OSI_write(si, -1, 0x200, Amount);
	OSI_write(si, -1, 0x100, NPCStatus);
	OSI_write(si, -1, 0x100, STREN);
	OSI_write(si, -1, 0x100, DEXTE);
	OSI_write(si, -1, 0x100, INTEL);
	OSI_write(si, -1, 0x200, ExpPoints);
	OSI_write(si, -1, 0x100, HitPoints);
	OSI_write(si, -1, 16 * 14, Names);
	OSI_write(si, -1, 0x10, Party);
	OSI_write(si, -1, 1, &PartySize);
	OSI_write(si, -1, 0x100, Level);
	OSI_write(si, -1, 0x100, SchedIndex);
	OSI_write(si, -1, 0x100, NPCMode);
	OSI_write(si, -1, 0x100, NPCComMode);
	OSI_write(si, -1, 0x100, MAGIC);
	OSI_write(si, -1, 0x100, MovePts);
	OSI_write(si, -1, 0x100 * sizeof(int), OrigShapeType);
	OSI_write(si, -1, 0x100, TalkFlags);
	OSI_write(si, -1, 0x100, Leader);
	OSI_write(si, -1, 0x100, NPCFlag);
	OSI_write(si, -1, 0x100, D_8C42);
	OSI_write(si, -1, (long)((char *)&D_2CCC - (char *)&obj_2C4A), &obj_2C4A);
	OSI_close(si);

	C_101C_054C();
	if(IN_VEHICLE) {
		for(di = 0; di < PartySize; di++)
			InsertObj(Party[di], 0, INVEN);
	}
	StatusDirty ++;
}

C_0C9C_0BCE(int ftile, int btile, int x, int y) {
	GR_42(btile, 0, 0);
	GR_42(ftile, 0, 0);
	/*backbuffer {0,0} 15x15 -> screen {x,y}*/
	GR_45(0, 0, 15, 15, x, y);
}

/*display 3x5 character?
'0'-'9' and 'A'-'P'*/
C_0C9C_0C10(int bp08, int bp06) {
	int si, di;
	unsigned bp_02;

	D_ECC4->_0f = 1;
	bp_02 = D_04E3[bp08];
	GR_06(D_2A56);
	for(di = 11; di < 16; di++) {
		for(si = bp06; si < bp06 + 3; si ++) {
			if(bp_02 & 0x8000) {
				GR_09(si, di);
			}
			bp_02 <<= 1;
		}
	}
	D_ECC4->_0f = 0;
}

/*display integer in 3x5 font?*/
C_0C9C_0C7D(int ftile, int val, int x, int y) {
	int si, bp_02;

	GR_42(TIL_19B, 0, 0);
	GR_42(ftile, 0, 0);
	if(val < 10)
		si = 1;
	else if(val < 100)
		si = 2;
	else if(val < 1000)
		si = 3;
	else if(val < 10000)
		si = 4;
	else
		si = 0;
	for(bp_02 = 0; bp_02 < si; bp_02 ++) {
		C_0C9C_0C10(val % 10, D_0517[si - 1][bp_02]);
		val /= 10;
	}
	GR_45(0, 0, 15, 15, x, y);
}

/*display object quality/quantity*/
C_0C9C_0D31(int objNum, int x, int y) {
	int bp_04, ftile;

	bp_04 = QuanType(ObjShapeType[objNum]);
	ftile = TILE_FRAME(objNum);
	if(GetType(objNum) == OBJ_040) {
		GR_42(TIL_19B, 0, 0);
		GR_42(ftile, 0, 0);
		C_0C9C_0C10(GetQual(objNum) + 9, 7);
		GR_45(0, 0, 15, 15, x, y);
	} else if(bp_04 == 4) {
		GR_2D(TIL_19B, x, y);
		C_0C9C_0C7D(ftile, Amount[objNum], x, y);
	} else if(bp_04 == 2) {
		GR_2D(TIL_19B, x, y);
		C_0C9C_0C7D(ftile, GetQuan(objNum), x, y);
	} else {
		C_0C9C_0BCE(ftile, TIL_19B, x, y);
	}
}

C_0C9C_0E57(int ftile, int btile, int x, int y) {
	C_0C9C_0BCE(ftile, btile, x, y);
}

/*C_0C9C_0E6E*/CON_prompt() {
	int promptX, promptY;
	int ch, bp_02;

	/*backup/set new fg*/
	bp_02 = D_04DF->_fg;
	D_04DF->_fg = D_2A56;

	D_B729 = D_04DF->_h;
	if(D_049F) {
		D_04DF->_attr &= ~0x10;
		D_049F = 0;
	}
	D_2FFE = 0;
	if(D_04DF->_curx >= D_04DF->_w)
		CON_putch0(0);
	promptX = D_04DF->_curx + D_04DF->_x0;
	promptY = D_04DF->_cury + D_04DF->_y0;
	ch = C_31FA_000C(D_04AB);/*keyboard function*/
	if(ch == 0) {/*animated cursor*/
		(*D_04CC)(PromptCh + ((PromptCnt ++) & 3), promptX, promptY);
		D_033E = 1;
		if(D_049C) {
			OtherAnimations();
			PaletteAnimation();
		} else {
			OSI_delay(1);
		}
		ServeMouse
	}
	(*D_04CC)(' ', promptX, promptY);
	D_2FFE = 1;

	/*restore fg*/
	D_04DF->_fg = bp_02;

	return ch;
}

C_0C9C_0F52(int ch) {
	int si, di;

	si = D_04DF->_curx + D_04DF->_x0;
	di = D_04DF->_cury + D_04DF->_y0;
	D_2FFE = 0;
	if(ch == '\b') {/*backspace*/
		if(D_04DF->_curx) {
			D_04DF->_curx --;
			si --;
		} else if(D_04DF->_cury) {
			D_04DF->_cury --;
			di --;
			D_04DF->_curx = D_04DF->_w - 1;
			si = D_04DF->_curx + D_04DF->_x0;
		}
		(*D_04CC)(' ', si, di);
	} else {
		(*D_04CC)(ch, si, di);
		if(++D_04DF->_curx >= D_04DF->_w) {
			D_04DF->_curx = 0;
			if(D_04DF->_cury < D_04DF->_h - 1) {
				D_04DF->_cury ++;
			} else if(D_04DF->_attr & 0x20) {
				D_2FFE = 1;
				(*D_04D0)(D_04DF->_x0, D_04DF->_y0, D_04DF->_w, D_04DF->_h);
				D_2FFE = 0;
			}
		}
	}
	D_2FFE = 1;
}

/*uncalled?*/
__0C9C_102A(unsigned char *bp0a, int bp08, int bp06) {
	while(*bp0a) {
		(*D_04CC)(*bp0a++, bp08++, bp06);
	}
}

static /*C_0C9C_1054*/mkMouseDir() {
	int si, di;
	int bp_02;

	si = SCR2TIL(PointerX);
	di = SCR2TIL(PointerY);
	bp_02 = MkDirection(PointerX, PointerY);

	if(AllowMouseMov) {
		if(
			MouseTrigger || MouseMapX == -1 ||
			(MouseMapX == MapX && abs(MouseMapY - MapY) < 2) ||
			(MouseMapY == MapY && abs(MouseMapX - MapX) < 2) ||
			si != D_0527 || di != D_0529
		) {
			MouseMapX = si + MapX - 5;
			MouseMapY = di + MapY - 5;
			D_0527 = si;
			D_0529 = di;
		}
	} else {
		MouseMapX =
		MouseMapY = -1;
	}

	return bp_02;
}

static /*C_0C9C_1107*/mkMouseSelection() {
	int si;

	D_B6DF = 0;
	if(PointerX < 168) {
		Selection.x = SCR2TIL(PointerX) + MapX - 5;
		Selection.y = SCR2TIL(PointerY) + MapY - 5;
		if(Obj_11x11[SCR2TIL(PointerY)][SCR2TIL(PointerX)] || Tile_11x11[SCR2TIL(PointerY)][SCR2TIL(PointerX)] >= 0x200) {
			Selection.obj = C_2337_08F1(Party[Active], Selection.x, Selection.y);
			return CMD_9D;
		}
		Selection.obj = -1;
		si = Tile_11x11[SCR2TIL(PointerY)][SCR2TIL(PointerX)];
		D_B6DF = (si == TIL_0FF || si == TIL_1BC);
	} else if(StatusDisplay == CMD_92) {
		Selection.x = -1;
		Selection.y = -1;
		if(C_155D_1267())
			return CMD_9B;
		if(C_155D_130E())
			return CMD_9C;
	} else {
		Selection.x = -1;
		Selection.y = -1;
		Selection.obj = -1;
	}
	return 0;
}

/*Status:Party*/
static /*C_0C9C_121F*/do_CMD_91() {
	int si;

	si = 0;
	if(PointerY > 24) {
		if(PointerX > 192) {/*to portrait view*/
			si = CMD_90;
			Selection.obj = SCR2TIL(PointerY - 28) + D_07CC;
			if(PartySize <= Selection.obj)
				si = 0;
		} else if(PointerX > 176) {/*to inventory view*/
			si = CMD_92;
			Selection.obj = SCR2TIL(PointerY - 24) + D_07CC;
			if(PartySize <= Selection.obj)
				si = 0;
		} else if(PointerY >= 24 && PointerY < 40) {/*party up*/
			if(D_07CC != 0)
				si = CMD_93;
		} else if(PointerY >= 88) {/*party down*/
			if(D_07CC < PartySize - 5)
				si = CMD_94;
		}
	}

	return si;
}

/*Status:Portrait*/
static /*C_0C9C_12B7*/do_CMD_90() {
	int si;

	si = 0;
	if(PointerY >= 88) {
		if(PointerX >= 192 && PointerX < 208) {/*to party view*/
			si = CMD_91;
		} else if(PointerX >= 208 && PointerX < 224) {/*to inventory view*/
			si = CMD_92;
			Selection.obj = -1;
		} else if(PointerX >= 224 && PointerX < 240) {/*next char*/
			si = CMD_99;
		} else if(PointerX >= 176 && PointerX < 192) {/*previous char*/
			si = CMD_9A;
		}
	}
	return si;
}

/*Status:Equipment*/
static /*C_0C9C_131D*/do_CMD_92() {
	int si, di;
	int bp_02;

	si = 0;
	if(IsInConversation)
		return si;

	if(
		D_04B3 != 0 &&
		PointerX >= 240 && PointerX < 256 && 
		PointerY >= 88 && PointerY < 104
	) {
		NPCComMode[Party[D_04B3]] ++;
		if(NPCComMode[Party[D_04B3]] == AI_ASSAULT + 1)
			NPCComMode[Party[D_04B3]] = AI_COMMAND;
		if(
			InCombat && 
			Active != D_04B3 && 
			!IsCharmed(Party[D_04B3])
		) NPCMode[Party[D_04B3]] = NPCComMode[Party[D_04B3]];
		STAT_refreshComMode();
	} else if(PointerY >= 88) {
		if(PointerX >= 192 && PointerX < 208) {/*to party view*/
			si = CMD_91;
		} else if(PointerX >= 208 && PointerX < 224) {/*to portrait view*/
			si = CMD_90;
			Selection.obj = -1;
		} else if(PointerX >= 224 && PointerX < 240) {/*next char*/
			si = CMD_99;
		} else if(PointerX >= 176 && PointerX < 192) {/*previous char*/
			si = CMD_9A;
		}
	} else if(PointerX >= 240 && PointerX < 248) {
		if(PointerY >= 32 && PointerY < 40) {/*inventory up*/
			if(D_07CE != 0)
				si = CMD_95;
		} else if(PointerY >= 72 && PointerY < 80) {/*inventory down*/
			if(InvenSize - D_07CE > 12)
				si = CMD_96;
		}
	} else if(
		PointerX >= 272 && PointerX < 288 &&
		PointerY >= 16 && PointerY < 32 &&
		MouseMode == 0
	) {/*inventory out/up?*/
		si = CMD_9F;
	} else if(MouseMode == 0 && D_04C2 != CMD_87) {
		bp_02 = mkMouseSelection();
		if(Selection.obj != -1) {
			if(bp_02 == CMD_9B) {
				di = GetType(Selection.obj);
				if(!C_27A1_0082(di) || di == OBJ_039) {
					si = CMD_A7;
				} else if(di == OBJ_062 && (GetFrame(Selection.obj) & 2)) {
					CON_printf(/*05A8*/"It's locked.\n");
					CON_printf(/*05B6*/"\n%s:\n%>", Names[Active]);
					CON_putch0(-1);
				} else if(
					di == OBJ_062 && GetFrame(Selection.obj) == 1 &&
					FindInvType(Selection.obj, OBJ_151, SPELL_16) != -1
				) {
					C_27A1_28A3(Selection.obj);
					CON_printf(/*05B6*/"\n%s:\n%>", Names[Active]);
					si = CMD_A0;
				} else {
					si = CMD_A0;
				}
			} else if(bp_02 == CMD_9C) {
				si = CMD_A8;
			}
		}
	}

	return si;
}

/*Status:Spellbook*/
static /*C_0C9C_15EB*/do_CMD_A1() {
	int si, di;

	si = 0;
	if(
		PointerY >= 88 && PointerY < 104 &&
		PointerX >= 176 && PointerX < 240
	) {
		/*navigation button?*/
		if(D_EBAD[di = SCR2TIL(PointerX - 176)] != 0xff)
			si = CMD_A2 + di;
		Selection.x = Selection.y = -1;
	} else if(PointerX > 184 && PointerY >= 16 && PointerY < 80) {
		/*selected spell?*/
		si = CMD_8E;
		Selection.obj = (PointerY - 16) >> 3;
		if(D_EBAA <= Selection.obj)
			si = 0;
		Selection.x = Selection.y = -1;
	}
	return si;
}

static /*C_0C9C_166D*/setDefaultCommand(int bp06) {
	int si;

	HideMouse
	if(DefaultCommand != 0xff) {
		GR_06(D_2A54);
		si = TIL2SCR(DefaultCommand - CMD_81) + 8;
		GR_12(si, 192, si + 15, 192);
	}
	DefaultCommand = bp06;
	if(DefaultCommand != 0xff) {
		GR_06(D_2A4E);
		si = TIL2SCR(DefaultCommand - CMD_81) + 8;
		GR_12(si, 192, si + 15, 192);
	}
	ShowMouse
}

static /*C_0C9C_1714*/mkMouseCommand() {
	int mouse_cmd, zone;
	int mouse_dir;

	mouse_cmd = 0;
	
	for(zone = 0; ; zone ++) {
		if(
			D_052B[zone][0] <= PointerX && D_052B[zone][2] >= PointerX &&
			D_052B[zone][1] <= PointerY && D_052B[zone][3] >= PointerY
		) break;
	}
	switch(zone) {
		case 0:/*map view*/
			if(MouseMode == 0) {
				if(DefaultCommand != 0xff && RIGHT_CLICK) {
					if(MouseTrigger) {
						mouse_cmd = CMD_A9;
						mkMouseSelection();
					}
				} else {
					mouse_dir = mkMouseDir();
					if(mouse_dir == -1) {
						if(MouseTrigger)
							mouse_cmd = CMD_8F;
						else
							mouse_cmd = 0;
					} else if(MouseTrigger || D_17AE == 0 || mouse_dir != AdvanceDir) {
						mouse_cmd = CMD_80;
						AdvanceDir = mouse_dir;
					}
				}
			} else if(MouseTrigger) {
				mouse_cmd = CMD_8E;
				mkMouseSelection();
			}
		break;
		case 1:/*icons*/
			if(MouseTrigger) {
				if(LEFT_CLICK)
					mouse_cmd = CMD_81 + SCR2TIL(PointerX - 8);
				else
					setDefaultCommand(CMD_81 + SCR2TIL(PointerX - 8));
			}
		break;
		case 2:/*party/portait/equip view*/
			if(MouseTrigger) {
				if(DefaultCommand != 0xff && MouseMode == 0 && RIGHT_CLICK) {
					mouse_cmd = CMD_A9;
					mkMouseSelection();
				} else {
					switch(StatusDisplay) {
						case CMD_91: mouse_cmd = do_CMD_91(); break;
						case CMD_90: mouse_cmd = do_CMD_90(); break;
						case CMD_92:
							mouse_cmd = do_CMD_92();
							if(mouse_cmd == 0 && MouseMode == 1 && mkMouseSelection())
								mouse_cmd = CMD_8E;
						break;
						case CMD_A1: mouse_cmd = do_CMD_A1(); break;
					}
				}
			}
		break;
		case 3:/*all screen*/
			if(MouseTrigger) {
				mouse_cmd = CMD_8F;
				Selection.x =
				Selection.y = -1;
			}
		break;
	}

	return mouse_cmd;
}

/*check if spell in spell list*/
static /*C_0C9C_18D8*/__Incant2Spell() {
	int i;
	unsigned char *di;
	int spellNum;
	long spellMask;

	spellNum = -1;
	for(i = 0; i < 8 * 0x10; i ++) {
		if(SpellList[i >> 4] & (1 << (i & 0xf))) {
			spellMask = 0;
			for(di = SpellCode[i]; *di; di ++)
				spellMask |= 1L << (*di - 'A');
			if(spellMask == D_04B6) {
				spellNum = i;
				break;
			}
		}
	}
	if(spellNum < 0)
		CON_printf(NotInBookMsg);
	CON_printf(/*05A8+0Ch*/"\n");
	D_04B6 = 0;
	SelectMode = 1;

	return spellNum;
}

static C_0C9C_1991(int spellNum) {
	D_EBB3 = spellNum;
	if(Spellbook[0])
		SetQual(Spellbook[0], spellNum);
	if(Spellbook[1])
		SetQual(Spellbook[1], spellNum);
}

/*spellbook navigation?*/
static C_0C9C_19D6() {
	int si, di;

	if(AdvanceDir == 2 && D_EBAD[2] != 0xff) {
		di = CMD_A4;
		D_0496 = 1;
	} else if(AdvanceDir == 6 && D_EBAD[1] != 0xff) {
		di = CMD_A3;
		D_0496 = 1;
	} else {
		if(AdvanceDir == 0) {
			si = C_1944_06CA(D_EBB3);
			if(si == -1)
				si = 0;
			if(si) {
				CON_setClip(&D_B6B5[1]);
				CON_gotoxy(0, si + 1);
				CON_putch0(' ');
				CON_gotoxy(0, si);
				CON_putch0(0x1a);
				CON_setClip(&D_B6B5[3]);
				C_0C9C_1991(D_EBB6[si-1]);
			}
		} else if(AdvanceDir == 4) {
			si = C_1944_06CA(D_EBB3);
			if(si == -1)
				si = 0;
			if(D_EBAA - 1 > si && si < 7) {
				CON_setClip(&D_B6B5[1]);
				CON_gotoxy(0, si + 1);
				CON_putch0(' ');
				CON_gotoxy(0, si + 2);
				CON_putch0(0x1a);
				CON_setClip(&D_B6B5[3]);
				C_0C9C_1991(D_EBB6[si+1]);
			}
		}
		di = 0;
	}
	return di;
}

/*draw equipment selector?*/
C_0C9C_1AE5(int bp06) {
	int x, y;
	int bp_06, bp_04, bp_02;

	if(SelectMode != 2)
		return;
	if(StatusDisplay != CMD_92 && StatusDisplay != CMD_90)
		return;
	if(D_04B3 == 0) {
		D_056E[0] =
		D_056E[4] = TIL_19B;
	} else {
		D_056E[0] = TIL_183;
		D_056E[4] = TIL_187;
	}
	if(PartySize - 1 == D_04B3)
		D_056E[3] = TIL_19B;
	else
		D_056E[3] = TIL_184;
	if(StatusDisplay == CMD_90)
		D_056E[2] = TIL_182;
	else
		D_056E[2] = TIL_181;
	if(StatusDisplay == CMD_90) {
		D_049A = 3;
		if(D_0499 > 3)
			D_0499 = 3;
	}
	if(D_049A < 0) {
		x = 272;
		PointerX = 272;
		y = 16;
		PointerY = 16;
	} else if(D_049A > 2) {
		if(D_0499 > 4)
			D_0499 = 4;
		x = D_0499 * 16 + 176;
		PointerX = x;
		y = 88;
		PointerY = y;
	} else if(D_0499 < 3) {
		PointerX = D_0499 * 24 + 176;
		PointerY = D_049A * 20 + 24;
	} else {
		PointerX = D_0499 * 16 + 200;
		PointerY = D_049A * 16 + 32;
	}
	if(D_049A >= 0 && D_049A < 3) {
		x = D_054B[D_0499];
		y = D_0559[D_049A][D_0499];
	}
	bp_02 = CON_mouseOffAt(x, y, x + 15, y + 15);
	if(bp06 == 1) {
		GR_2D(TIL_16D, x, y);
	} else if(bp06 == 0) {
		bp_06 = mkMouseSelection();
		if(D_049A > 2) {
			GR_2D(D_056E[D_0499], x, y);
		} else if((bp_06 == CMD_9B || bp_06 == CMD_9C) && Selection.obj >= 0) {
			if(D_049A < 0) {
				bp_04 = GetType(Selection.obj);
				C_0C9C_0BCE(OBJ_MakeDirFrame(bp_04, 4) + BaseTile[bp_04], TIL_19B, x, y);
			} else {
				C_0C9C_0D31(Selection.obj, x, y);
			}
		} else if(
			D_049A == 1 && D_0499 == 2 &&
			((!ShowNPCInven && Party_1Hand[D_04B3]) || (ShowNPCInven && NPC_1Hand))
		) {
			GR_2D(TIL_185, x, y);
		} else {
			GR_2D(TIL_19A, x, y);
		}
	} else if(bp06 == 2) {
		PointerX = x;
		PointerY = y;
	}
	CON_mouseOn(bp_02);
}

static C_0C9C_1D59() {
	int ptr_buttons, ptr_x;
	int ch, ptr_y, bp_06, bp_04, i;

	if(D_04AF)
		CON_putch0(-1);
	ch = 0;
	D_B729 = D_04DF->_h;
	while(!ch) {
		/* */
		if(MouseOn) {
			MOUSE_SERVE();
			ptr_buttons = (Mouse.f_00?1:0) | (Mouse.f_01?2:0);
			ptr_x = Mouse.f_02;
			ptr_y = Mouse.f_04;
			if(ptr_buttons & 3) {
				if(!MousePress) {
					MousePress =
					MouseTrigger = 1;
				} else {
					MouseTrigger = 0;
				}
				PointerX = ptr_x;
				PointerY = ptr_y;
				D_B72B = ptr_buttons;
				ch = mkMouseCommand();
				if(MouseTrigger)
					D_B64F = ch;
				else if(D_B64F != ch)
					ch = 0;
			} else {
				MousePress =
				MouseTrigger = 0;
			}
		}
		if(ch)
			continue;
		/* */
		ch = CON_prompt();
		if(ch) {
			MouseMapX =
			MouseMapY = -1;
		}
		if(D_04B1 && ch < 0x100)
			ch = toupper(ch);
		switch(ch) {
			case 0x148: ch = CMD_80; AdvanceDir = 0; break;/*up*/
			case 0x150: ch = CMD_80; AdvanceDir = 4; break;/*down*/
			case 0x14b: ch = CMD_80; AdvanceDir = 6; break;/*left*/
			case 0x14d: ch = CMD_80; AdvanceDir = 2; break;/*right*/
			case 0x147: ch = CMD_80; AdvanceDir = 7; break;/*up left*/
			case 0x149: ch = CMD_80; AdvanceDir = 1; break;/*up right*/
			case 0x14f: ch = CMD_80; AdvanceDir = 5; break;/*down left*/
			case 0x151: ch = CMD_80; AdvanceDir = 3; break;/*down right*/
			case 0x11e: setDefaultCommand(CMD_81); ch = 0; break;/*<ALT A>*/
			case 0x12e: setDefaultCommand(CMD_82); ch = 0; break;/*<ALT C>*/
			case 0x114: setDefaultCommand(CMD_83); ch = 0; break;/*<ALT T>*/
			case 0x126: setDefaultCommand(CMD_84); ch = 0; break;/*<ALT L>*/
			case 0x122: setDefaultCommand(CMD_85); ch = 0; break;/*<ALT G>*/
			case 0x120: setDefaultCommand(CMD_86); ch = 0; break;/*<ALT D>*/
			case 0x132: setDefaultCommand(CMD_87); ch = 0; break;/*<ALT M>*/
			case 0x116: setDefaultCommand(CMD_88); ch = 0; break;/*<ALT U>*/
			case 0x113: setDefaultCommand(CMD_89); ch = 0; break;/*<ALT R>*/
			case 0x130: setDefaultCommand(CMD_8A); ch = 0; break;/*<ALT B>*/
		}
		if(MouseMode == 0 && !D_0497) {
			C_0C9C_1AE5(0);
			SelectMode = 0;
		}
		if(D_04AF == 0 || (D_049B && ch == '\r'))
			continue;
		if(SelectMode != 0) {
			if(
				(D_04C2 == CMD_81 && ch == 'A') ||
				(D_04C2 == CMD_82 && ch == 'C' && SelectMode == 1) ||
				(D_04C2 == CMD_83 && ch == 'T') ||
				(D_04C2 == CMD_84 && ch == 'L') ||
				(D_04C2 == CMD_85 && ch == 'G') ||
				(D_04C2 == CMD_86 && ch == 'D') ||
				(D_04C2 == CMD_87 && ch == 'M') ||
				(D_04C2 == CMD_88 && ch == 'U')
			) ch = '\r';

			if(D_04C2 == CMD_82 && SelectMode == 3) {
				/*magic incantation?*/
				if(ch >= 'A' && ch <= 'Z') {
					if(D_04B5 < 4) {
						D_B6DB[D_04B5] = ch;
						D_B6A7[D_04B5] = D_04DF->_curx;
						D_B6AB[D_04B5] = D_04DF->_cury;
						CON_printf(D_0BF0[ch - 'A']);
						CON_putch(' ');
						CON_putch0(-1);
						if(D_04DF->_curx >= D_04DF->_w)
							CON_putch0(0);
						if(D_04DF->_cury == D_B6AB[D_04B5] && D_04DF->_curx < D_B6A7[D_04B5]) {
							for(i = 0; i <= D_04B5; i ++)
								D_B6AB[i] --;
						}
						D_04B5 ++;
					}
					ch = 0;
				} else if(ch == 0x10e/*backspace?*/ || ch == 0x7f) {
					if(D_04B5) {
						D_04B5 --;
						i = D_B6DB[D_04B5] - 'A';
						for(bp_06 = 0; D_0BF0[i][bp_06]; bp_06 ++);
						if(D_04DF->_curx > bp_06)
							CON_gotoxy(D_04DF->_curx - bp_06 - 1, D_04DF->_cury);
						else
							CON_gotoxy(D_B6A7[D_04B5], D_B6AB[D_04B5]);
						C_0C9C_32DE(bp_06, ' ');
						CON_gotoxy(D_B6A7[D_04B5], D_B6AB[D_04B5]);
					}
					ch = 0;
				} else {
					if(ch == ' ' || ch == 27) {
						D_04B6 = 0;
						D_04B5 = 0;
						ch = CMD_8F;
						Selection.x = Selection.y = -1;
					} else if(ch == '\r') {
						/*make spell mask*/
						D_04B6 = 0;
						for(i = 0; i < D_04B5; i ++)
							D_04B6 |= 1L << (D_B6DB[i] - 'A');
						if(D_04B6) {
							i = __Incant2Spell();
						} else {
							SelectMode = 1;
							if(Spellbook[0])
								i = GetQual(Spellbook[0]);
							else
								i = GetQual(Spellbook[1]);
						}
						if(i >= 0) {
							D_EBB3 = i;
							if(Spellbook[0])
								SetQual(Spellbook[0], i);
							if(Spellbook[1])
								SetQual(Spellbook[1], i);
							SPELL_RefreshList(Spellbook[0], Spellbook[1]/*bug?*/);
							SPELL_RefreshPage();
							Selection.obj = C_1944_06CA(i);
							Selection.x =
							Selection.y = -1;
							ch = CMD_8E;
						} else {
							ch = CMD_8F;
							Selection.x =
							Selection.y = -1;
						}
					} else if(ch == CMD_80) {
						ch = C_0C9C_19D6();
					} else {
						ch = 0;
					}
				}
			} else {
				if(ch == 9) {
					if(SelectMode == 1) {
						SelectMode = 2;
						if(StatusDisplay  != CMD_92) {
							if(IN_VEHICLE)
								D_04B3 = 0;
							else
								D_04B3 = Active;
							StatusDisplay = CMD_92;
							StatusDirty = 1;
							RefreshStatus();
						}
						C_0C9C_1AE5(1);
					} else if(StatusDisplay != CMD_9E && !IsInConversation) {
						C_0C9C_1AE5(0);
						SelectMode = 1;
					}
					C_0A33_09CE(1);
					ch = 0;
				} else {
					if(ch == ' ' || ch == 27) {
						C_0C9C_1AE5(0);
						SelectMode = 0;
						ch = 0;
					} else if(ch == '\r') {
						if(SelectMode == 1) {
							ch = CMD_8E;
							if(MouseMode == 0) {
								if(D_0497 && DefaultCommand != 0xff)
									ch = CMD_A9;
								D_0497 = 0;
							}
							SelectMode = 0;
							PointerX = TIL2SCR(AimX);
							PointerY = TIL2SCR(AimY);
							mkMouseSelection();
						} else if(SelectMode == 2) {
							if(StatusDisplay == CMD_92 || StatusDisplay == CMD_90) {
								C_0C9C_1AE5(2);
								if(StatusDisplay == CMD_92)
									ch = do_CMD_92();
								else
									ch = do_CMD_90();
								if(ch == 0 && MouseMode == 1 && mkMouseSelection())
									ch = CMD_8E;
							}
						}
					} else if(ch == CMD_80) {
						if(SelectMode == 1) {
							ch = 0;
							bp_06 = AimX + DirIncrX[AdvanceDir];
							bp_04 = AimY + DirIncrY[AdvanceDir];
							if(
								bp_06 >= 0 && bp_06 < 11 &&
								bp_04 >= 0 && bp_04 < 11 &&
								COMBAT_range(bp_06, bp_04, 5, 5) <= SelectRange
							) {
								AimX = bp_06;
								AimY = bp_04;
								C_0A33_09CE(1);
							} else if(SelectRange == -1) {
								ch = CMD_8E;
								PointerX = TIL2SCR(bp_06);
								PointerY = TIL2SCR(bp_04);
								mkMouseSelection();
							}
						} else if(SelectMode == 2 && StatusDisplay == CMD_92) {
							ch = 0;
							bp_06 = D_0499 + DirIncrX[AdvanceDir];
							bp_04 = D_049A + DirIncrY[AdvanceDir];
							if(
								bp_06 >= 0 && bp_06 < 7 &&
								bp_04 >= 0 && bp_04 < 3
							) {
								C_0C9C_1AE5(0);
								if(bp_06 == 1 && bp_04 == 1) {
									bp_06 += DirIncrX[AdvanceDir];
									bp_04 += DirIncrY[AdvanceDir];
								}
								D_0499 = bp_06;
								D_049A = bp_04;
								C_0C9C_1AE5(1);
							} else if((bp_06 > 2 && bp_06 < 7) || bp_04 > 2) {
								if(bp_04 > 2 && InvenSize - D_07CE > 12 && AdvanceDir == 4) {
									D_0499 = bp_06;
									ch = CMD_96;
								} else if(bp_04 < 0 && D_07CE) {
									D_0499 = bp_06;
									ch = CMD_95;
								} else if(bp_04 < 0 && D_EBB5 == 0) {
									C_0C9C_1AE5(0);
									D_049A = 0xff;
									C_0C9C_1AE5(1);
								} else if(bp_04 > 2 && D_EBB5 == 0) {
									C_0C9C_1AE5(0);
									D_049A = 3;
									D_0499 = bp_06;
									if(D_0499 > 4)
										D_0499 = 4;
									else if(D_0499 < 0)
										D_0499 = 0;
									C_0C9C_1AE5(1);
								}
							}
						} else if(SelectMode == 2 && StatusDisplay == CMD_90) {
							ch = 0;
							bp_06 = D_0499 + DirIncrX[AdvanceDir];
							if(bp_06 < 0)
								bp_06 = 0;
							else if(bp_06 > 3)
								bp_06 = 3;
							C_0C9C_1AE5(0);
							D_0499 = bp_06;
							D_049A = 3;
							C_0C9C_1AE5(1);
						}
					} else if(ch >= '1' && ch - '1' < PartySize && SelectMode == 1) {
						D_B6DF = 0;
						SelectMode = 0;
						Selection.obj = Party[ch - '1'];
						if(IN_VEHICLE) {
							Selection.x = MapX;
							Selection.y = MapY;
						} else {
							Selection.x = GetX(Selection.obj);
							Selection.y = GetY(Selection.obj);
						}
						ch = CMD_8E;
					}
				}
			}
		} else if(ch == 9 && StatusDisplay != CMD_9E && !IsInConversation) {
			if(StatusDisplay == CMD_92) {
				SelectMode = 2;
				C_0C9C_1AE5(1);
			} else if(StatusDisplay != CMD_A1) {
				SelectMode = 1;
			}
			if(SelectMode != 0) {
				AimX =
				AimY = 5;
				SelectRange = 7;
				D_0497 = 1;
				C_0A33_09CE(1);
				ch = 0;
			}
		}
		if(StatusDisplay == CMD_92 && !IsInConversation) {
			/*character equipment view*/
			if(ch == '*') {
				ch = CMD_90;
				Selection.obj = -1;
			} else if(ch == '/' || ch == 0x144/*F10*/) {
				ch = CMD_91;
			} else if(ch == '+') {
				ch = CMD_99;
			} else if(ch == '-') {
				ch = CMD_9A;
			} else if(ch == '`' || ch == '~') {
				PointerX = 248;
				PointerY = 96;
				do_CMD_92();
				ch = 0;
			} else if(ch >= 0x13b/*F1*/ && PartySize + 0x13b > ch) {
				Selection.obj = ch - 0x13b;
				ch = CMD_92;
			}
		} else if(StatusDisplay == CMD_90 && !IsInConversation) {
			/*character stat view*/
			if(ch == '*') {
				ch = CMD_92;
				Selection.obj = -1;
			} else if(ch == '/' || ch == 0x144/*F10*/) {
				ch = CMD_91;
			} else if(ch == '+') {
				ch = CMD_99;
			} else if(ch == '-') {
				ch = CMD_9A;
			} else if(ch >= 0x13b && PartySize + 0x13b > ch) {
				Selection.obj = ch - 0x13b;
				ch = CMD_90;
			}
		} else if(StatusDisplay == CMD_91 && !IsInConversation) {
			if(ch == '+' && D_07CC < PartySize - 5) {
				ch = CMD_94;
			} else if(ch == '-' && D_07CC != 0) {
				ch = CMD_93;
			} else if(ch >= 0x13b/*F1*/ && PartySize + 0x13b > ch) {
				Selection.obj = ch - 0x13b;
				ch = CMD_92;
			}
		} else if(StatusDisplay == CMD_A1 && !IsInConversation) {
			/*look at the spellbook*/
			if(ch == ' ' || ch == 27) {
				ch = CMD_8F;
				Selection.x = Selection.y = -1;
			} else if(ch == '\r') {
				SelectMode = 1;
				if(Spellbook[0])
					i = GetQual(Spellbook[0]);
				else
					i = GetQual(Spellbook[1]);
				if(i < 0) {
					ch = 0;
				} else {
					D_EBB3 = i;
					if(Spellbook[0])
						SetQual(Spellbook[0], i);
					if(Spellbook[1])
						SetQual(Spellbook[1], i);
					SPELL_RefreshList(Spellbook[0], Spellbook[1]/*bug?*/);
					SPELL_RefreshPage();
					Selection.obj = C_1944_06CA(i);
					Selection.x =
					Selection.y = -1;
					ch = CMD_8E;
				}
			} else if(ch == CMD_80) {
				ch = C_0C9C_19D6();
			}
		}
	}

	return ch;
}

static C_0C9C_2A16() {
	SPELL_RefreshPage();
	if(D_0496) {
		D_0496 = 0;
		C_0C9C_1991(D_EBB6[0]);
		CON_setClip(&D_B6B5[1]);
		CON_gotoxy(0, 1);
		CON_putch0(0x1a);
		CON_setClip(&D_B6B5[3]);
	}
}

/*get char?
wait for a char (or a mouse button)
to be pressed*/
/*C_0C9C_2A59*/CON_getch() {
	int si, ch;
	int endLoop, bp_04, bp_02;

	if(D_0578) {
		D_0578 = 0;
		return CMD_8E;
	}
	do {
		endLoop = 0;
		ch = C_0C9C_1D59();
		switch(ch) {
			case CMD_A9:
				ch = DefaultCommand;
				if(ch != CMD_82 && ch != CMD_89 && ch != CMD_AA)
					D_0578 = 1;
				endLoop = 1;
			break;
			case CMD_91:
				SelectMode = 0;
				StatusDisplay = CMD_91;
				D_04B3 = 0;
				StatusDirty = 1;
			break;
			case CMD_90:
				StatusDisplay = CMD_90;
				if(Selection.obj >= 0)
					D_04B3 = Selection.obj;
				StatusDirty = 1;
			break;
			case CMD_92:
				StatusDisplay = CMD_92;
				if(Selection.obj >= 0)
					D_04B3 = Selection.obj;
				StatusDirty = 1;
			break;
			case CMD_9A:/*portrait/equipment select --*/
				if(D_04B3 > 0) {
					D_04B3 --;
					StatusDirty = 1;
					Selection.obj = -1;
				}
			break;
			case CMD_99:/*portrait/equipment select ++*/
				if(PartySize - 1 > D_04B3) {
					D_04B3 ++;
					StatusDirty = 1;
					Selection.obj = -1;
				}
			break;
			case CMD_93:/*party select --*/
				D_07CC --;
				StatusDirty = 1;
			break;
			case CMD_94:/*party select --*/
				D_07CC ++;
				StatusDirty = 1;
			break;
			case CMD_95:/*equipment scroll?*/
				C_155D_123E(D_04B3);
				C_0C9C_1AE5(1);
			break;
			case CMD_96:/*equipment scroll?*/
				C_155D_120A(D_04B3);
				C_0C9C_1AE5(1);
			break;
			case CMD_9F:/*close container?*/
				if(D_E709 < 0x100) {
					StatusDisplay = CMD_91;
					D_04B3 = 0;
					StatusDirty = 1;
				} else {
					D_E709 = GetAssoc(D_E709);
					D_07CE = 0;
					C_155D_1022(D_E709);
					C_0C9C_1AE5(1);
				}
			break;
			case CMD_A0:/*open container?*/
				D_E709 = Selection.obj;
				D_07CE = 0;
				C_155D_1022(D_E709);
				C_0C9C_1AE5(1);
			break;
			case CMD_A7:/*Ready ...*/
				C_155D_144B();
				C_0C9C_1AE5(1);
			break;
			case CMD_A8:/*Unready ...*/
				C_155D_1738();
				C_0C9C_1AE5(1);
			break;
			case CMD_A2:/*spellbook related?*/
				D_EBB1 = 0;
				for(si = 0; si <= D_EBAD[0]; si ++) {
					for(bp_04 = 0; bp_04 < 16; bp_04 ++) {
						if(HAS_SPELL(si, bp_04)) {
							if(D_EBAD[0] == si)
								break;
							D_EBB1 ++;
						}
					}
				}
				SPELL_RefreshPage();
			break;
			case CMD_A5:/*spellbook related?*/
				D_EBB1 = 0;
				for(si = 0; si <= D_EBB0; si ++) {
					for(bp_04 = 0; bp_04 < 16; bp_04 ++) {
						if(HAS_SPELL(si, bp_04)) {
							if(D_EBB0 == si)
								break;
							D_EBB1 ++;
						}
					}
				}
				SPELL_RefreshPage();
			break;
			case CMD_A3:/*spellbook related?*/
				if(D_EBAD[1] == 2) {
					D_EBB1 -= 8;
				} else {
					bp_02 = D_EBB1;
					D_EBB1 = 0;
					for(si = 0; si <= D_EBAD[0]; si ++) {
						for(bp_04 = 0; bp_04 < 16; bp_04 ++) {
							if(HAS_SPELL(si, bp_04)) {
								if(D_EBAD[0] == si)
									break;
								D_EBB1 ++;
							}
						}
					}
					if(bp_02 - D_EBB1 > 8)
						D_EBB1 += 8;
				}
				C_0C9C_2A16();
			break;
			case CMD_A4:/*spellbook related?*/
				if(D_EBAD[2] == 2) {
					D_EBB1 += 8;
				} else {
					D_EBB1 = 0;
					for(si = 0; si <= D_EBB0; si ++) {
						for(bp_04 = 0; bp_04 < 16; bp_04 ++) {
							if(HAS_SPELL(si, bp_04)) {
								if(D_EBB0 == si)
									break;
								D_EBB1 ++;
							}
						}
					}
				}
				C_0C9C_2A16();
			break;
			default:
				endLoop = 1;
			case CMD_A6: ;
		}
		if(!endLoop)
			RefreshStatus();
	} while(!endLoop);

	return ch;
}

/*C_0C9C_2D3A*/CON_gets(char *bp08, int bp06) {
	int si, di;
	int ch;

	D_049B = 1;
	D_04B1 = 0;
	CON_putch0(-1);
	si =
	di = 0;
	D_04AB = 0;
	while(!di) {
		while(!(ch = CON_prompt()));
		switch(ch) {
			case '\r':
				bp08[si] = 0;
				D_04AD = 0;
				di ++;
			break;
			case 27:
				bp08[0] = 0;
				while(si --)
					C_0C9C_0F52(8);
				D_04AD = 1;
				di ++;
			break;
			case 0x10e:
				if(si) {
					si --;
					C_0C9C_0F52(8);
				}
			break;
			default:
				if(si < bp06 && ch >= ' ' && ch <= '~') {
					bp08[si] = ch;
					si ++;
					C_0C9C_0F52(ch);
				}
		}
	}
	D_04AB = 1;
	D_B729 = D_04DF->_h;
	D_04B1 = 1;
	D_049B = 0;
	return si;
}

/*C_0C9C_2E09*/CON_createClip(struct tClipInfo *si/*bp0e*/, int bp0c, int bp0a, int bp08, int bp06) {
	D_04DF = si;
	si->_x0 = bp0c;
	si->_y0 = bp0a;
	si->_w = bp08 - bp0c + 1;
	si->_h = bp06 - bp0a + 1;
	si->_curx = si->_cury = 0;
	si->_bg = D_2A54;
	si->_fg = D_2A56;
	si->_attr = D_04E1;
}

/*C_0C9C_2E54*/CON_setClip(struct tClipInfo *bp06) {
	CON_putch0(-1);
	D_04DF = bp06;
	D_ECC4->_0b = D_04DF->_bg;
	D_ECC4->_0a = D_04DF->_fg;
	CON_putch0(-1);
}

/*C_0C9C_2E8F*/CON_gotoxy(int bp08, int bp06)
{
	if(bp08 < 0)
		bp08 = 0;
	if(bp06 < 0)
		bp06 = 0;
	if(bp08 > D_04DF->_w)
		bp08 = D_04DF->_w;
	if(bp06 > D_04DF->_h)
		bp06 = D_04DF->_h;
	CON_putch0(-1);
	D_04DF->_curx = bp08;
	D_04DF->_cury = bp06;
	CON_putch0(-1);
}

/*C_0C9C_2EFA*/CON_putch0(int bp06) {
	int si, di;
	int bp_06, bp_04, bp_02;

	if(!TextOn)
		return;
	if(D_04A9 && (D_04DF->_attr & 0x20)) {
		if(bp06 == '\n')
			return;
		if(bp06 >= 0)
			D_04A9 = 0;
	}
	D_2FFE = 0;
	if((D_04DF->_attr & 4) && D_0498)
		bp06 = toupper(bp06);
	D_0498 = 0;
	if(bp06 == 0)
		bp06 = '\n';
	if((D_04DF->_attr & 8) && bp06 > ' ')
		bp06 |= 0x80;
	if(D_04DF->_w == D_057A || bp06 == '\n' || bp06 < 0) {
		if(D_B729 && bp06 >= 0 && (D_04DF->_attr & 0x10))
			D_B729 --;
		if((D_04DF->_attr & 0x10) && D_B729 == 0 && bp06 >= 0) {
			bp_04 = (D_04DF->_w >> 1) - 2;
			bp_02 = (D_04DF->_h + D_04DF->_y0) - 1;
			D_04AF = 0;
			PromptCh = 1;
			do
				di = CON_getch();
			while(di != ' ' && di != '\r' && di != 27 && di != CMD_8F);
			PromptCh = 5;
			D_04AF = 1;
			D_2FFE = 0;
			for(si = 0; si < D_04DF->_w; si ++)
				(*D_04CC)(' ', D_04DF->_x0 + si, bp_02);
			D_B729 = D_04DF->_h - 1;
		}
		bp_06 = D_057A;
		if((D_04DF->_attr & 1) && D_04DF->_w == D_057A && bp06 >= 0) {
			D_B651[D_057A] = (bp06 == '\n')?' ':bp06;
			D_B67A[D_057A] = D_04DF->_fg;
			while(D_B651[bp_06] != ' ' && D_B651[bp_06 - 1] != '-' && D_057B < bp_06)
				bp_06 --;
			if(bp_06 == 0)
				bp_06 = D_057A;
		}
		bp_02 = D_04DF->_y0 + D_04DF->_cury;
		if(D_04DF->_attr & 2) {
			for(si = D_057B; si < ((D_04DF->_w - bp_06) >> 1) + D_057B; si++)
				(*D_04CC)(' ', D_04DF->_x0 + si, bp_02);
		}
		for(si = D_057B; si < bp_06; si ++) {
			D_04DF->_fg = D_B67A[si];
			(*D_04CC)(D_B651[si], D_04DF->_x0 + si, bp_02);
		}
		D_04DF->_fg = D_2A56;
		if(D_057B < bp_06)
			D_04DF->_curx = bp_06;
		if((D_04DF->_attr & 1) && D_04DF->_w == D_057A && bp06 >= 0) {
			if(D_B651[bp_06] == ' ')
				bp_06 ++;
			for(si = bp_06; si <= D_057A; si ++) {
				D_B651[si - bp_06] = D_B651[si];
				D_B67A[si - bp_06] = D_B67A[si];
			}
			D_057B = 0;
			D_057A = si - bp_06;
			if(bp06 == ' ')
				D_057C = 0;
		} else if(bp06 >= 0) {
			D_057A = D_057B = 0;
			if(bp06 >= 0 && bp06 != '\n') {
				D_B67A[D_057A] = D_04DF->_fg;
				D_B651[D_057A] = bp06;
				D_057A ++;
			}
		}
		if(bp06 >= 0) {
			D_04DF->_curx = 0;
			if(D_04DF->_cury < D_04DF->_h - 1) {
				D_04DF->_cury ++;
			} else if(D_04DF->_attr & 0x20) {
				D_2FFE = 1;
				(*D_04D0)(D_04DF->_x0, D_04DF->_y0, D_04DF->_w, D_04DF->_h);
				D_2FFE = 0;
			}
		} else {
			D_057B = D_057A = D_04DF->_curx;
			D_04A3 = 0;
		}
	} else if((D_04DF->_attr & 1) == 0 || bp06 != ' ' || D_057C) {
		D_B67A[D_057A] = D_04DF->_fg;
		D_B651[D_057A] = bp06;
		D_057A ++;
	}
	if(bp06 != ' ')
		D_057C = 1;
	D_2FFE = 1;
}

C_0C9C_32DE(int bp08, int bp06) {
	int si;

	for(si = 0; si < bp08; si ++)
		CON_putch0(bp06);
}

C_0C9C_32F8(unsigned char far *bp06) {
	while(*bp06)
		CON_putch0(*bp06++);
}

/*C_0C9C_331A*/CON_atoi(unsigned number, char far *str, int maxLen, int base) {
	int si;
	char bp_0e[8];
	int bp_06, isNegative, bp_02;

	isNegative = 0;
	si = 0;
	if(base < 0) {
		base = -base;
		if(number > 0x7fff) {
			number = -number;
			isNegative = 1;
			maxLen --;
		}
	}
	if(base) {
		while(number && si < maxLen) {
			bp_02 = number % base;
			bp_0e[si++] = (bp_02 < 10)?(bp_02 + '0'):(bp_02 - 10 + 'A');
			number /= base;
		}
	}
	if(si == 0)
		bp_0e[si++] = '0';
	if(isNegative)
		bp_0e[si++] = '-';
	for(bp_06 = 0; bp_06 < si; bp_06 ++)
		str[bp_06] = bp_0e[si - bp_06 - 1];
	str[bp_06] = 0;
	return si;
}

/*C_0C9C_33D3*/CON_putch(int ch) {
	int si;
	char bp_08[8];

	if(!TextOn)
		return;
	if(ch != '&' && D_04A3 && !D_2CA8)
		return;
	switch(ch) {
		case '#': case '$': case '%':
		case '/': case '@': case '\\':
		case '^': D_057D = ch; return;
		case '*':
			CON_putch0(-1);
			PromptCh = 1;
			CON_getch();
			PromptCh = 5;
		return;
		case '&': D_04A3 = !D_04A3; return;
		case '<': D_04DF->_attr |= 8; return;
		case '>': D_04DF->_attr &= ~8; return;
		case '`': D_0498 = 1; return;
	}
	switch(D_057D) {
		case '#':
			ch = toupper(ch);
			if(ch >= '0' && ch <= '9')
				si = ch - '0';
			else if(ch >= 'A' && ch <= 'Z')
				si = (ch - 'A') + 10;
			else
				si = 0;
			D_049D = (VarInt[si] != 1);
			CON_atoi(VarInt[si], bp_08, 6, -10);
			C_0C9C_32F8(bp_08);
			D_057D = 0;
		break;
		case '$':
			ch = toupper(ch);
			if(ch >= '0' && ch <= '9')
				si = ch - '0';
			else if(ch >= 'A' && ch <= 'Z')
				si = (ch - 'A') + 10;
			else
				si = 0;
			C_0C9C_32F8(VarStr[si]);
			D_057D = 0;
		break;
		case '/':
			if(!isalpha(ch) || !D_049D) {
				CON_putch0(ch);
				if(!isalpha(ch))
					D_057D = 0;
			}
		break;
		case '\\':
			if(!isalpha(ch) || D_049D) {
				CON_putch0(ch);
				if(!isalpha(ch))
					D_057D = 0;
			}
		break;
		case '^':
			if(D_04DF->_attr & 8)
				CON_putch0((ch & 0x1f) | 0x80);
			else
				CON_putch0(ch & 0x1f);
			D_057D = 0;
		break;
		case '@':
			if(strchr(" ,.:;!?'-\"\n", ch)) {
				D_057D = 0;
				D_04DF->_fg = D_2A56;
			} else if(D_04A1) {
				D_04DF->_fg = D_2A4A;
			}
		default:
			CON_putch0(ch);
	}
}

/*C_0C9C_3621*/CON_puts(unsigned char far *bp06) {
	while(*bp06)
		CON_putch(*bp06++);
}

cdecl /*C_0C9C_3643*/CON_printf(unsigned char far *format, ...) {
	void **si;
	unsigned di;
	char bp_16[8];
	int bp_0e, bp_0c;
	int bp_0a, bp_08, bp_06, bp_04;
	char *bp_02;

	if(!TextOn)
		return;
	si = ...;
	while(*format) {
		bp_0e = *format++;
		bp_0a = bp_08 = bp_04 = 0;
		bp_06 = ' ';
		bp_02 = bp_16;
		switch(bp_0e) {
		case '%':
			bp_0c = *format++;
			while(bp_0c == '0' || bp_0c == '-') {
				switch(bp_0c) {
					case '0': bp_06 = '0'; break;
					case '-': bp_04 = 1; break;
				}
				bp_0c = *format++;
			}
			while(bp_0c >= '0' && bp_0c <= '9') {
				bp_0a *= 10;
				bp_0a += bp_0c - '0';
				bp_0c = *format++;
			}
			switch(tolower(bp_0c)) {
				case 'd':
					di = *(int *)si++;
					bp_08 = CON_atoi(di, bp_16, 6, -10);
					D_049D = (di != 1);
				break;
				case 'o':
					di = *(int *)si++;
					bp_08 = CON_atoi(di, bp_16, 6, 8);
					D_049D = (di != 1);
				break;
				case 'x':
					di = *(int *)si++;
					bp_08 = CON_atoi(di, bp_16, 4, 16);
					D_049D = (di != 1);
				break;
				case 'u':
					di = *(int *)si++;
					bp_08 = CON_atoi(di, bp_16, 5, 10);
					D_049D = (di != 1);
				break;
				case 'c':
					bp_16[0] = *(char *)si++;
					bp_16[1] = 0;
				break;
				case 's':
					bp_02 = (char *)*si++;
					bp_08 = strlen(bp_02);
				break;
				default:
					bp_16[0] = 0;
					CON_putch0(bp_0c);
			}
			if(bp_04)
				CON_puts(bp_02);
			C_0C9C_32DE(bp_0a - bp_08, bp_06);
			if(bp_04 == 0)
				CON_puts(bp_02);
		break;
		default:
			CON_putch(bp_0e);
		}
	}
}
