///////////////////////////////////////////

// sudo mint code

// Constants and Port Definitions

// Constants for SPI communication with LS7366R
#10 SPI_DATA_IN_PORT !   // MISO - SPI Data In
#11 SPI_DATA_OUT_PORT !  // MOSI - SPI Data Out
#12 SPI_CLOCK_PORT !     // SCK - SPI Clock
#13 SPI_CS_PORT !        // CS - Chip Select

// Constants for AM9511 FPU (MPU) communication
#10 DATA_PORT !          // FPU Data port
#11 COMMAND_PORT !       // FPU Command port
#12 STATUS_PORT !        // FPU Status port

// LS7366R Commands
#88 WRITE_MDR0 !         // Write to MDR0
#90 WRITE_MDR1 !         // Write to MDR1
#20 CLR_CNTR !           // Clear counter
#98 WRITE_DTR !          // Write to DTR (Data Transfer Register)
#E0 LOAD_CNTR !          // Load CNTR from DTR
#60 READ_CNTR !          // Read counter

// FPU Command for addition
#6C SADD !               // Command for addition
#80 BUSY !               // Busy flag for FPU
#1E ERROR_MASK !         // Error mask for FPU

// Variables
:RESULT 0 0 ;            // 16-bit result storage for FPU
:ERROR 0 ;               // 8-bit error storage

////////////////////////////////////////////////////////

// SPI Functions for LS7366R

:SPI_CLOCK_HIGH
  1 SPI_CLOCK_PORT !
;
:SPI_CLOCK_LOW
  0 SPI_CLOCK_PORT !
;
:SPI_CS_ENABLE
  0 SPI_CS_PORT !
;
:SPI_CS_DISABLE
  1 SPI_CS_PORT !
;

// Send a byte via SPI (MOSI)
:SEND_SPI_BYTE
  :arg1 !              // Store byte to send
  SPI_CS_ENABLE        // Enable the SPI device
  8 0 DO               // Loop for 8 bits
    arg1 0x80 & IF     // Check the MSB of arg1
      1 SPI_DATA_OUT_PORT !
    ELSE
      0 SPI_DATA_OUT_PORT !
    THEN
    SPI_CLOCK_HIGH     // Pulse the clock high
    SPI_CLOCK_LOW      // Pulse the clock low
    arg1 1 LSHIFT !    // Shift arg1 left by 1 bit
  LOOP
  SPI_CS_DISABLE       // Disable the SPI device
;

// Receive a byte via SPI (MISO)
:READ_SPI_BYTE
  0 result !          // Clear result register
  SPI_CS_ENABLE       // Enable SPI device
  8 0 DO              // Loop for 8 bits
    SPI_CLOCK_HIGH    // Pulse clock high
    SPI_CLOCK_LOW     // Pulse clock low
    SPI_DATA_IN_PORT @ 0x01 & IF   // Read MISO
      result 1 LSHIFT OR result !  // Store the bit
    ELSE
      result 1 LSHIFT !            // Shift result left by 1
    THEN
  LOOP
  SPI_CS_DISABLE      // Disable the SPI device
;

/////////////////////////////////////////////////////////

// LS7366R Configuration and Counter Operations

// Initialize LS7366R configuration

:INIT_LS7366R
  WRITE_MDR0 0x03 SINGLE_BYTE_WRITE   // Set MDR0: x4 quadrature, free-running, disable index
  WRITE_MDR1 0x00 SINGLE_BYTE_WRITE   // Set MDR1: 4-byte counter, counting enabled, no flags
;

// Clear the counter
:CLEAR_COUNTER
  CLR_CNTR LOAD_RESET_REGISTER        // Clear the CNTR register
;

// Load a specific value into the counter
:LOAD_COUNTER
  :value !                            // Store 32-bit value to load
  WRITE_DTR value 3 >> & 0xFF SINGLE_BYTE_WRITE   // Write high byte
  WRITE_DTR value 2 >> & 0xFF SINGLE_BYTE_WRITE   // Write next byte
  WRITE_DTR value 1 >> & 0xFF SINGLE_BYTE_WRITE   // Write next byte
  WRITE_DTR value & 0xFF SINGLE_BYTE_WRITE        // Write low byte
  LOAD_CNTR LOAD_RESET_REGISTER                    // Transfer value from DTR to CNTR
;

// Read the 32-bit counter value
:READ_COUNTER
  READ_CNTR LOAD_RESET_REGISTER         // Send read counter command
  0 result !                            // Clear result
  READ_SPI_BYTE                         // Read high byte
  result 24 LSHIFT OR !                 // Shift to high position
  READ_SPI_BYTE                         // Read next byte
  result 16 LSHIFT OR !                 // Shift to next position
  READ_SPI_BYTE                         // Read next byte
  result 8 LSHIFT OR !                  // Shift to next position
  READ_SPI_BYTE                         // Read low byte
  result OR !                           // Combine all bytes into result
;


///////////////////////////////////////////////////////

// FPU (AM9511A MPU) Functions

// Initialize arguments for FPU operations
:INIT_FPU
  1 a !          // Set arg1 to 1
  1 b !          // Set arg2 to 1
;

// Store arguments in FPU
:STORE_FPU
  a DATA_PORT /O      // Push arg1 lower byte
  a } DATA_PORT /O    // Push arg1 higher byte
  b DATA_PORT /O      // Push arg2 lower byte
  b } DATA_PORT /O    // Push arg2 higher byte
;

// Send add command to FPU
:ADD_FPU
  SADD COMMAND_PORT /O
;

// Wait until FPU is not busy
:WAIT_FPU
  /U ( STATUS_PORT /I BUSY & /W )
;

// Check and store any error code from FPU
:ERROR_FPU
  STATUS_PORT /I ERROR_MASK & { ERROR !
;

// Pop result from FPU and store it
:POP_FPU
  DATA_PORT /I l !      // Get lower byte
  DATA_PORT /I h !      // Get higher byte
  h 8 { l + RESULT !    // Combine bytes and store in RESULT
;

////////////////////////////////////////////////////////

// Main Program Sequence

:START
  INIT_LS7366R         // Initialize LS7366R configuration for quadrature counting
  CLEAR_COUNTER        // Clear the LS7366R counter to start at zero
  INIT_FPU             // Initialize FPU arguments for operations
  
  // If you want to load a specific value to the counter, use LOAD_COUNTER with value as an argument
  
  READ_COUNTER         // Read the current counter value from LS7366R
  STORE_FPU            // Store initialized values in FPU
  ADD_FPU              // Perform addition operation on FPU
  WAIT_FPU             // Wait for FPU operation to complete
  ERROR_FPU            // Check and store any errors from FPU
  POP_FPU              // Retrieve result from FPU into RESULT variable
  /U ( /K )            // Infinite loop for key input or further operations
;

///////////////////////////////////////////////////////////



