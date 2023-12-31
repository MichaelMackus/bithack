.include "global.inc"

.export _render_map
.export _render_buffer
.export _wait_for_vblank
.export _clear_screen
.export _draw_buffer
.export _draw_buffer_idx

.importzp sp

.segment "BSS"

_draw_buffer:      .res 255
_draw_buffer_idx:  .byte 0

.segment "CODE"

; render the draw buffer
;
; NOTE I only have max of ~2250 cycles here before graphical corruption
; NOTE on (very) rough calculation, each loop should take ~35 cycles currently (should be enough to draw our max room size in 1 frame)
; NOTE see: https://www.nesdev.org/wiki/The_frame_and_NMIs
.proc _render_buffer
    ldx #0
loop:
    cpx _draw_buffer_idx
    bne update ; if we are pointing to _draw_buffer_idx we are done
    jmp done
update:
    ; PPU high byte is in draw_buffer + 2
    lda _draw_buffer + 2, x
    sta PPU_ADDR
    ; PPU low byte is in draw_buffer + 1
    lda _draw_buffer + 1, x
    sta PPU_ADDR
    ; character tile is in draw_buffer + 0
    lda _draw_buffer, x
    sta PPU_DATA
    ; increment X for next iteration
    inx
    inx
    inx
    jmp loop

done:
    lda #0
    sta PPU_ADDR
    sta PPU_ADDR
    lda #0
    sta _draw_buffer_idx
    rts
.endproc

; takes two 16-bit args
;
; arg1: tiles_start (pushed onto stack, HB then LB)
; arg2: tiles_end   (LB stored in A, HB in X)
.proc _render_map
    screen_ptr = ptr1
    map_ptr    = ptr2
    map_end    = ptr3

    ; reset C stack pointer
    inc sp
    inc sp

    rts
.endproc

.proc _wait_for_vblank
    lda tickcount
:   cmp tickcount
    beq :-
    rts
.endproc

.proc _clear_screen
    bit PPU_STATUS
    lda #0
    sta PPU_MASK ; turn off PPU while clearing
    sta PPU_CTRL
    ; ---------------
    ; clear nametable
    ; ---------------
    bit PPU_STATUS
    lda #$20
    sta PPU_ADDR
    lda #$00
    sta PPU_ADDR
    ldx #0
    lda #$20
:   sta PPU_DATA
    inx
    bne :-
:   sta PPU_DATA
    inx
    bne :-
:   sta PPU_DATA
    inx
    bne :-
:   sta PPU_DATA
    inx
    bne :-
    ; reset scrolling
    bit PPU_STATUS
    lda #0
    sta PPU_SCROLL
    sta PPU_SCROLL
    ; ---------------------
    ; setup attribute table
    ; ---------------------
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

    ; turn on PPU
    lda PPU_STATUS
    lda #$3F
    sta PPU_ADDR
    lda #0
    sta PPU_ADDR
    lda #0
    sta PPU_ADDR
    sta PPU_ADDR
    lda #%00001110 ; turn on background & sprites
    sta PPU_MASK
    lda #%10000000 ; turn back on NMI
    sta PPU_CTRL

    rts
.endproc
