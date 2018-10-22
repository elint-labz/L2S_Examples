#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#define buzzer_pin 2
#define button1 13
#define button2 A0
#define button3 A2
#define button4 A3
#define relay_pin A5
#define number_of_74hc595s 3
#define numOfRegisterPins number_of_74hc595s*8
boolean registers[numOfRegisterPins];
char password[6]={'1','2','3','4','1','2'};
char pass_array[6];
uint8_t count=0;
int times=0;
int ret;
boolean previous_state = false;
uint8_t once=0;
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
void setup() 
{
pinMode(buzzer_pin,OUTPUT);
pinMode(button1,INPUT);
pinMode(button2,INPUT);
pinMode(button3,INPUT);
pinMode(button4,INPUT);
pinMode(relay_pin,OUTPUT);
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
display.drawBitmap(70,35,logo_el,10,10,BLACK);
display.drawRect(0,0,84,48,BLACK);  
}

void loop() {

    display.drawBitmap(70,35,logo_el,10,10,BLACK);
  display.drawRect(0,0,84,48,BLACK);
  display.clearDisplay();
  display.drawRect(0, 0, 84 , 48,BLACK);
  display.setCursor(4,8);
  display.println("ENTER YOUR");
  display.setCursor(4,18);
  display.print("PASSWORD");
  display.display();
while(1){
  
  if(digitalRead(button1)==HIGH)
  {
    display.setCursor(count,25);
    print_star();
    display.display();
    delay(500);
    pass_array[count]='1';
    count++;
  }
   if(digitalRead(button2)==HIGH)
  {
    display.setCursor(count+8,25);
    print_star();
    display.display();
    delay(500);
    pass_array[count]='2';
    count++;
  }
   if(digitalRead(button3)==HIGH)
  {
    display.setCursor(count+8,25);
    print_star();
    display.display();
    delay(500);
    pass_array[count]='3';
    count++;
  }
   if(digitalRead(button4)==HIGH)
  {
    display.setCursor(count+8,25);
    print_star();
    display.display();
    delay(500);
    pass_array[count]='4';
    count++;
  }
  if(count==6)
  {
     ret = check_password();
      if(ret==1)
      {
        relay();
        digitalWrite(buzzer_pin,LOW);
        display.setCursor(2,40);
        display.print("SUCCESS!!!!");
        Serial.println("SUCCESS");
        display.display();
        delay(2000);
        count=0;
        break;
      }
      else
      { 
        display.setCursor(2,40);
        display.print("FAILED!!!!");
        display.display();
       
        while(times<10)
        {
          times++;
          digitalWrite(2,HIGH);
          delay(300);
          digitalWrite(2,LOW);
          delay(300);
        }
        times=0;
       count=0;
       break;
      }
  }
 
}

}

//****************check password******************//
int check_password()
{
  int check;
      for(int l=0;l<6;l++)
      {
        if(password[l] == pass_array[l])
        {
          check++;
        }
      }
      if(check==6)
      {
        return 1;
      }
      else
      {
        return 0;
      }
      check=0;
}

//*************************print star for every button pressed*************//
void print_star()
{
  switch(count)
  {
    case 0 :  display.setCursor(2,28);
              display.print("*");
              break;
    case 1:   display.setCursor(8,28);
              display.print("*");
              break;
    case 2:   display.setCursor(14,28);
              display.print("*");
              break;
    case 3:   display.setCursor(20,28);
              display.print("*");
              break;
    case 4:   display.setCursor(26,28);
              display.print("*");
              break;
    case 5:   display.setCursor(32,28);
              display.print("*");
              break;
    case 6:   display.setCursor(38,28);
              display.print(" ");
              break;
   
              
  }
}
//********************TO TOGGLE LED*****************************//
void relay()
{
  
   if(previous_state==false)
   {
    digitalWrite(relay_pin,HIGH);
    previous_state=true;
   }
   else if(previous_state==true)
   {
    digitalWrite(relay_pin,LOW);
     previous_state=false;
   }
  
}

//***********************learn2solder**********************//
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

//***************functions to clear bar graph********************//
 void clearRegisters(){
  for(int i = numOfRegisterPins - 1; i >=  0; i--){
     registers[i] = LOW;
  }
  writeRegisters();
} 

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


//******************Title****************//
void display_title()
{
  display.drawRect(0, 0, 84 , 48,BLACK);
  display.setCursor(2,2);
  display.println("PASSWORD");
  display.setCursor(2,12);
  display.println("ENABLED HOME");
  display.setCursor(2,22);
  display.println("AUTOMATION");
  display.setCursor(2,32);
  display.println("SYSTEM"); 
  display.drawBitmap(70,35,logo_el,10,10,BLACK);
  display.display(); 
  delay(5000);
  display.clearDisplay();
}
