
// info

// https://taricorp.gitlab.io/83pa28d/lesson/week1/day03/index.html
// registers: af, bc, de, hl, ix

// the return value is to be stored in register `a` (tested with fnc that returns a char; not tested with fnc that takes any arguments)
//
// the first argument is stored in `a` (only tested with fnc that takes 1 char argument and does not return)

// static variables don't seem to work

#include "../TiConstructor/lib/essentials.c"

// AKO ZAKOMENTIRAM TIQ 2TA FAILA NE6TO STAVA I ELKATA KRA6VA
#include "../TiConstructor/lib/textio.c" // tova kato go zakomentiram i ne6tata po4vat da se usirat
// #include "../TiConstructor/lib/userinput.c" // za sega izglejda kato tova da moga da go zakomentiram bez problemi

#define SYMBOL_HEIGHT_PIXELS 5
// different symbols have different width; we can change this if we make a wrapper around the put_char fnc that checks for the cursor x
#define DISPLAY_HEIGHT_PIXELS 63
#define DISPLAY_WIDTH_PIXELS 95

// prototypes

void put_char(char ch);

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
	int old_x = get_cur_x();

	set_cur_x(0);

	// TODO tova e giga bavno
	for(int i=0; i<93; ++i){
		put_char(' ');
	}

	set_cur_x(old_x);
}

void new_line(){
	move_cur_to_next_line();
	clear_line();

	int original_line_y = get_cur_y();

	move_cur_to_next_line();
	for(int i=0; i<20; ++i){ // TODO tova e giga bavno
		put_char('^');
	}

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
void put_str(char *str){
	for(;;){
		char c = *str++;
		if(c==0){
			break;
		}
		put_char(c);
	}
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

	put_char('<');
	put_char_as_num(ch);
	put_char('>');
	
	return '?';
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
	set_cur_x(0);
	for(int i=0; i<93; ++i){
		put_char(' ');
	}
	new_line();

	put_str("return test");
	new_line();
	char ret_value = ret_test();
	put_str("asd value: ");
	put_char(ret_value);
	new_line();

	// put_str("enter key: ");
	// char ch = get_char_blk();
	// put_char(ch);
	for(int count=0;; count += 1){
		new_line();
		char ch = get_char_blk();
		if(ch == 'A'){
			break;
		}
		put_char(ch);

		put_char(' ');
		int cur_y = get_cur_y();
		put_char_as_num(cur_y);
	}

}
