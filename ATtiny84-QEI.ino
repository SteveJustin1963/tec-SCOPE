#include <avr/io.h>
#include <avr/interrupt.h>

// ATtiny84

// Define encoder pins
#define ENCODER1_A PA0
#define ENCODER1_B PA1
#define ENCODER2_A PA2
#define ENCODER2_B PA3

// Define SPI pins
#define MISO PB1
#define MOSI PA6
#define SCK  PA4
#define SS   PA5

// SPI Commands
#define CMD_READ_COUNTER1 0x01
#define CMD_READ_COUNTER2 0x02
#define CMD_RESET_COUNTER1 0x03
#define CMD_RESET_COUNTER2 0x04

// Debounce configuration
#define DEBOUNCE_TIMEOUT 10    // Minimum microseconds between valid transitions
#define COUNTER_MAX 1000000000 // Reset threshold for relative position tracking

// Encoder counters and state
volatile int32_t counter1 = 0;
volatile int32_t counter2 = 0;
volatile uint8_t prev_state1 = 0;
volatile uint8_t prev_state2 = 0;

// Debounce tracking
volatile uint16_t last_interrupt_time1 = 0;
volatile uint16_t last_interrupt_time2 = 0;

// SPI communication state
volatile uint8_t spi_command = 0;
volatile uint8_t byte_counter = 0;
volatile int32_t *active_counter = NULL;
volatile uint8_t spi_transaction_active = 0;

// Timer for debounce timing
void setup_timer0() {
    // Configure Timer0 for microsecond timing
    TCCR0A = (1 << WGM01);  // CTC mode
    TCCR0B = (1 << CS01);   // Prescaler = 8
    OCR0A = 199;            // For 16MHz clock: 16MHz/8/200 = 10kHz (100μs resolution)
    TIMSK0 |= (1 << OCIE0A);
}

volatile uint16_t microseconds = 0;
ISR(TIM0_COMPA_vect) {
    microseconds++;
}

void setup_spi() {
    DDRB |= (1 << MISO);
    DDRA &= ~((1 << MOSI) | (1 << SCK) | (1 << SS));
    PORTA |= (1 << SS);  // Enable pull-up on SS
    SPCR = (1 << SPE) | (1 << SPIE);
    
    // Configure SS pin change interrupt
    PCMSK1 |= (1 << PCINT13);  // SS pin
    GIMSK |= (1 << PCIE1);     // Enable PCINT1 vector
}

void setup_encoders() {
    DDRA &= ~((1 << ENCODER1_A) | (1 << ENCODER1_B) | 
              (1 << ENCODER2_A) | (1 << ENCODER2_B));
    PORTA |= ((1 << ENCODER1_A) | (1 << ENCODER1_B) | 
              (1 << ENCODER2_A) | (1 << ENCODER2_B));
    
    PCMSK0 |= (1 << PCINT0) | (1 << PCINT1) | 
              (1 << PCINT2) | (1 << PCINT3);
    GIMSK |= (1 << PCIE0);
}

// Handle SS pin changes
ISR(PCINT1_vect) {
    if (PINA & (1 << SS)) {  // SS went high (deselected)
        byte_counter = 0;     // Reset SPI state
        spi_transaction_active = 0;
        active_counter = NULL;
    } else {                  // SS went low (selected)
        spi_transaction_active = 1;
    }
}

ISR(SPI_STC_vect) {
    if (!spi_transaction_active) return;  // Ignore if SS is high
    
    uint8_t spi_data = SPDR;
    
    if (byte_counter == 0) {
        spi_command = spi_data;
        switch(spi_command) {
            case CMD_READ_COUNTER1:
                active_counter = &counter1;
                SPDR = (*active_counter >> 24) & 0xFF;
                byte_counter = 1;
                break;
            case CMD_READ_COUNTER2:
                active_counter = &counter2;
                SPDR = (*active_counter >> 24) & 0xFF;
                byte_counter = 1;
                break;
            case CMD_RESET_COUNTER1:
                counter1 = 0;
                SPDR = 0;
                break;
            case CMD_RESET_COUNTER2:
                counter2 = 0;
                SPDR = 0;
                break;
            default:
                SPDR = 0xFF;  // Invalid command
                break;
        }
    } else {
        // Handle multi-byte counter reading
        switch(byte_counter) {
            case 1:
                SPDR = (*active_counter >> 16) & 0xFF;
                break;
            case 2:
                SPDR = (*active_counter >> 8) & 0xFF;
                break;
            case 3:
                SPDR = *active_counter & 0xFF;
                byte_counter = 0;
                active_counter = NULL;
                return;
        }
        byte_counter++;
    }
}

// Optimized encoder state lookup table
static const int8_t encoder_states[] = {
    0,  1, -1,  0,
    -1,  0,  0,  1,
    1,  0,  0, -1,
    0, -1,  1,  0
};

// Overflow handling function
static inline void handle_counter_overflow(volatile int32_t *counter) {
    if (*counter >= COUNTER_MAX) {
        *counter -= COUNTER_MAX;
    } else if (*counter <= -COUNTER_MAX) {
        *counter += COUNTER_MAX;
    }
}

ISR(PCINT0_vect) {
    // Temporarily disable SPI interrupts for prioritization
    uint8_t old_spcr = SPCR;
    SPCR &= ~(1 << SPIE);
    
    uint16_t current_time = microseconds;
    
    // Read current states
    uint8_t curr_state1 = ((PINA & (1 << ENCODER1_A)) ? 1 : 0) | 
                         ((PINA & (1 << ENCODER1_B)) ? 2 : 0);
    uint8_t curr_state2 = ((PINA & (1 << ENCODER2_A)) ? 1 : 0) | 
                         ((PINA & (1 << ENCODER2_B)) ? 2 : 0);
    
    // Debounce and update encoder 1
    if ((current_time - last_interrupt_time1) >= DEBOUNCE_TIMEOUT) {
        uint8_t transition1 = (prev_state1 << 2) | curr_state1;
        counter1 += encoder_states[transition1];
        handle_counter_overflow(&counter1);
        prev_state1 = curr_state1;
        last_interrupt_time1 = current_time;
    }
    
    // Debounce and update encoder 2
    if ((current_time - last_interrupt_time2) >= DEBOUNCE_TIMEOUT) {
        uint8_t transition2 = (prev_state2 << 2) | curr_state2;
        counter2 += encoder_states[transition2];
        handle_counter_overflow(&counter2);
        prev_state2 = curr_state2;
        last_interrupt_time2 = current_time;
    }
    
    // Restore SPI interrupt state
    SPCR = old_spcr;
}

int main(void) {
    cli();
    setup_timer0();
    setup_spi();
    setup_encoders();
    sei();
    
    while (1) {
        // Main loop can implement power saving or other features
        // For now, just idle waiting for interrupts
    }
    
    return 0;
}
