/* PURPOSE: Game operations of the tank game.
 * AUTHOR: Nadith Pathirage <<StudentID>>
 * DATE CREATED: 27/05/2021
 * DATE MODIFIED: 13/08/2022
 */

/* Standard Include */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* Local Includes */
#include "util.h"
#include "macros.h"
#include "gameops.h"
#include "validate.h"

/**************************************************************************************************/
/* Forward Declarations													    		      		  */
/**************************************************************************************************/
static int animate(RefreshMapParam* pRP, GameObj* pStCell);

/**************************************************************************************************/
/* Helper Methods												    		      				  */
/**************************************************************************************************/
/**
 * @brief Whether the player or enemy got hit by the bullet.
 * 
 * @param pBlow blow object ('X') coordinates.
 * @param pPlayer player object.
 * @return GameStatus hit game status (PLAYER_HIT, ENEMY_HIT) 
 */
static GameStatus getHitStatus(GameObj* pBlow, GameObj* pPlayer)
{
	GameStatus gameStatus = ENEMY_HIT;
	
	if ( pBlow->row == pPlayer->row &&
		  pBlow->col == pPlayer->col )
	{
		gameStatus = PLAYER_HIT;
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
	/* MENU: */
	printf("%c to go/face up\n", KEY_UP);
	printf("%c to go/face down\n", KEY_DOWN);
	printf("%c to go/face left\n", KEY_LEFT);
	printf("%c to go/face right\n", KEY_RIGHT);
	printf("%c to shoot laser\n", KEY_SHOOT);
	printf("%c to print log file\n", KEY_LOG);
	printf("action: ");
	scanf(" %c", pcUserInput);
	printf("\n");
}

/**************************************************************************************************/
/**
 * @brief Game loop. Once the wining or loosing condition are met, 
 * the loop will exit.
 * 
 * @param pRP parameter object to pass across functions.
 * @return GameStatus Game status other then PROGRESSING. 
 * Refer to macros.h for other game status.
 */
GameStatus mainLoop(RefreshMapParam* pRP)
{
	GameStatus gameStatus = PROGRESSING;
	char cUserInput;

	do 
	{
		readUserInput(&cUserInput);

		switch(cUserInput)
		{
			/* all movement actions: */
			case KEY_UP:
			case KEY_DOWN:
			case KEY_LEFT:
			case KEY_RIGHT:			
				gameStatus = turnOrMove(pRP, cUserInput);
				debugObj(pRP->pPlayer, "Player");

				if (gameStatus == PROGRESSING)
					refreshMap(pRP);
			break;
				
			case KEY_SHOOT:
				pRP->isStoreMap = TRUE;
				gameStatus = shoot(pRP);
			break;

			case KEY_LOG:
				gameStatus = save(pRP);
				refreshMap(pRP);
			break;

			default:
				/* invalid user input */
				printError("Invalid User Input\n\n");
				refreshMap(pRP);
			break;
		}		

	} while (gameStatus == PROGRESSING);
	/* if object is hit, terminates while loop */

	/* save the log after game ends */
	save(pRP);

	return gameStatus;
}

/**************************************************************************************************/
/**
 * @brief Process the game status and display "Win" or "Loose" message.
 * 
 * @param gameStatus game status to process.
 */
void processGameStatus(GameStatus gameStatus)
{
	if (gameStatus == ENEMY_HIT)
	{
		printf("%sYou Won! :D%s\n", LIGHT_GREEN, CLEAN);
	}
	else if (gameStatus == PLAYER_HIT)
	{
		printf("%sYou Lost! :(%s\n", BRIGHT_RED, CLEAN);
	}
	else if (gameStatus == SAVE_ERROR)
	{
		printf("Game Save Error!\n");
	}
}

/**************************************************************************************************/
/* Animation Related Methods														    	      */
/**************************************************************************************************/
/**
 * @brief The common code for animateUp and animateDown functions.
 * 
 * @param isUp whether up animation or down animation.
 * @param animateLoop animate loop current status (TRUE / FALSE).
 * @param pStCell start cell of the animation.
 * @param stCol start column of the map.
 * @param i current column index of the map.
 * @param pRP parameter object to pass across functions.
 * @param pGameStatus export variable to send out the game status.
 * @return int whether to continue animation loop or not.
 */
static int animateUpDownCore(int isUp, int animateLoop, GameObj* pStCell, 
								int stCol, int i, RefreshMapParam* pRP,
								GameStatus* pGameStatus)
{
	pRP->isStoreMap = TRUE;
	
	if (pRP->pMapInfo->map[i][stCol] == '\\')
	{		
		updateObj(pStCell, i, (isUp ? stCol-1 : stCol+1), (isUp ? DIR_LEFT:DIR_RIGHT));		
		*pGameStatus = animate(pRP, pStCell);
		animateLoop = FALSE;
	}
	else if (pRP->pMapInfo->map[i][stCol] == '/')
	{
		updateObj(pStCell, i, (!isUp ? stCol-1 : stCol+1), (!isUp ? DIR_LEFT:DIR_RIGHT));								
		*pGameStatus = animate(pRP, pStCell);
		animateLoop = FALSE;
	}
	else if (pRP->pMapInfo->map[i][stCol] == MARKER_BORDER)
	{	
		/* hitting border */
		pRP->isStoreMap = (pRP->pBullet != NULL);
		pRP->pBullet = NULL;
		pauseAndRefreshMap(pRP);		
	}
	else if (pRP->pMapInfo->map[i][stCol] != ' ')
	{	
		/* shot enemy or player */
		GameObj blow; updateObj(&blow, i, stCol, 'X');		
		pRP->pBullet = &blow;
		pauseAndRefreshMap(pRP);
		pRP->pBullet = NULL;
		
		*pGameStatus = getHitStatus(&blow, pRP->pPlayer);
		animateLoop = FALSE;
	}
	else
	{		
		/* Bullet placement */
		GameObj bullet; updateObj(&bullet, i, stCol, '|');
		pRP->pBullet = &bullet;
		pauseAndRefreshMap(pRP);
	}

	return animateLoop;
}

/**************************************************************************************************/
/**
 * @brief Animate the bullet from bottom to top.
 * 
 * @param pRP parameter object to pass across functions.
 * @param pStCell start cell of the animation.
 * @return GameStatus game status. Refer to macros.h for game status.
 */
static GameStatus animateUp(RefreshMapParam* pRP, GameObj* pStCell)
{	

	int i, animateLoop = TRUE;
	GameStatus gameStatus = PROGRESSING;
	int stRow = pStCell->row, stCol = pStCell->col;

	for(i = stRow; i >= 0 && animateLoop; i--)
	{		
		animateLoop = animateUpDownCore(TRUE, animateLoop, pStCell, 
													stCol, i, pRP, &gameStatus);
	}
	
	return gameStatus;
}

/**************************************************************************************************/
/**
 * @brief Animate the bullet from top to bottom.
 * 
 * @param pRP parameter object to pass across functions.
 * @param pStCell start cell of the animation.
 * @return GameStatus game status. Refer to macros.h for game status.
 */
static GameStatus animateDown(RefreshMapParam* pRP, GameObj* pStCell)
{	
	int i, animateLoop = TRUE;
	GameStatus gameStatus = PROGRESSING;
	MapInfo* pMapInfo = pRP->pMapInfo;
	int stRow = pStCell->row, stCol = pStCell->col;
	
	for (i = stRow ; i < pMapInfo->rows && animateLoop ; i++)
	{
		animateLoop = animateUpDownCore(FALSE, animateLoop, pStCell, 
													stCol, i, pRP, &gameStatus);
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
 * @param pRP parameter object (void* []) to pass across functions.
 * @param map the canvas.
 * @param pBullet bullet object (int [3]).
 * @param pPlayer player object (int [3]).
 * @param gameStatus export variable to send out the game status.
 * @return int whether to continue the animation loop or not.
 */

/**
 * @brief The common code for animateLeft and animateRight functions.
 * 
 * @param isLeft whether left animation or right animation.
 * @param animateLoop animate loop current status (TRUE / FALSE).
 * @param pStCell start cell of the animation.
 * @param stCol start column of the map.
 * @param i current column index of the map.
 * @param pRP parameter object to pass across functions.
 * @param pGameStatus export variable to send out the game status.
 * @return int whether to continue animation loop or not.
 */
static int animateLeftRightCore(int isLeft, int animateLoop, GameObj* pStCell, 
								int stRow, int i, RefreshMapParam* pRP,
								GameStatus* pGameStatus)
{
	pRP->isStoreMap = TRUE;
	
	if (pRP->pMapInfo->map[stRow][i] == '\\')
	{		
		updateObj(pStCell, (isLeft ? stRow-1 : stRow+1), i, (isLeft ? DIR_UP:DIR_DOWN));
		*pGameStatus = animate(pRP, pStCell);
		animateLoop = FALSE;
	}
	else if (pRP->pMapInfo->map[stRow][i] == '/')
	{
		updateObj(pStCell, (!isLeft ? stRow-1 : stRow+1), i, (!isLeft ? DIR_UP:DIR_DOWN));							
		*pGameStatus = animate(pRP, pStCell);
		animateLoop = FALSE;
	}
	else if (pRP->pMapInfo->map[stRow][i] == MARKER_BORDER)
	{	
		/* hitting border */
		pRP->isStoreMap = (pRP->pBullet != NULL);
		pRP->pBullet = NULL;
		pauseAndRefreshMap(pRP);		
	}
	else if (pRP->pMapInfo->map[stRow][i] != ' ')
	{
		/* shot enemy or player */
		GameObj blow; updateObj(&blow, stRow, i, 'X');
		pRP->pBullet = &blow;
		pauseAndRefreshMap(pRP);
		pRP->pBullet = NULL;	
		
		*pGameStatus = getHitStatus(&blow, pRP->pPlayer);
		animateLoop = FALSE;
	}
	else
	{
		/* Bullet placement */
		GameObj bullet; updateObj(&bullet, stRow, i, '-');
		pRP->pBullet = &bullet;
		pauseAndRefreshMap(pRP);
	}

	return animateLoop;
}

/**************************************************************************************************/
/**
 * @brief Animate the bullet from right to left.
 * 
 * @param pRP parameter object to pass across functions.
 * @param pStCell start cell of the animation.
 * @return GameStatus game status. Refer to macros.h for game status.
 */
static GameStatus animateLeft(RefreshMapParam* pRP, GameObj* pStCell)
{	
	int i, animateLoop = TRUE;
	GameStatus gameStatus = PROGRESSING;
	int stRow = pStCell->row, stCol = pStCell->col;
	
	for (i = stCol ; i >= 0 && animateLoop; i--)
	{
		animateLoop = animateLeftRightCore(TRUE, animateLoop, pStCell, 
												stRow, i, pRP, &gameStatus);
	}

	return gameStatus;
}

/**************************************************************************************************/
/**
 * @brief Animate the bullet from left to right.
 * 
 * @param pRP parameter object to pass across functions.
 * @param pStCell start cell of the animation.
 * @return GameStatus game status. Refer to macros.h for game status.
 */
static GameStatus animateRight(RefreshMapParam* pRP, GameObj* pStCell)
{

	int i, animateLoop = TRUE;
	GameStatus gameStatus = PROGRESSING;	
	MapInfo* pMapInfo = pRP->pMapInfo;
	int stRow = pStCell->row, stCol = pStCell->col;
		
	for (i = stCol ; i < pMapInfo->cols && animateLoop; i++)
	{
		animateLoop = animateLeftRightCore(FALSE, animateLoop, pStCell, 
												stRow, i, pRP, &gameStatus);
	}

	return gameStatus;
}

/**************************************************************************************************/
/**
 * @brief Animate the bullet depending on the direction.
 * 
 * @param pRP parameter object to pass across functions.
 * @param pStCell start cell of the animation.
 * @return GameStatus game status. Refer to macros.h for game status.
 */
static int animate(RefreshMapParam* pRP, GameObj* pStCell)
{
	GameStatus gameStatus = PROGRESSING;

	switch (pStCell->direction)
	{
		case DIR_UP:
			gameStatus = animateUp(pRP, pStCell);
		break;

		case DIR_DOWN:
			gameStatus = animateDown(pRP, pStCell);
		break;

		case DIR_LEFT:
			gameStatus = animateLeft(pRP, pStCell);
		break;

		case DIR_RIGHT:
			gameStatus = animateRight(pRP, pStCell);
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
 * @param pPlayer player object.
 * @param cUserInput user choice (char).
 */
static void performTurnOrMove(GameObj* pPlayer, char cUserInput)
{
	switch(cUserInput)
	{
		case 'w':
			if (pPlayer->direction != DIR_UP)
				pPlayer->direction = DIR_UP;
			else
				pPlayer->row = pPlayer->row - 1;	
		break;
			
		case 's':
			if (pPlayer->direction != DIR_DOWN)
				pPlayer->direction = DIR_DOWN;
			else
				pPlayer->row = pPlayer->row  + 1;
		break;
			
		case 'a':
			if (pPlayer->direction != DIR_LEFT)
				pPlayer->direction =  DIR_LEFT;
			else 
				pPlayer->col = pPlayer->col - 1;
		break;
			
		case 'd':
			if (pPlayer->direction != DIR_RIGHT)
				pPlayer->direction =  DIR_RIGHT; 
			else 
				pPlayer->col = pPlayer->col + 1;			
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
 * @param pRP parameter object to pass across functions.
 * @param cUserInput user choice (char).
 * @return GameStatus game status. Refer to macros.h for game status.
 */
GameStatus turnOrMove(RefreshMapParam* pRP, char cUserInput)
{		
	GameStatus gameStatus = PROGRESSING;
	GameObj newPlayer = *(pRP->pPlayer);
	GameObj stCell;

	int validPosition = FALSE;
	int hasChangedDirection = FALSE;
	int enemyCanShoot = FALSE;	
	
	/* Update the direction of the player */
	performTurnOrMove(&newPlayer, cUserInput);



	/* New position (aiPlayerNew) may be just a change of direction */
	hasChangedDirection = (pRP->pPlayer->direction != newPlayer.direction);

	/* New position (aiPlayerNew) is within the bounds & not overlap enemy and mirrors */
	validPosition = validateObjBounds(pRP->pMapInfo, &newPlayer, NULL) &&
						!isObjOverlap(&newPlayer, pRP->pEnemy) &&
						pRP->pMapInfo->map[newPlayer.row][newPlayer.col] == ' ';	

	if (hasChangedDirection || validPosition)
	{
		/* Update player obj with new player obj */
		*(pRP->pPlayer) = newPlayer;
		pRP->isStoreMap = TRUE;

		/* PERF: Check enemy can shoot, only if player has moved to a new cell */
		if (!hasChangedDirection && validPosition)
			enemyCanShoot = isFacingPlayer(pRP->pEnemy, pRP->pPlayer, &stCell, pRP->pMapInfo);
	}	
	

	if (enemyCanShoot)
	{
		refreshMap(pRP);
		gameStatus = animate(pRP, &stCell);
	}
	
	return gameStatus;
}

/**************************************************************************************************/
/**
 * @brief Shoot a bullet from the player. For shooting a bullet from the enemy
 * refer to `turnOrMove()` method, `enemyCanShoot` boolean.
 * 
 * @param pRP parameter object to pass across functions.
 * @return GameStatus game status. Refer to macros.h for game status.
 */
GameStatus shoot(RefreshMapParam* pRP)
{
	GameObj stCell = *(pRP->pPlayer);

	switch(stCell.direction)
	{
		case DIR_UP:
			stCell.row -= 1;
		break;
			
		case DIR_DOWN:
			stCell.row += 1;
		break;
			
		case DIR_LEFT:
			stCell.col -= 1;
		break;
			
		case DIR_RIGHT:
			stCell.col += 1;
		break;
	}	

	return animate(pRP, &stCell);
}

/**************************************************************************************************/
/* Game Activities - Log Saving										    		      			  */
/**************************************************************************************************/
/**
 * @brief Write the node data to the output file.
 * 
 * @param pData data of the node in log linked list.
 */
void saveNodeLog(void* pData)
{	
	RefreshMapParam oRP;
	NodeData* pNodeData = (NodeData*) pData;
	fprintf(pNodeData->pLogFile->fptr, 
			"---------------------------------------------------------------\n");
	packRefreshParams(&oRP, pNodeData->pMapInfo, 
						NULL, NULL, NULL,
						NULL, NULL, pNodeData->pLogFile, FALSE);
	printAndStoreMap(&oRP);
}

/**************************************************************************************************/
/**
 * @brief Save log linked list to the output file.
 * 
 * @param pRP parameter object to pass across functions. 
 * @return GameStatus game status. Refer to macros.h for game status.
 */
GameStatus save(RefreshMapParam* pRP)
{
	GameStatus gameStatus = PROGRESSING;
	
	if (pRP->pLogList->len > 0)
	{
		assert(pRP->pLogFile->fptr == NULL);
		
		pRP->pLogFile->fptr = fopen(pRP->pLogFile->zFileName, "w");
		if ( !(pRP->pLogFile->fptr) )
		{
			perror("Could not open file");
		}
		else
		{
			printLinkedList(pRP->pLogList, &saveNodeLog);		
			fclose(pRP->pLogFile->fptr);
			pRP->pLogFile->fptr = NULL;
		}
	}
	else
		printf("There is nothing to save");
	
	return gameStatus;
}