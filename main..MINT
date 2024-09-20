// 
9 p! 		// 0x09 CONSTANT p

: I		//INIT ( -- )
  0 c ! 	// Initialize the pulse count to zero
  0 g ! 	// Initialize the degrees to zero
  0 m ! 	// Initialize the minutes to zero
;

// Routine to read the up & down clock signals from the latch
: R		//READ-PULSES ( -- )
  p C@ 		// Read the byte from the latch
  " 1& 0=(c+)(c-)	// upcomming feature 
			//Check bit 0 (UpClk), increment if high0
  			// Check bit 1 (DnClk), decrement if high
;

// Routine to calculate the angle in degrees & minutes
: C				//CALC-ANGLE ( -- )
  c @ 600 /MOD 			// 600 pulses per revolution
  $ g ! 			// Store the degrees
  60 * m ! 			// Convert remaining pulses to minutes & store
;

// Routine to display the angle
: D				//DISPLAY-ANGLE ( -- )
  g @ . ." degrees "
  m @ . ." minutes "
;

// Main routine
: M		//MAIN ( -- )
  I
  100(
    R		//READ-PULSES
    C		//CALC-ANGLE
    D		//DISPLAY-ANGLE
  )
;

// Start the program
M		//MAIN
