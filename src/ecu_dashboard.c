#include "ecu_dashboard.h"
#include "can_ids.h"
#include <stdio.h>

void dashboard_baslat(DashboardECU *ecu, CANBus *bus)
{
    int i;
    node_baslat(&ecu->node, DASHBOARD_NODE_ID, "Dashboard", bus);

    node_filtre_ekle(&ecu->node, ID_MOTOR_RPM);
    node_filtre_ekle(&ecu->node, ID_MOTOR_SICAK);
    node_filtre_ekle(&ecu->node, ID_MOTOR_YAKIT);
    node_filtre_ekle(&ecu->node, ID_ABS_TEKERLEK);
    node_filtre_ekle(&ecu->node, ID_ABS_FREN);
    node_filtre_ekle(&ecu->node, ID_AIRBAG);
    node_filtre_ekle(&ecu->node, ID_KLIMA);
    node_filtre_ekle(&ecu->node, ID_DIREKSIYON);
    node_filtre_ekle(&ecu->node, ID_KABIN);
    node_filtre_ekle(&ecu->node, ID_LASTIK);

    ecu->rpm = 0; ecu->sicaklik = 0; ecu->yakit = 0;
    ecu->fren_aktif = 0; ecu->abs_devrede = 0;
    ecu->airbag_carpma = 0; ecu->airbag_aktif = 0;
    ecu->klima_hedef = 22; ecu->klima_kompressor = 0;
    ecu->direksiyon_aci = 0;
    ecu->ic_sicaklik = 20; ecu->dis_sicaklik = 15;
    for (i = 0; i < 4; i++) {
        ecu->tekerlek_hizi[i] = 0;
        ecu->lastik[i] = 32;
    }
}

void dashboard_oku(DashboardECU *ecu)
{
    CANFrame frame;

    if (bus_al(ecu->node.bus, &frame, ID_MOTOR_RPM, &ecu->node.okuma_indeksi) == 0)
        ecu->rpm = (frame.data[0] << 8) | frame.data[1];

    if (bus_al(ecu->node.bus, &frame, ID_MOTOR_SICAK, &ecu->node.okuma_indeksi) == 0)
        ecu->sicaklik = frame.data[0];

    if (bus_al(ecu->node.bus, &frame, ID_MOTOR_YAKIT, &ecu->node.okuma_indeksi) == 0)
        ecu->yakit = frame.data[0];

    if (bus_al(ecu->node.bus, &frame, ID_ABS_TEKERLEK, &ecu->node.okuma_indeksi) == 0) {
        ecu->tekerlek_hizi[0] = frame.data[0];
        ecu->tekerlek_hizi[1] = frame.data[1];
        ecu->tekerlek_hizi[2] = frame.data[2];
        ecu->tekerlek_hizi[3] = frame.data[3];
    }

    if (bus_al(ecu->node.bus, &frame, ID_ABS_FREN, &ecu->node.okuma_indeksi) == 0) {
        ecu->fren_aktif  = frame.data[0];
        ecu->abs_devrede = frame.data[1];
    }

    if (bus_al(ecu->node.bus, &frame, ID_AIRBAG, &ecu->node.okuma_indeksi) == 0) {
        ecu->airbag_carpma = frame.data[0];
        ecu->airbag_aktif  = frame.data[1];
    }

    if (bus_al(ecu->node.bus, &frame, ID_KLIMA, &ecu->node.okuma_indeksi) == 0) {
        ecu->klima_hedef      = frame.data[0];
        ecu->klima_kompressor = frame.data[1];
    }

    if (bus_al(ecu->node.bus, &frame, ID_DIREKSIYON, &ecu->node.okuma_indeksi) == 0) {
        int aci_offset = (frame.data[0] << 8) | frame.data[1];
        ecu->direksiyon_aci = aci_offset - 540;
    }

    if (bus_al(ecu->node.bus, &frame, ID_KABIN, &ecu->node.okuma_indeksi) == 0) {
        ecu->ic_sicaklik  = (int)frame.data[0] - 40;
        ecu->dis_sicaklik = (int)frame.data[1] - 40;
    }

    if (bus_al(ecu->node.bus, &frame, ID_LASTIK, &ecu->node.okuma_indeksi) == 0) {
        ecu->lastik[0] = frame.data[0];
        ecu->lastik[1] = frame.data[1];
        ecu->lastik[2] = frame.data[2];
        ecu->lastik[3] = frame.data[3];
    }
}

void dashboard_yazdir(DashboardECU *ecu)
{
    printf("========== DASHBOARD ==========\n");
    printf("RPM        : %d\n",   ecu->rpm);
    printf("Sicaklik   : %d C\n", ecu->sicaklik);
    printf("Yakit      : %d%%\n", ecu->yakit);
    printf("Tekerlekler: %d %d %d %d km/h\n",
           ecu->tekerlek_hizi[0], ecu->tekerlek_hizi[1],
           ecu->tekerlek_hizi[2], ecu->tekerlek_hizi[3]);
    printf("Fren       : %s\n", ecu->fren_aktif   ? "BASILI"  : "bos");
    printf("ABS        : %s\n", ecu->abs_devrede  ? "DEVREDE" : "bos");
    printf("Airbag     : %s\n", ecu->airbag_aktif ? "ACILDI"  : "bos");
    printf("Klima      : %d C  Kompressor: %s\n",
           ecu->klima_hedef, ecu->klima_kompressor ? "ON" : "OFF");
    printf("Direksiyon : %d derece\n", ecu->direksiyon_aci);
    printf("Kabin      : ic=%d dis=%d C\n", ecu->ic_sicaklik, ecu->dis_sicaklik);
    printf("Lastik     : %d %d %d %d PSI\n",
           ecu->lastik[0], ecu->lastik[1], ecu->lastik[2], ecu->lastik[3]);
    printf("===============================\n\n");
}
