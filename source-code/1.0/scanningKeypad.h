#include <Keypad.h>

const byte numRows= 4;
const byte numCols= 4;

int xkeypad,ykeypad;

char keypressed;

int digitNow;

char keymap[numRows][numCols]= {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'} 
  };
byte rowPins[numRows] = {PB8, PB9, PB6, PB4}; //Rows 0 to 3

byte colPins[numCols]= {PC12, PB3, PB5, PB7}; //Columns 0 to 3

//byte rowPins[numRows] = {PC12, PB3, PB5, PB7}; //Rows 0 to 3
//
//byte colPins[numCols]= {PB8, PB9, PB6, PB4}; //Columns 0 to 3

//initializes an instance of the Keypad class

Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);


//If key is pressed, this key is stored in 'keypressed' variable //If key is not equal to 'NO_KEY', then this key is printed out //if count=17, then count is reset back to 0 (this means no key is pressed during the whole keypad scan process

char bacaKeypad() {
  
  keypressed = myKeypad.getKey();
  
  if (keypressed != NO_KEY && keypressed !='*' && keypressed !='#' && keypressed !='A' && keypressed !='B' && keypressed !='C' && keypressed !='D' && digitNow<=3)
  {
    digitNow++;

    tft.setRotation(3);
    tft.setCursor(160+xkeypad, ykeypad);
    tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
    tft.println(keypressed);
    xkeypad=xkeypad+13;

  }
  else if(keypressed =='*' && digitNow>0)
  {
    digitNow--;
    tft.setRotation(3);
    xkeypad=xkeypad-13;
    tft.setCursor(160+xkeypad, ykeypad);
    tft.fillRect(160+xkeypad, ykeypad, 13, 14, ILI9341_BLACK);
  }
  else if(keypressed == 'D')
  {
    digitNow=0;
  }
  delay(10);
  return keypressed;
  
    
}

char bacaKeypadTanpaPrint() {
  
  keypressed = myKeypad.getKey();
  delay(10);
  return keypressed;
}
