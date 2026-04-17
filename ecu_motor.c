#include "ecu_motor.h"
#include <stdlib.h>

void motor_baslat(MotorECU *ecu, CANBus *bus)
{
    node_baslat(&ecu->node, 0x10, "Motor ECU", bus);
    ecu->rpm            = 800;  /* rolvanti */
    ecu->sicaklik       = 20;   /* baslangic sicakligi */
    ecu->yakit_seviyesi = 100;
}

void motor_guncelle(MotorECU *ecu)
{
    unsigned char data[2];

    /* rpm: 800 ile 6000 arasinda rastgele degissin */
    ecu->rpm += (rand() % 201) - 100; /* -100 ile +100 arasi */
    if (ecu->rpm < 800)  ecu->rpm = 800;
    if (ecu->rpm > 6000) ecu->rpm = 6000;

    /* sicaklik: yavas yavas 90'a dogru cikar */
    if (ecu->sicaklik < 90)
        ecu->sicaklik += rand() % 3;

    /* yakit: her adimda biraz azalsin */
    if (ecu->yakit_seviyesi > 0)
        ecu->yakit_seviyesi--;

    /* rpm'i 2 byte olarak gonder (big-endian) */
    data[0] = (ecu->rpm >> 8) & 0xFF;
    data[1] = (ecu->rpm >> 0) & 0xFF;
    node_gonder(&ecu->node, MOTOR_RPM_ID, data, 2);

    /* sicaklik 1 byte yeter */
    data[0] = (unsigned char)ecu->sicaklik;
    node_gonder(&ecu->node, MOTOR_SICAK_ID, data, 1);

    /* yakit 1 byte */
    data[0] = (unsigned char)ecu->yakit_seviyesi;
    node_gonder(&ecu->node, MOTOR_YAKIT_ID, data, 1);
}
