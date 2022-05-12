 /*
    X -> 240px
 ------------------------
 |                      |
 |                      |
 |                      |   Y
 |                      |   |
 |                      |   V
 |                      |  320px
 |                      |
 |                      |
 |                      |
 |                      |
 |                      |
 ------------------------
 */

#include <Arduino.h>

#ifdef ARDUINO_ARCH_STM32
#include <USBSerial.h>
#define Serial SerialUSB
#elif defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F4)
#include <usb_serial.h>
#endif

#include <SPI.h>

SPIClass SPI_2(PB15, PB14, PB13);

#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341_STM.h"


#if defined(LED_BUILTIN)
#define  BOARD_LED_PIN LED_BUILTIN
#endif

#define panjangLCD 320
#define lebarLCD 220
double jumlahData=2*3.14;

double besarStep=jumlahData/panjangLCD;
double tengahX=lebarLCD/2;
double maxAmplitude=(lebarLCD/2)-1;
int y,yt1,yt2;
double x,xt1,xt2;
double x2,xt21,xt22;

unsigned long testFillScreen();
unsigned long testText();
unsigned long testLines(uint16_t color);
unsigned long testFastLines(uint16_t color1, uint16_t color2);
unsigned long testRects(uint16_t color);
unsigned long testFilledRects(uint16_t color1, uint16_t color2);
unsigned long testFilledCircles(uint8_t radius, uint16_t color);
unsigned long testCircles(uint8_t radius, uint16_t color);
unsigned long testTriangles();
unsigned long testFilledTriangles();
unsigned long testRoundRects();
unsigned long testFilledRoundRects();
void sleep(int count);
void diag();

void LCD_MenuAwal();

#define TFT_CS    PA10
#define TFT_CLK   PB13
#define TFT_MISO  PB14
#define TFT_MOSI  PB15
#define TFT_RST   PA9
#define TFT_DC    PA8

#define BACKLIGHT PC9




// This use hardware SPI and the above pins for CS/DC and lcd reset pin
// the LCD reset pin is important, the display often can't initialise if
// lcd reset pin is not setup or connected correctly
Adafruit_ILI9341_STM tft = Adafruit_ILI9341_STM(TFT_CS, TFT_DC, TFT_RST);


void setupLCD() {

  SPI_2.begin(); 
  
  pinMode(BOARD_LED_PIN, OUTPUT);
  digitalWrite(BOARD_LED_PIN, LOW);

  tft.begin(SPI_2);

  pinMode(BACKLIGHT, OUTPUT);
  digitalWrite(BACKLIGHT, HIGH);
}



void contohBikinTeks()
{
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
  tft.println("COBA!!!");
}

void contohBikinGaris()
{
  uint16_t color;
  color=ILI9341_RED;
  unsigned long start, t;
  int           x1, y1, x2, y2,
                w = tft.width(),
                h = tft.height();

  tft.fillScreen(ILI9341_BLACK);
  yield();
  
  x1 = y1 = 0;
  y2    = h - 230;
  x2 = w-230;
  start = micros();
  tft.drawLine(x1, y1, x2, y2, color);
  t     = micros() - start; // fillScreen doesn't count against timing
}

void contohBikinGarisHorizontalAtauVertikal()
{
  unsigned long start;
  int           x, y, w = tft.width(), h = tft.height();

  uint16_t color1, color2;

  color1=ILI9341_GREEN;
  color2=ILI9341_YELLOW;

  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  tft.drawFastHLine(0, y, w, color1);
  tft.drawFastVLine(x, 0, h, color2);

  
}

void contohBikinKotak()
{
  uint16_t color=ILI9341_YELLOW;
  unsigned long start;
  int           n, i, i2,
                cx = tft.width()  / 2,
                cy = tft.height() / 2;

  tft.fillScreen(ILI9341_BLACK);
  start = micros();
    tft.drawRect(40, 20, 150, 100, color);
    //           coordX,coordY,lengthX,lengthY
}

void contohBikinLingkaran()
{
  unsigned long start;
  int           x, y, r2, radius;
  uint16_t color=ILI9341_YELLOW;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.

  x=100;
  y=100;
  radius=90;

  tft.drawCircle(x, y, radius, color);
}



void contohRotasi()
{
  for(uint8_t rotation=0; rotation<4; rotation++) {
      tft.setRotation(rotation);
      testText();
    }
}




//-------------------------------------------------------------------

void diag() {
  uint8_t cmdbuf[10];
  // read diagnostics (optional but can help debug problems)

  tft.readcommandx(cmdbuf, ILI9341_RDDID, 4);
  Serial.print("Display id: 0x");
  for(int i=0;i<3;i++){
    if(i>0) Serial.print(',');
    Serial.print(cmdbuf[i], HEX);
  }
  Serial.println();

  tft.readcommandx(cmdbuf, ILI9341_RDDST,5);
  Serial.print("Display status: 0x");
  for(int i=0;i<5;i++){
    if(i>0) Serial.print(',');
    Serial.print(cmdbuf[i], HEX);
  }
  Serial.println();

  tft.readcommandx(cmdbuf, ILI9341_RDMODE,2);
  Serial.print("Display Power Mode: 0x");
  Serial.println(cmdbuf[0], HEX);
  Serial.println(cmdbuf[1], HEX);

  tft.readcommandx(cmdbuf, ILI9341_RDMADCTL,2);
  Serial.print("MADCTL Mode: 0x");
  Serial.println(cmdbuf[0], HEX);
  Serial.println(cmdbuf[1], HEX);

  tft.readcommandx(cmdbuf, ILI9341_RDPIXFMT,2);
  Serial.print("Pixel Format: 0x");
  Serial.println(cmdbuf[0], HEX);
  Serial.println(cmdbuf[1], HEX);

  Serial.println("set pix format 16 bits");
  tft.writecommand(ILI9341_PIXFMT);
  tft.writedata(0x55);

  tft.readcommandx(cmdbuf, ILI9341_RDPIXFMT,2);
  Serial.print("Pixel Format: 0x");
  Serial.println(cmdbuf[0], HEX);
  Serial.println(cmdbuf[1], HEX);

  tft.readcommandx(cmdbuf, ILI9341_RDIMGFMT, 2);
  Serial.print("Image Format: 0x");
  Serial.println(cmdbuf[0], HEX);
  Serial.println(cmdbuf[1], HEX);

  /*
  tft.readcommandxx(cmdbuf, ILI9341_RDSELFDIAG, 1);
  Serial.print("Self Diagnostic: 0x");
  Serial.println(cmdbuf[0], HEX);

  Serial.println("executing sleep in");
  tft.writecommand(ILI9341_SLPIN);
  delay(10);
  Serial.println("executing sleep out");
  tft.writecommand(ILI9341_SLPOUT);
  delay(500);
  */

  tft.readcommandx(cmdbuf, ILI9341_RDSELFDIAG, 2);
  Serial.print("Self Diagnostic: 0x");
  Serial.println(cmdbuf[0], HEX);
  Serial.println(cmdbuf[1], HEX);


}


unsigned long testFillScreen() {
  uint16_t colors[5] = {
  ILI9341_BLACK,
  ILI9341_RED,
  ILI9341_GREEN,
  ILI9341_BLUE,
  ILI9341_BLACK
  };
  unsigned long start = micros();
  for(int i=0; i< 5; i++) {
  tft.fillScreen(colors[i]);
  yield();
  //sleep(1000);
  }
  return micros() - start;
}

unsigned long testText() {
  tft.fillScreen(ILI9341_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(ILI9341_RED);    tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(5);
  tft.println("Groop");
  tft.setTextSize(2);
  tft.println("I implore thee,");
  tft.setTextSize(1);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
  return micros() - start;
}

unsigned long testLines(uint16_t color) {
  unsigned long start, t;
  int           x1, y1, x2, y2,
                w = tft.width(),
                h = tft.height();

  tft.fillScreen(ILI9341_BLACK);
  yield();
  
  x1 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t     = micros() - start; // fillScreen doesn't count against timing

  yield();
  tft.fillScreen(ILI9341_BLACK);
  yield();

  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  yield();
  tft.fillScreen(ILI9341_BLACK);
  yield();

  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  yield();
  tft.fillScreen(ILI9341_BLACK);
  yield();

  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);

  yield();
  return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2) {
  unsigned long start;
  int           x, y, w = tft.width(), h = tft.height();

  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  for(y=0; y<h; y+=5) tft.drawFastHLine(0, y, w, color1);
  for(x=0; x<w; x+=5) tft.drawFastVLine(x, 0, h, color2);

  return micros() - start;
}

unsigned long testRects(uint16_t color) {
  unsigned long start;
  int           n, i, i2,
                cx = tft.width()  / 2,
                cy = tft.height() / 2;

  tft.fillScreen(ILI9341_BLACK);
  n     = min(tft.width(), tft.height());
  start = micros();
  for(i=2; i<n; i+=6) {
    i2 = i / 2;
    tft.drawRect(cx-i2, cy-i2, i, i, color);
  }

  return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
  unsigned long start, t = 0;
  int           n, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  n = min(tft.width(), tft.height());
  for(i=n; i>0; i-=6) {
    i2    = i / 2;
    start = micros();
    tft.fillRect(cx-i2, cy-i2, i, i, color1);
    t    += micros() - start;
    // Outlines are not included in timing results
    tft.drawRect(cx-i2, cy-i2, i, i, color2);
    yield();
  }

  return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  for(x=radius; x<w; x+=r2) {
    for(y=radius; y<h; y+=r2) {
      tft.fillCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int           x, y, r2 = radius * 2,
                w = tft.width()  + radius,
                h = tft.height() + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros();
  for(x=0; x<w; x+=r2) {
    for(y=0; y<h; y+=r2) {
      tft.drawCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testTriangles() {
  unsigned long start;
  int           n, i, cx = tft.width()  / 2 - 1,
                      cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  n     = min(cx, cy);
  start = micros();
  for(i=0; i<n; i+=5) {
    tft.drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      tft.color565(i, i, i));
  }

  return micros() - start;
}

unsigned long testFilledTriangles() {
  unsigned long start, t = 0;
  int           i, cx = tft.width()  / 2 - 1,
                   cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  for(i=min(cx,cy); i>10; i-=5) {
    start = micros();
    tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.color565(0, i*10, i*10));
    t += micros() - start;
    tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.color565(i*10, i*10, 0));
    yield();
  }

  return t;
}

unsigned long testRoundRects() {
  unsigned long start;
  int           w, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  w     = min(tft.width(), tft.height());
  start = micros();
  for(i=0; i<w; i+=6) {
    i2 = i / 2;
    tft.drawRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(i, 0, 0));
  }

  return micros() - start;
}

unsigned long testFilledRoundRects() {
  unsigned long start;
  int           i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  for(i=min(tft.width(), tft.height()); i>20; i-=6) {
    i2 = i / 2;
    tft.fillRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(0, i, 0));
    yield();
  }

  return micros() - start;
}

/*
 * "wfi" is wait-for-interrupt, systick triggers every millisecond
 * and maybe there is usb start of frames interrupts etc. that would wakeup
 * this and increase the count. this keeps the mcu running cool ;)
 */
void sleep(int count) {
  for(int i=0; i<count; i++) {
    asm("wfi");
  }
}

void contohBikinSegitiga()
{
  int xs1,xs2,xs3,ys1,ys2,ys3;

  xs1=150; ys1=100;
  xs2=155; ys2=105;
  xs3=150; ys3=110;
  tft.drawTriangle(
    xs1, ys1, // peak
    xs2, ys2, // bottom left
    xs3, ys3, // bottom right
    ILI9341_YELLOW);
}

void segitiga1()
{
  int xs1,xs2,xs3,ys1,ys2,ys3;

  xs1=150; ys1=102;
  xs2=155; ys2=107;
  xs3=150; ys3=112;
  tft.drawTriangle(
    xs1, ys1, // peak
    xs2, ys2, // bottom left
    xs3, ys3, // bottom right
    ILI9341_YELLOW);
}

void segitiga2()
{
  int xs1,xs2,xs3,ys1,ys2,ys3;

  xs1=150; ys1=132;
  xs2=155; ys2=137;
  xs3=150; ys3=142;
  tft.drawTriangle(
    xs1, ys1, // peak
    xs2, ys2, // bottom left
    xs3, ys3, // bottom right
    ILI9341_YELLOW);
}

void LCD_MenuAwal()
{
//  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(3);
  
  tft.setCursor(40, 10);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(3);
  tft.println("STEPPER MOTOR");

  tft.setCursor(60, 40);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(3);
  tft.println("ANALYZER");

  tft.setCursor(10, 100);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(2);
  tft.println("MAX Current");

  tft.setCursor(10, 130);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(2);
  tft.println("Revolution");

//  tft.setCursor(10, 160);
//  tft.setTextColor(ILI9341_WHITE);  
//  tft.setTextSize(2);
//  tft.println("Inductance");

  tft.setCursor(160, 100);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(2);
  tft.println(":      mA");

  tft.setCursor(160, 130);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(2);
  tft.println(":"); 

  tft.setCursor(170, 100);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(2);
  tft.println("500");

  tft.setCursor(170, 130);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(2);
  tft.println("1");

//  tft.setCursor(160, 160);
//  tft.setTextColor(ILI9341_WHITE);  
//  tft.setTextSize(2);
//  tft.println(":");

  tft.setCursor(20, 200);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(1);
  tft.println("A : Edit MAX Current   B : Edit Revolution"); 

  tft.setCursor(20, 220);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(1);
  tft.println("* : Delete             D : Next");
}

void LCD_UkurArus()
{
  uint16_t warnaGaris;
  warnaGaris=ILI9341_YELLOW;

  tft.fillScreen(ILI9341_BLACK);

  tft.setRotation(3);
  tft.setCursor(10, 220);
  tft.setTextColor(ILI9341_GREEN);  
  tft.setTextSize(1);
  tft.println("x=step   y=current");

//  tft.setCursor(160, 230);
//  tft.setTextColor(ILI9341_WHITE);
//  tft.setTextSize(1);
//  tft.println(" D : Next"); 

  tft.setRotation(3);
  // for(y=0;y<=panjangLCD;y++)
  // {
  //   x=maxAmplitude*sin(y*besarStep);
  //   yt2=y;
  //   xt2=x+tengahX;

  //   tft.drawLine(xt1, yt1, xt2, yt2, warnaGaris);

    // yt1=yt2;
    // xt1=xt2;
    
  //   delay(1); 
  // }
}

void loadingScreen()
{
  tft.fillScreen(ILI9341_BLACK);

  tft.setRotation(3);
  tft.setCursor(10, 100);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("Sedang Mengukur"); 

  tft.setRotation(3);
  tft.setCursor(10, 130);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("Induktansi . . ."); 
  
  // tft.setCursor(10, 120);
  // tft.setTextColor(ILI9341_WHITE);
  // tft.setTextSize(1);
  // tft.println("ke socket Inductance Meter!"); 

  // tft.setCursor(10, 230);
  // tft.setTextColor(ILI9341_WHITE);
  // tft.setTextSize(1);
  // tft.println("A : Input Current   D : Input Steps/Rev"); 

//  tft.setCursor(160, 230);
//  tft.setTextColor(ILI9341_WHITE);
//  tft.setTextSize(1);
//  tft.println("               D : Next"); 
  
}

void LCD_UkurInduktansi()
{
  tft.fillScreen(ILI9341_BLACK);

  tft.setRotation(3);
  tft.setCursor(10, 100);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("Phase A : "); 

  tft.setRotation(3);
  tft.setCursor(140, 100);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println(inductance); 

  tft.setRotation(3);
  tft.setCursor(250, 100);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("uH"); 
  
  tft.setRotation(3);
  tft.setCursor(10, 130);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("Phase B : "); 

  tft.setRotation(3);
  tft.setCursor(140, 130);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println(inductance2); 

  tft.setRotation(3);
  tft.setCursor(250, 130);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("uH"); 

  tft.setCursor(160, 220);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(1);
  tft.println("D : Next"); 
}

void LCD_HasilUji()
{
  tft.fillScreen(ILI9341_BLACK);

  tft.setRotation(3);
  tft.setCursor(20, 30);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("Connection :");

  tft.setRotation(3);
  tft.setCursor(20, 60);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("Amplitude  :");

  tft.setRotation(3);
  tft.setCursor(20, 90);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("Inductance :");
  

  tft.setCursor(200, 220);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(1);
  tft.println("D : Home"); 
  
//  tft.setRotation(3);
//  tft.setCursor(10, 130);
//  tft.setTextColor(ILI9341_WHITE);
//  tft.setTextSize(2);
//  tft.println(" "); 
}
