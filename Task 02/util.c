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
 * @brief Pack the individual parameters into RefreshParams object.
 * 
 * @param pRP parameter object to pass across functions.
 * @param pMapInfo map object.
 * @param pEnemy enemy object.
 * @param pPlayer player object.
 * @param pBullet bullet object.
 * @param pMirrorList mirror linked list.
 * @param pLogList log linked list.
 * @param pLogFile log file - output file (FileEx).
 * @param isStoreMap whether to store the map as node in the log linked list.
 */
void packRefreshParams(RefreshMapParam* pRP, MapInfo* pMapInfo, 
				GameObj* pEnemy, GameObj* pPlayer, GameObj* pBullet,
				LinkedList* pMirrorList, LinkedList* pLogList, 
				FileEx* pLogFile, int isStoreMap)
{
	pRP->pMapInfo = pMapInfo;
	pRP->pEnemy = pEnemy;
	pRP->pPlayer = pPlayer;
	pRP->pBullet = NULL;
	pRP->pMirrorList = pMirrorList;
	pRP->pLogList = pLogList;
	pRP->pLogFile = pLogFile;
	pRP->isStoreMap = isStoreMap;
}

/**************************************************************************************************/
/**
 * @brief Unpack the individual parameters from RefreshParams object.
 * 
 * @param pRP parameter object to pass across functions.
 * @param ppMapInfo export variable for map object.
 * @param ppEnemy export variable for enemy object.
 * @param ppPlayer export variable for player object.
 * @param ppBullet export variable for bullet object.
 * @param ppMirrorList export variable for mirror linked list.
 * @param ppLogList export variable for log linked list.
 * @param ppLogFile export variable for log file.
 * @param piIsStoreMap export variable for store map boolean variable.
 */
void unpackRefreshParams(RefreshMapParam* pRP, MapInfo** ppMapInfo, 
				GameObj** ppEnemy, GameObj** ppPlayer, GameObj** ppBullet,
				LinkedList** ppMirrorList, LinkedList** ppLogList, 
				FileEx** ppLogFile, int* piIsStoreMap)
{
	*ppMapInfo = pRP->pMapInfo;
	*ppEnemy = pRP->pEnemy;
	*ppPlayer = pRP->pPlayer;
	*ppBullet = pRP->pBullet;
	*ppMirrorList = pRP->pMirrorList;
	*ppLogList = pRP->pLogList;
	*ppLogFile = pRP->pLogFile;
	*piIsStoreMap = pRP->isStoreMap;
}

/**************************************************************************************************/
/* Object (enemy, player, bullet, etc) Related Methods								          	  */
/**************************************************************************************************/
/**
 * @brief Update the object (enemy, player, bullet, mirror) values.
 * 
 * @param pObj the object (enemy, player, bullet, mirror).
 * @param row row index of the object.
 * @param col col index of the object.
 * @param dir dir index of the object.
 */
void updateObj(GameObj* pObj, int row, int col, int dir)
{
	pObj->row = row;
	pObj->col = col;
	pObj->direction = dir;
}

/**************************************************************************************************/
/**
 * @brief copy the object from source to destination.
 * 
 * @param pDestObj the detination object (enemy, player, bullet, etc).
 * @param pSrcObj the source object (enemy, player, bullet, etc).
 */
void copyObj(GameObj* pDestObj, GameObj* pSrcObj)
{
	if (pSrcObj)
		memcpy(pDestObj, pSrcObj, sizeof(GameObj));
}

/**************************************************************************************************/
/* Debug Prints Related Methods														    	  	  */
/**************************************************************************************************/
/**
 * @brief Get the string representation of the object (enemy, player, bullet, etc). 
 * 
 * @param pObj object to get the string representation.
 * @param zObjStr string buffer (char[]) to store the string representation of the object.
 */
void toString(GameObj* pObj, char zObjStr[])
{
	if (pObj)
		sprintf(zObjStr, "{%d, %d, %c}", pObj->row, pObj->col, pObj->direction);
	else
		sprintf(zObjStr, "{null}");
}

/**************************************************************************************************/
/**
 * @brief Debug print of the object (enemy, player, bullet, etc). 
 * 
 * @param pObj object to be printed.
 * @param zPrefix curtom prefix string.
 */
void debugObj(GameObj* pObj, const char* zPrefix)
{
#ifdef DEBUG
	if (pObj)
		printf("%s: %p {%d, %d, %c}\n", zPrefix, 
				(void*) pObj, pObj->row, pObj->col, pObj->direction);
#endif
}

/**************************************************************************************************/
/**
 * @brief Debug print of the map.
 * 
 * @param pMapInfo map object.
 * @param zPrefix curtom prefix string.
 */
void debugMap(MapInfo* pMapInfo, const char* zPrefix)
{
#ifdef DEBUG
	printf("%s: {%d, %d, %p}\n", zPrefix, pMapInfo->rows, pMapInfo->cols, (void*)pMapInfo->map);
#endif
}

/**************************************************************************************************/
/**
 * @brief Debug print of the refresh parameters.
 * 
 * @param pRP parameter object to pass across functions.
 * @param zPrefix curtom prefix string.
 */
void debugRefreshMapParams(RefreshMapParam* pRP, char* zPrefix)
{
#ifdef DEBUG

	char zBullet[20];

	MapInfo* pMapInfo;
	GameObj *pPlayer, *pEnemy, *pBullet; 	
	LinkedList *pMirrorList, *pLogList;
	FileEx* pLogFile; int isStoreMap;
	unpackRefreshParams(pRP, &pMapInfo, 
							&pEnemy, &pPlayer, &pBullet,
							&pMirrorList, &pLogList, 
							&pLogFile, &isStoreMap);
	toString(pBullet, zBullet);

	/*
	KEY:
	M -> map
	E -> enemy
	P -> player
	B -> bullet	
	L -> linked lists
	F -> file
	SM -> store map
	*/	
	printf("%s: M:{%d, %d, %p} E:{%d, %d, %c} P:{%d, %d, %c} B:%s LL:{%p, %p} F:%p SM:%d\n", 
			zPrefix, 
			pMapInfo->rows, pMapInfo->cols, (void*)pMapInfo->map,
			pEnemy->row, pEnemy->col, pEnemy->direction,
			pPlayer->row, pPlayer->col, pPlayer->direction,
			zBullet, (void*)pMirrorList, (void*)pLogList, (void*)pLogFile, isStoreMap);
#endif
}

/**************************************************************************************************/
/**
 * @brief Debug print of the linked list (nodes).
 * 
 * @param pList linked list.
 * @param prefix curtom prefix string.
 */
void debugLinkedList(LinkedList* pList, char* prefix) 
{
	printInfo("\n-----------------------------------------------------------------------------------\n");
#ifdef DEBUG
	if (pList->pHead && pList->pTail)
	{
		printf("%s: LinkedList: pHead:%p (%p) pTail:%p (%p)\n", 
						prefix, (void*)pList->pHead, (void*)pList->pHead->pData, 
						(void*)pList->pTail, (void*)pList->pTail->pData);
	}
	else
	{
		printf("%s: LinkedList: pHead:%p pTail:%p\n", 
						prefix, (void*)pList->pHead, (void*)pList->pTail);
	}
#endif
	printInfo("-----------------------------------------------------------------------------------\n");
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

/**************************************************************************************************/
/* Debug Prints Methods														    	  		  	  */
/**************************************************************************************************/
/**
 * @brief Put red color string/error on the terminal.
 * 
 * @param zStr string to color.
 */
void printInfo(char *zStr) 
{	
#ifdef DEBUG
    printf("\033[1;32m");
    printf("%s%s", zStr, "\033[0m");
#endif
}