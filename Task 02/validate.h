#ifndef VALIDATE_H
#define VALIDATE_H

/* Helper Methods */
int isFacingPlayer(GameObj* pEnemy, GameObj* pPlayer, 
                                GameObj* pBulletStCell, MapInfo* pMapInfo);
int isObjOverlap(GameObj* pObj1, GameObj* pObj2);

/* Validation Methods */
int validateDirection(char direction);
int validateObjBounds(MapInfo* pMapInfo, GameObj* pObject, char* zMsg);
int validateTanks(MapInfo* pMapInfo, GameObj* pEnemy, GameObj* pPlayer);
int validateMirror(GameObj* pMirror, MapInfo* pMapInfo,
 							GameObj* pEnemy, GameObj* pPlayer);
#endif