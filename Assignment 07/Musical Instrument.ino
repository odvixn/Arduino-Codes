int analogSensorValue;
bool prevSwitchState = false;

int outputPattern=0; // different sound making modes
int frequency = 0; // pitch
int bps = 1;  // Beat per second
long previousMillis = 0;  // signed long to store last time when made sound

int piezo = 8;
int switchSensor = A0;
int potentiometer = A2;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read values from potentiometer
  int sensorValue = analogRead(potentiometer)/5*5;  // <- to mitigate unintended frequency change caused by the sensor 
  delay(1);  // delay in between reads for stability
  frequency = map(sensorValue,0,1024,0,2000); // limit frequency range to avoid unpleasant sound

  manageOutputPattern(); // determines which sound mode will be played

  if (!!outputPattern) { // if outputPattern is 0, idle(). Else make sound.
    makeSound();
  } else {
    idle();
  }
}

void manageOutputPattern() {  // button press -> change output pattern
  bool switchState = digitalRead(switchSensor);
  if (switchState && !prevSwitchState) {  // !prevSwitchState prevent undesirable changes
    Serial.println(outputPattern);
    outputPattern = (outputPattern + 1) % 6;
    bps = pow(2,outputPattern+1); // 2 4 8 16 32 bps
  }
  prevSwitchState = switchState;
}

void makeSound() {
  unsigned long currentMillis = millis();
  // to calculate the note duration, take one second divided by a given bps value.
  int cycleLength = 1000 / bps;  // <- cycle length = tone duration + rest
  if (currentMillis - previousMillis > cycleLength) {
    previousMillis = currentMillis;
    tone(piezo, frequency, cycleLength/1.3); // Similar to "ToneMelody" Example, delay is *1.3
  }
}

void idle() {
  noTone(piezo); // turn off sound immediately
  delay(50);
}
