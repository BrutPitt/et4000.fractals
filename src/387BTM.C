#define BTM387

#include "b387.h"



static int far *LeftX  = (int far *)NULL;
static int far *RightX = (int far *)NULL;
static unsigned repeats;
char kbdcount=100;
extern int col,row,color;
int ixstart,iystart,ixstop,iystop;
unsigned char *decoderline;



int calc_xy(register int mx,register int my)
{


   if (color = inpixel(mx,my))
   {
      repeats++;
      return(color);
   }
   repeats = 0;
   col = mx;
   row = my;
   return(color=funx());
}


boundary_trace(register int C,register int R)   /* BTM main function */
{
   enum
       {
      North, East, South, West
   } 
   Dir;
   int modeON, C_first, bcolor, low_row, iters, gcolor;
   low_row = R;
   modeON = 0;
   Dir = East;
   bcolor = color;
   C_first = C;
   iters = 0;
   repeats = 0;

   /* main loop of BTM inside this loop the boundary is traced on screen! */
   do
   {
      iters++;          /* count times thru loop */

      if (C < LeftX[R])
         LeftX[R]  = C; /* to aid in filling polygon later */
      if (C > RightX[R])
         RightX[R] = C; /* maintain left and right limits */
      else
         if (R==low_row)
            if (C<=C_first) /* works 99.9% of time! */
               break;
      switch (Dir)
      {
      case North :
         if (R > low_row)
	    if(calc_xy(C,R-1)==bcolor)
            {
               R--;
               if (C > ixstart)
		  if(calc_xy(C-1,R)==bcolor)
                  {
                     C--;
                     Dir = West;
                  }
               break;
            }
         Dir = East;
         break;
      case East :
         if (C < ixstop)
	    if(calc_xy(C+1,R)==bcolor)
            {
               C++;
               if (R > low_row)
		  if(calc_xy(C,R-1)==bcolor)
                  {
                     R--;
                     Dir = North;
                  }
               break;
            }
         Dir = South;
         break;
      case South :
         if (R < iystop)
	    if(calc_xy(C,R+1)==bcolor)
            {
               R++;
               if (C < ixstop)
		  if(calc_xy(C+1,R)==bcolor)
                  {
                     C++;
                     Dir = East;
                  }
               break;
            }
         Dir = West;
         break;
      case West:
         if (C > ixstart)
	    if(calc_xy(C-1,R)==bcolor)
            {
               C--;
               if (R < iystop)
		  if(calc_xy(C,R+1)== bcolor)
                  {
                     R++;
                     Dir = South;
                  }
               break;
            }
         Dir = North;
         break;
      } /* case */
   }
   while (repeats<30000);

   if (iters<4)
   {
      LeftX[low_row] = 3000;
      RightX[low_row] = -3000;
      if (low_row+1 <= iystop)
      {
         LeftX[low_row+1] = 3000;
         RightX[low_row+1] = -3000;
      }
      return(0);  /* no need to fill a polygon of 3 points! */
   }

   if (iystop+1==risoluzione_y)
      if (LeftX[0]==0)
         if (RightX[0]==ixstop)
            if (LeftX[iystop]==0)
               if (RightX[iystop]==ixstop)
               {
                  /* clean up in this RARE case or next fills will fail! */
		  for (low_row = 0; low_row < risoluzione_y; low_row++)
                  {
                     LeftX[low_row] = 3000;
                     RightX[low_row] = -3000;
                  }
                  return(0);
               }
   /* fill in the traced polygon, simple but it works darn well */
   C = 0;
   for (R = low_row; R<iystop; R++)
      if (RightX[R] != -3000)
      {
	    C = fillseg(LeftX[R], RightX[R],R, bcolor);

         LeftX[R]  =  3000;
         RightX[R] = -3000; /* reset array element */
      }
      else if (C!=0) /* this is why C = 0 above! */
         return(0);
   return(0);
} /* BTM function */


fillseg(register int LeftX, int RightX, int R,  int bcolor)
{
   register  C;
   int  gcolor,i;
   char modeON=0;

   inarray(LeftX,R,RightX-LeftX,decoderline);
   for (C = LeftX; C <= RightX; C++)
   {
      gcolor=decoderline[C-LeftX];
      if (modeON && !gcolor)
	 decoderline[C-LeftX]=bcolor;
      else
      {
         if (gcolor==bcolor) /* TW saved a getcolor here */
            modeON = 1;
         else
            modeON = 0;
      }
   }
   outarray(LeftX,R,RightX-LeftX,decoderline);
   return(C);
}


bound_trace_main()
{
register int srow, scol;
int kk;

while(1) {
   iystart=ixstart=0;
   ixstop=risoluzione_x-1;
   iystop=risoluzione_y-1;
   xmin=x_min;
   xmax=x_max;
   ymin=y_min;
   ymax=y_max;
   step=1;
   FpuStep();

   if((decoderline  = calloc(risoluzione_x+10,sizeof(char)))==NULL)
      exit(0);
   if((LeftX  = calloc(risoluzione_y,sizeof(int)))==(int far *)NULL) {
      free(decoderline);
      exit(0);
   }
   if((RightX  = calloc(risoluzione_y,sizeof(int)))==(int far *)NULL)
   {
      free(decoderline);
      free((char far *)LeftX);
      exit(0);
   }


   for (srow = 0; srow < risoluzione_y; srow++)
   {
      LeftX[srow] = 3000;
      RightX[srow] = -3000;
   }


   for (srow = iystart; srow <= iystop; srow++)
   {
      for (scol = ixstart; scol <= ixstop; scol++)
      {
         if(--kbdcount<=0)
         {
	    if(kbhit())
	    {
	       kk=tasto(getkey());
	       if(!kk || kk==27) {
		  free((char far *)LeftX);
		  free((char far *)RightX);
		  free(decoderline);
		  if(!kk)
		     mainfunx();
		  return(0);
	       }
	    }
	    kbdcount=100;
         }
	 color = inpixel(scol,srow);
	 if (!color)
	 {
	    row = srow;
	    col = scol;
	    color=funx();
	    boundary_trace(scol, srow);
         }
      }
   }
   free(decoderline);
   free((char far *)LeftX);
   free((char far *)RightX);
   if(!flagMotion) {
	 while((srow=tasto(0))==1);
	 if(srow==27)
	    return(27);
   }
   else {
	 if(changemotion()==27)
	    return(27);
   }
}
} /* end of bound_trace_main */
