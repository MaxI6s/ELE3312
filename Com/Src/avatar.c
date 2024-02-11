#include "avatar.h"
#include "MCUFRIEND_kbv.h"
#include "stdio.h"
#include "stdlib.h"
#include "main.h"

void initGame(struct Avatar *avatar, struct Obstacle *obstacle){
	avatar->posy = AVATAR_POSY_START;
	avatar->previous_posy = AVATAR_POSY_START;

	avatar->width = AVATAR_POSX;
	avatar->posx = AVATAR_WIDTH;
	avatar->length = AVATAR_LENGTH;
	avatar->color = WHITE;
	
	obstacle->width = 30;
	obstacle->counter = 0;
	obstacle->posx = 0;
	obstacle->posy = 0;
	
	LCD_Begin();
	HAL_Delay(20);
	LCD_SetRotation(1);
	LCD_FillScreen(COLOR_BACKGROUND);
	
	drawAvatar(avatar);
}

void drawAvatar(struct Avatar *avatar){
	int posy = avatar->posy;
	int previous_posy = avatar->previous_posy;
	
	int posx = avatar->posx;
	int width = avatar->width;
	int length = avatar->length;
	int color = avatar->color;
	int step = avatar->step;
	
	//LCD_FillScreen(COLOR_BACKGROUND);
	
	// DELETE PREVIOUS
	LCD_FillRect(posx, previous_posy, width, length, COLOR_BACKGROUND);
	
	if(step%2 == 0){
	
	// PRINT NEW
	//Queue
	LCD_DrawLine(posx,posy+15,posx,posy+21,color);
	LCD_DrawLine(posx+1,posy+18,posx+1,posy+22,color);
	LCD_DrawLine(posx+2,posy+20,posx+2,posy+23,color);
	LCD_DrawLine(posx+3,posy+21,posx+3,posy+23,color);
	LCD_DrawLine(posx+4,posy+22,posx+4,posy+24,color);
	LCD_DrawLine(posx+5,posy+22,posx+5,posy+24,color);
	
	//Corps
	LCD_DrawLine(posx+6,posy+20,posx+6,posy+23,color);
	LCD_DrawLine(posx+7,posy+18,posx+7,posy+24,color);
	LCD_DrawLine(posx+8,posy+17,posx+8,posy+24,color);
	LCD_DrawLine(posx+9,posy+16,posx+9,posy+29,color);
	LCD_DrawLine(posx+10,posy+15,posx+10,posy+29,color);
	LCD_DrawLine(posx+11,posy+15,posx+11,posy+26,color);
	LCD_DrawLine(posx+12,posy+14,posx+12,posy+25,color);
	LCD_DrawLine(posx+13,posy+13,posx+13,posy+23,color);
	LCD_DrawLine(posx+14,posy+12,posx+14,posy+23,color);
	LCD_DrawLine(posx+15,posy+1,posx+15,posy+23,color);
	LCD_DrawLine(posx+16,posy+1,posx+16,posy+25,color);
	LCD_DrawLine(posx+17,posy+0,posx+17,posy+25,color);
	LCD_DrawLine(posx+18,posy+0,posx+18,posy+26,color);
	LCD_DrawLine(posx+19,posy+0,posx+19,posy+28,color);
	LCD_DrawLine(posx+20,posy+0,posx+20,posy+28,color);

	// Tête
	LCD_DrawLine(posx+19,posy+3,posx+19,posy+4,COLOR_BACKGROUND); //oeil
	LCD_DrawLine(posx+20,posy+3,posx+20,posy+4,COLOR_BACKGROUND); //oeil
	LCD_DrawLine(posx+21,posy+0,posx+21,posy+20,color);
	LCD_DrawLine(posx+22,posy+0,posx+22,posy+10,color);
	LCD_DrawLine(posx+23,posy+0,posx+23,posy+10,color);
	LCD_DrawLine(posx+24,posy+0,posx+24,posy+7,color);
	LCD_DrawLine(posx+25,posy+0,posx+25,posy+7,color);
	LCD_DrawLine(posx+26,posy+0,posx+26,posy+7,color);
	LCD_DrawLine(posx+27,posy+1,posx+27,posy+7,color);
	LCD_DrawLine(posx+28,posy+1,posx+28,posy+7,color);
	LCD_DrawLine(posx+29,posy+1,posx+29,posy+7,color);
	
	// Bouche
	LCD_DrawLine(posx+24,posy+9,posx+24,posy+10,color);
	LCD_DrawLine(posx+25,posy+9,posx+25,posy+10,color);
	LCD_DrawLine(posx+26,posy+9,posx+26,posy+10,color);
	LCD_DrawLine(posx+27,posy+9,posx+27,posy+10,color);
	
	//Pied
	LCD_DrawLine(posx+11,posy+28,posx+11,posy+29,color);
	LCD_DrawLine(posx+12,posy+28,posx+12,posy+29,color);
	LCD_DrawLine(posx+21,posy+27,posx+19,posy+28,color);
	LCD_DrawLine(posx+22,posy+27,posx+20,posy+28,color);
	
	//Bras
	LCD_DrawLine(posx+22,posy+15,posx+22,posy+16,color);
	LCD_DrawLine(posx+23,posy+15,posx+23,posy+17,color);
	LCD_DrawLine(posx+24,posy+15,posx+24,posy+17,color);
	}
	else {
	
	// PRINT NEW
	//Queue

	LCD_DrawLine(posx+2,posy+18,posx+2,posy+21,color);
	LCD_DrawLine(posx+3,posy+15,posx+3,posy+22,color);
	LCD_DrawLine(posx+4,posy+14,posx+4,posy+23,color);
	LCD_DrawLine(posx+5,posy+13,posx+5,posy+15,color);
	LCD_DrawLine(posx+5,posy+19,posx+5,posy+23,color);
	LCD_DrawLine(posx+6,posy+20,posx+6,posy+23,color);
	LCD_DrawLine(posx+6,posy+12,posx+6,posy+12,color);
	
	//Corps
	LCD_DrawLine(posx+7,posy+18,posx+7,posy+24,color);
	LCD_DrawLine(posx+8,posy+17,posx+8,posy+24,color);
	LCD_DrawLine(posx+9,posy+16,posx+9,posy+26,color);
	LCD_DrawLine(posx+10,posy+15,posx+10,posy+27,color);
	LCD_DrawLine(posx+11,posy+15,posx+11,posy+28,color);
	LCD_DrawLine(posx+12,posy+14,posx+12,posy+25,color);
	LCD_DrawLine(posx+13,posy+13,posx+13,posy+23,color);
	LCD_DrawLine(posx+14,posy+12,posx+14,posy+23,color);
	LCD_DrawLine(posx+15,posy+1,posx+15,posy+25,color);
	LCD_DrawLine(posx+16,posy+1,posx+16,posy+27,color);
	LCD_DrawLine(posx+17,posy+0,posx+17,posy+29,color);
	LCD_DrawLine(posx+18,posy+0,posx+18,posy+29,color);
	LCD_DrawLine(posx+19,posy+0,posx+19,posy+24,color);
	LCD_DrawLine(posx+20,posy+0,posx+20,posy+23,color);

	// Tête
	LCD_DrawLine(posx+19,posy+3,posx+19,posy+4,COLOR_BACKGROUND); //oeil
	LCD_DrawLine(posx+20,posy+3,posx+20,posy+4,COLOR_BACKGROUND); //oeil
	LCD_DrawLine(posx+21,posy+0,posx+21,posy+20,color);
	LCD_DrawLine(posx+22,posy+0,posx+22,posy+15,color);
	LCD_DrawLine(posx+23,posy+0,posx+23,posy+10,color);
	LCD_DrawLine(posx+24,posy+0,posx+24,posy+7,color);
	LCD_DrawLine(posx+25,posy+0,posx+25,posy+7,color);
	LCD_DrawLine(posx+26,posy+0,posx+26,posy+7,color);
	LCD_DrawLine(posx+27,posy+1,posx+27,posy+7,color);
	LCD_DrawLine(posx+28,posy+1,posx+28,posy+7,color);
	LCD_DrawLine(posx+29,posy+1,posx+29,posy+7,color);
	
	// Bouche
	LCD_DrawLine(posx+24,posy+9,posx+24,posy+10,color);
	LCD_DrawLine(posx+25,posy+9,posx+25,posy+10,color);
	LCD_DrawLine(posx+26,posy+9,posx+26,posy+10,color);
	LCD_DrawLine(posx+27,posy+9,posx+27,posy+10,color);
	
	//Pied
	LCD_DrawLine(posx+11,posy+28,posx+11,posy+29,color);
	LCD_DrawLine(posx+12,posy+28,posx+12,posy+29,color);
	LCD_DrawLine(posx+19,posy+28,posx+19,posy+29,color);
	LCD_DrawLine(posx+20,posy+28,posx+20,posy+29,color);
	
	//Bras
	LCD_DrawLine(posx+23,posy+14,posx+23,posy+16,color);
	LCD_DrawLine(posx+24,posy+14,posx+24,posy+16,color);
	}
}

void moveAvatar(struct Avatar *avatar){	
			//avatar->previous_posy = avatar->posy;
			//avatar->posy += 30;
			if(avatar->posy > 210)
				avatar->posy = 210;
			if(avatar->posy < 0)
				avatar->posy = 0;

		avatar->step++;
		drawAvatar(avatar);
}

void moveObstacle(struct Obstacle *obstacle){
		LCD_FillRect(obstacle->posx,obstacle->posy,obstacle->width, obstacle->length,BLACK);
		if (obstacle->counter <  0){	
			obstacle->counter = 0;
			if (obstacle->hand == 0) {
				obstacle->posx = 330;
				obstacle->counter = 330;
				obstacle->posy = obstacle->commande;
				obstacle->length = 10 + 10*(rand()%5+2);
				obstacle->width = 10 + 15*(rand()%5+2);
				switch (rand()%7+1) {
					case 1: obstacle->color=BLUE; break;
					case 2: obstacle->color=RED; break;
					case 3: obstacle->color=GREEN; break;
					case 4: obstacle->color=CYAN; break;
					case 5: obstacle->color=MAGENTA; break;
					case 6: obstacle->color=YELLOW; break;
					case 7: obstacle->color=ORANGE; break;
				}
			}
		}
		int avance = 20 + obstacle->score/20;
		obstacle->posx -= avance;
		LCD_FillRect(obstacle->posx,obstacle->posy,obstacle->width, obstacle->length,obstacle->color);
		
		obstacle->counter = obstacle-> posx + obstacle->width;
}

void drawIndicateurObstacle(struct Obstacle *obstacle){
	LCD_FillCircle(310, obstacle->last_commande, 4, BLACK);
	LCD_FillCircle(310, obstacle->commande, 4, RED);
}

int lookColision(struct Avatar *avatar, struct Obstacle *obstacle){
			if(obstacle->posx < 55 || obstacle->posx + obstacle->width < 55){
			if (avatar->posy >= obstacle->posy && avatar->posy < obstacle->posy+obstacle->length) {
				LCD_FillScreen(BLACK);
				char buffer[20] = {0};
				sprintf(buffer, "GAME OVER");
				LCD_SetCursor(45, 105);
				LCD_SetTextSize(4);
				LCD_SetTextColor(RED, BLACK);
				LCD_Printf(buffer);
				return 1;
			}
		}
		return 0;
}
