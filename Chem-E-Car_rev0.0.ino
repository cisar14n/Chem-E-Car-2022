//CHEM-E-CAR UANL 2020 rev2.1

//******Libraries*******//
//OLED
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Adalogger
#include <SPI.h>
#include <SD.h>

//RTC
#include <RTCZero.h>
RTCZero rtc;

//Neopixel
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

//******Definitions*******//
//OLED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Adalogger
const int chipSelect = 4;

//RTC
const byte seconds = 0;
const byte minutes = 0;
const byte hours = 0;
const byte day = 28;
const byte month = 11;
const byte year = 20;

//Neopixel
#define PIN        5 // Which pin on the Arduino is connected to the NeoPixels?
#define NUMPIXELS 11 // NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 30 // Time (in milliseconds) to pause between pixels
#define DELAYVAL2 85


//i/o
  //analog
  #define VBATPIN A7
  #define ldr A1
  #define pot A5
  //digital
  #define LEDR 13
  #define LEDG 8
  #define rel 6
  #define aux 3
  #define perf 10
  #define test 11
  #define estop 12


//Constants
const int delblink=100;
const int deloledi=1500;
const int revst=9;
//Variables
bool sderror=false;
bool olederror=false;
int setreached=0;
float measuredvbat;
bool testmode;
bool performance;
bool emergency;



//******Void Setup*******//
void setup() {
//Serial Config
Serial.begin(9600);
//Pin modes
pinMode(LEDR,OUTPUT);
pinMode(LEDG,OUTPUT);

pinMode(rel,OUTPUT);
pinMode(perf,INPUT);
pinMode(test,INPUT);
pinMode(estop,INPUT);
pinMode(aux,INPUT);
pinMode(pot,INPUT);
pinMode(ldr,INPUT);
digitalWrite(LEDR,LOW);
digitalWrite(LEDG,LOW);

//Serial config
Serial.begin(9600);


//******Initialize*******//
//RTC
rtc.begin();
 /*
 rtc.setHours(hours);
 rtc.setMinutes(minutes);
 rtc.setSeconds(seconds);
 rtc.setDay(day);
 rtc.setMonth(month);
 rtc.setYear(year);
 */
   
//Neopixel
pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
pixels.setBrightness(250);
pixels.clear();
pixels.show();
  
// OLED  
if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
  olederror=true;
  /*
  Serial.println(F("SSD1306 allocation failed"));
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    pixels.setPixelColor(i, pixels.Color(255, 0, 0));
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(DELAYVAL);// Pause before next pass through loop
  }
  while(1)
  {
    // 
  }
  */
}
display.clearDisplay();

//Afalogger
if (!SD.begin(chipSelect)) {
  display.setTextSize(1); 
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.print("SD...    ERROR");
  display.display();
  for(int piq=0; piq<revst; piq++){
  pixels.clear();
  pixels.show();
    for(int i=0; i<3; i++) { // For each pixel...
      pixels.setPixelColor(i, pixels.Color(255, 0, 0));
      pixels.show();   // Send the updated pixel colors to the hardware.
      delay(DELAYVAL2);// Pause before next pass through loop
    }
  }
  delay(deloledi);  
}
else
{
  display.setTextSize(1); 
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.print("SD...    OK");
  display.display();
  for(int piq=0; piq<revst; piq++){
  pixels.clear();
  pixels.show();
    for(int i=0; i<3; i++) { // For each pixel...
      pixels.setPixelColor(i, pixels.Color(0, 255, 0));
      pixels.show();   // Send the updated pixel colors to the hardware.
      delay(DELAYVAL2);// Pause before next pass through loop
    }
  }
  delay(deloledi);  
}

//Battery
//Read voltage of battery
measuredvbat = analogRead(VBATPIN);
measuredvbat *= 2;    // we divided by 2, so multiply back
measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
measuredvbat /= 1024; // convert to voltage
//Serial.print("VBat: " ); Serial.println(measuredvbat);
display.setTextSize(1);      // Normal 1:1 pixel scale
display.setTextColor(WHITE); // Draw white text
display.setCursor(0, 15);     
display.cp437(true);         // Use full 256 char 'Code Page 437' font
if(measuredvbat>=3.75 && measuredvbat<=4.30)
{
  display.print("LiPo...  OK");
  for(int piq=0; piq<revst; piq++){
    for(int i=4; i<7; i++) { // For each pixel...
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.show();   // Send the updated pixel colors to the hardware.
      //delay(DELAYVAL2);// Pause before next pass through loop
    }
  delay(DELAYVAL2);
    for(int i=4; i<7; i++) { // For each pixel...
      pixels.setPixelColor(i, pixels.Color(0, 255, 0));
      pixels.show();   // Send the updated pixel colors to the hardware.
      delay(DELAYVAL2);// Pause before next pass through loop
    }
  }
}
else if(measuredvbat>=3.65 && measuredvbat<=3.74)
{
  display.print("LiPo...  ALARM");
  for(int piq=0; piq<revst; piq++){
    for(int i=4; i<7; i++) { // For each pixel...
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.show();   // Send the updated pixel colors to the hardware.
      //delay(DELAYVAL2);// Pause before next pass through loop
    }
  delay(DELAYVAL2);
    for(int i=4; i<7; i++) { // For each pixel...
      pixels.setPixelColor(i, pixels.Color(200, 55, 0));
      pixels.show();   // Send the updated pixel colors to the hardware.
      delay(DELAYVAL2);// Pause before next pass through loop
    }
  }
}
else
{
  display.print("LiPo...  ERROR");
  for(int piq=0; piq<revst; piq++){
    for(int i=4; i<7; i++) { // For each pixel...
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.show();   // Send the updated pixel colors to the hardware.
      //delay(DELAYVAL2);// Pause before next pass through loop
    }
  delay(DELAYVAL2);
    for(int i=4; i<7; i++) { // For each pixel...
      pixels.setPixelColor(i, pixels.Color(255, 0, 0));
      pixels.show();   // Send the updated pixel colors to the hardware.
      delay(DELAYVAL2);// Pause before next pass through loop
    }
  }
}
display.setCursor(88, 15);     
display.cp437(true);// Use full 256 char 'Code Page 437' font
display.print("(");
display.setCursor(93, 15);     
display.cp437(true);// Use full 256 char 'Code Page 437' font
display.print(measuredvbat);
display.setCursor(116, 15);     
display.cp437(true);// Use full 256 char 'Code Page 437' font
display.print(")");
display.display();
delay(deloledi);

//Neopixel Spare Status
if(olederror==false)
{
  for(int piq=0; piq<revst; piq++){
      for(int i=8; i<11; i++) { // For each pixel...
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));
        pixels.show();   // Send the updated pixel colors to the hardware.
        //delay(DELAYVAL2);// Pause before next pass through loop
      }
    delay(DELAYVAL2);
    for(int i=8; i<11; i++) { // For each pixel...
      pixels.setPixelColor(i, pixels.Color(0, 255, 0));
      pixels.show();   // Send the updated pixel colors to the hardware.
      delay(DELAYVAL2);// Pause before next pass through loop
    }
  }
}
else
{
  for(int piq=0; piq<revst; piq++){
      for(int i=8; i<11; i++) { // For each pixel...
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));
        pixels.show();   // Send the updated pixel colors to the hardware.
        //delay(DELAYVAL2);// Pause before next pass through loop
      }
    delay(DELAYVAL2);
    for(int i=8; i<11; i++) { // For each pixel...
      pixels.setPixelColor(i, pixels.Color(255, 0, 0));
      pixels.show();   // Send the updated pixel colors to the hardware.
      delay(DELAYVAL2);// Pause before next pass through loop
    }
  }
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    pixels.setPixelColor(i, pixels.Color(255, 0, 0));
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(DELAYVAL);// Pause before next pass through loop
  }
  while(1)
  {
  }
}
delay(deloledi);

//Neopixel test
for(int piq=0; piq<8; piq++){
  pixels.clear(); // Set all pixel colors to 'off'
  pixels.show();
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    //----------------------------------R G B
    pixels.setPixelColor(i, pixels.Color(220, 40, 0));
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(DELAYVAL);// Pause before next pass through loop
    pixels.clear();
    pixels.show();
    }
  }
pixels.clear();
pixels.show();
//Test Neopixel routines
  //theaterChase(pixels.Color(127,   0,   0), 50); // Red, half brightness este
  //rainbow(30);             // Flowing rainbow cycle along the whole strip
//Final Neopixel sequence
//Red
delay(50);
for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
  pixels.setPixelColor(i, pixels.Color(255, 0, 0));
  pixels.show();   // Send the updated pixel colors to the hardware.
}
delay(950);
pixels.clear();
pixels.show();
delay(100);
//Yellow
for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
  pixels.setPixelColor(i, pixels.Color(220, 60, 0));
  pixels.show();   // Send the updated pixel colors to the hardware.
}
delay(950);
pixels.clear();
pixels.show();
delay(50);
//Green
for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
  pixels.setPixelColor(i, pixels.Color(0, 255, 0));
  pixels.show();   // Send the updated pixel colors to the hardware.
}
}




//******Void Loop*******//
void loop() {
//Reset variables
setreached=0;
digitalWrite(rel,LOW);
/*
//Neopixel to Green status
for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
  pixels.setPixelColor(i, pixels.Color(0, 255, 0));
  pixels.show();   // Send the updated pixel colors to the hardware.
}
*/
//Clear display and leds
display.clearDisplay();
//digitalWrite(LEDG,testmode); 
digitalWrite(LEDR,performance); 

//Read Performance start comamnd
performance = digitalRead(perf);
testmode = digitalRead(test);
emergency = digitalRead(estop);

//Read Pot value(Set-point)
float pots = analogRead(pot); 
pots *= 3.3;  // Multiply by 3.3V, our reference voltage
pots /= 1024; // convert to voltage  
delay(20);

//Read Battery voltage
measuredvbat = analogRead(VBATPIN);
measuredvbat *= 2;    // we divided by 2, so multiply back
measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
measuredvbat /= 1024; // convert to voltage
delay(20);

//Stand by screen    
display.setTextSize(2);      // Normal 1:1 pixel scale
display.setCursor(0, 0);     // Start at top-left corner
display.cp437(true);         // Use full 256 char 'Code Page 437' font
display.print("Stand by..");
display.setTextSize(1); 
display.setCursor(0, 25);     // Start at top-left corner
display.cp437(true);         // Use full 256 char 'Code Page 437' font
display.print("Set =");
display.setCursor(35, 25);     // Start at top-left corner
display.cp437(true);         // Use full 256 char 'Code Page 437' font
display.print(pots);
display.setCursor(88, 25);     
display.cp437(true);// Use full 256 char 'Code Page 437' font
display.print("(");
display.setCursor(93, 25);     
display.cp437(true);// Use full 256 char 'Code Page 437' font
display.print(measuredvbat);
display.setCursor(116, 25);     
display.cp437(true);// Use full 256 char 'Code Page 437' font
display.print(")");
display.display();
//
if(emergency==true){  
  //Read Test mode switch
  if (testmode == 1){
    digitalWrite(LEDG,testmode); 
    while(testmode == 1){
      testmode = digitalRead(test);
      emergency = digitalRead(estop);
      //Emergency?
      if(emergency==false){  
        break; 
      }
      //Read Battery voltage
      measuredvbat = analogRead(VBATPIN);
      measuredvbat *= 2;    // we divided by 2, so multiply back
      measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
      measuredvbat /= 1024; // convert to voltage
      delay(10);
      //Read Pot value(Set-point)
      float pots = analogRead(pot); 
      pots *= 3.3;  // Multiply by 3.3V, our reference voltage
      pots /= 1024; // convert to voltage  
      delay(10);
      //Test screen 
      //Clear OLED
      display.clearDisplay();      
      display.setTextSize(1);      // Normal 1:1 pixel scale
      display.setCursor(0, 0);     // Start at top-left corner
      display.cp437(true);         // Use full 256 char 'Code Page 437' font
      display.print("Test mode");
      display.setTextSize(1); 
      display.setCursor(0, 25);     // Start at top-left corner
      display.cp437(true);         // Use full 256 char 'Code Page 437' font
      display.print("Set =");
      display.setCursor(35, 25);     // Start at top-left corner
      display.cp437(true);         // Use full 256 char 'Code Page 437' font
      display.print(pots);
      display.setCursor(88, 25);     
      display.cp437(true);// Use full 256 char 'Code Page 437' font
      display.print("(");
      display.setCursor(93, 25);     
      display.cp437(true);// Use full 256 char 'Code Page 437' font
      display.print(measuredvbat);
      display.setCursor(116, 25);     
      display.cp437(true);// Use full 256 char 'Code Page 437' font
      display.print(")");
      display.display();
      //
      theaterChase(pixels.Color(200,   0,   105), 50);
      //rainbow(15);
    }
  }
  
  //Read performance Switch
  if (performance == 1){
    //Initialize clock
    rtc.setHours(hours);
    rtc.setMinutes(minutes);
    rtc.setSeconds(seconds);
    //Turn on Relay
    digitalWrite(rel,HIGH);
    //Turn on RED led to indicate rely status
    digitalWrite(LEDR,performance);
    //Neopixel to RED status
    for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
      pixels.setPixelColor(i, pixels.Color(255, 0, 0));
      pixels.show();   // Send the updated pixel colors to the hardware.
    } 
    //Loop until switch is turned top center position
    while(performance == 1){
      //Read status of selector switch
      performance = digitalRead(perf);
      emergency = digitalRead(estop);
      //Emergency?
      if(emergency==false){  
        break; 
      }   
      //Read LDR value
      float sensor2 = analogRead(ldr); 
      sensor2 *= 3.3;  // Multiply by 3.3V, our reference voltage
      sensor2 /= 1024; // convert to voltage
      delay(5);
      measuredvbat = analogRead(VBATPIN);
      measuredvbat *= 2;    // we divided by 2, so multiply back
      measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
      measuredvbat /= 1024; // convert to voltage
      delay(5);
      //Clear OLED
      display.clearDisplay();    
      display.setTextSize(1);      // Normal 1:1 pixel scale
      display.setCursor(0, 10);     // Start at top-left corner
      display.cp437(true);         // Use full 256 char 'Code Page 437' font
      display.print("LDR =");
      display.setCursor(35, 10);     // Start at top-left corner
      display.cp437(true);         // Use full 256 char 'Code Page 437' font
      display.print(sensor2);
      display.setCursor(0, 25);     // Start at top-left corner
      display.cp437(true);         // Use full 256 char 'Code Page 437' font
      //Print last value of Pot
      display.print("Set =");
      display.setCursor(35, 25);     // Start at top-left corner
      display.cp437(true);         // Use full 256 char 'Code Page 437' font
      display.print(pots);
      display.setCursor(85, 25);     // Start at top-left corner
      display.cp437(true);         // Use full 256 char 'Code Page 437' font
      display.print(setreached);
      
      //Print elapsed time
      display.setCursor(0, 0);     // Start at top-left corner
      display.cp437(true);         // Use full 256 char 'Code Page 437' font
      display.print(rtc.getMinutes());
      display.setCursor(8, 0);     // Start at top-left corner
      display.cp437(true);
      display.print(":");// Use full 256 char 'Code Page 437' font
      display.setCursor(14, 0);     // Start at top-left corner
      display.cp437(true);         // Use full 256 char 'Code Page 437' font
      display.print(rtc.getSeconds());
      
      display.setCursor(50, 0);    
      display.cp437(true);         
      display.print(performance);

      display.setCursor(88, 25);     
      display.cp437(true);// Use full 256 char 'Code Page 437' font
      display.print("(");
      display.setCursor(93, 25);     
      display.cp437(true);// Use full 256 char 'Code Page 437' font
      display.print(measuredvbat);
      display.setCursor(116, 25);     
      display.cp437(true);// Use full 256 char 'Code Page 437' font
      display.print(")");

      display.display();
            
      //Serial debug
      Serial.print(rtc.getHours());
      Serial.print(":");
      Serial.print(rtc.getMinutes());
      Serial.print(":");
      Serial.print(rtc.getSeconds());
      Serial.print("\t");
      //LDR
      Serial.print("Sensor2= ");
      Serial.print(sensor2);
      Serial.print("\t");
      //Status of Performance selector switch
      Serial.print("Status= ");
      Serial.println(performance);
      //Write to SD card
      if(sderror==false)
      {
        String dataString = "";
        dataString += String(rtc.getHours());
        dataString += ":";
        dataString += String(rtc.getMinutes());
        dataString += ":";
        dataString += String(rtc.getSeconds());
        dataString += "\t"; 
        dataString += String(sensor2);//LDR
        dataString += "\t"; 
        dataString += String(performance);
        dataString += "\t"; 
        dataString += String(pots);   
        dataString += "\t"; 
        dataString += String(setreached);
        File dataFile = SD.open("chem799.txt", FILE_WRITE);
        if (dataFile) {
          digitalWrite(LEDG,HIGH); 
          dataFile.println(dataString);
          dataFile.close();
          digitalWrite(LEDG,LOW);
        }
        else 
        {
          //
        }
      }
      if(sensor2>pots){
        digitalWrite(rel,LOW);
        setreached=1;
      }
      else
      {
        //
      }
      delay(50);
    }
  }
}
else
{
//Emergency status
performance = digitalRead(perf);
testmode = digitalRead(test);
emergency = digitalRead(estop);
//Clear LEDs, Neopixel and LCD
display.clearDisplay();
digitalWrite(LEDG,LOW); 
digitalWrite(LEDR,LOW);
display.setTextSize(1);      // Normal 1:1 pixel scale
display.setCursor(0, 0);     // Start at top-left corner
display.cp437(true);         // Use full 256 char 'Code Page 437' font
display.print("Emergency mode");
display.setTextSize(1);      // Normal 1:1 pixel scale
display.setCursor(0, 10);     // Start at top-left corner
display.cp437(true);         // Use full 256 char 'Code Page 437' font
display.print("SS to center position");
display.setCursor(0, 20);     // Start at top-left corner
display.cp437(true);         // Use full 256 char 'Code Page 437' font
display.print("Release Estop");
display.display();
pixels.clear();
pixels.show();
while(emergency == false || performance==true || testmode==true)
  {
  performance = digitalRead(perf);
  testmode = digitalRead(test);
  emergency = digitalRead(estop);
  theaterChaseE(pixels.Color(255,   0,   0), 50);
  theaterChaseE(pixels.Color(220,   40,   0), 40);
    if(emergency == true){
      for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
        pixels.setPixelColor(i, pixels.Color(225, 40, 0));
        pixels.show();   // Send the updated pixel colors to the hardware.
      }
    }
  }
  pixels.clear();
  pixels.show();
  delay(250);
}
//Neopixel to Green status
for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
  pixels.setPixelColor(i, pixels.Color(0, 255, 0));
  pixels.show();   // Send the updated pixel colors to the hardware.
}
//End of loop
}





//******Neopixel routines*******//
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<7; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      pixels.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<pixels.numPixels(); c += 3) {
        pixels.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      pixels.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
      testmode = digitalRead(test);
      emergency = digitalRead(estop);
      if(testmode==false || emergency==false)
      {
        break;
      }
    }
  if(testmode==false || emergency==false)
    {
      break;
    }
  }
}

void theaterChaseE(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      pixels.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<pixels.numPixels(); c += 3) {
        pixels.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      pixels.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
      emergency = digitalRead(estop);
      if(emergency==true)
      {
        break;
      }
    }
  if(emergency==true)
    {
      break;
    }
  }
}

void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<pixels.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (pixels.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / pixels.numPixels());
      // pixels.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through pixels.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue)));
    }
    pixels.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}
