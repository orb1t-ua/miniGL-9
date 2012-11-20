//
//  main.cpp
//  glfwSimple.cpp
//
//  Created by Xian Xiao on 18/11/12.
//  Copyright (c) 2012  All rights reserved.
// 
//  Ref: http://www.lighthouse3d.com/cg-topics/code-samples/opengl-3-3-glsl-1-5-sample/
//

#define GLFW_INCLUDE_GL3 
#include <GL/glfw.h>

#include <stdio.h>
#include <iostream>
#include <math.h>

using namespace std;

//--------------------------------------------------------------------------------------
// vertex shader 
//--------------------------------------------------------------------------------------
static const char* pVS = "                                     \n \
#version 150                                                   \n \
                                                               \n \
uniform mat4 viewMatrix, projMatrix;                           \n \
in vec4 position;                                              \n \
in vec3 color;                                                 \n \
                                                               \n \
out vec3 Color;                                                \n \
                                                               \n \
void main(void)                                                \n \
{                                                              \n \
    Color = color;                                             \n \
    gl_Position = projMatrix * viewMatrix * position ;         \n \
}";

//--------------------------------------------------------------------------------------
// fragment shader
//--------------------------------------------------------------------------------------
static const char* pFS = "                                     \n \
#version 150                                                   \n \
                                                               \n \
in vec3 Color;                                                 \n \
out vec4 outputF;                                              \n \
                                                               \n \
void main(void)                                                \n \
{                                                              \n \
    outputF = vec4(Color,1.0);                                 \n \
}";

//--------------------------------------------------------------------------------------
// Vertex and Color Data for drawing Axis
//--------------------------------------------------------------------------------------

float verticesAxis[] = {
    -20.0f, 0.0f, 0.0f, 1.0f,
    20.0f, 0.0f, 0.0f, 1.0f,
    
    0.0f, -20.0f, 0.0f, 1.0f,
    0.0f,  20.0f, 0.0f, 1.0f,
    
    0.0f, 0.0f, -20.0f, 1.0f,
    0.0f, 0.0f,  20.0f, 1.0f
};

float colorAxis[] = {   
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f
};

//--------------------------------------------------------------------------------------
// Vertex and Color Data for drawing Unit Cube
//--------------------------------------------------------------------------------------

float cubes[] = {
    
    // face1 :
    0.000000, 0.000000, 1.000000, 1.0, // v1
    1.000000, 0.000000, 1.000000, 1.0, // v2 
    0.000000, 1.000000, 1.000000, 1.0, // v3
    
    // face2 :
    0.000000, 1.000000, 1.000000, 1.0, // v3
    1.000000, 0.000000, 1.000000, 1.0, // v2
    1.000000, 1.000000, 1.000000, 1.0, // v4 
    
    // face3 :
    0.000000, 1.000000, 1.000000, 1.0, // v3 
    1.000000, 1.000000, 1.000000, 1.0, // v4
    0.000000, 1.000000, 0.000000, 1.0, // v5
    
    // face4 :
    0.000000, 1.000000, 0.000000, 1.0, // v5
    1.000000, 1.000000, 1.000000, 1.0, // v4
    1.000000, 1.000000, 0.000000, 1.0, // v6
    
    // face5 : 
    0.000000, 1.000000, 0.000000, 1.0, // v5
    1.000000, 1.000000, 0.000000, 1.0, // v6 
    0.000000, 0.000000, 0.000000, 1.0, // v7 
    
    // face6 :
    0.000000, 0.000000, 0.000000, 1.0, // v7
    1.000000, 1.000000, 0.000000, 1.0, // v6 
    1.000000, 0.000000, 0.000000, 1.0, // v8
    
    // face7 : 
    0.000000, 0.000000, 0.000000, 1.0, // v7 
    1.000000, 0.000000, 0.000000, 1.0, // v8
    0.000000, 0.000000, 1.000000, 1.0, // v1
    
    // face8 :
    0.000000, 0.000000, 1.000000, 1.0, // v1
    1.000000, 0.000000, 0.000000, 1.0, // v8
    1.000000, 0.000000, 1.000000, 1.0, // v2
    
    // face9 :
    1.000000, 0.000000, 1.000000, 1.0, // v2
    1.000000, 0.000000, 0.000000, 1.0, // v8    
    1.000000, 1.000000, 1.000000, 1.0, // v4
    
    // face10 : 
    1.000000, 1.000000, 1.000000, 1.0, // v4
    1.000000, 0.000000, 0.000000, 1.0, // v8  
    1.000000, 1.000000, 0.000000, 1.0, // v6 
    
    // face11 :
    0.000000, 0.000000, 0.000000, 1.0, // v7 
    0.000000, 0.000000, 1.000000, 1.0, // v1
    0.000000, 1.000000, 0.000000, 1.0, // v5
    
    // face12 :
    0.000000, 1.000000, 0.000000, 1.0, // v5
    0.000000, 0.000000, 1.000000, 1.0, // v1
    0.000000, 1.000000, 1.000000, 1.0  // v3 
    
};

// Program and Shader Identifiers
GLuint p,v,f;

// Vertex Attribute Locations
GLuint vertexLoc, colorLoc;

// Uniform variable Locations
GLuint projMatrixLoc, viewMatrixLoc;

// Vertex Array Objects Identifiers
GLuint vao[2];

// storage for Matrices
float projMatrix[16];
float viewMatrix[16];

//--------------------------------------------------------------------------------------
// Cross product : res = a X b
//--------------------------------------------------------------------------------------
void crossProduct( float *a, float *b, float *res) 
{
    res[0] = a[1] * b[2]  -  b[1] * a[2];
    res[1] = a[2] * b[0]  -  b[2] * a[0];
    res[2] = a[0] * b[1]  -  b[0] * a[1];
}

//--------------------------------------------------------------------------------------
//  Normalize a vec3
//--------------------------------------------------------------------------------------
void normalize(float *a)
{    
    float mag = sqrt(a[0] * a[0]  +  a[1] * a[1]  +  a[2] * a[2]);
    
    a[0] /= mag;
    a[1] /= mag;
    a[2] /= mag;
}

//--------------------------------------------------------------------------------------
// sets the square matrix mat to the identity matrix,
// size refers to the number of rows (or columns)
//--------------------------------------------------------------------------------------
void setIdentityMatrix( float *mat, int size) 
{
    // fill matrix with 0s
    for (int i = 0; i < size * size; ++i)
        mat[i] = 0.0f;
    
    // fill diagonal with 1s
    for (int i = 0; i < size; ++i)
        mat[i + i * size] = 1.0f;
}

//--------------------------------------------------------------------------------------
//  Matrix multiplication a = a * b
//--------------------------------------------------------------------------------------
void multMatrix(float *a, float *b) 
{
    float res[16];
    
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            res[j*4 + i] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                res[j*4 + i] += a[k*4 + i] * b[j*4 + k];
            }
        }
    }
    memcpy(a, res, 16 * sizeof(float));
}

//--------------------------------------------------------------------------------------
//  Defines a transformation matrix mat with a translation
//--------------------------------------------------------------------------------------
void setTranslationMatrix(float *mat, float x, float y, float z) 
{    
    setIdentityMatrix(mat,4);
    mat[12] = x;
    mat[13] = y;
    mat[14] = z;
}

//--------------------------------------------------------------------------------------
//  Projection Matrix
//--------------------------------------------------------------------------------------
void buildProjectionMatrix(float fov, float ratio, float nearP, float farP)
{
    float f = 1.0f / tan (fov * (M_PI / 360.0));
    
    setIdentityMatrix(projMatrix,4);    
    projMatrix[ 0 ]       = f / ratio;
    projMatrix[1 * 4 + 1] = f;
    projMatrix[2 * 4 + 2] = (farP + nearP) / (nearP - farP);
    projMatrix[3 * 4 + 2] = (2.0f * farP * nearP) / (nearP - farP);
    projMatrix[2 * 4 + 3] = -1.0f;
    projMatrix[3 * 4 + 3] = 0.0f;
}
    
//--------------------------------------------------------------------------------------
// View Matrix
//
// note: it assumes the camera is not tilted,
// i.e. a vertical up vector (remmeber gluLookAt?)
//
//--------------------------------------------------------------------------------------
void setCamera(float posX, float posY, float posZ,
               float lookAtX, float lookAtY, float lookAtZ) 
{
    
    float dir[3], right[3], up[3];
    
    up[0] = 0.0f; up[1] = 1.0f; up[2] = 0.0f;
    
    dir[0] =  (lookAtX - posX);
    dir[1] =  (lookAtY - posY);
    dir[2] =  (lookAtZ - posZ);
    normalize(dir);
    
    crossProduct(dir,up,right);
    normalize(right);

    crossProduct(right,dir,up);
    normalize(up);
    
    float aux[16];
    
    viewMatrix[0]  = right[0];
    viewMatrix[4]  = right[1];
    viewMatrix[8]  = right[2];
    viewMatrix[12] = 0.0f;
    
    viewMatrix[1]  = up[0];
    viewMatrix[5]  = up[1];
    viewMatrix[9]  = up[2];
    viewMatrix[13] = 0.0f;
    
    viewMatrix[2]  = -dir[0];
    viewMatrix[6]  = -dir[1];
    viewMatrix[10] = -dir[2];
    viewMatrix[14] =  0.0f;
    
    viewMatrix[3]  = 0.0f;
    viewMatrix[7]  = 0.0f;
    viewMatrix[11] = 0.0f;
    viewMatrix[15] = 1.0f;
    
    setTranslationMatrix(aux, -posX, -posY, -posZ);
    
    multMatrix(viewMatrix, aux);
}

//--------------------------------------------------------------------------------------
//  Callback for changing window's size
//--------------------------------------------------------------------------------------
void changeSize(int w, int h)
{
    float ratio;
    
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    h =  (h == 0 ? 1 : h );
        
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    
    ratio = (1.0f * w) / h;
    buildProjectionMatrix(53.13f, ratio, 0.01f, 30.0f);
}

//--------------------------------------------------------------------------------------
//  setup VAO/VBO for display
//--------------------------------------------------------------------------------------
void setupBuffers()
{
    GLuint buffers[2];
    
    glGenVertexArrays(2, vao);
 
    // VAO for cubes: 
    glBindVertexArray(vao[0]);
    // Generate two slots for the vertex and color buffers
    glGenBuffers(2, buffers);
    
    // bind buffer for vertices and copy data into buffer
    
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubes), cubes, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertexLoc);
    glVertexAttribPointer(vertexLoc, 4, GL_FLOAT, 0, 0, 0);
    
    // bind buffer for colors and copy data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubes), cubes, GL_STATIC_DRAW);
    glEnableVertexAttribArray(colorLoc);
    glVertexAttribPointer(colorLoc, 4, GL_FLOAT, 0, 0, 0);
    
    // This VAO is for the Axis
    glBindVertexArray(vao[1]);
    // Generate two slots for the vertex and color buffers
    glGenBuffers(2, buffers);
    // bind buffer for vertices and copy data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesAxis), verticesAxis, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertexLoc);
    glVertexAttribPointer(vertexLoc, 4, GL_FLOAT, 0, 0, 0);
    
    // bind buffer for colors and copy data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorAxis), colorAxis, GL_STATIC_DRAW);
    glEnableVertexAttribArray(colorLoc);
    glVertexAttribPointer(colorLoc, 4, GL_FLOAT, 0, 0, 0);
    
}

//--------------------------------------------------------------------------------------
//  setUniforms for projection and viewing matrices. TODO: need model matrix 
//--------------------------------------------------------------------------------------
void setUniforms() 
{
    // must be called after glUseProgram
    glUniformMatrix4fv(projMatrixLoc,  1, false, projMatrix);
    glUniformMatrix4fv(viewMatrixLoc,  1, false, viewMatrix);
}

//--------------------------------------------------------------------------------------
//  core of rendering function
//--------------------------------------------------------------------------------------
void renderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setCamera( 3.5,2, 4, -2,-1,-2);
    
    glUseProgram(p);
    setUniforms();
    
    glBindVertexArray(vao[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36 );
    
    glBindVertexArray(vao[1]);
    glDrawArrays(GL_LINES, 0, 6);
}

//--------------------------------------------------------------------------------------
//  Callback for handle keyboard events. 
//--------------------------------------------------------------------------------------
void processNormalKeys(unsigned char key, int x, int y) 
{    
    if (key == 27) {
        glDeleteVertexArrays(3,vao);
        glDeleteProgram(p);
        glDeleteShader(v);
        glDeleteShader(f);
        exit(0);
    }
}

//--------------------------------------------------------------------------------------
//  Setup shader: creating, loading, linking and compiling, 
//   and setting up in/out of variables 
//--------------------------------------------------------------------------------------
GLuint setupShaders() 
{        
    GLuint p,v,f;
    
    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);
    
    glShaderSource(v, 1, &pVS,NULL);
    glShaderSource(f, 1, &pFS,NULL);
    
    glCompileShader(v);
    glCompileShader(f);
    
    p = glCreateProgram();
    glAttachShader(p,v);
    glAttachShader(p,f);
    
    glBindFragDataLocation(p, 0, "outputF");
    glLinkProgram(p);
    
    vertexLoc = glGetAttribLocation(p,"position");
    colorLoc = glGetAttribLocation(p, "color"); 
    
    projMatrixLoc = glGetUniformLocation(p, "projMatrix");
    viewMatrixLoc = glGetUniformLocation(p, "viewMatrix");
    
    return(p);
}

int main(int argc, char **argv) 
{
    int running = GL_TRUE;
    // Initialize GLFW
    if( !glfwInit() )
    {
        exit( EXIT_FAILURE );
    }
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Open an OpenGL window
    if( !glfwOpenWindow( 640, 480, 8,8,8,8,24,8, GLFW_WINDOW ) )
    {
        glfwTerminate();
        exit( EXIT_FAILURE );
    }
    
    p = setupShaders();
    setupBuffers();
        
    glClearColor(1.0,1.0,1.0,1.0);
    glEnable(GL_DEPTH_TEST );
    
    // Main loop
    while( running )
    {   
        // resize callback to setup viewing
        glfwSetWindowSizeCallback( changeSize );
        
        // core rendering:
        renderScene();
        
        // swap buffers:
        glfwSwapBuffers();
        
        // Check if ESC key was pressed or window was closed
        running = !glfwGetKey( GLFW_KEY_ESC ) &&
            glfwGetWindowParam( GLFW_OPENED );
    }
    
    // Close window and terminate GLFW
    glfwTerminate();
    // Exit program
    exit( EXIT_SUCCESS );
}
