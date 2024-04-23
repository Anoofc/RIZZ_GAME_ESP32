#define DEBUG 0
#define BAUDRATE 9600

#define SWITCH_1_PIN 2
#define SWITCH_2_PIN 3
#define SWITCH_3_PIN 4
#define SWITCH_4_PIN 8

#define RELAY_1_PIN 5
#define RELAY_2_PIN 6
#define RELAY_3_PIN 7
#define RELAY_4_PIN 9

#define PC_SERIAL Serial
#define DEBUG_SERIAL Serial

#define RELAY_ON  0
#define RELAY_OFF 1

#define SWITCH_1_COMMAND "A"
#define SWITCH_2_COMMAND "B"
#define SWITCH_3_COMMAND "C"
#define SWITCH_4_COMMAND "D"

#define STOP_COMMAND "Z"
#define START_COMMAND "S"


#define DEBOUNCE_TIME 250
#define RESET_TIME 20000
#include <Arduino.h>

static unsigned long lastPressTime_1 = 0;
static unsigned long lastPressTime_2 = 0;
static unsigned long lastPressTime_3 = 0;
static unsigned long lastPressTime_4 = 0;

unsigned long currentMillis = millis();
  

void pinSetup() {
  pinMode(SWITCH_1_PIN, INPUT_PULLUP);
  pinMode(SWITCH_2_PIN, INPUT_PULLUP);
  pinMode(SWITCH_3_PIN, INPUT_PULLUP);
  pinMode(SWITCH_4_PIN, INPUT_PULLUP);

  pinMode(RELAY_1_PIN, OUTPUT);
  pinMode(RELAY_2_PIN, OUTPUT);
  pinMode(RELAY_3_PIN, OUTPUT);
  pinMode(RELAY_4_PIN, OUTPUT);

  digitalWrite(RELAY_1_PIN, RELAY_ON);
  digitalWrite(RELAY_2_PIN, RELAY_ON);
  digitalWrite(RELAY_3_PIN, RELAY_ON);
  digitalWrite(RELAY_4_PIN, RELAY_ON);
}

void allOff(){
  digitalWrite(RELAY_1_PIN, RELAY_OFF);
  digitalWrite(RELAY_2_PIN, RELAY_OFF);
  digitalWrite(RELAY_3_PIN, RELAY_OFF);
  digitalWrite(RELAY_4_PIN, RELAY_OFF);
}

void allOn(){
  digitalWrite(RELAY_1_PIN, RELAY_ON);
  digitalWrite(RELAY_2_PIN, RELAY_ON);
  digitalWrite(RELAY_3_PIN, RELAY_ON);
  digitalWrite(RELAY_4_PIN, RELAY_ON);
}

void buttonCheck(){

  if (digitalRead(SWITCH_1_PIN) == LOW){
    if (currentMillis - lastPressTime_1 < DEBOUNCE_TIME) { return; }
      digitalWrite(RELAY_2_PIN, RELAY_OFF);
      digitalWrite(RELAY_3_PIN, RELAY_OFF);
      digitalWrite(RELAY_4_PIN, RELAY_OFF);
      PC_SERIAL.println(SWITCH_1_COMMAND);

      if (DEBUG) { DEBUG_SERIAL.println("SWITCH 1 PRESSED"); }

      lastPressTime_1 = currentMillis; // Update the last press time
  }

  if (digitalRead(SWITCH_2_PIN) == LOW){
    if (currentMillis - lastPressTime_2 < DEBOUNCE_TIME) { return; }
      digitalWrite(RELAY_1_PIN, RELAY_OFF);
      digitalWrite(RELAY_3_PIN, RELAY_OFF);
      digitalWrite(RELAY_4_PIN, RELAY_OFF);
      PC_SERIAL.println(SWITCH_2_COMMAND);

      if (DEBUG) { DEBUG_SERIAL.println("SWITCH 2 PRESSED"); }

      lastPressTime_2 = currentMillis; // Update the last press time
  }

  if (digitalRead(SWITCH_3_PIN) == LOW){
    if (currentMillis - lastPressTime_3 < DEBOUNCE_TIME) { return; }
      digitalWrite(RELAY_1_PIN, RELAY_OFF);
      digitalWrite(RELAY_2_PIN, RELAY_OFF);
      digitalWrite(RELAY_4_PIN, RELAY_OFF);
      PC_SERIAL.println(SWITCH_3_COMMAND);

      if (DEBUG) { DEBUG_SERIAL.println("SWITCH 3 PRESSED"); }

      lastPressTime_3 = currentMillis; // Update the last press time
  }

  if (digitalRead(SWITCH_4_PIN) == LOW){
    if (currentMillis - lastPressTime_4 < DEBOUNCE_TIME) { return; }
      digitalWrite(RELAY_1_PIN, RELAY_OFF);
      digitalWrite(RELAY_2_PIN, RELAY_OFF);
      digitalWrite(RELAY_3_PIN, RELAY_OFF);
      PC_SERIAL.println(SWITCH_4_COMMAND);

      if (DEBUG) { DEBUG_SERIAL.println("SWITCH 4 PRESSED"); }

      lastPressTime_4 = currentMillis; // Update the last press time
  }

}


void resetAfterDelay(){
  if (currentMillis - lastPressTime_1 == RESET_TIME) {
    allOn();
    if (DEBUG) { DEBUG_SERIAL.println("ALL RELAY ON"); }
  } 
  else if (currentMillis - lastPressTime_2 == RESET_TIME) {
    allOn();
    if (DEBUG) { DEBUG_SERIAL.println("ALL RELAY ON"); }
  }
  else if (currentMillis - lastPressTime_3 == RESET_TIME) {
    allOn();
    if (DEBUG) { DEBUG_SERIAL.println("ALL RELAY ON"); }
  }
  else if (currentMillis - lastPressTime_4 == RESET_TIME) {
    allOn();
    if (DEBUG) { DEBUG_SERIAL.println("ALL RELAY ON"); }
  }

}

// Process the data recieved from the serial

void process_data(String data){
  if (data == STOP_COMMAND){
    allOff();
    if (DEBUG) { DEBUG_SERIAL.println("ALL LIGHT OFF"); }
  }
  if (data == START_COMMAND){
    allOn();
    if (DEBUG) { DEBUG_SERIAL.println("ALL LIGHT ON"); }
  }
}

void readSerial(){
  if (PC_SERIAL.available()) {
    String recieved_data = PC_SERIAL.readString();
    recieved_data.trim();
    process_data(recieved_data);
  }

}

void setup() {
  if (PC_SERIAL != DEBUG_SERIAL) { DEBUG_SERIAL.begin(115200); }
  PC_SERIAL.begin(BAUDRATE);
  pinSetup();

}

void loop() {
  currentMillis = millis();
  
  readSerial();
  buttonCheck();
  // resetAfterDelay();
 
}
