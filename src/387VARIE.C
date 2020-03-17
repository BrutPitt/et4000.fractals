#define VARIE387

#include "b387.h"

void sierpinski(void);
int loadsavedfile(void);
void popcorn(void);
void tange(void);
int tangepix(void);
void Julia(void);
long double oldcr,oldcj;
void Lambda(void);
char tasto_premuto[5];
int searchfunz(char *);
int bound_trace_main(void);
int FpuMandelPixel(void);
int FpuBran(void);
int FpuJuliaPixel(void);
int FpuMandelPixelfn(void);
int FpuMandelEpsPixel(void);
int FpuJuliaEpsPixel(void);
int FpuJuliaPixelfn(void);
void FpuPixelize(void);
void FpuPopcorn(void);
void Popcorn(void);
char flag_rapidita=1;
void Mandel3(void);
void pp3(void);
void pp6(void);
void pp4(void);
extern char goflag;
extern char cvg[12];
extern char cvgval;

void FpuHenoninit(void);
void FpuHenonend(void);
void FpuHenon(void);

void FpuMartininit(void);
void FpuMartinend(void);
void FpuMartin(void);


void FpuPlankinit(void);
void FpuPlankend(void);

void FpuHopalonginit(void);
void FpuHopalongend(void);

void lyapf(void);


void inarea0(int,int,int,int,char *);
void outarea(int,int,int,int,char *);

void set50(void)
{
	sect_text();
	inreg.x.ax=0x1112;
	inreg.x.bx=0;
	int86(0x10,&inreg,&outreg);
}

void sect_text(void)
{

	inreg.x.ax=3;
	int86(0x10,&inreg,&outreg);
}


void conv_1(char *str)
{
	       cvgval=1;
	       if(toupper(str[0])=='R' && toupper(str[1])=='I') {
		  convergenza=converg_re_im;
		  strcpy(cvg,"Real & Imag");
	       }
	       else {
		  if(toupper(str[0])=='R') {
		     strcpy(cvg,"Real");
		     convergenza=converg_re;
		  }
		  else {
		     convergenza=converg_im;
		     strcpy(cvg,"Imag");
		  }
	       }
}

void conv_2(char *str)
{
	       cvgval=2;
	       if(toupper(str[0])=='R' && toupper(str[1])=='I') {
		  convergenza=converg_re_im_2;
		    strcpy(cvg,"Real & Imag");
	       }
	       else {
		  if(toupper(str[0])=='R') {
		       strcpy(cvg,"Real");
		     convergenza=converg_re_2;
		  }
		  else {
		     convergenza=converg_im_2;
		       strcpy(cvg,"Imag");
		  }
	       }
}

void conv_3(char *str)
{
	       cvgval=3;
	       if(toupper(str[0])=='R' && toupper(str[1])=='I') {
		  convergenza=converg_re_im_3;
		    strcpy(cvg,"Real & Imag");
	       }
	       else {
		  if(toupper(str[0])=='R') {
		      strcpy(cvg,"Real");
		     convergenza=converg_re_3;
		  }
		  else {
		    strcpy(cvg,"Imag");
		     convergenza=converg_im_3;
		  }
	       }
}


void fine(char *str)
{
	if(tono=='T')
	    fclose(bmp);
	else {
	   sect_text();
	}
	free(value);
	free(bex);
	free(dacbox);
	puts(str);
	exit(0);

}



void charont(char col,char rig)
{

	inreg.x.ax=0x200;
	inreg.h.dl=col;
	inreg.h.dh=rig;
	inreg.x.bx=0;
	int86(0x10,&inreg,&outreg);
}

void incharont(int *col,int *rig)
{

	inreg.x.ax=0x300;
	inreg.x.bx=0;
	int86(0x10,&inreg,&outreg);
	*col = outreg.h.dl;
	*rig = outreg.h.dh;
}


int scrivi(int dime,char *lbl)
{
int coli,rig;
register int j;
char first=0;
char s[2]={0,0};
int col,key;

	j=0;
	incharont(&coli,&rig);
	col=coli;
	charont(col,rig);
	for(;;) {
	   charont(col,rig);
	   if((key=getkey())>128)
	      continue;
	   s[0]=key;
	   switch(s[0]) {
	      case 0xd :
		 lbl[j]=0;
		 charont(col,rig);
		 putchar(' ');
		 return (1);
	      case 8 :
		 if(col>coli) {
		    j--;
		    col--;
		    charont(col,rig);
		    putchar(' ');

		 }
		 continue;
	      case 27 :
		 if(!first)
		    lbl[0]=0;
		 return(27);
	   }
	   if (j<dime) {
	      charont(col,rig);
	      putchar(s[0]);
	      lbl[j]=s[0];
	      j++;
	      col++;
	   }
	}
}




void setmandelval(void)
{
	  x_min=xmin=!kelcr ? -2 : cr-kelcr;
	  x_max=xmax=!kelcr ? 2 : cr+kelcr;
	  y_min=ymin=!kelcj ? -2 : cj-kelcj;
	  y_max=ymax=!kelcj ? 2 : cj+kelcj;
	  cr=oldcr;
	  cj=oldcj;
}


void setjuliaval(void)
{
     oldcj=cj;
     oldcr=cr;
     kelcr=(x_max-x_min)/2;
     kelcj=(y_max-y_min)/2;
     cr=x_max-kelcr;
     cj=y_max-kelcj;
     x_min=xmin=-2;
     x_max=xmax=2;
     y_min=ymin=-2;
     y_max=ymax=2;
}



int changemotion(void)
{

	if(flagSave)
	   control_gif(savepicture,1);
	if(--counterMotion<=0)
	   return(27);

	if(zinc!=1.0) {
	   x_min-=stx*((float)risoluzione_x)/(step*zinc);
	   x_max+=(stx*((float)risoluzione_x))/(step*zinc);
	   y_min-=sty*((float)risoluzione_y)/(step*zinc);
	   y_max+=(sty*((float)risoluzione_y))/(step*zinc);
	}

	x_min+=xmininc;
	y_min+=ymininc;
	x_max+=xmaxinc;
	y_max+=ymaxinc;
	xmin=x_min;
	xmax=x_max;
	ymin=y_min;
	ymax=y_max;
	cr+=crinc;
	cj+=cjinc;
	kostre+=kostreinc;
	kostim+=kostiminc;
	epsilon+=epsiloninc;
	bailout+=bailoutinc;
	aval+=avalinc;
	bval+=bvalinc;
	cval+=cvalinc;
	zoom+=zoominc;
	video();
	return(0);
}


int changemmotion(void)
{

	if(flagSave)
	   control_gif(savepicture,1);
	if(--counterMotion<=0)
	   return(27);

	if(zinc!=1.0) {
	   x_min+=stx*((float)risoluzione_x)/(step*zinc);
	   x_max-=(stx*((float)risoluzione_x))/(step*zinc);
	   y_min+=sty*((float)risoluzione_y)/(step*zinc);
	   y_max-=(sty*((float)risoluzione_y))/(step*zinc);
	}

	x_min-=xmininc;
	y_min-=ymininc;
	x_max-=xmaxinc;
	y_max-=ymaxinc;
	xmin=x_min;
	xmax=x_max;
	ymin=y_min;
	ymax=y_max;
	cr-=crinc;
	cj-=cjinc;
	kostre-=kostreinc;
	kostim-=kostiminc;
	epsilon-=epsiloninc;
	bailout-=bailoutinc;
	aval-=avalinc;
	bval-=bvalinc;
	cval-=cvalinc;
	zoom-=zoominc;
	video();
	return(0);
}







int selectparm(char *str,char flag)
{


	switch(toupper(str[0])) {
	    case 'M' :

	       mainfunx=flag_rapidita ? (void *)bound_trace_main : (void *)Mandel;
	       strcpy(tasto_premuto,str);

	       switch(atoi(&str[1])) {
		  case 1 :

		     strcpy(tasto_premuto,"M1");
		     funx=flag_rapidita ? (void *)FpuMandelPixel : (void *)FpuMandel;
		     conv_2(converg);
		     return(27);
		  case 2 :
		     strcpy(tasto_premuto,"M2");
		     funx=flag_rapidita ? (void *)FpuMandelEpsPixel :(void *)FpuMandelEps;
		     conv_3(converg);
		     return(27);
		  case 3 :

		     strcpy(tasto_premuto,"M3");
		     funx=flag_rapidita ? (void *)FpuMandelPixelfn : (void *)FpuMandelfn;
		     internalfunz=FpuMandelSin;
		     conv_1(converg);
		     return(27);
		  case 4 :

		     strcpy(tasto_premuto,"M4");
		     funx=flag_rapidita ? (void *)FpuMandelPixelfn : (void *)FpuMandelfn;
		     internalfunz=FpuMandelCos;
		     conv_1(converg);
		     return(27);
		  case 5 :

		     strcpy(tasto_premuto,"M5");
		     funx=flag_rapidita ? (void *)FpuMandelPixelfn : (void *)FpuMandelfn;
		     internalfunz=FpuMandelExp;
		     conv_1(converg);
		     return(27);
		  default :
		     return(27);
	       }

	    case 'I' :
	       switch(atoi(&str[1])) {
		  case 1 :
		     strcpy(tasto_premuto,"I1");
		     mainfunx=sierpinski;
		     return(27);
		  default :
		     return(27);
	       }
	    case 'F' :
	       switch(atoi(&str[1])) {
		  case 1 :
		     strcpy(tasto_premuto,"F1");
		     mainfunx=Mandel;
		     funx=FpuBran;
		     conv_1(converg);
		     return(27);
		  default :
		     return(27);
	       }
	    case 'J' :
	       strcpy(tasto_premuto,str);
	       mainfunx=flag_rapidita ? (void *)bound_trace_main : (void *)Mandel;
	       kelcr=0;
	       kelcj=0;


	       switch(atoi(&str[1])) {
		  case 1 :

		     strcpy(tasto_premuto,"J1");
		     funx=flag_rapidita ? (void *)FpuJuliaPixel : (void *)FpuJulia;
		     conv_2(converg);
		     return(27);
		  case 2 :

		     strcpy(tasto_premuto,"J2");
		     funx=flag_rapidita ? (void *)FpuJuliaEpsPixel : (void *)FpuJuliaEps;
		     conv_3(converg);
		     return(27);
		  case 3 :
		     strcpy(tasto_premuto,"J3");
		     funx=flag_rapidita ? (void *)FpuJuliaPixelfn : (void *)FpuJuliafn;
		     internalfunz=FpuJuliaSin;
		     conv_1(converg);
		     return(27);
		  case 4 :
		      strcpy(tasto_premuto,"J4");
		     funx=flag_rapidita ? (void *)FpuJuliaPixelfn : (void *)FpuJuliafn;
		     internalfunz=FpuJuliaCos;
		     conv_1(converg);
		     return(27);
		  case 5 :
		     strcpy(tasto_premuto,"J5");
		     funx=flag_rapidita ? (void *)FpuJuliaPixelfn : (void *)FpuJuliafn;
		     internalfunz=FpuJuliaExp;
		     conv_1(converg);
		     return(27);
		  default :
		     return(27);
	       }
	    case 'H' :

	       mainfunx=Hopalong;
	       funx=FpuPixelize;
	       internalpixelize=FpuHopalong;
	       initpixelize=FpuHopalonginit;
	       endpixelize=FpuHopalongend;
	       strcpy(tasto_premuto,str);

	       switch(atoi(&str[1])) {
		  case 1 :
		     addrHop=HopNull;
		     return(27);
		  case 2 :
		     addrHop=HopSin;
		     return(27);
		  case 3 :
		     addrHop=HopCos;
		     return(27);
		  case 4 :
		     addrHop=HopSqrt;
		     return(27);
		  case 5 :
		     addrHop=HopSinCos;
		     return(27);
		  default :
		     return(27);
	       }
	    case 'P' :
	       strcpy(tasto_premuto,str);
	       switch(str[1]) {
		  case '1' :

		    mainfunx=Hopalong;
		    funx=FpuPixelize;
		    internalpixelize=FpuPlank;
		    initpixelize=FpuPlankinit;
		    endpixelize=FpuPlankend;
		     return(27);
	       case '2' :

		     mainfunx=Popcorn;
		     funx=FpuPopcorn;
		     conv_1(converg);
		     return(27);
	       case '3' :
		     mainfunx=Hopalong;
		     funx=FpuPixelize;
		     internalpixelize=FpuHenon;
		     initpixelize=FpuHenoninit;
		     endpixelize=FpuHenonend;
		     return(27);
	       case '4' :
		     mainfunx=Hopalong;
		     funx=FpuPixelize;
		     internalpixelize=FpuMartin;
		     initpixelize=FpuMartininit;
		     endpixelize=FpuMartinend;
		     return(27);
	
		default :
		     return(27);
	       }
	    case 'B' :
	       strcpy(tasto_premuto,str);
	       mainfunx=Mandel;

	       switch(atoi(&str[1])) {
		  case 1 :
		    
		     conv_1(converg);
		     funx=FpuSprtSin2z;
		     return(27);
		  default :
		     return(27);
	       }
	    case 'Q':
	       if(!stricmp("Quit",str))
		  fine("B387 by Mik27.");
	       else
		  return(27);
	    case 'D':
	       switch(str[1]) {
		  case '1' :
		      outcoord();
		      return(27);
		  case '2' :
		      changeval();
		      return(27);
		  case '3' :
		     sect_text();
		     set50();
		     system("dir /w *.gif");
		     puts("\nPremi un tasto . . .");
		     getkey();
		     return(27);
		  case '4' :
		     return(loadsavedfile());
		  case '5' :
		     sect_text();
		     set50();
		     system("dir /w *.val");
		     puts("\nPremi un tasto . . .");
		     getkey();
		     return(27);
		  case '6' :
		      if(searchfunz(funztype)==27)
			   return(27);
		      get_single_fileval();
		      return(27);
		  case '7' :
		     outcoordfile(1);
		     return(27);
		  case '9' :
		     flagMotion=0;
		     return(0);
	       }
	    case 'S':
	       switch(str[1]) {
		  case '1' :
		     flagMotion=1;
		     if(getmotion()==27)
			return(27);
		     return(0);
		  case '2' :
		     sect_text();
		     set50();
		     if(load_f[0]) {
			if(spawnl(P_WAIT,editor,editor,load_f,NULL)==-1) {
			   boxerror("Mo memory Error !");
			}
		     }
		     else {
			if(spawnl(P_WAIT,editor,editor,"*.val",NULL)==-1) {
			   boxerror("Mo memory Error !");
			}
		     }
		     return(27);
		  case '3' :
		     sect_text();
		     set50();
		     puts("Type 'EXIT' to return to B387 !!");
		     system("");
		     return(27);
		  case '+' :
		     mainfunx=lyapf;
		     return(27);
		  default :
		     return(27);
	       }
	    default :
	       return(27);

	}
}




int loadsavedfile(void)
{
char cppp[10];
register t;

	   select_val(readf,"Tape New Picture  :");

	   if ((bmp=fopen(readf,"rb")) == NULL) {
	      boxerror("File non trovato !!! Premi un tasto . . .");
	      return(27);
	   }
	   fseek(bmp,-203L,SEEK_END);

	   fread(cppp,4,1,bmp);
	      if(strnicmp("B387",cppp,4)) {
		 boxerror("Non e' un file precedentemente salvato!!!");
		 return(27);
	      }
	      fread(tasto_premuto,3,1,bmp);
	      fread(funztype,13,1,bmp);
	      fread((unsigned char *)&xmin,sizeof(xmin),1,bmp);
	      fread((unsigned char *)&xmax,sizeof(xmax),1,bmp);
	      fread((unsigned char *)&ymin,sizeof(ymin),1,bmp);
	      fread((unsigned char *)&ymax,sizeof(ymax),1,bmp);
	      fread((unsigned char *)&aval,sizeof(aval),1,bmp);
	      fread((unsigned char *)&bval,sizeof(bval),1,bmp);
	      fread((unsigned char *)&cval,sizeof(cval),1,bmp);
	      fread((unsigned char *)&zoom,sizeof(zoom),1,bmp);
	      fread((unsigned char *)&cr,sizeof(cr),1,bmp);
	      fread((unsigned char *)&cj,sizeof(cj),1,bmp);
	      fread((unsigned char *)&epsilon,sizeof(epsilon),1,bmp);
	      fread((unsigned char *)&kostre,sizeof(kostre),1,bmp);
	      fread((unsigned char *)&kostim,sizeof(kostim),1,bmp);
	      fread((unsigned char *)&step,sizeof(step),1,bmp);
	      fread((unsigned char *)&bailout,sizeof(bailout),1,bmp);
	      fread((unsigned char *)&iteraz,sizeof(iteraz),1,bmp);
	      fread((unsigned char *)&vid,sizeof(vid),1,bmp);
	      fread((unsigned char *)converg,2,1,bmp);
	      fread((unsigned char *)palettef,sizeof(palettef),1,bmp);
	      fread((unsigned char *)load_f,sizeof(load_f),1,bmp);
	      fseek(bmp,0L,SEEK_SET);
	      control_gif(" ",0);
	      fclose(bmp);

	      x_min=xmin;
	      x_max=xmax;
	      y_min=ymin;
	      y_max=ymax;
	      FpuStep();
	      goflag=1;
	      selectparm(tasto_premuto,0);
	      while((t=tasto(getkey()))==1);
	      if(t==27)
		 return(27);
	      else
		 return(0);

}



int tasto(int kkey)
{
register int t,j;
char *boxed;
int tcx,tcy,bcx,bcy;

#ifdef __TURBOC__

struct ffblk find;

#else

struct find_t find;

#endif


	while(1) {
	switch(kkey) {

	   case 13 :
	      if(show_mouse_window(&tcx,&tcy,&bcx,&bcy)==27)
		 return(1);
	      x_min+=stx*tcx/step;
	      x_max-=(stx*(risoluzione_x-bcx))/step;
	      y_min+=sty*tcy/step;
	      y_max-=(sty*(risoluzione_y-bcy))/step;
	      xmin=x_min;
	      xmax=x_max;
	      ymin=y_min;
	      ymax=y_max;
	      video();
	      return(0);
	   case Home :
	      x_min-=stx*((float)risoluzione_x/3)/step;
	      x_max+=(stx*((float)risoluzione_x/3))/step;
	      y_min-=sty*((float)risoluzione_y/3)/step;
	      y_max+=(sty*((float)risoluzione_y/3))/step;
	      xmin=x_min;
	      xmax=x_max;
	      ymin=y_min;
	      ymax=y_max;
	      video();
	      return(0);

	   case 'S' :
	   case 's' :
	      control_gif(savepicture,1);

	      return(1);
	   case 'l' :
	   case 'L' :

	      while(1) {
	      if((boxed=calloc(10200,sizeof(char)))==NULL) {
		sect_text();
		boxerror("No Memory Error !!");
		return(0);
	      }
	      t=(risoluzione_x>>1)-138;
	      j=risoluzione_y-37;
	      inarea0(t,j,277,35,boxed);
	      graph_box(t+4,j+5,268,23,maxCol);
	      spectro();

#ifdef __TURBOC__

		 if(findfirst("*.gin",&find,0)) {
		    return(27);
		 }
		 strcpy(palettef,find.ff_name);
		 load_xtern(palettef);
		 set_modo_video(0,256,dacbox);
		 if(getkey()==27) {
		    outarea(t,j,277,35,boxed);
		    free(boxed);
		    return(1);
		 }
		 while( !findnext( &find ) ) {
		    strcpy(palettef,find.ff_name);
		    load_xtern(palettef);
		    set_modo_video(0,256,dacbox);
		    if(getkey()==27) {
		       outarea(t,j,277,35,boxed);
		       free(boxed);
		       return(1);
		    }
		 }

#else


		 if(_dos_findfirst("*.gin",_A_NORMAL|_A_ARCH,&find)) {
		     return(27);
		 }
		 strcpy(palettef,find.name);
		 load_xtern(palettef);
		 set_modo_video(0,256,dacbox);
		 if(getkey()==27) {
		    outarea(t,j,277,35,boxed);
		    free(boxed);
		    return(1);
		 }
		 while( !_dos_findnext( &find ) ) {
		    strcpy(palettef,find.name);
		    load_xtern(palettef);
		    set_modo_video(0,256,dacbox);
		    if(getkey()==27) {
		       outarea(t,j,277,35,boxed);
		       free(boxed);
		       return(1);
		    }
		 }

#endif
	      }
	   case 'w' :
	   case 'W' :
	      outcoordfile(0);
	      return(1);
	   case 9 :
	      sect_text();
	      outcoord();
	      video();
	      return(0);
	   case 'c' :
	   case 'C' :
	      sect_text();
	      changeval();
	      video();
	      return(0);
	   case 'i' :
	   case 'I' :
	      sect_text();
	      set50();
	      getmotion();
	      video();
	      return(0);
	   case '+' :
	      if(flagMotion)
		 if(changemotion()==27)
		    return(27);
	      return(0);
	   case '-' :
	      if(flagMotion)
		 if(changemmotion()==27)
		    return(27);
	      return(0);
	   case F5 :
	      rupt=0x13;
	      risoluzione_x=320;
	      risoluzione_y=200;
	      vid=320;
	      video();
	      return(0);
	   case F6 :
	      rupt=0x2e;
	      risoluzione_x=640;
	      risoluzione_y=480;
	      vid=640;
	      video();
	      return(0);
	   case F7 :
	      rupt=0x30;
	      risoluzione_x=800;
	      risoluzione_y=600;
	      vid=800;
	      video();
	      return(0);
	   case F8 :
	      rupt=0x38;
	      risoluzione_x=1024;
	      risoluzione_y=768;
	      vid=1024;
	      video();
	      return(0);
	   case F10 :
	      if((boxed=calloc(10200,sizeof(char)))==NULL) {
		sect_text();
		boxerror("No Memory Error !!");
		return(0);
	      }
	      t=(risoluzione_x>>1)-138;
	      j=risoluzione_y-37;
	      inarea0(t,j,277,35,boxed);
	      graph_box(t+4,j+5,268,23,maxCol);
	      spectro();
	      getkey();
	      outarea(t,j,277,35,boxed);
	      free(boxed);
	      return(1);
	   case '1' :
	   case '2' :
	   case '3' :
	   case '4' :
	   case '5' :
	   case '6' :
	   case '7' :
	   case '8' :
	      step=kkey-'0';
	      video();
	      return(0);
	   case F1 :
	   case 'h':
	   case 'H':
	      sect_text();
	      hotkeyhelp();
	      video();
	      return(0);
	   case ' ' :
	      flag_rapidita^=1;
	      selectparm(tasto_premuto,0);
	      xmin=x_min;
	      xmax=x_max;
	      ymin=y_min;
	      ymax=y_max;
	      video();
	      return(0);
	   case 27 :
	      return(27);
	   default :
	      kkey=getkey();
	}

	}
}


int load_xtern(char *name_map)
{
register int t,trecolori;
int tot_colori=0;
FILE *file_map;
char test[6];


	if((file_map=fopen(name_map,"rb"))==NULL) {
	   get_modo_video(0,256,dacbox);
	   return(27);
	}
	fread(test,5,1,file_map);
	if(strncmp("Mik27",test,5)) {
	   fclose(file_map);
	   get_modo_video(0,256,dacbox);
	   min_max();
	   return(27);
	}
	fread(&tot_colori,2,1,file_map);
	fseek(file_map,13L,SEEK_CUR);
	trecolori=tot_colori*3;
	fread(dacbox,trecolori,1,file_map);
	for(t=0;t<trecolori;t++)
	    dacbox[t]>>=2;
	fclose(file_map);
	min_max();
	return(0);

}

int getkey()
{
int key,lo,hi;

#ifdef __TURBOC__

	key = bioskey(0);
#else

	key = _bios_keybrd(0);

#endif
        lo = key & 0x00ff;
        hi = (key & 0xff00) >> 8;
        return((lo == 0) ? hi + 256 : lo);
}


void set_color(void)
{
	   inreg.x.ax=rupt;
	   int86(0x10,&inreg,&outreg);
	   set_modo_video(0,256,dacbox);
}


void interpol(int k)
{
register int t,j;
long double coeff;
long double intra;
long double pip;

	for(t=0;t<risoluzione_x;t+=step) {
	   coeff=(long double)((int)bex[t+step]-(int)bex[t])/(double)step;
	   for(j=t+1;j<t+step;j++) {
	      pip=(int)bex[j-1]+coeff;
	      if(pip<0)
		 bex[j]=0;
	      else {
		 if(pip>0xff)
		    bex[j]=0xff;
		 else
		    bex[j]=(unsigned char)pip;
	      }
	   }
	}
	if(k) {
	   for(t=0;t<risoluzione_x;t++) {
	      intra=inpixel(t,k-step);
	      coeff=(long double)((int)bex[t]-intra)/(double)step;
	      for(j=(k-step)+1;j<k;j++) {
		 intra+=coeff;
		 if(intra<0)
		    outonepixel(t,j,0);
		 else {
		    if(pip>0xff)
		       outonepixel(t,j,0xff);
		    else
		       outonepixel(t,j,(int)intra);
		 }
	      }
	   }
	}


}

int searchfunz(char *ftype)
{
register int t=0,j;
char str[30];

	while((j=stricmp(ftype,value[t].fractal_type)) && t<NUMBER_OF_FRACTALS)
	   t++;
	if(j) {
	    boxerror("Tipo di frattale non inserito nel System.387 !");
	    return(27);
	}
	else {
	    strcpy(save_f,value[t].save_f_file);
	    strcpy(load_f,value[t].load_f_file);
	}
	
	select_val(load_f,"Tape New DataName :");
	strcpy(value[t].load_f_file,load_f);

}






