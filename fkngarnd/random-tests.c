
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
