/**
 * The following code is part of academic project"Smart Parking Mangement System".
 * It is writtern in arduino for simulating smart parking.
 * @author  Umang Shrestha<shresthu@uwindsor.ca>, Mohini Sarkar<sarkar9@uwindsor.ca>
 * @version 101, 2/06/22
 * @since   JDK1.0
 */

#include <LiquidCrystal.h>
#include <Servo.h>

/* LCD */
#define LCD_RS 13
#define LCD_EN 12
#define LCD_D4 11
#define LCD_D5 10
#define LCD_D6 9
#define LCD_D7 8

/* ServoMoter */
#define SERVO_PIN 7
#define GATE_OPEN 100
#define GATE_CLOSE 0

/* Led */
#define RED_LED_ENTER 6
#define GREEN_LED_ENTER 5
#define RED_LED_EXIT 15
#define GREEN_LED_EXIT 14

/* Button */
#define BUTTON_ENTER 3
#define BUTTON_EXIT 4

// delay
#define DELAY_SERVO 10
#define DELAY_LED 50
#define DELAY 1000

// state
#define BAUD_RATE 9600
#define MAX_ALLOCATED_SPACE 13

/* creating LCD object */
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

/* creating Servo Object */
Servo servo;

static int usedSlots = 0;

/* Predicates */
int isFull() { return usedSlots == MAX_ALLOCATED_SPACE; }
int isEmpty() { return usedSlots == 0; }
int incrementCar() { usedSlots++; }
int decrementCar() { usedSlots--; }
int availableSlots() { return MAX_ALLOCATED_SPACE - usedSlots; }

/* displays messsag in the given row */
void showMessage(const char *message, int row)
{
    lcd.setCursor(0, row);
    lcd.print(message);
}

/**
 * @brief default display for LCD in rest
 */
void defaultDisplay()
{
    // Note that all the strings have been hardcoded to  16 charectors size to prevent any overlap between updating messages
    showMessage((usedSlots < MAX_ALLOCATED_SPACE) ? "Welcome!!!     " : "Closed!!!       ", 0);
    showMessage("Available:      ", 1);
    lcd.setCursor(14, 1);
    lcd.print(availableSlots());
}

/**
 * @brief this is the main function containing work flow
 */
void ledOnOff(int onLed, int offLed)
{
    digitalWrite(offLed, 0); // first turn off the offLed
    delay(DELAY_LED);        // give it few millisecond to completely turn off
    digitalWrite(onLed, 1);  // turn on the onLed
}

/**
 * @brief show Student ID and Student Number in Load Screen
 */
void showTeamInformation()
{
    showMessage("Smart Parking   ", 0);
    showMessage("By Team 13      ", 1);
    delay(DELAY);
    showMessage("Mohini Sarkar   ", 0);
    showMessage("110086149       ", 1);
    delay(DELAY);
    showMessage("Umang Shrestha  ", 0);
    showMessage("110071431       ", 1);
    delay(DELAY);
}

/**
 * @brief Open the gate to given angle
 *  */
void setGate(int angle)
{
    servo.write(angle);
    delay(DELAY_SERVO);
}

/**
 * @brief performs all the operation required for car to enter parking
 */
void allowCarToEnter()
{
    incrementCar();
    setGate(GATE_OPEN);
    ledOnOff(GREEN_LED_ENTER, RED_LED_ENTER);
    showMessage("PLEASE ENTER!!! ", 0);
    delay(DELAY);
    setGate(GATE_CLOSE);
    ledOnOff(RED_LED_ENTER, GREEN_LED_ENTER);
}

/**
 * @brief performs all the operation required for car to exit parking
 */
void allowCarToExit()
{
    decrementCar();
    setGate(GATE_OPEN);
    ledOnOff(GREEN_LED_EXIT, RED_LED_EXIT);
    showMessage("PLEASE EXIT!!!  ", 0);
    delay(DELAY);
    setGate(GATE_CLOSE);
    ledOnOff(RED_LED_EXIT, GREEN_LED_EXIT);
}

/**
 * @brief initialize arduino pins
 */
void setup()
{
    Serial.begin(BAUD_RATE);        // initializing baud rate for serial communication
    pinMode(RED_LED_ENTER, OUTPUT); // initaling RGB led
    pinMode(GREEN_LED_ENTER, OUTPUT);
    pinMode(RED_LED_EXIT, OUTPUT);
    pinMode(GREEN_LED_EXIT, OUTPUT);
    pinMode(BUTTON_ENTER, INPUT); // initializing buttons
    pinMode(BUTTON_EXIT, INPUT);
    ledOnOff(RED_LED_ENTER, GREEN_LED_ENTER); // setting default color RED
    ledOnOff(RED_LED_EXIT, GREEN_LED_EXIT);
    lcd.begin(16, 2);        // configuring LCD rows and columns
    servo.attach(SERVO_PIN); // connecting servo
    setGate(GATE_CLOSE);     // closing gate
    showTeamInformation();
    defaultDisplay();
}

/**
 * @brief this is the main function containing work flow
 */
void loop()
{
    if (digitalRead(BUTTON_ENTER)) // enter button is pressed
    {
        !isFull() ? allowCarToEnter() : showMessage("PLZ Come Later: ", 0);
        delay(DELAY);
        defaultDisplay();
    }
    else if (digitalRead(BUTTON_EXIT)) // exit button is pressed
    {
        !isEmpty() ? allowCarToExit() : showMessage("No Car Inside!!!", 0);
        delay(DELAY);
        defaultDisplay();
    }
    delay(DELAY_LED);
}
