#ifndef EEPROM_H
#define EEPROM_H

#include "def_principais.h"

void EEPROM_escrita(unsigned int uiadress, unsigned char ucdata);
unsigned char EEPROM_leitura(unsigned int uiadress);

#endif
