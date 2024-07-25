//arduino ports
int sensorPin = A0;   
int redPin = 11;
int bluePin = 10;
int greenPin = 9;

//set up arduino on startup
void setup() {
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

//code that runs on loop
void loop() {
  //reads value from temperature sensor
  int sensorValue = analogRead(sensorPin);
  Serial.print('Analog value: ');
  Serial.println(sensorValue);

  if (sensorValue < 143) {
    setColour(0, 0, 255); //blue
  } else if (sensorValue < 223) {
    setColour(0, 255, 0); //green
  } else {
    setColour(255, 0, 0); //red
  }
  //loop after 1 second
  delay(1000);
}

//define setcolour method
//parameters from the if statement 
void setColour(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}
