.include "global.inc"
.export _init

.import _draw_buffer_idx
.import _clear_screen

.segment "DATA"
charset:
    .incbin "res/tiles.s"

.segment "CODE"
_init:
    sei         ;turn off interrupts & I/O

    ; disable SHIFT-Commodore
    lda #$80
    sta $0291
    ; set single-color text mode 40x25 and no X offset
    lda #%00011111
    sta $d011
    lda #%00001111
    sta $d016
    ; set border color
    lda #$00
    sta $d020
    ; set background color
    lda #$00
    sta $d021
    ; set text color
    lda #$01
    sta $0286

    ; -------------------------
    ; load in custom characters
    ; -------------------------
    ldx #0
:   lda charset, x
    sta CHAR_RAM, x
    lda charset + $100, x
    sta CHAR_RAM + $100, x
    lda charset + $200, x
    sta CHAR_RAM + $200, x
    lda charset + $300, x
    sta CHAR_RAM + $300, x
    lda charset + $400, x
    sta CHAR_RAM + $400, x
    lda charset + $500, x
    sta CHAR_RAM + $500, x
    lda charset + $600, x
    sta CHAR_RAM + $600, x
    lda charset + $700, x
    sta CHAR_RAM + $700, x
    inx
    bne :-
    ; ------------
    ; clear screen
    ; ------------
    ldx #0
    lda #$20
:   sta SCREEN_RAM,      x
    sta SCREEN_RAM+$100, x
    sta SCREEN_RAM+$200, x
    sta SCREEN_RAM+$300, x
    inx
    bne :-
    ; clear color ram
    lda #1
:   sta COLOR_RAM,       x
    sta COLOR_RAM+$100,  x
    sta COLOR_RAM+$200,  x
    sta COLOR_RAM+$300,  x
    inx
    bne :-

    ; initialize draw buffer
    lda #0
    sta _draw_buffer_idx

    ; ensure IO lines are set to output
    lda $dd02
    ora #%00000011
    sta $dd02
    ; choose last VIC bank
    lda $dd00
    and #%11111100
    sta $dd00
    ; set screen memory ($C400) and charset bitmap offset ($C800)
    lda #%00010010
    sta $d018
    ; tell KERNAL to use our charset
    lda #>SCREEN_RAM
    sta $288

    ;reactivate interrupts
    cli

    rts           ; Return to Basic  
 
hello:  .asciiz "HELLO WORLD"
