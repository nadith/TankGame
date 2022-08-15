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
#include "linkedlist.h"

int main(int argc, char *argv[])
{
	/* Declrations: Convient Parameter Passing */
	RefreshMapParam oRP;
	
	/* Declrations: LinkLists & User Input/Output */
	LinkedList* pMirrorList = NULL;
	LinkedList* pLogList = NULL;
		
	/* Declrations: Map Related */
	MapInfo* pMapInfo = NULL;
	GameObj player, enemy;
		
	/* Declrations: Game related */	
	const char* zConfigFileName;
	FileEx logFile;

    /* Initialize the game */
    if (parseCmdArgs(argc, argv, &zConfigFileName, &logFile) &&
		initGame(zConfigFileName, &pMapInfo, &player, &enemy, &pMirrorList, &pLogList))
	/*if (initGame(&map, aiMapSize, aiEnemy, aiPlayer, argv, argc))*/
	{
        /* pack the individual params to RefreshPrams object */
		packRefreshParams(&oRP, pMapInfo, &enemy, &player, NULL, 
                                    pMirrorList, pLogList, &logFile, TRUE);

        /* RP debug print Print */
        debugRefreshMapParams(&oRP, "RP");

		/* Print the canvas along with other objects (enemy, player) */
		refreshMap(&oRP);

        /* Enter into the main loop and process the game status once loop exits */
		processGameStatus(mainLoop(&oRP));

        /* Exit from the game, cleanup ! */
		exitGame(pMapInfo, pMirrorList, pLogList);
    }

    return 0;
}
