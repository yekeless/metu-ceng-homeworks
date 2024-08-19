PROCESSOR 18F8722

#include <xc.inc>

; CONFIGURATION (DO NOT EDIT)
; CONFIG1H
CONFIG OSC = HSPLL      ; Oscillator Selection bits (HS oscillator, PLL enabled (Clock Frequency = 4 x FOSC1))
CONFIG FCMEN = OFF      ; Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
CONFIG IESO = OFF       ; Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)
; CONFIG2L
CONFIG PWRT = OFF       ; Power-up Timer Enable bit (PWRT disabled)
CONFIG BOREN = OFF      ; Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
; CONFIG2H
CONFIG WDT = OFF        ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
; CONFIG3H
CONFIG LPT1OSC = OFF    ; Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
CONFIG MCLRE = ON       ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
; CONFIG4L
CONFIG LVP = OFF        ; Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
CONFIG XINST = OFF      ; Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))
CONFIG DEBUG = OFF      ; Disable In-Circuit Debugger


GLOBAL var1
GLOBAL var2
GLOBAL var3
GLOBAL var4
GLOBAL prev_re1_status
GLOBAL curr_re1_status
GLOBAL prev_re0_status
GLOBAL curr_re0_status
GLOBAL D_status
GLOBAL counterB
GLOBAL counterC
GLOBAL work_statusB
GLOBAL work_statusC

; Define space for the variables in RAM
PSECT udata_acs
var1:
    DS 1 ; Allocate 1 byte for var1
var2:
    DS 1 
var3:
    DS 1
var4:
    DS 1
prev_re1_status:
    DS 1
curr_re1_status:
    DS 1
prev_re0_status:
    DS 1
curr_re0_status:
    DS 1    
D_status:
    DS 1    
counterB:
    DS 1
counterC:
    DS 1
work_statusB:
    DS 1
work_statusC:
    DS 1
prev_work_statusC:
    DS 1
prev_work_statusB:
    DS 1
PSECT resetVec,class=CODE,reloc=2
resetVec:
    goto       main

PSECT CODE
main:
    ; initialization beginning
    clrf var1	; var1 = 0		
    clrf var2   ; var2 = 0
    clrf counterB
    clrf counterC
    clrf curr_re1_status
    clrf prev_re1_status
    clrf curr_re0_status
    clrf prev_re0_status
    clrf D_status
    clrf var3
    clrf var4
    clrf work_statusB
    clrf work_statusC
    
    
    ; initialization ending
    ; input output port selection
    clrf TRISB
    clrf TRISC
    clrf TRISD
    setf TRISE

    call initializer ; call the initializer function
    movlw 1
    movwf D_status
    goto main_loop  ; Jump to the main loop 
initializer:
    setf PORTB
    setf PORTC
    setf PORTD
    ; light up all the output pins
    call busy_wait ; call busy waiting
    clrf PORTB
    clrf PORTC
    clrf PORTD
    return
busy_wait:
    setf var1 ; loop variable initalization
    outer_loop:
	setf var2 ; inner loop 1 initialization
	first_inner_loop:
	    decf var2
	    bnz first_inner_loop
	setf var2 ; inner loop 2 initalization    
	second_inner_loop:
	    decf var2
	    bnz second_inner_loop
	setf var2 ; inner loop 3 initialization
	third_inner_loop:
	    decf var2
	    bnz third_inner_loop
	setf var2 ; inner loop 4 initialization
	fourth_inner_loop:
	    decf var2
	    bnz fourth_inner_loop
	setf var2 ; inner loop 5 initialization
	fifth_inner_loop:
	    decf var2
	    bnz fifth_inner_loop
	decf var1
	bnz outer_loop
    return
main_loop:
    call check_buttons
    goto main_loop
check_buttons:
    movlw 55
    movwf var3
    check_outer_loop:
	setf var4 ; first inner check loop initialization
	first_check:
	    
	    
	    call re0_check ; re0 bit check
	    
	    call re1_check ; re1 bit check
	    
	    decf var4 
	    bnz first_check
	
	decf var3
	bnz check_outer_loop
	
    call update_display
    
    return
re0_check:
    movlw 1
    cpfseq curr_re0_status
    goto curr_re0_is_zero
    goto curr_re0_is_one
    curr_re0_is_zero:
	btfsc PORTE, 0
	goto input_re0_is_one
	goto input_re0_is_zero
	input_re0_is_one:
	    movff curr_re0_status,prev_re0_status
	    movlw 1
	    movwf curr_re0_status
	    return
	input_re0_is_zero:
	    movff curr_re0_status,prev_re0_status
	    return
    curr_re0_is_one:
	btfsc PORTE, 0
	goto input_re0_is_1
	goto input_re0_is_0
	input_re0_is_1:
	    movff curr_re0_status,prev_re0_status
	    return
	input_re0_is_0:
	    movff curr_re0_status,prev_re0_status
	    movlw 0
	    movwf curr_re0_status
	    movlw 0
	    cpfseq work_statusC
	    goto close_C
	    goto open_C
	    close_C:
		clrf work_statusC
		clrf counterC
		return
	    open_C:
		movlw 1
		movwf work_statusC
		clrf counterC
		return
	    
	
	
re1_check:
    movlw 1
    cpfseq curr_re1_status
    goto curr_re1_is_zero
    goto curr_re1_is_one
    curr_re1_is_zero:
	btfsc PORTE, 1
	goto input_re1_is_one
	goto input_re1_is_zero
	input_re1_is_one:
	    movff curr_re1_status,prev_re1_status
	    movlw 1
	    movwf curr_re1_status
	    return
	input_re1_is_zero:
	    movff curr_re1_status,prev_re1_status
	    return
    curr_re1_is_one:
	btfsc PORTE, 0
	goto input_re1_is_1
	goto input_re1_is_0
	input_re1_is_1:
	    movff curr_re1_status,prev_re1_status
	    return
	input_re1_is_0:
	    movff curr_re1_status,prev_re1_status
	    movlw 0
	    movwf curr_re1_status
	    movlw 0
	    cpfseq work_statusB
	    goto close_B
	    goto open_B
	    close_B:
		clrf work_statusB
		clrf counterB
		return
	    open_B:
		movlw 1
		movwf work_statusB
		clrf counterB
		return
update_display:
    call update_display_B
    call update_display_C
    call update_display_D
    return
update_display_D:
    movlw 1
    cpfseq D_status
    goto no_light_D
    goto light_D
    no_light_D:
	clrf PORTD
	movlw 1
	movwf D_status
	return
    light_D:
	movlw 1
	movwf PORTD
	movlw 0
	movwf D_status
	return
update_display_B:
    movlw 1
    cpfseq work_statusB
    goto not_workingB
    goto workingB
    not_workingB:
	clrf PORTB
	clrf counterB
	return
    workingB:
	incf counterB
	movlw 9
	cpfseq counterB
	goto switch_B
	goto reset_counterB
	switch_B:
	    movlw 8
	    cpfslt counterB
	    goto case_8_B
	    movlw 7
	    cpfslt counterB
	    goto case_7_B
	    movlw 6
	    cpfslt counterB
	    goto case_6_B
	    movlw 5
	    cpfslt counterB
	    goto case_5_B
	    movlw 4
	    cpfslt counterB
	    goto case_4_B
	    movlw 3
	    cpfslt counterB
	    goto case_3_B
	    movlw 2
	    cpfslt counterB
	    goto case_2_B
	    movlw 1
	    cpfslt counterB
	    goto case_1_B
	    case_8_B:
		movlw 255
		movwf PORTB
		return
	    case_7_B:
		movlw 127
		movwf PORTB
		return
	    case_6_B:
		movlw 63
		movwf PORTB
		return
	    case_5_B:
		movlw 31
		movwf PORTB
		return
	    case_4_B:
		movlw 15
		movwf PORTB
		return
	    case_3_B:
		movlw 7
		movwf PORTB
		return
	    case_2_B:
		movlw 3
		movwf PORTB
		return
	    case_1_B:
		movlw 1
		movwf PORTB
		return	
		
	reset_counterB:
	    clrf counterB
	    clrf PORTB
	    return
update_display_C:
    movlw 1
    cpfseq work_statusC
    goto not_workingC
    goto workingC
    not_workingC:
	clrf PORTC
	clrf counterC
	return
    workingC:
	incf counterC
	movlw 9
	cpfseq counterC
	goto switch_C
	goto reset_counterC
	switch_C:
	    movlw 8
	    cpfslt counterC
	    goto case_8_C
	    movlw 7
	    cpfslt counterC
	    goto case_7_C
	    movlw 6
	    cpfslt counterC
	    goto case_6_C
	    movlw 5
	    cpfslt counterC
	    goto case_5_C
	    movlw 4
	    cpfslt counterC
	    goto case_4_C
	    movlw 3
	    cpfslt counterC
	    goto case_3_C
	    movlw 2
	    cpfslt counterC
	    goto case_2_C
	    movlw 1
	    cpfslt counterC
	    goto case_1_C
	    case_8_C:
		movlw 255
		movwf PORTC
		return
	    case_7_C:
		movlw 254
		movwf PORTC
		return
	    case_6_C:
		movlw 252
		movwf PORTC
		return
	    case_5_C:
		movlw 248
		movwf PORTC
		return
	    case_4_C:
		movlw 240
		movwf PORTC
		return
	    case_3_C:
		movlw 224
		movwf PORTC
		return
	    case_2_C:
		movlw 192
		movwf PORTC
		return
	    case_1_C:
		movlw 128
		movwf PORTC
		return	
		
	reset_counterC:
	    clrf counterC
	    clrf PORTC
	    return
	
    

end resetVec
