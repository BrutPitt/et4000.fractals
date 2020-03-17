;
;       Modello da definire.............
;
.Model LARGE,C


.DATA

extrn rvcx:word
extrn rvcy:word
extrn rvbutton:word
extrn ctcx:word
extrn ctcy:word


.CODE
.286p

public  cpu

cpu     proc
	push    bp

	push    sp                      ; 86/186 will push SP-2;
	pop     ax                      ; 286/386 will push SP.
	cmp     ax, sp
	jz      not86                   ; If equal, SP was pushed
	mov     ax, 186
	mov     cl, 32                  ;   186 uses count mod 32 = 0;
	shl     ax, cl                  ;   86 shifts 32 so ax = 0
	jnz     exit                    ; Non-zero: no shift, so 186
	mov     ax, 86                  ; Zero: shifted out all bits
	jmp     short exit
not86:
	pushf                           ; Test 16 or 32 operand size:
	mov     ax, sp                  ;   Pushed 2 or 4 bytes of flags?
	popf
	inc     ax
	inc     ax
	cmp     ax, sp                  ;   Did pushf change SP by 2?
	jnz     is32bit                 ;   If not, then 4 bytes of flags
is16bit:
	sub     sp, 6                   ; Is it 286 or 386 in 16-bit mode?
	mov     bp, sp                  ; Allocate stack space for GDT pointer
	sgdt    fword ptr [bp]
	add     sp, 4                   ; Discard 2 words of GDT pointer
	pop     ax                      ; Get third word
	inc     ah                      ; 286 stores -1, 386 stores 0 or 1
	jnz     is386
is286:
	mov     ax, 286
	jmp     short testprot          ; Check for protected mode
is32bit:
	db      66h                     ; 16-bit override in 32-bit mode
is386:
	mov     ax, 386
testprot:
	smsw    cx                      ; Protected?  Machine status -> CX
	ror     cx,1                    ; Protection bit -> carry flag
	jnc     exit                    ; Real mode if no carry
	neg     ax                      ; Protected:  return neg value
exit:
	pop     bp
	ret
cpu     endp


public get_status_mouse

get_status_mouse PROC

	xor     ax,ax
	int     33h
	ret

get_status_mouse endp


public show_mouse

show_mouse PROC

	mov     ax,1
	int     33h
	ret

show_mouse endp


public hide_mouse

hide_mouse PROC

	mov     ax,2
	int     33h
	ret

hide_mouse endp


public set_mouse_position

set_mouse_position PROC xrate:word,yrate:word

	mov     cx,xrate
	mov     dx,yrate
	mov     ax,4
	int     33h
	ret

set_mouse_position endp


public set_mouse_xlimit

set_mouse_xlimit PROC xrate:word,yrate:word

	mov     cx,xrate
	mov     dx,yrate
	mov     ax,7
	int     33h
	ret

set_mouse_xlimit endp



public set_mouse_ylimit

set_mouse_ylimit PROC xrate:word,yrate:word

	mov     cx,xrate
	mov     dx,yrate
	mov     ax,8
	int     33h
	ret

set_mouse_ylimit endp



public ret_mouse_position

ret_mouse_position PROC

	mov     ax,03h
	int     33h
	mov     rvcx,cx
	mov     rvcy,dx
	mov     rvbutton,bx
	ret

ret_mouse_position endp



public ret_mouse_counter

ret_mouse_counter PROC

	mov     ax,0bh
	int     33h
	mov     ctcx,cx
	mov     ctcy,dx
	ret

ret_mouse_counter endp




public set_shape_mouse

set_shape_mouse PROC USES es,xrate:word,yrate:word,pointmouse:dword

	mov     bx,xrate
	mov     cx,yrate
	mov     dx,word ptr pointmouse
	mov     ax,word ptr pointmouse+2
	mov     es,ax
	mov     ax,09h
	int     33h
	ret

set_shape_mouse endp


public set_mouse_ratio_mik

set_mouse_ratio_mik PROC

	mov     dx,8
	mov     cx,32
	mov     ax,0fh
	int     33h
	ret

set_mouse_ratio_mik endp

PUBLIC  movsb_mem

movsb_mem     PROC USES ds si di,dest:dword,sourc:dword,count:word

	mov     ax,word ptr dest+2
	mov     es,ax
	mov     di,word ptr dest
	mov     cx,count
	mov     si,word ptr sourc
	mov     ds,word ptr sourc+2
	cld
	sar     cx,1
	jnc     f10
	movsb
f10:
	jcxz    fine0
	rep     movsw
fine0:
	ret

movsb_mem endp

PUBLIC  movsb_mem_val

movsb_mem_val     PROC USES di,dest:dword,vals:byte,count:word

	mov     ax,word ptr dest+2
	mov     es,ax
	mov     di,word ptr dest
	mov     cx,count
	mov     al,byte ptr vals
	mov     ah,al
	cld
	sar     cx,1
	jnc     f13
	stosb
f13:
	jcxz    fine3
	rep     stosw
fine3:
	ret

movsb_mem_val     endp




END

