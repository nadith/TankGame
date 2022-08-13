/* PURPOSE: Game operations of the tank game.
 * AUTHOR: Nadith Pathirage <<StudentID>>
 * DATE CREATED: 27/05/2021
 * DATE MODIFIED: 13/08/2022
 */

/* Standard Include */
#include <stdio.h>
#include <stdlib.h>

/* Local Includes */
#include "map.h"
#include "util.h"
#include "macros.h"
#include "gameops.h"
#include "validate.h"

/**************************************************************************************************/
/* Forward Declarations													    		      		  */
/**************************************************************************************************/
static int animate(void* pRefreshParams[], int* pBulletStCell);

/**************************************************************************************************/
/* Helper Methods												    		      				  */
/**************************************************************************************************/
/**
 * @brief Whether the player or enemy got hit by the bullet.
 * 
 * @param pBlow blow object ('X') coordinates (int [3]).
 * @param pPlayer player object (int [3]).
 * @return int 
 */
static int getHitStatus(int* pBlow, int* pPlayer)
{
	int gameStatus = GS_ENEMY_HIT;
	
	if(pBlow[IDX_ROW] == pPlayer[IDX_ROW] &&
		  pBlow[IDX_COL] == pPlayer[IDX_COL])
	{
		gameStatus = GS_PLAYER_HIT;
	}
	
	return gameStatus;
}

/**************************************************************************************************/
/**
 * @brief Prompt the controls and read the user input.
 * 
 * @param pcUserInput user input (character).
 */
void readUserInput(char* pcUserInput)
{
	/* MENU:
	to add -> l to print log file */
	printf("w to go/face up\n");
	printf("s to go/face down\n");
	printf("a to go/face left\n");
	printf("d to go/face right\n");
	printf("f to shoot laser\n");
	/*printf("l to print log file\n");*/
	printf("action: ");
	scanf(" %c", pcUserInput);
	printf("\n");
}

/**************************************************************************************************/
/**
 * @brief Game loop. Once the wining or loosing condition are met, 
 * the loop will exit.
 * 
 * @param pRefreshParams parameter object (void* []) to pass across other functions.
 * @return int Game status other then GS_NONE_HIT. 
 * Refer to macros.h for other game status.
 */
int mainLoop(void* pRefreshParams[])
{
	int gameStatus = GS_NONE_HIT;
	char cUserInput;

	do 
	{
		readUserInput(&cUserInput);

		switch(cUserInput)
		{
			/* all movement actions: */
			case 'w':
			case 's':
			case 'a':
			case 'd':			
				gameStatus = turnOrMove(pRefreshParams, cUserInput);
				debugObj(pRefreshParams[IDX_RP_PLAYER], "Player");

				if(gameStatus == GS_NONE_HIT)
					refreshMap(pRefreshParams);
			break;
				
			case 'f':
				gameStatus = shoot(pRefreshParams);
			break;

			default:
				/* invalid user input */
				refreshMap(pRefreshParams);
			break;
		}		

	} while(gameStatus == GS_NONE_HIT);
	/* if object is hit, terminates while loop */

	return gameStatus;
}

/**************************************************************************************************/
/**
 * @brief Process the game status and display "Win" or "Loose" message.
 * 
 * @param gameStatus game status to process.
 */
void processGameStatus(int gameStatus)
{
	if(gameStatus == GS_ENEMY_HIT)
	{
		printf("%sYou Won! :D%s\n", LIGHT_GREEN, CLEAN);
	}
	else
	{
		printf("%sYou Lost! :(%s\n", BRIGHT_RED, CLEAN);
	}
}

/**************************************************************************************************/
/* Animation Related Methods														    	      */
/**************************************************************************************************/
/**
 * @brief The common code for animateUp and animateDown functions.
 * 
 * @param animateLoop animate loop current status (TRUE / FALSE).
 * @param stCol start column of the map.
 * @param i current column index of the map.
 * @param pRefreshParams parameter object (void* []) to pass across other functions.
 * @param map the canvas.
 * @param pBullet bullet object (int [3]).
 * @param pPlayer player object (int [3]).
 * @param gameStatus export variable to send out the game status.
 * @return int 
 */
static int animateUpDownCore(int animateLoop, int stCol, int i, void* pRefreshParams[], 
								char** map, int pBullet[], int pPlayer[], int* gameStatus)
{
	if(map[i][stCol] == '*')
	{	
		/* hitting border */
		pRefreshParams[IDX_RP_BULLET] = NULL;
		pauseAndRefreshMap(pRefreshParams);		
	}
	else if(map[i][stCol] != ' ')
	{	
		/* shot enemy or player */			
		updateObj(pBullet, i, stCol, 'X');
		pRefreshParams[IDX_RP_BULLET] = pBullet;
		pauseAndRefreshMap(pRefreshParams);
		pRefreshParams[IDX_RP_BULLET] = NULL;
		
		*gameStatus = getHitStatus(pBullet, pPlayer);
		animateLoop = FALSE;
	}
	else
	{		
		/* lazer placement */
		updateObj(pBullet, i, stCol, '|');
		pRefreshParams[IDX_RP_BULLET] = pBullet;
		pauseAndRefreshMap(pRefreshParams);	
	}

	return animateLoop;
}

/**************************************************************************************************/
/**
 * @brief Animate the bullet from bottom to top.
 * 
 * @param pRefreshParams parameter object (void* []) to pass across other functions.
 * @param pBulletStCell starting cell of the bullet animation.
 * @return int Game status. Refer to macros.h for game status.
 */
static int animateUp(void* pRefreshParams[], int* pBulletStCell)
{	
	int gameStatus = GS_NONE_HIT;	
	int i, animateLoop = TRUE, 
		stRow = pBulletStCell[IDX_ROW], stCol = pBulletStCell[IDX_COL];

	char** map;
	int *pMapSize, *pEnemy, *pPlayer, *pBullet;	
	unPackRefreshParams(pRefreshParams, &map, &pMapSize, &pEnemy, &pPlayer, &pBullet);
	
	for(i = stRow; i >= 0 && animateLoop; i--)
	{		
		animateLoop = animateUpDownCore(animateLoop, stCol, i, pRefreshParams, 
											map, pBullet, pPlayer, &gameStatus);
	}
	
	return gameStatus;
}

/**************************************************************************************************/
/**
 * @brief Animate the bullet from top to bottom.
 * 
 * @param pRefreshParams parameter object (void* []) to pass across other functions.
 * @param pBulletStCell starting cell of the bullet animation.
 * @return int Game status. Refer to macros.h for game status.
 */
static int animateDown(void* pRefreshParams[], int* pBulletStCell)
{	
	int gameStatus = GS_NONE_HIT;	
	int i, animateLoop = TRUE, 
		stRow = pBulletStCell[IDX_ROW], stCol = pBulletStCell[IDX_COL];

	char** map;
	int *pMapSize, *pEnemy, *pPlayer, *pBullet;	
	unPackRefreshParams(pRefreshParams, &map, &pMapSize, &pEnemy, &pPlayer, &pBullet);
	
	for(i = stRow; i < pMapSize[IDX_MAP_NROW] && animateLoop; i++)
	{	
		animateLoop = animateUpDownCore(animateLoop, stCol, i, pRefreshParams, 
											map, pBullet, pPlayer, &gameStatus);
	}

	return gameStatus;	
}

/**************************************************************************************************/
/**
 * @brief The common code for animateLeft and animateRight functions.
 * 
 * @param animateLoop animate loop current status (TRUE / FALSE).
 * @param stRow start row of the map.
 * @param i current column index of the map.
 * @param pRefreshParams parameter object (void* []) to pass across other functions.
 * @param map the canvas.
 * @param pBullet bullet object (int [3]).
 * @param pPlayer player object (int [3]).
 * @param gameStatus export variable to send out the game status.
 * @return int whether to continue the animation loop or not.
 */
static int animateLeftRightCore(int animateLoop, int stRow, int i, void* pRefreshParams[], 
								char** map, int pBullet[], int pPlayer[], int* gameStatus)
{
	if(map[stRow][i] == '*')
	{	
		/* hitting border */
		pRefreshParams[IDX_RP_BULLET] = NULL;
		pauseAndRefreshMap(pRefreshParams);			
	}
	else if(map[stRow][i] != ' ')
	{
		/* shot enemy or player */
		updateObj(pBullet, stRow, i, 'X');
		pRefreshParams[IDX_RP_BULLET] = pBullet;
		pauseAndRefreshMap(pRefreshParams);
		pRefreshParams[IDX_RP_BULLET] = NULL;	
		
		*gameStatus = getHitStatus(pBullet, pPlayer);
		animateLoop = FALSE;
	}
	else
	{
		/* lazer placement */
		updateObj(pBullet, stRow, i, '-');
		pRefreshParams[IDX_RP_BULLET] = pBullet;
		pauseAndRefreshMap(pRefreshParams);			
	}

	return animateLoop;
}

/**************************************************************************************************/
/**
 * @brief Animate the bullet from right to left.
 * 
 * @param pRefreshParams parameter object (void* []) to pass across other functions.
 * @param pBulletStCell starting cell of the bullet animation.
 * @return int Game status. Refer to macros.h for game status.
 */
static int animateLeft(void* pRefreshParams[], int* pBulletStCell)
{	
	int gameStatus = GS_NONE_HIT;	
	int i, animateLoop = TRUE, 
		stRow = pBulletStCell[IDX_ROW], stCol = pBulletStCell[IDX_COL];

	char** map;
	int *pMapSize, *pEnemy, *pPlayer, *pBullet;	
	unPackRefreshParams(pRefreshParams, &map, &pMapSize, &pEnemy, &pPlayer, &pBullet);
	
	for(i = stCol; i >= 0 && animateLoop; i--)
	{
		animateLoop = animateLeftRightCore(animateLoop, stRow, i, pRefreshParams, 
											map, pBullet, pPlayer, &gameStatus);
	}
	
	return gameStatus;
}

/**************************************************************************************************/
/**
 * @brief Animate the bullet from left to right.
 * 
 * @param pRefreshParams parameter object (void* []) to pass across other functions.
 * @param pBulletStCell starting cell of the bullet animation.
 * @return int game status. Refer to macros.h for game status.
 */
static int animateRight(void* pRefreshParams[], int* pBulletStCell)
{
	int gameStatus = GS_NONE_HIT;	
	int i, animateLoop = TRUE, 
		stRow = pBulletStCell[IDX_ROW], stCol = pBulletStCell[IDX_COL];

	char** map;
	int *pMapSize, *pEnemy, *pPlayer, *pBullet;	
	unPackRefreshParams(pRefreshParams, &map, &pMapSize, &pEnemy, &pPlayer, &pBullet);
		
	for(i = stCol; i < pMapSize[IDX_MAP_NCOL] && animateLoop; i++)
	{	
		animateLoop = animateLeftRightCore(animateLoop, stRow, i, pRefreshParams, 
											map, pBullet, pPlayer, &gameStatus);
	}
	
	return gameStatus;
}

/**************************************************************************************************/
/**
 * @brief Animate the bullet depending on the direction.
 * 
 * @param pRefreshParams parameter object (void* []) to pass across other functions.
 * @param pBulletStCell starting cell of the bullet animation.
 * @return int game status. Refer to macros.h for game status.
 */
static int animate(void* pRefreshParams[], int* pBulletStCell)
{
	int aiBullet[ARR_SIZE_OBJ] = {-1, -1, ' '};
	int gameStatus = GS_NONE_HIT;

	if ( !(pRefreshParams[IDX_RP_BULLET]) )
		pRefreshParams[IDX_RP_BULLET] = aiBullet;

	switch (pBulletStCell[IDX_DIR])
	{
		case 'u':
			gameStatus = animateUp(pRefreshParams, pBulletStCell);
		break;

		case 'd':
			gameStatus = animateDown(pRefreshParams, pBulletStCell);
		break;

		case 'l':
			gameStatus = animateLeft(pRefreshParams, pBulletStCell);
		break;

		case 'r':
			gameStatus = animateRight(pRefreshParams, pBulletStCell);
		break;
	}	
	
	return gameStatus;
}


/**************************************************************************************************/
/* Game Activities													    		      			  */
/**************************************************************************************************/
/**
 * @brief Proceed turn or move the player. First the player needs to turn when a 
 * key is pressed. Then the player will move on the next key press.
 * 
 * @param pPlayer player object (int [3]).
 * @param cUserInput user choice (char).
 */
static void performTurnOrMove(int* pPlayer, char cUserInput)
{
	switch(cUserInput)
	{
		case 'w':
			if (pPlayer[IDX_DIR] != 'u')
				pPlayer[IDX_DIR] = 'u';
			else
				pPlayer[IDX_ROW] = pPlayer[IDX_ROW] - 1;	
		break;
			
		case 's':
			if (pPlayer[IDX_DIR] != 'd')
				pPlayer[IDX_DIR] = 'd';
			else
				pPlayer[IDX_ROW] = pPlayer[IDX_ROW]  + 1;
		break;
			
		case 'a':
			if (pPlayer[IDX_DIR] != 'l')
				pPlayer[IDX_DIR] =  'l';
			else 
				pPlayer[IDX_COL] = pPlayer[IDX_COL] - 1;
		break;
			
		case 'd':
			if (pPlayer[IDX_DIR] != 'r')
				pPlayer[IDX_DIR] =  'r'; 
			else 
			pPlayer[IDX_COL] = pPlayer[IDX_COL] + 1;			
		break;
			
		default:
			printf("Invalid Direction!\n");
		break;
	}	
}

/**************************************************************************************************/
/**
 * @brief Turn or move the player. First the player needs to turn when a 
 * key is pressed. Then the player will move on the next key press. If the
 * player is moved to new cell, check whether enemy can shoot. If yes, shoot.
 * 
 * @param pRefreshParams parameter object (void* []) to pass across other functions. 
 * @param cUserInput user choice (char).
 * @return int game status. Refer to macros.h for game status.
 */
int turnOrMove(void* pRefreshParams[], char cUserInput)
{		
	int gameStatus = GS_NONE_HIT;
	int aiPlayerNew[ARR_SIZE_OBJ];
	int aiBulletStCell[ARR_SIZE_OBJ];

	int hasMoved = FALSE;
	int hasChangedDirection = FALSE;
	int enemyCanShoot = FALSE;	

	char** map;
	int *pMapSize, *pEnemy, *pPlayer, *pBullet;	
	unPackRefreshParams(pRefreshParams, &map, &pMapSize, &pEnemy, &pPlayer, &pBullet);	
	
	copyObj(aiPlayerNew, pPlayer);
	
	/* Update the direction of the player */
	performTurnOrMove(aiPlayerNew, cUserInput);

	/* New position (aiPlayerNew) is within the bounds & not overlap enemy */
	hasMoved = validateObjBounds(pMapSize, aiPlayerNew, NULL) &&
									!isObjOverlap(aiPlayerNew, pEnemy);

	/* New position (aiPlayerNew) may be just a change of direction */
	hasChangedDirection = (pPlayer[IDX_DIR] != aiPlayerNew[IDX_DIR]);

	if (hasMoved || hasChangedDirection)
	{
		/* Update player obj with new player obj */
		copyObj(pPlayer, aiPlayerNew);

		/* PERF: Check enemy can shoot, only if player has moved to a new cell */
		if (!hasChangedDirection && hasMoved)					
			enemyCanShoot = isFacingPlayer(pEnemy, pPlayer, aiBulletStCell);
	}	
	
	if(enemyCanShoot)
		gameStatus = animate(pRefreshParams, aiBulletStCell);
	
	return gameStatus;
}

/**************************************************************************************************/
/**
 * @brief Shoot a bullet from the player. For shooting a bullet from the enemy
 * refer to `turnOrMove()` method, `enemyCanShoot` boolean.
 * 
 * @param pRefreshParams parameter object (void* []) to pass across other functions. 
 * @return int game status. Refer to macros.h for game status.
 */
int shoot(void* pRefreshParams[])
{
	int* pPlayer = pRefreshParams[IDX_RP_PLAYER];

	int aiBulletStCell[ARR_SIZE_OBJ];	
	copyObj(aiBulletStCell, pPlayer);

	switch(aiBulletStCell[IDX_DIR])
	{
		case 'u':
			aiBulletStCell[IDX_ROW] -= 1;
		break;
			
		case 'd':
			aiBulletStCell[IDX_ROW] += 1;
		break;
			
		case 'l':
			aiBulletStCell[IDX_COL] -= 1;
		break;
			
		case 'r':
			aiBulletStCell[IDX_COL] += 1;
		break;
	}	

	return animate(pRefreshParams, aiBulletStCell);
}
