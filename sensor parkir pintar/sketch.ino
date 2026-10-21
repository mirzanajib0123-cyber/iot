// Definisi Pin Komponen
const int TRIG_PIN = 5;      // Pin TRIG Sensor Ultrasonik
const int ECHO_PIN = 18;     // Pin ECHO Sensor Ultrasonik

const int LED_RED = 25;      // LED Merah (Sangat Dekat / Bahaya)
const int LED_YELLOW = 26;   // LED Kuning (Hati-hati)
const int LED_GREEN = 27;    // LED Hijau (Aman)
const int BUZZER_PIN = 14;   // Pin Buzzer

long duration;
float distance;

void setup() {
  Serial.begin(115200);

  // Konfigurasi Mode Pin
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  // Mengirimkan sinyal ultrasonik
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Membaca waktu pantulan sinyal
  duration = pulseIn(ECHO_PIN, HIGH);

  // Menghitung jarak dalam satuan centimeter (cm)
  distance = duration * 0.034 / 2;

  // Menampilkan jarak pada Serial Monitor Wokwi
  Serial.print("Jarak Parkir: ");
  Serial.print(distance);
  Serial.println("cm ");

  // LOGIKA KONDISI SESUAI INSTRUKSI GURU:

  // KONDISI 1: Jarak > 60 cm (AMAN)
  if (distance > 60) {
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, LOW);
    
    noTone(BUZZER_PIN); // Buzzer mati
  } 
  // KONDISI 2: Jarak 30 cm - 60 cm (HATI-HATI)
  else if (distance >= 30 && distance <= 60) {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_RED, LOW);
    
    // Buzzer berbunyi lambat
    tone(BUZZER_PIN, 1000);
    delay(300);
    noTone(BUZZER_PIN);
    delay(300);
  } 
  // KONDISI 3: Jarak < 30 cm (BAHAYA / STOP!)
  else if (distance < 30) {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, HIGH);
    
    // Buzzer berbunyi cepat / terus-menerus
    tone(BUZZER_PIN, 1000);
    delay(80);
    noTone(BUZZER_PIN);
    delay(80);
  }

  delay(100);
}
