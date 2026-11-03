#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

// Inisialisasi LCD pada alamat 0x27 dengan ukuran 16 kolom x 2 baris
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Inisialisasi RTC DS1307
RTC_DS1307 rtc;

// Array untuk nama hari
const char* namaHari[] = {"Min", "Sen", "Sel", "Rab", "Kam", "Jum", "Sab"};

void setup() {
  Serial.begin(115200);

  // Mulai komunikasi I2C dan inisialisasi LCD
  lcd.init();
  lcd.backlight();
  
  // Menampilkan pesan awal
  lcd.setCursor(0, 0);
  lcd.print("Inisialisasi...");
  
  // Memulai RTC
  if (!rtc.begin()) {
    Serial.println("RTC tidak ditemukan");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RTC Error!");
    while (1); // Berhenti jika RTC tidak ada
  }

  // Jika RTC kehilangan daya (pertama kali dijalankan), atur waktunya
  if (!rtc.isrunning()) {
    Serial.println("RTC belum diatur, mengatur waktu sekarang...");
    // Mengatur RTC ke tanggal & waktu saat kode dikompilasi
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    
    // Jika ingin mengatur waktu secara manual, gunakan format ini:
    // rtc.adjust(DateTime(2026, 11, 3, 14, 50, 0)); // Tahun, Bulan, Tanggal, Jam, Menit, Detik
  }

  delay(1000);
  lcd.clear();
}

void loop() {
  // Ambil waktu dari RTC
  DateTime now = rtc.now();

  // Buffer untuk format string
  char timeBuffer[10];
  char dateBuffer[15];

  // Format Jam (HH:MM:SS) menggunakan sprintf agar selalu 2 digit (misal: 09:05:01)
  sprintf(timeBuffer, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  
  // Format Tanggal (DD/MM/YYYY)
  sprintf(dateBuffer, "%s,%02d/%02d/%04d", namaHari[now.dayOfTheWeek()], now.day(), now.month(), now.year());

  // Tampilkan Jam di baris pertama
  lcd.setCursor(0, 0);
  lcd.print("Jam: ");
  lcd.print(timeBuffer);

  // Tampilkan Tanggal di baris kedua
  lcd.setCursor(0, 1);
  lcd.print(dateBuffer);

  // Delay 1 detik
  delay(1000);
}
