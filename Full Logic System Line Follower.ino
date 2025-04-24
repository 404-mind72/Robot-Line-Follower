// --- Sensor Setup ---
const int sensorPins[5] = {A0, A1, A2, A3, A4}; // Left to Right
int sensorRaw[5];
int sensorMin[5] = {1023, 1023, 1023, 1023, 1023};
int sensorMax[5] = {0, 0, 0, 0, 0};
bool calibrated = false;

const int buttonPin = 8; // Push button for calibration

// --- Motor Pins ---
const int in1 = 7, in2 = 6, ena = 9;   // Motor A - Right
const int in3 = 5, in4 = 4, enb = 10;  // Motor B - Left

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 5; i++) pinMode(sensorPins[i], INPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(in1, OUTPUT); pinMode(in2, OUTPUT); pinMode(ena, OUTPUT);
  pinMode(in3, OUTPUT); pinMode(in4, OUTPUT); pinMode(enb, OUTPUT);

  Serial.println("🔧 Tekan tombol kalibrasi atau kirim 'c' untuk mulai kalibrasi.");
}

void loop() {
  // Start calibration if button is pressed or 'c' is sent over Serial
  if (digitalRead(buttonPin) == LOW || (Serial.available() && Serial.read() == 'c')) {
    kalibrasiSensor();
    calibrated = true;
  }

  bacaSensor();

  if (!calibrated) {
    Serial.println("⚠️ Belum dikalibrasi! Tekan tombol atau kirim 'c'");
    delay(1000);
    return;
  }

  // Interpretasi sensor: TRUE = garis hitam (nilai rendah)
  bool s1 = isHitam(0);
  bool s2 = isHitam(1);
  bool s3 = isHitam(2);
  bool s4 = isHitam(3);
  bool s5 = isHitam(4);

  // Debug output (optional)
  Serial.print("Sensor: ");
  for (int i = 0; i < 5; i++) {
    Serial.print(sensorRaw[i]); Serial.print(" ");
  }
  Serial.print(" | H:");
  Serial.print(s1); Serial.print(" ");
  Serial.print(s2); Serial.print(" ");
  Serial.print(s3); Serial.print(" ");
  Serial.print(s4); Serial.print(" ");
  Serial.println(s5);

  // --- Decision Logic for Line Following ---
  if (s3 && !s2 && !s4)        maju();       // Line is in the center
  else if (s2 && !s1)          kiri();       // Slightly to the left
  else if (s4 && !s5)          kanan();      // Slightly to the right
  else if (s1)                 putarKiri();  // Sharp left
  else if (s5)                 putarKanan(); // Sharp right
  else                         berhenti();   // Lost line

  delay(50);
}

// --- Sensor Reading ---
void bacaSensor() {
  for (int i = 0; i < 5; i++) {
    sensorRaw[i] = analogRead(sensorPins[i]);
  }
}

// Interpretasi nilai sebagai 'hitam'
bool isHitam(int index) {
  int batas = (sensorMin[index] + sensorMax[index]) / 2;
  return sensorRaw[index] < batas;
}

// --- Calibration ---
void kalibrasiSensor() {
  Serial.println("🛠️ Kalibrasi dimulai... Geser robot melintasi garis & latar selama 5 detik.");

  // Reset values
  for (int i = 0; i < 5; i++) {
    sensorMin[i] = 1023;
    sensorMax[i] = 0;
  }

  unsigned long start = millis();
  while (millis() - start < 5000) {
    bacaSensor();
    for (int i = 0; i < 5; i++) {
      if (sensorRaw[i] < sensorMin[i]) sensorMin[i] = sensorRaw[i];
      if (sensorRaw[i] > sensorMax[i]) sensorMax[i] = sensorRaw[i];
    }
    delay(10);
  }

  Serial.println("✅ Kalibrasi selesai!");
  for (int i = 0; i < 5; i++) {
    Serial.print("Sensor "); Serial.print(i);
    Serial.print(" | Min: "); Serial.print(sensorMin[i]);
    Serial.print(" | Max: "); Serial.println(sensorMax[i]);
  }
}

// --- Motor Logic ---
void maju() {
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
  analogWrite(ena, 180); analogWrite(enb, 180);
}

void kiri() {
  digitalWrite(in1, LOW); digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
  analogWrite(ena, 0); analogWrite(enb, 180);
}

void kanan() {
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); digitalWrite(in4, LOW);
  analogWrite(ena, 180); analogWrite(enb, 0);
}

void putarKiri() {
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
  analogWrite(ena, 180); analogWrite(enb, 180);
}

void putarKanan() {
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); digitalWrite(in4, HIGH);
  analogWrite(ena, 180); analogWrite(enb, 180);
}

void berhenti() {
  digitalWrite(in1, LOW); digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); digitalWrite(in4, LOW);
  analogWrite(ena, 0); analogWrite(enb, 0);
}
