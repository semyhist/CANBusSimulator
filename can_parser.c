#include "can_parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* hex string'i byte dizisine cevir, kac byte yazildigini doner */
static int hex_cevir(const char *hex, unsigned char *cikis)
{
    int boy = 0;
    int uzunluk = (int)strlen(hex);
    int i;

    for (i = 0; i + 1 < uzunluk && boy < 8; i += 2) {
        char iki[3] = { hex[i], hex[i+1], '\0' };
        cikis[boy++] = (unsigned char)strtol(iki, NULL, 16);
    }

    return boy;
}

/* istatistiklere id ekle veya guncelle */
static void istatistik_guncelle(ParseSonuc *sonuc, unsigned int id, int dlc)
{
    int i;

    for (i = 0; i < sonuc->farkli_id_sayisi; i++) {
        if (sonuc->istatistikler[i].id == id) {
            sonuc->istatistikler[i].adet++;
            sonuc->istatistikler[i].toplam_dlc += dlc;
            return;
        }
    }

    /* yeni id */
    if (sonuc->farkli_id_sayisi < MAX_FARKLI_ID) {
        sonuc->istatistikler[sonuc->farkli_id_sayisi].id        = id;
        sonuc->istatistikler[sonuc->farkli_id_sayisi].adet      = 1;
        sonuc->istatistikler[sonuc->farkli_id_sayisi].toplam_dlc = dlc;
        sonuc->farkli_id_sayisi++;
    }
}

int parser_calistir(const char *dosya_adi, ParseSonuc *sonuc)
{
    FILE *dosya;
    char  satir[256];
    char  id_ve_data[64];
    char  id_str[16];
    char  data_str[32];
    char *ayrac;
    double timestamp;

    sonuc->satir_sayisi    = 0;
    sonuc->farkli_id_sayisi = 0;

    dosya = fopen(dosya_adi, "r");
    if (dosya == NULL)
        return -1;

    while (fgets(satir, sizeof(satir), dosya) != NULL) {
        if (sonuc->satir_sayisi >= MAX_LOG_SATIR)
            break;

        LogSatir *ls = &sonuc->satirlar[sonuc->satir_sayisi];

        /* format: (timestamp) bus0 ID#DATA */
        if (sscanf(satir, "(%lf) %*s %63s", &timestamp, id_ve_data) != 2)
            continue;

        ls->timestamp = timestamp;

        /* ID#DATA'yi # ile ayir */
        ayrac = strchr(id_ve_data, '#');
        if (ayrac == NULL)
            continue;

        /* id kismi */
        int id_uzunluk = (int)(ayrac - id_ve_data);
        if (id_uzunluk >= (int)sizeof(id_str))
            continue;
        strncpy(id_str, id_ve_data, id_uzunluk);
        id_str[id_uzunluk] = '\0';

        ls->frame.id      = (unsigned int)strtol(id_str, NULL, 16);
        ls->frame.uzun_id = 0;
        ls->frame.rtr     = 0;

        /* data kismi */
        strncpy(data_str, ayrac + 1, sizeof(data_str) - 1);
        data_str[sizeof(data_str) - 1] = '\0';

        /* satir sonu karakterini temizle */
        data_str[strcspn(data_str, "\r\n")] = '\0';

        ls->frame.dlc = (unsigned char)hex_cevir(data_str, ls->frame.data);

        istatistik_guncelle(sonuc, ls->frame.id, ls->frame.dlc);
        sonuc->satir_sayisi++;
    }

    fclose(dosya);
    return 0;
}

void parser_yazdir(ParseSonuc *sonuc, unsigned int filtre_id)
{
    int i, j;

    for (i = 0; i < sonuc->satir_sayisi; i++) {
        LogSatir *ls = &sonuc->satirlar[i];

        if (filtre_id != 0 && ls->frame.id != filtre_id)
            continue;

        printf("(%.6f) bus0 %X#", ls->timestamp, ls->frame.id);
        for (j = 0; j < ls->frame.dlc; j++)
            printf("%02X", ls->frame.data[j]);
        printf("\n");
    }
}

void parser_istatistik_yazdir(ParseSonuc *sonuc)
{
    int i;

    printf("\n=== Istatistik ===\n");
    printf("Toplam mesaj : %d\n", sonuc->satir_sayisi);
    printf("Farkli ID    : %d\n\n", sonuc->farkli_id_sayisi);
    printf("%-8s  %-8s  %-12s\n", "ID", "Adet", "Ort. DLC");
    printf("-----------------------------\n");

    for (i = 0; i < sonuc->farkli_id_sayisi; i++) {
        IDIstatistik *ist = &sonuc->istatistikler[i];
        double ort_dlc = (double)ist->toplam_dlc / ist->adet;
        printf("0x%-6X  %-8d  %.1f\n", ist->id, ist->adet, ort_dlc);
    }
}
