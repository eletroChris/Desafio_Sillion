#include<reg51.h>
#include <string.h>

sbit botao =P1^0;               // Pino ligado ao push button
sbit rs = P3^1;					// Pino rs do LCD
sbit en = P3^0;					// Pino en do LCD
sbit rw = P3^2;					// Pino rw do LCD
sbit busy_flag = P0^7;			// Busy flag 
sbit motor=P2^5;     			// Pino que controla o relé do motor

void busy(void);
void delay(int);

void cmd_escrita(unsigned char a){		
	busy();							// Verifica a busy flag
	rs = 0;							
	rw = 0;							
	P0 = a;							// Seleciona o comando a ser passado ao LCD
    en = 1;
    delay(20);                       // Delay de 20 ms
    en = 0;
  }
void escreve_dado(unsigned char b)	// envia o dado para o LCD
  {
	busy();							// Verifica a busy flag
	rs = 1;							
	rw = 0;							
	P0 = b;							// Envia o caractere pelos pinos P0
	en = 1;							
	delay(20);                       // Delay de 20 ms    
    en = 0;													
  }
void busy()							// Verifica se a busy flag está ativada
  {
	en = 0;							// Desabilita o display
	P0 = 0xFF;						// Seta pinos P0 como input
	rs = 0;							// 
	rw = 1;							// habilita leitura
	while(busy_flag==1)						
	{
	 delay(5);						// Permanece no loop de 5 ms enquanto a busy flag estiver ativa 	 
	}
							
  }
void escreve_string(unsigned char *s)	// Envia string para o LCD
 {
	unsigned int l,i;
	l = strlen(s);					
	for(i=0;i<l;i++)				 
	{
		escreve_dado(*s);		 
		s++;						// Envia os caracteres um por um
	}
 }

void delay(int x){ // Gera um delay de x ms)
    int i=0;
    while(i<x)
        {
        TMOD=0x10;        //Configura timer 1 em modo de 16-bit 
        TH1=0xFC;         //Contagem de 1 ms usando oscilador de 12MHz
        TL1=0x18;         //Count=65536-1000
        TR1=1;            //Inicia o timer
        while(TF1==0);    //Espera ate a flag de  Timer overflow ser ativada
        TF1=0;            //Limpa a flag de Timer overflow
        TR1=0;            //Pausa o timer
        i++;
    }
}


 void main(){
    cmd_escrita(0x38); // Seleciona matrix 5X7 de duas linhas
    cmd_escrita(0x01); // Limpa a tela de LCD
    cmd_escrita(0x80); // Coloca o cursor na primeira posição da primeira linha
    escreve_string("Motor Desligado");
    while(1){
        if(botao==1){
            escreve_string("Motor Ligado");
            motor=1;
            delay(10000); // Delay de 10 segundos
            motor=0;
            cmd_escrita(0x01); // Limpa a tela de LCD
            cmd_escrita(0x80); // Coloca o cursor na primeira posição da primeira linha
            escreve_string("Motor Desligado");            
        }
    }
 }