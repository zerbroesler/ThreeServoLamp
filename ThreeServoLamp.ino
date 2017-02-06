// Tries to detect the servo stall
// by measuring the voltage along a shunt resistor
// of 1 Ohm

#include <Servo.h>
#include <FastLED.h>

#define NUM_LEDS 4 
#define DATA_PIN 7
CRGB leds[NUM_LEDS];

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo speed
int minpos = 0;
int minpos1 = 0;
int minpos2 = 0;
int minpos3 = 0;
int sensorPin = A1;
int i;
int analog;
int anzahl = 10;
int minimum = 0;
int maxpower = 0;
uint8_t hue = 0;


void setup() {
  Serial.begin(57600);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo.write(78);
  LEDS.addLeds<WS2812B,DATA_PIN,RGB>(leds,NUM_LEDS);
  LEDS.setBrightness(55);    
}

void loop() {

  minimum = 1024;
  maxpower = 0;

// down a little bit
  myservo.write(110);
  delay(1000);

  
  sweep(80,100);
  minpos1=minpos;
  sweep(minpos1-5,minpos1+5);
  minpos2=minpos;
  sweep(minpos1-3,minpos1+3);
  minpos3=minpos;
  Serial.print("Minpos: ");
  Serial.println((0.0+minpos1+minpos2+minpos3)/3.0);
  Serial.print("Maxpower: ");
  Serial.println(maxpower);
  Serial.println("Servo calibrated");
  Serial.println("");
  myservo.write(minpos);
  delay(1000);

  do{
    
  Serial.println("Pulling string");
  myservo.write(minpos-10);
  do{
    analog=readValue(sensorPin);
    ledcycle();
    delay(30);
  }while(analog<maxpower+40);
  myservo.write(minpos+10);
  delay(500);
  myservo.write(minpos);
  Serial.println("Finished pulling");
  delay(7000);
  }while(true);
  

}
void sweep(int from, int to){
  myservo.write(from);
  delay(100); // Avoid peaks due to abrupt changes
  for (pos = from; pos < to; pos++ ) {
    myservo.write(pos);

    analog=readValue(sensorPin);
    if(analog>maxpower){
      maxpower=analog;
    }
    if(analog<minimum){
      minimum=analog;
      minpos = pos;
    }
  }
  
}

int readValue(int readPin){
    analog = 0;
    for(i = 0;i<anzahl;i++){
      analog += analogRead(readPin);
      delay(7);
    }
    analog /= anzahl;
    return analog;
}

void ledcycle(){
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show(); 
//    fadeall();
  }  
}

