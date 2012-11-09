//
//  main.cpp
//  GLSL Phong Shader 
//  shader code : http://www.ozone3d.net/tutorials/glsl_lighting_phong_p2.php
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
static const char* aVS = "                                                  \n \
varying vec3  normal;                                                       \n \
varying vec3  lightDir;                                                     \n \
varying vec3  eyeVec;                                                       \n \
                                                                            \n \
void main(void)                                                             \n \
{                                                                           \n \
    gl_Position    = ftransform();                                          \n \
    normal         = normalize(gl_NormalMatrix * gl_Normal);                \n \
    vec3 pos       = vec3( gl_ModelViewMatrix * gl_Vertex );                \n \
    lightDir       = vec3(gl_LightSource[0].position.xyz - pos);            \n \
    eyeVec         = -pos;                                                  \n \
}";     

//--------------------------------------------------------------------------------------
// fragment shader
//--------------------------------------------------------------------------------------
static const char* aFS = "                                                  \n \
varying vec3  normal;                                                       \n \
varying vec3  lightDir;                                                     \n \
varying vec3  eyeVec;                                                       \n \
                                                                            \n \
void main(void)                                                             \n \
{                                                                           \n \
    vec4 final_color =                                                      \n \
    (gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) +     \n \
    (gl_LightSource[0].ambient * gl_FrontMaterial.ambient);                 \n \
                                                                            \n \
    vec3 N = normalize(normal);                                             \n \
    vec3 L = normalize(lightDir);                                           \n \
                                                                            \n \
    float lambertTerm = dot(N,L);                                           \n \
                                                                            \n \
    if(lambertTerm > 0.0)                                                   \n \
    {                                                                       \n \
	final_color += gl_LightSource[0].diffuse *                          \n \
        gl_FrontMaterial.diffuse *                                          \n \
        lambertTerm;                                                        \n \
                                                                            \n \
	vec3 E = normalize(eyeVec);                                         \n \
	vec3 R = reflect(-L, N);                                            \n \
	float specular = pow( max(dot(R, E), 0.0),                          \n \
                             gl_FrontMaterial.shininess );                  \n \
	final_color += gl_LightSource[0].specular *                         \n \
        gl_FrontMaterial.specular *                                         \n \
        specular;                                                           \n \
    }                                                                       \n \
                                                                            \n \
    gl_FragColor = final_color;                                             \n \
}";

//--------------------------------------------------------------------------------------
// global variables
//--------------------------------------------------------------------------------------
int         g_dxWindow      = 800;
int         g_dyWindow      = 600;
GLuint      g_shaderProgram = 0;
int         g_mouseX        = 0;
int         g_mouseY        = 0;
float       g_cameraDist    = 5.f;
float       g_cameraAngleX  = 15.f;
float       g_cameraAngleY  = 0.f;
bool        g_rightDown     = false;
bool        g_leftDown      = false;

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
    
    // Move the object based on the mouse
    glTranslatef(0, 0, -g_cameraDist );
    glRotatef( g_cameraAngleX, 1, 0, 0 );
    glRotatef( g_cameraAngleY, 0, 1, 0 );

    glColor3f(0.0f, 1.0f, 0.0f );
    glutSolidTeapot(1.5);
    
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
    
    add_shader(g_shaderProgram, aVS, GL_VERTEX_SHADER);
    add_shader(g_shaderProgram, aFS, GL_FRAGMENT_SHADER);
    
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

//--------------------------------------------------------------------------------------
// Capture mouse screen position
//--------------------------------------------------------------------------------------
void mouse_capture(int button, int state, int x, int y)
{
    g_mouseX = x;
    g_mouseY = y;
    
    if( button == GLUT_LEFT_BUTTON )
    {
        if(state == GLUT_DOWN )     g_leftDown = true;
        else if(state == GLUT_UP )  g_leftDown = false;
    }
    else if( button == GLUT_RIGHT_BUTTON )
    {
        if( state == GLUT_DOWN )    g_rightDown = true;
        else if( state == GLUT_UP ) g_rightDown = false;
    }
}

//--------------------------------------------------------------------------------------
// Work out the mouse motion
//--------------------------------------------------------------------------------------
void mouse_motion(int x, int y)
{
    if(g_leftDown)
    {
        g_cameraAngleY += (x - g_mouseX);
        g_cameraAngleX += (y - g_mouseY);
        g_mouseX = x;
        g_mouseY = y;
    }
    
    if(g_rightDown)
    {
        g_cameraDist -= (y - g_mouseY) * 0.2f;
        g_mouseY = y;
    }
}

//--------------------------------------------------------------------------------------
// GL fixed functions to initialize a certain properties, materials, lighting etc.
//--------------------------------------------------------------------------------------
void init(void) 
{
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 80.0 };
    GLfloat light_position[] = { 1.0, 1.0, -1.0, 0.0 };
    glClearColor (0.1, 0.1, 0.1, 0.0);
    glShadeModel (GL_SMOOTH);
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    // Ambient light
    float As[4] = {0.0f, 1.0f, 1.0f, 1.0f };
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, As );
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    
    glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT, GL_NICEST );
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL );
    glutInitWindowPosition(10, 50);
    glutInitWindowSize(g_dxWindow, g_dyWindow);
    glutCreateWindow("GLSL Phong Shader Demo");
    init();
    glutMouseFunc(mouse_capture);
    glutMotionFunc(mouse_motion);
    glutDisplayFunc(display_handler);
    glutReshapeFunc(reshape_handler);
    glutIdleFunc( display_handler );
    
    // if glew initialised sucessfully, use GLSL shader
    if (glewInit() == GLEW_OK) 
    {
        compile_shaders();
    }
    glutMainLoop();
    
    return 0;
}
