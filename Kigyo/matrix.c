#include <stdio.h>
#ifndef __GNUC__
#ifndef WIN32
#define ANSI 1
#endif
#endif
#ifdef WIN32
#include "econio.h"
#elif __GNUC__
#include "ncur.h"
#ifdef CURSES
#include "linux.h"
#else
#define ANSI 1
#endif
#endif
#include "types.h"
#include "matrix.h"
#include "kigyo.h"
#include <malloc.h>

char** mold;
int cleared=0;
int cleared1=0;

int initMatrix(char* m[]) {
	int i,j;
	mold=(char**) malloc(sizeof(char*)*Y);
	for (i=0; i<Y; i++) {
		m[i]=(char*) malloc(X*sizeof(char));
		mold[i]=(char*) malloc(X*sizeof(char));
		for (j=0; j<X; j++) {
			m[i][j]=' ';
			mold[i][j]=m[i][j];
		}
	}
	return 1;
}

int writeMatrix(char* m[], kigyo** k) {
	int i,j;
	for (i=-1; i<=Y; i++) {
		for (j=-1; j<=X; j++) {
			if (i==-1 || i == Y) kiir("%c",'-');
			else if (j==-1 || j==X) kiir("%c",'|');
			else kiir("%c",m[i][j]);

			if ( i==-1 && j==X ) kiir(" Pontszamok:");
			else if ( i==0 && j == X ) kiir(" %c jatekos: %3d",k[0][0].j,k[0][0].pont);
			else if ( getMulti() && i==1 && j == X ) kiir(" %c jatekos: %3d",k[1][0].j,k[1][0].pont);
			else if ( i==3 && j == X ) kiir(" Meg %3d etel",getEtel());
			if ( i>-1 && j>-1 && j<X && i < Y) { 
				mold[i][j]=m[i][j];
			}

		}
		kiir("%c",'\n');
	}
	if ( getMulti() ) kiir("%c - iranyitas w,a,s,d BULLET TIME q\n",k[1][0].j);
	kiir("%c - iranyitas i,j,k,l BULLET TIME u\n",k[0][0].j);
	return 0;
}
int refreshMatrix(char* m[], kigyo** k) {
#ifdef ANSI
    return writeMatrix (m,k);
#else	
	int i,j;
	for (i=0; i<Y; i++) {
		for (j=0; j<X; j++) {
				if ( mold[i][j] != m[i][j] ) {
					gotoxy(j+1,i+1);
					kiir("%c",m[i][j]);
					mold[i][j]=m[i][j];
				}
		}
	}
	gotoxy(X+14,1);
	kiir("%3d",k[0][0].pont);
	if ( getMulti() ) {
		gotoxy(X+14,2);
		kiir("%3d",k[1][0].pont);
	}
	gotoxy(X+7,4);
	kiir("%3d",getEtel());
#ifdef LINUX
	refresh();
#endif
	return 0;
#endif
}
#ifndef ANSI
void writeStatus(char* s) {
	gotoxy(X+3,Y-2);
	kiir("%c - %s",getKch(0),s);
	cleared=0;
}

void writeStatus2(char* s) {
	gotoxy(X+3,Y-1);
	kiir("%c - %s",getKch(1),s);
	cleared1=0;
}

void clearStatus() {
	char s[]="                ";
	if (!cleared) {
		gotoxy(X+3,Y-2);
		kiir("%s",s);
		cleared=1;
	}
}

void clearStatus2() {
	char s[]="                ";
	if (!cleared1) {
		gotoxy(X+3,Y-1);
		kiir("%s",s);
		cleared1=1;
	}
}
#endif