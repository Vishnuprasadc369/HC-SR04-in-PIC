/*
 * File:   newmain.c
 * Author: Hp
 *
 * Created on 13 October, 2023, 2:02 PM
 */


#include <xc.h>
#include<stdio.h>
#define _XTAL_FREQ 20000000
void delay_us(){
         while(TMR2IF==0);
         TMR2IF=0;
         PR2=12;
        }
void delay()
{
for(int i=0;i<30;i++)
for(int j=0;j<10;j++);
}
void cmd(char a)
{
PORTB = (a & 0xF0);
RB0=0;
RB1=1;
delay();
RB1=0;
PORTB =((a<<4)&0xF0);
RB1=1;
delay();
RB1=0;
}
void data(char b)
{
PORTB=(b&0xF0);
RB0=1;
RB1=1;
delay();
RB1=0;
PORTB=((b<<4)&0xF0);
RB0=1;
RB1=1;
delay();
RB1=0;
}
void display(const char *p)
{
while(*p)
{
data(*p);
p++;
}
}

void init(){
    
cmd(0x02);
cmd(0x28);
cmd(0x01);

cmd(0x0E);
data('h');

}
void value(int a){
    int r[10];
   
      for(int i=0;i<4;i++){
          r[3-i]=(a%10)+48;
          a=a/10;
          
      }for(int i=0;i<4;i++){
          data(r[i]);
      
      }

}
void LCD_value_float(float a){
    char r[10];
    sprintf(r,"%0.2f",a);
    display(r);

}

void main(void) {
    ADCON1=0X0F;
    TRISA=0X00;
    TRISB=0X00;
    TRISC=0xF4;
    T2CON=0b00000101;
    CCP1CON = 0b00000101; 
    T1CON = 0b00000001;
    uint16_t pulseWidth;
    init();
    while(1){
     
        delay();
        CCP1CON = 0b00000101;  
        RC0=1;
        delay();
        RC0=0;
        CCP1IF = 0;  
        while(!CCP1IF); 
        uint16_t startTime = CCPR1;
        CCP1CON = 0b00000100; 
        CCP1IF = 0; 
        while(!CCP1IF);
        uint16_t endTime = CCPR1;
        if(endTime>startTime)
        pulseWidth = endTime - startTime;
        else
        pulseWidth= 65536-startTime+endTime;
        cmd(0xC0);
        float result =(4*1*(pulseWidth))/20000000.0;
        float cm = ((34300*result)/2)+.06;
        cmd(0x80);
        display("cm");
        LCD_value_float(cm);

     }
    return;
}
