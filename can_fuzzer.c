#include "can_fuzzer.h"
#include "can_ids.h"
#include <stdlib.h>

/* bilinen id listesi — fuzzer bunlarin disinda id uretebilir */
static unsigned int bilinen_idler[] = {
    ID_MOTOR_RPM, ID_MOTOR_SICAK, ID_MOTOR_YAKIT,
    ID_ABS_TEKERLEK, ID_ABS_FREN
};
static int bilinen_id_sayisi = 5;

void fuzzer_baslat(CANFuzzer *fuzzer, CANBus *bus)
{
    fuzzer->bus              = bus;
    fuzzer->gonderici_id     = 0x7FF; /* en yuksek id, en dusuk oncelik */
    fuzzer->toplam_gonderilen = 0;
}

static void frame_gonder(CANFuzzer *fuzzer, CANFrame *frame)
{
    /* fuzzer direkt bus'a yazar, arbitration'a girmez */
    bus_yaz(fuzzer->bus, frame);
    fuzzer->toplam_gonderilen++;
}

void fuzzer_gonder(CANFuzzer *fuzzer, int tip)
{
    CANFrame frame;
    int i;

    frame.uzun_id = 0;
    frame.rtr     = 0;

    switch (tip) {

    case FUZZ_GECERSIZ_DLC:
        /* gecerli bir id sec ama dlc'yi 8'den buyuk yap */
        frame.id  = bilinen_idler[rand() % bilinen_id_sayisi];
        frame.dlc = 9 + (rand() % 6); /* 9-14 arasi */
        for (i = 0; i < 8; i++)
            frame.data[i] = rand() % 256;
        break;

    case FUZZ_BILINMEYEN_ID:
        /* bilinen id listesinde olmayan bir id uret */
        do {
            frame.id = rand() % 0x7FF;
        } while (frame.id == ID_MOTOR_RPM    ||
                 frame.id == ID_MOTOR_SICAK  ||
                 frame.id == ID_MOTOR_YAKIT  ||
                 frame.id == ID_ABS_TEKERLEK ||
                 frame.id == ID_ABS_FREN);
        frame.dlc = rand() % 9;
        for (i = 0; i < frame.dlc; i++)
            frame.data[i] = rand() % 256;
        break;

    case FUZZ_DEGER_TASMA:
        /* bilinen id ama deger aralik disi */
        frame.id  = ID_MOTOR_RPM;
        frame.dlc = DLC_MOTOR_RPM;
        /* rpm max 8000, biz 60000+ gonderiyoruz */
        frame.data[0] = 0xEA; /* 0xEA60 = 60000 */
        frame.data[1] = 0x60;
        break;

    case FUZZ_FLOOD:
        /* ayni id'den 10 mesaj arka arkaya */
        frame.id  = ID_MOTOR_RPM;
        frame.dlc = DLC_MOTOR_RPM;
        frame.data[0] = rand() % 256;
        frame.data[1] = rand() % 256;
        for (i = 0; i < 10; i++)
            frame_gonder(fuzzer, &frame);
        return; /* zaten gonderdik, altta tekrar gonderme */

    case FUZZ_RASTGELE:
    default:
        frame.id  = rand() % 0x7FF;
        frame.dlc = rand() % 15; /* kasitli olarak 8'den buyuk olabilir */
        for (i = 0; i < 8; i++)
            frame.data[i] = rand() % 256;
        break;
    }

    frame_gonder(fuzzer, &frame);
}

void fuzzer_rastgele_gonder(CANFuzzer *fuzzer)
{
    fuzzer_gonder(fuzzer, rand() % 5);
}
