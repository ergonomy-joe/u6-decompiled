/*
 * Ultima VI - The False Prophet
 * Copyright (c) 1990 Origin Systems, Inc. and Richard Gariott
 * reverse-coded by Ergonomy Joe in 2013
 */
/*
	Drive/floppy management module
*/
#include "u6.h"

char *D_2E88[] = {
	/*2EC8*/"",
	/*2EC9*/"Program",
	/*2ED1*/"Introduction",
	/*2EDE*/"Endgame",
	/*2EE6*/"Game",
	/*2EEB*/"Map",
	/*2EEF*/"Populace A",
	/*2EFA*/"Populace B",
	/*2F05*/"Player"
};
int D_2E9A = 0;/*last selected floppy?*/
int D_2E9C = 1;/*last selected drive?*/
int D_2E9E = ' ';
unsigned char D_2EA0[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned char D_2EA9[26] = {0};
char D_2EC3[] = "u6.1";


static DISK_0005() {
	return 0;
}

/*check floppy?*/
static DISK_0008(int bp06) {
	int si;

	D_032E.f = OSI_callback;
	D_2EC3[3] = bp06 + '0';
	si = OSI_open(D_2EC3);
	if(si != -1)
		OSI_close(si);
	D_032E.f = C_0903_0430;
	return (si != -1);
}

static DISK_0053(int bp06) {
	int si;
	int bp_02;

	D_2E9C =
	bp_02 = D_2EA0[bp06];
	/*selectDrive*/C_3360_0023(bp_02 + 'A' - 1);
	for(si = DISK_1; si <= DISK_8; si++)
		if(DISK_0008(si))
			D_2EA0[si] = bp_02;
}

/*DISK_0097*/DISK_confirm(int di/*bp06*/) {
	int si;
	int bp_04;
	struct tClipInfo *bp_02;

	if(D_2E9A == di)
		return;
	while(D_2E9A != di && D_2EA0[di] == 0) {
		bp_02 = D_04DF;
		CON_setClip(&(D_B6B5[3]));
		CON_printf(InsertDiskMsg, D_2E88[di]);
		CON_putch0(-1);
		D_049C = 0;
		si = 0;
		while(!si) {
			if(MouseOn) {
				MOUSE_SERVE();
				if(Mouse.f_00 || Mouse.f_01) {
					if(!MousePress) {
						MousePress =
						MouseTrigger = 1;
						si = ' ';
					} else {
						MouseTrigger = 0;
					}
				} else {
					MousePress =
					MouseTrigger = 0;
				}
			}
			if(!si)
				si = CON_prompt();
		}
		CON_putch0('\n');
		CON_setClip(bp_02);
		D_049C = 1;
		if(si == 3) {
			D_2E9A = di;
			D_2EA0[di] = D_2E9C - ('A' - 1);
			/*selectDrive*/C_3360_0023(D_2E9C);
			return;
		}
		bp_04 = 0;
		for(si = 0; si < 26; si++) {
			if(D_2EA9[si] != 2)
				continue;
			/*selectDrive*/C_3360_0023(si + 'A');
			if(DISK_0008(di)) {
				bp_04 = si + 1;
				D_2EA0[di] = bp_04;
				for(si = DISK_1; si <= DISK_8; si++) {
					if(si != di && D_2EA0[si] == bp_04)
						D_2EA0[si] = 0;
				}
				DISK_0053(di);
				break;
			}
		}
	}
	if(D_2EA0[di] != D_2E9C) {
		D_2E9C = D_2EA0[di];
		/*selectDrive*/C_3360_0023(D_2E9C + 'A' - 1);
	}
	D_2E9A = di;
}

/*uncalled?*/
DISK_0202() {
	D_2EA0[D_2E9A] = 0;
	DISK_confirm(D_2E9A);
}

/*DISK_0211*/DISK_full() {
	int si;

	D_0340 = 0;
	CON_printf("Disk full.  Do you want to Save the game and continue, ");
	CON_printf("or Restore your last saved game? ");
	do {
		si = CON_getch();
	} while(si != 'S' && si != 'R');
	D_2E9E = si;
	if(si == 'S')
		CON_printf("S\n\nSaving...\n");
	else
		CON_printf("R\n\nRestoring...\n");
}

/*DISK_0266*/DISK_init() {
	int si;
	union REGS bp_10;

	D_3004 = DISK_0005;
	/*get dos version*/
	bp_10.h.ah = 0x30;
	intdos(&bp_10, &bp_10);
	D_ECC8 = bp_10.h.al;
	D_ECC9 = bp_10.h.ah;

	if(D_ECC8 >= 3) {
		for(si = 0; si < 26; si++) {
			bp_10.h.ah = 0x44;
			bp_10.h.al = 8;
			bp_10.h.bl = si + 1;
			intdos(&bp_10, &bp_10);
			if(bp_10.x.cflag != 0)
				continue;
			if(bp_10.x.ax)
				D_2EA9[si] = 1;
			else
				D_2EA9[si] = 2;
			/*dos 3.20 and above*/
			if(D_ECC8 >= 4 || D_ECC9 >= 20) {
				bp_10.h.ah = 0x44;
				bp_10.h.al = 0xe;
				bp_10.h.bl = si + 1;
				intdos(&bp_10, &bp_10);
				if(bp_10.h.al && bp_10.h.al != si + 1)
					D_2EA9[si] = 0;
			}
		}
	}
	/*below dos 3.20*/
	if(D_ECC8 < 3 || (D_ECC8 == 3 && D_ECC9 < 20)) {
		int86(0x11, &bp_10, &bp_10);
		if(bp_10.x.ax & 1) {
			D_2EA9[0] = 2;
			if(bp_10.x.ax & 0xc0)
				D_2EA9[1] = 2;
		} else {
			D_2EA9[0] =
			D_2EA9[1] = 0;
		}
		si = /*getDefaultDrive*/C_3366_000E();
		if(si > 'B')
			D_2EA9[si - 'A'] = 1;
	}

	D_2E9A = DISK_4;
	D_2EA0[DISK_4] = /*getDefaultDrive*/C_3366_000E() + 1 - 'A';
	DISK_0053(4);
}
