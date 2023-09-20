; SEE: https://petscii.krissz.hu/

	; set to 25 line text mode and turn on the screen
	lda #$1B
	sta $d011

	; disable SHIFT-Commodore
	lda #$80
	sta $0291

	; set screen memory ($0400) and charset bitmap offset ($2000)
	lda #$18
	sta $d018

	; set multicolor mode with 40 col and no X offset
	lda #$D8
	sta $d016
	
	; set multicolor 1
	lda #$08
	sta $d022
	
	; set multicolor 2
	lda #$0C
	sta $d023

	; set border color
	lda #$00
	sta $d020

	; set background color
	lda #$00
	sta $d021

	; set text color
	lda #$07
	sta $0286
	rts

    ; TODO store chardata in RAM at $D000
    ; TODO set bits 0-1 of $0001 to binary %00 after copying char rom

chardata:
    .BYTE   0,  0,  4,  0,  0, 32,  0,  0 ; passage 
    .BYTE  63, 63, 63,  0,252,252,252,  0 ; wall
    .BYTE  60, 90,126, 36, 60, 90, 24, 36 ; monster / goblin
    .BYTE  60,126,219,219,255,255,102, 60 ; player
    .BYTE   0,  0,  0,  0,128,160,168,170 ; downstair
    .BYTE   3,  7,142, 92, 56,112,232,196 ; sword
    .BYTE   0, 66,231,255,126,126, 60, 24 ; armor
    .BYTE  60, 36, 36, 82, 82, 82, 66, 60 ; potion
    .BYTE 126,129,126, 74,118, 74,255,126 ; scroll
    .BYTE  60,126,102,102,126,122,126,  0 ; door
    .BYTE   0,  0,  0,124,254,238,238,254 ; chest
    .BYTE  24, 60, 60, 24, 60, 90, 24, 36 ; human
    .BYTE   0,  0,  0,  0,  2, 10, 42,170 ; upstair
    .BYTE   3,  3,  4,  8, 16, 96,224,192 ; arrow 
    .BYTE   0,255,129,189,189, 90, 36, 24 ; shield
    .BYTE   3,  5,  6,  8, 16, 32, 64,128 ; wand

	; multicolor character data
; multi_chardata:
; 	BYTE	$00, $00, $00, $00, $00, $00, $00, $00
; 	BYTE	$00, $0C, $00, $00, $00, $00, $30, $00  ; lit/colored passsage
; 	BYTE	$2A, $3F, $3F, $00, $A8, $FC, $FC, $00  ; brick
; 	BYTE	$3C, $FF, $DD, $DD, $FF, $FC, $30, $CC  ; monster / goblin
; 	BYTE	$3C, $BE, $AA, $AA, $96, $D7, $D7, $28  ; human
; 	BYTE	$00, $00, $00, $00, $02, $0A, $2A, $AA  ; upstair

; 	BYTE	$00, $08, $00, $00, $00, $00, $20, $00  ; unlit passage
; 	BYTE	$14, $55, $41, $41, $55, $55, $55, $00  ; door
; 	BYTE	$00, $00, $00, $00, $FF, $55, $7D, $55  ; chest
; 	BYTE	$00, $00, $00, $00, $00, $00, $00, $00
; 	BYTE	$00, $00, $00, $00, $00, $00, $00, $00
; 	BYTE	$00, $00, $00, $00, $80, $A0, $A8, $AA  ; downstair
