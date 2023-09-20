.include "global.inc"

.importzp tmp1, tmp2
.export divide, mult16

.segment "CODE"

; divide x / y
;
; in: dividend
; x:  divisor
;
; out: remainder (mod)
; x:   result of division
; affects: x, tmp1
.proc divide
    divisor  = tmp1
    stx divisor
    ldx #0
loop:
    cmp divisor
    bcc done
    ; dividend - divisor
    sec
    sbc divisor
    inx
    jmp loop
done:
    rts
.endproc

; 16-bit multiplication x * y
;
; tmp1: y (16-bit value)
; x:    x (8-bit value)
;
; out: 16-bit result put in tmp1
; affects: x, y, tmp1, tmp2, tmp3
.proc mult16
    result = tmp1
    count  = tmp2
    start  = tmp3
    stx count
    ldx #0
    cpx count
    bne go
    ; special case for zero
    lda #0
    sta result
    sta result + 1
    jmp done
go:
    lda result
    sta start
    lda result + 1
    sta start + 1
    inx
loop:
    cpx count
    beq done
    ; dividend - divisor
    add16addrs result, start
    inx
    jmp loop
done:
    rts
.endproc
