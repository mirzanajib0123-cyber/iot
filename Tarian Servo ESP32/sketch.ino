#include <ESP32Servo.h>
#include <math.h>

// Urutan 14 pin searah jarum jam mengelilingi ESP32:
// [0..1] Atas: servo14(32), servo13(23)
// [2..6] Kanan (atas->bawah): servo12(22), servo9(21), servo8(19), servo7(18), servo6(5)
// [7..8] Bawah: servo11(17), servo10(13)
// [9..13] Kiri (bawah->atas): servo5(14), servo4(27), servo3(26), servo1(25), servo2(33)
const int servoPins[] = {32, 23, 22, 21, 19, 18, 5, 17, 13, 14, 27, 26, 25, 33};
const int NUM_SERVOS = 14;

Servo servos[NUM_SERVOS];

// Fungsi gerak halus (ease-in-out) untuk seluruh 14 servo sekaligus
void gerakHalus(int targetSudut[], int durasiMs) {
  int sudutAwal[NUM_SERVOS];
  for (int i = 0; i < NUM_SERVOS; i++) {
    sudutAwal[i] = servos[i].read();
  }

  int langkah = durasiMs / 15;
  if (langkah < 1) langkah = 1;

  for (int step = 1; step <= langkah; step++) {
    float t = (float)step / langkah;
    float ease = 0.5 - 0.5 * cos(t * PI);

    for (int i = 0; i < NUM_SERVOS; i++) {
      int pos = sudutAwal[i] + (targetSudut[i] - sudutAwal[i]) * ease;
      servos[i].write(constrain(pos, 0, 180));
    }
    delay(15);
  }
}

// Babak 1: Gelombang Naga 360 Derajat (melingkar mulus tanpa putus)
void tarianGelombangNaga(int putaran) {
  Serial.println("Babak 1: Gelombang Naga 360 Derajat");
  float jarakFase = (2.0 * PI) / NUM_SERVOS; // 1 gelombang penuh mengelilingi 14 servo

  for (int step = 0; step < putaran * 70; step++) {
    float fase = step * 0.12;
    for (int i = 0; i < NUM_SERVOS; i++) {
      float gelombang = sin(fase - i * jarakFase);
      int sudut = 90 + (int)(gelombang * 75); // Rentang 15° - 165°
      servos[i].write(constrain(sudut, 0, 180));
    }
    delay(20);
  }
}

// Babak 2: Tarian Seribu Tangan (Mekar Simetris Kiri & Kanan)
void tarianSeribuTangan() {
  Serial.println("Babak 2: Tarian Seribu Tangan & Kipas Sutra");
  int target[NUM_SERVOS];

  // 1. Posisi netral tegak
  for (int i = 0; i < NUM_SERVOS; i++) target[i] = 90;
  gerakHalus(target, 600);
  delay(200);

  // 2. Mekar bertingkat dari atas ke bawah (sayap kiri & kanan simetris)
  // Catatan: servo kiri di-rotate 180 di diagram.json, jadi sudut cerminnya = 180 - sudutKanan
  int sudutSayap[5] = {25, 55, 90, 125, 155};
  for (int baris = 0; baris < 5; baris++) {
    int idxKanan = 2 + baris;   // index 2..6
    int idxKiri  = 13 - baris;  // index 13..9

    target[idxKanan] = sudutSayap[baris];
    target[idxKiri]  = 180 - sudutSayap[baris];
    gerakHalus(target, 280);
  }

  // Mahkota atas & bawah ikut mekar
  target[0] = 40;  target[1] = 140; // Atas (servo14 & servo13)
  target[8] = 140; target[7] = 40;  // Bawah (servo10 & servo11)
  gerakHalus(target, 450);
  delay(400);

  // 3. Ombak bergantian antara sayap kiri dan kanan
  for (int ayun = 0; ayun < 3; ayun++) {
    for (int baris = 0; baris < 5; baris++) {
      target[2 + baris]  = (baris % 2 == 0) ? 35 : 145;
      target[13 - baris] = (baris % 2 == 0) ? 145 : 35;
    }
    gerakHalus(target, 450);

    for (int baris = 0; baris < 5; baris++) {
      target[2 + baris]  = (baris % 2 == 0) ? 145 : 35;
      target[13 - baris] = (baris % 2 == 0) ? 35 : 145;
    }
    gerakHalus(target, 450);
  }
}

// Babak 3: Irama Barongsai & Finale Pusaran
void tarianBarongsai() {
  Serial.println("Babak 3: Irama Barongsai & Pusaran");
  for (int ketukan = 0; ketukan < 4; ketukan++) {
    // Anggukan ritmis serempak
    for (int i = 0; i < NUM_SERVOS; i++) {
      servos[i].write((i % 2 == 0) ? 140 : 40);
    }
    delay(160);

    for (int i = 0; i < NUM_SERVOS; i++) {
      servos[i].write(90);
    }
    delay(130);

    for (int i = 0; i < NUM_SERVOS; i++) {
      servos[i].write((i % 2 == 0) ? 160 : 20);
    }
    delay(220);
  }

  // Sapuan berantai (cascade) searah jarum jam lalu kembali ke tengah
  for (int i = 0; i < NUM_SERVOS; i++) {
    servos[i].write(165);
    delay(65);
  }
  delay(200);
  for (int i = NUM_SERVOS - 1; i >= 0; i--) {
    servos[i].write(90);
    delay(65);
  }
  delay(400);
}

void setup() {
  Serial.begin(115200);

  // Wajib alokasi ke-4 timer PWM ESP32 agar 14 servo bisa aktif bersamaan
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  for (int i = 0; i < NUM_SERVOS; i++) {
    servos[i].setPeriodHertz(50);
    servos[i].attach(servoPins[i], 500, 2400);
    servos[i].write(90);
  }
  delay(800);
}

void loop() {
  tarianGelombangNaga(3);
  tarianSeribuTangan();
  tarianBarongsai();
}
