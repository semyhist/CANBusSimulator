#include "can_bus.h"
#include "can_fuzzer.h"
#include "can_detector.h"
#include "can_logger.h"
#include "ecu_motor.h"
#include "ecu_abs.h"
#include "ecu_dashboard.h"
#include <stdio.h>
#include <stdlib.h>

#define ADIM_SAYISI    10
#define ANOMALI_LOG    "anomali.log"

int main()
{
    CANBus       bus;
    MotorECU     motor;
    AbsECU       abs_ecu;
    DashboardECU dashboard;
    CANFuzzer    fuzzer;
    CANDetector  detector;
    CANLogger    anomali_logger;
    int          adim, i;
    double       timestamp;

    srand(42);

    bus_baslat(&bus);
    motor_baslat(&motor, &bus);
    abs_baslat(&abs_ecu, &bus);
    dashboard_baslat(&dashboard, &bus);
    fuzzer_baslat(&fuzzer, &bus);

    logger_baslat(&anomali_logger, ANOMALI_LOG);
    detector_baslat(&detector, &anomali_logger);

    printf("=== Fuzzing Basladi ===\n\n");

    for (adim = 1; adim <= ADIM_SAYISI; adim++) {
        timestamp = adim * 0.01;
        printf("--- Adim %d (t=%.3fs) ---\n", adim, timestamp);

        /* normal ECU'lar mesaj gonderir */
        motor_guncelle(&motor);
        abs_guncelle(&abs_ecu);

        /* fuzzer her adimda farkli tipte saldiri yapar */
        switch (adim % 5) {
        case 0: fuzzer_gonder(&fuzzer, FUZZ_GECERSIZ_DLC);  break;
        case 1: fuzzer_gonder(&fuzzer, FUZZ_BILINMEYEN_ID); break;
        case 2: fuzzer_gonder(&fuzzer, FUZZ_DEGER_TASMA);   break;
        case 3: fuzzer_gonder(&fuzzer, FUZZ_FLOOD);         break;
        case 4: fuzzer_gonder(&fuzzer, FUZZ_RASTGELE);      break;
        }

        /* arbitration: mesajlari bus'a yaz */
        while (bus_arbitration_calistir(&bus) != -1)
            ;

        /* bus'taki tum mesajlari detector'dan gecir */
        for (i = 0; i < BUS_KUYRUK_BOYUTU; i++) {
            if (bus.dolu[i])
                detector_kontrol(&detector, &bus.slots[i], timestamp);
        }

        detector_sayac_sifirla(&detector);
        dashboard_oku(&dashboard);
        printf("\n");
    }

    printf("Fuzzer toplam gonderdigi: %d\n", fuzzer.toplam_gonderilen);
    detector_istatistik_yazdir(&detector);
    printf("Anomaliler kaydedildi: %s\n", ANOMALI_LOG);

    logger_kapat(&anomali_logger);
    return 0;
}
