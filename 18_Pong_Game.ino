#define number_of_74hc595s 3
#define numOfRegisterPins number_of_74hc595s*8
boolean registers[numOfRegisterPins];
int SER_Pin=A1;
int RCLK_Pin=6;
int SRCLK_Pin = 5;
#include <LCD5110_Graph.h>
LCD5110 myGLCD(8,9,10,11,12);
extern uint8_t SmallFont[];
#define PIN_SCE   12
#define PIN_RESET 11
#define PIN_DC    10
#define PIN_SDIN  9
#define PIN_SCLK  8
#define LCD_C     LOW
#define LCD_D     HIGH
#define LCD_X     84
#define LCD_Y     6
int pot1,pot2;
int barWidth = 16;
int barHeight = 4;
int ballPerimeter = 4;
unsigned int bar1X = 0;
unsigned int bar1Y = 0;
unsigned int bar2X = 0;
unsigned int bar2Y = LCD_Y * 8 - barHeight;
int ballX = 0;
int ballY = 0;
boolean isBallUp = false;
boolean isBallRight = true;
void myGLCD_prg();
void Game();
byte pixels[LCD_X][LCD_Y];
unsigned long lastRefreshTime;
const int refreshInterval = 150;
byte gameState = 1;
byte ballSpeed = 2;
byte player1WinCount = 0;
byte player2WinCount = 0;
byte hitCount = 0;
int a,b,c,d;
int data=15;
int clock=5;
int latch=6;
int button1=13;
int button2=14;
int button4=17;
int button3=16;
int buzzer=2;
//****************void setup***********************************//
void setup(){
  pinMode(SER_Pin,OUTPUT);
  pinMode(RCLK_Pin,OUTPUT);
  pinMode(SRCLK_Pin,OUTPUT);
  clearRegisters();
  writeRegisters();
  LcdInitialise();
  myGLCD_prg();
  restartGame();
}
//*******************void loop*************************************//
void loop(){
  unsigned long now = millis();
  if(now - lastRefreshTime > refreshInterval){
      update();
      refreshScreen();
      lastRefreshTime = now;  
  }  
}
//*********************restart game**********************************//
void restartGame(){
   ballSpeed = 1;
   gameState = 1;
   ballX = random(0, 60);
   ballY = 20;
   isBallUp = false; 
   isBallRight = true;
   hitCount = 0;
}
//*********************refreshing the screeen*****************************//
void refreshScreen()
{
  if(gameState == 1){
    for(int y = 0; y < LCD_Y; y++){
        for(int x = 0; x < LCD_X; x++){
           byte pixel = 0x00;
           int realY = y * 8;
           // draw ball if in the frame
           if(x >= ballX && x <= ballX + ballPerimeter -1 && ballY + ballPerimeter > realY && ballY < realY + 8 ){
             byte ballMask = 0x00;
             for(int i = 0; i < realY + 8 - ballY; i++){
               ballMask = ballMask >> 1;
               if(i < ballPerimeter)
                 ballMask = 0x80 | ballMask;
             }
             pixel = pixel | ballMask;
           }
           
           // draw bars if in the frame
           if(x >= bar1X && x <= bar1X + barWidth -1 && bar1Y + barHeight > realY && bar1Y < realY + 8 ){
             byte barMask = 0x00;
             for(int i = 0; i < realY + 8 - bar1Y; i++){
               barMask = barMask >> 1;
               if(i < barHeight)
                 barMask = 0x80 | barMask;
             }
             pixel = pixel | barMask;
           }
           
           if(x >= bar2X && x <= bar2X + barWidth -1 && bar2Y + barHeight > realY && bar2Y < realY + 8 ){
             byte barMask = 0x00;
             for(int i = 0; i < realY + 8 - bar2Y; i++){
               barMask = barMask >> 1;
               if(i < barHeight)
                 barMask = 0x80 | barMask;
             }
             pixel = pixel | barMask;
           }
           LcdWrite(LCD_D, pixel);
         }
    }
  } else if(gameState == 2){
      
  }
}
//*******************************updating LCD*****************************//
void update(){
  if(gameState == 1){
     int barMargin = LCD_X - barWidth;
     int var1 = digitalRead(button1);
     int var2 = digitalRead(button4);
     int var3 = digitalRead(button2);
     int var4 = digitalRead(button3);
     if(var1 == HIGH)
     {
       pot1 = pot1 - 50;
     }
      if(var2 == HIGH)
     {
       pot1 = pot1 + 50;
     }
     if(var3 == HIGH)
     {
       pot2 = pot2 - 50;
     }
      if(var4 == HIGH)
     {
       pot2 = pot2 + 50;
     }
     if(pot1 <0)
     {
       pot1=0;
     }
     else if (pot1>1023)
     {
      pot1=1023;
     }
     if(pot2 <0)
     {
       pot2=0;
     }
     else if (pot2>1023)
     {
      pot2=1023;
     } 
       
     bar1X = pot1 / 2 * LCD_X / 512;
     bar2X = pot2 / 2 * LCD_X / 512;
    
     if(bar1X > barMargin) bar1X = barMargin;
     if(bar2X > barMargin) bar2X = barMargin;
     
     //move the ball now
     if(isBallUp)
       ballY -= ballSpeed;
     else
       ballY += ballSpeed;
     if(isBallRight)
       ballX += ballSpeed;
     else
       ballX -= ballSpeed;
     //check collisions  
     if(ballX < 1){
       isBallRight = true;
       ballX = 0;
     }
     else if(ballX > LCD_X - ballPerimeter - 1){
       isBallRight = false;
       ballX = LCD_X - ballPerimeter;
     }
     if(ballY < barHeight){
       if(ballX + ballPerimeter >= bar1X && ballX <= bar1X + barWidth){ // ball bounces from bar1
         isBallUp = false;
         if(ballX + ballPerimeter/2 < bar1X + barWidth/2)
           isBallRight = false;
         else
           isBallRight = true;
         ballY = barHeight;
         if(++hitCount % 10 == 0 && ballSpeed < 5) 
           ballSpeed++;//ball speed increase
       }else{ //player2 wins
         gameState = 2;
         player2WinCount++;
       }
     }
     if(ballY + ballPerimeter > LCD_Y * 8 - barHeight){
       if(ballX + ballPerimeter >= bar2X && ballX <= bar2X + barWidth){ //ball bounces from bar2
         isBallUp = true; 
         if(ballX + ballPerimeter/2 < bar2X + barWidth/2)
           isBallRight = false;
         else
           isBallRight = true;
         ballY = LCD_Y * 8 - barHeight - ballPerimeter;
         if(++hitCount % 10 == 0 && ballSpeed < 5) 
           ballSpeed++; // ball speed increase
       }else{ // player 1 wins
         gameState = 2;
         player1WinCount++;
       }
     }
  }else if(gameState == 2){
      for(int i =0; i < 2; i++){
        LcdWrite(LCD_C, 0x0D );  // LCD in inverse mode.
        delay(300);
        LcdWrite(LCD_C, 0x0C );  // LCD in inverse mode.
        delay(300);
        digitalWrite(buzzer,HIGH);
        delay(1000);
        digitalWrite(buzzer,LOW);
      }
    restartGame();
  }  
}
//************************LCD initialising**************************//
void LcdInitialise(void){
  pinMode(13,OUTPUT);
  pinMode(A0,OUTPUT);
  pinMode(A2,OUTPUT);
  pinMode(A3,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(PIN_SCE, OUTPUT);
  pinMode(PIN_RESET, OUTPUT);
  pinMode(PIN_DC, OUTPUT);
  pinMode(PIN_SDIN, OUTPUT);
  pinMode(PIN_SCLK, OUTPUT);
  delay(200);
  digitalWrite(PIN_RESET, LOW);
  delay(500);
  digitalWrite(PIN_RESET, HIGH);
  LcdWrite(LCD_C, 0x21 );  // LCD Extended Commands.
  LcdWrite(LCD_C, 0xB1 );  // Set LCD Vop (Contrast). 
  LcdWrite(LCD_C, 0x04 );  // Set Temp coefficent. //0x04
  LcdWrite(LCD_C, 0x14 );  // LCD bias mode 1:48. //0x13
  LcdWrite(LCD_C, 0x0C );  // LCD in normal mode.
  LcdWrite(LCD_C, 0x20 );
  LcdWrite(LCD_C, 0x80 ); //select X Address 0 of the LCD Ram
  LcdWrite(LCD_C, 0x40 ); //select Y Address 0 of the LCD Ram - Reset is not working for some reason, so I had to set these addresses
  LcdWrite(LCD_C, 0x0C );
}
//*****************************writting to LCD**************************//
void LcdWrite(byte dc, byte data){
  digitalWrite(PIN_DC, dc);
  digitalWrite(PIN_SCE, LOW);
  shiftOut(PIN_SDIN, PIN_SCLK, MSBFIRST, data);
  digitalWrite(PIN_SCE, HIGH);
}
//***************************display name********************************//
void myGLCD_prg()
{
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  display_El_logo();
  display_L2S();
  while(1)
  {  
   myGLCD.drawRect(0,0,83,47);
   myGLCD.print(" PONG",25,10);
   myGLCD.print(" GAME",25,30);
   myGLCD.update();
   a=digitalRead(button1);
   delay(50);
   if(a==HIGH)
   {
     Game();
     break;
   }
  }
}
//**************************Elintlabz logo*****************************//
void display_El_logo()
{
for(int j=2;j<8;j++)
      {
       myGLCD.drawLine(15,j,35,j);
      }
      for(int j=12;j<18;j++)
      {
       myGLCD.drawLine(15,j,35,j);
      }
      for(int j=22;j<28;j++)
      {
       myGLCD.drawLine(15,j,35,j);
      }

      
      for(int j=45;j<51;j++)
      {
       myGLCD.drawLine(j,2,j,27);
      }

      for(int j=22;j<28;j++)
      {
       myGLCD.drawLine(45,j,65,j);
      }
   myGLCD.print("ElintLabz",15,35);
   myGLCD.update(); 
   delay(5000);
   myGLCD.clrScr();
}
//*****************************learn2solder*********************************//
void display_L2S()
{
   myGLCD.drawRect(0, 0, 83 , 47);
   myGLCD.print("LEARN",26,10);
   myGLCD.print("2",38,20);
   myGLCD.print("SOLDER",22,30);
   myGLCD.update(); 
   delay(5000);
   myGLCD.clrScr();
}
//**************************void game*******************************//
void Game()
{
  
}
//*************************clearing registers***************************//
 void clearRegisters(){
  for(int i = numOfRegisterPins - 1; i >=  0; i--){
     registers[i] = LOW;
  }
  writeRegisters();
} 
//**************************writting registers***************************// 
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
