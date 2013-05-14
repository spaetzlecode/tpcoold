#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

int read_temp(char *filename)
{
    FILE * fp = fopen(filename,"r");
    int temp = -1;

    if (0==fp) {
        fprintf(stderr, "Failed to open %s", filename);
        exit(1);
    }

    char buf[100];
    while (fgets(buf,sizeof(buf),fp)) {
        // only first temperature in the list
        // is relevant for me (on TP410) - YMMV
        if (sscanf(buf, "temperature: %d", temp)) break;
    }
    fclose(fp);

    return temp;
}

void write_fan(char *filename, bool turbo_mode)
{
    FILE * fp = fopen(filename,"w");

    if (0==fp) {
        fprintf(stderr, "Failed to open %s", filename);
        exit(1);
    }

    fprintf(fp, "level %s", turbo_mode ? "full-speed" : "auto");

    fclose(fp);
}

#define THERMAL_FILE "/proc/acpi/ibm/thermal"
#define FAN_FILE     "/proc/acpi/ibm/fan"

#define SLEEP_DURATION_S    5

#define SWITCH_TEMP_LOW_C   63
#define SWITCH_TEMP_HIGH_C  66



int main()
{
    bool turbo_mode=false;

    for (;;) {
        int temp = read_temp(THERMAL_FILE);

        int switch_temp = turbo_mode ? 
            SWITCH_TEMP_LOW_C :SWITCH_TEMP_HIGH_C;

        turbo_mode = (temp>switch_temp);
        write_fan(FAN_FILE, turbo_mode);

        sleep(SLEEP_DURATION_S);

    }


    exit(1);
}
