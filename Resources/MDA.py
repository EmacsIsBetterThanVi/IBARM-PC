from pygame import image
im = image.load("MDA.png")
chars_x = int(im.get_width()/9)
chars_y = int(im.get_height()/14)
"""
MDA font is formated as the first 8 pixels on each line in thier own byte, then the last bit on each line is stored in the last 2 bytes
"""
f = open("MDA.h", "w")
f.write("const uint8_t MDA_FONT[4096] = {0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0, 0, 0\n")
for chy in range(chars_y):
    for chx in range(chars_x):
        if chy*32+chx == 0:
            continue
        char = []
        for i in range(16):
            char.append([])
            for j in range(8):
                char[i].append('0')
        for y in range(14):
            for x in range(9):
                bit = '0' if im.get_at((chx*9+x, chy*14+y))[0]==0 else '1'
                if (x==8):
                    char[14 if y<8 else 15][y%8] = bit
                else:
                    char[y][x] = bit
        for i in range(16):
            f.write(f", {int(''.join(char[i]), 2)}" + ("\n" if i == 15 else ''))
f.write("};")
f.close()
