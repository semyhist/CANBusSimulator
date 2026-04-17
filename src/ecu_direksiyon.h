#ifndef ECU_DIREKSIYON_H
#define ECU_DIREKSIYON_H

#include "can_node.h"

#define DIREKSIYON_DURUM_ID  0x302  /* aci, tork */

typedef struct {
    CANNode node;
    int     aci;   /* -540 ile +540 derece arasi */
    int     tork;  /* 0-100 Nm */
} DireksiyonECU;

void direksiyon_baslat(DireksiyonECU *ecu, CANBus *bus);
void direksiyon_guncelle(DireksiyonECU *ecu);

#endif
