#define MOUSE387

#include "b387.h"

void outarray(int,int,int,char *);

void min_max(void)
{
register int j=0,t;
int minVal,maxVal;
unsigned punt[0x100];

	for(t=0;t<0x300;t+=3) {
	   punt[j++]=(float)((float)dacbox[t]*.30)+(float)((float)dacbox[t+1]*.59)+(float)((float)dacbox[t+2]*.11);
	}
	minVal=500;
	maxVal=0;
	for(t=0;t<0x100;t++) {
	   if(minVal>punt[t]) {
	      minVal=punt[t];
	      minCol=t;
	   }
	   if(maxVal<punt[t]) {
	      maxVal=punt[t];
	      maxCol=t;
	   }
	}
}



void spectro(void)
{
register int t,j;
int coeff_x,coeff_y;
unsigned char addr[256];

	coeff_x=(risoluzione_x>>1)-128;
	coeff_y=(risoluzione_y-27);
	for(j=255;j>=0;j--)
	  addr[j]=j;
	for(t=13;t>=0;t--)
	   outarray(coeff_x,coeff_y+t,256,(unsigned char far *)addr);


}



void graph_box(register int s_x,int s_y,int l_x,int l_y,unsigned char kol)
{
register int t;

	outpixel(s_x,s_y,l_x+1,kol);
	outpixel(s_x,s_y+l_y,l_x+1,kol);
	for(t=1;t<l_y;t++) {
	   outonepixel(s_x,s_y+t,kol);
	   outonepixel(s_x+l_x,s_y+t,kol);
	}
}





void out_graph_box(int s_x,int s_y,int l_x,int l_y,unsigned char *kol)
{
register int t,j=(l_x+2)<<1;;

	outarray(s_x,s_y,l_x+1,kol);
	outarray(s_x,s_y+l_y,l_x+1,&kol[l_x+1]);
	for(t=1;t<l_y;t++) {
	   outarray(s_x,s_y+t,1,&kol[j+t]);
	   outarray(s_x+l_x,s_y+t,1,&kol[j+l_y+t]);
	}
}



void in_graph_box(int s_x,int s_y,int l_x,int l_y,unsigned char *kol)
{
register int t,j=(l_x+2)<<1;;

	inarray(s_x,s_y,l_x+1,kol);
	inarray(s_x,s_y+l_y,l_x+1,&kol[l_x+1]);
	for(t=1;t<l_y;t++) {
	   inarray(s_x,s_y+t,1,&kol[j+t]);
	   inarray(s_x+l_x,s_y+t,1,&kol[j+l_y+t]);
	}
}





void keypressed(void)
{
	do {
	   ret_mouse_position();
	} while(!kbhit() && !rvbutton&0x3);
	if(rvbutton&0x3)
	   while(rvbutton&0x3)
	      ret_mouse_position();
	else
	   getkey();
}


int gettype(void)
{

	do {
	   ret_mouse_counter();
	   ret_mouse_position();
	} while(!kbhit() && !ctcy && !ctcx && (!rvbutton&0x3));
	if(ctcy)
	   return(ctcy>0 ? DwKey : UpKey);
	if(ctcx)
	   return(ctcx>0 ? RgKey : LfKey);
	if(rvbutton&0x1) {
	   while(rvbutton&0x1)
	      ret_mouse_position();
	   return(13);
	}
	if(rvbutton&0x2) {
	   while(rvbutton&0x2)
	      ret_mouse_position();
	   return(ESC);
	}
	return(getkey());
}


int show_mouse_window(int *upx,int *upy,int *dwx,int *dwy)
{
unsigned char *window_tr;
float shftx=0,shfty=0;
float oldsx,oldsy,oldszx,oldszy;
float sizebox_x,sizebox_y;
int maxbox_x=sizebox_x=risoluzione_x-1;
int maxbox_y=sizebox_y=risoluzione_y-1;
unsigned char selcolor=maxCol;
int key;
int steppiu=2;
float factor=(float)risoluzione_x/risoluzione_y;
float factor8=factor*8;

	if((window_tr=calloc((risoluzione_x<<1)+(risoluzione_y<<1)+100,sizeof(char)))==NULL) {
	   return(27);
	}
	set_mouse_xlimit(0,maxbox_x-sizebox_x);
	set_mouse_ylimit(0,maxbox_y-sizebox_y);
	set_mouse_position(shftx,shfty);
	in_graph_box(shftx,shfty,sizebox_x,sizebox_y,window_tr);
	graph_box(shftx,shfty,sizebox_x,sizebox_y,selcolor);
	ctcx=0;
	ctcy=0;
	while(1) {
	   while(1) {

	      if(flagmouse) {
	      if(kbhit()) {
		 key=getkey();
		 break;
	      }
	      oldsx=shftx;
	      oldsy=shfty;
	      ret_mouse_position();
	      ret_mouse_counter();
	      if(ctcx<0 && shftx+ctcx>=0)
		    shftx+=ctcx;
	      else {
		 if(ctcx>0 && shftx+ctcx<=maxbox_x-sizebox_x)
		    shftx+=ctcx;
		 else
		    ctcx=0;
	      }
	      if(ctcy<0 && shfty+ctcy>=0)
		    shfty+=ctcy;
	      else {
		 if(ctcy>0 && shfty+ctcy<=maxbox_y-sizebox_y)
		    shfty+=ctcy;
		 else
		    ctcy=0;
	      }
	      if(ctcx || ctcy) {
		 out_graph_box(oldsx,oldsy,sizebox_x,sizebox_y,window_tr);
		 in_graph_box(shftx,shfty,sizebox_x,sizebox_y,window_tr);
		 graph_box(shftx,shfty,sizebox_x,sizebox_y,selcolor);
	      }
	      if(rvbutton&0x2) {
		 while(rvbutton&0x2) {
		    oldszx=sizebox_x;
		    oldszy=sizebox_y;
		    ret_mouse_position();
		    ret_mouse_counter();
		    if(ctcx<0 && sizebox_x+(float)ctcx*factor>=factor8 && sizebox_y+ctcx>=8) {
			  sizebox_x+=(float)ctcx*factor;
			  sizebox_y+=ctcx;
		    }
		    else {
		       if(ctcx>0 && sizebox_x+shftx+(float)ctcx*factor<=maxbox_x && sizebox_y+shfty+ctcx<=maxbox_y) {
			  sizebox_x+=(float)ctcx*factor;
			  sizebox_y+=ctcx;
		       }
		       else
			  ctcx=0;
		    }
		    if(ctcx) {
		       out_graph_box(shftx,shfty,oldszx,oldszy,window_tr);
		       in_graph_box(shftx,shfty,sizebox_x,sizebox_y,window_tr);
		       graph_box(shftx,shfty,sizebox_x,sizebox_y,selcolor);
		    }
		 }
		 set_mouse_xlimit(0,maxbox_x-sizebox_x);
		 set_mouse_ylimit(0,maxbox_y-sizebox_y);
		 set_mouse_position(shftx,shfty);
	      }

	      if(rvbutton&0x1) {
		 while(rvbutton&0x1)
		    ret_mouse_position();
		 key=13;
		 break;
	      }
	      }
	      else {
		 key=getkey();
		 break;
	      }

	   }
	   switch(key) {
	      case ESC :
		  out_graph_box(shftx,shfty,sizebox_x,sizebox_y,window_tr);
		  free(window_tr);
		  return(ESC);
	      case ' ' :
		  selcolor=selcolor==maxCol ? minCol : maxCol;
		  graph_box(shftx,shfty,sizebox_x,sizebox_y,selcolor);
		  break;
	      case PgUp :
		  oldsx=shftx;
		  oldsy=shfty;
		  oldszx=sizebox_x;
		  oldszy=sizebox_y;
		  if(sizebox_x+shftx+steppiu*factor<=maxbox_x && sizebox_y+shfty+steppiu<=maxbox_y) {
		     sizebox_x+=(float)steppiu*factor;
		     sizebox_y+=steppiu;
		  }
		  out_graph_box(shftx,shfty,oldszx,oldszy,window_tr);
		  in_graph_box(shftx,shfty,sizebox_x,sizebox_y,window_tr);
		  graph_box(shftx,shfty,sizebox_x,sizebox_y,selcolor);
		  set_mouse_xlimit(0,maxbox_x-sizebox_x);
		  set_mouse_ylimit(0,maxbox_y-sizebox_y);
		  set_mouse_position(shftx,shfty);
		  break;
	      case PgDn :
		  oldsx=shftx;
		  oldsy=shfty;
		  oldszx=sizebox_x;
		  oldszy=sizebox_y;
		  if(sizebox_x-(float)steppiu*factor>=factor8 && sizebox_y-steppiu>=8) {
		     sizebox_x-=(float)steppiu*factor;
		     sizebox_y-=steppiu;
		  }
		  out_graph_box(shftx,shfty,oldszx,oldszy,window_tr);
		  in_graph_box(shftx,shfty,sizebox_x,sizebox_y,window_tr);
		  graph_box(shftx,shfty,sizebox_x,sizebox_y,selcolor);
		  set_mouse_xlimit(0,maxbox_x-sizebox_x);
		  set_mouse_ylimit(0,maxbox_y-sizebox_y);
		  set_mouse_position(shftx,shfty);
		  break;
	      case DwKey :
		  oldsx=shftx;
		  oldsy=shfty;
		  oldszx=sizebox_x;
		  oldszy=sizebox_y;
		  if(shfty+steppiu<=maxbox_y-sizebox_y)
		    shfty+=steppiu;
		  out_graph_box(oldsx,oldsy,sizebox_x,sizebox_y,window_tr);
		  in_graph_box(shftx,shfty,sizebox_x,sizebox_y,window_tr);
		  graph_box(shftx,shfty,sizebox_x,sizebox_y,selcolor);
		  set_mouse_xlimit(0,maxbox_x-sizebox_x);
		  set_mouse_ylimit(0,maxbox_y-sizebox_y);
		  set_mouse_position(shftx,shfty);
		  break;
	      case UpKey :
		  oldsx=shftx;
		  oldsy=shfty;
		  oldszx=sizebox_x;
		  oldszy=sizebox_y;
		  if(shfty-steppiu>=0)
		    shfty-=steppiu;
		  out_graph_box(oldsx,oldsy,sizebox_x,sizebox_y,window_tr);
		  in_graph_box(shftx,shfty,sizebox_x,sizebox_y,window_tr);
		  graph_box(shftx,shfty,sizebox_x,sizebox_y,selcolor);
		  set_mouse_xlimit(0,maxbox_x-sizebox_x);
		  set_mouse_ylimit(0,maxbox_y-sizebox_y);
		  set_mouse_position(shftx,shfty);
		  break;
	      case RgKey :
		  oldsx=shftx;
		  oldsy=shfty;
		  oldszx=sizebox_x;
		  oldszy=sizebox_y;
		  if(shftx+steppiu<=maxbox_x-sizebox_x)
		     shftx+=steppiu;
		  out_graph_box(oldsx,oldsy,sizebox_x,sizebox_y,window_tr);
		  in_graph_box(shftx,shfty,sizebox_x,sizebox_y,window_tr);
		  graph_box(shftx,shfty,sizebox_x,sizebox_y,selcolor);
		  set_mouse_xlimit(0,maxbox_x-sizebox_x);
		  set_mouse_ylimit(0,maxbox_y-sizebox_y);
		  set_mouse_position(shftx,shfty);
		  break;
	      case LfKey :
		  oldsx=shftx;
		  oldsy=shfty;
		  oldszx=sizebox_x;
		  oldszy=sizebox_y;
		  if(shftx-steppiu>=0)
		    shftx-=steppiu;
		  out_graph_box(oldsx,oldsy,sizebox_x,sizebox_y,window_tr);
		  in_graph_box(shftx,shfty,sizebox_x,sizebox_y,window_tr);
		  graph_box(shftx,shfty,sizebox_x,sizebox_y,selcolor);
		  set_mouse_xlimit(0,maxbox_x-sizebox_x);
		  set_mouse_ylimit(0,maxbox_y-sizebox_y);
		  set_mouse_position(shftx,shfty);
		  break;
	      case '+' :
		 if(steppiu<32) {
		    steppiu++;
		  }
		  continue;
	      case '-' :
		  if(steppiu>1) {
		     steppiu--;
		  }
		  continue;
	      case 13 :
		  out_graph_box(shftx,shfty,sizebox_x,sizebox_y,window_tr);
		  *upx=shftx;
		  *upy=shfty;
		  *dwx=(shftx+sizebox_x);
		  *dwy=(shfty+sizebox_y);
		  free(window_tr);
		  return(0);
	   }
	}



}



