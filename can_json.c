#include "can_json.h"
#include <stdio.h>

int json_yaz(const char *dosya_adi, DashboardVeri *veri)
{
    FILE *f = fopen(dosya_adi, "w");
    if (f == NULL)
        return -1;

    fprintf(f, "{\n");
    fprintf(f, "  \"rpm\":      %d,\n", veri->rpm);
    fprintf(f, "  \"hiz\":      %d,\n", veri->hiz);
    fprintf(f, "  \"sicaklik\": %d,\n", veri->sicaklik);
    fprintf(f, "  \"yakit\":    %d,\n", veri->yakit);
    fprintf(f, "  \"abs\":      %s,\n", veri->abs  ? "true" : "false");
    fprintf(f, "  \"fren\":     %s\n",  veri->fren ? "true" : "false");
    fprintf(f, "}\n");

    fclose(f);
    return 0;
}
