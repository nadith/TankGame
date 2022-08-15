#ifndef MACROS_H
#define MACROS_H

#define FALSE 0
#define TRUE !FALSE

#define BETWEEN(min,max,value) ((value <= max && value >= min) ? TRUE : FALSE)

/* Map Size (int [2]) Index for No. of Rows and No. of Columns */
#define IDX_MAP_NROW 0
#define IDX_MAP_NCOL 1

/* Object (Enemy, Player, Bullet - int []) Array Size */
#define ARR_SIZE_OBJ 3

/* Index for Object (Enemy, Player, Bullet) row, col, direction */
#define IDX_ROW 0
#define IDX_COL 1
#define IDX_DIR 2

/* Markers */
#define MARKER_BORDER       '*'
#define MARKER_FACE_LEFT    '<'
#define MARKER_FACE_RIGHT   '>'
#define MARKER_FACE_UP      '^'
#define MARKER_FACE_DOWN    'v'

/* Direction */
#define DIR_LEFT    'l'
#define DIR_RIGHT   'r'
#define DIR_UP      'u'
#define DIR_DOWN    'd'

/* Keyboard Controls */
#define KEY_LEFT    'a'
#define KEY_RIGHT   'd'
#define KEY_UP      'w'
#define KEY_DOWN    's'
#define KEY_SHOOT   'f'

/* Index for Refresh Params */
#define IDX_RP_MAP          0
#define IDX_RP_MAP_SIZE     1
#define IDX_RP_ENEMY        2
#define IDX_RP_PLAYER       3
#define IDX_RP_BULLET        4

/* Game Status */
#define GS_PLAYER_HIT   0 
#define GS_ENEMY_HIT    1
#define GS_NONE_HIT     2 

/* Terminal Colors */
#define LIGHT_GREEN "\033[38;5;0;48;5;194m"
#define BRIGHT_RED  "\033[48;5;1m"
#define CLEAN       "\033[0m"

#endif