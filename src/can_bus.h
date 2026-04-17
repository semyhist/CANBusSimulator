#ifndef CAN_BUS_H
#define CAN_BUS_H

#include "can_frame.h"
#include "can_error.h"

#define BUS_KUYRUK_BOYUTU   32
#define MAX_BEKLEYEN        16  /* arbitration icin bekleyen mesaj sayisi */

/* arbitration bekleyen mesaj */
typedef struct {
    CANFrame frame;
    int      gonderici_node_id;
    int      dolu;
} BekleyenMesaj;

typedef struct {
    CANFrame      slots[BUS_KUYRUK_BOYUTU];
    int           dolu[BUS_KUYRUK_BOYUTU];
    int           yazi_indeksi;
    int           toplam_mesaj;
    BekleyenMesaj bekleyenler[MAX_BEKLEYEN]; /* arbitration kuyruğu */
    int           bekleyen_sayisi;
} CANBus;

void bus_baslat(CANBus *bus);

/* mesaji arbitration kuyruğuna ekler, henuz bus'a yazmaz */
int  bus_gonder_iste(CANBus *bus, CANFrame *frame, int gonderici_node_id);

/*
 * Arbitration: bekleyen mesajlar arasinda ID'si en kucuk olani bus'a yazar.
 * Kazanan node'un id'sini doner, bekleyen yoksa -1.
 */
int  bus_arbitration_calistir(CANBus *bus);

/* direkt bus'a yazar (arbitration olmadan, ic kullanim) */
void bus_yaz(CANBus *bus, CANFrame *frame);

int  bus_al(CANBus *bus, CANFrame *hedef, unsigned int mesaj_id, int *okuma_indeksi);

#endif
