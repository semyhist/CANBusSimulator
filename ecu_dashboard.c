#include "ecu_dashboard.h"
#include <stdio.h>

void dashboard_baslat(DashboardECU *ecu, CANBus *bus)
{
    int i;
    node_baslat(&ecu->node, 0x30, "Dashboard", bus);

    /* dinleyecegi tum mesaj id'lerini ekle */
    node_filtre_ekle(&ecu->node, MOTOR_RPM_ID);
    node_filtre_ekle(&ecu->node, MOTOR_SICAK_ID);
    node_filtre_ekle(&ecu->node, MOTOR_YAKIT_ID);
    node_filtre_ekle(&ecu->node, ABS_TEKERLEK_ID);
    node_filtre_ekle(&ecu->node, ABS_FREN_ID);

    ecu->rpm        = 0;
    ecu->sicaklik   = 0;
    ecu->yakit      = 0;
    ecu->fren_aktif = 0;
    ecu->abs_devrede = 0;
    for (i = 0; i < 4; i++)
        ecu->tekerlek_hizi[i] = 0;
}

void dashboard_oku(DashboardECU *ecu)
{
    CANFrame frame;

    if (bus_al(ecu->node.bus, &frame, MOTOR_RPM_ID, &ecu->node.okuma_indeksi) == 0)
        ecu->rpm = (frame.data[0] << 8) | frame.data[1];

    if (bus_al(ecu->node.bus, &frame, MOTOR_SICAK_ID, &ecu->node.okuma_indeksi) == 0)
        ecu->sicaklik = frame.data[0];

    if (bus_al(ecu->node.bus, &frame, MOTOR_YAKIT_ID, &ecu->node.okuma_indeksi) == 0)
        ecu->yakit = frame.data[0];

    if (bus_al(ecu->node.bus, &frame, ABS_TEKERLEK_ID, &ecu->node.okuma_indeksi) == 0) {
        ecu->tekerlek_hizi[0] = frame.data[0];
        ecu->tekerlek_hizi[1] = frame.data[1];
        ecu->tekerlek_hizi[2] = frame.data[2];
        ecu->tekerlek_hizi[3] = frame.data[3];
    }

    if (bus_al(ecu->node.bus, &frame, ABS_FREN_ID, &ecu->node.okuma_indeksi) == 0) {
        ecu->fren_aktif  = frame.data[0];
        ecu->abs_devrede = frame.data[1];
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
    printf("Fren       : %s\n", ecu->fren_aktif  ? "BASILI" : "bos");
    printf("ABS        : %s\n", ecu->abs_devrede ? "DEVREDE" : "bos");
    printf("===============================\n\n");
}
