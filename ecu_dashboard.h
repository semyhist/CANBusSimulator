#ifndef ECU_DASHBOARD_H
#define ECU_DASHBOARD_H

#include "can_node.h"
#include "ecu_motor.h"
#include "ecu_abs.h"

typedef struct {
    CANNode node;
    int     rpm;
    int     sicaklik;
    int     yakit;
    int     tekerlek_hizi[4];
    int     fren_aktif;
    int     abs_devrede;
} DashboardECU;

void dashboard_baslat(DashboardECU *ecu, CANBus *bus);
void dashboard_oku(DashboardECU *ecu);   /* bus'tan veri topla */
void dashboard_yazdir(DashboardECU *ecu); /* ekrana bas */

#endif
