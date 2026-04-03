#include <LiquidCrystal.h>

// LCD pin mapping: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(7, 6, 8, 9, 10, 11);

// LEDs
const int redLED = 13;
const int greenLED = 12;

// Buttons
const int estopButton = 2;
const int resetButton = 3;
const int startButton = 4;

// System state
bool rideRunning = false;
bool estopLatched = false;
bool attackDetected = false;

String command = "";

void showAttackAlert() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("UNAUTHORIZED");
  lcd.setCursor(0, 1);
  lcd.print("COMMAND");
  delay(1500);
  attackDetected = false;
}

void updateOutputs() {
  if (attackDetected) {
    showAttackAlert();
  }

  lcd.clear();

  if (estopLatched) {
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, HIGH);

    lcd.setCursor(0, 0);
    lcd.print("EMERGENCY STOP");
    Serial.println("STATE: EMERGENCY STOP");
  }
  else if (rideRunning) {
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);

    lcd.setCursor(0, 0);
    lcd.print("RIDE RUNNING");
    Serial.println("STATE: RIDE RUNNING");
  }
  else {
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, LOW);

    lcd.setCursor(0, 0);
    lcd.print("RIDE STOPPED");
    Serial.println("STATE: RIDE STOPPED");
  }
}

void handleCommand(String cmd) {
  cmd.trim();
  cmd.toUpperCase();

  if (cmd == "STATUS") {
    updateOutputs();
  }
  else if (cmd == "START") {
    if (!estopLatched) {
      rideRunning = true;
      Serial.println("COMMAND ACCEPTED: START");
    } else {
      Serial.println("COMMAND BLOCKED: FAULT ACTIVE");
      attackDetected = true;
    }
    updateOutputs();
  }
  else if (cmd == "STOP") {
    rideRunning = false;
    Serial.println("COMMAND ACCEPTED: STOP");
    updateOutputs();
  }
  else {
    Serial.println("UNKNOWN COMMAND");
  }
}

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);

  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  pinMode(estopButton, INPUT_PULLUP);
  pinMode(resetButton, INPUT_PULLUP);
  pinMode(startButton, INPUT_PULLUP);

  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  delay(1000);

  updateOutputs();

  Serial.println("Ride Controller Online");
  Serial.println("Available commands: START, STOP, STATUS");
}

void loop() {
  int estopState = digitalRead(estopButton);
  int resetState = digitalRead(resetButton);
  int startState = digitalRead(startButton);

  // E-STOP always wins
  if (estopState == LOW) {
    rideRunning = false;
    estopLatched = true;
    updateOutputs();
    delay(200);
  }

  // Reset clears fault only
  if (resetState == LOW && estopLatched) {
    estopLatched = false;
    rideRunning = false;
    updateOutputs();
    delay(200);
  }

  // Start only works if no fault is active
  if (startState == LOW && !estopLatched) {
    rideRunning = true;
    updateOutputs();
    delay(200);
  }

  // Serial command handling
  if (Serial.available() > 0) {
    command = Serial.readStringUntil('\n');
    handleCommand(command);
  }
}