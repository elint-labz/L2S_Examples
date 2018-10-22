#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <SPI.h>
 
//Inicializa o display
Adafruit_PCD8544 display = Adafruit_PCD8544(8, 9, 10, 12, 11);
#define number_of_74hc595s 3
#define numOfRegisterPins number_of_74hc595s*8
boolean registers[numOfRegisterPins];
#define LEFT 13
#define DOWN A2
#define RIGHT A0
#define UP A3
#define speakerPin 2
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
//Verifica em qual direcao a cobra esta se movendo
boolean dl = false, dr = false, du = false, dd = false;
 
int x[50], y[50], i, slength, tempx = 10, tempy = 10, xx, yy;
unsigned int high;
uint8_t bh, bl;
int xegg, yegg;
int freq, tb;
int l, r, u, d, p;
unsigned long time = 280, beeptime = 50;
int score = 0, flag = 0;
 
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
  display.display();
  delay(2000);
 
  pinMode(LEFT, INPUT);
  pinMode(RIGHT, INPUT);
  pinMode(UP, INPUT);
  pinMode(DOWN, INPUT);
  pinMode(speakerPin, OUTPUT);
  digitalWrite(LEFT, LOW);
  digitalWrite(RIGHT, LOW);
  digitalWrite(UP, LOW);
  digitalWrite(DOWN, LOW );
 
  slength = 8;
  xegg = (display.width()) / 2;
  yegg = (display.height()) / 2;
  display.clearDisplay();
  display.drawRect(0, 0, 84, 48, 1);
  display.setCursor(4, 2);
  display.print("P:     R:");
  display.setCursor(22, 2);
  display.print(score);
  display.setCursor(64, 2);
  display.print(high);
  display.drawRect(0, 0, 84, 11, 1);
  //Define coordenadas iniciais
  for (i = 0; i <= slength; i++)
  {
    x[i] = 25 - 3 * i;
    y[i] = 30;
  }
  for (i = 0; i < slength; i++)  //Draw the snake
  {
    display.drawCircle(x[i], y[i], 1, BLACK);
  }
  display.display();
  dr = true;
}
 
void loop()
{
  movesnake();
}
 
void movesnake()
{
  //Leitura do valor dos botoes
  l = digitalRead(LEFT);
  d = digitalRead(DOWN);
  r = digitalRead(RIGHT);
  u = digitalRead(UP);
 
  if (flag == 0)
  {
    direct();
  }
 
  if (millis() % time == 0)
  {
    if (flag == 0)
    {
      if (dr == true) {
        tempx = x[0] + 3;
        tempy = y[0];
      }
      if (dl == true) {
        tempx = x[0] - 3;
        tempy = y[0];
      }
      if (du == true) {
        tempy = y[0] - 3;
        tempx = x[0];
      }
      if (dd == true) {
        tempy = y[0] + 3;
        tempx = x[0];
      }
    }
    flag = 0;
    //Verifica se a cobra esta nas mesmas coordenadas do ovo
    checkgame();
    checkegg();
 
    //Altera as coordenadas de todos os pontos da cobra
    for (i = 0; i <= slength; i++)
    {
      xx = x[i];
      yy = y[i];
      x[i] = tempx;
      y[i] = tempy;
      tempx = xx;
      tempy = yy;
    }
    //Desenha a cobra e o ovo nas novas coordenadas
    drawsnake();
  }
}
 
void checkgame()
{
  for (i = 1; i < slength; i++)
  {
    //Verifica se o recorde foi batido e
    //mostra na tela o novo valor
    high = (((0xff00 + bh) << 8) + bl);
    if (score > high)
    {
      high = score;
      bh = (high >> 8);
      bl = high & 0xff;
      display.fillRect(61, 1, 20, 9, 0);
      display.setCursor(65, 2);
      display.print(high);
    }
    //Verifica se tocou nas bordas
    if ((x[0] <= 1 || x[0] >= 83) || (y[0] <= 12 || y[0] >= 47) || (x[i] == x[0] && y[i] == y[0]) )
    {
      //Jogo terminado. Mostra informacoes na tela
      display.clearDisplay();
      display.fillRoundRect(0, 0, 84 , 31, 1, 2);
      display.setTextColor(WHITE);
      display.setTextSize(2);
      display.setCursor(18, 1);
      display.print("GAME");
      display.setCursor(18, 16);
      display.print("OVER");
      display.setTextColor(BLACK);
      display.setTextSize(1);
      display.setCursor(12, 33);
      display.print("SCORE");
      display.setCursor(60, 33);
      display.print(score);
      display.setCursor(12, 41);
      display.print("RECORD");
      display.setCursor(60, 41);
      display.print(high);
      display.display();
      clearRegisters();
      //Aguarda 5 segundos e reinicia o jogo
      delay(5000);
 
      //Apaga a tela
      display.clearDisplay();
      //Retorna aos valores iniciais
      slength = 8;
      score = 0;
      time = 280;
      redraw();
    }
  }
}
 
void checkegg()      //Snake meets egg
{
  //Verifica se a cobra está nas mesmas coordenadas do ovo
  if (x[0] == xegg or x[0] == (xegg + 1) or x[0] == (xegg + 2) or x[0] == (xegg - 1))
  {
    if (y[0] == yegg or y[0] == (yegg + 1) or y[0] == (yegg + 2) or y[0] == (yegg - 1))
    {
      //Incrementa a cobra em uma posição, incrementa o placar e
      //aumenta a velocidade do jogo, diminuindo o tempo
      score += 1;
      lightup(score);
      
      display.fillRect(21, 1, 20, 9, 0);
      display.setCursor(22, 2);
      display.print(score);
      slength += 1;
      if (time >= 90)
      {
        time -= 10;
      }
      display.fillRect(xegg, yegg, 3, 3, WHITE);
      display.display();
      beep(35, beeptime);
      //Cria novo ovo em posicao aleatoria
      xegg = random(2, 80);
      yegg = random(15, 40);
    }
  }
}
 
void direct()
{
  //Altera o movimento caso uma tecla tenha sido pressionada
  if (l == HIGH and dr == false)
  {
    dl = true; du = false; dd = false;
    //Salva novas coordenadas em Tempx, Tempy
    tempx = x[0] - 3;
    tempy = y[0];
    flag = 1;
  }
  else if (r == HIGH and dl == false)
  {
    dr = true; du = false; dd = false;
    tempx = x[0] + 3;
    tempy = y[0];
    flag = 1;
  }
  else if (u == HIGH and dd == false)
  {
    du = true; dl = false; dr = false;
    tempy = y[0] - 3;
    tempx = x[0];
    flag = 1;
  }
  else if (d == HIGH and du == false)
  {
    dd = true; dl = false; dr = false;
    tempy = y[0] + 3;
    tempx = x[0];
    flag = 1;
  }
}
 
void drawsnake()
{
  display.fillRect(xegg, yegg, 3, 3, BLACK);
  display.drawCircle(x[0], y[0], 1, BLACK);
  display.drawCircle(x[slength], y[slength], 1, WHITE);
  display.display();
}
 
void redraw()
{
  display.drawRect(0, 0, 84, 48, 1);
  display.drawRect(0, 0, 84, 48, 1);
  display.setCursor(4, 2);
  display.print("P:     R:");
  display.drawRect(0, 0, 84, 11, 1);
  display.fillRect(21, 1, 20, 9, 0);
  display.setCursor(22, 2);
  display.print(score);
  display.fillRect(61, 1, 20, 9, 0);
  display.setCursor(65, 2);
  display.print(high);
 
  xegg = (display.width()) / 2;
  yegg = (display.height()) / 2;
  dl = false, dr = false, du = false, dd = false;
  dr = true;
  display.setCursor(4, 2);
  display.print("P:     R:");
  display.drawRect(0, 0, 84, 11, 1);
  //Retorna as coordenadas iniciais
  for (i = 0; i <= slength; i++)
  {
    x[i] = 25 - 3 * i;
    y[i] = 30;
  }
  tempx = 33 - 3 * i;
  tempy = 30;
  display.display();
}
 
void beep (int freq, long tb)
{
  int x;
  long delayAmount = (long)(500 / freq);
  long loopTime = (long)(tb / (delayAmount * 2));
  for (x = 0; x < loopTime; x++)
  {
    digitalWrite(speakerPin, HIGH);
    delay(delayAmount);
    digitalWrite(speakerPin, LOW);
    delay(delayAmount);
  }
  delay(2);
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
  display.println("SNAKE");
  display.setCursor(6,20);
  display.println("GAME");
  display.drawBitmap(70,35,logo_el,10,10,BLACK);
  display.display(); 
  delay(5000);
  display.clearDisplay();
}


void clearRegisters(){
  for(int i = numOfRegisterPins - 1; i >=  0; i--){
     registers[i] = LOW;
  }
  writeRegisters();
} 


//Set and display registers
//Only call AFTER all values are set how you would like (slow otherwise)
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

//set an individual pin HIGH or LOW
void setRegisterPin(int index, int value){
  registers[index] = value;
}


void lightup(int a){
  for(int i=0;i<a;i++)
  {
    
    setRegisterPin(i,HIGH);
    writeRegisters();
  
  }
}
