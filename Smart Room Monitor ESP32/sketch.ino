#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

// Definisi Pin
#define DHTPIN 15     // Pin Data DHT22 
#define DHTTYPE DHT22 // Tipe sensor DHT
#define LDR_PIN 34    // Pin Analog LDR (Input ke-2)
#define BUZZER_PIN 5  // Pin Buzzer
#define LED_PANAS 13  // Pin LED Merah (Overheat)
#define LED_DINGIN 12 // Pin LED Biru (Suhu Dingin)

// Inisialisasi Objek
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  
  // Inisialisasi Sensor dan Output
  dht.begin();
  lcd.init();
  lcd.backlight();
  
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PANAS, OUTPUT);
  pinMode(LED_DINGIN, OUTPUT);
  pinMode(LDR_PIN, INPUT); // LDR sebagai Input
  
  // Pesan Awal di LCD
  lcd.setCursor(0, 0);
  lcd.print("Sistem Pemantau ");
  lcd.setCursor(0, 1);
  lcd.print("Ruangan Aktif!  ");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Membaca data dari 2 sensor input
  float suhu = dht.readTemperature();
  int nilaiCahaya = analogRead(LDR_PIN); 
  
  // Menentukan status cahaya (Di Wokwi, nilai analog tinggi = gelap)
  String statusCahaya = (nilaiCahaya > 2000) ? "Gelap " : "Terang";

  // Baris 1: Menampilkan Suhu dan Status Cahaya
  lcd.setCursor(0, 0);
  if (isnan(suhu)) {
    lcd.print("Error DHT!      ");
  } else {
    lcd.print("S:");
    lcd.print(suhu, 1); // 1 angka di belakang koma
    lcd.print("C ");
    lcd.print(statusCahaya); 
    lcd.print("   "); // Spasi untuk membersihkan sisa karakter
  }

  // Baris 2: Logika Otomatisasi Suhu (Buzzer & 2 LED)
  lcd.setCursor(0, 1);
  
  // Kondisi 1: Suhu Panas
  if (suhu > 35.0) {
    lcd.print("SUHU OVERHEAT!  ");
    digitalWrite(LED_PANAS, HIGH); 
    digitalWrite(LED_DINGIN, LOW); 
    tone(BUZZER_PIN, 2000);      
    delay(500);
    digitalWrite(LED_PANAS, LOW);  
    noTone(BUZZER_PIN);
  } 
  // Kondisi 2: Suhu Dingin
  else if (suhu < 20.0) {
    lcd.print("SUHU DINGIN!    ");
    digitalWrite(LED_DINGIN, HIGH); 
    digitalWrite(LED_PANAS, LOW);   
    tone(BUZZER_PIN, 1000);         
    delay(500);
    digitalWrite(LED_DINGIN, LOW);  
    noTone(BUZZER_PIN);
  } 
  // Kondisi 3: Suhu Normal
  else {
    lcd.print("Suhu Normal     ");
    digitalWrite(LED_PANAS, LOW); 
    digitalWrite(LED_DINGIN, LOW); 
    noTone(BUZZER_PIN);         
  }

  delay(500);
}
