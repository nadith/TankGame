#ifndef UTIL_H
#define UTIL_H

/* RefreshParams related methods */
void packRefreshParams(void* pRefreshParams[], char** map, int* pMapSize, 
							int* pEnemy, int* pPlayer, int* pBullet);
void unpackRefreshParams(void* pRefreshParams[], char*** map, int** ppMapSize, 
							int** ppEnemy, int** ppPlayer, int** ppBullet);

/* Object (enemy, player, bullet, etc) related methods	*/
void updateObj(int* pObj, int val1, int val2, int val3);
void copyObj(int pDestObj[], int pSrcObj[]);
int isObjInitialized(int pObj[]);
void resetObj(int pObj[]);

/* Debug related methods */
void toString(int pObj[], char zObjStr[]);
void debugObj(int pObj[], const char* zPrefix);
void debugMap(char** map, int* pMapSize, const char* zPrefix);
void debugRefreshMapParams(void* pRefreshParams[], char* zPrefix);

/* Color Related */
void red(char ch);
void green(char ch);

/* Debug Prints */
void printError(char *zStr);

#endif