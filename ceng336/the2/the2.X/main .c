// ============================ //
// Do not edit this part!!!!    //
// ============================ //
// 0x300001 - CONFIG1H
#pragma config OSC = HSPLL      // Oscillator Selection bits (HS oscillator,
                                // PLL enabled (Clock Frequency = 4 x FOSC1))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit
                                // (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit
                                // (Oscillator Switchover mode disabled)
// 0x300002 - CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out
                                // Reset disabled in hardware and software)
// 0x300003 - CONFIG1H
#pragma config WDT = OFF        // Watchdog Timer Enable bit
                                // (WDT disabled (control is placed on the SWDTEN bit))
// 0x300004 - CONFIG3L
// 0x300005 - CONFIG3H
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit
                                // (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled;
                                // RE3 input pin disabled)
// 0x300006 - CONFIG4L
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply
                                // ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit
                                // (Instruction set extension and Indexed
                                // Addressing mode disabled (Legacy mode))

#pragma config DEBUG = OFF      // Disable In-Circuit Debugger

#define KHZ 1000UL
#define MHZ (KHZ * KHZ)
#define _XTAL_FREQ (40UL * MHZ)

// ============================ //
//             End              //
// ============================ //

#include <xc.h>

// ============================ //
//        DEFINITIONS           //
// ============================ //

// You can write struct definitions here...

// ============================ //
//          GLOBALS             //
// ============================ //
int interrupt_blink = 0;
//int rb5_toggled = 0;
int rb7_toggled = 0;
int rb6_toggled = 0;
//int b5_prev;
int b6_prev;
int b7_prev;
int timerCounter = 0;
int blink;
int g0_prev ,g4_prev, g2_prev, g3_prev;
int interrupt_fall;
int submit;
int tile_number;
int current_shape[4][8];
int l_shape_phase;
int blink_state;
int table[4][8];
int total_allocation;
int rb7_try, rb6_try;
int rb7_counter, rb6_counter;
// You can write globals definitions here...

// ============================ //
//          FUNCTIONS           //
// ============================ //
void update_7segment();
void initialize(){
    INTCONbits.GIE=0;
    //ADCON1 = 0x0F; // for simulator not necessary for board
    TRISB=0xC0;
    TRISC=0x00;
    TRISD=0x00;
    TRISE=0x00;
    TRISF=0x00;
    TRISG=0xFF;
    TRISH=0x00;
    TRISJ=0x00;
    LATH=0x00;
    LATJ=0x00;
    PORTH=0x00;
    PORTJ=0x00;// for seven segment
    LATB=0x00;
    LATG=0x00;
    T0CON=0x85;
    TMR0H=0x67;TMR0L=0x6A;
    RCON=0x00;
    INTCON=0x68;
    INTCON2=0x80;
    
    interrupt_blink = 0;
    rb6_toggled = 0;
    rb7_toggled = 0;
    b6_prev = 0;
    b7_prev = 0;
    timerCounter = 0;
    interrupt_fall = 0;
    submit = 1;
    tile_number = 0;
    
    
    g0_prev = 0; // right
    g4_prev = 0; // up
    g2_prev = 0; // down
    g3_prev = 0; // left
    
    l_shape_phase = 0;
    blink_state = 1;
    total_allocation = 0;
    
    rb6_try = 0;
    rb7_try = 0;
    rb6_counter = 0;
    rb7_counter = 0;
    
    for(int i = 0; i < 4 ; i++){
        for(int j = 0; j < 8 ; j++){
            current_shape[i][j]=0;
            table[i][j]=0;
        }
    }
    update_7segment();
    INTCONbits.GIE=1;
}

void update_leds(){
    int val=0;
    for(int j = 0 ; j < 8 ;j++){
        if(current_shape[0][j])
            val = blink_state;
        else
            val = table[0][j];
        switch(j){
            case 0:
                if(val)
                    LATCbits.LATC0=1;
                else
                    LATCbits.LATC0=0;
                break;
            case 1:
                if(val)
                    LATCbits.LATC1=1;
                else
                    LATCbits.LATC1=0;
                break;
            case 2:
                if(val)
                    LATCbits.LATC2=1;
                else
                    LATCbits.LATC2=0;
                break;
            case 3:
                if(val)
                    LATCbits.LATC3=1;
                else
                    LATCbits.LATC3=0;
                break;
            case 4:
                if(val)
                    LATCbits.LATC4=1;
                else
                    LATCbits.LATC4=0;
                break;
            case 5:
                if(val)
                    LATCbits.LATC5=1;
                else
                    LATCbits.LATC5=0;
                break;
            case 6:
                if(val)
                    LATCbits.LATC6=1;
                else
                    LATCbits.LATC6=0;
                break;
            case 7:
                if(val)
                    LATCbits.LATC7=1;
                else
                    LATCbits.LATC7=0;
                break;    
        }
        if(current_shape[1][j])
            val = blink_state;
        else
            val = table[1][j];
        switch(j){
            case 0:
                if(val)
                    LATDbits.LATD0=1;
                else
                    LATDbits.LATD0=0;
                break;
            case 1:
                if(val)
                    LATDbits.LATD1=1;
                else
                    LATDbits.LATD1=0;
                break;
            case 2:
                if(val)
                    LATDbits.LATD2=1;
                else
                    LATDbits.LATD2=0;
                break;
            case 3:
                if(val)
                    LATDbits.LATD3=1;
                else
                    LATDbits.LATD3=0;
                break;
            case 4:
                if(val)
                    LATDbits.LATD4=1;
                else
                    LATDbits.LATD4=0;
                break;
            case 5:
                if(val)
                    LATDbits.LATD5=1;
                else
                    LATDbits.LATD5=0;
                break;
            case 6:
                if(val)
                    LATDbits.LATD6=1;
                else
                    LATDbits.LATD6=0;
                break;
            case 7:
                if(val)
                    LATDbits.LATD7=1;
                else
                    LATDbits.LATD7=0;
                break;    
        }
        if(current_shape[2][j])
            val = blink_state;
        else
            val = table[2][j];
        switch(j){
            case 0:
                if(val)
                    LATEbits.LATE0=1;
                else
                    LATEbits.LATE0=0;
                break;
            case 1:
                if(val)
                    LATEbits.LATE1=1;
                else
                    LATEbits.LATE1=0;
                break;
            case 2:
                if(val)
                    LATEbits.LATE2=1;
                else
                    LATEbits.LATE2=0;
                break;
            case 3:
                if(val)
                    LATEbits.LATE3=1;
                else
                    LATEbits.LATE3=0;
                break;
            case 4:
                if(val)
                    LATEbits.LATE4=1;
                else
                    LATEbits.LATE4=0;
                break;
            case 5:
                if(val)
                    LATEbits.LATE5=1;
                else
                    LATEbits.LATE5=0;
                break;
            case 6:
                if(val)
                    LATEbits.LATE6=1;
                else
                    LATEbits.LATE6=0;
                break;
            case 7:
                if(val)
                    LATEbits.LATE7=1;
                else
                    LATEbits.LATE7=0;
                break;    
        }
        if(current_shape[3][j])
            val = blink_state;
        else
            val = table[3][j];
        switch(j){
            case 0:
                if(val)
                    LATFbits.LATF0=1;
                else
                    LATFbits.LATF0=0;
                break;
            case 1:
                if(val)
                    LATFbits.LATF1=1;
                else
                    LATFbits.LATF1=0;
                break;
            case 2:
                if(val)
                    LATFbits.LATF2=1;
                else
                    LATFbits.LATF2=0;
                break;
            case 3:
                if(val)
                    LATFbits.LATF3=1;
                else
                    LATFbits.LATF3=0;
                break;
            case 4:
                if(val)
                    LATFbits.LATF4=1;
                else
                    LATFbits.LATF4=0;
                break;
            case 5:
                if(val)
                    LATFbits.LATF5=1;
                else
                    LATFbits.LATF5=0;
                break;
            case 6:
                if(val)
                    LATFbits.LATF6=1;
                else
                    LATFbits.LATF6=0;
                break;
            case 7:
                if(val)
                    LATFbits.LATF7=1;
                else
                    LATFbits.LATF7=0;
                break;    
        }
    }
}

void update_7segment(){
    int right_digit = total_allocation % 10;
    int left_digit = total_allocation / 10;
    switch(right_digit){
        case 0:
            PORTJ=0x3F;
            break;
        case 1:
            PORTJ=0x06;
            break;
        case 2:
            PORTJ=0x5B;
            break;
        case 3:
            PORTJ=0x4F;
            break;
        case 4:
            PORTJ=0x66;
            break;
        case 5:
            PORTJ=0x6D;
            break;
        case 6:
            PORTJ=0x7D;
            break;
        case 7:
            PORTJ=0x07;
            break;
        case 8:
            PORTJ=0x7F;
            break;
        case 9:
            PORTJ=0x6F;
            break;
                
    }
    PORTHbits.RH3=1;
    __delay_ms(1);
    PORTHbits.RH3=0;
    switch(left_digit){
        case 0:
            PORTJ=0x3F;
            break;
        case 1:
            PORTJ=0x06;
            break;
        case 2:
            PORTJ=0x5B;
            break;
        case 3:
            PORTJ=0x4F;
            break;
        case 4:
            PORTJ=0x66;
            break;
        case 5:
            PORTJ=0x6D;
            break;
        case 6:
            PORTJ=0x7D;
            break;
        case 7:
            PORTJ=0x07;
            break;
        case 8:
            PORTJ=0x7F;
            break;
        case 9:
            PORTJ=0x6F;
            break;
                
    }
    PORTHbits.RH2=1;
    __delay_ms(1);
    PORTHbits.RH2=0;
    PORTJ=0x3F;
    PORTHbits.RH1=1;
    __delay_ms(1);
    PORTHbits.RH1=0;
    PORTHbits.RH0=1;
    __delay_ms(1);
    PORTHbits.RH0=0;
}

void rg0_pressed(){
     //right move
    int can_move = 1;
    for(int j = 0; j < 8 ; j++){
        if(current_shape[3][j] == 1){
            can_move = 0;
            break;
        }
    }
    if(!can_move) return;
    for(int j = 0; j < 8 ; j++){
        for(int i = 2; i >= 0 ; i--){
            if(current_shape[i][j] == 1){
                current_shape[i][j] = 0;
                current_shape[i+1][j] = 1;
            }
        }
    }
    update_leds();
}

void rg4_pressed(){
     // up move
    int can_move = 1;
    for(int i = 0; i < 4 ; i++){
        if(current_shape[i][0] == 1){
            can_move = 0;
            break;
        }
    }
    if(!can_move) return;
    for(int i = 0; i < 4 ; i++){
        for(int j = 1; j < 8 ; j++){
            if(current_shape[i][j] == 1){
                current_shape[i][j] = 0;
                current_shape[i][j-1] = 1;
            }
        }
    }
    update_leds();
}

void rg2_pressed(){
     // down move
    int can_move = 1;
    for(int i = 0; i < 4 ; i++){
        if(current_shape[i][7] == 1){
            can_move = 0;
            break;
        }
    }
    if(!can_move) return;
    for(int i = 0; i < 4 ; i++){
        for(int j = 6; j >= 0 ; j--){
            if(current_shape[i][j] == 1){
                current_shape[i][j] = 0;
                current_shape[i][j+1] = 1;
            }
        }
    }
    update_leds();
}

void rg3_pressed(){
     // left move
    int can_move = 1;
    for(int j = 0; j < 8 ; j++){
        if(current_shape[0][j] == 1){
            can_move = 0;
            break;
        }
    }
    if(!can_move) return;
    for(int j = 0; j < 8 ; j++){
        for(int i = 1; i < 4 ; i++){
            if(current_shape[i][j] == 1){
                current_shape[i][j] = 0;
                current_shape[i-1][j] = 1;
            }
        }
    }
    update_leds();
}

void rb7_pressed(){
     //clockwise rotate
    if(tile_number != 2) return;
    int isDone = 0;
    for(int i = 0; i < 4 ; i++){
        for(int j = 0; j < 8 ; j++){
            if(current_shape[i][j] == 1){
                if(l_shape_phase == 0){
                    current_shape[i][j] = 0;
                    current_shape[i][j+1] = 1;
                }
                else if(l_shape_phase == 1){
                    current_shape[i][j-1] = 1;
                    current_shape[i+1][j-1] = 0;
                }
                else if(l_shape_phase == 2){
                    current_shape[i+1][j] = 1;
                    current_shape[i+1][j+1] = 0;
                }
                else if(l_shape_phase == 3){
                    current_shape[i][j+1] = 0;
                    current_shape[i+1][j+1] = 1;
                }
                isDone = 1;
                break;
            }
        }
        if(isDone) break;
    }
    l_shape_phase = (l_shape_phase + 1) % 4;
    update_leds();
}

void rb6_pressed(){
     //submission request
    for(int i = 0; i < 4 ; i++){
        for(int j = 0; j < 8 ; j++){
            if(current_shape[i][j] == 1 && table[i][j] == 1) return;
        }
    }
    for(int i = 0; i < 4 ; i++){
        for(int j = 0; j < 8 ; j++){
            if(current_shape[i][j] == 1){
                total_allocation++;
                table[i][j] = 1;
            }
        }
    }
    for(int i = 0; i < 4 ; i++){
        for(int j = 0; j < 8 ; j++){
            current_shape[i][j] = 0;
        }
    }
    submit = 1;
    tile_number = (tile_number + 1) % 3;
    update_leds();
}

void blink_curr_shape(){
    update_leds();
}

void checkRGs(){
    if((PORTGbits.RG0 != g0_prev) && g0_prev == 0){
        rg0_pressed(); // right
    }
    g0_prev = PORTGbits.RG0;
    if((PORTGbits.RG4 != g4_prev) && g4_prev == 0){
        rg4_pressed(); // up
    }
    g4_prev = PORTGbits.RG4;
    if((PORTGbits.RG2 != g2_prev) && g2_prev == 0){
        rg2_pressed(); // down
    }
    g2_prev = PORTGbits.RG2;
    if((PORTGbits.RG3 != g3_prev) && g3_prev == 0){
        rg3_pressed(); // left
    }
    g3_prev = PORTGbits.RG3;
}

void create_dot(){
    for(int i = 0; i < 4 ; i++){
        for(int j =0; j < 8 ; j++){
            current_shape[i][j]=0;
        }
    }
    current_shape[0][0]=1;
}

void create_square(){
    for(int i = 0; i < 4 ; i++){
        for(int j =0; j < 8 ; j++){
            current_shape[i][j]=0;
        }
    }
    current_shape[0][0]=1;
    current_shape[0][1]=1;
    current_shape[1][0]=1;
    current_shape[1][1]=1;
}

void create_lshape(){
    l_shape_phase = 0;
    for(int i = 0; i < 4 ; i++){
        for(int j =0; j < 8 ; j++){
            current_shape[i][j]=0;
        }
    }
    current_shape[0][0]=1;
    current_shape[1][0]=1;
    current_shape[1][1]=1;
}

void create_new_tile(){
    switch (tile_number){
        case 0:
            create_dot();
            break;
        case 1:
            create_square();
            break;
        case 2:
            create_lshape();
            break;
    }
    TMR0H=0x67;TMR0L=0x6A; // optional
    timerCounter=0;
    blink_state = 1;
    interrupt_blink = 0;
    update_leds();
}

/*void busy_wait(){
    for(int i = 0; i < 14 ; i++){
        for(int j = 0; j < 252 ; j++){
            for(int k = 0; k < 256 ; k++){
                ;
            }
        }
    }
}*/

int is_game_over(){
    int over=1;
    for(int i=0; i < 4 ; i++){
        for(int j=0;j<8;j++){
            if(table[i][j]==1){continue;}
            switch(tile_number){
                case 2:
                    ;//l_shape
                    int mayover1 = 1;
                    int mayover2 = 1;
                    int mayover3 = 1;
                    int mayover4 = 1;
                    // l_shape
                    if(i!=3 && j!=7 ){
                        if(table[i+1][j] == 0 && table[i+1][j+1] == 0){
                            mayover1=0;
                        }
                    }
                    if(i!=3 && j!=0 ){
                        if(table[i+1][j] == 0 && table[i+1][j-1] == 0){
                            mayover2=0;
                        }
                    }
                    if(i!=3 && j!=7 ){
                        if( table[i][j+1] == 0 && table[i+1][j+1] == 0){
                            mayover3=0;
                        }
                    }
                    if(i!=3 && j!=7 ){
                        if(table[i+1][j] == 0 && table[i][j+1] == 0){
                            mayover4=0;
                        }
                    }
                    if(mayover1 == 0 || mayover2 == 0 || mayover3 == 0 || mayover4 == 0)
                        over=0;
                    break;
                case 0:
                    //dot
                    over=0;
                    break;
                case 1:
                    //square
                    if(i!=3 && j!=7 ){
                        if(table[i+1][j] == 0 && table[i][j+1] == 0 && table[i+1][j+1] == 0){
                            over=0;
                        }
                    }
                    break;
            }
            if(over == 0){
                break;
            }
        }
        if(over == 0){
            break;
        }
    }
    return over;
}
// You can write function definitions here...

// ============================ //
//   INTERRUPT SERVICE ROUTINE  //
// ============================ //
__interrupt(high_priority)
void HandleInterrupt()
{
    // ISR ...
    //INTCONbits.GIE=0;
    if(INTCONbits.TMR0IF == 1){
        timerCounter++;
        interrupt_blink = 1;
        if(timerCounter == 8){
            interrupt_fall = 1;
            timerCounter = 0;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
        }
        INTCONbits.TMR0IF = 0;
        TMR0H=0x67;TMR0L=0x6A;
    }
    if(INTCONbits.RBIF == 1) {
        /*int notpressed = 0;
        for(int i = 0; i < 255; i++){
            if(PORTBbits.RB7 != 1 || b7_prev != 0){
                notpressed = 1;
                break;
            }
        }
        b7_prev = PORTBbits.RB7;
        if(!notpressed){
            rb7_toggled = 1;
            b7_prev = PORTBbits.RB7;
        }
        notpressed = 0;
        for(int i = 0; i < 255; i++){
            if(PORTBbits.RB6 != 1 || b6_prev != 0){
                notpressed = 1;
                break;
            }
        }
        b6_prev = PORTBbits.RB6;
        if(!notpressed){
            rb6_toggled = 1;
            b6_prev = PORTBbits.RB6;
        }*/
        /*if(PORTBbits.RB7 != b7_prev && b7_prev == 0){
            rb7_toggled = 1;
        }
        b7_prev = PORTBbits.RB7;
        if(PORTBbits.RB6 != b6_prev && b6_prev == 0){
            rb6_toggled = 1;
        }
        b6_prev = PORTBbits.RB6;*/
        if(PORTBbits.RB7 != b7_prev){
            rb7_try = 1;
            rb7_counter = 0;
        }
        if(PORTBbits.RB6 != b6_prev){
            rb6_try = 1;
            rb6_counter = 0;
        }
        INTCONbits.RBIF = 0;
    }    
    //INTCONbits.GIE=1;
    
}

void checkRBs(){
    if(rb7_try){
        if(PORTBbits.RB7 != b7_prev){
            rb7_counter++;
            if(rb7_counter == 10){
                if(b7_prev == 0) rb7_toggled = 1;
                rb7_counter = 0;
                rb7_try = 0;
                b7_prev = PORTBbits.RB7;
            }
        }
        else{
            rb7_try = 0;
            rb7_counter = 0;
        }
    }
    else{
        rb7_counter = 0;
    }
    
    if(rb6_try){
        if(PORTBbits.RB6 != b6_prev){
            rb6_counter++;
            if(rb6_counter == 10){
                if(b6_prev == 0) rb6_toggled = 1;
                rb6_counter = 0;
                rb6_try = 0;
                b6_prev = PORTBbits.RB6;
            }
        }
        else{
            rb6_try = 0;
            rb6_counter = 0;
        }
    }
    else{
        rb6_counter = 0;
    }
}

// ============================ //
//            MAIN              //
// ============================ //
void main()
{
    // Main ...
    initialize();
    __delay_ms(1000);
    g0_prev = PORTGbits.RG0;
    g4_prev = PORTGbits.RG4;
    g2_prev = PORTGbits.RG2;
    g3_prev = PORTGbits.RG3;
    b7_prev = PORTBbits.RB7;
    b6_prev = PORTBbits.RB6;
    INTCONbits.GIE=1;
    while(1){
        if(is_game_over()){
            for(int i = 0; i < 5; i++){
                for(int j = 0; j < 50; j++){
                    update_7segment();
                }
                __delay_ms(200);
            }
            initialize();
            g0_prev = PORTGbits.RG0;
            g4_prev = PORTGbits.RG4;
            g2_prev = PORTGbits.RG2;
            g3_prev = PORTGbits.RG3;
            b7_prev = PORTBbits.RB7;
            b6_prev = PORTBbits.RB6;
            INTCONbits.GIE=1;
        }
        if(submit){
            create_new_tile();
            submit=0;
        }  
        checkRBs();
        checkRGs();
        if(interrupt_blink){
            interrupt_blink = 0;
            blink_state = (blink_state + 1) % 2;
            blink_curr_shape();
        }
        if(interrupt_fall){
            interrupt_fall = 0;
            rg2_pressed();
        }
        if(rb7_toggled){
            rb7_toggled = 0;
            rb7_pressed();
        }
        if(rb6_toggled){
            rb6_toggled = 0;
            rb6_pressed();
        }
        update_7segment();
    }
}