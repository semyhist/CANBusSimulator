#include "can_detector.h"
#include "can_ids.h"
#include <stdio.h>
#include <string.h>

#define FLOOD_ESIGI 5  /* bir tick'te ayni id'den bu kadar mesaj gelirse flood */

void detector_baslat(CANDetector *detector, CANLogger *logger)
{
    int i;
    detector->logger         = logger;
    detector->toplam_anomali = 0;
    for (i = 0; i < MAX_TAKIP_ID; i++)
        detector->sayaclar[i].dolu = 0;
}

/* flood sayacinda id'yi bul veya yeni slot ac */
static IDSayac *sayac_bul(CANDetector *detector, unsigned int id)
{
    int i;
    int bos_slot = -1;

    for (i = 0; i < MAX_TAKIP_ID; i++) {
        if (detector->sayaclar[i].dolu && detector->sayaclar[i].id == id)
            return &detector->sayaclar[i];
        if (!detector->sayaclar[i].dolu && bos_slot == -1)
            bos_slot = i;
    }

    if (bos_slot != -1) {
        detector->sayaclar[bos_slot].id     = id;
        detector->sayaclar[bos_slot].sayac  = 0;
        detector->sayaclar[bos_slot].dolu   = 1;
        return &detector->sayaclar[bos_slot];
    }

    return NULL;
}

static void anomali_logla(CANDetector *detector, CANFrame *frame,
                          double timestamp, const char *aciklama)
{
    printf("[ANOMALI] t=%.3f | ID: 0x%X | DLC: %d | %s\n",
           timestamp, frame->id, frame->dlc, aciklama);

    /* anomaliyi log dosyasina da yaz */
    if (detector->logger != NULL)
        logger_yaz(detector->logger, frame, timestamp);

    detector->toplam_anomali++;
}

int detector_kontrol(CANDetector *detector, CANFrame *frame, double timestamp)
{
    int rpm;
    IDSayac *sayac;

    /* --- Kural 1: DLC 8'den buyuk olamaz --- */
    if (frame->dlc > 8) {
        anomali_logla(detector, frame, timestamp,
                      "Gecersiz DLC");
        return ANOMALI_GECERSIZ_DLC;
    }

    /* --- Kural 2: Bilinmeyen ID --- */
    if (frame->id != ID_MOTOR_RPM    &&
        frame->id != ID_MOTOR_SICAK  &&
        frame->id != ID_MOTOR_YAKIT  &&
        frame->id != ID_ABS_TEKERLEK &&
        frame->id != ID_ABS_FREN) {
        anomali_logla(detector, frame, timestamp,
                      "Bilinmeyen ID");
        return ANOMALI_BILINMEYEN_ID;
    }

    /* --- Kural 3: Deger aralik kontrolu --- */
    if (frame->id == ID_MOTOR_RPM && frame->dlc == DLC_MOTOR_RPM) {
        rpm = (frame->data[0] << 8) | frame->data[1];
        if (rpm < RPM_MIN || rpm > RPM_MAX) {
            anomali_logla(detector, frame, timestamp,
                          "RPM aralik disi");
            return ANOMALI_DEGER_ARALIK;
        }
    }

    if (frame->id == ID_MOTOR_SICAK && frame->dlc == DLC_MOTOR_SICAK) {
        if (frame->data[0] > SICAK_MAX) {
            anomali_logla(detector, frame, timestamp,
                          "Sicaklik aralik disi");
            return ANOMALI_DEGER_ARALIK;
        }
    }

    if (frame->id == ID_MOTOR_YAKIT && frame->dlc == DLC_MOTOR_YAKIT) {
        if (frame->data[0] > YAKIT_MAX) {
            anomali_logla(detector, frame, timestamp,
                          "Yakit aralik disi");
            return ANOMALI_DEGER_ARALIK;
        }
    }

    /* --- Kural 4: Flood tespiti --- */
    sayac = sayac_bul(detector, frame->id);
    if (sayac != NULL) {
        sayac->sayac++;
        if (sayac->sayac > FLOOD_ESIGI) {
            anomali_logla(detector, frame, timestamp,
                          "Flood saldirisi");
            return ANOMALI_FLOOD;
        }
    }

    return ANOMALI_YOK;
}

void detector_sayac_sifirla(CANDetector *detector)
{
    int i;
    for (i = 0; i < MAX_TAKIP_ID; i++)
        if (detector->sayaclar[i].dolu)
            detector->sayaclar[i].sayac = 0;
}

void detector_istatistik_yazdir(CANDetector *detector)
{
    printf("\n=== Guvenlik Raporu ===\n");
    printf("Toplam anomali: %d\n", detector->toplam_anomali);
}
