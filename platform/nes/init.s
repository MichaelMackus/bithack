.include "global.inc"
.export _init

.segment "RODATA"
charset:
    .incbin "res/tiles.s"

.segment "CODE"
_init:
    ; -------------------------
    ; initialize ppu
    ; -------------------------
    ; TODO check PPU_STATUS first
    lda #0
    sta PPU_CTRL
    lda PPU_STATUS
    lda #0
    sta PPU_ADDR
    sta PPU_ADDR
    sta PPU_MASK

    ; -----------------------------------------
    ; load custom characters into pattern table
    ; -----------------------------------------
    ldx #0
:   lda charset, x
    sta PPU_DATA
    sta PPU_DATA
    inx
    bne :-
:   lda charset + $100, x
    sta PPU_DATA
    sta PPU_DATA
    inx
    bne :-
:   lda charset + $200, x
    sta PPU_DATA
    sta PPU_DATA
    inx
    bne :-
:   lda charset + $300, x
    sta PPU_DATA
    sta PPU_DATA
    inx
    bne :-
:   lda charset + $400, x
    sta PPU_DATA
    sta PPU_DATA
    inx
    bne :-
:   lda charset + $500, x
    sta PPU_DATA
    sta PPU_DATA
    inx
    bne :-
:   lda charset + $600, x
    sta PPU_DATA
    sta PPU_DATA
    inx
    bne :-
:   lda charset + $700, x
    sta PPU_DATA
    sta PPU_DATA
    inx
    bne :-

    ; -------------------------
    ; setup example nametable
    ; -------------------------
    lda PPU_STATUS
    lda #$20
    sta PPU_ADDR
    lda #$00
    sta PPU_ADDR
    ldx #0
    lda #0
:   sta PPU_DATA
    inx
    bne :-
:   stx PPU_DATA
    inx
    bne :-
:   stx PPU_DATA
    inx
    bne :-
:   stx PPU_DATA
    inx
    bne :-
    ; -----------------------------
    ; setup example attribute table
    ; -----------------------------
    lda PPU_STATUS
    lda #$23
    sta PPU_ADDR
    lda #$C0
    sta PPU_ADDR
    ldx #64
    lda #%00000000 ; choose pallette 1 for each pattern entry
:   sta PPU_DATA
    dex
    bne :-
    ; -----------------------------
    ; setup palettes
    ; -----------------------------
    lda PPU_STATUS
    lda #$3F
    sta PPU_ADDR
    lda #$00
    sta PPU_ADDR
    ; universal background color
    lda #$0F
    sta PPU_DATA
    ; background color 0
    lda #$20 ; white
    sta PPU_DATA
    sta PPU_DATA
    sta PPU_DATA
    sta PPU_DATA ; skipped byte
    ; background color 1
    lda #$10 ; light grey
    sta PPU_DATA
    sta PPU_DATA
    sta PPU_DATA
    sta PPU_DATA ; skipped byte
    ; background color 2
    lda #$00 ; dark grey
    sta PPU_DATA
    sta PPU_DATA
    sta PPU_DATA
    sta PPU_DATA ; skipped byte
    ; background color 3
    lda #$2D ; very dark grey
    sta PPU_DATA
    sta PPU_DATA
    sta PPU_DATA
    ; universal background color mirror
    lda #$0F
    sta PPU_DATA
    ldx #14
    lda #$20 ; white
:   sta PPU_DATA
    dex
    bne :-
    ; NTSC NES quirk fix
    lda PPU_STATUS
    lda #$3F
    sta PPU_ADDR
    lda #0
    sta PPU_ADDR
    ; point back to nametable
    sta PPU_ADDR
    sta PPU_ADDR

    ; turn on PPU
    lda #%00001010 ; TODO color & sprites
    sta PPU_MASK

    ; infinite loop
:   jmp :-

    rts
