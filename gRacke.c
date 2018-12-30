#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include "image.h"

#define TIMER_ID 0
#define CYLINDER_TIMER 2
#define QUAD_TIMER 1
#define TIMER_INTERVAL 20
#define TIMER_SKRETANJE 10
#define STEP 100

#define TIMER_ZA_SPIRALU 10
#define ID_SPIRALE 3 

#define TIMER_ZA_RUPU 10
#define ID_RUPE 4

#define CIRCLE_SEGMENTS 12
#define PI 3.1415926535

#define FILENAME0 "mud2.bmp"
#define FILENAME1 "desert.bmp"
#define FILENAME2 "sky.bmp"
//#define FILENAME3 "horizont.bmp"
static GLuint names[3];

static void initialize(void);

static int window_width, window_height;

static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_realese(unsigned char key, int x, int y);


void pomeriQuad(int value);

static void iscrtaj_sinusoidu(double);
static void iscrtaj_ravan();
static void iscrtaj_motor();
static void iscrtaj_prepreke();
static void iscrtaj_spiralu();
static void iscrtaj_rupu();
static void iscrtaj_metak(int);

static void iscrtaj_sarzer();

static void timer_spirala(int);
static float parametar_spirale = 0;
static int animacija_spirale = 0;


static void teleportuj_se();


static void timer_rupe(int);
static int animacija_rupe = 0;
static float parametar_rupe = 0;

void skreni_desno(int value);
void skreni_levo(int value);

typedef struct {

    float xPrepreke, yPrepreke, zPrepreke;
    float leviX, desniX, prednjiY, zadnjY;
    int tipPrepreke;
    float ugraoRotacije;
    int pogodjena;

}Prepreka;

typedef struct {

    float x, y, z;
    int ispaljen;

}Metak;

typedef struct Vertex
{
    double position[3];
    double texcoord[2];
    double normal[3];
} Vertex;

typedef struct VertRef
{
    int v, vt, vn;
} VertRef;


static Vertex *model;
static int model_size = 0;

Vertex *LoadObj(FILE *);



float brzina_metka = 0.5;
float scale_metka = 1;

static float distance(Prepreka p);

static void kolizija();
static void kolizija_metak(Metak m);

Prepreka niz_prepreka[1000];
static int moguci_tipovi_prepreka[] = {0, 1, 2, 3, 4}; //feder, kocka, teleport, rupa
static int niz_mogucih_x[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};


static int br_prepreka = 0;
static float koordinate_prepreka_na_sinusoidi[]  = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95};
int br_koordinata = 10;

static int animation_ongoing = 0;

static float y_sinusoide = 10;
static float x_kocke, y_kocke, z_kocke;
static float X = 2;

static float rand_factor = 0.8;

static float niz_z[1000];
static float niz_rotacija[1000];

static float ugrao_rotacije = 0;
static int br_vrednosti = 0; 

static int trenutni_element = 0;

static float eyeX, eyeY, eyeZ;

//ovde su za skretanje
static float ugoa_desno = 0;
static float ugao_levo = 0;
static int animacija_skretanja_levo = 0;
static int animacija_skretanja_desno = 0;

static float rotation = 0;
static float rotation_desno = 0;


static int imune = 0;

static int score = 0;
static int skriveni_skor = 0;


static int prvi_put = 0;


static float y_ravni = 120;
static float z_ravni;



static Metak sarzer[5];
static int redni_metak = 5;
static Metak m;


static void postavi_teksturu_pustinje(void);
static void postavi_teksturu_neba(void);
static void postavi_teksturu_horizonta(void);


int main(int argc, char **argv) {

    

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100,100);
    glutCreateWindow(argv[0]);

    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
    glutKeyboardUpFunc(on_realese);

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

    //glutFullScreen();
    FILE * file = fopen("./andra.obj", "r");
    if(file == NULL) 
    {
        printf("Los fajl");
        return 1;
    }

    model = LoadObj(file);

    initialize();

    glutMainLoop();


    return 0;
}

void on_reshape(int width, int height) {

    window_width = width;
    window_height = height;
   // br_prepreka = 0;
}


static void on_realese(unsigned char key, int x, int y) {

    switch (key) {

        case 'd' :
        case 'D' :

            animacija_skretanja_desno = 0;
            ugoa_desno = 0;
            animacija_skretanja_desno = 0;
            eyeY = y_kocke - 1.5;
            eyeX = X;
            rotation_desno = 0;
            break;

        case 'a' :
        case 'A' :

            ugao_levo = 0;
            eyeY = y_kocke - 1.5;
            eyeX = X;
            animacija_skretanja_levo = 0;
            rotation = 0;

            break;
    }



}
static void on_keyboard(unsigned char key, int x, int y) {

    switch (key) {
    case 27:
        exit(0);
        break;
    case 'w':
    case 'W':
        
        y_kocke += 0.2;
        
        glutPostRedisplay();
        break;
    case 'd':
    case 'D' :

        if(X >= 8.7)
            return;

        X += 0.2;
        eyeX += 0.2;
         if(!animacija_skretanja_desno && !animacija_skretanja_levo) {
             animacija_skretanja_desno = 1;
             glutTimerFunc(TIMER_INTERVAL, skreni_desno, CYLINDER_TIMER);
         }
        glutPostRedisplay();
        break;
    case 'a':
    case 'A' :

        if(X <= 0.3)
            return;
        
        X -= 0.2;
        eyeX -= 0.2;
        if(!animacija_skretanja_levo && !animacija_skretanja_desno) {
            animacija_skretanja_levo = 1;
            glutTimerFunc(TIMER_INTERVAL, skreni_levo, CYLINDER_TIMER);
        }
        glutPostRedisplay();
        break;
    case 's':
    case 'S' :
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
    case 32 :

        if(redni_metak < 5) {

            m.x = eyeX;
            m.y = eyeY;
            m.z = eyeZ;
            m.ispaljen = 1;

            sarzer[redni_metak++] = m;
        }

        break;
    }

    

}

void pomeriQuad(int value) {
    if (value != TIMER_ID)
        return;

    skriveni_skor++;
    if(skriveni_skor == 10) {
        score++;
        skriveni_skor = 0;
    }
    if(imune > 0) {
        imune--;
    }
    y_sinusoide -= 0.1;
    y_ravni -= 0.1;

    for(int i=0;i<br_prepreka;i++) {
        niz_prepreka[i].yPrepreke -= 0.1;
    }
    kolizija();
    if(y_sinusoide <= 0.5) {

        if(y_sinusoide + 100 <= 0) {
            z_kocke = z_ravni;
        }
        
        else
        {
            ugrao_rotacije = niz_rotacija[trenutni_element];
        }
        
        
        //ugrao_rotacije += 1 % 360;
        //printf("%f ", ugrao_rotacije);
        if(animacija_spirale)
        {
            trenutni_element++;
            //z_kocke = 0;
            //eyeZ = z_kocke;
        }
        else if(animacija_rupe) {
            animation_ongoing = 0;
        }
        else {
            z_kocke = niz_z[trenutni_element++] + 0.2;
            eyeZ = z_kocke + 2;
        }
        
        //1.3;
    }

    if(y_ravni + 10 <= 0) {
        y_ravni = 110;
    }

    if(y_sinusoide + 100 <= 0) {
        trenutni_element = 0;
        ugrao_rotacije = 0;
        y_sinusoide = 20;
        br_prepreka = 0;
        //y_ravni = 120;
    }

    glutPostRedisplay();

    if(animation_ongoing) 
        glutTimerFunc(TIMER_INTERVAL, pomeriQuad, TIMER_ID);
}


void on_display(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Podesava se viewport. */
    glViewport(0, 0, window_width, window_height);
    //printf("%d----%d\n", window_width, window_height);
    //glRasterPos3f(window_width - window_width/4.0, 10, window_height - window_height/2.0);
    


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
        X, y_kocke + 10, 2,
        0, 0, 1
    );

    // glPushMatrix();
    // {

    //     glBegin(GL_TRIANGLES);
    //         for(int i=0; i<model_size; i++){
    //                 glNormal3f(model[i].normal[0], model[i].normal[1], model[i].normal[2]);
    //                 glVertex3f(model[i].position[0], model[i].position[1], model[i].position[2]);
    //         } 
    //     glEnd();
        
    // }
    // glPopMatrix();

    postavi_teksturu_pustinje();
    postavi_teksturu_neba();
    postavi_teksturu_horizonta();

    for(int i=0;i<redni_metak;i++) {

        iscrtaj_metak(i);
        kolizija_metak(sarzer[i]);

    }

    glRasterPos3f(eyeX + 5, 10, eyeZ + 3);
    char score_display[50] = "SCORE : ";
    char string_score[50];
    sprintf(string_score, "%d", score);
    strcat(score_display, string_score);
    int len = (int) strlen(score_display);

    for(int i=0; i<len;i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, score_display[i]);
    }
    glRasterPos3f(eyeX - 5, 10, eyeZ + 3);
    char preostalo_metkova[50] = "SARZER : ";
    char meci[50];
    sprintf(meci, "%d", 5 - redni_metak);
    strcat(preostalo_metkova, meci);
    int len2 = (int) strlen(preostalo_metkova);

    for(int i=0; i<len2;i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, preostalo_metkova[i]);
    }

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

    iscrtaj_motor();

    glPushMatrix();
    rand_factor = 0.8;

    iscrtaj_sinusoidu(rand_factor);
    iscrtaj_ravan();
    iscrtaj_prepreke();
    
    glPopMatrix();

    glutSwapBuffers();
}

void iscrtaj_sinusoidu(double rand_factor) {


    glBindTexture(GL_TEXTURE_2D, names[0]);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUAD_STRIP);

    glNormal3f(1, 0, 0);

        

        br_vrednosti = 0;
        
        float prva_kord = 0, druga_kord = 1, uvecanje = 0;
        
        for (float i=0;i<=100;i+=0.1) {



                // 0 0    1 0
                // 0 0.1  1 0.1
                // 0 0.2  1 0.2
            
            niz_z[br_vrednosti] = sin(i * rand_factor);
            niz_rotacija[br_vrednosti++] = cos(i * rand_factor);
            glTexCoord2f(prva_kord, uvecanje); 
            glVertex3f(0, y_sinusoide + i, sin(i * rand_factor));

            glTexCoord2f(druga_kord, uvecanje); 
            glVertex3f(9, y_sinusoide + i, sin(i * rand_factor));

            

            uvecanje += 0.1;
            if(uvecanje == 1)
                uvecanje = 0;
            //printf("%f--%d--%d\n", y_sinusoide, prvi_put, animation_ongoing);
            if (fabs(y_sinusoide - 10) < 0.01 && (prvi_put == 0 || animation_ongoing) ) {
                
                z_ravni = sin(100 * rand_factor);

                for (int j=0;j<19;j++) {

                    if( fabs(i - koordinate_prepreka_na_sinusoidi[j]) < 0.01) {
                        //printf("usoo\n");
                        int broj_prepreka = (int)rand() % 3 + 1;
                        int niz_zauzetih[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
                        int broj_pogodaka = 0;
                        while(broj_pogodaka < broj_prepreka) {

                            Prepreka p;
                            int k = 100;
                            while(k == 100  || niz_zauzetih[k]) {
                                k = niz_mogucih_x[(int)rand() % 9];
                            }
                            broj_pogodaka++;
                            niz_zauzetih[k] = 1;
                            p.pogodjena = 0;
                            p.xPrepreke = k;
                            p.tipPrepreke = moguci_tipovi_prepreka[(int)rand() % 5];
                            p.yPrepreke = y_sinusoide + i;
                            p.zPrepreke = sin(i * rand_factor) + 0.5;
                            p.ugraoRotacije = cos(i * rand_factor);
                            niz_prepreka[br_prepreka++] = p;

                        }
                    }

                }
            }

        }

        
        prvi_put = 1; //zbog reshape u prvom
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);

}

static void iscrtaj_ravan() {

    glPushMatrix();

          glBindTexture(GL_TEXTURE_2D, names[0]);
          glEnable(GL_TEXTURE_2D);
         

          int prvi = 0, drugi = 0;
          glBegin(GL_QUAD_STRIP);
          glNormal3f(0, 0, 1);
        for(int i=0;i<=24;i+=2) {
            glTexCoord2f(prvi, drugi);
            prvi = (prvi + 1) % 2;
             glVertex3f(0, y_ravni - 12 + i, z_ravni + 0.7);
            glTexCoord2f(prvi, drugi);
            prvi = (prvi + 1) % 2;
            drugi = (drugi + 1) % 2;
             glVertex3f(9, y_ravni - 12 + i, z_ravni + 0.7);
         }

         glEnd();
         glDisable(GL_TEXTURE_2D);
         glBindTexture(GL_TEXTURE_2D, 0);
        
        
        glTranslatef(4, y_ravni, z_ravni);
        glScalef(8, 20, 1);
        glutSolidCube(1);

    glPopMatrix();

}

void iscrtaj_prepreke() {


    for(int i=0; i<br_prepreka;i++) {
        Prepreka p = niz_prepreka[i];
        glPushMatrix();
        glTranslatef(p.xPrepreke, p.yPrepreke, p.zPrepreke);
        
        if (p.tipPrepreke == 0 && !(p.pogodjena)) {
            glPushMatrix();
            glColor3f(0, 1, 0);
            glRotatef(p.ugraoRotacije * 180/PI, 1, 0, 0);
            glutSolidCube(1);
            glPopMatrix();
        }
        else if (p.tipPrepreke == 1) {
            glPushMatrix();
            glTranslatef(0, 0, -1.5);
            glRotatef(90, 1, 0, 0);
            glColor3f(0, 1, 0);
            iscrtaj_spiralu();
            glPopMatrix();
        }
        else if (p.tipPrepreke == 2) {
            glPushMatrix();
            glTranslatef(0, 0, 0.5);
            glRotatef(p.ugraoRotacije * 180/PI, 1, 0, 0);
            glRotatef(180, 1, 0, 0);
            glColor3f(0, 1, 0);
            iscrtaj_spiralu();
            glPopMatrix();
            //iscratj_teleport(p.yPrepreke);
        }
        else if (p.tipPrepreke == 3) {
            glRotatef(p.ugraoRotacije * 180/PI, 1, 0, 0);
            glPushMatrix();
                iscrtaj_rupu();
            glPopMatrix();
        }
        else if(p.tipPrepreke == 4) {

            glPushMatrix();
                iscrtaj_sarzer();
            glPopMatrix();
        }
        //glutSolidCube(1);
        glPopMatrix();

    }
}

void iscrtaj_motor() {

    glPushMatrix();
    glColor3f(1, 0, 0);
    if(animacija_skretanja_desno)
        glRotatef(-15*sin(ugoa_desno * PI/180), 0, 0, 1);
    else if(animacija_skretanja_levo) {
        //glTranslatef(-1, 0, 0);
        //eyeX + 0.5;
        glRotatef(15*sin(ugao_levo * PI/180), 0, 0, 1);
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

    if(ugoa_desno < 90)
        ugoa_desno += 5;
    rotation_desno += 0.1;

    //eyeX =  eyeX - 0.01 * cos((rotation) * PI/180);
    //eyeY =  eyeY - 0.01 * sin((rotation) * PI/180);

    if(ugoa_desno >= 90) {

            // ugoa_desno = 0;
            // animacija_skretanja_desno = 0;
            // eyeY = y_kocke - 1.5;
            // eyeX = X;
            // rotation_desno = 0;

    }
        
    
    glutPostRedisplay();

    if(animacija_skretanja_desno)
        glutTimerFunc(TIMER_INTERVAL, skreni_desno, CYLINDER_TIMER);

}
void skreni_levo(int value) {

    if(value != CYLINDER_TIMER)
        return;

    if(ugao_levo < 90)
        ugao_levo += 5;
    rotation += 3;

    //eyeX =  eyeX + 0.01 *cos(rotation * PI/180);
    //eyeY =  eyeY + 0.01 * sin(rotation * PI/180);

    // if(ugao_levo >= 180) {
    //     ugao_levo = 0;
    //     eyeY = y_kocke - 1.5;
    //     eyeX = X;
    //     animacija_skretanja_levo = 0;
    //     rotation = 0;
    // }
        
    
    glutPostRedisplay();

    if(animacija_skretanja_levo)
        glutTimerFunc(TIMER_INTERVAL, skreni_levo, CYLINDER_TIMER);

}

static void iscrtaj_spiralu() {
    glPushMatrix();
        //glTranslatef(x_obst,y_obst,z_obst);
        GLfloat x = 0,y=0.8,z=0,angle, r  = .0001;
        glBegin(GL_LINE_STRIP);
        glLineWidth(7);
        for(angle = 0; angle < 1400; angle += 1)
        {   
            x = r * cos(angle * PI/180);
            z = r * sin(angle * PI/180);
            glVertex3f(x,y,z);
            y+=0.0006;
            r+= .0005;
        }
        glEnd();
    glPopMatrix();
}

static void iscrtaj_rupu() {

    //glDisable(GL_LIGHTING);
    glPushMatrix();
        glColor3f(0,0,0);
        glTranslatef(0,0, -0.5);
        glScalef(1, 1, 0.1);
        
        glutSolidCube(1);
    glPopMatrix();
    //glEnable(GL_LIGHTING);
}

static float distance(Prepreka p)
{
    float x = powf((p.xPrepreke - X),2);
    float y = powf((p.yPrepreke - y_kocke),2);
    float z = powf((p.zPrepreke - z_kocke),2);

    return sqrtf(x + y + z);
}

static void kolizija() {

    if(imune)
        return;

    for (int i=0;i<br_prepreka;i++) {

        Prepreka p = niz_prepreka[i];
        if(distance(p) <= 1.02 && !(p.pogodjena)) {
            if(p.tipPrepreke == 0) {
                animation_ongoing = 0;
            }
            else if(p.tipPrepreke == 1) {
                if(!animacija_spirale) {
                   animacija_spirale = 1;
                   score += 5;
                    glutTimerFunc(10, timer_spirala, ID_SPIRALE);
                }
            }
            else if(p.tipPrepreke == 2) {
                teleportuj_se();
            }
            else if(p.tipPrepreke == 3) {
                if(!animacija_rupe) {
                    animacija_rupe = 1;
                    glutTimerFunc(TIMER_ZA_RUPU, timer_rupe, ID_RUPE);
                }
            }
            else if(p.tipPrepreke == 4) {
                redni_metak = 0;
            }
        }

    }

}
static void kolizija_metak(Metak m) {

    for (int i=0; i<br_prepreka;i++) {
        Prepreka p = niz_prepreka[i];
        if(fabs(p.xPrepreke - m.x) < 0.5 && fabs(p.yPrepreke - m.y +scale_metka) < 0.2 
        &&  fabs(p.zPrepreke - m.z) < 0.5)   {
            niz_prepreka[i].pogodjena = 1;
            // && fabs(p.yPrepreke - m.y +scale_metka) < 0.01
                                         //&& fabs(p.zPrepreke - m.z) < 0.5) {
                                             niz_prepreka[i].pogodjena = 1;
                                         }

    }

}

static void timer_spirala(int value)
{
    if(value != ID_SPIRALE)
        return;
    
    parametar_spirale += 5;

    if(parametar_spirale >=180){
        parametar_spirale = 0;
        animacija_spirale = 0;
    }

    z_kocke = 15*sin(parametar_spirale * PI/180)+1;
    eyeZ = z_kocke + 3;

    glutPostRedisplay();

    if(animacija_spirale)
        glutTimerFunc(20,timer_spirala,ID_SPIRALE);   
}

static void teleportuj_se() {
    score += 5;
    imune = 101;
    X = niz_mogucih_x[(int)rand() % 5];
    eyeX = X;
}

static void timer_rupe (int value)
{
    if(value != ID_RUPE)
        return;

    parametar_rupe += 1;
    if(parametar_rupe > 90){
        animacija_rupe = 0;
        parametar_rupe = 0;
    }
    z_kocke = -15*sin(parametar_rupe * PI/180);
    eyeZ = z_kocke + 3;

    glutPostRedisplay();
    if(animacija_rupe)
        glutTimerFunc(20,timer_rupe,ID_RUPE);
}

static void initialize(void)
{
    /* Objekat koji predstavlja teskturu ucitanu iz fajla. */
    Image * image;

    /* Ukljucuju se teksture. */
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);

    /*
     * Inicijalizuje se objekat koji ce sadrzati teksture ucitane iz
     * fajla.
     */
    // 0 0    1 0
    // 0 0.1  1 0.1
    // 0 0.2  1 0.2
    image = image_init(0, 0);

    /* Kreira se prva tekstura. */
    

    /* Generisu se identifikatori tekstura. */
    glGenTextures(2, names);

    image_read(image, FILENAME0);

    glBindTexture(GL_TEXTURE_2D, names[0]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    image_read(image, FILENAME1);

    glBindTexture(GL_TEXTURE_2D, names[1]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    image_read(image, FILENAME2);

    glBindTexture(GL_TEXTURE_2D, names[2]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);


    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Unistava se objekat za citanje tekstura iz fajla. */
    image_done(image);

    /* Inicijalizujemo matricu rotacije. */
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
    // glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
}

static void postavi_teksturu_pustinje() {

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, names[1]);

        glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 0);
        glVertex3f(-50, 0, -2);

        glTexCoord2f(5, 0);
        glVertex3f(50, 0, -2);

        glTexCoord2f(5, 5);
        glVertex3f(50, 50, -2);

        glTexCoord2f(0, 5);
        glVertex3f(-50, 50, -2);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    
}

static void postavi_teksturu_horizonta() {

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, names[1]);
        glBegin(GL_QUADS);
        glNormal3f(1, 0, 0);

        glTexCoord2f(0, 0);
        glVertex3f(-50, 23, -2);

        glTexCoord2f(10, 0);
        glVertex3f(50, 23, -2);

        glTexCoord2f(1, 1);
        glVertex3f(50, 28, 1.5);

        glTexCoord2f(0, 1);
        glVertex3f(-50, 28, 1.5);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

static void postavi_teksturu_neba() {

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, names[2]);

        glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 0);
        glVertex3f(-50, 0, 50);

        glTexCoord2f(1, 0);
        glVertex3f(50, 0, 50);

        glTexCoord2f(1, 1);
        glVertex3f(50, 23, 1.5);

        glTexCoord2f(0, 1);
        glVertex3f(-50, 23, 1.5);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

static void iscrtaj_metak(int redni_broj) {

        if(sarzer[redni_broj].y >= 35) {
            return;
        }

        glPushMatrix();

        glColor3f(0, 0, 1);
        glLineWidth(3);
        glBegin(GL_LINES);

            Metak m1 = sarzer[redni_broj];

            glVertex3f(m1.x, m1.y, m1.z);
            glVertex3f(m1.x, m1.y + scale_metka, m1.z);

            m1.y += brzina_metka;
            sarzer[redni_broj] = m1;
            
            

        glEnd();

    glPopMatrix();
}

static void iscrtaj_sarzer() {

    glPushMatrix();

        glColor3f(0.2, 0.2, 0.2);
        glRotatef(35, 0, 1, 0);
        glScalef(0.3, 0.3, 1.2);
        glutSolidCube(1);
    glPopMatrix();

}

Vertex* LoadObj(FILE * file){
    int verts_count = 0;
    int verts_count_of = STEP;
    Vertex *verts = malloc(verts_count_of * sizeof(Vertex));
    int num_of_pos = STEP;
    double **positions = malloc(num_of_pos * sizeof(double*));
    for(int i=0; i<num_of_pos; i++){
        positions[i] = malloc(3 * sizeof(double));
    }
    positions[0][0] = 0;
    positions[0][1] = 0;
    positions[0][2] = 0;

    int num_of_tc = STEP;
    double **texcoords = malloc(num_of_tc * sizeof(double*));
    for(int i=0; i<num_of_tc; i++){
        texcoords[i] = malloc(3*sizeof(double));
    }
    texcoords[0][0] = 0;
    texcoords[0][1] = 0;
    texcoords[0][2] = 0;

    int num_of_n = STEP;
    double **normals = malloc(num_of_n * sizeof(double*));
    for(int i=0; i<num_of_n; i++){
        normals[i] = malloc(3*sizeof(double));
    }
    normals[0][0] = 0;
    normals[0][1] = 0;
    normals[0][2] = 0;

    char *line = NULL;
    size_t len = 0;
    ssize_t read = 0;
    int countPos = 1;
    int countTC = 1;
    int countN = 1;
    while((read = getline(&line, &len, file)) != -1){
        char type[5];
        sscanf(line, "%s ", type);
        if(strcmp(type, "v") == 0){
            double x = 0, y = 0, z = 0;
            sscanf(line, "v %lf %lf %lf",   &x, &y, &z);
            if(countPos >= num_of_pos){
                num_of_pos += STEP;
                positions = realloc(positions, num_of_pos * sizeof(double*));
                for(int i=countPos; i<num_of_pos; i++)
                    positions[i] = malloc(3 * sizeof(double));
            }
            positions[countPos][0] = x;
            positions[countPos][1] = y;
            positions[countPos][2] = z;
            countPos += 1;
        }
        
        if(strcmp(type, "vt") == 0){
            double u = 0, v = 0, t = 0;
            sscanf(line, "vt %lf %lf %lf", &u, &v, &t);
            if(countTC >= num_of_tc){
                num_of_tc += STEP;
                texcoords = realloc(texcoords, num_of_tc * sizeof(double*));
                for(int i=countTC; i<num_of_tc; i++)
                    texcoords[i] = malloc(3*sizeof(double));
            }
            texcoords[countTC][0] = u;
            texcoords[countTC][1] = v;
            texcoords[countTC][2] = t;
            countTC += 1;
        }

        if(strcmp(type, "vn") == 0){
            double i = 0, j = 0, k = 0;
            sscanf(line, "vn %lf %lf %lf", &i, &j, &k);
            if(countN >= num_of_n){
                num_of_n += STEP;
                normals = realloc(normals, num_of_n * sizeof(double*));
                for(int i=countN; i<num_of_n; i++)
                    normals[i] = malloc(3*sizeof(double));
            }
            normals[countN][0] = i;
            normals[countN][1] = j;
            normals[countN][2] = k;
            countN += 1;
        }

        if(strcmp(type, "f") == 0){
            int ref_step = STEP;
            VertRef *refs = malloc(ref_step * sizeof(VertRef));
            char a[256];
            char *newF = strchr(line, 'f') + 2;
            
            int offset = 0;
            int ref_count = 0;
            while(sscanf(newF, " %s%n", a, &offset) == 1){
                char *vta = strchr(a, '/')+1;
                char *vna = strchr(vta, '/')+1;
                a[strlen(a) - strlen(vta)-1] = '\0';
                if(vta[0] == '/'){
                    vta = "0";
                }
                
                
                newF += offset;
                if(ref_count >= ref_step){
                    ref_step += STEP;
                    refs = realloc(refs, ref_step*sizeof(VertRef));
                }
                refs[ref_count].v = atoi(a);
                refs[ref_count].vn = atoi(vna);
                refs[ref_count].vt = atoi(vta);
                ref_count += 1;
            }
            for(int i=1; i+1 < ref_count; i++){
                const VertRef *p[3] = {&refs[0], &refs[i], &refs[i+1]};
                double U[3] ={0};
                U[0] = positions[ p[1]->v ][0] - positions[ p[0]->v ][0];
                U[1] = positions[ p[1]->v ][1] - positions[ p[0]->v ][1];
                U[2] = positions[ p[1]->v ][2] - positions[ p[0]->v ][2];
                double V[3] ={0};
                V[0] = positions[ p[2]->v ][0] - positions[ p[0]->v ][0];
                V[1] = positions[ p[2]->v ][1] - positions[ p[0]->v ][1];
                V[2] = positions[ p[2]->v ][2] - positions[ p[0]->v ][2];
                double N[3] = {0};

                N[0] = U[1]*V[2] - U[2]*V[1];
                N[1] = U[2]*V[0] - U[0]*V[2];
                N[2] = U[0]*V[1] - U[1]*V[0];

                double w = sqrt(N[0]*N[0] + N[1]*N[1] + N[2]*N[2]);
                N[0] /= w;
                N[1] /= w;
                N[2] /= w;
                for(int j=0; j<3; j++){
                    Vertex vert;

                    vert.position[0] = positions[ p[j]->v ][0];
                    vert.position[1] = positions[ p[j]->v ][1];
                    vert.position[2] = positions[ p[j]->v ][2];

                    vert.texcoord[0] = texcoords[ p[j]->vt ][0];
                    vert.texcoord[1] = texcoords[ p[j]->vt ][1];
                    if(p[j]->vn != 0){
                        vert.normal[0] = normals[ p[j]->vn ][0];
                        vert.normal[1] = normals[ p[j]->vn ][1];
                        vert.normal[2] = normals[ p[j]->vn ][2];
                    } else {
                        vert.normal[0] = N[0];
                        vert.normal[1] = N[1];
                        vert.normal[2] = N[2];
                    }
                    if(verts_count >= verts_count_of){
                        verts_count_of += STEP;
                        verts = realloc(verts, verts_count_of*sizeof(Vertex));
                    }
                    verts[verts_count].position[0] = vert.position[0];
                    verts[verts_count].position[1] = vert.position[1];
                    verts[verts_count].position[2] = vert.position[2];

                    verts[verts_count].normal[0] = vert.normal[0];
                    verts[verts_count].normal[1] = vert.normal[1];
                    verts[verts_count].normal[2] = vert.normal[2];

                    verts[verts_count].texcoord[0] = vert.texcoord[0];
                    verts[verts_count].texcoord[1] = vert.texcoord[1];
                    verts_count += 1;
                }
            }
        }

    }
    model_size = verts_count;
    return verts;
}