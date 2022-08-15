#ifndef ENVINIT_H
#define ENVINIT_H

#include "map.h"
#include "linkedlist.h"

/* Initialization / Deinitialization Methods */
int parseCmdArgs(int argc, char** argv, 
                    const char** pzCfgFileName, FileEx* pLogFile);

int initGame(const char* zCfgFileName, MapInfo** ppMapInfo, 
				GameObj* pEnemy, GameObj* pPlayer, 
				LinkedList** ppMirrorList, LinkedList** ppLogList);

void exitGame(MapInfo* pMapInfo, LinkedList* pMirrorList, LinkedList* pLogList);

#endif