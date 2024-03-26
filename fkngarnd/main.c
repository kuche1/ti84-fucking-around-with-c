
//// INFO

// https://taricorp.gitlab.io/83pa28d/lesson/week1/day03/index.html
// registers: af, bc, de, hl, ix

// ascii representation
// https://taricorp.gitlab.io/83pa28d/appendix/sfont/index.html

// the return value is to be stored in register `a` (tested with fnc that returns a char; not tested with fnc that takes any arguments)
//
// the first argument is stored in `a` (only tested with fnc that takes 1 char argument and does not return)
//
// argumenta na funkciq koqto iziskva edin string pointer i nqma return idva v `hl`

// static variables don't seem to work

// use chars whenever possible, as opposed to ints; at some point I got absolute dogshit performance in a for loop, and I fixed the problem by simply using chars instead of ints

// list of (all?) bcalls can be found here: https://wikiti.brandonw.net/index.php?title=Category:83Plus:BCALLs:By_Name
// if u need to check for a specific hex code just change the last 4 digits of the following link: https://wikiti.brandonw.net/index.php?title=83Plus:RAM:8478

// if your cursor has reached the end of the screen and you try to put a character, the cursor's position will actually stay the same instead of increasing, overflowing or saturating
// tested with the X position of the cursor

//// CONVENTIONS

// if we are to signify any "warning" we do it by printing `<TEXT>`, where `TEXT` is the actual text of the warning

// if we are to signify a runtime error we do it by printing `<EN>`, where `N` is replaced by a unique number (so that there isn't one `N` used for multiple errors found on multiple lines in the source code)

//// TODO

// empty

//// CODE

#include "ti83plus.h"

// config compiler
#pragma disable_warning 85 // disable parameter not used warning
// #pragma disable_warning 59 // disable no return warning
#pragma disable_warning 283 // disable: function declarator with no prototype

// display params
#define SYMBOL_HEIGHT_PIXELS 5
// different symbols have different width; we can change this if we make a wrapper around the put_char fnc that checks for the cursor x
#define FATTEST_SYMBOL_WIDTH_PIXELS 4 // I actually don't know if this is the case; currently `Y` with 4 pixels
#define DISPLAY_HEIGHT_PIXELS 64 // indexed 0-63
#define DISPLAY_WIDTH_PIXELS 96 // indexed 0-95

// use some of the worthless ASCII codes
#define CHAR_CLEAR 1
#define CHAR_BACKSPACE 2

// macro functions

#define LENOF(var) \
	sizeof(var) / sizeof(*var)

#define BCALL(__LABEL__) \
    rst 40 \
    .dw __LABEL__

#define GET_STR(storage) \
	get_str(storage, sizeof(storage))

#define PUT_COMPTIME_STR(str) \
	{ \
		put_str(str, LENOF(str)-1); /* `-1` as to discard the last 0 */ \
	}

// prototypes

void put_char(char ch);
void put_char_as_num(char ch);
void put_str(char *str, int len);

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
		BCALL(_VPutMap)
		pop ix
		ret
	__endasm;
}

// time

// // https://wikiti.brandonw.net/index.php?title=83Plus:RAM:8478
// char get_time_sec() __naked{
// 	__asm

// 		push af
// 		push bc
// 		push de
// 		push hl
// 		push ix

// 		BCALL(_getTime)

// 		BCALL(_PopRealO1)

// 		// ld a, (#OP1)
// 		// ld a, (#FPS)

// 		ld hl, #OP1
// 		inc hl
// 		// inc hl
// 		// inc hl
// 		// inc hl
// 		// dec hl
// 		ld a, (hl)

// 		pop ix
// 		pop hl
// 		pop de
// 		pop bc
// 		pop af

// 		ret
// 	__endasm;
// }

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

		BCALL(_ClrScrnFull) // trash(all); https://taricorp.gitlab.io/83pa28d/lesson/week2/day11/index.html

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

	for(char i=0; i+=4; i<DISPLAY_WIDTH_PIXELS){
		put_char(0x06); // fat 4 pixel wide space
	}

	set_cur_x(0);
}

void new_line(){
	set_cur_x(0);
	// PUT_COMPTIME_STR("   "); // enogh to wipe the `>` - https://taricorp.gitlab.io/83pa28d/appendix/sfont/index.html
	put_char(0x06); // see https://taricorp.gitlab.io/83pa28d/appendix/sfont/index.html

	move_cur_to_next_line();
	clear_line(); // tova ne e prekaleno burzo

	put_char('>');
}

// output - hardcoded letters

// void put_big_A() __naked{
// 	__asm
// 		ld a, #'A'
// 		BCALL(_PutC)
// 		ret
// 	__endasm;
// }

// void put_small_A() __naked{
// 	__asm
// 		ld a, #'A'
// 		BCALL(_VPutMap)
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
		BCALL(_VPutMap) // in(A) trash(all but BC and HL) // https://taricorp.gitlab.io/83pa28d/lesson/week2/day11/index.html

		pop ix
		pop hl
		pop de
		pop af

		ret
	__endasm;
}

// output - any string

// if we want some safety we can implement maxlen
void put_str(char *str, int len){
	if(len < 0){
		PUT_COMPTIME_STR("<E1>");
	}

	char *end = str + len;

	while(str != end){
		put_char(*str++);
	}
}

// // tova mi se struva 4e ne e po-burzo ot C for versiqta
// void put_str(char *str) __naked{
// 	// kato vidq .asm i vijdam 4e argumenta se slaga v `hl`

// 	__asm

// 		push hl

// 		BCALL(_VPutS) // in(HL); out(HL); https://taricorp.gitlab.io/83pa28d/lesson/week2/day11/index.html

// 		pop hl;

// 		ret
// 	__endasm;
// }

// output - debug

// we can use `_DispHL` instead - https://taricorp.gitlab.io/83pa28d/lesson/week2/day11/index.html
void put_char_as_num(char ch){
	// doing recursion on cuch a shitty hardware is risky... I'll use something else if I ever crash

	// this will never happen since char in an unsigned type
	// if(ch < 0){
	// 	put_char('-');
	// 	return put_char_as_num(ch*-1);
	// }

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

		BCALL(_getkey) // in(a); trash(bc, de, hl); https://taricorp.gitlab.io/83pa28d/lesson/week2/day12/index.html

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
		case 10: // del
			return CHAR_BACKSPACE;
		case 128:
			return '+';
		case 129:
			return '-';
		case 130:
			return '*';
		case 131:
			return '/';
		case 133:
			return '(';
		case 134:
			return ')';
		case 153:
			return ' ';
	}

	PUT_COMPTIME_STR("<chr");
	put_char_as_num(ch);
	put_char('>');
	
	return '?';
}

// input - string

int get_str(char *arg_place_to_store, int arg_size_place_to_store){
	char *storage = arg_place_to_store;
	int bytes_left = arg_size_place_to_store;
	int bytes_written = 0;

	char starting_x = get_cur_x();

	char byte_start_xs[DISPLAY_WIDTH_PIXELS];

	if(bytes_left <= 0){
		PUT_COMPTIME_STR("<E0>");
		return 0;
	}

	for(;;){
		if(bytes_left <= 1){
			PUT_COMPTIME_STR("<mem>");
			break;
		}

		char ch = get_char_blk();

		if(ch == '\n'){
			break;
		}else if(ch == CHAR_CLEAR){
			char current_x = get_cur_x();
			char pixels_to_clean = current_x - starting_x;
			set_cur_x(starting_x);
			for(char i=0; i<pixels_to_clean; ++i){ // TODO we can optimise this by using the space that is 4 pixels long
				put_char(' ');
			}
			set_cur_x(starting_x);

			storage = arg_place_to_store;
			bytes_left = arg_size_place_to_store;
			bytes_written = 0;
			continue;
		}else if(ch == CHAR_BACKSPACE){
			if(bytes_written <= 0){
				continue;
			}
			bytes_written -= 1;
			bytes_left += 1;
			--storage;

			char start_x = byte_start_xs[bytes_written];
			set_cur_x(start_x);
			put_char(0x06); // this fat space is 4 pixels wide; it will actually not clear all possible characters (there are some niche 5px chars), but we'll close our eyes for now
			set_cur_x(start_x);
			continue;
		}

		byte_start_xs[bytes_written] = get_cur_x();

		put_char(ch);

		char cur_x = get_cur_x();
		// char cur_y = get_cur_y();

		// set_cur_y(0);
		// clear_line();
		// put_char_as_num(cur_x);
		// set_cur_y(cur_y);
		// set_cur_x(cur_x);

		if(cur_x >= DISPLAY_WIDTH_PIXELS - FATTEST_SYMBOL_WIDTH_PIXELS - 1){ // we actually need to leave some headroom here because the cursor will not saturate or overflow when the end has been reched - instead it will stay the same
			// no more space on screen
			break;
		}

		*storage++ = ch;
		bytes_left -= 1;
		bytes_written += 1;
	}

	*storage = 0;

	new_line();

	return bytes_written;
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

	// for(int i=0; i<5; ++i){
	// 	char time = get_time_sec();
	// 	put_char_as_num(time);
	// 	new_line();
	// }

	{
		char x0 = get_cur_x();
		put_char('S');
		char x1 = get_cur_x();

		put_char(' ');
		put_char_as_num(x0);
		put_char(' ');
		put_char_as_num(x1);

		new_line();
	}

	put_char('A');
	new_line();
	put_char('B');
	put_char('C');
	put_char('D');
	put_char('E');
	new_line();
	PUT_COMPTIME_STR("q6 mi huq");
	new_line();

	// for(int i=0; i<35; ++i){
	// 	put_char('a');
	// }
	// new_line();

	PUT_COMPTIME_STR("return test");
	new_line();
	char ret_value = ret_test();
	PUT_COMPTIME_STR("asd value: ");
	put_char(ret_value);
	new_line();

	{
		PUT_COMPTIME_STR("inp:");

		char inp[35];
		int written = GET_STR(inp);

		PUT_COMPTIME_STR("input:");
		put_str(inp, written);
		new_line();
	}

	PUT_COMPTIME_STR("press any key to exit");
	get_sk_blk();
	new_line();

	PUT_COMPTIME_STR("BYE");
	new_line();
}
