   /*!
  * @file PROJETO_TESTE.ino
  * @mainpage PROJETO BRAÇO ROBÓTICO
  * @section intro_sec Introdução
  * Braço Robótico Engenharia da Computação 2021.
  * @author 
  * Ailton Fernandes \n
  * Lucas Komatsu \n
  * Manuel Cassiano \n
  * Kevin Tani
  * @version 1.0
  * @since 11-2021
 */




#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>


//configuro ADC-------------------------------------------------------------
void InitADC(){                        
  ADMUX |= (1<<REFS0); 
  ADCSRA |= (1<<ADPS2) |(1<<ADPS1) |(1<<ADPS0) |(1<<ADEN); // CONFIGURANDO OS BITS PARA HABILITAR ADC E PRESCALER 128.
}
uint16_t ReadADC(uint8_t ADCchannel)
{
 
ADMUX = (ADMUX & 0xF0) | (ADCchannel & 0x0F); // SELECIONAR CANAL ADC COM MASCARA DE SEGURANCA.
ADCSRA |= (1<<ADSC); // SINGLE MODE
 
while(ADCSRA & (1<<ADSC)); // ESPERAR ATÉ A CONVERSAO ADC SER REALIZADA. 
return ADC;
}
//configuro ADC--------------------------------------------------------------


//configuro porta botoes-----------------------------------------------------
int porta_botoes(){ 
    PORTD |= (1 << 2) | (1<<4) | (1<<7); // CONFIGURO AS PORTAS QUE SERAO UTILIZADAS PELOS BOTõES.
    PORTB |= (1 << PB0);
  }
//configuro porta botoes-----------------------------------------------------


//configuro porta led--------------------------------------------------------
int porta_led(){ 
    // CONFIGURO AS PORTAS QUE SERAO UTILIZADAS PELOS LEDS.
    DDRB |= (1 << PB4); 
  }
//configuro porta led--------------------------------------------------------


//configuro PWM timer 1------------------------------------------------------
/*!
*@brief InitPWM()
*
*CONTA FEITA PARA CHEGAR NO VALOR DE TOPO CORRETO.
*Tpwm = 50hz = 16mhz / (top+1) x prescaler
*Top = 16mhz / (50 x 8) - 1
*Top = 39999
*
*/
void InitPWM(){

  DDRB |= (1<<PB1) | (1<<PB2); // HABILITA OC1A E OC1B COMO SAÍDA.
  
  ICR1 = 39999; // CONFIGURA PERIODO PWM (20ms).
  TCCR1A = 0b10100010; // FAST PWM.
  TCCR1B = 0b00011010; // PRESCALER = 8.

  OCR1A = 1100; // BASE A
  OCR1B = 2900; // BASE B

} 
//configuro PWM timer 1------------------------------------------------------


//configuro PWM timer 0------------------------------------------------------
void InitPWM_8(){
    DDRD |= (1<<DDD6) |(1<<DDD5); // HABILITA 0C0A E OC0B COMO SAÍDA.
    TCCR0A = (1<<COM0A1) |(1<<WGM01) |(1<<WGM00)|(1<<COM0B1); // FAST PWM, OC0A E OC0B.
    TCCR0B = (1<<CS02)| (1<<CS00); // SEM PRESCALER.
   
    OCR0A = 20; // CIMA
    OCR0B = 28; // BRACINHO
 
}
//configuro PWM timer 0------------------------------------------------------


//configuro PWM timer 2------------------------------------------------------
void InitPWM_8_2(){

    DDRB |= (1<<PB3);
    DDRD |= (1<<DDD3); // HABILITA 0C2A E OC2B COMO SAÍDA.
    TCCR2A = (1<<COM2A1)|(1<<COM2B1) |(1<<WGM21) |(1<<WGM20); // FAST PWM, OC2A E OC2B.
    TCCR2B = (1<<CS20) | (1<<CS21)| (1<<CS22); // SEM PRESCALER. 
   
    OCR2A = 22; // ROTACAO
    OCR2B = 4; // GARRA
 
}
//configuro PWM timer 2------------------------------------------------------


//Variáveis globais----------------------------------------------------------


///  HABILITO CONTADOR QUE AUXILIARÁ O MODO AUTOMATICO.
int contadorautomatico  = 1; 
///  HABILITO CONTADOR QUE AUXILIARÁ O MODO AUTOMATICO.
int contadorwhile = 0;
///  HABILITO CONTADOR QUE AUXILIARÁ O MODO AUTOMATICO.
int contadorconfirma = 0;

//Joystick 1
/// Eixo x DO ANALÓGICO 1.
int JoyX1=0;
/// VARIAVEL QUE IRA GUARDAR O VALOR ANALÓGICO 1. 
int valorADC=0;
/// Eixo y DO ANALÓGICO 1. 
int JoyY1=0;
/// VARIAVEL QUE IRA GUARDAR O VALOR ANALÓGICO 2. 
int valorADC1=0; 

//Joystick 2
/// Eixo x DO ANALÓGICO 2.
int JoyX2=0;
/// VARIAVEL QUE IRA GUARDAR O VALOR ANALÓGICO 3. 
int valorADC2=0;
/// Eixo y DO ANALÓGICO 2. 
int JoyY2=0;
/// VARIAVEL QUE IRA GUARDAR O VALOR ANALÓGICO 4. 
int valorADC3=0; 
//Variáveis globais------------------------------------------------------


//Main-------------------------------------------------------------------
int main(void){ 
  
// CHAMO AS FUNCÕES CRIADAS----------------------------------------------
  InitADC(); // FUNÇÃO ADC.
  porta_led(); // PORTAS DOS LEDS.
  InitPWM_8(); // PWM TIMER 0.
  InitPWM(); // PWM TIMER 1.
  InitPWM_8_2(); // PWM TIMER 2.
  porta_botoes(); // PORTAS DOS BOTÕES.
// CHAMO AS FUNCOES CRIADAS----------------------------------------------
  

//inicio do looping------------------------------------------------------

  while(1){


  valorADC = ReadADC(0);  //A0
  valorADC1 = ReadADC(1); //A1
  valorADC2 = ReadADC(2); //A2
  valorADC3 = ReadADC(3); //A3
  JoyX1 = valorADC;
  JoyY1 = valorADC1;
  JoyX2 = valorADC2;
  JoyY2 = valorADC3;

  
//Botoes do Joystick, GARRA------------------------------------------------------
  if ( (PIND &(1<<2))==0) // SE PIND2 FOR PRESSIONADO INCREMENTA O REGISTRADOR 0CR2B.
      {
       if(OCR2B<14){ // SE VALOR DA PORTA FOR MENOR QUE 40.
        OCR2B++; // INCREMENTA PWM NA PORTA.
       _delay_ms(100); // DELAY DE SEGURANÇA.
      }
    }  
  if ( (PIND &(1<<4))==0) // SE PIND4 FOR PRESSIONADO DECREMENTA O REGISTRADOR 0CR2B.
      {
      if(OCR2B>4){ // SE VALOR DA PORTA FOR MAIOR QUE 4. 
        OCR2B--; // DECREMENTA PWM NA PORTA.
       _delay_ms(100); // DELAY DE SEGURANÇA.
      }
   }

//Botoes do Joystick, GARRA-------------------------------------------------------
 

//Joystick 1----------------------------------------------------------------------
//2 SERVOS DA BASE----------------------------------------------------------------
  if(JoyX1 < 400){ // VERIFICA SE O JOYSTICK ESTÁ POSICIONADO PARA TRÁS.

    //Serial.println(OCR1A);
      if(OCR1A < 4001 && OCR1B > 999) // ESTABELECENDO LIMITE PARA O SERVO NÃO GIRAR 360º.
      {
        // SERVOS OPOSTOS, UM DEVE GIRAR NO SENTIDO HORÁRIO E O OUTRO NO SENTIDO ANTI-HORÁRIO, UM INCREMENTA E O OUTRO DECREMENTA.
        OCR1A += 60; 
        OCR1B -= 60;

      }
            _delay_ms(40); // DELAY DE SEGURANÇA.
  }
  if(JoyX1 > 800){ // VERIFICA SE O JOYSTICK ESTÁ POSICIONADO PARA FRENTE.
    if(OCR1A >999 && OCR1B < 4001) // ESTABELECENDO LIMITE PARA O SERVO NÃO GIRAR 360º.
    {
      // SERVOS OPOSTOS, UM DEVE GIRAR NO SENTIDO HORÁRIO E O OUTRO NO SENTIDO ANTI-HORÁRIO, UM INCREMENTA E O OUTRO DECREMENTA.
      OCR1A -= 60;
      OCR1B += 60;


    }
    //Serial.println(OCR1A);
            _delay_ms(40); // DELAY DE SEGURANÇA.
  }
//2 SERVOS DA BASE-----------------------------------------------------


//SERVO DE ROTAÇÃO-----------------------------------------------------
  if(JoyY1 < 400){ // VERIFICA SE O JOYSTICK ESTÁ POSICIONADO PARA ESQUERDA.

    
      if(OCR2A > 4) // ESTABELECENDO LIMITE PARA O SERVO NÃO GIRAR 360º.
      {
        OCR2A--; // DECREMENTA PWM NA PORTA.

      }
            _delay_ms(100); // DELAY DE SEGURANÇA.
  }
  if(JoyY1 > 800){ // VERIFICA SE O JOYSTICK ESTÁ POSICIONADO PARA DIREITA.
    if(OCR2A <40) // ESTABELECENDO LIMITE PARA O SERVO NÃO GIRAR 360º.
    {
      OCR2A++; // INCREMENTA PWM NA PORTA.

    }
  
            _delay_ms(100); // DELAY DE SEGURANÇA.
  }
//Joystick 1-------------------------------------------------------------


//SERVO DE ROTAÇÃO-------------------------------------------------------

//Joystick 2-------------------------------------------------------------
//SERVO DE CIMA----------------------------------------------------------
  if(JoyX2 < 400){ // VERIFICA SE O JOYSTICK ESTÁ POSICIONADO PARA TRÁS.
   
    //Serial.println(OCR0A);
      if(OCR0A > 4) // ESTABELECENDO LIMITE PARA O SERVO NÃO GIRAR 360º.
      {
        OCR0A -= 1; // DECREMENTA PWM NA PORTA.


      }
            _delay_ms(20); // DELAY DE SEGURANÇA.
  }
  if(JoyX2 > 800){ // VERIFICA SE O JOYSTICK ESTÁ POSICIONADO PARA FRENTE.
   
    if(OCR0A <40) // ESTABELECENDO LIMITE PARA O SERVO NÃO GIRAR 360º.
    {
      OCR0A += 1; // INCREMENTA PWM NA PORTA.

    }
    //Serial.println(OCR0A);
            _delay_ms(100); // DELAY DE SEGURANÇA.
  }
//SERVO DE CIMA-----------------------------------------------------------
//SERVO BRACINHO----------------------------------------------------------
  if(JoyY2 < 400){ // VERIFICA SE O JOYSTICK ESTÁ POSICIONADO PARA ESQUERDA. 

    
      if(OCR0B < 40) // ESTABELECENDO LIMITE PARA O SERVO NÃO GIRAR 360º.
      {
        OCR0B += 1; // INCREMENTA PWM NA PORTA.

      }
            _delay_ms(100); // DELAY DE SEGURANÇA.
  }
  if(JoyY2 > 800){ // VERIFICA SE O JOYSTICK ESTÁ POSICIONADO PARA DIREITA.
    if(OCR0B >4)// ESTABELECENDO LIMITE PARA O SERVO NÃO GIRAR 360º.
    {
      OCR0B -= 1; // DECREMENTA PWM NA PORTA.

    }
  
            _delay_ms(100); // DELAY DE SEGURANÇA.
  }
//SERVO BRACINHO------------------------------------------------------------
//Joystick 2----------------------------------------------------------------





//PARTE AUTOMÁTICA

if ( (PIND &(1<<7))==0){//se pind7 foi pressionado
        contadorautomatico = contadorautomatico + 1; 
        contadorwhile = 1;
        _delay_ms(100);
        if(contadorautomatico > 12){
        contadorautomatico = 1;
            }
}

while (contadorwhile == 1){
if(contadorautomatico == 1){
  PORTB |= (1 << PB4);
  _delay_ms(700);
   PORTB &= ~(1 << PB4);
   _delay_ms(500);
   contadorwhile = 0;
   }
 if(contadorautomatico == 2){
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     contadorwhile = 0;
  }
 if(contadorautomatico == 3){
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     contadorwhile = 0;
  }
 if(contadorautomatico == 4){
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     contadorwhile = 0;
  }
 if(contadorautomatico == 5){
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     contadorwhile = 0;

  }
 if(contadorautomatico == 6){
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     contadorwhile = 0;

  }
 if(contadorautomatico == 7){
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     contadorwhile = 0;

  }
 if(contadorautomatico == 8){
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     contadorwhile = 0;
  }
 if(contadorautomatico == 9){
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     contadorwhile = 0;
  }
 if(contadorautomatico == 10){
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     contadorwhile = 0;
     
  }
 if(contadorautomatico == 11){
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     contadorwhile = 0;

  }
 if(contadorautomatico == 12){
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     PORTB |= (1 << PB4);
     _delay_ms(500);
     PORTB &= ~(1 << PB4);
     _delay_ms(500);
     contadorwhile = 0;

  }


}

if ( (PINB &(1<<PB0))==0){
      contadorconfirma = 1;
     
    }  

while(contadorconfirma ==1){
    if(contadorautomatico == 1){ // 1 -> 2 ----------------------------- QUADRADO DE CIMA  
      OCR1A = 1100; // BASE A
      OCR1B = 2900; // BASE B
      OCR0A = 20; // CIMA
      OCR0B = 28; // BRACINHO
      OCR2A = 23; // ROTACAO
      OCR2B = 4;//abrir garra
      _delay_ms(150); 
      OCR2A = 22; //rotacao
      _delay_ms(1000); 
      OCR0B = 19;//bracinho
      _delay_ms(1000);
      OCR0A = 4  ;//cima
      _delay_ms(1000); 
      OCR1A = 1580;//base
      OCR1B = 2420;
      _delay_ms(1000);
      OCR1A = 1940;//base
      OCR1B = 2060;
      _delay_ms(1000);
      OCR1A = 2240;//base
      OCR1B = 1760;
      _delay_ms(1000);
      OCR2B = 14;//fechar garra ---------------- pegou
      _delay_ms(1000);
      OCR1A = 1940;//base
      OCR1B = 2060;
      _delay_ms(1000);
       OCR1A = 1580;//base
      OCR1B = 2420;
      _delay_ms(1000);
       OCR0B = 25;
       _delay_ms(500); 
       OCR0B = 33;//bracinho
      _delay_ms(1000);
        OCR1A = 1460;//base
      OCR1B = 2540;
      _delay_ms(1000);
      OCR1A = 1100;//base
      OCR1B = 2900;
      _delay_ms(1000);

        //-------------------------- deixou no meio
      OCR0A = 9; // CIMA
      _delay_ms(1000);
      OCR0B = 10; // BRACINHO
      _delay_ms(1000);
      OCR1A = 1220; // BASE A
      OCR1B = 2780; // BASE B
      _delay_ms(1000);
      OCR1A = 1400; // BASE A
      OCR1B = 2600; // BASE B
      _delay_ms(1000);
      OCR2B = 4; // GARRA ----------------------------- deixou NO MEIO
      _delay_ms(1000);
      OCR1A = 1100; // BASE A 
      OCR1B = 2900; // BASE B
      _delay_ms(1000); 
      OCR0A = 20; // CIMA
      OCR0B = 28; // BRACINHO
      OCR2A = 22; // ROTACAO
      OCR2B = 4; // GARRA
      
      contadorconfirma = 0;
    }
    if(contadorautomatico == 2){// 1 -> 3 ----------------------------- QUADRADO DE CIMA  
       OCR1A = 1100; // BASE A
      OCR1B = 2900; // BASE B
      OCR0A = 20; // CIMA
      OCR0B = 28; // BRACINHO
      OCR2A = 22; // ROTACAO
      OCR2B = 4;//abrir garra
      _delay_ms(150); 
      OCR2A = 21; //rotacao
      _delay_ms(1000); 
      OCR0B = 19;//bracinho
      _delay_ms(1000);
      OCR0A = 4  ;//cima
      _delay_ms(1000); 
      OCR1A = 1580;//base
      OCR1B = 2420;
      _delay_ms(1000);
      OCR1A = 1940;//base
      OCR1B = 2060;
      _delay_ms(1000);
      OCR1A = 2240;//base
      OCR1B = 1760;
      _delay_ms(1000);
      OCR2B = 14;//fechar garra ---------------- pegou
      _delay_ms(1000);
      OCR1A = 1940;//base
      OCR1B = 2060;
      _delay_ms(1000);
       OCR1A = 1580;//base
      OCR1B = 2420;
      _delay_ms(1000);
       OCR0B = 25;
       _delay_ms(500); 
       OCR0B = 33;//bracinho
      _delay_ms(1000);
       for(int aux = 5; aux <21; aux++){
      OCR0A = aux ;//cima
      _delay_ms(100);
      }
        OCR1A = 1460;//base
      OCR1B = 2540;
      _delay_ms(1000);
      OCR1A = 1100;//base
      OCR1B = 2900;
      _delay_ms(1000);

        //-------------------------- deixou no meio
         OCR2A = 33; // ROTACAO
         _delay_ms(1000);
      OCR0A = 9; // CIMA
      _delay_ms(1000);
       for(int aux1 = 33; aux1 >9; aux1--){
      OCR0B = aux1 ;//bracinho
      _delay_ms(100);
      }
       _delay_ms(1000);
      OCR1A = 1220; // BASE A
      OCR1B = 2780; // BASE B
      _delay_ms(1000);
      OCR1A = 1640; // BASE A
      OCR1B = 2360; // BASE B
      _delay_ms(1000);
      OCR2B = 4; // GARRA ----------------------------- deixou na esquerda
            _delay_ms(1000);
      OCR1A = 1100; // BASE A 
      OCR1B = 2900; // BASE B
      _delay_ms(1000); 
      OCR0A = 20; // CIMA
      OCR0B = 28; // BRACINHO
      OCR2A = 22; // ROTACAO
      OCR2B = 4; // GARRA

        
      
      contadorconfirma = 0;

    }
    if(contadorautomatico == 3){// 1 -> 4 ----------------------------- QUADRADO DE CIMA  
        OCR1A = 1100; // BASE A
      OCR1B = 2900; // BASE B
      OCR0A = 20; // CIMA
      OCR0B = 28; // BRACINHO
      OCR2A = 22; // ROTACAO
      OCR2B = 4;//abrir garra
      _delay_ms(150); 
      OCR2A = 21; //rotacao
      _delay_ms(1000); 
      OCR0B = 19;//bracinho
      _delay_ms(1000);
      OCR0A = 4  ;//cima
      _delay_ms(1000); 
      OCR1A = 1580;//base
      OCR1B = 2420;
      _delay_ms(1000);
      OCR1A = 1940;//base
      OCR1B = 2060;
      _delay_ms(1000);
      OCR1A = 2240;//base
      OCR1B = 1760;
      _delay_ms(1000);
      OCR2B = 14;//fechar garra ---------------- pegou
      _delay_ms(1000);
      OCR1A = 1940;//base
      OCR1B = 2060;
      _delay_ms(1000);
       OCR1A = 1580;//base
      OCR1B = 2420;
      _delay_ms(1000);
       OCR0B = 25;
       _delay_ms(500); 
       OCR0B = 33;//bracinho
      _delay_ms(1000);
       for(int aux = 5; aux <21; aux++){
      OCR0A = aux ;//cima
      _delay_ms(100);
      }
        OCR1A = 1460;//base
      OCR1B = 2540;
      _delay_ms(1000);
      OCR1A = 1100;//base
      OCR1B = 2900;
      _delay_ms(1000);

        //-------------------------- deixou no meio
         _delay_ms(1000);
      OCR2A = 16; // ROTACAO
      _delay_ms(1000);
      OCR0B = 29;//bracinho
      _delay_ms(1000);
      OCR0A = 15; // CIMA
      _delay_ms(500);
      OCR0A = 10; // CIMA
      _delay_ms(500);
      OCR0A = 7; // CIMA
      _delay_ms(1000);
      OCR1A = 1340; // BASE A
      OCR1B = 2660; // BASE B
      _delay_ms(1000);
      OCR1A = 1760; // BASE A
      OCR1B = 2240; // BASE B
            _delay_ms(1000);
      OCR1A = 2000; // BASE A
      OCR1B = 2000; // BASE B
      _delay_ms(1000);
      OCR2B = 4; // GARRA ----------------------- DEIXOU NA DIREITA
            _delay_ms(1000);
      OCR1A = 1100; // BASE A 
      OCR1B = 2900; // BASE B 
      _delay_ms(1000);
      OCR0A = 20; // CIMA
      OCR0B = 28; // BRACINHO
      OCR2A = 22; // ROTACAO
      OCR2B = 4; // GARRA
      contadorconfirma = 0;
    }
    if(contadorautomatico == 4){// 2 -> 1 ----------------------------- QUADRADO DA DIREITA
      OCR1A = 1100; // BASE A
      OCR1B = 2900; // BASE B
      OCR0A = 20; // CIMA
      OCR0B = 28; // BRACINHO
      OCR2A = 22; // ROTACAO
      OCR2B = 4; // GARRA
      _delay_ms(1000);
      OCR2A = 16; // ROTACAO
      _delay_ms(1000);
      OCR0A = 15; // CIMA
      _delay_ms(500);
      OCR0A = 10; // CIMA
      _delay_ms(500);
      OCR0A = 7; // CIMA
      _delay_ms(1000);
      OCR1A = 1340; // BASE A
      OCR1B = 2660; // BASE B
      _delay_ms(1000);
      OCR1A = 1760; //   BASE A
      OCR1B = 2240; // BASE B
            _delay_ms(1000);
      OCR1A = 2000; // BASE A
      OCR1B = 2000; // BASE B
      _delay_ms(1000);
      OCR2B = 13; // GARRA ----------------------- pegou
      _delay_ms(1000);
      OCR1A = 1760; // BASE A
      OCR1B = 2240; // BASE B
       _delay_ms(1000);
      OCR1A = 1340; // BASE A
      OCR1B = 2660; // BASE B
       _delay_ms(1000);
      OCR1A = 1190; // BASE A
      OCR1B = 2900; // BASE B
       _delay_ms(1000);
       for(int aux = 7; aux <21; aux++){
      OCR0A = aux ;//cima
      _delay_ms(100);
      }
      _delay_ms(1000);
      OCR2A = 22; // ROTACAO --------------------- deixou no meio
      _delay_ms(1000);
         OCR2A = 33; // ROTACAO
         _delay_ms(1000);
      OCR0A = 9; // CIMA
      _delay_ms(1000);
       for(int aux1 = 33; aux1 >9; aux1--){
      OCR0B = aux1 ;//bracinho
      _delay_ms(100);
      }
       _delay_ms(1000);
      OCR1A = 1220; // BASE A
      OCR1B = 2780; // BASE B
      _delay_ms(1000);
      OCR1A = 1640; // BASE A
      OCR1B = 2360; // BASE B
      _delay_ms(1000);
      OCR2B = 4; // GARRA ----------------------------- deixou na esquerda
            _delay_ms(1000);
      OCR1A = 1100; // BASE A 
      OCR1B = 2900; // BASE B
      _delay_ms(1000);
      OCR0A = 20; // CIMA
      OCR0B = 28; // BRACINHO
      OCR2A = 22; // ROTACAO
      OCR2B = 4; // GARRA
      
      
       contadorconfirma = 0;
    }
    if(contadorautomatico == 5){// 2 -> 3 ----------------------------- QUADRADO DA DIREITA
            OCR1A = 1100; // BASE A
      OCR1B = 2900; // BASE B
      OCR0A = 20; // CIMA
      OCR0B = 28; // BRACINHO
      OCR2A = 22; // ROTACAO
      OCR2B = 4; // GARRA
      _delay_ms(1000);
      OCR2A = 16; // ROTACAO
      _delay_ms(1000);
      OCR0A = 15; // CIMA
      _delay_ms(500);
      OCR0A = 10; // CIMA
      _delay_ms(500);
      OCR0A = 7; // CIMA
      _delay_ms(1000);
      OCR1A = 1340; // BASE A
      OCR1B = 2660; // BASE B
      _delay_ms(1000);
      OCR1A = 1760; // BASE A
      OCR1B = 2240; // BASE B
            _delay_ms(1000);
      OCR1A = 2000; // BASE A
      OCR1B = 2000; // BASE B
      _delay_ms(1000);
      OCR2B = 13; // GARRA ----------------------- pegou
      _delay_ms(1000);
      OCR1A = 1760; // BASE A
      OCR1B = 2240; // BASE B
       _delay_ms(1000);
      OCR1A = 1340; // BASE A
      OCR1B = 2660; // BASE B
       _delay_ms(1000);
      OCR1A = 1190; // BASE A
      OCR1B = 2900; // BASE B
       _delay_ms(1000);
       for(int aux = 7; aux <21; aux++){
      OCR0A = aux ;//cima
      _delay_ms(100);
      }
      _delay_ms(1000);
      OCR2A = 22; // ROTACAO --------------------- deixou no meio
      _delay_ms(1000);
       OCR0A = 9; // CIMA
      _delay_ms(1000);
      OCR0B = 10; // BRACINHO
      _delay_ms(1000);
      OCR1A = 1220; // BASE A
      OCR1B = 2780; // BASE B
      _delay_ms(1000);
      OCR1A = 1400; // BASE A
      OCR1B = 2600; // BASE B
      _delay_ms(1000);
      OCR2B = 4; // GARRA ----------------------------- deixou NO MEIO
      _delay_ms(1000);
      OCR1A = 1100; // BASE A 
      OCR1B = 2900; // BASE B
      _delay_ms(1000); 
      OCR0A = 20; // CIMA
      OCR0B = 28; // BRACINHO
      OCR2A = 22; // ROTACAO
      OCR2B = 4; // GARRA
      
       contadorconfirma = 0;

    }
    if(contadorautomatico == 6){// 2 -> 4
            OCR1A = 1100; // BASE A --------------------- QUADRADO DA DIREITA
      OCR1B = 2900; // BASE B
      OCR0A = 20; // CIMA
      OCR0B = 28; // BRACINHO
      OCR2A = 22; // ROTACAO
      OCR2B = 4; // GARRA
      _delay_ms(1000);
      OCR2A = 16; // ROTACAO
      _delay_ms(1000);
      OCR0A = 15; // CIMA
      _delay_ms(500);
      OCR0A = 10; // CIMA
      _delay_ms(500);
      OCR0A = 7; // CIMA
      _delay_ms(1000);
      OCR1A = 1340; // BASE A
      OCR1B = 2660; // BASE B
      _delay_ms(1000);
      OCR1A = 1760; // BASE A
      OCR1B = 2240; // BASE B
            _delay_ms(1000);
      OCR1A = 2000; // BASE A
      OCR1B = 2000; // BASE B
      _delay_ms(1000);
      OCR2B = 13; // GARRA ----------------------- pegou
      _delay_ms(1000);
      OCR1A = 1760; // BASE A
      OCR1B = 2240; // BASE B
       _delay_ms(1000);
      OCR1A = 1340; // BASE A
      OCR1B = 2660; // BASE B
       _delay_ms(1000);
      OCR1A = 1190; // BASE A
      OCR1B = 2900; // BASE B
       _delay_ms(1000);
       for(int aux = 7; aux <21; aux++){
      OCR0A = aux ;//cima
      _delay_ms(100);
      }
      _delay_ms(1000);
      OCR2A = 22; // ROTACAO --------------------- deixou no meio
      OCR2A = 21; //rotacao
      _delay_ms(1000); 
      OCR0B = 19;//bracinho
      _delay_ms(1000);
      OCR0A = 4  ;//cima
      _delay_ms(1000); 
      OCR1A = 1580;//base
      OCR1B = 2420;
      _delay_ms(1000);
      OCR1A = 1940;//base
      OCR1B = 2060;
      _delay_ms(1000);
      OCR1A = 2240;//base
      OCR1B = 1760;
      _delay_ms(1000);
      OCR2B = 4;// ----------- DEIXOU EM CIMA
      _delay_ms(1000);
      OCR1A = 1100; // BASE A 
      OCR1B = 2900; // BASE B
      _delay_ms(1000); 
      OCR0A = 20; // CIMA
      OCR0B = 28; // BRACINHO
      OCR2A = 22; // ROTACAO
      OCR2B = 4; // GARRA
      
       contadorconfirma = 0;
    }
    if(contadorautomatico == 7){// 3 -> 1
      OCR1A = 1100; // BASE A ------------- QUADRADO DO MEIO
      OCR1B = 2900; // BASE B
      OCR0A = 20; // CIMA
      OCR0B = 28; // BRACINHO
      OCR2A = 22; // ROTACAO
      OCR2B = 4; // GARRA
      _delay_ms(1000);
      OCR0B = 27;// BRACINHO
      _delay_ms(1000);
      OCR0A = 18;// CIMA
      _delay_ms(1000);
      OCR1A = 1580; // BASE A
      OCR1B = 2420; // BASE B
      _delay_ms(1000);
      OCR2B = 14; // GARRA ------------------- pegou
      _delay_ms(1000);
      OCR1A = 1100; // BASE A
      OCR1B = 2900; // BASE B
      _delay_ms(1000);
      OCR0A = 20; // CIMA
      OCR0B = 28; // BRACINHO
      //---DEIXOU NO MEIO
      _delay_ms(1000);
            OCR2A = 22; // ROTACAO --------------------- deixou no meio
      OCR2A = 21; //rotacao
      _delay_ms(1000); 
      OCR0B = 19;//bracinho
      _delay_ms(1000);
      OCR0A = 4  ;//cima
      _delay_ms(1000); 
      OCR1A = 1580;//base
      OCR1B = 2420;
      _delay_ms(1000);
      OCR1A = 1940;//base
      OCR1B = 2060;
      _delay_ms(1000);
      OCR1A = 2240;//base
      OCR1B = 1760;
      _delay_ms(1000);
      OCR2B = 4;// ----------- DEIXOU EM CIMA
       OCR1A = 1100; // BASE A 
      OCR1B = 2900; // BASE B
      _delay_ms(1000); 
      OCR0A = 20; // CIMA
      OCR0B = 28; // BRACINHO
      OCR2A = 22; // ROTACAO
      OCR2B = 4; // GARRA

      
       contadorconfirma = 0;
    }
    if(contadorautomatico == 8){// 3 -> 2
      OCR1A = 1100; // BASE A ------------------- QUADRADO DO MEIO
      OCR1B = 2900; // BASE B 
      OCR0A = 20; // CIMA
      OCR0B = 28; // BRACINHO
      OCR2A = 22; // ROTACAO
      OCR2B = 4; // GARRA
      _delay_ms(1000);
      OCR0B = 27;// BRACINHO
      _delay_ms(1000);
      OCR0A = 18;// CIMA
      _delay_ms(1000);
      OCR1A = 1580; // BASE A
      OCR1B = 2420; // BASE B
      _delay_ms(1000);
      OCR2B = 14; // GARRA ------------------- pegou
      _delay_ms(1000);
      OCR1A = 1100; // BASE A
      OCR1B = 2900; // BASE B
      _delay_ms(1000);
      OCR0A = 20; // CIMA
      OCR0B = 28; // BRACINHO
      //deixou no meio
        _delay_ms(1000);
      OCR2A = 16; // ROTACAO
      _delay_ms(1000);
      OCR0B = 29;//bracinho
      _delay_ms(1000);
      OCR0A = 15; // CIMA
      _delay_ms(500);
      OCR0A = 10; // CIMA
      _delay_ms(500);
      OCR0A = 7; // CIMA
      _delay_ms(1000);
      OCR1A = 1340; // BASE A
      OCR1B = 2660; // BASE B
      _delay_ms(1000);
      OCR1A = 1760; // BASE A
      OCR1B = 2240; // BASE B
            _delay_ms(1000);
      OCR1A = 2000; // BASE A
      OCR1B = 2000; // BASE B
      _delay_ms(1000);
      OCR2B = 4; // GARRA ----------------------- DEIXOU NA DIREITA

      _delay_ms(1000);
      OCR1A = 1100; // BASE A 
      OCR1B = 2900; // BASE B 
      _delay_ms(1000);
      OCR0A = 20; // CIMA
      OCR0B = 28; // BRACINHO
      OCR2A = 22; // ROTACAO
      OCR2B = 4; // GARRA


      

      
       contadorconfirma = 0;
    }
    if(contadorautomatico == 9){// 3 -> 4 
      OCR1A = 1100; // BASE A ------------------------- QUADRADO DO MEIO
      OCR1B = 2900; // BASE B
      OCR0A = 20; // CIMA
      OCR0B = 28; // BRACINHO
      OCR2A = 22; // ROTACAO
      OCR2B = 4; // GARRA
      _delay_ms(1000);
      OCR0B = 27;// BRACINHO
      _delay_ms(1000);
      OCR0A = 18;// CIMA
      _delay_ms(1000);
      OCR1A = 1580; // BASE A
      OCR1B = 2420; // BASE B
      _delay_ms(1000);
      OCR2B = 14; // GARRA ------------------- pegou
      _delay_ms(1000);
      OCR1A = 1100; // BASE A
      OCR1B = 2900; // BASE B
      _delay_ms(1000);
      OCR0A = 20; // CIMA
      OCR0B = 28; // BRACINHO
      //DEIXOU NO MEIO
      _delay_ms(1000);
            OCR2A = 33; // ROTACAO
         _delay_ms(1000);
      OCR0A = 9; // CIMA
      _delay_ms(1000);
       for(int aux1 = 33; aux1 >9; aux1--){
      OCR0B = aux1 ;//bracinho
      _delay_ms(100);
      }
       _delay_ms(1000);
      OCR1A = 1220; // BASE A
      OCR1B = 2780; // BASE B
      _delay_ms(1000);
      OCR1A = 1640; // BASE A
      OCR1B = 2360; // BASE B
      _delay_ms(1000);
      OCR2B = 4; // GARRA ----------------------------- deixou na esquerda
      
            _delay_ms(1000);
      OCR1A = 1100; // BASE A 
      OCR1B = 2900; // BASE B 
      _delay_ms(1000);
      OCR0A = 20; // CIMA
      OCR0B = 28; // BRACINHO
      OCR2A = 22; // ROTACAO
      OCR2B = 4; // GARRA
       contadorconfirma = 0;
    }
    if(contadorautomatico == 10){// 4 -> 1
      OCR1A = 1100; // BASE A --------------- QUADRADO DA ESQUERDA
      OCR1B = 2900; // BASE B
      OCR0A = 20; // CIMA
      OCR0B = 28; // BRACINHO
      OCR2A = 33; // ROTACAO
      OCR2B = 4; // GARRA
      _delay_ms(1000);
      OCR0A = 9; // CIMA
      _delay_ms(1000);
      OCR0B = 10; // BRACINHO
      _delay_ms(1000);
      OCR1A = 1220; // BASE A
      OCR1B = 2780; // BASE B
      _delay_ms(1000);
      OCR1A = 1640; // BASE A
      OCR1B = 2360; // BASE B
      _delay_ms(1000);
      OCR2B = 14; // GARRA ----------------------------- pegou
      _delay_ms(1000);
      OCR1A = 1220; // BASE A
      OCR1B = 2780; // BASE B
      _delay_ms(1000);
      OCR1A = 1100; // BASE A
      OCR1B = 2900; // BASE B
      _delay_ms(1000);
      OCR2A = 22; // ROTACAO ----------------------------- levou ao meio
      _delay_ms(1000);
            OCR2A = 21; //rotacao
      _delay_ms(1000); 
      OCR0B = 19;//bracinho
      _delay_ms(1000);
      OCR0A = 4  ;//cima
      _delay_ms(1000); 
      OCR1A = 1580;//base
      OCR1B = 2420;
      _delay_ms(1000);
      OCR1A = 1940;//base
      OCR1B = 2060;
      _delay_ms(1000);
      OCR1A = 2240;//base
      OCR1B = 1760;
      _delay_ms(1000);
      OCR2B = 4;//abrir garra---------------- deixou
      _delay_ms(1000);
      OCR1A = 1940;//base
      OCR1B = 2060;
      _delay_ms(1000);
       OCR1A = 1580;//base
      OCR1B = 2420;
      _delay_ms(1000);
       OCR0B = 25;
       _delay_ms(500); 
       OCR0B = 33;//bracinho
      _delay_ms(1000);
       for(int aux = 5; aux <21; aux++){
      OCR0A = aux ;//cima
      _delay_ms(100);
      }
        OCR1A = 1460;//base
      OCR1B = 2540;
      _delay_ms(1000);
      OCR1A = 1100;//base
      OCR1B = 2900;
      _delay_ms(1000);

        //-------------------------- deixou no meio
          OCR2A = 22; // ROTACAO --------------------- deixou no meio
      OCR2A = 21; //rotacao
      _delay_ms(1000); 
      OCR0B = 19;//bracinho
      _delay_ms(1000);
      OCR0A = 4  ;//cima
      _delay_ms(1000); 
      OCR1A = 1580;//base
      OCR1B = 2420;
      _delay_ms(1000);
      OCR1A = 1940;//base
      OCR1B = 2060;
      _delay_ms(1000);
      OCR1A = 2240;//base
      OCR1B = 1760;
      _delay_ms(1000);
      OCR2B = 4;// ----------- DEIXOU EM CIMA
            _delay_ms(1000);
      OCR1A = 1100; // BASE A 
      OCR1B = 2900; // BASE B 
      _delay_ms(1000);
      OCR0A = 20; // CIMA
      OCR0B = 28; // BRACINHO
      OCR2A = 22; // ROTACAO
      OCR2B = 4; // GARRA
      
      
       contadorconfirma = 0;
    }
    if(contadorautomatico == 11){// 4 -> 2
      OCR1A = 1100; // BASE A
      OCR1B = 2900; // BASE B
      OCR0A = 20; // CIMA
      OCR0B = 28; // BRACINHO
      OCR2A = 33; // ROTACAO
      OCR2B = 4; // GARRA
      _delay_ms(1000);
      OCR0A = 9; // CIMA
      _delay_ms(1000);
      OCR0B = 10; // BRACINHO
      _delay_ms(1000);
      OCR1A = 1220; // BASE A
      OCR1B = 2780; // BASE B
      _delay_ms(1000);
      OCR1A = 1640; // BASE A
      OCR1B = 2360; // BASE B
      _delay_ms(1000);
      OCR2B = 14; // GARRA ----------------------------- pegou
      _delay_ms(1000);
      OCR1A = 1220; // BASE A
      OCR1B = 2780; // BASE B
      _delay_ms(1000);
      OCR1A = 1100; // BASE A
      OCR1B = 2900; // BASE B
      _delay_ms(1000);
      OCR2A = 22; // ROTACAO ----------------------------- levou ao meio
      _delay_ms(1000);
            OCR0A = 9; // CIMA
      _delay_ms(1000);
      OCR0B = 10; // BRACINHO
      _delay_ms(1000);
      OCR1A = 1220; // BASE A
      OCR1B = 2780; // BASE B
      _delay_ms(1000);
      OCR1A = 1400; // BASE A
      OCR1B = 2600; // BASE B
      _delay_ms(1000);
      OCR2B = 4; // GARRA ----------------------------- deixou NO MEIO
      _delay_ms(1000);
            OCR0A = 9; // CIMA
      _delay_ms(1000);
      OCR0B = 10; // BRACINHO
      _delay_ms(1000);
      OCR1A = 1220; // BASE A
      OCR1B = 2780; // BASE B
      _delay_ms(1000);
      OCR1A = 1400; // BASE A
      OCR1B = 2600; // BASE B
      _delay_ms(1000);
      OCR2B = 4; // GARRA ----------------------------- deixou NO MEIO
            _delay_ms(1000);
      OCR1A = 1100; // BASE A 
      OCR1B = 2900; // BASE B 
      _delay_ms(1000);
      OCR0A = 20; // CIMA
      OCR0B = 28; // BRACINHO
      OCR2A = 22; // ROTACAO
      OCR2B = 4; // GARRA
       contadorconfirma = 0;
    }
    if(contadorautomatico == 12){// 4 -> 3
      OCR1A = 1100; // BASE A
      OCR1B = 2900; // BASE B
      OCR0A = 20; // CIMA
      OCR0B = 28; // BRACINHO
      OCR2A = 33; // ROTACAO
      OCR2B = 4; // GARRA
      _delay_ms(1000);
      OCR0A = 9; // CIMA
      _delay_ms(1000);
      OCR0B = 10; // BRACINHO
      _delay_ms(1000);
      OCR1A = 1220; // BASE A
      OCR1B = 2780; // BASE B
      _delay_ms(1000);
      OCR1A = 1640; // BASE A
      OCR1B = 2360; // BASE B
      _delay_ms(1000);
      OCR2B = 14; // GARRA ----------------------------- pegou
      _delay_ms(1000);
      OCR1A = 1220; // BASE A
      OCR1B = 2780; // BASE B
      _delay_ms(1000);
      OCR1A = 1100; // BASE A
      OCR1B = 2900; // BASE B
      _delay_ms(1000);
      OCR2A = 22; // ROTACAO ----------------------------- levou ao meio
      _delay_ms(1000);
      OCR2A = 16; // ROTACAO
      _delay_ms(1000);
      OCR0B = 29;//bracinho
      _delay_ms(1000);
      OCR0A = 15; // CIMA
      _delay_ms(500);
      OCR0A = 10; // CIMA
      _delay_ms(500);
      OCR0A = 7; // CIMA
      _delay_ms(1000);
      OCR1A = 1340; // BASE A
      OCR1B = 2660; // BASE B
      _delay_ms(1000);
      OCR1A = 1760; // BASE A
      OCR1B = 2240; // BASE B
            _delay_ms(1000);
      OCR1A = 2000; // BASE A
      OCR1B = 2000; // BASE B
      _delay_ms(1000);
      OCR2B = 4; // GARRA ----------------------- DEIXOU NA DIREITA
            _delay_ms(1000);
      OCR1A = 1100; // BASE A 
      OCR1B = 2900; // BASE B 
      _delay_ms(1000);
      OCR0A = 20; // CIMA
      OCR0B = 28; // BRACINHO
      OCR2A = 22; // ROTACAO
      OCR2B = 4; // GARRA
       contadorconfirma = 0;
    }
    
}








  

  }
  //Fim Do Looping--------------------------------------------------------
}
//Main---------------------------------------------------------------------
