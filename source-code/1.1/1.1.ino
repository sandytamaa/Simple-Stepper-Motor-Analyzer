#define maxDigitArus      10
#define triggerInductance PB10
#define triggerA          PC5
#define triggerB          PB1
#define opbC              PA3
#define opbCC             PA4
#define ADC1              PA0
#define ADC2              PA1
#define pulseIN1          PB12
#define pulseIN2          PC6

int failure;
int i, j;
int simpanArus[10240];
int simpanArus2[10240];
int simpanArusSementara;
int simpanArusSementara2;
char inputedKey;
int flac;
int arusRaw[maxDigitArus];
int arus;
int revRaw[maxDigitArus];
int revolution;
int inductanceRaw[maxDigitArus];
int inductanceSpec;
double inductanceError;

double amplitudeA, amplitudeB;
double amplitudeAatas, amplitudeBatas;
double amplitudeAbawah, amplitudeBbawah;
double Apeaktopeak, Bpeaktopeak;

double pulse, frequency, capacitance, inductance;
double pulse2, frequency2, capacitance2, inductance2;

// #include <STM32FreeRTOS.h>
#include "TFT_LCD.h"
#include "scanningKeypad.h"
#include "drivingStepper.h"
#include "userFunction.h"
#include "inductanceMeter.h"


char pilihanMenu1, pilihanMenu2, pilihanMenu3;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  setupLCD();
  setupInductance();
  SPI.begin();

  pinMode(triggerA, OUTPUT);
  pinMode(triggerB, OUTPUT);
  pinMode(ADC1,INPUT);
  pinMode(ADC2,INPUT);
  analogReadResolution(12);
  
  pinMode(EN_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
 
  
  tft.fillScreen(ILI9341_BLACK);
}

void loop() {
  
  digitalWrite(EN_PIN, HIGH);

  digitalWrite(triggerA, HIGH);
  digitalWrite(triggerB, HIGH);

  delay(500);

  for(i=0;i<=maxDigitArus;i++)
  {
    arusRaw[i]=0;
    revRaw[i]=0;
  }

//  for(i=0;i<=10240;i++)
//  {
//    simpanArus[i]=0;
//    simpanArus2[i]=0;
//  }
  
  failure=0;
  digitNow=0;

  tft.fillScreen(ILI9341_BLACK);
  LCD_MenuAwal();

  inputedKey=NO_KEY;
  
  while(inputedKey!='D')
  {
    if(inputedKey!='A' && inputedKey!='B')
    {
      inputedKey=bacaKeypad();
    }
    
    if(inputedKey=='A')
    {
      for(i=0;i<=maxDigitArus;i++)
      {
        arusRaw[i]=0;
      }
      tft.fillRect(170, 95, 100, 20, ILI9341_BLACK);
      segitiga1();
      xkeypad=10;
      ykeypad=100;
      inputedKey=NO_KEY;
      flac=0;
      digitNow=0;
      
      while(inputedKey!='D' && inputedKey!='B')
      {
        inputedKey=bacaKeypad();
        inputArus();
      }
      
      arus=0;

      for(i=1;i<=flac;i++)
      {
        arus=arus+(arusRaw[i]*pow(10,flac-i));
      }
      
      tft.fillRect(149, 101, 7, 20, ILI9341_BLACK);
    }
    if(inputedKey=='B')
    {
      for(i=0;i<=maxDigitArus;i++)
      {
        revRaw[i]=0;
      }
      tft.fillRect(170, 125, 100, 20, ILI9341_BLACK);
      segitiga2();
      xkeypad=10;
      ykeypad=130;
      inputedKey=NO_KEY;
      flac=0;
      digitNow=0;
      
      while(inputedKey!='D' && inputedKey!='A')
      {
        inputedKey=bacaKeypad();
        inputRevolution();
      }
      
      revolution=0;

      for(i=1;i<=flac;i++)
      {
        revolution=revolution+(revRaw[i]*pow(10,flac-i));
      }
      
      tft.fillRect(149, 131, 7, 12, ILI9341_BLACK);
    }

  }

  Serial.println(arus);
  Serial.println(revolution);

  if(arus==0)
  {
    arus=500;
  }
  if(revolution==0)
  {
    revolution=1;
  }
  
  tft.fillScreen(ILI9341_BLACK);
  LCD_UkurArus();

  setupDriverStepper(arus*0.7071);
  delay(500);
  
  int i=0;
  j=0;

  xt1=120;
  yt1=0;
  tft.setRotation(2);

  amplitudeA=0;
  amplitudeB=0;
  amplitudeAatas=0;
  amplitudeBatas=0;
  amplitudeAbawah=0;
  amplitudeBbawah=0;

  simpanArusSementara=0;
  simpanArusSementara2=0;

  for(i=0;i<=2560;i++)
  {
    driveStepperCCW();
  }

  delay(100);

  while(i<102400 && revolution>0)
  {
    driveStepperCCW();

    if(i%10==0 || (i%10<=9 && i%10>=7))
    {
      simpanArusSementara+=analogRead(ADC1);
      simpanArusSementara2+=analogRead(ADC2);
    }
    

    if(i%10==0)
    {
      simpanArus[i/10]=simpanArusSementara/4;
      simpanArus2[i/10]=simpanArusSementara2/4;

      simpanArusSementara=0;
      simpanArusSementara2=0;

      Serial.println(simpanArus[i/10]);

      if((simpanArus[i/10]-2047)>amplitudeAatas)
      {
        amplitudeAatas=simpanArus[i/10]-2047;
      }

      if((simpanArus[i/10]-2047)<amplitudeAbawah)
      {
        amplitudeAbawah=simpanArus[i/10]-2047;
      }

      if((simpanArus2[i/10]-2047)>amplitudeBatas)
      {
        amplitudeBatas=simpanArus2[i/10]-2047;
      }

      if((simpanArus2[i/10]-2047)<amplitudeBbawah)
      {
        amplitudeBbawah=simpanArus2[i/10]-2047;
      }

      tft.fillRect(14, j, 225, 5, ILI9341_BLACK);

      xt2=simpanArus[i/10]/17.1;
      xt22=simpanArus2[i/10]/17.1;
      yt2=j;
//      Serial.print(xt1);
//      Serial.print("\t");
//      Serial.print(xt2);
//      Serial.print("\t");
//      Serial.print(yt1);
//      Serial.print("\t");
//      Serial.println(yt2);
      tft.drawLine(xt1, yt1, xt2, yt2, ILI9341_YELLOW);
      tft.drawLine(xt21, yt1, xt22, yt2, ILI9341_RED);
      j++;
      xt1=xt2;
      xt21=xt22;
      yt1=yt2;
    }
    
    if(j>319)
    {
      j=0;
      yt1=0;
    }
    
    i++;
    if(i==102399)
    {
      i=0;
      revolution--;
    }
    
  } 
  tft.setRotation(3);
  tft.setCursor(160, 220);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(1);
  tft.println("D : Next"); 

  
  i=0;


  Apeaktopeak=amplitudeAatas-amplitudeAbawah;
  Bpeaktopeak=amplitudeBatas-amplitudeBbawah;

  if(Apeaktopeak<400 || Bpeaktopeak<400)
  {
    failure++;
    Serial.println("BAD CONNECTION");
  }
  
  Serial.println("Peak to Peak");
  Serial.println(Apeaktopeak);
  Serial.println(Bpeaktopeak);
  Serial.println("Beda Peak to Peak");
  Serial.println(abs(Apeaktopeak-Bpeaktopeak));
  Serial.println("Beda Arus Peak to Peak");
  Serial.println((abs(Apeaktopeak-Bpeaktopeak)*3.3/4095)/(0.015*20)*1000);

  if(((abs(Apeaktopeak-Bpeaktopeak)*3.3/4095)/(0.015*20)*1000)>(arus*0.3))
  {
    failure++;
    Serial.println("BEDA AMPLITUDE");
  } 

  digitalWrite(EN_PIN, HIGH);

  inputedKey=NO_KEY;

  while(inputedKey!='D')
  {
    inputedKey=bacaKeypadTanpaPrint();
  }
  tft.fillScreen(ILI9341_BLACK);
  


// -----------------------------

  inductance=0;
  inductance2=0;
  
  if(failure==0)
  {
    loadingScreen();

    cekInductance();
    inductanceError=abs(inductance-inductance2)/((inductance+inductance2)/2);
    if(inductanceError>0.25)
    {
      failure+=1;
      Serial.println("BEDA INDUKTANCE");
    }
    tft.fillScreen(ILI9341_BLACK);
    LCD_UkurInduktansi();
  
    inputedKey=NO_KEY;
  
    while(inputedKey!='D')
    {
      inputedKey=bacaKeypadTanpaPrint();
    }
  }
  


// -----------------------------
  digitalWrite(EN_PIN, HIGH);

  digitalWrite(triggerA, HIGH);
  digitalWrite(triggerB, HIGH);


  tft.fillScreen(ILI9341_BLACK);

  LCD_HasilUji();

  tft.setRotation(3);
  tft.setCursor(170, 30);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  
  if(Apeaktopeak<400 || Bpeaktopeak<400)
  {
    tft.println("NOT Good");
     
    tft.setRotation(3);
    tft.setCursor(170, 60);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.println("Not Tested");
  }
  else
  {
    tft.println("GOOD");
    
    tft.setRotation(3);
    tft.setCursor(170, 60);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    
    if(((abs(Apeaktopeak-Bpeaktopeak)*3.3/4095)/(0.015*20)*1000)>(arus*0.3))
    {
      tft.println("NOT Good"); 
    }
    else
    {
      tft.println("GOOD");
    }
  }

  tft.setRotation(3);
  tft.setCursor(170, 90);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  
  if(inductance==0 && inductance2==0)
  {
    tft.println("Not Tested");
  }
  else if(inductanceError>0.1)
  {
    tft.println("NOT Good"); 
  }
  else
  {
    tft.println("GOOD");
  }

  
  
  inputedKey=NO_KEY;

  while(inputedKey!='D')
  {
    inputedKey=bacaKeypadTanpaPrint();
  }

  
}
