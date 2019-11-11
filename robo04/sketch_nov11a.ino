#include "pitches.h"

//arduino led pin
const int ledPin = 13;

// for pushbutton
const int pushButtonPin = 2;

//active buzzer
const int activeBuzzerPin = 11;
int activeBuzzerTone = 500;

//passive buzzer reader
const int knockPin = A0;
bool isKnock(const int pin, const int threshold){
    int pinVal = analogRead(pin);
    return (pinVal > threshold) ? true : false;
}

//time elapsed functions
int t_count = 0;
void reset_time() {t_count = millis();}
bool wait(const int timeDelay){
     return ((millis() - t_count) > timeDelay) ? true : false; 
}

int counter = 0;
bool allowed;

void setup() {
    Serial.begin(9600);
    pinMode(pushButtonPin, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
    allowed = false;
    reset_time();
}



void loop() {
    Serial.println(counter);
    if(isKnock(knockPin, 50) && counter < 4){
        if(wait(1000)){
            counter++;
            reset_time();
        }  
    }
  
    if(counter >= 4) {
      if(wait(5000))
           allowed = true;
    }

    if(allowed){
        tone(activeBuzzerPin, activeBuzzerTone, 500);
        int readButton = digitalRead(pushButtonPin);
        if(readButton == 0){
          noTone(activeBuzzerPin);
          reset_time();
          counter = 0;
          allowed = false;
        }
    }
}
