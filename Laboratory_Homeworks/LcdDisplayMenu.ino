#include "LiquidCrystal.h"
#include <EEPROM.h>
#define EEPROM_AVAILABLE 0
byte customChar[] = {
  B00000,
  B01010,
  B11111,
  B11111,
  B11111,
  B01110,
  B01110,
  B00100
};

enum class Direction {
  leftX, leftY, rightX, rightY, none
};
typedef void (*runProg)();

void firstMenu();
void settings();
void highScore();
void selectGame();
void runGame();

runProg exec;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
const int pinJoyStickX = A1;
const int pinJoyStickY = A0;
const int pinJoyStickSw = 13;
const unsigned int minThreshold = 200;
const unsigned int maxThreshold = 800;
unsigned int value;
bool pressedX = false;
bool pressedY = false;
bool pressedSW = false;
int optionVal = 0, increase = 0, minOpVal = 0, maxOpVal = 2;
int lastOpVal;
bool printLocked = true;
bool stopEvent;
Direction cur_d;
String content;
String opt;
char userName[16], tempName[16];
bool down;
unsigned int lives = 3;
unsigned int level;
unsigned int sec, lastSec;
unsigned int gameTime;
unsigned int startingLevelValue = 1;
int maxScore = 0;
int score;
int nrOfPlayers;
int addr = 0;
byte check;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int getPinJx () {
  return analogRead(pinJoyStickX);
}
int getPinJy () {
  return analogRead(pinJoyStickY);
}
bool getPinJSw () {
  return digitalRead(pinJoyStickSw);
}

Direction isPressedX() {
  value = getPinJx();
  if (((value < minThreshold) || (value > maxThreshold)) && pressedX == false) {
    pressedX = true;
    if (value < minThreshold) return Direction::leftX;
    else if (value > maxThreshold) return Direction::rightX;

  }
  if (value >= minThreshold && value <= maxThreshold) {
    pressedX = false;
  }

  return Direction::none;
}

Direction isPressedY() {
  value = getPinJy();
  if (((value < minThreshold) || (value > maxThreshold)) && pressedY == false) {
    pressedY = true;
    if (value < minThreshold) return Direction::leftY;
    else if (value > maxThreshold) return Direction::rightY;

  }
  if (value >= minThreshold && value <= maxThreshold) {
    pressedY = false;
  }

  return Direction::none;
}



bool isButtonPressed() {
  value = !getPinJSw();
  if (value == 1 && pressedSW == false) {
    pressedSW = true;
    return true;
  }
  else if (value == 0) {
    pressedSW = false;
  }
  return false;
}

void writeIntEEPROM(int addr, int value) {
  byte *int_ptr = (byte*)&value;
  unsigned int index;
  for (index = 0; index < sizeof(value); index++)
    EEPROM.write(addr + index, *(int_ptr + index));
}

void readIntEEPROM(int addr, int &value) {
  unsigned int index;
  value = 0;
  for (index = 0; index < sizeof(value); index++)
    value += (int)((EEPROM.read(addr + index)) << index * sizeof(byte));
}

String gameList[5] = {
  "Mario Bros",
  "Tomb Raider",
  "Tetris",
  "Snake",
};
int t_addrs;

bool gameRunning = true;
unsigned int addDelay;
void runGame() {
  if (!printLocked || (sec - lastSec) > 100 && gameRunning) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Lives : ");
    for (unsigned int i = 0; i < lives; i++) {
      lcd.write((int)0);
    }
    lcd.setCursor(0, 1);
    lcd.print("Level:");
    lcd.print(level);
    lcd.print(" T:");
    lcd.print(sec / 1000);
    lcd.print("s");
    printLocked = true;
    addDelay = sec;
    lastSec = sec;
  }
  if (cur_d == Direction::rightX) level += increase;
  if (isButtonPressed() && (addDelay - sec) > 50) {
    lives--;
    addDelay = sec;
    if (lives == 0 && gameRunning) {
      lcd.clear();
      lcd.print("  Game Over");
      lcd.setCursor(0, 1);
      lcd.print("  Score:");
      score = level * 3;
      lcd.print(score);
      delay(2000);
      lcd.clear();
      lcd.print("Press button");
      lcd.setCursor(0, 10);
      lcd.print("to exit");
      gameRunning = false;
      if (score > maxScore)
        maxScore = score;
      if (check != EEPROM_AVAILABLE) {
        EEPROM.write(0, 0);
        check = EEPROM_AVAILABLE;
      }
      writeIntEEPROM(1, maxScore);
    } else {
      if (!gameRunning) {
        exec = firstMenu;
        minOpVal = 0;
        maxOpVal = 2;
        optionVal = 0;
        printLocked = false;
      }
    }
  }
  if (sec > 10000 && gameRunning) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Congratulations");
    delay(2000);
    lcd.clear();
    lcd.print("You finished");
    lcd.setCursor(0, 1);
    lcd.print("the game");
    delay(2000);
    lcd.clear();
    lcd.print(userName);
    lcd.setCursor(0, 1);
    lcd.print("Score:");
    score = level * 3;
    lcd.print(score);
    delay(2000);
    lcd.clear();
    lcd.print("Press button");
    lcd.setCursor(0, 10);
    lcd.print("to exit");
    gameRunning = false;
    if (score > maxScore)
      maxScore = score;
    if (check != EEPROM_AVAILABLE) {
      EEPROM.write(0, 0);
      check = EEPROM_AVAILABLE;
    }
    writeIntEEPROM(1, maxScore);
  }
  sec = floor((millis() - gameTime));
}

void selectGame() {
  cur_d = isPressedY();
  if (cur_d != Direction::none) {
    down = !down;
    printLocked = false;
  }
  if (!down) {
    minOpVal = 0;
    maxOpVal = 3;
  } else {
    minOpVal = 0;
    maxOpVal = 1;
  }
  if (!printLocked) {
    lcd.clear();
    lcd.setCursor(1, 0);
    if (!down) {
      opt = gameList[optionVal];
    }
    lcd.print(opt + ((!down) ? "<<" : ""));
    lcd.setCursor(0, 1);
    if (down) {
      if (optionVal == 1) lcd.print(">PLAY<  EXIT");
      else lcd.print(" PLAY  >EXIT<");
    }
    else lcd.print(" PLAY   EXIT ");
    printLocked = true;
  }
  if (isButtonPressed()) {
    if (down) {
      if (optionVal == 1) {
        exec = runGame;
        gameTime = millis();
        sec = 0;
        lastSec = 0;
        level = startingLevelValue;
        score = 0;
        gameRunning = true;
        lives = 3;
      }
      else {
        exec = firstMenu;
        minOpVal = 0;
        maxOpVal = 2;
        optionVal = 0;
      }
    }

    printLocked = false;
  }
}

void highScore() {
  if (!printLocked) {
    if (check == EEPROM_AVAILABLE) {
      lcd.clear();
      lcd.print("Highest Score : ");
      lcd.setCursor(0, 1);
      lcd.print(maxScore);
    }
    else {
      lcd.clear();
      lcd.print("No score ");
      lcd.setCursor(0, 1);
      lcd.print("available");
    }
    printLocked = true;
  }
  if (isButtonPressed()) {
    exec = firstMenu;
    minOpVal = 0;
    maxOpVal = 2;
    optionVal = 1;
    printLocked = false;
  }
}

void setLevel() {

  if(cur_d == Direction::rightX )startingLevelValue += 1;
  else if(cur_d == Direction::leftX) startingLevelValue += -1;
  if(startingLevelValue < 1) startingLevelValue = 1; 
  
  if (!printLocked) {
    lcd.clear();
    lcd.print("Set level : ");
    lcd.print(startingLevelValue);
    printLocked = true;
  }

  if (isButtonPressed()) {
    exec = firstMenu;
    minOpVal = 0;
    maxOpVal = 2;
    optionVal = 2;
    printLocked = false;
  }
}

unsigned int index_name =0;
void enterInput(){
    if (!printLocked) {
        lcd.clear();
        lcd.print("Enter name : ");
        lcd.setCursor(0, 1);
        if(index_name > 0) tempName[index_name-1] = NULL;
        index_name = 0;
        lcd.print(tempName);
        printLocked = true;
    }
    if(Serial.available() > 0){
        char c = Serial.read();
        tempName[index_name++] = c;  
        if(c == (char) 13) printLocked = false;
    }

   if (isButtonPressed()) {
    exec = firstMenu;
    if(tempName[0] != NULL) strcpy(userName, tempName);
    minOpVal = 0;
    maxOpVal = 2;
    optionVal = 2;
    printLocked = false;
  }
}

void settings() {
  switch (optionVal) {
    case 0: {
        content = String("> Enter name <");
        break;
      }
    case 1: {
        content = String("> Set Level <");
        break;
      }
  }

      if (!printLocked) {
        lcd.clear();
        unsigned int math = 8 - (content.length() / 2);
        lcd.setCursor(math, 0);
        lcd.print(content);
        printLocked = true;
      }
      if (isButtonPressed()) {
        if (optionVal == 1) {
          exec = setLevel;
        }
        else {
          exec = enterInput;
          tempName[0] = NULL;
          index_name = 0;
        }
        printLocked = false;
      }
  
}
runProg listMenu[3] = {
  selectGame, highScore, settings
};
void firstMenu() {
  switch (optionVal) {
    case 0: {
        content = String("> Select Game <");
        break;
      }
    case 1: {
        content = String("> HighScore <");
        break;
      }
    case 2: {
        content = String("> Settings <");
        break;
      }
  }
  if (!printLocked) {
    lcd.clear();
    unsigned int math = 8 - (content.length() / 2);
    lcd.setCursor(math, 0);
    lcd.print(content);
    printLocked = true;
  }
  if (isButtonPressed()) {
    exec = listMenu[optionVal];
    if (optionVal == 0) {
      minOpVal = 0;
      maxOpVal = 3;
      down = false;
    }
    else if (optionVal == 2) {
      minOpVal = 0;
      maxOpVal = 1;
      delay(200);
    }
    optionVal = 0;
    printLocked = false;
  }
}

void resetEEPROM() {
  EEPROM.write(0, 1);
  writeIntEEPROM(1, 0);
}

void setup() {
  pinMode(pinJoyStickSw, INPUT_PULLUP);
  strcpy(userName, "Default");
  check = EEPROM.read(0);
  if (check == EEPROM_AVAILABLE) readIntEEPROM(1, maxScore);
  lcd.begin(16, 2);
  Serial.begin(9600);
  exec = firstMenu;
  printLocked = false;
  lcd.createChar(0, customChar);
  Serial.println(optionVal);
  if (printLocked) lcd.clear();
  exec();
}

void loop() {
  cur_d = isPressedX();
  if (cur_d == Direction::leftX) {
    printLocked = false; increase = -1;
  }
  else if (cur_d == Direction::rightX) {
    printLocked = false; increase = 1;
  }
  else if (cur_d == Direction::none) increase = 0;
  lastOpVal = optionVal;
  optionVal += increase;

  if (optionVal < minOpVal) optionVal = maxOpVal;
  else if (optionVal > maxOpVal) optionVal = minOpVal;

  exec();
}
