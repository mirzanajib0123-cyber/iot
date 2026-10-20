const int ledPin = 2; // ganti sesuai dengan pin  anda guanakan
const int buttonPin = 21; // ganti sesuai dentan pin yang anda gunakan

int buttonState = 0; // variabel untuk menyimpan sengan status tombol

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  buttonState = digitalRead(buttonPin); //baca status tombol

  if (buttonState == HIGH) { // tombol ditekan 
    digitalWrite(ledPin, HIGH); // hidupkan led
  } else {
    digitalWrite(ledPin, LOW); // matikan led
  }
}
