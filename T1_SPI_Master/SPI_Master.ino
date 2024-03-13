#include<SPI.h>                             
#define LED1 7           
#define butt1 2

int pushvalue;
int x;
void setup()

{
  Serial.begin(115200);                   
  
  pinMode(butt1,INPUT_PULLUP);                  
  pinMode(LED1,OUTPUT);                  
  
  SPI.begin();                               
  digitalWrite(SS,HIGH);                 
}

void loop()
{
  byte Mastersend,Mastereceive;          
  pushvalue=digitalRead(butt1);   
  
  if(pushvalue==HIGH)               
  {
    x=1;
  }
  else
  {
    x=0;
  }
  
  digitalWrite(SS,LOW);                  
  
  Mastersend = x;                            
  Mastereceive=SPI.transfer(Mastersend); 
  
  if(Mastereceive == 1)                   
  {
    digitalWrite(LED1,HIGH);              
    Serial.println("Master LED ON");
  }
  else
  {
   digitalWrite(LED1,LOW);               
   Serial.println("Master LED OFF");
  }
  
}