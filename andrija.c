#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

static int window_width, window_height;

static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void testjumpfunc(int a);

static float xKocke = 1, yKocke = 1, zKocke = 0;
static int jumpFlag = 0;


int main(int argc, char **argv) {

    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100,100);
    glutCreateWindow(argv[0]);

    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    glClearColor(0.75, 0.75, 0.75, 0);
    glEnable(GL_DEPTH_TEST);
    glLineWidth(2);    

    glutMainLoop();


    return 0;
}

void triggerFunc(int arg) {

    zKocke += arg;
    
    if(arg<0)
     zKocke += 100;
    glutPostRedisplay();

}

//funkcija za skok
static void testjumpfunc(int a) {
    if(a == 2)
        return;

    glutTimerFunc(300, testjumpfunc, a+1);
    if(jumpFlag == 0){
        zKocke += 2;
        jumpFlag = 1;
    } else {
        zKocke -= 2;
        jumpFlag = 0;
    }
    glutPostRedisplay();
}

void on_reshape(int width, int height) {

    window_width = width;
    window_height = height;
}

static void on_keyboard(unsigned char key, int x, int y) {

    switch (key) {
    case 27:
        exit(0);
        break;
    case 32:
        //pritiskom na space bar poziva fju za skok
        testjumpfunc(0);
        break;
    case 'w':
        
        yKocke += 0.2;
        glutPostRedisplay();
        break;
    case 'd':
        
        xKocke += 0.2;
        glutPostRedisplay();
        break;
    case 'a':
        
        xKocke -= 0.2;
        glutPostRedisplay();
        break;
    case 's':
        yKocke -= 0.2;
        glutPostRedisplay();
        break;

    // case 'l' :

    //     //zKocke += 10;
    //     for(int i=0;i<100;i++)
    //         triggerFunc(0.1);
    //     //glutPostRedisplay();
    //     sleep(2);
    //     for(int i=0;i<100;i++)
    //         triggerFunc(-0.1);
    //      break;
    }

}

void on_display(void) {

    // sleep(2);
    // if(zKocke > 0) {
    //     zKocke -= 2;
    //     glutPostRedisplay();
    // }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Podesava se viewport. */
    glViewport(0, 0, window_width, window_height);

    /* Podesava se projekcija. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
            60,
            window_width/(float)window_height,
            1, 50);

     /* Podesava se tacka pogleda. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(

        xKocke+0.5, yKocke - 5, zKocke+3,
        xKocke+0.5, yKocke + 0.5, zKocke,
        0, 0, 1
    );

    glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex3f(0,0,0);
        glVertex3f(100,0,0);
        
        glColor3f(0,1,0);
        glVertex3f(0,0,0);
        glVertex3f(0,100,0);
        
        glColor3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,100);
    glEnd();


    glPushMatrix();

     glColor3f(0, 0, 1);
     glTranslatef(xKocke, yKocke, zKocke);
     glutSolidCube(1);

    glPopMatrix();

    


    glutSwapBuffers();
}