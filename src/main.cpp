#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Dimmer.h"

// Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);

// --- Mapeamento de Hardware ---
#define BUZZER     11                               //BUZZER D5
#define LEDVERDE   25                              //LEDVERDE A2
#define LEDAMARELO 24                              //LEDVERDE A1
#define BUTTON     A0                              //Pino analógico A0  BOTAO 


// --- Protótipo das Funções Auxiliares ---
void buttonread()
void changeMenu();                                      //Função para modificar o menu atual
void dispMenu();                                        //Função para mostrar o menu atual
void data_hora();                                       //Função do menu1, data e hora
void temperatura();                                     //Função do menu2, temperatura
void lights();                                          //Função do menu3, acionamento de lampadas
void menu4();                                           //Função do menu4


void dispSubMenu4();                                    //Função do sub menu4  
void readSelect(char option);                           //Função de Leitura do botão select para seleção de subMenus
void subMenu4_1();                                      //Função para subMenu4_1
void subMenu4_2();                                      //Função para subMenu4_2
void subMenu4_3();                                      //Função para subMenu4_3




// --- Variáveis Globais ---
char menu = 0x01;                                       //Variável para selecionar o menu
char set1 = 0x00, set2 = 0x00;                          //Controle das lâmpadas
boolean t_butUp, t_butDown, t_butR, t_butL, t_butS , t_butver , t_buthor, t_buttselec;             //Flags para armazenar o estado dos botões


 // --- Configurações Iniciais ---
void setup()
{
 lcd.begin (16,2);

  pinMode(BUZZER, OUTPUT);                               //Configura saída para BUZZER
  pinMode(LEDVERDE, OUTPUT);                             //Configura saída para LEDVERDE
  pinMode(LEDAMARELO, OUTPUT);                           //Configura saída para LEDAMARELO
  digitalWrite(BUZZER, LOW);                              //BUZZER apagada
  digitalWrite(LEDVERDE, LOW);                            //LEDVERDE apagada
  digitalWrite(LEDAMARELO, LOW);                          //LEDVERDE apagada



   t_butver  = 0x00;                                      //limpa flag do botão vertical
   t_buthor   = 0x00;                                      //limpa flag do botão horizontal
   t_buttselec   = 0x00;                                    //limpa flag do select e todos 
  




}   //end setup
 
void loop()
{
   buttonread();




  lcd.setCursor(0,0);
  lcd.print("********************");
  lcd.setCursor(2,1);
  lcd.print("Hello World");
  
  changeMenu();
  dispMenu();



}
  // --- Desenvolvimento das Funções Auxiliares ---
void buttonread()
{
 
  int pos = analogRead(buttons);        // Lê o valor do potenciômetro (de 0 a 1023)
  
  if (pos >= 69 && pos <= 230) t_butUp   = 0x01 ;      // Botão UP :  

  if (pos >= 231 && pos <= 407) t_butDown = 0x01;   // Botão Down : 

    if (pos > 897 && t_butUp)                    //Botão Up solto e flag setada?
   {                                                    //Sim...
      t_butUp = 0x00;                                   //Limpa flag
      
      lcd.clear();                                     //Limpa display
      menu++;                                           //Incrementa menu
      
      if(menu > 0x04) menu = 0x01;                      //Se menu maior que 4, volta a ser 1
   
   } //end butUp
   
if(pos > 897 && t_butDown)                //Botão Down solto e flag setada?
   {                                                    //Sim...
      t_butDown = 0x00;                                 //Limpa flag
      
      lcd.clear();                                     //Limpa display
      menu--;                                           //Decrementa menu
      
      if(menu < 0x01) menu = 0x04;                      //Se menu menor que 1, volta a ser 4
   
   } //end butDown



 




  if (pos < 69) t_buthor--;                      // Botão RIGHT: 

  if (pos >= 408 && pos <= 630) t_buthor++;      // Botão LEFT: 

  if (pos >= 231 && pos <= 407) t_butS += delta;      // Botão Select : 
  
  
  
  // Esperar para ler o botão novamente
  delay(50);
  }







// --- Desenvolvimento das Funções Auxiliares ---
void changeMenu()                                       //Modifica o menu atual
{
   if(!digitalRead(butUp))   t_butUp   = 0x01;          //Botão Up pressionado? Seta flag
   if(!digitalRead(butDown)) t_butDown = 0x01;          //Botão Down pressionado? Seta flag
      
   if(digitalRead(butUp) && t_butUp)                    //Botão Up solto e flag setada?
   {                                                    //Sim...
      t_butUp = 0x00;                                   //Limpa flag
      
      disp.clear();                                     //Limpa display
      menu++;                                           //Incrementa menu
      
      if(menu > 0x04) menu = 0x01;                      //Se menu maior que 4, volta a ser 1
   
   } //end butUp
   
   if(digitalRead(butDown) && t_butDown)                //Botão Down solto e flag setada?
   {                                                    //Sim...
      t_butDown = 0x00;                                 //Limpa flag
      
      disp.clear();                                     //Limpa display
      menu--;                                           //Decrementa menu
      
      if(menu < 0x01) menu = 0x04;                      //Se menu menor que 1, volta a ser 4
   
   } //end butDown   

} //end changeMenu
