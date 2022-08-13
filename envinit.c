/* PURPOSE: Game environment initialization functionality.
 * AUTHOR: Nadith Pathirage <<StudentID>>
 * DATE CREATED: 27/05/2021
 * DATE MODIFIED: 13/08/2022
 */

/* Standard Include */
#include <stdio.h>
#include <stdlib.h>

/* Local Includes */
#include "map.h"
#include "macros.h"
#include "validate.h"

/**************************************************************************************************/
/* Game Init/Exit Related Methods														   		  */
/**************************************************************************************************/
/**
 * @brief Initialize the game enviornment. Fails if validations fail.
 * 
 * @param map the canvas.
 * @param pMapSize size of the map (int [2]).
 * @param pEnemy player object (int [2]).
 * @param pPlayer player object (int [2]).
 * @param argv command line args strings.
 * @param argc command line args count.
 * @return int initialization status (pass or fail).
 */
int initGame(char*** map, int pMapSize[] , 
				int pEnemy[], int pPlayer[],
				char** argv, int argc)
{
	int isSuccess = TRUE;
    if (argc != 9)
    {
        printf("Please enter:\n");
        printf("%s <row size> <column size> <player row> <player col> "
					"<player face> <enemy row> <enemy col> <enemy face>\n", 
					argv[0]);
		isSuccess = FALSE;
    }
	else
	{
		/* Read from command line arguments */
		sscanf(argv[1], " %d ", &(pMapSize[IDX_MAP_NROW]));
		sscanf(argv[2], " %d ", &(pMapSize[IDX_MAP_NCOL]));
		sscanf(argv[3], " %d ", &(pPlayer[IDX_ROW]));
		sscanf(argv[4], " %d ", &(pPlayer[IDX_COL]));
		sscanf(argv[5], " %c ", (char*) &(pPlayer[IDX_DIR]));
		sscanf(argv[6], " %d ", &(pEnemy[IDX_ROW]));
		sscanf(argv[7], " %d ", &(pEnemy[IDX_COL]));
		sscanf(argv[8], " %c ", (char*)  &(pEnemy[IDX_DIR]));

		isSuccess = 
			validateObjBounds(pMapSize, pPlayer, "Player out of bounds.\n") &&
			validateObjBounds(pMapSize, pEnemy, "Enemy out of bounds.\n") && 
			validateTanks(pEnemy, pPlayer);
	}

	if (isSuccess)
		*map = createMap(pMapSize);

	return isSuccess;
}

/**************************************************************************************************/
/**
 * @brief Exit from the game enviornment. Destroy any dynamic allocations in the 
 * game enviornment.
 * 
 * @param map the canvas.
 * @param pMapSize size of the map (int [2]).
 */
void exitGame(char** map, int pMapSize[])
{
		/* Destroy the map in main() */
		destroyMap(map, pMapSize);
}

