/*
 * Ultima VI - The False Prophet
 * Copyright (c) 1990 Origin Systems, Inc. and Richard Gariott
 * reverse-coded by Ergonomy Joe in 2013
 */
#ifndef __U6_H__
#define __U6_H__

union tHandler {
	struct {
		unsigned ofs, seg;
	} iii;
	void far *p;
	void far *(*f)();
};

#include <dos.h>
#include <stdlib.h>
#include <alloc.h>
#include <process.h>
#include <ctype.h>
#include <string.h>

#include "gr.h"
#include "mouse.h"
#include "osi_file.h"
#include "seg_3522.h"
#include "seg_356A.h"

#include "ai.h"
#include "tile.h"
#include "obj.h"
#include "cmd.h"
#include "spells.h"

#define SCR2TIL(i) ((i) >> 4)
#define TIL2SCR(i) ((i) << 4)

/* */

#define DELTA(a,b) (abs((a)-(b)))
#define CLOSE_ENOUGH(d,x1,y1,x2,y2) (DELTA(x1,x2) <= (d) && DELTA(y1,y2) <= (d))
#define CLOSE_ENOUGH_S(d,x1,y1,x2,y2) (DELTA(x1,x2) < (d) && DELTA(y1,y2) < (d))

#define DELTA0(a,b) (__abs__((a)-(b)))
#define CLOSE_ENOUGH0(d,x1,y1,x2,y2) (DELTA0(x1,x2) <= (d) && DELTA0(y1,y2) <= (d))
#define CLOSE_ENOUGH0_S(d,x1,y1,x2,y2) (DELTA0(x1,x2) < (d) && DELTA0(y1,y2) < (d))

/* */

#define __Amount_ptr(o, i) ((unsigned char far *)(Amount + (o)))[i]

#define GetQuan(o) (__Amount_ptr(o, 0))
#define GetQual(o) (__Amount_ptr(o, 1))
#define SetQuan(o, v) (__Amount_ptr(o, 0) = (v))
#define SetQual(o, v) (__Amount_ptr(o, 1) = (v))
#define AddQuan(o, v) (__Amount_ptr(o, 0) += (v))
#define AddQual(o, v) (__Amount_ptr(o, 1) += (v))
#define SubQuan(o, v) (__Amount_ptr(o, 0) -= (v))
#define SubQual(o, v) (__Amount_ptr(o, 1) -= (v))
#define QualQuan(quality, quantity) ((quality) << 8 | (quantity))

/*----------------------------------------------------*/
/*these macros test the simple Bit fields in ObjStatus*/
/*----------------------------------------------------*/
#define LOCXYZ 0
#define CONTAINED 8
#define INVEN 0x10
#define EQUIP 0x18

#define SetCoordUse(i, v) (ObjStatus[i] = (ObjStatus[i] & ~0x18) | (v))
#define GetCoordUse(i) (ObjStatus[i] & 0x18)
/*i.e Is INVEN or EQUIP*/
#define Is_0010(i) (ObjStatus[i] & 0x10)

#define OWNED     0x01
#define INVISIBLE 0x02
#define CHARMED   0x04
#define LOCAL     0x20
#define CURSED    0x40
#define MUTANT    0x40
#define HATCHED   0x40
#define LIT       0x80

/*clear cursed and charmed*/
#define Rst_0044(i) (ObjStatus[i] &= ~(CURSED|CHARMED))
/*clear cursed, charmed and invisible*/
#define Rst_0046(i) (ObjStatus[i] &= ~(CURSED|CHARMED|INVISIBLE))

#define IsOwned(i)      (ObjStatus[i] & OWNED)
#define IsInvisible(i)  (ObjStatus[i] & INVISIBLE)
#define IsCharmed(i)    (ObjStatus[i] & CHARMED)
#define IsLocal(i)      (ObjStatus[i] & LOCAL)
#define IsCursed(i)     (ObjStatus[i] & CURSED)
#define IsHatched(i)    (ObjStatus[i] & HATCHED)
#define IsMutant(i)     (ObjStatus[i] & MUTANT)
#define IsLit(i)        (ObjStatus[i] & LIT)
#define SetOwned(i)     ObjStatus[i] |= OWNED
#define SetInvisible(i) ObjStatus[i] |= INVISIBLE
#define SetCharmed(i)   ObjStatus[i] |= CHARMED
#define SetLocal(i)     ObjStatus[i] |= LOCAL
#define SetCursed(i)    ObjStatus[i] |= CURSED
#define SetHatched(i)   ObjStatus[i] |= HATCHED
#define SetLit(i)       ObjStatus[i] |= LIT
#define ClrInvisible(i) ObjStatus[i] &= ~INVISIBLE
#define ClrCharmed(i)   ObjStatus[i] &= ~CHARMED
#define ClrLocal(i)     ObjStatus[i] &= ~LOCAL
#define ClrCursed(i)    ObjStatus[i] &= ~CURSED
#define ClrHatched(i)   ObjStatus[i] &= ~HATCHED
#define ClrLit(i)       ObjStatus[i] &= ~LIT

/*----------------------------------------------------*/
/*these macros test the simple Bit fields in NPCStatus*/
/*----------------------------------------------------*/
#define NEUTRAL 0
#define EVIL 0x20
#define GOOD 0x40
#define CHAOTIC 0x60

#define SetAlignment(i, v) (NPCStatus[i] = (NPCStatus[i] & ~0x60) | (v))
#define GetAlignment(i) (NPCStatus[i] & 0x60)
#define Is_ATKPLR(i) (GetAlignment(i) & 0x20)
#define Is_ATKMON(i) (GetAlignment(i) & 0x40)

#define PROTECTED  0x01
#define PARALYZED  0x02
#define ASLEEP     0x04
#define POISONED   0x08
#define DEAD       0x10
#define PLRCONTROL 0x80

#define Isbis_0014(i) (NPCStatus[i] & (DEAD|ASLEEP))
#define Isbis_0016(i) (NPCStatus[i] & (DEAD|ASLEEP|PARALYZED))
#define Rstbis_0006(i) (NPCStatus[i] &= ~(ASLEEP|PARALYZED))
#define Rstbis_001e(i) (NPCStatus[i] &= ~(DEAD|POISONED|ASLEEP|PARALYZED))
#define Rstbis_001f(i) (NPCStatus[i] &= ~(DEAD|POISONED|ASLEEP|PARALYZED|PROTECTED))

#define IsProtected(i)   (NPCStatus[i] & PROTECTED)
#define IsParalyzed(i)   (NPCStatus[i] & PARALYZED)
#define IsAsleep(i)      (NPCStatus[i] & ASLEEP)
#define IsPoisoned(i)    (NPCStatus[i] & POISONED)
#define IsDead(i)        (NPCStatus[i] & DEAD)
#define IsPlrControl(i)  (NPCStatus[i] & PLRCONTROL)
#define SetProtected(i)  (NPCStatus[i] |= PROTECTED)
#define SetParalyzed(i)  (NPCStatus[i] |= PARALYZED)
#define SetAsleep(i)     (NPCStatus[i] |= ASLEEP)
#define SetPoisoned(i)   (NPCStatus[i] |= POISONED)
#define SetDead(i)       (NPCStatus[i] |= DEAD)
#define SetPlrControl(i) (NPCStatus[i] |= PLRCONTROL)
#define ClrProtected(i)  (NPCStatus[i] &= ~PROTECTED)
#define ClrParalyzed(i)  (NPCStatus[i] &= ~PARALYZED)
#define ClrAsleep(i)     (NPCStatus[i] &= ~ASLEEP)
#define ClrPoisoned(i)   (NPCStatus[i] &= ~POISONED)
#define ClrDead(i)       (NPCStatus[i] &= ~DEAD)
#define ClrPlrControl(i) (NPCStatus[i] &= ~PLRCONTROL)

/*--------------------------------------------------*/
/*these macros test the simple Bit fields in NPCFlag*/
/*--------------------------------------------------*/
#define GetDirection(i)      (NPCFlag[i] & 7)
#define SetDirection(i, v)   (NPCFlag[i] = (NPCFlag[i] & ~7) | (v))

#define GetBKAlignment(i)    (NPCFlag[i] & 0x60)
#define SetBKAlignment(i, v) (NPCFlag[i] = (NPCFlag[i] & ~0x60) | (v))

#define IsSkipSomeTest(i)    (NPCFlag[i] & 8)
#define IsDraggedUnder(i)    (NPCFlag[i] & 0x10)
#define IsWalking(i)         (NPCFlag[i] & 0x80)
#define SetSkipSomeTest(i)   (NPCFlag[i] |= 8)
#define SetDraggedUnder(i)   (NPCFlag[i] |= 0x10)
#define SetWalking(i)        (NPCFlag[i] |= 0x80)
#define ClrSkipSomeTest(i)   (NPCFlag[i] &= ~8)
#define ClrDraggedUnder(i)   (NPCFlag[i] &= ~0x10)
#define ClrWalking(i)        (NPCFlag[i] &= ~0x80)

/*------------------------------------------------------*/
/*                                                      */
/*------------------------------------------------------*/
/*====---- TerrainType ----====*/
/*[Wet]*/
#define TERRAIN_FLAG_01 0x01
/*[Impass]able*/
#define TERRAIN_FLAG_02 0x02
/*[Wall]*/
#define TERRAIN_FLAG_04 0x04
/*[Damag]ing*/
#define TERRAIN_FLAG_08 0x08
/*[W]*/
#define TERRAIN_FLAG_10 0x10
/*[S]*/
#define TERRAIN_FLAG_20 0x20
/*[E]*/
#define TERRAIN_FLAG_40 0x40
/*[N]*/
#define TERRAIN_FLAG_80 0x80

#define IsTerrainWet(tile)    (TerrainType[tile] & TERRAIN_FLAG_01)
#define IsTerrainImpass(tile) (TerrainType[tile] & TERRAIN_FLAG_02)
#define IsTerrainWall(tile)   (TerrainType[tile] & TERRAIN_FLAG_04)
#define IsTerrainDamage(tile) (TerrainType[tile] & TERRAIN_FLAG_08)

/*====---- TileFlag ----====*/
/*[Light] level*/
#define TILE_FLAG1_03 0x03
/*[Opa]que*/
#define TILE_FLAG1_04 0x04
/*[Win]dow*/
#define TILE_FLAG1_08 0x08
/*[For]eground*/
#define TILE_FLAG1_10 0x10
/*[Nos]hooting through*/
#define TILE_FLAG1_20 0x20
/*[Double] horizontal*/
#define TILE_FLAG1_40 0x40
/*[Double] vertical*/
#define TILE_FLAG1_80 0x80

#define GetTileLight(tile)  (TileFlag[tile] & TILE_FLAG1_03)
#define IsTileOpa(tile)     (TileFlag[tile] & TILE_FLAG1_04)
#define IsTileWin(tile)     (TileFlag[tile] & TILE_FLAG1_08)
#define IsTileFor(tile)     (TileFlag[tile] & TILE_FLAG1_10)
#define IsTileNos(tile)     (TileFlag[tile] & TILE_FLAG1_20)
#define IsTileDoubleV(tile) (TileFlag[tile] & TILE_FLAG1_40)
#define IsTileDoubleH(tile) (TileFlag[tile] & TILE_FLAG1_80)

/*====---- D_B3EF ----====*/
/*[Wa]rm*/
#define TILE_FLAG2_01 0x01
/*[Su]pporting*/
#define TILE_FLAG2_02 0x02
/*[Br]eakthrough*/
#define TILE_FLAG2_04 0x04
/*[Ge]neric -- "Replicate"-able and "Vanish"-able*/
#define TILE_FLAG2_08 0x08
/*[Ig]nore*/
#define TILE_FLAG2_10 0x10
/*[Ba]ckground*/
#define TILE_FLAG2_20 0x20
/*x a an the*/
#define TILE_FLAG2_C0 0xC0

#define IsTileWa(tile) (D_B3EF[tile] & TILE_FLAG2_01)
#define IsTileSu(tile) (D_B3EF[tile] & TILE_FLAG2_02)
#define IsTileBr(tile) (D_B3EF[tile] & TILE_FLAG2_04)
#define IsTileGe(tile) (D_B3EF[tile] & TILE_FLAG2_08)
#define IsTileIg(tile) (D_B3EF[tile] & TILE_FLAG2_10)
#define IsTileBa(tile) (D_B3EF[tile] & TILE_FLAG2_20)
/*------------------------------------------------------*/
/* */
#define TypeFrame(t, s) ((t) + ((s) << 10))
#define GetType(i) (ObjShapeType[i] & 0x3ff)
#define GetFrame(i) (ObjShapeType[i] >> 10)
#define SetType(i, v) (ObjShapeType[i] = (ObjShapeType[i] & ~0x3ff) + (v))
#define SetFrame(i, v) (ObjShapeType[i] = TypeFrame(GetType(i),(v)))

#define GetCoordX(o) ((*(int *)&((o).raw[0]))&0x3ff)
#define GetCoordY(o) (((*(int *)&((o).raw[1]))>>2)&0x3ff)
#define GetCoordZ(o) ((o).raw[2] >> 4)
#define GetCoordAssoc(i) *(unsigned int *)(&(i))

#define SetCoordX(o, v) { (*(int *)&((o).raw[0])) &= ~0x3ff; (*(int *)&((o).raw[0])) |= (v); }
#define SetCoordY(o, v) { (*(int *)&((o).raw[1])) &= ~(0x3ff << 2); (*(int *)&((o).raw[1])) |= (v) << 2; }

#define SetCoordXYZ(o, x, y, z) {      \
	(o).raw[2] = ((z) << 4);           \
	*(int *)&((o).raw[0]) = (x);       \
	*(int *)&((o).raw[1]) |= (y) << 2; \
}

#define SetCoordAssoc(i, j) *(unsigned int *)(&(i)) = (j)

#define SetXYZ(i, x, y, z) SetCoordXYZ(ObjPos[i], x, y, z)
#define SetAssoc(i, j) SetCoordAssoc(ObjPos[i], j)

#define GetX(i) GetCoordX(ObjPos[i])
#define GetY(i) GetCoordY(ObjPos[i])
#define GetZ(i) GetCoordZ(ObjPos[i])
#define GetAssoc(i) GetCoordAssoc(ObjPos[i])

/*------------------------------------------------------*/
#define AREA_H 40
#define AREA_W 40
#define SHAMINO_COMMENT (IsPlrControl(3) && !IsDead(3) && COMBAT_getCathesus(3, MapX, MapY) < 6)
#define IN_VEHICLE (Active == PartySize)
#define TILE_FRAME(objNum) (BaseTile[GetType(objNum)] + GetFrame(objNum))

#define MAX_ANIM 0x20

/*for Equipment[]*/
#define SLOT_HEAD 0
#define SLOT_NECK 1
#define SLOT_RHND 2
#define SLOT_RFNG 3
#define SLOT_CHST 4
#define SLOT_LHND 5
#define SLOT_LFNG 6
#define SLOT_FEET 7
#define SLOT_2HND 8
#define SLOT_RING 9
/* */

struct coord {
	unsigned char raw[3];
};

struct t_9E39 {
	struct {
		unsigned char _00;/*ObjStatus*/
		struct coord _01;/*ObjPos*/
		int _04;/*ObjShapeType*/
		int _06;/*Amount*/
	} inner[0xc00];
	int _6000[0xc00];
	unsigned char _7800[0x100];
};

struct tClipInfo/*size:9*/ {
	unsigned char _x0, _y0, _w, _h;
	unsigned char _curx, _cury;
	unsigned char _fg, _bg;
	unsigned char _attr;
};

/*C_0903_00DE*/extern void far *LoadFile(char *, void far *);
extern void C_0903_0430();
/*C_0903_033A*/extern LoadFileGr(char far *, void far *, int);


extern C_0C9C_01FB();
/*C_0C9C_331A*/extern CON_atoi(unsigned, char far *, int, int);
/*C_0C9C_3643*/extern cdecl CON_printf(unsigned char far *, ...);

/*C_101C_0DC9*/extern unsigned char GetTileAtXYZ(int, int, int);

/*C_2FC1_10D2*/extern int far *Explosion(int, int, int);
/*C_2FC1_11EB*/extern int far *MagicWind(int, int, int, int);

#define DISK_1 1
#define DISK_2 2
#define DISK_3 3
#define DISK_4 4
#define DISK_5 5
#define DISK_6 6
#define DISK_7 7
#define DISK_8 8
/*C_32C3_0097*/extern DISK_confirm(int);/*request floppy*/

/*C_32FD_0000*/extern cdecl decompress(int, unsigned long, void far *, unsigned);


/*0194*/extern int IsZFile;
extern char *D_0198[];
/*01A2*/extern char DirIncrX[];
/*01AA*/extern char DirIncrY[];
/*01B2*/extern char *DirNames[];
extern int D_01CA;/*music adapter*/
extern int D_01CE;/*gr card type*/
/*01D0*/extern unsigned char MouseOn;

extern int D_0338;
/*033A*/extern unsigned char StatusDirty;
/*033B*/extern unsigned char PromptFlag;
/*033C*/extern unsigned char CyclopsFlag;
/*033D*/extern unsigned char IsArmageddon;
extern int D_033E;
extern int D_0340;
/*0342*/extern int Erupting;
/*0492*/extern unsigned char SelectMode;
/*0493,0494*/extern char AimX,AimY;
/*0495*/extern char SelectRange;
extern unsigned char D_0497;
extern char D_0499;
extern char D_049A;
extern unsigned char D_049C;
extern int D_049D;
extern int D_049F;
extern int D_04A1;
/*04A5*/extern int TextOn;
extern int D_04A7;
extern int D_04A9;
extern int D_04AD;
extern int D_04B3;
extern unsigned char D_04B5;
extern unsigned long D_04B6;
/*04BA*/extern int MousePress;
/*04BC*/extern int MouseTrigger;
/*04BE*/extern int MouseMode;
/*04C0*/extern int StatusDisplay;
extern int D_04C2;
/*04C4*/extern int AllowMouseMov;
/*04C6,04C8*/extern int MouseMapX,MouseMapY;
extern unsigned char D_04CA;
extern unsigned char D_04CB;
/*04D4*/extern int PromptCh;
extern struct tClipInfo *D_04DF;
extern int D_05CA;
extern int D_05CC;/*file "chunks" handler*/
extern int D_05DA;
extern int D_05DC;
extern int D_05DE;
/*05E4,05E6*/extern int ViewX,ViewY;
extern int D_05E8;
extern int D_0658;
extern int D_065C[];
extern int D_06E6;
extern unsigned char D_07CC;
extern unsigned char D_07CE;
/*098B*/extern unsigned char IsInConversation;
/*0AF0*/extern unsigned char *SpellCode[];
extern unsigned char *D_0BF0[];/*magic words*/
/*0C24*/extern unsigned char *SpellName[];
/*0D24*/extern char *Reagents_name[];
/*0D44*/extern unsigned char Reagents_needed[];
extern char *D_0DDC[];
/*17A1*/extern int AllowNPCTeleport;
extern int D_17AE;
extern int D_17B0;
extern int D_17B2;
extern int D_17B6;/*"shamino hears something..." flag*/
extern unsigned char D_17CE[];
extern int D_1968;
extern int D_196A;
extern unsigned char D_196C[][8];
extern unsigned char D_19AC[][11];
/*28A0*/extern int ForceHatching;

/*28BA*/extern unsigned char MUS_SongIndex;
/*28BB*/extern unsigned char MUS_Song;
/*28BC*/extern unsigned char MUS_Clock;
/*28BD*/extern unsigned char MUS_Bell;
/*28C0*/extern int MUS_MIndex;

/*2A48*/extern int ScreenFade;
extern int D_2A4A;
extern int D_2A4C;
extern int D_2A4E;
extern int D_2A50;
extern int D_2A54;
extern int D_2A56;
extern int D_2A58;
#include "D_2C4A.h"
extern int D_2E9E;
extern int D_2FFC;
extern int D_2FFE;
extern void (*D_3004)();

/*====---- uninitialized ----====*/
/*module seg_0903*/
/*322E*/extern char far *Mem_BP;
/*3232*/extern long Mem_Remain;
/*3236*/extern char Names[][14];/*[savegame\objlist]*/
/*3316*/extern unsigned char DEXTE[];/*[savegame\objlist]*/
/*3416*/extern struct tMouse Mouse;
/*342F*/extern unsigned char far *Leader;/*[savegame\objlist]*/
/*3433*/extern unsigned char INTEL[];/*[savegame\objlist]*/
/*3533*/extern unsigned char Party[];/*[savegame\objlist]*/
/*3544*/extern unsigned char far *NPCMode;/*[savegame\objlist]*/
/*3548*/extern unsigned ObjShapeType[];
extern void huge *D_4D48;
/*4D4C*/extern unsigned char far *NPCFlag;/*[savegame\objlist]*/
/*4D50*/extern unsigned char far *TalkBuf;
extern unsigned D_4D54[];
/*4EE4*/extern int Amount[];
/*66E4*/extern unsigned char HitPoints[];/*[savegame\objlist]*/
/*67E4*/extern unsigned int D_67E4[MAX_ANIM];/*part of animdata*/
/*6824*/extern int far *BaseTile;
/*6828*/extern struct coord ObjPos[];

struct tSchedule {
	unsigned char time;/*_00*/
	unsigned char action;/*_01*/
	struct coord xyz;/*_02*/
};
/*8C28*/extern struct tSchedule far *Schedule;
extern struct {
	unsigned char content[3];
} far *D_8C2C;/*palette*/
/*8C30*/extern int far *OrigShapeType;/*[savegame\objlist]*/
/*8C34*/extern struct tSurface Screen;
extern struct {
	unsigned char content[8][32];
} far *D_8C42;/*[savegame\objlist]*/
/*8C46*/extern unsigned char far *TileFlag;
/*8C4A*/extern unsigned char STREN[];/*[savegame\objlist]*/
/*8D4A*/extern char MovePts[];/*[savegame\objlist]*/
extern unsigned D_8E4A;
/*8E4C*/extern unsigned char far *Level;/*[savegame\objlist]*/
/*8E50*/extern unsigned char PartySize;/*[savegame\objlist]*/
/*8E51*/extern unsigned char AreaTiles[AREA_H][AREA_W];/*current tile map?*/
/*9491*/extern unsigned char far *SchedIndex;/*[savegame\objlist]*/
extern unsigned char D_9495[MAX_ANIM];/*part of animdata*/
/*94B5*/extern unsigned char AreaFlags[AREA_H][AREA_W];
/*9AF5*/extern unsigned char MAGIC[];/*[savegame\objlist]*/
/*9BF5*/extern unsigned ExpPoints[];/*[savegame\objlist]*/
extern int D_9DF5[MAX_ANIM];/*part of animdata*/
/*9E35*/extern unsigned char far *NPCComMode;/*[savegame\objlist]*/
/*9E39*/extern struct t_9E39 far *ScratchBuf;
extern struct tSurface D_9E3D;
extern unsigned D_9E4B[];
/*9FAB*/extern unsigned char NPCStatus[];/*[savegame\objlist]*/
/*A0AB*/extern unsigned char ObjStatus[];/*[savegame\objlist]*/
/*ACAB*/extern unsigned char AreaLight[AREA_H][AREA_W];
/*B2EB*/extern unsigned char TalkFlags[];/*[savegame\objlist]*/
/*B3EB*/extern unsigned char far *TerrainType;
extern unsigned char far *D_B3EF;
extern void far *D_B3F3;/*all system far memory*/
extern unsigned char D_B3F7[MAX_ANIM];/*part of animdata*/
/*B417*/extern unsigned char far *TypeWeight;
extern unsigned long D_B41B;/*farcoreleft result*/
/*B41F*/extern int NumAnimData;
/*B421*/extern int SchedPointer[];
extern void far *D_B623;
extern unsigned char D_B628[];
/*B64E*/extern unsigned char PromptCnt;
extern int D_B64F;
extern unsigned char D_B651[];
extern unsigned char D_B67A[];
/*B62D*/extern unsigned char StateAnimData[];
/*B6A3,B6A5*/extern int PointerX,PointerY;
extern unsigned char D_B6A7[4];/*incatation word x*/
extern unsigned char D_B6AB[4];/*incatation word y*/
extern struct {
	/*B6AF,B6B1,B6B3*/int x,y,obj;
} Selection;
extern struct tClipInfo D_B6B5[4];
/*B6D9*/extern int AdvanceDir;
extern unsigned char D_B6DB[4];/*incantation*/
extern int D_B6DF;
/*B6E1*/extern int VarInt[10+26];
extern int D_B729;
extern int D_B72B;
/*B72D*/extern char *VarStr[10+26];
/*B775*/extern struct tSurface MouseArea;
/*module seg_101c*/
#define CHUNKS_CACHE_MAX 16

extern int D_B784[CHUNKS_CACHE_MAX];/*cached chunks "age"?*/
extern unsigned char far *D_B7A4;/*currently loaded chunk map*/
extern int D_B7A8[CHUNKS_CACHE_MAX];/*chunks cache list*/
extern unsigned char D_B7C8[CHUNKS_CACHE_MAX][8][8];/*chunks cache*/
/*BBC8,BBCA*/extern int AreaX,AreaY;
/*module seg_1100*/
extern unsigned char D_BBCC[256];
extern unsigned char D_BCCC[256];
/*BDCC*/extern int AreaOffset;
/*module seg_1184*/
extern int D_BDCE[4];/*current search area*/
extern int D_BDD6;/*last monster index?*/
extern int D_BDD8;
/*BDDA*/extern int Link[];
extern int D_D5DA;/*last object index?*/
extern int D_D5DC[];
extern char D_D7DC[];
/*D7F5*/extern int Tile_11x11[11][11];
/*D8E7*/extern int MapObjPtr[AREA_H][AREA_W];
/*E567*/extern unsigned char Obj_11x11[11][11];
extern unsigned char D_E5E0[];
extern int D_E6E0;/*remaining object indexes?*/
extern int D_E6E2;/*remaining monster indexes?*/
/*E6E4*/extern int Equipment[];
/*E6F4*/extern int WeightEquip;
/*E6F6*/extern unsigned char InvenSize;
/*E6F7*/extern int WeightInven;
/*E6F9*/extern unsigned char Party_1Hand[16];
extern int D_E709;
/*E70B*/extern int NPC_1Hand;
/*E70D*/extern int ShowNPCInven;
extern int D_E70F[];/*"backpack" slots*/
extern unsigned char D_E728[];
extern unsigned char D_E732[];
extern char D_E764[];
extern int D_E796[2];/*interlocutor/locutor*/
extern unsigned char D_E79C;
extern unsigned long D_E79D;
extern int D_E7A1;
extern long D_E7A7;
/*E7AB*/extern int Talk_PC;
extern char D_E7AD[10][100];
/*EB96*/extern int Spellbook[2];
/*EB9A*/extern int SpellList[8];
extern unsigned char D_EBAA;
extern int D_EBAB;/*previous StatusDisplay(for seg_1944)*/
extern unsigned char D_EBAD[];
extern unsigned char D_EBB0;
extern unsigned char D_EBB1;
extern unsigned char D_EBB3;
/*EBB2*/extern char SpellResult;
/*EBB4*/extern unsigned char SeanceFlag;
extern unsigned char D_EBB5;
extern unsigned char D_EBB6[];
extern unsigned char D_EBBE[];
/*EBC6*/extern unsigned char PTH_Cur;
/*EBC7*/extern int PTH_Top;
/*EBC9*/extern unsigned char PTH_Index;
/*EBCA*/extern int PTH_AnX;
/*EBCC*/extern int PTH_Found;
/*EBCE*/extern int PTH_AnY;
/*EBD0*/extern unsigned char PTH_Min;
/*EBD1*/extern int PTH_direct;
/*EBD3*/extern int PTH_object;
extern unsigned char D_EBD5[];
/*EBE5*/extern int PartyGravityX;
/*EBE7*/extern int PartyGravityY;
extern int D_EBE9;/*PartyTarget#1*/
/*EBEB*/extern int EnemiesGravityX;
extern int D_EBED;/*PartyTarget#0*/
/*EBEF*/extern int EnemiesGravityY;
extern int D_EBF1;/*EnemyTarget#1*/
extern int D_EBF3;
extern int D_EBF5;/*PartyTarget#2*/
extern int D_EBF7;/*EnemyTarget#0*/
extern int D_EBF9;/*EnemyTarget#2*/
/*EBFB*/extern int EnemiesNum;
extern int D_EBFE[2];/*missile x,y?*/
extern int D_EC02[3],D_EC08[3];/*3xcrossbow bolts aim x,y*/
extern int D_EC0E[];
extern int D_EC1E[];
extern int D_EC9E;
/*module seg_2f1a*/
extern int D_ECA0;/*index in MIDI.DAT?*/
extern int D_ECA2[4];
extern int D_ECAA;
extern int D_ECAC[4];
extern union tHandler D_ECB4;/*music driver*/
/*module seg_2fc1 */
extern union tHandler D_ECB8;
extern int D_ECBC[4];/*main map clipping?left,right,top,bottom*/
extern struct tGr far *D_ECC4;
/*module seg_32c3*/
extern unsigned char D_ECC8,D_ECC9;/*dos version.major/minor*/
/*====---- ----====*/

#endif
