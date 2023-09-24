.include "global.inc"

.export _read_input

.segment "CODE"
_read_input:
read_keyboard:
    jsr $FF9F ; SCNKEY
    jsr $FFE4 ; GETIN
    bne done
read_joystick:
    lda #%00000001
    bit $DC00 ; CIA #1 for joystick #2
    bne :+
    lda #$4B ; up
    jmp done
:   lda #%00000010
    bit $DC00 ; CIA #1 for joystick #2
    bne :+
    lda #$4A ; down
    jmp done
:   lda #%00000100
    bit $DC00 ; CIA #1 for joystick #2
    bne :+
    lda #$48 ; left
    jmp done
:   lda #%00001000
    bit $DC00 ; CIA #1 for joystick #2
    bne :+
    lda #$4C ; right
    jmp done
; :   lda #%00010000
;     bit $DC00 ; CIA #1 for joystick #2
;     bne :+
;     lda #$4C ; TODO fire
;     jmp done
:   lda #0
done:
    ldx #0
    cmp #0 ; reset flags
    rts
