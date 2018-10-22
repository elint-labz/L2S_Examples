#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
int trx1=1;
int rx1=0;
int trx2=A4;
int rx2=A5;
int proxy1=0;
int proxy2=0;
int count1=0;
int count2=0;
int val;
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
#define numOfRegisterPins number_of_74hc595s*8
boolean registers[numOfRegisterPins];
Adafruit_PCD8544 display= Adafruit_PCD8544(8,9,10,12,11);
void setup() {
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
display.drawRect(0,0,84,48,BLACK);
proxy1=digitalRead(rx1);
if(proxy1==HIGH)
{
  display.setCursor(2,6);
  display.print("ENTERED =");
  display.setTextSize(1);
  display.setTextColor(BLACK);
  count1++;
  delay(500);
  display.setCursor(60,6);
  display.println(count1);
  display.setTextSize(1);
  display.setTextColor(BLACK);

  display.setCursor(2,20);
  display.print("EXITED =");
  display.setTextSize(1);
  display.setTextColor(BLACK);
  
  display.setCursor(53,20);
  display.println(count2);
  display.setTextSize(1);
  display.setTextColor(BLACK);
}
proxy2=digitalRead(rx2);
if(proxy2==HIGH)
{
display.setCursor(2,20);
  display.print("EXITED =");
  display.setTextSize(1);
  display.setTextColor(BLACK);
  count2++;
  delay(500);
  display.setCursor(53,20);
  display.println(count2);
  display.setTextSize(1);
  display.setTextColor(BLACK);

  display.setCursor(2,6);
  display.print("ENTERED =");
  display.setTextSize(1);
  display.setTextColor(BLACK);
  
  display.setCursor(60,6);
  display.println(count1);
  display.setTextSize(1);
  display.setTextColor(BLACK);
}
  val=count1-count2;
  display.setCursor(2,35);
  display.println("INSIDE =");
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(55,35);
  display.println(val);
  display.setTextSize(1);
  display.setTextColor(BLACK);

  display.setCursor(2,20);
  display.print("EXITED =");
  display.setTextSize(1);
  display.setTextColor(BLACK);
  
  display.setCursor(53,20);
  display.println(count2);
  display.setTextSize(1);
  display.setTextColor(BLACK);

  display.setCursor(2,6);
  display.print("ENTERED =");
  display.setTextSize(1);
  display.setTextColor(BLACK);
 
  display.setCursor(60,6);
  display.println(count1);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.display();
  display.clearDisplay();
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

//********************************learn2solder**********************************//

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
  display.println("COUNT");
  display.setCursor(6,30);
  display.println("SYSTEM");
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
void clearRegisters(){
  for(int i = numOfRegisterPins - 1; i >=  0; i--){
     registers[i] = LOW;
  }
  writeRegisters();
} 
