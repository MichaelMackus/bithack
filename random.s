
; prng
;
; Returns a random 8-bit number in A (0-255), clobbers X (0).
;
; Requires a 2-byte value on the zero page called "seed".
; Initialize seed to any value except 0 before the first call to prng.
; (A seed value of 0 will cause prng to always return 0.)
;
; This is a 16-bit Galois linear feedback shift register with polynomial $002D.
; The sequence of numbers it generates will repeat after 65535 calls.
;
; Execution time is an average of 125 cycles (excluding jsr and rts)

.exportzp seed
.export prng, d2, d3, d4, d6, d8, d12
.import divide

.segment "ZEROPAGE"

seed: .res 2       ; initialize 16-bit seed to any value except 0

.segment "CODE"

; clobbers: x and accum
prng:
	ldx #8     ; iteration count (generates 8 bits)
	lda seed+0
:
	asl        ; shift the register
	rol seed+1
	bcc :+
	eor #$2D   ; apply XOR feedback whenever a 1 bit is shifted out
:
	dex
	bne :--
	sta seed+0
	cmp #0     ; reload flags
	rts

; generate random value from 1-2
d2:
    jsr prng
    and #%00000001
    beq :+
    lda #2
:
    rts

; generate random value from 1-3
d3:
    jsr prng
    ; divide result by 3
    ldx #3
    jsr divide
    ; add 1 to remainder get 1-3
    clc
    adc #1
    rts

; generate random value from 1-4
d4:
    jsr prng
    and #%00000011
    clc
    adc #1
    rts

; generate random value from 1-6
d6:
    jsr prng
    ; divide result by 6
    ldx #6
    jsr divide
    ; add 1 to remainder get 1-6
    clc
    adc #1
    rts

; generate random value from 1-8
d8:
    jsr prng
    and #%00000111
    clc
    adc #$01
    rts

; generate random value from 1-12
d12:
    jsr prng
    ; divide result by 12
    ldx #12
    jsr divide
    ; add 1 to remainder get 1-12
    clc
    adc #1
    rts
