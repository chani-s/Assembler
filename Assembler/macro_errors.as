;invalid characters after endmacro.
macro  m1
	inc K
	mov S1.2, r3
endmacro error


;missing macro name.
macro  
	inc K
	mov S1.2, r3
endmacro hello


;invalid characters after declaration of macro.
macro  m2 error
	inc K
	mov S1.2, r3
endmacro


;invalid macro name.
macro  1m
	inc K
	mov S1.2, r3
endmacro

macro  mov
	inc K
	mov S1.2, r3
endmacro

macro  r1
	inc K
	mov S1.2, r3
endmacro


;using twice macro name.
macro  m1
	inc K
	mov S1.2, r3
endmacro
macro  m1
	inc K
	mov S1.2, r3
endmacro
