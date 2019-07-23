#ifndef __PRN_HPP_INCLUDED__   // if x.h hasn't been included yet...
#define __PRN_HPP_INCLUDED__   //   #define this so the compiler knows it has been included

#include <stdio.h>

namespace sys_tools {

    void prnl(char* prefix, char* message, char* suffix)
    {
        fprintf(stdout, "\033[0m%s \033[0;36m%s : \033[1;32m%s\n\033[0m", prefix, message, suffix);
    }

}

/* This prints a ✓. */
//printf("\xE2\x9C\x93\n");

/*
printf(“\033[0;31m”); //Set the text to the color red
CODE	COLOR
[0;31m	Red
[1;31m	Bold Red
[0;32m	Green
[1;32m	Bold Green
[0;33m	Yellow
[01;33m	Bold Yellow
[0;34m	Blue
[1;34m	Bold Blue
[0;35m	Magenta
[1;35m	Bold Magenta
[0;36m	Cyan
[1;36m	Bold Cyan
[0m	Reset
*/

#endif