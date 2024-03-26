
//// INFO

// https://taricorp.gitlab.io/83pa28d/lesson/week1/day03/index.html
// registers: af, bc, de, hl, ix

// the return value is to be stored in register `a` (tested with fnc that returns a char; not tested with fnc that takes any arguments)
//
// the first argument is stored in `a` (only tested with fnc that takes 1 char argument and does not return)
//
// argumenta na funkciq koqto iziskva edin string pointer i nqma return idva v `hl`

// static variables don't seem to work

//// TODO

// make put_str take string length as argument
// also make get_str return the length

//// CODE

#include "ti83plus.h"

// config compiler
#pragma disable_warning 85 // disable parameter not used warning
// #pragma disable_warning 59 // disable no return warning
#pragma disable_warning 283 // disable: function declarator with no prototype

// use bcall in asm
#define abcall(__LABEL__) \
    rst 40 \
    .dw __LABEL__

// display params
#define SYMBOL_HEIGHT_PIXELS 5
// different symbols have different width; we can change this if we make a wrapper around the put_char fnc that checks for the cursor x
#define DISPLAY_HEIGHT_PIXELS 63
#define DISPLAY_WIDTH_PIXELS 95

// use some of the worthless ASCII codes
#define CHAR_CLEAR 1

// prototypes

void put_char(char ch);
void put_str(char *str);

// magic - don't remove this or the code stops working; I don't know why

void printc(char ch) __naked{
	ch;
	__asm
		pop hl      ; Get input
		pop bc
		push bc
		push hl 
		ld a, c
		push ix
		abcall(_VPutMap)
		pop ix
		ret
	__endasm;
}

// cursor get

char get_cur_y() __naked{
	__asm
		ld a, (#penRow)
		ret
	__endasm;
}

char get_cur_x() __naked{
	__asm
		ld a, (#penCol)
		ret
	__endasm;
}

// cursor set

void set_cur_y(char y) __naked{
	__asm
		ld (#penRow), a
		ret
	__endasm;
}

void set_cur_x(char x) __naked{
	__asm
		ld (#penCol), a
		ret
	__endasm;
}

// cursor reset

void reset_cur(){
	set_cur_y(0);
	set_cur_x(0);
}

// void reset_cur() __naked{
// 	__asm
// 		xor a, a
// 		ld (#penRow), a
// 		ld (#penCol), a
// 		ret
// 	__endasm;
// }

// cursor - move on next line

void move_cur_to_next_line(){
	char y = get_cur_y();

	y += SYMBOL_HEIGHT_PIXELS + 1; // +1 so that letters are not glued togethere

	if(y + SYMBOL_HEIGHT_PIXELS > DISPLAY_HEIGHT_PIXELS){ // `+ SYMBOL_HEIGHT_PIXELS` so that we know there is enough screen space for the next symbol
		y = 0;
	}

	set_cur_y(y);

	set_cur_x(0);
}

// screen clear

void clear_screen() __naked{
	__asm

		push af
		push bc
		push de
		push hl
		push ix

		abcall(_ClrScrnFull) // trash(all); https://taricorp.gitlab.io/83pa28d/lesson/week2/day11/index.html

		pop ix
		pop hl
		pop de
		pop bc
		pop af

		ret
	__endasm;
}

// screen reset

void reset_screen(){
	clear_screen();
	reset_cur();
}

// output - new line

void clear_line(){
	set_cur_x(0);

	// for(int i=0; i<93; ++i){
	// 	put_char(' ');
	// }
	put_str("                                                                                             "); // tova ne e prekaleno burzo

	set_cur_x(0);
}

void new_line(){
	// TODO tova koeto izpolzvame za indikaciq na reda e super bavno
	// trqbva da izmislq ne6to po-umno

	move_cur_to_next_line();
	clear_line(); // tova ne e prekaleno burzo

	int original_line_y = get_cur_y();

	move_cur_to_next_line();
	// for(int i=0; i<20; ++i){
	// 	put_char('^');
	// }
	put_str("^^^^^^^^^^^^^^^^^^^^^^^"); // tova ne e prekaleno burzo

	set_cur_y(original_line_y);
	set_cur_x(0);
}

// output - hardcoded letters

// void put_big_A() __naked{
// 	__asm
// 		ld a, #'A'
// 		abcall(_PutC)
// 		ret
// 	__endasm;
// }

// void put_small_A() __naked{
// 	__asm
// 		ld a, #'A'
// 		abcall(_VPutMap)
// 		ret
// 	__endasm;
// }

// output - any letter

void put_char(char ch) __naked{
	// this doesn't automatically go on the next line upon reaching end of current line
	// in fact, if we reach the end of the screen we have to spam a lot of new lines in order to come back from the top

	__asm

		// kato pogledna .asm faila i izglejda vikagi `ch` e `a`
		// qvno putviq argument na dadena funkciq vikagi e `a`

		push af
		push de
		push hl
		push ix

		// https://z88dk.org/wiki/doku.php?id=usage:stackframe
		// // bate ne znam za6to tova raboti
		// ld hl, #2 // tova raboti i s 0 i s 4 nz 6to
		// add hl, sp
		// // ld e, (hl) // tova raboti
		// ld a, (hl) // tova ne raboti
		abcall(_VPutMap) // in(A) trash(all but BC and HL) // https://taricorp.gitlab.io/83pa28d/lesson/week2/day11/index.html

		pop ix
		pop hl
		pop de
		pop af

		ret
	__endasm;
}

// outpu - any string

// we can use `_VPutS` instead - https://taricorp.gitlab.io/83pa28d/lesson/week2/day11/index.html
// if we want some safety we can implement maxlen
// void put_str(char *str){
// 	for(;;){
// 		char c = *str++;
// 		if(c==0){
// 			break;
// 		}
// 		put_char(c);
// 	}
// }
void put_str(char *str) __naked{
	// kato vidq .asm i vijdam 4e argumenta se slaga v `hl`

	__asm

		push hl

		abcall(_VPutS) // in(HL); out(HL); https://taricorp.gitlab.io/83pa28d/lesson/week2/day11/index.html

		pop hl;

		ret
	__endasm;
}

// output - debug

// we can use `_DispHL` instead - https://taricorp.gitlab.io/83pa28d/lesson/week2/day11/index.html
void put_char_as_num(char ch){
	// doing recursion on cuch a shitty hardware is risky... I'll use something else if I ever crash

	if(ch < 0){
		put_char('-');
		return put_char_as_num(ch*-1);
	}

	char last_digit = ch % 10;
	ch /= 10;

	if(ch > 0){
		put_char_as_num(ch);
	}

	char to_put = '0' + last_digit;
	put_char(to_put);
}

// input - scan code

// returns keycode of pressed key (0 is for the ON button)
// for a nonblocking version we could use `_GetCSC` - https://taricorp.gitlab.io/83pa28d/lesson/week2/day12/index.html
char get_sk_blk() __naked{
	__asm

		push bc
		push de
		push hl

		abcall(_getkey) // in(a); trash(bc, de, hl); https://taricorp.gitlab.io/83pa28d/lesson/week2/day12/index.html

		pop hl
		pop de
		pop bc

		ret
	__endasm;
}

// input - letters

char get_char_blk(){
	char ch = get_sk_blk();

	// A to Z
	if((ch >= 154) && (ch <= 179)){
		return (ch - 154) + 'A';
	}

	// 0 to 9
	if((ch >= 142) && (ch <= 151)){
		return (ch - 142) + '0';
	}

	// other keys
	switch(ch){
		case 5: // enter
			return '\n';
		case 9:
			return CHAR_CLEAR;
		// case 10: // del
		// 	return CHAR_BACKSPACE;
		case 128:
			return '+';
		case 129:
			return '-';
		case 130:
			return '*';
		case 131:
			return '/';
	}

	put_char('<');
	put_char_as_num(ch);
	put_char('>');
	
	return '?';
}

// input - string

void get_str(char *arg_place_to_store, int arg_size_place_to_store){
	char *storage = arg_place_to_store;
	int bytes_left = arg_size_place_to_store;

	if(bytes_left <= 0){
		return;
	}

	while(bytes_left > 1){

		char ch = get_char_blk();

		if(ch == '\n'){
			break;
		}

		if(ch == CHAR_CLEAR){
			clear_line();
			storage = arg_place_to_store;
			bytes_left = arg_size_place_to_store;
			continue;
		}

		*storage++ = ch;
		bytes_left -= 1;

		put_char(ch);
	}

	*storage = 0;

	new_line();
}

// return test

// char ret_test() {
// 	return 'A';
// }

// it seems that the return value goes to `a`
char ret_test() __naked{
	__asm
		ld a, #'A'
		ret
	__endasm;
}

// main

void main() {
	reset_screen();

	put_char('A');
	new_line();
	put_char('B');
	put_char('C');
	put_char('D');
	put_char('E');
	new_line();
	put_str("q6 mi huq");
	new_line();

	for(int i=0; i<35; ++i){
		put_char('a');
	}
	// set_cur_x(0);
	// for(int i=0; i<93; ++i){
	// 	put_char(' ');
	// }
	new_line();

	put_str("return test");
	new_line();
	char ret_value = ret_test();
	put_str("asd value: ");
	put_char(ret_value);
	new_line();

	// // put_str("enter key: ");
	// // char ch = get_char_blk();
	// // put_char(ch);
	// for(int count=0;; count += 1){
	// 	new_line();
	// 	char ch = get_char_blk();
	// 	if(ch == 'A'){
	// 		break;
	// 	}
	// 	put_char(ch);

	// 	put_char(' ');
	// 	int cur_y = get_cur_y();
	// 	put_char_as_num(cur_y);
	// }

	char inp[20] = "";
	get_str(inp, 20);

	put_str("input:");
	put_str(inp);
	new_line();

	put_str("BYE");

}
