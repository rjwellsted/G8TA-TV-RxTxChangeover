/****************************************************************************
 *
 * Copyright(c) 2022-2023, Ron Wellsted M0RNW. All rights reserved.
 *           http://m0rnw.uk   mailto: ron@m0rnw.uk
 * --------------------------------------------------------------------------
 * This file is part of The G8TA-DATV project, which is free software. You may redistribute
 * and/or modify it under the terms of the GNU General Public License,
 * version 3 or later, as published by the Free Software Foundation.
 *      G8TA-DATV project is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY, not even the implied warranty of MERCHANTABILITY.
 * See the GNU General Public License for specific details.
 *      By using G8TA-DATV project, you warrant that you have read, understood and
 * agreed to these terms and conditions, and that you possess the legal
 * right and ability to enter into this agreement and to use G8TA-DATV project
 * in accordance with it.
 *      Your G8TA-DATV project distribution files should contain the file LICENSE,
 * an ascii text copy of the GNU General Public License, version 3.
 * If not, point your browser to  http://www.gnu.org/licenses/
 * or write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330,  Boston, MA 02111-1307 USA.
 * --------------------------------------------------------------------------
 * M0YDH/M0RNW Rx/Tx Changeover for G8TA-DATV
 */

//#define DEBUG

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
#ifdef DEBUG
  Serial.begin(9600);
  Serial.write("Start setup, Setting up COAX relay\n");
#endif
  // configure outputs
  pinMode(RELAY_PULSE, OUTPUT);  
  digitalWrite(RELAY_PULSE, LOW);
  pinMode(PORT_1, OUTPUT);
  digitalWrite(PORT_1, HIGH); // Rx enable
  pinMode(PORT_2, OUTPUT);
  digitalWrite(PORT_2, HIGH); // 4m enable
  pinMode(PORT_3, OUTPUT);
  digitalWrite(PORT_3, HIGH); // 2m enable
  pinMode(PORT_4, OUTPUT);
  digitalWrite(PORT_4, HIGH); // 70cm enable
  pinMode(PORT_5, OUTPUT);
  digitalWrite(PORT_5, HIGH); // 23cm enable
  pinMode(PORT_6, OUTPUT);
  digitalWrite(PORT_6, HIGH); // spare
  digitalWrite(RELAY_PULSE, HIGH);
  delay(RELAY_STROBE);
  digitalWrite(RELAY_PULSE, LOW);
#ifdef DEBUG
  Serial.write("COAX relay setup completed, setting up PTT lines\n");
#endif
  pinMode(PTT_RX, OUTPUT);
  digitalWrite(PTT_RX, HIGH);
  pinMode(PTT_4m, OUTPUT);
  digitalWrite(PTT_4m, HIGH);
  pinMode(PTT_2m, OUTPUT);
  digitalWrite(PTT_2m, HIGH);
  pinMode(PTT_70cm, OUTPUT);
  digitalWrite(PTT_70cm, HIGH);
  pinMode(PTT_23cm, OUTPUT);
  digitalWrite(PTT_23cm, HIGH);
  pinMode(PTT_RX, OUTPUT);
  digitalWrite(PTT_RX, HIGH);
  pinMode(RX_LED,OUTPUT);
  digitalWrite(RX_LED, HIGH);   // turn on RX LED

#ifdef DEBUG
  Serial.write("PTT Lines setup, setting up inputs\n");
#endif
  // configure inputs
  pinMode(PTT_IN, INPUT_PULLUP);
  pinMode(DECODE_2m, INPUT_PULLUP);
  pinMode(DECODE_4m, INPUT_PULLUP);
  pinMode(DECODE_70cm, INPUT_PULLUP);
  pinMode(DECODE_23cm, INPUT_PULLUP);

#ifdef DEBUG
Serial.write("Inputs setup, set inital state\n");
#endif

  pttActive = false;
  allHigh();
  EnableRX();
}

void loop() {
  // put your main code here, to run repeatedly:
  // band select goes active first then the PTT
  if ((digitalRead(PTT_IN) == LOW) && !pttActive){    
#ifdef DEBUG
    Serial.write("PTT Activated\n");
#endif
    pttActive = true;
    // deactive RX
    digitalWrite(PTT_RX, HIGH);
    digitalWrite(RX_LED, LOW);   // turn off RX LED
    // select required port and enable the correct PTT
    if (digitalRead(DECODE_23cm) == LOW) {
#ifdef DEBUG
      Serial.write("23cm Active\n");
#endif            
      allHigh();
      digitalWrite(PORT_5, LOW);
      pulseRelay();
      digitalWrite(PTT_4m, HIGH);
      digitalWrite(PTT_2m, HIGH);
      digitalWrite(PTT_70cm, HIGH);
      digitalWrite(PTT_23cm, LOW);
    } else if (digitalRead(DECODE_70cm) == LOW) {
#ifdef DEBUG
      Serial.write("70cm Active\n");
#endif            
      allHigh();
      digitalWrite(PORT_4, LOW);
      pulseRelay();
      digitalWrite(PTT_4m, HIGH);
      digitalWrite(PTT_2m, HIGH);
      digitalWrite(PTT_70cm, LOW);
      digitalWrite(PTT_23cm, HIGH);      
    } else if (digitalRead(DECODE_2m) == LOW){
#ifdef DEBUG
      Serial.write("2m Active\n");
#endif            
      allHigh();
      digitalWrite(PORT_3, LOW);
      pulseRelay();
      digitalWrite(PTT_4m, HIGH);
      digitalWrite(PTT_2m, LOW);
      digitalWrite(PTT_70cm, HIGH);
      digitalWrite(PTT_23cm, HIGH);
    } else if (digitalRead(DECODE_4m) == LOW) {
#ifdef DEBUG
      Serial.write("4m Active\n");
#endif            
      allHigh();
      digitalWrite(PORT_2, LOW);
      pulseRelay();
      digitalWrite(PTT_4m, LOW);
      digitalWrite(PTT_2m, HIGH);
      digitalWrite(PTT_70cm, HIGH);
      digitalWrite(PTT_23cm, HIGH);
    }
  } else if ((digitalRead(PTT_IN) == HIGH) && pttActive) {
#ifdef DEBUG
    Serial.write("PTT deactivated\n");
#endif
    pttActive = false;
    allHigh();
    EnableRX();
  }
}

void allHigh() {
#ifdef DEBUG
  Serial.write("allHigh\n");
#endif
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
#ifdef DEBUG
  Serial.write("enableRX\n");
#endif
  digitalWrite(PORT_1, LOW);
  pulseRelay();
  allHigh();
  digitalWrite(PTT_RX, LOW);
  digitalWrite(RX_LED, HIGH);
}
void pulseRelay() {  
#ifdef DEBUG
  Serial.write("pulseRelay\n");
#endif
  delay(SETTLING_TIME);
  digitalWrite(RELAY_PULSE, HIGH);
  delay(RELAY_STROBE);
  digitalWrite(RELAY_PULSE, LOW);
  delay(SETTLING_TIME);
}
