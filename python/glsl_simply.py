#!/usr/bin/env python
import sys
import math

try:
    from OpenGL.GL import *
    from OpenGL.GL.shaders import *
    from OpenGL.GLU import *
    from OpenGL.GLUT import *
    from OpenGL.GLUT.freeglut import *
    from OpenGL.arrays import vbo
except ImportError:
    app = QtGui.QApplication(sys.argv)
    QtGui.QMessageBox.critical(None, "OpenGL hellogl",
                            "PyOpenGL must be installed to run this example.",
                            QtGui.QMessageBox.Ok | QtGui.QMessageBox.Default,
                            QtGui.QMessageBox.NoButton)
    sys.exit(1)
    
import Image
import numpy    

# vertex shader
pVS = """

void main(void) 
{ 
    gl_Position = ftransform(); 
}

"""
# fragment shader
pFS = """

void main(void) 
{
   gl_FragColor = vec4( 0.2,0.3,0.4,1);
}
"""

class GLClass:
    def __init__(self):
        self.Splash = True    #There's actually more here, but it's impertinent
        self.g_dxWindow = 800
        self.g_dyWindow = 600
        self.g_mouseX = 0
        self.g_mouseY = 0
        self.g_cameraDist = 5.0
        self.g_cameraAngleX = 15
        self.g_cameraAngleY = 0
        self.g_rightDown = False
        self.g_leftDown = False
    def run(self):
        glutInitDisplayMode(GLUT_RGBA)
        glutInitWindowSize( self.g_dxWindow, self.g_dyWindow )
        self.window = glutCreateWindow("GLSL Phong")
        ver = glGetString( GL_VERSION )
        print( ver )
        ver = glGetString( GL_SHADING_LANGUAGE_VERSION )
        print( ver )
        glutReshapeFunc(self.reshape)
        glutDisplayFunc(self.draw)
        self.g_shaderProgram = compileProgram( compileShader( pVS, GL_VERTEX_SHADER ), 
                                              compileShader( pFS, GL_FRAGMENT_SHADER ) )
        
        glutMainLoop()
    def reshape(self, width, height):
        self.g_dxWindow = width
        self.g_dyWindow = height
        glViewport( 0, 0, self.g_dxWindow, self.g_dyWindow );
        glMatrixMode( GL_PROJECTION);
        glLoadIdentity();
        ratio = self.g_dxWindow / self.g_dyWindow
        gluPerspective( 60, ratio, 1., 1000 )
        glMatrixMode( GL_MODELVIEW )
        glLoadIdentity()
        
    def draw(self):
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT)
        glPushMatrix()
        glTranslate( 0, 0, -self.g_cameraDist )
        glColor3f( 0, 1, 0 )
        glUseProgram( self.g_shaderProgram )
        glutSolidTeapot( 1. )
        glPopMatrix()            
        glutSwapBuffers()

glutInit(sys.argv)
test = GLClass()
test.run()

