#include <DHT.h>

#define DHTPIN1 14
#define DHTPIN2 13
#define DHTTYPE DHT22

DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

void setup() {
  Serial.begin(115200);

  dht1.begin();
  dht2.begin();

  Serial.println("Membaca dua sensor DHT22...");
}

void loop() {
  // Sensor pertama
  float suhu1 = dht1.readTemperature();
  float lembap1 = dht1.readHumidity();

  // Sensor kedua
  float suhu2 = dht2.readTemperature();
  float lembap2 = dht2.readHumidity();

  Serial.println("\n--- SENSOR DHT22 PERTAMA ---");

  if (isnan(suhu1) || isnan(lembap1)) {
    Serial.println("Gagal membaca sensor 1");
  } else {
    Serial.print("Suhu: ");
    Serial.print(suhu1);
    Serial.println(" ℃");

    Serial.print("Kelembapan: ");
    Serial.print(lembap1);
    Serial.println(" %");
  }

  Serial.println("--- SENSOR DHT22 KEDUA ---");

  if (isnan(suhu2) || isnan(lembap2)) {
    Serial.println("Gagal membaca sensor 2");
  } else {
    Serial.print("Suhu: ");
    Serial.print(suhu2);
    Serial.println(" ℃");

    Serial.print("Kelembapan: ");
    Serial.print(lembap2);
    Serial.println(" %");
  }

  delay(2500);
}
