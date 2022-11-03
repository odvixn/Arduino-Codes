int counter = 0;  // similar to frameCount, but repeats from 0 ~ 199
int outputPattern = 0;  // 3 different output(LED) patterns
int brightness = 0;  // brightness of blue LED; used in glow()
int brightnessControl = 1; // increment or decrement brightness
int speedMultiplier = 1; // affect LED pattern changing speed. ex) flash faster
bool lightMode = false; // binary state of LED. used in blink() and alternate()
bool prevSwitchState = false; // used in manageOutputPattern() to turn temporary button state to permanent one

// IO
int ledRED = 2;
int ledBLUE = 5;
int powerSwitch = A1;
int potentiometer = A2;

// the setup routine runs once when you press reset:
void setup() {
  pinMode(ledRED, OUTPUT);
  pinMode(ledBLUE, OUTPUT);
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 2:
  int sensorValue = analogRead(potentiometer);
  delay(25/speedMultiplier);  // controls loop speed
  speedMultiplier = sq(map(sensorValue,0,1024,1,5)); // speed controlled by potentiometer analog value

  switch (outputPattern) { // Different LED light patterns
    case 0:
      glow();
      break;
    case 1:
      blink();
      break;
    case 2:
      alternate();
      break;
  }

  manageOutputPattern();
  counter = (counter + 1) % 200;
}

void manageOutputPattern() {  // button press -> change output pattern
  bool switchState = digitalRead(powerSwitch);
  if (switchState && !prevSwitchState) {  // !prevSwitchState ensures unique ON signal
    outputPattern = (outputPattern + 1) % 3;
    counter = 0;
    allOff();
  }
  prevSwitchState = switchState;
}

void blink() { // both turn on&off at the same time
  if (lightMode) {
    digitalWrite(ledRED, HIGH);
    digitalWrite(ledBLUE, HIGH);
  } else {
    digitalWrite(ledRED, LOW);
    digitalWrite(ledBLUE, LOW);
  }
  if (counter < 100 && counter % 25 == 0) {  // flashing pattern;
    lightMode = !lightMode;
  }
}

void alternate() { // alternate lighting
  if (lightMode) {
    digitalWrite(ledRED, HIGH);
    digitalWrite(ledBLUE, LOW);
  } else {
    digitalWrite(ledRED, LOW);
    digitalWrite(ledBLUE, HIGH);
  }
  if (counter % 100 == 0) {
    lightMode = !lightMode;
  }
}

void glow() { // only blue LED; gradually increase and decrease brightness
  brightness += brightnessControl*2; // faster brightness control
  if (brightness <= 0 || brightness >= 254) {
    brightnessControl *= -1;
  }
  analogWrite(ledBLUE, brightness);
}

void allOff() { // turn all LED off
  digitalWrite(ledRED, LOW);
  digitalWrite(ledBLUE, LOW);
}
