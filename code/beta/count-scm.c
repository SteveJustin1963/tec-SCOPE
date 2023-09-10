 "simple_capacitance_meter.ino"
  Code by lingib
  https://www.instructables.com/member/lingib/instructables/
  Last update 2 January 2023

  ----------
  ABOUT
  ----------
  This Arduino code allows you to measure the value of unknown capacitors.

  Two series connected capacitors form a voltage divider.
  The unknown capacacitance value is the voltage ratio multiplied by the reference capacitance

  The meter has two ranges
    - the LOW range is for capacitance values below 1uF
    - the HIGH range is for capacitance values above 1uF

  ----------
  COPYRIGHT
  ----------
  This code is free software: you can redistribute it and/or
  modify it under the terms of the GNU General Public License as published
  by the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This software is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License. If
  not, see <http://www.gnu.org/licenses/>.
***************************************************************************/

// ----- Arduino pinouts
const int   OUTPUT_pin = A2;
const int   INPUT_pin = A1;
const int   GND_pin = A0;

// ----- Measurements
/* C2 = C1*(1023 - Adc)/Adc - Cstray */
float C1 = 9554;                            // pF
float C2 = 0.0;                             // unknown capacitance
float Cstray = 37.0;                        // open circuit reading during calibration
int Adc = 0;                                // Analog to digital converter reading

// ----- Serial
signed long Baud = 115200;

// -----------------
//  setup()
// -----------------
void setup() {

  // ----- Serial connection
  Serial.begin(Baud);
}

// -----------------
//  loop()
// -----------------
void loop() {

  // ----- Discharge all capacitors
  pinMode(OUTPUT_pin, OUTPUT);
  pinMode(INPUT_pin, OUTPUT);
  pinMode(GND_pin, OUTPUT);  
  digitalWrite(OUTPUT_pin, LOW);
  digitalWrite(INPUT_pin, LOW);
  digitalWrite(GND_pin, LOW);
  delay(500);

  // ----- Charge the capacitors
  pinMode(INPUT_pin, INPUT);
  digitalWrite(OUTPUT_pin, HIGH);
  delay(100);

  // ----- Take reading
  Adc = analogRead(INPUT_pin);
  C2 = C1 * (1023 - Adc) /Adc - Cstray;     // pF

  // ----- Display results
  Serial.print(Adc);
  Serial.print("   ");
  Serial.print(C2); Serial.print(" pF   ");
  Serial.print(C2 / 1000);  Serial.print(" nF   ");
  Serial.print(C2 / 1000000);  Serial.println(" uF");

}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; convert to c code
;note that this code might not work as expected as it is missing 
; some C library function such as pinMode, digitalWrite, analogRead, delay. 
; You may need to include additional libraries or write your own functions to perform these operations.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define OUTPUT_pin 2
#define INPUT_pin 1
#define GND_pin 0

float C1 = 9554;                            // pF
float C2 = 0.0;                             // unknown capacitance
float Cstray = 37.0;                        // open circuit reading during calibration
int Adc = 0;                                // Analog to digital converter reading

signed long Baud = 115200;

int main(){

    // ----- Serial connection
    printf("Serial connection started\n");

    // ----- Discharge all capacitors
    pinMode(OUTPUT_pin, OUTPUT);
    pinMode(INPUT_pin, OUTPUT);
    pinMode(GND_pin, OUTPUT);  
    digitalWrite(OUTPUT_pin, LOW);
    digitalWrite(INPUT_pin, LOW);
    digitalWrite(GND_pin, LOW);
    delay(500);

    // ----- Charge the capacitors
    pinMode(INPUT_pin, INPUT);
    digitalWrite(OUTPUT_pin, HIGH);
    delay(100);

    // ----- Take reading
    Adc = analogRead(INPUT_pin);
    C2 = C1 * (1023 - Adc) /Adc - Cstray;     // pF

    // ----- Display results
    printf("%d   %f pF   %f nF   %f uF\n", Adc, C2, C2 / 1000, C2 / 1000000);

    return 0;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

\\\\\\\\\\\\\\\\\\\
forth 83



\ Serial connection
." Serial connection started" cr

\ Discharge all capacitors
2 output !
1 output !
0 output !
500 ms

\ Charge the capacitors
1 input !
2 1 !
100 ms

\ Take reading
1 @adc !
C1 1023 @adc - @adc / * Cstray - C2 !

\ Display results
@adc @ . cr
C2 @ . ." pF" cr
C2 1000 / @ . ." nF" cr
C2 1000000 / @ . ." uF" cr
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
another forth

: pinMode ( pin# -- )
  2dup dup GND_pin = if 0 else
  dup INPUT_pin = if 1 else
  dup OUTPUT_pin = if 2 else
  drop drop
;

: digitalWrite ( pin# value# -- )
  ...
;

: delay ( time-in-ms# -- )
  ...
;

: discharge-all-capacitors ( -- )
  OUTPUT_pin pinMode
  INPUT_pin pinMode
  GND_pin pinMode
  OUTPUT_pin 0 digitalWrite
  INPUT_pin 0 digitalWrite
  GND_pin 0 digitalWrite
  500 delay
;




\\\\\\\\\\\\\\\\\\\\\\\
;code converted to use integers only:
;this code may not run as expected 
; not defined are the functions; pinMode, digitalWrite and analogRead which come from an arduino library.
;;;;;;;;;;;;;;

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define OUTPUT_pin 2
#define INPUT_pin 1
#define GND_pin 0

int C1 = 9554;                            // pF
int C2 = 0;                             // unknown capacitance
int Cstray = 37;                        // open circuit reading during calibration
int Adc = 0;                                // Analog to digital converter reading

signed long Baud = 115200;

int main(){

    // ----- Serial connection
    printf("Serial connection started\n");

    // ----- Discharge all capacitors
    pinMode(OUTPUT_pin, OUTPUT);
    pinMode(INPUT_pin, OUTPUT);
    pinMode(GND_pin, OUTPUT);  
    digitalWrite(OUTPUT_pin, LOW);
    digitalWrite(INPUT_pin, LOW);
    digitalWrite(GND_pin, LOW);
    delay(500);

    // ----- Charge the capacitors
    pinMode(INPUT_pin, INPUT);
    digitalWrite(OUTPUT_pin, HIGH);
    delay(100);

    // ----- Take reading
    Adc = analogRead(INPUT_pin);
    C2 = (C1 * (1023 - Adc))/Adc - Cstray;     // pF

    // ----- Display results
    printf("%d   %d pF   %d nF   %d uF\n", Adc, C2, C2 / 1000, C2 / 1000000);

    return 0;
}
\\\\\\\\\\\\\\\\\\\\\\\\\
