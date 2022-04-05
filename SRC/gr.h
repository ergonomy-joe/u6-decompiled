/*
 * Ultima VI - The False Prophet
 * Copyright (c) 1990 Origin Systems, Inc. and Richard Gariott
 * reverse-coded by Ergonomy Joe in 2013
 */
#ifndef __GR_H__
#define __GR_H__

struct tGr {
	/*common part*/
	unsigned char _00[6];
	void far *_06;/*character data*/
	unsigned char _0a,_0b;/*text bg/fg color*/
	/* */
	unsigned char _0c;
	unsigned char _0d;
	unsigned char _0e;/*tile anim:on/off*/
	unsigned char _0f;
	void (*_10)();/*request floppy callback*/
	unsigned char _14[8];
	void far *_1c;
	void far *_20;
	void (*_24)();/*load file callback*/
	void (*_28)();/*open callback*/
	void (*_2c)();/*read callback*/
	void (*_30)();/*close callback*/
};

struct tPixmap/*size:6*/ {
	int width,height;/*_00,_02*/
	unsigned char data[0x1000];/*_04*/
};

#define GR_00(a)               (D_ECB8.iii.ofs = 0x00, (struct tGr far *)(*D_ECB8.f)(a))
#define GR_03()                (D_ECB8.iii.ofs = 0x03, (*D_ECB8.f)())
/*color(c)*/
#define GR_06(c)               (D_ECB8.iii.ofs = 0x06, (*D_ECB8.f)(c))
/*point(x,y)*/
#define GR_09(x,y)             (D_ECB8.iii.ofs = 0x09, (*D_ECB8.f)(x,y))
/*fillRect(x0,y0,x1,y1)*/
#define GR_12(x0,y0,x1,y1)     (D_ECB8.iii.ofs = 0x12, (*D_ECB8.f)(x0,y0,x1,y1))
/*moveRectY(x0,y0,x1,y1,dy)*/
#define GR_15(x0,y0,x1,y1,dy)  (D_ECB8.iii.ofs = 0x15, (*D_ECB8.f)(x0,y0,x1,y1,dy))
/*drawPixmap(p,x,y)*/
#define GR_18(p,x,y)           (D_ECB8.iii.ofs = 0x18, (*D_ECB8.f)(p,x,y))
/*putchar(ch,x,y)*/
#define GR_1E(ch,x,y)          (D_ECB8.iii.ofs = 0x1e, (*D_ECB8.f)(ch,x,y))
#define GR_24()                (D_ECB8.iii.ofs = 0x24, (*D_ECB8.f)())
/*palette animation related?not VGA?*/
#define GR_27(a)               (D_ECB8.iii.ofs = 0x27, (*D_ECB8.f)(a))
/*init resources(size in para)*/
#define GR_2A(seg,size)        (D_ECB8.iii.ofs = 0x2a, (*D_ECB8.f)(seg,size))
/*putTile(tile,x,y) onscreen?*/
#define GR_2D(tile,x,y)        (D_ECB8.iii.ofs = 0x2d, (*D_ECB8.f)(tile,x,y))
/*define sprite?*/
#define GR_33(i,a,b,c,d)       (D_ECB8.iii.ofs = 0x33, (*D_ECB8.f)(i,a,b,c,d))
#define GR_36(i,x,y)           (D_ECB8.iii.ofs = 0x36, (*D_ECB8.f)(i,x,y))
#define GR_39(i,x,y)           (D_ECB8.iii.ofs = 0x39, (*D_ECB8.f)(i,x,y))
#define GR_3F()                (D_ECB8.iii.ofs = 0x3f, (*D_ECB8.f)())
/*putTile(tile,x,y) offscreen?*/
#define GR_42(tile,x,y)        (D_ECB8.iii.ofs = 0x42, (*D_ECB8.f)(tile,x,y))
/*backbuffer->screen?*/
#define GR_45(x0,y0,x1,y1,a,b) (D_ECB8.iii.ofs = 0x45, (*D_ECB8.f)(x0,y0,x1,y1,a,b))
/*backbuffer->screen+fadein?*/
#define GR_48(x0,y0,x1,y1)     (D_ECB8.iii.ofs = 0x48, (*D_ECB8.f)(x0,y0,x1,y1))

#define GR_4B(a,b)             (D_ECB8.iii.ofs = 0x4b, (*D_ECB8.f)(a,b))
#define GR_4E()                (D_ECB8.iii.ofs = 0x4e, (*D_ECB8.f)())

#endif
