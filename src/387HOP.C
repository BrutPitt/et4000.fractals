#define HOP387

#include "b387.h"
extern char step,flagMotion;


void Hopalong(void) {
register int t;


	step=1;
	while(1) {
	   xmin=x_min;
	   xmax=x_max;
	   ymin=y_min;
	   ymax=y_max;
	   FpuStep();
	   if(funx()) {
	       if((t=tasto(getkey()))==27)
			return;
		     else {
			video();
			mainfunx();
			return;
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
/*	w2vx=risoluzione_x/(50/zoom-(-50/zoom));
	w2vy=risoluzione_y/(30/zoom-(-30/zoom));*/


/*	xval=aval*yval + cval*xval + 2*xval*xval*(1-cval)/(1+xval*xval);
	yval=-temp     + cval*xval + 2*xval*xval*(1-cval)/(1+xval*xval);*/


/*      x0w=-50/zoom;
	y0w=-30/zoom;
	xval=0;
	yval=0;
	for(t=iteraz;t>0;t--) {
	   xco=(xval>=0 ? 1 : -1);
	   xxval=yval-xco*sqrt(fabs(xmax*xval-ymin));
	   xcval=xmin-xval;
	   xval=xxval;
	   yval=xcval;
	   xco=(xval-x0w)*w2vx;
	   yco=(yval-y0w)*w2vy;
	   in_outpixel(xco,yco);
	   if(!(t%30000)) {
	      if(kbhit())
		 break;
	   }
	}
	sect_text();*/
