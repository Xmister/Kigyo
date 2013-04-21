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
#include <time.h>
#include <string.h>

void saveSettings();
void loadSettings();

tele* t;
char* path;
void newGame(char** m, kigyo** k) {
	kigyo* k1=(kigyo*) malloc(sizeof(kigyo));
	kigyo* k2=(kigyo*) malloc(sizeof(kigyo));
	int i,x,y;
	srand(time(NULL));
	newK(m,k1,'#');
	if ( getMulti() ) newK(m,k2,'*');
	k[0]=k1;
	if ( getMulti() ) k[1]=k2;
	t=(tele*) malloc(sizeof(tele)*getTele());
	for (i=0; i<getTele(); i++) {
		do {
			x=rand();
			while ( x > X-1 ) x/=10;
			y=rand();
			while ( y > Y-1 ) y/=10;
		} while (m[y][x] != ' ');
		t[i].id=i;
		t[i].p1.pX=x;
		t[i].p1.pY=y;
		m[y][x]='O';
		do {
			x=rand();
			while ( x > X-1 ) x/=10;
			y=rand();
			while ( y > Y-1 ) y/=10;
		} while (m[y][x] != ' ');
		t[i].p2.pX=x;
		t[i].p2.pY=y;
		m[y][x]='O';
	}
	writeMatrix(m,k);
#ifdef LINUX
	refresh();
#endif
}

int gethighscore(int print) {
	FILE* f;
	score sc2;
	scorelist* sc, *sc_, *stemp, *tizedik;
	int n=0, elso,i,res;
	f=fopen("scores.dat","rb");
	tizedik=sc=sc_=stemp=NULL;
	if (f) {
		while ( !feof(f) ) {
			sc2.nev[0]='\0';
			if (fread(&sc2,sizeof(score),1,f) && strlen(sc2.nev) ) {
				stemp=(scorelist*) malloc(sizeof(scorelist));
				strcpy(stemp->sc.nev,sc2.nev);
				stemp->sc.sco=sc2.sco;
				stemp->next=NULL;
				if ( !n ) {
					sc=stemp;
				}
				elso=0;
				if ( sc->sc.sco < stemp->sc.sco ) elso=1;
				else for (sc_=sc; sc_->next && sc_->next->sc.sco >= stemp->sc.sco; sc_=sc_->next);
				if (elso ) {
					stemp->next=sc;
					sc=stemp;
				}
				else if (sc_!=stemp) {
					stemp->next=sc_->next;
					sc_->next=stemp;
				}
				n++;
				if ( n == 10) tizedik=stemp;
			}
		}
		fclose(f);
		if (print) for (sc_=sc,i=1; sc_; sc_=sc_->next,i++) {
			kiir("%d. %s\t%d\n",i,strlen(sc_->sc.nev) <=4 ? strcat(sc_->sc.nev,"\t") : sc_->sc.nev,sc_->sc.sco);
		}
		if ( n>=10) res=tizedik->sc.sco;
		if (stemp) free(stemp);
		return n<10 ? 0 : res;
	}
	return -1;
}

int highscore() {
	int score=gethighscore(1);
	if ( score == -1 ) kiir("Meg nincsenek pontszamok.\nVagy nem lehet megnyitni a fajlt.\n");
#ifdef LINUX
    refresh();
#endif
#ifdef ANSI
    karakter(); //Mivel az input buffer még tartalmazza a menünél leütött entert
#endif
	return score;
}

int jatek() {
	char** m = (char**) malloc(Y*sizeof(char*));
	char* buf=(char*)malloc(sizeof(char)*100);
	kigyo** kigyok=(kigyo**) malloc(sizeof(struct kigyo_)*2);
	int res,i;
	char c;
	score sc;
	FILE* f;
	initMatrix(m);
	initKigyo();
	newGame(m,kigyok);
	res=lepes(m,kigyok);
#ifndef ANSI
	gotoxy(0,Y+3);
#endif
	kiir("\n\n");
	buf[0]='\0';
		if (!res) {
			buf[1]='\0';
			kiir("Sajnos rossz helyre leptel.\nJatek vege!\n");
			if (getMulti()) {
				buf[0]=kigyok[0][0].pont > kigyok[1][0].pont ? kigyok[0][0].j : kigyok[1][0].pont>kigyok[0][0].pont ? kigyok[1][0].j : '\0';
				sc.sco=kigyok[0][0].pont > kigyok[1][0].pont ? kigyok[0][0].pont : kigyok[1][0].pont;
			} else sc.sco=kigyok[0][0].pont;
			kiir("Pontozassal %s nyert!\n",!buf[0] ? "senki sem" : buf);
		}
		else if ( res >= 4 && res < 6 ) {
			int id=kigyok[res-4][0].id;
			int id2=id ? id-1 : id+1;
			if ( getMulti() ) {
				kiir("%c - Az ongyilkossag halalos bun, nem tudtad? Ezert most %c nyert.\n",kigyok[id][0].j,kigyok[id2][0].j);
				sc.sco=kigyok[id2][0].pont;
				buf[0]=kigyok[id2][0].j;
			} else {
				kiir("%c - Meghaltal.\n",kigyok[0][0].j);
				sc.sco=kigyok[0][0].pont;
				buf[0]=kigyok[0][0].j;
			}
		}
		else if ( res >= 6 ) {
			int id=kigyok[res-6][0].id;
			int id2=id ? id-1 : id+1;
			kiir("%c - Gyilkos! Nekimentel %c-nek! Ezert most %c nyert.\n",kigyok[id][0].j,kigyok[id2][0].j,kigyok[id2][0].j);
			sc.sco=kigyok[id2][0].pont;
			buf[0]=kigyok[id2][0].j;
		}
		else {
			kiir("%c - Megettel mindent! Nyertel!\n",kigyok[res-1][0].j);
			sc.sco=kigyok[res-1][0].pont;
			buf[0]=kigyok[res-1][0].j;
		}
	#ifdef ANSI
		karakter(); //Input bufferben még ott az enter
	#endif
		karakter(); //Kerüljük el a gyors leütések miatti kilépést.
		if (buf[0] && sc.sco > gethighscore(0)) {
			do {
				buf[0]='\0';
				kiir("Gyoztes neve? ");
				c=0;
				for (i=0; i< 9 && (c=karakter()) && c!=13 && c!='\n'; i++) {
					if (c==8 || c == 127) {
						i=i-2;
						kiir("\b \b");
					} else if ( c<'0' || c>'z' ) {
						i=i--;
					}
					else {
						kiir("%c",c); 
						buf[i]=c;
					}
				}
				buf[i]='\0';
				kiir("\n");
			} while ( !strlen(buf) );
			strcpy(sc.nev,buf);
			f=fopen("scores.dat","ab");
			fwrite(&sc,sizeof(score),1,f);
			fclose(f);
		}
	free(m);
	free(buf);
	free(kigyok[0]);
	if ( getMulti() ) free(kigyok[1]);
	free(kigyok);
	return 0;
}

int kilep() {
	kiir("Koszonom, hogy jatszottal velem!\n");
	return 1;
}

void setSavePath(char* s) {
	char per =
#ifdef WIN32
		'\\'
#else 
		'/'
#endif
		;
	int i,li;
	for (i=li=0; s[i]; i++) {
		if (s[i]==per ) li=i;
	}
	path=(char*) malloc(sizeof(char)*(li+2+strlen("settings.dat")));
	strncpy(path,s,li+1);
	path[li+1]='\0';
	if ( !strlen(path) ) path=".";
	strcat(path,"settings.dat");
}

int changelevel() {
	int l;
	kiir("Adj meg egy szamot 1-100-ig(Jelenleg %d): ",getLevel());
	#ifdef LINUX
			refresh();
			echo();
#endif
			beolv("%d",&l);
#ifdef LINUX
            noecho();
#endif
	if ( l>0 && l<=100) {
		setLevel(l);
	}
	return 1;
}

int kincsek() {
	int l;
	kiir("Adj meg egy szamot 1-1000-ig(Jelenleg %d): ",getKc());
	#ifdef LINUX
			refresh();
			echo();
#endif
			beolv("%d",&l);
#ifdef LINUX
            noecho();
#endif
	if ( l>0 && l<=1000) {
		setKc(l);
	}
	return 1;
}

int telemenu() {
	int l;
	kiir("Adj meg egy szamot 0-10-ig(Mivel parban vannak, ennek a duplaja fog megjelenni)(Jelenleg %d): ",getTele());
	#ifdef LINUX
			refresh();
			echo();
#endif
			beolv("%d",&l);
#ifdef LINUX
            noecho();
#endif
	if ( l>=0 && l<=10) {
		setTele(l);
	}
	return 1;
}

int falmenu() {
	int l;
	kiir("A palya oldalan at lehessen menni?(1 - igen, 0 - nem)(Jelenleg %d): ",getFal());
	#ifdef LINUX
			refresh();
			echo();
#endif
			beolv("%d",&l);
#ifdef LINUX
            noecho();
#endif
	if ( l>=0 && l<=1) {
		setFal(l);
	}
	return 1;
}

int bullmenu() {
	int l;
	kiir("Engedelyezed a BULLET TIME-ot?(1 - igen, 0 - nem)(Jelenleg %d): ",getBull());
	#ifdef LINUX
			refresh();
			echo();
#endif
			beolv("%d",&l);
#ifdef LINUX
            noecho();
#endif
	if ( l>=0 && l<=1) {
		setBull(l);
	}
	return 1;
}

int fugmenu() {
	int l;
	kiir("Szeretned lassitani a fuggoleges mozgast?(1 - igen, 0 - nem)(Jelenleg %d): ",getFug());
	#ifdef LINUX
			refresh();
			echo();
#endif
			beolv("%d",&l);
#ifdef LINUX
            noecho();
#endif
	if ( l>=0 && l<=1) {
		setFug(l);
	}
	return 1;
}

int multimenu() {
	int l;
	kiir("Bekapcsolod a tobbjatekos modot?(1 - igen, 0 - nem)(Jelenleg %d): ",getMulti());
	#ifdef LINUX
			refresh();
			echo();
#endif
			beolv("%d",&l);
#ifdef LINUX
            noecho();
#endif
	if ( l>=0 && l<=1) {
		setMulti(l);
	}
	return 1;
}

int settingsend() {
	saveSettings();
		kiir("A beallitasok mentve\n");
	return 0;
}

int secChance() {
	int l;
	kiir("Bekapcsolod a masodik eselyt?(1 - igen, 0 - nem)(Jelenleg %d): ",getSch());
	#ifdef LINUX
			refresh();
			echo();
#endif
			beolv("%d",&l);
#ifdef LINUX
            noecho();
#endif
	if ( l>=0 && l<=1) {
		setSch(l);
	}
	return 1;
}

int settingsmenu() {
	int i;
	int in;
	menu prog[]={
		{0,settingsend,"Vissza"},
		{1,changelevel,"Sebesseg"},
		{2,kincsek,"Etel-csomagok"},
		{3,telemenu,"Teleportok szama"},
		{4,falmenu,"Palya falanak viselkedese"},
		{5,bullmenu,"BULLET TIME funkcio"},
		{6,fugmenu,"Fuggoleges mozgas lassitasa"},
		{7,multimenu,"Tobbjatekos mod"},
		{8,secChance,"Masodik esely"},
		{-1,0,0}
	};
	do {
		do {
			keptorl();
			kiir("Kigyo jatek!\n-------------------------\n\t\tBeallitasok\n-------------------------\n");
			for (i=0;prog[i].szam!=-1;i++) {
				kiir("\t\t%d: %s\n",prog[i].szam,prog[i].szoveg);
			}
			kiir("\n\t\tMenupont: ");
#ifdef LINUX
			refresh();
#endif
			in=karakter();
			if ( in<'0' || in>=i+'0' ) {
				kiir("%c - Nem lehetseges!",in);
				in=-1;
#ifdef LINUX
	            refresh();
#endif
#ifndef ANSI
				Sleep(1000);
#endif
			} else in=in-'0';
		} while ( in<0 || in>=i );
		keptorl();
		prog[in].prog();
	} while (in);
	return 1;
}

int main(int argc, char* argv[]) {
	int in;
	int i;
	menu prog[]={
		{0,kilep,"Kilepes"},
		{1,jatek,"Uj jatek"},
		{2,highscore,"Pontszamok"},
		{3,settingsmenu,"Beallitasok"},
		{-1,0,0}
	};
	initPrint();
#ifdef LINUX
    linux_init();
    refresh();
#endif
	setSavePath (argv[0]);
	loadSettings();
	do {
		do {
			keptorl();
			kiir("Kigyo jatek!\n-------------------------\n\t\tFomenu\n-------------------------\n");
			for (i=0;prog[i].szam!=-1;i++) {
				kiir("\t\t%d: %s\n",prog[i].szam,prog[i].szoveg);
			}
			kiir("\n\t\tMenupont: ");
#ifdef LINUX
			refresh();
#endif
			in=karakter();
			if ( in<'0' || in>=i+'0' ) {
				kiir("%c - Nem lehetseges!",in);
				in=-1;
#ifdef LINUX
	            refresh();
#endif
#ifndef ANSI
				Sleep(1000);
#endif
				} else in=in-'0';
		} while ( in<0 || in>=i );
		keptorl();
		prog[in].prog();
		kiir("Nyomj ENTER-t a tovabblepeshez!\n");
		karakter();
	} while (in);
#ifdef LINUX
    linux_restore();
#endif
}

tele* getT() {
	return t;
}

void saveSettings() {
	FILE* f;
	int l;
	f=fopen(path,"wb");
	l=getLevel();
	fwrite(&l,sizeof(int),1,f);
	l=getKc();
	fwrite(&l,sizeof(int),1,f);
	l=getTele();
	fwrite(&l,sizeof(int),1,f);
	l=getFal();
	fwrite(&l,sizeof(int),1,f);
	l=getBull();
	fwrite(&l,sizeof(int),1,f);
	l=getFug();
	fwrite(&l,sizeof(int),1,f);
	l=getMulti();
	fwrite(&l,sizeof(int),1,f);
	l=getSch();
	fwrite(&l,sizeof(int),1,f);
	fclose(f);
}

void loadSettings() {
	FILE* f;
	int l;
	setLevel(2);
	setKc(10);
	setTele(0);
	setFal(1);
	setBull(1);
	setFug(0);
	setMulti(1);
	setSch(1);
	f=fopen(path,"rb");
	if ( f) {
		if ( fread(&l,sizeof(int),1,f) )
			setLevel(l);
		if ( fread(&l,sizeof(int),1,f) )
			setKc(l);
		if ( fread(&l,sizeof(int),1,f) )
			setTele(l);
		if ( fread(&l,sizeof(int),1,f) )
			setFal(l);
		if ( fread(&l,sizeof(int),1,f) )
			setBull(l);
		if ( fread(&l,sizeof(int),1,f) )
			setFug(l);
		if ( fread(&l,sizeof(int),1,f) )
			setMulti(l);
		if ( fread(&l,sizeof(int),1,f) )
			setSch(l);
		fclose(f);
	}
}