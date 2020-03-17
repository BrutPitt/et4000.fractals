;------------------- ------------------
; fpu.asm - modulo assembler
;
;
    .MODEL LARGE,c              ; select near data pointer (faster),
.386                            ;  C parameter passing,
.387                            ;  automatically put '_' in front of publics

extrn	cr:qword
extrn	cj:qword
extrn	dif_x:tbyte
extrn   dif_y:tbyte
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

extrn	outonepixel:far
extrn	inpixel:far
extrn	kbhit:far
extrn	getkey:far
extrn	tasto:far
extrn	addrHop:dword
extrn	addrTseng:dword
extrn   convergenza:dword
extrn   internalfunz:dword
extrn	FpuExp:far


.DATA

control dw      0
due	dw	2
tre	dw	3
quattro dw	4
dieci   dw      10
cento   dw      100
oldcw   dw      0         ; original control word
newcw   dw      0         ; new control word
xco	dw	0
yco	dw	0
temp	dd	0
tempw	dw	0




.CODE



	public setGS
setGS   proc

	mov	gs,word ptr bex+2
	ret

setGS	endp

	public setFS
setFS   proc

	mov	ax,0a000h
	mov	fs,ax
	ret

setFS	endp


	public  Falog
Falog   proc x_addr:QWORD

        fld	x_addr
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

Falog   endp

PUBLIC fputype

fputype proc
	push	bp

	fninit				; Defaults to 64-bit mantissa
	mov	byte ptr control+1, 0
	fnstcw	control 		; Store control word over 0
;	dw	3ed9h			; (klooge to avoid the MASM \e switch)
;	dw	offset control		; ((equates to the above 'fnstcw' cmd))
	mov	ah, byte ptr control+1	; Test contents of byte written
	cmp	ah, 03h 		; Test for 64-bit precision flags
	je	gotone			; Got one!  Now let's find which
	xor	ax, ax
	jmp	short fexit		; No NDP found
gotone:
	and	control, not 0080h	; IEM = 0 (interrupts on)
	fldcw	control
	fdisi				; Disable ints; 287/387 will ignore
	fstcw	control
	test	control, 0080h
	jz	not87			; Got 287/387; keep testing
	mov	ax, 87
	jmp	short freset
not87:
	finit
	fld1
	fldz
	fdiv				; Divide 1/0 to create infinity
	fld	st
	fchs				; Push -infinity on stack
	fcompp				; Compare +-infinity
	fstsw	control
	mov	ax, control
	sahf
	jnz	got387			; 387 will compare correctly
	mov	ax, 287
	jmp	short freset
got387: 				; Only one left (until 487/Weitek
	mov	ax, 387 		;   test is added)
freset:
	fninit				; in case tests have had strange
	finit				; side-effects, reset
fexit:
	pop	bp
	ret
fputype endp


;    PUBLIC FpuInit
;FpuInit PROC
;    finit                       ; initialize Fpu
;    ret
;FpuInit ENDP



      PUBLIC FpuMul
FpuMul PROC x_addr:QWORD, y_addr:QWORD

      fld  x_addr
      fld  y_addr


      fmul
      ret

    ; now top of Fpu stack contains the result
    ; there's no need to store it in a mem variable because TC expects to
    ;  find it here.

FpuMul ENDP


           ; activate 80387-specific instructions

      PUBLIC FpuSin
FpuSin Proc x_addr:QWORD

      fld  x_addr
      fsin
      ret


FpuSin ENDP


      PUBLIC FpuTan
FpuTan Proc x_addr:QWORD

      fld  x_addr
      fptan
      fstp	st
      ret


FpuTan ENDP

      PUBLIC FpuCos
FpuCos Proc x_addr:QWORD

      fld  x_addr
      fcos
      ret


FpuCos ENDP

      PUBLIC FpuSinh
FpuSinh Proc x_addr:QWORD

	fld  	x_addr
	call	FpuExp
	fld1
	fdiv	st,st(1)
	fsub
	fidiv   due
	ret


FpuSinh ENDP


      PUBLIC FpuCosh
FpuCosh Proc x_addr:QWORD

	fld  	x_addr
	call	FpuExp
	fld1
	fdiv	st,st(1)
	fadd
	fidiv	due
	ret

FpuCosh ENDP


      PUBLIC FpuAtan
FpuAtan Proc x_addr:QWORD

      fld  x_addr
      fld1
      fpatan
      ret



FpuAtan ENDP

      PUBLIC FpuSqrt4
FpuSqrt4 Proc x_addr:QWORD

      fld  x_addr
      fsqrt
      fsqrt
      ret

FpuSqrt4 ENDP


      PUBLIC FpuSqrt
FpuSqrt Proc x_addr:QWORD

      fld  x_addr
      fsqrt
      ret

FpuSqrt ENDP








public FpuSprtSin2z
FpuSprtSin2z PROC Uses di


;	mov	eax,oldxmin
;	mov 	xmin,eax
	fld	oldxmin
	fstp	xmin
	mov	di,risoluzione_x
	mov	bx,word ptr bex
	add	di,bx
	mov	dl,step

start:
;	xor	dx,dx
;	mov	flagzr,dh                  ;dl=flagzr
;	mov	flagzj,dh                  ;dh=flagzj
	mov	ecx,iteraz
	fld	xmin
	fadd	st,st
	fld     ymin
	fadd	st,st
inizio:

	call	FpuExp
	fld1
	fdiv	st,st(1)
	fsub
	fild	due
	fdiv
	fxch	st(1)                      ;st=z2r st(1)=ff1
	fld     st(1)
	fld	st(1)                      ;cc=ff1*sin(z2r)
	fsin
	fmul                               ;st=cc st(1)=z2r st(2)=ff1
	fxch    st(2)                      ;st=ff1 st(1)=z2r st(2)=cc
	fchs
	fxch    st(1)                      ;st=z2r st(1)=-ff1 st(2)=cc
	fcos
	fmul                               ;st=dd st(1)=cc
	fld	st(1)
	fmul	st,st                      ;st=cc*cc st(1)=dd st(2)=cc
	fld	st(1)
	fmul	st,st                      ;st=dd*dd st(1)=cc*cc st(2)=dd st(3)=cc
	fadd
	fsqrt
	fsqrt                              ;st=ff2 st(1)=dd st(2)=cc
	fxch	st(2)                      ;st=cc st(1)=dd st(2)=ff2
	fdiv
	fld1
	fpatan                             ;st=atan(dd/cc) st(1)=ff2
	fild	due
	fdiv                               ;st=ff3 st(1)=ff2
	fld	st(1)
	fld	st(1)                      ;st=ff3 st(1)=ff2 st(2)=ff3 st(3)=ff2
	fcos
	fmul
	fadd    cr                           ;st=zr  st(1)=ff3 st(2)=ff2
	fxch	st(2)
	fxch	st(1)                      ;st=ff3 st(1)=ff2 st(2)=zr
	fsin
	fmul
	fadd	cj                           ;st=zj st(1)=zr

	fld	st(1)
	fabs
	ficomp  dieci
	fstsw	ax
	sahf
	ja      short fine

	fld	st
	fabs
	ficomp  dieci
	fstsw	ax
	sahf
	ja      short fine

	fld	st(1)
	fmul	st,st
	fld	st(1)
	fmul	st,st
	fadd
	fabs
	ficomp	cento
	fstsw	ax
	sahf
	ja      short fine

	fadd	st,st
	fxch    st(1)
	fadd	st,st
	fxch	st(1)
	or	ecx,ecx
	jz	fine
	dec	ecx
	jmp	inizio

;fin1:
;	inc	dl
;	jmp	short fine
;
;fin2:
;	inc	dh

fine:
;	or	dh,dh
;	jz      colore2
;	fabs
;	fld	dcs
;	fmul
;	fistp   colt
;	fistp	newcw
;	jmp	short cont
;colore2:
	fxch    st(1)
	fabs
	fld	epsilon
	fmul
	fistp   colt
	fstp    st
;cont:
	mov     ah,byte ptr colt
	mov	gs:[bx],ah
	add	bl,dl
	adc     bh,0
	fld	xmin
	fld	stx
	fadd
	fstp	xmin
	cmp	bx,di
	jg	endx
	jmp	start
endx:
	fld	ymin                    ;ymin+=sty
	fld	sty
	fadd
	fstp	ymin                    ;
	ret

FpuSprtSin2z endp








public FpuBiomZzz
FpuBiomZzz PROC


;	mov	eax,oldxmin
;	mov 	xmin,eax
	fld	oldxmin
	fstp	xmin
	mov	dx,risoluzione_x
	mov	bx,word ptr bex
	add	dx,bx
	mov	ch,step
start03:
;	xor	dx,dx
;	mov	flagzr,dh                  ;dl=flagzr
;	mov	flagzj,dh                  ;dh=flagzj
	mov	cl,byte ptr iteraz
	fld	xmin
	fld     ymin
inizio03:
	fld	st(1)			;zxr
	fmul	st,st
	fld	st(1)                   ;zxj
	fmul	st,st
	fild	tre                     ;zr
	fmul	st,st(1)
	fsubr	st,st(2)
	fmul	st,st(4)
	fadd	cr
	fstp	st(4)
	fild	tre
	fmul	st,st(1)		;zj
	fsub
	fmul
	fadd	cr


	fld	st(1)
	fabs
	ficomp  dieci
	fstsw	ax
	sahf
	ja      short fine03

	fld	st
	fabs
	ficomp  dieci
	fstsw	ax
	sahf
	ja      short fine03

	fld	st(1)
	fmul	st,st
	fld	st(1)
	fmul	st,st
	fadd
	fabs
	ficomp	cento
	fstsw	ax
	sahf
	ja      short fine03

	loop	inizio03


fine03:
	fxch    st(1)
	fabs
	fmul    epsilon
	fistp   colt
	fstp	st

	mov     ah,byte ptr colt
	mov	gs:[bx],ah
	add	bl,ch
	adc     bh,0
	fld	xmin
	fld	stx
	fadd
	fstp	xmin
	cmp	bx,dx
	jg	endx03
	jmp	start03
endx03:
	fld	ymin                    ;ymin+=sty
	fld	sty
	fadd
	fstp	ymin                    ;
	ret



FpuBiomZzz endp

;-------------------------------------------------------
;stx=((xmax-xmin)/risoluzione_x)*step
;sty=((ymax-ymin)/risoluzione_y)*step
;-------------------------------------------------------



public FpuStep
FpuStep PROC

	xor	ax,ax
	mov	al,step
	mov	newcw,ax
	fld	xmax
	fld	xmin
	fsub
	fabs
	fidiv	risoluzione_x
	fimul	newcw
	fstp	stx

	fld	ymax
	fld	ymin
	fsub
	fabs
	fidiv	risoluzione_y
	fimul	newcw
	fstp	sty

	ret

FpuStep endp

public FpuDif
FpuDif PROC

	fld	x_max
	fld	x_min
	fsub
	fabs
	fild	tre
	fdiv
	fstp	dif_x

	fld	y_max
	fld	y_min
	fsub
	fabs
	fild	tre
	fdiv
	fstp	dif_y

	ret

FpuDif endp

END