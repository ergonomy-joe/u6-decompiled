PUBLIC Mem_BP,Mem_Remain
PUBLIC Names,DEXTE
PUBLIC Mouse
PUBLIC Leader,INTEL,Party
PUBLIC NPCMode,ObjShapeType
PUBLIC D_4D48,NPCFlag,TalkBuf,D_4D54,Amount
PUBLIC HitPoints,D_67E4,BaseTile,ObjPos
PUBLIC Schedule,D_8C2C,OrigShapeType
PUBLIC Screen
PUBLIC D_8C42,TileFlag,STREN,MovePts,D_8E4A,Level
PUBLIC PartySize,AreaTiles,SchedIndex,D_9495,AreaFlags,MAGIC
PUBLIC ExpPoints,D_9DF5,NPCComMode,ScratchBuf
PUBLIC D_9E3D
PUBLIC D_9E4B,NPCStatus,ObjStatus
PUBLIC AreaLight,TalkFlags,TerrainType,D_B3EF,D_B3F3,D_B3F7,TypeWeight,D_B41B,NumAnimData
PUBLIC SchedPointer
PUBLIC D_B623
PUBLIC D_B628,StateAnimData,PromptCnt,D_B64F,D_B651,D_B67A
PUBLIC PointerX,PointerY,D_B6A7,D_B6AB
PUBLIC Selection
PUBLIC D_B6B5
PUBLIC AdvanceDir,D_B6DB,D_B6DF,VarInt,D_B729,D_B72B,VarStr
PUBLIC MouseArea
PUBLIC D_B784,D_B7A4,D_B7A8,D_B7C8,AreaX,AreaY
PUBLIC D_BBCC
PUBLIC D_BCCC,AreaOffset,D_BDCE,D_BDD6
PUBLIC D_BDD8,Link,D_D5DA,D_D5DC,D_D7DC,Tile_11x11,MapObjPtr
PUBLIC Obj_11x11,D_E5E0,D_E6E0,D_E6E2,Equipment,WeightEquip,InvenSize
PUBLIC WeightInven,Party_1Hand,D_E709,NPC_1Hand,ShowNPCInven,D_E70F,
PUBLIC D_E728,D_E732,D_E764,D_E796
PUBLIC D_E79C,D_E79D,D_E7A1
PUBLIC D_E7A7,Talk_PC,D_E7AD
PUBLIC Spellbook,SpellList,D_EBAA,D_EBAB,D_EBAD,D_EBB0
PUBLIC D_EBB1,SpellResult,D_EBB3,SeanceFlag,D_EBB5,D_EBB6,D_EBBE
PUBLIC PTH_Cur,PTH_Top,PTH_Index,PTH_AnX,PTH_Found,PTH_AnY,PTH_Min
PUBLIC PTH_direct,PTH_object,D_EBD5,PartyGravityX,PartyGravityY,D_EBE9,EnemiesGravityX
PUBLIC D_EBED,EnemiesGravityY,D_EBF1,D_EBF3,D_EBF5,D_EBF7,D_EBF9
PUBLIC EnemiesNum
PUBLIC D_EBFE,D_EC02,D_EC08,D_EC0E,D_EC1E,D_EC9E
PUBLIC D_ECA0,D_ECA2,D_ECAA,D_ECAC,D_ECB4
PUBLIC D_ECB8,D_ECBC,D_ECC4,D_ECC8,D_ECC9


_BSS	SEGMENT PUBLIC WORD 'BSS'

Mem_BP	dd ?	;D_322E
Mem_Remain	dd ?	;D_3232
Names	db 16 * 14 dup(?)	;D_3236
DEXTE	db 100h dup(?)	;D_3316
Mouse	db 19h dup(?)	;struct tMouse	;D_3416
Leader	dd ?	;D_342F
INTEL	db 100h dup(?)	;D_3433
Party	db 11h dup(?)	;D_3533
NPCMode	dd ?	;D_3544
ObjShapeType	dw 0C00h dup(?)	;D_3548
D_4D48	dd ?
NPCFlag	dd ?	;D_4D4C
TalkBuf	dd ?	;D_4D50
D_4D54	dw 200 dup(?)	;scanlines offset
Amount	db 0C00h dup(?,?)	;D_4EE4
HitPoints	db 100h dup(?)	;D_66E4
D_67E4	dw 20h dup(?)	;part of animdata	;D_67E4
BaseTile	dd ?	;D_6824
ObjPos	db 3*0C00h dup(?)	;D_6828
Schedule	dd ?	;D_8C28
D_8C2C	dd ?	;u6pal
OrigShapeType	dd ?	;D_8C30
Screen	db 0Eh dup(?)	;struct tSurface	;D_8C34
D_8C42	dd ?	;[savegame\\objlist]
TileFlag	dd ?	;D_8C46
STREN	db 100h dup(?)	;D_8C4A
MovePts	db 100h dup(?)	;D_8D4A
D_8E4A	dw ?
Level	dd ?	;D_8E4C
PartySize	db ?	;D_8E50
AreaTiles	db 40*40 dup(?)	;D_8E51
SchedIndex	dd ?	;D_9491
D_9495	db 20h dup(?)	;part of animdata
AreaFlags	db 40*40 dup(?)	;D_94B5
MAGIC	db 100h dup(?)	;D_9AF5
ExpPoints	dw 100h dup(?)	;D_9BF5
D_9DF5	dw 20h dup(?)	;part of animdata
NPCComMode	dd ?	;D_9E35
ScratchBuf	dd ?	;D_9E39
;-- struct tSurface --
D_9E3D	dw ?	;D_9E3D.segment
__9E3F	dd ?	;D_9E3D.pScanlines
__9E43	dw ?	;D_9E3D.right
__9E45	dw ?	;D_9E3D.left
__9E47	dw ?	;D_9E3D.top
__9E49	dw ?	;D_9E3D.bottom
;-- --
D_9E4B	dw 176 dup(?)	;scanlines offset
NPCStatus	db 100h dup(?)	;D_9FAB
ObjStatus	db 0C00h dup(?)	;D_A0AB
AreaLight	db 40*40 dup(?)	;D_ACAB
TalkFlags	db 100h dup(?)	;D_B2EB
TerrainType	dd ?	;part of tileflag	;D_B3EB
D_B3EF	dd ?	;part of tileflag
D_B3F3	dd ?	;all system far memory
D_B3F7	db 20h dup(?)	;part of animdata
TypeWeight	dd ?	;part of tileflag	;D_B417
D_B41B	dd ?	;farcoreleft result
NumAnimData	dw ?	;part of animdata	;D_B41F
SchedPointer	dw (100h + 1) dup(?)	;D_B421
D_B623	dd ?
__B627	db ?
;--
D_B628	db 5 dup(?)	;a string?
StateAnimData	db 21h dup(?)	;D_B62D
PromptCnt	db ?	;D_B64E
D_B64F	dw ?
D_B651	db 29h dup(?)
D_B67A	db 29h dup(?)
PointerX	dw ?	;D_B6A3
PointerY	dw ?	;D_B6A5
D_B6A7	db 4 dup(?)
D_B6AB	db 4 dup(?)
Selection	dw ?	;D_B6AF
	dw ?	;D_B6B1
	dw ?	;D_B6B3
D_B6B5	db 9 dup(?)	;window/clip info?
__B6BE	db 9 dup(?)	;window/clip info?
__B6C7	db 9 dup(?)
__B6D0	db 9 dup(?)	;window/clip info?
AdvanceDir	dw ?	;D_B6D9
D_B6DB	db 4 dup(?)
D_B6DF	dw ?
VarInt	dw (10+26) dup(?)	;D_B6E1
D_B729	dw ?
D_B72B	dw ?
VarStr	dw (10+26) dup(?)	;D_B72D
;-- struct tSurface	;D_B775
MouseArea	dw ?	;MouseArea.segment
__B777	dd ?	;MouseArea.pScanlines
__B77B	dw ?	;MouseArea.right
__B77D	dw ?	;MouseArea.left
__B77F	dw ?	;MouseArea.top
__B781	dw ?	;MouseArea.bottom
;--
__B783	db ?	;padding?
;--
D_B784	dw 10h dup(?)
D_B7A4	dd ?	;part of map
D_B7A8	dw 10h dup(?)
D_B7C8	db 10h * 8 * 8 dup(?)
AreaX	dw ?	;D_BBC8
AreaY	dw ?	;D_BBCA
D_BBCC	db 100h dup(?)
D_BCCC	db 100h dup(?)
AreaOffset	dw ?	;D_BDCC
D_BDCE	dw 4 dup(?)
D_BDD6	dw ?
D_BDD8	dw ?
Link	dw 0C00h dup(?)	;D_BDDA
D_D5DA	dw ?
D_D5DC	dw 100h dup(?)
D_D7DC	db 19h dup(?)
Tile_11x11	dw 11*11 dup(?)	;D_D7F5
MapObjPtr	dw 40*40 dup(?)	;D_D8E7
Obj_11x11	db 11*11 dup(?)	;D_E567
D_E5E0	db 100h dup(?)
D_E6E0	dw ?
D_E6E2	dw ?
Equipment	dw 8 dup(?)	;D_E6E4
WeightEquip	dw ?	;D_E6F4
InvenSize	db ?	;D_E6F6
WeightInven	dw ?	;D_E6F7
Party_1Hand	db 10h dup(?)	;D_E6F9
D_E709	dw ?
NPC_1Hand	dw ?	;D_E70B
ShowNPCInven	dw ?	;D_E70D
D_E70F	dw 0Ch dup(?)
__E727	db ?
;--
D_E728	db 0Ah dup(?)
D_E732	db 32h dup(?)	;input string for conversation?
D_E764	db 32h dup(?)	;interlocutor's name
D_E796	dw 2 dup(?)	;interlocutor/locutor
__E79A	db ?
__E79B	db ?
D_E79C	db ?
D_E79D	dd ?
D_E7A1	dw ?
__E7A3	db 4 dup(?)
D_E7A7	dd ?
Talk_PC	dw ?	;D_E7AB
D_E7AD	db 10*100 dup(?)
__EB95	db ?
Spellbook	dw 2 dup(?)	;D_EB96
SpellList	dw 8 dup(?)	;D_EB9A
D_EBAA	db ?
D_EBAB	dw ?
D_EBAD	db 3 dup(?)
D_EBB0	db ?
D_EBB1	db ?
SpellResult	db ?	;D_EBB2
D_EBB3	db ?
SeanceFlag	db ?	;D_EBB4
D_EBB5	db ?
D_EBB6	db 8 dup(?)
D_EBBE	db 8 dup(?)
PTH_Cur	db ?	;D_EBC6
PTH_Top	dw ?	;D_EBC7
PTH_Index	db ?	;D_EBC9
PTH_AnX	dw ?	;D_EBCA
PTH_Found	dw ?	;D_EBCC
PTH_AnY	dw ?	;D_EBCE
PTH_Min	db ?	;D_EBD0
PTH_direct	dw ?	;D_EBD1
PTH_object	dw ?	;D_EBD3
D_EBD5	db 10h dup(?)
PartyGravityX	dw ?	;D_EBE5
PartyGravityY	dw ?	;D_EBE7
D_EBE9	dw ?
EnemiesGravityX	dw ?	;D_EBEB
D_EBED	dw ?
EnemiesGravityY	dw ?	;D_EBEF
D_EBF1	dw ?
D_EBF3	dw ?
D_EBF5	dw ?
D_EBF7	dw ?
D_EBF9	dw ?
EnemiesNum	dw ?	;D_EBFB
__EBFD	db ?
D_EBFE	dw 2 dup(?)	;missile x,y?
D_EC02	dw 3 dup(?)
D_EC08	dw 3 dup(?)
D_EC0E	dw 8 dup(?)
D_EC1E	dw 40h dup(?)
D_EC9E	dw ?
D_ECA0	dw ?	;index in MIDI.DAT?
D_ECA2	dw 4 dup(?)
D_ECAA	dw ?
D_ECAC	dw 4 dup(?)
D_ECB4	dw ?,?	;music driver
;-- --
D_ECB8	dw ?,?	;graphic driver
D_ECBC	dw 4 dup(?)
D_ECC4	dd ?
D_ECC8	db ?	;dos version.major
D_ECC9	db ?	;dos version.minor

_BSS	ENDS

END
