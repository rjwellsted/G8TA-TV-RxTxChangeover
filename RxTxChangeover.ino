/*
   M0YDH/M0RNW Rx/Tx Changeover for G8TA DATV

   Date       Revision  Notes
   11-08-2022 V0.1      First attempt
   07-02-2023 V1.0      Initial live version
*/

// Pin definitions
// inputs
const byte PTT_IN = A0;
const byte DECODE_4m = A1;
const byte DECODE_2m = A2;
const byte DECODE_70cm = A3;
const byte DECODE_23cm = A4;


// outputs
const byte PORT_1 = 2;
const byte PORT_2 = 3;
const byte PORT_3 = 4;
const byte PORT_4 = 5;
const byte PORT_5 = 6;
const byte PORT_6 = 7;
const byte RELAY_PULSE = 13;
const byte PTT_RX = 8;
const byte PTT_4m = 9;
const byte PTT_2m = 10;
const byte PTT_70cm = 11;
const byte PTT_23cm = 12;
const byte RX_LED = A5;

// timings
const int SETTLING_TIME = 100;
const int RELAY_STROBE = 250;
const int PTT_B_DELAY = 1000;

// variables
bool pttActive;

void setup() {
  // put your setup code here, to run once:
  // configure outputs
  pinMode(RELAY_PULSE, OUTPUT);
  digitalWrite(RELAY_PULSE, LOW);
  pinMode(PORT_1, OUTPUT);
  digitalWrite(PORT_1, HIGH); // Rx input
  pinMode(PORT_2, OUTPUT);
  digitalWrite(PORT_2, HIGH); // 4m output
  pinMode(PORT_3, OUTPUT);
  digitalWrite(PORT_3, HIGH); // 2m output
  pinMode(PORT_4, OUTPUT);
  digitalWrite(PORT_4, HIGH); // 70cm output
  pinMode(PORT_5, OUTPUT);
  digitalWrite(PORT_5, HIGH); // 23cm output
  pinMode(PORT_6, OUTPUT);
  digitalWrite(PORT_6, HIGH); // spare
  pinMode(PTT_RX, OUTPUT);
  digitalWrite(PTT_RX, LOW);
  pinMode(PTT_4m, OUTPUT);
  digitalWrite(PTT_4m, LOW);
  pinMode(PTT_2m, OUTPUT);
  digitalWrite(PTT_2m, LOW);
  pinMode(PTT_70cm, OUTPUT);
  digitalWrite(PTT_70cm, LOW);
  pinMode(PTT_23cm, OUTPUT);
  digitalWrite(PTT_23cm, LOW);
  pinMode(PTT_RX, OUTPUT);
  digitalWrite(PTT_RX, HIGH);   // turn on RX LED

  // configure inputs
  pinMode(PTT_IN, INPUT_PULLUP);
  pinMode(DECODE_2m, INPUT_PULLUP);
  pinMode(DECODE_4m, INPUT_PULLUP);
  pinMode(DECODE_70cm, INPUT_PULLUP);
  pinMode(DECODE_23cm, INPUT_PULLUP);

  pttActive = false;
  EnableRX();
  allHigh();
}

void loop() {
  // put your main code here, to run repeatedly:
  // band select goes active first then the PTT
  if ((digitalRead(PTT_IN) == LOW) && !pttActive){
    pttActive = true;
    // deactive RX
    digitalWrite(PTT_RX, HIGH);
    digitalWrite(RX_LED, LOW);   // turn off RX LED
    // select required port and enable the correct PTT
    if (DECODE_23cm == LOW) {
      allHigh();
      digitalWrite(PORT_5, LOW);
      pulseRelay();
      digitalWrite(PTT_4m, HIGH);
      digitalWrite(PTT_2m, HIGH);
      digitalWrite(PTT_70cm, HIGH);
      digitalWrite(PTT_23cm, LOW);
    } else if (DECODE_70cm == LOW) {
      allHigh();
      digitalWrite(PORT_4, LOW);
      pulseRelay();
      digitalWrite(PTT_4m, HIGH);
      digitalWrite(PTT_2m, HIGH);
      digitalWrite(PTT_70cm, LOW);
      digitalWrite(PTT_23cm, HIGH);      
    } else if (DECODE_2m == LOW){
      allHigh();
      digitalWrite(PORT_3, LOW);
      pulseRelay();
      digitalWrite(PTT_4m, HIGH);
      digitalWrite(PTT_2m, LOW);
      digitalWrite(PTT_70cm, HIGH);
      digitalWrite(PTT_23cm, HIGH);
    } else if (DECODE_4m == LOW) {
      allHigh();
      digitalWrite(PORT_2, LOW);
      pulseRelay();
      digitalWrite(PTT_4m, LOW);
      digitalWrite(PTT_2m, HIGH);
      digitalWrite(PTT_70cm, HIGH);
      digitalWrite(PTT_23cm, HIGH);
    }
  } else if ((digitalRead(PTT_IN) == HIGH) && pttActive) {
    pttActive = false;
    allHigh();
    pulseRelay();
    EnableRX();
  }
}

void allHigh() {
  digitalWrite(PORT_1, HIGH);
  digitalWrite(PORT_2, HIGH);
  digitalWrite(PORT_3, HIGH);
  digitalWrite(PORT_4, HIGH);
  digitalWrite(PORT_5, HIGH);
  digitalWrite(PORT_6, HIGH);
  digitalWrite(PTT_4m, HIGH);
  digitalWrite(PTT_2m, HIGH);
  digitalWrite(PTT_70cm, HIGH);
  digitalWrite(PTT_23cm, HIGH);  
  pulseRelay();
}

void EnableRX() {
  // Set the antennna relay to Rx
  digitalWrite(PORT_1, LOW);
  pulseRelay();
  allHigh();
  pulseRelay();
  digitalWrite(PTT_RX, LOW);
  digitalWrite(RX_LED, HIGH);
}
void pulseRelay() {
  delay(SETTLING_TIME);
  digitalWrite(RELAY_PULSE, HIGH);
  delay(RELAY_STROBE);
  digitalWrite(RELAY_PULSE, LOW);
  delay(SETTLING_TIME);
}
