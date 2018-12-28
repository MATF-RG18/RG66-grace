#include "globalne.h"


#define PI 3.1415926535

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