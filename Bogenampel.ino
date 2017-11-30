#include <LiquidCrystal_I2C.h> 
#include <Wire.h>
#include <LCD.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define AMPEL 8
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, AMPEL, NEO_GRB + NEO_KHZ800);

int isstop = 0;
int isstopable = 0;
int sectowait = 10;

#define I2C_ADDR    0x3F
#define BACKLIGHT_PIN     3

LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

#define RelayABCD 9
#define RelayHorn 10
#define ButtonStart 13
#define ButtonStop 2

unsigned long time;
int rounds = 0;

void setup() {
  Serial.begin(9600);
  strip.begin();
  Serial.println("Initialize all pixels to 'off'");
  strip.show(); // Initialize all pixels to 'off'
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.println("Print Vereinsname on LCD");
  // Print a message to the LCD.
  lcd.setCursor(0, 0);
  lcd.print("Bogenschuetzen");
  lcd.setCursor(0, 1);
  lcd.print("Ennepe-Ruhr e.V.");

  Serial.println("Set OUTPUTPINS: AB/CD: " + RelayABCD );
  pinMode(RelayABCD,OUTPUT);

  Serial.println("Set OUTPUTPINS: HORN: " + RelayHorn);
  pinMode(RelayHorn,OUTPUT);
  
  Serial.println("Set StartButton:" + ButtonStart);
  pinMode(ButtonStart,INPUT_PULLUP);
  
  
  Serial.println("Set initial OutputIO -> LOW");
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
 
  if(sensorValue == LOW)
  {    
    lcd.clear();
    startpasse();
    
    switchABCD();

    startpasse();
    
    Serial.println("3 Horns get arrows");
    horn(3);
    lcd.clear();
    rounds++;
    writeLCD(0,"Runde Ende");
    writeLCD(1,String(rounds) + " Runde");
    Serial.println("Round end...");
    Serial.println(String(rounds) + "Passen geschossen"); 
  }  
}


void setgreen()
{
  Serial.println("Set Color 0,100,0");
  colorSet(strip.Color(0, 100, 0),10);
  Serial.println("Set ISSTOPABLE -> 1");
  isstopable=1;
  Serial.println("Countdown 90sec green. ");
  countdown(90, "Count Gruen:");
}

void setyellow()
{
  Serial.println("Set Color to Yellow: ");
  colorSet(strip.Color(100, 100, 0), 200);
  Serial.println("Countdown 30 sec yellow");
  countdown(30, "Count Gelb:");
}

void setred()
{
  Serial.println("Set ISSTOP -> 0");
  isstop = 0;
  Serial.println("Set ISSTOPABLE -> 0");
  isstopable=0;

  Serial.println("Set Color to red: ");
  colorSet(strip.Color(100, 0, 0),10);

  Serial.println("2 Horns next team to line");
}

void setprestart()
{
  colorSet(strip.Color(100, 0, 0),10);
  Serial.println("PRESTART 10sec");
    Serial.println("");
  countdown(sectowait, "Count pre Start:");
}

void startpasse()
{
  horn(2);
  setprestart();
 
  horn(1);
  setgreen();

  Serial.println("Check if stop is not set");
  if(isstop==0)
  {
    Serial.println("STOP IS NOT SET ");
    setyellow();
  } 
  setred();
}


void countdown(long sec, String  msg)
{
  long timeadd = (1000 * sec);
  time = millis() + timeadd;
  unsigned long starttime = millis();
    while(time >= millis() )
    {
        if(isstop==1)
      {
        return;
      }

      lcd.clear();
      
      Serial.println(msg);
      writeLCD(0,msg);
       
      writeLCD(1,String(((millis() - starttime) / 1000)));
      
      delay(100);
    }
}

void writeLCD(int line, String msg)
{
  lcd.setCursor(0, line);
  lcd.print(msg);
}

void stopMe()
{
  if(isstopable ==1)
  {
    isstop =1;
      Serial.println("STOP ME INTERRUPT");
  }
}
void switchABCD()
{
   digitalWrite(RelayABCD,  !digitalRead(RelayABCD));  
     Serial.println("SET AB/CD" + !digitalRead(RelayABCD));
}

void horn(int times)
{
  int runs = 0;
  while(runs <times)
  {     
    Serial.println("HORN");
    digitalWrite (RelayHorn, LOW);
    delay(1000);
    digitalWrite (RelayHorn, HIGH);
    runs++;
    delay(1000);
  }
}


void colorSet(uint32_t c, uint32_t wait){
 for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
      strip.show();
}


