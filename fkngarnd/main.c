
// info
//
// the return value is to be stored in register `a` (not tested with fnc that takes any arguments)
//
// the first argument is stored in `a` (only tested with fnc that takes 1 argument and does not return)

#define USE_GETKEY
#define USE_HEXDUMP

#include "../TiConstructor/lib/essentials.c"
#include "../TiConstructor/lib/textio.c"
#include "../TiConstructor/lib/userinput.c"

// screen state

// void clear_screen() __naked{
// 	__asm
		
// 		bacall(_ClrScrnFull)

// 	__endasm;
// }

void reset_cursor(){
	bcall(_HomeUp);
}

void reset_screen(){
	clearScreen(); // TODO check regs
	resetPen(); // TODO check regs
	reset_cursor();
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

// output - new line

// void newline() __naked{
// 	__asm
// 		ld a, (#penRow)
// 		ld b, #6
// 		add b
// 		ld (#penRow), a

// 		xor a, a
// 		ld (#penCol), a
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
	newline();
	put_char('B');
	put_char('C');
	put_char('D');
	put_char('E');
	newline();
	put_str("q6 mi huq");
	newline();

	for(int i=0; i<30; ++i){
		put_char('a');
	}
	newline();

	put_str("return test");
	newline();
	char ret_value = ret_test();
	put_str("returned value: ");
	put_char(ret_value);
	newline();

	// put_str("enter key: ");
	// char ch = get_char_blk();
	// put_char(ch);
	for(;;){
		newline();
		char ch = get_char_blk();
		if(ch == 'A'){
			break;
		}
		put_char(ch);
	}

	// char ch = get_key_blk();
	// put_char(ch);

	// get_key_blk();
	// get_key_blk();
	// get_key_blk();
	// get_key_blk();
	// get_key_blk();

	// for(;;){
	// 	char key = getKey();
	// 	if(key == sk0){
	// 		break;
	// 	}

	// 	println("fuck you");
	// }

	// PressAnyKey();
}
