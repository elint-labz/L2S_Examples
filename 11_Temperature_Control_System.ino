#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
int count=0;
#define sensor_pin A4    
float vout; 
int SER_Pin=A1;
int RCLK_Pin=6;
int SRCLK_Pin = 5;       
Adafruit_PCD8544 display = Adafruit_PCD8544(8, 9, 10, 12, 11); 
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
#define number_of_74hc595s 3
#define numOfRegisterPins number_of_74hc595s*8
boolean registers[numOfRegisterPins];     
void setup()
{
  pinMode(SER_Pin,OUTPUT);
  pinMode(RCLK_Pin,OUTPUT);
  pinMode(SRCLK_Pin,OUTPUT);
  clearRegisters();
  writeRegisters();
  pinMode(sensor_pin,INPUT);
  pinMode(1,OUTPUT);
  display.begin();
  display.setContrast(50);
  display.clearDisplay();
  display_El_logo();
  display_L2S();
  display_title();
}

void loop()
{

if(digitalRead(13)==HIGH)
{
  display.clearDisplay();
  int b=digitalRead(13);
    while(1)
    {
      display.drawRect(0, 0, 84 , 48,BLACK);
      display.setCursor(18,15);
      display.println("Set Temp");
      if(digitalRead(13)== HIGH)
      count++;
      delay(300);
      display.setCursor(38,24);
      display.println(count);
      display.setTextSize(1);
      display.setTextColor(BLACK);
      if(digitalRead(A0)==HIGH)
      break;
      display.drawBitmap(70,35,logo_el,10,10,BLACK);
      display.display();
      display.clearDisplay();
    }
}
else
{
   display.drawRect(0, 0, 84 , 48,BLACK);
   display.setCursor(10,10);
   display.println("Temparature");
   vout=analogRead(sensor_pin);  // Reads the analog pin value & store it 
   vout=(vout*5)/(1023);        // calculating the voltage at analog output pin
   vout=vout*100;                    // calculating temperature in degree celsius
   display.setCursor(25,22);
   display.print(vout);
   display.drawPixel(56,22,BLACK);
   display.drawPixel(55,23,BLACK);
   display.drawPixel(56,24,BLACK);
   display.drawPixel(57,23,BLACK);
   display.setCursor(59,22);
   display.println("C");
  
   if(vout>=count  &&  count>=28)
   {
     digitalWrite(1,HIGH);
     display.setCursor(25,32);
     display.println("Fan ON");
   }
   else
   {
     digitalWrite(1,LOW);
   }
   display.drawBitmap(70,35,logo_el,10,10,BLACK);
   display.display();
   display.clearDisplay();
   delay(500);
}
}
//***********************Elintlabz logo*****************************************//
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

 //******************************learn2solder**********************************//
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

//********************************Title********************************************//
void display_title()
{
  display.drawRect(0, 0, 84 , 48,BLACK);
  display.setCursor(10,10);
  display.print("Temperature");
  display.setCursor(20,18);
  display.print("control");
  display.setCursor(22,26);
  display.print("system ");
  display.drawBitmap(70,35,logo_el,10,10,BLACK);
  display.display(); 
  delay(5000);
  display.clearDisplay();
}

//***********************************clear bargraph**********************//
 void writeRegisters()
 {
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
 
 void clearRegisters()
 {
  for(int i = numOfRegisterPins - 1; i >=  0; i--)
  {
     registers[i] = LOW;
  }
  writeRegisters();
 }
