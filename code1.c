    /*
  HN3806-AB-Rotary-Encoder
  modified on 12 oct 2020
  by Amir Mohamad Shojaee @ Electropeak
  Home

  Based on electricdiylab.com Example
*/

//This variable will increase or decrease depending on the rotation of encoder
volatile long x , counter = 0; 
    
void setup() {
  Serial.begin (9600);
  pinMode(2, INPUT_PULLUP); 
  
  pinMode(3, INPUT_PULLUP); 
   
  attachInterrupt(0, A, RISING);
   
  attachInterrupt(1, B, RISING);
  }
   
  void loop() {
  // Send the value of counter
  if( counter != x ){
  Serial.println (counter);
  x = counter;
  }
  }
   
  void A() {
  if(digitalRead(3)==LOW) {
  counter++;
  }else{
  counter--;
  }
  }
   
  void B() {
  if(digitalRead(2)==LOW) {
  counter--;
  }else{
  counter++;
  }
}

\\\\\\\\\\\\\\\\\\\\
Forth version of the Arduino sketch for reading a rotary encoder with AB 2-phase rectangular orthogonal pulse output:
\\\\\\\\\\\\\\\\\\\\\


\ This variable will increase or decrease depending on the rotation of encoder
variable counter 0

: setup
\ Set up serial communication at 9600 baud
9600 9600 baud
\ Configure pin 2 and 3 as inputs with pull-up resistors
2 input
3 input
\ Set up external interrupts on pin 0 and 1
0 A
1 B
;

: loop
\ Send the value of counter over serial
counter . cr
;

: A
\ Check the state of pin 3
3 invert and
\ If pin 3 is low, increment counter
if
1 counter +!
then
;

: B
\ Check the state of pin 2
2 invert and
\ If pin 2 is low, decrement counter
if
1 counter -!
then
;

\\\\\\\\\\\\\\\\\\
