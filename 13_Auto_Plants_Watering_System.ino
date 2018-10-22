#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
int moistureSensor=A4;
int sensorValue;
int SER_Pin=A1;
int RCLK_Pin=6;
int SRCLK_Pin = 5;
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
#define numOfRegisterPins number_of_74hc595s*10
boolean registers[numOfRegisterPins];
Adafruit_PCD8544 display= Adafruit_PCD8544(8,9,10,12,11);

  void setup() 
  {
  Serial.begin(9600);
  pinMode(A4,INPUT);
  pinMode(SER_Pin,OUTPUT);
  pinMode(RCLK_Pin,OUTPUT);
  pinMode(SRCLK_Pin,OUTPUT);
 clearRegisters();
  writeRegisters();
  display.begin();
  display.setContrast(50);
  display.clearDisplay();
  display_El_logo();
  display_L2S();
  display_title();
}
  
  void loop()
  {
    display.drawBitmap(70,35,logo_el,10,10,BLACK);  
    display.clearDisplay(); 
    display.drawRect(0,0,84,48,BLACK);
    int  b=analogRead(A4);
    int c=b;
           Serial.println(b);
    int  a=map(b,900,50,0,20);
         
  if(c>800)
  {
  display.setCursor(4,20);
  display.print("START WATER");
  display.setTextSize(1);
  display.setTextColor(BLACK);
  }
 else
  {
   display.drawBitmap(70,35,logo_el,10,10,BLACK);
  display.setCursor(4,20);
  display.println("STOP WATER");
  display.setTextSize(1);
  display.setTextColor(BLACK);
  }
  
  display.display();
  display.clearDisplay();
   lightup(a);
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

//********************************learn2solder***********************************//

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
  display.setCursor(6,10);
  display.println("AUTOMATIC");
  display.setCursor(6,20);
  display.println("PLANTS");
  display.setCursor(6,30);
  display.println("WATERING");
  display.drawBitmap(70,35,logo_el,10,10,BLACK);
  display.display(); 
  delay(5000);
  display.clearDisplay();
}

//***********************************clear bargraph**********************//

  
void writeRegisters(){

  digitalWrite(RCLK_Pin, LOW);

  for(int i = numOfRegisterPins - 1; i >=  0; i--){
    digitalWrite(SRCLK_Pin, LOW);

    int val = registers[i];

    digitalWrite(SER_Pin, val);
    digitalWrite(SRCLK_Pin, HIGH);

  }
  digitalWrite(RCLK_Pin, HIGH);

}
  

void setRegisterPin(int index, int value){
  registers[index] = value;
}
  
  void lightup(int a){
  for(int i=0;i<a;i++)
  {
    
    setRegisterPin(i,HIGH);
    writeRegisters();
  
  }
  for(int i=a;i<20;i++)
  {
    
    setRegisterPin(i,LOW);
    writeRegisters();
  }
  }
  void clearRegisters(){
  for(int i = numOfRegisterPins - 1; i >=  0; i--){
     registers[i] = LOW;
  }
  writeRegisters();
} 
