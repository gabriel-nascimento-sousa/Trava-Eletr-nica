/*01. Elaborar um programa para um controle de acesso por senha numérica. 
A senha pode conter 3 dígitos. Toda vez que uma tecla for pressionada, um 
pequeno alto-falante deve ser acionado. Quando a senha for correta, o relé 
deve ser ligado por um pequeno tempo (utilize um LED de sinalização). 
Preveja que a senha possa ser alterada e salva na EEPROM. O circuito abaixo 
exemplifica o hardware de controle, mas deverá ser modificado de acordo com 
a necessidade, por exemplo, na inclusão de um LCD para visualização da senha digitada. 

Características específicas:
- Deve existir uma senha mestre gravada na memória FLASH para permitir a inclusão de uma 
nova senha de usuário.
- As senhas de usuário (4 senhas de 3 dígitos) devem ser gravadas na memória EEPROM.
- A senha de usuário só poderá ser apagada da EEPROM, através da senha mestre. 
- Deve-se apagar somente a senha desejada.
- Deve ser sinalizado quando a senha digitada estiver errada.
*/

#include "def_principais.h"
#include "lcd.h"
#include "eeprom.h"
#include "keyboard.h"

#define LIN PIND
#define COL PORTD

//VARIAVEIS GLOBAIS//
const unsigned char mestre[] PROGMEM = {'7', '7', '7'};   //senha mestre

int i=0;                                                  //contador para a quantidade de numeros nas senhas
int j=0;                                                  //contador para colocar a senha 
int cont=0;                                               //contador para a quantidade de senhas
bool lcd_lig=true;                                        //para desligar o LCD quando for igual a true e ligar o LCD quando for false
unsigned char senha[3] = {'\0', '\0', '\0'};              //recebe todas as senhas para fazer as leituras e validações
unsigned char senha1[3] = {'\0', '\0', '\0'};             //salvar a senha 1
unsigned char senha2[3] = {'\0', '\0', '\0'};             //salvar a senha 2
unsigned char senha3[3] = {'\0', '\0', '\0'};             //salvar a senha 3
unsigned char senha4[3] = {'\0', '\0', '\0'};             //salvar a senha 4
unsigned char tecla;                                      //recebe o valor da tecla que foi precionada
unsigned char sim_mestre=0;                               //quando sim_mestre==1 (senha mestre foi digitada)

//BUZZER//
void buzzer()
{
  PORTB |=(1<<PB2);                                       //liga o buzzer
  _delay_ms(200);
  PORTB &= ~(1<<PB2);                                     //desliga o buzzer
}

//ESCREVE "DIGITE A SENHA"//
void digita_senha()
{
  cmd_LCD(0x01, 0);                                       //limpa o LCD
  cmd_LCD(0x80, 0);  
  escreve_LCD("Digite a senha: ");                        //escreve "Digite a senha"
}

//HIDE//
void hide()
{
  _delay_ms(50);
  escreve_LCD("*");                                       //escreve "*" no lugar do número que foi digitado
}

//ESCREVE "SENHA DELETADA"//
void senha_deletada()
{
  cmd_LCD(0x01, 0);                                       //limpa o LCD
  cmd_LCD(0x80, 0);
  escreve_LCD("SENHA DELETADA!");                         //escreve "SENHA DELETADA" 
  _delay_ms(2000);                                        //espera 2 segundos e retorna a pedir uma senha
  i=0;
}

//ESCREVE "ACESSO LIBERADO"//
void acesso_liberado()
{
  cmd_LCD(0x01, 0);                                       //limpa o LCD
  cmd_LCD(0x80, 0);                                       
  escreve_LCD("ACESSO LIBERADO!");                        //escreve "ACESSO LIBERADO"
  PORTB |=(1<<PB0);
  _delay_ms(800);
  PORTB &= ~(1<<PB0);
  _delay_ms(2000);                                        //espera 2 segundos
  cmd_LCD(0x01, 0);                                       //limpa o LCD
  cmd_LCD(0x80, 0);  
  digita_senha();                                         //escreve "Digite a senha"
}

void toque()
{
  PORTB |=(1<<PB1);                                       //liga o Led
  PORTB |=(1<<PB2);                                       //liga o buzzer
  _delay_ms(300);
  PORTB &= ~(1<<PB1);                                     //desliga o led
  PORTB &= ~(1<<PB2);                                     //desliga o buzzer
  _delay_ms(300);
}

//ACESSO NEGADO//
void acesso_negado()
{
  cmd_LCD(0x01, 0);                                       //limpa o LCD
  cmd_LCD(0x80, 0);  
  escreve_LCD("ACESSO NEGADO!");                          //escreve "ACESSO NEGADO"
  toque();
  toque();
  toque();
  cmd_LCD(0x01, 0);                                       //limpa o LCD
  cmd_LCD(0x80, 0);  
  digita_senha();                                         //escreve "Digite a senha"
}

//RECEBE A SENHA QUE FOI DIGITADA//
void ler_senha()
{
  if(tecla != 0xFF)                                                       
  { 
    if ((tecla != 'A') && (tecla != 'B')  && (tecla != 'C')  && (tecla != 'D') && (tecla != '#') && (tecla != '*'))   //se a tecla nao for nenhum caracter especial
    {
      if (i==0)                                           //primeiro digito da senha
      {
        cmd_LCD(0xC0, 0);                                 //inicia cursor na segunda linha e primeira posição do LCD
        cmd_LCD(tecla, 1);                                //escreve a tecla precionada no LCD
        cmd_LCD(0xC0, 0);                                 //mostra a tecla precionada no LCD
        buzzer();
        hide();
        senha[i] = tecla;                                 //salva a tecla precionada na memória eemprom
        _delay_ms(200);  
      }
      if (i==1)                                           //segundo digito da senha                       
      {
        cmd_LCD(0xC1, 0);                                 //inicia cursor na segunda linha e segunda posição do LCD
        cmd_LCD(tecla, 1);                                //escreve a tecla precionada no LCD
        cmd_LCD(0xC1, 0);                                 //mostra a tecla precionada no LCD
        buzzer();
        hide();
        senha[i] = tecla;                                 //salva a tecla precionada na memória eemprom
        _delay_ms(200);  
      } 
      if (i==2)                                           //terceiro digito da senha       
      {
        cmd_LCD(0xC2, 0);                                 //inicia cursor na segunda linha e terceira posição do LCD
        cmd_LCD(tecla, 1);                                //escreve a tecla precionada no LCD 
        cmd_LCD(0xC2, 0);                                 //mostra a tecla precionada no LCD
        buzzer();
        hide();
        senha[i] = tecla;                                 //Salva a tecla precionada na memória eemprom
        _delay_ms(200);
      }
      i++;                                                //incrementa 1 quando algum digito da senha é precionado (nao incrementa se caracteres especiais foram precionados)
    }
  }
}

//FUNÇÃO PRINCIPAL//
int main()
{
  DDRD = 0x0F;                                            //COLunas como entrada
  PORTD = 0xFF;                                           //habilita os pullups das colunas e coloca as linhas como saída

  DDRC |= ((1<<PC0) | (1<<PC1) | (1<<PC2) | (1<<PC3));    //4 bits de dados do LCD no PORTC 
  DDRC |= ((1<<PC5) | (1<<PC4));                          //PORTs com os pinos de controle do LCD (pino R/W em 0).
  UCSR0B = 0x00;                                          //desabilita os pinos RX e TX

  DDRB |= (1<<PB0);                                       //habilita transistor conectado ao rele
  DDRB |= (1<<PB2);                                       //habilita transistor conectado ao buzzer
  DDRB |= (1<<PB1);                                       //habilita led vermelho


  inic_LCD_4bits();                                       //inicializa o LCD
  cmd_LCD(0x80, 0);                                       //inicia cursor na primeira linha do LCD
  digita_senha();                                         //escreve "Digite a senha"

  unsigned char eeprom = EEPROM_leitura(0x00);
  cmd_LCD(eeprom,1);
  
  //LOOP INFINITO//
  while (1)
  {
    tecla = leTeclado();                                  //coloca a função 'leTeclado' dentro da variavel 'tecla'

    ler_senha();                                          //função que verifica se o teclado foi pressionado e o usuário digita uma senha

    //FUNÇÃO DA TECLA 'A' QUANDO NÃO HOUVER SENHAS SALVAS//
    if ((i>2) && (tecla == 'A') && (sim_mestre == 1) && (cont==0))      //(sim_mestre==1 significa que a senha mestre foi digitada)
    {
     cmd_LCD(0x01, 0);                                                 //tecla == 'A' significa que o ADM deseja deletar uma senha
      cmd_LCD(0x80, 0);                                                 //se cont==0 significa que nenhuma senha esta salva na memoria
      escreve_LCD("MEMORIA VAZIA!");
      _delay_ms(2000);
      digita_senha();
      sim_mestre=0;
      i=0;
    }

    //FUNÇÃO DA TECLA 'A' QUANDO HOUVER SENHAS SALVAS//
    if ((i>2) && (tecla == 'A') && (sim_mestre == 1) && (cont!=0))    //(sim_mestre==1 significa que a senha mestre foi digitada)
    {                                                                 //tecla == 'A' significa que o ADM deseja deletar uma senha
      buzzer();
      sim_mestre=0;
      i=0;
      digita_senha();
      while (tecla!='C')                            //enquando o ADM digita a senha que será deletada
      {
        tecla = leTeclado();
        ler_senha();                                //função que verifica se o teclado foi pressionado e o usuário digita uma senha
      }
      buzzer();
      if ((tecla == 'C') && (senha[0] == senha1[0]) && (senha[1] == senha1[1]) && (senha[2] == senha1[2]))            //se a senha digita for a 1, todas as suas posições recebem '\0'
      {
        senha1[0] = '\0';
        senha1[1] = '\0';
        senha1[2] = '\0';
        senha_deletada();
        digita_senha();
      } else if ((tecla == 'C') && (senha[0] == senha2[0]) && (senha[1] == senha2[1]) && (senha[2] == senha2[2])) {   //se a senha digita for a 2, todas as suas posições recebem '\0'
        senha2[0] = '\0';
        senha2[1] = '\0';
        senha2[2] = '\0';
        senha_deletada();
        digita_senha();
      } else if ((tecla == 'C') && (senha[0] == senha3[0]) && (senha[1] == senha3[1]) && (senha[2] == senha3[2])) {   //se a senha digita for a 3, todas as suas posições recebem '\0'
        senha3[0] = '\0';
        senha3[1] = '\0';
        senha3[2] = '\0';
        senha_deletada();
        digita_senha();
      } else if ((tecla == 'C') && (senha[0] == senha4[0]) && (senha[1] == senha4[1]) && (senha[2] == senha4[2])) {   //se a senha digita for a 4, todas as suas posições recebem '\0'
        senha4[0] = '\0';
        senha4[1] = '\0';
        senha4[2] = '\0';
        senha_deletada();
        digita_senha();
      }
    }

    //FUNÇÃO DA TECLA 'B'//
    if ((i>2) && (tecla == 'B') && (sim_mestre == 1))   //sim_mestre==1 significa que a senha mestre foi digitada
    {                                                   //tecla == 'B' significa que o ADM deseja salvar uma senha
      buzzer();
      i=0;
      sim_mestre = 0;
      digita_senha();
      while (tecla!='C')                            //enquando o ADM digita a senha que será deletada
      {
        tecla = leTeclado();
        ler_senha();
      }
      buzzer();
      if ((senha1[0] == '\0') && (tecla=='C')){    //salvando senha 1
        cont++;
        cmd_LCD(0x01, 0);                          //limpa o LCD
        cmd_LCD(0x80, 0);  
        escreve_LCD("SENHA 1");
        cmd_LCD(0x87, 1); 
        for (j=0; j<3; j++) 
        {
          EEPROM_escrita(j, senha[j]);            //salva o numero da posição senha[j] na posição j da eeprom
          senha1[j] = EEPROM_leitura(j);
          //cmd_LCD(senha1[j], 1);                //OBS: habilitar caso o professor queira visualizar
        }
        cmd_LCD(0x87, 0);                         //CASO O PROFF QUEIRA VIZUALIZAR MUDAR PARA 8A
        escreve_LCD(" SALVA.");
        _delay_ms(2000);
        i=0;
        digita_senha();
      } else if ((senha2[0] == '\0') && (tecla=='C')) { //salvando senha 2
        cont++;
        cmd_LCD(0x01, 0);                         //limpa o LCD
        cmd_LCD(0x80, 0);  
        escreve_LCD("SENHA 2");
        cmd_LCD(0x87, 1); 
        for (j=0; j<3; j++) 
        {
          EEPROM_escrita(j, senha[j]);
          senha2[j] = EEPROM_leitura(j);
          //cmd_LCD(senha2[j], 1);              //OBS: habilitar caso o professor queira visualizar
        }
        cmd_LCD(0x87, 0);                       //CASO O PROFF QUEIRA VIZUALIZAR MUDAR PARA 8A
        escreve_LCD(" SALVA.");
        _delay_ms(2000);
        i=0;
        digita_senha();
      } else if ((senha3[0] == '\0') && (tecla=='C')) {   //salvando senha 3
        cont++;
        cmd_LCD(0x01, 0);                       //limpa o LCD
        cmd_LCD(0x80, 0);  
        escreve_LCD("SENHA 3");
        cmd_LCD(0x87, 1); 
        for (j=0; j<3; j++) 
        {
          EEPROM_escrita(j, senha[j]);
          senha3[j] = EEPROM_leitura(j);
          //cmd_LCD(senha3[j], 1);              //OBS: habilitar caso o professor queira visualizar
        }
        cmd_LCD(0x87, 0);                       //CASO O PROFF QUEIRA VIZUALIZAR MUDAR PARA 8A
        escreve_LCD(" SALVA.");
        _delay_ms(2000);
        i=0;
        digita_senha();
      } else if ((senha4[0] == '\0') && (tecla=='C')) {   //salvando senha 4
        cont++;
        cmd_LCD(0x01, 0);                       //limpa o LCD
        cmd_LCD(0x80, 0);  
        escreve_LCD("SENHA 4");
        cmd_LCD(0x87, 1); 
        for (j=0; j<3; j++) 
        {
          EEPROM_escrita(j, senha[j]);
          senha[j] = EEPROM_leitura(j);
          //cmd_LCD(senha4[j], 1);             //OBS: habilitar caso o professor queira visualizar
        }
        cmd_LCD(0x87, 0);                      //CASO O PROFF QUEIRA VIZUALIZAR MUDAR PARA 8A
        escreve_LCD(" SALVA.");
        _delay_ms(2000);
        i=0;
        digita_senha();
      } 
    }

    //FUNÇAÕ DA TECLA 'C'//
    if ((tecla == 'C') && (i>2) && (sim_mestre == 0))   //Tecla C para confirmar QUAISQUER senha digitada (exceto a senha mestre)
    {
      buzzer();                                     //função buzzer
     if ((senha[0] == senha1[0]) && (senha[1] == senha1[1]) && (senha[2] == senha1[2])) {           //Verifica se a senha 1 foi digita
        acesso_liberado();                          //escreve "ACESSO LIBERADO!"
        i=0;                                        //reinicia o vetor na posição[0]
      } else if ((senha[0] == senha2[0]) && (senha[1] == senha2[1]) && (senha[2] == senha2[2])) {   //Verifica se a senha 2 foi digitada
          acesso_liberado();
          i=0;                                      
      } else if ((senha[0] == senha3[0]) && (senha[1] == senha3[1]) && (senha[2] == senha3[2])) {   //Verifica se a senha 3 foi digitada
          acesso_liberado();
          i=0;                                      
      } else if ((senha[0] == senha4[0]) && (senha[1] == senha4[1]) && (senha[2] == senha4[2])) {   //Verifica se a senha 4 foi digitada
          acesso_liberado();
          i=0;                                     
        //NEGA ACESSO SE A SENHA DIGITADA NÃO ESTIVER SALVA
      } else if (((senha[0]!=senha1[0]) && (senha[1]!=senha1[1]) && (senha[2]!=senha1[2])) || ((senha[0]!=senha2[0]) && (senha[1]!=senha2[1]) && (senha[2]!=senha2[2])) || 
                ((senha[0]!=senha3[0]) && (senha[1]!=senha3[1]) && (senha[2]!=senha3[2])) || ((senha[0]!=senha4[0]) && (senha[1]!=senha4[1]) && (senha[2]!=senha4[2]))) {
        acesso_negado();                            //escreve "ACESSO NEGADO!"
        i=0;                                        //reinicia o vetor na posição[0]
      }
    }

    //FUNÇÃO DA TECLA 'D'//
    if (tecla=='D')                                     //se a tecla 'D' for precionada o LCD irá desligar quando 'lcd_lig == true  
    {
      if (lcd_lig==true)                                  
      {
        buzzer();                                        //função buzzer
        cmd_LCD(0x0A, 0);                                //apaga o LCD
        _delay_ms(200);
        lcd_lig=false;                                   
      } else {
        buzzer();                                       //função buzzer
        inic_LCD_4bits();                               //inicializa o LCD
        cmd_LCD(0x80,0);                           
        digita_senha();                                 //escreve "Digite a senha"
        i=0; 
        _delay_ms(200);
        lcd_lig=true; 
        sim_mestre=0;                                   //sim_mestre=0 para que não haja conflito se 'D' for precionado quando as funções da senha mestre estiverem habilitadas
      }
    }

    //FUNÇÃO DA TECLA '*'//
    if ((tecla =='*') && (i==1) && (sim_mestre==0))   //se a tecla '*' for precionada para deletar o primeiro digito
    {                                                 //OBS: '*' só pode ser precionado fora das funções da senha mestre
      buzzer();                                       //função buzzer
      cmd_LCD(0xC0, 0);
      _delay_ms(50);
      escreve_LCD(" ");                               //escreve um espaço vazio
      i=0;                                            //reinicia o vetor na posição[0]
    } else if ((tecla =='*') && (i==2) && (sim_mestre==0)) {    //Se a tecla '*'mfor precionada para deletar o segundo digito
      buzzer();                                       //função buzzer
      cmd_LCD(0xC1, 0);
      _delay_ms(50);
      escreve_LCD(" ");                              //escreve um espaço vazio
      i=1;                                           //reinicia o vetor na posição[1]
    } else if ((tecla == '*') && (i != 0) && (sim_mestre==0)) {   //Se a tecla '*' for precionada para deletar o terceiro digito
      buzzer();                                      //função buzzer
      cmd_LCD(0xC2, 0);
      _delay_ms(50);
      escreve_LCD(" ");                             //escreve um espaço vazio
      i=2;                                          //reinicia o vetor na posição[2]
    }

    //HABILITAÇÃO DA FUNÇAÕ MESTRE COM A TECLA '#'//
    if ((i>2) && (sim_mestre==0) && (tecla=='#') && (senha[0]==mestre[0]) && (senha[1]==mestre[1]) && (senha[2]==mestre[2]))   //hibilita a senha mestre
    {
      buzzer();
      i=0;
      sim_mestre=1;
      if (sim_mestre==1)
      {
        i=4;                                           //impossibilita que o usuário digite algo enquanto está nas      funções da senha mestre
        cmd_LCD(0x01, 0);                              //limpa o LCD
        cmd_LCD(0x80, 0);
        escreve_LCD("ACESSO DE ADM!");
        PORTB |=(1<<PB0);
        _delay_ms(2000);
        PORTB &= ~(1<<PB0);
        _delay_ms(2000);
        cmd_LCD(0x01, 0);                              //limpa o LCD
        cmd_LCD(0x80, 0);   
        escreve_LCD("A-Deletar");
        cmd_LCD(0xC0, 0);  
        escreve_LCD("B-Salvar");
      }
    }
  }
}