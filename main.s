.include "global.inc"

.import init

.segment "ONCE"
.segment "STARTUP"

	jsr init

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
