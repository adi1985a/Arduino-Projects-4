# Relay Control with Button and LCD I2C (with Polish Characters)

## Project Description

In this project, we will create a system for controlling a relay using a button. The state of the relay will be displayed on a 16x2 LCD with an I2C interface. Additionally, we will display Polish characters such as `Ź`, `ł`, and `ą` on the LCD by defining custom characters in the code.

This project is useful for applications where you need to control devices via a button and monitor their state on a display.

## Required Components

- **Arduino** (e.g., Arduino Uno, Nano, or compatible)
- **16x2 LCD Display with I2C interface** (e.g., with address 0x27)
- **Button** (to control the relay state)
- **Relay** (5V relay module)
- **Pull-up Resistor** (if not using `INPUT_PULLUP` on the button pin)
- **Jumper wires**
- **`LiquidCrystal_I2C` library** (to be installed in the Arduino IDE)

## Circuit Diagram

| Component         | Arduino Pin |
|-------------------|-------------|
| **Button**        | D3 (with `INPUT_PULLUP`) |
| **Relay**         | D2          |
| **LCD SDA**       | A4 (Uno)    |
| **LCD SCL**       | A5 (Uno)    |
| **LCD Power**     | 5V, GND     |

### Pin Connections:

1. **Button**:
   - The button is connected to pin D3.
   - We use the internal pull-up resistor (`INPUT_PULLUP`), so no external resistor is needed.
   - When the button is pressed, the state of pin D3 will be `LOW`.

2. **Relay**:
   - The relay is connected to pin D2.
   - The relay state is controlled by toggling the state of pin D2 between `LOW` and `HIGH`.

3. **LCD Display**:
   - The LCD is connected to the SDA (A4) and SCL (A5) pins of the Arduino.
   - The LCD is powered by 5V and GND.

## Installation

1. **Install Libraries**:
   To use the LCD display, you need to install the `LiquidCrystal_I2C` library in Arduino IDE:

   - Open Arduino IDE.
   - Go to **Sketch > Include Library > Manage Libraries**.
   - Search for **LiquidCrystal_I2C** and install it.

2. **Connect Components**:
   - Connect the button, relay, and LCD display as described in the circuit diagram.

3. **Upload the Code**:
   - Open Arduino IDE and upload the code to your Arduino.

## System Operation

### Displaying the Relay State

Once the system is powered on, the initial state of the relay will be displayed on the LCD as "Off." Pressing the button will toggle the relay state.

Each press of the button changes the state of the relay:
- If the relay is off, it will turn on.
- If the relay is on, it will turn off.

The relay state will be displayed on the LCD, and the state change will also be visible in the serial monitor.

### Polish Characters

To display Polish characters (`Ź`, `ł`, `ą`) on the LCD, custom characters are defined in the code. This allows us to display Polish letters without issues with encoding.

### Button Handling

The button toggles the relay state:
- When the relay is off, pressing the button turns it on.
- When the relay is on, pressing the button turns it off.

Debouncing is implemented to eliminate noise from button presses, ensuring stable button operation.

## Source Code

Here is the full source code:

```cpp
#include <LiquidCrystal_I2C.h> // Include the library for LCD display with I2C interface

// Polish characters definitions
byte customCharZ[8] = {B00010,B00100,B11111,B00010,B00100,B01000,B11111,B00000}; // Custom character for 'Ź'
byte customCharL[8] = {B01100, B00100, B00110, B00100, B01100, B00100, B01110, B00000}; // Custom character for 'ł'
byte customCharA[8] = {B00000,B0000,B01110,B00001,B01111,B10001,B01111,B00001}; // Custom character for 'ą'

const int relayPin = 2; // Pin for relay control
const int buttonPin = 3; // Pin for button input
bool relayState = LOW;   // Initial relay state (off)
bool lastButtonState = HIGH;  // Initial button state (not pressed, using pull-up)
bool buttonPressed = false;   // Flag indicating if the button was pressed

LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize LCD with I2C address 0x27 and size 16x2

void setup() {
  pinMode(relayPin, OUTPUT); // Set the relay pin as output
  pinMode(buttonPin, INPUT_PULLUP); // Set the button pin as input with internal pull-up resistor
  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on the LCD backlight
  Serial.begin(9600); // Start serial communication at 9600 baud

  // Set the initial state of the relay
  digitalWrite(relayPin, relayState);

  // Create custom characters for LCD
  lcd.createChar(0, customCharL); // Create character for 'ł'
  lcd.createChar(1, customCharA); // Create character for 'ą'
  lcd.createChar(2, customCharZ); // Create character for 'Ź'

  // Display the initial state on the LCD and in the serial monitor
  updateDisplayAndConsole();
}

void loop() {
  bool buttonState = digitalRead(buttonPin); // Read the button state

  // Check if the button was pressed
  if (buttonState == LOW && lastButtonState == HIGH && !buttonPressed) {
    relayState = !relayState; // Toggle the relay state
    digitalWrite(relayPin, relayState); // Apply the new relay state
    updateDisplayAndConsole(); // Update the display and serial monitor
    buttonPressed = true;  // Set the button pressed flag
  } else if (buttonState == HIGH) {
    buttonPressed = false;  // Reset the button pressed flag when the button is released
  }

  lastButtonState = buttonState; // Update the last button state
  delay(100);  // Short delay for debouncing
}

void updateDisplayAndConsole() {
  lcd.clear(); // Clear the display
  lcd.setCursor(0, 0); // Set the cursor to the start of the first line
  if (relayState) {
    lcd.print("Relay "); // Display text on the LCD
    lcd.write(byte(2)); // Display 'Ź'
    lcd.print(" ON");
  } else {
    lcd.print("Relay ");
    lcd.write(byte(2)); // Display 'Ź'
    lcd.print(" OFF");
  }

  lcd.setCursor(0, 1); // Set the cursor to the start of the second line
  if (relayState) {
    lcd.print("Acti");
    lcd.write(byte(0)); // Display 'ł'
    lcd.write(byte(1)); // Display 'ą'
    lcd.print("vated");
    Serial.println("Relay Activated");
  } else {
    lcd.print("Deacti");
    lcd.write(byte(0)); // Display 'ł'
    lcd.write(byte(1)); // Display 'ą'
    lcd.print("vated");
    Serial.println("Relay Deactivated");
  }
}
