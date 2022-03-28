/*
 * Ultima VI - The False Prophet
 * Copyright (c) 1990 Origin Systems, Inc. and Richard Gariott
 * reverse-coded by Ergonomy Joe in 2013
 */
#ifndef __AI_H__
#define __AI_H__

#define AI_MOTIONLESS 0x00
#define AI_FOLLOW     0x01
#define AI_COMMAND    0x02
/*combat modes*/
#define AI_FRONT      0x03
#define AI_REAR       0x04
#define AI_FLANK      0x05
#define AI_BERSEK     0x06
#define AI_RETREAT    0x07
#define AI_ASSAULT    0x08

#define AI_SHY        0x09
#define AI_LIKE       0x0a
#define AI_UNFRIENDLY 0x0b
#define AI_GRAZE      0x0c
#define AI_TANGLE     0x0d
#define AI_IMMOBILE   0x0e
/*looks like guard?*/
#define AI_0F         0x0f
/*looks like guard?*/
#define AI_10         0x10
#define AI_ARREST     0x12
#define AI_FEAR       0x13

#define AI_SCHEDULE   0x80
#define AI_FINDPATH   0x81
#define AI_SEEKOBJ    0x82
#define AI_ONPATH     0x83
/*b1?*/
#define AI_84         0x84
/*b2?*/
#define AI_85         0x85
/*bl?*/
#define AI_86         0x86
/*schedule modes*/
#define AI_STAND_N    0x87
#define AI_STAND_E    0x88
#define AI_STAND_S    0x89
#define AI_STAND_W    0x8a
#define AI_GUARD_N    0x8b
#define AI_GUARD_E    0x8c
#define AI_GUARD_S    0x8d
#define AI_GUARD_W    0x8e
#define AI_WANDER     0x8f
#define AI_LOITER     0x90
#define AI_SLEEP      0x91
#define AI_SIT        0x92
#define AI_EAT        0x93
#define AI_FARM       0x94
#define AI_PLAY       0x95
#define AI_CONVERSE   0x96
#define AI_THIEF      0x97
#define AI_RINGBELL   0x98
#define AI_BRAWL      0x99

/*[NPC]RETREAT?*/
#define AI_9A         0x9a
#define AI_VIGILANTE  0x9b

#endif
