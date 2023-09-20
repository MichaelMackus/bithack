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
:   lda (map_ptr), y
    cmp #MAP_FLOOR
    bne clear
    lda #1 ; floor tile
    jmp update
clear:
    lda #$20 ; space (don't render for now)
update:
    sta (screen_ptr), y
    add16 map_ptr, .sizeof(Tile)
    add16 screen_ptr, 1
    is_end_of_map map_ptr
    bne :-

    rts
.endproc
