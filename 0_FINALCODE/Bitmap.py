from PIL import Image
import pathlib
import os

print("Current cwd: ", os.getcwd())

# Get current script's folder
script_location = pathlib.Path(__file__).parent

# get data folder
data_location = script_location / "files"

# single file example
png_file = data_location / "shark.png"
output_file = data_location / "output.txt"
img = Image.open(png_file) #change file
# img.show()

width,height = img.size #check size to see if need to be rotated
print(height, width) #checking something
if height > width:
    img = img.transpose(Image.Transpose.ROTATE_90)

# img.show()

img_resized = img.resize((400,240))

# img_resized.show()

# print(img_resized.height, img_resized.width)

#convert to black and white
img_resized = img_resized.convert("1")

img_resized.show()

pixels = list(img_resized.getdata())
print(pixels)


## now i need to convert to 0 and 1, and flip due to the storage method
for i in range(len(pixels)):
    if pixels[i] == 255:
        pixels[i] = 1
    elif pixels[i] == 0:
        pixels[i] = 0
    else:
        pixels[i] = 0

chararray = []

for i in range(0, len(pixels), 8):
    byte = 0
    chunk = pixels[i:i+8]
    while len(chunk) < 8:
        chunk.append(0)  # pad with 0s if not enough bits
    for bit in chunk:
        byte = (byte << 1) | bit
    chararray.append(byte)

with open(output_file, "w") as f:
    hex_strings = [f"0x{val:02X}" for val in chararray]  # format each int as 0x??
    f.write(", ".join(hex_strings))