typedef struct {

    float xPrepreke, yPrepreke, zPrepreke;
    float leviX, desniX, prednjiY, zadnjY;
    int tipPrepreke;
    float ugraoRotacije;

}Prepreka;


extern Prepreka niz_prepreka[1000];
extern int moguci_tipovi_prepreka[4]; //feder, kocka, teleport, rupa
extern int niz_mogucih_x[6];


extern int br_prepreka;
extern float koordinate_prepreka_na_sinusoidi[19];  
extern int br_koordinata;

extern int animation_ongoing;

extern float y_sinusoide;
extern float x_kocke, y_kocke, z_kocke;
extern float X;

extern float rand_factor;
extern float pomeraj_sinusoide;

extern float niz_z[1000];
extern float niz_rotacija[1000];

extern float ugrao_rotacije;
extern int br_vrednosti; 

extern int trenutni_element;

extern float eyeX, eyeY, eyeZ;

//float look_x, look_y, look_z;
//ovde su za skretanje
extern float ugoa_desno;
extern float ugao_levo;
extern int animacija_skretanja_levo;
extern int animacija_skretanja_desno;

extern float rotation;
extern float rotation_desno;


extern int imune;

extern int score;
extern int skriveni_skor;


extern int prvi_put;