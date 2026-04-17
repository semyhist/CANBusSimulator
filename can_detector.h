#ifndef CAN_DETECTOR_H
#define CAN_DETECTOR_H

#include "can_frame.h"
#include "can_logger.h"

/* tespit edilen anomali tipleri */
#define ANOMALI_YOK            0
#define ANOMALI_GECERSIZ_DLC   1
#define ANOMALI_BILINMEYEN_ID  2
#define ANOMALI_DEGER_ARALIK   3
#define ANOMALI_FLOOD          4

#define MAX_TAKIP_ID 32

/* flood tespiti icin id basina mesaj sayaci */
typedef struct {
    unsigned int id;
    int          sayac;
    int          dolu;
} IDSayac;

typedef struct {
    IDSayac    sayaclar[MAX_TAKIP_ID];
    CANLogger  *logger;         /* anomalileri log'a da yaz */
    int        toplam_anomali;
} CANDetector;

void detector_baslat(CANDetector *detector, CANLogger *logger);

/*
 * frame'i kurallara gore kontrol eder.
 * anomali varsa anomali tipini doner, yoksa ANOMALI_YOK doner.
 */
int  detector_kontrol(CANDetector *detector, CANFrame *frame, double timestamp);

/* flood sayaclarini sifirla (her tick'te cagrilmali) */
void detector_sayac_sifirla(CANDetector *detector);

void detector_istatistik_yazdir(CANDetector *detector);

#endif
