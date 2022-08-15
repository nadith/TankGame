/* PURPOSE: Game environment initialization functionality.
 * AUTHOR: Nadith Pathirage <<StudentID>>
 * DATE CREATED: 27/05/2021
 * DATE MODIFIED: 13/08/2022
 */

/* Standard Include */
#include <stdio.h>
#include <stdlib.h>

/* Local Includes */
#include "util.h"
#include "macros.h"
#include "envinit.h"
#include "validate.h"
#include "gameops.h"

/**************************************************************************************************/
/* Mirror Linked List Related Methods													    	  */
/**************************************************************************************************/

/**
 * @brief Print method for mirror linked list nodes. Used in printLinkedList().
 * 
 * @param pData data of the node in mirror linked list.
 */
void printNodeMirror(void* pData)
{
	/* Optional */
	GameObj* pMirror = (GameObj*) pData;
	debugObj(pMirror, "Data");	
}

/**************************************************************************************************/

/**
 * @brief Clean data method for mirror linked list nodes. Used in freeLinkedList().
 * 
 * @param pData data of the node in mirror linked list.
 */
void cleanNodeMirror(void* pData)
{
	/* Optional */
	GameObj* pMirror = (GameObj*) pData;
	debugObj(pMirror, "Clean Mirror Data");
	free(pMirror);
}

/**************************************************************************************************/
/* Log Linked List Related Functions													    	  */
/**************************************************************************************************/

/**
 * @brief Print method for log linked list nodes. Used in printLinkedList().
 * 
 * @param pData data of the node in log linked list.
 */
void printNodeLog(void* pData)
{
#ifdef DEBUG

	/* Declarations */	
	RefreshMapParam oRP;
	NodeData* pNodeData = (NodeData*) pData;
	printf("Log Data: %p\n", (void*)pNodeData);
	
	packRefreshParams(&oRP, pNodeData->pMapInfo, 
						NULL, NULL, NULL,
						NULL, NULL, NULL, FALSE);	
	printAndStoreMap(&oRP);

#endif
}

/**************************************************************************************************/

/**
 * @brief Clean data method for log linked list nodes. Used in freeLinkedList().
 * 
 * @param pData data of the node in log linked list.
 */
void cleanNodeLog(void* pData)
{	
	NodeData* pNodeData = (NodeData*) pData;
#ifdef DEBUG	
	printf("Clean Log Data: %p\n", (void*)pNodeData);	
#endif
	destroyMap(pNodeData->pMapInfo);
	pNodeData->pMapInfo = NULL;
	free(pNodeData);	
}

/**
 * @brief Read mirros from the configuration file and add them to a linked list.
 * 
 * @param configFilePtr configuration file (FILE*)
 * @param pMirrorList linked list to add mirror objects.
 * @param pMapInfo map object (struct MapInfo).
 * @param pEnemy enemy object.
 * @param pPlayer player object.
 * @return int success status.
 */
static int addMirrorsToList(FILE* configFilePtr, LinkedList* pMirrorList, 
					MapInfo* pMapInfo, GameObj* pEnemy, GameObj* pPlayer)
{	
	/* Declarations: linked list debug prints */
	int nodeCounter = 0;
	char prefix[50];
	
	int isSuccess = TRUE;
	int nItems;
	int row, col;
	char direction;
	
	do {
		nItems = fscanf(configFilePtr, "%d %d %c ", &row, &col, &direction);
		
		if (nItems == 3)
		{
			GameObj* pMirror = (GameObj*) malloc(sizeof(GameObj));
			pMirror->row = row; pMirror->col = col; pMirror->direction = direction;
			
			isSuccess = validateMirror(pMirror, pMapInfo, pPlayer, pEnemy);
			if (isSuccess)	
			{
				insertLast(pMirrorList, pMirror);
				
				/* Debug print Mirror LinkedList */
				nodeCounter++;		
				sprintf(prefix, "Mirror: Insert_%2d", nodeCounter);
				debugLinkedList(pMirrorList, prefix);
			}
			else
			{
				free(pMirror);
			}
		}
		
	} while(isSuccess && nItems == 3);
	
	printInfo("\n--------------------------\n");
	printInfo("Print Mirror Linked List\n");
	printInfo("----------------------------\n");
	printLinkedList(pMirrorList, &printNodeMirror);
	printInfo("----------------------------\n");	
	
	return isSuccess;
}

/**************************************************************************************************/
/* Game Init/Exit Related Methods														   		  */
/**************************************************************************************************/

/**
 * @brief Parse command line arguments and export the necessary variables.
 * 
 * 
 * @param argc command line args count.
 * @param argv command line args strings.
 * @param pzConfigFileName export variable for configuration file name (input file).
 * @param pLogFile export variable for log file object - output file (struct FileEx).
 * @return int success status.
 */
int parseCmdArgs(int argc, char** argv, const char** pzConfigFileName, FileEx* pLogFile)
{
	int success = FALSE;
	
	if (argc != 3)
	{
		printf("Correct Usage:\n");
        printf("%s <input_filename> <output_filename>\n", argv[0]);		
	}
	else
	{
		*pzConfigFileName = argv[1];
		pLogFile->fptr = NULL;
		pLogFile->zFileName = argv[2];
		success = TRUE;
	}	
	
	return success;
}

/**************************************************************************************************/
/**
 * @brief Read the fixed (mandatory configs) from the file.
 * 
 * @param pCfgFile configuration file object - input file (FILE*).
 * @param pRows export variable for map/canvas number of rows.
 * @param pCols export variable for map/canvas number of columns.
 * @param pPlayer export variable for player object.
 * @param pEnemy export variable for enemy object.
 * @return int success status.
 */
static int readFixedConfigs(FILE* pCfgFile, int* pRows, int* pCols, 
										GameObj* pPlayer, GameObj* pEnemy)
{
	int isSuccess = TRUE;

	int nItems = fscanf(pCfgFile, "%d %d ", pRows, pCols);
	isSuccess = isSuccess && (nItems == 2);
	nItems = fscanf(pCfgFile, "%d %d %c ", &(pPlayer->row), &(pPlayer->col), &(pPlayer->direction));
	isSuccess = isSuccess && (nItems == 3);
	nItems = fscanf(pCfgFile, "%d %d %c ", &(pEnemy->row), &(pEnemy->col), &(pEnemy->direction));
	isSuccess = isSuccess && (nItems == 3);	

	return isSuccess;
}

/**************************************************************************************************/
/**
 * @brief Initialize game elements (map, players, mirror, log, etc).
 * 
 * @param pCfgFile configuration file - input file (FILE*).
 * @param rows map/canvas number of rows.
 * @param cols map/canvas number of columns.
 * @param ppMapInfo export variable for map object (struct MapInfo).
 * @param pEnemy export variable for enemy object.
 * @param pPlayer export variable for player object.
 * @param ppMirrorList export variable for enemy mirror linked list.
 * @param ppLogList export variable for log linked list.
 * @return int success status.
 */
static int initGameElements(FILE* pCfgFile, int rows, int cols, 
								MapInfo** ppMapInfo, 
								GameObj* pEnemy, 
								GameObj* pPlayer, 
								LinkedList** ppMirrorList, 
								LinkedList** ppLogList)
{
	RefreshMapParam oRP;
	int isSuccess = TRUE;
	*ppMapInfo = createMap(rows, cols);
		
	/* Check if tanks in bounds */
	isSuccess = validateObjBounds(*ppMapInfo, pPlayer, "Player out of bounds.\n") && 
				validateObjBounds(*ppMapInfo, pEnemy, "Enemy out of bounds.\n");
				
	if (isSuccess)
	{
		*ppMirrorList = createLinkedList();
		*ppLogList = createLinkedList();
				
		/* Read mirrors from the file to a LinkedList.
		   Variable configs in the file will be read. */
		isSuccess = addMirrorsToList(pCfgFile, *ppMirrorList, *ppMapInfo, pEnemy, pPlayer);		
	}

	/* Place the elemnts on the map before validating tanks with mirrors */
	packRefreshParams(&oRP, *ppMapInfo, pEnemy, pPlayer, NULL, 
                                    *ppMirrorList, *ppLogList, NULL, FALSE);
	refreshMapEx(&oRP, FALSE);
	isSuccess = isSuccess && validateTanks(*ppMapInfo, pEnemy, pPlayer);

	return isSuccess;
}

/**************************************************************************************************/
/**
 * @brief Destroy game elements (map, mirror, log).
 * 
 * @param ppMapInfo map object (struct MapInfo).
 * @param ppMirrorList mirrors linked list.
 * @param ppLogList log linked list.
 */
static void destroyGameElements(MapInfo** ppMapInfo, 
								LinkedList** ppMirrorList, 
								LinkedList** ppLogList)
{
	if (*ppMapInfo)		
		destroyMap(*ppMapInfo);
	
	if (*ppMirrorList)		
		freeLinkedList(*ppMirrorList, &cleanNodeMirror);
				
	if (*ppLogList)		
		freeLinkedList(*ppLogList, &cleanNodeMirror);
	
	*ppMapInfo = NULL;
	*ppMirrorList = NULL;
	*ppLogList = NULL;	
} 

/**************************************************************************************************/
/**
 * @brief Initialize the game enviornment. Fails if validations fail.
 * 
 * @param zCfgFileName configuration file name (input file).
 * @param ppMapInfo export variable for map object (struct MapInfo).
 * @param pEnemy export variable for enemy object.
 * @param pPlayer export variable for player object.
 * @param ppMirrorList export variable for enemy mirror linked list.
 * @param ppLogList export variable for log linked list.
 * @return int success status.
 */
int initGame(const char* zCfgFileName, MapInfo** ppMapInfo, 
				GameObj* pEnemy, GameObj* pPlayer, 
				LinkedList** ppMirrorList, LinkedList** ppLogList)
{
	int rows, cols, isSuccess = TRUE;
	FILE* pCfgFile = NULL;
	
	*ppMapInfo = NULL;
	*ppMirrorList = NULL;
	*ppLogList = NULL;
	
	/* Open the file to read the configs */
	pCfgFile = fopen(zCfgFileName, "r");
	isSuccess = (pCfgFile != NULL);
	if (!isSuccess)
	{
		printError("Input file cannot be opened: "); printf("%s\n", zCfgFileName);
	}

	/* Reading fixed configs from file */
	isSuccess = isSuccess && readFixedConfigs(pCfgFile, &rows, &cols, pPlayer, pEnemy);

	/* If fixed file config reading is success */
	if (isSuccess)	
		isSuccess = initGameElements(pCfgFile, rows, cols, 
										ppMapInfo, 
										pEnemy, 
										pPlayer, 
										ppMirrorList, 
										ppLogList);
	if (!isSuccess)	
		destroyGameElements(ppMapInfo, ppMirrorList, ppLogList);
	
	if (pCfgFile)
		fclose(pCfgFile);

	return isSuccess;
}

/**************************************************************************************************/
/**
 * @brief Exit from the game enviornment. Destroy any dynamic allocations in the 
 * game enviornment.
 * 
 * @param pMapInfo map object (struct MapInfo).
 * @param pMirrorList mirror linked list.
 * @param pLogList log linked list.
 */
void exitGame(MapInfo* pMapInfo, LinkedList* pMirrorList, LinkedList* pLogList)
{
		/* Destroy the map in main() */
		destroyMap(pMapInfo);

		printInfo("\n----------------------------\n");
		printInfo("Print Log Linked List\n");
		printInfo("----------------------------\n");
		printLinkedList(pLogList, &printNodeLog);
		printInfo("----------------------------\n");
		
		printInfo("\n----------------------------\n");
		printInfo("Clean Log Linked List\n");
		printInfo("----------------------------\n");
		freeLinkedList(pLogList, &cleanNodeLog);
		printInfo("----------------------------\n");
		
		printInfo("\n----------------------------\n");
		printInfo("Clean Mirror Linked List\n");
		printInfo("----------------------------\n");
		freeLinkedList(pMirrorList, &cleanNodeMirror);
		printInfo("----------------------------\n");
}

