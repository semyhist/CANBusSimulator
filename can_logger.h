#ifndef CAN_LOGGER_H
#define CAN_LOGGER_H

#include <stdio.h>
#include "can_frame.h"

typedef struct {
    FILE *dosya;
    int   mesaj_sayisi;
} CANLogger;

/* log dosyasini acar, basarili olursa 0 doner */
int  logger_baslat(CANLogger *logger, const char *dosya_adi);

/* bir frame'i log dosyasina yazar */
void logger_yaz(CANLogger *logger, CANFrame *frame, double timestamp);

/* dosyayi kapatir */
void logger_kapat(CANLogger *logger);

#endif
