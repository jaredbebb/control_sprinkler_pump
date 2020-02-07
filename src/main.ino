#include <Arduino.h>
#include <Adafruit_TFTLCD.h> 
#include <Adafruit_GFX.h>    
#include <TouchScreen.h>
#include <MCUFRIEND_kbv.h>


#define LCD_CS A3 
#define LCD_CD A2 
#define LCD_WR A1 
#define LCD_RD A0 
#define LCD_RESET A4 
#define TS_MINX 143
#define TS_MINY 126
#define TS_MAXX 947
#define TS_MAXY 902
#define YP A2  
#define XM A3  
#define YM 8  
#define XP 9  
#define BLACK   0x0000
#define RED     0xF800
#define WHITE   0xFFFF

/******************* UI details */
#define BUTTON_X 40
#define BUTTON_Y 100
#define BUTTON_W 60
#define BUTTON_H 30
#define BUTTON_SPACING_X 20
#define BUTTON_SPACING_Y 20
#define BUTTON_TEXTSIZE 2

#define TEXT_X 10
#define TEXT_Y 10
#define TEXT_W 220
#define TEXT_H 50
#define TEXT_TSIZE 3
#define TEXT_TCOLOR WHITE

// We have a status line for like, is FONA working
#define STATUS_X 10
#define STATUS_Y 65

// the data (phone #) we store in the textfield
#define TEXT_LEN 12
char textfield[TEXT_LEN+1] = "";
uint8_t textfield_i=0;

const int PUMPCYLCEDAYS = 5;
int nextdayflag = false;
int daynum = 0;
int freqDur[2][PUMPCYLCEDAYS] = {0,0,0,0,0,0,0,0,0,0};


MCUFRIEND_kbv tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
boolean buttonEnabled = true;

void setup() {

  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));
  uint16_t identifier = tft.readID();

  if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Adafruit 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    return;
  }
  
  tft.reset(); 
  tft.begin(identifier);
  tft.setRotation(1);
  home_screen();
}

void loop() {
  TSPoint p = ts.getPoint();  
  if (p.z > ts.pressureThreshhold) {
   p.x = map(p.x, TS_MAXX, TS_MINX, 0, 320);
   p.y = map(p.y, TS_MAXY, TS_MINY, 0, 240);     
    if (p.x > 60 && p.x < 260 && p.y > 180 && p.y < 220 && buttonEnabled) {
    buttonEnabled = false;
    button_press_me_activated(p);
    delay(5000);
    buttonEnabled = true;
    
    pump_screen();
    tft.setRotation(2);
    
    delay(5000);
    home_screen();
    }
  }
}

void button_press_me_activated(TSPoint p){
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      tft.fillScreen(BLACK);
      tft.drawRect(0,0,319,240,WHITE);
      tft.setCursor(20,90);
      tft.setTextColor(WHITE);
      tft.setTextSize(2);
      tft.print("Your display is working!"); // Text after the button press
}

void home_screen(){
    tft.fillScreen(BLACK); // Background screen
    tft.drawRect(0,0,319,240,WHITE);
    tft.setCursor(40,80);
    tft.setTextColor(WHITE); // Text color
    tft.setTextSize(2);
    tft.print(" 2.8 inch TFT Shield"); // Text above the button
    tft.fillRect(60,180, 200, 40, RED);
    tft.drawRect(60,180,200,40,WHITE);
    tft.setCursor(80,188);
    tft.setTextColor(WHITE);
    tft.setTextSize(3);
    tft.print("Press me!"); // Button Text
}

void pump_screen(){
  tft.setRotation(2);
  tft.fillScreen(BLACK);
  Adafruit_GFX_Button buttons[15];
  char buttonlabels[15][5] = {"Next", "Clr", "End", "1", "2", "3", "4", "5", "6", "7", "8", "9", "*", "0", "#" };
  uint16_t buttoncolors[15] = {RED, RED, RED, 
                               RED, RED, RED, 
                               RED, RED, RED, 
                               RED, RED, RED, 
                               RED, RED, RED};
  // create buttons
  for (uint8_t row=0; row<5; row++) {
    for (uint8_t col=0; col<3; col++) {
      buttons[col + row*3].initButton(&tft, BUTTON_X+col*(BUTTON_W+BUTTON_SPACING_X), 
                 BUTTON_Y+row*(BUTTON_H+BUTTON_SPACING_Y),    // x, y, w, h, outline, fill, text
                  BUTTON_W, BUTTON_H, WHITE, buttoncolors[col+row*3], WHITE,
                  buttonlabels[col + row*3], BUTTON_TEXTSIZE); 
      buttons[col + row*3].drawButton();
    }
  }
  // create 'text field'
  tft.drawRect(TEXT_X, TEXT_Y, TEXT_W, TEXT_H, WHITE);

  // default status message'
  status(F("Default status"));
  readButtonInput(buttons, buttonlabels);
}

// or string
void status(String msg) {
  tft.fillRect(STATUS_X, STATUS_Y, 240, 8, BLACK);
  tft.setCursor(STATUS_X, STATUS_Y);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.print(msg);
}

void readButtonInput(Adafruit_GFX_Button buttons[], char buttonlabels[15][5]){
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);


  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if (p.z > ts.pressureThreshhold){
    // scale from 0->1023 to tft.width
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = (tft.height()-map(p.y, TS_MINY, TS_MAXY, tft.height(), 0));
   }
   
  // go thru all the buttons, checking if they were pressed
  for (uint8_t b=0; b<15; b++) {
    if (buttons[b].contains(p.x, p.y)) {
      buttons[b].press(true);  // tell the button it is pressed
    } else {
      buttons[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // now we can ask the buttons if their state has changed
  for (uint8_t b=0; b<15; b++) {
    if (buttons[b].justReleased()) {
      // Serial.print("Released: "); Serial.println(b);
      buttons[b].drawButton();  // draw normal
    }
    
    if (buttons[b].justPressed()) {
        buttons[b].drawButton(true);  // draw invert!
        
        // if a numberpad button, append the relevant # to the textfield
        if (b >= 3) {
          if (textfield_i < TEXT_LEN) {
            textfield[textfield_i] = buttonlabels[b][0];
            textfield_i++;
      textfield[textfield_i] = 0; // zero terminate
            
          }
        }

        // clr button! delete char
        if (b == 1) {
          
          textfield[textfield_i] = 0;
          if (textfield > 0) {
            textfield_i--;
            textfield[textfield_i] = ' ';
          }
        }

        // update the current text field
        Serial.println(textfield);
        tft.setCursor(TEXT_X + 2, TEXT_Y+10);
        tft.setTextColor(TEXT_TCOLOR, BLACK);
        tft.setTextSize(TEXT_TSIZE);
        tft.print(textfield);

        // its always OK to just hang up
        if (b == 2) {
          status(F("Hanging up"));
          //fona.hangUp();
        }
        // we dont really check that the text field makes sense
        // just try to next screen
        if (b == 0) {

          Serial.println(textfield);
          nextdayflag = true;
          daynum = daynum + 1;
          status("Daynum:"+(String) daynum);
          
        }
        
      delay(100); // UI debouncing
    }
  }
}