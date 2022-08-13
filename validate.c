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
#include "validate.h"

/**************************************************************************************************/
/* Helper Methods												    		      				  */
/**************************************************************************************************/
/**
 * @brief Core logic to check wheter the enemy is facing the player.
 * 
 * @param pEnemy enemy object (int [3]).
 * @param pPlayer player object (int [3]).
 * @param pBulletStCell if facing = TRUE, start cell of the bullet.
 * @return int whether enemy is facing or not.
 */
int isFacingPlayerCore(int pEnemy[], int pPlayer[], int* pBulletStCell)
{
	int st = -1, en = -1;
	int isFacing = FALSE;

	/* Check based on enemy's direction */
	switch (pEnemy[IDX_DIR])
	{
		case 'u': /* if same col && enemy below the player */
			if ((pEnemy[IDX_COL] == pPlayer[IDX_COL]) && 
                (pEnemy[IDX_ROW] > pPlayer[IDX_ROW]))
			{
				st = pPlayer[IDX_ROW] + 1;
				en = pEnemy[IDX_ROW] - 1;
				pBulletStCell[IDX_ROW] = en;
			}
			break;
			
		case 'd': /* if same row && enemy above player */
			if ((pEnemy[IDX_COL] == pPlayer[IDX_COL]) && 
                (pEnemy[IDX_ROW] < pPlayer[IDX_ROW]))
			{
				st = pEnemy[IDX_ROW] + 1;
				en = pPlayer[IDX_ROW] - 1;
                pBulletStCell[IDX_ROW] = st;
			}
			break;
			
		case 'l': /* if same row && enemy after player */
            if ((pEnemy[IDX_ROW] == pPlayer[IDX_ROW]) && 
                (pEnemy[IDX_COL] > pPlayer[IDX_COL]))
			{
				st = pPlayer[IDX_COL] + 1;
				en = pEnemy[IDX_COL] - 1;
                pBulletStCell[IDX_COL] = en;
			}
			break;
			
		case 'r': /* if same row && enemy is before player */
            if ((pEnemy[IDX_ROW] == pPlayer[IDX_ROW]) && 
                (pEnemy[IDX_COL] < pPlayer[IDX_COL]))
			{
				st = pEnemy[IDX_COL] + 1;
				en = pPlayer[IDX_COL] - 1;
				pBulletStCell[IDX_COL] = st;
			}
			break;
	}

	if (st != -1)    
		isFacing = TRUE;

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
int isFacingPlayer(int pEnemy[], int pPlayer[], int* pBulletStCell)
{
    int isFacing = FALSE;
    int aiBulletStCell[ARR_SIZE_OBJ];
	copyObj(aiBulletStCell, pEnemy);
	    
	isFacing = isFacingPlayerCore(pEnemy, pPlayer, aiBulletStCell);
	
	if (isFacing && pBulletStCell)
        copyObj(pBulletStCell, aiBulletStCell);

    return isFacing;
}

/**************************************************************************************************/
/**
 * @brief Whether object1 overlaps another object (enemy, player, bullet, etc).
 * 
 * @param pObj1 object1 (int [3]).
 * @param pObj2 object2 (int [3]).
 * @return int overlapping status.
 */
int isObjOverlap(int pObj1[], int pObj2[])
{
    return (pObj1[IDX_ROW] == pObj2[IDX_ROW] && 
                pObj1[IDX_COL] == pObj2[IDX_COL]);
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
    return (direction == 'r' || direction == 'l' || direction == 'u' || direction == 'd');
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
int validateObjBounds(int pMapSize[], int pObj[], char* zMsg)
{    
	/* validity in the rows direction (y-direction) */
    int valid = BETWEEN(1, (pMapSize[IDX_MAP_NROW] - 2), pObj[IDX_ROW]);

	/* validity in the cols direction (x-direction) */
    valid = valid && BETWEEN(1, (pMapSize[IDX_MAP_NCOL] - 2), pObj[IDX_COL]);

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
int validateTanks(int* pEnemy, int* pPlayer)
{
	int isValid = TRUE;
	
	/* checks if player is placed in front of enemy -> insant lose */
	if (isFacingPlayer(pEnemy, pPlayer, NULL))
	{
		printError("Cannot place tanks in instant lose position.\n");
		isValid = FALSE;
	}
	
	/* checks if player and enemy are on the same starting cell */
	else if (isObjOverlap(pEnemy, pPlayer))
	{
		printError("Player and enemy cannot start on the same cell.\n");
		isValid = FALSE;
	}
	
	return isValid;
}