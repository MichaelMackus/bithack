.include "global.inc"
.export _init
.import _draw_buffer
.import _draw_buffer_idx

.segment "CODE"
_init:
    ; disable SHIFT-Commodore
    lda #$80
    sta $0291
    ; set screen memory ($0400) and charset bitmap offset ($2000)
    ; lda #$18
    ; sta $d018
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

    ; clear screen
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
    sta _draw_buffer

    rts
