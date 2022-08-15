/* PURPOSE: Main flow of the program.
 * AUTHOR: Nadith Pathirage <<StudentID>>
 * DATE CREATED: 27/05/2021
 * DATE MODIFIED: 13/08/2022
 */

/* Standard Include */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Local Includes */
#include "map.h"
#include "util.h"
#include "macros.h"
#include "envinit.h"
#include "gameops.h"

int main(int argc, char *argv[])
{
    /* Declaring variables */
    char** map;

    /* 0 -> no. of rows, 1 -> no. of cols */
    int aiMapSize[2] = {0, 0};
    
    /* RefreshPrams object (void* [5]) is used when printing the map/canvas */
    void* apvRefreshParams[5] = {NULL, NULL, NULL, NULL, NULL};

    /* Objects to represent enemy, player (int [3]) */
    /* 0 -> row index, 1 -> col index, 2 -> facing direction */    
    int aiEnemy[ARR_SIZE_OBJ]; 
    int aiPlayer[ARR_SIZE_OBJ]; 
    resetObj(aiEnemy); resetObj(aiPlayer);

    /* Initialize the game */
	if (initGame(&map, aiMapSize, aiEnemy, aiPlayer, argv, argc))
	{
        /* pack the individual params to RefreshPrams object */
		packRefreshParams(apvRefreshParams, map, aiMapSize, aiEnemy, aiPlayer, NULL);

        /* Print the refresh map params */
        debugRefreshMapParams(apvRefreshParams, "RP");

		/* Print the canvas along with other objects (enemy, player) */
		refreshMap(apvRefreshParams);

        /* Enter into the main loop and process the game status once loop exits */
		processGameStatus(mainLoop(apvRefreshParams));

        /* Exit from the game, cleanup ! */
		exitGame(map, aiMapSize);
    }

    return 0;
}
