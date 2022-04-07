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
