;
;       Modello da definire.............
;
.Model LARGE,C



.DATA

extrn   risoluzione_x:word
extrn	risoluzione_y:word
extrn   minCol:word
extrn   maxCol:word
extrn   addrTseng:dword
extrn   addrTsengc:dword


	char_y   db      0
	num      dw      0
	mom      db      48 dup (?)
	conf     db      0
	bank     db      0
	regdi    dw      0



.CODE


public testTSENG

testTSENG proc

	mov     ch,0
	mov     dx,3d4h                 ;check for Tseng 4000 series
	mov     al,33h
	out     dx,al
	inc     dx
	in      al,dx
	dec     dx
	mov     cl,al
	mov     ax,00a33h
	out     dx,ax
	mov     al,33h
	out     dx,al
	inc     dx
	in      al,dx
	and     al,0fh
	dec     dx
	cmp     al,00ah
	jnz     noct2
	mov     ax,00533h
	out     dx,ax
	mov     al,33h
	out     dx,al
	inc     dx
	in      al,dx
	dec     dx
	and     al,0fh
	cmp     al,005h
	jnz     noct2
	mov     al,33h
	mov     ah,cl
	out     dx,ax
	mov     ch,1

	mov     dx,3bfh                 ;Enable access to extended registers
	mov     al,3
	out     dx,al
	mov     dl,0d8h
	mov     al,0a0h
	out     dx,al

noct2:  mov     dx,3cdh                 ;Test for Tseng
	in      al,dx                   ;save bank switch register
	mov     cl,al
	mov     al,0aah                 ;test register with 0aah
	out     dx,al
	in      al,dx
	cmp     al,0aah
	jnz     nots
	mov     al,055h                 ;test register with 055h
	out     dx,al
	in      al,dx
	cmp     al,055h
	jnz     nots
	mov     al,cl
	out     dx,al
	mov     ax,3                    ;Tseng 3000

					;bkadr   tseng,$tseng, tseng_entries    ; Bert
					;mov     [vga512],1

	cmp     ch,0
	jz      oldts
	mov     ax,4
					;Tseng 4000
					;mov     tseng,0
					;bkadr   tseng4,$tseng4, tseng4_entries ; Bert

oldts:
	ret

nots:
	xor     ax,ax
	ret

testTSENG endp


public tseng

tseng  proc            ;Tseng
	and	al,7
	mov	ah,al
	shl	ah,1
	shl	ah,1
	shl	ah,1
	or	al,ah
	or	al,01000000b
	mov	dx,3cdh
	out	dx,al
	sti
	ret
tseng  endp

public tsengc

tsengc  proc            ;Tseng
	and	al,7
	mov	ah,al
	shl	ah,1
	shl	ah,1
	shl	ah,1
	or	al,ah
	or	al,01000000b
	ret
tsengc  endp



public tseng4

tseng4 proc            ;Tseng 4000
	mov	ah,al
        mov     dx,3bfh
	mov	al,3
	out	dx,al
	mov	dl,0d8h
	mov	al,0a0h
	out	dx,al
	and	ah,15
	mov	al,ah
	shl	al,1
	shl	al,1
	shl	al,1
	shl	al,1
	or	al,ah
	mov	dl,0cdh
	out	dx,al
	sti
	ret
tseng4 endp



public tseng4c

tseng4c proc            ;Tseng 4000
        mov     ah,al
        mov     dx,3bfh
	mov	al,3
	out	dx,al
	mov	dl,0d8h
	mov	al,0a0h
	out	dx,al
	and	ah,15
	mov	al,ah
	shl	al,1
	shl	al,1
	shl	al,1
	shl	al,1
	or	al,ah
	ret
tseng4c endp



public change_pag

change_pag proc ,pagina:byte

	mov     al,pagina
	call    addrTseng
	ret

change_pag endp


;-------------------------------------------------------------------------------
;INARRAY
;
;    inarray(int start_x,int start_y,int numero_iterazioni,unsigned char *array)
;
;Modelli : Small Medium (Tiny).
;                            o qualsiasi se ARRAY e' definito NEAR
;
;Descrizione :
;
;       Ritorna in un ARRAY il numero di pixel di ITERAZ a partire dalla
;       locazione di coordinate STARTX, STARTY.
;       Il numero di ITERAZ + STARTX non deve essere superiore alle dimensioni
;       in pixel dello schermo (risoluzione_x).
;-------------------------------------------------------------------------------

public inarray

inarray PROC USES di si, startx:word,starty:word,iteraz:word,arry:dword


	mov     ax,word ptr arry+2
	mov     es,ax
	xor     bx,bx
	mov     di,word ptr arry
	mov     cx,iteraz
;        mov     bank,0

	mov     dx,starty
	mov     si,startx
	add     si,iteraz
	clc
	mov     ax,risoluzione_x
	mul     dx
	add     ax,si
	adc     dx,0
	mov     si,ax
	mov     al,dl

	call    addrTsengc

	mov     conf,al

start0:
	mov     dx,starty
	mov     si,startx
	add     si,bx
	clc
	mov     ax,risoluzione_x
	mul     dx
	add     ax,si
	adc     dx,0
	mov     si,ax
	mov     al,dl
	cli

	call    addrTseng


	cld

;        cmp     bank,0
;        jne     continue0

	cmp     conf,al
	je      zip0
;        mov     bank,1

continue0:
	push    ds
	mov     dx,0a000h
	mov     ds,dx

	movsb                                   ;mov    al,byte ptr ds:[si]
						;mov    es:[di],al
	pop     ds
						;inc    si
	inc     bx
	loop    start0
	jmp     chiudi0

zip0:
	push    ds
	mov     dx,0a000h
	mov     ds,dx

	sar     cx,1
	jnc     f10
	movsb
f10:
	rep     movsw
	pop     ds

chiudi0:
	ret

inarray         endp


;-------------------------------------------------------------------------------
;INARRAY_FAR
;
;    inarray_far(int start_x,int start_y,int numero_iterazioni,unsigned char far *array)
;
;Modelli : Large Compact Huge
;                           o qualsiasi se ARRAY e' definito FAR
;
;
;Descrizione :
;
;       Ritorna in un ARRAY il numero di pixel di ITERAZ a partire dalla
;       locazione di coordinate STARTX, STARTY.
;       Il numero di ITERAZ + STARTX non deve essere superiore alle dimensioni
;       in pixel dello schermo (risoluzione_x).
;-------------------------------------------------------------------------------


public inarray_far

inarray_far PROC USES di si, startx:word,starty:word,iteraz:word,arry:dword


	mov     ax,word ptr arry+2
	mov     es,ax
	xor     bx,bx
	mov     di,word ptr arry
	mov     cx,iteraz
;        mov     bank,0

	mov     dx,starty
	mov     si,startx
	add     si,iteraz
	clc
	mov     ax,risoluzione_x
	mul     dx
	add     ax,si
	adc     dx,0
	mov     si,ax
	mov     al,dl

	call    addrTsengc

	mov     conf,al

start1:
	mov     dx,starty
	mov     si,startx
	add     si,bx
	clc
	mov     ax,risoluzione_x
	mul     dx
	add     ax,si
	adc     dx,0
	mov     si,ax
	mov     al,dl
	cli

	call    addrTseng

	cld

;        cmp     bank,0
;        jne     continue1

	cmp     conf,al
	je      zip1
;        mov     bank,1

continue1:
	push    ds
	mov     dx,0a000h
	mov     ds,dx

	movsb                                   ;mov    al,byte ptr ds:[si]
						;mov    es:[di],al
	pop     ds
						;inc    si
	inc     bx
	loop    start1
	jmp     chiudi1

zip1:
	push    ds
	mov     dx,0a000h
	mov     ds,dx

	sar     cx,1
	jnc     f11
	movsb
f11:
	rep     movsw
	pop     ds

chiudi1:
	ret

inarray_far         endp


;-------------------------------------------------------------------------------
;INPIXEL
;
;    int inpixel(int start_x,int start_y)
;
;Modelli : TUTTI
;
;Descrizione :
;
;       Ritorna un intero che contiene il valore del pixel letto alle
;       coordinate STARTX, STARTY.
;-------------------------------------------------------------------------------



public inpixel

inpixel  PROC , startx:word,starty:word


	mov     ax,0a000h
	mov     es,ax
	mov     dx,starty
	clc
	mov     ax,risoluzione_x
	mul     dx
	add     ax,startx
	adc     dx,0
	mov     bx,ax
	mov     al,dl
	cli

	call    addrTseng

	xor     ah,ah
	mov     al,es:[bx]
	ret

inpixel        endp


public in_outpixel

in_outpixel  PROC , startx:word,starty:word


	mov     dx,starty
	mov	bx,startx

	or	dx,dx
	jl	tinue
	or	bx,bx
	jl	tinue
	cmp	dx,risoluzione_y
	jge	tinue
	cmp	bx,risoluzione_x
	jge	tinue

	mov	ax,0a000h
	mov	es,ax

	clc
	mov     ax,risoluzione_x
	mul     dx
	add     ax,bx
	adc     dx,0
	mov     bx,ax
	mov     al,dl
	cli
	call    addrTseng
	inc     byte ptr es:[bx]
tinue:
	ret

in_outpixel        endp

;-------------------------------------------------------------------------------
;OUTARRAY
;
;    outarray(int start_x,int start_y,int numero_iterazioni,unsigned char *array)
;
;Modelli : Small Medium (Tiny).
;                            o qualsiasi se ARRAY e' definito NEAR
;
;Descrizione :
;
;       Accende sullo schermo i pixel contenuti in ARRAY a partire dalla
;       locazione di coordinate STARTX, STARTY.ITERAZ contiene il numero di
;       pixel da accendere.
;       Il numero di ITERAZ + STARTX non deve essere superiore alle dimensioni
;       in pixel dello schermo (risoluzione_x).
;-------------------------------------------------------------------------------

public outarray

outarray PROC USES di si, startx:word,starty:word,iteraz:word,arry:dword


	mov     ax,0a000h
	mov     es,ax
	xor     bx,bx
	mov     si,word ptr arry
	mov     cx,iteraz
;        mov     bank,0

	mov     dx,starty
	mov     di,startx
	add     di,iteraz
	clc
	mov     ax,risoluzione_x
	mul     dx
	add     ax,di
	adc     dx,0
	mov     di,ax
	mov     al,dl

	call    addrTsengc

        mov     conf,al

start2:
	mov     dx,starty
	mov     di,startx
	add     di,bx
	clc
	mov     ax,risoluzione_x
	mul     dx
	add     ax,di
	adc     dx,0
	mov     di,ax
	mov     al,dl
	cli

	call    addrTseng


	cld

;        cmp     bank,0
;        jne     continue2

	cmp     conf,al
	je      zip2
;        mov     bank,1

continue2:

	push    ds
	mov     dx,word ptr arry+2
	mov     ds,dx

	movsb                                   ;mov    al,byte ptr ds:[si]
						;mov    es:[di],al
	pop     ds
						;inc    si
	inc     bx
	loop    start2
	jmp     chiudi2


zip2:

	push    ds
	mov     dx,word ptr arry+2
	mov     ds,dx

	sar     cx,1
	jnc     f12
	movsb
f12:
	rep     movsw
	pop     ds

chiudi2:
	ret

outarray         endp


;-------------------------------------------------------------------------------
;OUTARRAY_FAR
;
;    outarray_far(int start_x,int start_y,int numero_iterazioni,unsigned char far *array)
;
;Modelli : Large Compact Huge
;                           o qualsiasi se ARRAY e' definito FAR
;
;
;Descrizione :
;
;       Accende sullo schermo i pixel contenuti in ARRAY a partire dalla
;       locazione di coordinate STARTX, STARTY.ITERAZ contiene il numero di
;       pixel da accendere.
;       Il numero di ITERAZ + STARTX non deve essere superiore alle dimensioni
;       in pixel dello schermo (risoluzione_x).
;-------------------------------------------------------------------------------

public outarray_far

outarray_far PROC USES di si, startx:word,starty:word,iteraz:word,arry:dword


	mov     ax,0a000h
	mov     es,ax
	xor     bx,bx
	mov     si,word ptr arry
	mov     cx,iteraz
;        mov     bank,0

	mov     dx,starty
	mov     di,startx
	add     di,iteraz
	clc
	mov     ax,risoluzione_x
	mul     dx
	add     ax,di
	adc     dx,0
	mov     di,ax
	mov     al,dl

	call    addrTsengc

	mov     conf,al

start3:
	mov     dx,starty
	mov     di,startx
	add     di,bx
	clc
	mov     ax,risoluzione_x
	mul     dx
	add     ax,di
	adc     dx,0
	mov     di,ax
	mov     al,dl
	cli

	call    addrTseng


	cld

;        cmp     bank,0
;        jne     continue3

	cmp     conf,al
	je      zip3
;        mov     bank,1

continue3:

	push    ds
	mov     dx,word ptr arry+2
	mov     ds,dx

	movsb                                   ;mov    al,byte ptr ds:[si]
						;mov    es:[di],al
	pop     ds
						;inc    si
	inc     bx
	loop    start3
	jmp     chiudi3

zip3:
	push    ds
	mov     dx,word ptr arry+2
	mov     ds,dx

	sar     cx,1
	jnc     f13
	movsb
f13:
	rep     movsw
	pop     ds

chiudi3:
	ret

outarray_far         endp






;-------------------------------------------------------------------------------
;OUTPIXEL
;
;    outpixel(int start_x,int start_y,int iteraz,unsigned char pixel)
;
;Modelli : TUTTI
;
;Descrizione :
;
;       Accende sul video un pixel alle STARTX, STARTY.L'operazione viene
;       ripetuta per ITERAZ volte incrementando il valore di STARTX.
;       Il numero di ITERAZ + STARTX non deve essere superiore alle dimensioni
;       in pixel dello schermo (risoluzione_x).
;-------------------------------------------------------------------------------


public outonepixel

outonepixel  PROC , startx:word,starty:word,pixel:byte


	mov     ax,0a000h
	mov     es,ax
	mov     dx,starty
        mov     bx,startx
	clc
	mov     ax,risoluzione_x
        mul     dx
        add     ax,bx
	adc     dx,0
        mov     bx,ax
	mov     al,dl
	cli

	call    addrTseng

	mov     al,pixel
        mov     es:[bx],al
	ret

outonepixel        endp

;-------------------------------------------------------------------------------
;OUTPIXEL
;
;    outpixel(int start_x,int start_y,int iteraz,unsigned char pixel)
;
;Modelli : TUTTI
;
;Descrizione :
;
;       Accende sul video un pixel alle STARTX, STARTY.L'operazione viene
;       ripetuta per ITERAZ volte incrementando il valore di STARTX.
;       Il numero di ITERAZ + STARTX non deve essere superiore alle dimensioni
;       in pixel dello schermo (risoluzione_x).
;-------------------------------------------------------------------------------


public outpixel

outpixel  PROC USES  di, startx:word,starty:word,iteraz:word,pixel:byte


	mov     ax,0a000h
	mov     es,ax
	xor     bx,bx
	mov     cx,iteraz
start4:
	mov     dx,starty
	mov     di,startx
	add     di,bx
	clc
	mov     ax,risoluzione_x
	mul     dx
	add     ax,di
	adc     dx,0
	mov     di,ax
	mov     al,dl
	cli

	call    addrTseng

	mov     al,pixel
	mov     es:[di],al
	inc     bx
	loop    start4
	ret

outpixel        endp

;-------------------------------------------------------------------------------
;INAREA0
;
;    inarea0(int start_x,int start_y,int iteraz_x,int iteraz_y,unsigned char far *array)
;
;Modelli : Large Compact Huge
;                           o qualsiasi se ARRAY e' definito FAR
;
;
;Descrizione :
;
;       Ritorna in un ARRAY i pixel contenuti in un poligono di coordinate
;       (punto alto sinistra) STARTX, STARTY e (punto basso destra) STARTX+ITERAZX
;       STARTY+ITERAZY e li elimina dallo schermo.
;       Il numero di ITERAZ + STARTX non deve essere superiore alle dimensioni
;       in pixel dello schermo (risoluzione_x) ed il numero di pixel
;       STARTY + ITERAZY non deve essere maggiore della risoluzione_y ,in pixel
;       dello schermo.
;-------------------------------------------------------------------------------



public inarea0

inarea0  PROC USES di si, startx:word,starty:word,iteraz_x:word,iteraz_y:word,arry:dword


	mov     ax,word ptr arry+2
	mov     es,ax
	mov     di,word ptr arry
pre0:
	mov     cx,iteraz_x
	xor     bx,bx
;        mov     bank,0

	mov     dx,starty
	mov     si,startx
	add     si,iteraz_x
	clc
	mov     ax,risoluzione_x
	mul     dx
	add     ax,si
	adc     dx,0
	mov     si,ax
	mov     al,dl

	call    addrTsengc

	mov     conf,al

start5:
	mov     dx,starty
	mov     si,startx
	add     si,bx
	clc
	mov     ax,risoluzione_x
	mul     dx
	add     ax,si
	adc     dx,0
	mov     si,ax
	mov     al,dl
	cli

	call    addrTseng


	cld

;        cmp     bank,0
;        jne     continue4

	cmp     conf,al
	je      zip4
;        mov     bank,1

continue4:

	mov     ax,minCol

	push    ds
	mov     dx,0a000h
	mov     ds,dx

	movsb
	mov     byte ptr ds:[si-1],al

	pop     ds

	inc     bx
	loop    start5
	jmp     secondo0

zip4:
	mov     ax,minCol
	mov     ah,al

	push    ds
	mov     dx,0a000h
	mov     ds,dx

	sar     cx,1
	jnc     f14
	movsb
	mov     byte ptr ds:[si-1],al
f14:
	movsw
	mov     word ptr ds:[si-2],ax
	loop    f14

	pop     ds

secondo0:

	inc     starty
	dec     iteraz_y
	cmp     iteraz_y,0
	jle     chiudi4
	jmp     pre0
chiudi4:
	ret

inarea0         endp


;-------------------------------------------------------------------------------
;INAREA
;
;    inarea(int start_x,int start_y,int iteraz_x,int iteraz_y,unsigned char far *array)
;
;Modelli : Large Compact Huge
;                           o qualsiasi se ARRAY e' definito FAR
;
;
;Descrizione :
;
;       Ritorna in un ARRAY i pixel contenuti in un poligono di coordinate
;       (punto alto sinistra) STARTX, STARTY e (punto basso destra) STARTX+ITERAZX
;       STARTY+ITERAZY.
;       Il numero di ITERAZ + STARTX non deve essere superiore alle dimensioni
;       in pixel dello schermo (risoluzione_x) ed il numero di pixel
;       STARTY + ITERAZY non deve essere maggiore della risoluzione_y ,in pixel
;       dello schermo.
;-------------------------------------------------------------------------------


public inarea


inarea  PROC USES di si, startx:word,starty:word,iteraz_x:word,iteraz_y:word,arry:dword


	mov     ax,word ptr arry+2
	mov     es,ax
	mov     di,word ptr arry
pre1:
	mov     cx,iteraz_x
	xor     bx,bx
;        mov     bank,0

	mov     dx,starty
	mov     si,startx
	add     si,iteraz_x
	clc
	mov     ax,risoluzione_x
	mul     dx
	add     ax,si
	adc     dx,0
	mov     si,ax
	mov     al,dl

	call    addrTsengc

	mov     conf,al

start6:
	mov     dx,starty
	mov     si,startx
	add     si,bx
	clc
	mov     ax,risoluzione_x
	mul     dx
	add     ax,si
	adc     dx,0
	mov     si,ax
	mov     al,dl
	cli

	call    addrTseng


	cld

;        cmp     bank,0
;        jne     continue5

	cmp     conf,al
	je      zip5
;        mov     bank,1

continue5:

	push    ds
	mov     dx,0a000h
	mov     ds,dx

	movsb

	pop     ds

	inc     bx
	loop    start6
	jmp     secondo1

zip5:
	push    ds
	mov     dx,0a000h
	mov     ds,dx

	sar     cx,1
	jnc     f15
	movsb
f15:
	rep     movsw
	pop     ds

secondo1:

	inc     starty
	dec     iteraz_y
	cmp     iteraz_y,0
	jle     chiudi5
	jmp     pre1
chiudi5:
	ret

inarea         endp


;-------------------------------------------------------------------------------
;OUTAREA
;
;    outarea(int start_x,int start_y,int iteraz_x,int iteraz_y,unsigned char far *array)
;
;Modelli : Large Compact Huge
;                           o qualsiasi se ARRAY e' definito FAR
;
;
;Descrizione :
;
;       Accende in un poligono i pixel contenuti in un ARRAY .Il poligono avra'
;       come coordinate  (punto alto sinistra) STARTX, STARTY e
;       (punto basso destra) STARTX + ITERAZX,  STARTY + ITERAZY.
;       Il numero di ITERAZ + STARTX non deve essere superiore alle dimensioni
;       in pixel dello schermo (risoluzione_x) ed il numero di pixel
;       STARTY + ITERAZY non deve essere maggiore della risoluzione_y ,in pixel
;       dello schermo.
;-------------------------------------------------------------------------------

public outarea


outarea  PROC USES di si, startx:word,starty:word,iteraz_x:word,iteraz_y:word,arry:dword


	mov     ax,0a000h
	mov     es,ax
	mov     si,word ptr arry
	mov     bx,iteraz_y

pre2:
	mov     cx,iteraz_x
	mov     dx,starty
	mov     di,startx
	add     di,cx
	clc
	mov     ax,risoluzione_x
	mul     dx
	add     ax,di
	adc     dx,0
	mov     regdi,ax
	mov     conf,dl

start7:
	mov     dx,starty
	mov     di,startx
	clc
	mov     ax,risoluzione_x
	mul     dx
	add     ax,di
	adc     dx,0
	mov     di,ax
	mov     al,dl
	cli

	call    addrTseng


	cld


	cmp     di,regdi
	jb      zip6


	sub     cx,regdi
	push    ds
	mov     dx,word ptr arry+2
	mov     ds,dx

	sar     cx,1
	jnc     f016
	movsb
f016:
	rep     movsw
	pop     ds

	mov     al,conf
	call    addrTseng
	mov     cx,regdi

zip6:
	push    ds
	mov     dx,word ptr arry+2
	mov     ds,dx

	sar     cx,1
	jnc     f16
	movsb
f16:
	rep     movsw
	pop     ds

secondo2:

	inc     starty
	dec     bx
	or      bx,bx
	jz      chiudi6
	jmp     pre2
chiudi6:
	ret

outarea         endp




public outmouse


outmouse  PROC USES di si, startx:word,starty:word,iteraz_x:word,iteraz_y:word,arry:dword


	mov     ax,0a000h
	mov     es,ax
	mov     si,word ptr arry
pre3:
	mov     cx,iteraz_x
	mov     dx,starty
	mov     di,startx
	mov     ax,risoluzione_x
	mul     dx
	add     di,ax
	cld

	push    ds
	mov     dx,word ptr arry+2
	mov     ds,dx

	sar     cx,1
	jnc     f17
	movsb
f17:
	rep     movsw
	pop     ds


	inc     starty
	dec     iteraz_y
	cmp     iteraz_y,0
	jle     chiudi7
	jmp     pre3
chiudi7:
	ret

outmouse         endp







public outmouse_arrow


outmouse_arrow  PROC USES di si, startx:word,starty:word,iteraz_x:word,iteraz_y:word,arry:dword


	mov     ax,0a000h
	mov     es,ax
	mov     si,word ptr arry
	mov     bh,byte ptr maxCol
	mov     bl,byte ptr minCol
pre4:
	mov     cx,iteraz_x

	mov     dx,starty
	mov     di,startx
	mov     ax,risoluzione_x
	mul     dx
	add     di,ax

	cld
	push    ds
	mov     dx,word ptr arry+2
	mov     ds,dx
	jmp     cart2
cart1:
	inc     di
	dec     cx
	jcxz    secondo3
cart2:

	lodsb
	or      al,0
	je      cart1
	cmp     al,1
	je      u1
	jmp     u2
u1:
	mov     al,bl
	jmp     u3
u2:
	mov     al,bh
u3:
	stosb
	jcxz    secondo3
	loop    cart2



secondo3:
	pop     ds
	inc     starty
	dec     iteraz_y
	cmp     iteraz_y,0
	jle     chiudi8
	jmp     pre4
chiudi8:
	ret

outmouse_arrow  endp


public inmouse


inmouse  PROC USES di si, startx:word,starty:word,iteraz_x:word,iteraz_y:word,arry:dword


	mov     ax,word ptr arry+2
	mov     es,ax
	mov     di,word ptr arry
pre5:
	mov     cx,iteraz_x
	mov     dx,starty
	mov     si,startx
	mov     ax,risoluzione_x
	mul     dx
	add     si,ax
	cld

	push    ds
	mov     dx,0a000h
	mov     ds,dx

	sar     cx,1
	jnc     f18
	movsb
f18:
	rep     movsw
	pop     ds


	inc     starty
	dec     iteraz_y
	cmp     iteraz_y,0
	jle     chiudi9
	jmp     pre5
chiudi9:
	ret

inmouse         endp





public outareab

outareab  PROC USES di si, startx:word,starty:word,iteraz_x:word,iteraz_y:word,arry:dword


	mov     ax,0a000h               ;Indirizzo video
	mov     es,ax                   ;...
	mov     si,word ptr arry        ;Offset array passato alla func
pre6:
	mov     cx,iteraz_x             ;Prende le iterazioni X
	xor     bx,bx                   ;Azzera registro associato a DI
;        mov     bank,0                  ;Flag pre controllare se si e' a
					;cavallo di un banco

	mov     dx,starty               ;Start Y
	mov     di,startx               ;Start X
	add     di,iteraz_x             ;Calcola ultimo pixel di riga
	clc
	mov     ax,risoluzione_x        ;Risoluzione srceen
	mul     dx                      ;calcola offset screen
	add     ax,di                   ;
	adc     dx,0                    ;
	mov     di,ax                   ;Calcola il valore del
	mov     al,dl                   ;banco dell'ultimo pixel di

	call    addrTsengc

	mov     conf,al                 ;Memorizza il banco per dopo

start8:                                  ;
	mov     dx,starty               ;Fa la stessa cosa detta
	mov     di,startx               ;precedentemente relativa al
	add     di,bx                   ;primo pixel e con la
	clc                             ;istruzione out alla porta 3cdh
	mov     ax,risoluzione_x        ;seleziona il banco idoneo.
	mul     dx                      ;Se la riga si trova a cavallo di un
	add     ax,di                   ;banco questa procedura di
	adc     dx,0                    ;ricerca banco,selezione banco
	mov     di,ax                   ;appropriato viene ripetuto per
	mov     al,dl                   ;tutti i pixel della riga.
	cli                             ;Altrimenti tutto questo viene

	call    addrTseng


	cld                             ;Seleziona senso avanti

;        cmp     bank,0                  ;controlla se e' gia' stata verifica
;        jne     continue7                ;ta la riga a cavallo...e continua

	cmp     conf,al                 ;Se non e' stata verificata lo fa
	je      zip9                     ;e se e' negativo va a ZIP
;        mov     bank,1                  ;altrimenti setta il flag di contollo
					;cavallo

continue7:

	push    ds                      ;salvo per dopo
	mov     dx,word ptr arry+2      ;faccio puntare ds:[si] al seg:off
	mov     ds,dx                   ;di arry

	movsb                           ;ds:[si] -> es:[di] si++ di++
					; arry       video
	pop     ds                      ;Ripristino DS
					;
	inc     bx                      ;Incremento BX che e' associato a DI
	loop    start8                   ;Continua il LOOP a cavallo...
	jmp     secondo4                 ;e finito esce.

zip9:

	push    ds                      ;salvo per dopo
	mov     dx,word ptr arry+2      ;faccio puntare ds:[si] al seg:off
	mov     ds,dx                   ;di arry

					;Se non e' a cavallo
	sar     cx,1                    ;ITERAZ_X / 2
	jnc     f19                     ;pari o dispari ??
	movsb                           ;dispari: ne mando uno sul video cosi'
f19:                                    ;adesso e' pari e....
	rep     movsw                   ;con UNA istruzione muovo due BYTE
	pop     ds                      ;alla volta e ripristino DS
					;
secondo4:                                ;Continua per tutte le iterazioni
					;rispetto ad Y
	;mov     ax,risoluzione_x
	;sub     ax,iteraz_x
	;add     bx,ax
	inc     starty                  ;
	dec     iteraz_y                ;
	cmp     iteraz_y,0              ;
	jle     chiudi10                  ;
	jmp     pre6                     ;
chiudi10:
	ret

outareab         endp



;-------------------------------------------------------------------------------
;OUTAREA
;
;    outarea(int start_x,int start_y,int iteraz_x,int iteraz_y,unsigned char far *array)
;
;Modelli : Large Compact Huge
;                           o qualsiasi se ARRAY e' definito FAR
;
;
;Descrizione :
;
;       Accende in un poligono i pixel contenuti in un ARRAY .Il poligono avra'
;       come coordinate  (punto alto sinistra) STARTX, STARTY e
;       (punto basso destra) STARTX + ITERAZX,  STARTY + ITERAZY.
;       Il numero di ITERAZ + STARTX non deve essere superiore alle dimensioni
;       in pixel dello schermo (risoluzione_x) ed il numero di pixel
;       STARTY + ITERAZY non deve essere maggiore della risoluzione_y ,in pixel
;       dello schermo.
;-------------------------------------------------------------------------------



public outarea0

outarea0  PROC USES di si, startx:word,starty:word,iteraz_x:word,iteraz_y:word


	mov     ax,0a000h
	mov     es,ax
pre7:
	mov     cx,iteraz_x
	xor     bx,bx
;        mov     bank,0

	mov     dx,starty
	mov     di,startx
	add     di,iteraz_x
	clc
	mov     ax,risoluzione_x
	mul     dx
	add     ax,di
	adc     dx,0
	mov     di,ax
	mov     al,dl

	call    addrTsengc

	mov     conf,al

start9:
	mov     dx,starty
	mov     di,startx
	add     di,bx
	clc
	mov     ax,risoluzione_x
	mul     dx
	add     ax,di
	adc     dx,0
	mov     di,ax
	mov     al,dl
	cli

	call    addrTseng


	cld

;        cmp     bank,0
;        jne     continue8

	cmp     conf,al
	je      zip10
;        mov     bank,1

continue8:

	mov     ax,minCol


	stosb                                   ;mov    al,byte ptr ds:[si]
						;mov    es:[di],al
						;inc    si
	inc     bx
	loop    start9
	jmp     secondo5

zip10:
	mov     ax,minCol
	mov     ah,al

	sar     cx,1
	jnc     f20
	stosb
f20:
	rep     stosw

secondo5:

	inc     starty
	dec     iteraz_y
	cmp     iteraz_y,0
	jle     chiudi11
	jmp     pre7
chiudi11:
	ret

outarea0         endp





;-------------------------------------------------------------------------------
;SET_MODO_VIDEO
;
;    set_modo_video(int start_c,int n_colori,unsigned char *dac_pal)
;
;Modelli : Small Medium (Tiny).
;                            o qualsiasi se XMX e' definito NEAR;
;
;Descrizione :
;
;       Seleziona la pallette del DAC.STARTC contiene il valore del registro
;       latch da cui iniziare la selezione,NCOLORI contiene il numero di
;       colori da selezionare,DACPAL e' un array che contiene consecutivamente
;       per ogni colore tre valori di,rispettivamente,RED GREEN BLUE.
;       DACPAL deve essere quindi almeno di dimensioni = DACPAL[3*ncolori].
;-------------------------------------------------------------------------------




public set_modo_video

set_modo_video  PROC  , startc:word,ncolori:word,dacpal:dword

	mov     ax,word ptr dacpal+2
	mov     es,ax
	mov     ax,1012h
	mov     bx,startc
	mov     cx,ncolori
	mov     dx,word ptr dacpal
	int     10h
	ret

set_modo_video        endp

public get_modo_video

get_modo_video  PROC  , startc:word,ncolori:word,dacpal:dword

	mov     ax,word ptr dacpal+2
	mov     es,ax
	mov     ax,1017h
	mov     bx,startc
	mov     cx,ncolori
	mov     dx,word ptr dacpal
	int     10h
	ret

get_modo_video        endp



public set_gray_video

set_gray_video  PROC , startc:word,ncolori:word,dacpal:dword

	mov     ax,word ptr dacpal+2
	mov     es,ax
	mov     ax,101bh
	mov     bx,startc
	mov     cx,ncolori
	mov     dx,word ptr dacpal
	int     10h
	ret

set_gray_video        endp



public outarrayb_far

outarrayb_far PROC USES di si, startx:word,starty:word,iteraz:word,arry:dword


	mov     ax,0a000h
	mov     es,ax
	xor     bx,bx
	mov     si,word ptr arry
	mov     cx,iteraz
;        mov     bank,0

	mov     dx,starty
	mov     di,startx
	add     di,iteraz
	clc
	mov     ax,risoluzione_x
	mul     dx
	add     ax,di
	adc     dx,0
	mov     di,ax
	mov     al,dl

	call    addrTsengc

	mov     conf,al

start10:
	mov     dx,starty
	mov     di,startx
	add     di,bx
	clc
	mov     ax,risoluzione_x
	mul     dx
	add     ax,di
	adc     dx,0
	mov     di,ax
	mov     al,dl
	cli

	call    addrTseng


	cld

;        cmp     bank,0
;        jne     continue9

	cmp     conf,al
	je      zip11
;        mov     bank,1

continue9:

	push    ds
	mov     dx,word ptr arry+2
	mov     ds,dx

	movsb                                   ;mov    al,byte ptr ds:[si]
						;mov    es:[di],al
	pop     ds
						;inc    si
	inc     bx
	loop    start10
	jmp     chiudi12

zip11:
	push    ds
	mov     dx,word ptr arry+2
	mov     ds,dx

	sar     cx,1
	jnc     f21
	movsb
f21:
	rep     movsw
	pop     ds

chiudi12:
	ret

outarrayb_far         endp






;public inarray_emm
;
;inarray_emm  PROC USES ds es di si, startx:word,starty:word,iteraz:word,array:dword
;
;	xor     bx,bx
;	mov     di,word ptr array
;	mov     cx,iteraz
;	sar     cx,1
;	jnc     start11
;	inc     cx
;start11:
;	mov     dx,starty
;	mov     si,startx
;	add     si,bx
;	clc
;	mov     ax,risoluzione_x
;	mul     dx
;	add     ax,si
;	adc     dx,0
;	mov     si,ax
;	mov     al,dl
;	cli
;
;	call    addrTseng
;
;	push    bx
;	mov     bx,0a000h
;	mov     es,bx
;	mov     ax,es:[si]
;	mov     bx,word ptr array+2
;	mov     es,bx
;	stos    word ptr es:[di]
;	pop     bx
;	inc     bx
;	inc     bx
;	loop    start11
;	ret
;
;inarray_emm     endp


;public outarray_emm
;
;outarray_emm  PROC USES es ds di si, startx:word,starty:word,iteraz:word,array:dword
;
;	xor     bx,bx
;	mov     si,word ptr array
;	mov     cx,iteraz
;	sar     cx,1
;	jnc     start12
;	inc     cx
;start12:
;	mov     dx,starty
;	mov     di,startx
;	add     di,bx
;	clc
;	mov     ax,risoluzione_x
;	mul     dx
;	add     ax,di
;	adc     dx,0
;	mov     di,ax
;	mov     al,dl
;	cli
;
;	call    addrTseng
;
;	push    bx
;	mov     bx,word ptr array+2
;	mov     es,bx
;	lods    word ptr es:[si]
;	mov     bx,0a000h
;	mov     es,bx
;	mov     word ptr es:[di],ax
;	pop     bx
;
;					;mov     al,byte ptr ds:[si]
;					;mov     es:[di],al
;	inc     bx
;	inc     bx
;	loop    start12
;	ret
;
;outarray_emm    endp

END
