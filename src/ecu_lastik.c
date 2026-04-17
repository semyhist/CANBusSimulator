#include "ecu_lastik.h"
#include <stdlib.h>

void lastik_baslat(LastikECU *ecu, CANBus *bus)
{
    int i;
    node_baslat(&ecu->node, 0x34, "Lastik ECU", bus);
    for (i = 0; i < 4; i++)
        ecu->basinc[i] = 32; /* baslangicta normal basinc */
}

void lastik_guncelle(LastikECU *ecu)
{
    unsigned char data[4];
    int i;

    for (i = 0; i < 4; i++) {
        /* basinc cok yavas degissin */
        ecu->basinc[i] += (rand() % 3) - 1;
        if (ecu->basinc[i] < 20) ecu->basinc[i] = 20; /* patlak lastik alt siniri */
        if (ecu->basinc[i] > 38) ecu->basinc[i] = 38;
        data[i] = (unsigned char)ecu->basinc[i];
    }

    node_gonder(&ecu->node, LASTIK_BASINC_ID, data, 4);
}
