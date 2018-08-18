#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>

#define LEFT 75		// Keyboard Input
#define RIGHT 77 
#define UP 72 
#define DOWN 80 
#define p 112
#define P 80 
#define ESC 27  

//+ Key
#define W 87
#define w 119
#define A 65
#define a 97
#define S 83
#define s 115
#define D 68
#define d 100
#define F 70
#define f 102
#define M 77
#define m 109
// Key + End
#define false 0
#define true 1

#define ACTIVE_BLOCK -2 // Block State
#define CEILLING -1      
#define EMPTY 0         
#define WALL 1
#define INACTIVE_BLOCK 2 

#define MAIN_X 11	// Game borad Size
#define MAIN_Y 23 
#define MAIN_X_ADJ 3 
#define MAIN_Y_ADJ 1  

#define STATUS_X_ADJ MAIN_X_ADJ+MAIN_X+1 // Status info position

int STATUS_Y_GOAL; // Status info position

int STATUS_Y_SCORE;

int blocks[7][4][4][4] = {
	{ { 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 },
	{ 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 } },
	{ { 0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0 },{ 0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0 },
	{ 0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0 },{ 0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0 } },
	{ { 0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0 },
	{ 0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0 } },
	{ { 0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0 },{ 0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0 },
	{ 0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0 },{ 0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0 } },
	{ { 0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0 },{ 0,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0 },
	{ 0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0 },{ 0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0 } },
	{ { 0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,0 },
	{ 0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0 },{ 0,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0 } },
	{ { 0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0 },
	{ 0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0 },{ 0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0 } }
}; // Shape of block

int b_type; // block type
int b_rotation; // block rotation
int b_type_next; //new block

int main_org[MAIN_Y][MAIN_X];
int main_cpy[MAIN_Y][MAIN_X];

int bx, by; // x,y position of moving block

int key; // keyboard input 

int speed; // Game Speed 

		   //+변수
int score_p1; // Game Score
int score_p2;
int p1_temp;
int p2_temp;
int item_p1 = 1, item_p1a;
int item_p2 = 1, item_p2a;
int turn = 1;
int now = 1;
FILE * fptr;
int item_again;
int item_use;
int item_use_on;
//추가 변수 End
int new_block_on = 0;
int crush_on = 0;



void title(void);
void reset(void);
void reset_main(void); // 
void reset_main_cpy(void);
void draw_map(void);
void draw_main(void);
void new_block(void);
void check_key(void);

int check_crush(int bx, int by, int rotation);
void drop_block(void);
void move_block(int dir);
void check_line(void);
void gotoxy(int x, int y) {
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
//

void item_show(void) {
	int x = 5, y = 5;
	gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
	gotoxy(x, y + 1); printf("▤                                          ▤");
	gotoxy(x, y + 2); printf("▤  +-----------------------------------+   ▤");
	gotoxy(x, y + 3); printf("▤  |  P%d uses item. Select P%d block.   |   ▤", item_use, now);
	gotoxy(x, y + 4); printf("▤  +-----------------------------------+   ▤");
	gotoxy(x, y + 5); printf("▤  Press any key to resume..               ▤");
	gotoxy(x, y + 6); printf("▤    1   ■ 2 ■■   3   ■       4   ■   ▤");
	gotoxy(x, y + 7); printf("▤        ■   ■■     ■■■       ■■   ▤");
	gotoxy(x, y + 8); printf("▤        ■                         ■     ▤");
	gotoxy(x, y + 9); printf("▤        ■                                ▤");
	gotoxy(x, y + 10); printf("▤              %d sec(s) lefts.             ▤", x);
	gotoxy(x, y + 11); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");

}
//
void check_game_over(void);
void pause(void);
void setcolor(unsigned short text, unsigned short back)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text | (back << 4));
}
void colorset(void) {
	if (now == 1)
		setcolor(3, 0);
	if (now == 2)
		setcolor(6, 0);
}


typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; // hide cursor
void setcursortype(CURSOR_TYPE c) { // hide cursor
	CONSOLE_CURSOR_INFO CurInfo;

	switch (c) {
	case NOCURSOR:
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize = 20;
		CurInfo.bVisible = TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

int main() {
	int i;

	srand((unsigned)time(NULL));
	setcursortype(NOCURSOR);
	title();
	reset();

	while (1) {
		for (i = 0; i<5; i++) {
			check_key();
			draw_main();

			Sleep(speed);
			if (crush_on&&check_crush(bx, by + 1, b_rotation) == false) Sleep(100);

		}
		drop_block();

		check_game_over();
		if (new_block_on == 1) new_block();
	}
}

void title(void) {
	int x = 5; 
	int y = 4; 


	gotoxy(x, y + 0); printf("┌─────────────┐");
	gotoxy(x, y + 1); printf("│                          │");
	gotoxy(x, y + 2); printf("│                          │");
	gotoxy(x, y + 3); printf("│                          │");
	gotoxy(x, y + 4); printf("└─────────────┘");

	gotoxy(x + 5, y + 2); printf("T E T R I S");
	gotoxy(x, y + 7); printf("Please Enter Any Key to Start..");
	gotoxy(x, y + 10); printf("Player 1:");
	gotoxy(x, y + 11); printf("   W   : Shift");
	gotoxy(x, y + 12); printf("  A  D : Left / Right");
	gotoxy(x, y + 13); printf("   S   : Drop");
	gotoxy(x, y + 14); printf("   F   : Use Item");

	gotoxy(x, y + 16); printf("Player 2:");
	gotoxy(x, y + 17); printf("  △   : Shift");
	gotoxy(x, y + 18); printf("◁  ▷ : Left / Right");
	gotoxy(x, y + 19); printf("  ▽   : Drop");
	gotoxy(x, y + 20); printf("   M   : Use Item");

	gotoxy(x, y + 22); printf(" Pause : P    ESC  : Quit");

	while (true) {
		if (kbhit()) break;
	}

	while (kbhit()) getch();

}

void reset(void) {
	item_p1 = 0;
	item_p1a = 0;
	item_p2 = 0;
	item_p2a = 0;
	score_p1 = 0;
	score_p2 = 0;
	item_again = 0;
	item_use = 0;
	item_use_on = 0;
	time_t timer;
	struct tm *t;
	timer = time(NULL);
	t = localtime(&timer);

	if ((fptr = fopen("tetris.txt", "a")) == NULL) {
		printf("File could not be opened\n");
		return -1;
	}
	else
	{
		fprintf(fptr, "\n %d:%d:%d 시작\n", t->tm_hour, t->tm_min, t->tm_sec);
	}
	fclose(fptr);

	turn = 1;
	score_p1 = 0;
	score_p2 = 0;
	key = 0;
	crush_on = 0;

	speed = 100;

	system("cls");
	reset_main();
	draw_map();
	draw_main();

	b_type_next = rand() % 7;
	new_block();

}

void reset_main(void) {
	int i, j;

	for (i = 0; i<MAIN_Y; i++) { // Game board initialize
		for (j = 0; j<MAIN_X; j++) {
			main_org[i][j] = 0;
			main_cpy[i][j] = 100;
		}
	}
	for (j = 1; j<MAIN_X; j++) {
		main_org[3][j] = CEILLING;
	}
	for (i = 1; i<MAIN_Y - 1; i++) {
		main_org[i][0] = WALL;
		main_org[i][MAIN_X - 1] = WALL;
	}
	for (j = 0; j<MAIN_X; j++) {
		main_org[MAIN_Y - 1][j] = WALL;
	}
}

void reset_main_cpy(void) {
	int i, j;

	for (i = 0; i<MAIN_Y; i++) {
		for (j = 0; j<MAIN_X; j++) {
			main_cpy[i][j] = 100;
		}
	}
}

void draw_map(void) { // Game Status 
	int y = 3;

	gotoxy(STATUS_X_ADJ, y + 1); printf("%P%d차례/%d번째 turn", now, turn);
	gotoxy(STATUS_X_ADJ, y + 2); printf("+-  N E X T  -+ ");
	gotoxy(STATUS_X_ADJ, y + 3); printf("|             | ");
	gotoxy(STATUS_X_ADJ, y + 4); printf("|             | ");
	gotoxy(STATUS_X_ADJ, y + 5); printf("|             | ");
	gotoxy(STATUS_X_ADJ, y + 6); printf("|             | ");
	gotoxy(STATUS_X_ADJ, y + 7); printf("+-- -  -  - --+ ");
	gotoxy(STATUS_X_ADJ, y + 8); printf("P1 score  :");
	gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE = y + 9); printf("        %6d", score_p1);
	gotoxy(STATUS_X_ADJ, y + 10); printf("P2 score  :");
	gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE = y + 11); printf("        %6d", score_p2);
	gotoxy(STATUS_X_ADJ, y + 12); printf("P1 item  :");
	gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE = y + 13); printf("        %6d", item_p1);
	gotoxy(STATUS_X_ADJ, y + 14); printf("P2 item  :");
	gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE = y + 15); printf("        %6d", item_p2);

	gotoxy(STATUS_X_ADJ, 20); printf("Player 1:");
	gotoxy(STATUS_X_ADJ, 21); printf("   W   : Shift");
	gotoxy(STATUS_X_ADJ, 22); printf("  A  D : Left / Right");
	gotoxy(STATUS_X_ADJ, 23); printf("   S   : Drop");
	gotoxy(STATUS_X_ADJ, 24); printf("   F   : Use Item");

	gotoxy(STATUS_X_ADJ, 25); printf("Player 2:");
	gotoxy(STATUS_X_ADJ, 26); printf("  △   : Shift");
	gotoxy(STATUS_X_ADJ, 27); printf("◁  ▷ : Left / Right");
	gotoxy(STATUS_X_ADJ, 28); printf("  ▽   : Drop");
	gotoxy(STATUS_X_ADJ, 29); printf("   M   : Use Item");


}

void draw_main(void) {
	int i, j;

	for (j = 1; j<MAIN_X - 1; j++) {
		if (main_org[3][j] == EMPTY) main_org[3][j] = CEILLING;
	}
	for (i = 0; i<MAIN_Y; i++) {
		for (j = 0; j<MAIN_X; j++) {
			if (main_cpy[i][j] != main_org[i][j]) {

				gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i);
				switch (main_org[i][j]) {
				case EMPTY:
					printf("  ");
					break;
				case CEILLING:
					printf(". ");
					break;
				case WALL:
					printf("▩");
					break;
				case INACTIVE_BLOCK:
					setcolor(7, 0);
					printf("□");
					break;
				case ACTIVE_BLOCK:
					colorset();
					printf("■");
					setcolor(7, 0);
					break;
				}
			}
		}
	}
	for (i = 0; i<MAIN_Y; i++) {
		for (j = 0; j<MAIN_X; j++) {
			main_cpy[i][j] = main_org[i][j];
		}
	}
}

void new_block(void) {
	int i, j, countdown = 5;
	char choice = '5';
	bx = (MAIN_X / 2) - 1;
	by = 0;

	//판별 시작
	if (item_use_on == 1) {
		item_show();

		if (item_use == 1) {
			item_p1--;
		}
		if (item_use == 2) {
			item_p2--;
		}
		item_again = 0;
		item_use_on = 0;
		item_use = 0;
		for (countdown = 5; countdown > 0;) {
			Sleep(1000);
			gotoxy(5, 15); printf("▤              %d sec(s) lefts.             ▤", countdown);
			if (kbhit()) {
				choice = getch();
			}
			countdown -= 1;
		}

		switch (choice) {
		case '1':
			b_type_next = 1; break;
		case '2':
			b_type_next = 0; break;
		case '3':
			b_type_next = 6; break;
		case '4':
			b_type_next = 2; break;
		default: break;
		}


		system("cls");
		draw_main(); draw_map(); reset_main_cpy(); //Draw again

	}

	b_type = b_type_next;
	b_type_next = rand() % 7;
	b_rotation = 0;

	new_block_on = 0;

	for (i = 0; i<4; i++) {
		for (j = 0; j<4; j++) {
			if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = ACTIVE_BLOCK;
		}
	}
	for (i = 1; i<3; i++) {
		for (j = 0; j<4; j++) {
			if (blocks[b_type_next][0][i][j] == 1) {
				gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
				printf("■");
			}
			else {
				gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
				printf("  ");
			}
		}
	}
}

void check_key(void) {
	key = 0;

	if (kbhit()) {
		key = getch();
		if (key == 224) { // direction key
			do { key = getch(); } while (key == 224);
			switch (key) {
			case LEFT:
				if (now == 1)
					break;

				if (check_crush(bx - 1, by, b_rotation) == true) move_block(LEFT);
				break;
			case RIGHT:
				if (now == 1)
					break;
				if (check_crush(bx + 1, by, b_rotation) == true) move_block(RIGHT);
				break;
			case DOWN:
				if (now == 1)
					break;
				if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN);
				break;
			case UP:
				if (now == 1)
					break;
				if (check_crush(bx, by, (b_rotation + 1) % 4) == true) move_block(UP);

				else if (crush_on == 1 && check_crush(bx, by - 1, (b_rotation + 1) % 4) == true) move_block(100);
			}
		}
		else { // Not direction eky
			switch (key) {
			case W:
			case w:
				if (now == 2)
					break;
				if (check_crush(bx, by, (b_rotation + 1) % 4) == true) move_block(UP);
				break;

			case A:
			case a:
				if (now == 2)
					break;
				if (check_crush(bx - 1, by, b_rotation) == true) move_block(LEFT);
				break;
			case S:
			case s:
				if (now == 2)
					break;
				if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN);
				break;

			case D:
			case d:
				if (now == 2)
					break;
				if (check_crush(bx + 1, by, b_rotation) == true) move_block(RIGHT);
				break;


				//Item 
			case F:
			case f:
				if (item_p1 > 0 && item_again == 0) {
					item_use = 1;
					item_use_on = 1;
					item_again = 1;
					//show
				}
				break;
			case M:
			case m:
				if (item_p2 > 0 && item_again == 0) {
					item_use = 2;
					item_use_on = 1;
					item_again = 1;

				}
				break;
			case P:
			case p:
				pause();
				break;
			case ESC:
				system("cls");
				if ((fptr = fopen("tetris.txt", "a")) == NULL) {
					printf("File could not be opened\n");
					return -1;
				}
				else
				{
					fprintf(fptr, "ESC 종료 사용\n");
				}
				fclose(fptr);

				exit(0);
			}
		}
	}
	while (kbhit()) getch();
}


int check_crush(int bx, int by, int b_rotation) {
	int i, j;

	for (i = 0; i<4; i++) {
		for (j = 0; j<4; j++) {
			if (blocks[b_type][b_rotation][i][j] == 1 && main_org[by + i][bx + j]>0) return false;
		}
	}
	return true;
};

void drop_block(void) {
	int i, j;

	if (crush_on&&check_crush(bx, by + 1, b_rotation) == true) crush_on = 0;
	if (crush_on&&check_crush(bx, by + 1, b_rotation) == false) {
		for (i = 0; i<MAIN_Y; i++) {
			for (j = 0; j<MAIN_X; j++) {
				if (main_org[i][j] == ACTIVE_BLOCK) main_org[i][j] = INACTIVE_BLOCK;
			}
		}
		crush_on = 0;
		check_line();
		new_block_on = 1;
		return;
	}
	if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN);
	if (check_crush(bx, by + 1, b_rotation) == false) crush_on++;
}


void move_block(int dir) {
	int i, j;

	switch (dir) {
	case LEFT:
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j - 1] = ACTIVE_BLOCK;
			}
		}
		bx--;
		break;

	case RIGHT:
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j + 1] = ACTIVE_BLOCK;
			}
		}
		bx++;
		break;

	case DOWN:
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i + 1][bx + j] = ACTIVE_BLOCK;
			}
		}
		by++;
		break;

	case UP:
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		b_rotation = (b_rotation + 1) % 4;
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = ACTIVE_BLOCK;
			}
		}
		break;

	case 100:

		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		b_rotation = (b_rotation + 1) % 4;
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i - 1][bx + j] = ACTIVE_BLOCK;
			}
		}
		by--;
		break;
	}
}

void check_line(void) {
	int i, j, k, l;

	int block_amount; // block num of one line


	for (i = MAIN_Y - 2; i>3;) {
		block_amount = 0;
		for (j = 1; j<MAIN_X - 1; j++) {
			if (main_org[i][j]>0) block_amount++;
		}
		if (block_amount == MAIN_X - 2) {
			//플레이어에 따라 점수 +1
			if (now == 1)
				score_p1++;
			if (now == 2)
				score_p2++;

			for (k = i; k>1; k--) {  // Block Down
				for (l = 1; l<MAIN_X - 1; l++) {
					if (main_org[k - 1][l] != CEILLING) main_org[k][l] = main_org[k - 1][l];
					if (main_org[k - 1][l] == CEILLING) main_org[k][l] = EMPTY;

				}
			}
		}
		else i--;
	}
	//아이템 ++
	if (score_p1 != p1_temp) {
		if (score_p1 - p1_temp >= 2)
			item_p1++, item_p1a++;
	}
	if (score_p2 != p2_temp) {
		if (score_p2 - p2_temp >= 2)
			item_p2++, item_p2a++;
	}
	p1_temp = score_p1;
	p2_temp = score_p2;


	switch (now) {
	case 1:
		now = 2;
		break;
	case 2:
		now = 1;
		break;
	}
	turn++;
	int y = 3;
	gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", score_p1);
	gotoxy(STATUS_X_ADJ, y + 1); printf("P%d차례/%d번째 turn", now, turn);
	gotoxy(STATUS_X_ADJ, y + 8); printf("P1 score  :");
	gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE = y + 9); printf("        %6d", score_p1);
	gotoxy(STATUS_X_ADJ, y + 10); printf("P2 score  :");
	gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE = y + 11); printf("        %6d", score_p2);
	gotoxy(STATUS_X_ADJ, y + 12); printf("P1 item  :");
	gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE = y + 13); printf("        %6d", item_p1);
	gotoxy(STATUS_X_ADJ, y + 14); printf("P2 item  :");
	gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE = y + 15); printf("        %6d", item_p2);
}



void check_game_over(void) {
	int i;
	int x = 5;
	int y = 5;
	int win = 0;
	if (score_p1 > score_p2)
		win = 1;
	if (score_p1 < score_p2)
		win = 2;

	for (i = 1; i<MAIN_X - 2; i++) {
		if (main_org[3][i]>0 || turn == 61) {  // end condition 
			int used_p1 = item_p1a - item_p1;
			int used_p2 = item_p2a - item_p2;
			gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
			if (win == 0) {
				gotoxy(x, y + 1); printf("▤       Score same - DRAW!     ▤");
			}
			if (win != 0) {
				gotoxy(x, y + 1); printf("▤           P%d Win!            ▤", win);
			}
			gotoxy(x, y + 2); printf("▤  +-----------------------+   ▤");
			gotoxy(x, y + 3); printf("▤  |  G A M E  O V E R..   |   ▤");
			gotoxy(x, y + 4); printf("▤  +-----------------------+   ▤");
			gotoxy(x, y + 5); printf("▤     P1   SCORE: %6d         ", score_p1);
			gotoxy(x, y + 6); printf("▤     P2   SCORE: %6d         ", score_p2);
			gotoxy(x + 16, y + 5); printf("▤");
			gotoxy(x + 16, y + 6); printf("▤");
			gotoxy(x, y + 7); printf("▤                              ▤");
			gotoxy(x, y + 8); printf("▤  Press any key to restart..  ▤");
			gotoxy(x, y + 9); printf("▤                              ▤");
			gotoxy(x, y + 10); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");

			// 기록 저장
			if ((fptr = fopen("tetris.txt", "a")) == NULL) {
				printf("File could not be opened\n");
				return -1;
			}
			else
			{
				fprintf(fptr, "Game Records\n");
				fprintf(fptr, "Score:\n");
				fprintf(fptr, "P1: %d & P2: %d\n", score_p1, score_p2);
				fprintf(fptr, "Obtained Item:\n");
				fprintf(fptr, "P1: %d & P2: %d\n", item_p1a, item_p2a);
				fprintf(fptr, "Used Item:\n");
				fprintf(fptr, "P1: %d & P2: %d\n", used_p1, used_p2);


			}
			fclose(fptr);

			Sleep(1000);
			while (kbhit()) getch();
			key = getch();
			reset();
		}
	}
}

void pause(void) {
	int i, j;

	int x = 5;
	int y = 5;

	for (i = 1; i<MAIN_X - 2; i++) {
		gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
		gotoxy(x, y + 1); printf("▤                              ▤");
		gotoxy(x, y + 2); printf("▤  +-----------------------+   ▤");
		gotoxy(x, y + 3); printf("▤  |       P A U S E       |   ▤");
		gotoxy(x, y + 4); printf("▤  +-----------------------+   ▤");
		gotoxy(x, y + 5); printf("▤  Press any key to resume..   ▤");
		gotoxy(x, y + 6); printf("▤                              ▤");
		gotoxy(x, y + 7); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
	}
	getch();

	system("cls");
	reset_main_cpy();
	draw_main();
	draw_map();

	for (i = 1; i<3; i++) {
		for (j = 0; j<4; j++) {
			if (blocks[b_type_next][0][i][j] == 1) {
				gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6);
				printf("■");
			}
			else {
				gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6);
				printf("  ");
			}
		}
	}
}