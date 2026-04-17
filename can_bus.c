#include "can_bus.h"
#include <string.h>

void bus_baslat(CANBus *bus)
{
    int i;
    bus->yazi_indeksi   = 0;
    bus->toplam_mesaj   = 0;
    bus->bekleyen_sayisi = 0;
    for (i = 0; i < BUS_KUYRUK_BOYUTU; i++)
        bus->dolu[i] = 0;
    for (i = 0; i < MAX_BEKLEYEN; i++)
        bus->bekleyenler[i].dolu = 0;
}

int bus_gonder_iste(CANBus *bus, CANFrame *frame, int gonderici_node_id)
{
    int i;

    if (bus->bekleyen_sayisi >= MAX_BEKLEYEN)
        return -1;

    /* bos slot bul */
    for (i = 0; i < MAX_BEKLEYEN; i++) {
        if (bus->bekleyenler[i].dolu == 0) {
            bus->bekleyenler[i].frame            = *frame;
            bus->bekleyenler[i].gonderici_node_id = gonderici_node_id;
            bus->bekleyenler[i].dolu             = 1;
            bus->bekleyen_sayisi++;
            return 0;
        }
    }

    return -1;
}

int bus_arbitration_calistir(CANBus *bus)
{
    int i;
    int kazanan_indeks   = -1;
    unsigned int en_kucuk_id = 0xFFFFFFFF;

    if (bus->bekleyen_sayisi == 0)
        return -1;

    /* en kucuk id'yi bul — gercek CAN'da dominant bit kazanir, kucuk id = daha fazla 0 */
    for (i = 0; i < MAX_BEKLEYEN; i++) {
        if (bus->bekleyenler[i].dolu == 0)
            continue;
        if (bus->bekleyenler[i].frame.id < en_kucuk_id) {
            en_kucuk_id    = bus->bekleyenler[i].frame.id;
            kazanan_indeks = i;
        }
    }

    if (kazanan_indeks == -1)
        return -1;

    /* kazanan mesaji bus'a yaz */
    bus_yaz(bus, &bus->bekleyenler[kazanan_indeks].frame);

    int kazanan_node_id = bus->bekleyenler[kazanan_indeks].gonderici_node_id;

    /* kazanani kuyruktan cikar */
    bus->bekleyenler[kazanan_indeks].dolu = 0;
    bus->bekleyen_sayisi--;

    return kazanan_node_id;
}

void bus_yaz(CANBus *bus, CANFrame *frame)
{
    bus->slots[bus->yazi_indeksi] = *frame;
    bus->dolu[bus->yazi_indeksi]  = 1;
    bus->yazi_indeksi = (bus->yazi_indeksi + 1) % BUS_KUYRUK_BOYUTU;
    bus->toplam_mesaj++;
}

int bus_al(CANBus *bus, CANFrame *hedef, unsigned int mesaj_id, int *okuma_indeksi)
{
    int i;
    int indeks;

    for (i = 0; i < BUS_KUYRUK_BOYUTU; i++) {
        indeks = (*okuma_indeksi + i) % BUS_KUYRUK_BOYUTU;

        if (bus->dolu[indeks] == 0)
            continue;

        if (bus->slots[indeks].id == mesaj_id) {
            *hedef         = bus->slots[indeks];
            *okuma_indeksi = (indeks + 1) % BUS_KUYRUK_BOYUTU;
            return 0;
        }
    }

    return -1;
}
