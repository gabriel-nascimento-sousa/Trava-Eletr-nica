#ifndef KEY_BOARD_H
#define KEY_BOARD_H

#include "def_principais.h"

#define LIN PIND
#define COL PORTD

//const progmem salva memoria flash
const unsigned char keybord[4][4] PROGMEM = { 
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};

unsigned char leTeclado();

#endif