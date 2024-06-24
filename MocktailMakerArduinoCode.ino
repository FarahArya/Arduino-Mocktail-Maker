#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// Defining the pins for the relay modules
const int relayPin1 = 10;
const int relayPin2 = 11;
const int relayPin3 = 12; // New relay pin for the third pump

// Defining the rows and columns of the keypad
const byte ROWS = 4; // 4 rows
const byte COLS = 4; // 4 columns

// Defining the keymap
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Connecting keypad rows and columns to these Arduino pins
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

// Creating the Keypad object
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Creating the LCD object
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change 0x27 to your LCD's I2C address

unsigned long startTime1 = 0;
unsigned long startTime2 = 0;
unsigned long startTime3 = 0;
unsigned long duration1 = 0;
unsigned long duration2 = 0;
unsigned long duration3 = 0;
bool relay1On = false;
bool relay2On = false;
bool relay3On = false;

void setup() {
  Serial.begin(9600);

  // Relay pins as outputs
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  pinMode(relayPin3, OUTPUT); // Initialize the third relay pin
  
  // Set Relays as OFF in the beginning(active-low)
  digitalWrite(relayPin1, HIGH);
  digitalWrite(relayPin2, HIGH);
  digitalWrite(relayPin3, HIGH);

  // LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("System Init");
  delay(1000);
  lcd.clear();
  lcd.print("Welcome to Mochtail Maker :)");
  delay(2000);
  lcd.clear();
}

void loop() {
  char key = keypad.getKey(); // Get the key pressed

  unsigned long currentTime = millis(); // Get the current time

  if (key) { // Check if a key is pressed
    Serial.print("Key pressed: ");
    Serial.println(key); // Print the key to the serial monitor

    lcd.clear();
    lcd.print("Key: ");
    lcd.print(key);

    switch (key) {
      case '1':
        Serial.println("Pump 1 ON");
        lcd.setCursor(0, 1);
        lcd.print("Orange Juice");
        digitalWrite(relayPin1, LOW); // ON
        digitalWrite(relayPin2, HIGH); // OFF
        digitalWrite(relayPin3, HIGH); // OFF
        startTime1 = currentTime;
        duration1 = 3900; // Pump 1 on
        relay1On = true;
        relay2On = false;
        relay3On = false;
        break;

      case '2':
        Serial.println("Pump 2 ON");
        lcd.setCursor(0, 1);
        lcd.print("Pomegrenate/Punica Juice");
        digitalWrite(relayPin1, HIGH); // OFF
        digitalWrite(relayPin2, LOW); // ON
        digitalWrite(relayPin3, HIGH); // OFF
        startTime2 = currentTime;
        duration2 = 3900; // Pump 2 ON
        relay1On = false;
        relay2On = true;
        relay3On = false;
        break;

      case '3':
        Serial.println("Pump 3 ON");
        lcd.setCursor(0, 1);
        lcd.print("Cherry Juice");
        digitalWrite(relayPin1, HIGH); // OFF
        digitalWrite(relayPin2, HIGH); // OFF
        digitalWrite(relayPin3, LOW); // ON
        startTime3 = currentTime;
        duration3 = 3900; // Pump 3 ON
        relay1On = false;
        relay2On = false;
        relay3On = true;
        break;

      case '4':
        Serial.println("Pumps 1 and 3 ON");
        lcd.setCursor(0, 1);
        lcd.print("Orange-Cherry Juice");
        digitalWrite(relayPin1, LOW); // ON
        digitalWrite(relayPin2, HIGH); // OFF
        digitalWrite(relayPin3, LOW); // ON
        startTime1 = currentTime;
        startTime3 = currentTime;
        duration1 = 1950; // Pumps 1 and 3 ON
        duration3 = 1950;
        relay1On = true;
        relay2On = false;
        relay3On = true;
        break;

      case '5':
        Serial.println("Pumps 2 and 3 ON");
        lcd.setCursor(0, 1);
        lcd.print("Punica-Cherry Juice");
        digitalWrite(relayPin1, HIGH); // OFF
        digitalWrite(relayPin2, LOW); // ON
        digitalWrite(relayPin3, LOW); // ON
        startTime2 = currentTime;
        startTime3 = currentTime;
        duration2 = 1950; // Pumps 2 and 3 ON
        duration3 = 1950;
        relay1On = false;
        relay2On = true;
        relay3On = true;
        break;

      case '6':
        Serial.println("Pumps 1 and 2 ON");
        lcd.setCursor(0, 1);
        lcd.print("Orange-Punica Juice");
        digitalWrite(relayPin1, LOW); // ON
        digitalWrite(relayPin2, LOW); // ON
        digitalWrite(relayPin3, HIGH); // OFF
        startTime1 = currentTime;
        startTime2 = currentTime;
        duration1 = 1950; // Pumps 1 and 2 ON
        duration2 = 1950;
        relay1On = true;
        relay2On = true;
        relay3On = false;
        break;

      case '7':
        Serial.println("All Pumps ON");
        lcd.setCursor(0, 1);
        lcd.print("Summer Mix Juice");
        digitalWrite(relayPin1, LOW); // ON
        digitalWrite(relayPin2, LOW); // ON
        digitalWrite(relayPin3, LOW); // ON
        startTime1 = currentTime;
        startTime2 = currentTime;
        startTime3 = currentTime;
        duration1 = 1300; // All pumps ON
        duration2 = 1300;
        duration3 = 1300;
        relay1On = true;
        relay2On = true;
        relay3On = true;
        break;

      case 'D':
        Serial.println("All Pumps OFF");
        lcd.setCursor(0, 1);
        lcd.print("Stop The Machine");
        digitalWrite(relayPin1, HIGH); // OFF
        digitalWrite(relayPin2, HIGH); // OFF
        digitalWrite(relayPin3, HIGH); // OFF
        relay1On = false;
        relay2On = false;
        relay3On = false;
        break;
      
      default:
        Serial.println("Invalid Input");
        lcd.setCursor(0, 1);
        lcd.print("Enter a Valid Key!");

    }
  }

  // Check if it's time to turn off relay 1
  if (relay1On && (currentTime - startTime1 >= duration1)) {
    Serial.println("Turning off Pump 1");
    digitalWrite(relayPin1, HIGH); // OFF
    relay1On = false;
  }

  // Check if it's time to turn off relay 2
  if (relay2On && (currentTime - startTime2 >= duration2)) {
    Serial.println("Turning off Pump 2");
    digitalWrite(relayPin2, HIGH); // OFF
    relay2On = false;
  }

  // Check if it's time to turn off relay 3
  if (relay3On && (currentTime - startTime3 >= duration3)) {
    Serial.println("Turning off Pump 3");
    digitalWrite(relayPin3, HIGH); // OFF
    relay3On = false;
  }

  // Debugging state: Display relay states
  Serial.print("Relay 1: ");
  Serial.print(digitalRead(relayPin1) == LOW ? "ON" : "OFF");
  Serial.print(" | Relay 2: ");
  Serial.print(digitalRead(relayPin2) == LOW ? "ON" : "OFF");
  Serial.print(" | Relay 3: ");
  Serial.println(digitalRead(relayPin3) == LOW ? "ON" : "OFF");
  delay(1000); // Add a delay to reduce serial output spam
}
