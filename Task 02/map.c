/* PURPOSE: Map/Canvas related functionality.
 * AUTHOR: Nadith Pathirage <<StudentID>>
 * DATE CREATED: 27/05/2021
 * DATE MODIFIED: 13/08/2022
 */

/* Standard Include */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Local Includes */
#include "map.h"
#include "util.h"
#include "macros.h"
#include "newSleep.h"

typedef void (*Colours)(char);

/**************************************************************************************************/
/* Map Managment Methods												    		      		  */
/**************************************************************************************************/
/**
 * @brief Create a Map object
 * 
 * @param rows map/canvas number of rows.
 * @param cols map/canvas number of columns.
 * @return MapInfo* map object.
 */
MapInfo* createMap(int rows, int cols)
{
	int i;
	
	MapInfo* pMapInfo = (MapInfo*) malloc(sizeof(MapInfo));
	pMapInfo->map = (char**) malloc(sizeof(char*) * rows);
		
	for (i = 0; i < rows ; i++)	
		pMapInfo->map[i] = (char*) malloc(sizeof(char) * cols);
	
	pMapInfo->rows = rows;
	pMapInfo->cols = cols;
	
	return pMapInfo;
}

/**************************************************************************************************/
/**
 * @brief Destroy the map/canvas object. Call free().
 * 
 * @param pMapInfo map object.
 */
void destroyMap(MapInfo* pMapInfo)
{
	int i;
	for (i = 0; i < pMapInfo->rows ; i++)	
		free(pMapInfo->map[i]);
	
	free(pMapInfo->map);
		
	pMapInfo->map = NULL;
	pMapInfo->rows = -1;
	pMapInfo->cols = -1;
	free(pMapInfo);
}

/**************************************************************************************************/
/**
 * @brief Reset the map. place the border of the canvas.
 * 
 * @param pMapInfo map object.
 */
void resetMap(MapInfo* pMapInfo)
{	
	int i;
	for (i = 0; i < pMapInfo->rows ; i++)
	{
		if (i == 0 || i == pMapInfo->rows - 1)
		{
			memset(pMapInfo->map[i], MARKER_BORDER, pMapInfo->cols);
		}
		else
		{
			memset(pMapInfo->map[i], MARKER_BORDER, 1);
			memset(pMapInfo->map[i] + 1, ' ', pMapInfo->cols - 2);
			memset(pMapInfo->map[i] + pMapInfo->cols - 1, MARKER_BORDER, 1);
		}
	}	
}

/**************************************************************************************************/
/**
 * @brief Place object (enemy, player, bullet, mirror) with the correct face.
 * 
 * @param pMapInfo map object.
 * @param pObj game object (enemy, player, bullet).
 */
void placeObj(MapInfo* pMapInfo, GameObj* pObj)
{
	if (pObj)
	{
		char face = ' ';
		
		switch(pObj->direction)
		{
			case DIR_LEFT:
				face = MARKER_FACE_LEFT;
				break;
				
			case DIR_RIGHT:
				face = MARKER_FACE_RIGHT;
				break;
				
			case DIR_UP:
				face = MARKER_FACE_UP;
				break;
				
			case DIR_DOWN:
				face = MARKER_FACE_DOWN;
				break;
				
			case DIR_F:
				face = MARKER_FACE_FMIRROR;
				break;
				
			case DIR_B:
				face = MARKER_FACE_BMIRROR;
				break;
				
			default:
				face = pObj->direction;
				break;
		}
		
		pMapInfo->map[pObj->row][pObj->col] = face;
	}
}

/**************************************************************************************************/
/**
 * @brief Place mirror objects with the correct face.
 * 
 * @param pMapInfo map object.
 * @param pMirrorList mirror linked list.
 */
void placeMirrors(MapInfo* pMapInfo, LinkedList* pMirrorList)
{
	if (pMirrorList)
	{		
		LinkedListNode* pCur = pMirrorList->pHead;
		while (pCur != NULL)
		{
			GameObj* pMirror = (GameObj*)(pCur->pData);
			placeObj(pMapInfo, pMirror);
			
			pCur = pCur->pNext;
		}
	}
}

/**************************************************************************************************/
/**
 * @brief Creates a new map object (malloc()) and copy pMapInfo object to it. 
 * 
 * @param pMapInfo map object.
 * @return MapInfo* new map object.
 */
MapInfo* copyMapInfo(const MapInfo* pMapInfo)
{
	int i ;
	MapInfo* pMapInfoCopy = createMap(pMapInfo->rows, pMapInfo->cols);
		
	/* Take a copy of map */ 
	for (i = 0 ; i < pMapInfo->rows ; i++)
	{
		memcpy(pMapInfoCopy->map[i], pMapInfo->map[i], sizeof(char) * pMapInfo->cols);
	}
	
	return pMapInfoCopy;
}

/**************************************************************************************************/
/* Map Display Methods												    		      		  	  */
/**************************************************************************************************/
/**
 * @brief Store the map object (the current canvas with all objects placed) in
 * a node in the linked list.
 * 
 * @param pRP parameter object to pass across functions.
 * @param pMapInfo map object.
 */
static void storeMap(RefreshMapParam* pRP, MapInfo* pMapInfo)
{
	/* Declarations: linked list debug prints */
	static int nodeCounter = 0;
	char prefix[50];

	NodeData* pNodeData = (NodeData*) malloc(sizeof(NodeData));
	pNodeData->pMapInfo = copyMapInfo(pMapInfo);
	pNodeData->pLogFile = pRP->pLogFile;

	/* Insert to linked list */
	insertLast(pRP->pLogList, pNodeData);
	
	/* Debug print */		
	nodeCounter++;		
	sprintf(prefix, "Log: Insert_%2d", nodeCounter);
	debugLinkedList(pRP->pLogList, prefix);
}

/**************************************************************************************************/
/**
 * @brief Print map (with all the other objects[enemy, player, bullet, mirrors]) 
 * to the terminal or file, and store the map in a linked list node.
 * 
 * @param i row index of the map.
 * @param j column index of the map.
 * @param pColors colors array (type function pointer).
 * @param pMapInfo map object.
 * @param pCfgFile configuration file - input file (FILE).
 */
static void printAndStoreMapExCore(int i, int j, Colours pColors[], MapInfo* pMapInfo, FILE* pCfgFile)
{
	static int colourIdx = 0;

	if (pMapInfo->map[i][j] == '|' || pMapInfo->map[i][j] == '-')
	{ 
		if (pCfgFile)
			fprintf(pCfgFile, "%c", pMapInfo->map[i][j]);
		else
		{
			(*pColors[colourIdx++ % 2])(pMapInfo->map[i][j]);
			colourIdx %= 2;
		}
	}
	else
	{
		if (pCfgFile)
			fprintf(pCfgFile, "%c", pMapInfo->map[i][j]);
		else
			printf("%c", pMapInfo->map[i][j]);
	}
}

/**************************************************************************************************/
/**
 * @brief Print map (with all the other objects[enemy, player, bullet, mirrors]) 
 * to the terminal or file, and store the map in a linked list node.
 * 
 * @param pRP parameter object to pass across functions.
 */
void printAndStoreMap(RefreshMapParam* pRP)
{	
	int i, j;
	MapInfo* pMapInfo = pRP->pMapInfo;
	FILE* pCfgFile = (pRP->pLogFile) ? pRP->pLogFile->fptr:NULL;

	/* Cullet color change */
    Colours aColors[2];
    aColors[0] = &red;
    aColors[1] = &green;
	
	for (i = 0 ; i < pMapInfo->rows ; i++)
	{
		for (j = 0 ; j < pMapInfo->cols ; j++)
		{
			printAndStoreMapExCore(i, j, aColors, pMapInfo, pCfgFile);
		}

		if (pCfgFile)
			fprintf(pRP->pLogFile->fptr, "\n");
		else
			printf("\n");
	}
	
	if (pRP->isStoreMap)
		storeMap(pRP, pMapInfo);	
}

/**************************************************************************************************/
/**
 * @brief Refresh the map. This used to print the map to the screen or file
 * every time a user activity is performed.
 * 
 * @param pRP parameter object to pass across functions.
 */
void refreshMap(RefreshMapParam* pRP)
{
	refreshMapEx(pRP, TRUE);
}

/**
 * @brief Refresh the map. This used to print the map to the screen or file
 * every time a user activity is performed.
 * 
 * @param pRP parameter object to pass across functions.
 * @param isPrintAndStoreMap whether to print and store map or just place the
 * objects on the map/canvas.
 */
void refreshMapEx(RefreshMapParam* pRP, int isPrintAndStoreMap)
{	
	MapInfo* pMapInfo = pRP->pMapInfo;
	
	/* Reset map and set the border */
	resetMap(pMapInfo);

	/* Place the objects on the map */	
	placeObj(pMapInfo, pRP->pEnemy);
	placeMirrors(pMapInfo, pRP->pMirrorList);
	placeObj(pMapInfo, pRP->pPlayer);
	placeObj(pMapInfo, pRP->pBullet);
	
	/* Print the Map */
	if (isPrintAndStoreMap) 
	{
#ifndef DEBUG
		system("clear");  /* <= comment this line if you want to see all past frames on terminal */
#endif
		printAndStoreMap(pRP);
	}
	
	pRP->isStoreMap = FALSE;
}

/**************************************************************************************************/
/**
 * @brief Add pause time before refresh the map / screen. Refer refreshMap() for
 * more details.
 * 
 * @param pRP parameter object to pass across functions.
 */
void pauseAndRefreshMap(RefreshMapParam* pRP)
{
	newSleep(0.2);
	refreshMap(pRP);
}
