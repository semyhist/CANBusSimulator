#include "can_logger.h"
#include <stdio.h>

/*
 * log format (candump ile ayni):
 * (timestamp) bus0 ID#DATA
 * ornek: (1000.000000) bus0 1A2#DEADBEEF
 */

int logger_baslat(CANLogger *logger, const char *dosya_adi)
{
    logger->dosya = fopen(dosya_adi, "w");
    if (logger->dosya == NULL)
        return -1;

    logger->mesaj_sayisi = 0;
    return 0;
}

void logger_yaz(CANLogger *logger, CANFrame *frame, double timestamp)
{
    int i;

    if (logger->dosya == NULL || frame == NULL)
        return;

    /* timestamp ve id yaz */
    fprintf(logger->dosya, "(%.6f) bus0 %X#", timestamp, frame->id);

    /* data yaz, bos ise sadece bosluk birak */
    if (frame->dlc == 0) {
        fprintf(logger->dosya, " ");
    } else {
        for (i = 0; i < frame->dlc; i++)
            fprintf(logger->dosya, "%02X", frame->data[i]);
    }

    fprintf(logger->dosya, "\n");
    logger->mesaj_sayisi++;
}

void logger_kapat(CANLogger *logger)
{
    if (logger->dosya != NULL) {
        fclose(logger->dosya);
        logger->dosya = NULL;
    }
}
