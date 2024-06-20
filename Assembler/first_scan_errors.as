;too long line
K1: .data 12, 34, 54,90,-23         ,       24,          346,   +504,  -7,  12,75, 76, 34           ,45

;too long symbol name
thisIsAveryVeryVeryVeryVeryLongLabel: .data 12, 4, 56, 78, 10
mov r1,thisIsAveryVeryVeryVeryVeryLongLabel

;unfitting operands
mov #1, #4
add #1,  #4
sub #1, #4
not #1
clr #1
lea #1, #4
lea r5 , #1
inc #1
dec #1
jmp #1
bne #1
get #1
jsr #1

; illegal operands
W: mov illegal_label, r1
add W, 3 
bne W_
sub W, !@
lea W, 3
bne B.3
.struct 3, "f

;extra text
not W, #3, 4 
clr W, #3, 4 
inc W, #3, 4 
dec W, #3, 4 
jmp W, #3, 4 
bne W, #3, 4 
get W, #3, 4 
jsr W, #3, 4 

; empty label
label:

;reserved word label
add: add #3	, W
r3: .data 1,2,3

;illegal numbers
mov #23a, r1
mov r1, #23a

;missing space after symbol declaration
K1:.data 12

;undefined first word
error

;missing numbers
.data 3,$
.struct

;illegal comma/s
.data ,3
mov r1 r2
.struct 3,,4
.data 6,, 8
mov r1 ,,r2

;illegal / missing string
.string "
.struct 5,,
.string 

;illegal label name
_!@+: hlt

;declaring an already defined label as external
 hello: hlt
 .extern hello

;defining label that already declared as external
.extern G
G: hlt

;missing label after ".entry" \ ".extern"
.extern
.entry


