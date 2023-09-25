.include "global.inc"

.export _read_input

BUTTON_A      = 1 << 7
BUTTON_B      = 1 << 6
BUTTON_SELECT = 1 << 5
BUTTON_START  = 1 << 4
BUTTON_UP     = 1 << 3
BUTTON_DOWN   = 1 << 2
BUTTON_LEFT   = 1 << 1
BUTTON_RIGHT  = 1 << 0

.segment "BSS"

input: .byte 0

.segment "CODE"

.proc _read_input
    lda #1
    sta input ; initialize input ring counter
    sta APU_PAD1
    lda APU_PAD1
    lsr a
    sta APU_PAD1
:   lda APU_PAD1 ; each read of $4016 will return 1 bit of next button
    lsr a
    rol input
    bcc :-
    ; translate to ascii
    lda input
    and #BUTTON_A | BUTTON_B
    beq :+
    lda #10 ; \n
    jmp done
:   lda input
    and #BUTTON_SELECT | BUTTON_START
    beq :+
    lda #'i'
    jmp done
:   lda input
    and #BUTTON_UP
    beq :+
    lda #'k'
    jmp done
:   lda input
    and #BUTTON_DOWN
    beq :+
    lda #'j'
    jmp done
:   lda input
    and #BUTTON_LEFT
    beq :+
    lda #'h'
    jmp done
:   lda input
    and #BUTTON_RIGHT
    beq :+
    lda #'l'
    jmp done
:   lda #0
done:
    ldx #0
    cmp #0 ; reset flags
    rts
.endproc
