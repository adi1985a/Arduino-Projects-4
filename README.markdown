# ⚡ Relay Control with Button and LCD I2C (with Polish Characters)

![Arduino](https://img.shields.io/badge/Platform-Arduino-blue.svg)
![Language](https://img.shields.io/badge/Language-C%2B%2B-brightgreen.svg)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)
![Status](https://img.shields.io/badge/Status-Completed-success.svg)

---

## 📋 Opis projektu  
Ten projekt umożliwia sterowanie przekaźnikiem za pomocą przycisku, a stan przekaźnika jest wyświetlany na wyświetlaczu LCD 16x2 z interfejsem I2C. Na wyświetlaczu pojawiają się także polskie znaki (Ź, ł, ą) dzięki definiowaniu własnych znaków w kodzie. Projekt ten znajduje zastosowanie wszędzie tam, gdzie konieczne jest ręczne sterowanie urządzeniem elektrycznym oraz podgląd jego stanu.

---

## ⚙️ Wymagane komponenty
- Arduino (Uno, Nano lub kompatybilne)
- Wyświetlacz LCD 16x2 z interfejsem I2C (np. 0x27)
- Przycisk
- Przekaźnik 5V
- Rezystor podciągający (jeśli nie używamy `INPUT_PULLUP`)
- Przewody połączeniowe
- Biblioteka [`LiquidCrystal_I2C`](https://github.com/johnrickman/LiquidCrystal_I2C) (instalacja przez menedżer bibliotek w Arduino IDE)

---

## 🛠️ Schemat połączeń

| Komponent     | Pin Arduino        |
|---------------|--------------------|
| Przycisk      | D3 (`INPUT_PULLUP`)|
| Przekaźnik    | D2                 |
| LCD SDA       | A4 (dla Uno)       |
| LCD SCL       | A5 (dla Uno)       |
| LCD VCC / GND | 5V / GND           |

---

## ⚡ Działanie systemu

- **LCD** pokazuje stan przekaźnika: `Ź ON` lub `Ź OFF`
- **Przycisk** przełącza stan przekaźnika
- Zaimplementowano **debouncing**
- Polskie znaki (Ź, ł, ą) wyświetlane są dzięki znakom niestandardowym

---

## 💻 Kod źródłowy

```cpp
#include <LiquidCrystal_I2C.h>

// Definicje znaków specjalnych (Ź, ł, ą)
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
  delay(100); // debounce
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
