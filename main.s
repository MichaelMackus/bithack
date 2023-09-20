.include "global.inc"

.import init, generate, render
.importzp seed

map_end = map + MAP_TILES*.sizeof(Tile)

.segment "ZEROPAGE"
tmp1: .res 2
tmp2: .res 2
tmp3: .res 2

.segment "BSS"
map: .res MAP_TILES * .sizeof(Tile)

.segment "INIT"
.segment "ONCE"
.segment "STARTUP"
    jsr init
    jsr titlescreen
    jsr generate
    jsr render

gameloop:
:   ; lookup keyboard matrix for return
    ; see: https://c64os.com/post/howthekeyboardworks
    lda #%11111110
    sta $DC00
    lda $DC01
    cmp #%11111101
    beq :+
    jmp gameloop
:   jsr generate
    jsr render
    jmp gameloop

titlescreen:
    lda #$1F
    sta seed + 1
    ; TODO render a title screen
:   ; lookup keyboard matrix for return
    ; see: https://c64os.com/post/howthekeyboardworks
    lda #%11111110
    sta $DC00
    lda $DC01
    cmp #%11111101
    beq :+
    add16 seed, 1
    jmp :-
:   rts

