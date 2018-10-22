void setup() {
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
Adafruit_PCD8544 display= Adafruit_PCD8544(8,9,10,12,11);
#define number_of_74hc595s 3
#define numOfRegisterPins number_of_74hc595s*8
boolean registers[numOfRegisterPins];
#define left 13
#define down A2
#define right A0
#define up A3
int l,r,d,u,i,k,j,m,q=0;
int SER_Pin=A1;
int RCLK_Pin=6;
int SRCLK_Pin = 5;
//******************************void setup***********************************//
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
  pinMode(left, INPUT);
  pinMode(right, INPUT);
  pinMode(up, INPUT);
  pinMode(down, INPUT);
}
//********************************void loop*********************************//
void loop()
 {
  l = digitalRead(left);
  d = digitalRead(down);
  r = digitalRead(right);
  u = digitalRead(up); 
 
  if(l==LOW && q==0)
  {  
   display.setCursor(10,20);
   display.println("STOPWATCH : ");
   display.setTextSize(1);
   display.setTextColor(BLACK);
   display.fillCircle(4,4,2,BLACK);
   display.drawRect(0,0,10,10,BLACK);
   display.setCursor(12,4);
   display.println("START");
   display.fillCircle(76,4,2,BLACK);
   display.drawRect(72,0,10,10,BLACK);
   display.setCursor(48,4);
   display.println("STOP");
   display.fillCircle(4,40,2,BLACK);
   display.drawRect(0,36,10,10,BLACK);
   display.fillCircle(76,40,2,BLACK);
   display.drawRect(72,36,10,10,BLACK);
   display.display();
  }

  if(l==HIGH)
 { 
   display.clearDisplay();
   for(i=0;i<24;i++)
   {
     for(j=0;j<60;j++)
     {
       for(k=0;k<60;k++)
      {  
         for(m=0;m<55;m++)
        { 
          q=map(m,0,55,1,99);
          u = digitalRead(up); 
          if(u==HIGH)
           {
              break;
           } 
                display.fillCircle(4,4,2,BLACK);
                display.drawRect(0,0,10,10,BLACK);
                display.setCursor(12,2);
                display.println("START");
                display.fillCircle(76,4,2,BLACK);
                display.drawRect(72,0,10,10,BLACK);
                display.setCursor(48,2);
                display.println("STOP");
                display.fillCircle(4,40,2,BLACK);
                display.drawRect(0,36,10,10,BLACK);
                display.fillCircle(76,40,2,BLACK);
                display.drawRect(72,36,10,10,BLACK);
         if (q<10)
      { 
        display.setCursor(60,20);
        display.print('0');
        display.println(q);
      }
     else
      {
        display.setCursor(60,20);
        display.println(q);
      }
        if (k<10)
      {
        display.setCursor(42,20);
        display.print('0');
        display.println(k);
      }
     else
     {
        display.setCursor(42,20);
        display.println(k);
     }
       display.setCursor(54,20);
       display.print(':');
      if (j<10)
     {
 
       display.setCursor(24,20);
       display.print('0');
       display.println(j);
     }
     else
    {
       display.setCursor(24,20);
       display.println(j);
    }
       display.setCursor(36,20);
       display.print(':');
     if (i<10)
    {
 
       display.setCursor(6,20);
       display.print('0');
       display.print(i);
     }
    else
    {
       display.setCursor(6,20);
       display.print(i);
    }
       display.setCursor(19,20);
       display.print(":");
       display.display(); 
       display.clearDisplay();
    }
     if(u==HIGH)
   {
    break;
    }
   }
   if(u==HIGH)
   {
    break;
    }
   }
   if(u==HIGH)
   {
    break;
    }
   }
  }
 }
//****************************Elintlabz logo************************************//
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

//*****************************learn2solder*********************************//
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
//************************clearing registers****************************//
void clearRegisters()
{
  for(int i = numOfRegisterPins - 1; i >=  0; i--)
  {
     registers[i] = LOW;
   }
  writeRegisters();
} 
//**************************writting registers******************************//
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
