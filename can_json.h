#ifndef CAN_JSON_H
#define CAN_JSON_H

typedef struct {
    int rpm;
    int hiz;       /* km/h */
    int sicaklik;  /* celsius */
    int yakit;     /* yuzde 0-100 */
    int abs;       /* 1 = devrede */
    int fren;      /* 1 = basili */
} DashboardVeri;

/* veriyi json dosyasina yazar, basarili olursa 0 doner */
int json_yaz(const char *dosya_adi, DashboardVeri *veri);

#endif
