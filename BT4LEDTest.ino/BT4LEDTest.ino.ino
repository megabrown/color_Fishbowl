    /*
Connect Bluetooth RX,TX to Arduino RX,TX
Requires NeoPixel Library
https://github.com/adafruit/Adafruit_NeoPixel
If you want to view via Serial Monitor and have your Arduino connect to BT module 
simultaneously use Software Serial to open a second port.
#include <SoftwareSerial.h>
SoftwareSerial mySerial(3, 4); // RX, TX
*/
#include <Servo.h> 
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN 6
#define NUM_LEDS 32
#define BRIGHTNESS 50
Servo myservo;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards
int pos = 0;    // variable to store the servo position 
int red,green,blue;
int point=0;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show(); // Initialize all pixels to 'off'
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
  // initialize serial:
  Serial.begin(9600);
}

void loop() {
  // if there's any serial available, read it:
  int sig=0;
  char input=NULL;
  while (sig = Serial.available()) {
    if(sig!=0){
      input = Serial.read();
      point=1;
    }
        switch(input){
          case 'F': colorSet(strip.Color(0,0,0),5);
                  break;
          case 'O': colorSet(strip.Color(255,255,255),5);
                  break;
          case 'W':  
          while(1){
          colorWipe(strip.Color(255, 0, 0), 20);
          if(Serial.available()>0 && point==1)
             break;
          colorWipe(strip.Color(255, 100, 0), 20);
          if(Serial.available()>0 && point==1)
             break;
          colorWipe(strip.Color(255, 200, 0), 20);
          if(Serial.available()>0 && point==1)
             break;
          colorWipe(strip.Color(0, 255, 0), 20);
          if(Serial.available()>0 && point==1)
             break;
          colorWipe(strip.Color(0, 255, 100), 20);
          if(Serial.available()>0 && point==1)
             break;
          colorWipe(strip.Color(50, 255, 150), 20);
          if(Serial.available()>0 && point==1)
             break;
          colorWipe(strip.Color(150, 0, 255), 20);
          if(Serial.available()>0 && point==1)
             break;
          }
                  break;
          case 'R': 
          while(1){
          rainbow(50);
          if(Serial.available()>0 && point==1)
             break;
          }
                    break;
          case 'D': feedFish();
                  break;
          case 'H': 
          while(1){
          theaterChase(strip.Color(255, 0, 0), 50);
          theaterChase(strip.Color(255, 100, 0), 50);
          theaterChase(strip.Color(255, 200, 0), 50);
          theaterChase(strip.Color(0, 255, 0), 50);
          theaterChase(strip.Color(0, 255, 100), 50);
          theaterChase(strip.Color(50, 255, 150), 50);
          theaterChase(strip.Color(150, 0, 255), 50);
          if(Serial.available()>0 && point==1)
             break;
          }       
                  break;
          case 'A': 
          while(1){
          theaterChaseRainbow(50);
          if(Serial.available()>0 && point==1)
             break;
          }       
                  break;
          default: 
        // look for the next valid integer in the incoming serial stream:
         int red = Serial.parseInt(); 
        // do it again:
         int green = Serial.parseInt(); 
        // do it again:
         int blue = Serial.parseInt(); 
    
        // look for the newline. That's the end of your
        // sentence:
        if (Serial.read() == '\n') {
          // sends confirmation
//          Serial.println("received");
          // constrain the values to 0 - 255
          red = constrain(red, 0, 255);
          green = constrain(green, 0, 255);
          blue = constrain(blue, 0, 255);
          // fill strip
         colorSet(strip.Color(red, green, blue), 0);
        }       
             break;
        }
        point=0;
   }
}


void feedFish(){
    for(pos = 0; pos <= 180; pos +=5) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 180; pos>=0; pos-=5)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    if(Serial.available()>0 && point==1)
             break;
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
    if(Serial.available()>0 && point==1)
             break;
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
    if(Serial.available()>0 && point==1)
             break;
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// Fill strip with a color
void colorSet(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
  }
   strip.show();
   delay(wait);
}
