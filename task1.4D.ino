const int Motionsens = 2;       
const int Sound = 7;             
const int sensTilt = 3;         
const int Motionsens2 = 4;      
int led = 13;                      
int blueLed = 11;                  
const int greenLed = PB4;          

uint8_t motionState, tiltState, motionState2 = 0;

uint8_t premotion, premotion1, premotion2 = 0;


const uint16_t timer = 0;          
const uint16_t compare = 31250;    

void setup() {
  Serial.begin(9600);    
  
  pinMode(Sound, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(sensTilt, INPUT_PULLUP);
  pinMode(Motionsens, INPUT);
  pinMode(Motionsens2, INPUT);
  DDRB |= (1 << greenLed);         
  
  PCICR |= 0b00000111;
  PCMSK2 |= 0b10011100;
  
  TCCR1A = 0;
  TCCR1B |= (1 << CS12);         
  TCCR1B &= ~(1 << CS11);
  TCCR1B &= ~(1 << CS10);
  
  TCNT1 = timer;                    
  OCR1A = compare;                  
  
  TIMSK1 = (1 << OCIE1A);           
  
  sei();
}

void loop() {
  if (premotion != motionState) {
    digitalWrite(led, motionState);
    Serial.println("Motion Sensor 1 working, RED ALERT");
    premotion = motionState;
  }
  
  if (premotion1 != motionState2) {
    digitalWrite(blueLed, motionState2);
    Serial.println("Motion Sensor 2 Working, BLUE ALERT");
    premotion1 = motionState2;
  }

  if (premotion2 != tiltState) {
    digitalWrite(Sound, tiltState);
    Serial.println("Tilt Sensor working, Buzz Buzz Buzz!!!!");
    premotion2 = tiltState;
  }
  
  delay(200); 
}


ISR(PCINT2_vect) {
  motionState = PINB & B00010000;   
  motionState2 = PINB & B00000100;  
  tiltState = PINB & B00001000;     
}

ISR(TIMER1_COMPA_vect) {
  TCNT1 = timer;
  PORTB ^= (1 << greenLed);
}
