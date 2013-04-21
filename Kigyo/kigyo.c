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
#include <stdlib.h>
#include "types.h"
#include "matrix.h"
#include "kigyo.h"
#include <math.h>
#include <time.h>
#include <string.h>

int id;
int KINCS=10;
int kc;
int ka;
int level=0;
int teleport;
char karak[2];
int fal;
int kor=0;
int bull;
int fug;
int multi;
int sch;
int newK(char* m[],kigyo* k,char c) {
	kigyo *k_;
	int i=1;
	do {
		k->p.pX=rand();
		while ( k->p.pX > X-5 ) k->p.pX/=10;
		k->p.pY=rand();
		while ( k->p.pY > Y-5 ) k->p.pY/=10;
	} while (m[k->p.pY][k->p.pX] != ' ' || m[k->p.pY+1][k->p.pX] != ' ' || m[k->p.pY+2][k->p.pX] != ' ');
	k->id=id++;
	k->pont=0;
	k->li=fel;
	k->f=0;
	k->s=0;
	k->c='^';
	k->bullet=0;
	k->usedb=1;
	k->activeb=0;
	k->stopped=0;
	m[k->p.pY][k->p.pX]=k->c;
	karak[k->id]=c;
	k->j=c;
	k->kprev=NULL;
	for ( k_=k; i<KIGYO_MERET; i++) {
		k_->k=(kigyo*) malloc(sizeof(kigyo));
		k_->k->li=fel;
		k_->k->p.pY=k_->p.pY+1;
		k_->k->p.pX=k_->p.pX;
		m[k_->k->p.pY][k_->k->p.pX]=c;
		k_->k->id=k_->id;
		k_->k->c=c;
		k_->k->f=1;
		k_->k->kprev=k_;
		k_->k->k=NULL;
		k_=k_->k;
	}
	return 1;
}

int incSize(char* m[], kigyo* k) {
	kigyo *k_;
	char* st;
	int i;
	k->pont++;
#ifndef ANSI
	if ( bull && !k->activeb && k->usedb ) {
		k->bullet++;
		st=(char*) malloc(sizeof(char)*k->bullet);
		st[0]='\0';
		for (i=1; i<=k->bullet; i++) strcat(st,"=");
		k->id ? writeStatus2(st) : writeStatus(st);
	}
	if ( bull && k->bullet == 5 ) {
		k->bullet=6;
		k->usedb=0;
		if ( !k->id ) writeStatus ("BULLET READY");
		else writeStatus2("BULLET READY");
	}
#endif
	for (k_=k; k_->k!=NULL; k_=k_->k);
	switch (k_->li) {
		case fel:
			k_->k=(kigyo*) malloc(sizeof(kigyo));
			k_->k->li=k_->li;
			k_->k->c=k_->c;
			k_->k->id=k_->id;
			k_->k->p.pY=k_->kprev->p.pY+1;
			k_->k->p.pX=k_->kprev->p.pX;
			if (k_->k->p.pY >= Y) {
				if (!getFal () ) {
					k_->k->p.pY--;
					if ( k_->k->p.pX-1 >= 0 ) k_->k->p.pX--;
					else if ( k_->k->p.pX+1 < X ) k_->k->p.pX++;
				} else	k_->k->p.pY=0;
			}
			m[k_->k->p.pY][k_->k->p.pX]=k_->k->c;
			k_->k->id=0;
			k_->k->k=NULL;
			k_->k->kprev=k_;
			break;
		case le:
			k_->k=(kigyo*) malloc(sizeof(kigyo));
			k_->k->li=k_->li;
			k_->k->c=k_->c;
			k_->k->id=k_->id;
			k_->k->p.pY=k_->kprev->p.pY-1;
			k_->k->p.pX=k_->kprev->p.pX;
			if (k_->k->p.pY < 0) {
				if ( !getFal () ) {
					k_->k->p.pY++;
					if ( k_->k->p.pX-1 >= 0 ) k_->k->p.pX--;
					else if ( k_->k->p.pX+1 < X ) k_->k->p.pX++;
				} else k_->k->p.pY=Y-1;
			}
			m[k_->k->p.pY][k_->k->p.pX]=k_->k->c;
			k_->k->id=0;
			k_->k->k=NULL;
			k_->k->kprev=k_;
			break;
		case balra:
			k_->k=(kigyo*) malloc(sizeof(kigyo));
			k_->k->li=k_->li;
			k_->k->c=k_->c;
			k_->k->id=k_->id;
			k_->k->p.pY=k_->kprev->p.pY;
			k_->k->p.pX=k_->kprev->p.pX+1;
			if (k_->k->p.pX < 0) {
				if (!getFal ()) {
					k_->k->p.pX--;
					if ( k_->k->p.pY-1 >= 0 ) k_->k->p.pY--;
					else if ( k_->k->p.pY+1 < Y ) k_->k->p.pY++;
				} else k_->k->p.pX=X-1;
			}
			m[k_->k->p.pY][k_->k->p.pX]=k_->k->c;
			k_->k->id=0;
			k_->k->k=NULL;
			k_->k->kprev=k_;
			break;
		case jobbra:
			k_->k=(kigyo*) malloc(sizeof(kigyo));
			k_->k->li=k_->li;
			k_->k->c=k_->c;
			k_->k->id=k_->id;
			k_->k->p.pY=k_->kprev->p.pY;
			k_->k->p.pX=k_->kprev->p.pX-1;
			if (k_->k->p.pX >= X) {
				if (!getFal ()) {
					k_->k->p.pX++;
					if ( k_->k->p.pY-1 >= 0 ) k_->k->p.pY--;
					else if ( k_->k->p.pY+1 < Y ) k_->k->p.pY++;
				} else k_->k->p.pX=0;
			}
			m[k_->k->p.pY][k_->k->p.pX]=k_->k->c;
			k_->k->id=0;
			k_->k->k=NULL;
			k_->k->kprev=k_;
			break;
	}
	kc--;
	ka--;
	if ( kc <= 0 ) return 1;
	return 0;
}

int himself(char* m[],kigyo* k) {
	if ( m[k->p.pY][k->p.pX] == k->j ) return 1;
	return 0;
}

int checkpos(char* m[],kigyo* k) {
	char c;
	static int k1s=0;
	static int k2s=0;
	if (m[k->p.pY][k->p.pX] == ' ') if ( k->id ) k2s=0; else k1s=0;
	else {
		if (m[k->p.pY][k->p.pX] == KJ) return 2;
		if ( m[k->p.pY][k->p.pX] == 'O' ) return 6;
		if ( sch && ( ( k->id && !k2s ) || ( !k->id && !k1s ) ) ) {
			k->s=1;
			if ( k->id ) k2s=1; else k1s=1;
			return 3;
		} else {
			c=k->id ? karak[k->id-1] : karak[k->id+1]; 
			if ( m[k->p.pY][k->p.pX] ==  c ) return 5;
		}
		return 0;
	}
	return 1;
}

int lep(char* m[], kigyo* k, int ir) {
	kigyo *k_;
	int res;
	int ck;
	pos pp,pp2;
	tele* t;
	int i;
	switch (ir) {
		case fel2:
			ir=fel;
			break;
		case le2:
			ir=le;
			break;
		case balra2:
			ir=balra;
			break;
		case jobbra2:
			ir=jobbra;
			break;
	}
	//if ( ir==balra || ir == jobbra || ( (ir == fel || ir == le) && k->f )) {
	if ( 
		( 
			!k->stopped &&
			( 
				!k->activeb || 
				( k->activeb && !k->f ) 
			) && !k->s
		)
		&& 
		( 
			! fug || 
			( ir==balra || ir == jobbra ||  
				( 
					( ir == fel || ir == le ) && kor 
				)
			)
		)
	   )		{
		if ( bull ) {
			k->f=level/2;
			if (k->activeb) {
				k->bullet--;
			}
		}
		pp.pY=k->p.pY;
		pp.pX=k->p.pX;
		if (ir == fel && k->li == le ) ir=le;
		else if ( ir == le && k->li == fel ) ir=fel;
		else if ( ir == balra && k->li == jobbra ) ir = jobbra;
		else if ( ir == jobbra && k->li == balra ) ir = balra;
		k->li=ir;
		switch ( ir ) {
			case le:
			case fel:
				k->c='^';
				break;
			case balra:
				k->c='<';
				break;
			case jobbra:
				k->c='>';
				break;
		}
		switch ( ir ) {
			case fel:
				if (--k->p.pY < 0) {
					if ( getFal () ) k->p.pY=Y-1;
					else return 0;
				}//Fej léptetése
				break;
			case le:
				if (++k->p.pY > Y-1) {
					if ( getFal() ) k->p.pY=0;	
					else return 0;
				}
				break;
			case balra:
				if (--k->p.pX <0) {
					if ( getFal () ) k->p.pX=X-1;	
					else return 0;
				}
				break;
			case jobbra:
				if (++k->p.pX > X-1) {
					if (getFal () ) k->p.pX=0;	
					else return 0;
				}
				break;
		}
		res=0;
		switch (ck=checkpos(m,k)) {
			//és ellenõrzése.
			case 2: res=2; break;
			case 1: 
				t=getT();
				for (i=getTele()-1; i>=0; i--) {
					m[t[i].p1.pY][t[i].p1.pX] = 'O';
					m[t[i].p2.pY][t[i].p2.pX] = 'O';
				}
				break;
			case 6:
				t=getT();
				for (i=getTele()-1; i>=0 && ! ( ( t[i].p1.pY==k->p.pY  &&  t[i].p1.pX==k->p.pX ) || ( t[i].p2.pY==k->p.pY && t[i].p2.pX == k->p.pX ) ); i--);
				if (i>=0) {
					if ( t[i].p1.pY==k->p.pY  &&  t[i].p1.pX==k->p.pX ) {
						k->p.pX=t[i].p2.pX;
						k->p.pY=t[i].p2.pY;
					} else {
						k->p.pX=t[i].p1.pX;
						k->p.pY=t[i].p1.pY;
					}
				}
				break;
			case 3: 
				k->p.pX=pp.pX;
				k->p.pY=pp.pY;
				return 3;
				break;
			default: return ck;
		}
	#ifdef DEBUG
		writeMatrix(m,k);
	#endif
		m[k->p.pY][k->p.pX]=k->c;
	#ifdef DEBUG
		writeMatrix(m,k);
	#endif
		for ( k_=k; k_->k!=NULL; k_=k_->k) { //Farok léptetése utána
			m[k_->k->p.pY][k_->k->p.pX]=' ';
	#ifdef DEBUG
		writeMatrix(m,k);
	#endif
			pp2.pX=k_->k->p.pX;
			pp2.pY=k_->k->p.pY;
			k_->k->p.pY=pp.pY;
			k_->k->p.pX=pp.pX;
			pp=pp2;
			m[k_->k->p.pY][k_->k->p.pX]=k_->k->c;
	#ifdef DEBUG
		writeMatrix(m,k);
	#endif
		}
		for ( k_; k_->kprev!=NULL; k_=k_->kprev) {
			k_->li=k_->kprev->li;
		}
		if (res) {
			res=incSize(m,k);
		}
		if (himself(m,k)){
			return 4;
			}
		if (res) return res;
	} else if ( bull ) {
		if ( k->f ) k->f--; 
		if ( k->s ) k->s--;
		}
	return 3;
}

int lepes(char* m[], kigyo** kigyok) {
	int res1=3,res2=3,i;
	int ir=0;
	int ir1=-1, ir2=-1,tir1=-1,tir2=-1;
	int x,y,ok;
	char* st;
	struct bill_ { char c; struct bill_ *next, *prev;} *bill, *tbill;
	bill=tbill=NULL;
	while ( 1 ) {
#ifdef ANSI
		ir=karakter();
#elif LINUX
		if (ir=kbhit()) {
#else
		while ( kbhit() ) {
			ir=karakter();
#endif
			if ( ir >= 'A' && ir <= 'Z' ) {
				ir='a'+(ir-'A');
				}
			if ( bill == NULL ) {
				bill= (struct bill_ *) malloc(sizeof(struct bill_));
				bill->c=ir;
				bill->next=NULL;
				bill->prev=NULL;
			}
			else {
				for (tbill=bill; tbill->next!=NULL; tbill=tbill->next);
				tbill->next= (struct bill_ *) malloc(sizeof(struct bill_));
				tbill->next->c=ir;
				tbill->next->next=NULL;
				tbill->next->prev=tbill;
			}
			}
		if ( bill ) {
			for(bill; bill!=NULL; bill=bill->next) {
				if ( bill->prev!=NULL ) free(bill->prev);
				ir=bill->c;
				if ( ir == 27 ) return 0;
				if ( ir == le || ir == fel || ir == balra || ir == jobbra) {
					ir1=ir;
				} else if ( ir == le2 || ir == fel2 || ir == balra2 || ir == jobbra2) {
						ir2=ir;
				}
#ifndef ANSI
				else if ( getMulti() && bull && ir == 'q' && !kigyok[1][0].usedb ) {
					kigyok[1][0].activeb=1;
					kigyok[1][0].usedb=1;
					kigyok[1][0].stopped=0;
					if ( !kigyok[0][0].activeb ) kigyok[0][0].stopped=1;
					writeStatus2("BULLET TIME!");
				}
				else if ( bull && ir == 'u' && !kigyok[0][0].usedb ) {
					kigyok[0][0].activeb=1;
					kigyok[0][0].usedb=1;
					kigyok[0][0].stopped=0;
					if ( getMulti() && !kigyok[1][0].activeb ) kigyok[1][0].stopped=1;
					writeStatus("BULLET TIME!");
				} 
#endif
				if ( ir1 != -1) 
					{
					tir1=ir1;
					res1=lep(m,kigyok[0],ir1);
					ir1=-1;
					}
				if ( getMulti() && ir2 != -1) {
					tir2=ir2;
					res2=lep(m,kigyok[1],ir2);
					ir2=-1;
					}
				}
			ir1=tir1;
			ir2=tir2;
		}
		else {
				if ( ir1 != -1) res1=lep(m,kigyok[0],ir1);
				if ( getMulti() && ir2 != -1) res2=lep(m,kigyok[1],ir2);
			}
				if (!res1 || !res2) return 0;
					if (res1 == 1) return 1;
					else if (res2 == 1) return 2;
					else if (res1 == 4) return 4;
					else if (res2 == 4) return 5;
					else if (res1 == 5) return 6;
					else if (res2 == 5) return 7;
				ok=0;
				if (!ka) do {
					x=rand();
					while ( x>X-5 ) x/=10;
					y=rand();
					while ( y>Y-5) y/=10;
					if ( m[y][x] == ' ' ) {
						m[y][x]=KJ;
						ok=1;
						ka++;
					}
				} while ( !ok);
			refreshMatrix(m,kigyok);
#ifndef ANSI
				if ( bull && kigyok[0][0].bullet <=0 ) {
					kigyok[0][0].activeb=0;
					kigyok[0][0].bullet=0;
					if ( getMulti() ) kigyok[1][0].stopped=0;
					clearStatus();
				}
				if ( getMulti() && bull && kigyok[1][0].bullet <=0 ) {
					kigyok[1][0].activeb=0;
					kigyok[1][0].bullet=0;
					kigyok[0][0].stopped=0;
					clearStatus2();
				}
				if ( bull && kigyok[0][0].activeb ) {
					st=(char*) malloc(sizeof(char)*kigyok[0][0].bullet);
					st[0]='\0';
					for (i=1; i<=kigyok[0][0].bullet; i++) strcat(st,"=");
					clearStatus(); writeStatus(st);
				}
				if (getMulti() && bull && kigyok[1][0].activeb ) {
					st=(char*) malloc(sizeof(char)*kigyok[1][0].bullet);
					st[0]='\0';
					for (i=1; i<=kigyok[1][0].bullet; i++) strcat(st,"=");
					clearStatus2(); writeStatus2(st);
				}
				Sleep(500/level*2);
				kor=~kor;
#endif
			}
}

int getEtel() {
	return kc;
}

void keptorl() {
#ifdef WIN32
	clrscr();
#elif LINUX
    clear();
#else
	kiir("\033[2J");
#endif
}

int karakter() {
#ifdef WIN32
    return getch();
#elif LINUX
    return getch();
#else
    return getchar();
#endif
}

void initKigyo() {
	ka=0;
	kc=KINCS;
	id=0;
}

void setLevel(int lev) {
	level=lev;
}
int getLevel() {
	return level;
}

void setKc(int kincs) {
	KINCS=kincs;
}

int getKc() {
	return KINCS;
}

char getKch(int id) {
	return karak[id];
}

int getTele() {
	return teleport;
}

void setTele(int t) {
	teleport=t;
}

int getFal() {
	return fal;
}

void setFal(int f) {
	fal=f;
}

int getBull() {
	return bull;
}
void setBull(int b) {
	bull=b;
}
int getFug() {
	return fug;
}
void setFug(int f) {
	fug=f;
}

int getMulti() {
	return multi;
}
void setMulti(int m) {
	multi=m;
}

int getSch() {
	return sch;
}
void setSch(int s) {
	sch=s;
}