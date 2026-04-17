#ifndef ECU_MOTOR_H
#define ECU_MOTOR_H

#include "can_node.h"

/* mesaj id'leri */
#include "can_ids.h"
#define MOTOR_RPM_ID    ID_MOTOR_RPM
#define MOTOR_SICAK_ID  ID_MOTOR_SICAK
#define MOTOR_YAKIT_ID  ID_MOTOR_YAKIT

typedef struct {
    CANNode node;
    int     rpm;            /* devir/dakika */
    int     sicaklik;       /* celsius */
    int     yakit_seviyesi; /* yuzde 0-100 */
    int     yakit_adim;     /* azalma sayaci */
} MotorECU;

void motor_baslat(MotorECU *ecu, CANBus *bus);
void motor_guncelle(MotorECU *ecu); /* veri uret ve bus'a gonder */

#endif
