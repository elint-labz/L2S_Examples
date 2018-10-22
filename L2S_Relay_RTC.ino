#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
Adafruit_PCD8544 display = Adafruit_PCD8544(8, 9, 10, 12, 11);
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
#define LEFT 13
#define DOWN A2
#define RIGHT A0
#define UP A3
int l, r, u, dn, p;
int a=0,b=0,c=0,d=0,e=0,f=0;
int count=0;
int SER_Pin=A1;
int RCLK_Pin=6;
int SRCLK_Pin = 5;
//******************************void setup**********************************// 
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
  display_El_logo();
  display_L2S();
  display_title();
  pinMode(LEFT, INPUT);
  pinMode(RIGHT, INPUT);
  pinMode(UP, INPUT);
  pinMode(DOWN, INPUT);
  pinMode(0,OUTPUT);
  digitalWrite(LEFT, LOW);
  digitalWrite(RIGHT, LOW);
  digitalWrite(UP, LOW);
  digitalWrite(DOWN, LOW );
  digitalWrite(0,LOW); 
}
//******************************void loop*******************************//
void loop()
{  
   tmElements_t tm;
   display.setContrast(50);
   display.drawRoundRect(0, 0, 84 , 48, 9, 4);
   dn = digitalRead(DOWN);
   u = digitalRead(UP);
if (RTC.read(tm) && dn!=HIGH) 
{
   display.setCursor(2,8);
   display.println("TIME : ");
   display.setTextSize(1);
   display.setTextColor(BLACK);
if (tm.Hour<10)
{
 
   display.setCursor(16,16);
   display.print('0');
   display.print(tm.Hour);
}
else
{
   display.setCursor(16,16);
   display.print(tm.Hour);
}
   display.setCursor(29,16);
   display.print(":");
   if (tm.Minute<10)
{
 
   display.setCursor(34,16);
   display.print('0');
   display.println(tm.Minute);
}
else
{
   display.setCursor(34,16);
   display.println(tm.Minute);
}
   display.setCursor(46,16);
   display.print(':');
  if (tm.Second<10)
{
 
   display.setCursor(52,16);
   display.print('0');
   display.println(tm.Second);
}
else
{
   display.setCursor(52,16);
   display.println(tm.Second);
}
   display.setCursor(2,30);
   display.println("DATE : ");
  if (tm.Day<10)
{
 
   display.setCursor(16,38);
   display.print('0');
   display.print(tm.Day);
}
else
{
   display.setCursor(16,38);
   display.print(tm.Day);
}
    display.setCursor(28,38);
    display.print('/'); 
   if (tm.Month<10)
{
 
  display.setCursor(35,38);
  display.print('0');
  display.print(tm.Month);
}
else
{
  display.setCursor(35,38);
  display.print(tm.Month);
}
    display.setCursor(47,38);
    display.print('/');
    display.setCursor(54,38);
    display.println(tmYearToCalendar(tm.Year));
    display.display(); 
    display.clearDisplay();
   
  }
  else if(digitalRead(DOWN)==HIGH && digitalRead(RIGHT)==HIGH)
  {
    alarm();
  }

  
 if(a==tm.Hour && b==tm.Minute && c==tm.Second )
 { 
 digitalWrite(0,HIGH);
 }
 if(d==tm.Hour && e==tm.Minute && f==tm.Second)
 { 
  digitalWrite(0,LOW);
 }
 
}
//**********************setting on and off time******************************//
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
   display.drawRoundRect(0, 0, 84 , 48, 9, 4);
   display.setCursor(4,4);
   display.println("ON_TIME : ");
   if (a<10)
{
 
   display.setCursor(16,14);
   display.print('0');
   display.print(a);
}
else
{
   display.setCursor(16,14);
   display.print(a);
}
   display.setCursor(29,14);
   display.print(":");
   if (b<10)
{
 
   display.setCursor(34,14);
   display.print('0');
   display.println(b);
}
else
{
   display.setCursor(34,14);
   display.println(b);
}
   display.setCursor(46,14);
   display.print(':');
   if (c<10)
{
 
   display.setCursor(52,14);
   display.print('0');
   display.println(c);
}
else
{
   display.setCursor(52,14);
   display.println(c);
}
   display.setCursor(2,30);
   display.println("OFF_TIME : ");
   if (d<10)
{
 
   display.setCursor(16,38);
   display.print('0');
   display.print(d);
}
else
{
   display.setCursor(16,38);
   display.print(d);
}
   display.setCursor(29,38);
   display.print(":");
   if (e<10)
{
 
   display.setCursor(34,38);
   display.print('0');
   display.println(e);
}
else
{
   display.setCursor(34,38);
   display.println(e);
}
   display.setCursor(46,38);
   display.print(':');
    if (f<10)
{
 
   display.setCursor(52,38);
   display.print('0');
   display.println(f);
}
else
{
   display.setCursor(52,38);
   display.println(f);
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
   if(l==HIGH && count==3)
  { delay(500);
    d=d+1;
    if(d>23)
    {
      d=0;
    }
  }
  if(l==HIGH && count==4)
  { delay(500);
    e=e+1;
    if(e>59)
    {
      e=0;
    }
  }
  if(l==HIGH && count==5)
  { delay(500);
    f=f+1;
    if(f>59)
    {
      f=0;
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
   if(u==HIGH && count==3)
  { delay(500);
    d=d-1;
    if(d<0)
    {
      d=23;
    }
  }
  if(u==HIGH && count==4)
  { delay(500);
    e=e-1;
    if(e<0)
    {
      e=59;
    }
  }
  if(u==HIGH && count==5)
  { delay(500);
    f=f-1;
    if(f<0)
    {
      f=59;
    }
  }
   if(r==HIGH)
  { delay(500);
    count=count+1;
    if(count>6)
    {
      count=0;
    }
  }

if( digitalRead(DOWN)==HIGH)
  break;

switch(count)
{
  case 0: if (f<10)                                      // remove previous highlight
          {
              display.setTextColor(BLACK,WHITE);
              display.setCursor(52,38);
              display.print('0');
              display.print(f);
              display.display();
          }
           else
          {
           display.setTextColor(BLACK,WHITE);
           display.setCursor(52,38);
           display.print(f);
           display.display();
          }


 
          if (a<10)                            // highlight selected one
          {
              display.setTextColor(WHITE,BLACK);
              display.setCursor(16,14);
              display.print('0');
              display.print(a);
              display.display();
          }
           else
          {
           display.setTextColor(WHITE,BLACK);
           display.setCursor(16,14);
           display.print(a);
           display.display();
          }
          break;
case 1:  if (a<10)                            //remove previous highlight
          {
              display.setTextColor(BLACK,WHITE);
              display.setCursor(16,14);
              display.print('0');
              display.print(a);
              display.display();
          }
           else
          {
           display.setTextColor(BLACK,WHITE);
           display.setCursor(16,14);
           display.print(a);
           display.display();
          }

          if (b<10)
          {
              display.setTextColor(WHITE,BLACK);
              display.setCursor(34,14);
              display.print('0');
              display.print(b);
              display.display();
          }
           else
          {
           display.setTextColor(WHITE,BLACK);
           display.setCursor(34,14);
           display.print(b);
           display.display();
          }
          break;
case 2 : if (b<10)
          {
              display.setTextColor(BLACK,WHITE);
              display.setCursor(34,14);
              display.print('0');
              display.print(b);
              display.display();
          }
           else
          {
           display.setTextColor(BLACK,WHITE);
           display.setCursor(34,14);
           display.print(b);
           display.display();
          }
          
         
          if (c<10)
          {
              display.setTextColor(WHITE,BLACK);
              display.setCursor(52,14);
              display.print('0');
              display.print(c);
              display.display();
          }
           else
          {
           display.setTextColor(WHITE,BLACK);
           display.setCursor(52,14);
           display.print(c);
           display.display();
          }
          break;
case 3:  if (c<10)
          {
              display.setTextColor(BLACK,WHITE);
              display.setCursor(52,14);
              display.print('0');
              display.print(c);
              display.display();
          }
           else
          {
           display.setTextColor(BLACK,WHITE);
           display.setCursor(52,14);
           display.print(c);
           display.display();
          }
          
          if (d<10)
          {
              display.setTextColor(WHITE,BLACK);
              display.setCursor(16,38);
              display.print('0');
              display.print(d);
              display.display();
          }
           else
          {
           display.setTextColor(WHITE,BLACK);
           display.setCursor(16,38);
           display.print(d);
           display.display();
          }
          break;
case 4:  if (d<10)
          {
              display.setTextColor(BLACK,WHITE);
              display.setCursor(16,38);
              display.print('0');
              display.print(d);
              display.display();
          }
           else
          {
           display.setTextColor(BLACK,WHITE);
           display.setCursor(16,38);
           display.print(d);
           display.display();
          }
          if (e<10)
          {
              display.setTextColor(WHITE,BLACK);
              display.setCursor(34,38);
              display.print('0');
              display.print(e);
              display.display();
          }
           else
          {
           display.setTextColor(WHITE,BLACK);
           display.setCursor(34,38);
           display.print(e);
           display.display();
          }
          break;
case 5:   if (e<10)
          {
              display.setTextColor(BLACK,WHITE);
              display.setCursor(34,38);
              display.print('0');
              display.print(e);
              display.display();
          }
           else
          {
           display.setTextColor(BLACK,WHITE);
           display.setCursor(34,38);
           display.print(e);
           display.display();
          }
          if (f<10)
          {
              display.setTextColor(WHITE,BLACK);
              display.setCursor(52,38);
              display.print('0');
              display.print(f);
              display.display();
          }
           else
          {
           display.setTextColor(WHITE,BLACK);
           display.setCursor(52,38);
           display.print(f);
           display.display();
          }
          break;          
}
 if( digitalRead(DOWN)==HIGH)
  break;
   
if(a==tm.Hour && b==tm.Minute && c==tm.Second)
 { 
 digitalWrite(0,HIGH);
 }

 if(d==tm.Hour && e==tm.Minute && f==tm.Second)
 { 
  digitalWrite(0,LOW);
 }
  }
} 
}
//***********************Elintlabz logo********************************//
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

//**************************learn2solder********************************//
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
//***************************clearing register*******************************//
void clearRegisters(){
  for(int i = numOfRegisterPins - 1; i >=  0; i--){
     registers[i] = LOW;
  }
  writeRegisters();
} 
//**************************writting registers********************************//
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
//******************To Display Tittle**************//
void display_title()
{
  display.drawRect(0, 0, 84 , 48,BLACK);
  display.setCursor(16,12);
  display.println("USER");
  display.setCursor(13,22);
  display.println("FRIENDLY");
  display.setCursor(20,32);
  display.println("TIMER");
  display.drawBitmap(70,35,logo_el,10,10,BLACK);
  display.display(); 
  delay(5000);
  display.clearDisplay();
}
