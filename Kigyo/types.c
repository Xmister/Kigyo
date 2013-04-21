#include <stdio.h>
#include "types.h"
#ifdef __GNUC__
#include "ncur.h"
#endif
#ifdef CURSES
#include "linux.h"
#else 
#define ANSI 1
#endif

void initPrint() {
#ifdef LINUX
    kiir=printw;
    beolv=scanw;
#else
    kiir=printf;
    beolv=scanf;
#endif
}   
    