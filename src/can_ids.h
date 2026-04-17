#ifndef CAN_IDS_H
#define CAN_IDS_H

/* bilinen mesaj id'leri */
#define ID_MOTOR_RPM    0x100
#define ID_MOTOR_SICAK  0x101
#define ID_MOTOR_YAKIT  0x102
#define ID_ABS_TEKERLEK 0x200
#define ID_ABS_FREN     0x201
#define ID_AIRBAG       0x300
#define ID_KLIMA        0x301
#define ID_DIREKSIYON   0x302
#define ID_KABIN        0x303
#define ID_LASTIK       0x304

/* her id icin beklenen deger araliklari */
#define RPM_MIN     0
#define RPM_MAX     8000

#define SICAK_MIN   0
#define SICAK_MAX   120

#define YAKIT_MIN   0
#define YAKIT_MAX   100

#define HIZ_MIN     0
#define HIZ_MAX     250

/* beklenen dlc degerleri */
#define DLC_MOTOR_RPM    2
#define DLC_MOTOR_SICAK  1
#define DLC_MOTOR_YAKIT  1
#define DLC_ABS_TEKERLEK 4
#define DLC_ABS_FREN     2

#endif
