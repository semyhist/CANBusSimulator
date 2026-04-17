#ifndef CAN_PARSER_H
#define CAN_PARSER_H

#include "can_frame.h"

#define MAX_LOG_SATIR 1024

/* parse edilmis tek bir log satiri */
typedef struct {
    double        timestamp;
    CANFrame      frame;
} LogSatir;

/* istatistik icin id bazli sayac */
typedef struct {
    unsigned int id;
    int          adet;
    int          toplam_dlc;
} IDIstatistik;

#define MAX_FARKLI_ID 64

typedef struct {
    LogSatir     satirlar[MAX_LOG_SATIR];
    int          satir_sayisi;
    IDIstatistik istatistikler[MAX_FARKLI_ID];
    int          farkli_id_sayisi;
} ParseSonuc;

/* dosyayi okur ve parse eder, basarili olursa 0 doner */
int  parser_calistir(const char *dosya_adi, ParseSonuc *sonuc);

/* sadece belirli id'ye ait satirlari yazdirir, filtre_id 0 ise hepsini yazar */
void parser_yazdir(ParseSonuc *sonuc, unsigned int filtre_id);

/* istatistikleri yazdirir */
void parser_istatistik_yazdir(ParseSonuc *sonuc);

#endif
