#ifndef ECU_ABS_H
#define ECU_ABS_H

#include "can_node.h"

#define ABS_TEKERLEK_ID 0x200
#define ABS_FREN_ID     0x201

typedef struct {
    CANNode node;
    int     tekerlek_hizi[4]; /* on sol, on sag, arka sol, arka sag */
    int     fren_aktif;       /* 1 = fren basili */
    int     abs_devrede;      /* 1 = abs calisiyor */
} AbsECU;

void abs_baslat(AbsECU *ecu, CANBus *bus);
void abs_guncelle(AbsECU *ecu);

#endif
