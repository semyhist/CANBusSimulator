#ifndef CAN_JSON_H
#define CAN_JSON_H

typedef struct {
    /* motor */
    int rpm;
    int sicaklik;
    int yakit;
    /* abs */
    int hiz;
    int abs;
    int fren;
    /* airbag */
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
    int lastik[4]; /* PSI */
} DashboardVeri;

int json_yaz(const char *dosya_adi, DashboardVeri *veri);

#endif
