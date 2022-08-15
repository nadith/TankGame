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
 * @brief Create the canvas. malloc().
 * 
 * @param pMapSize size of the map (int [2]).
 * @return char** 2D canvas
 */
char** createMap(int pMapSize[])
{
	int rowSize = pMapSize[IDX_MAP_NROW];
	int colSize = pMapSize[IDX_MAP_NCOL];

    char** map = malloc(rowSize * sizeof(char*));
    int i = 0;

    for(i = 0; i < rowSize; i++)
        map[i] = malloc(colSize * sizeof(char));    

    return map;
}

/**************************************************************************************************/
/**
 * @brief Destroy the canvas. Call free().
 * 
 * @param map the canvas.
 * @param pMapSize size of the map (int [2]).
 */
void destroyMap(char** map, int pMapSize[])
{
    int rowSize = pMapSize[IDX_MAP_NROW];
	int i;	
    for(i = 0; i < rowSize; i++)
    {
        free(map[i]);
		map[i] = NULL;
    }

    free(map);
    map = NULL;
}

/**************************************************************************************************/
/**
 * @brief Reset the map. place the border of the canvas.
 * 
 * @param map the canvas.
 * @param pMapSize size of the map (int [2]).
 */
void resetMap(char** map, int pMapSize[])
{	
	int rowSize = pMapSize[IDX_MAP_NROW];
	int colSize = pMapSize[IDX_MAP_NCOL];
	
	int i;
	for(i = 0; i < rowSize; i++)
	{
		if (i == 0 || i == rowSize - 1)
		{
			memset(map[i], MARKER_BORDER, colSize);
		}
		else
		{
			memset(map[i], MARKER_BORDER, 1);
			memset(map[i] + 1, ' ', colSize - 2);
			memset(map[i] + colSize - 1, MARKER_BORDER, 1);
		}
	}
}

/**************************************************************************************************/
/**
 * @brief Place object (enemy, player, bullet) with the correct face.
 * 
 * @param map the canvas.
 * @param pObj the object (enemy, player, bullet).
 */
void placeObj(char** map, int pObj[])
{
	if (pObj && isObjInitialized(pObj))
	{
		int row = pObj[IDX_ROW];
		int col = pObj[IDX_COL];
		int direction = pObj[IDX_DIR];
		char face = ' ';
			
		switch(direction)
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
				
			default:
				face = direction; /* For the bullet */
			break;
		}
			
		map[row][col] = face;	
	}
}


/**************************************************************************************************/
/* Map Display Methods												    		      		  	  */
/**************************************************************************************************/
/**
 * @brief Prints the map along with all objects (enemy, player, bullet).
 * 
 * @param map the canvas.
 * @param pMapSize size of the map (int [2]).
 */
static void printMap(char** map, int pMapSize[])
{	
	int rowSize = pMapSize[IDX_MAP_NROW];
	int colSize = pMapSize[IDX_MAP_NCOL];
	int i, j; 
	
	/* Bullet color change */
	static int colourIdx = 0;
    Colours colours[2];
    colours[0] = &red;  	/* using function pointer */
    colours[1] = &green;	/* using function pointer */	

	for(i = 0; i < rowSize; i++)
	{
		for(j = 0; j < colSize; j++)
		{
			/* bullet */
			if (map[i][j] == '|' || map[i][j] == '-')
            { 
				(*colours[colourIdx++ % 2])(map[i][j]);
				colourIdx %= 2;
			}
			else
			{				
				printf("%c", map[i][j]);
			}
		}
		printf("\n");
	}
}

/**************************************************************************************************/
/**
 * @brief Refresh the map. This used to print the map to the screen every time a
 * user activity is performed.
 * 
 * @param pRefreshParams parameter object (void* []) to pass across functions.
 */
void refreshMap(void* pRefreshParams[])
{
	char** map;
	int *pMapSize, *pEnemy, *pPlayer, *pBullet;	
	unpackRefreshParams(pRefreshParams, &map, &pMapSize, &pEnemy, &pPlayer, &pBullet);

	/* Reset map and sets the border */
	resetMap(map, pMapSize);

	/* Place the objects on the map */	
	placeObj(map, pEnemy);
	placeObj(map, pPlayer);
	placeObj(map, pBullet);
	
	/* Print the Map */
#ifndef DEBUG
	system("clear");  /* <= comment this line if you want to see all past frames on terminal */
#endif
	printMap(map, pMapSize);
}

/**************************************************************************************************/
/**
 * @brief Add pause time before refresh the map / screen. Refer refreshMap() for
 * more details.
 * 
 * @param pRefreshParams parameter object (void* []) to pass across functions.
 */
void pauseAndRefreshMap(void* pRefreshParams[])
{
	newSleep(0.2);
	refreshMap(pRefreshParams);
}