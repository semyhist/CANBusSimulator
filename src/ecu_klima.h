#ifndef ECU_KLIMA_H
#define ECU_KLIMA_H

#include "can_node.h"

#define KLIMA_DURUM_ID  0x301  /* hedef sicaklik, kompressor durumu */

typedef struct {
    CANNode node;
    int     hedef_sicaklik; /* kullanicinin ayarladigi sicaklik */
    int     kompressor;     /* 1 = kompressor calisiyor */
} KlimaECU;

void klima_baslat(KlimaECU *ecu, CANBus *bus);
void klima_guncelle(KlimaECU *ecu);

#endif
