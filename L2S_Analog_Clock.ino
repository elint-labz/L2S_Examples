#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
Adafruit_PCD8544 display = Adafruit_PCD8544(8,9,10,12,11);
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
int SER_Pin=A1;
int RCLK_Pin=6;
int SRCLK_Pin = 5;
int count=0;
int a=0,b=0,c=0,d=0,e=0,f=0;
int l, r, u, dn, p;
#define LEFT 13
#define DOWN A2
#define RIGHT A0
#define UP A3
//****************************void setup**********************************//
void setup() 
{
  pinMode(SER_Pin,OUTPUT);
  pinMode(RCLK_Pin,OUTPUT);
  pinMode(SRCLK_Pin,OUTPUT);
  clearRegisters();
  writeRegisters();
  display.begin();
  display.setContrast(50);
  display.clearDisplay();
  pinMode(2,OUTPUT);
  display_El_logo();
  display_L2S();
  display_title();
  }
//*****************************void loop*********************************//
void loop() {
  tmElements_t tm;
  display.setContrast(50);
  if (RTC.read(tm))
  {
  int h=tm.Hour;
  int m=tm.Minute;
  int s=tm.Second;
  
  drawClock(h,m,s);
  if(digitalRead(DOWN)==HIGH && digitalRead(RIGHT)==HIGH)
  {
    alarm();
  }
  }
  unsigned long currentMillis = millis();
   unsigned long previousMillis;
   
  if(a==tm.Hour && b==tm.Minute)
 { 
   digitalWrite(2,HIGH);
 if(currentMillis-previousMillis>50000)
 {
  previousMillis = currentMillis; 
  display.setCursor(50,30);
  display.print("ALARM");
  display.display();
 }
  
 }
else
{
digitalWrite(2,LOW);
}
}
//******************************Elintlabz logo**************************//
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

//***********************learn2solder**********************************//
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
//*************************draw the clock*******************************//
void drawClock(int h, int m, int s)
{ 
  tmElements_t tm;
const int r=23;
const double rot=-M_PI/2;
double x,y,x0,y0,anglerad;
   if (RTC.read(tm))
  {
  display.clearDisplay();
  display.drawPixel(24,24,BLACK);
  display.drawCircle(24,24,r,BLACK);
  for(int i=0;i<12;i++)
    {
    anglerad=2*M_PI/12*i+rot;
    x=r*cos(anglerad);
    y=r*sin(anglerad);
    x0=(r-3)*cos(anglerad);
    y0=(r-3)*sin(anglerad);
    display.drawLine(24+x0,24+y0,24+x,24+y,BLACK);
    }
  anglerad=2*M_PI/12*(h%12)+2*M_PI/12/60*m+rot;
  x=(r-15)*cos(anglerad);
  y=(r-15)*sin(anglerad);
  x0=0;
  y0=0;
  display.drawLine(24+x0,24+y0,24+x,24+y,BLACK);
  anglerad=2*M_PI/60*m+rot;
  x=(r-10)*cos(anglerad);
  y=(r-10)*sin(anglerad);
  x0=0;
  y0=0;
  display.drawLine(24+x0,24+y0,24+x,24+y,BLACK);
  anglerad=2*M_PI/60*s+rot;
  x=(r-5)*cos(anglerad);
  y=(r-5)*sin(anglerad);
  x0=0;
  y0=0;
  display.drawLine(24+x0,24+y0,24+x,24+y,BLACK);
  display.drawLine(0,0,3,0,BLACK);
  display.drawLine(0,0,0,3,BLACK);
  display.drawLine(0,47,3,47,BLACK);
  display.drawLine(0,47,0,47-3,BLACK);
  display.drawLine(83,0,83-3,0,BLACK);
  display.drawLine(83,0,83,3,BLACK);
  display.drawLine(83,47,83-3,47,BLACK);
  display.drawLine(83,47,83,47-3,BLACK);
  if (tm.Day<10)
{
 
   display.setCursor(50,10);
   display.print('0');
   display.print(tm.Day);
}
else
{
   display.setCursor(50,10);
   display.print(tm.Day);
}
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(62,10);
    display.print('/'); 
   if (tm.Month<10)
{
 
  display.setCursor(68,10);
  display.print('0');
  display.print(tm.Month);
}
else
{
  display.setCursor(68,10);
  display.print(tm.Month);
}
    display.display();  
}
}
//*************************clearing registers*****************************//
void clearRegisters(){
  for(int i = numOfRegisterPins - 1; i >=  0; i--){
     registers[i] = LOW;
  }
  writeRegisters();
} 
//************************writting registers*******************************//
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
//***************************set the alarm******************************//
void alarm()
{
  delay(1000);
 tmElements_t tm;
 dn = digitalRead(DOWN);
 count=0;
 int onlyonce=0;
  while(RTC.read(tm) && dn!=HIGH)
{ 
   display.clearDisplay();
   display.fillCircle(4,4,2,BLACK);
   display.drawRect(0,0,10,10,BLACK);
   display.setCursor(12,2);
   display.println("INC");
   display.fillCircle(76,4,2,BLACK);
   display.drawRect(72,0,10,10,BLACK);
   display.setCursor(48,2);
   display.println("DEC");
   display.fillCircle(4,40,2,BLACK);
   display.drawRect(0,36,10,10,BLACK);
   display.fillCircle(76,40,2,BLACK);
   display.drawRect(72,36,10,10,BLACK);
   display.setCursor(12,38);
   display.println("SET");
   display.setCursor(48,38);
   display.println("EXIT");
   display.display();
   display.setCursor(14,14);
   display.println("SET ALARM: ");
    if (a<10)
{
 
   display.setCursor(16,24);
   display.print('0');
   display.print(a);
}
else
{
   display.setCursor(16,24);
   display.print(a);
}
   display.setCursor(29,24);
   display.print(":");
    if (b<10)
{
 
   display.setCursor(34,24);
   display.print('0');
   display.println(b);
}
else
{
   display.setCursor(34,24);
   display.println(b);
}
   display.setCursor(46,24);
   display.print(':');
    if (c<10)
{
 
   display.setCursor(52,24);
   display.print('0');
   display.println(c);
}
else
{
   display.setCursor(52,24);
   display.println(c);
}  
  if(onlyonce==0)
   {
   display.display();
   onlyonce++;
   } 
  l = digitalRead(LEFT);
  dn= digitalRead(DOWN);
  r = digitalRead(RIGHT);
  u = digitalRead(UP); 
  while(1)
  {
  l = digitalRead(LEFT);
  dn= digitalRead(DOWN);
  r = digitalRead(RIGHT);
  u = digitalRead(UP); 
  if( digitalRead(DOWN)==HIGH)
  break;
  if(l==HIGH && count==0)
  { delay(500);
    a=a+1;
    if(a>23)
    {
      a=0;
    }
  }
  if(l==HIGH && count==1)
  { delay(500);
    b=b+1;
    if(b>59)
    {
      b=0;
    }
  }
  if(l==HIGH && count==2)
  { delay(500);
    c=c+1;
    if(c>59)
    {
      c=0;
    }
  }
   
   if(u==HIGH && count==0)
  { delay(500);
    a=a-1;
    if(a<0)
    {
      a=23;
    }
  }
  if(u==HIGH && count==1)
  { delay(500);
    b=b-1;
    if(b<0)
    {
      b=59;
    }
  }
  if(u==HIGH && count==2)
  { delay(500);
    c=c-1;
    if(c<0)
    {
      c=59;
    }
  }
   
   if(r==HIGH)
  { delay(500);
    count=count+1;
    if(count>3)
    {
      count=0;
    }
  }

if( digitalRead(DOWN)==HIGH)
  break;

switch(count)
{
  case 0:
 
          if (a<10)                            // highlight selected one
          {
             display.setTextColor(WHITE,BLACK);
             display.setCursor(16,24);
              display.print('0');
              display.print(a);
              display.display();
          }
           else
          {
          display.setTextColor(WHITE,BLACK);
           display.setCursor(16,24);
           display.print(a);
           display.display();
          }
          break;
case 1:  if (a<10)                            //remove previous highlight
          {
             display.setTextColor(BLACK,WHITE);
             display.setCursor(16,24);
              display.print('0');
              display.print(a);
              display.display();
          }
           else
          {
          display.setTextColor(BLACK,WHITE);
           display.setCursor(16,24);
           display.print(a);
           display.display();
          }

          if (b<10)
          {
             display.setTextColor(WHITE,BLACK);
             display.setCursor(34,24);
              display.print('0');
              display.print(b);
              display.display();
          }
           else
          {
          display.setTextColor(WHITE,BLACK);
           display.setCursor(34,24);
           display.print(b);
           display.display();
          }
          break;
case 2 : if (b<10)
          {
             display.setTextColor(BLACK,WHITE);
             display.setCursor(34,24);
              display.print('0');
              display.print(b);
              display.display();
          }
           else
          {
          display.setTextColor(BLACK,WHITE);
           display.setCursor(34,24);
           display.print(b);
           display.display();
          }
          
         
          if (c<10)
          {
             display.setTextColor(WHITE,BLACK);
             display.setCursor(52,24);
              display.print('0');
              display.print(c);
              display.display();
          }
           else
          {
          display.setTextColor(WHITE,BLACK);
           display.setCursor(52,24);
           display.print(c);
           display.display();
          }
          break;
case 3:  if (c<10)
          {
             display.setTextColor(BLACK,WHITE);
             display.setCursor(52,24);
              display.print('0');
              display.print(c);
              display.display();
          }
           else
          {
          display.setTextColor(BLACK,WHITE);
           display.setCursor(52,24);
           display.print(c);
           display.display();
          }
          break;                  
}
 if( digitalRead(DOWN)==HIGH)
  break;
 }
  }
}
 //******************To Display Tittle**************//
void display_title()
{
  display.drawRect(0, 0, 84 , 48,BLACK);
  display.setCursor(16,12);
  display.println("ANALOG");
  display.setCursor(13,22);
  display.println("CLOCK");
  display.setCursor(20,32);
  display.println("SYSTEM");
  display.drawBitmap(70,35,logo_el,10,10,BLACK);
  display.display(); 
  delay(5000);
  display.clearDisplay();
}
