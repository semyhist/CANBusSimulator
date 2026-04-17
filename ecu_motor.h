#ifndef ECU_MOTOR_H
#define ECU_MOTOR_H

#include "can_node.h"

/* mesaj id'leri */
#define MOTOR_RPM_ID    0x100
#define MOTOR_SICAK_ID  0x101
#define MOTOR_YAKIT_ID  0x102

typedef struct {
    CANNode node;
    int     rpm;            /* devir/dakika */
    int     sicaklik;       /* celsius */
    int     yakit_seviyesi; /* yuzde 0-100 */
} MotorECU;

void motor_baslat(MotorECU *ecu, CANBus *bus);
void motor_guncelle(MotorECU *ecu); /* veri uret ve bus'a gonder */

#endif
