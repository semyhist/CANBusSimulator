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

    /* motor sogukken rpm dusuk kalsin, isininca normal araliga gec */
    int rpm_max = (ecu->sicaklik < 60) ? 1500 : 6000;
    int rpm_min = (ecu->sicaklik < 40) ? 800  : 800;
    ecu->rpm += (rand() % 201) - 100;
    if (ecu->rpm < rpm_min) ecu->rpm = rpm_min;
    if (ecu->rpm > rpm_max) ecu->rpm = rpm_max;

    /* soguk motordan 90 dereceye yavas isin, sonra sabit kal */
    if (ecu->sicaklik < 90)
        ecu->sicaklik += 1;

    /* yakit: her 50 adimda bir azalsin */
    ecu->yakit_adim++;
    if (ecu->yakit_adim >= 50) {
        ecu->yakit_adim = 0;
        if (ecu->yakit_seviyesi > 0)
            ecu->yakit_seviyesi--;
    }

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
