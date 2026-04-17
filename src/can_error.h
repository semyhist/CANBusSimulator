#ifndef CAN_ERROR_H
#define CAN_ERROR_H

/* node'un bus uzerindeki durumu */
#define DURUM_NORMAL         0  /* hata sayaci < 128 */
#define DURUM_ERROR_PASSIVE  1  /* hata sayaci 128-254 */
#define DURUM_BUS_OFF        2  /* hata sayaci 255, node sustu */

/* hata tipleri */
#define HATA_YOK             0
#define HATA_BIT_STUFFING    1  /* 5 ayni bit ust uste geldi */
#define HATA_ACK             2  /* kimse mesaji onaylamadi */
#define HATA_FORM            3  /* frame formati yanlis */

/* hata sayaci esik degerleri (gercek CAN spesifikasyonundan) */
#define ESIK_ERROR_PASSIVE  128
#define ESIK_BUS_OFF        256

#endif
