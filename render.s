.include "global.inc"

.export _render_map
.export _render_buffer
.export _wait_for_vblank
.export _draw_buffer
.export _draw_buffer_idx

.importzp sp

.segment "BSS"

_draw_buffer:     .res 255
_draw_buffer_idx: .byte 0

.segment "CODE"

; render the draw buffer
.proc _render_buffer
    idx = ptr1
    ch  = ptr2
    ldx #0
:   cpx _draw_buffer_idx
    beq done ; if we are pointing to y we are done
    lda _draw_buffer, x
    sta ch
    inx
    lda _draw_buffer, x
    sta idx
    inx
    lda _draw_buffer, x
    sta idx + 1
    inx
    add16 idx, SCREEN_RAM ; store the absolute address in screen ram into idx
    ; store the character into screen ram pointed to by idx
    lda ch
    ldy #0
    sta (idx), y
    jmp :-

done:
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

.proc _wait_for_vblank
wait_for_next_frame:
     bit $d011
     bpl wait_for_next_frame
     lda $d012
:    cmp $d012
     bmi :-
     rts
.endproc
