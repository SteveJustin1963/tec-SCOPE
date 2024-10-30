// Digispark Attiny85

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/atomic.h>

// Define encoder pins for two encoders
#define ENCODER1_A PB0  // Encoder 1 A channel
#define ENCODER1_B PB1  // Encoder 1 B channel
#define ENCODER2_A PB3  // Encoder 2 A channel (formerly USB-)
#define ENCODER2_B PB2  // Encoder 2 B channel and also used as SCK for SPI (formerly USB+)

// Define SPI pin
#define MISO PB4        // MISO on PB4

// Error codes
#define ERR_INVALID_CMD 0xFF

// Counter limits
#define MAX_COUNT 0x7FFFFFFF
#define MIN_COUNT -0x80000000

// Debounce configuration
#define DEBOUNCE_TIME 1000  // microseconds

// Encoder counters (32-bit signed for bidirectional counting)
volatile int32_t counter1 = 0;
volatile int32_t counter2 = 0;
volatile uint8_t prev_state1 = 0;
volatile uint8_t prev_state2 = 0;

// SPI communication state
volatile uint8_t spi_command = 0;
volatile uint8_t byte_counter = 0;  // Track which byte we're sending
volatile int32_t * volatile active_counter = NULL;  // Double volatile for pointer safety

// Status tracking
volatile struct {
    uint8_t spi_errors;
    uint8_t overflow_count;
    uint8_t last_command;
} status = {0, 0, 0};

// Timing variables for debounce
volatile uint32_t last_interrupt_time = 0;

// Function to get microseconds since startup
// Note: This is a simplified implementation and may need adjustment based on your clock settings
volatile uint32_t micros_counter = 0;
uint32_t micros(void) {
    uint32_t mc;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        mc = micros_counter;
    }
    return mc;
}

// Timer0 interrupt for microsecond counting
ISR(TIM0_OVF_vect) {
    micros_counter += 256; // Assuming 1MHz clock, each tick is 1µs
}

void setup_timer(void) {
    // Setup Timer0 for microsecond counting
    TCCR0A = 0;
    TCCR0B = (1 << CS00); // No prescaling
    TIMSK |= (1 << TOIE0); // Enable overflow interrupt
}

void setup_spi(void) {
    DDRB |= (1 << MISO);             // Set MISO as output
    USICR = (1 << USIWM0) | (1 << USICS1) | (1 << USIOIE); // Enable USI in SPI slave mode with interrupt
}

void setup_encoders(void) {
    // Set encoder pins as inputs with pull-ups
    DDRB &= ~((1 << ENCODER1_A) | (1 << ENCODER1_B) | 
              (1 << ENCODER2_A) | (1 << ENCODER2_B));
    PORTB |= ((1 << ENCODER1_A) | (1 << ENCODER1_B) | 
              (1 << ENCODER2_A) | (1 << ENCODER2_B)); // Enable internal pull-up resistors
    
    // Enable pin change interrupts on encoder pins
    GIMSK |= (1 << PCIE);            // Enable pin change interrupt
    PCMSK |= (1 << ENCODER1_A) | (1 << ENCODER1_B) |
             (1 << ENCODER2_A) | (1 << ENCODER2_B);  // Interrupt on A and B channels for both encoders
}

// SPI interrupt handler using USI for ATtiny85
ISR(USI_OVF_vect) {
    uint8_t spi_data = USIDR;         // Read received data
    int32_t counter_value;
    
    if (byte_counter == 0) {          // New command received
        status.last_command = spi_data;
        
        if (spi_data != 0x01 && spi_data != 0x02) {
            USIDR = ERR_INVALID_CMD;
            status.spi_errors++;
            return;
        }
        
        spi_command = spi_data;
        if (spi_command == 0x01) {    // Command to send counter1
            active_counter = &counter1;
        } else if (spi_command == 0x02) { // Command to send counter2
            active_counter = &counter2;
        }
        
        if (active_counter) {
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                counter_value = *active_counter;
            }
            USIDR = (counter_value >> 24) & 0xFF; // Send MSB
            byte_counter = 1;
        }
    } else {
        // Send remaining bytes of the active counter
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            counter_value = *active_counter;
        }
        
        switch (byte_counter) {
            case 1:
                USIDR = (counter_value >> 16) & 0xFF;
                break;
            case 2:
                USIDR = (counter_value >> 8) & 0xFF;
                break;
            case 3:
                USIDR = counter_value & 0xFF;
                byte_counter = 0;     // Reset byte counter for next transfer
                active_counter = NULL;
                return;
        }
        byte_counter++;
    }

    USISR |= (1 << USIOIF);           // Clear interrupt flag to prepare for the next transfer
}

// Quadrature encoder ISR for pin change interrupt
ISR(PCINT0_vect) {
    uint32_t interrupt_time = micros();
    if (interrupt_time - last_interrupt_time < DEBOUNCE_TIME) {
        return;
    }
    last_interrupt_time = interrupt_time;

    // Encoder 1
    uint8_t curr_state1 = ((PINB & (1 << ENCODER1_A)) ? 1 : 0) | 
                         ((PINB & (1 << ENCODER1_B)) ? 2 : 0);
    uint8_t transition1 = (prev_state1 << 2) | curr_state1;

    // Encoder 2
    uint8_t curr_state2 = ((PINB & (1 << ENCODER2_A)) ? 1 : 0) | 
                         ((PINB & (1 << ENCODER2_B)) ? 2 : 0);
    uint8_t transition2 = (prev_state2 << 2) | curr_state2;

    // Update counters using quadrature state transitions for Encoder 1
    switch (transition1) {
        case 0b0001:
        case 0b0111:
        case 0b1110:
        case 0b1000:
            if (counter1 < MAX_COUNT) {
                counter1++;
            } else {
                status.overflow_count++;
            }
            break;
        case 0b0010:
        case 0b0100:
        case 0b1101:
        case 0b1011:
            if (counter1 > MIN_COUNT) {
                counter1--;
            } else {
                status.overflow_count++;
            }
            break;
    }

    // Update counters using quadrature state transitions for Encoder 2
    switch (transition2) {
        case 0b0001:
        case 0b0111:
        case 0b1110:
        case 0b1000:
            if (counter2 < MAX_COUNT) {
                counter2++;
            } else {
                status.overflow_count++;
            }
            break;
        case 0b0010:
        case 0b0100:
        case 0b1101:
        case 0b1011:
            if (counter2 > MIN_COUNT) {
                counter2--;
            } else {
                status.overflow_count++;
            }
            break;
    }

    // Store current states
    prev_state1 = curr_state1;
    prev_state2 = curr_state2;
}

int main(void) {
    cli();                     // Disable global interrupts during setup
    
    // Enable watchdog timer
    wdt_enable(WDTO_250MS);
    
    setup_timer();            // Initialize timer for debouncing
    setup_spi();             // Initialize SPI
    setup_encoders();        // Initialize encoders
    
    sei();                   // Enable global interrupts
    
    while (1) {
        wdt_reset();         // Reset watchdog timer
        // Main loop is empty as everything is handled by interrupts
    }

    return 0;
}
