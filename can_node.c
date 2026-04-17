#include "can_node.h"
#include <string.h>
#include <stdio.h>

void node_baslat(CANNode *node, int id, char *isim, CANBus *bus)
{
    node->id            = id;
    node->bus           = bus;
    node->okuma_indeksi = 0;
    node->filtre_sayisi = 0;
    node->hata_sayaci   = 0;
    node->durum         = DURUM_NORMAL;
    strncpy(node->isim, isim, sizeof(node->isim) - 1);
    node->isim[sizeof(node->isim) - 1] = '\0';
}

void node_filtre_ekle(CANNode *node, unsigned int mesaj_id)
{
    if (node->filtre_sayisi < MAX_FILTRE)
        node->filtreler[node->filtre_sayisi++] = mesaj_id;
}

int node_gonder(CANNode *node, unsigned int mesaj_id, unsigned char *data, int dlc)
{
    CANFrame frame;
    int i;

    /* bus-off durumdaysa gonderme */
    if (node->durum == DURUM_BUS_OFF) {
        printf("[%s] BUS-OFF durumunda, mesaj gonderilemez!\n", node->isim);
        return -1;
    }

    frame.id      = mesaj_id;
    frame.dlc     = (unsigned char)dlc;
    frame.uzun_id = 0;
    frame.rtr     = 0;

    for (i = 0; i < dlc; i++)
        frame.data[i] = data[i];

    return bus_gonder_iste(node->bus, &frame, node->id);
}

int node_al(CANNode *node, CANFrame *hedef)
{
    int i;
    for (i = 0; i < node->filtre_sayisi; i++) {
        if (bus_al(node->bus, hedef, node->filtreler[i], &node->okuma_indeksi) == 0)
            return 0;
    }
    return -1;
}

void node_hata_ekle(CANNode *node, int hata_tipi)
{
    /* gercek CAN'da hata tipine gore farkli artis var, biz 8 kullaniyoruz */
    (void)hata_tipi; /* simdilik tip farketmiyor */
    node->hata_sayaci += 8;

    if (node->hata_sayaci >= ESIK_BUS_OFF) {
        node->hata_sayaci = ESIK_BUS_OFF;
        node->durum       = DURUM_BUS_OFF;
        printf("[%s] BUS-OFF! Hata sayaci: %d\n", node->isim, node->hata_sayaci);
    } else if (node->hata_sayaci >= ESIK_ERROR_PASSIVE) {
        node->durum = DURUM_ERROR_PASSIVE;
        printf("[%s] ERROR PASSIVE. Hata sayaci: %d\n", node->isim, node->hata_sayaci);
    }
}

void node_hata_azalt(CANNode *node)
{
    /* basarili gonderim/alimda hata sayaci azalir */
    if (node->hata_sayaci > 0)
        node->hata_sayaci -= 1;

    if (node->hata_sayaci < ESIK_ERROR_PASSIVE)
        node->durum = DURUM_NORMAL;
}

void node_durum_yazdir(CANNode *node)
{
    const char *durum_str;

    if      (node->durum == DURUM_NORMAL)         durum_str = "NORMAL";
    else if (node->durum == DURUM_ERROR_PASSIVE)  durum_str = "ERROR PASSIVE";
    else                                           durum_str = "BUS-OFF";

    printf("[%s] Durum: %s | Hata sayaci: %d\n",
           node->isim, durum_str, node->hata_sayaci);
}
