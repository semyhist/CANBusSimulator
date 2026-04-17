#ifndef ECU_AIRBAG_H
#define ECU_AIRBAG_H

#include "can_node.h"

#define AIRBAG_DURUM_ID  0x300  /* carpma tespiti, airbag aktif mi */

typedef struct {
    CANNode node;
    int     carpma_tespiti; /* 1 = carpma var */
    int     airbag_aktif;   /* 1 = airbag acildi */
} AirbagECU;

void airbag_baslat(AirbagECU *ecu, CANBus *bus);
void airbag_guncelle(AirbagECU *ecu);

#endif
