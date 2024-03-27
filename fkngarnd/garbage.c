
// outputs a regular C string that end with 0
// this doesn't seem to be much faster than manually doing a loop in C
void put_bad_str(char *str) __naked{
	// kato vidq .asm i vijdam 4e argumenta se slaga v `hl`

	__asm

		push hl

		BCALL(_VPutS) // in(HL); out(HL); https://taricorp.gitlab.io/83pa28d/lesson/week2/day11/index.html

		pop hl;

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







// void reset_cur() __naked{
// 	__asm
// 		xor a, a
// 		ld (#penRow), a
// 		ld (#penCol), a
// 		ret
// 	__endasm;
// }








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


	PUT_COMPTIME_STR("return test");
	new_line();
	char ret_value = ret_test();
	PUT_COMPTIME_STR("asd value: ");
	put_char(ret_value);
	new_line();
