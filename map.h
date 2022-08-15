#ifndef MAP_H
#define MAP_H

/* Map Managment Methods */
char** createMap(int pMapSize[]);
void destroyMap(char** map, int pMapSize[]);
void resetMap(char** map, int pMapSize[]);
void placeObj(char** map, int pTank[]);

/* Map Display Methods */
void refreshMap(void* pRefreshParams[]);
void pauseAndRefreshMap(void* pRefreshParams[]);

#endif