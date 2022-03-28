/*
 * Ultima VI - The False Prophet
 * Copyright (c) 1990 Origin Systems, Inc. and Richard Gariott
 * reverse-coded by Ergonomy Joe in 2013
 */
#ifndef __SPELLS_H__
#define __SPELLS_H__

#define REAGENT_MR 0x01
#define REAGENT_NS 0x02
#define REAGENT_BP 0x04
#define REAGENT_BM 0x08
#define REAGENT_SS 0x10
#define REAGENT_GA 0x20
#define REAGENT_GS 0x40
#define REAGENT_SA 0x80

/*1st circle*/
#define SPELL_00 0x00
#define SPELL_01 0x01
#define SPELL_02 0x02
#define SPELL_03 0x03
#define SPELL_04 0x04
#define SPELL_05 0x05
#define SPELL_06 0x06
#define SPELL_07 0x07
#define SPELL_08 0x08
#define SPELL_09 0x09
/*2nd circle*/
#define SPELL_10 0x10
#define SPELL_11 0x11
#define SPELL_12 0x12
#define SPELL_13 0x13
#define SPELL_14 0x14
#define SPELL_15 0x15
#define SPELL_16 0x16
#define SPELL_17 0x17
#define SPELL_18 0x18
#define SPELL_19 0x19
/*3rd circle*/
#define SPELL_20 0x20
#define SPELL_21 0x21
#define SPELL_22 0x22
#define SPELL_23 0x23
#define SPELL_24 0x24
#define SPELL_25 0x25
#define SPELL_26 0x26
#define SPELL_27 0x27
#define SPELL_28 0x28
#define SPELL_29 0x29
/*4th circle*/
#define SPELL_30 0x30
#define SPELL_31 0x31
#define SPELL_32 0x32
#define SPELL_33 0x33
#define SPELL_34 0x34
#define SPELL_35 0x35
#define SPELL_36 0x36
#define SPELL_37 0x37
#define SPELL_38 0x38
#define SPELL_39 0x39
/*5th circle*/
#define SPELL_40 0x40
#define SPELL_41 0x41
#define SPELL_42 0x42
#define SPELL_43 0x43
#define SPELL_44 0x44
#define SPELL_45 0x45
#define SPELL_46 0x46
#define SPELL_47 0x47
#define SPELL_48 0x48
#define SPELL_49 0x49
/*6th circle*/
#define SPELL_50 0x50
#define SPELL_51 0x51
#define SPELL_52 0x52
#define SPELL_53 0x53
#define SPELL_54 0x54
#define SPELL_55 0x55
#define SPELL_56 0x56
#define SPELL_57 0x57
#define SPELL_58 0x58
#define SPELL_59 0x59
/*7th circle*/
#define SPELL_60 0x60
#define SPELL_61 0x61
#define SPELL_62 0x62
#define SPELL_63 0x63
#define SPELL_64 0x64
#define SPELL_65 0x65
#define SPELL_66 0x66
#define SPELL_67 0x67
#define SPELL_68 0x68
#define SPELL_69 0x69
/*8th circle*/
#define SPELL_70 0x70
#define SPELL_71 0x71
#define SPELL_72 0x72
#define SPELL_73 0x73
#define SPELL_74 0x74
#define SPELL_75 0x75
#define SPELL_76 0x76
#define SPELL_77 0x77
#define SPELL_78 0x78
#define SPELL_79 0x79

/*kind of invisibility?*/
#define SPELL_80 0x80
/*teleport (for wisps)*/
#define SPELL_81 0x81

#define MK_CIRCLE(spell) (spell / 0x10 + 1)

#define HAS_SPELL(circle,spell) ((SpellList[circle] >> (spell)) & 1)
#define SET_SPELL(circle,spell) (SpellList[circle] |= 1 << (spell))

#endif
