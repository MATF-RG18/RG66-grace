#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>

#define TIMER_ID 0
#define CYLINDER_TIMER 2
#define QUAD_TIMER 1
#define TIMER_INTERVAL 20
#define TIMER_SKERANJE 10

#define CIRCLE_SEGMENTS 12
#define PI 3.1415926535

static int window_width, window_height;

static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);


void pomeriQuad(int value);

void iscrataj_sinusoidu(double);

void iscrataj_kocku();

void iscrtaj_motor();

void skreni_desno(int value);
void skreni_levo(int value);

typedef struct {

    float xPrepreke, yPrepreke, zPrepreke;
    float leviX, desniX, prednjiY, zadnjY;
    int tipPrepreke;
    float ugraoRotacije;

}Prepreka;

int animation_ongoing = 0;

float y_sinusoide = 10;

float x_kocke, y_kocke, z_kocke;

float X = 2;

float rand_factor = 0.8;

float pomeraj_sinusoide = 0;

float niz_z[1000];
float niz_rotacija[1000];

float ugrao_rotacije = 0;
int br_vrednosti = 0; 

int trenutni_element = 0;

float eyeX, eyeY, eyeZ;

//float look_x, look_y, look_z;

float ugoa_desno = 0;
float ugao_levo = 0;
int animacija_skretanja_levo = 0;
int animacija_skretanja_desno = 0;



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

    srand(time(NULL));

    x_kocke = 2;
    y_kocke = 0.5;
    z_kocke = 0.5;

    eyeX = 2;
    eyeY = y_kocke - 1.5;
    eyeZ =  z_kocke + 1.2;

    glutMainLoop();


    return 0;
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
    case 'w':
        
        y_kocke += 0.2;
        
        glutPostRedisplay();
        break;
    case 'd':
        
        X += 0.7;
        eyeX += 0.7;
        if(!animacija_skretanja_desno && !animacija_skretanja_levo) {
            animacija_skretanja_desno = 1;
            glutTimerFunc(TIMER_SKERANJE, skreni_desno, CYLINDER_TIMER);
        }
        glutPostRedisplay();
        break;
    case 'a':
        
        X -= 0.7;
        eyeX -= 0.7;
        if(!animacija_skretanja_levo && !animacija_skretanja_desno) {
            animacija_skretanja_levo = 1;
            glutTimerFunc(TIMER_SKERANJE, skreni_levo, CYLINDER_TIMER);
        }
        glutPostRedisplay();
        break;
    case 's':
        y_kocke -= 0.2;
        glutPostRedisplay();
        break;

    case 'g':
    case 'G':
        /* Pokrece se animacija. */
        if (!animation_ongoing) {
            animation_ongoing = 1;
            glutTimerFunc(TIMER_INTERVAL, pomeriQuad, TIMER_ID);
        }
        break;
    case 'f' :

        animation_ongoing = 0;
    }
    

}

void pomeriQuad(int value) {
    if (value != TIMER_ID)
        return;

    y_sinusoide -= 0.1;

    if(y_sinusoide <= 0.5) {

        ugrao_rotacije = niz_rotacija[trenutni_element];
        //ugrao_rotacije += 1 % 360;
        //printf("%f ", ugrao_rotacije);
        z_kocke = niz_z[trenutni_element++] + 0.2;
        eyeZ = z_kocke + 2;//1.3;
    }

    if(y_sinusoide + 90 <= 0) {
        trenutni_element = 0;
        ugrao_rotacije = 0;
        y_sinusoide = 10;
    }

    glutPostRedisplay();

    if(animation_ongoing) 
        glutTimerFunc(TIMER_INTERVAL, pomeriQuad, TIMER_ID);
}


void on_display(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Podesava se viewport. */
    glViewport(0, 0, window_width, window_height);

    /* Podesava se projekcija. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
            60,
            window_width/(float)window_height,
            1, 30);

     /* Podesava se tacka pogleda. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(

        //2, -8, 5,
        eyeX, eyeY  - 0.8, eyeZ ,
        eyeX, eyeY + 10, eyeZ,
        0, 0, 1
    );

    glPushMatrix();

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

    glPopMatrix();

   // glPushMatrix();

        iscrataj_kocku();

   // glPopMatrix();

    iscrtaj_motor();

    glPushMatrix();
    rand_factor = 0.8;
    iscrataj_sinusoidu(rand_factor);
    glPopMatrix();

    glutSwapBuffers();
}

void iscrataj_sinusoidu(double rand_factor) {

    glColor3f(0, 0, 1);
    glBegin(GL_QUAD_STRIP);

        br_vrednosti = 0;
        
        for (float i=0;i<=100;i+=0.1) {

            
            niz_z[br_vrednosti] = sin(i * rand_factor);
            niz_rotacija[br_vrednosti++] = cos(i * rand_factor);

            glVertex3f(0, y_sinusoide + i, sin(i * rand_factor));
            glVertex3f(5, y_sinusoide + i, sin(i * rand_factor));
        }


    glEnd();


}

void iscrataj_kocku() {

    glPushMatrix();
        glTranslatef(X, y_kocke, z_kocke);

        glRotatef(ugrao_rotacije, 1, 0, 0);
        glColor3f(1, 0, 0);
        glutWireCube(1);
    glPopMatrix();

}

void iscrtaj_motor() {

    glPushMatrix();
    glColor3f(1, 0, 0);
    if(animacija_skretanja_desno)
        glRotatef(-15*sin(ugoa_desno * PI/180), 0, 0, 1);
    else if(animacija_skretanja_levo) {
        glRotatef(-15*sin(ugao_levo * PI/180), 0, 0, -1);
    }
    //glRotatef(ugrao_rotacije * 180/PI, 1, 0, 0);
    glPushMatrix();
        
        glTranslatef(X, y_kocke + 0.5, z_kocke);
        glRotatef(90, 0, 1, 0);
        glutSolidTorus(0.1, 1, 50, 50);

    glPopMatrix();

    glPushMatrix();
        
        glTranslatef(X, y_kocke + 0.3, z_kocke + 0.3);
        glRotatef(45, 1, 0, 0);
        glRotatef(90, 0, 1, 0);
        glScalef(1, 0.2, 0.2);
        glutSolidCube(1);

    glPopMatrix();

    glPushMatrix();
        
       
        glTranslatef(X - 1, y_kocke + 1, z_kocke + 0.5);
        glRotatef(90, 0, 1, 0);
        GLUquadric * guvernal = gluNewQuadric();
        gluCylinder(guvernal, 0.2, 0.2, 2, 30, 30);

    glPopMatrix();

    glPushMatrix();
        glColor3f(0, 1, 0);
        glTranslatef(X - 0.8, y_kocke + 0.3, z_kocke + 0.6);
        glRotatef(5, 0, 0, 1);
        glRotatef(60, 1, 0, 0);
        GLUquadric * leva_ruka = gluNewQuadric();
        gluCylinder(leva_ruka, 0.2, 0.2, 2, 30, 30);


    glPopMatrix();

        glPushMatrix();
        glColor3f(0, 1, 0);
        glTranslatef(X + 0.8, y_kocke + 0.3, z_kocke + 0.6);
        glRotatef(-5, 0, 0, 1);
        glRotatef(60, 1, 0, 0);
        GLUquadric * desna_ruka = gluNewQuadric();
        gluCylinder(desna_ruka, 0.2, 0.2, 2, 30, 30);


    glPopMatrix();

    glPopMatrix();

}

void skreni_desno(int value) {

    if(value != CYLINDER_TIMER)
        return;

    ugoa_desno += 5;

    if(ugoa_desno >= 180) {
        //ugoa_desno += 1;

        //if(ugoa_desno >= 0) {
            ugoa_desno = 0;
            animacija_skretanja_desno = 0;
        //}
    }
        
    
    glutPostRedisplay();

    if(animacija_skretanja_desno)
        glutTimerFunc(TIMER_INTERVAL, skreni_desno, CYLINDER_TIMER);

}
void skreni_levo(int value) {

    if(value != CYLINDER_TIMER)
        return;

    ugao_levo += 5;

    if(ugao_levo >= 180) {
        ugao_levo = 0;
        animacija_skretanja_levo = 0;
    }
        
    
    glutPostRedisplay();

    if(animacija_skretanja_levo)
        glutTimerFunc(TIMER_INTERVAL, skreni_levo, CYLINDER_TIMER);

}