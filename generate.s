.include "global.inc"

.export generate
.import prng, divide, mult16, d3, d4, d8
.import seed

X_DIVISION      = MAP_COLS / 3
Y_DIVISION      = MAP_ROWS / 3
ROOM_MIN_WIDTH  = 3
ROOM_MIN_HEIGHT = 2
ROOM_MAX_WIDTH  = X_DIVISION - 2
ROOM_MAX_HEIGHT = X_DIVISION - 2
MAP_ROOM_WIDTH  = X_DIVISION * .sizeof(Tile)
MAP_ROOM_HEIGHT = Y_DIVISION * .sizeof(Tile) * MAP_COLS

.segment "CODE"

.proc generate
    map_ptr   = ptr1
    map_start = ptr2
    result    = ptr3

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
    ldx #0
    ldy #0
gen_room:
    lda #0
    sta result
    sta result + 1
    ; lda #0
    ; sta result
    ; sta result + 1
    ; sta map_ptr
    ; sta map_ptr + 1
    ; ; multiply room width by X to determine X offset
    ; txa
    ; pha
    ; tya
    ; pha
    ; add16 map_ptr, MAP_ROOM_WIDTH
    ; jsr mult16 ; stores result at "map_ptr"
    ; pla
    ; tax
    ; lda map_ptr
    ; pha
    ; lda map_ptr + 1
    ; pha
    ; ; multiply room height by Y to determine Y offset
    ; lda #0
    ; sta map_ptr
    ; sta map_ptr + 1
    ; add16 map_ptr, MAP_ROOM_HEIGHT
    ; jsr mult16 ; stores result at "map_ptr"
    ; add16 map_ptr, map
    ; pla
    ; sta result + 1
    ; pla
    ; sta result
    ; add16addrs map_ptr, result
    ; TODO add X & Y to stack
; TODO generate X offset
gen_room_x:
    ; generate number 0-2
    jsr d3
    sec
    sbc #1
    ; multiply number to get # of bytes to offset
    tax
    lda #<MAP_ROOM_WIDTH
    sta ptr1
    lda #>MAP_ROOM_WIDTH
    sta ptr1 + 1
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
    lda #<MAP_ROOM_HEIGHT
    sta ptr1
    lda #>MAP_ROOM_HEIGHT
    sta ptr1 + 1
    jsr mult16 ; stores count at "map_ptr"
    ; store address for X offset
    pla
    sta ptr3 + 1
    pla
    sta ptr3
    add16addrs map_ptr, ptr3
room_ptr_generated:
    ; calculate map_ptr = start of room
    lda map_ptr
    sta map_start
    lda map_ptr + 1
    sta map_start + 1
room_gen_width:
    ; generate random width
    jsr d8
    clc
    adc #2
    cmp #ROOM_MAX_WIDTH
    bcs :+
    sta result
    jmp room_gen_height
:   lda #ROOM_MAX_WIDTH
    sta result
room_gen_height:
    ; generate random height
    jsr d4
    clc
    adc #2
    cmp #ROOM_MAX_HEIGHT
    bcs :+
    sta result + 1
    jmp room_dimensions
:   lda #ROOM_MAX_HEIGHT
    sta result + 1
room_dimensions:
    ; increment map_ptr until we hit desired width
    ldx #0
    ldy #0
floor_x:
    lda #MAP_FLOOR
    sta (map_ptr), y
    add16 map_ptr, .sizeof(Tile)
    inx
    cpx result
    bne floor_x
floor_y:
    add16 map_start, MAP_ROW_SIZE
    lda map_start
    sta map_ptr
    lda map_start + 1
    sta map_ptr + 1
    ldx #0
    ldy #0
    dec result + 1
    bne floor_x

    rts
.endproc
