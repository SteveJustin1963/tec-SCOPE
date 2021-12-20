
//**********************************************
// Config your sensor pins and location here!
//**********************************************


// Define the number of pulses that your encoder (1 and 2) gives by turn, and multiply by 4
// In my case: 600 x 15 x 4  (600 pulses by turn; 15 is the gear ratio), so:
long pulses_enc1 = 36000;              
long pulses_enc2 = 36000;             

// Define DUE's pins
#define enc_1A 2   // define DUE pin to encoder 1-channel A                     
#define enc_1B 3   // define DUE pin to encoder 1-channel B  
#define enc_2A 4   // define DUE pin to encoder 2-channel A  
#define enc_2B 5   // define DUE pin to encoder 2-channel B  

// enter your latitude (example: North 40º33'20'')
int latHH = 40;    // this means 40º North
int latMM = 33;
int latSS = 20;

// enter Pole Star right ascention (AR: HH:MM:SS)
int poleAR_HH = 2;    // this means 2 hours, 52 minutes and 16 seconds
int poleAR_MM = 52;
int poleAR_SS = 16;

// enter Pole Star hour angle (H: HH:MM:SS)
int poleH_HH = 8;
int poleH_MM = 32;
int poleH_SS = 7;

