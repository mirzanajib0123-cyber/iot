#define TRIG_PIN 5
#define ECHO_PIN 18
#define LED_PIN 14

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT); // TRIG MEMANCARKAN SUARA
  pinMode(ECHO_PIN, INPUT); // ECHO MEMDENGARKAN PANTULAN
  pinMode(LED_PIN, OUTPUT); // KONFIGURASI PIN LED SEBAGI OUTPUT

  digitalWrite(LED_PIN, LOW); // PASTIKAN LED MATI SAAT AWAL BOOT
}

void loop() {
  // 1. MEMANCARKAN GELOMBANG SUARA SELAMA 10 MIKRODETIK
   digitalWrite(TRIG_PIN, LOW);
   delayMicroseconds(2);
   digitalWrite(TRIG_PIN, HIGH);
   delayMicroseconds(10);
   digitalWrite(TRIG_PIN, LOW);

   // 2. MEMBACA WAKTU PANTULAN (DALAM MIKRODETIK)
   long duration = pulseIn(ECHO_PIN, HIGH);

   // 3. MENHITUNG JARAK KE CENTIMETER
   float distance = duration * 0.034 / 2;

   Serial.print("jarak: ");
   Serial.print(distance);
   Serial.print(" cm");

   delay(1000); // JEDA 1 DETIK ANTAR PEMBACA

   // 4. KONDISI LED NYALA JIKA JARAK KURANG DARI 20 CM
  if (distance < 20) {
    digitalWrite(LED_PIN, HIGH); // LED menyala
  } else {
    digitalWrite(LED_PIN, LOW);  // LED mati
  }
}
