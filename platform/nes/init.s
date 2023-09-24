.include "global.inc"
.export _init
.import _clear_screen

.segment "CHARS"
charset:
    .incbin "res/tiles.nes.s"

.segment "CODE"
.proc _init
:   bit PPU_STATUS
    bpl :-

    ; -------------------------
    ; initialize ppu
    ; -------------------------
    bit PPU_STATUS
    lda #0
    sta PPU_CTRL
    sta PPU_ADDR
    sta PPU_ADDR
    sta PPU_MASK

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

    jsr _clear_screen ; clear nametables & attributes, turn back on PPU

    rts
.endproc
