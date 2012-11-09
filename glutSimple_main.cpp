//
//  main.cpp
//  glutSimple
//
//  Created by Xian Xiao on 8/11/12.
//  Copyright (c) 2012  All rights reserved.
//
#include <GLUT/glut.h>

int g_dxWindow = 800;
int g_dyWindow = 600;

void ReshapeHandler(int w, int h)
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

void DisplayHandler(void)
{
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // Push to save the current matrix 
    glPushMatrix();
    glTranslatef(0, 0, -5.0);
    // Display
    glColor3f(0.0f, 1.0f, 0.0f );
    glutWireTeapot(1.0);
    // Pop to recover 
    glPopMatrix();
    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL );
    glutInitWindowPosition(10, 50);
    glutInitWindowSize(g_dxWindow, g_dyWindow);
    glutCreateWindow("GLUT Simplest Demo");
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glutDisplayFunc(DisplayHandler);
    glutReshapeFunc(ReshapeHandler);
    glutMainLoop();    
    return 0;
}