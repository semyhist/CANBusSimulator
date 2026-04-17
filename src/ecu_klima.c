#include "ecu_klima.h"
#include <stdlib.h>

void klima_baslat(KlimaECU *ecu, CANBus *bus)
{
    node_baslat(&ecu->node, 0x31, "Klima ECU", bus);
    ecu->hedef_sicaklik = 22; /* varsayilan 22 derece */
    ecu->kompressor     = 0;
}

void klima_guncelle(KlimaECU *ecu)
{
    unsigned char data[2];

    /* hedef sicaklik 18-26 arasi hafifce degissin */
    ecu->hedef_sicaklik += (rand() % 3) - 1;
    if (ecu->hedef_sicaklik < 18) ecu->hedef_sicaklik = 18;
    if (ecu->hedef_sicaklik > 26) ecu->hedef_sicaklik = 26;

    /* kompressor %40 ihtimalle calisiyor */
    ecu->kompressor = (rand() % 10 < 4) ? 1 : 0;

    data[0] = (unsigned char)ecu->hedef_sicaklik;
    data[1] = (unsigned char)ecu->kompressor;
    node_gonder(&ecu->node, KLIMA_DURUM_ID, data, 2);
}
