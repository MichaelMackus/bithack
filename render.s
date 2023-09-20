.include "global.inc"

.export render

.segment "CODE"

.proc render
    screen_ptr = tmp1
    map_ptr    = tmp2

    ldy #0
    lda #<SCREEN_RAM
    sta screen_ptr
    lda #>SCREEN_RAM
    sta screen_ptr + 1
    lda #<map
    sta map_ptr
    lda #>map
    sta map_ptr + 1
    ; loop through map, storing at SCREEN_RAM
    ; TODO lda wrong - map size less than screen size, also needs conversion from Tile -> screen code
:   lda (map_ptr), y
    sta (screen_ptr), y
    iny
    bne :-
    ; go to next 256 offset, by increasing high byte of screen_ptr
    inc map_ptr + 1
    inc screen_ptr + 1
    lda screen_ptr + 1
    ; TODO need to compare 16 bit values to determine if done
    ; loop back if we're not at end of screen ram
    cmp #>SCREEN_RAM_END + 1
    bne :-
    
    rts
.endproc
