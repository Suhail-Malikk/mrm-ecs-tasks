#include<SPI.h>

#define LED2 7
#define butt2 2

volatile boolean received;
volatile byte Slavereceived,Slavesend;
int pushvalue;
int x;

void setup()
{
  Serial.begin(115200);
  
  pinMode(butt2,INPUT_PULLUP);           
  pinMode(LED2,OUTPUT);                 
  pinMode(MISO,OUTPUT);               

  SPCR |= _BV(SPE);                     
  received = false;

  SPI.attachInterrupt();        
  
}

ISR (SPI_STC_vect)                        
{
  Slavereceived = SPDR;         
    received = true;                       
}

void loop()
{ if(received)                        
   {
      if(Slavereceived==1) 
      {
        digitalWrite(LED2,HIGH);         
        Serial.println("Slave LED ON");
      }else
      {
        digitalWrite(LED2,LOW);          
        Serial.println("Slave LED OFF");
      }
      
      pushvalue=digitalRead(butt2);  
      
      if(pushvalue==HIGH)               
      {
        x=1;
      }
      else
      {
        x=0;
      }
      
  Slavesend=x;                             
  SPDR = Slavesend;         
  
}
}