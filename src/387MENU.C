#define MENU387

#include "b387.h"
#include "tcu.h"

extern char cvg[12];
extern char cvgval;
void FpuFtoa(long double,char *);

static char str[6];
char goflag=0;
MENU mainmenu,secondmenu,termenu;
FORM form;
FIELD_VALUE fval;

int selezione,rkey=ESC_ESC;

static char *IFS_type[]={"Sierpinsky",
			 "Nullo",
		     NULL};

static char *Barnsley_type[]={"Barnsley329",
		     "Nullo",
		     NULL};


static char *mandel_type[]={"Mandel",
		     "MandEpsilon",
		     "MandelSin",
		     "MandelCos",
		     "MandelExp",
		     NULL};

static char *julia_type[]={"Julia",
		    "JuliaEpsilon",
		    "JuliaSin",
		    "JuliaCos",
		    "JuliaExp",
		    NULL};


static char *fractal_type[]={"Mandel      =>",
		      "Julia       =>",
		      "Biomprph",
		      "Barnsley    =>",
		      "IFS         =>",
		      "Prova",
		      NULL};

static char *pixel_type[]={"Hopalong    =>",
		    "Plankton",
		    "Popcorn",
		    "Henon",
		    "Martin",
		    NULL};

static char *hopalong_type[]={"Hopalong",
		    "HopSin",
		    "HopCos",
		    "HopSqrt",
		    "HopSin*Cos",
		    NULL};


static char *system_type[]={"Motion",
		     "Call Editor",
		     "DOS exit",
		     NULL};

static char *data_type[]={"Show Data",
		   "Change Data",
		   "Show Picfiles",
		   "Load Picture",
		   "Show Datafiles",
		   "Load Datafile",
		   "Save Datafile",
		   NULL};


static char *main_type[] = { "Go Draw",   /* Menu options */
		      "Fractals        =>  ",
		      "Pixel Fractal   =>  ",
		      "Data & Files    =>  ",
		      "System          =>  ",
		      "Configure       =>  ",
		      "Quit",
		      NULL};

void boxerror(char *sst)
{
	prepare_notice (colour_attrib (WHITE, RED),
			 colour_attrib (YELLOW, RED),
			 BOX_SINGLE);
	notice_text ("ERROR :");
	notice_text (sst);   /* Prompt bit for input */
	display_notice (1,20);   /* Put up prompt & get string */

	clear_notice ();                  /* Get rid of prompt definition */
}


void outcoord(void)
{
int kk;

   prepare_notice (colour_attrib (WHITE, LIGHTGRAY),
		   colour_attrib (BLACK, LIGHTGRAY),
		   BOX_SINGLE);


	notice_text("B387.exe by Michele Morrone.");
	notice_text("");
	notice_text("Type ú ú ú ú %s",funztype);
	notice_text("");
	   notice_text("Xmin ú ú ú ú %4.18lf",(double)x_min);
	   notice_text("Xmax ú ú ú ú %4.18lf",(double)x_max);
	   notice_text("Ymin ú ú ú ú %4.18lf",(double)y_min);
	   notice_text("Ymax ú ú ú ú %4.18lf",(double)y_max);
           notice_text("Aval ú ú ú ú %4.18lf",(double)aval);
	   notice_text("Bval ú ú ú ú %4.18lf",(double)bval);
	   notice_text("Cval ú ú ú ú %4.18lf",(double)cval);
	   notice_text("Costreal ú ú %3.18lf",(double)cr);
	   notice_text("Costimag ú ú %3.18lf",(double)cj);
	   notice_text("KostanteRe ú %3.18lf",kostre);
	   notice_text("KostanteIm ú %3.18lf",kostim);
	   notice_text("Epsilon  ú ú %3.18lf",epsilon);
	   notice_text("Bailout  ú ú %d",bailout);
	notice_text("Iterazioni ú %ld",iteraz);
	notice_text("Convergenzaú %s",cvg);
	notice_text("RisúVideo  ú %d",vid);
	notice_text("Palette  ú ú %s",palettef);
	notice_text("DataFile ú ú %s",load_f);
	notice_text("Picture  ú ú %s",savepicture);
	display_notice (1,1);
	clear_notice();


}

void hotkeyhelp(void)
{

	prepare_notice (colour_attrib (WHITE, GREEN),
		   colour_attrib (BLACK, GREEN),
		   BOX_SINGLE);

	notice_text("B387.exe by Michele Morrone.");
	notice_text("");
	notice_text("HotKey Help");
	notice_text("");

	notice_text("F1      Help.");
	notice_text("{1..8}  Livello di interpolazione punti (1=No interpolazione).");
	notice_text("F5      Modalita' 320x200x256.   Vga & Mcga.");
	notice_text("F6      Modalita' 640x480x256.   Tseng3000 or Tseng4000 mode.");
	notice_text("F7      Modalita' 800x600x256.   Tseng3000 or Tseng4000 & Multisync mode.");
	notice_text("F8      Modalita' 1024x768x256.  Tseng4000 1Mb & Multisync mode.");
	notice_text("Tab     Elenco parametri.");
	notice_text("L       Carica ad ogni pressione successiva di un tasto una delle palette");
	notice_text("        contenute sul disco.Esc per selezionare la palette e tornare al");
	notice_text("        modo grafico.");
	notice_text("S       Salva il file,col nome specificato.");
	notice_text("F10     Visualizza la palette corrente.Se non si e' in modo pixel ..");
	notice_text("        Un tasto per tornare in modo");
	notice_text("        grafico.");
	notice_text("");
	notice_text("Press Return. . .");
	display_notice (1,1);
	clear_notice();

	prepare_notice (colour_attrib (WHITE, GREEN),
		   colour_attrib (BLACK, GREEN),
		   BOX_SINGLE);
	notice_text("Enter  Zoom usabile con Mouse o con tastiera:");
	notice_text("           Mouse :");
	notice_text("             Tasto destro   : ingrandisce o rimpicciolisce il box.");
	notice_text("             Tasto sinistro : Go zoom.");
	notice_text("             No tasti       : Muove il box di zoom.");
	notice_text("           Tastiera :");
	notice_text("             PgUp : Ingrandisce il box.");
	notice_text("             PgDn : Rimpicciolisce il box.");
	notice_text("             +    : Aumenta lo step di spostamento o ingrandimento box.");
	notice_text("             -    : Diminuisce lo step di spostamento o ingrandimento box.");
	notice_text("             Freccie : Spostamento box.");
	notice_text("C       Cambio parametri di esplorazione.");
	notice_text("           Enter o Frecce: per confermare il valore.");
	notice_text("           Esc o CTRL-C  : per abortire lasciando i vecchi parametri.");
	notice_text("	        PgUp o PgDn   : per uscire confermando i cambiamenti.");
	notice_text("+       Aumenta di 8 unita' il numero di iterazioni.");
	notice_text("-       Diminuisce di 8 unita' il numero di iterazioni.");
	notice_text("W	     Scrive su di un file i parametri dell'esplorazione,successivamente");
	notice_text("	     richiamabili da programma. (DataFile)");
	notice_text("Esc     Ritorno al menu.");
	notice_text("");
	notice_text("Press Return. . .");
	display_notice (1,1);
	clear_notice();

}




int getmotion(void)
{
int kk;

	load_form (&form, "changem");
	display_form (&form, 1, 1);

	get_field_id(&form,"cou",&kk);
	fval.v_int=counterMotion;
	put_field(&form,kk,&fval);

	get_field_id(&form,"sv",&kk);
	fval.v_logical=flagSave;
	put_field(&form,kk,&fval);

	get_field_id(&form,"zm",&kk);
	fval.v_float=zinc;
	put_field(&form,kk,&fval);

	get_field_id(&form,"xmn",&kk);
	fval.v_float=xmininc;
	put_field(&form,kk,&fval);

	get_field_id(&form,"xmx",&kk);
	fval.v_float=xmaxinc;
	put_field(&form,kk,&fval);

	get_field_id(&form,"ymn",&kk);
	fval.v_float=ymininc;
	put_field(&form,kk,&fval);

	get_field_id(&form,"ymx",&kk);
	fval.v_float=ymaxinc;
	put_field(&form,kk,&fval);

	get_field_id(&form,"avl",&kk);
	fval.v_float=avalinc;
	put_field(&form,kk,&fval);

	get_field_id(&form,"bvl",&kk);
	fval.v_float=bvalinc;
	put_field(&form,kk,&fval);

	get_field_id(&form,"cvl",&kk);
	fval.v_float=cvalinc;
	put_field(&form,kk,&fval);

	get_field_id(&form,"dvl",&kk);
	fval.v_float=dvalinc;
	put_field(&form,kk,&fval);

	get_field_id(&form,"ar",&kk);
	fval.v_float=crinc;
	put_field(&form,kk,&fval);

	get_field_id(&form,"ai",&kk);
	fval.v_float=cjinc;
	put_field(&form,kk,&fval);

	get_field_id(&form,"mr",&kk);
	fval.v_float=kostreinc;
	put_field(&form,kk,&fval);

	get_field_id(&form,"mi",&kk);
	fval.v_float=kostiminc;
	put_field(&form,kk,&fval);

	get_field_id(&form,"eps",&kk);
	fval.v_float=epsiloninc;
	put_field(&form,kk,&fval);

	get_field_id(&form,"fn",&kk);
	fval.v_string=funztype;
	put_field(&form,kk,&fval);


	edit_form (&form, &rkey);

	if(rkey==1) {
	   remove_form (&form);
	   unload_form (&form);
	   return(27);
	}

	get_field_id(&form,"cou",&kk);
	get_field(&form,kk,&fval);
	counterMotion=fval.v_int;

	get_field_id(&form,"sv",&kk);
	get_field(&form,kk,&fval);
	flagSave=fval.v_logical;

	get_field_id(&form,"zm",&kk);
	get_field(&form,kk,&fval);
	zinc=fval.v_float;

	get_field_id(&form,"xmn",&kk);
	get_field(&form,kk,&fval);
	xmininc=fval.v_float;

	get_field_id(&form,"xmx",&kk);
	get_field(&form,kk,&fval);
	xmaxinc=fval.v_float;

	get_field_id(&form,"ymn",&kk);
	get_field(&form,kk,&fval);
	ymininc=fval.v_float;

	get_field_id(&form,"ymx",&kk);
	get_field(&form,kk,&fval);
	ymaxinc=fval.v_float;

	get_field_id(&form,"avl",&kk);
	get_field(&form,kk,&fval);
	avalinc=fval.v_float;

	get_field_id(&form,"bvl",&kk);
	get_field(&form,kk,&fval);
	bvalinc=fval.v_float;

	get_field_id(&form,"cvl",&kk);
	get_field(&form,kk,&fval);
	cvalinc=fval.v_float;

	get_field_id(&form,"dvl",&kk);
	get_field(&form,kk,&fval);
	dvalinc=fval.v_float;


	get_field_id(&form,"ar",&kk);
	get_field(&form,kk,&fval);
	crinc=fval.v_float;

	get_field_id(&form,"ai",&kk);
	get_field(&form,kk,&fval);
	cjinc=fval.v_float;

	get_field_id(&form,"mr",&kk);
	get_field(&form,kk,&fval);
	kostreinc=fval.v_float;

	get_field_id(&form,"mi",&kk);
	get_field(&form,kk,&fval);
	kostiminc=fval.v_float;

	get_field_id(&form,"eps",&kk);
	get_field(&form,kk,&fval);
	epsiloninc=fval.v_float;

	remove_form (&form);
	unload_form (&form);
	if(!goflag) {
	   boxerror("No function selected !!");
	   return(27);
	}
	return(0);
}


void changeval(void)
{
int kk;




	load_form (&form, "changef");
	display_form (&form, 1, 1);

	get_field_id(&form,"xmn",&kk);
	fval.v_float=x_min;
	put_field(&form,kk,&fval);

	get_field_id(&form,"xmx",&kk);
	fval.v_float=x_max;
	put_field(&form,kk,&fval);

	get_field_id(&form,"ymn",&kk);
	fval.v_float=y_min;
	put_field(&form,kk,&fval);

	get_field_id(&form,"ymx",&kk);
	fval.v_float=y_max;
	put_field(&form,kk,&fval);

	get_field_id(&form,"avl",&kk);
	fval.v_float=aval;
	put_field(&form,kk,&fval);

	get_field_id(&form,"bvl",&kk);
	fval.v_float=bval;
	put_field(&form,kk,&fval);

	get_field_id(&form,"cvl",&kk);
	fval.v_float=cval;
	put_field(&form,kk,&fval);

	get_field_id(&form,"dvl",&kk);
	fval.v_float=dval;
	put_field(&form,kk,&fval);


	get_field_id(&form,"ar",&kk);
	fval.v_float=cr;
	put_field(&form,kk,&fval);

	get_field_id(&form,"ai",&kk);
	fval.v_float=cj;
	put_field(&form,kk,&fval);

	get_field_id(&form,"mr",&kk);
	fval.v_float=kostre;
	put_field(&form,kk,&fval);

	get_field_id(&form,"mi",&kk);
	fval.v_float=kostim;
	put_field(&form,kk,&fval);

	get_field_id(&form,"eps",&kk);
	fval.v_float=epsilon;
	put_field(&form,kk,&fval);

	get_field_id(&form,"ite",&kk);
	fval.v_float=iteraz;
	put_field(&form,kk,&fval);

	get_field_id(&form,"bai",&kk);
	fval.v_int=bailout;
	put_field(&form,kk,&fval);

	get_field_id(&form,"cv",&kk);
	fval.v_string=converg;
	put_field(&form,kk,&fval);

	get_field_id(&form,"pal",&kk);
	fval.v_string=palettef;
	put_field(&form,kk,&fval);

	get_field_id(&form,"df",&kk);
	fval.v_string=load_f;
	put_field(&form,kk,&fval);

	get_field_id(&form,"pic",&kk);
	fval.v_string=savepicture;
	put_field(&form,kk,&fval);

	get_field_id(&form,"fn",&kk);
	fval.v_string=funztype;
	put_field(&form,kk,&fval);



	edit_form (&form, &rkey);

	if(rkey==1) {
	   remove_form (&form);
	   unload_form (&form);
	   return;
	}

	get_field_id(&form,"xmn",&kk);
	get_field(&form,kk,&fval);
	xmin=x_min=fval.v_float;

	get_field_id(&form,"xmx",&kk);
	get_field(&form,kk,&fval);
	xmax=x_max=fval.v_float;

	get_field_id(&form,"ymn",&kk);
	get_field(&form,kk,&fval);
	ymin=y_min=fval.v_float;

	get_field_id(&form,"ymx",&kk);
	get_field(&form,kk,&fval);
	ymax=y_max=fval.v_float;

	get_field_id(&form,"avl",&kk);
	get_field(&form,kk,&fval);
	aval=fval.v_float;

	get_field_id(&form,"bvl",&kk);
	get_field(&form,kk,&fval);
	bval=fval.v_float;

	get_field_id(&form,"cvl",&kk);
	get_field(&form,kk,&fval);
	cval=fval.v_float;

	get_field_id(&form,"dvl",&kk);
	get_field(&form,kk,&fval);
	dval=fval.v_float;

	get_field_id(&form,"ar",&kk);
	get_field(&form,kk,&fval);
	cr=fval.v_float;

	get_field_id(&form,"ai",&kk);
	get_field(&form,kk,&fval);
	cj=fval.v_float;

	get_field_id(&form,"mr",&kk);
	get_field(&form,kk,&fval);
	kostre=fval.v_float;

	get_field_id(&form,"mi",&kk);
	get_field(&form,kk,&fval);
	kostim=fval.v_float;

	get_field_id(&form,"eps",&kk);
	get_field(&form,kk,&fval);
	epsilon=fval.v_float;

	get_field_id(&form,"ite",&kk);
	get_field(&form,kk,&fval);
	iteraz=fval.v_float;

	get_field_id(&form,"bai",&kk);
	get_field(&form,kk,&fval);
	bailout=fval.v_int;

	get_field_id(&form,"cv",&kk);
	fval.v_string=converg;
	get_field(&form,kk,&fval);
	switch(cvgval) {
	    case 1 :
	       conv_1(converg);
	       break;
	    case 2 :
	       conv_2(converg);
	       break;
	    case 3 :
	       conv_3(converg);
	       break;
	}
	get_field_id(&form,"pal",&kk);
	fval.v_string=palettef;
	get_field(&form,kk,&fval);

	get_field_id(&form,"df",&kk);
	fval.v_string=load_f;
	get_field(&form,kk,&fval);

	get_field_id(&form,"pic",&kk);
	fval.v_string=savepicture;
	get_field(&form,kk,&fval);

	get_field_id(&form,"fn",&kk);
	fval.v_string=funztype;
	get_field(&form,kk,&fval);

	remove_form (&form);
	unload_form (&form);

}




void select_val(char *str,char *name)
{
char bufx[40];
char sst[40];

	prepare_notice (colour_attrib (BLACK, LIGHTGRAY),
			 colour_attrib (YELLOW, LIGHTGRAY),
                         BOX_SINGLE);
	notice_text("Old DataName      : %s",str);
	notice_text (name);   /* Prompt bit for input */

	 bufx[0] = 30;    /* Maximum length of input string */
	 strcpy(&bufx[3],str);
	 prompt_input (20, 2, bufx, colour_attrib (WHITE, BLACK)); /* Prompt */

	 display_notice (15,4);   /* Put up prompt & get string */

	 clear_notice ();                  /* Get rid of prompt definition */
	 if(bufx[1]) {
	    strcpy(str,&bufx[2]);
	 }
}


int terlevel(void)
{

		switch(selezione) {
		      case 1 :
			 selezione = define_menu (&termenu,
				 "Mandel",
				 colour_attrib (YELLOW, LIGHTGRAY),       /* Title */
				 colour_attrib (BLACK, LIGHTGRAY),         /* Box */
				 colour_attrib (BLUE, LIGHTGRAY),     /* Options */
				 colour_attrib (WHITE, BLUE),        /* Selected */
				 colour_attrib (LIGHTGRAY, CYAN), /* Deselezioneed */
				 255,
				 BOX_SINGLE,
				 mandel_type);
			if (selezione != MENU_OK)
			   fine("Error in defining menu!");
			selezione = display_menu (&termenu, 21, 3);
			selezione = read_menu_selection (&termenu);
			switch(selezione) {
			   case 1 :
			      strcpy(str,"M1");
			      strcpy(funztype,"Mandel");
			      break;
			   case 2 :
			      strcpy(str,"M2");
			      strcpy(funztype,"MandelEps");
			      break;
			   case 3 :
			      strcpy(str,"M3");
			      strcpy(funztype,"MandelSin");
			      break;
			   case 4 :
			      strcpy(str,"M4");
			      strcpy(funztype,"MandelCos");
			      break;
			   case 5 :
			      strcpy(str,"M5");
			      strcpy(funztype,"MandelExp");
			      break;
			   default :
			      remove_menu (&termenu);
			      return(27);

			}
			remove_menu (&termenu);
			if(searchfunz(funztype)==27)
			   return(27);
			get_single_fileval();
			goflag=1;
			return(0);
		      case 2 :
			 selezione = define_menu (&termenu,
				 "Julia",
				 colour_attrib (YELLOW, LIGHTGRAY),       /* Title */
				 colour_attrib (BLACK, LIGHTGRAY),         /* Box */
				 colour_attrib (BLUE, LIGHTGRAY),     /* Options */
				 colour_attrib (WHITE, BLUE),        /* Selected */
				 colour_attrib (LIGHTGRAY, CYAN), /* Deselezioneed */
				 255,
				 BOX_SINGLE,
				 julia_type);
			if (selezione != MENU_OK)
			   fine("Error in defining menu!");
			selezione = display_menu (&termenu, 21, 3);
			selezione = read_menu_selection (&termenu);
			switch(selezione) {
			   case 1 :
			      strcpy(str,"J1");
			      strcpy(funztype,"Julia");
			      break;
			   case 2 :
			      strcpy(str,"J2");
                              strcpy(funztype,"JuliaEps");
			      break;
			   case 3 :
			      strcpy(str,"J3");
			      strcpy(funztype,"JuliaSin");
			      break;
			   case 4 :
			      strcpy(str,"J4");
			      strcpy(funztype,"JuliaCos");
			      break;
			   case 5 :
			      strcpy(str,"J5");
			      strcpy(funztype,"JuliaExp");
			      break;
			   default :
			      remove_menu (&termenu);
			      return(27);
			}
			remove_menu (&termenu);
			if(searchfunz(funztype)==27)
			   return(27);
			get_single_fileval();
			goflag=1;
			return(0);
		      case 3 :
			 strcpy(str,"B1");
			 strcpy(funztype,"Biomorph");
			 if(searchfunz(funztype)==27)
			    return(27);
			 get_single_fileval();
			 goflag=1;
			 return(0);
		      case 4 :
			 selezione = define_menu (&termenu,
				 "Barnsley",
				 colour_attrib (YELLOW, LIGHTGRAY),       /* Title */
				 colour_attrib (BLACK, LIGHTGRAY),         /* Box */
				 colour_attrib (BLUE, LIGHTGRAY),     /* Options */
				 colour_attrib (WHITE, BLUE),        /* Selected */
				 colour_attrib (LIGHTGRAY, CYAN), /* Deselezioneed */
				 255,
				 BOX_SINGLE,
				 Barnsley_type);
			if (selezione != MENU_OK)
			   fine("Error in defining menu!");
			selezione = display_menu (&termenu, 21, 3);
			selezione = read_menu_selection (&termenu);
			switch(selezione) {
			   case 1 :
			      strcpy(str,"F1");
			      strcpy(funztype,"Barnsley329");
			      break;
			   default :
			      remove_menu (&termenu);
			      return(27);
			}
			remove_menu (&termenu);
			if(searchfunz(funztype)==27)
			   return(27);
			get_single_fileval();
			goflag=1;
			return(0);
		      case 5 :
			 selezione = define_menu (&termenu,
				 "IFS",
				 colour_attrib (YELLOW, LIGHTGRAY),       /* Title */
				 colour_attrib (BLACK, LIGHTGRAY),         /* Box */
				 colour_attrib (BLUE, LIGHTGRAY),     /* Options */
				 colour_attrib (WHITE, BLUE),        /* Selected */
				 colour_attrib (LIGHTGRAY, CYAN), /* Deselezioneed */
				 255,
				 BOX_SINGLE,
				 IFS_type);
			if (selezione != MENU_OK)
			   fine("Error in defining menu!");
			selezione = display_menu (&termenu, 21, 3);
			selezione = read_menu_selection (&termenu);
			switch(selezione) {
			   case 1 :
			      strcpy(str,"I1");
			      strcpy(funztype,"Sierpinski");
			      break;
			   default :
			      remove_menu (&termenu);
			      return(27);
			
			}
			remove_menu (&termenu);
			if(searchfunz(funztype)==27)
			   return(27);
			get_single_fileval();
			goflag=1;
			return(0);
		      case 6 :
			 strcpy(str,"S+");
			 if(searchfunz(funztype)==27)
			    return(27);
			 strcpy(funztype,"Julia");
			 get_single_fileval();
			 goflag=1;
			 return(0);
		      default :
			 return(27);
	       }

}

int terlevel2(void)
{

		switch(selezione) {
		      case 1 :
			 selezione = define_menu (&termenu,
				 "Hopalong",
				 colour_attrib (YELLOW, LIGHTGRAY),       /* Title */
				 colour_attrib (BLACK, LIGHTGRAY),         /* Box */
				 colour_attrib (BLUE, LIGHTGRAY),     /* Options */
				 colour_attrib (WHITE, BLUE),        /* Selected */
				 colour_attrib (LIGHTGRAY, CYAN), /* Deselezioneed */
				 255,
				 BOX_SINGLE,
				 hopalong_type);
			if (selezione != MENU_OK)
			   fine("Error in defining menu!");
			selezione = display_menu (&termenu, 21, 3);
			selezione = read_menu_selection (&termenu);
			remove_menu (&termenu);
			switch(selezione) {
			   case 1 :
			      strcpy(str,"H1");
                              strcpy(funztype,"Hopalong");
			      break;
			   case 2 :
			      strcpy(str,"H2");
			      strcpy(funztype,"HopSin");
			      break;
			   case 3 :
			      strcpy(str,"H3");
                              strcpy(funztype,"HopCos");
			      break;
			   case 4 :
			      strcpy(str,"H4");
			      strcpy(funztype,"HopSqrt");
			      break;
			   case 5 :
			      strcpy(str,"H5");
			      strcpy(funztype,"HopSinCos");
			      break;
			   default :
			      remove_menu (&termenu);
			      return(27);

			}
			if(searchfunz(funztype)==27)
			   return(27);
			get_single_fileval();
			goflag=1;
			return(0);
		      case 2 :
			 strcpy(str,"P1");
			 strcpy(funztype,"Plankton");
			 if(searchfunz(funztype)==27)
			    return(27);
			 get_single_fileval();
			 goflag=1;
			 return(0);
		      case 3 :
			 strcpy(str,"P2");
			 strcpy(funztype,"Popcorn");
			 if(searchfunz(funztype)==27)
			    return(27);
			 get_single_fileval();
			 goflag=1;
			 return(0);
		      case 4 :
			 strcpy(str,"P3");
			 strcpy(funztype,"Henon");
			 if(searchfunz(funztype)==27)
			    return(27);
			 get_single_fileval();
			 goflag=1;
			 return(0);
		      case 5 :
			 strcpy(str,"P4");
			 strcpy(funztype,"Martin");
			 if(searchfunz(funztype)==27)
			    return(27);
			 get_single_fileval();
			 goflag=1;
			 return(0);
		      default :
			 return(27);
	       }

}




int secondlevel(void)
{

	switch(selezione) {
	   case 1 :
	       strcpy(str,"");
	       if(!goflag)
		  return(0);
	       xmin=x_min;
	       xmax=x_max;
	       ymin=y_min;
	       ymax=y_max;
	       strcpy(str,"D9");
	       return(0);
	   case 2 :
	       selezione = define_menu (&secondmenu,
			 "Fractals",
			 colour_attrib (YELLOW, BLUE),       /* Title */
			 colour_attrib (WHITE, BLUE),         /* Box */
			 colour_attrib (LIGHTCYAN, BLUE),     /* Options */
			 colour_attrib (WHITE, CYAN),        /* Selected */
			 colour_attrib (LIGHTGRAY, CYAN), /* Deselezioneed */
			 ESC_ESC,
			 BOX_SINGLE,
			 fractal_type);
		if (selezione != MENU_OK)
		   fine("Error in defining menu!");
		selezione = display_menu (&secondmenu, 11, 2);
		do {
		   if((selezione = read_menu_selection (&secondmenu))==-1) {
		      remove_menu (&secondmenu);
		      return(27);
		   }
		} while(terlevel()==27);
		remove_menu (&secondmenu);
		return(0);
	    case 3 :
	       selezione = define_menu (&secondmenu,
			 "Pixel Fract",
			 colour_attrib (YELLOW, BLUE),       /* Title */
			 colour_attrib (WHITE, BLUE),         /* Box */
			 colour_attrib (LIGHTCYAN, BLUE),     /* Options */
			 colour_attrib (WHITE, CYAN),        /* Selected */
			 colour_attrib (LIGHTGRAY, CYAN), /* Deselezioneed */
			 ESC_ESC,
			 BOX_SINGLE,
			 pixel_type);
		if (selezione != MENU_OK)
		   fine("Error in defining menu!");
		selezione = display_menu (&secondmenu, 11, 2);
		do {
		   if((selezione = read_menu_selection (&secondmenu))==-1) {
		      remove_menu (&secondmenu);
		      return(27);
		   }
		} while(terlevel2()==27);
		remove_menu (&secondmenu);
		return(0);
	    case 4 :
	       selezione = define_menu (&secondmenu,
			 "Data",
			 colour_attrib (YELLOW, BLUE),       /* Title */
			 colour_attrib (WHITE, BLUE),         /* Box */
			 colour_attrib (LIGHTCYAN, BLUE),     /* Options */
			 colour_attrib (WHITE, CYAN),        /* Selected */
			 colour_attrib (LIGHTGRAY, CYAN), /* Deselezioneed */
			 ESC_ESC,
			 BOX_SINGLE,
			 data_type);
		if (selezione != MENU_OK)
		   fine("Error in defining menu!");
		selezione = display_menu (&secondmenu, 11, 2);
		selezione = read_menu_selection (&secondmenu);
		remove_menu (&secondmenu);
		switch(selezione) {
		   case 1 :
		      strcpy(str,"D1");
		      break;
		   case 2 :
		      strcpy(str,"D2");
		      break;
		   case 3 :
		      strcpy(str,"D3");
		      break;
		   case 4 :
		      strcpy(str,"D4");
		      break;
		   case 5 :
		      strcpy(str,"D5");
		      break;
		   case 6 :
		      strcpy(str,"D6");
		      break;
		   case 7 :
		      strcpy(str,"D7");
		      break;
		   default :
		      return(27);
		}

		return(0);
	    case 5 :
	       selezione = define_menu (&secondmenu,
			 "System",
			 colour_attrib (YELLOW, BLUE),       /* Title */
			 colour_attrib (WHITE, BLUE),         /* Box */
			 colour_attrib (LIGHTCYAN, BLUE),     /* Options */
			 colour_attrib (WHITE, CYAN),        /* Selected */
			 colour_attrib (LIGHTGRAY, CYAN), /* Deselezioneed */
			 ESC_ESC,
			 BOX_SINGLE,
			 system_type);
		if (selezione != MENU_OK)
		   fine("Error in defining menu!");
		selezione = display_menu (&secondmenu, 11, 2);
		selezione = read_menu_selection (&secondmenu);
		remove_menu (&secondmenu);
		switch(selezione) {
		   case 1 :
		      strcpy(str,"S1");
		      break;
		   case 2 :
		      strcpy(str,"S2");
		      break;
		   case 3 :
		      strcpy(str,"S3");
		      break;
		   default :
		      return(27);
		}
		return(0);
	    case 6 :
	       return(27);
	    case 7 :
	       remove_menu (&mainmenu);
	       fine("B387 by Mik27.");
	}
	return(0);

}

void selectfractal(void)
{


	while(1) {
	sect_text();
	selezione = define_menu (&mainmenu,
			 "Main Menu",
			 colour_attrib (YELLOW, CYAN),       /* Title */
			 colour_attrib (BLACK, CYAN),         /* Box */
			 colour_attrib (WHITE, CYAN),     /* Options */
			 colour_attrib (WHITE, BLACK),        /* Selected */
			 colour_attrib (LIGHTGRAY, CYAN), /* Deselezioneed */
			 255,
			 BOX_SINGLE,
			 main_type);

	if (selezione != MENU_OK)
	   fine("No Memori Error !!!");
	selezione = display_menu (&mainmenu, 1, 1);
	do {
	   do {
	      selezione = read_menu_selection (&mainmenu);
	   } while(selezione<1 || selezione>7);
	} while(secondlevel()==27);
	remove_menu (&mainmenu);




	if(selectparm(str,1)==27)
	   continue;
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

}




