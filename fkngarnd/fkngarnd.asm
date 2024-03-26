;--------------------------------------------------------
; File Created by SDCC : free open source ISO C Compiler 
; Version 4.3.0 #14184 (Linux)
;--------------------------------------------------------
	.module main
	.optsdcc -mz80
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _main
	.globl _ret_test
	.globl _get_char_blk
	.globl _get_sk_blk
	.globl _put_char_as_num
	.globl _put_small_str
	.globl _put_small_char
	.globl _reset_screen
	.globl _reset_cursor
	.globl _getKey
	.globl _doubleHexdump
	.globl _hexdump
	.globl _printc
	.globl _newline
	.globl _println
	.globl _fputs
	.globl _resetPen
	.globl _setPenCol
	.globl _setPenRow
	.globl _hexTab
;--------------------------------------------------------
; special function registers
;--------------------------------------------------------
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area _DATA
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area _INITIALIZED
;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area _DABS (ABS)
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area _HOME
	.area _GSINIT
	.area _GSFINAL
	.area _GSINIT
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area _HOME
	.area _HOME
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area _CODE
;../TiConstructor/lib/textio.c:20: void setPenRow(char row) __naked{
;	---------------------------------
; Function setPenRow
; ---------------------------------
_setPenRow::
;../TiConstructor/lib/textio.c:32: __endasm;
	pop	hl ; Get input
	pop	bc ; and perserve
	push	bc
	push	hl ; ret value
	ld	a, c
	ld	(#0x86D8), a
	ret
;../TiConstructor/lib/textio.c:33: }
;../TiConstructor/lib/textio.c:38: void setPenCol(char col) __naked{
;	---------------------------------
; Function setPenCol
; ---------------------------------
_setPenCol::
;../TiConstructor/lib/textio.c:50: __endasm;
	pop	hl ; Get input
	pop	bc ; and perserve
	push	bc
	push	hl ; ret value
	ld	a, c
	ld	(#0x86D7), a
	ret
;../TiConstructor/lib/textio.c:51: }
;../TiConstructor/lib/textio.c:54: void resetPen() __naked{
;	---------------------------------
; Function resetPen
; ---------------------------------
_resetPen::
;../TiConstructor/lib/textio.c:61: __endasm;
	xor	a, a
	ld	(#0x86D7), a
	ld	(#0x86D8), a
	ret
;../TiConstructor/lib/textio.c:62: }
;../TiConstructor/lib/textio.c:67: void fputs(char* loc) __naked {
;	---------------------------------
; Function fputs
; ---------------------------------
_fputs::
;../TiConstructor/lib/textio.c:86: __endasm;
	pop	hl ; Get input
	pop	bc ; and perserve
	push	bc ; ret value
	push	hl
the_char_loop_i_need_more_good_names_for_labels:
	ld	a, (bc)
	or	a, a
	ret	z
	push	ix
	rst	40 
	.dw 0x455E
	pop	ix
	inc	bc
	jr	the_char_loop_i_need_more_good_names_for_labels
;../TiConstructor/lib/textio.c:87: }
;../TiConstructor/lib/textio.c:92: void println(char* loc){
;	---------------------------------
; Function println
; ---------------------------------
_println::
;../TiConstructor/lib/textio.c:93: fputs(loc);
	call	_fputs
;../TiConstructor/lib/textio.c:102: __endasm;
	ld	a, (#0x86D8)
	ld	b, #6
	add	b
	ld	(#0x86D8), a
	xor	a, a
	ld	(#0x86D7), a
;../TiConstructor/lib/textio.c:103: }
	ret
;../TiConstructor/lib/textio.c:106: void newline() __naked{
;	---------------------------------
; Function newline
; ---------------------------------
_newline::
;../TiConstructor/lib/textio.c:116: __endasm;
	ld	a, (#0x86D8)
	ld	b, #6
	add	b
	ld	(#0x86D8), a
	xor	a, a
	ld	(#0x86D7), a
	ret
;../TiConstructor/lib/textio.c:117: }
;../TiConstructor/lib/textio.c:121: void printc(char ch) __naked{
;	---------------------------------
; Function printc
; ---------------------------------
_printc::
;../TiConstructor/lib/textio.c:133: __endasm;
	pop	hl ; Get input
	pop	bc
	push	bc
	push	hl
	ld	a, c
	push	ix
	rst	40 
	.dw 0x455E
	pop	ix
	ret
;../TiConstructor/lib/textio.c:134: }
;../TiConstructor/lib/textio.c:184: void hexdump(char v)__naked{
;	---------------------------------
; Function hexdump
; ---------------------------------
_hexdump::
;../TiConstructor/lib/textio.c:220: __endasm;
	pop	hl ; Get input
	pop	bc ; and perserve
	push	bc
	push	hl
	push	ix
	ld	a, c
	ld	hl, #_hexTab
	and	a, #0xF0
	SRL	a
	SRL	a
	SRL	a
	SRL	a
	ld	e, a
	ld	d, #0
	add	hl, de
	ld	a, (hl)
	rst	40 
	.dw 0x455E
	ld	a, c
	ld	hl, #_hexTab
	and	a, #0x0F
	ld	e, a
	ld	d, #0
	add	hl, de
	ld	a, (hl)
	rst	40 
	.dw 0x455E
	pop	ix
	ret
;../TiConstructor/lib/textio.c:221: }
;../TiConstructor/lib/textio.c:227: void doubleHexdump(int v) __naked{
;	---------------------------------
; Function doubleHexdump
; ---------------------------------
_doubleHexdump::
;../TiConstructor/lib/textio.c:242: __endasm;
	pop	hl
	pop	bc
	push	bc
	push	hl
	push	bc
	push	bc
	inc	sp
	call	_hexdump
	inc	sp
	call	_hexdump
	pop	bc
	ret
;../TiConstructor/lib/textio.c:243: }
;../TiConstructor/lib/userinput.c:28: char getKey(){
;	---------------------------------
; Function getKey
; ---------------------------------
_getKey::
;../TiConstructor/lib/userinput.c:29: bcall(0x4015);
	rst	40 
	.dw 0x4015 
;../TiConstructor/lib/userinput.c:30: return *((char*)(0x843F));
	ld	a, (#0x843f)
;../TiConstructor/lib/userinput.c:31: }
	ret
_hexTab:
	.db #0x30	; 48	'0'
	.db #0x31	; 49	'1'
	.db #0x32	; 50	'2'
	.db #0x33	; 51	'3'
	.db #0x34	; 52	'4'
	.db #0x35	; 53	'5'
	.db #0x36	; 54	'6'
	.db #0x37	; 55	'7'
	.db #0x38	; 56	'8'
	.db #0x39	; 57	'9'
	.db #0x41	; 65	'A'
	.db #0x42	; 66	'B'
	.db #0x43	; 67	'C'
	.db #0x44	; 68	'D'
	.db #0x45	; 69	'E'
	.db #0x46	; 70	'F'
;main.c:19: void reset_cursor(){
;	---------------------------------
; Function reset_cursor
; ---------------------------------
_reset_cursor::
;main.c:20: bcall(_HomeUp);
	rst	40 
	.dw 0x4558 
;main.c:21: }
	ret
;main.c:23: void reset_screen(){
;	---------------------------------
; Function reset_screen
; ---------------------------------
_reset_screen::
;main.c:24: clearScreen();
	rst	40 
	.dw 0x4540 
;main.c:25: resetPen();
	call	_resetPen
;main.c:26: reset_cursor();
;main.c:27: }
	jp	_reset_cursor
;main.c:64: void put_small_char(char ch) __naked{
;	---------------------------------
; Function put_small_char
; ---------------------------------
_put_small_char::
;main.c:85: __endasm;
	push	hl
	push	de
	ld	hl, #0
	add	hl, sp
	ld	e, (hl)
	rst	40 
	.dw 0x455E
	pop	de
	pop	hl
	ret
;main.c:86: }
;main.c:90: void put_small_str(char *str){
;	---------------------------------
; Function put_small_str
; ---------------------------------
_put_small_str::
00104$:
;main.c:94: char c = *str++;
	ld	a, (hl)
	inc	hl
	ld	c, a
;main.c:95: if(c==0){
	or	a, a
	ret	Z
;main.c:98: put_small_char(c);
	push	hl
	ld	a, c
	call	_put_small_char
	pop	hl
;main.c:100: }
	jr	00104$
;main.c:105: void put_char_as_num(char ch){
;	---------------------------------
; Function put_char_as_num
; ---------------------------------
_put_char_as_num::
	ld	l, a
;	spillPairReg hl
;	spillPairReg hl
;main.c:113: char last_digit = ch % 10;
	ld	h, #0x00
;	spillPairReg hl
;	spillPairReg hl
	push	hl
	ld	de, #0x000a
	call	__modsint
	ld	c, e
	pop	hl
;main.c:114: ch /= 10;
	push	bc
	ld	de, #0x000a
	call	__divsint
	pop	bc
;main.c:116: if(ch > 0){
	ld	a, e
	or	a, a
	jr	Z, 00104$
;main.c:117: put_char_as_num(ch);
	push	bc
	ld	a, e
	call	_put_char_as_num
	pop	bc
00104$:
;main.c:120: char to_put = '0' + last_digit;
	ld	a, c
	add	a, #0x30
;main.c:121: put_small_char(to_put);
	ld	c, a
;main.c:122: }
	jp	_put_small_char
;main.c:127: char get_sk_blk() __naked{
;	---------------------------------
; Function get_sk_blk
; ---------------------------------
_get_sk_blk::
;main.c:143: __endasm;
	push	bc
	push	de
	push	hl
	rst	40 
	.dw 0x4972
	pop	hl
	pop	de
	pop	bc
	ret
;main.c:144: }
;main.c:148: char get_char_blk(){
;	---------------------------------
; Function get_char_blk
; ---------------------------------
_get_char_blk::
;main.c:149: char ch = get_sk_blk();
	call	_get_sk_blk
	ld	c, a
;main.c:153: return (ch - 154) + 'A';
	ld	b, c
;main.c:152: if((ch >= 154) && (ch <= 179)){
	ld	a, c
	sub	a, #0x9a
	jr	C, 00102$
	ld	a, #0xb3
	sub	a, c
	jr	C, 00102$
;main.c:153: return (ch - 154) + 'A';
	ld	a, b
	add	a, #0xa7
	ret
00102$:
;main.c:157: if((ch >= 142) && (ch <= 151)){
	ld	a, c
	sub	a, #0x8e
	jr	C, 00105$
	ld	a, #0x97
	sub	a, c
	jr	C, 00105$
;main.c:158: return (ch - 142) + '0';
	ld	a, b
	add	a, #0xa2
	ret
00105$:
;main.c:161: put_small_char('<');
	push	bc
	ld	a, #0x3c
	call	_put_small_char
	pop	bc
;main.c:162: put_char_as_num(ch);
	ld	a, c
	call	_put_char_as_num
;main.c:163: put_small_char('>');
	ld	a, #0x3e
	call	_put_small_char
;main.c:165: return '?';
	ld	a, #0x3f
;main.c:166: }
	ret
;main.c:175: char ret_test() __naked{
;	---------------------------------
; Function ret_test
; ---------------------------------
_ret_test::
;main.c:179: __endasm;
	ld	a, #'A'
	ret
;main.c:180: }
;main.c:184: void main() {
;	---------------------------------
; Function main
; ---------------------------------
_main::
;main.c:185: reset_screen();
	call	_reset_screen
;main.c:187: put_small_char('A');
	ld	a, #0x41
	call	_put_small_char
;main.c:188: newline();
	call	_newline
;main.c:189: put_small_char('B');
	ld	a, #0x42
	call	_put_small_char
;main.c:190: put_small_char('C');
	ld	a, #0x43
	call	_put_small_char
;main.c:191: put_small_char('D');
	ld	a, #0x44
	call	_put_small_char
;main.c:192: put_small_char('E');
	ld	a, #0x45
	call	_put_small_char
;main.c:193: newline();
	call	_newline
;main.c:194: put_small_str("q6 mi huq");
	ld	hl, #___str_0
	call	_put_small_str
;main.c:195: newline();
	call	_newline
;main.c:197: for(int i=0; i<30; ++i){
	ld	bc, #0x0000
00106$:
	ld	a, c
	sub	a, #0x1e
	ld	a, b
	rla
	ccf
	rra
	sbc	a, #0x80
	jr	NC, 00101$
;main.c:198: put_small_char('a');
	push	bc
	ld	a, #0x61
	call	_put_small_char
	pop	bc
;main.c:197: for(int i=0; i<30; ++i){
	inc	bc
	jr	00106$
00101$:
;main.c:200: newline();
	call	_newline
;main.c:202: put_small_str("return test");
	ld	hl, #___str_1
	call	_put_small_str
;main.c:203: newline();
	call	_newline
;main.c:204: char ret_value = ret_test();
	call	_ret_test
	ld	c, a
;main.c:205: put_small_str("returned value: ");
	push	bc
	ld	hl, #___str_2
	call	_put_small_str
	pop	bc
;main.c:206: put_small_char(ret_value);
	ld	a, c
	call	_put_small_char
;main.c:207: newline();
	call	_newline
00108$:
;main.c:213: newline();
	call	_newline
;main.c:214: char ch = get_char_blk();
	call	_get_char_blk
;main.c:215: if(ch == 'A'){
	ld	c, a
	sub	a, #0x41
	ret	Z
;main.c:218: put_small_char(ch);
	ld	a, c
	call	_put_small_char
;main.c:240: }
	jr	00108$
___str_0:
	.ascii "q6 mi huq"
	.db 0x00
___str_1:
	.ascii "return test"
	.db 0x00
___str_2:
	.ascii "returned value: "
	.db 0x00
	.area _CODE
	.area _INITIALIZER
	.area _CABS (ABS)
