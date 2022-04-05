/*
 * Ultima VI - The False Prophet
 * Copyright (c) 1990 Origin Systems, Inc. and Richard Gariott
 * reverse-coded by Ergonomy Joe in 2013
 */
/*
	module talk
*/
#include "u6.h"

/*C_16E1_0007*/TALK_initTalk(int partyId, int objNum) {
	int si;

	/*set strings $*/
	VarStr['G' - 0x37] = D_2CCA?/*0920*/"milady":/*0927*/"milord";/*[g]reetings*/
	VarStr['N' - 0x37] = /*092E*/"lots of space for any possible NPC name";/*[n]ame of NPC*/
	VarStr['P' - 0x37] = Names[partyId];/*name of PC*/
	VarStr['T' - 0x37] =
		(Time_H < 12)?/*0956*/"morning":
		(Time_H < 18)?/*095E*/"afternoon":
		/*0968*/"evening"
	;/*[t]ime of day*/
	/*set variables #*/
	VarInt['A' - 0x37] = GetDex(Party[partyId]);/*PC's [a]gility*/
	VarInt['D' - 0x37] = Date_D;/*[d]ay*/
	VarInt['E' - 0x37] = ExpPoints[Party[partyId]];/*PC's [e]xperience*/
	VarInt['G' - 0x37] = (D_2CCA != 0);/*[g]ender?*/
	VarInt['H' - 0x37] = Time_H;/*[h]our*/
	VarInt['I' - 0x37] = GetInt(Party[partyId]);/*PC's [i]ntelligence*/
	VarInt['K' - 0x37] = KARMA;/*[k]arma*/
	VarInt['L' - 0x37] = D_2CA8;/*[l]anguage*/
	VarInt['M' - 0x37] = Date_M;/*[m]onth*/
	VarInt['O' - 0x37] = PartySize - 1;
	VarInt['P' - 0x37] = HitPoints[Party[partyId]];/*PC's hit [p]oints*/
	VarInt['S' - 0x37] = GetStr(Party[partyId]);/*PC's [s]trength*/
	VarInt['W' - 0x37] = NPCMode[objNum];/*NPC's [w]ork type*/
	VarInt['Y' - 0x37] = Date_Y;/*[y]ear*/
	/*-- reset $0~$9 --*/
	for(si = 0; si < 10; si ++)
		VarStr[si] = D_E7AD[si];
	/*-- reset #0~#9 --*/
	for(si = 0; si < 10; si ++)
		VarInt[si] = 0;
	/*--*/
	D_04DF->_attr |= 0x10;
	MUS_091A(0);
	TalkDriver(Party[partyId], objNum);
	MUS_09A8();
	D_049F = 1;
	D_2CA8 = VarInt['L' - 0x37];
}

/*"talk"?
;1 - avatar
;2 - dupre
;3 - shamino 
;4 - iolo
;5 - lord british
;...*/
/*C_16E1_015C*/TALK_talkTo(int partyId, int objNum, int aFlag) {
	int objTyp;

	objTyp = GetType(objNum);
	if(objTyp == OBJ_1AE || objTyp == OBJ_1AF)
		objNum = COMBAT_getHead(objNum);
	if(
		(objNum >= 0 && objNum < 0x100) ||
		objTyp == OBJ_189 || objTyp == OBJ_18D || objTyp == OBJ_18E || objTyp == OBJ_18F
	) {
		if(aFlag) {
			if(objTyp == OBJ_189) {
				CON_printf(/*0970*/"shrine");
			} else if(objTyp == OBJ_18D || objTyp == OBJ_18E || objTyp == OBJ_18F) {
				CON_printf(/*0977*/"statue");
			} else {
				if(TalkFlags[objNum] & 1)
					/*met already*/
					CON_printf((char *)C_1703_0116(objNum));
				else
					/*first time*/
					CON_printf((char *)GetObjectString(objNum));
			}
		}
		TALK_initTalk(partyId, objNum);
	} else {
		CON_printf(/*097E*/"nothing!\n");
	}
}
