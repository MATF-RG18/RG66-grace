#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define TIMER_ID 0
#define QUAD_TIMER 1
#define TIMER_INTERVAL 20

#define CIRCLE_SEGMENTS 12
#define PI 3.1415926535

static int window_width, window_height;

static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void testjumpfunc(int a);

static float xKocke = 0.5, yKocke = 0, zKocke = 30;
static int jumpFlag = 0;

static float promenaZ = 0;
static float yQuada = 10;
static float koordinateMogZ[1000];
static float ugloviRotacija[1000];
static float rotacijePrepreka[100];
static int br = 0;
static int brojPreprekaTobogan = 0;
static float rot = 0;
static float proba = 0;

int xKoordinatePrepreke[] = {0, 1, 2, 3, 4};

static int animation_ongoing; 
static int igraUtoku;
static float xPrepreke = 2, yPrepreke = 50, zPrepreke = 0, yPozPrepreke = 50;


static float xRavniA = 2, yRavniA = 60, zRavniA=30-0.5, scaleXRavniA=5, scaleYRavniA=60, scaleZRavniA=0.3;

static float xRavniB = 2, yRavniB = 120, zRavniB=30-0.5, scaleXRavniB=5, scaleYRavniB=60, scaleZRavniB=0.3;



typedef struct {

    float xPrepreke, yPrepreke, zPrepreke;
    float leviX, desniX, prednjiY, zadnjY;
    int tipPrepreke;
    float ugraoRotacije;

}Prepreka;


static Prepreka nizRotiranih[200];
static Prepreka nizPreprekaA[200];
static Prepreka nizPreprekaB[200];
static int ukupnoPreprekaA = 0;
static int ukupnoPreprekaB = 0;


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

    animation_ongoing = 0;
    igraUtoku = 0;
    srand(time(NULL));

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

static void postaviRavanA() {

    Prepreka p;
    ukupnoPreprekaA = 0;
    

    for (int k=1;k<=scaleYRavniA;k+=5) {   /// hardkodovana ostahe duzina ravni, to sve da se izmeni

        int brPrepreka = rand() % 3 + 1;
        for(int j=0; j<brPrepreka;j++) {

                p.xPrepreke = xKoordinatePrepreke[rand() % 5];
                p.yPrepreke = yRavniA-(int)scaleYRavniA/2 + k;
                p.zPrepreke = 30;
                p.leviX = p.xPrepreke - 0.5;
                p.desniX = p.xPrepreke + 0.5;
                p.prednjiY = p.yPrepreke - 0.5;

                nizPreprekaA[ukupnoPreprekaA++] = p;
        }

    }
}

static void postaviRavanB() {

    Prepreka p;
    ukupnoPreprekaB = 0;
    

    for (int k=1;k<=scaleYRavniB;k+=5) {   /// hardkodovana ostahe duzina ravni, to sve da se izmeni

        int brPrepreka = rand() % 3 + 1;
        for(int j=0; j<brPrepreka;j++) {

                p.xPrepreke = xKoordinatePrepreke[rand() % 5];
                p.yPrepreke = yRavniB-(int)scaleYRavniB/2 + k;
                p.zPrepreke = 30;
                p.leviX = p.xPrepreke - 0.5;
                p.desniX = p.xPrepreke + 0.5;
                p.prednjiY = p.yPrepreke - 0.5;

                nizPreprekaB[ukupnoPreprekaB++] = p;
        }

    }
}

static void osveziRavanA() {

    for(int i=0;i<ukupnoPreprekaA;i++) {

        nizPreprekaA[i].yPrepreke -= 0.1;
        nizPreprekaA[i].prednjiY = nizPreprekaA[i].yPrepreke - 0.5;
        //printf("%f ", fabs((yKocke + 0.5) - nizPreprekaA[15].prednjiY));
        if(fabs((yKocke + 0.5) - nizPreprekaA[i].prednjiY) < 0.0002) {
            
            if((xKocke - 0.5 < nizPreprekaA[i].desniX && xKocke - 0.5 > nizPreprekaA[i].leviX) || 
                (xKocke + 0.5 < nizPreprekaA[i].desniX && xKocke + 0.5 > nizPreprekaA[i].leviX)) {
                    //printf("ovo je nas prednji %f, a ovo je prednji od prepreke %f, a abs razlika je %f",
                    yKocke+0.5, nizPreprekaA[i].prednjiY, fabs((yKocke + 0.5) - nizPreprekaA[i].prednjiY);
                    animation_ongoing = 0;
                }
        }
    }
}

static void osveziRavanB() {

    for(int i=0;i<ukupnoPreprekaB;i++) {

        nizPreprekaB[i].yPrepreke -= 0.1;
        nizPreprekaB[i].prednjiY = nizPreprekaB[i].yPrepreke - 0.5;
        if(yKocke + 0.5 == nizPreprekaB[i].prednjiY) {
            //printf("usoooooo u ravan");
            if((xKocke - 0.5 <= nizPreprekaB[i].desniX && xKocke - 0.5 >= nizPreprekaB[i].leviX) || 
                (xKocke + 0.5 <= nizPreprekaB[i].desniX && xKocke + 0.5 >= nizPreprekaB[i].leviX)) {
                    animation_ongoing = 0;
                }
        }

    }
}

static void postaviTobogan() {
    brojPreprekaTobogan = 0;
    Prepreka p;
    for(int i=0;i<60;i+=5) {

        int brPrepreka = rand() % 3 + 1;
        for(int j=0; j<brPrepreka;j++) {
        
            p.xPrepreke = xKoordinatePrepreke[rand() % 5];
            p.yPrepreke = yQuada + i;
            p.zPrepreke = 30.5 + 0.8*sin(i*0.8);
            p.leviX = p.xPrepreke - 0.5;
            p.desniX = p.xPrepreke + 0.5;
            p.prednjiY = p.yPrepreke - 0.5;
            p.ugraoRotacije = rotacijePrepreka[i];
            nizRotiranih[brojPreprekaTobogan++] = p;
        }
    }
}

static void pomeriQuad(int value) {

    if(value != QUAD_TIMER)
        return;
    yQuada -= 0.1;
    if(yQuada <= 0) {
        int poz = -10*yQuada;
        //printf(" pozicija u nizu %d ", poz);
        zKocke = koordinateMogZ[poz];
        rot = ugloviRotacija[poz] * PI/180;
        proba = ugloviRotacija[poz];
        //printf("%f,  ", rot);
    }
    if(yQuada < -99) {
        yQuada = 10;
        zKocke = 30;
        rot=0;
        postaviTobogan();
    }
    for(int i=0;i<brojPreprekaTobogan;i++) {
        nizRotiranih[i].yPrepreke -= 0.1;
    }
    glutPostRedisplay();

    if (animation_ongoing) {
        glutTimerFunc(TIMER_INTERVAL, pomeriQuad, QUAD_TIMER);
    }

}

static void pomeriRavni(int value) {

    if(value != TIMER_ID)
        return;
    yQuada -= 0.1;
    yRavniA-=0.1;
    yRavniB -= 0.1;
    if(yQuada <= 0) {
        int poz = -10*yQuada;
        //printf(" pozicija u nizu %d ", poz);
        zKocke = koordinateMogZ[poz];
        rot = ugloviRotacija[poz];
    }
    if(yQuada < -50) {
        yQuada = 10;
        zKocke = 30;
        rot=0;
    }
    
    promenaZ++;
    osveziRavanA();
    osveziRavanB();
    if(yRavniA + (int)scaleYRavniA/2 < 0) {
        yRavniA = 90;
        postaviRavanA();
    }
    if(yRavniB + (int)scaleYRavniB/2 < 0) {
        yRavniB = 90;
        postaviRavanB();
    }
    glutPostRedisplay();

    if (animation_ongoing) {
        glutTimerFunc(TIMER_INTERVAL, pomeriRavni, TIMER_ID);
    }
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

    case 'g':
    case 'G':
        /* Pokrece se animacija. */
        if (!animation_ongoing) {
            igraUtoku = 1;
            glutTimerFunc(TIMER_INTERVAL, pomeriQuad, QUAD_TIMER);
            animation_ongoing = 1;
        }
        break;
    case 'f' :

        animation_ongoing = 0;
    }
    

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

        xKocke + 0.5, yKocke - 5, zKocke + 2,
        xKocke+0.5, yKocke, zKocke,
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
    glPushMatrix();

     glColor3f(0, 0, 1);
     glTranslatef(xKocke, yKocke, zKocke);
     if (rot < 0)
        glRotatef(-rot, -1, 0, 0);
     else
        glRotatef(rot,1, 0, 0);

     glutSolidCube(1);

    glPopMatrix();


    // glPushMatrix();

    //     glTranslatef(xRavniA, yRavniA, zRavniA);
    //     glColor3f(1, 0, 0);
    //     glScalef(scaleXRavniA, scaleYRavniA, scaleZRavniA);
    //     if(!igraUtoku) {
    //         postaviRavanA();
    //         postaviRavanB();
    //     }
    //     glutSolidCube(1);
        
    // glPopMatrix();

    // glPushMatrix();

    //     glTranslatef(xRavniB, yRavniB, zRavniB);
    //     glColor3f(1, 0, 0);
    //     glScalef(scaleXRavniB, scaleYRavniB, scaleZRavniB);
    //     glutSolidCube(1);
        
    // glPopMatrix();

     glPushMatrix();

        glColor3f(0, 0, 1);
        glTranslatef(1, 1, zKocke);
        glutWireSphere(20, 300, 300);

     glPopMatrix();

    // for (int i=0;i<ukupnoPreprekaA;i++) {

    //     glPushMatrix();

    //         glColor3f(0, 1, 0);
    //         glTranslatef(nizPreprekaA[i].xPrepreke, nizPreprekaA[i].yPrepreke, nizPreprekaA[i].zPrepreke);
    //         glutSolidCube(1);
    //     glPopMatrix();

    // }

    // for (int i=0;i<ukupnoPreprekaB;i++) {

    //     glPushMatrix();

    //         glColor3f(0, 1, 0);
    //         glTranslatef(nizPreprekaB[i].xPrepreke, nizPreprekaB[i].yPrepreke, nizPreprekaB[i].zPrepreke);
    //         glutSolidCube(1);
    //     glPopMatrix();

    //}

    glPushMatrix();
        br = 0;
       // glColor3f(1, 0, 0);
        glBegin(GL_QUAD_STRIP);
            int k = 0;
            for(float i=0;i<=100;i+=0.1) {
               // printf("%f  ", i);
               
                glColor3f(0, 0, 0);
                glVertex3d(0, yQuada + i, 30 + sin(i*0.4));
                glVertex3d(5, yQuada + i, 30 + sin(i*0.4));
                koordinateMogZ[br] = 30.5 + sin(i*0.4);

               float x = i*0.4;
               ugloviRotacija[br++] = cos(x);
               if(fabs(i - 0) < 0.1 || fabs(i - 5) < 0.1 || fabs(i - 10) < 0.1   || fabs(i - 15) < 0.1   || fabs(i - 20) < 0.1 
                    || fabs(i - 25) < 0.1  || fabs(i - 30) < 0.1  || fabs(i - 35) < 0.1  || fabs(i - 40) < 0.1  || fabs(i - 45) < 0.1 ) {
                   //printf("%d-%f ", (int)i, i);
                   rotacijePrepreka[k] = cos(x);
                   k += 5;
               }
            }

        glEnd();

    glPopMatrix();

    for(int i=0;i<brojPreprekaTobogan;i++) {

        glPushMatrix();

            glColor3f(0, 1, 0);     
            glTranslatef(nizRotiranih[i].xPrepreke, nizRotiranih[i].yPrepreke, nizRotiranih[i].zPrepreke);
            glRotatef(nizRotiranih[i].ugraoRotacije * PI/180, 1, 0, 0);
            //nizRotiranih[i].ugraoRotacije * 360/2*PI
            glutSolidCube(1);

        glPopMatrix();

        }

    glutSwapBuffers();
}