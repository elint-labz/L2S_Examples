#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
const int trigPin = A5;
const int echoPin = A4;
Adafruit_PCD8544 display= Adafruit_PCD8544(8,9,10,12,11);
#define number_of_74hc595s 3
#define numOfRegisterPins number_of_74hc595s*8
boolean registers[numOfRegisterPins];
const byte logo_el[] PROGMEM = {16,10,
B00000000,B00000000,
B11111010,B00000000,
B11111010,B00000000,
B00000010,B00000000,
B11111010,B00000000,
B11111010,B00000000,
B00000010,B00000000,
B11111011,B11000000,
B11111011,B11000000,
B00000000,B00000000,
};
int RCLK_Pin=6;
int SRCLK_Pin = 5;
int SER_Pin=A1;
void setup() 
{
   pinMode(2,OUTPUT);
   pinMode(SER_Pin,OUTPUT);
   pinMode(RCLK_Pin,OUTPUT);
   pinMode(SRCLK_Pin,OUTPUT);
   clearRegisters();
   writeRegisters();
   display.begin();
   display.setContrast(50);
   display.clearDisplay();
   pinMode(A4,INPUT);
   pinMode(A5,INPUT);
   display_El_logo();
   display_L2S();
   display_title();
   display.drawBitmap(70,35,logo_el,10,10,BLACK);
   display.drawRect(0,0,84,48,BLACK);  
}
  int count=0;
  long duration,cm;
  void loop()
{  
  display.drawBitmap(70,35,logo_el,10,10,BLACK);
  display.drawRect(0,0,84,48,BLACK);
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  // convert the time into a distance

  cm = microsecondsToCentimeters(duration);
  delay(100);
   int  b=digitalRead(13);
   if(b==1){
   display.clearDisplay();      
   int b=digitalRead(13);
   while(1){
            display.drawBitmap(70,35,logo_el,10,10,BLACK);  
            display.drawRect(0,0,84,48,BLACK);
            display.setCursor(6,6);
            display.println("SET DISTANCE:");
            if(digitalRead(13)==HIGH)
                  count++;
                  delay(300);
                  display.setCursor(20,15);
                  display.println(count);
            if(digitalRead(A0)==HIGH)
                  break;
                  display.display();
                  display.clearDisplay();
                  }    
  }
  else
  {
   display.drawBitmap(70,35,logo_el,10,10,BLACK);
   display.drawRect(0,0,84,48,BLACK);
   display.setCursor(2,2);
   display.println("DISTANCE : ");
   display.setCursor(15,15);
   display.print(cm); 
   display.display();
   display.clearDisplay();
   delay(100);
    } 
     if(cm<=9)
   {
   display.setCursor(22,15);
   display.println("cm");
   }
   else if(cm<=99)
   {
   display.setCursor(27,15);
   display.println("cm");
   }
   else if(cm>99)
   {
   display.setCursor(33,15);
   display.println("cm");
   }
    alert();
  }


long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}
void alert()
{
  if(count>=cm)
  {
    delay(100);
     digitalWrite(2,HIGH);
    delay(100);
     digitalWrite(2,LOW);
  }
  else
  {

    digitalWrite(2,LOW);
  }
}
 
void clearRegisters(){
  for(int i = numOfRegisterPins - 1; i >=  0; i--){
     registers[i] = LOW;
  }
  writeRegisters();
} 
void writeRegisters(){
  digitalWrite(RCLK_Pin, LOW);
  for(int i = numOfRegisterPins - 1; i >=  0; i--)
  {
    digitalWrite(SRCLK_Pin, LOW);
    int val = registers[i];
    digitalWrite(SER_Pin, val);
    digitalWrite(SRCLK_Pin, HIGH);

  }
    digitalWrite(RCLK_Pin, HIGH);
}  


//**************To Display EL*************//
void display_El_logo()
{
   display.fillRect(15,2,21,6,BLACK);
   display.fillRect(15,12,21,6,BLACK);
   display.fillRect(15,22,21,6,BLACK);
   display.fillRect(45,2,6,26,BLACK);
   display.fillRect(50,22,18,6,BLACK);
   display.setCursor(15,35);
   display.print("ElintLabz");
   display.display();
   delay(5000);
   display.clearDisplay();
}
//************To Display L2S*********************//
void display_L2S()
{
  display.drawRect(0, 0, 84 , 48,BLACK);
  display.setCursor(26, 10);
  display.println("LEARN");
  display.setCursor(38,20);
  display.println("2");
  display.setCursor(22,30);
  display.println("SOLDER");
  display.display(); 
  delay(5000);
  display.clearDisplay();
}
//******************To Display Tittle**************//
void display_title()
{
  display.drawRect(0, 0, 84 , 48,BLACK);
  display.setCursor(16,12);
  display.println("OBSTACLE");
  display.setCursor(13,22);
  display.println("DETECTION");
  display.setCursor(20,32);
  display.println("SYSTEM");
  display.drawBitmap(70,35,logo_el,10,10,BLACK);
  display.display(); 
  delay(5000);
  display.clearDisplay();
}
