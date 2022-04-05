/*
 * Ultima VI - The False Prophet
 * Copyright (c) 1990 Origin Systems, Inc. and Richard Gariott
 * reverse-coded by Ergonomy Joe in 2013
 */
#ifndef __MOUSE_H__
#define __MOUSE_H__

/*a graphic structure?*/
struct tSurface {
	unsigned segment;/*_00*/
	unsigned far *pScanlines;/*_02*/
	unsigned right,left,top,bottom;/*f_06,f_08,f_0a,f_0c*/
};

struct tMouse {
	unsigned char f_00,f_01;/*button 1 & 2?*/
	int f_02,f_04;/*x, y?*/
	unsigned f_06;
	void far *f_08;/*graphic data?*/
	unsigned f_0c;/*pointer index?*/
	struct tSurface *f_0e;
	void far *f_10;/*work memory?0x400 bytes*/
	void (far *f_14)();/*a callback*/
	unsigned char f_18;/*"is installed" flag?*/
};

extern cdecl MOUSE_0000(struct tMouse *);
/*MOUSE_0056*/extern cdecl MOUSE_POINTER(void far */*graphic data?*/, unsigned);
/*MOUSE_009F*/extern cdecl MOUSE_SERVE();
/*MOUSE_0135*/extern cdecl MOUSE_SHOW();
/*MOUSE_0146*/extern cdecl MOUSE_HIDE();
extern cdecl MOUSE_015C();
extern cdecl MOUSE_0183();
/*MOUSE_01F0*/extern int cdecl MOUSE_IN_AREA(struct tSurface *);

#define ShowMouse if(MouseOn) MOUSE_SHOW();
#define HideMouse if(MouseOn) MOUSE_HIDE();
#define ServeMouse if(MouseOn) MOUSE_SERVE();

#endif
