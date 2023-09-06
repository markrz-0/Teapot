from PIL import Image

TRANSPARENT_COLOR = 0xff00ff

def rgba_to_color_ref_number(r, g, b, a):
    if a < 240:
        return TRANSPARENT_COLOR

    return  r * 256 * 256 +\
        g * 256 +\
        b

width = 600

img = Image.open('minecraft.png')
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

cpp_array_str = str(arr).replace("[", "{").replace("]", "}").replace(",", ",\n")

with open('image_arr.cpp', 'w') as f:
    f.write('#pragma once\n')
    f.write('\n')
    f.write('#include <Windows.h>\n')
    f.write('\n')
    f.write('// Dont touch this file; it is auto generated by create_pixel_array.py\n')
    f.write('\n')
    f.write(f"COLORREF* GetImageColorArray() {{")
    f.write(f'    return new COLORREF[{len(arr)}]{cpp_array_str};')
    f.write(f"}}")


with open("image_arr.h", 'w') as f:
    f.write('#pragma once\n')
    f.write('\n')
    f.write('#include <Windows.h>\n')
    f.write('\n')
    f.write('// Dont touch this file; it is auto generated by create_pixel_array.py\n')
    f.write('\n')
    f.write(f'const int IMG_WIDTH = {width};\n')
    f.write(f'const int IMG_HEIGHT = {height};\n')
    f.write(f'const int IMG_COLOR_ARR_LENGTH = {len(arr)};\n')
    f.write(f"COLORREF* GetImageColorArray();\n")
