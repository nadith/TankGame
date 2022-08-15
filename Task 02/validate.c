/* PURPOSE: Various validation methods of the Tank Game.
 * AUTHOR: Nadith Pathirage <<StudentID>>
 * DATE CREATED: 27/05/2021
 * DATE MODIFIED: 13/08/2022
 */

/* Standard Include */
#include <stdio.h>

/* Local Includes */
#include "util.h"
#include "macros.h"

/**************************************************************************************************/
/* Helper Methods												    		      				  */
/**************************************************************************************************/

static int mirrorExistsBetween(GameObj* pEnemy, GameObj* pPlayer, GameObj* pStCell, 
																MapInfo* pMapInfo, int st, int en)
{	
	int mirrorExists = FALSE;
	int swapNeeded = (st > en);
	int temp = st;
	st = swapNeeded ? en : st;
	en = swapNeeded ? temp : en;

	while ((!mirrorExists) && (st <= en))
	{
		mirrorExists = (
						((pStCell->direction == DIR_UP || pStCell->direction == DIR_DOWN) && 
							pMapInfo->map[st][pStCell->col] != ' ') 
						||
						((pStCell->direction == DIR_RIGHT || pStCell->direction == DIR_LEFT) && 
						pMapInfo->map[pStCell->row][st] != ' ')
						);
		st++;
	}

	return mirrorExists;
}

/**
 * @brief Core logic to check wheter the enemy is facing the player.
 * 
 * @param pEnemy enemy object (int [3]).
 * @param pPlayer player object (int [3]).
 * @param pBulletStCell if facing = TRUE, start cell of the bullet.
 * @return int whether enemy is facing or not.
 */
static int isFacingPlayerCore(GameObj* pEnemy, GameObj* pPlayer, GameObj* pBulletStCell, MapInfo* pMapInfo)
{
	int st = -1, en = -1;
	int isFacing = FALSE;

	/* Check based on enemy's direction */
	switch (pEnemy->direction)
	{
		case DIR_UP: /* if same col && enemy below the player */
			if ((pEnemy->col == pPlayer->col) && 
                (pEnemy->row > pPlayer->row))
			{
				st = pPlayer->row + 1;
				en = pEnemy->row - 1;
				pBulletStCell->row = en;
			}
			break;
			
		case DIR_DOWN: /* if same row && enemy above player */
			if ((pEnemy->col == pPlayer->col) && 
                (pEnemy->row < pPlayer->row))
			{
				st = pEnemy->row + 1;
				en = pPlayer->row - 1;
                pBulletStCell->row = st;
			}
			break;
			
		case DIR_LEFT: /* if same row && enemy after player */
            if ((pEnemy->row == pPlayer->row) && 
                (pEnemy->col > pPlayer->col))
			{
				st = pPlayer->col + 1;
				en = pEnemy->col - 1;
                pBulletStCell->col = en;
			}
			break;
			
		case DIR_RIGHT: /* if same row && enemy is before player */
            if ((pEnemy->row == pPlayer->row) && 
                (pEnemy->col < pPlayer->col))
			{
				st = pEnemy->col + 1;
				en = pPlayer->col - 1;
				pBulletStCell->col = st;
			}
			break;
	}

	if (st != -1)    
		isFacing = TRUE;

	isFacing = isFacing &&
				!mirrorExistsBetween(pEnemy, pPlayer, pBulletStCell, pMapInfo, st, en);
				/* Check whether mirros exists between player and enemy */

	return isFacing;
}

/**************************************************************************************************/
/**
 * @brief Check wheter the enemy is facing the player. Typically checked, after
 * the player moves to a new cell.
 * 
 * @param pEnemy enemy object (int [3]).
 * @param pPlayer player object (int [3]).
 * @param pBulletStCell if facing = TRUE, start cell of the bullet.
 * @return int whether enemy is facing or not.
 */
int isFacingPlayer(GameObj* pEnemy, GameObj* pPlayer, GameObj* pBulletStCell, MapInfo* pMapInfo)
{
	GameObj bulletStCell;
	
	if (pBulletStCell == NULL)	
		pBulletStCell = &bulletStCell;

	*pBulletStCell = *pEnemy;

    return isFacingPlayerCore(pEnemy, pPlayer, pBulletStCell, pMapInfo);
}

/**************************************************************************************************/
/**
 * @brief Whether object1 overlaps another object (enemy, player, bullet, etc).
 * 
 * @param pObj1 object1 (int [3]).
 * @param pObj2 object2 (int [3]).
 * @return int overlapping status.
 */
int isObjOverlap(GameObj* pObj1, GameObj* pObj2)
{
    return (pObj1->row == pObj2->row && pObj1->col == pObj2->col);
}

/**************************************************************************************************/
/* Validation Methods											    		      				  */
/**************************************************************************************************/
/**
 * @brief Validate the object (enemy, player, bullet) direction.
 * 
 * @param direction direction of the object.
 * @return int validation status.
 */
int validateDirection(char direction)
{    
    return (direction == DIR_RIGHT || direction == DIR_LEFT 
				|| direction == DIR_UP || direction == DIR_DOWN);
}

/**************************************************************************************************/
/**
 * @brief Validate object (enemy, player, bullet) placement is within in the 
 * bounds of the canvas.
 * 
 * @param pMapSize size of the map
 * @param pObj object to be validated.
 * @param zMsg message to display if invalid.
 * @return int validation status.
 */
int validateObjBounds(MapInfo* pMapInfo, GameObj* pObject, char* zMsg)
{    
	/* validity in the rows direction (y-direction) */
    int valid = BETWEEN(1, pMapInfo->rows-2, pObject->row);

	/* validity in the cols direction (x-direction) */
    valid = valid && BETWEEN(1, pMapInfo->cols-2, pObject->col);

    if (!valid && zMsg != NULL)
        printError(zMsg);
    
    return valid;
}

/**************************************************************************************************/
/**
 * @brief Validate the enemy and player tanks. Are they facing each other?, do 
 * they overlap each other?
 * 
 * @param pEnemy enemy object (int [3]).
 * @param pPlayer player object (int [3]).
 * @return int validation status.
 */
int validateTanks(MapInfo* pMapInfo, GameObj* pEnemy, GameObj* pPlayer)
{
	int isValid = TRUE;

	/* checks if player is placed in front of enemy -> insant lose */
	if (isObjOverlap(pEnemy, pPlayer))
	{
		printError("Player and enemy cannot start on the same cell.\n");
		isValid = FALSE;
	}
	else if (isFacingPlayer(pEnemy, pPlayer, NULL, pMapInfo))
	{
		printError("Cannot place tanks in instant lose position.\n");
		isValid = FALSE;
	}

	return isValid;
}


int validateMirror(GameObj* pMirror, MapInfo* pMapInfo,
 							GameObj* pEnemy, GameObj* pPlayer)
{
	int isValid = TRUE;
	
	if( !BETWEEN(0, pMapInfo->rows, pMirror->row) || 
		!BETWEEN(0, pMapInfo->cols, pMirror->col) )
	{
		printError("Mirror must be placed inside bounds\n");
		isValid = FALSE;
	}	
	else if (isObjOverlap(pMirror, pPlayer))
	{
		printError("Mirror must not overlap player\n");
		isValid = FALSE;
	}
	else if (isObjOverlap(pMirror, pEnemy))
	{
		printError("Mirror must not overlap enemy\n");
		isValid = FALSE;
	}
	
	return isValid;
}