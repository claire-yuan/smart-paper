""" Python script to visualize the printed data from TSC2046 serial output. """

from PIL import Image

# Function to read the file and return coordinates
def read_coordinates(file_path):
    with open(file_path, 'r') as f:
        coordinates = []
        for line in f:
            # Remove any surrounding whitespace and split by the comma
            x, y = line.strip().split(',')
            coordinates.append((int(x), int(y)))
    return coordinates

# Function to create the grid and mark the coordinates
def create_grid(coordinates):
    # Create a blank white image with dimensions 2047x2047
    img = Image.new('RGB', (2048, 2048), color='white')  # 2048 because of 0-indexing
    
    # Mark the specified coordinates as black
    for x, y in coordinates:
        img.putpixel((x, (2047-y)), (0, 0, 0))  # Set the pixel to black

        # Set the surrounding pixels too
        pen_width = 2;
        for i in range(-pen_width, pen_width):
            for j in range (-pen_width, pen_width):
                if 0 <= x+i <= 2047 and 0 <= (2047-y)+j <= 2047:
                    img.putpixel((x+i, (2047-y)+j), (0, 0, 0))  # Set the pixel to black

    return img

# Path to your input file
file_path = 'test_data_03-23/full_screen_test_1.csv'

# Read the coordinates from the file
coordinates = read_coordinates(file_path)

# Create the grid and mark the coordinates
image = create_grid(coordinates)

# Save the image or show it
image.save(file_path + '.png')
image.show()
