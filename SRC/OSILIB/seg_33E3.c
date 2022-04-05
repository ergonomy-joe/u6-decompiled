#include "u6.h"

/*C_33E3_0008*/OSI_playNote(int bp08, int bp06) {
	if((char)SoundFlag)
		OSI_sound(bp08);
	OSI_sDelay(1, bp06);
	OSI_nosound();
}

/*__emit__(0x90);*/
