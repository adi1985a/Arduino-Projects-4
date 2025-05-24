# ⚡ Relay Control with Button and LCD I2C (with Polish Characters)

![Arduino](https://img.shields.io/badge/Platform-Arduino-blue.svg)
![Language](https://img.shields.io/badge/Language-C%2B%2B-brightgreen.svg)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)
![Status](https://img.shields.io/badge/Status-Completed-success.svg)

---

## 📋 Project Description  
This project allows controlling a relay with a button, while the relay state is displayed on a 16x2 LCD with an I2C interface. Polish characters (Ź, ł, ą) are shown using custom characters defined in the code. This project is useful for manual device control and monitoring the device state on the display.

---

## ⚙️ Required Components
- Arduino (Uno, Nano or compatible)
- 16x2 LCD Display with I2C interface (e.g., 0x27)
- Button
- 5V Relay Module
- Pull-up resistor (if not using `INPUT_PULLUP`)
- Jumper wires
- [`LiquidCrystal_I2C`](https://github.com/johnrickman/LiquidCrystal_I2C) library (install via Arduino Library Manager)

---

## 🛠️ Circuit Diagram

| Component    | Arduino Pin        |
|--------------|--------------------|
| Button       | D3 (`INPUT_PULLUP`)|
| Relay        | D2                 |
| LCD SDA      | A4 (Uno)           |
| LCD SCL      | A5 (Uno)           |
| LCD Power    | 5V and GND         |

---

## ⚡ System Operation

- The **LCD** shows relay state: `Ź ON` or `Ź OFF`
- The **button** toggles the relay state
- Debouncing implemented
- Polish characters displayed using custom LCD characters

---

## 💻 Source Code

```cpp
#include <LiquidCrystal_I2C.h>

// Polish characters definitions (Ź, ł, ą)
byte customCharZ[8] = {B00010,B00100,B11111,B00010,B00100,B01000,B11111,B00000}; // 'Ź'
byte customCharL[8] = {B01100, B00100, B00110, B00100, B01100, B00100, B01110, 0}; // 'ł'
byte customCharA[8] = {B00000,B00000,B01110,B00001,B01111,B10001,B01111,B00001}; // 'ą'

const int relayPin = 2;
const int buttonPin = 3;
bool relayState = LOW;
bool lastButtonState = HIGH;
bool buttonPressed = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(relayPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);

  digitalWrite(relayPin, relayState);

  lcd.createChar(0, customCharL); // 'ł'
  lcd.createChar(1, customCharA); // 'ą'
  lcd.createChar(2, customCharZ); // 'Ź'

  updateDisplayAndConsole();
}

void loop() {
  bool buttonState = digitalRead(buttonPin);

  if (buttonState == LOW && lastButtonState == HIGH && !buttonPressed) {
    relayState = !relayState;
    digitalWrite(relayPin, relayState);
    updateDisplayAndConsole();
    buttonPressed = true;
  } else if (buttonState == HIGH) {
    buttonPressed = false;
  }

  lastButtonState = buttonState;
  delay(100); // debounce delay
}

void updateDisplayAndConsole() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Relay ");
  lcd.write(byte(2)); // 'Ź'
  lcd.print(relayState ? " ON" : " OFF");

  lcd.setCursor(0, 1);
  if (relayState) {
    lcd.print("Acti");
    lcd.write(byte(0)); // 'ł'
    lcd.write(byte(1)); // 'ą'
    lcd.print("vated");
    Serial.println("Relay Activated");
  } else {
    lcd.print("Deacti");
    lcd.write(byte(0)); // 'ł'
    lcd.write(byte(1)); // 'ą'
    lcd.print("vated");
    Serial.println("Relay Deactivated");
  }
}


```

---

## 📸 Screenshots

📷 *Coming soon!*  
You can add `.png`, `.jpg`, or `.gif` screenshots in the `/screenshots/` folder and embed them here.

---

## 📃 License

This project is licensed under the **MIT License**.  
Feel free to use, modify, and share it freely.

---

## 👨‍💻 Author

**Adrian Lesniak**  
> 💡 Focused on electronics, embedded programming, and user-centric visual feedback.
