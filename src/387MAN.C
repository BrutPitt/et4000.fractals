#define MAN387

#include "b387.h"

#define DIMBUF 20000



void get_single_fileval(void)
{
char str[30];



	       xmin=-3;
	       xmax=3;
	       ymin=-3;
	       ymax=3;
	       aval=1;
	       cval=1;
	       bval=1;
	       zoom=1;
	       iteraz=150;
	       cr=0;
	       cj=0;
	       kostre=1;
	       kostim=1;
	       step=1;
	       epsilon=0.01;
	       bailout=4;
	       step=1;
	       vid=1024;
	       strcpy(savepicture,"b387_0");
	       strcpy(palettef,"rainbow.gin");

	total=0;
	if((list=fopen(load_f,"rb"))==NULL) {
	   fclose(list);
	   boxerror("Non trovo la lista dei valori !");
	   return;
	}
	if((buffer=calloc(DIMBUF,sizeof(char)))==NULL) {
	   boxerror("No Memory Error !!");
	}
	bpp=buffer;
	fread(buffer,DIMBUF,1,list);
	fclose(list);
	while(strnicmp("End",bpp,3) && total < DIMBUF-5000 ) {
	   if(*bpp!=' ' && *bpp!=';' && *bpp!=9 && *bpp!=10 && *bpp!=13 && *bpp) {
	      if(!strnicmp("Xmin",bpp,4)) {
		 getstrval(str);
		 xmin=atof(str);
		 continue;
	      }
	      if(!strnicmp("Xmax",bpp,4)) {
		 getstrval(str);
		 xmax=atof(str);
		 continue;
	      }
	      if(!strnicmp("Ymin",bpp,4)) {
		 getstrval(str);
		 ymin=atof(str);
		 continue;
	      }
	      if(!strnicmp("Ymax",bpp,4)) {
		 getstrval(str);
		 ymax=atof(str);
		 continue;
	      }
	      if(!strnicmp("Aval",bpp,4)) {
		 getstrval(str);
		 aval=atof(str);
		 continue;
	      }
	      if(!strnicmp("Bval",bpp,4)) {
		 getstrval(str);
		 bval=atof(str);
		 continue;
	      }
	      if(!strnicmp("Cval",bpp,4)) {
		 getstrval(str);
		 cval=atof(str);
		 continue;
	      }
	      if(!strnicmp("Dval",bpp,4)) {
		 getstrval(str);
		 dval=atof(str);
		 continue;
	      }
	      if(!strnicmp("Zoom",bpp,4)) {
		 getstrval(str);
		 zoom=atof(str);
		 continue;
	      }
	      if(!strnicmp("RealCost",bpp,8)) {
		 getstrval(str);
		 cr=atof(str);
		 continue;
	      }
	      if(!strnicmp("ImagCost",bpp,8)) {
		 getstrval(str);
		 cj=atof(str);
		 continue;
	      }
	      if(!strnicmp("KostanteRe",bpp,10)) {
		 getstrval(str);
		 kostre=atof(str);
		 continue;
	      }
	      if(!strnicmp("KostanteIm",bpp,10)) {
		 getstrval(str);
		 kostim=atof(str);
		 continue;
	      }
	      if(!strnicmp("Epsilon",bpp,7)) {
		 getstrval(str);
		 epsilon=atof(str);
		 continue;
	      }
	      if(!strnicmp("Bailout",bpp,7)) {
		 getstrval(str);
		 bailout=atoi(str);
		 continue;
	      }
	      if(!strnicmp("ConvergenzaSu",bpp,7)) {
		 getstrval(str);
		 strcpy(converg,str);
		 continue;
	      }
	      if(!strnicmp("N.Iterazioni",bpp,12)) {
		 getstrval(str);
		 iteraz=atol(str);
		 continue;
	      }
	      if(!strnicmp("Ris.Video",bpp,9)) {
		 getstrval(str);
		 vid=atoi(str);
		 continue;
	      }
	      if(!strnicmp("Interpolazione",bpp,14)) {
		 getstrval(str);
		 step=atoi(str);
		 continue;
	      }
	      if(!strnicmp("Palette",bpp,7)) {
		 getstrval(str);
		 strcpy(palettef,str);
		 continue;
	      }
	      if(!strnicmp("SavePicture",bpp,8)) {
		 getstrval(str);
		 strcpy(savepicture,str);
		 continue;
	      }
	      boxerror("Variabile o direttiva mancante in DATAFILE !!");
	      free(buffer);
	      return;
	   }
	   else
	      fineriga();
	}
	x_min=xmin;
	x_max=xmax;
	y_min=ymin;
	y_max=ymax;
	free(buffer);
}

void Mandel(void)
{
register int j,t;

	   while(1) {
	      t=1;
	      xmin=x_min;
	      xmax=x_max;
	      ymin=y_min;
	      ymax=y_max;
	      FpuStep();
	      oldxmin=xmin;
	      for(j=0;j<risoluzione_y;j+=step) {
		 funx();
		 if(step!=1)
		    interpol(j);
		 if(kbhit()) {
		     if((t=tasto(getkey()))==27)
			return;
		     else
			if(!t) {
			   mainfunx();
			   return;
			}

		 }
		 outarray(0,j,risoluzione_x,bex);
	      }
	      if(!flagMotion) {
		 while((t=tasto(0))==1);
		 if(t==27)
		    return;
	      }
	      else {
		 if(changemotion()==27)
		    return;
	      }
	   }
}

