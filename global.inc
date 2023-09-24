CHROUT         = $FFD2
SCREEN_RAM     = $C400
CHAR_RAM       = $C800
COLOR_RAM      = $D800
MAP_COLS       = 40
MAP_ROWS       = 20
MAP_TILES      = MAP_COLS * MAP_ROWS

.globalzp ptr1, ptr2, ptr3

.macro add16 addr, word
    clc
    lda addr
    adc #<word
    sta addr
    lda addr + 1
    adc #>word
    sta addr + 1
.endmacro
.macro add16addrs addr, addr2
    clc
    lda addr
    adc addr2
    sta addr
    lda addr + 1
    adc addr2 + 1
    sta addr + 1
.endmacro
.macro cp16 addr, addr2
.scope
    lda addr + 1
    cmp addr2 + 1
    bne no
    lda addr
    cmp addr2
    bne no
    lda #0
    jmp end
no:
    lda #1
end:
.endscope
.endmacro
; TODO sub16 func
