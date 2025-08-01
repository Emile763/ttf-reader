# ttf-reader
Convert a .ttf file into usable texture
gcc -g -Wall -Wextra -Werror -pedantic-errors -Iinclude src/main.c src/misc/* src/tables/** -lglfw  -lGL -lGLEW