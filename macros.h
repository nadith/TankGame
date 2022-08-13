#ifndef MACROS_H
#define MACROS_H

#define FALSE 0
#define TRUE !FALSE

#define IDX_MAP_NROW 0
#define IDX_MAP_NCOL 1

#define ARR_SIZE_OBJ 3

/* Index for TANK ROW, TANK COL, TANK DIRECTION */
#define IDX_ROW 0
#define IDX_COL 1
#define IDX_DIR 2

#define MARKER_BORDER *

/* Index for refresh params */
#define IDX_RP_MAP          0
#define IDX_RP_MAP_SIZE     1
#define IDX_RP_ENEMY        2
#define IDX_RP_PLAYER       3
#define IDX_RP_BULLET        4

/* Game status */
#define GS_PLAYER_HIT   0 
#define GS_ENEMY_HIT    1
#define GS_NONE_HIT     2 

/* highlighting colours: */
#define LIGHT_GREEN "\033[38;5;0;48;5;194m"
#define BRIGHT_RED  "\033[48;5;1m"
#define CLEAN       "\033[0m"

#define BETWEEN(min,max,value) ((value <= max && value >= min) ? TRUE : FALSE)

#endif