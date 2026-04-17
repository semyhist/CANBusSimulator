#ifndef CAN_NODE_H
#define CAN_NODE_H

#include "can_bus.h"
#include "can_error.h"

#define MAX_FILTRE 8

typedef struct {
    int          id;
    char         isim[32];
    CANBus       *bus;
    int          okuma_indeksi;
    unsigned int filtreler[MAX_FILTRE];
    int          filtre_sayisi;
    int          hata_sayaci;   /* TEC/REC yerine tek sayac kullaniyoruz */
    int          durum;         /* DURUM_NORMAL, ERROR_PASSIVE, BUS_OFF */
} CANNode;

void node_baslat(CANNode *node, int id, char *isim, CANBus *bus);
void node_filtre_ekle(CANNode *node, unsigned int mesaj_id);

/* mesaji arbitration kuyruğuna ekler */
int  node_gonder(CANNode *node, unsigned int mesaj_id, unsigned char *data, int dlc);

/* bus'tan mesaj al */
int  node_al(CANNode *node, CANFrame *hedef);

/* hata sayacini guncelle ve durumu kontrol et */
void node_hata_ekle(CANNode *node, int hata_tipi);
void node_hata_azalt(CANNode *node);
void node_durum_yazdir(CANNode *node);

#endif
