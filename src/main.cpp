#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Dimmer.h"
#include <stdio.h>
#include "Nanoshield_RTC.h"
#include <Nanoshield_ADC.h>

// Inicializa o display no endereco 0x27
LiquidCrystal_I2C disp(0x27,2,1,0,4,5,6,7,3, POSITIVE);



// 16-bit ADC (change to Nanoshield_ADC12 to use the 12-bit version)
Nanoshield_ADC adc;



// Use channel 
int channel10 = 0;
int channel11 = 1;
int channel12 = 2;
int channel13 = 3;

int channel20 = 0;
int channel21 = 1;
int channel22 = 2;
int channel23 = 3;

int channel30 = 0;
int channel31 = 1;
int channel32 = 2;
int channel33 = 3;






Nanoshield_RTC rtc;
// ---  Clock           ---
#define BUFFER_SIZE 50
char buf[BUFFER_SIZE];
char date[11];
char time[9];
char itime[20];

// --- Mapeamento de Hardware ---
#define BUZZER     11                               //BUZZER D5
#define LEDVERDE   25                              //LEDVERDE A2
#define LEDAMARELO 24                              //LEDVERDE A1
#define BUTTON     A0                              //Pino analógico A0  BOTÃO 


// --- Protótipo das Funções Auxiliares ---

void MainreadSelect(char option);                           //Função de Leitura do botão select para seleção de subMenus   
void changeMainMenu();                                      //Função para modificar o menu atual
void dispMainMenu();                                        //Função para mostrar o menu atual

void Monitoramento();                                     //Função do menu1
void subMonitoramento();




void changedata_horaMenu();
void data_hora();                                       //Função do menu2, data e hora







 
// --- Variáveis Globais ---
int ano,mes,dia, hora,minuto;                                     // Set date and time
char menu = 0x01;                                                           //Variável para selecionar o menu
char submenuhora = 0x00;                                                       //Variável para selecionar subMenu no mmenurelogio
boolean t_butUp, t_butDown, t_butRight, t_butLeft, t_butSelect,t_butSelectwhile;           //Flags para armazenar o estado dos botões
                                                         


 // --- Configurações Iniciais ---
void setup()
{
  disp.begin (16,2);
  // Inicializa RTC
    if (!rtc.begin()) {
    Serial.println("Failed starting RTC");
    while(true);
   };

   Serial.println("ADC Nanoshield - Continuous Mode");
   adc.begin();
  
  /* Ajuste o ganho para dois (faixa de 2.048 V) 
        para obter a resolução máxima para a faixa de 4-20 mA*/
   adc.setGain(GAIN_TWO);

  /* Definir taxa de amostragem. Os valores possíveis são:
   16 bits (Nanoshield_ADC): 8, 16, 32, 64, 128, 250, 475, 860
   Se não for um dos acima, o valor inferior mais próximo será selecionado. */
   adc.setSampleRate(250);

  // Entre no modo contínuo e comece a ler o canal 0 ao 3 no endereço i2c 1001000
   adc.setContinuous(true);
   adc.readRegister(0x48,channel10); 
   adc.readRegister(0x48,channel11);
   adc.readRegister(0x48,channel12);
   adc.readRegister(0x48,channel13);

   adc.readADC_SingleEnded(channel10);
   adc.readADC_SingleEnded(channel11);
   adc.readADC_SingleEnded(channel12);
   adc.readADC_SingleEnded(channel13);

   adc.readRegister(0x49,channel20); 
   adc.readRegister(0x49,channel21);
   adc.readRegister(0x49,channel22);
   adc.readRegister(0x49,channel23);

   adc.readADC_SingleEnded(channel20);
   adc.readADC_SingleEnded(channel21);
   adc.readADC_SingleEnded(channel22);
   adc.readADC_SingleEnded(channel23);

   adc.readRegister(0x4a,channel30); 
   adc.readRegister(0x4a,channel31);
   adc.readRegister(0x4a,channel32);
   adc.readRegister(0x4a,channel33);

   adc.readADC_SingleEnded(channel30);
   adc.readADC_SingleEnded(channel31);
   adc.readADC_SingleEnded(channel32);
   adc.readADC_SingleEnded(channel33);
  // end            -------------------------------------------------------------------------------


  
  Serial.print("Configured sample rate: ");
  Serial.print(adc.getSampleRate());
  Serial.println(" SPS");
  Serial.println("The actual sample rate will be lower because of I2C communications");

  pinMode(BUZZER, OUTPUT);                               //Configura saída para BUZZER
  pinMode(LEDVERDE, OUTPUT);                             //Configura saída para LEDVERDE
  pinMode(LEDAMARELO, OUTPUT);                           //Configura saída para LEDAMARELO
  digitalWrite(BUZZER, LOW);                              //BUZZER apagada
  digitalWrite(LEDVERDE, LOW);                            //LEDVERDE apagada
  digitalWrite(LEDAMARELO, LOW);                          //LEDVERDE apagada

   t_butUp = 0x00;                                      //limpa flag do botão menuhora
   t_butDown = 0x00;                                      //limpa flag do botão menuhora
   t_butRight = 0x00;                                      //limpa flag do botão menuhora
   t_butLeft = 0x00;                                      //limpa flag do botão menuhora
   t_butSelect = 0x00;                                    
  


}   //end setup
 
void loop()
{
 
  changeMainMenu();                                     
  dispMainMenu();                               
 



} //end loop


void changeMainMenu()  

{

  int pos = analogRead(BUTTON);        // Lê o valor do potenciômetro (de 0 a 1023)
  //if (pos >= 69 && pos <= 230) t_butUp   = 0x01 ;     // Botão UP :  pressionado? Seta flag
  //if (pos >= 231 && pos <= 407) t_butDown = 0x01;     // Botão Down : pressionado? Seta flag
  if (pos >= 631 && pos <= 896) t_butSelect = 0x01 ;        // Botão Select : pressionado? Seta flag
  if (pos < 69)  t_butRight = 0x01 ;                          // Botão RIGHT: pressionado? Seta flag
  if (pos >= 408 && pos <= 630) t_butLeft = 0x01;           // Botão LEFT: pressionado? Seta flag


    if ((pos > 897) && t_butRight)                    //Botão Up solto e flag setada?
   {                                                    //Sim...
      t_butRight = 0x00;                                   //Limpa flag
      
      disp.clear();                                     //Limpa display
      menu++;                                           //Incrementa menu
      
      if(menu > 0x02) menu = 0x01;                      //Se menu maior que 4, volta a ser 1
   
   } //end butUp
   
 if((pos > 897) && t_butLeft)                      //Botão Down solto e flag setada?
   {                                                    //Sim...
       t_butLeft = 0x00;                                 //Limpa flag
      
      disp.clear();                                     //Limpa display
      menu--;                                           //Decrementa menu

      if(menu < 0x01) menu = 0x02;                      //Se menu menor que 1, volta a ser 4
   
   } //end butDown

    


} //end changeMainMenu


void dispMainMenu()
{

  switch (menu)
  {
   case 1:
    Monitoramento();
    MainreadSelect(1);
    break;
   case 2:
    data_hora();
    MainreadSelect(2);
    break;
  }


}   //end dispMainMenu()

void MainreadSelect(char option)                            //Leitura do botão select para seleção de subMenus
{
    
   int pos = analogRead(BUTTON);                              // Lê o valor do potenciômetro (de 0 a 1023)
  if (pos >= 631 && pos <= 896) t_butSelect = 0x01 ;        // Botão Select : pressionado? Seta flag
   
   
   if((pos > 897)  && t_butSelect)                  //Botão select solto e flag setada?
   {                                                    //Sim...
        t_butSelect = 0x00;                                //Limpa flag
        
        switch(option)
        {
           case 0x01: subMonitoramento(); break;
           
           case 0x02: changedata_horaMenu(); break;
        
        } //end switch option
   
   } //end if


} //end readSelect








void Monitoramento()
{

   disp.setCursor(0,0);                                 //Posiciona cursor na coluna 1, linha 1
   disp.print("Tarefa 1");                              //Imprime mensagem
   disp.setCursor(0,1);                                 //Posiciona cursor na coluna 1, linha 2   
   disp.print("Monitoramento");                      //Mostrador (apenas ilustrativo......)  
    
   

}

void subMonitoramento()
{
  while (1)
  {
  int pos = analogRead(BUTTON);        // Lê o valor do potenciômetro (de 0 a 1023)
  
  if ((pos >= 631) && (pos <= 896)) t_butSelect = 0x01 ;        // Botão Select : pressionado? Seta flag
  
   disp.setCursor(0,0);                                 //Posiciona cursor na coluna 1, linha 1
   disp.print("entra");                              //Imprime mensagem
   disp.setCursor(0,1);                                 //Posiciona cursor na coluna 1, linha 2   
   disp.print("monitoramento");                      //Mostrador (apenas ilustrativo......)   


     if ((pos > 897) && t_butSelect)
  {  
     t_butSelect = 0x00;                                 //Limpa flag
     disp.clear();                                     //Limpa display
     asm volatile ("  jmp 0");                         //reseta uno
  }



  } //end subMenu4_1  

} //end subMonitoramento

    



   



void data_hora()                                        //Data e Hora (menu1)
{


  // Print time in the Display
  rtc.read();
  sprintf(date, "%04d/%02d/%02d", rtc.getYear(), rtc.getMonth(), rtc.getDay());
  sprintf(time, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(),rtc.getSeconds());
  disp.setCursor(0,1);                                 //Posiciona cursor na coluna 1, linha 2
  disp.print(date);
  disp.setCursor(0,0);                                 //Posiciona cursor na coluna 1, linha 2
  disp.print(time);
 
}




void changedata_horaMenu()                         //Mostra o sub menu atual para o menu 4
{




  
  
  while(1){
    int pos = analogRead(BUTTON);        // Lê o valor do potenciômetro (de 0 a 1023)
  if ((pos >= 69) && (pos <= 230)) t_butUp   = 0x01 ;     // Botão UP :  pressionado? Seta flag
  if ((pos >= 231) && (pos <= 407)) t_butDown = 0x01;     // Botão Down : pressionado? Seta flag
  if ((pos >= 631) && (pos <= 896)) t_butSelect = 0x01 ;        // Botão Select : pressionado? Seta flag
  if (pos < 69)  t_butRight = 0x01 ;                          // Botão RIGHT: pressionado? Seta flag
  if ((pos >= 408) && (pos <= 630)) t_butLeft = 0x01;           // Botão LEFT: pressionado? Seta flag

    
  
    if ((pos > 897) && t_butRight)                    //Botão Up solto e flag setada?
   {                                                    //Sim...
      t_butRight = 0x00;                                   //Limpa flag
      disp.clear();                                     //Limpa display
      
      submenuhora++;                                       //incrementa submenuhora
      
      if(submenuhora > 5) submenuhora = 0x01;                 //se maior que 3, volta a ser 1
      
   
   } //end butUP
   
  if((pos > 897) && t_butLeft)                      //Botão Down solto e flag setada?
   {                                                    //Sim...
       t_butLeft = 0x00;                                 //Limpa flag
      disp.clear();                                     //Limpa display
      
      submenuhora--;                                       //decrementa submenuhora
      
      if(submenuhora < 1) submenuhora = 0x05;                 //se menor que 1, volta a ser 3
      
    
   
   } //end butDown
  
  


  
    switch(submenuhora)                                    //Controle da variável subMenu
    {
       case 0x01:                                       //Caso 1

             if ((pos > 897) && t_butUp )                    //Botão Up solto e flag setada?
          {                                                    //Sim...
             t_butUp = 0x00;                                   //Limpa flag
             rtc.writeYear(ano);
             ano++;  
             if(ano > 2050)  ano = 2000;
          }


            if ((pos > 897) && t_butDown )                      //Botão Down solto e flag setada?
          {                                                    //Sim...
            t_butDown = 0x00;                                   //Limpa flag
            rtc.writeYear(ano);
            ano--;
            if(ano < 2020)  ano = 2050;
          }


             data_hora();
             disp.setCursor(13,0);                       //Posiciona cursor na coluna 2, linha 2   
             disp.print("ano");  

             

             break;                                     //break
       case 0x02:                                       //Caso 2


             if ((pos > 897) && t_butUp )                    //Botão Up solto e flag setada?
          {                                                    //Sim...
             t_butUp = 0x00;                                   //Limpa flag
             rtc.writeMonth(mes);
             mes++;
             if(mes > 12) mes = 1;   
          }


            if ((pos > 897) && t_butDown )                      //Botão Down solto e flag setada?
          {                                                    //Sim...
            t_butDown = 0x00;                                   //Limpa flag
            rtc.writeMonth(mes);
            mes--;
            if(mes < 1) mes = 12;  
          }


             data_hora();
             disp.setCursor(13,0);                       //Posiciona cursor na coluna 2, linha 2   
             disp.print("Mes");                 
            
             
             break;                                     //break
       case 0x03:                                       //Caso 2

           if ((pos > 897) && t_butUp )                    //Botão Up solto e flag setada?
          {                                                    //Sim...
             t_butUp = 0x00;                                   //Limpa flag
             rtc.writeDay(dia);
             dia++; 
             if(dia > 31) dia = 1;  
          }


            if ((pos > 897) && t_butDown )                      //Botão Down solto e flag setada?
          {                                                    //Sim...
            t_butDown = 0x00;                                   //Limpa flag
            rtc.writeMonth(dia);
            dia--;  
            if(dia < 1) dia = 31;  
          }

          
             data_hora();
             disp.setCursor(13,0);                       //Posiciona cursor na coluna 2, linha 2   
             disp.print("Dia");   
                         
             
             break;                                     //break
       case 0x04:                                       //Caso 2

            if ((pos > 897) && t_butUp )                    //Botão Up solto e flag setada?
          {                                                    //Sim...
            t_butUp = 0x00;                                   //Limpa flag
            rtc.writeHours(hora);
            hora++;  
            if(hora > 23) hora = 0;  
          }


            if ((pos > 897) && t_butDown )                      //Botão Down solto e flag setada?
          {                                                    //Sim...
            t_butDown = 0x00;                                   //Limpa flag
            rtc.writeHours(hora);
            hora--; 
            if(hora < 0) hora = 23;      
          }
              
             data_hora();
             disp.setCursor(12,0);                       //Posiciona cursor na coluna 2, linha 2   
             disp.print("Hora");   
                        
             
             break;                                 //break
       case 0x05:                                       //Caso 2


            if ((pos > 897) && t_butUp )                    //Botão Up solto e flag setada?
          {                                                    //Sim...
            t_butUp = 0x00;                                   //Limpa flag
            rtc.writeMinutes(minuto);
            minuto++;     
           if(minuto > 60) minuto = 0;     
          }


            if ((pos > 897) && t_butDown )                      //Botão Down solto e flag setada?
          {                                                    //Sim...
            t_butDown = 0x00;                                   //Limpa flag
            rtc.writeMinutes(minuto);
            minuto--;    
            if(minuto < 0) minuto = 60;        
          }
              
             data_hora();

             disp.setCursor(10,0);                       //Posiciona cursor na coluna 2, linha 2   
             disp.print("Minuto");   
                      
             
             break;              
        
    
    } //end switch menu

     if ((pos > 897) && t_butSelect)
  {  
     t_butSelect = 0x00;                                 //Limpa flag
     disp.clear();                                     //Limpa display
     asm volatile ("  jmp 0");                         //reseta uno
  }
   




 } //end while
} //end dispMenu




