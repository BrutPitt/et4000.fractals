#define JUL387

#define LOG2 1.442695041
#define MAXVAL 1.99e100


#include "b387.h"

float a,b,zr,zj,d=0,d1;
extern int bailout;
extern double epsilon;
extern flagMotion;
extern double aval,bval,cval,dval;



void Jcos(void)
{
float cc,ss;

	ss=FpuSin(a)*FpuSinh(b);
	cc=FpuCos(a)*FpuCosh(b);
/*	ss=sin(a)*sinh(b);
	cc=cos(a)*cosh(b);*/
	zr=cc*kostre+ss*kostim+cr;
	zj=cc*kostim-ss*kostre+cj;
	return;
}

void Jsin(void)
{
float cs,sc;

	sc=FpuSin(a)*FpuCosh(b);
	cs=FpuCos(a)*FpuSinh(b);
	zr=sc*kostre-cs*kostim+cr;
	zj=cs*kostim+sc*kostre+cj;
	return;
}

void ME(void)
{
	zr=a*a-b*b+xmin;
	zj=2*a*b+ymin;
}



void Lambda(void)
{
register int t,n;
int kk,j;
char uscita=1;
float u,v,a1,a2,aa,bb;


	   while(uscita) {
	      kk=1;
	      xmin=x_min;
	      xmax=x_max;
	      ymin=y_min;
	      ymax=y_max;
	      u=cr;
	      v=cj;
	      FpuStep();
	      oldxmin=xmin;
	      for(j=0;j<risoluzione_y;j+=step) {
		 xmin=oldxmin;
		 for(t=0;t<risoluzione_x;t+=step) {
		     a=xmin;
		     b=ymin;
		     d=0;
		     d1=1;
		     n=0;
		     while(d<bailout && n++<iteraz /*&& fabs(d1-d)>epsilon*/) {
		      /*	 a1=a-a*a*a+3*a*b*b;
			 a2=3*a*a*b-b-b*b*b;
			 zr=u*a1+v*a2+u;
			 zj=v*a1-u*a2+v; */

		      /*	 a1=a*a-b*b;
			 a2=2*a*b;
			 zr=u*a1-v*a2;
			 zj=v*a1+u*a2;*/

/*	Lambda       */

			 a1=a-a*a+b*b;
			 a2=b-2*a*b;
			 zr=u*a1-v*a2;
			 zj=v*a1+u*a2;

/*			 aa=a*a;
			 bb=b*b;
			 a1=aa-aa*a-bb+3*a*bb;
			 a2=bb*b-3*aa*b+2*a*b;
			 zr=u*a1-v*a2;
			 zj=v*a1+u*a2;*/

			 a=zr;
			 b=zj;
			 d=a*a+b*b;
		     }
		     bex[t]=n<iteraz ? n : 0;
		     xmin+=stx;
		 }
		 if(step!=1)
		    interpol(j);

		 if(kbhit()) {
		     if((kk=tasto(getkey()))==27)
			return;
		     else
			if(!kk) {
			   mainfunx();
			   return;
			}


		    
		 }
		 outarray(0,j,risoluzione_x,(unsigned char far *)bex);
		 ymin+=sty;
	      }
		 while((t=tasto(0))==1);
		 if(t==27)
		    return;

	   }

}


void Mandel3(void)
{
register int t,n;
int kk,j;
char uscita=1;
float u,v,a1,a2,aa,bb;


	   while(uscita) {
	      kk=1;
	      xmin=x_min;
	      xmax=x_max;
	      ymin=y_min;
	      ymax=y_max;
	      FpuStep();
	      oldxmin=xmin;
	      for(j=0;j<risoluzione_y;j+=step) {
		 xmin=oldxmin;
		 for(t=0;t<risoluzione_x;t+=step) {
		     a=0;
		     b=0;
		     d=0;
		     d1=1;
		     n=0;
		     aa=a*a;
		     bb=b*b;
		     while(d<bailout && n++<iteraz /*&& fabs(d1-d)>epsilon*/) {
/* Mandel Z^3 -------------------------------------
			 a1=aa*a-3*a*bb+xmin;
			 a2=3*aa*b-bb*b+ymin;
*/
/* Mandel Z^4 -------------------------------------
			 a1=aa*aa-6*aa*bb+bb*bb+xmin;
			 a2=4*a*b*(aa-bb)+ymin;
*/
/* Mandel Z^5 --------------------------------------
			 a1=a*(aa*aa-10*aa*bb+5*bb*bb)+xmin;
			 a2=b*(bb*bb-10*aa*bb+5*aa*aa)+ymin;
*/                       a1=aa+bb+xmin;
			 a2=2*a*b+ymin;
			 a=a1;
			 b=a2;
			 aa=a*a;
			 bb=b*b;
			 d=aa+bb;
		     }
		     bex[t]=n<iteraz ? n : 0;
		     xmin+=stx;
		 }
		 if(step!=1)
		    interpol(j);

		 if(kbhit()) {
		     if((kk=tasto(getkey()))==27)
			return;
		     else
			if(!kk) {
			   mainfunx();
			   return;
			}


		    
		 }
		 outarray(0,j,risoluzione_x,(unsigned char far *)bex);
		 ymin+=sty;
	      }
		 while((t=tasto(0))==1);
		 if(t==27)
		    return;

	   }

}

/* tappezzeria */

void pp3(void)
{
register int t,n;
int kk,j;
unsigned c;
char uscita=1;
float u,v,a1,a2,aa,bb;


	   while(uscita) {
	      kk=1;
	      xmin=x_min;
	      xmax=x_max;
	      ymin=y_min;
	      ymax=y_max;
	      FpuStep();
	      oldxmin=xmin;
	      for(j=0;j<risoluzione_y;j++) {
		 xmin=oldxmin;
		 for(t=0;t<risoluzione_x;t++) {
			 c=xmin*xmin+ymin*ymin;
			 bex[t]=c%2 ? 0 : 254;
			 xmin+=stx;
		 }
		 if(step!=1)
		    interpol(j);

		 if(kbhit()) {
		     if((kk=tasto(getkey()))==27)
			return;
		     else
			if(!kk) {
			   mainfunx();
			   return;
			}



		 }
		 outarray(0,j,risoluzione_x,(unsigned char far *)bex);
		 ymin+=sty;
	      }
		 while((t=tasto(0))==1);
		 if(t==27)
		    return;

	   }

}



void lyapfx(void)
{
int t,j,kk;
register int n,i=0;
int col;
double tot,mom;
double x,r,lyap;
char array[]={"abba\0"};
char uscita=1;


	while(uscita) {
	   kk=1;
	   xmin=x_min;
	   xmax=x_max;
	   ymin=y_min;
	   ymax=y_max;
	   FpuStep();
	   oldxmin=xmin;
	   for(j=0;j<risoluzione_y;j+=step) {
	      xmin=oldxmin;
	      for(t=0;t<risoluzione_x;t+=step) {
		 tot=0;
		 x=aval;
		 i=0;
		 for(n=0;n<bailout;n++) {
		    if(!array[i])
		       i=0;
		    if(fabs(x*=(array[i++]=='a' ? xmin : ymin)*(1-x))>MAXVAL)
		       break;
		 }
		 i=0;
		 for(n=0;++n<=iteraz;) {
		    if(!array[i])
		       i=0;
		    r=array[i++]=='a' ? xmin : ymin;
		    if(fabs(x*=r*(1-x))>MAXVAL)
		       break;
		    if(!(mom=fabs(r-2*x*r))) {
		       tot=MAXVAL;
		       break;
		    }
		    if(fabs(tot+=log(mom)*LOG2)>MAXVAL)
		       break;
		 }
		 lyap=tot/(double)n;
		 bex[t]=lyap>=0 ? 0 : 254.0/(1+log(1-lyap))+1;
		 xmin+=stx;
	      }
	      if(step!=1)
		 interpol(j);
	      if(kbhit()) {
		 if((kk=tasto(getkey()))==27)
		    return;
		 else
		    if(!kk) {
		       mainfunx();
		       return;
		    }
	      }
	      outarray(0,j,risoluzione_x,bex);
	      ymin+=sty;
	   }
	   while((t=tasto(0))==1);
	   if(t==27)
	      return;

       }

}


void lyapfxx(void)
{
int t,j,kk;
register int n,i=0;
int col;
double tot,mom;
double x,r,lyap;
char array[]={"abba\0"};
char uscita=1;
unsigned l1;

/*
	SetHCMode(0x2e);
	while(uscita) {
	   kk=1;
	   xmin=x_min;
	   xmax=x_max;
	   ymin=y_min;
	   ymax=y_max;
	   FpuStep();
	   oldxmin=xmin;
	   for(j=0;j<risoluzione_y;j+=step) {
	      xmin=oldxmin;
	      for(t=0;t<risoluzione_x;t+=step) {
		 tot=0;
		 x=aval;
		 i=0;
		 for(n=0;n<bailout;n++) {
		    if(!array[i])
		       i=0;
		    if(fabs(x*=(array[i++]=='a' ? xmin : ymin)*(1-x))>MAXVAL)
		       break;
		 }
		 i=0;
		 for(n=0;++n<=iteraz;) {
		    if(!array[i])
		       i=0;
		    r=array[i++]=='a' ? xmin : ymin;
		    if(fabs(x*=r*(1-x))>MAXVAL)
		       break;
		    if(!(mom=fabs(r-2*x*r))) {
		       tot=MAXVAL;
		       break;
		    }
		    if(fabs(tot+=log(mom)*LOG2)>MAXVAL)
		       break;
		 }
		 lyap=tot/(double)n;

		 l1=lyap>=0 ? 0 : 32768.0/(1+log(1-lyap))+1;
		 if(!l1)
		    hcpixel(t,j,0);
		 else
		    hcpixel(t,j,RetColor(l1));
		 xmin+=stx;
	      }
	      /*
	      if(step!=1)
		 interpol(j);*/
	      if(kbhit()) {
		 if((kk=tasto(getkey()))==27)
		    return;
		 else
		    if(!kk) {
		       mainfunx();
		       return;
		    }
	      }
	      ymin+=sty;
	   }
	   while((t=tasto(0))==1);
	   if(t==27)
	      return;

       }
*/
}







void pp4(void)
{
register int n=0,rad=(int)dval;
int kk,t;
long j;
unsigned c;
char uscita=1;
float u,v,a1,a2,a3,aa,bb;
float ccxx,ccyy,cczz;
int xxx,yyy;





	   while(uscita) {
	      kk=1;
	      n=0;
	      xmin=x_min;
	      xmax=x_max;
	      ymin=y_min;
	      ymax=y_max;
	      FpuStep();
	      for(j=0;j<iteraz;j++) {
		 aa=aval*j;
		 bb=bval*j;
		 ccxx=FpuSin(aa)*FpuCos(bb);
		 ccyy=FpuSin(aa)*FpuSin(bb);
		 cczz=FpuCos(aa);
		 if(!(j%10000))
		   if(kbhit()) {
		     if((kk=tasto(getkey()))==27)
			return;
		     else
			if(!kk) {
			   mainfunx();
			   return;
			}

		   }

		 xxx=((ccxx+FpuCos(cval)*cczz)-x_min)/stx;
		 yyy=((ccyy+FpuSin(cval)*cczz)-y_min)/sty;
		 if((xxx-rad)<0 || (yyy-rad)<0 || (xxx+rad)>=risoluzione_x || (yyy+rad)>=risoluzione_y)
		    continue;
		 cerchio(xxx,yyy,rad,n++);
		 /*outonepixel(xxx,yyy,inpixel(xxx,yyy)+1);*/

	      }
	      while((t=tasto(0))==1);
	      if(t==27)
		  return;

	   }

}



/* Sierpinski      */


void sierpinski(void)
{
register int t,n;
double a=0,b=0,d,zr,zi;
int kk,j;
unsigned c;
char uscita=1;
float u,v,a1,a2,aa,bb;



	   while(uscita) {
	      kk=1;
	      xmin=x_min;
	      xmax=x_max;
	      ymin=y_min;
	      ymax=y_max;
	      FpuStep();
	      oldxmin=xmin;
	      for(j=0;j<risoluzione_y;j+=step) {
		 xmin=oldxmin;
		 for(t=0;t<risoluzione_x;t+=step) {
		     zr=0;
		     zi=0;
		     a=xmin;
		     b=ymin;;
		     n=-1;
		     d=0;
		     while(d<bailout && ++n<iteraz) {
			if(zi>.5) {
			   zr=a+a;
			   zi=b+b-1;
			}
			else {
			   if(zr>.5) {
			      zr=a+a-1;
			      zi=b+b;
			   }
			   else {
			      zr=a+a;
			      zi=b+b;
			   }
			}
			a=zr;
			b=zi;
			d=zr*zr+zi*zi;
		     }
		     xmin+=stx;
		     bex[t]=n;
		 }
		 if(step!=1)
		    interpol(j);

		 if(kbhit()) {
		     if((kk=tasto(getkey()))==27)
			return;
		     else
			if(!kk) {
			   mainfunx();
			   return;
			}



		 }
		 outarray(0,j,risoluzione_x,bex);
		 ymin+=sty;
	      }
		 while((t=tasto(0))==1);
		 if(t==27)
		    return;

	   }

}

void pp6(void)
{
register int t,n;
double a=0,b=0,d,zr,zi;
int kk,j;
unsigned c;
char uscita=1;
float u,v,a1,a2,aa,bb;



	   while(uscita) {
	      kk=1;
	      xmin=x_min;
	      xmax=x_max;
	      ymin=y_min;
	      ymax=y_max;
	      FpuStep();
	      oldxmin=xmin;
	      for(j=0;j<risoluzione_y;j+=step) {
		 xmin=oldxmin;
		 for(t=0;t<risoluzione_x;t+=step) {
		     zr=0;
		     zi=0;
		     a=xmin;
		     b=ymin;;
		     n=-1;
		     d=0;
		     while(d<bailout && ++n<iteraz) {
			if(zi>.521739) {
			   zr=-(2*.3793)*a+(2*.6207)*b-1;
			   zi=-(2*.4138)*a-(2*.4138)*b+.6666;
			}
			else {
			      zr=-(2*.3793)*a-(2*.3793)*b-1;
			      zi=-(2*.4138)*a+(2*.5862)*b+.6666;
			}
			a=zr;
			b=zi;
			d=zr*zr+zi*zi;
		     }
		     xmin+=stx;
		     bex[t]=n;
		 }
		 if(step!=1)
		    interpol(j);

		 if(kbhit()) {
		     if((kk=tasto(getkey()))==27)
			return;
		     else
			if(!kk) {
			   mainfunx();
			   return;
			}



		 }
		 outarray(0,j,risoluzione_x,bex);
		 ymin+=sty;
	      }
		 while((t=tasto(0))==1);
		 if(t==27)
		    return;

	   }

}


/* pagina 329 Fractals Everywhere

	 / (z-1)/l   If Real(z)>=0        l' = il complesso coniugato di l
Fl(z) = {
	 \ (z+1)/l'  If Real(z)<0


	 l = (xmin + I ymin);
	 z = (zr   + I zi);

		   (zr-1)*xmin + zi*ymin      zi*xmin - (zr-1)*ymin
      (z-1)/l =    ---------------------  + I ---------------------
		     xmin^2 + ymin^2             xmin^2 + ymin^2

		   (zr+1)*xmin - zi*ymin      zi*xmin + (zr+1)*ymin
      (z+1)/l'=    ---------------------  + I ---------------------
		     xmin^2 + ymin^2             xmin^2 + ymin^2

*/

void lyapf(void)
{
register int t,n;
double a=0,b=0,d,zr,zi;
int kk,j;
unsigned c;
char uscita=1;
float u,v,a1,a2,aa,bb,kl;
/*

	   SetHCMode(0x2e);
	   while(uscita) {
	      kk=1;
	      xmin=x_min;
	      xmax=x_max;
	      ymin=y_min;
	      ymax=y_max;
	      FpuStep();
	      oldxmin=xmin;
	      for(j=0;j<risoluzione_y;j+=step) {
		 xmin=oldxmin;
		 for(t=0;t<risoluzione_x;t+=step) {
		     zr=0;
		     zi=0;
		     n=0;
		     d=0;
		     do {
			kl=xmin*xmin+ymin*ymin;
			if(zr>=0) {
			   zr-=1;
			   a=(zr*xmin+zi*ymin)/kl;
			   b=(xmin*zi-ymin*zr)/kl;
			}
			else {
			   zr+=1;
			   a=(zr*xmin-zi*ymin)/kl;
			   b=(xmin*zi+ymin*zr)/kl;
			}
			zr=a;
			zi=b;
		     } while((zr*zr+zi*zi)<bailout && n++<iteraz);
		     xmin+=stx;
		     if(n)
			hcpixel(t,j,n);
		 }

		 if(kbhit()) {
		     if((kk=tasto(getkey()))==27)
			return;
		     else
			if(!kk) {
			   mainfunx();
			   return;
			}



		 }
		 ymin+=sty;
	      }
		 while((t=tasto(0))==1);
		 if(t==27)
		    return;

	   }
  */
}



void pp9(void)
{
register int t,n;
double a=0,b=0,d,zr,zi;
int kk,j;
unsigned c;
char uscita=1;
float u,v,a1,a2,aa,bb,kl;



	   while(uscita) {
	      kk=1;
	      xmin=x_min;
	      xmax=x_max;
	      ymin=y_min;
	      ymax=y_max;
	      FpuStep();
	      oldxmin=xmin;
	      for(j=0;j<risoluzione_y;j+=step) {
		 xmin=oldxmin;
		 for(t=0;t<risoluzione_x;t+=step) {
		     zr=0;
		     zi=0;
		     n=0;
		     d=0;
		     do {
			kl=xmin*xmin+ymin*ymin;
			if(zr>=0) {
			   zr-=1;
			   a=(zr*xmin+zi*ymin)/kl;
			   b=(xmin*zi-ymin*zr)/kl;
			}
			else {
			   zr+=1;
			   a=(zr*xmin-zi*ymin)/kl;
			   b=(xmin*zi+ymin*zr)/kl;
			}
			zr=a;
			zi=b;
		     } while((zr*zr+zi*zi)<bailout && n++<iteraz);
		     xmin+=stx;
		     bex[t]=n;
		 }
		 if(step!=1)
		    interpol(j);

		 if(kbhit()) {
		     if((kk=tasto(getkey()))==27)
			return;
		     else
			if(!kk) {
			   mainfunx();
			   return;
			}



		 }
		 outarray(0,j,risoluzione_x,bex);
		 ymin+=sty;
	      }
		 while((t=tasto(0))==1);
		 if(t==27)
		    return;

	   }

}



/* pag 295 */


void pp10(void)
{
register int t,n;
double a=0,b=0,d,zr,zi;
int kk,j;
unsigned c;
char uscita=1;
float u,v,a1,a2,aa,bb,kl,d1;



	   while(uscita) {
	      kk=1;
	      xmin=x_min;
	      xmax=x_max;
	      ymin=y_min;
	      ymax=y_max;
	      FpuStep();
	      oldxmin=xmin;
	      for(j=0;j<risoluzione_y;j+=step) {
		 xmin=oldxmin;
		 for(t=0;t<risoluzione_x;t+=step) {
		     zr=xmin;
		     zi=ymin;
		     n=0;
		     d=0;
		     d1=0;
/*		     kl=xmin*xmin+ymin*ymin;
		     if(!kl)
			kl=.0000000001;*/
		     do {
			if(zr>0) {
			   a=zr*zr-zi*zi-1;
			   b=zr*zi;
			}
			else {
			   a=zr*zr-zi*zi-1+aval*zr;
			   b=zr*zi;
			}
			zr=a;
			zi=b+b;
			d=zr*zr+zi*zi;
/*			if(fabs(d-d1)<.0000001)
			   break;
			d1=d;*/
		     } while(d<bailout && n++<iteraz);
		     xmin+=stx;
/*		     if(n>=iteraz)
			bex[t]=0;
		     else*/
			bex[t]=n;
		 }
		 if(step!=1)
		    interpol(j);

		 if(kbhit()) {
		     if((kk=tasto(getkey()))==27)
			return;
		     else
			if(!kk) {
			   mainfunx();
			   return;
			}



		 }
		 outarray(0,j,risoluzione_x,bex);
		 ymin+=sty;
	      }
		 while((t=tasto(0))==1);
		 if(t==27)
		    return;

	   }

}



void pp12(void)
{
register int t,n;
double a=0,b=0,d,zr,zi;
int kk,j;
unsigned c;
char uscita=1;
float u,v,a1,a2,aa,bb,kl,d1;



	   while(uscita) {
	      kk=1;
	      xmin=x_min;
	      xmax=x_max;
	      ymin=y_min;
	      ymax=y_max;
	      FpuStep();
	      oldxmin=xmin;
	      for(j=0;j<risoluzione_y;j+=step) {
		 xmin=oldxmin;
		 for(t=0;t<risoluzione_x;t+=step) {
		     zr=0;
		     zi=0;
		     n=0;
		     d=0;
		     d1=0;
/*		     kl=xmin*xmin+ymin*ymin;
			if(!kl)
			   kl=.0000000001;*/
		     do {
			a=zr*xmin-zi+ymin;
			b=zi*xmin+zr*ymin;
			zi=b;
			if(zr>=0)
			   zr=a-1;
			else
			   zr=a+1;
			d=zr*zr+zi*zi;
/*			a1=2*bailout*(zr*zr-zi*zi)*iteraz;
			if(fabs(d-d1)<.0000001)
			   break;
			d1=d;*/
		     } while(d<bailout && n++<iteraz);
		     xmin+=stx;
		     if(n>=iteraz)
			bex[t]=0;
		     else
			bex[t]=254;
		 }
		 if(step!=1)
		    interpol(j);

		 if(kbhit()) {
		     if((kk=tasto(getkey()))==27)
			return;
		     else
			if(!kk) {
			   mainfunx();
			   return;
			}


		    
		 }
		 outarray(0,j,risoluzione_x,bex);
		 ymin+=sty;
	      }
		 while((t=tasto(0))==1);
		 if(t==27)
		    return;

	   }

}


