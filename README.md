# Global Description
This project aims to create a simple C library for generating text textures from a .ttf font.

# Progress of the project
Curve points of a character can be gathered and displayed correctly. (tested with the font "Tagesschrift-Regular")


https://github.com/user-attachments/assets/3f6dcd17-b372-4d1a-bd03-7336140bd146


# Todo
- Interpolate the points using Bézier Curves.  
- Create an image by filling the inside of the characters.

# How To Compile
At the root of the repository:
```console
mkdir build
cd build
cmake ..
cmake --build .
# To launch
./TTF_Reader {a/font/directory.ttf}
```

# How To Use
The current app draws a character on the screen point by point. You can change the character using your keyboard.
