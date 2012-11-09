//
//  main.cpp
//  GLSLSimple
//
//  Created by Xian Xiao on 9/11/12.
//  Copyright (c) 2012  All rights reserved.
//

#include <GL/glew.h>
#include <GLUT/glut.h>
#include <string.h>
#include <iostream>
using namespace std;

//--------------------------------------------------------------------------------------
// vertex shader 
//--------------------------------------------------------------------------------------
static const char* pVS = "                                     \n \
void main(void)                                                \n \
{                                                              \n \
    gl_Position = ftransform();                                \n \
}";

//--------------------------------------------------------------------------------------
// fragment shader
//--------------------------------------------------------------------------------------
static const char* pFS = "                                     \n \
void main(void)                                                \n \
{                                                              \n \
    gl_FragColor = vec4(1,0,0,1);                              \n \
}";

//--------------------------------------------------------------------------------------
// global variables
//--------------------------------------------------------------------------------------
int         g_dxWindow      = 800;
int         g_dyWindow      = 600;
float       g_rotate        = 0;
GLuint      g_shaderProgram = 0;

//--------------------------------------------------------------------------------------
// function callback for reshaping window
//--------------------------------------------------------------------------------------
void reshape_handler(int w, int h)
{
    // Stores the width and height
    g_dxWindow = w;
    g_dyWindow = h;
    
    // Viewport transform
    glViewport(0, 0, (GLsizei)g_dxWindow, (GLsizei)g_dyWindow);
    
    // Perspective projection transform
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // FOV, AspectRatio, NearClip, FarClip
    gluPerspective(60.0f, (float)(g_dxWindow)/g_dyWindow, 1.0f, 1000.0f); 
    
    // Switch to modelview matrix in order to set scene
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//--------------------------------------------------------------------------------------
// function callback for core render
//--------------------------------------------------------------------------------------
void display_handler(void)
{
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // Push to save the current matrix
    glPushMatrix();
    
    glTranslatef(0, 0, -5.0);
    
    g_rotate = ( g_rotate >= 360 ? 0 : g_rotate + 0.01 );
    glRotatef( g_rotate, 0, 1, 0 );

    glColor3f(0.0f, 1.0f, 0.0f );
    glutWireTeapot(1.0);
    //glutSolidTeapot(1.0);
    
    // Pop to recover
    glPopMatrix();
    glutSwapBuffers();
}

//--------------------------------------------------------------------------------------
// add shader 
//--------------------------------------------------------------------------------------
void add_shader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
    GLuint ShaderObj = glCreateShader(ShaderType);
    if (ShaderObj == 0) 
    {
        cout<<"Error creating shader type : "<<ShaderType<<endl;
        return;
    }
    
    const GLchar* p[1];
    p[0] = pShaderText;
    GLint Lengths[1];
    Lengths[0]= strlen(pShaderText);
    glShaderSource(ShaderObj, 1, p, Lengths);
    glCompileShader(ShaderObj);
    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        cout<<"Error compiling shader type : "<<ShaderType<<" : "<<InfoLog<<endl; 
        return;
    }
    
    glAttachShader(ShaderProgram, ShaderObj);
}

//--------------------------------------------------------------------------------------
// create, add, link, compile shaders
//--------------------------------------------------------------------------------------
static void compile_shaders()
{
    g_shaderProgram = glCreateProgram();
    
    if (g_shaderProgram == 0)
    {
        cout<<"Error creating shader program! "<<endl;
        return;
    }
    
    add_shader(g_shaderProgram, pVS, GL_VERTEX_SHADER);
    add_shader(g_shaderProgram, pFS, GL_FRAGMENT_SHADER);
    
    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };
    
    glLinkProgram(g_shaderProgram);
    glGetProgramiv(g_shaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0) 
    {
        glGetProgramInfoLog(g_shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        cout<< "Error linking shader program : "<<ErrorLog<<endl;
        return; 
    }
    
    glValidateProgram(g_shaderProgram);
    glGetProgramiv(g_shaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) 
    {
        glGetProgramInfoLog(g_shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        cout<<"Invalid shader program: "<< ErrorLog<<endl;
        return;
    }
    
    glUseProgram(g_shaderProgram);
    
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL );
    glutInitWindowPosition(10, 50);
    glutInitWindowSize(g_dxWindow, g_dyWindow);
    glutCreateWindow("GLSL Simplest Demo");
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glutDisplayFunc(display_handler);
    glutReshapeFunc(reshape_handler);
    glutIdleFunc( display_handler );
    
    // if glew initialised sucessfully, use GLSL shader
    GLenum res = glewInit();
    if (res == GLEW_OK) 
    {
        compile_shaders();
    }
    glutMainLoop();
    
    return 0;
}
