/*
 * File:   main.c
 * Author: baranbologur,yunusemrekeles,mustafaumuteyidogan
 *
 * Created on May 25, 2024, 4:03 PM
 */

#include "pragmas.h"
#include <xc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// enumeration for command types
typedef enum {
    CMD_GOO,
    CMD_END,
    CMD_SPD,
    CMD_ALT,
    CMD_MAN,
    CMD_LED,
    CMD_UNKNOWN
} CommandType;
// enumeration for operation modes
typedef enum {
    IDLE,
    ACTIVE,
    END
} OperationMode;

#define START_CHAR '$'
#define END_CHAR '#'
//enumeration for message packet state
typedef enum {
    PKT_WAIT_HDR,
    PKT_GET_BODY,
    PKT_WAIT_ACK
} pkt_state_t;

pkt_state_t pkt_state;


typedef enum {INBUF = 1, OUTBUF = 0} buf_t; // buffer type
#define BUF_SIZE 128
typedef struct {
    char buffer[BUF_SIZE];
    int head;
    int tail;
} CircularBuffer;  //buffer structure

// all the global variables we need to use

CircularBuffer input, output;

char packet[BUF_SIZE];
int pkt_body_size;
int packet_is_proccessing;

int distance, speed;
int alt_period, alt_counter;

int rb4_prev, rb5_prev, rb6_prev, rb7_prev; // rb ports previous states
int rb4_pressed_flag, rb5_pressed_flag, rb6_pressed_flag, rb7_pressed_flag;
int rb4mode, rb5mode, rb6mode, rb7mode; // led command turns on these variables so we can only check for the correct buttons interrupt

int manualflag = 0; // manual mode flag in order to check while sending button press responses

OperationMode operation_mode;
// function prototypes
void int_to_hex_and_push(int num);
void write_string_to_output_buffer(char *str);
void disable_sc(void) { PIE1bits.RC1IE = 0; PIE1bits.TX1IE = 0;}
void enable_sc(void)  { PIE1bits.RC1IE = 1; PIE1bits.TX1IE = 1;}
void initialize_buffer(buf_t buf_type);
int buffer_is_empty (buf_t buf_type);
void buffer_push (char c, buf_t buf_type);
char buffer_pop (buf_t buf_type);
void create_packet();
int char_to_int(char c);
int hex_string_to_int(char* packet, int size);
void evaluate_packet(void);
void set_message(void);
void send_message(void);
void clear_packet(void);
void initialize_variables(void);
void initialize(void);
CommandType get_command_type(char *command);

// function for buffer initialization
void initialize_buffer(buf_t buf_type) {
    if(buf_type == INBUF){
        input.head = 0;
        input.tail = 0;
        for(int i = 0; i < BUF_SIZE; i++) {
            input.buffer[i] = 0;
        }
    }
    else{
        output.head = 0;
        output.tail = 0;
        for(int i = 0; i < BUF_SIZE; i++) {
            output.buffer[i] = 0;
        }
    }
    return;
}

// function for checking if buffer is empty or not
int buffer_is_empty (buf_t buf_type) { 
    if(buf_type == INBUF){
        return (input.head == input.tail) ? 1 : 0; 
    }
    else{
        return (output.head == output.tail) ? 1 : 0; 
    }
}
// function for char pushing to the buffer
void buffer_push (char c, buf_t buf_type){
    
    
    if(buf_type == INBUF){
        input.buffer[input.head++] = c;
        if(input.head == BUF_SIZE) input.head = 0;
        if(input.head == input.tail){
            ;
        }
    }
    else{
        output.buffer[output.head++] = c;
        if(output.head == BUF_SIZE) output.head = 0;
        if(output.head == output.tail){
            ;
        }
    }
    
    return;
}
// function for buffer pop
char buffer_pop (buf_t buf_type){
    
    
    char result = '\0';
    if(buf_type == INBUF){
        if(buffer_is_empty(INBUF)){
            ;
        }
        result = input.buffer[input.tail];
        input.tail++;
        if(input.tail == BUF_SIZE) input.tail = 0;
    }
    else{
        if(buffer_is_empty(OUTBUF)){
            ;
        }
        result = output.buffer[output.tail];
        output.tail++;
        if(output.tail == BUF_SIZE) output.tail = 0;
    }
    
    
    
    return result;
}
// function for creating message packets
void create_packet(void){
    if(!buffer_is_empty(INBUF)){
        if(pkt_state == PKT_WAIT_HDR){
            char c = buffer_pop(INBUF);
            if (c == START_CHAR){
                pkt_body_size = 0;
                clear_packet();
                pkt_state = PKT_GET_BODY;
            }
            else{
                ;
            }
        }
        else if(pkt_state == PKT_GET_BODY){
            char c = buffer_pop(INBUF);
            if (c == START_CHAR){
                ;
            }
            else if (c == END_CHAR){
                packet_is_proccessing = 1;
                pkt_state = PKT_WAIT_ACK;
            }
            else{
                packet[pkt_body_size++] = c;
            }
        }
        else if(pkt_state == PKT_WAIT_ACK){
            if(packet_is_proccessing == 0){
                pkt_state = PKT_WAIT_HDR;
            }
        }
    }
    return;
}
// function which converts char to int
int char_to_int(char c){
    if('0' <= c && c <= '9'){
        return c - '0';
    }
    if('A' <= c && c <= 'F'){
        return c - 'A' + 10;
    }
    if('a' <= c && c <= 'f'){
        return c - 'a' + 10;
    }
    else{
        ;
    }
    return -1;
}
// function for converting hex string to the integer
int hex_string_to_int(char* packet, int size){
    int result = 0;
    for(int i = 3; i < 3 + size; i++){
        result += char_to_int(packet[i]);
        if(i != 3 + size - 1){
            result *= 16;
        }
    }
    return result;
}
// function which returns command type information
CommandType get_command_type(char *command) {
    if (strcmp(command, "GOO") == 0) return CMD_GOO;
    if (strcmp(command, "END") == 0) return CMD_END;
    if (strcmp(command, "SPD") == 0) return CMD_SPD;
    if (strcmp(command, "ALT") == 0) return CMD_ALT;
    if (strcmp(command, "MAN") == 0) return CMD_MAN;
    if (strcmp(command, "LED") == 0) return CMD_LED;
    return CMD_UNKNOWN;
}
// function for message packet evaluation, first it gets the command type then does the required operations on the variables
void evaluate_packet(void){
    if(!packet_is_proccessing) return;
    
    
    char command[4];
    for(int i = 0; i < 3; i++){
        command[i] = packet[i];
    }
    command[3] = '\0';
    
    CommandType command_type = get_command_type(command);
    
    switch(command_type){
        case CMD_GOO:
            TMR0L = 0xF7; // Start value is set to 49911, so that it overflows after 15625 instructions
            TMR0H = 0xC2;
            INTCONbits.TMR0IE = 1;   //Timer 0 interrupt is enabled
            operation_mode = ACTIVE;
            distance = hex_string_to_int(packet, 4);
            break;
        case CMD_END:
            operation_mode = END;
            break;
        case CMD_SPD:
            speed = hex_string_to_int(packet, 4);
            break;
        case CMD_ALT:
            alt_period = hex_string_to_int(packet, 4);
            break;
        case CMD_MAN:
            ;
            int open_rbs = hex_string_to_int(packet, 2);
            if(open_rbs == 0) {
                INTCONbits.RBIE = 0;
                manualflag = 0;
                LATAbits.LA0 = 0;
                LATBbits.LB0 = 0;
                LATCbits.LC0 = 0;
                LATDbits.LD0 = 0;
                rb7mode = 0;
                rb6mode = 0;
                rb5mode = 0;
                rb4mode = 0;
            }
            else if(open_rbs == 1) {
                INTCONbits.RBIE = 1;
                manualflag = 1;
            }
            break;
        case CMD_LED:
            if (manualflag == 0) break;
            int led_number = hex_string_to_int(packet, 2);
            if(led_number == 0){
                LATAbits.LA0 = 0;
                LATBbits.LB0 = 0;
                LATCbits.LC0 = 0;
                LATDbits.LD0 = 0;
                rb7mode = 0;
                rb6mode = 0;
                rb5mode = 0;
                rb4mode = 0;
            }
            else if(led_number == 4){
                LATAbits.LA0 = 1;
                rb7mode=1;
                rb7_prev = PORTBbits.RB7;
            }
            else if(led_number == 3){
                LATBbits.LB0 = 1;
                rb6mode=1;
                rb6_prev = PORTBbits.RB6;
            }
            else if(led_number == 2){
                LATCbits.LC0 = 1;
                rb5mode=1;
                rb5_prev = PORTBbits.RB5;
            }
            else if(led_number == 1){
                LATDbits.LD0 = 1;
                rb4mode=1;
                rb4_prev = PORTBbits.RB4;
            }
            else{
                ;
            }
            break;
        case CMD_UNKNOWN:
            ;
            break;
    }
    
    packet_is_proccessing = 0;
    return;
}

// function for erasing message packets between messages
void clear_packet(void){
    for(int i = 0; i < BUF_SIZE; i++){
        packet[i] = 0;
    }
    return;
}
// initialization function for variables and ports
void initialize_variables(void) {
    initialize_buffer(INBUF);
    initialize_buffer(OUTBUF);
    
    pkt_body_size = 0;
    clear_packet();
    packet_is_proccessing = 0;
    
    pkt_state = PKT_WAIT_HDR;
    
    distance = 0;
    speed = 0;
    rb4_pressed_flag = 0;
    rb5_pressed_flag = 0;
    rb6_pressed_flag = 0;
    rb7_pressed_flag = 0;
    rb4mode = 0;
    rb5mode = 0;
    rb6mode = 0;
    rb7mode = 0;
    operation_mode = IDLE;
    
    alt_period = 0;
    alt_counter = 0;
    
    LATB = 0x00;
    TRISB = 0xF0;
    rb4_prev = 0;
    rb5_prev = 0;
    rb6_prev = 0;
    rb7_prev = 0;
    
    LATAbits.LA0 = 0;
    TRISAbits.RA0 = 0;
    
    LATBbits.LB0 = 0;
    TRISBbits.RB0 = 0;
    
    LATCbits.LC0 = 0;
    TRISCbits.RC0 = 0;
    
    LATDbits.LD0 = 0;
    TRISDbits.RD0 = 0;
    return;
}

// function for register configurations

void initialize(void){
    // ------------ Serial Communication Part Start ------------
    // configure I/O ports 
    TRISC = 0x00;
    TXSTA1=0x00;
    RCSTA1=0x00;
    BAUDCON1 = 0x00;
    PIE1 = 0x00;
    INTCON = 0x00;
    PIR1 = 0x00;
    IPR1 = 0x00;
    ADCON1 = 0x00;
    
    
    TRISCbits.RC7 = 1; // TX1 and RX1 pin configuration
    TRISCbits.RC6 = 0;
    // configure USART transmitter/receiver 
	//TXSTA1 = 0x04;      // (= 00000100) 8-bit transmit, transmitter NOT enabled,TXSTA1.TXEN not enabled!
                        // asynchronous, high speed mode
	//RCSTA1 = 0x90;      // (= 10010000) 8-bit receiver, receiver enabled,
                        // continuous receive, serial port enabled RCSTA.CREN = 1
    SPBRG1 = 0x56 ;// for 40 MHz, to have 115200 baud rate, it should be 86
    SPBRGH1 = 0x00 ;
    BAUDCON1bits.BRG16 = 1;
    TXSTA1bits.SYNC = 0;
    RCSTA1bits.SPEN = 1;
	PIE1bits.RC1IE = 1;	// enable USART receive interrupt
    RCSTA1bits.RX9 = 0;
    RCSTA1bits.CREN = 1;
    TXSTA1bits.TX9 = 0;
    TXSTA1bits.TXEN = 0;
    TXSTA1bits.BRGH = 1;
    SPBRG1 = 0x56 ;// for 40 MHz, to have 115200 baud rate, it should be 86
    SPBRGH1 = 0x00 ;
    
    // configure the interrupts 
    INTCON = 0;			// clear interrupt register completely
	PIE1bits.TX1IE = 1;	// enable USART transmit interrupt
	PIR1 = 0;			// clear all peripheral flags
    // ------------ Serial Communication Part End --------------
	
    
    // ------------ ADC Part Start -----------------------------
    // Set ADC Inputs
    TRISH = 0x10; // AN12 input RH4
    // Configure ADC
    ADCON0 = 0x31; // Channel 12; Turn on AD Converter
    ADCON1 = 0x00; // All analog pins
    ADCON2 = 0xAA; // Right Align | 12 Tad | Fosc/32 
    ADRESH = 0x00; // Result registers are cleared
    ADRESL = 0x00;
    
    // configure the interrupts 
    PIE1bits.ADIE = 1;	 // Enables the A/D interrupt
    IPR1bits.ADIP = 1;   // A/D interrupt has high priority 
    // ------------ ADC Part End -------------------------------
    
    // ------------ Timer 0 Part Start -------------------------
    // configure the timer 
    
    T0CON = 0b10000101; // Timer 0 is enabled, 16-bit, 64 pre-scale
    TMR0L = 0xF7; // Start value is set to 49911, so that it overflows after 15625 instructions
    TMR0H = 0xC2;
    // configure the interrupts 
    INTCONbits.TMR0IE = 0;   //Timer 0 interrupt is enabled
    //INTCON2bits.TMR0IP = 1;  //Timer 0 interrupt has high priority 
    // ------------ Timer 0 Part End ---------------------------
    // configure the interrupts 
    //INTCONbits.RBIE = 1; //Enable RB interrupt 
    INTCONbits.RBIF = 0;
            
    // start the interrupts 
    RCONbits.IPEN = 0; //Disable low priority interrupts
	INTCONbits.PEIE = 1;// enable peripheral interrupts
	INTCONbits.GIE = 1;	// globally enable interrupts
    return;
}
// function for converting integer to hexadecimal string
void int_to_hex_and_push(int num) { 
    char hexStr[9];
    snprintf(hexStr, sizeof(hexStr), "%04X", num);
    char *ptr = hexStr;
    while (*ptr) {
        buffer_push(*ptr++, OUTBUF);
    }
}
// function for put string into the buffer
void write_string_to_output_buffer(char *str) {
    while (*str) {
        buffer_push(*str, OUTBUF);
        str++;
    }
}
// function which is called at every 100ms and sends the required message
// in this design altitude message has higher priority then button press response but it does not matter since both of them will not happen at the same time
void set_message(void){
    distance -= speed;
    int message_sent = 0;
    if(alt_period != 0){
        alt_counter += 100; // for altitude message period calculation
        if(alt_counter == alt_period){
            buffer_push(START_CHAR, OUTBUF);
            char start_string[4] = "ALT";
            write_string_to_output_buffer(start_string);
            GODONE = 1; // ADC started here
            message_sent = 1; // flag set
            alt_counter = 0; // counter reset
        }
    }
    else alt_counter = 0;
    if(!message_sent && rb4_pressed_flag){
        char string[8] = "$PRS04#";
        write_string_to_output_buffer(string);
        message_sent = 1;
        TXSTA1bits.TXEN = 1;
        rb4_pressed_flag = 0;
    }
    if(!message_sent && rb5_pressed_flag){
        char string[8] = "$PRS05#";
        write_string_to_output_buffer(string);
        message_sent = 1;
        TXSTA1bits.TXEN = 1;
        rb5_pressed_flag = 0;
    }
    if(!message_sent && rb6_pressed_flag){
        char string[8] = "$PRS06#";
        write_string_to_output_buffer(string);
        message_sent = 1;
        TXSTA1bits.TXEN = 1;
        rb6_pressed_flag = 0;
    }
    if(!message_sent && rb7_pressed_flag){
        char string[8] = "$PRS07#";
        write_string_to_output_buffer(string);
        message_sent = 1;
        TXSTA1bits.TXEN = 1;
        rb7_pressed_flag = 0;
    }
    if(!message_sent){
        char string[5] = "$DST";
        write_string_to_output_buffer(string);
        int_to_hex_and_push(distance);
        buffer_push(END_CHAR, OUTBUF);
        message_sent = 1;
        TXSTA1bits.TXEN = 1;
    }
    return;
}
// Interrupt service routine
void __interrupt (high_priority) highPriorityISR (void) {
    if (INTCONbits.TMR0IF == 1){ // Timer 0 interrupted
        INTCONbits.TMR0IF = 0;
        TMR0L = 0xF7; // Start value is set to 49911, so that it overflows after 15625 instructions
        TMR0H = 0xC2;
        if(operation_mode == ACTIVE){
            set_message();
        }
    } 
    if (INTCONbits.RBIF == 1){ // RB<4:7> interrupted
        INTCONbits.RBIF = 0;
        if(rb4_prev != PORTBbits.RB4 && rb4mode){ // modela andle
            rb4_prev = PORTBbits.RB4;
            if(PORTBbits.RB4 == 1){
                rb4_pressed_flag = 1;
            }
        }
        if(rb5_prev != PORTBbits.RB5 && rb5mode ){
            rb5_prev = PORTBbits.RB5;
            if(PORTBbits.RB5 == 1){
                rb5_pressed_flag = 1;
            }
        }
        if(rb6_prev != PORTBbits.RB6 && rb6mode){
            rb6_prev = PORTBbits.RB6;
            if(PORTBbits.RB6 == 1){
                rb6_pressed_flag = 1;
            }
        }
        if(rb7_prev != PORTBbits.RB7 && rb7mode){
            rb7_prev = PORTBbits.RB7;
            if(PORTBbits.RB7 == 1){
                rb7_pressed_flag = 1;
            }
        }
    } 
    if (PIR1bits.ADIF == 1){ // A/D finished is interrupted
        PIR1bits.ADIF = 0;
        unsigned int result = (ADRESH << 8) + ADRESL;
        if(result < 256) int_to_hex_and_push(9000);
        else if(result < 512) int_to_hex_and_push(10000);
        else if(result < 768) int_to_hex_and_push(11000);
        else int_to_hex_and_push(12000);
        buffer_push(END_CHAR, OUTBUF);
        TXSTA1bits.TXEN = 1;
    } 
    if (PIR1bits.TX1IF == 1){ // Transmit finish is interrupted
        PIR1bits.TX1IF = 0;    // Acknowledge interrupt
        // If all bytes are transmitted, turn off transmission
        if (buffer_is_empty(OUTBUF)){ 
            while(!TXSTA1bits.TRMT){
                ;
            } 
            TXSTA1bits.TXEN = 0;
        }
        // Otherwise, send next byte
        else TXREG1 = buffer_pop(OUTBUF);
    } 
    if (PIR1bits.RC1IF == 1){ // Receive start is interrupted
        if(RCSTA1bits.OERR == 1){
            RCSTA1bits.CREN = 0;
            RCSTA1bits.CREN = 1;
        }
        else{
            char c = (char)RCREG1;
            buffer_push(c, INBUF);
        }
        PIR1bits.RC1IF = 0;
    } 
}
// our main function
void main(void) {
    initialize_variables();
    initialize();
    TMR0L = 0xF7; // Start value is set to 49911, so that it overflows after 15625 instructions
    TMR0H = 0xC2;
    pkt_state = PKT_WAIT_HDR;
    while(1){
        if(operation_mode == END) break;
        create_packet();
        evaluate_packet();
    }
    return;
}
