
const uint8_t LED_PIN = 13;
const uint8_t LED2_PIN = 12;
const uint8_t METER_PIN = A4;
const uint8_t BTN_PIN = 2;

//button press and LED state need to be set to volatile bool 
  //so the system doesnt use cached data and only what is the 
  //current status of the button or LED
volatile bool led2State = false;
volatile bool buttonPressed = false;

//initializing with a value that the potentiometer won't produce
int previousMeterReading = -1; 

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(METER_PIN, INPUT);
  //using internal pullup resistor
  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(LED2_PIN, OUTPUT);
  //attaching an interrupt to the button pin, 
  //triggering the btnISR when the signal does CHANGE, meaning while the button is pushed down
  attachInterrupt(digitalPinToInterrupt(BTN_PIN), btnISR, CHANGE);
  Serial.begin(9600);
  
}


void loop()
{ 
  if (buttonPressed) {
    //toggle lED when button is pressed
    led2State = !led2State;
    //write new status
    digitalWrite(LED2_PIN, led2State);
    //reset button status
    buttonPressed = false;
  }
  setTimerFromPotentiometer();
}

void setTimerFromPotentiometer(){
  //read from potentiometer
  int meterReading = analogRead(METER_PIN);
  
  // Check if the reading has changed
  if(meterReading != previousMeterReading) {
    // Update the previous reading
    previousMeterReading = meterReading;

    //map the potentiometer reading to a frequency range
    double freq = map(meterReading, 0, 1023, 1, 4);
  
    //print for debugging
    Serial.print("frequency: ");
    Serial.println(freq);
 
    //set timer to value from meter
    startTimer(freq);
  }
}

void startTimer(double timerFrequency){
  noInterrupts();
  //calculate comparator value from frequency value
  uint16_t cmpr = 16000000/(1024 * timerFrequency) - 1;
  
  //8-bit timer controllers registers reset
  //TCCR1A control input interrupts
  //TCCR1B control output interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  
  //timer count reset
  TCNT1 = 0;
  
  //setting this bit to one in the timer control b register
  //selects mode of operation for the counter
  //clears timer after reaching the comparator number
  TCCR1B |= (1 << WGM12);
  
  //set prescaler to 1024
  TCCR1B |= (1 << CS12) | (1 << CS10);
  
  //this is the comparator value to pulse for 2 seconds before we trigger interrupt
  OCR1A = cmpr;
  
  //status bit to enable interrupts in our system
  TIMSK1 = (1 << OCIE1A);
  //---
  
  interrupts();
}

ISR(TIMER1_COMPA_vect){
   digitalWrite(LED_PIN, digitalRead(LED_PIN) ^ 1);
}
    
void btnISR(){
  	//setting button state to true in ISR
	    buttonPressed = true;
  		Serial.println("Button pressed");
}
    


//the clock computes at 16mhz so it produces a lot of pulses. 
//1 pulse in 1/16,000,000 second
//building a time interrupt every 2 seconds means it has to count 
//the pulses that are happening at the speed
//means 32 million counts


//this is where we can use prescalar eg. 1024 to count a pulse
//after 1024 pulses occur.

//2x16000000/1024 = 31250

//if our register has 2^16 we can count to 2 seconds (31250 pulses with prescalar)
//and it would reset the counter before it hits the 2^16 limit


