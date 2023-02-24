#include "keyboard.h"

/* Função de leitura do teclado*/
unsigned char leTeclado()
{
    unsigned char key=0xFF;
    /*Varredura de colunas*/ 
    for (int j = 0; j < 4; j++)
    {
        COL &=~(1<<j);
        _delay_ms(10);
        for (int i = 0; i < 4; i++)
        {
            if (!((PIND>>4)&(1<<i)))
            {
                key = pgm_read_byte(&keybord[i][j]);
            }
        }
        COL |= (1<<j);
    }
    return key;
}