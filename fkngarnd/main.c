
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
//
// CALL CONVENTION: https://www.msx.org/forum/msx-talk/development/sdcc-4112-a-game-changer-for-c-programming

// static variables don't seem to work

// use chars whenever possible, as opposed to ints; at some point I got absolute dogshit performance in a for loop, and I fixed the problem by simply using chars instead of ints

// list of (all?) bcalls can be found here: https://wikiti.brandonw.net/index.php?title=Category:83Plus:BCALLs:By_Name
// if u need to check for a specific hex code just change the last 4 digits of the following link: https://wikiti.brandonw.net/index.php?title=83Plus:RAM:8478

// if your cursor has reached the end of the screen and you try to put a character, the cursor's position will actually stay the same instead of increasing, overflowing or saturating
// tested with the X position of the cursor

// the syntax `char str* = "asdf";` seems to not work at all; instead you should use the string inplace `"asdasd"` or the bracket syntax `char str[] = "asdf";`

// if we are to print a list of strings, the way we declared the string makes the difference between the code working and failing
//
// this works:
// char str0[] = "asd";
// char str1[] = "fgh";
// char *str_arr[] = {str0, str1};
//
// this doesn't work:
// char *str_arr[] = {"asd", "fgh"};

//// CONVENTIONS

// if we are to signify any "warning" we do it by printing `<TEXT>`, where `TEXT` is the actual text of the warning

// if we are to signify a runtime error we do it by printing `<EN>`, where `N` is replaced by a unique number (so that there isn't one `N` used for multiple errors found on multiple lines in the source code)

//// TODO

// perhaps we should dedicate the top line for error codes, battery, and memory

//// CODE

#include "ti83plus.h"

// config compiler
#pragma disable_warning 85 // disable parameter not used warning
// #pragma disable_warning 59 // disable no return warning
#pragma disable_warning 283 // disable: function declarator with no prototype
// #pragma disable_warning 196 // disable: pointer target lost const qualifier

// display params
#define SYMBOL_HEIGHT_PIXELS 5
// different symbols have different width; we can change this if we make a wrapper around the put_char fnc that checks for the cursor x
#define FATTEST_SYMBOL_WIDTH_PIXELS 5 // `*` is 5 pixels wide, excluding the space pixel
#define DISPLAY_HEIGHT_PIXELS 64 // indexed 0-63
#define DISPLAY_WIDTH_PIXELS 96 // indexed 0-95

#define REPEAT_SPACE1PX 2 // how many times to repeat the space1px character when requested, as to increase readability; 1 mean repeat once, so it will be printed twice in total // predi prosto zamenqh s 4px space-a no tova nqkak suzdade nqkakuv bug koito qde bukvi
#define ASCII_SPACE_1PX ' '
#define ASCII_SPACE_4PX 0x06
#define ASCII_CURRENT_LINE_INDICATOR 0x05 // this needs to be at max 4px wide, so that we can clear it with a single 4px space
#define ASCII_UNKNOWN_CHARACTER '?' // what are we going to return if we get an unknown key code

// use some of the worthless ASCII codes
#define CHAR_CLEAR 0x01
#define CHAR_BACKSPACE 0x02

#define PUT_MULTILINE_STR_PAUSE_EVERY_N_LINES 7

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

char get_sk_blk();
void put_char(char ch);
void put_char_as_num(char ch);
void put_str(char *str, int len);

// magic - don't remove this or the code stops working; I don't know why

void printc(char ch) __naked{
	__asm
		pop hl      ; Get input
		pop bc
		push bc
		push hl 
		ld a, c
		push ix // kogato zakomentiram tova i suotvetniq pop printeneto na novi redove spira da raboti
		BCALL(_VPutMap)
		pop ix
		ret
	__endasm;
}

// total ram reset

char total_ram_reset() __naked{
	__asm
		BCALL(_resetRam)
		ret // we don't really need this, but I'll keep in just in case I call the wrong bcall on the line up above
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

// screen - clear

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

// screen - reset

void reset_screen(){
	clear_screen();
	reset_cur();
	put_char(ASCII_CURRENT_LINE_INDICATOR);
}

// screen - check if there is enough space

// TODO I need to check what happens if we try to print a character that is too wide
// since FATTEST_SYMBOL_WIDTH_PIXELS doesn't take into account the last space character
char screen_check_x_if_enough_space_for_1_more_char(char cur_x){
	return cur_x +1 + FATTEST_SYMBOL_WIDTH_PIXELS <= DISPLAY_WIDTH_PIXELS; // `+1` because `cur_x` is 0-indexed
}

// output - new line

void clear_line(){
	set_cur_x(0);

	for(char i=0; i+=4; i<DISPLAY_WIDTH_PIXELS){
		put_char(ASCII_SPACE_4PX);
	}

	set_cur_x(0);

	put_char(ASCII_CURRENT_LINE_INDICATOR);
}

void new_line(){
	set_cur_x(0);
	put_char(ASCII_SPACE_4PX);

	move_cur_to_next_line();
	clear_line();
}

// output - char

void put_char_nowrapper(char ch) __naked{
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

// returns 1 if the character was a new line
void put_char(char ch){

#if REPEAT_SPACE1PX
	if(ch == ASCII_SPACE_1PX){
		for(char i=0; i<REPEAT_SPACE1PX; ++i){
			put_char_nowrapper(ch);
		}
	}
#endif

	put_char_nowrapper(ch);
}

// output - string

void put_bad_str(char *str){
	for(;;){
		char ch = *str++;
		if(ch == 0){
			break;
		}
		put_char(ch);
	}
}

// if we want some more safety we can implement maxlen
void put_str(char *str, int len){
	if(len < 0){
		PUT_COMPTIME_STR("<E1>");
	}

	char *end = str + len;

	while(str != end){
		put_char(*str++);
	}
}

void put_multiline_str(char *str, int len){
	if(len < 0){
		PUT_COMPTIME_STR("<E2>");
	}

	char *end = str + len;

	char new_lines_put = 0;

	while(str != end){
		char ch = *str++;

		char cur_x = get_cur_x();
		char no_more_space = !screen_check_x_if_enough_space_for_1_more_char(cur_x);
		// char char_is_new_line = (ch == '\n');

		// if(no_more_space || char_is_new_line){
		if(no_more_space){
			new_line();
			new_lines_put += 1;
			if(new_lines_put >= PUT_MULTILINE_STR_PAUSE_EVERY_N_LINES){
				new_lines_put = 0;
				PUT_COMPTIME_STR("press key to keep going");
				get_sk_blk();
				clear_line();
			}

			// if(!char_is_new_line){
			// 	str -= 1;
			// }
			// continue;
		}

		put_char(ch);

	}
}

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
		case 132:
			return '^';
		case 133:
			return '(';
		case 134:
			return ')';
		case 153:
			return ' ';
		case 198:
			return ':';
		case 202:
			return '?';
		case 203:
			return '"';
	}

	PUT_COMPTIME_STR("<chr");
	put_char_as_num(ch);
	put_char('>');
	
	return ASCII_UNKNOWN_CHARACTER;
}

// input - logic

char get_yes_or_no(){
	PUT_COMPTIME_STR("Y/N:");

	char ret;

	for(;;){
		char ch = get_char_blk();

		switch(ch){

			case 'Y':
			case '1':
				ret = 1;
				break;

			case 'N':
			case '0':
				ret = 0;
				break;

			default:
				continue;
		}

		put_char(ch);

		break;
	}

	new_line();
	return ret;
}

// input - string

int get_str(char *arg_place_to_store, int arg_size_place_to_store){
	char *storage = arg_place_to_store;
	int bytes_left = arg_size_place_to_store;
	int bytes_written = 0;

	char starting_x = get_cur_x();

	char byte_start_xs[DISPLAY_WIDTH_PIXELS]; // I have the fact that we have to loose about 100 bytes; oh well...

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
			for(char i=0; i<pixels_to_clean; i+=4){
				put_char(ASCII_SPACE_4PX);
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
			put_char(ASCII_SPACE_4PX); // not possible characters will be cleared (there are some niche 5px chars), but we'll close our eyes for now
			set_cur_x(start_x);
			continue;
		}

		byte_start_xs[bytes_written] = get_cur_x();

		put_char(ch);

		*storage++ = ch;
		bytes_left -= 1;
		bytes_written += 1;

		char cur_x = get_cur_x();
		// char cur_y = get_cur_y();

		// set_cur_y(0);
		// clear_line();
		// put_char_as_num(cur_x);
		// set_cur_y(cur_y);
		// set_cur_x(cur_x);

		if(!screen_check_x_if_enough_space_for_1_more_char(cur_x)){
			// no more space on screen
			break;
		}
	}

	*storage = 0;

	new_line();

	return bytes_written;
}

// main

void main() {
	reset_screen();

	{

		char autogenerated_str_110738937_0[] = {53, 46, 32, 40, 53, 32, 116, 111, 99, 104, 107, 105, 41, 32, 75, 97, 107, 118, 111, 32, 112, 114, 101, 100, 115, 116, 97, 118, 108, 113, 118, 97, 116, 32, 109, 114, 101, 106, 105, 116, 101, 32, 115, 32, 115, 32, 109, 97, 114, 115, 104, 114, 117, 116, 105, 122, 105, 114, 97, 110, 101, 32, 110, 97, 32, 100, 117, 108, 106, 105, 110, 97, 32, 110, 97, 32, 118, 117, 108, 110, 97, 116, 97, 32, 87, 97, 118, 101, 108, 101, 110, 103, 116, 104, 32, 82, 111, 117, 116, 101, 100, 32, 78, 101, 116, 119, 111, 114, 107, 115, 44, 32, 87, 82, 78, 63};
		char autogenerated_str_110738937_1[] = {84, 101, 122, 105, 32, 109, 114, 101, 106, 105, 32, 105, 109, 97, 116, 32, 112, 111, 116, 101, 110, 99, 105, 97, 108, 97, 32, 100, 97, 32, 105, 122, 98, 101, 103, 110, 97, 116, 32, 112, 114, 111, 98, 108, 101, 109, 105, 116, 101, 32, 118, 32, 66, 38, 83, 32, 109, 114, 101, 106, 105, 116, 101, 58, 32, 124, 124, 124, 32, 108, 105, 112, 115, 97, 116, 97, 32, 110, 97, 32, 112, 111, 118, 116, 111, 114, 110, 97, 32, 117, 112, 111, 116, 114, 101, 98, 97, 32, 110, 97, 32, 100, 117, 108, 106, 105, 110, 97, 116, 97, 32, 110, 97, 32, 118, 117, 108, 110, 97, 116, 97, 44, 32, 122, 97, 103, 117, 98, 105, 116, 101, 32, 111, 116, 32, 114, 97, 122, 100, 101, 108, 113, 110, 101, 116, 111, 32, 110, 97, 32, 109, 111, 115, 104, 116, 110, 111, 115, 116, 116, 97, 32, 105, 32, 109, 97, 115, 104, 116, 97, 98, 105, 114, 117, 101, 109, 111, 115, 116, 32, 100, 111, 32, 87, 65, 78, 46, 32, 124, 124, 124, 32, 87, 82, 78, 32, 115, 101, 32, 115, 117, 115, 116, 111, 105, 32, 111, 116, 32, 109, 97, 114, 115, 104, 114, 117, 116, 105, 122, 105, 114, 97, 115, 104, 116, 105, 32, 40, 105, 108, 105, 32, 98, 97, 105, 112, 97, 115, 41, 32, 118, 117, 122, 108, 105, 44, 32, 115, 118, 117, 114, 122, 97, 110, 105, 32, 112, 111, 109, 101, 106, 100, 117, 32, 115, 105, 32, 115, 32, 118, 108, 97, 107, 110, 101, 115, 116, 105, 32, 118, 114, 117, 122, 107, 105, 32, 118, 32, 112, 114, 111, 105, 122, 118, 111, 108, 110, 97, 32, 116, 111, 112, 111, 108, 111, 103, 105, 113, 32, 110, 97, 32, 109, 114, 101, 106, 97, 116, 97, 46, 32, 124, 124, 124, 32, 75, 114, 97, 105, 110, 105, 113, 116, 32, 112, 111, 116, 114, 101, 98, 105, 116, 101, 108, 115, 107, 105, 32, 118, 117, 122, 101, 108, 32, 101, 32, 115, 118, 117, 114, 122, 97, 110, 32, 107, 117, 109, 32, 109, 114, 101, 106, 111, 118, 32, 118, 117, 122, 101, 108, 32, 99, 104, 114, 101, 122, 32, 111, 112, 116, 105, 99, 104, 110, 97, 32, 118, 114, 117, 122, 107, 97, 46, 32, 124, 124, 124, 32, 86, 115, 101, 107, 105, 32, 107, 114, 97, 101, 110, 32, 118, 117, 122, 101, 108, 32, 101, 32, 111, 98, 111, 114, 117, 100, 118, 97, 110, 32, 115, 32, 110, 97, 98, 111, 114, 32, 111, 116, 32, 111, 112, 116, 105, 99, 104, 110, 105, 32, 112, 114, 101, 100, 97, 118, 97, 116, 101, 108, 105, 32, 105, 32, 112, 114, 105, 101, 109, 110, 105, 99, 105, 32, 122, 97, 32, 105, 122, 112, 114, 97, 115, 104, 116, 97, 110, 101, 32, 110, 97, 32, 100, 97, 110, 110, 105, 32, 118, 32, 109, 114, 101, 106, 97, 116, 97, 32, 105, 32, 112, 111, 108, 117, 99, 104, 97, 118, 97, 110, 101, 32, 110, 97, 32, 100, 97, 110, 110, 105, 32, 111, 116, 32, 109, 114, 101, 106, 97, 116, 97, 44, 32, 115, 117, 111, 116, 118, 101, 116, 110, 111, 32, 109, 111, 106, 101, 32, 100, 97, 32, 115, 101, 32, 114, 101, 103, 117, 108, 105, 114, 97, 116, 32, 112, 111, 32, 100, 117, 108, 106, 105, 110, 97, 32, 110, 97, 32, 118, 117, 108, 110, 97, 116, 97, 46};
		char *pairs_vupros_otgovor[] = {autogenerated_str_110738937_0, autogenerated_str_110738937_1};
		int pairs_vupros_otgovor_lens[] = {116, 604};

		for(char i=0; i<LENOF(pairs_vupros_otgovor)/2; ++i){
			char *vupros = pairs_vupros_otgovor[i];
			char *otgovor = pairs_vupros_otgovor[i+1];

			int vupros_len = pairs_vupros_otgovor_lens[i];
			int otgovor_len = pairs_vupros_otgovor_lens[i+1];

			reset_screen();

			PUT_COMPTIME_STR("QUESTION");
			new_line();

			put_multiline_str(vupros, vupros_len);
			new_line();

			PUT_COMPTIME_STR("show answer?");
			char show_answer = get_yes_or_no();

			if(!show_answer){
				continue;
			}

			reset_screen();

			PUT_COMPTIME_STR("ANSWER");
			new_line();

			put_multiline_str(otgovor, otgovor_len);
			new_line();

			PUT_COMPTIME_STR("PRES KEY 4 NEXT QUESTION");
			get_sk_blk();
			new_line();
		}
	}

	// {
	// 	PUT_COMPTIME_STR("enter:");

	// 	char data[30];
	// 	int written = GET_STR(data);

	// 	PUT_COMPTIME_STR("got:")
	// 	put_str(data, written);
	// 	new_line();
	// }

	new_line();
	PUT_COMPTIME_STR("press any key to exit");
	new_line();
	PUT_COMPTIME_STR("and totally clear RAM");
	get_sk_blk();

	// it sucks that we have to do this, but the RAM gets corrupted for some reason
	total_ram_reset();
}
