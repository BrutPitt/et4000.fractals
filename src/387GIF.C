#define GIF387

#include "b387.h"

#define MAXTEST   100
#define MAXENTRY  4093
#define MAXSTRING 64000
#define MAX_CODES   4095



/*extern struct SREGS sreg;*/
/*
extern char vix;
extern char flag_disk;
extern unsigned char *pacchetto;
extern unsigned totale_dati;
extern int (* scarica_pacchetto)();
extern long hist_mem[0x100];
extern float gapvalue;
extern int (* read_supporto_area)();
extern int (* write_supporto_area)();
extern FILE *in;
extern FILE *out;

*/

unsigned fin_color;

extern char tasto_premuto[5];

extern union REGS inreg,outreg;
char *en;
static int n_Righe;
static unsigned char flag_GCM,flag_CR;            /* Flag Global Color Map - Color Res. */
static unsigned char sfondo,flag_PIXEL,flag_IL;   /* flag Sfondo - colorPixel - IntetLace */
static unsigned I_left,I_top;                /* flag locali LeftPoint - TopPoint */
static unsigned I_width,I_height;            /* flag locali Width_inPixel - Heigth_inPixel */



/* variables */
static int curr_size;
static int clear;
static int ending;
static int newcodes;
static int top_slot;
static int slot;
static int navail_bytes;
static int nbits_left;
static unsigned char b1;
static unsigned char byte_buff[257];
static unsigned char *pbytes;

static long code_mask[13] = {
     0,
     0x0001, 0x0003,
     0x0007, 0x000F,
     0x001F, 0x003F,
     0x007F, 0x00FF,
     0x01FF, 0x03FF,
     0x07FF, 0x0FFF
     };



static int     colors=256;


static int strlocn[MAXENTRY], entrynum[MAXENTRY];
static unsigned char teststring[MAXTEST];

static int lentest, lastentry, numentries, numrealentries;
static unsigned nextentry;
static int clearcode, endcode;
static unsigned int hashcode;
static unsigned char blockcount, block[266];
static int startbits, codebits, bytecount, bitcount;
static unsigned char dstack[MAX_CODES + 1];
static unsigned char suffix[MAX_CODES + 1];
static unsigned prefix[MAX_CODES + 1];
static unsigned char decoderline[2];

extern int minCol,maxCol;
extern int risoluzione_x,risoluzione_y;
extern FILE *bmp;
extern unsigned char *dacbox;
extern unsigned char *bex;


static char pass;
int save_dati(int,unsigned char);


void init_exp(int size)
{
	curr_size = size + 1;
	top_slot = 1 << curr_size;
	clear = 1 << size;
	ending = clear + 1;
	slot = newcodes = ending + 1;
	navail_bytes = nbits_left = 0;
}

void shftwrite(unsigned char *color,int numcolors)
{
unsigned char thiscolor;
int i,j;

	for (i = 0; i < numcolors; i++)
	for (j = 0; j < 3; j++) {
		thiscolor = color[3*i+j];
		thiscolor = thiscolor << 2;
		thiscolor += (thiscolor >> 6);
		fwrite(&thiscolor,1,1,bmp);
		}
}

void raster(unsigned int code)
{
unsigned int icode, i, j;

	if (code == 9999) {
	   bytecount = 0;
	   bitcount = 0;
	   for (i = 0; i < 266; i++)
	      block[i] = 0;
	   return;
	}
	icode = code << bitcount;
	block[bytecount  ] |= (icode & 255);
	block[bytecount+1] |= ((icode>>8) & 255);
	icode = (code>>8) << bitcount;
	block[bytecount+2] |= ((icode>>8) & 255);
	bitcount += codebits;
	while (bitcount >= 8) {
	   bitcount -= 8;
	   bytecount++;
	}
	if (bytecount > 250 || code == endcode) {
	   if (code == endcode)
	      while (bitcount > 0) {
		 bitcount -= 8;
		 bytecount++;
	      }
	   i = bytecount;
	   blockcount = i;
	   fwrite(&blockcount,1,1,bmp);
	   fwrite(block,i,1,bmp);
	   bytecount = 0;
	   for (j = 0; j < 5; j++)
	      block[j] = block[j+i];
	   for (j = 5; j < 266; j++)
	      block[j] = 0;
	}
}

int compextra(unsigned first,char *second,unsigned count)
{
register unsigned t,j=0;

	for(t=first;t<first+count;t++)
	   if(en[t]!=second[j++])
	      return(1);
	return(0);
}


/*void copextra(unsigned first,char *second,unsigned count)
{
register unsigned t,j=0;

       for(t=first;t<first+count;t++)
	   en[t]=second[j++];

} */



void inittable(void)
{
int i;

	raster(clearcode);
	numentries = 0;
	numrealentries = 0;
	nextentry = 1;
	lentest = 0;
	codebits = startbits;
	movsb_mem_val(en,0,1);
	movsb_mem_val((unsigned char *)strlocn,0,MAXENTRY*2);

}



int get_next_code()
{
register unsigned i;
unsigned long ret;
unsigned char rat;

	if(nbits_left == 0) {
	   if (navail_bytes <= 0) {
	      pbytes = byte_buff;

	      navail_bytes=fgetc(bmp);
	      for (i = 0; i < (unsigned)navail_bytes; ++i) {
		byte_buff[i]=fgetc(bmp);
	      }

	   }
	   b1 = *pbytes++;
	   nbits_left = 8;
	   --navail_bytes;
	}
	ret = b1 >> (8 - nbits_left);
	while(curr_size > nbits_left) {
	   if(navail_bytes <= 0) {
	      pbytes = byte_buff;
	      navail_bytes=fgetc(bmp);
	      for(i = 0; i < navail_bytes; ++i) {
		 byte_buff[i]=fgetc(bmp);
	      }

	   }
	   b1 = *pbytes++;
	   ret |= b1 << nbits_left;
	   nbits_left += 8;
	   --navail_bytes;
	}
	nbits_left -= curr_size;
	ret &= code_mask[curr_size];
	return((int)(ret));
}




void inter_lace(void)
{

	switch(pass) {
	   case 0 :
	      n_Righe+=8;
	      if(n_Righe>=risoluzione_y) {
		 n_Righe=4;
		 pass++;
	      }
	      return;
	   case 1 :
	      n_Righe+=8;
	      if(n_Righe>=risoluzione_y) {
		 n_Righe=2;
		 pass++;
	      }
	      return;
	   case 2 :
	      n_Righe+=4;
	      if(n_Righe>=risoluzione_y) {
		 n_Righe=1;
		 pass++;
	      }
	      return;
	   case 3 :
	      n_Righe+=2;
	      return;
	}
}

/*

int adgust_file(void)
{
register int t;
unsigned char *datix;
register int rrx=risoluzione_x>>1;
char numb[5];

	if((datix=calloc(risoluzione_x,sizeof(char)))==NULL) {
	   box_error(-4);
	   getkey();
	   return(-1);
	}
	n_Righe=0;
	pass=0;
	charont(rrx+86,94,"  ");
	fseek(xout1,0L,SEEK_SET);
	fseek(xout2,0L,SEEK_SET);
	for(t=0;t<risoluzione_y;t++) {
	   fread(datix,risoluzione_x,1,xout1);
	   fseek(xout2,(long)risoluzione_x*n_Righe,SEEK_SET);
	   fwrite(datix,risoluzione_x,1,xout2);
	   itoa(t,numb,10);
	   charont(rrx+80,94,numb);
	   inter_lace();
	}
	fseek(xout1,0L,SEEK_SET);
	fseek(xout2,0L,SEEK_SET);
	charont(rrx+86,94,"  ");
	for(t=0;t<risoluzione_y;t++) {
	   fread(datix,risoluzione_x,1,xout2);
	   fwrite(datix,risoluzione_x,1,xout1);
	   itoa(t,numb,10);
	   charont(rrx+80,94,numb);
	}
	fseek(xout1,0L,SEEK_SET);
	fseek(xout2,0L,SEEK_SET);
	free(datix);
	return(0);
}



int adgust_file(void)
{
register int t;
unsigned char *datix;
register int rrx=risoluzione_x>>1;
char numb[5];

	if((datix=calloc(risoluzione_x[0],sizeof(char)))==NULL) {
	   box_error(-4);
	   getkey();
	   return(-1);
	}
	n_Righe=0;
	pass=0;
	charont(rrx+86,94,"  ");
	lseek(hout1,0L,SEEK_SET);
	lseek(hout2,0L,SEEK_SET);
	for(t=0;t<risoluzione_y[0];t++) {
	   read(hout1,datix,risoluzione_x[0]);
	   lseek(hout2,(long)risoluzione_x[0]*n_Righe,SEEK_SET);
	   write(hout2,datix,risoluzione_x[0]);
	   itoa(t,numb,10);
	   charont(rrx+80,94,numb);
	   inter_lace();
	}
	lseek(hout1,0L,SEEK_SET);
	lseek(hout2,0L,SEEK_SET);
	charont(rrx+86,94,"  ");
	for(t=0;t<risoluzione_y[0];t++) {
	   read(hout2,datix,risoluzione_x[0]);
	   write(hout1,datix,risoluzione_x[0]);
	   itoa(t,numb,10);
	   charont(rrx+80,94,numb);
	}
	lseek(hout1,0L,SEEK_SET);
	lseek(hout2,0L,SEEK_SET);
	free(datix);
	return(0);
}

*/


int decoder_GIF(void)
{
register int c,t=0;
unsigned char *sp;
int  code ,fc, oc, bufcnt,tx;
int  size=0;




	fread(&size,1,1,bmp);
	if (size < 0)  {
	   fine("Errore nel file GIF !!");
	}
	if (size < 2 || 9 < size) {
	   fine("Errore nel file GIF !!");
	}
	init_exp(size);
	t=0;
	oc=fc=0;
	sp = dstack;
	bufcnt = risoluzione_x;
	while((c = get_next_code()) != ending) {
	   if(c == clear) {
	      curr_size = size + 1;
	      slot = newcodes;
	      top_slot = 1 << curr_size;
	      while ((c = get_next_code()) == clear);
	      if(c == ending)
		 break;
	      if(c >= slot)
		 c=0;
	      oc = fc = c;
	      bex[t++] = c;
	      if (--bufcnt == 0) {

		 outarray(0,n_Righe,risoluzione_x,bex);
		 if(flag_IL) {
		     inter_lace();
		 }
		 else {
		     n_Righe++;
		 }

		 t=0;
		 bufcnt = risoluzione_x;
	      }
	   }
	   else {
	      code = c;
	      if (code >= slot) {
		 code = oc;
		 *sp++ = fc;
	      }
	      while (code >= newcodes) {
		 *sp++ = suffix[code];
		 code = prefix[code];
	      }
	      *sp++ = code;
	      if (slot < top_slot) {
		 suffix[slot] = fc = code;
		 prefix[slot++] = oc;
		 oc = c;
	      }
	      if (slot >= top_slot)
		 if (curr_size < 12) {
		    top_slot <<= 1;
		    ++curr_size;
		 }
	      while (sp > dstack) {
		 bex[t++] = *(--sp);
		 if(--bufcnt == 0) {

		    outarray(0,n_Righe,risoluzione_x,bex);
		    if(flag_IL) {
			inter_lace();
		    }
		    else {
			n_Righe++;
		    }
		    t=0;
		    bufcnt = risoluzione_x;
		 }
	      }
	   }
	}
	return(0);
}



int get_GIF_file(void)
{
register int t;
int piani;
char moment;
char sstr[10];
int trefin;


	fread(sstr,6,1,bmp);
	if(strncmp(sstr,"GIF",3)) {
	   fine("Errore nel file GIF !!");
	}
	fread(&risoluzione_x,2,1,bmp);
	fread(&risoluzione_y,2,1,bmp);
	moment=0;
	fread(&moment,1,1,bmp);
	flag_GCM=moment>>7;
	flag_CR=((moment>>4)&0x7)+1;
	piani=moment&0x7;
	fread(&sfondo,1,1,bmp);
	fread(&moment,1,1,bmp);
	if(flag_GCM) {
	   fin_color=1;
	   fin_color<<=piani+1;
	   trefin=fin_color*3;
	   fread(dacbox,trefin,1,bmp);
	   for(t=0;t<trefin;t++)
	      dacbox[t]>>=2;
	   for(t=trefin;t<0x300;t++)
	      dacbox[t]=0;
	}
	fread(&moment,1,1,bmp);
	while(moment==44) {
	   fread(&I_left,2,1,bmp);
	   fread(&I_top,2,1,bmp);
	   fread(&I_width,2,1,bmp);
	   fread(&I_height,2,1,bmp);
	   fread(&moment,1,1,bmp);
	   flag_GCM=moment>>7;
	   flag_IL=(moment>>6)&1;
	   flag_PIXEL=moment&0x7;
	}
}



int ciclo_compr(void)
{
register int i,hashentry;
int xdot,color;
unsigned char *getpix;


     for(n_Righe=0;n_Righe<risoluzione_y;n_Righe++) {
	inarray(0,n_Righe,risoluzione_x,bex);
	for(xdot = 0; xdot < risoluzione_x; xdot++) {
	   color = bex[xdot];
	   teststring[0] = ++lentest;
	   teststring[lentest] = color;
	   if (lentest == 1) {
	      lastentry = color;
	      continue;
	   }
	   if (lentest == 2)
	      hashcode = 301 * (teststring[1]+1);
	   hashcode *= (color + lentest);
	   hashentry = ++hashcode % MAXENTRY;
	   for( i = 0; i < MAXENTRY; i++) {
	      if (++hashentry >= MAXENTRY) hashentry = 0;
/*cmpextra*/  if (compextra(strlocn[hashentry],teststring,lentest+1) == 0)
		 break;
	      if (strlocn[hashentry] == 0) i = MAXENTRY;
	   }
	   if (strlocn[hashentry] != 0 && lentest < MAXTEST-3) {
	      lastentry = entrynum[hashentry];
	      continue;
	   }
	   raster(lastentry);
	   numentries++;
	   if (strlocn[hashentry] == 0) {
	      entrynum[hashentry] = numentries+endcode;
	      strlocn[hashentry] = nextentry;
	      movsb_mem(&en[nextentry],teststring,lentest+1);
/*toextra     copextra(nextentry,teststring,lentest+1);*/
	      nextentry += lentest+1;
	      numrealentries++;
	   }
	   teststring[0] = 1;
	   teststring[1] = color;
	   lentest = 1;
	   lastentry = color;
	   if ((numentries+endcode) == (1<<codebits))
	      codebits++;
	   if(numentries+endcode>4093 || numrealentries>(MAXENTRY*2)/3
				      || nextentry > 64000L-MAXTEST-3) {
	      raster(lastentry);
	      inittable();
	   }
	}
     }
     return(0);
}


void end_comp(void)
{
register int t;
int i;
	raster(lastentry);
	raster(endcode);
	i = 0;
	fwrite(&i,1,1,bmp);
	fwrite(";",1,1,bmp);
	      fwrite("B387",4,1,bmp);
	      fwrite(tasto_premuto,3,1,bmp);
	      fwrite(funztype,13,1,bmp);
	      fwrite((unsigned char *)&x_min,sizeof(xmin),1,bmp);
	      fwrite((unsigned char *)&x_max,sizeof(xmax),1,bmp);
	      fwrite((unsigned char *)&y_min,sizeof(ymin),1,bmp);
	      fwrite((unsigned char *)&y_max,sizeof(ymax),1,bmp);
	      fwrite((unsigned char *)&aval,sizeof(aval),1,bmp);
	      fwrite((unsigned char *)&bval,sizeof(bval),1,bmp);
	      fwrite((unsigned char *)&cval,sizeof(cval),1,bmp);
	      fwrite((unsigned char *)&zoom,sizeof(zoom),1,bmp);
	      fwrite((unsigned char *)&cr,sizeof(cr),1,bmp);
	      fwrite((unsigned char *)&cj,sizeof(cj),1,bmp);
	      fwrite((unsigned char *)&epsilon,sizeof(epsilon),1,bmp);
	      fwrite((unsigned char *)&kostre,sizeof(kostre),1,bmp);
	      fwrite((unsigned char *)&kostim,sizeof(kostim),1,bmp);
	      fwrite((unsigned char *)&step,sizeof(step),1,bmp);
	      fwrite((unsigned char *)&bailout,sizeof(bailout),1,bmp);
	      fwrite((unsigned char *)&iteraz,sizeof(iteraz),1,bmp);
	      fwrite((unsigned char *)&vid,sizeof(vid),1,bmp);
	      fwrite((unsigned char *)converg,2,1,bmp);
	      fwrite((unsigned char *)palettef,sizeof(palettef),1,bmp);
	      fwrite((unsigned char *)load_f,sizeof(load_f),1,bmp);
	fclose(bmp);
	printf("\007");
}




void savetodisk(void)
{
int bitsperpixel,x;
int i;

/*	if ((out=fopen(filename,"wb")) == NULL) {
	   box_error(-1);
	   return;
	}*/

	bitsperpixel = 0;
	for (i = colors; i >= 2; i /= 2 )
	   bitsperpixel++;
	startbits = bitsperpixel+1;
	if (colors == 2)
	   startbits++;
	clearcode = 1 << (startbits - 1);
	endcode = clearcode+1;
	fwrite("GIF87a",1,6,bmp);
	fwrite(&risoluzione_x,2,1,bmp);
	fwrite(&risoluzione_y,2,1,bmp);
	x = 128 + ((6-1)<<4) + (bitsperpixel-1);
	fwrite(&x,1,1,bmp);
	i = 0;
	fwrite(&i,1,1,bmp);
	fwrite(&i,1,1,bmp);
	if (colors == 256)
	   shftwrite(dacbox,colors);
	fwrite(",",1,1,bmp);
	i = 0;
	fwrite(&i,2,1,bmp);
	fwrite(&i,2,1,bmp);
	fwrite(&risoluzione_x,2,1,bmp);
	fwrite(&risoluzione_y,2,1,bmp);
	i = 0;
	fwrite(&i,1,1,bmp);
	bitsperpixel = startbits - 1;
	fwrite(&bitsperpixel,1,1,bmp);
	codebits = startbits;
	raster(9999);
	inittable();
}








int control_gif(char *flen,char rw)
{

	navail_bytes = 0;
	nbits_left = 0;
	pass=0;
	n_Righe=0;

	if(!rw) {
           get_GIF_file();
	   video();
	   decoder_GIF();
	   min_max();
	}

	else {
	   if((en=calloc(65000,sizeof(char)))==NULL) {
	      sect_text();
	      boxerror("No memory error !!!");
	      return;
	   }
	   if(apre(flen,".gif")==27)
	      return;
	   savetodisk();
	   ciclo_compr();
	   end_comp();
	   free(en);
	}

}

#undef GIFLIB
