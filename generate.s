.include "global.inc"

.export generate
.import prng, divide, mult16, d3
.importzp seed

X_DIVISION = MAP_COLS / 3
Y_DIVISION = MAP_ROWS / 3

.segment "CODE"

.proc generate
    map_ptr   = tmp1
    map_start = tmp2
    count    = tmp3

    .macro setup_map_ptr
        ; setup map ptr
        lda #<map
        sta map_ptr
        lda #>map
        sta map_ptr + 1
    .endmacro

    ; -------------------------------
    ; clear map by filling with walls
    ; -------------------------------
    setup_map_ptr
    ldy #0
loop: 
    lda #MAP_WALL
    sta (map_ptr), y
    add16 map_ptr, .sizeof(Tile)
    is_end_of_map map_ptr
    bne loop

    ; ---------------
    ; generate a room
    ; ---------------
gen_room:
    lda #0
    sta count
    sta count + 1
gen_room_x:
    ; generate number 0-2
    jsr d3
    sec
    sbc #1
    ; multiply number to get # of bytes to offset
    tax
    lda #<(X_DIVISION * .sizeof(Tile))
    sta tmp1
    lda #>(X_DIVISION * .sizeof(Tile))
    sta tmp1 + 1
    jsr mult16 ; stores count at "map_ptr"
    ; store address for X offset
    add16 map_ptr, map
    lda map_ptr
    pha
    lda map_ptr + 1
    pha
gen_room_y:
    ; generate number 0-2
    jsr d3
    sec
    sbc #1
    ; multiply number to get # of bytes to offset
    tax
    lda #<(Y_DIVISION * .sizeof(Tile) * MAP_COLS)
    sta tmp1
    lda #>(Y_DIVISION * .sizeof(Tile) * MAP_COLS)
    sta tmp1 + 1
    jsr mult16 ; stores count at "map_ptr"
    ; store address for X offset
    pla
    sta tmp3 + 1
    pla
    sta tmp3
    add16addrs map_ptr, tmp3
room_ptr_generated:
    ; calculate map_ptr = start of room
    lda map_ptr
    sta map_start
    lda map_ptr + 1
    sta map_start + 1
    ; increment map_ptr until we hit desired width
    lda #X_DIVISION ; TODO should be RNG
    sta count
    lda #Y_DIVISION ; TODO should be RNG
    sta count + 1
    ldx #0
    ldy #0
floor_x:
    lda #MAP_FLOOR
    sta (map_ptr), y
    add16 map_ptr, .sizeof(Tile)
    inx
    cpx count
    bne floor_x
floor_y:
    add16 map_start, .sizeof(Tile) * MAP_COLS
    lda map_start
    sta map_ptr
    lda map_start + 1
    sta map_ptr + 1
    ldx #0
    ldy #0
    dec count + 1
    bne floor_x

    rts
.endproc
