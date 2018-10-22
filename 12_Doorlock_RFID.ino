#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

int SER_Pin=A1;
int RCLK_Pin=6;
int SRCLK_Pin = 5;
#define number_of_74hc595s 3
#define numOfRegisterPins number_of_74hc595s*8
boolean registers[numOfRegisterPins];
Adafruit_PCD8544 display= Adafruit_PCD8544(8,9,10,12,11);
int count = 0;                                          // count = 0
char input[12];                                         // character array of size 12 
boolean flag = 0;                                       // flag =0
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
  Serial.begin(9600);
   digitalWrite(A4,HIGH);
  digitalWrite(A3,LOW);
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
  display.drawBitmap(70,35,logo_el,10 ,10,BLACK);
  display.drawRect(0,0,84,48,BLACK); 
   if(Serial.available())
   {
      count = 0;
      while(Serial.available() && count < 12)          // Read 12 characters and store them in input array
      {
         input[count] = Serial.read();
         count++;
         
      }
                                    // Print RFID tag number 
      
      if((input[0] ^ input[2] ^ input[4] ^ input[6] ^ input[8] == input[10]) && 
         (input[1] ^ input[3] ^ input[5] ^ input[7] ^ input[9] == input[11]))
         {
            
            display.setCursor(42,20);
            display.print("Door opened");
            display.setTextSize(1);
            display.setTextColor(BLACK);
            
            digitalWrite(A4,LOW);
            digitalWrite(A3,HIGH);
            delay(2000);
            display.display();
            delay(3000);
            display.clearDisplay();
            
         }}
      else
      {
            display.setCursor(4,20);
            display.print("Door closed");
            display.setTextSize(1);
            display.setTextColor(BLACK); 
             digitalWrite(A4,HIGH);
             digitalWrite(A3,LOW);
             display.display(); 
             delay(2000);
            display.clearDisplay(); 
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

//********************************learn2solder****************************************//

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
  display.println("DOOR");
  display.setCursor(6,30);
  display.println("SYSTEM");
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
void clearRegisters(){
  for(int i = numOfRegisterPins - 1; i >=  0; i--){
     registers[i] = LOW;
  }
  writeRegisters();
}
