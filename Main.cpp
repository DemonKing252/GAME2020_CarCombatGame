#pragma once
#include "Game.h"
/**************************************
Project:		GAME2020_CarCombatGame
Author:			Liam Blake
Date Created:	Sept 9/2019
Date Modified:	Nov 24/2019
Phase:			Milestone 4
Lines of code:  3165
**************************************/

/*************************************
In this program you will find many comments that detail what features will be changed/implemented 
for the upcoming milestone assignments. If you see a feature in the concept document that is not
in the game yet, thats because it will be added later on.
*************************************/
int main(int args, char* arvs[])
{
	Game::Instance()->Run("GAME2020 Milestone 4", 
		SDL_WINDOWPOS_CENTERED,	SDL_WINDOWPOS_CENTERED, 1024, 768, 0);	
	return 0;
}