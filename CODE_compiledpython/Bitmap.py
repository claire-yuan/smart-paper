from PIL import Image
import pathlib
import os

print("Current cwd: ", os.getcwd())

# Get current script's folder
script_location = pathlib.Path(__file__).parent

# get data folder
data_location = script_location / "files"

# single file example
png_file = data_location / "image.png"
output_file = data_location / "output.txt"
img = Image.open(png_file) #change file

width,height = img.size #check size to see if need to be rotated
print(height, width) #checking something
if height > width:
    img = img.transpose(Image.Transpose.ROTATE_90)

img_resized = img.resize((800,480))

print(img_resized.height, img_resized.width)

#convert to black and white
img_resized = img_resized.convert("1")

pixels = list(img_resized.getdata())

## now i need to convert to 0 and 1
pixels = [pixels[i]/255 for i in range(len(pixels))]
pixels = [int(x) for x in pixels]

## converting to hex unsinged char array
byte_array = bytearray()

for i in range(0, len(pixels), 8):
    byte = 0
    for j in range(8):
        byte = (byte << 1) | pixels[i + j]
        byte = int(byte)
    byte_array.append(byte)

# C-style formatting
hex_string = ', '.join(f'0x{b:02X}' for b in byte_array)

# Write to text file
with open(output_file, 'w') as f:
    f.write(hex_string)
