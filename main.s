.include "global.inc"

.import init, generate, render

.segment "ZEROPAGE"
tmp:  .res 2
tmp2: .res 2

.segment "DATA"
map: .res MAP_COLS * MAP_ROWS * .sizeof(Tile)

.segment "ONCE"
.segment "STARTUP"
    jsr init
    ; TODO title screen
    jsr generate
    jsr render

    ldx #0
    lda hello
:   jsr CHROUT
    inx
    lda hello, x
    bne :-

gameloop:
    jmp gameloop

hello:
    .asciiz "hello world!"

