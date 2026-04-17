#include "ecu_abs.h"
#include <stdlib.h>

void abs_baslat(AbsECU *ecu, CANBus *bus)
{
    int i;
    node_baslat(&ecu->node, 0x20, "ABS ECU", bus);
    for (i = 0; i < 4; i++)
        ecu->tekerlek_hizi[i] = 0;
    ecu->fren_aktif  = 0;
    ecu->abs_devrede = 0;
}

void abs_guncelle(AbsECU *ecu)
{
    unsigned char data[8];
    int i;

    /* tekerlekler benzer hizda, ama ufak farklar olabilir */
    int ana_hiz = 60 + (rand() % 41); /* 60-100 km/h arasi */
    for (i = 0; i < 4; i++)
        ecu->tekerlek_hizi[i] = ana_hiz + (rand() % 5) - 2;

    /* %20 ihtimalle fren basili */
    ecu->fren_aktif = (rand() % 5 == 0) ? 1 : 0;

    /* fren basiliysa ve tekerlekler cok farkli donuyorsa abs devreye girer */
    ecu->abs_devrede = 0;
    if (ecu->fren_aktif) {
        int fark = ecu->tekerlek_hizi[0] - ecu->tekerlek_hizi[3];
        if (fark < 0) fark = -fark;
        if (fark > 3) ecu->abs_devrede = 1;
    }

    /* 4 tekerlek hizini gonder, her biri 1 byte (max 255 km/h yeter) */
    for (i = 0; i < 4; i++)
        data[i] = (unsigned char)ecu->tekerlek_hizi[i];
    node_gonder(&ecu->node, ABS_TEKERLEK_ID, data, 4);

    /* fren durumu: byte0=fren_aktif, byte1=abs_devrede */
    data[0] = (unsigned char)ecu->fren_aktif;
    data[1] = (unsigned char)ecu->abs_devrede;
    node_gonder(&ecu->node, ABS_FREN_ID, data, 2);
}
