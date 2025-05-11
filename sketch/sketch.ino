#include <LiquidCrystal_I2C.h> // Dołączenie biblioteki do obsługi wyświetlacza LCD z interfejsem I2C

// Definicje polskich znaków
byte customCharZ[8] = {B00010,B00100,B11111,B00010,B00100,B01000,B11111,B00000}; // Zdefiniowanie własnego znaku dla litery 'Ź'
byte customCharL[8] = {B01100, B00100, B00110, B00100, B01100, B00100, B01110, B00000}; // Zdefiniowanie własnego znaku dla litery 'ł'
byte customCharA[8] = {B00000,B0000,B01110,B00001,B01111,B10001,B01111,B00001}; // Zdefiniowanie własnego znaku dla litery 'ą'

const int relayPin = 2; // Deklaracja pinu do sterowania przekaźnikiem
const int buttonPin = 3; // Deklaracja pinu do odczytu stanu przycisku
bool relayState = LOW;   // Początkowy stan przekaźnika (wyłączony)
bool lastButtonState = HIGH;  // Początkowy stan przycisku (nie naciśnięty, przy użyciu pullup)
bool buttonPressed = false;   // Flaga wskazująca, czy przycisk został naciśnięty

LiquidCrystal_I2C lcd(0x27, 16, 2); // Inicjalizacja wyświetlacza LCD z adresem I2C 0x27 i rozmiarem 16x2

void setup() {
  pinMode(relayPin, OUTPUT); // Ustawienie pinu przekaźnika jako wyjście
  pinMode(buttonPin, INPUT_PULLUP); // Ustawienie pinu przycisku jako wejście z wewnętrznym rezystorem podciągającym
  lcd.init(); // Inicjalizacja LCD
  lcd.backlight(); // Włączenie podświetlenia LCD
  Serial.begin(9600); // Rozpoczęcie komunikacji szeregowej z prędkością 9600 bps

  // Ustawienie początkowego stanu przekaźnika
  digitalWrite(relayPin, relayState);

  // Utworzenie własnych znaków na LCD
  lcd.createChar(0, customCharL); // Utworzenie znaku dla 'ł'
  lcd.createChar(1, customCharA); // Utworzenie znaku dla 'ą'
  lcd.createChar(2, customCharZ); // Utworzenie znaku dla 'Ź'

  // Wyświetlenie początkowego stanu na LCD i konsoli
  updateDisplayAndConsole();
}

void loop() {
  bool buttonState = digitalRead(buttonPin); // Odczytanie stanu przycisku

  // Sprawdzenie czy przycisk został naciśnięty
  if (buttonState == LOW && lastButtonState == HIGH && !buttonPressed) {
    relayState = !relayState; // Zmiana stanu przekaźnika
    digitalWrite(relayPin, relayState); // Zastosowanie zmiany stanu przekaźnika
    updateDisplayAndConsole(); // Aktualizacja wyświetlacza i konsoli
    buttonPressed = true;  // Ustawienie flagi naciśnięcia przycisku
  } else if (buttonState == HIGH) {
    buttonPressed = false;  // Reset flagi naciśnięcia gdy przycisk zostanie zwolniony
  }

  lastButtonState = buttonState; // Aktualizacja ostatniego stanu przycisku
  delay(100);  // Krótkie opóźnienie dla debouncing
}

void updateDisplayAndConsole() {
  lcd.clear(); // Wyczyszczenie wyświetlacza
  lcd.setCursor(0, 0); // Ustawienie kursora na początku pierwszej linii
  if (relayState) {
    lcd.print("Przeka"); // Wyświetlenie tekstu na LCD
    lcd.write(byte(2)); // Wstawienie znaku 'Ź'
    lcd.print("nik ");
  } else {
    lcd.print("Przeka");
    lcd.write(byte(2)); // Wstawienie znaku 'Ź'
    lcd.print("nik ");
  }

  lcd.setCursor(0, 1); // Ustawienie kursora na początku drugiej linii
  if (relayState) {
    lcd.print("Za");
    lcd.write(byte(0)); // Wstawienie znaku 'ł'
    lcd.write(byte(1)); // Wstawienie znaku 'ą'
    lcd.print("czony");
    Serial.println("Przekaźnik załączony");
  } else {
    lcd.print("Wy");
    lcd.write(byte(0)); // Wstawienie znaku 'ł'
    lcd.write(byte(1)); // Wstawienie znaku 'ą'
    lcd.print("czony");
    Serial.println("Przekaźnik wyłączony");
  }
}
