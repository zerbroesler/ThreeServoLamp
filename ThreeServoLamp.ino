

#include <Servo.h>
#include <FastLED.h>

#define debug
#define NUM_LEDS 4 
#define DATA_PIN 7
CRGB leds[NUM_LEDS];

Servo myservo;


int pos = 0;    // variable to store the servo speed
int minpos = 90;
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
  #ifdef debug
    Serial.begin(57600);
    Serial.println("Setup");
  #endif
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo.write(78);
  LEDS.addLeds<WS2812B,DATA_PIN,GRB>(leds,NUM_LEDS);
  LEDS.setBrightness(55);    

  ledColor(CRGB::ForestGreen);
// down a little bit
  down();
  delay(1000);
  
  calibrate();

  down();
  delay(2000);
    
  pullToStall();
    
  down();
  delay(1000);
  stop();
  #ifdef debug
    Serial.println("Ready in starting position");
  #endif
}

void loop() {
  down();
//  ledcycle();
  ledColor(CRGB::Crimson);
  for(i=0;i<100;i++){
    delay(40);
  }
  up();
  for(i=0;i<100;i++){
    ledcycle();    
    delay(40);
  }

}

void up(){
  #ifdef debug
    Serial.println("Up");
  #endif
  myservo.write(minpos-15);
}
void down(){
  #ifdef debug
    Serial.println("down");
  #endif
  myservo.write(minpos+10);
}
void stop(){
  myservo.write(minpos);
}

void calibrate(){
  sweep(80,100);
  minpos1=minpos;
  sweep(minpos1-5,minpos1+5);
  minpos2=minpos;
  sweep(minpos1-3,minpos1+3);
  minpos3=minpos;
  minpos=round((0.0+minpos1+minpos2+minpos3)/3.0);
  #ifdef debug
    Serial.print("Minpos: ");
    Serial.println(minpos);
    Serial.print("Maxpower: ");
    Serial.println(maxpower);
    Serial.println("Servo calibrated");
    Serial.println("");
  #endif
}

void pullToStall(){
// Tries to detect the servo stall
// by measuring the voltage along a shunt resistor
// of 1 Ohm at an analog input
  up();
  #ifdef debug
    Serial.println("Pulling string");
  #endif
  delay(100); // Avoid peaks due to abrupt changes
  ledColor(CRGB::DarkMagenta);
  do{
    analog=readValue(sensorPin);
    #ifdef debug
      Serial.println(analog);
    #endif
    delay(20);
  }while(analog<maxpower+35);

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
  myservo.write(minpos);
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
  }  
}

void ledColor(CRGB color){
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(color);
    FastLED.show(); 
  }  
  
}


