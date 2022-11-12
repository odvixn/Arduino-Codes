int counter = 0;
int analogSensorValue;
bool prevSwitchState = false;

int outputPattern=0; // different sound making modes
int frequency = 0; // pitch
int tempo = 1;  // Beat per second

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

  // was supposed to do something similar to x 1.3 delay in "ToneMelody" example
  // But this is flawed as noteDuration is less than (1300-1000) when BPS is >2. 
  // However, the product seems to work fine so will not correct this issue.
  counter = (counter + 1) % 1300;
}

void manageOutputPattern() {  // button press -> change output pattern
  bool switchState = digitalRead(switchSensor);
  if (switchState && !prevSwitchState) {  // !prevSwitchState prevent undesirable changes
    Serial.println(outputPattern);
    outputPattern = (outputPattern + 1) % 6;
    tempo = pow(2,outputPattern+1); // 2 4 8 16 32 bps
    counter = 0;
  }
  prevSwitchState = switchState;
}

void makeSound() {
    // to calculate the note duration, take one second divided by a given tempo value.
    int noteDuration = 1000 / tempo;  // <- create how many times it will make sound in a given second.
    if (counter%noteDuration==0) {
      tone(piezo, frequency, noteDuration);
    }
}

void idle() {
  noTone(piezo); // turn off sound immediately
  delay(50);
}


