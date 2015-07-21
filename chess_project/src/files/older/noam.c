///*
// * noam.c
// *
// *  Created on: Jun 18, 2015
// *      Author: nirattar
// */
//
//
//
///*setting func, geting the init values of the game
//from the user, return -1 if malloc failed*/
//int settings_mode(){
//
//	/*print the command to enter settings to the user*/
//	printf(ENTER_SETTINGS);
//
//	/*while user does'nt enter QUIT or START*/
//	while (1){
//	char *command;
//	char *call_func;
//	int c;
//	int count_char = 2;
//	char x = 'z';
//	int y = -1;
//	char a = 'z';
//	char b = 'z';
//
//	/*getting the current command*/
//	/////////////////////////////////////////////////////////////////////////////////
//
//	if ((command = (char *)malloc(2 * sizeof(char))) == NULL){
//	printf("%s", "ERROR: malloc has failed\n");
//	return -1;/*there is nothing to free at this point*/
//	}
//	numOfMallocStr++;
//	c = getchar();
//
//	command[0] = c;
//	while ((c = getchar()) != '\n'){
//	count_char++;
//	/*realloc command*/
//	if ((command = (char *)realloc(command, count_char*sizeof(char))) == NULL){
//	printf("%s", "ERROR: realloc has failed\n");
//	free(command);/*free the command*/
//	numOfMallocStr--;
//	return -1;
//	}
//	command[count_char - 2] = c;
//	}
//
//	command[count_char - 1] = '\0';
//
//	////////////////////////////////////////////////////////////////////////////////
//
//	call_func = get_command_setting(command, &x, &y, &a, &b);
//
//	/*checking which function to call*/
//
//	/*call MINI - if the user dosen't sets minimax_depth,
//	the default value is 1*/
//	if (!strcmp(call_func, MINI)){
//	sets_minimax_depth(y);
//	}
//	/*call USER_COLOR*/
//	else if (!strcmp(call_func, USER_COLOR)){
//	sets_user_color(a);
//	}
//	/*call CLEAR*/
//	else if (!strcmp(call_func, CLEAR)){
//	init_board(&board, EMPTY, EMPTY);
//	/*update the number of players to br zero*/
//	count_black_players = 0;
//	count_white_players = 0;
//	}
//	/*call RM*/
//	else if (!strcmp(call_func, RM)){
//	remove1(x, y);
//	}
//	/*call SET*/
//	else if (!strcmp(call_func, SET)){
//	set_position(x, y, a, b);
//	}
//	/*call PRINT*/
//	else if (!strcmp(call_func, PRINT)){
//	print_board(board, BOARD_SIZE);
//	}
//	/*call QUIT*/
//	else if (!strcmp(call_func, QUIT)){
//	/*free command*/
//	free(command);
//	numOfMallocStr--;
//	return 1;
//	}
//	/*call START*/
//	else if (!strcmp(call_func, START)){
//	int is_start;
//	is_start = check_if_can_start();
//	if (is_start){/*we can start*/
//	free(command);/*free command at the end of the loop*/
//	numOfMallocStr--;
//	return 0;
//	}
//	}
//	/*ILLEGAL COMMAND*/
//	else if (!strcmp(call_func, ILLEGAL)){
//	printf(ILLEGAL_COMMAND);
//	}
//	free(command);/*free command at the end of the loop*/
//	numOfMallocStr--;
//	}//end while
//}
///*getting the string the user entered,
//and finding the right command, asuuming
//that if the prefix is legal - so is the command.
//in case of illegal command return ILLEGAL string*/
//char* get_command_setting(char *command, char *x, int *y, char *a, char *b){
//	int i = 0;
//	char str[3];
//	/*check if this is "rm <x,y>" command*/
//	if (command[0] == 'r' && command[1] == 'm' && command[2] == ' '){
//	i = 3;
//	while (command[i] == ' '){
//	i++;
//	}
//	/*initialize the borad position parameters*/
//	*x = command[i + 1];
//
//	str[0] = command[i + 3];
//	if (command[i + 4] != '>'){/*the number is 2 - digits*/
//	str[1] = command[i + 4];
//	str[2] = '\0';
//	}
//	else{/*the number is 1 digit*/
//	str[1] = '\0';
//
//	}
//	*y = atoi(str);
//	return RM;
//	}
//	/*check if this is "set <x,y> a b" command*/
//	else if (command[0] == 's' && command[1] == 'e' && command[2] == 't' && command[3] == ' '){
//	i = 4;
//	while (command[i] == ' '){
//	i++;
//	}
//	/*initialize the borad position parameters*/
//	*x = command[i + 1];
//
//	str[0] = command[i + 3];
//	if (command[i + 4] != '>'){/*the number is 2 - digits*/
//	str[1] = command[i + 4];
//	str[2] = '\0';
//	i = i + 6;
//	}
//	else{/*the number is 1 digit*/
//	str[1] = '\0';
//	i = i + 5;
//	}
//	*y = atoi(str);
//	/*initialize the borad position color & type*/
//
//	while (command[i] == ' '){
//	i++;
//	}
//	*a = command[i];/*taking just the first char! should be 'b' or 'w'*/
//	i = i + 5;
//	while (command[i] == ' '){
//	i++;
//	}
//	*b = command[i];
//	return SET;
//	}
//
//
//	/*check if this is "clear" command*/
//	else if (!strcmp(command, CLEAR)){
//	return CLEAR;
//	}
//	/*check if this is "print" command*/
//	else if (!strcmp(command, PRINT)){
//	return PRINT;
//	}
//	/*check if this is "quit" command*/
//	else if (!strcmp(command, QUIT)){
//	return QUIT;
//	}
//	/*check if this is "start" command*/
//	else if (!strcmp(command, START)){
//	return START;
//	}
//	/*check if this is "minimax_depth x" command*/
//	else if (strlen(command) >= 15){
//	if (command[0] == 'm' && command[1] == 'i' && command[2] == 'n' && command[3] == 'i'  && command[4] == 'm'
//	&& command[5] == 'a' && command[6] == 'x' && command[7] == '_' && command[8] == 'd'
//	&& command[9] == 'e' && command[10] == 'p' && command[11] == 't' && command[12] == 'h'){
//	i = 13;
//	while (command[i] == ' '){
//	i++;
//	}
//	str[0] = command[i];
//	i++;
//	if (command[i] != '\0'){/*entered a number with more then one digit - i.e > 6*/
//	str[0] = '9';
//	}
//	str[1] = '\0';
//	*y = atoi(str);
//
//	return MINI;
//	}
//	/*check if this is "user_color x" command*/
//	else if (strlen(command) == 16){
//	if (command[0] == 'u' && command[1] == 's' && command[2] == 'e' && command[3] == 'r'  && command[4] == '_'
//	&& command[5] == 'c' && command[6] == 'o' && command[7] == 'l' && command[8] == 'o'
//	&& command[9] == 'r'){
//	i = 10;
//	while (command[i] == ' '){
//	i++;
//	}
//	*a = command[i];/*taking just the first char! should be 'b' or 'w'*/
//
//	return USER_COLOR;
//	}
//	else{/*illegal command*/
//	return ILLEGAL;
//
//	}
//	}
//	else{/*illegal command*/
//	return ILLEGAL;
//
//	}
//
//	}
//
//
//	else{/*illegal command*/
//	return ILLEGAL;
//
//	}
//	return "a"; /*shold not get here*/
//}
//
///*sets the minimax depth according to
//the entered value by the user - if none the
//default is 1, returning error mess in case of ilegall
//value*/
//void sets_minimax_depth(int x){
//	/*value should be between 1 to 6, including both*/
//	if (x <= 6 && x >= 1){
//	minimax_depth = x;
//	}
//	/*ilegal value*/
//	else{
//	printf(WRONG_MINIMAX_DEPTH);
//	}
//}
//
///*sets the user color according to
//the entered color by the user - if none the
//default for the user is white*/
//void sets_user_color(char a){
//	if (a == 'b'){
//	userIsBlack = 1;
//	}
//	else if (a == 'w'){
//	userIsBlack = 0;
//
//	}
//}
//
///*gets the two position cordination
//and removes the position if possible,
//in case of illegal position - prints error mess*/
//void remove1(char x, int y){
//	if (check_legal_scope_and_white_block(x, y)){/*the position is legal*/
//
//	int d = x - 97;
//
//	/*checks the color of the player in this position to update the players counter*/
//	if (board[d][y - 1] == 'm' || board[d][y - 1] == 'k'){
//	count_white_players--;
//	}
//	else if (board[d][y - 1] == 'M' || board[d][y - 1] == 'K'){
//	count_black_players--;
//	}
//	board[d][y - 1] = EMPTY;/*setting the position to be EMPTY*/
//
//	}
//}
///*gets the two position cordination
//and the position color & type,
//and sets the position if possible,
//in case of illegal position - prints error mess*/
//void set_position(char x, int y, char a, char b){
//	if (check_legal_scope_and_white_block(x, y)){/*the position is legal*/
//
//	int d = x - 97;
//	/*check if at this position there is already a player
//	and update his counter*/
//	update_players_counter(d, y - 1);
//
//	/*allocate the new position*/
//	if ((a == 'b') && (b == 'm')){ /*a black man*/
//	/*if user entered a black man at his "winning line"*/
//	if ((y - 1) == 0){
//	board[d][y - 1] = BLACK_K;
//	}
//	else{
//	board[d][y - 1] = BLACK_M;
//	}
//
//	count_black_players++;
//	}
//	else if ((a == 'b') && (b == 'k')){/*a black king*/
//	board[d][y - 1] = BLACK_K;
//	count_black_players++;
//	}
//	else if ((a == 'w') && (b == 'k')){/*a white king*/
//	board[d][y - 1] = WHITE_K;
//	count_white_players++;
//	}
//	else{/*a white man*/
//	/*if user entered a white man at his "winning line"*/
//	if ((y - 1) == 9){
//	board[d][y - 1] = WHITE_K;
//	}
//	else{
//	board[d][y - 1] = WHITE_M;
//	}
//	count_white_players++;
//	}
//
//	}
//
//}
//
///*calling is_leganl_position to cheack position
//and in addition cheack if it isn't a white block*/
//int check_legal_scope_and_white_block(char x, int y){
//	int is_legal;
//	Position p;
//	p.x = x - 97;
//	p.y = y - 1;
//	/*calling a function that checks that the range of x & y is valid*/
//	is_legal = is_legal_position(&p);
//	if (!is_legal){/*the position is NOT legal*/
//	printf(WRONG_POSITION);
//	return 0;
//	}
//	return 1; /*legal position*/
//
//}
//
//
