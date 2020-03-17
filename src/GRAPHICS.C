

void linea(int st_x,int st_y,int ed_x,int ed_y,int colore)
{
register int t;
int dx,dy,tmp;
char sign1,sign2;
register int err;
char change=0;

        dx=abs(ed_x-st_x);
        dy=abs(ed_y-st_y);
	if(!dx && !dy)
	   return;
	if(!dx)
	   sign1=0;
	else
           sign1=(ed_x-st_x)>0 ? 1 : -1;
	if(!dy)
	   sign2=0;
	else
           sign2=(ed_y-st_y)>0 ? 1 : -1;

	if(dy>dx) {
	   tmp=dx;
	   dx=dy;
	   dy=tmp;
	   change=1;

	}

	err=(dy-dx)<<1;

	if(change) {
	   for(t=dx-1;t>=0;t--) {
              outonepixel(st_x,st_y,colore);
	      while(err>=0) {
                 st_x+=sign1;
		 err-=(dx<<1);
	      }
              st_y+=sign2;
	      err+=(dy<<1);
	   }
	}
	else {
	   for(t=dx-1;t>=0;t--) {
              outonepixel(st_x,st_y,1,colore);
	      while(err>=0) {
                 st_y+=sign2;
		 err-=(dx<<1);
	      }
              st_x+=sign1;
	      err+=(dy<<1);
	   }
	}
}

void cerchio(int st_x,int st_y,int raggio,int colore)
{
register int yi=raggio;
int xi=0;
register int Delta=((1-raggio)<<1);



	while(yi>=0) {
	   outonepixel(st_x+xi,st_y+yi,colore);
	   outonepixel(st_x-xi,st_y-yi,colore);
	   outonepixel(st_x+xi,st_y-yi,colore);
	   outonepixel(st_x-xi,st_y+yi,colore);
	   if(Delta<0) {
	      xi++;
	      if(((Delta<<1)+(yi<<1)-1)<=0) {
		 Delta++;
                 Delta+=(xi<<1);
	      }
	      else {
		 yi--;
		 Delta+=(xi<<1)-(yi<<1)+2;
	      }
	   }
	   else {
	      if(Delta>0) {
		 yi--;
		 if(((Delta<<1)-(xi<<1)-1)<=0) {
		    xi++;
		    Delta+=(xi<<1)-(yi<<1)+2;
		 }
		 else {
		    Delta++;
		    Delta-=(yi<<1);
		 }
	      }
	      else {
		 yi--;
		 xi++;
		 Delta+=(xi<<1)-(yi<<1)+2;
	      }
	  }
	}
}
