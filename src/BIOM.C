#define BIOM

#include "b387.h"

#define NEWVAL "\nNew val ---> "

int cpu(void);



void outcoordfile(char flag)
{
	if(flag) {
	   select_val(save_f,"Tape New DataName :");
	}
	apre(save_f,".val");
	fprintf(bmp,"\r\n;B387.exe by Michele Morrone.\r\n\r\n");
	   fprintf(bmp,"\;nType     %s\r\n\r\n",funztype);
	   fprintf(bmp,"\r\nXmin                          %4.18lf",(double)x_min);
	   fprintf(bmp,"\r\nXmax                          %4.18lf",(double)x_max);
	   fprintf(bmp,"\r\nYmin                          %4.18lf",(double)y_min);
	   fprintf(bmp,"\r\nYmax                          %4.18lf",(double)y_max);
	   fprintf(bmp,"\r\nRealCost                      %3.18lf",(double)cr);
	   fprintf(bmp,"\r\nImagCost                      %3.18lf",(double)cj);
	   fprintf(bmp,"\r\nKostanteRe                    %3.18lf",kostre);
	   fprintf(bmp,"\r\nKostanteIm                    %3.18lf",kostim);
	   fprintf(bmp,"\r\nEpsilon                       %3.18lf",epsilon);
	   fprintf(bmp,"\r\nBailout                       %d",bailout);
	   fprintf(bmp,"\r\nInterpolazione                %d",step);
	   fprintf(bmp,"\r\nAval                          %4.18lf",(double)aval);
	   fprintf(bmp,"\r\nBval                          %4.18lf",(double)bval);
	   fprintf(bmp,"\r\nCval                          %4.18lf",(double)cval);
	   fprintf(bmp,"\r\nN.Iterazioni                  %ld",iteraz);
	   if(!converg[0])
	      strcpy(converg,"RI");
	   fprintf(bmp,"\r\nConvergenzaSu                 %s",converg);
	   fprintf(bmp,"\r\nRis.Video                     %d",vid);
	   if(!palettef[0])
	      strcpy(converg,"rainbow.gin");
	   fprintf(bmp,"\r\nPalette                       %s",palettef);
	   if(!savepicture[0])
	      strcpy(savepicture,"N0");
	   fprintf(bmp,"\r\nSavePicture                   %s",savepicture);
	   fprintf(bmp,"\r\n\r\nEnd");
	fclose(bmp);

}


int apre(char *tfile,char *extenz)
{
char ff[20],num[5];

	strcpy(ff,tfile);
	itoa(contfile++,num,10);
	strcat(ff,num);
	strcat(ff,extenz);
	if((bmp=fopen(ff,"wb"))==NULL) {
	   boxerror("Errore in apertura file!!");
	   return(27);
	}
	return(0);
}


void video(void)
{

	switch(vid) {
	   case 640 :
	      rupt=0x2e;
	      risoluzione_x=640;
	      risoluzione_y=480;
	      break;
	   case 800 :
	      rupt=0x30;
	      risoluzione_x=800;
	      risoluzione_y=600;
	      break;
	   case 1024 :
	      rupt=0x38;
	      risoluzione_x=1024;
	      risoluzione_y=768;
	      break;
	   default :
	      rupt=0x13;
	      risoluzione_x=320;
	      risoluzione_y=200;
	      break;


	}
	tono=0;
	if(tono!='T') {
	   set_color();
	}
	else
	   fwrite(dacbox,768,1,bmp);

}




void fineriga(void)
{
char crlf[3];

	crlf[0]=13;
	crlf[1]=10;
	crlf[2]=0;
	while(strncmp(crlf,bpp,2)) {
	  total++;
	  *bpp++;
	}
	total+=2;
	*bpp++;
	*bpp++;
}


void getstrval(char *str)
{
register int t=0,j;

	for(j=0;j<30;j++)
	   if(*bpp++==9) {
	      j+=8;
	      total+=8;
	      bpp=&bpp[8];
	   }
	do {
	   str[t++]=*bpp++;
	   total++;
	} while(*bpp!=' ' && t<30 && *bpp!=10 && *bpp!=13 && *bpp!=9 && *bpp);
	str[t]=0;
}

void systemval(void)
{
char str[30];

	while(strnicmp("End",bpp,3)) {
	   if(*bpp!=' ' && *bpp!=';' && *bpp!=9 && *bpp!=10 && *bpp!=13 && *bpp) {
	      if(!strnicmp("Editor",bpp,4)) {
		 getstrval(str);
		 strcpy(editor,str);
		 continue;
	      }
	   }
	   else
	      fineriga();
	}
}

void getfileval(char *load_param_file,char *save_param_file)
{
char str[30];

	while(strnicmp("End",bpp,3)) {
	   if(*bpp!=' ' && *bpp!=';' && *bpp!=9 && *bpp!=10 && *bpp!=13 && *bpp) {
	      if(!strnicmp("DataFile",bpp,8)) {
		 getstrval(str);
		 strcpy(load_param_file,str);
		 continue;
	      }
	      if(!strnicmp("SaveFile",bpp,8)) {
		 getstrval(str);
		 strcpy(save_param_file,str);
		 continue;
	      }
	      boxerror("Variabile o direttiva mancante in DATAFILE !!");
	      return;
	   }
	   else
	      fineriga();
	}
}






void funzione(void)
{
register int t=0;
int kk,jj;
char str[30];

	total=0;
	if((buffer=calloc(DIMBUF,sizeof(char)))==NULL) {
	   boxerror("No Memory Error !!");
	   return;
	}
	bpp=buffer;
	if((list=fopen("system.val","rb"))==NULL) {
	   boxerror("Non trovo la lista dei valori !!!!");
	   return;
	}
	fread(buffer,DIMBUF,1,list);
	fclose(list);
	while(total<DIMBUF-5000) {
		jj=strnicmp("Type",bpp,4);
		kk=strnicmp("EndAll",bpp,6);
		if(!kk) {
		   free(buffer);
		   buffer=0L;
		   return;
		}
		if(jj) {
		   fineriga();
		   continue;
		}
		getstrval(str);
		if(!stricmp("System",str)) {
		    systemval();
		    strcpy(calleditor,editor);
		    strcat(calleditor," system.387");
		    continue;
		}
		if(!stricmp("Mandel",str) || !stricmp("MandelSin",str) || !stricmp("MandelCos",str) ||
		   !stricmp("MandelExp",str) || !stricmp("MandelEps",str)) {
		    strcpy(value[t].fractal_type,str);
		    getfileval(value[t].load_f_file,value[t].save_f_file);
		    t++;
		    continue;
		}
		if(!stricmp("Barnsley329",str)) {
		    strcpy(value[t].fractal_type,str);
		    getfileval(value[t].load_f_file,value[t].save_f_file);
		    t++;
		    continue;
		}
		if(!stricmp("Sierpinski",str)) {
		    strcpy(value[t].fractal_type,str);
		    getfileval(value[t].load_f_file,value[t].save_f_file);
		    t++;
		    continue;
		}
		if(!stricmp("Julia",str) || !stricmp("JuliaSin",str) || !stricmp("JuliaCos",str) ||
		   !stricmp("JuliaExp",str) || !stricmp("JuliaEps",str)) {
		    strcpy(value[t].fractal_type,str);
		    getfileval(value[t].load_f_file,value[t].save_f_file);
		    t++;
		    continue;
		}
		if(!stricmp("Hopalong",str) || !stricmp("HopSin",str) || !stricmp("HopCos",str) ||
		   !stricmp("HopSqrt",str) || !stricmp("HopSinCos",str)) {
		    strcpy(value[t].fractal_type,str);
		    getfileval(value[t].load_f_file,value[t].save_f_file);
		    t++;
		    continue;
		}
		if(!stricmp("Biomorph",str) || !stricmp("Plankton",str) || !stricmp("Popcorn",str) ||
		   !stricmp("Henon",str) || !stricmp("Martin",str)) {
		    strcpy(value[t].fractal_type,str);
		    getfileval(value[t].load_f_file,value[t].save_f_file);
		    t++;
		    continue;
		}
	}
	free(buffer);
	buffer=0L;
	return;


}

extern char goflag;
void main(int argc, char **argv)
{
register int t,j;
int keyts,kk;


	if(!(keyts=testTSENG())) {
	   puts("ATTENZIONE !!! No TSENG compatibility !!!");
	   puts("Il programma potrebbe non girare correttamente nelle modalita' diverse");
	   puts("dalla modalita' 320x200.  Press a key to continue . . .");
	   getkey();
	}
	if(keyts==4) {
	   addrTseng=tseng4;
	   addrTsengc=tseng4c;
	}
	else {
	   addrTseng=tseng;
	   addrTsengc=tsengc;
	}
	flagmouse=1;
	if(!get_status_mouse()) {
	   boxerror("No driver mouse found !!!");
	   flagmouse=0;
	}
	if(abs(cpu())!=386) {
	   boxerror("Il computer non ha una CPU 386 !!");
	   exit(0);
	}

	if(fputype()<387) {
	   boxerror("Il programma necessita di coprocessore (o emulatore) !!");
	   exit(0);
	}
	if((oldbex=bex=calloc(8192,sizeof(char)))==NULL) {
	   boxerror("No Memory Error !!!!");
	   exit(0);
	}
	if((value=calloc(NUMBER_OF_FRACTALS,sizeof(struct VALUE)))==NULL) {
	   boxerror("No Memory Error !!!!");
	   exit(0);
	}
	if((dacbox=calloc(768,sizeof(char)))==NULL) {
	   boxerror("No Memory Error !!!!");
	   exit(0);
	}
	setGS();
	setFS();
	funzione();
	if(argc>1) {

	    strcpy(funztype, "Mandel");
	    strcpy(load_f, argv[1]);
	    get_single_fileval();
	selectparm(argv[2], 1);
	    goflag=1;
	if(step<=0)
	   step=1;
	if(step>8)
	   step=8;
	load_xtern(palettef);
	video();
	x_min=xmin;
	y_min=ymin;
	x_max=xmax;
	y_max=ymax;
	mainfunx();


	}
	selectfractal();

}


/*	if(!stricmp("Hopalong",argc[1])) {
	    mainfunx=Hopalong;
	    addrHop=HopNull;
	    return;
	}
	if(!stricmp("HopSqrt",argc[1])) {
	    mainfunx=Hopalong;
	    addrHop=HopSqrt;
	    return;
	}
	if(!stricmp("HopSin",argc[1])) {
	    mainfunx=Hopalong;
	    addrHop=HopSin;
	    return;
	}
	if(!stricmp("HopCos",argc[1])) {
	    mainfunx=Hopalong;
	    addrHop=HopCos;
	    return;
	}
	if(!stricmp("HopSinCos",argc[1])) {
	    mainfunx=Hopalong;
	    addrHop=HopSinCos;
	    return;
	}*/
	/*if(!stricmp("Plankton",argc[1])) {
	    mainfunx=plank;
	    return;
	} */
	/*if(!stricmp("JuliaCos",argc[1])) {
	    mainfunx=Julia;
	    return;
	}
	if(!stricmp("Julia",argc[1])) {
	    funx=FpuJulia;
	    mainfunx=Mandel;
	    return;
	}
	if(!stricmp("JuliaEps",argc[1])) {
	    funx=FpuJuliaEps;
	    mainfunx=Mandel;
	    return;
	} */
	/*if(!stricmp("Mandel",argc[1])) {
	    funx=FpuMandel;
	    mainfunx=Mandel;
	    return;
	}
	if(!stricmp("MandelEps",argc[1])) {
	    funx=FpuMandelEps;
	    mainfunx=Mandel;
	    return;
	} */

/* BiomSin default */

	/*funx=FpuSprtSin2z;
	mainfunx=Mandel;*/


