#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

class glwidget : public QGLWidget
{
    Q_OBJECT
public:
    glwidget(QGLFormat &format, QWidget *parent = 0);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    
signals:
    
public slots:

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    //void mousePressEvent(QMouseEvent *event);
    //void mouseMoveEvent(QMouseEvent *event);


    void setUniforms();


    void setupBuffers();
    GLuint setupShaders();

    void crossProduct( float *a, float *b, float *res);
    void normalize(float *a);
    void setIdentityMatrix( float *mat, int size);
    void multMatrix(float *a, float *b);
    void setTranslationMatrix(float *mat, float x, float y, float z);
    void setRotationMatrix( float *mat, float angle, const short axis );
    void buildProjectionMatrix(float fov, float ratio, float nearP, float farP);
    void setCamera(float posX, float posY, float posZ,
                   float lookAtX, float lookAtY, float lookAtZ);

    // Program and Shader Identifiers
    GLuint p,v,f;

    // Vertex Attribute Locations
    GLuint vertexLoc, colorLoc;

    // Uniform variable Locations
    GLuint modelMatrixLoc, projMatrixLoc, viewMatrixLoc;

    // Vertex Array Objects Identifiers
    GLuint vao[2];

    // Mouse movement variables:
    int         g_mouseX        ;
    int         g_mouseY        ;
    float       g_cameraDist    ;
    float       g_cameraAngleX  ;
    float       g_cameraAngleY  ;

    // storage for Matrices
    float projMatrix[16];
    float viewMatrix[16];

    static const char* pVS;
    static const char* pFS;



};

#endif // GLWIDGET_H
