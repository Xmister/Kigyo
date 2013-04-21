#ifdef __GNUC__
#include "ncur.h"
#ifdef CURSES
#include <stdio.h>
#include "linux.h"
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


int kbhit(void)
{
  int ch=0;
  struct termios oldt, newt;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  read(0,&ch,1);

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return ch;
  }

  return 0;
  
}

void linux_init() {
    initscr();
    cbreak();
    noecho();
}
void linux_restore() {
   endwin();
}

int Sleep(int nanosec) {
    usleep(nanosec*1000);
}

int mygetch() {
struct termios oldt,
newt;
int ch;
tcgetattr( STDIN_FILENO, &oldt );
newt = oldt;
newt.c_lflag &= ~( ICANON | ECHO );
tcsetattr( STDIN_FILENO, TCSANOW, &newt );
ch = getchar();
tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
return ch;
}

int gotoxy(int x, int y) {
 move(y,x);
 return 0;
}
#endif
#endif
