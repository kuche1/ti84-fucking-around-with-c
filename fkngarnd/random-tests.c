
void main() {
	reset_screen();

	// char *pairs_vupros_otgovor[] = {
	// 	"4. (4 tochki) Kakvo predstavlqva plana za upravlenie v optichnata mreja",
	// 	"Plan za upravlenie. Tova e vseobhvatnata ravnina, koqto se zanimava s vsichki operacii po administrirane, poddrujka, monitoring na izpulnenieto, diagnostika na greshki, subirane na statisticheski danni i dr.",
	// };

	// for(char i=0; i<LENOF(pairs_vupros_otgovor); ++i){
	// 	char *str = pairs_vupros_otgovor[i];
	// 	put_bad_str(str);
	// }

	char str0[] = "4. (4 tochki) Kakvo predstavlqva plana za upravlenie v optichnata mreja";
	char str1[] = "Plan za upravlenie. Tova e vseobhvatnata ravnina, koqto se zanimava s vsichki operacii po administrirane, poddrujka, monitoring na izpulnenieto, diagnostika na greshki, subirane na statisticheski danni i dr.";

	// put_bad_str(str0);
	// PUT_COMPTIME_STR("4. (4 tochki) Kakvo predstavlqva plana za upravlenie v optichnata mreja");
	PUT_COMPTIME_STR(str0);
	new_line();

	put_bad_str(str1);
	new_line();

	char *str_arr[] = {str0, str1};

	// char *str_arr[] = {
	// 	"4. (4 tochki) Kakvo predstavlqva plana za upravlenie v optichnata mreja",
	// 	"Plan za upravlenie. Tova e vseobhvatnata ravnina, koqto se zanimava s vsichki operacii po administrirane, poddrujka, monitoring na izpulnenieto, diagnostika na greshki, subirane na statisticheski danni i dr.",
	// };

	for(char i=0; i<LENOF(str_arr); ++i){
		char *str = str_arr[i];
		put_bad_str(str);
		new_line();
	}

	PUT_COMPTIME_STR("press enter to exit");
	get_sk_blk();
}

//////////////////////////////////////////

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
