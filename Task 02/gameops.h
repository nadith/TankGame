#ifndef GAMEOPS_H
#define GAMEOPS_H

#include "map.h"

/* Helper Methods */
void readUserInput(char* pcUserInput);
GameStatus mainLoop(RefreshMapParam* pRP);
void processGameStatus(GameStatus gameStatus);

/* Game Activities */
GameStatus turnOrMove(RefreshMapParam* pRP, char cUserInput);
GameStatus shoot(RefreshMapParam* pRP);
GameStatus save(RefreshMapParam* pRP);

#endif