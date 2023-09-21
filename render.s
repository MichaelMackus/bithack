.include "global.inc"

.export _render
.importzp sp

.segment "CODE"

; takes two 16-bit args
;
; arg1: tiles_start (pushed onto stack, HB then LB)
; arg2: tiles_end   (LB stored in A, HB in X)
.proc _render
    screen_ptr = ptr1
    map_ptr    = ptr2
    map_end    = ptr3

    sta map_end
    stx map_end + 1
    ldy #1
    lda (sp), y
    sta map_ptr + 1
    dey
    lda (sp), y
    sta map_ptr
    ldy #0
    lda #<SCREEN_RAM
    sta screen_ptr
    lda #>SCREEN_RAM
    sta screen_ptr + 1

    ; loop through map, storing at SCREEN_RAM
:   lda (map_ptr), y
    ; cmp #MAP_FLOOR
    beq clear
    lda #$AE ; floor tile
    jmp update
clear:
    lda #$20 ; space (don't render for now)
update:
    sta (screen_ptr), y
    add16 map_ptr, 1
    add16 screen_ptr, 1
    cp16 map_ptr, map_end
    bne :-

    ; reset C stack pointer
    inc sp
    inc sp

    rts
.endproc
