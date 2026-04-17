#ifndef ECU_DASHBOARD_H
#define ECU_DASHBOARD_H

#include "can_node.h"
#include "ecu_motor.h"
#include "ecu_abs.h"

#define DASHBOARD_NODE_ID 0x7F  /* diger ECU'larla cakismasin */

typedef struct {
    CANNode node;
    /* motor */
    int rpm;
    int sicaklik;
    int yakit;
    /* abs */
    int tekerlek_hizi[4];
    int fren_aktif;
    int abs_devrede;
    /* airbag */
    int airbag_carpma;
    int airbag_aktif;
    /* klima */
    int klima_hedef;
    int klima_kompressor;
    /* direksiyon */
    int direksiyon_aci;
    /* kabin */
    int ic_sicaklik;
    int dis_sicaklik;
    /* lastik */
    int lastik[4];
} DashboardECU;

void dashboard_baslat(DashboardECU *ecu, CANBus *bus);
void dashboard_oku(DashboardECU *ecu);
void dashboard_yazdir(DashboardECU *ecu);

#endif
