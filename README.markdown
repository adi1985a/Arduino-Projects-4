# Sterowanie przekaźnikiem za pomocą przycisku i LCD I2C (z polskimi znakami)

## Opis projektu

W tym projekcie stworzymy system do sterowania przekaźnikiem za pomocą przycisku, a stan przekaźnika będzie wyświetlany na wyświetlaczu LCD 16x2 z interfejsem I2C. Dodatkowo, na LCD użyjemy polskich znaków, takich jak `Ź`, `ł` i `ą`, przy pomocy własnych znaków graficznych zdefiniowanych w kodzie.

Projekt będzie przydatny w różnych aplikacjach, gdzie konieczne jest kontrolowanie urządzeń za pomocą przycisku, a także monitorowanie ich stanu na wyświetlaczu.

## Wymagane elementy

- **Arduino** (np. Arduino Uno, Nano lub kompatybilne)
- **Wyświetlacz LCD 16x2 z interfejsem I2C** (np. z adresem 0x27)
- **Przycisk** (do sterowania stanem przekaźnika)
- **Przekaźnik** (moduł przekaźnika, np. 5V)
- **Rezystor podciągający** (jeśli nie używamy `INPUT_PULLUP` na pinie przycisku)
- **Przewody połączeniowe**
- **Biblioteka `LiquidCrystal_I2C`** (do zainstalowania w Arduino IDE)

## Schemat połączeń

| Komponent         | Pin Arduino |
|-------------------|-------------|
| **Przycisk**      | D3 (z `INPUT_PULLUP`) |
| **Przekaźnik**    | D2          |
| **LCD SDA**       | A4 (Uno)    |
| **LCD SCL**       | A5 (Uno)    |
| **Zasilanie LCD** | 5V, GND     |

### Opis połączeń:

1. **Przycisk**:
   - Przycisk podłączony do pinu D3.
   - Używamy wbudowanego rezystora podciągającego (`INPUT_PULLUP`), więc nie potrzebujemy zewnętrznego rezystora.
   - Gdy przycisk jest naciśnięty, stan pinu D3 będzie niski (`LOW`).

2. **Przekaźnik**:
   - Przekaźnik podłączony do pinu D2.
   - Sterowanie stanem przekaźnika odbywa się poprzez zmianę stanu pinu D2 z LOW na HIGH i odwrotnie.

3. **Wyświetlacz LCD**:
   - Wyświetlacz LCD podłączony do pinów SDA (A4) i SCL (A5) na Arduino.
   - Zasilanie LCD podłączone do 5V i GND.

## Instalacja

1. **Zainstalowanie bibliotek**:
   Aby móc korzystać z wyświetlacza LCD, należy zainstalować bibliotekę `LiquidCrystal_I2C` w Arduino IDE:

   - Otwórz Arduino IDE.
   - Przejdź do **Szkic > Dołącz bibliotekę > Zarządzaj bibliotekami**.
   - Wyszukaj **LiquidCrystal_I2C** i zainstaluj ją.

2. **Podłączenie komponentów**:
   - Podłącz przycisk, przekaźnik i wyświetlacz LCD zgodnie z powyższym schematem.

3. **Wgranie kodu**:
   - Otwórz Arduino IDE i wgraj kod do swojego Arduino.

## Działanie systemu

### Wyświetlanie stanu przekaźnika

Po uruchomieniu urządzenia na wyświetlaczu LCD pojawi się początkowy stan przekaźnika – wyłączony. Przycisk w Arduino będzie odpowiedzialny za przełączanie stanu przekaźnika.

Każde naciśnięcie przycisku zmieni stan przekaźnika:
- Jeśli przekaźnik był wyłączony, zostanie włączony.
- Jeśli przekaźnik był włączony, zostanie wyłączony.

Stan przekaźnika będzie wyświetlany na wyświetlaczu LCD. Zmiana stanu będzie również widoczna w monitorze szeregowym.

### Polskie znaki

W celu wyświetlenia polskich znaków (`Ź`, `ł`, `ą`) na wyświetlaczu LCD, zdefiniowane zostały niestandardowe znaki. Dzięki temu możemy wyświetlać polskie litery bez problemów z kodowaniem.

### Obsługa przycisku

Przycisk umożliwia przełączanie stanów:
- Jeśli przekaźnik jest wyłączony, po naciśnięciu przycisku włączy się.
- Jeśli przekaźnik jest włączony, po naciśnięciu przycisku wyłączy się.

System obsługuje debouncing (eliminowanie zakłóceń wynikających z drgań styków przycisku), co zapewnia stabilne działanie przycisku.

## Kod źródłowy

Poniżej znajduje się pełny kod źródłowy programu:

```cpp
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
    lcd.write(byte(1)); // Wstawienie zn
