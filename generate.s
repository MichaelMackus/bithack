.include "global.inc"

.export generate

.segment "CODE"

generate:
    ; first clear map
    lda #MAP_WALL
    ldx #0

:   sta map, x
    inx
    cpx #20
    bne :-

    ; TODO need to compare 16 bit values to determine if done
    ; cmp #>map + MAP_SIZE

    rts
