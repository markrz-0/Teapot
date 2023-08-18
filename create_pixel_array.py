from PIL import Image

TRANSPARENT_COLOR = 255 # just blue

def rgba_to_color_ref_number(r, g, b, a):
    if a < 240:
        return TRANSPARENT_COLOR

    return  b * 256 * 256 +\
        g * 256 +\
        r

width = 600

img = Image.open('teapot.png')
print("MODE", img.mode)
original_size = img.size

height = int((width / original_size[0]) * original_size[1])

img = img.resize((width, height))

arr = []
if img.mode == 'RGBA':
    for w in range(width):
        for h in range(height):
            p = img.getpixel((h, w))
            arr.append(rgba_to_color_ref_number(*p))
else:
    print("unsupported mode")

with open('image_arr.hpp', 'w') as f:
    f.write('#pragma once\n')
    f.write('\n')
    f.write(f'int IMG_WIDTH = {width};\n')
    f.write(f'int IMG_COLOR_ARR_LENGTH = {len(arr)};\n')
    f.write(f'COLORREF* IMG_COLOR_ARR = new COLORREF[{len(arr)}]{str(arr).replace("[", "{").replace("]", "}")};')
