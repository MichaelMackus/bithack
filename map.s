.include "global.inc"

.export MAP_COLS, MAP_ROWS, MAP_WALL, MAP_FLOOR, MAP_DOOR, MAP_STAIR, MAP_CHEST, MAP_FOUNTAIN
.export map

MAP_COLS  = 40
MAP_ROWS  = 23

MAP_WALL     = %00000000
MAP_FLOOR    = %00000001
MAP_DOOR     = %00000011
MAP_STAIR    = %00000101
MAP_CHEST    = %00001001
MAP_FOUNTAIN = %00010001

; tile on map - contains MAP_* type and object specifics
.segment "BSS"

map: .res MAP_COLS * MAP_ROWS * .sizeof(Tile)
