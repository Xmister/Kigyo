//#define DEBUG 1
#define X 60
#define Y 20
#define KJ '@'
typedef struct{int pX; int pY;} pos;
#ifdef irany
#else
enum irany{le='k',fel='i',balra='j',jobbra='l',le2='s',fel2='w',balra2='a',jobbra2='d'};
#endif
#ifdef kigyo
#else
typedef struct kigyo_{int id;int li;int pont;pos p;char c;char j;int f;int bullet;int usedb;int activeb;int stopped;int s;struct kigyo_* k; struct kigyo_* kprev;} kigyo;
#endif
#ifdef score
#else
typedef struct {char nev[10]; int sco;} score;
#endif
#ifdef scorelist
#else
typedef struct scorelist_ {score sc; struct scorelist_* next;} scorelist;
#endif
#ifdef menu
#else
typedef struct {int szam; int (*prog)();char szoveg[100];} menu;
#endif
typedef int (*kiir_)(const char* format,...);
typedef struct {int id; pos p1; pos p2;} tele;
void initPrint();
tele* getT();
kiir_ kiir;
kiir_ beolv;