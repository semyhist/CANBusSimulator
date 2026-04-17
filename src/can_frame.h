#ifndef CAN_FRAME_H
#define CAN_FRAME_H

#define MAX_DATA_BOYUTU  8
/* bit stuffing sonrasi en kotu durumda alan %20 buyur */
#define MAX_STUFFED_BOYUTU 16

typedef struct {
    unsigned int  id;
    unsigned char dlc;
    unsigned char data[MAX_DATA_BOYUTU];
    unsigned char uzun_id;
    unsigned char rtr;
} CANFrame;

int  can_encode(CANFrame *frame, unsigned char *bytes, int max_boy);
int  can_decode(CANFrame *frame, unsigned char *bytes, int boy);
void can_yazdir(CANFrame *frame);

/*
 * Bit stuffing: 5 ayni bit ust uste gelirse araya zit bit eklenir.
 * Gercek CAN'da bit seviyesinde yapilir, biz byte dizisi uzerinde simule ediyoruz.
 * stuff_uygula: stuffing uygular, yeni uzunlugu doner
 * stuff_kaldir: stuffing'i kaldirir, orijinal uzunlugu doner, hata olursa -1
 */
int stuff_uygula(unsigned char *giris, int giris_boy,
                 unsigned char *cikis, int max_cikis_boy);

int stuff_kaldir(unsigned char *giris, int giris_boy,
                 unsigned char *cikis, int max_cikis_boy);

#endif
