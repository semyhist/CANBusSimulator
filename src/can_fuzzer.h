#ifndef CAN_FUZZER_H
#define CAN_FUZZER_H

#include "can_frame.h"
#include "can_bus.h"

/* fuzzer tarafindan uretilen frame tipleri */
#define FUZZ_GECERSIZ_DLC      0  /* dlc > 8 */
#define FUZZ_BILINMEYEN_ID     1  /* tanimli olmayan id */
#define FUZZ_DEGER_TASMA       2  /* bilinen id ama deger aralik disi */
#define FUZZ_FLOOD             3  /* ayni id'den cok fazla mesaj */
#define FUZZ_RASTGELE          4  /* tamamen rastgele her sey */

typedef struct {
    CANBus *bus;
    int     gonderici_id;   /* fuzzer'in bus'taki node id'si */
    int     toplam_gonderilen;
} CANFuzzer;

void fuzzer_baslat(CANFuzzer *fuzzer, CANBus *bus);

/* belirli tipte fuzz frame uret ve bus'a gonder */
void fuzzer_gonder(CANFuzzer *fuzzer, int tip);

/* rastgele tipte fuzz frame gonder */
void fuzzer_rastgele_gonder(CANFuzzer *fuzzer);

#endif
