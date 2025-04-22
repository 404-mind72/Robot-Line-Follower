// Pin sensor TCRT5000
const int sensor1 = A0; // paling kiri
const int sensor2 = A1;
const int sensor3 = A2; // tengah
const int sensor4 = A3;
const int sensor5 = A4; // paling kanan

// Motor kanan (motor A)
const int in1 = 7;
const int in2 = 6;

// Motor kiri (motor B)
const int in3 = 5;
const int in4 = 4;

// Kecepatan motor (PWM)
const int ena = 9; // motor kanan (PWM)
const int enb = 10; // motor kiri (PWM)

// Nilai pembacaan sensor (LOW = garis hitam, HIGH = putih)
int s1, s2, s3, s4, s5;

void setup() {
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);
  pinMode(sensor5, INPUT);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  s1 = digitalRead(sensor1);
  s2 = digitalRead(sensor2);
  s3 = digitalRead(sensor3);
  s4 = digitalRead(sensor4);
  s5 = digitalRead(sensor5);

  // Tampilkan pembacaan sensor di Serial Monitor
  Serial.print(s1); Serial.print(" ");
  Serial.print(s2); Serial.print(" ");
  Serial.print(s3); Serial.print(" ");
  Serial.print(s4); Serial.print(" ");
  Serial.println(s5);

  // Logika line follower dasar
  if (s3 == LOW && s2 == HIGH && s4 == HIGH) {
    maju(); // garis di tengah
  }
  else if (s2 == LOW && s1 == HIGH) {
    kiri(); // garis ke kiri
  }
  else if (s4 == LOW && s5 == HIGH) {
    kanan(); // garis ke kanan
  }
  else if (s1 == LOW) {
    putarKiri(); // belok tajam kiri
  }
  else if (s5 == LOW) {
    putarKanan(); // belok tajam kanan
  }
  else {
    berhenti(); // tidak menemukan garis
  }

  delay(50);
}

void maju() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(ena, 180);
  analogWrite(enb, 180);
}

void kiri() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(ena, 0);
  analogWrite(enb, 180);
}

void kanan() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(ena, 180);
  analogWrite(enb, 0);
}

void putarKiri() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(ena, 180);
  analogWrite(enb, 180);
}

void putarKanan() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(ena, 180);
  analogWrite(enb, 180);
}

void berhenti() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(ena, 0);
  analogWrite(enb, 0);
}
