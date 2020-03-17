
#define BOX_SINGLE                  1      /* Box bordered by single lines */
#define BOX_DOUBLE                  2      /* Box bordered by double lines */


#define ESC_ESC                  0x01      /* ESCAPE key exit */
#define ESC_PGUP                 0x02      /* Page Up key exit */
#define ESC_PGDN                 0x04      /* Page Down key exit */
#define ESC_CLEFT                0x08      /* Cursor left exit */
#define ESC_CRIGHT               0x10      /* Cursor right exit */
#define ESC_FUNC                 0x20      /* Function key exit */
#define ESC_CNTL_C               0x40      /* Control-C exit */


#define MENU_TITLE                  1      /* Title */
#define MENU_BOX                    2      /* Surrounding box */
#define MENU_OPTION                 3      /* Option line */
#define MENU_SELECT                 4      /* Selected option line */
#define MENU_UNAVAIL                5      /* Unavailable option line */


#define MENU_OK                     0      /* Menu function success */
#define MENU_ERROR                  1      /* Error in menu function call */


#define NOTICE_OK                   0      /* Prompt/Notice success */
#define NOTICE_ERROR                1      /* Error in prompt/notice fn. */


#define FORM_OK           1                /* Successful form operation */
#define FORM_ERROR        2                /* Unsuccessful form operation */


#define FORM_EDIT         1                /* Enable edit ability for field */
#define FORM_ECHO         2                /* Echo field when input */
#define FORM_NOEDIT       3                /* Disable edit ability for field */
#define FORM_NOECHO       4                /* No echo on input field */


#define FRM_MAXFIELD      128              /* Maximum fields on form */


#define FLD_INT           1                /* Integer (32 bits) */
#define FLD_FLOAT         2                /* Floating point (64 bits) */
#define FLD_STRING        3                /* String, ASCIIZ format */
#define FLD_DATE          4                /* Date (01.01.70 ...) */
#define FLD_LOGICAL       5                /* True/False, Yes/No, On/Off */


#define FLD_DAYFIRST      1                /* Dates with day number first */
#define FLD_MONTHFIRST    2                /* Dates with month number first */


#define FLD_TRUEFALSE     1                /* Logicals as TRUE/FALSE */
#define FLD_YESNO         2                /* Logicals as YES/NO */
#define FLD_ONOFF         3                /* Logicals as ON/OFF */
#define FLD_JANEIN        4                /* Logicals as JA/NEIN */


#define FLD_ESCNEXT       1                /* NEXT FIELD field edit escape */
#define FLD_ESCPREV       2                /* PREVIOUS FIELD escape */
#define FLD_ESCFIRST      3                /* FIRST FIELD escape */
#define FLD_ESCLAST       4                /* LAST FIELD escape */
#define FLD_ESCESC       -1                /* ESC key escape */
#define FLD_ESCCNTLC     -2                /* CNTL/C key escape */
#define FLD_ESCPGUP      -3                /* Page-Up key escape */
#define FLD_ESCPGDN      -4                /* Page-Down key escape */


typedef struct {
   short               x, y;               /* X and Y coords of current pos */
   short               height,             /* External size of menu */
                       width;              /* External width of menu */
   char               *title;              /* Title of menu */
   unsigned char       title_attrib,       /* Attributes of title */
                       box_attrib,         /* Attributes of surrounding box */
                       option_attrib,      /* Attributes of menu option */
                       select_attrib,      /* Attributes of selected option */
                       unavail_attrib;     /* Attributes of unavailables */
   unsigned char       escape_keys;        /* Keys which allow exit */
   unsigned char       box_type;           /* Type of external box */
   unsigned char       last_selection;     /* Last selected option */
   void              (*help_fn)(int);      /* User's help function */
   char              **options;            /* Pointer to list of options */
   unsigned char      *cover_zone;         /* Pointer to covered screen */
   unsigned long       option_avail;       /* Option availability flags */
} MENU;


typedef struct {
   unsigned char       linecolour,         /* Colour of header line */
                       optioncolour,       /* Colour of possible options */
                       selectcolour;       /* Colour of selected option */
   char              **headings;           /* List of header titles */
   MENU              **menus;              /* List of menus */
   void              (*help_fn)(int);      /* User's help function */
   unsigned char       selected,           /* Currently selected menu */
                       use_mode,           /* Current mode of use */
                      *cover_zone;         /* Screen contents area */
} PULLDOWN;


typedef union {
   long                v_int;              /* Integer field value */
   double              v_float;            /* Floating field value */
   char               *v_string;           /* String field value */
   unsigned short      v_date;             /* Date value (days -> 01.01.70) */
   unsigned char       v_logical;          /* Logical field value */
} FIELD_VALUE;


typedef struct {
   char                   f_name[9];       /* Field name */
   unsigned char          f_type,          /* Type of field */
                          f_size,          /* Width of field on screen */
                          f_decimal,       /* Decimal places if numeric */
                          f_present,       /* Presentation form of data */
                          f_xpos,          /* Window x-coordinate */
                          f_ypos;          /* Window y-coordinate */
   int                  (*f_verify)(int, FIELD_VALUE *); /* verification fn */
   void                 (*f_help)(int);    /* User's help function */
   unsigned char          f_usemin,        /* Flags use of minimum range */
                          f_usemax,        /* Flags use of maximum range */
                          f_useval,        /* Flags if initial value used */
                          f_usetmp;        /* Flags if string template used */
   union {
      struct {
         long             f_min,           /* Minimum value */
                          f_max;           /* Maximum value */
      } i;
      struct {
         double           f_min,           /* Minimum value */
                          f_max;           /* Maximum value */
      } f;
      struct {
         unsigned short   f_min,           /* Minimum value */
                          f_max;           /* Maximum value */
      } d;
   } range;
   char                  *f_template;      /* Template for string inputs */
   unsigned char          f_colour;        /* Colour attributes */
   struct {
      unsigned int        a_ronly : 1,     /* Read only flag */
                          a_noecho : 1,    /* No-echo input flag */
                          a_fixtext : 1;   /* Fixed text field flag */
   } attrib;
   FIELD_VALUE            val;             /* Value of field */
} FIELD;


typedef struct {
   unsigned int           num_fields;      /* Number of fields */
   unsigned char         *cover_zone;      /* Screen cover buffer */
   unsigned char          colour,          /* Colour attribute */
                          input_colour;    /* Input field colour default */
   FIELD                 *field[FRM_MAXFIELD]; /* Fields */
   unsigned char          xpos,            /* x-coordinate on screen */
                          ypos,            /* y-coordinate on screen */
                          height,          /* Height of form */
                          width,           /* Width of form */
                          type,            /* Form surround type */
                          use_mode;        /* Flags whether in use/displayed */
} FORM;



/*--------------------------------------------------------------------------*/
/*                                                                          */
/*                  Pop-Up & Pulldown Menu Services                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/


int change_menu_attribs      (MENU *, int, unsigned char);

int change_menu_escapes      (MENU *, unsigned char);

int clear_menu_in_pulldown   (PULLDOWN *);

int define_menu              (MENU *, char *, unsigned char, unsigned char,
                              unsigned char, unsigned char, unsigned char,
                              unsigned char, unsigned char, char **);

int define_pulldown          (PULLDOWN *, unsigned char, unsigned char,
                              unsigned char, char **, MENU **);

int display_menu             (MENU *, int, int);

int escape_fkey              (void);

int new_pulldown_cover       (PULLDOWN *);

int read_menu_selection      (MENU *);

int read_pulldown_selection  (PULLDOWN *, int *, int *);

int remove_menu              (MENU *);

int remove_pulldown          (PULLDOWN *);

int set_menu_help            (MENU *, void (*)(int));

int set_menu_option          (MENU *, int, int);

int set_pulldown_help        (PULLDOWN *, void (*)(int));



/*--------------------------------------------------------------------------*/
/*                                                                          */
/*                            Window Utilities                              */
/*                                                                          */
/*--------------------------------------------------------------------------*/


int clear_notice             (void);

int display_notice           (int, int);

int notice_text              (char *, ...);

int prepare_notice           (unsigned char, unsigned char, unsigned char);

int prompt_input             (int, int, char *, unsigned char);



/*--------------------------------------------------------------------------*/
/*                                                                          */
/*                           Screen/Form Utilities                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/


int display_form (FORM *, int, int);

int edit_form (FORM *, int *);

int get_field (FORM *, int, FIELD_VALUE *);

int get_field_id (FORM *, char *, int *);

int load_form (FORM *, char *);

int load_form_source (FORM *, char *);

int put_field (FORM *, int, FIELD_VALUE *);

int remove_form (FORM *);

int set_field_attrib (FORM *, int, unsigned char);

int set_field_help (FORM *, int, void (*)(int));

int set_field_mode (FORM *, int, int);

int set_field_verify (FORM *, int, int (*)(int, FIELD_VALUE *));

int unload_form (FORM *);



/*--------------------------------------------------------------------------*/
/*                                                                          */
/*                            General Utilities                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/


unsigned char colour_attrib  (int, int);
