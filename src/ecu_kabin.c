#include "ecu_kabin.h"
#include <stdlib.h>

void kabin_baslat(KabinECU *ecu, CANBus *bus)
{
    node_baslat(&ecu->node, 0x33, "Kabin ECU", bus);
    ecu->ic_sicaklik  = 20;
    ecu->dis_sicaklik = 15;
}

void kabin_guncelle(KabinECU *ecu)
{
    unsigned char data[2];

    ecu->ic_sicaklik  += (rand() % 3) - 1;
    ecu->dis_sicaklik += (rand() % 3) - 1;

    if (ecu->ic_sicaklik  < 15) ecu->ic_sicaklik  = 15;
    if (ecu->ic_sicaklik  > 35) ecu->ic_sicaklik  = 35;
    if (ecu->dis_sicaklik < -10) ecu->dis_sicaklik = -10;
    if (ecu->dis_sicaklik >  45) ecu->dis_sicaklik =  45;

    /* offset 40 ile negatif degerleri pozitife cek */
    data[0] = (unsigned char)(ecu->ic_sicaklik  + 40);
    data[1] = (unsigned char)(ecu->dis_sicaklik + 40);
    node_gonder(&ecu->node, KABIN_SICAKLIK_ID, data, 2);
}
