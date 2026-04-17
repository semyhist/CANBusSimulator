#include "can_json.h"
#include <stdio.h>

int json_yaz(const char *dosya_adi, DashboardVeri *veri)
{
    FILE *f = fopen(dosya_adi, "w");
    if (f == NULL)
        return -1;

    fprintf(f, "{\n");
    fprintf(f, "  \"rpm\": %d,\n",              veri->rpm);
    fprintf(f, "  \"hiz\": %d,\n",              veri->hiz);
    fprintf(f, "  \"sicaklik\": %d,\n",         veri->sicaklik);
    fprintf(f, "  \"yakit\": %d,\n",            veri->yakit);
    fprintf(f, "  \"abs\": %s,\n",              veri->abs    ? "true" : "false");
    fprintf(f, "  \"fren\": %s,\n",             veri->fren   ? "true" : "false");
    fprintf(f, "  \"airbag_aktif\": %s,\n",     veri->airbag_aktif   ? "true" : "false");
    fprintf(f, "  \"klima_hedef\": %d,\n",      veri->klima_hedef);
    fprintf(f, "  \"klima_kompressor\": %s,\n", veri->klima_kompressor ? "true" : "false");
    fprintf(f, "  \"direksiyon_aci\": %d,\n",   veri->direksiyon_aci);
    fprintf(f, "  \"ic_sicaklik\": %d,\n",      veri->ic_sicaklik);
    fprintf(f, "  \"dis_sicaklik\": %d,\n",     veri->dis_sicaklik);
    fprintf(f, "  \"lastik\": [%d, %d, %d, %d]\n",
        veri->lastik[0], veri->lastik[1], veri->lastik[2], veri->lastik[3]);
    fprintf(f, "}\n");

    fclose(f);
    return 0;
}
