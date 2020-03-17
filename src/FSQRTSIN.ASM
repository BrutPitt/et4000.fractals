;------------------- ------------------
; fpu.asm - modulo assembler
;
;
    .MODEL LARGE,c              ; select near data pointer (faster),
.386                            ;  C parameter passing,
.387                            ;  automatically put '_' in front of publics

extrn	MY_OVERFLOWD:qword
extrn	cr:qword
extrn	cj:qword
extrn	dif_x:tbyte
extrn   dif_y:tbyte
extrn	aval:qword
extrn   bval:qword
extrn	cval:qword
extrn	dval:qword
extrn	xmin:tbyte
extrn   ymin:tbyte
extrn	xmax:tbyte
extrn   ymax:tbyte
extrn	x_min:tbyte
extrn   y_min:tbyte
extrn	x_max:tbyte
extrn   y_max:tbyte
extrn	colt:word
extrn   bex:dword
extrn   oldxmin:tbyte
extrn   stx:tbyte
extrn   sty:tbyte
extrn	step:byte
extrn   risoluzione_x:word
extrn   risoluzione_y:word
extrn	iteraz:dword
extrn	epsilon:qword
extrn   kostre:qword
extrn   kostim:qword
extrn   bailout:word
extrn	col:word
extrn	row:word
extrn   absymin:qword
extrn   absxmin:qword
extrn	rate_x:qword
extrn	rate_y:qword
extrn   rigx:word

;extrn	MY_OVERFLOWD:dword
;extrn	cr:dword
;extrn	cj:dword
;extrn	dif_x:dword
;extrn   dif_y:dword
;extrn	aval:dword
;extrn   bval:dword
;extrn	cval:dword
;extrn	dval:dword
;extrn	xmin:dword
;extrn   ymin:dword
;extrn	xmax:dword
;extrn   ymax:dword
;extrn	x_min:dword
;extrn   y_min:dword
;extrn	x_max:dword
;extrn   y_max:dword
;extrn	colt:word
;extrn   bex:dword
;extrn   oldxmin:dword
;extrn   stx:dword
;extrn   sty:dword
;extrn	step:byte
;extrn   risoluzione_x:word
;extrn   risoluzione_y:word
;extrn	iteraz:dword
;extrn	epsilon:dword
;extrn   kostre:dword
;extrn   kostim:dword
;extrn   bailout:word
;extrn	col:word
;extrn	row:word
;extrn   absymin:dword
;extrn   absxmin:dword
;extrn	rate_x:dword
;extrn	rate_y:dword
;extrn   rigx:word




extrn	FpuSinh:far
extrn	FpuCosh:far
extrn	outonepixel:far
extrn	inpixel:far
extrn	kbhit:far
extrn	getkey:far
extrn	tasto:far
extrn	addrHop:dword
extrn	addrTseng:dword
extrn   convergenza:dword
extrn   internalfunz:dword
extrn   internalpixelize:dword
extrn   initpixelize:dword
extrn   endpixelize:dword


.DATA

uno	dw	1
due	dw	2
tre	dw	3
quattro dw	4
dieci   dw      10
cento   dw      100
due56   dw	256
oldcw   dw      0         ; original control word
newcw   dw      0         ; new control word
xco	dw	0
yco	dw	0
temp	dd	0
tempw	dw	0

MY_OVERFLOW	dw	3000;






.CODE



	public  FpuExp
FpuExp   proc

	fldl2e                          ; load log2(10)
	fmulp   st(1),st(0)             ; log2(10) * argument
	fld     st(0)                   ; duplicate product
	fstcw   oldcw                   ; get old control word
	fwait                           ; wait till it arrives
	mov     ax,oldcw                ; change rounding mode
	and     ah,0f3h               ; field to "round down"
	or      ah,04h
	mov     newcw,ax
	fldcw   newcw                   ; force rounding mode
	frndint                         ; get int part of product
	fldcw   oldcw                   ; restore old rounding mode
	fld     st(0)                   ; duplicate integer part
	fxch    st(2)                   ; get original product
	fsubrp  st(1),st(0)             ; find fractional part
	fld1
	fchs
	fxch    st(1)                   ; scale fractional part
	fscale
	fstp    st(1)                   ; discard coprocessor junk
	f2xm1                           ; raise 2 to power-1
	fld1
	faddp   st(1),st(0)             ; correct for the -1
	fmul    st(0),st(0)             ; square result
	fscale                          ; scale by int part
	fstp    st(1)                   ; discard coprocessor junk
	ret

FpuExp   endp


public pixelpixelize
pixelpixelize PROC


	fld	x_min 			;xco
	fsubr	st,st(2)
	fld	stx
	fdiv
	fistp   xco

	fld	y_min                   ;yco
	fsubr	st,st(1)
	fld	sty
	fdiv
	fistp   yco

;------------------------------------------
;
;	InPixel  &  OutOnePixel
;
;------------------------------------------

	mov     dx,yco
	mov	bx,xco

	or	dx,dx
	jl	t0inue
	cmp	dx,si
	jge	t0inue
	or	bx,bx
	jl	t0inue
	cmp	bx,di
	jge	t0inue

	clc
	mov     ax,di
	mul     dx
	add     ax,bx
	adc     dx,0
	mov     bx,ax
	mov     al,dl
	cli
	call    addrTseng
	inc     byte ptr es:[bx]

t0inue:
	ret

endp	pixelpixelize


public FpuHenoninit
FpuHenoninit PROC

	fld	dval
	fld	aval
;--------------------------------------
;	Calcolo N. iterazioni
;--------------------------------------
	fld	cval
	fdiv	bval
	fistp   temp
	mov	ecx,temp
;--------------------------------------
	ret

endp    FpuHenoninit


public FpuHenonend
FpuHenonend PROC

	fstp	st
	fstp	st
	ret

endp    FpuHenonend

;----------------------------------------------------------------
; x(0)   = y(0) = orbit/3
; x(n+1) = x(n)*Cos(a) + (x(n)*x(n)-y(n))*Sin(a)
; y(n+1) = x(n)*Sin(a) - (x(n)*x(n)-y(n))*Cos(a)
;----------------------------------------------------------------

;----------------------------------------------------------------------------------
; aval = angle , bval = orbit_step , cval = stop_orbit  dval = start_orbit , iteraz = n_point_for_orbit
;----------------------------------------------------------------------------------

public FpuHenon
FpuHenon PROC Uses ecx

	fld	st			; ,aval ,aval ,orbit
	fsincos				; ,cos(a) ,sin(a) ,aval ,orbit

	fld	st(3)
	fidiv	tre
	fld	st
	mov	ecx,iteraz              ; ,yval ,xval ,cos(a) ,sin(a) ,aval ,orbit
sts:
	fld	st(1)
	fmul	st,st                   ; ,x*x ,yval ,xval ,cos(a) ,sin(a) ,aval ,orbit
	fsubr	st,st(1)		; ,y-x*x ,yval ,xval ,cos(a) ,sin(a) ,aval ,orbit
	fmul	st,st(4)                ; ,sin(a)*(y-x^2) ,yval ,xval ,cos(a) ,sin(a) ,aval ,orbit
	fld	st(2)
	fmul	st,st(4)		; ,x*cos(a) ,sin(a)*(y-x^2) ,yval ,xval ,cos(a) ,sin(a) ,aval ,orbit
	fsubr				; ,new_xval ,yval ,xval ,cos(a) ,sin(a) ,aval ,orbit
	fxch	st(2)			; ,xval ,yval ,new_xval ,cos(a) ,sin(a) ,aval ,orbit
	fld	st			; ,xval ,xval ,yval ,new_xval ,cos(a) ,sin(a) ,aval ,orbit
	fmul	st,st(5)                ; ,sin(a)*xval ,xval ,yval ,new_xval ,cos(a) ,sin(a) ,aval ,orbit

	fxch	st(1)                   ; ,xval ,sin(a)*xval ,yval ,new_xval ,cos(a) ,sin(a) ,aval ,orbit
	fmul	st,st                   ; ,xval*xval ,sin(a)*xval ,yval ,new_xval ,cos(a) ,sin(a) ,aval ,orbit
	fsubp	st(2),st                ; ,sin(a)*xval ,yval-x*x ,new_xval ,cos(a) ,sin(a) ,aval ,orbit
	fxch	st(1)                   ; ,yval-x*x ,sin(a)*xval ,new_xval ,cos(a) ,sin(a) ,aval ,orbit
	fmul	st,st(3)                ; ,(yval-x*x)*cos(a) ,sin(a)*xval ,new_xval ,cos(a) ,sin(a) ,aval ,orbit
	fadd                            ; ,new_yval ,new_xval ,cos(a) ,sin(a) ,aval ,orbit
	call	pixelpixelize
	loopd	sts

	fstp	st
	fstp	st
	fstp	st
	fstp	st
	fld	bval
	faddp	st(2),st
	call	kbhit
	ret

endp    FpuHenon




      PUBLIC HopSinCos
HopSinCos Proc

	fsincos
	fmul
	ret


HopSinCos ENDP



      PUBLIC HopSin
HopSin Proc

      fsin
      ret


HopSin ENDP


      PUBLIC HopCos
HopCos Proc

;      fcos
      call	FpuCosh
      ret


HopCos ENDP

      PUBLIC HopSqrt
HopSqrt Proc

      fsqrt
      ret


HopSqrt ENDP

      PUBLIC HopNull
HopNull Proc

      ret


HopNull ENDP



public FpuHopalonginit
FpuHopalonginit PROC

	fld	aval
	fld	bval
	fld	cval
	fldz
	fldz
	ret

endp    FpuHopalonginit


public FpuHopalongend
FpuHopalongend PROC

	fstp	st
	fstp	st
	fstp	st
	fstp	st
	fstp	st
	ret

endp    FpuHopalongend


public FpuHopalong
FpuHopalong PROC


strtxx:
	ftst
	fstsw	ax
	fld1
	sahf
	jae	okx
	fchs                        	; ,1 o -1 ,xval ,yval ,cval ,bval ,aval
okx:
	fld	st(4)
	fmul	st,st(2)                ; ,bval*xval ,1 o -1 ,xval ,yval ,cval ,bval ,aval
	fsub    st,st(4)                ; ,bval*xval-cval ,1 o -1 ,xval ,yval ,cval ,bval ,aval
	fabs
	call	addrHop
	fmul				; sign*Fn(abs(bval*xval-cval)) ,xval ,yval ,cval ,bval ,aval
	fsubp	st(2),st                ; ,xval ,new_xval ,cval ,bval ,aval
	fsubr	st,st(4)                ; ,new_yval ,new_xval ,cval ,bval ,aval
	fxch	st(1)                   ; ,new_xval ,new_yval ,cval ,bval ,aval

	call	pixelpixelize

	xor	ax,ax

	or	cx,cx
	jnz	fhop
	call	kbhit
fhop:
	ret

endp    FpuHopalong

public FpuMartininit
FpuMartininit PROC

	fld	aval
	fldz
	fldz
	ret

endp    FpuMartininit


public FpuMartinend
FpuMartinend PROC

	fstp	st
	fstp	st
	fstp	st
	ret

endp    FpuMartinend

public FpuMartin
FpuMartin PROC


	ftst
	fstsw	ax
	fld1
	sahf
	jb	omar
	fchs
omar:
	fadd	st,st(2)                ; ,new_xval ,xval ,yval ,aval
	fxch	st(2)                   ; ,yval ,xval ,new_xval ,aval
	fstp	st                      ; ,xval ,new_xval ,aval
	fsubr	st,st(2)
;	fstp	st                      ; ,new_xval ,aval
;	fld	st(1)                   ; ,aval ,new_xval ,aval
;	fsubr	st,st(1)


	call	pixelpixelize
	fxch    st(1)                   ; ,xval ,yval ,aval

	xor	ax,ax
	or	cx,cx
	jnz	fmar
	call	kbhit
fmar:
	ret

endp    FpuMartin


public	FpuPlankinit
FpuPlankinit PROC


	fld	aval
	fld	bval
	fld1
	fld1
	ret


endp	FpuPlankinit

public	FpuPlankend
FpuPlankend PROC

	fstp	st
	fstp	st
	fstp	st
	fstp	st
	ret

endp	FpuPlankend

;------------Plankton
;
;xval=aval*yval + bval*xval + 2*xval*xval*(1-bval)/(1+xval*xval);
;yval=-temp     + bval*xval + 2*xval*xval*(1-bval)/(1+xval*xval);
;--------------------



public FpuPlank
FpuPlank PROC                           ; ,yval ,xval ,bval ,aval

	fld	st(1)
	fmul	st,st
	fld	st
	fld1
	fadd    st(1),st                ; ,1 ,1+x*x ,x*x ,yval ,xval ,bval ,aval
	fsub    st,st(5)                ; ,1-bval ,1+x*x ,x*x ,yval ,xval ,bval ,aval
	fdivr                           ; ,(1-bval)\(1+x*x) ,x*x ,yval ,xval ,bval ,aval
	fmul                            ; ,(x*x )(1-bval)\(1+x*x) ,yval ,xval ,bval ,aval
	fadd	st,st			; ,2*(x*x )(1-bval)\(1+x*x) ,yval ,xval ,bval ,aval
	fld	st(3)                   ; ,bval ,2*(x*x )(1-bval)\(1+x*x) ,yval ,xval ,bval ,aval
	fmul    st,st(3)		; ,bval*xval ,2*(x*x )(1-bval)\(1+x*x) ,yval ,xval ,bval ,aval
	fadd                            ; ,(bval*xval)+2*(x*x )(1-bval)\(1+x*x) ,yval ,xval ,bval ,aval
	fxch	st(1)                   ; ,yval ,(bval*xval)+2*(x*x )(1-bval)\(1+x*x) ,xval ,bval ,aval
	fmul	st,st(4)
	fadd                            ; ,new_xval ,xval ,bval ,aval
	fxch	st(1)

        fld	st(1)
	fmul	st,st
	fld	st
	fld1
	fadd    st(1),st                ; ,1 ,1+x*x ,x*x ,xval ,new_xval ,bval ,aval
	fsub    st,st(5)                ; ,1-bval ,1+x*x ,x*x ,xval ,new_xval ,bval ,aval
	fdivr                           ; ,(1-bval)\(1+x*x) ,x*x ,xval ,new_xval ,bval ,aval
	fmul                            ; ,(x*x )(1-bval)\(1+x*x) ,xval ,new_xval ,bval ,aval
	fadd	st,st			; ,2*(x*x )(1-bval)\(1+x*x) ,xval ,new_xval ,bval ,aval
	fld	st(3)                   ; ,bval ,2*(x*x )(1-bval)\(1+x*x) ,xval ,new_xval ,bval ,aval
	fmul    st,st(3)		; ,bval*xval ,2*(x*x )(1-bval)\(1+x*x) ,xval ,new_xval ,bval ,aval
	fadd                            ; ,(bval*xval)+2*(x*x )(1-bval)\(1+x*x) ,xval ,new_xval ,bval ,aval
	fsubr                           ; ,new_yval ,new_xval ,bval ,aval

	call	pixelpixelize

	xor	ax,ax
	or	cx,cx
	jnz	fpla
	call	kbhit
fpla:
	ret

endp    FpuPlank






public FpuPixelize
FpuPixelize PROC Uses di si


	mov     ax,0a000h
	mov     es,ax
	mov	di,risoluzione_x
	mov	si,risoluzione_y
	mov	ecx,iteraz

	call	initpixelize

s0trt:

	call	internalpixelize


	or	ax,ax
	jnz	e0ndz

	loopd	s0trt
	xor	ax,ax

e0ndz:
	call	endpixelize
	ret

endp    FpuPixelize






public julia_pixel_stack
julia_pixel_stack proc

	or	ecx,ecx                 ;se lago-> pixel=0..
	jnz	nzero07
	mov	gs:[bx],cl
	jmp	short cnt07
nzero07:
	sub     ecx,iteraz              ;..altrimenti colore pixel
	neg	ecx
	inc	ecx
	mov     gs:[bx],cl
cnt07:
	add	bl,dh                   ;x+=step
	adc	bh,0

	ret

julia_pixel_stack endp

public julia_pixel_reverse
julia_pixel_reverse proc

	or	ecx,ecx                 ;se lago-> salta e colore..
	jz	zero99
	mov     gs:[bx],0
	add	bl,dh                   ;x+=step
	adc	bh,0
	ret

zero99:
	fld	st(1)
	fadd	st,st(1)
	fimul	due56
	fistp	word ptr gs:[bx]
;	fistp	tempw
;	mov	cx,tempw
;	mov	gs:[bx],cl
	add	bl,dh                   ;x+=step
	adc	bh,0
	ret

julia_pixel_reverse endp



public juliabegin
juliabegin proc

	fld	oldxmin
	fstp	xmin
	mov     bx,word ptr bex
	mov	di,risoluzione_x
	add	di,bx
	mov	dh,step
	ret

juliabegin endp





public converg_re_im_2
converg_re_im_2 PROC


	fld	st(1)			;d=a*a+b*b
	fmul	st,st
	fld	st(1)
	fmul	st,st
	fld	st(1)
	fadd    st,st(1)                ;st=d st(1)=zj st(2)=zr

	ficomp  bailout			 ;if(d>=4) break
	fstsw	ax
	sahf
	setae   al
	ret


converg_re_im_2 endp


public converg_re_2
converg_re_2 PROC

	fld	st(1)
	fmul    st,st                   ; st=zr*zr st(1)=zj st(2)=zr
	ficom   bailout                 ;if(d>=4) break
	fstsw	ax
	sahf
	jb	kkre1
	fld	st
	mov	al,1
	ret
kkre1:
	fld	st(1)
	fmul	st,st
	fcom	MY_OVERFLOWD
	fstsw	ax
	sahf
	setae	al

	ret

converg_re_2 endp

public converg_im_2
converg_im_2 PROC

	fld	st(1)
	fmul    st,st                   ; st=zr*zr st(1)=zj st(2)=zr
	fcom	MY_OVERFLOWD
	fstsw	ax
	sahf
	jb	kkim1
	mov	al,1
	fld	st
	ret
kkim1:
	fld	st(1)
	fmul    st,st
	ficom   bailout                 ;if(d>=4) break
	fstsw	ax
	sahf
	setae	al
	ret

converg_im_2 endp



public converg_re_im_3
converg_re_im_3 PROC


	fld	st(1)			;d=a*a+b*b
	fmul	st,st
	fld	st(1)
	fmul	st,st
	fld	st(1)
	fadd    st,st(1)                ;st=d st(1)=zj st(2)=zr

	ficom   bailout			 ;if(d>=4) break
	fstsw	ax
	sahf
	setae   al
	ret


converg_re_im_3 endp


public converg_re_3
converg_re_3 PROC

	fld	st(1)
	fmul    st,st                   ; st=zr*zr st(1)=zj st(2)=zr
	ficom   bailout                 ;if(d>=4) break
	fstsw	ax
	sahf

	jb	kkre2
	mov	al,1
	fld	st
	fld	st
	ret
kkre2:

	fld	st(1)
	fmul	st,st
	fld	st(1)
	fadd    st,st(1)                ;st=d st(1)=zj st(2)=zr

	fcom	MY_OVERFLOWD
	fstsw	ax
	sahf
	setae	al

	ret

converg_re_3 endp

public converg_im_3
converg_im_3 PROC

	fld	st(1)
	fmul    st,st                   ; st=zr*zr st(1)=zj st(2)=zr
	fcom	MY_OVERFLOWD
	fstsw	ax
	sahf

	jb	kkim2
	mov	al,1
	fld	st
	fld	st
	ret
kkim2:

	fld	st(1)
	fmul    st,st
	ficom   bailout                 ;if(d>=4) break
	fstsw	ax
	sahf
	setae	al

	fld	st(1)
	fadd    st,st(1)                ;st=d st(1)=zj st(2)=zr
	ret

converg_im_3 endp




public converg_re_im
converg_re_im PROC

	fld	st(1)			;d=a*a+b*b
	fmul	st,st
	fld	st(1)
        fmul    st,st                   ;st=zj*zj st(1)=zr*zr st(2)=zj st(3)=zr
	fadd
	ficomp  bailout                 ;if(d>=4) break
	fstsw	ax
	sahf
	setae   al
	ret

converg_re_im endp

public converg_re
converg_re PROC

	fld	st(1)
	fmul    st,st                   ; st=zr*zr st(1)=zj st(2)=zr
	ficomp  bailout                 ;if(d>=4) break
	fstsw	ax
	sahf
	jb	kkre
	mov	al,1
	ret
kkre:
	fld	st
	fabs
	ficomp	MY_OVERFLOW
	fstsw	ax
	sahf
	setae	al

	ret

converg_re endp

public converg_im
converg_im PROC

	fld	st
	fmul    st,st                   ; st=zr*zr st(1)=zj st(2)=zr
	ficomp  bailout                 ;if(d>=4) break
	fstsw	ax
	sahf
	jb	kkim
	mov	al,1
	ret
kkim:
	fld	st(1)
	fabs
	ficomp	MY_OVERFLOW
	fstsw	ax
	sahf
	setae	al

	ret

converg_im endp


;------------------------------------------------
;       ca=Exp(a)*cos(b)
;       cb=Exp(a)*sin(b)
;	zr=ca*kostre-cb*kostim+cr;
;       zj=cb*kostre+ca*kostim+cj;
;------------------------------------------------


public FpuJuliaExp
FpuJuliaExp proc

	fsincos				;0=cos(b) 1=sin(b) 2=a
	fxch	st(2)
	call	FpuExp                  ;0=Exp(a) 1=sin(b) 2=cos(b)
	fxch    st(1)
	fmul	st,st(1)                ;0=Exp(a)*sin(b) 1=Exp(a) 2=cos(b)
	fxch	st(2)
	fmul                            ; =Exp(a)*cos(b) =Exp(a)*sin(b)

	fld	st(1)
	fmul	kostim
	fld	st(1)
	fmul	kostre
	fsubr                            ; =zr =Exp(a)*cos(b) =Exp(a)*sin(b)
	fadd    cr

	fxch	st(2)

	fmul	kostre
	fxch	st(1)
	fmul	kostim
	fadd
	fadd	cj

	ret

FpuJuliaExp endp




;------------------------------------------------
;       ca=Exp(a)*cos(b)
;       cb=Exp(a)*sin(b)
;	zr=ca*kostre-cb*kostim+cr;
;       zj=cb*kostre+ca*kostim+cj;
;------------------------------------------------


public FpuMandelExp
FpuMandelExp proc

	fsincos				;0=cos(b) 1=sin(b) 2=a
	fxch	st(2)
	call	FpuExp                  ;0=Exp(a) 1=sin(b) 2=cos(b)
	fxch    st(1)
	fmul	st,st(1)                ;0=Exp(a)*sin(b) 1=Exp(a) 2=cos(b)
	fxch	st(2)
	fmul                            ; =Exp(a)*cos(b) =Exp(a)*sin(b)

	fld	st(1)
;	fld	ymin
	fmul    st,st(3)
	fld	st(1)
;	fld     xmin
	fmul    st,st(5)
	fsubr                            ; =zr =Exp(a)*cos(b) =Exp(a)*sin(b)
	fadd    cr

	fxch	st(2)

;	fld	xmin
	fmul    st,st(4)
	fxch	st(1)
;	fld	ymin
	fmul    st,st(3)
	fadd
	fadd	cj

	ret

FpuMandelExp endp




;------------------------------------------------
;	sc=FpuSin(a)*FpuCosh(b);
;	cs=FpuCos(a)*FpuSinh(b);
;	zr=cs*kostre-sc*kostim+cr;
;	zj=sc*kostim+cs*kostim+cj;
;------------------------------------------------


public FpuJuliaSin
FpuJuliaSin proc

	fxch	st(1)
	fsincos				;0=cos(a) 1=sin(a) 2=b

;  FpuCosh

	fxch	st(2)                   ;0=b 1=sin(a) 2=cos(a)
	call	FpuExp                  ;0=e^b 1=sin(a) 2=sin(a)
	fld1
	fdiv	st,st(1)                ;0=e^(-b) 1=e^b 2=sin(a) 3=sin(a)
	fld	st(1)                   ;0=e^b 1=e^(-b) 2=e^b 3=sin(a) 4=sin(a)
	fadd	st,st(1)                ;0=e^b+e^(-b) 1=e^(-b) 2=e^b 3=sin(a) 4=sin(a)
;	fild	due
	fidiv   due                     ;0=Cosh 1=e^(-b) 2=e^b 3=sin(a) 4=sin(a)
	fxch	st(2)                   ;0=e^b 1=e^(-b) 2=Cosh 3=sin(a) 4=sin(a)
	fsubr                           ;0=e^b-e^(-b) 1=Cosh 2=sin(a) 3=sin(b)
;	fild	due
	fidiv	due                        ;0=Sinh 1=Cosh 2=sin(a) 3=cos(b)
	fxch	st(2)                   ;0=sin(a) 1=Cosh 2=Sinh 3=cos(a)


	fmul                            ; 0=Cosh(b)*sin(a) =Sinh =cos(a)
	fxch	st(2)
	fmul                            ; =cs =sc

	fld	st(1)
	fmul	kostre
	fld	st(1)
	fmul	kostim
	fsub
	fadd	cr			; =zr =cs =sc

	fxch	st(2)

	fmul	kostim                  ; =sc =cs =zr
	fxch	st(1)
	fmul	kostre
	fadd
	fadd    cj                        ;=zj =zr

	ret                             ; 0=new_b 1=new_a

FpuJuliaSin endp





;------------------------------------------------
;	sc=FpuSin(a)*FpuCosh(b);
;	cs=FpuCos(a)*FpuSinh(b);
;	zr=cs*xmin-sc*ymin+cr;
;	zj=sc*xmin+cs*ymin+cj;
;------------------------------------------------


public FpuMandelSin
FpuMandelSin proc

	fxch	st(1)
	fsincos				;0=cos(a) 1=sin(a) 2=b

;  FpuCosh

	fxch	st(2)                   ;0=b 1=sin(a) 2=cos(a)
	call	FpuExp                  ;0=e^b 1=sin(a) 2=sin(a)
	fld1
	fdiv	st,st(1)                ;0=e^(-b) 1=e^b 2=sin(a) 3=sin(a)
	fld	st(1)                   ;0=e^b 1=e^(-b) 2=e^b 3=sin(a) 4=sin(a)
	fadd	st,st(1)                ;0=e^b+e^(-b) 1=e^(-b) 2=e^b 3=sin(a) 4=sin(a)
;	fild	due
	fidiv   due                     ;0=Cosh 1=e^(-b) 2=e^b 3=sin(a) 4=sin(a)
	fxch	st(2)                   ;0=e^b 1=e^(-b) 2=Cosh 3=sin(a) 4=sin(a)
	fsubr                           ;0=e^b-e^(-b) 1=Cosh 2=sin(a) 3=sin(b)
;	fild	due
	fidiv	due                        ;0=Sinh 1=Cosh 2=sin(a) 3=cos(b)
	fxch	st(2)                   ;0=sin(a) 1=Cosh 2=Sinh 3=cos(a)


	fmul                            ; 0=Cosh(b)*sin(a) =Sinh =cos(a)
	fxch	st(2)
	fmul                            ; =cs =sc

	fld	st(1)
;	fld	xmin
	fmul    st,st(4)
	fld	st(1)
;	fld	ymin
	fmul    st,st(4)
	fsub				; =zr =cs =sc

	fxch	st(2)
;	fld	ymin
	fmul    st,st(3)
	fxch	st(1)
;	fld	xmin
	fmul    st,st(4)
	fadd                            ;=zj =zr

	ret                             ; 0=new_b 1=new_a

FpuMandelSin endp





;------------------------------------------------
;	ss=FpuSin(a)*FpuSinh(b);
;	cc=FpuCos(a)*FpuCosh(b);
;	zr=cc*kostre+ss*kostim+cr;
;	zj=cc*kostim-ss*kostre+cj;
;------------------------------------------------



public FpuJuliaCos
FpuJuliaCos proc

	fxch	st(1)
	fsincos				;0=cos(a) 1=sin(a) 2=b
	fxch	st(2)
	call	FpuExp                  ;0=e^b 1=sin(a) 2=sin(a)
	fld1
	fdiv	st,st(1)                ;0=e^(-b) 1=e^b 2=sin(a) 3=sin(a)
	fld	st(1)                   ;0=e^b 1=e^(-b) 2=e^b 3=sin(a) 4=sin(a)
	fadd	st,st(1)                ;0=e^b+e^(-b) 1=e^(-b) 2=e^b 3=sin(a) 4=sin(a)
	fidiv   due                         ;0=Cosh 1=e^(-b) 2=e^b 3=sin(a) 4=sin(a)
	fxch	st(2)                   ;0=e^b 1=e^(-b) 2=Cosh 3=sin(a) 4=sin(a)
	fsubr                           ;0=e^b-e^(-b) 1=Cosh 2=sin(a) 3=sin(b)
	fidiv	due                     ;0=Sinh 1=Cosh 2=sin(a) 3=cos(b)
	fxch	st(3)                   ;0=cos(a) 1=Cosh 2=sin(a) 3=Sinh

	fmul                            ;0=Cosh(b)*cos(a) =sin(b) =Sinh(b)

	fxch	st(2)                   ; =Sinh(b) =sin(b) =Cosh(b)*cos(a)

        fmul                            ; =Sinh(b)*Sin(a) =Cosh(b)*cos(a)

	fld	st(1)
	fmul	kostre
	fld	st(1)
	fmul	kostim
	fadd
	fadd    cr                        ; =zr =Sinh(b)*Sin(a) =Cosh(b)*cos(a)
	fxch	st(2)
	fmul	kostim                  ; =Cosh(b)*cos(a) =Sinh(b)*Sin(a) =zr
	fxch	st(1)                   ;
	fmul	kostre                  ; =Sinh(b)*Sin(a)*re =Cosh(b)*cos(a)*im =zr
	fsubr
	fadd    cj

	ret                             ; 0=new_b 1=new_a

FpuJuliaCos endp

;------------------------------------------------
;	ss=FpuSin(a)*FpuSinh(b);
;	cc=FpuCos(a)*FpuCosh(b);
;	zr=cc*xmin+ss*ymin+cr;
;	zj=cc*ymin-ss*xmin+cj;
;------------------------------------------------




public FpuMandelCos
FpuMandelCos proc

	fxch	st(1)
	fsincos				;0=cos(a) 1=sin(a) 2=b
	fxch	st(2)
	call	FpuExp                  ;0=e^b 1=sin(a) 2=sin(a)
	fld1
	fdiv	st,st(1)                ;0=e^(-b) 1=e^b 2=sin(a) 3=sin(a)
	fld	st(1)                   ;0=e^b 1=e^(-b) 2=e^b 3=sin(a) 4=sin(a)
	fadd	st,st(1)                ;0=e^b+e^(-b) 1=e^(-b) 2=e^b 3=sin(a) 4=sin(a)
	fidiv   due                         ;0=Cosh 1=e^(-b) 2=e^b 3=sin(a) 4=sin(a)
	fxch	st(2)                   ;0=e^b 1=e^(-b) 2=Cosh 3=sin(a) 4=sin(a)
	fsubr                           ;0=e^b-e^(-b) 1=Cosh 2=sin(a) 3=sin(b)
	fidiv	due                     ;0=Sinh 1=Cosh 2=sin(a) 3=cos(b)
	fxch	st(3)                   ;0=cos(a) 1=Cosh 2=sin(a) 3=Sinh

	fmul                            ;0=Cosh(b)*cos(a) =sin(b) =Sinh(b)

	fxch	st(2)                   ; =Sinh(b) =sin(b) =Cosh(b)*cos(a)

	fmul                            ; =Sinh(b)*Sin(a) =Cosh(b)*cos(a) =ymin =xmin

	fld	st(1)                   ; =Cosh(b)*cos(a) =Sinh(b)*Sin(a) =Cosh(b)*cos(a) =ymin =xmin
;	fld	xmin
	fmul    st,st(4)
	fld	st(1)
;	fld	ymin
	fmul    st,st(4)
	fadd
	fxch	st(2)                   ;=Cosh(b)*cos(a) =Sinh(b)*Sin(a)  =zr =ymin =xmin
;	fld	ymin
	fmul    st,st(3)
	fxch	st(1)
;	fld	xmin
	fmul    st,st(4)
	fsubr	   			; =zj =zr

	ret                             ; 0=new_b 1=new_a

FpuMandelCos endp







public FpuJuliafn
FpuJuliafn PROC	 Uses di

	call	juliabegin
	fld	xmin
	fld	ymin

start07:

	mov	ecx,iteraz
	fld	st(1)
	fld	st(1)



inizio07:


	call	internalfunz

	call 	convergenza



	or	al,al
	jnz     short fine07	    ;st=zj st(1)=zr

	loopd	inizio07
fine07:

	call    julia_pixel_stack
	fstp	st
	fstp	st

	fld	stx                    ;xmin+=stx
	faddp   st(2),st


	cmp     bx,di
	jge     endx07
	jmp	start07
endx07:

;	fld	ymin                    ;ymin+=sty
	fld	sty
	fadd
	fstp	ymin                    ;
	fstp	st
	ret

FpuJuliafn endp




public FpuMandelfn
FpuMandelfn PROC  Uses di

	call	juliabegin
	fld	xmin
	fld	ymin


start09:

	fld	st(1)
	fadd    cr
	fld	st(1)
	fadd    cj
	mov	ecx,iteraz

inizio09:


	call	internalfunz

	call 	convergenza


	or	al,al
	jnz     short fine09	    ;st=zj st(1)=zr

	loopd	inizio09
fine09:

	call    julia_pixel_stack
	fstp	st
	fstp	st

	fld	stx                    ;xmin+=stx
	faddp   st(2),st


;	fld	xmin                    ;xmin+=stx
;	fld	stx
;	fadd
;	fstp	xmin                    ;


	cmp     bx,di
	jge     endx09
	jmp	start09
endx09:

;	fld	ymin                    ;ymin+=sty
	fld	sty
	fadd
	fstp	ymin                    ;

	fstp	st                      ;pop xmin

	ret

FpuMandelfn endp


public FpuJulia
FpuJulia PROC Uses di

	call    juliabegin
	fld	xmin
	fld	ymin

start01:

	mov	ecx,iteraz

	fld	st(1)
	fld	st(1)
	fld	st(1)                   ;zr=cr*cr-cj*cj+xmin
	fmul	st,st
	fld	st(1)
	fmul	st,st

inizio01:


	fsub
	fadd    cr                      ;st=zr st(1)=cj st(2)=cr
	fxch	st(2)                   ;st=cr st(1)=cj st(2)=zr

	fmul                            ;zi=2*cr*cj+ymin
	fadd	st,st
	fadd	cj			;st=zi st(1)=zr

	call	convergenza


	or	al,al
	jnz     short fine01            ;st=zj st(1)=zr

	loopd	inizio01
fine01:

	call	julia_pixel_reverse

	fstp    st
	fstp    st
	fstp	st
	fstp	st

	fld	stx                     ;xmin+=stx
	faddp   st(2),st


	cmp     bx,di
	jge     endx01
	jmp	start01
endx01:

	fld	sty                     ;ymin+=sty
	fadd
	fstp	ymin                    ;

	fstp	st                      ;pop xmin

	ret

FpuJulia endp



public FpuJuliaEps
FpuJuliaEps PROC Uses di

	call	juliabegin

start1:

	mov	ecx,iteraz
	fldz
	fld	xmin
	fld	ymin
	fld	st(1)                   ;zr=cr*cr-cj*cj+xmin
	fmul	st,st
	fld	st(1)
	fmul	st,st


inizio1:

	;fld	st                     ;zr=cj*cj-cr*cr+xmin
	;fmul	st,st
	;fld	st(2)
	;fmul	st,st

	fsub
	fadd    cr                        ;st=zr st(1)=cj st(2)=cr
	fxch	st(2)                   ;st=cr st(1)=cj st(2)=zr

	fmul                            ;zi=2*cr*cj+ymin
	fadd	st,st
	fadd	cj			;st=zi st(1)=zr


	call	convergenza


	or	al,al
	jnz     short fine10            ;st=d st(1)=zj st(2)=zr

	fld     st(5)
	fsub    st,st(1)
	fabs
	fcomp   epsilon                 ;if(d>=4) break
	fstsw	ax
	fstp	st(5)
	sahf
	jb      short fine1             ;st=zj st(1)=zr


	loopd	inizio1
	jmp	short fine1
fine10:
	fstp	st
fine1:

	call	julia_pixel_stack

	fstp	st
	fstp	st
	fstp	st
	fstp	st
	fstp	st

	fld	xmin                    ;xmin+=stx
	fld	stx
	fadd
	fstp	xmin                    ;


	cmp	bx,di
	jg	endx1
	jmp	start1
endx1:
	fld	ymin                    ;ymin+=sty
	fld	sty
	fadd
	fstp	ymin                    ;

	ret

FpuJuliaEps endp


public FpuMandel
FpuMandel PROC Uses di


	call	juliabegin
	fld	xmin
	fld	ymin

start0:

	mov	ecx,iteraz


	fld	cr
	fld	cj
	fld	st(1)                   ;zr=cr*cr-cj*cj+xmin
	fmul	st,st
	fld	st(1)
	fmul	st,st


inizio0:


	fsub
;	fld	xmin
	fadd	st,st(4)

	fxch	st(2)                   ;st=cr st(1)=cj st(2)=zr

	fmul                            ;zi=2*cr*cj+ymin
	fadd	st,st
;	fld	ymin
	fadd    st,st(2)

	call	convergenza

	or	al,al
	jnz     short fine0             ;st=zj st(1)=zr

	loopd	inizio0
fine0:

	call	julia_pixel_stack

	fstp	st
	fstp	st
	fstp	st
	fstp	st


	fld	stx                    ;xmin+=stx
	faddp   st(2),st

	cmp     bx,di
	jge     endx0
	jmp	start0
endx0:

	fld	sty                     ;ymin+=sty
	fadd
	fstp	ymin                    ;
	fstp	st                      ;pop xmin


	ret

FpuMandel endp


public FpuBran
FpuBran PROC Uses di


	call	juliabegin
	fld	xmin
	fld	ymin                   ; ymin ,xmin

FORLOOP:

	mov	ecx,iteraz


	fld     cr
	fld     cj                     ; zi ,zr , ymin, xmin
	fld	st(3)                  ; zr ,zi , zr, ymin ,xmin
	fmul	st,st                  ; zr*zr ,zi , zr, ymin ,xmin
	fld	st(3)                  ; zi ,zr*zr ,zi , zr, ymin ,xmin
	fmul	st,st                  ; zi*zi ,zr*zr ,zi , zr, ymin ,xmin
	fadd                           ; kl ,zi , zr, ymin ,xmin
	fxch	st(2)                  ; zr ,zi , kl, ymin ,xmin

DOLOOP:

	fldz                           ; 0 ,zr ,zi , kl, ymin ,xmin
	fcomp   st(1)                  ; zr ,zi , kl, ymin ,xmin
	fstsw	ax
	sahf
	ja      MINORE
	fisub	uno                    ;  zr-1 ,zi , kl, ymin ,xmin
	fld	st(4)                  ;  xmin ,zr-1 ,zi , kl, ymin ,xmin
	fmul    st,st(1)               ;  zr*xmin ,zr ,zi , kl, ymin ,xmin
	fxch    st(1)                  ;  zr ,  zr*xmin ,zi , kl, ymin ,xmin
	fmul    st,st(4)               ;  zr*ymin ,zr*xmin ,zi , kl, ymin ,xmin
	fld     st(4)                  ;  ymin ,zr*ymin ,zr*xmin ,zi , kl, ymin ,xmin
	fmul	st,st(3)               ;  zi*ymin ,zr*ymin ,zr*xmin ,zi , kl, ymin ,xmin
	fxch	st(3)                  ;  zi ,zr*ymin ,zr*xmin ,zi*ymin , kl, ymin ,xmin
	fmul	st,st(6)               ;  zi*xmin ,zr*ymin ,zr*xmin ,zi*ymin , kl, ymin ,xmin
	fsubr                          ;  zi*xmin - zr*ymin ,zr*xmin ,zi*ymin , kl, ymin ,xmin
	fdiv	st,st(3)               ;  b ,zr*xmin ,zi*ymin , kl, ymin ,xmin
	fxch	st(2)                  ;  zi*ymin, zr*xmin , b, , kl, ymin ,xmin
	fadd                           ;  zi*ymin+zr*xmin , a, kl, ymin ,xmin
	fdiv    st,st(2)               ;  b , a, kl, ymin ,xmin
	jmp	CONTINUA
MINORE:
	fiadd	uno
	fld	st(4)                  ;  xmin ,zr-1 ,zi , kl, ymin ,xmin
	fmul    st,st(1)               ;  zr*xmin ,zr ,zi , kl, ymin ,xmin
	fxch    st(1)                  ;  zr ,  zr*xmin ,zi , kl, ymin ,xmin
	fmul    st,st(4)               ;  zr*ymin ,zr*xmin ,zi , kl, ymin ,xmin
	fld     st(4)                  ;  ymin ,zr*ymin ,zr*xmin ,zi , kl, ymin ,xmin
	fmul	st,st(3)               ;  zi*ymin ,zr*ymin ,zr*xmin ,zi , kl, ymin ,xmin
	fxch	st(3)                  ;  zi ,zr*ymin ,zr*xmin ,zi*ymin , kl, ymin ,xmin
	fmul	st,st(6)               ;  zi*xmin ,zr*ymin ,zr*xmin ,zi*ymin , kl, ymin ,xmin
	fadd                           ;  zi*xmin + zr*ymin ,zr*xmin ,zi*ymin , kl, ymin ,xmin
	fdiv	st,st(3)               ;  b ,zr*xmin ,zi*ymin , kl, ymin ,xmin
	fxch	st(2)                  ;  zi*ymin, zr*xmin , b, , kl, ymin ,xmin
	fsub                           ;  -zi*ymin+zr*xmin , a, kl, ymin ,xmin
	fdiv    st,st(2)               ;  b , a, kl, ymin ,xmin


CONTINUA:


	call	convergenza

	or	al,al
	jnz     short CONVERGE         ;st=zj st(1)=zr

	loopd	DOLOOP

CONVERGE:

	call	julia_pixel_stack

	fstp	st
	fstp	st
	fstp	st


	fld	stx                    ;xmin+=stx
	faddp   st(2),st

	cmp     bx,di
	jge     ENDOS
	jmp	FORLOOP
ENDOS:

	fld	sty                     ;ymin+=sty
	fadd
	fstp	ymin                    ;
	fstp	st                      ;pop xmin


	ret

FpuBran endp




public FpuMandelEps
FpuMandelEps PROC Uses di


	call	juliabegin
start02:

	mov	ecx,iteraz
	fldz
	fld	cr
	fld	cj
	fld	st(1)                   ;zr=cr*cr-cj*cj+xmin
	fmul	st,st
	fld	st(1)
	fmul	st,st


inizio02:

	;fld	st                     ;zr=cj*cj-cr*cr+xmin
	;fmul	st,st
	;fld	st(2)
	;fmul	st,st

	fsub
	fld	xmin
	fadd
	fxch	st(2)                   ;st=cr st(1)=cj st(2)=zr

	fmul                            ;zi=2*cr*cj+ymin
	fadd	st,st
	fld	ymin
	fadd				;st=zi st(1)=zr

	call    convergenza


	or	al,al
	jnz     short fine02            ;st=d st(1)=zj st(2)=zr

	fld     st(5)
	fsub    st,st(1)
	fabs
	fcomp   epsilon                 ;if(d>=4) break
	fstsw	ax
	fstp	st(5)
	sahf
	jb      short fine002           ;st=zj st(1)=zr


	loopd	inizio02
	jmp	short fine002
fine02:
	fstp	st
fine002:
	call	julia_pixel_stack

	fstp    st
	fstp	st
	fstp    st
	fstp    st
	fstp	st

	fld	xmin                    ;xmin+=stx
	fld	stx
	fadd
	fstp	xmin                    ;


	cmp	bx,di
	jg	endx02
	jmp	start02
endx02:
	fld	ymin                    ;ymin+=sty
	fld	sty
	fadd
	fstp	ymin                    ;

	ret

FpuMandelEps endp






public poppixel
poppixel PROC



;--------------------------------
;	Coord of pixel
;--------------------------------

	fld	st(1)
	fadd	absxmin
	fmul	rate_x
	fistp	col

	fld	st
	fadd	absymin
	fmul	rate_y
	fistp	row

;-------------------------------
;       Pixel on video
;-------------------------------
	mov     dx,row
	mov	bx,col

	or	dx,dx
	jl	tin
	cmp	dx,risoluzione_y
	jge	tin
	or	bx,bx
	jl	tin
	cmp	bx,di
	jge	tin


	clc
	mov     ax,di
	mul     dx
	add     ax,bx
	adc     dx,0
	mov     bx,ax
	mov     al,dl
	cli
	call    addrTseng
	mov	ax,rigx
	mov     byte ptr fs:[bx],al
;	inc     byte ptr fs:[bx]
tin:
	ret

poppixel endp





public FpuPopcorn
FpuPopcorn PROC  Uses di si



	fld	oldxmin
	fstp	xmin
	mov	di,risoluzione_x
	xor	si,si
	fld	xmin
	fld	ymin
s0x:
	fld	st(1)
	fld	st(1)
	mov	ecx,iteraz
g0:    					; ,ymin ,xmin ,ymin ,xmin
	fld	st                      ; ,ymin ,ymin ,xmin ,ymin ,xmin
	fimul	tre
	fptan
	fstp	st                      ; ,Tan(y*3) ,ymin ,xmin ,ymin ,xmin
	fadd    st,st(1)
	fsin                            ; ,Sin(y+Tan(y*3)) ,ymin ,xmin ,ymin ,xmin
	fmul	kostre
	fsubr   st,st(2)                ; ,xx ,ymin ,xmin ,ymin ,xmin
	fxch	st(2)
	fld	st			; ,xmin ,xmin ,ymin ,xx ,ymin ,xmin
	fimul	tre
	fptan
	fstp	st                      ; ,Tan(x*3) ,xmin ,ymin ,xx ,ymin ,xmin
	fadd
	fsin                            ; ,Sin(x+Tan(x*3)) ,ymin ,xx ,ymin ,xmin
	fmul	kostim
	fsub                            ; ,yy ,xx ,ymin ,xmin



	call	convergenza

	or	al,al
	jnz     short fe0

	call	poppixel

	loopd	g0

fe0:

	fstp	st
	fstp	st

	fld	stx                    ;xmin+=stx
	faddp   st(2),st

;	mov	al,step
;	xor	ah,ah
;	add	si,ax
	inc	si
	cmp	si,di
	jl	s0x


	fld	sty                     ;ymin+=sty
	fadd
	fstp	ymin                    ;
	fstp	st                      ;pop xmin


	ret

FpuPopcorn endp












public set_pixel
set_pixel PROC


	or	ecx,ecx                 ;se lago-> pixel=0..
	jnz	nz7
	mov	ecx,255
	jmp	short fixi
nz7:
	sub     ecx,iteraz              ;..altrimenti colore pixel
	neg	ecx
	inc	ecx
	or	cl,cl
	jne     fixi
	inc	ecx

fixi:
;-------------------------------
;       Pixel on video
;-------------------------------
	mov     dx,row
	clc
	mov     ax,risoluzione_x
	mul     dx
	add     ax,col
	adc     dx,0
	mov     bx,ax
	mov     al,dl
	cli
	call    addrTseng
	mov     byte ptr fs:[bx],cl
	xor	ax,ax
	mov	al,cl

	ret

set_pixel endp



public FpuJuliaEpsPixel
FpuJuliaEpsPixel PROC Uses di


	mov	ecx,iteraz
	fldz

        fld	x_min
	fld	stx
	fimul	col
	fadd
	fld	y_min
	fld	sty
	fimul	row
	fadd

	fld	st(1)                   ;zr=cr*cr-cj*cj+xmin
	fmul	st,st
	fld	st(1)
	fmul	st,st


ii1:


	fsub
	fadd    cr                        ;st=zr st(1)=cj st(2)=cr
	fxch	st(2)                   ;st=cr st(1)=cj st(2)=zr

	fmul                            ;zi=2*cr*cj+ymin
	fadd	st,st
	fadd	cj			;st=zi st(1)=zr


	call	convergenza


	or	al,al
	jnz     short ff10            ;st=d st(1)=zj st(2)=zr

	fld     st(5)
	fsub    st,st(1)
	fabs
	fcomp   epsilon                 ;if(d>=4) break
	fstsw	ax
	fstp	st(5)
	sahf
	jb      short ff1             ;st=zj st(1)=zr


	loopd	ii1
	jmp	short ff1
ff10:
	fstp	st
ff1:


	fstp	st
	fstp	st
	fstp	st
	fstp	st
	fstp	st

	call	set_pixel


	ret

FpuJuliaEpsPixel endp




public FpuMandelEpsPixel
FpuMandelEpsPixel PROC Uses di



	mov	ecx,iteraz

	fld	x_min
	fld	stx
	fimul	col
	fadd
	fstp	xmin
	fld	y_min
	fld	sty
	fimul	row
	fadd
	fstp	ymin


	fldz
	fld	cr
	fld	cj
	fld	st(1)                   ;zr=cr*cr-cj*cj+xmin
	fmul	st,st
	fld	st(1)
	fmul	st,st


i02:


	fsub
	fld	xmin
	fadd
	fxch	st(2)                   ;st=cr st(1)=cj st(2)=zr

	fmul                            ;zi=2*cr*cj+ymin
	fadd	st,st
	fld	ymin
	fadd				;st=zi st(1)=zr

	call    convergenza


	or	al,al
	jnz     short f02               ;st=d st(1)=zj st(2)=zr

	fld     st(5)
	fsub    st,st(1)
	fabs
	fcomp   epsilon                 ;if(d>=4) break
	fstsw	ax
	fstp	st(5)
	sahf
	jb      short f002              ;st=zj st(1)=zr


	loopd	i02
	jmp	short f002
f02:
	fstp	st
f002:

	fstp    st
	fstp	st
	fstp    st
	fstp    st
	fstp	st

	call	set_pixel

	ret

FpuMandelEpsPixel endp




public FpuMandelPixel
FpuMandelPixel PROC



	mov	ecx,iteraz

	fld	x_min
	fld	stx
	fimul	col
	fadd
	fld	y_min
	fld	sty
	fimul	row
	fadd

	fld	cr
	fld	cj
	fld	st(1)                   ;zr=cr*cr-cj*cj+xmin
	fmul	st,st
	fld	st(1)
	fmul	st,st


inz:


	fsub                            ;zr-zj , cj , cr , ymin , xmin
;	fld	st(4)
	fadd    st,st(4)

	fxch	st(2)                   ;st=cr st(1)=cj st(2)=zr

	fmul                            ;zi=2*cr*cj+ymin
	fadd	st,st
;	fld	st(2)
	fadd    st,st(2)

	call	convergenza

	or	al,al
	jnz     short fnc             ;st=zj st(1)=zr

	loopd	inz
fnc:

	fstp	st
	fstp	st
	fstp	st
	fstp	st
	fstp	st
	fstp	st

	call	set_pixel
	ret

FpuMandelPixel endp





public FpuJuliaPixel
FpuJuliaPixel PROC


	mov	ecx,iteraz

	fld	x_min
	fld	stx
	fimul	col
	fadd

	fld	y_min
	fld	sty
	fimul	row
	fadd

	fld	st(1)                   ;zr=cr*cr-cj*cj+xmin
	fmul	st,st
	fld	st(1)
	fmul	st,st

ini01:


	fsub
	fadd    cr                      ;st=zr st(1)=cj st(2)=cr
	fxch	st(2)                   ;st=cr st(1)=cj st(2)=zr

	fmul                            ;zi=2*cr*cj+ymin
	fadd	st,st
	fadd	cj			;st=zi st(1)=zr

	call	convergenza


	or	al,al
	jnz     short fi01            ;st=zj st(1)=zr

	loopd	ini01
fi01:


	fstp    st
	fstp    st
	fstp	st
	fstp	st

	call	set_pixel

	ret


FpuJuliaPixel endp




public FpuJuliaPixelfn
FpuJuliaPixelfn PROC


	mov	ecx,iteraz

	fld	x_min
	fld	stx
	fimul	col
	fadd

	fld	y_min
	fld	sty
	fimul	row
	fadd

iniz07:


	call	internalfunz

	call 	convergenza



	or	al,al
	jnz     short fin07	    ;st=zj st(1)=zr

	loopd	iniz07
fin07:

	call    set_pixel
	fstp	st
	fstp	st

	ret

FpuJuliaPixelfn endp




public FpuMandelPixelfn
FpuMandelPixelfn PROC


	mov	ecx,iteraz

	fld	x_min
	fld	stx
	fimul	col
	fadd

	fld	y_min
	fld	sty
	fimul	row
	fadd

	fld	st(1)
	fadd    cr
	fld	st(1)
	fadd    cj



iniz09:


	call	internalfunz

	call 	convergenza


	or	al,al
	jnz     short fin09	    ;st=zj st(1)=zr

	loopd	iniz09
fin09:

	call    set_pixel
	fstp	st
	fstp	st
	fstp	st
	fstp	st

	ret

FpuMandelPixelfn endp



     END

