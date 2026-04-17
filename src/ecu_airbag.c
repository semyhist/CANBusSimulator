#include "ecu_airbag.h"
#include <stdlib.h>

void airbag_baslat(AirbagECU *ecu, CANBus *bus)
{
    node_baslat(&ecu->node, 0x30, "Airbag ECU", bus);
    ecu->carpma_tespiti = 0;
    ecu->airbag_aktif   = 0;
}

void airbag_guncelle(AirbagECU *ecu)
{
    unsigned char data[2];

    /* %2 ihtimalle carpma tespiti */
    ecu->carpma_tespiti = (rand() % 50 == 0) ? 1 : 0;

    /* carpma varsa airbag acilir, bir kez acilinca kapanmaz */
    if (ecu->carpma_tespiti)
        ecu->airbag_aktif = 1;

    data[0] = (unsigned char)ecu->carpma_tespiti;
    data[1] = (unsigned char)ecu->airbag_aktif;
    node_gonder(&ecu->node, AIRBAG_DURUM_ID, data, 2);
}
