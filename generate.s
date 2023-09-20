.include "global.inc"

.export generate

.segment "CODE"

.proc generate
    map_end = tmp1
    map_ptr = tmp2

    .macro check_loop_end
        lda map_ptr + 1
        cmp map_end + 1
        bne loop
        lda map_ptr
        cmp map_end
        bne loop
        jmp loop_end
    .endmacro

    ; first we need to find end of map by adding MAP_SIZE to map start
    ; TODO this isn't calculating right?
    clc
    lda #<map
    adc #<MAP_SIZE
    sta map_end
    lda #>map
    adc #>MAP_SIZE
    sta map_end + 1

    ; setup map ptr
    lda #<map
    sta map_ptr
    lda #>map
    sta map_ptr + 1

    ; now, clear map
    ldy #0
loop: 
    lda #$20
    sta (map_ptr), y
    inc map_ptr
    beq :+
    check_loop_end
:   inc map_ptr + 1
    check_loop_end

loop_end:
    rts

.endproc
