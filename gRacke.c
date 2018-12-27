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

#define TIMER_ZA_SPIRALU 10
#define ID_SPIRALE 3 

#define TIMER_ZA_RUPU 10
#define ID_RUPE 4

#define CIRCLE_SEGMENTS 12
#define PI 3.1415926535

#define FILENAME0 "mud2.bmp"
static GLuint names[2];
static float matrix[16];


static void initialize(void);

static int window_width, window_height;

static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);


void pomeriQuad(int value);

void iscrataj_sinusoidu(double);

void iscrataj_kocku();

void iscrtaj_motor();

void iscrtaj_prepreke();

static void iscrtaj_spiralu();
static void timer_spirala(int);
static float parametar_spirale = 0;
static int animacija_spirale = 0;

static void iscratj_teleport(double);
static void teleportuj_se();

static void iscrtaj_rupu();
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

}Prepreka;

static float distance(Prepreka p);

static void kolizija();

static Prepreka niz_prepreka[1000];
static int moguci_tipovi_prepreka[] = {0, 1, 2, 3}; //feder, kocka, teleport, rupa
static int niz_mogucih_x[] = {0, 1, 2, 3, 4, 5};


static int br_prepreka = 0;
static float koordinate_prepreka_na_sinusoidi[]  = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95};
int br_koordinata = 10;

static int animation_ongoing = 0;

static float y_sinusoide = 10;
static float x_kocke, y_kocke, z_kocke;
static float X = 2;

static float rand_factor = 0.8;
static float pomeraj_sinusoide = 0;

static float niz_z[1000];
static float niz_rotacija[1000];

static float ugrao_rotacije = 0;
static int br_vrednosti = 0; 

static int trenutni_element = 0;

static float eyeX, eyeY, eyeZ;

//float look_x, look_y, look_z;
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

static void iscrtaj_kvadar(void);

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

    //glutFullScreen();

    initialize();

    glutMainLoop();


    return 0;
}

void on_reshape(int width, int height) {

    window_width = width;
    window_height = height;
   // br_prepreka = 0;
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
        
        
        X += 0.7;
        eyeX += 0.7;
        if(!animacija_skretanja_desno && !animacija_skretanja_levo) {
           animacija_skretanja_desno = 1;
           glutTimerFunc(TIMER_INTERVAL, skreni_desno, CYLINDER_TIMER);
        }
        glutPostRedisplay();
        break;
    case 'a':
    case 'A' :
        
        X -= 0.7;
        eyeX -= 0.7;
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

    for(int i=0;i<br_prepreka;i++) {
        niz_prepreka[i].yPrepreke -= 0.1;
    }
    kolizija();
    if(y_sinusoide <= 0.5) {

        ugrao_rotacije = niz_rotacija[trenutni_element];
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

    if(y_sinusoide + 90 <= 0) {
        trenutni_element = 0;
        ugrao_rotacije = 0;
        y_sinusoide = 10;
        br_prepreka = 0;
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
        X, y_kocke + 10, z_kocke,
        0, 0, 1
    );

    // glPushMatrix();
    // iscrtaj_kvadar();
    // glPopMatrix();

    glRasterPos3f(eyeX + 3, 10, eyeZ + 3);
    char score_display[50] = "SCORE : ";
    char string_score[50];
    sprintf(string_score, "%d", score);
    strcat(score_display, string_score);
    int len = (int) strlen(score_display);

    for(int i=0; i<len;i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, score_display[i]);
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

    iscrataj_sinusoidu(rand_factor);
    iscrtaj_prepreke();
    
    glPopMatrix();

    glutSwapBuffers();
}

void iscrataj_sinusoidu(double rand_factor) {

    // glColor3f(0, 0, 1);
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
            glTexCoord2f(prva_kord, uvecanje); // 0 0 // 1 0
            //druga_kord = (druga_kord + 1) % 2; // 1 1
            glVertex3f(0, y_sinusoide + i, sin(i * rand_factor));
            glTexCoord2f(druga_kord, uvecanje); // 0 1
            // druga_kord = (druga_kord + 1) % 2;
            // prva_kord = (prva_kord + 1) % 2;
            //  // 1 0 // 0 0
            glVertex3f(8, y_sinusoide + i, sin(i * rand_factor));
            uvecanje += 0.1;
            if(uvecanje == 1)
                uvecanje = 0;
            //printf("%f--%d--%d\n", y_sinusoide, prvi_put, animation_ongoing);
            if (fabs(y_sinusoide - 10) < 0.01 && (prvi_put == 0 || animation_ongoing) ) {
                
                for (int j=0;j<19;j++) {

                    if( fabs(i - koordinate_prepreka_na_sinusoidi[j]) < 0.01) {
                        //printf("usoo\n");
                        int broj_prepreka = (int)rand() % 3 + 1;
                        int niz_zauzetih[] = {0, 0, 0, 0, 0};
                        int broj_pogodaka = 0;
                        while(broj_pogodaka < broj_prepreka) {

                            //if()

                            Prepreka p;
                            int k = 100;
                            while(k == 100  || niz_zauzetih[k]) {
                                k = niz_mogucih_x[(int)rand() % 5];
                            }
                            broj_pogodaka++;
                            niz_zauzetih[k] = 1;
                            p.xPrepreke = k;
                            p.tipPrepreke = moguci_tipovi_prepreka[(int)rand() % 4];
                            p.yPrepreke = y_sinusoide + i;
                            p.zPrepreke = sin(i * rand_factor) + 0.5;
                            p.ugraoRotacije = cos(i * rand_factor);
                            niz_prepreka[br_prepreka++] = p;

                        }
                    }

                }
            }

        }

        prvi_put = 1;
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

}

void iscrtaj_prepreke() {


    for(int i=0; i<br_prepreka;i++) {
        Prepreka p = niz_prepreka[i];
        glPushMatrix();
        glTranslatef(p.xPrepreke, p.yPrepreke, p.zPrepreke);
        
        if (p.tipPrepreke == 0) {
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
        //glutSolidCube(1);
        glPopMatrix();

    }
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
    rotation_desno += 0.1;

    eyeX =  eyeX - 0.01 * cos((rotation) * PI/180);
    eyeY =  eyeY - 0.01 * sin((rotation) * PI/180);

    if(ugoa_desno >= 180) {

            ugoa_desno = 0;
            animacija_skretanja_desno = 0;
            eyeY = y_kocke - 1.5;
            eyeX = X;
            rotation_desno = 0;

    }
        
    
    glutPostRedisplay();

    if(animacija_skretanja_desno)
        glutTimerFunc(TIMER_INTERVAL, skreni_desno, CYLINDER_TIMER);

}
void skreni_levo(int value) {

    if(value != CYLINDER_TIMER)
        return;

    ugao_levo += 5;
    rotation += 3;

    eyeX =  eyeX + 0.01 *cos(rotation * PI/180);
    eyeY =  eyeY + 0.01 * sin(rotation * PI/180);

    if(ugao_levo >= 180) {
        ugao_levo = 0;
        eyeY = y_kocke - 1.5;
        eyeX = X;
        animacija_skretanja_levo = 0;
        rotation = 0;
    }
        
    
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

static void iscratj_teleport(double y_prepreke)
{
    glPushMatrix();
        //glTranslatef(x_obst,y_obst+1.5,z_obst);
        //glScalef(.6,1.2,0);
        GLfloat x = 0,y=0,z=0,angle, r  = .0001;
        glBegin(GL_LINE_STRIP);
        glLineWidth(7);
        for(angle = 0; angle < 1800; angle += 1)
        {   
            x = r * cos(angle * PI/180);
            z = r * sin(angle * PI/180);
            y=y_prepreke;
            glVertex3f(x,y,z);
            
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
        if(distance(p) <= 1.02) {
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
    image_read(image, FILENAME0);

    /* Generisu se identifikatori tekstura. */
    glGenTextures(2, names);

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

    /* Kreira se druga tekstura. */
    // image_read(image, FILENAME1);

    // glBindTexture(GL_TEXTURE_2D, names[1]);
    // glTexParameteri(GL_TEXTURE_2D,
    //                 GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D,
    //                 GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
    //              image->width, image->height, 0,
    //              GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Unistava se objekat za citanje tekstura iz fajla. */
    image_done(image);

    /* Inicijalizujemo matricu rotacije. */
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
    // glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
}

static void iscrtaj_kvadar() {

    // glBindTexture(GL_TEXTURE_2D, names[0]);
    // glEnable(GL_TEXTURE_2D);

    // int u = 0;
    // float angle = 0;

    // /* crtamo zarubljenu kupu */
    // glBegin(GL_TRIANGLE_STRIP);
    //     for (u = 0; u <= 16; u++) {
    //         angle = u * (2 * PI / 16);

    //         /* definisemo normalu povrsi */
    //         glNormal3f(cos(angle), 0, sin(angle));

    //         /* definisemo koordinate tacaka - u prostoru i teksturi */
    //         glTexCoord2f(u / (float) 16, 0);
    //         glVertex3f(2 * cos(angle), 0, 2 * sin(angle));

    //         glTexCoord2f(u / (float) 16, 1);
    //         glVertex3f(2 * cos(angle), 1, 2 * sin(angle));
    //     }
    // glEnd();

    // /* Iskljucujemo crtanje tekstura */
    // glDisable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, names[0]);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);

        glNormal3f(1, 0, 0);

        glTexCoord2f(0, 0);
        glVertex3f(1, 0, 2);

        glTexCoord2f(1, 0);
        glVertex3f(2.2, 0, 2);

        glTexCoord2f(1, 1);
        glVertex3f(2.2, 2.2, 2);

        glTexCoord2f(0, 1);
        glVertex3f(1, 2.2, 2);

    glEnd();

    glDisable(GL_TEXTURE_2D);

}
