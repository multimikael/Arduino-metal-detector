const byte interruptPin = 3, btnPin = 9, diodePin = 12;
const float TIMER = 400, DEVIATION = 0.26;
volatile long count = 0;
long startTime;
unsigned long last0, last1, last2, notMetal=1;

void setup() {
  Serial.begin(9600);
  startTime = millis();
  pinMode(diodePin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), on_interrupt, RISING);
}

void loop() {
  if (digitalRead(btnPin) == HIGH) {
    notMetal = (last2+last1+last0)/3;
    delay(125);
    Serial.print("New notMetal: ");
    Serial.println(notMetal);
  }
  
  if (millis()-startTime >= TIMER) {
    noInterrupts();
    Serial.println(count);
	float countDeviation = float(count)/notMetal
    Serial.println(countDeviation);
    if (countDeviation >= 1+DEVIATION/100 || countDeviation <= 1-DEVIATION/100) {
      Serial.println("METAL");
      digitalWrite(diodePin, HIGH);
    } else {
      digitalWrite(diodePin, LOW);
    }

    last2 = last1;
    last1 = last0;
    last0 = count;
    count = 0;
    startTime = millis();
    interrupts();
  }
}

void on_interrupt() {
  count++;
}
