#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

static GLfloat windowWidth  = 100.0f;  // world-coord half-width or height (depends on aspect)
static GLfloat windowHeight = 100.0f;

void RenderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 0.0f, 0.0f);

    glutSwapBuffers();
}

void SetupRC(void)
{
    // Clear color: blue
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}

void ChangeSize(int w, int h)
{
    if (h == 0) h = 1;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLfloat aspect = (GLfloat)w / (GLfloat)h;

    if (w <= h)
    {
        windowWidth  = 100.0f;
        windowHeight = 100.0f / aspect;
        glOrtho(-100.0, 100.0, -windowHeight, windowHeight, 1.0, -1.0);
    }
    else
    {
        windowWidth  = 100.0f * aspect;
        windowHeight = 100.0f;
        glOrtho(-windowWidth, windowWidth, -100.0, 100.0, 1.0, -1.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void SpecialKeys(int key, int x, int y)
{
    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Bounce (freeglut)");

    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutSpecialFunc(SpecialKeys);

    SetupRC();
    glutMainLoop();
    return 0;
}