#include <LiquidCrystal.h>
#include <Servo.h>

/* LCD */
#define RS 13
#define EN 12
#define D4 11
#define D5 10
#define D6 9
#define D7 8
/* creating LCD object */
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

/* ServoMoter */
#define SERVO_PIN 7
#define GATE_OPEN 90
#define GATE_CLOSE 0
/* Creating Servo Object */
Servo servo;

// ENTER_LED
#define RED_LED_ENTER 6
#define GREEN_LED_ENTER 5

//  BUTTON
#define BUTTON_ENTER 3
#define BUTTON_EXIT 4

// EXIT_LED
#define RED_LED_EXIT 14
#define GREEN_LED_EXIT 15

// ENUM
#define ON 255
#define OFF 0

//color
#define RED 1
#define GREEN 0

// delay
#define DELAY_LED 50
#define DELAY 50

// state
#define MAX_ALLOCATED_SPACE 13
int UsedSlots = 0;

void exitLED(int isRED) {
  if (isRED) {
    analogWrite(GREEN_LED_EXIT, OFF);
    delay(DELAY_LED);
    analogWrite(RED_LED_EXIT, ON);
  } else {
    analogWrite(RED_LED_EXIT, OFF);
    delay(DELAY_LED);
    analogWrite(GREEN_LED_EXIT, ON);
  }
}

void enterLED(int isRED) {
  if (isRED) {
    analogWrite(GREEN_LED_ENTER, OFF);
    delay(DELAY_LED);
    analogWrite(RED_LED_ENTER, ON);
  } else {
    analogWrite(RED_LED_ENTER, OFF);
    delay(DELAY_LED);
    analogWrite(GREEN_LED_ENTER, ON);
  }
}

int IsPushed(int button) {
  return digitalRead(button);
}

void initialPrint() {
  lcd.print("Smart Parking");
  lcd.setCursor(6, 1);
  lcd.print("By Team 13");

  delay(DELAY);

  lcd.setCursor(0, 0);
  lcd.print("Mohini Sarkar       ");
  lcd.setCursor(0, 1);
  lcd.print("110086149           ");

  delay(DELAY);
  lcd.setCursor(0, 0);
  lcd.print("Umang Shrestha      ");
  lcd.setCursor(0, 1);
  lcd.print("110071431           ");
  delay(DELAY);
}

void setGate(int pos) {
  Serial.begin(9600);
  servo.write(pos);
  delay(10);
  servo.write(pos);
}

void intermediatePrint() {
  lcd.setCursor(0, 1);
  lcd.print("Available:          ");
  lcd.setCursor(14, 1);
  lcd.print(MAX_ALLOCATED_SPACE - UsedSlots);
}

void setup() {
  // ENTER LED
  pinMode(RED_LED_ENTER, OUTPUT);
  pinMode(GREEN_LED_ENTER, OUTPUT);
  //  BUTTON
  pinMode(BUTTON_ENTER, INPUT);
  pinMode(BUTTON_EXIT, INPUT);
  // EXIT LED
  pinMode(RED_LED_EXIT, OUTPUT);
  pinMode(GREEN_LED_EXIT, OUTPUT);
  // setting default color RED
  enterLED(RED);
  exitLED(RED);
  // LCD
  lcd.begin(16, 2);
  // SERVO
  servo.attach(SERVO_PIN);
  setGate(GATE_CLOSE);
  initialPrint();
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Welcome!!!          ");
  intermediatePrint();

  if (IsPushed(BUTTON_ENTER)) {
    if (UsedSlots < MAX_ALLOCATED_SPACE) {
      UsedSlots++;
      setGate(GATE_OPEN);
      enterLED(GREEN);
      lcd.setCursor(0, 0);
      lcd.print("PLEASE ENTER!!!     ");
      intermediatePrint();
      delay(DELAY);
      setGate(GATE_CLOSE);
      enterLED(RED);
    } else {
      lcd.setCursor(0, 0);
      lcd.print("PLZ Come Later:     ");
      intermediatePrint();
    }
    delay(DELAY);

  } else if (IsPushed(BUTTON_EXIT)) {
    if (UsedSlots > 0) {
      UsedSlots--;
      setGate(GATE_OPEN);
      exitLED(GREEN);
      lcd.setCursor(0, 0);
      lcd.print("PLEASE EXIT!!!       ");
      intermediatePrint();
      delay(DELAY);
      setGate(GATE_CLOSE);
      exitLED(RED);
    } else {
      lcd.setCursor(0, 0);
      lcd.print("No Car Inside!!!    ");
      intermediatePrint();
    }
    delay(DELAY);
  }
  delay(DELAY_LED);
}
