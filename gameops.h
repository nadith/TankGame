#ifndef GAMEOPS_H
#define GAMEOPS_H

/* Helper Methods */
void readUserInput(char* pcUserInput);
int mainLoop(void* pRefreshParams[]);
void processGameStatus(int gameStatus);

/* Game Activities */
int turnOrMove(void* pRefreshParams[], char userInput);
int shoot(void* pRefreshParams[]);


#endif