#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h> 

const byte ROWS = 4;
const byte COLS = 4;

// Definisi layout keypad
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; // Pin baris
byte colPins[COLS] = {5, 4, 3, 2}; // Pin kolom

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);  

// Servo
Servo servo1; 
Servo servo2; 
int servo1Pin = 11;        // Pin servo pertama
int servo2Pin = 12;        // Pin servo kedua
int servo1Closed = 0;      // Posisi awal servo 1 (pintu terkunci)
int servo1Activated = 180; // Posisi servo 1 saat aktif (180 derajat)/kunci terbuka
int servo2Closed = 0;      // Posisi awal servo 2 (pintu tertutup)
int servo2Activated = 90;  // Posisi servo 2 saat aktif (90 derajat)/pintu terbuka

// Sensor sentuh
int touchSensorPin = 10;   // Pin sensor sentuh

// Variabel password
String enteredPassword = "";       // Tempat menyimpan input password
String correctPassword = "123456"; // Password yang benar

void setup() {
  // Inisialisasi LCD
  lcd.backlight();
  lcd.init(); 
  lcd.setCursor(0, 0);
  lcd.print("Enter Password:");
  lcd.setCursor(0, 1);

  // Inisialisasi servo
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo1.write(servo1Closed); // Servo 1 ke posisi awal
  servo2.write(servo2Closed); // Servo 2 ke posisi awal

  // Inisialisasi sensor sentuh
  pinMode(touchSensorPin, INPUT);
}

void loop() {
  char customKey = customKeypad.getKey();

  // Cek input dari keypad
  if (customKey) {
    if (customKey == '#') { // Tombol '#' untuk mengonfirmasi password
      lcd.clear();
      if (enteredPassword == correctPassword) {
        lcd.print("correct");
        activateServos(); // Jalankan logika servo
      } else {
        lcd.print("Incorrect");
      }
      delay(2000);
      enteredPassword = ""; // Reset input password
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Password:");
      lcd.setCursor(0, 1);
    } else if (customKey == '*') { // Tombol '*' untuk menghapus input
      enteredPassword = "";
      lcd.setCursor(0, 1);
      lcd.print("                "); // Clear baris kedua
      lcd.setCursor(0, 1);
    } else { // Tambahkan input ke password
      if (enteredPassword.length() < 16) { // Batasi panjang password
        enteredPassword += customKey;
        lcd.print(customKey); // Tampilkan input
      }
    }
  }

  // Cek sensor sentuh
  if (digitalRead(touchSensorPin) == HIGH) {
    lcd.clear();
    lcd.print("Touch sensor ON");
    activateServos(); // Jalankan logika servo
    delay(2000); // Hindari pembacaan berulang
  }
}

// Fungsi untuk menggerakkan servo
void activateServos() {
  // Gerakkan servo 1 ke posisi aktif
  servo1.write(servo1Activated);
  delay(3000); // Tunggu 3 detik

  // Gerakkan servo 2 ke posisi aktif
  servo2.write(servo2Activated);
  delay(2000); // Tunggu 2 detik

  // Kembalikan servo ke posisi awal
  servo2.write(servo2Closed); // Servo 2 kembali ke posisi awal
  delay(3000); // Tunggu 3 detik
  servo1.write(servo1Closed); // Servo 1 kembali ke posisi awal
}
