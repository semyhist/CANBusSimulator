#ifndef ECU_KABIN_H
#define ECU_KABIN_H

#include "can_node.h"

#define KABIN_SICAKLIK_ID  0x303

typedef struct {
    CANNode node;
    int     ic_sicaklik;  /* arac ici sicaklik */
    int     dis_sicaklik; /* dis ortam sicakligi */
} KabinECU;

void kabin_baslat(KabinECU *ecu, CANBus *bus);
void kabin_guncelle(KabinECU *ecu);

#endif
