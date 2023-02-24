#include "eeprom.h"

void EEPROM_escrita(unsigned int endereco, unsigned char dado)
{
    while(EECR &(1<<EEPE));
    EEAR = endereco;
    EEDR = dado;
    EECR |=(1<<EEMPE);
    EECR |=(1<<EEPE); 
}
unsigned char EEPROM_leitura(unsigned int endereco)
{
    while(EECR &(1<<EEPE));
    EEAR = endereco;
    EECR |= (1<<EERE);
    return EEDR;
}