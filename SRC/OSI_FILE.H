/*
 * Ultima VI - The False Prophet
 * Copyright (c) 1990 Origin Systems, Inc. and Richard Gariott
 * reverse-coded by Ergonomy Joe in 2013
 */
#ifndef __OSI_FILE_H__
#define __OSI_FILE_H__

/*error management */
extern union tHandler pascal D_032E;/*error callback*/
extern int pascal D_0332;/*error code*/
extern int pascal D_0334;/*error origin*/
extern char * pascal D_0336;/*last fileName*/
/* */

extern pascal OSI_callback();

extern int pascal OSI_open(char *path);
extern int pascal OSI_close(int handle);
extern int pascal OSI_create(char *path);
extern unsigned long pascal OSI_read(int handle, unsigned long offset, unsigned long length, void far *buf);
extern unsigned long pascal OSI_write(int handle, unsigned long offset, unsigned long length, void far *buf);
extern unsigned long pascal OSI_fileSize(int handle);


/*TODO*/

#endif
