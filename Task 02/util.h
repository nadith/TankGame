#ifndef UTIL_H
#define UTIL_H

#include "map.h"
#include "linkedlist.h"

/* RefreshParams related methods */
void packRefreshParams(RefreshMapParam* pRP, MapInfo* pMapInfo, 
				GameObj* pEnemy, GameObj* pPlayer, GameObj* pBullet,
				LinkedList* pMirrorList, LinkedList* pLogList, 
				FileEx* pLogFile, int isStoreMap);

void unpackRefreshParams(RefreshMapParam* pRP, MapInfo** ppMapInfo, 
				GameObj** ppEnemy, GameObj** ppPlayer, GameObj** ppBullet,
				LinkedList** ppMirrorList, LinkedList** ppLogList, 
				FileEx** ppLogFile, int* piIsStoreMap);


/* Object (enemy, player, bullet, etc) related methods	*/
void updateObj(GameObj* pObj, int row, int col, int dir);
void copyObj(GameObj* pDestObj, GameObj* pSrcObj);

/* Debug related methods */
void toString(GameObj* pObj, char zObjStr[]);
void debugObj(GameObj* pObj, const char* zPrefix);
void debugMap(MapInfo* pMapInfo, const char* zPrefix);
void debugRefreshMapParams(RefreshMapParam* pRP, char* zPrefix);
void debugLinkedList(LinkedList* pList, char* prefix);

/* Color Related */
void red(char ch);
void green(char ch);

/* Debug Prints */
void printError(char *zStr);
void printInfo(char *zStr);

#endif