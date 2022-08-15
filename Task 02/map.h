#ifndef MAP_H
#define MAP_H

#include "linkedlist.h"

/* Object Definitions */
typedef struct MapInfo
{
	char** map;
	int rows;
	int cols;
	
} MapInfo;

typedef struct GameObj
{
	int row;
	int col;
	char direction;
} GameObj;

typedef struct FileEx
{
	FILE* fptr;	
	const char* zFileName;	
} FileEx;

typedef struct RefreshMapParam
{
	MapInfo* pMapInfo;
	GameObj* pPlayer;
	GameObj*	pEnemy;
	GameObj* pBullet;
	LinkedList* pMirrorList;
	LinkedList* pLogList;
	int isStoreMap;
	FileEx* pLogFile;	
} RefreshMapParam;

typedef struct NodeData
{
	MapInfo*  pMapInfo;
	/* Can have a file pointer here */
	FileEx* pLogFile;
	
} NodeData;


/* Map Managment Methods */
MapInfo* createMap(int rows, int cols);
void destroyMap(MapInfo* pMapInfo);
void resetMap(MapInfo* pMapInfo);
void placeObj(MapInfo* pMapInfo, GameObj* pObj);
void placeMirrors(MapInfo* pMapInfo, LinkedList* pMirrorList);
MapInfo* copyMapInfo(const MapInfo* pMapInfo);

/* Map Display Methods */
void printAndStoreMap(RefreshMapParam* pRP);
void refreshMap(RefreshMapParam* pRP);
void refreshMapEx(RefreshMapParam* pRP, int isPrintAndStoreMap);
void pauseAndRefreshMap(RefreshMapParam* pRP);

#endif