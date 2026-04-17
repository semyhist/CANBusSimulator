#ifndef ECU_LASTIK_H
#define ECU_LASTIK_H

#include "can_node.h"

#define LASTIK_BASINC_ID  0x304  /* 4 tekerlek basinc degerleri */

/* normal lastik basinci 30-35 PSI arasi */
#define LASTIK_BASINC_MIN 28
#define LASTIK_BASINC_MAX 36

typedef struct {
    CANNode node;
    int     basinc[4]; /* on sol, on sag, arka sol, arka sag (PSI) */
} LastikECU;

void lastik_baslat(LastikECU *ecu, CANBus *bus);
void lastik_guncelle(LastikECU *ecu);

#endif
