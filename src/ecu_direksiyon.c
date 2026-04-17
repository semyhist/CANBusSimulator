#include "ecu_direksiyon.h"
#include <stdlib.h>

void direksiyon_baslat(DireksiyonECU *ecu, CANBus *bus)
{
    node_baslat(&ecu->node, 0x32, "Direksiyon ECU", bus);
    ecu->aci  = 0;
    ecu->tork = 0;
}

void direksiyon_guncelle(DireksiyonECU *ecu)
{
    unsigned char data[4];

    /* aci yavas yavas degissin */
    ecu->aci += (rand() % 21) - 10;
    if (ecu->aci < -540) ecu->aci = -540;
    if (ecu->aci >  540) ecu->aci =  540;

    ecu->tork = rand() % 101;

    /* aci 2 byte (signed, offset 540 ile pozitife cek) */
    int aci_offset = ecu->aci + 540;
    data[0] = (aci_offset >> 8) & 0xFF;
    data[1] = (aci_offset >> 0) & 0xFF;
    data[2] = (unsigned char)ecu->tork;
    data[3] = 0;
    node_gonder(&ecu->node, DIREKSIYON_DURUM_ID, data, 4);
}
