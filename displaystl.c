#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "readstl.h"

static GLfloat windowWidth  = 100.0f;  // world-coord half-width or height (depends on aspect)
static GLfloat windowHeight = 100.0f;

static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

uint32_t numTriangles;
struct Triangle* triangles = NULL;

static GLfloat xCenter = 0.0f;
static GLfloat yCenter = 0.0f;
static GLfloat zCenter = 0.0f;

void RenderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);

    glPushMatrix();
    glTranslatef(-xCenter, -yCenter, -zCenter);
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);

    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_TRIANGLES);
    for (uint32_t i = 0; i < numTriangles; i++) {
        glNormal3fv(triangles[i].normal);
        glVertex3fv(triangles[i].vertex1);
        glVertex3fv(triangles[i].vertex2);
        glVertex3fv(triangles[i].vertex3);
    }
    glEnd();

    glPopMatrix();

    glutSwapBuffers();
}

void SetupRC(void)
{
    // Clear color: blue
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    /* Lighting setup */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat ambientLight[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight[]  = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightPos[]      = { 100.0f, 100.0f, 100.0f, 1.0f }; // positional light

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    /* Let glColor* calls set the material diffuse color */
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    /* Set default specular and shininess */
    GLfloat matSpecular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0f);
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
        glOrtho(-100.0, 100.0, -windowHeight, windowHeight, -1000.0, 1000.0);
    }
    else
    {
        windowWidth  = 100.0f * aspect;
        windowHeight = 100.0f;
        glOrtho(-windowWidth, windowWidth, -100.0, 100.0, -1000.0, 1000.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void SpecialKeys(int key, int x, int y)
{
    const GLfloat rotationStep = 5.0f;

    switch (key) {
        case GLUT_KEY_UP:
            xRot -= rotationStep;
            break;
        case GLUT_KEY_DOWN:
            xRot += rotationStep;
            break;
        case GLUT_KEY_LEFT:
            yRot -= rotationStep;
            break;
        case GLUT_KEY_RIGHT:
            yRot += rotationStep;
            break;
    }

    // Reset angles if they exceed 360 or go below 0
    if (xRot >= 360.0f) xRot -= 360.0f;
    if (xRot < 0.0f) xRot += 360.0f;
    if (yRot >= 360.0f) yRot -= 360.0f;
    if (yRot < 0.0f) yRot += 360.0f;
    glutPostRedisplay();
}

void loadSTL()
{
    // numTriangles = readBinSTL("Bunny_Binary.stl", &triangles);
    numTriangles = readAsciiSTL("Bunny_ASCII.stl", &triangles);
    printf("Number of triangles read: %u\n", numTriangles);
    
    // Re-calculate the normal vectors
    for (uint32_t i = 0; i < numTriangles; i++) {
        GLfloat u[3], v[3], n[3];
        for (int j = 0; j < 3; j++) {
            u[j] = triangles[i].vertex2[j] - triangles[i].vertex1[j];
            v[j] = triangles[i].vertex3[j] - triangles[i].vertex1[j];
        }
        n[0] = u[1]*v[2] - u[2]*v[1];
        n[1] = u[2]*v[0] - u[0]*v[2];
        n[2] = u[0]*v[1] - u[1]*v[0];
        GLfloat length = sqrtf(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
        if (length > 0.0f) {
            triangles[i].normal[0] = n[0] / length;
            triangles[i].normal[1] = n[1] / length;
            triangles[i].normal[2] = n[2] / length;
        } else {
            triangles[i].normal[0] = 0.0f;
            triangles[i].normal[1] = 0.0f;
            triangles[i].normal[2] = 0.0f;
        }
    }

    // Compute the center of the model
    if (numTriangles > 0) {
        GLfloat xMin = triangles[0].vertex1[0];
        GLfloat xMax = triangles[0].vertex1[0];
        GLfloat yMin = triangles[0].vertex1[1];
        GLfloat yMax = triangles[0].vertex1[1];
        GLfloat zMin = triangles[0].vertex1[2];
        GLfloat zMax = triangles[0].vertex1[2];

        for (uint32_t i = 1; i < numTriangles; i++) {
            for (int j = 0; j < 3; j++) {
                xMin = fminf(xMin, triangles[i].vertex1[j]);
                xMax = fmaxf(xMax, triangles[i].vertex1[j]);
                yMin = fminf(yMin, triangles[i].vertex1[j]);
                yMax = fmaxf(yMax, triangles[i].vertex1[j]);
                zMin = fminf(zMin, triangles[i].vertex1[j]);
                zMax = fmaxf(zMax, triangles[i].vertex1[j]);
            }
        }

        xCenter = (xMin + xMax) / 2.0f;
        yCenter = (yMin + yMax) / 2.0f;
        zCenter = (zMin + zMax) / 2.0f;
        printf("Model center: (%f, %f, %f)\n", xCenter, yCenter, zCenter);
    }
}

int main(int argc, char* argv[])
{
    loadSTL();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Display STL");

    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutSpecialFunc(SpecialKeys);

    SetupRC();
    glutMainLoop();
    return 0;
}