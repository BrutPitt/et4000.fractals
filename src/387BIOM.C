#define BIOM387

#include "b387.h"

extern int row,col;
extern int bailout;
extern unsigned char *bex;
extern char step;
extern double cr,cj;
extern double kostre,kostim;
extern char flagMotion;
int rigx;




void Popcorn(void)
{
register int j,t;

	   while(1) {
	      t=1;
	      xmin=x_min;
	      xmax=x_max;
	      ymin=y_min;
	      ymax=y_max;
	      rate_x=risoluzione_x/(xmax-xmin);
	      rate_y=risoluzione_y/(ymax-ymin);
	      absxmin=fabs(x_min);
	      absymin=fabs(y_min);
	      FpuStep();
	      oldxmin=xmin;
	      rigx=0;
	      for(j=0;j<risoluzione_y;j+=step) {
		 rigx++;
		 funx();
		 if(kbhit()) {
		     if((t=tasto(getkey()))==27)
			return;
		     else
			if(!t) {
			   mainfunx();
			   return;
			}

		 }
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





void popcorn(void)
{
register int t,j=0;
int r;
float h=.05,x,y,xx,yy;
int x0,y0;
float pmx1,pmx2,pmy1,pmy2,ft;

	  xmin=x_min;
	  xmax=x_max;
	  ymin=y_min;
	  ymax=y_max;
	  FpuStep();
	  oldxmin=xmin;
	  pmx1=risoluzione_x/(xmax-xmin);
	  pmy1=risoluzione_y/(ymax-ymin);
	  /*ft=(0.0-sty)*stx*risoluzione_x*risoluzione_y-(xmax-xmin)*(ymin-ymax);
	  pmx1=stx*risoluzione_x*risoluzione_y/ft;
	  pmx2=(ymin-ymax)*risoluzione_x/ft;
	  pmy1=(0.0-sty)*risoluzione_x/ft;
	  pmy2=(xmax-xmin)*risoluzione_y/ft;*/
	for(r=1;r<=risoluzione_y;r++) {
	   xmin=oldxmin;
	   for(t=1;t<=risoluzione_x;t++) {
	      x=xmin;
	      y=ymin;
	      j=0;
	      while(j++<iteraz) {
		 xx=x-kostre*(FpuSin(y+FpuTan(3*y*y)));
		 yy=y-kostim*(FpuSin(x+FpuTan(3*x*x)));
		 x=xx;
		 y=yy;

		 y0=(fabs(y_min)+y)*pmy1;
		 x0=(fabs(x_min)+x)*pmx1;
		 if(x0>=0 && x0<risoluzione_x && y0>=0 && y0<risoluzione_y)
		    outonepixel(x0,y0,r);
		 if(x*x>4)
		    break;

	      }

	      xmin+=stx;
	   }
	   ymin+=sty;
	   if(kbhit()) {
	      if(tasto(getkey())==27)
		  return;
	      set_color();
	   }
	}
}

void tange(void)
{
register int t,j=0;
int r;
float h=.05,x,y,xx,yy,divis;
int x0,y0;
float pmx1,pmx2,pmy1,pmy2,ft,d1,d;

	while(1) {
	  xmin=x_min;
	  xmax=x_max;
	  ymin=y_min;
	  ymax=y_max;
	  FpuStep();
	  oldxmin=xmin;
	for(r=0;r<risoluzione_y;r+=step) {
	   xmin=oldxmin;
	   for(t=0;t<risoluzione_x;t+=step) {
	      j=0;
	      d=0;
	      y=ymin;
	      x=xmin;
	      do {
		 d1=d;
		 yy=y+y;
		 xx=x+x;
		 divis=FpuCos(xx)+FpuCosh(yy);
		 x=(kostre*FpuSin(xx)-kostim*FpuSinh(yy))/divis+cr;
		 y=(kostim*FpuSin(xx)+kostre*FpuSinh(yy))/divis+cj;
		 d=y*y;
		 if(x*x>9999999L)
		    break;
	      } while(d<bailout && j++<iteraz/* && fabs(d1-d)>epsilon*/);
	      bex[t]=j>=iteraz ? 0 : j;
	      xmin+=stx;
	   }

	 ymin+=sty;
	 if(kbhit()) {
		     if((t=tasto(getkey()))==27)
			return;
		     else
			if(!t) {
			   mainfunx();
			   return;
			}

		 }
	   if(step!=1)
	      interpol(r);
	   outarray(0,r,risoluzione_x,bex);
	}
	while((t=tasto(0))==1);
	      if(t==27)
		 return;
	}
}


int tangepix(void)
{
register int t,j=0;
float x,y,xx,yy,divis;
float d;


	      /*d=0;*/
	      y=y_min+sty*row;
	      x=x_min+stx*col;
	      do {
	      /*	 d1=d;*/
		 yy=y+y;
		 xx=x+x;
		 divis=FpuCos(xx)+FpuCosh(yy);
		 x=(kostre*FpuSin(xx)-kostim*FpuSinh(yy))/divis+cr;
		 y=(kostim*FpuSin(xx)+kostre*FpuSinh(yy))/divis+cj;
		 d=x*x+y*y;
	      } while(d<bailout && j++<iteraz/* && fabs(d1-d)>epsilon*/);
	      t=j>=iteraz ? 255 : j;
	      if(!t)
		 t++;
	      outonepixel(col,row,t);
	      return(t);

}



