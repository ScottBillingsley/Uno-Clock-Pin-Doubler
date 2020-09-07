/*                 Uno, Nano Clock Pin Doubler
                   Copyright (c) 2020 Vernon Billingsley


    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission
    notice shall be included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/
/**************************** Defines *****************************/
#define DEBUG 0

#if DEBUG == 1
#define dprint(expression) Serial.print("# "); Serial.print( #expression ); Serial.print( ": " ); Serial.println( expression )
#define dshow(expression) Serial.println( expression )
#else
#define dprint(expression)
#define dshow(expression)
#endif

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define TOGGLE_CLK1 PIND |= _BV(PIND2)  //Toggle D2 for debug

#define CLK 9
#define NOT_CLK 8


void setup() {
  if (DEBUG) {
    Serial.begin(115200);
  }
  pinMode(CLK, OUTPUT);
  pinMode(NOT_CLK, OUTPUT);

  cli();                //stop interrupts
  //set timer1
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  /*Set timer one to toggle output pin 9 on comapre */
  sbi(TCCR1A, COM1A0);
  //OCR1A = ((16000000 / (prescale * hZ)) - 1) * 2
  /********************************************/
  OCR1A = 1000; /* Change to desired freq     */
  /********************************************/
  // turn on CTC
  cbi(TCCR1B, WGM13);
  sbi(TCCR1B, WGM12);
  cbi(TCCR1B, WGM11);
  cbi(TCCR1B, WGM10);
  // Set 1 prescaler
  sbi(TCCR1B, CS10);
  cbi(TCCR1B, CS11);
  cbi(TCCR1B, CS12);
  // disanable timer compare interrupt
  cbi(TIMSK1, OCIE1A);

  /*Setup the comparator */
  cbi(DDRD, PD6); /*AIN0 + NANO D6*/
  cbi(DDRD, PD7); /*AIN1 - NAN0 D7*/
  /*Disable the digital input buffer */
  sbi(DIDR1, AIN1D);
  sbi(DIDR1, AIN0D);
  /*Enable the comarator */
  cbi(ACSR, ACD);
  /*Set interrupt mode to toggle*/
  cbi(ACSR, ACIS1);
  cbi(ACSR, ACIS0);
  /*Set AIN0 + input to bandgap */
  sbi(ACSR, ACBG);
  /*Enable the interrupt */
  sbi(ACSR, ACIE);
  sei();                //allow interrupts

}

ISR(ANALOG_COMP_vect) {
  if (!(PINB & _BV (1))) {  //Read state of pin 9
    PORTB |= _BV (0);       //If pin 9 is LOW set pin 8 HIGH
  } else {
    PORTB &= ~_BV (0);      //If pin 9 is HIGH set pin 8 LOW
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
