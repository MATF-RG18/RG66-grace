#include "globalne.h"

Prepreka niz_prepreka[1000];
int moguci_tipovi_prepreka[] = {0, 1, 2, 3}; //feder, kocka, teleport, rupa
int niz_mogucih_x[] = {0, 1, 2, 3, 4, 5};


int br_prepreka = 0;
float koordinate_prepreka_na_sinusoidi[]  = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95};
int br_koordinata = 10;

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
//ovde su za skretanje
float ugoa_desno = 0;
float ugao_levo = 0;
int animacija_skretanja_levo = 0;
int animacija_skretanja_desno = 0;

float rotation = 0;
float rotation_desno = 0;


int imune = 0;

int score = 0;
int skriveni_skor = 0;


int prvi_put;