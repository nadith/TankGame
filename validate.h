#ifndef VALIDATE_H
#define VALIDATE_H

/* Helper Methods */
int isFacingPlayer(int pEnemy[], int pPlayer[], int* pBulletStCell);
int isObjOverlap(int pTank1[], int pTank2[]);

/* Validation Methods */
int validateDirection(char direction);
int validateObjBounds(int pMapSize[], int pTank[], char* zMsg);
int validateTanks(int* pEnemy, int* pPlayer);

#endif