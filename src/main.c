#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "misc/ttf_types.h"
#include "tables/table_offset_subtable.h"
#include "tables/table_directories.h"
#include "tables/table_head.h"
#include "tables/table_hhea.h"
#include "tables/table_maxp.h"
#include "tables/table_name.h"
#include "tables/table_cmap.h"
#include "tables/table_loca.h"
#include "tables/table_glyph.h"



const char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
const size_t number_of_char = sizeof(characters) - 1;
size_t glyph_index = 0;
int new_char = 0;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {

    // unused
    (void)window;
    (void)scancode;
    (void)mode;

    if (action == GLFW_PRESS) {
        // Slide through the characters 
        if (key == GLFW_KEY_LEFT) {
            glyph_index--;
            if (glyph_index == (size_t)(-1)) {
                glyph_index = number_of_char - 1;
            }
        }
        else if (key == GLFW_KEY_RIGHT) {
            glyph_index++;
            if (glyph_index == number_of_char) {
                glyph_index = 0;
            }
        }
        // Glyph index set to the characters of the key which is pressed
        else if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
            glyph_index = key - GLFW_KEY_A;
            if (mode != GLFW_MOD_SHIFT) {
                glyph_index = key - GLFW_KEY_A + 26;
            }
        }
        else if (key >= GLFW_KEY_KP_0 && key <= GLFW_KEY_KP_9) {
            glyph_index = key - GLFW_KEY_KP_0 + 52;
        }

        // When a key is pressed, inform the user
        if (key != GLFW_KEY_LEFT_SHIFT && key != GLFW_KEY_RIGHT_SHIFT)
            new_char = 1;
    }


}

// optional: cvt, fpgm, hdmx, kern, OS/2, prep
int main(int argc, char* argv[])
{

    assert(argc == 2);

    // open the font file given by the user
    int font = open(argv[1], O_RDONLY);
    if (errno != 0)
    {
        perror("Open font error: ");
        exit(EXIT_FAILURE);
    }

    // Read the offset_subtable 
    t_offset_subtable offset_subtable;
    readOffsetSubtable(font, &offset_subtable);

    // Init the table directories and read them
    // The directories are placed right after the offset_subtable in the font file
    t_table_directories table_directories;
    initTableDirectories(&table_directories);
    for (uint16_t i = 0; i < offset_subtable.numTables; i++)
    {

        // If the table is known, add it to the table_directories
        // else skip it
        uint32_t tag;
        read(font, &tag, sizeof(tag));
        e_tag tagEnum = tagToEnum(tag);
        if (tagEnum != TAG_UNKNOWN)
        {
            readTableDirectory(font, &table_directories.directories[tagEnum]);
            table_directories.is_present[tagEnum] = 1;
        }
        else
        {
            lseek(font, sizeof(t_table_directory), SEEK_CUR);
        }
    }

    // Check if all the required tags are presents
    int incomplete_font = 0;
    for (size_t i = 0; i < NUMBER_OF_REQUIRED_TAG; i++)
    {
        if (!table_directories.is_present[i])
        {
            incomplete_font = 1;
            printf("%.4s not found...\n", tagEToS[i]);
        }
    }
    if (incomplete_font)
    {
        exit(EXIT_FAILURE);
    }

    // Read The Head Table
    t_head head;
    readHeadTable(font, &head, &table_directories);
    printHeadTable(&head);

    // Read the Hhea table
    t_hhea hhea;
    readHheaTable(font, &hhea, &table_directories);
    printHheaTable(&hhea);

    // Read the Maxp table
    t_maxp maxp;
    readMaxpTable(font, &maxp, &table_directories);
    printMaxpTable(&maxp);

    // Read the name table
    t_name name;
    readNameTable(font, &name, &table_directories);
    printNameTable(&name);

    // Read the cmap table
    t_cmap cmap;
    readCmapTable(font, &cmap, &table_directories);
    printCmapTable(&cmap);

    // Read the loca table
    t_loca loca;
    readLocaTable(font, &loca, &table_directories, &head, &maxp);
    printLocaTable(&loca);

    // Init glfw and create a window
    if (glfwInit() == GLFW_FALSE) {
        exit(EXIT_FAILURE);
    }
    GLFWwindow* window = glfwCreateWindow(800, 800, "ttf demo", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Initialize opengl for the current context
    glfwMakeContextCurrent(window);
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Handle Users Inputs
    glfwSetKeyCallback(window, key_callback);

    // Vertex Array Objects for all the characters
    unsigned int VAOs[number_of_char];
    glGenVertexArrays(number_of_char, VAOs);
    // Vertex Buffer Objects for all the characters
    unsigned int VBOs[number_of_char];
    glGenBuffers(number_of_char, VBOs);
    // sizes of the data needed to draw each characters
    size_t sizes[number_of_char];

    // Get all the characters glyph points from the font files
    for (size_t i = 0; i < number_of_char; i++) {
        t_glyph glyph;
        readCharGlyph(font, &glyph, characters[i], &table_directories, &cmap, &loca);

        // constants need to resize the characters to fit the window
        FWord maxWidth = (glyph.xMax - glyph.xMin) / 2;
        FWord maxHeight = (glyph.yMax - glyph.yMin) / 2;
        FWord max_dim = maxWidth > maxHeight ? maxWidth : maxHeight;

        // Compute the size of the data stored in the glyphs (points coordinates)
        sizes[i] = 0;
        for (size_t j = 0; j < glyph.number_of_parts; j++) {
            sizes[i] += glyph.parts[j].numberOfPoints;
        }
        
        // Get all the points from all the parts of the centered and scaled glyph 
        size_t l = 0;
        float* points = (float*)malloc(2 * sizes[i] * sizeof(float));
        for(size_t j = 0; j < glyph.number_of_parts; j++) {
            for (size_t k = 0; k < glyph.parts[j].numberOfPoints; k++) {
                points[2 * l] = ((float)glyph.parts[j].xCoordinates[k] - glyph.xMin - maxWidth) / max_dim;
                points[2 * l + 1] = ((float)glyph.parts[j].yCoordinates[k] - glyph.yMin - maxHeight) / max_dim;
                l++;
            }
        }
        
        // Use the corresponding VAO an VBO to store data on GPU
        glBindVertexArray(VAOs[i]);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
        // Store data on GPU
        glBufferData(GL_ARRAY_BUFFER, sizes[i] * 2 * sizeof(float), points, GL_STATIC_DRAW);
        // Tel the GPU how to access the data
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // points are stored on GPU, not needed anymore on CPU
        free(points);
        
        printf("======= %c =======\n", characters[i]);
        printf("number of contours: %d\n", glyph.number_of_contours);
        // Free the data of the glyph, not needed anymore
        closeGlyph(&glyph);
    }

    size_t number_of_points_to_draw = 0;
    // Set the background color
    glClearColor(0.12, 0.12, 0.12, 1);
    // Main Loop stop when window is closed
    while (!glfwWindowShouldClose(window))
    {
        // pseudo periodicity
        usleep(10000);
        // Handle window events
        glfwPollEvents();

        // Set Clear the buffer in which the drawing is done
        glClear(GL_COLOR_BUFFER_BIT);

        // If a new characters is selected, use the corresponding VAO
        // Reset the number of points to be drawn
        if (new_char) {
            new_char = 0;
            number_of_points_to_draw = 0;
            printf("%zu\n", sizes[glyph_index]);
            glBindVertexArray(VAOs[glyph_index]);
        }
        

        // draw the points of the character until the number of points to draw
        glDrawArrays(GL_POINTS, 0, number_of_points_to_draw);

        // swap the images buffer to show the result on screen
        glfwSwapBuffers(window);

        // Increment the number of points to draw, to animate the result
        if (number_of_points_to_draw < sizes[glyph_index]) {
            number_of_points_to_draw++;
        }

    }


    // free everything
    closeLocaTable(&loca);
    closeCmapTable(&cmap);
    closeNameTable(&name);
    close(font);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}