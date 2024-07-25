from PIL import Image

spritePixels = Image.open("docs\screens\init_screen.png")

matrizRGB = []
rgb = []

for line in range(60):
  for col in range(80):
    coordinates = col, line
    rgbValues = spritePixels.getpixel(coordinates)
    rgb.append(rgbValues[:3])

for color in rgb:
    newColor = []
    for number in color:
      convertedRGB = int(number/32)
      newColor.append(convertedRGB)
    matrizRGB.append(newColor)

print(matrizRGB)
