#include <math.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <process.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#ifdef __TURBOC__
#include <dir.h>
#endif
#include "mymath.h"

#define Home 327
#define End 335
#define PgUp 329
#define PgDn 337
#define UpKey 328
#define DwKey 336
#define ESC 27
#define INSKEY 338
#define LfKey 331
#define RgKey 333
#define F1 315
#define F2 316
#define F3 317
#define F4 318
#define F5 319
#define F6 320
#define F7 321
#define F8 322
#define F9 323
#define F10 324

#define DIMBUF 20000
#define NUMBER_OF_FRACTALS 50


void FpuSprtSin2z(void);
void FpuMandel(void);
void FpuJulia(void);
void FpuJuliaEps(void);
void FpuMandelEps(void);
void FpuStep(void);
void FpuDif(void);
void FpuJuliaCos(void);
void FpuMandelExp(void);
void FpuJuliaExp(void);
void FpuJuliaSin(void);
void FpuMandelCos(void);
void FpuMandelSin(void);

void FpuJuliafn(void);
void FpuMandelfn(void);

void converg_re_im(void);
void converg_re(void);
void converg_im(void);
void converg_re_im_2(void);
void converg_re_2(void);
void converg_im_2(void);
void converg_re_im_3(void);
void converg_re_3(void);
void converg_im_3(void);


int FpuPlank(void);
int FpuHopalong(void);


double FpuCos(double);
double FpuCosh(double);
double FpuSin(double);
double FpuSinh(double);
double FpuTan(double);

void HopNull(void);
void HopSin(void);
void HopCos(void);
void HopSinCos(void);
void HopSqrt(void);

void Julia(void);
void Mandel(void);
void Hopalong(void);
void plank(void);
void Jcos(void);
void Jsin(void);


void tseng4(void);
void tseng4c(void);
void tseng(void);
void tsengc(void);

void outonepixel(int,int,unsigned char);
unsigned char inpixel(int,int);
void outarray(int,int,int,char *);
void inarray(int,int,int,char *);
void set_modo_video(int,int,char *);

void sect_text(void);
void set_color(void);

int tasto(int);

int apre(char *,char *);




#ifdef BIOM

char cvg[12];
char cvgval=0;

struct VALUE {

char fractal_type[15];
char load_f_file[30];
char save_f_file[30];

} *value;

float zinc=1;

int counterMotion=1;
char flagMotion=0;

char save_f[30],load_f[30];
char savepicture[30];
char filename[30];
char plotfunz[30];
char palettef[30];
char editor[30];
char funztype[20];
char readf[60];
char calleditor[60];
char *buffer,*bpp;



void *addrTseng,*addrTsengc;
void *addrHop;
int (* funx)(void);
void (* mainfunx)(void);
void (* select)(void);
void (* internalpixelize)(void);
void (* initpixelize)(void);
void (* endpixelize)(void);

void *internalfunz;
void *convergenza;

char converg[3];

unsigned _stklen=20000;

int maxCol=255,minCol=0;
unsigned char *dacbox;
int risoluzione_x,risoluzione_y;

double epsilon;

double absymin,absxmin,rate_x,rate_y;
double epsiloninc=0;
double xmininc=0,ymininc=0,xmaxinc=0,ymaxinc=0;
double crinc=0,cjinc=0,avalinc=0,bvalinc=0,cvalinc=0,dvalinc=0,zoominc=0;
double kostreinc=0,kostiminc=0;
int bailoutinc=0;

long double xmin,ymin,xmax,ymax;
long double oldxmin,stx,sty,trsfx,trsfy,kelcr=0,kelcj=0;
long double dif_x,dif_y,x_min,y_min,x_max,y_max;

double MY_OVERFLOWD=1.0e+300;


double cr,cj,aval,bval,cval,dval,zoom;
double kostre,kostim;

union REGS inreg,outreg;

char tono;
int colt,vid,rupt;
long iteraz;
int bailout;
int contfile=0;
long total=0;

unsigned char *bex,*oldbex;
unsigned char step;

FILE *bmp;
FILE *list;

int ctcx,ctcy,rvbutton,rvcx,rvcy;
char flagmouse;
int col,row,color;

#endif


#ifdef MENU387

extern float zinc;
extern char flagSave;

extern int counterMotion;
extern char flagMotion;


extern struct VALUE {

char fractal_type[15];
char load_f_file[30];
char save_f_file[30];

} *value;



extern double epsiloninc;
extern double xmininc,ymininc,xmaxinc,ymaxinc;
extern double crinc,cjinc,avalinc,bvalinc,cvalinc,dvalinc,zoominc;
extern double kostreinc,kostiminc;
extern int bailoutinc;



extern char save_f[30],load_f[30];
extern char savepicture[30];


extern union REGS inreg,outreg;

extern FILE *bmp;
extern FILE *list;
extern char filename[30];
extern char plotfunz[30];
extern char palettef[30];
extern char funztype[20];
extern char editor[30];
extern char readf[60];
extern char calleditor[60];
extern char savepicture[30];
extern char *buffer,*bpp;

extern double cr,cj,epsilon;
extern long double xmin,ymin,xmax,ymax;
extern long double oldxmin,stx,sty,trsfx,trsfy,kelcr,kelcj;
extern long double dif_x,dif_y,x_min,y_min,x_max,y_max;


extern int risoluzione_x,risoluzione_y;

extern char tono;
extern int colt,vid,rupt;
extern long iteraz;
extern bailout;
extern int contfile;

extern void *addrTseng,*addrTsengc;
extern void *addrHop;
extern int (* funx)(void);
extern void (* mainfunx)(void);
extern void (* select)(void);
extern void *internalfunz;
extern void *convergenza;
extern char converg[3];

extern double aval,bval,cval,dval,zoom;
extern double kostre,kostim;

extern unsigned char *bex;
extern unsigned char step;
extern unsigned char *dacbox;
extern int maxCol,minCol;


#endif



#ifdef VARIE387

extern void (* internalpixelize)(void);
extern void (* initpixelize)(void);
extern void (* endpixelize)(void);

extern float zinc;
char flagSave=1;

extern int counterMotion;
extern char flagMotion;


extern struct VALUE {

char fractal_type[15];
char load_f_file[30];
char save_f_file[30];

} *value;



extern double epsiloninc;
extern double xmininc,ymininc,xmaxinc,ymaxinc;
extern double crinc,cjinc,avalinc,bvalinc,cvalinc,dvalinc,zoominc;
extern double kostreinc,kostiminc;
extern int bailoutinc;



extern char save_f[30],load_f[30];
extern char savepicture[30];


extern union REGS inreg,outreg;

extern FILE *bmp;
extern FILE *list;
extern char filename[30];
extern char plotfunz[30];
extern char palettef[30];
extern char funztype[20];
extern char editor[30];
extern char readf[60];
extern char calleditor[60];
extern char savepicture[30];
extern char *buffer,*bpp;

extern double cr,cj,epsilon;
extern long double xmin,ymin,xmax,ymax;
extern long double oldxmin,stx,sty,trsfx,trsfy,kelcr,kelcj;
extern long double dif_x,dif_y,x_min,y_min,x_max,y_max;


extern int risoluzione_x,risoluzione_y;

extern char tono;
extern int colt,vid,rupt;
extern long iteraz;
extern bailout;
extern int contfile;

extern void *addrTseng,*addrTsengc;
extern void *addrHop;
extern int (* funx)(void);
extern void (* mainfunx)(void);
extern void (* select)(void);
extern void *internalfunz;
extern void *convergenza;
extern char converg[3];

extern double aval,bval,cval,dval,zoom;
extern double kostre,kostim;

extern unsigned char *bex;
extern unsigned char step;
extern unsigned char *dacbox;
extern int maxCol,minCol;


#endif



#ifdef MAN387

extern double epsiloninc;
extern double xmininc,ymininc,xmaxinc,ymaxinc;
extern double crinc,cjinc,avalinc,bvalinc,cvalinc,dvalinc,zoominc;
extern double kostreinc,kostiminc;
extern int bailoutinc;

extern int counterMotion;
extern char flagMotion;


extern struct VALUE {

char fractal_type[15];
char load_f_file[30];
char save_f_file[30];

} *value;

extern char save_f[30],load_f[30];
extern char savepicture[30];
extern double aval,bval,cval,dval,zoom;
extern double kostre,kostim;
extern long total;


extern void *addrTseng,*addrTsengc;
extern void *addrHop;
extern int (* funx)(void);
extern void (* mainfunx)(void);


extern unsigned char *bex;
extern unsigned char step;


extern FILE *bmp;
extern FILE *list;
extern char filename[30];
extern char plotfunz[30];
extern char palettef[30];
extern char funztype[20];
extern char converg[3];
extern char readf[60];
extern char *buffer,*bpp;

extern double cr,cj,epsilon;
extern long double xmin,ymin,xmax,ymax;
extern long double oldxmin,stx,sty,trsfx,trsfy;
extern long double dif_x,dif_y,x_min,y_min,x_max,y_max;
extern double kostre,kostim;

extern int risoluzione_x,risoluzione_y;

extern char tono;
extern int colt,vid,rupt;
extern long iteraz;
extern int contfile;
extern bailout;
extern int contfile;


#endif


#ifdef JUL387

extern struct VALUE {

char fractal_type[15];
char load_f_file[30];
char save_f_file[30];

} *value;


extern void *addrTseng,*addrTsengc;
extern void *addrHop;
extern int (* funx)(void);
extern void (* mainfunx)(void);
extern void (* select)(void);


extern FILE *bmp;
extern FILE *list;
extern char filename[30];
extern char plotfunz[30];
extern char palettef[30];
extern char funztype[20];
extern char readf[60];
extern char *buffer,*bpp;

extern long double xmin,ymin,xmax,ymax;
extern long double oldxmin,stx,sty,trsfx,trsfy;
extern long double dif_x,dif_y,x_min,y_min,x_max,y_max;
extern double cr,cj,kostre,kostim;

extern int risoluzione_x,risoluzione_y;

extern char tono;
extern int colt,vid,rupt;
extern long iteraz;
extern int contfile;

extern unsigned char *bex;
extern unsigned char step;


#endif


#ifdef HOP387

extern int (* HPfunx)(double,double,double,double,double,double,double);
extern void *addrTseng,*addrTsengc;
extern void *addrHop;
extern int (* funx)(void);
extern void (* mainfunx)(void);

extern FILE *bmp;
extern FILE *list;
extern char filename[30];
extern char plotfunz[30];
extern char palettef[30];
extern char funztype[20];
extern char readf[60];
extern char *buffer,*bpp;

extern double cr,cj,epsilon;
extern long double xmin,ymin,xmax,ymax;
extern long double oldxmin,stx,sty,trsfx,trsfy;
extern long double dif_x,dif_y,x_min,y_min,x_max,y_max;

extern int risoluzione_x,risoluzione_y;

extern char tono;
extern int colt,vid,rupt;
extern long iteraz;
extern int contfile;

extern double aval,bval,cval,dval,zoom;

#endif

#ifdef BIOM387

extern double absymin,absxmin,rate_x,rate_y;
extern void *addrTseng,*addrTsengc;
extern void *addrHop;
extern int (* funx)(void);
extern void (* mainfunx)(void);


extern FILE *bmp;
extern FILE *list;
extern char filename[30];
extern char plotfunz[30];
extern char funztype[20];
extern char palettef[30];
extern char readf[60];
extern char *buffer,*bpp;

extern double cr,cj,epsilon;
extern long double xmin,ymin,xmax,ymax;
extern long double oldxmin,stx,sty,trsfx,trsfy;
extern long double dif_x,dif_y,x_min,y_min,x_max,y_max;
extern double kostre,kostim;

extern int risoluzione_x,risoluzione_y;

extern char tono;
extern int colt,vid,rupt;
extern long iteraz;
extern int contfile;

#endif



#ifdef MOUSE387

extern int ctcx,ctcy,rvbutton;

extern union REGS inreg,outreg;

extern FILE *bmp;
extern FILE *list;
extern char filename[30];
extern char plotfunz[30];
extern char palettef[30];
extern char funztype[20];
extern char editor[30];
extern char readf[60];
extern char calleditor[60];
extern char *buffer,*bpp;

extern double cr,cj,epsilon;
extern long double xmin,ymin,xmax,ymax;
extern long double oldxmin,stx,sty,trsfx,trsfy,kelcr,kelcj;
extern long double dif_x,dif_y,x_min,y_min,x_max,y_max;

extern int risoluzione_x,risoluzione_y;
extern int maxCol,minCol;
extern unsigned char *dacbox;

extern char tono;
extern int colt,vid,rupt;
extern long iteraz;
extern bailout;
extern int contfile;

extern void *addrTseng,*addrTsengc;
extern void *addrHop;
extern int (* funx)(void);
extern void (* mainfunx)(void);
extern void (* select)(void);
extern void *internalfunz;
extern void *convergenza;


extern double aval,bval,cval,dval,zoom;
extern double kostre,kostim;

extern unsigned char *bex;
extern unsigned char step;
extern unsigned char *dacbox;
extern char flagmouse;

#endif


#ifdef BTM387

extern int counterMotion;
extern char flagMotion;


extern double epsiloninc;
extern double xmininc,ymininc,xmaxinc,ymaxinc;
extern double crinc,cjinc,avalinc,bvalinc,cvalinc,dvalinc,zoominc;
extern double kostreinc,kostiminc;
extern int bailoutinc;


extern struct VALUE {

char fractal_type[15];
char load_f_file[30];
char save_f_file[30];

} *value;

extern char save_f[30],load_f[30];
extern char savepicture[30];
extern double aval,bval,cval,dval,zoom;
extern double kostre,kostim;
extern long total;


extern void *addrTseng,*addrTsengc;
extern void *addrHop;
extern int (* funx)(void);
extern void (* mainfunx)(void);


extern unsigned char *bex;
extern unsigned char step;


extern FILE *bmp;
extern FILE *list;
extern char filename[30];
extern char plotfunz[30];
extern char palettef[30];
extern char funztype[20];
extern char converg[3];
extern char readf[60];
extern char *buffer,*bpp;

extern double cr,cj,epsilon;
extern long double xmin,ymin,xmax,ymax;
extern long double oldxmin,stx,sty,trsfx,trsfy;
extern long double dif_x,dif_y,x_min,y_min,x_max,y_max;
extern double kostre,kostim;

extern int risoluzione_x,risoluzione_y;

extern char tono;
extern int colt,vid,rupt;
extern long iteraz;
extern int contfile;
extern bailout;
extern int contfile;


#endif


#ifdef GIF387

extern double cr,cj,epsilon;
extern long double xmin,ymin,xmax,ymax;
extern long double oldxmin,stx,sty,trsfx,trsfy;
extern long double dif_x,dif_y,x_min,y_min,x_max,y_max;
extern double kostre,kostim;

extern char funztype[20];
extern char palettef[30];

extern char tono;
extern int colt,vid,rupt;
extern long iteraz;
extern int contfile;
extern bailout;
extern int contfile;
extern unsigned char step;
extern void *convergenza;
extern char converg[3];
extern double aval,bval,cval,dval,zoom;
extern char save_f[30],load_f[30];

#endif
