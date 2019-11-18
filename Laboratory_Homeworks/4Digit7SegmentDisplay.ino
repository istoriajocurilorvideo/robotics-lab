const int pinA = 2;
const int pinB = 3;
const int pinC = 4;
const int pinD = 5;
const int pinE = 6;
const int pinF = 7;
const int pinG = 8;
const int D1 = 9;
const int D2 = 10;
const int D3 = 11;
const int D4 = 12;

const int segSize = 8;
const int noOfDigits = 10;
const int nrDigitPos = 4;

int dpState = LOW;
int index = 0;
int state = HIGH;

int digitPos[nrDigitPos] = {
    D1, D2, D3, D4
};

int segments[segSize] = {
    pinA, pinB, pinC, pinD, pinE, pinF,  pinG 
};

byte digitalMatrix[noOfDigits][segSize] = {
// a  b  c  d  e  f  g
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

//joystick input
const int SWJpin = 2;
const int xJPin = 0;
const int yJPin = 1;
const int maxJOut = 1023;
const int minThreshold = maxJOut / 4;
const int maxThreshold = maxJOut * (3 / 4);
unsigned int xValue, yValue;

unsigned int getXVal() {
    return analogRead(xJPin);
}

unsigned int getYVal() {
    return analogRead(yJPin);
}

unsigned int getSwInput() { 
    return digitalRead(SWJpin);
}

void displayNumber(byte digit){
  for(int i=0; i < segSize; i++){
      digitalWrite(segments[i], digitalMatrix[digit][i]);
  }  
}

void selectDigitPos(int nr) {
    for(int index = 0; index < nrDigitPos; index ++){
         if(index == nr) digitalWrite(digitPos[index], HIGH);
         digitalWrite(digitPos[index], LOW);
    }
}

void setup() {
    pinMode(xJPin, INPUT);
    pinMode(yJPin, INPUT);
    pinMode(SWJpin, INPUT_PULLUP);

    for(int i=0; i < segSize; i++)
        pinMode(segments[i], OUTPUT);  
    for(int i=0; i < nrDigitPos; i++)
        pinMode(digitPos[i], OUTPUT);  

    Serial.begin(9600);
}

bool setInputMode = false;
bool SWpressed = false;
bool stateJx = true;
bool stateJy = true;
int value, digitSelect = 0, digitNrValue = 0;
int segmDigitValues[nrDigitPos] = { 0, 0, 0, 0 };
void loop() {
    value = getSwInput();
    if(value == 1 && SWpressed == false) {
        SWpressed = true;
        setInputMode = !setInputMode;
    }
    else if(value == 0){
        SWpressed = false;
    }

    if(setInputMode){
        //setting the value for selected digit
        digitNrValue = segmDigitValues[digitSelect];
        yValue = getYVal();
          if((yValue < minThreshold || yValue > maxThreshold) && stateJy){
              stateJy = false;
              if(yValue < minThreshold) {
                  digitNrValue--;
                  if(digitNrValue < 0) digitNrValue = 9;
              }
              else if(yValue > maxThreshold) {
                  digitNrValue++;
                  if(digitNrValue > 9) digitNrValue = 0;
              }
          }

          if (yValue >= minThreshold && yValue <= maxThreshold) {
              stateJx = true;
          }
        segmDigitValues[digitSelect] = digitNrValue;
    }
    else{
        // change digit
        xValue = getXVal();
        if((xValue < minThreshold || xValue > maxThreshold) && stateJx){
            stateJx = false;
            if(xValue < minThreshold) {
                digitSelect--;
                if(digitSelect < 0) digitSelect = 3;
            }
            else if(xValue > maxThreshold) {
                digitSelect++;
                if(digitSelect > 3) digitSelect = 0;
            }
        }

        if (xValue >= minThreshold && xValue <= maxThreshold) {
            stateJx = true;
        }
    }

    for(int index=0; index < nrDigitPos; index ++){
        selectDigitPos(index);
        displayNumber(segmDigitValues[index]);
        delay(1);
    }

    Serial.print("SW pin : ");
    Serial.print(SWJpin);
    Serial.print(" | Xvalue : ");
    Serial.print(getXVal());
    Serial.print(" | Yvalue : ");
    Serial.print(getYVal());
    Serial.print(" | DigitSelect : ");
    Serial.print(digitSelect);
    Serial.print(" | DigitNrValue : ");
    Serial.print(digitNrValue);
    Serial.println();
}
