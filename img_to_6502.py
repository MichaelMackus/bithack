# img_to_6502 script
# TODO output both NES and c64 tiles

from PIL import Image

filename = 'res/tiles.png'

class TilesLoadException(Exception):
    pass
class TilesFormatException(Exception):
    pass

class Tiles:
    def __init__(self, img):
        img = img.convert('RGB')
        (self.width, self.height) = img.size
        self.img = img
        self.tile_width = 8
        self.tile_height = 8
        if self.width % self.tile_width != 0 or self.height % self.tile_height != 0:
            raise TilesLoadException('Image incorrect size - must be 8x8 tiles')
    # commodore 64 monochrome tile format
    def __iter__(self):
        for start_y in range(0, self.height, self.tile_height):
            for start_x in range(0, self.width, self.tile_width):
                for y in range(start_y, start_y + self.tile_height):
                    byte = 0
                    bit = 7
                    for x in range(start_x, start_x + self.tile_width):
                        value = self.img.getpixel((x, y)) != (0,0,0)
                        byte |= value << bit
                        bit -= 1
                    yield byte.to_bytes(1)

def file_to_tiles(filename):
    with open(filename, 'rb') as f:
        with Image.open(filename) as img:
            return Tiles(img)
    return None

if __name__ == "__main__":
    tiles = file_to_tiles(filename)
    # write generic image format in assembly
    with open(filename.removesuffix('.png') + '.s', 'wb') as f:
        for byte in tiles:
            f.write(byte)
    # NES image format
    with open(filename.removesuffix('.png') + '.nes.s', 'wb') as f:
        char = []
        for byte in tiles:
            char.append(byte)
            if len(char) == 8:
                # NES format requires 16 bytes per tile
                for b in char:
                    f.write(b)
                for b in char:
                    f.write(b)
                char = []
