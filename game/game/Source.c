#include "Conlib.h"
#include <stdio.h>
#include <malloc.h>
#include <conio.h>
#include <math.h>
#include <time.h>

int score, life, jump, motion, difficulty;

typedef struct Hero
{
	int fx;
	int bx;
	int uy;
	int dy;

}Hero;

typedef struct Barr
{
	int fx;
	int bx;
	int uy;
	int dy;
	struct Barr *next;
	
}Barr;

typedef struct Area
{
	int h;
	int w;

}Area;


void delay(unsigned int mseconds)
{
	clock_t goal = mseconds/motion + clock();
	while (goal > clock());
}


void save_to_file(Area*stad, Hero*hero, Barr*barr)
{
	FILE*file;
	fopen_s(&file, "save.txt" , " w");

	/*if (file == NULL)
	{
	return NULL;
	}*/
	fprintf(file, "%d %d ", stad->h, stad->w);
	fprintf(file, " %d %d %d %d ", hero->fx, hero->bx, hero->uy, hero->dy);
	fprintf(file, " %d %d %d %d %d ", motion, jump, life, score, difficulty);
	while (barr != NULL)
	{
		fprintf(file, " %d %d %d %d", barr->fx, barr->bx, barr->uy, barr->dy);
		barr = barr->next;
	}
	fclose(file);

}
Barr* contin(Area*stad, Hero*hero, Barr*barr)
{
	FILE*file;
	fopen_s(&file, "save.txt", " r+");

	/*if (file == NULL)
	{
	return NULL;
	}*/
	
	
	fscanf_s(file, "%d %d", &(stad->h), &(stad->w));
	fscanf_s(file, "%d %d %d %d", &(hero->fx), &(hero->bx), &(hero->uy), &(hero->dy));
	fscanf_s(file, "%d %d %d %d %d", &motion, &jump, &life, &score, &difficulty);
	while (!feof(file))
	{
		Barr* fbarr = (Barr*)malloc(sizeof(Barr));;
		fscanf_s(file, "%d %d %d %d", &(fbarr->fx), &(fbarr->bx), &(fbarr->uy), &(fbarr->dy));
		fbarr->next = barr;
		barr = fbarr;
	}
	
	fclose(file);	
	return barr;
}


Barr *add_barr(Barr *barr, Area*stad)
{
	Barr* fbarr = barr;

	if (barr == NULL)
	{
		barr = (Barr*)malloc(sizeof(Barr));
		barr->fx = stad->w - 4;
		barr->bx = stad->w - 3;
		barr->uy = 23;
		barr->dy = 24;
		barr->next = NULL;
	}
	else
	{
		while (fbarr->next != NULL)
		{
			fbarr = fbarr->next;
		}
		fbarr->next = (Barr*)malloc(sizeof(Barr));
		fbarr->next->fx = stad->w - 4;
		fbarr->next->bx = stad->w - 3;
		fbarr->next->uy = 23;
		fbarr->next->dy = 24;
		fbarr->next->next = NULL;
	}
	return barr;
}
Barr* clean(Barr* h)
{
	Barr*p = NULL;
	while (h != NULL)
	{
		p = h;
		h = h->next;
		free(p);
	}
	free(h);
	return NULL;
}


void setup(Area*stad, Hero*hero/*, Barr*barr*/)
{
	
	stad->h = 25;
	stad->w = 100;

	hero->fx = stad->w/4;
	hero->bx = stad->w/ 4 + 4;
	hero->uy = stad->h - 4;
	hero->dy = stad->h - 1;
	
	
	//barr->fx = stad->w - 4;
	//barr->bx = barr->fx + 1;
	//barr->uy = 23;
	//barr->dy = 24;
	
	motion = 1000;
	jump = 10;
	life = 1;
	score = 0;
	difficulty = 30;
}
Barr*draw(Area*stad, Hero*hero, Barr*barr)
{
	ClearConsole();
	GotoXY(0, 25);
	for (int i = 0; i <= stad->w; i++)
		printf("#");
	printf("\n");
	for (int i = 0; i < stad->h; i++)
	{
		for (int j = 0; j <= stad->w; j++)
		{

			if (j == 0 || j == stad->w)
			{
				printf("#");
			}
			if (i >= hero->uy && i <= hero->dy && j >= hero->fx && j <= hero->bx)
			{

				SetColor(11);
				printf("@");
				SetColor(14);
			}
			else
			{
				printf(" ");
			}
			Barr* fbarr = barr;
			while (fbarr != NULL)
			{
				if ((i >= fbarr->uy && i <= fbarr->dy) && (j >= fbarr->fx && j <= fbarr->bx))
				{
					SetColor(12);
					printf("[");
					SetColor(14);
				}
				fbarr = fbarr->next;
			}
			
			
		}
		printf("\n");
	}
		

	for (int i = 0; i <= stad->w; i++)
		printf("#");
	printf("\n");
	return barr;
}
Barr*logic(Area*stad, Hero*hero, Barr*barr)
{
	srand(time(NULL));
	if (KeyPressed() != 0)
	{
		switch (GetKey())
		{
		case(-72):
			if (hero->dy == stad->h)
			{
				hero->uy = hero->uy - jump;
				hero->dy = hero->dy - jump;
			}
			break;
			case(32):
				while (GetKey() != 32)
				{
					delay(1);
				}
			break;
		//case(-80):
		//	/*if (hero->dy == stad->h)*/
		//	/*{*/
		//		hero->uy = hero->uy + jump;
		//		hero->dy = hero->dy + jump;
		//	/*}*/
		//	break;
		case(-77):
			{
			motion = motion * 100;
			}
			break;
		case(113):
		{
			save_to_file(stad, hero, barr);
			printf("This session has been saved");
			life = 0;
			return 0;
		}
		}
	}
	if (hero->dy >= stad->h - 1)
	{
		hero->dy = stad->h - 1;
	}
	if (hero->uy >= stad->h - 4)
	{
		hero->uy = stad->h - 4;
	}
	if (hero->uy <= stad->h - 1 - jump)
	{
		hero->uy = stad->h - 1 - jump;
	}
	Barr* fbarr = barr;
	while (fbarr != NULL)
	{

		if ((hero->bx >= fbarr->fx && hero->fx <= fbarr->bx) && hero->dy >= fbarr->uy)
		{
			life--;
		}
		fbarr = fbarr->next;
	}
	if (barr == NULL)
	{
		barr = add_barr(barr, stad);
	}
	if (barr->fx <= 0 && barr->bx == 0)
	{
		Barr * temp = barr;
		barr = barr->next;
		free(temp);
	}
	if ((score+1) % difficulty == 0)
	{
		int decide;
		decide = rand()%2;
		if (decide == 1)
		{
			barr = add_barr(barr, stad);
		}
	}
	return barr;
}
Barr*back(Area*stad, Hero*hero, Barr*barr)
{
	Barr* fbarr = barr;
	while (fbarr != NULL)
	{
		fbarr->fx--;
		fbarr->bx--;
		fbarr = fbarr->next;
	}
	hero->uy++;
	hero->dy++;
	score++;
	/*motion = motion + score * 10;*/
	//difficulty = difficulty - score /1000;
	return barr;
}


void play()
{
	printf("\nSTART NEW GAME - PRESS(N)\n");
	printf("\nCONTINUE FROM SAVE - PRESS(C)\n");
	Area area;
	Hero hero;
	Barr *barr = NULL;


	switch (GetKey())
	{
	case(110):
	{
		int again = -72;
		ConShowCursor(0);
		while (again == -72)
		{
			setup(&area, &hero/*, barr*/);
			barr = add_barr(barr, &area);
			while (life != 0)
			{
				barr = logic(&area, &hero, barr);
				barr = draw(&area, &hero, barr);
				barr = back(&area, &hero, barr);
				printf("\nScore: %d", score);
				printf("\nPush: Space - for pause, Up - for jump, Q - for quit with save, another key - just for quiet");
				delay(1);

			}

			ClearConsole();
			barr = clean(barr);
			GotoXY(0, 0);
			printf("\nBEST SCORE: %d\n", score);
			printf("\nEnter Up for repeat\n");

			again = GetKey();
			setup(&area, &hero);
		}
		break;
	}

	case(99):
	{
		int again = -72;
		ConShowCursor(0);
		barr = contin(&area, &hero, barr);
		while (again == -72)
		{
			while (life != 0)
			{
				barr = logic(&area, &hero, barr);
				barr = draw(&area, &hero, barr);
				barr = back(&area, &hero, barr);
				printf("\nScore: %d", score);
				printf("\nPush: Space - for pause, Up - for jump, Q - for quit with save, another key - just for quiet");
				delay(1);

			}

			ClearConsole();
			barr = clean(barr);
			GotoXY(0, 0);
			printf("\nBEST SCORE: %d\n", score);
			printf("\nEnter Up for repeat\n");

			again = GetKey();
			setup(&area, &hero);
		}
		break;
	}
	/*int again = -72;
	ConShowCursor(0);
	while (again == -72)
	{
		barr = add_barr(barr, &area);
		while (life != 0)
		{
			barr = logic(&area, &hero, barr);
			barr = draw(&area, &hero, barr);
			barr = back(&area, &hero, barr);
			printf("\nScore: %d", score);
			printf("\nPush: Space - for pause, Up - for jump, Q - for quit with save, another key - just for quiet");
			delay(1);

		}

		ClearConsole();
		barr = clean(barr);
		GotoXY(0, 0);
		printf("\nBEST SCORE: %d\n", score);
		printf("\nEnter Up for repeat\n");

		again = GetKey();
		setup(&area, &hero);
	}*/

	}
}

int main()
{
	play();
	/*while (1)
	{
		printf("%d", GetKey());
	}*/
	system("pause");
	/*return 0;*/
}




