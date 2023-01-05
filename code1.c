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
