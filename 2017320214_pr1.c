#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
int RollingDice(void);
void drawDice(int _i, int _number);
int main(void)
{
	int choice, wcount = 0, lcount = 0;//win count, lose count
	int cash = 100000, bcash;//betting 하는 금액
	int csum, total;//csum이랑 total 비교하기 위해.
	int i, j, a;
	int udice[3], cdice[3], usum = 0;
	int win = 0, lose = 0;//stage 별 count
	do
	{
	main:

		printf("1. My state\n2. Dice Rolling Game\n3. End\n\nEnter your choice : ");
		scanf_s("%d", &choice);

		switch (choice)
		{
		case 1:
			system("cls");
			printf("Current cash : %d\n", cash);
			printf("Wins: %d\n", wcount);
			printf("Loses: %d\n", lcount);
			printf("\nType any key to go main menu...\n");
			switch (getch())
			{
			case 1:
				system("cls");
				goto main;
				break;
			default:
				system("cls");
				goto main;
				break;
			}
			break;
		case 2:
		Current:
			if (cash <= 0)
			{
				printf("\nNo cash! T");
				switch (getch())
				{
				case 1:
					system("cls");
					goto main;
					break;
				default:
					system("cls");
					goto main;
					break;
				}
				break;
			}
			system("cls");
			printf("Current cash : %d\n", cash);
			srand(time(NULL));
			csum = rand() % 9 + 6;
			printf("\nSum of computer's dices: %d\n", csum);
			do
			{
				printf("\nInput your betting: ");
				scanf_s("%d", &bcash);
				if (bcash > cash)
					printf("Betting money is larger than current cash.\n");
			} while (bcash>cash);

		Rollingscreen:
			system("cls");
			printf("Computer's dice total is %d\n", csum);
			srand(time(NULL));
			//csum에 맞게 세개의 숫자 배열
			do {
				usum = 0;
				for (i = 0; i <3; i++) {
					udice[i] = RollingDice();
					usum += udice[i];
				}
			} while (usum != csum);
			//빈주사위그리기
			for (i = 1; i <= 3; i++)
			{
				drawDice(1, 0);
			}
			printf("\n");
			for (j = 1; j <= 3; j++) {
				for (i = 1; i <= 3; i++) {
					drawDice(3, 0);
				}printf("\n");
			}
			for (i = 1; i <= 3; i++)drawDice(5, 0);
			printf("\n");
			//빈 주사위 그리기 끝

			//user dice 출력
			for (i = 1; i <= 5; i++)
			{
				for (j = 0; j < 3; j++)
				{
					drawDice(i, udice[j]);
				}
				printf("\n");
			}
			printf("Type ESC to Start Game\n");
			printf("Type s to Surrender\n");
			printf("Any key. Rolling dice again\n");

			switch (getch())
			{
			case 27:
				system("cls");//게임시작
				do {
					total = 0;
					for (i = 0; i <3; i++) {
						cdice[i] = RollingDice();
						total += cdice[i];
					}
				} while (total != csum);
				printf("Stage 1\n");
				//stage 1
				for (i = 1; i <= 5; i++)
				{
					drawDice(i, cdice[0]);
					printf("\n");
				}
				for (i = 1; i <= 5; i++)
				{
					drawDice(i, udice[0]);
					printf("\n");
				}
				if (cdice[0] < udice[0])
					printf("User Win"), win++;
				else
					printf("Computer Win"), lose++;
				Sleep(2000);
				system("cls");
				printf("Stage 2\n");
				for (i = 1; i <= 5; i++)
				{
					for (j = 0; j < 2; j++)
					{
						drawDice(i, cdice[j]);
					}
					printf("\n");
				}
				for (i = 1; i <= 5; i++)
				{
					for (j = 0; j < 2; j++)
					{
						drawDice(i, udice[j]);
					}
					printf("\n");
				}
				if (cdice[1] < udice[1])
					printf("User Win"), win++;
				else
					printf("Computer Win"), lose++;
				Sleep(2000);
				system("cls");
				printf("Stage 3\n");
				for (i = 1; i <= 5; i++)
				{
					for (j = 0; j < 3; j++)
					{
						drawDice(i, cdice[j]);
					}
					printf("\n");
				}
				for (i = 1; i <= 5; i++)
				{
					for (j = 0; j < 3; j++)
					{
						drawDice(i, udice[j]);
					}
					printf("\n");
				}
				if (cdice[2] < udice[2])
				{
					printf("User Win");
					win++;
				}

				else
				{
					printf("Computer Win");
					lose++;
				}
				Sleep(1000);
				printf("\nFinal winner: %s\n", win>lose ? "User!!" : "Computer!!");
				if (win > lose)
					wcount++, cash = cash + 2*bcash;
				else
					lcount++, cash = cash - bcash;
				printf("1. Retry\n2. Back to main menu\n");
				do
				{
					printf("Enter your choice:");
					scanf_s("%d", &a);
					if (a == 1)
						goto Current;
					if (a == 2)
					{
						system("cls");
						goto main;
					}
				} while (a != 1 || a != 2);//a가 1이나 2 아닐 때 다시 입력하라고 할 것.
				break;//game start 의 경우 끝.(case 27 끝.)
			case 's':
			case 'S':
				system("cls");
				cash = cash - bcash;
				goto Current;
				break;
			default:
				goto Rollingscreen;
				break;
			}
		case 3:
			return 0;
		default:;

		}
			
	} while (choice != 3);
	return 0;
}
int RollingDice(void) {

	return rand() % 6 + 1;
}

void drawDice(int _i, int _number)
{
	switch (_i)
	{
	case 1:
		printf("┌───┐");
		break;
	case 2:
		switch (_number)
		{
		case 0:
			printf
			("│      │");
			break;
		case 1:
		case 2:
			printf("│      │");
			break;
		case 3:
			printf
			("│    ●│");
			break;
		case 4:
		case 5:
		case 6:
			printf("│●  ●│");
			break;
		}
		break;
	case 3:
		switch (_number)
		{
		case 0:
			printf("│      │");
			break;
		case 1:
		case 3:
		case 5:
			printf("│  ●  │");
			break;
		case 2:
		case 6:
			printf("│●  ●│");
			break;
		case 4:
			printf("│      │");
			break;
		}
		break;
	case 4:
		switch (_number)
		{
		case 0:
			printf("│      │");
			break;
		case 1:
		case 2:
			printf("│      │");
			break;
		case 3:
			printf("│●    │");
			break;
		case 4:
		case 5:
		case 6:
			printf("│●  ●│");
			break;
		}
		break;
	case 5:
		printf("└───┘");
		break;
	}
}
