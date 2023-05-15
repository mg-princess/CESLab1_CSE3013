#include "tetris.h"

static struct sigaction act, oact;

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));
	createRankList();
	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_RANK: rank(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	endwin();
	system("clear");
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;
	nextBlock[2]=rand()%7;
	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	DrawOutline();
	DrawField();
	DrawBlockWithFeatures(blockY,blockX,nextBlock[0],blockRotate);
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);
	DrawBox(9,WIDTH+10,4,8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(15,WIDTH+10);
	printw("SCORE");
	DrawBox(16,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(17,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
	for( i = 0; i < 4; i++ ){
		move(10+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[2]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}
		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);
	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	for(int i = 0; i<4; i++) {
		for(int j = 0; j<4; j++) {
			if(block[currentBlock][blockRotate][i][j] == 1) {
				if(i+blockY >= HEIGHT || i+blockY < 0 || j+blockX >= WIDTH || j+blockX < 0)
					return 0;
				if(field[i+blockY][j+blockX] == 1)
					return 0;
			}
		}
	}
	return 1;
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	int i,j;
	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	
	int oldBlockX = blockX, oldBlockY = blockY, oldBlockRotate = blockRotate;
	switch(command) {
		case KEY_UP:
			oldBlockRotate=(oldBlockRotate+3)%4;
			break;
		case KEY_DOWN:
			oldBlockY--;
			break;
		case KEY_RIGHT:
			oldBlockX--;
			break;
		case KEY_LEFT:
			oldBlockX++;
			break;
	}
	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(block[currentBlock][oldBlockRotate][i][j]==1){
				move(i+oldBlockY+1,j+oldBlockX+1);
				printw(".");
			}
		}
	}
	move(HEIGHT,WIDTH+10);

	//3. 새로운 블록 정보를 그린다. 
	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
}

void BlockDown(int sig){
	//강의자료 p26-27의 플로우차트를 참고한다.
	if((CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX))){
		blockY++;
		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
	}
	else {
		if(blockY == -1) {
			gameOver = 1;
		}
		AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
		score += DeleteLine(field);
		PrintScore(score);
		nextBlock[0] = nextBlock[1];
		nextBlock[1] = nextBlock[2];
		nextBlock[2] = rand()%7;
		DrawNextBlock(nextBlock);
		blockRotate=0;
		blockY=-1;
		blockX=WIDTH/2-2;
		DrawField();
	}
	timed_out = 0;
}

void AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	//Block이 추가된 영역의 필드값을 바꾼다.
	int i,j,touched=0;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[currentBlock][blockRotate][i][j]==1 && i+blockY>=0){
				if(field[i+blockY+1][j+blockX] == 1 || i+blockY == HEIGHT-1)
					touched++;
				field[i+blockY][j+blockX] = 1;
			}
		}
	score += touched*10;
	move(HEIGHT,WIDTH+10);
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	// user code

	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
	int i, j, k, flag = 1, score_cnt = 0;
	for(i = HEIGHT-1; i>=0; i--){
		flag = 1;
		for(j = 0; j<WIDTH; j++) {
			if(field[i][j] == 0){
				flag = 0;
				break;
			}
		}
		if(flag) { //한줄이 모두 채워짐
			for(k = i; k>0; k--) {
				for(j = 0; j<WIDTH; j++) 
					field[k][j] = field[k-1][j];
			}
			for(j = 0; j<WIDTH; j++) {
				field[0][j] = 0;
			}
			score_cnt++;
			i++;
		}
	}
	return (score_cnt*score_cnt)*100;
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID,int blockRotate){
	while(CheckToMove(field, blockID, blockRotate, y+1, x))
		y++;
	DrawBlock(y, x, blockID, blockRotate, '/');
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate) {
	DrawField();
	DrawBlock(y, x, blockID, blockRotate, ' ');
	DrawShadow(y, x, blockID, blockRotate);
}

void createRankList(){
	FILE *file;
	int i, userScore;
	char name[NAMELEN];

	file = fopen("rank.txt", "r+");
	fscanf(file, "%d", &userNum);
	fgetc(file);
	node = malloc(sizeof(*node));
	node->score = userNum;
	Node *curNode = node, *newNode, *preNode;

	for(i = 0; i<userNum; i++) {
		newNode = malloc(sizeof(*node));
		fscanf(file, "%s %d", name, &userScore);
		fgetc(file);
		
		strncpy(newNode->name, name, NAMELEN);
		newNode->name[NAMELEN-1] = '\0';
		newNode->score = userScore;
		newNode->next = NULL;

		curNode->next = newNode;
		preNode = curNode;
		curNode = curNode->next;
	}
	fclose(file);
}

void rank(){
	clear();
	printw("1. list ranks from X to Y\n");
	printw("2. list ranks by a specific name\n");
	printw("3. delete a specific rank\n");
	int input = 0, count = 0, flag = 1;
	int X = 1, Y = 5; 
	Node *curNode = node->next;
	input = wgetch(stdscr);
	switch(input) {
		case '1':
			printw("X: "); scanw("%d", &X); X--;
			printw("Y: "); scanw("%d", &Y); Y--;
			printw("      name     |   score\n");
			printw("----------------------------\n");
			if(X > Y || X*Y < 0){
				printw("search failure: no rank in the list\n");
				wgetch(stdscr);
				return;
			}

			while(curNode != NULL) {
				if(X <= count && count <= Y)
					printw("%-16s| %d\n", curNode->name, curNode->score);
				curNode = curNode->next;
				count++;
			}
			break;
		case '2':
			printw("input the name: ");
			char name[NAMELEN];
			echo();
			scanw("%s", name);
			noecho();
			printw("      name     |   score\n");
			printw("----------------------------\n");
			
			while(curNode != NULL) {
				if(!strcmp(name, curNode->name)){
					printw("%-16s| %d\n", curNode->name, curNode->score);
					flag = 0;
				}
				curNode = curNode->next;
			}
			if(flag)
				printw("search failure: no name in the list\n");
			break;
		case '3':
			printw("input the rank: "); 
			int rank;
			scanw("%d", &rank); rank--;

			if(curNode == NULL) {
				printw("search failure: the rank not in the list\n");
			}
			else {
				Node *preNode = node;
				while(curNode->next != NULL) {
					if(count == rank) {
						preNode->next = curNode->next;
						userNum--;
						free(curNode);
						flag = 0;
						printw("result: the rank deleted");
					}
					curNode = curNode->next;
					preNode = preNode->next;
					count++;
				}
				if(flag) 
					printw("search failure: the rank not in the list\n");
			}
			break;
	}
	writeRankFile();
	wgetch(stdscr);
}

void writeRankFile(){
	if(node->score == userNum)
		return;
	FILE *file;
	file = fopen("rank.txt", "w");
	fprintf(file, "%d\n", userNum);
	Node *curNode = node->next;
	while(curNode != NULL) {
		fprintf(file, "%s %d\n", curNode->name, curNode->score);
		curNode = curNode->next;
	}
	fclose(file);
}

void newRank(int score){
	Node *curNode = node->next, *newNode;
	newNode = malloc(sizeof(*node));

	clear();
	char name[NAMELEN];
	printw("your name: ");
	scanw("%s", name);
	strncpy(newNode->name, name, NAMELEN);
	newNode->name[NAMELEN-1] = '\0';
	newNode->score = score;
	newNode->next = NULL;
	if(curNode == NULL || curNode->score < score) {
		newNode->next = curNode;
		node->next = newNode;
	}
	else {
		while(curNode->next != NULL) {
			if(curNode->next->score < score) {
				newNode->next = curNode->next->next;
				curNode->next = newNode;
				break;
			}
			curNode = curNode->next;
		}
	}
	userNum++;
	writeRankFile();
	wgetch(stdscr);
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
}

int recommend(RecNode *root){
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	// user code

	return max;
}

void recommendedPlay(){
	// user code
}
