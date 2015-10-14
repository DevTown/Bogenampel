
#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 8
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

int isstop = 0;
int isstopable = 0;

LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
#define RelayABCD 9
#define RelayHorn 10
#define ButtonStart 1
#define ButtonStop 2

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(0, 0);
  lcd.print("Bogenschuetzen");
  lcd.setCursor(0, 1);
  lcd.print("Ennepe-Ruhr e.V.");
  
  pinMode(RelayABCD,OUTPUT);
  pinMode(RelayHorn,OUTPUT);
  pinMode(ButtonStart,INPUT_PULLUP);
  digitalWrite (RelayABCD, LOW);
  digitalWrite (RelayHorn, LOW);

  digitalWrite (RelayABCD, HIGH);
  digitalWrite (RelayHorn, HIGH);
  digitalWrite (ButtonStop, HIGH);
  attachInterrupt(digitalPinToInterrupt(ButtonStop),stopMe,LOW);
}

void loop() {
  
  int sensorValue = digitalRead(ButtonStart);
  isstop = 0;
  isstopable=0;
  if(sensorValue == 0)
  {
    lcd.clear();
    horn();
    delay(1000);
    horn();
    colorSet(strip.Color(100, 0, 0),10);
    countdown(10, "Count pre Start:");
    //delay(10000);
    horn();
    colorSet(strip.Color(0, 100, 0),10);
    isstopable=1;
    countdown(120, "Count Gruen:");
    //delay(1200000);
    if(isstop==0)
    {
      colorSet(strip.Color(100, 100, 0), 200);
      countdown(30, "Count Gelb:");
      //delay(30000);
    
    }
    isstop = 0;
    isstopable=0;
    colorSet(strip.Color(100, 0, 0),10);
    horn();
    delay(1000);
    horn();
    delay(1000);
    switchABCD();

    countdown(10, "Count pre Start:");
    horn();
    colorSet(strip.Color(0, 100, 0),10);
    isstopable=1;
    countdown(120, "Count Gruen:");
    if(isstop==0)
    {
      colorSet(strip.Color(100, 100, 0), 200);
      countdown(30, "Count Gelb:");
     
    }
    isstop = 0;
    colorSet(strip.Color(100, 0, 0),10);
    horn();
    delay(1000);
    horn();
    delay(1000);
    horn();
    delay(1000);
    lcd.clear();
    writeLCD(0,"Runde Ende");
    
  }  
}

void countdown(int sec, char msg[16])
{
  for(int i =0; i<sec;i++)
  {
    if(isstop==1)
    {
      return;
      }
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(msg);
  lcd.setCursor(0, 1);
  lcd.print(sec - i);
    
  }
}

void writeLCD(int line, char msg[16])
{
  lcd.setCursor(0, line);
  lcd.print(msg);
}

void stopMe()
{
  if(isstopable ==1)
  {
    isstop =1;
  }
}
void switchABCD()
{
  
   digitalWrite(RelayABCD,  !digitalRead(RelayABCD));  
}

void horn()
{
  digitalWrite (RelayHorn, LOW);
  delay(1000);
  digitalWrite (RelayHorn, HIGH);
}


void colorSet(uint32_t c, uint32_t wait){
 for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);

  }
      strip.show();
    delay(wait);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint32_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
      strip.show();
    delay(wait);
  }

}

// Fill the dots one after the other with a color
void colorWipeInv(uint32_t c, uint32_t wait) {
  for(uint16_t i=strip.numPixels(); i>0; i--) {
    strip.setPixelColor(i, c);
      strip.show();
    delay(wait);
  }

}

