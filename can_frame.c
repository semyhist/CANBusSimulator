#include "can_frame.h"
#include <stdio.h>
#include <string.h>

/*
 * byte dizisinin yapisi:
 * [0-3] -> id (big-endian)
 * [4]   -> flags: bit0=uzun_id, bit1=rtr
 * [5]   -> dlc
 * [6..] -> data
 */

int can_encode(CANFrame *frame, unsigned char *bytes, int max_boy)
{
    int toplam_boy;

    if (frame == NULL || bytes == NULL)
        return -1;

    if (frame->dlc > MAX_DATA_BOYUTU)
        return -1;

    toplam_boy = 6 + frame->dlc;

    if (max_boy < toplam_boy)
        return -1;

    bytes[0] = (frame->id >> 24) & 0xFF;
    bytes[1] = (frame->id >> 16) & 0xFF;
    bytes[2] = (frame->id >> 8)  & 0xFF;
    bytes[3] = (frame->id >> 0)  & 0xFF;

    bytes[4] = 0;
    if (frame->uzun_id) bytes[4] |= 0x01;
    if (frame->rtr)     bytes[4] |= 0x02;

    bytes[5] = frame->dlc;

    memcpy(bytes + 6, frame->data, frame->dlc);

    return toplam_boy;
}

int can_decode(CANFrame *frame, unsigned char *bytes, int boy)
{
    if (frame == NULL || bytes == NULL || boy < 6)
        return -1;

    frame->id  = (unsigned int)bytes[0] << 24;
    frame->id |= (unsigned int)bytes[1] << 16;
    frame->id |= (unsigned int)bytes[2] << 8;
    frame->id |= (unsigned int)bytes[3];

    frame->uzun_id = (bytes[4] & 0x01) ? 1 : 0;
    frame->rtr     = (bytes[4] & 0x02) ? 1 : 0;
    frame->dlc     = bytes[5];

    if (frame->dlc > MAX_DATA_BOYUTU)
        return -1;

    if (boy < 6 + frame->dlc)
        return -1;

    memcpy(frame->data, bytes + 6, frame->dlc);

    return 0;
}

void can_yazdir(CANFrame *frame)
{
    int i;

    if (frame == NULL)
        return;

    printf("--- CAN Frame ---\n");
    printf("ID     : 0x%X (%s)\n", frame->id, frame->uzun_id ? "29-bit" : "11-bit");
    printf("RTR    : %s\n", frame->rtr ? "evet" : "hayir");
    printf("DLC    : %d\n", frame->dlc);
    printf("Data   :");

    if (frame->dlc == 0) {
        printf(" (bos)");
    } else {
        for (i = 0; i < frame->dlc; i++)
            printf(" %02X", frame->data[i]);
    }
    printf("\n");
}

/*
 * Bit stuffing bit seviyesinde calisir.
 * Biz bunu byte dizisi uzerinde bit bit isliyoruz:
 * her biti tek tek okuyup yaziyoruz, 5 ayni bit gorursek zit bit ekliyoruz.
 */

/* yardimci: byte dizisinden bit oku (0 veya 1) */
static int bit_oku(unsigned char *dizi, int bit_indeks)
{
    int byte_no = bit_indeks / 8;
    int bit_no  = 7 - (bit_indeks % 8); /* MSB once */
    return (dizi[byte_no] >> bit_no) & 0x01;
}

/* yardimci: byte dizisine bit yaz */
static void bit_yaz(unsigned char *dizi, int bit_indeks, int deger)
{
    int byte_no = bit_indeks / 8;
    int bit_no  = 7 - (bit_indeks % 8);
    if (deger)
        dizi[byte_no] |=  (1 << bit_no);
    else
        dizi[byte_no] &= ~(1 << bit_no);
}

int stuff_uygula(unsigned char *giris, int giris_boy,
                 unsigned char *cikis, int max_cikis_boy)
{
    int giris_bit_sayisi = giris_boy * 8;
    int cikis_bit        = 0;
    int i;
    int son_bit     = -1;
    int tekrar_sayisi = 0;

    memset(cikis, 0, max_cikis_boy);

    for (i = 0; i < giris_bit_sayisi; i++) {
        int bit = bit_oku(giris, i);

        /* mevcut biti yaz */
        if (cikis_bit / 8 >= max_cikis_boy) return -1;
        bit_yaz(cikis, cikis_bit++, bit);

        /* ayni bit mi? */
        if (bit == son_bit) {
            tekrar_sayisi++;
        } else {
            tekrar_sayisi = 1;
            son_bit = bit;
        }

        /* 5 ayni bit ust uste geldiyse zit bit ekle */
        if (tekrar_sayisi == 5) {
            int stuff_bit = bit ? 0 : 1;
            if (cikis_bit / 8 >= max_cikis_boy) return -1;
            bit_yaz(cikis, cikis_bit++, stuff_bit);
            son_bit       = stuff_bit;
            tekrar_sayisi = 1;
        }
    }

    /* kac byte doldu (kismi byte da sayilir) */
    return (cikis_bit + 7) / 8;
}

int stuff_kaldir(unsigned char *giris, int giris_boy,
                 unsigned char *cikis, int max_cikis_boy)
{
    int giris_bit_sayisi = giris_boy * 8;
    int cikis_bit        = 0;
    int i;
    int son_bit       = -1;
    int tekrar_sayisi = 0;
    int atla          = 0; /* bir sonraki bit stuff bit mi? */

    memset(cikis, 0, max_cikis_boy);

    for (i = 0; i < giris_bit_sayisi; i++) {
        int bit = bit_oku(giris, i);

        if (atla) {
            /* bu stuff bit, sadece dogrula, yazma */
            atla          = 0;
            son_bit       = bit;
            tekrar_sayisi = 1;
            continue;
        }

        /* normal bit, yaz */
        if (cikis_bit / 8 >= max_cikis_boy) return -1;
        bit_yaz(cikis, cikis_bit++, bit);

        if (bit == son_bit) {
            tekrar_sayisi++;
        } else {
            tekrar_sayisi = 1;
            son_bit = bit;
        }

        /* 5 ayni bit geldiyse bir sonraki bit stuff bit */
        if (tekrar_sayisi == 5)
            atla = 1;
    }

    return (cikis_bit + 7) / 8;
}
