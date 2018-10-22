#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
int array1[84];
int count=0;
#define sensor_pin A5   
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
  display.drawBitmap(70,35,logo_el,10,10,BLACK);
  display.drawRect(0,0,84,48,BLACK);  
}
int t=0;
void loop()
{
 display.setRotation(0);
 display.drawRect(0, 0, 84 , 48,BLACK);
 display.setTextSize(1);
 display.setTextColor(BLACK);
if(digitalRead(13)==HIGH)
{
  display.setRotation(2);
  display.clearDisplay();
  int b=digitalRead(13);
    while(1)
    {
      int a=analogRead(A5);
  display.setContrast(50);
  display.clearDisplay();

t++;
a=map(a,0,200,0,40);

array1[t]=a;

for(int i=0;i<t;i++)
{
  if(array1[i]>array1[i-1])
  {
    display.writeLine(i-1,array1[i-1],i,array1[i],BLACK);
  }
  else if(array1[i]<array1[i-1])
  {
    display.writeLine(i,array1[i],i-1,array1[i-1],BLACK);
  }
  else
  {
display.drawPixel(i,array1[i],BLACK);
  }
      array1[i-1]=array1[i];
}
   
 display.display(); 
  delay(25);
  if(t==83)
  {
    t=80;
  }
  if(digitalRead(A0)==HIGH)
  {
  display.clearDisplay();
  break;
  }
    }
      
    }
else
{
   display.drawRect(0, 0, 84 , 48,BLACK);
display.setTextSize(1);
display.setTextColor(BLACK);

  display.setCursor(10,10);
display.println("NTC Value:");
display.setTextSize(1);
display.setTextColor(BLACK);
 
   
vout=analogRead(sensor_pin);        // Reads the analog pin value & store it in vout        
display.setCursor(25,22);
display.print(vout);
  display.setTextSize(1);
  display.setTextColor(BLACK);

display.drawBitmap(70,35,logo_el,10,10,BLACK);
display.display();
display.clearDisplay();
delay(500);
}
}

//********************clear bargraph*****************************//
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
void clearRegisters(){
  for(int i = numOfRegisterPins - 1; i >=  0; i--){
     registers[i] = LOW;
  }
  writeRegisters();
} 

//******************Elint labz logic****************//
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

//******************L2S****************//
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

//******************Title****************//
void display_title()
{
  display.drawRect(0, 0, 84 , 48,BLACK);
  display.setCursor(2,12);
  display.println("SMART");
  display.setCursor(2,22);
  display.println("TEMPERATURE");
  display.setCursor(2,32);
  display.println("DISPLAY");
  display.drawBitmap(70,35,logo_el,10,10,BLACK);
  display.display(); 
  delay(5000);
  display.clearDisplay();
}
