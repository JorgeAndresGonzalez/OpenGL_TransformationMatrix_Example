///This program is a simple rectangle in screen with color and texture
///that you can move with your mouse, using ModernOpenGL
///Author: Jorge Andres Gonzalez Borboa
///Following: https://learnopengl.com/#!Getting-started/Transformations

///You need GLFW||GLUT||freeGLUT and GLEW||GLAD to compile this example.
///Also stb_image.h to do the texture loading:
///stb_image-v2.16 - public domain image loader-http://nothings.org/stb_image.h
///As well as GLM to do all the linear algebra
///https://glm.g-truc.net/0.9.8/index.html

///Write this define before including GLEW
#define GLEW_STATIC

///Library for loading files, (jgp, jpeg, png, etc)
#define STB_IMAGE_IMPLEMENTATION

/// third-party libraries
#include <windows.h>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cassert>
#include <stdexcept>

///Use this if the header file is in the include folder
#include <stb_image.h>
///Use this if the header file is in the project
//#include "stb_image.h"


#include <iostream>

using namespace std;
using namespace glm;

#include "Shader.h"

///Data for the square
///we are drawing
GLfloat vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
};

///Corresponding indices
///for the triangles
///this info is for the
///EBO
unsigned int indices[] =
{
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

///\//////////////////////////DECLARATIONS//////////////////////////////////////

///Declare Window
GLFWwindow* window;

///Declare VAO, VBO and EBO
//VAO-Vertex Array Object
//VBO-Vertex Buffer Object
//EBO-Element Buffer Object
unsigned int VBO;
unsigned int VAO;
unsigned int EBO;

///Declare the Texture OpenGL Object
unsigned int iTexture;

///Declare the Second Texture
unsigned int  iTexture2;

///Declare the transformation matrix
mat4 transMat;

///Declare the position Offset of the square
float fX = 0;
float fY = 0;

///\////////////////////////////////////////////////////////////////////////////

///This is the callback function for when the window gets resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    ///Just adjust the viewport to the new window size
    glViewport(0, 0, width, height);
}

///This is the callback function for input data, keyboard, mouse etc
void processInput(GLFWwindow *window)
{
    ///If the 'esc' key was pressed
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        ///Close the window
        glfwSetWindowShouldClose(window, true);
    }

    ///If the 'right arrow' key was pressed
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        fX += 0.0005f;
    }

    ///If the 'left arrow' key was pressed
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        fX -= 0.0005f;
    }

    ///If the 'up arrow' key was pressed
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        fY += 0.0005f;
    }

    ///If the 'down arrow' key was pressed
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        fY -= 0.0005f;
    }

}

///\///////////////////INITIALIZE ALL THE FRAMEWORKS////////////////////////////
void initialize()
{
    ///Initialize GLFW
    if(glfwInit() == GLFW_TRUE)
    {
        cout << "GLFW initialized successfully" << endl;
    }
    else
    {
        cout << "Unable to initialize GLFW" << endl;
    }

    ///Set GLFW version to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    ///Uncomment this function for MAC-OS
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    ///Create a window
    window = glfwCreateWindow(800, 600, "ModernOpenGL", NULL, NULL);
    if (window == NULL)
    {
        ///If it fails, end the program
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
    }

    ///Set the current context, the window we are currently working in.
    glfwMakeContextCurrent(window);

    ///Register 'framebuffer_size_callback' as the resize window callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    ///Set the initial viewport
    glViewport(0, 0, 800, 600);

    ///Initialize GLEW
    if(glewInit() == GLEW_OK)
    {
        cout << "GLEW initialized successfully" << endl;
    }
    else
    {
        cout << "Unable to initialize GLEW" << endl;
    }


}
///\////////////////////////////////////////////////////////////////////////////


///\////////////////CREATION OF VAOs VBOs and EBOs//////////////////////////////
void setBufferObjects()
{
    ///Generate VBO
    glGenBuffers(1, &VBO);

    ///Generate VAO
    glGenVertexArrays(1, &VAO);

    ///Generate EBO
    glGenBuffers(1, &EBO);

    ///First Bind the VAO, so that all the configuration is saved in this VAO
    glBindVertexArray(VAO);

    ///Bind the VBO to GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    ///Bind EBO to GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    ///Populate VBO with data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                 GL_STATIC_DRAW);

    ///Populate EBO with data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    ///Set the info of how the VBO must be read
    /// position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    /// color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    /// texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void*)(6* sizeof(float)));
    glEnableVertexAttribArray(2);
}
///\////////////////////////////////////////////////////////////////////////////

void drawSquare(Shader s)
{
    ///Set the shader program
    s.use();

    ///Set the VAO
    glBindVertexArray(VAO);

    ///Draw
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void loadTextures(Shader s)
{

///\///////////////////////////FIRST TEXTURE////////////////////////////////////

    ///Generate the OpenGL texture object
    glGenTextures(1, &iTexture);

    ///Active the texture unit before binding
    glActiveTexture(GL_TEXTURE0);

    ///Bind our texture
    glBindTexture(GL_TEXTURE_2D, iTexture);

    ///Set the texture wrapping/filtering options
    ///(on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

     ///Load the texture data
    int width, height, nrChannels;
    unsigned char *data = stbi_load("textures/container.jpg", &width, &height,
                                    &nrChannels, 0);

    if(data)
    {
        ///Populate the object with data and generate its MipMap
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture data" << endl;
    }


    ///Free resources
    stbi_image_free(data);

///\///////////////////////SECOND TEXTURE///////////////////////////////////////

    ///Generate the second texture object
    glGenTextures(1,&iTexture2);

    ///Active the texture unit
    glActiveTexture(GL_TEXTURE1);

    ///Bind it
    glBindTexture(GL_TEXTURE_2D, iTexture2);

    ///Set the texture wrapping/filtering options
    ///(on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    ///Flip image
    stbi_set_flip_vertically_on_load(true);

    ///Load the texture data
    data = stbi_load("textures/face.png", &width, &height, &nrChannels, 0);

    if(data)
    {
        ///Populate the object with data and generate its MipMap
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture data" << endl;
    }

    ///Free resources
    stbi_image_free(data);

    ///SET THE UNIFORM DATA FOR THE FRAGMENT SHADER
    s.use();
    s.setInt("myTexture", 0);
    s.setInt("myTexture2", 1);

}

void setTransMat(Shader s, float fTime)
{
    ///Load Identity Matrix
    transMat = mat4();

    ///Translate it by the X and Y offsets
    transMat = translate(transMat, vec3(fX, fY, 0.0f));

    ///Rotate in the z-axis
    transMat = rotate(transMat, fTime, vec3(0.0, 0.0, 1.0));

    ///Scale x0.5 in all axis
    transMat = scale(transMat, vec3(0.5,0.5,0.5));

    ///Set Shader
    s.use();

    ///Set uniform -For the Vertex Shader
    s.setMatrix4fv("transMat",transMat);

}

int main ()
{
    ///Initialize all the frameworks
    initialize();

    ///Compile and Link Shaders into Shader Program
    Shader shader("shaders/vShader.vs","shaders/fShader.fs");

    ///Set all the info regarding buffer Objects
    setBufferObjects();

    ///Load Texture
    loadTextures(shader);

    ///Draw in Wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    ///Draw in Fillmode, this is default.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    ///Tell the user to use the arrow to move
    cout << "\n-----------------------------------" << endl;
    cout << "Use the arrows to move the square" << endl;
    cout << "-----------------------------------" << endl;

    ///This is the render loop *While the window is open*
    while(!glfwWindowShouldClose(window))
    {
        ///Set background color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ///Draw
        drawSquare(shader);

        ///Set the Transformation Matrix
        setTransMat(shader, (float) glfwGetTime());

        ///Process user input, in this case if the user presses the 'esc' key
        ///to close the application or the arrow keys to move the square
        processInput(window);

        ///Swap the Front and Back buffer.
        glfwSwapBuffers(window);

        ///Poll CallBack Events
        glfwPollEvents();
    }

    ///Free resources when application ends.
    glfwTerminate();
    return 0;
}
