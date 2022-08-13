/* PURPOSE: Utility functions of the Tank Game.
 * AUTHOR: Nadith Pathirage <<StudentID>>
 * DATE CREATED: 27/05/2021
 * DATE MODIFIED: 13/08/2022
 */

/* Standard Include */
#include <stdio.h>
#include <string.h>

/* Local Includes */
#include "util.h"
#include "macros.h"

/**************************************************************************************************/
/* Refresh Params Related Methods												    		      */
/**************************************************************************************************/
/**
 * @brief Pack the individual parameters into RefreshParams array.
 * 
 * @param pRefreshParams parameter object (void* []) to pass across other functions.
 * @param map canvas (char**)
 * @param pMapSize size of the map (int [2]).
 * @param pEnemy enemy object (int [3]).
 * @param pPlayer player object (int [3]).
 * @param pBullet bullet object (int [3]).
 */
void packRefreshParams(void* pRefreshParams[], char** map, int* pMapSize, 
							int* pEnemy, int* pPlayer, int* pBullet)
{
	pRefreshParams[IDX_RP_MAP] = map;
    pRefreshParams[IDX_RP_MAP_SIZE] = pMapSize;
    pRefreshParams[IDX_RP_ENEMY] = pEnemy;
    pRefreshParams[IDX_RP_PLAYER] = pPlayer;
    pRefreshParams[IDX_RP_BULLET] = pBullet;
}

/**************************************************************************************************/
/**
 * @brief Unpack the individual parameters from RefreshParams array.
 * 
 * @param pRefreshParams parameter object (void* []) to pass across other functions.
 * @param map canvas (char**)
 * @param ppMapSize export variable, size of the map (int [2]).
 * @param ppEnemy export variable, enemy object (int [3]).
 * @param ppPlayer export variable, player object (int [3]).
 * @param ppBulet export variable, bullet object (int [3]).
 */
void unPackRefreshParams(void* pRefreshParams[], char*** map, int** ppMapSize, 
							int** ppEnemy, int** ppPlayer, int** ppBulet)
{
	*map = pRefreshParams[IDX_RP_MAP];
	*ppMapSize = pRefreshParams[IDX_RP_MAP_SIZE];
	*ppEnemy = pRefreshParams[IDX_RP_ENEMY];
	*ppPlayer = pRefreshParams[IDX_RP_PLAYER];
	*ppBulet = pRefreshParams[IDX_RP_BULLET];
}

/**************************************************************************************************/
/* Object (enemy, player, bullet, etc) Related Methods								          	  */
/**************************************************************************************************/
/**
 * @brief Update the object (int [3]) values.
 * 
 * @param pObj the object (enemy, player, bullet, etc).
 * @param val1 the value for object[IDX_ROW].
 * @param val2 the value for object[IDX_COL].
 * @param val3 the value for object[IDX_DIR].
 */
void updateObj(int* pObj, int val1, int val2, int val3)
{
	pObj[IDX_ROW] = val1;
	pObj[IDX_COL] = val2;
	pObj[IDX_DIR] = val3;
}

/**************************************************************************************************/
/**
 * @brief copy the object from source to destination.
 * 
 * @param pDestObj the detination object (enemy, player, bullet, etc).
 * @param pSrcObj the source object (enemy, player, bullet, etc).
 */
void copyObj(int* pDestObj, int* pSrcObj)
{
	if (pSrcObj)
		memcpy(pDestObj, pSrcObj, sizeof(int)*ARR_SIZE_OBJ);
}

/**************************************************************************************************/
/**
 * @brief Checks whether the object (enemy, player, bullet, etc) is initalized.
 * i.e. player[3] => should not have {-1, -1, ' '} if initialized.
 * 
 * @param pObj the object (enemy, player, bullet, etc).
 * @return int whether initialized or not.
 */
int isObjInitialized(int pObj[])
{
	return !(pObj[IDX_ROW] == -1 && pObj[IDX_COL] == -1 && pObj[IDX_DIR] == ' ');
}

/**************************************************************************************************/
/**
 * @brief Resets the object values to {-1, -1, ' '}.
 * 
 * @param pObj the object (enemy, player, bullet, etc).
 */
void resetObj(int pObj[])
{
	updateObj(pObj, -1, -1, ' ');
}

/**************************************************************************************************/
/**
 * @brief Get the string representation of the object (enemy, player, bullet, etc). 
 * 
 * @param pObj object to get the string representation.
 * @param zObjStr string buffer (char[]) to store the string representation of the object.
 */
void toString(int pObj[], char zObjStr[])
{
	if (pObj && isObjInitialized(pObj))
		sprintf(zObjStr, "{%d, %d, %c}", pObj[IDX_ROW], pObj[IDX_COL], pObj[IDX_DIR]);
	else
		sprintf(zObjStr, "{null}");
}

/**************************************************************************************************/
/* Debug Prints Related Methods														    	  	  */
/**************************************************************************************************/
/**
 * @brief Debug print of the object (enemy, player, bullet, etc). 
 * 
 * @param pObj object to be printed.
 * @param zPrefix curtom prefix string.
 */
void debugObj(int pObj[], const char* zPrefix)
{
#ifdef DEBUG
	printf("%s: {%d, %d, %c}\n", zPrefix, pObj[IDX_ROW], pObj[IDX_COL], pObj[IDX_DIR]);
#endif
}

/**************************************************************************************************/
/**
 * @brief Debug print of the map.
 * 
 * @param map canvas (char**).
 * @param pMapSize size of the map (int [2]).
 * @param zPrefix curtom prefix string.
 */
void debugMap(char** map, int* pMapSize, const char* zPrefix)
{
#ifdef DEBUG
	printf("%s: {%d, %d, %p}\n", zPrefix, pMapSize[IDX_MAP_NROW], pMapSize[IDX_MAP_NCOL], (void*)map);
#endif
}

/**************************************************************************************************/
/**
 * @brief Debug print of the refresh parameters.
 * 
 * @param pRefreshParams parameter object (void* []) to pass across other functions.
 * @param zPrefix curtom prefix string.
 */
void debugRefreshMapParams(void* pRefreshParams[], char* zPrefix)
{
#ifdef DEBUG

	char zBullet[20];

	char** map;
	int *pMapSize, *pEnemy, *pPlayer, *pBullet;	
	unPackRefreshParams(pRefreshParams, &map, &pMapSize, &pEnemy, &pPlayer, &pBullet);
	toString(pBullet, zBullet);

	/*
	KEY:
	M -> map
	E -> enemy
	P -> player
	L -> lazer	
	*/		
	printf("%s: M:{%d, %d, %p} E:{%d, %d, %c} P:{%d, %d, %c} B:%s\n", 
			zPrefix, 
			pMapSize[IDX_MAP_NROW], pMapSize[IDX_MAP_NCOL], (void*)map,
			pEnemy[IDX_ROW], pEnemy[IDX_COL], pEnemy[IDX_DIR],
			pPlayer[IDX_ROW], pPlayer[IDX_COL], pPlayer[IDX_DIR],
			zBullet
			);
#endif
}

/**************************************************************************************************/
/* Color Related Methods														    	  		  */
/**************************************************************************************************/
/**
 * @brief Put red color character on the terminal.
 * 
 * @param ch character to color.
 */
void red(char ch) 
{
    printf("\033[1;31m");
    printf("%c%s", ch, "\033[0m");

	return;
}

/**************************************************************************************************/
/**
 * @brief Put green color character on the terminal.
 * 
 * @param ch character to color.
 */
void green(char ch)
{
    printf("\033[1;32m");
    printf("%c%s", ch, "\033[0m");

	return;
}

/**************************************************************************************************/
/**
 * @brief Put red color string/error on the terminal.
 * 
 * @param zStr string to color.
 */
void printError(char *zStr) 
{
    printf("\033[1;31m");
    printf("[ERROR] %s%s", zStr, "\033[0m");

	return;
}