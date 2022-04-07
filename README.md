# u6-decompiled
Decompiled code for PC/MSDOS version of Ultima VI - The False Prophet (version 4.5)


This project allows you to build `GAME.EXE`, with very few differences from the original.
I have also decompiled most of the other executables, but I have to make the code more readable before releasing it.

The techniques used to decompile `GAME.EXE` is basically the same I used to [decompile Ultima IV](https://github.com/ergonomy-joe/u4-decompiled):
* disassemble the executable
* gather the tools/libraries that were originaly used (Borland Turbo C 2.0 in our case)
* search for modules/functions separation in the code.
* for each function, read assembly code and try to guess what was the original C code
* build new code and compare the resulting binary with original binary
* eventually correct code and build again
* repeat 3 last steps until all the executable file has been reversed

Some years ago, someone released **Ultima 6 Technical Documents** online, which proved very helpful to understand some parts of the code, and to give a name to some functions/variables/macros.
Be aware though that these documents refer to an earlier version of the game, and so they do not reflect exactly what is in the code.


Concerning the build process:  
The original game was build with Borland Turbo C 2.0 for the part written in C language (most of the files).
There are some `asm` files too, you will need tasm 2.0 and masm 3.0 to build them.

All you need to know for the build process is in the file `SRC/doit.bat` which uses `MAKEFILE`.
Before that, you will need to launch `SRC/OSILIB/doit.bat`. I think I put most of the `asm` files in there.

The naming of the C files is not friendly at all, and I apologize for that. Maybe some renaming will take place in the future ?

Have a lot of decompiling fun!  
2022/03/29 ergonomy_joe

PS: when I first decompiled this code, I posted some comments on a [french abandonware related forum](https://www.abandonware-forums.org/), you can take a look [here](https://www.abandonware-forums.org/forum/forum-ltf-abandonware-france/le-bar-des-amis/36760-ultima-vi-les-codes-sources-r%C3%A9g%C3%A9n%C3%A9r%C3%A9s?35874-Ultima-VI-les-codes-sources-r%E9g%E9n%E9r%E9s=).

# Steps to compile

1. Download dosbox https://www.dosbox.com
1. Launch dosbox
1. >cycles 15000
	1. This speeds up builds
1. >MOUNT c "[Local-Directory]/SRC"
	e.g. MOUNT c "..\SRC"
1. Download tools
	1. Go to https://www.pcjs.org/software/pcx86/util/other/unp/4.11/
	1. In the dropdown under the command prompt select "UNP (Executable Unpacker) Source"
	1. Click "Load" and then "Save"
	1. In the dropdown select "MS C 4.00 (Disk 2)"
	1. Rename downloaded file to MSC2.img
	1. In the dropdown select "MS Macro Assembler 3.00"
	1. Rename downloaded file to MASM300.img
	1. Go to https://winworldpc.com/product/turbo-assembler/20
	1. Download Borland Turbo Assembler 2.0 (5.25-360k)
	1. Extract Disk01.img from the download and place with other img files
	1. Go to https://winworldpc.com/product/borland-turbo-c/2x
	1. Download "Borland Turbo C 2.0 (1988) (3.5-720k)"
	1. Extract disk02.img and disk03.img from download and place with other img files
1. Extract tools
	1. >mkdir C:\tools
	1. >mkdir C:\tools\turboc20
	1. >mkdir C:\tools\turboc20\include
	1. >mkdir C:\tools\turboc20\lib
	1. >MOUNT d "[Local Directory with img files]"
	1. >imgmount a D:\UNP411~1.IMG -t floppy
	1. >copy A:\UNP.EXE C:\TOOLS
	1. >imgmount -u a
	1. >imgmount a D:\Disk01.IMG -t floppy
	1. >a:
	1. >mkdir D:\TASM
	1. >unzip TASM.ZIP D:\TASM
	1. >copy A:\TASM.EXE C:\TOOLS\TASM200.EXE
	1. >imgmount -u a
	1. >imgmount a D:\MSC2.IMG -t floppy
	1. >copy A:\EXEPACK.EXE C:\tools
	1. >imgmount -u a
	1. >imgmount a D:\MASM300.IMG -t floppy
	1. >copy A:\MASM.EXE C:\TOOLS\MASM300.EXE
	1. >imgmount -u a
	1. >imgmount a D:\disk02.img -t floppy
	1. >copy A:\*.EXE C:\TOOLS\TURBOC20\
	1. >imgmount -u a
	1. >imgmount a D:\disk03.img -t floppy
	1. >copy A:\TLIB.EXE C:\TOOLS\TURBOC20\TLIB.EXE
	1. >copy A:\*.H C:\TOOLS\TURBOC20\INCLUDE\
	1. >copy A:\*.OBJ C:\TOOLS\TURBOC20\LIB\
	1. >copy A:\*.LIB C:\TOOLS\TURBOC20\LIB\
1. Build
	1. >C:
	1. >cd C:\OSILIB
	1. >DOIT
	1. >cd ..
	1. >DOIT