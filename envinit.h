#ifndef ENVINIT_H
#define ENVINIT_H

/* Initialization / Deinitialization Methods */
int initGame(char*** map, int pMapSize[], 
            int pEnemy[], int pPlayer[],
            char** argv, int argc);

void exitGame(char** map, int pMapSize[]);

#endif