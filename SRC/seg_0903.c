/*
 * Ultima VI - The False Prophet
 * Copyright (c) 1990 Origin Systems, Inc. and Richard Gariott
 * reverse-coded by Ergonomy Joe in 2013
 */
#include "u6.h"

/*====---- ----====*/
/*0194*/int IsZFile = 0;
char *D_0196[] = {
	/*01EB*/""
};
char *D_0198[] = {
	/*01EC*/"paper.bmp",/*main screen*/
	/*01F6*/"newmagic.bmp",/*small scroll*/
	/*0203*/"u6face0.bmp",
	/*020F*/"stats.bmp",
	/*0219*/"worldmap.bmp"
};
/*01A2*/char DirIncrX[] = { 0, 1, 1, 1, 0,-1,-1,-1};
/*01AA*/char DirIncrY[] = {-1,-1, 0, 1, 1, 1, 0,-1};
/*01B2*/char *DirNames[] = {
	/*0226*/"North",
	/*022C*/"Northeast",
	/*0236*/"East",
	/*023B*/"Southeast",
	/*0245*/"South",
	/*024B*/"Southwest",
	/*0255*/"West",
	/*025A*/"Northwest"
};
int __01C2 = 0;
int __01C4 = 0;
int D_01C6 = 1;
int D_01C8 = 0;/*gr type*/
int D_01CA = 0;/*music adapter?*/
int D_01CC = 0x388;/*sound card i/o port*/
int D_01CE = 0;/*gr flag*/
/*01D0*/unsigned char MouseOn = 1;
char D_01D1 = 0;
unsigned char D_01D2 = 0;
char D_01D3[] = "vetch";/*[vga/ega/tandy/cga/hercules?]*/
char D_01D9[] = "naxyrzi";
/*====---- ----====*/

#define PTR2NORM(p) (((unsigned long)FP_SEG(p) << 4) + FP_OFF(p))
#define NORM2PTR(n) (MK_FP((n) >> 4, (n) & 0xf))

/*C_0903_0001*/static __MemAlloc(char huge **pptr, unsigned long size) {
	unsigned long i, bp_04;

	if(size > Mem_Remain) {
		/*not enough memory*/
		OSI_delay_clean();
		GR_03();
		exit(1);
		return;
	}

	*pptr = Mem_BP;
	/*update remaining memory and base pointer*/
	bp_04 = PTR2NORM(Mem_BP);
	bp_04 += size;
	Mem_BP = NORM2PTR(bp_04);
	Mem_Remain -= size;
	/*zero memory*/
	for(i = 0; i < size; i ++)
		*(*pptr + i) = 0; 
}

/*C_0903_00DE*/void far *LoadFile(char *fname, void far *pBuffer) {
	int fh;
	unsigned aSegment;
	void far *pRetBuff;
	long bp_08;
	long fsize;

	if(pBuffer == 0) {
		/*para align*/
		bp_08 = PTR2NORM(Mem_BP);
		if(bp_08 & 0xf) {
			Mem_Remain -= 0x10 - (bp_08 & 0xf);
			bp_08 = (bp_08 & ~0xf) + 0x10;
			Mem_BP = MK_FP(bp_08 >> 4, 0);
		}

		fh = OSI_open(fname);
		pRetBuff = 0;
		if(fh >= 0) {
			if(IsZFile) {
				OSI_read(fh, 0, 4, &fsize);
				if(fsize + 0x3410 <= Mem_Remain) {
					pRetBuff = Mem_BP;
					decompress(fh, 0L, Mem_BP, FP_SEG(Mem_BP) + ((fsize + 0xf) >> 4));
				}
			} else {
				fsize = OSI_fileSize(fh);
				if(fsize <= Mem_Remain) {
					pRetBuff = Mem_BP;
					OSI_read(fh, 0, fsize, Mem_BP);
				}
			}
			bp_08 += fsize;
			Mem_BP = NORM2PTR(bp_08);
			Mem_Remain -= fsize;
			OSI_close(fh);
		}

		return pRetBuff;
	}

	if(IsZFile) {
		pRetBuff = pBuffer;
		fh = OSI_open(fname);
		if(fh >= 0) {
			OSI_read(fh, 0, 4, &fsize);
			if(pBuffer == ScratchBuf) {
				aSegment = FP_SEG(pBuffer) + ((FP_OFF(pBuffer) + fsize + 0xf) >> 4);
			} else {
				aSegment = FP_SEG(ScratchBuf);
				aSegment += ((FP_OFF(ScratchBuf) + 0xf) >> 4);
			}
			decompress(fh, 0L, pBuffer, aSegment);
			OSI_close(fh);
		}
	} else {
		pRetBuff = pBuffer;
		fh = OSI_open(fname);
		OSI_read(fh, 0, OSI_fileSize(fh), pBuffer);
		OSI_close(fh);
	}

	return pRetBuff;
}

/*for graphic driver*/
/*C_0903_033A*/LoadFileGr(char far *fn, void far *pBuffer, int isZ) {
	int fh, di;
	void far *pRetBuff;
	long fsize;
	unsigned aSegment;
	char fname[14];

	for(di = 0; di < 13; di++)
		fname[di] = fn[di];
	
	if(isZ) {
		pRetBuff = pBuffer;
		fh = OSI_open(fname);
		if(fh >= 0) {
			OSI_read(fh, 0, 4, &fsize);
			if(pBuffer == ScratchBuf) {
				aSegment = FP_SEG(pBuffer) + ((FP_OFF(pBuffer) + fsize + 0xf) >> 4);
			} else {
				aSegment = FP_SEG(ScratchBuf);
				aSegment += ((FP_OFF(ScratchBuf) + 0xf) >> 4);
			}
			decompress(fh, 0L, pBuffer, aSegment);
			OSI_close(fh);
		}
	} else {
		pRetBuff = pBuffer;
		fh = OSI_open(fname);
		OSI_read(fh, 0, OSI_fileSize(fh), pBuffer);
		OSI_close(fh);
	}
}

/*some OSI_callback handler?*/
void C_0903_0430() {
	int si;
	struct tClipInfo *di;

	if(D_0332 == -1 && D_0334 == 5/*OSI_read*/)
		if(D_01C6)
			return;
	if(D_0332 == 2 && D_0334 == 4/*OSI_delete*/)
		return;
	if(D_01D1) {
		di = D_04DF;
		CON_setClip(&(D_B6B5[3]));
		if(D_0332 == -1 && D_0334 == 6/*OSI_write*/) {
			DISK_full();
		} else {
			CON_printf(/*0264*/"DISK ERROR!!!! Function: %d, error: %d, file: ", D_0334, D_0332);
			CON_printf(D_0336);
			CON_printf("\n");
			while(!(si = C_31FA_000C(1)));
		}
		CON_setClip(di);
	}
	if(si != 3 && si != 27)
		D_0332 = 0;
}

static C_0903_04DB() {
	int si, di;
	unsigned aSegment;
	long bp_04;

	bp_04 = PTR2NORM(Mem_BP);
	if(bp_04 & 0xf) {
		Mem_Remain -= 0x10 - (bp_04 & 0xf);
		bp_04 = (bp_04 & ~0xf) + 0x10;
		Mem_BP = NORM2PTR(bp_04);
	}
	aSegment = bp_04 >> 4;
	DISK_confirm(DISK_4);
	GR_2A(aSegment, (unsigned)(Mem_Remain >> 4));
	D_4D48 = D_ECC4->_20;
	if(D_4D48 == 0) {
		OSI_delay_clean();
		GR_03();
		exit(1);
	}
	D_8E4A = FP_SEG(D_4D48) + (((long)D_4D48 & 0xffff) >> 4);/*unused?*/

	DISK_confirm(DISK_4);
	si = OSI_open(/*0295*/"animdata");
	OSI_read(si, 0, 2, &NumAnimData);
	OSI_read(si, -1, MAX_ANIM * sizeof(int), D_67E4);
	OSI_read(si, -1, MAX_ANIM * sizeof(int), D_9DF5);
	OSI_read(si, -1, MAX_ANIM, D_9495);/*anim tile mask*/
	OSI_read(si, -1, MAX_ANIM, D_B3F7);/*anim tile shift*/
	OSI_close(si);

	DISK_confirm(DISK_4);
	di = OSI_open(/*029E*/"tileflag");
	OSI_read(di, 0, 0x800, TerrainType);
	OSI_read(di, -1, 0x800, TileFlag);
	OSI_read(di, -1, 0x400, TypeWeight);
	OSI_read(di, -1, 0x800, D_B3EF);
	OSI_close(di);
}

/*display "paper.bmp"*/
C_0903_070B() {
	int si;
	struct tPixmap far *bp_04;

	DISK_confirm(DISK_4);
	bp_04 = Mem_BP;
	si = OSI_open(D_0198[0]);
	if(si < 0)
		return;
	decompress(si, 0L, bp_04, FP_SEG(bp_04) + (FP_OFF(bp_04) >> 4) + (0xfa20 >> 4));
	OSI_close(si);
	GR_18(bp_04, 0, 0);
}

/*palette related*/
C_0903_0776() {
	union REGS bp_18;
	struct SREGS bp_08;

	if(D_01C8 != 0)
		return;
	bp_18.x.ax = 0x1012;
	bp_18.x.bx = 0;
	bp_18.x.cx = 0x100;
	bp_18.x.dx = FP_OFF(D_8C2C);
	segread(&bp_08);
	bp_08.es = FP_SEG(D_8C2C);
	int86x(0x10, &bp_18, &bp_18, &bp_08);
}

C_0903_07C4() {
	int si;

	if(D_01C8 == 0) {
		__MemAlloc(&D_8C2C, 0x100 * 3);
		DISK_confirm(DISK_4);
		si = OSI_open(/*02A7*/"u6pal");
		OSI_read(si, 0, 0x100 * 3, D_8C2C);
		OSI_close(si);
		C_0903_0776();
	} else {
		GR_24();
	}
}

/*fire/water*/
/*C_0903_0820*/PaletteAnimation() {
	int si;
	union REGS bp_12;
	int bp_02;

	if(SpellFx[14] != 0)
		return;
	if(D_01C8 == 0) {
		bp_02 = (D_01D2 ++) & 0xf;
		if(bp_02 > 7)
			bp_02 = 0xf - bp_02;
		bp_02 += 0xc;
		bp_12.x.ax = 0x1010;
		bp_12.x.bx = 0xff;
		bp_12.h.dh = 
		bp_12.h.ch = 
		bp_12.h.cl = bp_02;
		int86(0x10, &bp_12, &bp_12);
		for(bp_02 = 0; bp_02 < 8; bp_02 ++) {
			outportb(0x3c8, ((D_01D2 - bp_02) & 7) + 0xe0);
			for(si = 0; si < 3; si ++)
				outportb(0x3c9, D_8C2C[0xe7 - bp_02].content[si]);

			outportb(0x3c8, ((D_01D2 - bp_02) & 7) + 0xe8);
			for(si = 0; si < 3; si ++)
				outportb(0x3c9, D_8C2C[0xef - bp_02].content[si]);
		}
		for(bp_02 = 0; bp_02 < 4; bp_02 ++) {
			outportb(0x3c8, (((D_01D2 >> 1) - bp_02) & 3) + 0xf0);
			for(si = 0; si < 3; si ++)
				outportb(0x3c9, D_8C2C[0xf3 - bp_02].content[si]);

			outportb(0x3c8, (((D_01D2 >> 1) - bp_02) & 3) + 0xf4);
			for(si = 0; si < 3; si ++)
				outportb(0x3c9, D_8C2C[0xf7 - bp_02].content[si]);

			outportb(0x3c8, (((D_01D2 >> 1) - bp_02) & 3) + 0xf8);
			for(si = 0; si < 3; si ++)
				outportb(0x3c9, D_8C2C[0xfb - bp_02].content[si]);
		}
	} else {
		GR_27((unsigned char)(D_01D2 ++));
	}
}

/*create param string*/
C_0903_09BB(char *si/*bp06*/) {
	*(si++) = D_01D3[D_01C8];
	*(si++) = MouseOn?'M':'K';
	*(si++) = D_01D9[D_01CA];
	if(D_01CA == 1 || D_01CA == 2) {
		*(si++) = (D_01CC >> 8) + '0';
		*(si++) = ((D_01CC >> 4) & 0xf) + '0';
		*(si++) = (D_01CC & 0xf) + '0';
	}
	*si = 0;
}

/*go to ending?*/
C_0903_0A1E() {
	char param_str[8],y_str[6],m_str[6],d_str[6];
	int years,monthes,days;

	years = Date_Y - 161;
	monthes = Date_M - 7;
	days = Date_D - 4;
	if(Date_D < 0/*should be "days"?bug?*/) {
		Date_D += 28;
		monthes --;
	}
	if(monthes < 0) {
		monthes += 13;
		years --;
	}
	CON_atoi(years, y_str, 5, 10);
	CON_atoi(monthes, m_str, 5, 10);
	CON_atoi(days, d_str, 5, 10);
	DISK_confirm(DISK_3);
	MUS_091A(0);
	OSI_delay_clean();
	GR_03();
	C_0903_09BB(param_str);
	do {
		execl(/*02AD*/"end.exe", /*02AD*/"end.exe", param_str, y_str, m_str, d_str, 0);
	} while(1);
}

/*parse music i/o port?*/
char *C_0903_0AE6(unsigned char *di/*bp06*/) {
	int si;

	for(D_01CC = 0; ((si = toupper(*di)) >= '0' && si <= '9') || (si >= 'A' && si <='F'); di ++) {
		D_01CC <<= 4;
		if(si >= '0' && si <= '9')
			D_01CC += si - '0';
		else
			D_01CC += si - 'A' + 10;
	}
	return di;
}

/*C_0903_0B46*/cfg_parse(unsigned char *si/*bp06*/) {
	while(*si) {
		switch(toupper(*si++)) {
			case '$': D_01CE = 1; break;
			case 'V': D_01C8 = 0; break;
			case 'E': D_01C8 = 1; break;
			case 'T': D_01C8 = 2; break;
			case 'C': D_01C8 = 3; break;
			case 'H': D_01C8 = 4; break;
			case 'M': MouseOn = 1; break;
			case 'K': MouseOn = 0; break;
			case 'N': D_01CA = 0; break;
			case 'A': D_01CA = 1; D_01CC = 0x388;
				if(*si == '3' || *si == '2')
					si = C_0903_0AE6(si);
			break;
			case 'X': D_01CA = 2; D_01CC = 0x220;
				if(*si == '2')
					si = C_0903_0AE6(si);
			break;
			case 'Y': D_01CA = 3; break;
			case 'R': D_01CA = 4; break;
			case 'Z': D_01CA = 5; D_01CC = 0x220;
				if(*si == '2')
					si = C_0903_0AE6(si);
			break;
			case 'I': D_01CA = 6; D_01CC = 0x280;
				if(*si == '2')
					si = C_0903_0AE6(si);
			break;
		}
	}
}

char *D_01E1[] = {
	/*02B5*/"u6mcga.ptr",
	/*02C0*/"u6curs.ega",
	/*02CB*/"u6curs.tga",
	/*02D6*/"u6curs.cga",
	/*02D6*/"u6curs.cga"
};

/*C_0903_0C5E*/cdecl main(int argc, char **argv) {
	int si, di;
	char bp_0a[7];
	int bp_02;

	OSI_randomize(C_33CA_000E());
	/*config*/
	di = OSI_open(/*02E1*/"config.u6");
	if(di >= 0) {
		OSI_read(di, 0, 6, bp_0a);
		bp_0a[6] = 0;
		cfg_parse(bp_0a);
		OSI_close(di);
	}
	for(si = 1; si < argc; si ++)
		cfg_parse(argv[si]);
	/*memory*/
	D_B41B = farcoreleft();
	D_B3F3 = farmalloc(D_B41B);
	if(!D_B3F3)
		goto c_12ef;
	Mem_BP = D_B3F3;
	Mem_Remain = D_B41B;

	DISK_init();
	C_3368_0002();/*init LOW?*/
	OSI_delay_init();
	D_032E.f = C_0903_0430;
	if(MouseOn) {
		Screen.pScanlines = D_4D54;
		Screen.left = 0;
		Screen.right = 319;
		Screen.top = 0;
		Screen.bottom = 199;

		D_9E3D.pScanlines = D_9E4B;
		D_9E3D.left = 0;
		D_9E3D.right = 159;
		D_9E3D.top = 0;
		D_9E3D.bottom = 159;

		if(D_01C8 == 0) {/*VGA*/
			Screen.segment = 0xa000;
			D_4D54[0] = 0;
			for(si = 1; si < 200; si++)
				D_4D54[si] = D_4D54[si - 1] + 0x140;
		} else
		if(D_01C8 == 1) {/*EGA*/
			Screen.segment = 0xa000;
			D_4D54[0] = 0;
			for(si = 1; si < 200; si++)
				D_4D54[si] = D_4D54[si - 1] + 40;
			Screen.pScanlines = D_4D54;
			D_9E4B[0] = 0x141;
			for(si = 1; si < 176; si++)
				D_9E4B[si] = D_9E4B[si - 1] + 40;
			D_9E3D.segment = 0xa200;
		} else
		if(D_01C8 == 2) {/*TANDY*/
			Screen.segment = 0xb800;
			D_4D54[0] = 0;
			D_4D54[1] = 0x2000;
			D_4D54[2] = 0x4000;
			D_4D54[3] = 0x6000;
			for(si = 4; si < 200; si++)
				D_4D54[si] = D_4D54[si - 4] + 0xa0;
			Screen.pScanlines = D_4D54;
		} else
		if(D_01C8 == 3) {/*CGA*/
			Screen.segment = 0xb800;
			D_4D54[0] = 0;
			D_4D54[1] = 0x2000;
			for(si = 2; si < 200; si++)
				D_4D54[si] = D_4D54[si - 2] + 80;
			Screen.pScanlines = D_4D54;
		} else
		if(D_01C8 == 4) {/*HERCULES*/
			Screen.segment = 0xb000;
			for(si = 0; si < 200; si++) {
				bp_02 = (si * 3 >> 1) + 0xf;
				D_4D54[si] = ((bp_02 & 3) << 13) + (bp_02 >> 2) * 90 + 5;
			}
			Screen.pScanlines = D_4D54;
		}
		__MemAlloc(&Mouse.f_10, 0x400);
		__MemAlloc(&D_B623, 0x800);
		DISK_confirm(DISK_4);
		di = OSI_open(D_01E1[D_01C8]);
		if(di >= 0) {
			decompress(di, 0L, D_B623, FP_SEG(Mem_BP) + 1);
			OSI_close(di);
		}
		Mouse.f_08 = D_B623;
		Mouse.f_0e = &Screen;
		MOUSE_0000(&Mouse);
		Mouse.f_14 = C_0C9C_01FB;
		MOUSE_POINTER(D_B623, 0);
	}
	C_2FC1_000A(D_01C8);/*init gr*/
	MUS_0A3A(D_01CA, D_01CC);/*init music*/
	C_0903_07C4();
	DISK_confirm(DISK_4);
	D_ECC4->_06 = LoadFile(/*02EB*/"U6.CH", 0L);
	D_01D1 = 1;
	CON_createClip(&(D_B6B5[0]), 0, 0, 39, 24);
	C_0903_070B();
	CON_createClip(&(D_B6B5[1]), 22,  1, 38, 12);
	CON_createClip(&(D_B6B5[3]), 22, 14, 38, 23);
	CON_setClip(&(D_B6B5[3]));
	D_04DF->_attr |= 1;
	D_04DF->_attr |= 0x20;
	D_04DF->_attr |= 4;

	__MemAlloc(&TerrainType, 0x800);
	__MemAlloc(&TileFlag, 0x800);
	__MemAlloc(&BaseTile, 0x800);
	__MemAlloc(&TypeWeight, 0x400);
	__MemAlloc(&D_B3EF, 0x800);

	__MemAlloc(&ScratchBuf, 0x7900);
	D_ECC4->_1c = ScratchBuf;
	if(D_01C8 == 0) {/*VGA*/
		D_9E3D.segment = FP_SEG(ScratchBuf);
		D_9E4B[0] = FP_OFF(ScratchBuf) + 0x588;
		for(si = 1; si < 0xa0; si++)
			D_9E4B[si] = D_9E4B[si - 1] + 0xb0;
	} else
	if(D_01C8 == 2) {/*TANDY*/
		D_9E3D.segment = FP_SEG(ScratchBuf);
		D_9E4B[0] = FP_OFF(ScratchBuf) + 0x404;
		for(si = 1; si < 0xa0; si++)
			D_9E4B[si] = D_9E4B[si - 1] + 0x80;
	} else
	if(D_01C8 == 3 || D_01C8 == 4) {/*CGA/HERCULES*/
		D_9E3D.segment = FP_SEG(ScratchBuf);
		D_9E4B[0] = FP_OFF(ScratchBuf) + 0x282;
		for(si = 1; si < 0xa0; si++)
			D_9E4B[si] = D_9E4B[si - 1] + 0x50;
	}

	__MemAlloc(&Schedule, 600 * sizeof(struct tSchedule));
	__MemAlloc(&NPCMode, 0x100);
	__MemAlloc(&NPCComMode, 0x100);
	__MemAlloc(&SchedIndex, 0x100);
	__MemAlloc(&Leader, 0x100);
	__MemAlloc(&NPCFlag, 0x100);
	__MemAlloc(&OrigShapeType, 0x100 * sizeof(int));
	__MemAlloc(&Level, 0x100);
	__MemAlloc(&D_8C42, 0x100);
	__MemAlloc(&D_B7A4, 0x600);

	__MemAlloc(&TalkBuf, 0x2800);

	C_1184_3B1D();

	C_0903_04DB();

	/*draw icons*/
	for(si = 0; si < 9; si ++)
		GR_2D(TIL_190 + si, TIL2SCR(si) + 8, 176);
	GR_2D(TIL_19E, 152, 176);
	/* */
	D_2FFC = 1;
	C_101C_052D();
	C_0C9C_042A();/*load game?*/
	for(si = 0; si < NumAnimData; si ++)
		GR_4B(D_67E4[si], D_9DF5[si]);

	if(D_2CCB == 0) {
		CON_printf(/*02F1*/"You must first create or transfer a character. ");
		D_049C = 0;
		CON_getch();
		DISK_confirm(DISK_1);
		OSI_delay_clean();
		GR_03();
		C_0903_09BB(bp_0a);
		while(1)
			execl(/*0321*/"ultima6.exe", /*0321*/"ultima6.exe", bp_0a, 0);
	}

	C_1E0F_512A();

	C_0A33_1355(0);

	ShowMouse
	if(InCombat)
		COMBAT_begin();
	else
		MUS_09A8();/*play a random song?*/

	/*-- game loop --*/
	C_0A33_1CB4();
	/*-- --*/

	MUS_091A(0);
	C_1184_26E2();
	OSI_delay_clean();

c_12ef:
	GR_03();
	exit(0);
}
