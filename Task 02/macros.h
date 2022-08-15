#ifndef MACROS_H
#define MACROS_H

#define FALSE 0
#define TRUE !FALSE
#define BETWEEN(min,max,value) ((value <= max && value >= min) ? TRUE : FALSE)

/* Markers */
#define MARKER_BORDER       '*'
#define MARKER_FACE_LEFT    '<'
#define MARKER_FACE_RIGHT   '>'
#define MARKER_FACE_UP      '^'
#define MARKER_FACE_DOWN    'v'
#define MARKER_FACE_BMIRROR '\\'
#define MARKER_FACE_FMIRROR '/'

/* Direction */
#define DIR_LEFT    'l'
#define DIR_RIGHT   'r'
#define DIR_UP      'u'
#define DIR_DOWN    'd'
#define DIR_F       'f' /* foward mirror */
#define DIR_B       'b' /* backward mirror */

/* Keyboard Controls */
#define KEY_LEFT    'a'
#define KEY_RIGHT   'd'
#define KEY_UP      'w'
#define KEY_DOWN    's'
#define KEY_SHOOT   'f'
#define KEY_LOG     'l'

/* Game status */
typedef enum {PLAYER_HIT, ENEMY_HIT, PROGRESSING, SAVE_ERROR} GameStatus;

/* Terminal Colors */
#define LIGHT_GREEN "\033[38;5;0;48;5;194m"
#define BRIGHT_RED  "\033[48;5;1m"
#define CLEAN       "\033[0m"



#endif