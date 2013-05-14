#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <syslog.h>

#define THERMAL_FILE "/proc/acpi/ibm/thermal"
#define FAN_FILE     "/proc/acpi/ibm/fan"
#define PID_FILE     "/var/run/tpcool.pid"

#define SLEEP_DURATION_S    5

#define SWITCH_TEMP_LOW_C   63
#define SWITCH_TEMP_HIGH_C  66


FILE *fp_log=0;

void daemon_loop();
int read_temp(char *filename);
void write_fan(char *filename, bool turbo_mode);
void write_pid(char *filename, pid_t pid);

int main(int argc, char** argv)
{
    // fork daemon process ///////////
    pid_t pid = fork();
    if (pid < 0) {
        // failed
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        // parent
        write_pid(PID_FILE, pid);
        exit(EXIT_SUCCESS);
    }

    // set umask /////////////////////
    umask(0);

    // open logs /////////////////////
    openlog(argv[0],LOG_NOWAIT|LOG_PID,LOG_USER);

    // create session id /////////////
    pid_t sid;
    sid = setsid();
    if (sid < 0) {
        syslog(LOG_ERR, "Could not create process group\n");
        exit(EXIT_FAILURE);
    }

    // set working directory /////////
    if ((chdir("/")) < 0) {
        syslog(LOG_ERR, "Could not change working directory to /\n");
        exit(EXIT_FAILURE);
    }

    // close standard files //////////
    fclose(stdin);
    fclose(stdout);
    fclose(stderr);

    // start loop ////////////////////
    daemon_loop();

    exit(EXIT_FAILURE);
}

void daemon_loop()
{

    bool turbo_mode=false;

    for (;;) {
        int temp = read_temp(THERMAL_FILE);

        int switch_temp = turbo_mode ? 
            SWITCH_TEMP_LOW_C : SWITCH_TEMP_HIGH_C;

        turbo_mode = (temp>switch_temp);
        write_fan(FAN_FILE, turbo_mode);

        sleep(SLEEP_DURATION_S);

    }

}

int read_temp(char *filename)
{
    FILE * fp = fopen(filename,"r");
    int temp = -1;

    if (0==fp) {
        syslog(LOG_ERR, "Failed to open %s", filename);
        exit(EXIT_FAILURE);
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
        syslog(LOG_ERR, "Failed to open %s", filename);
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "level %s", turbo_mode ? "full-speed" : "auto");

    fclose(fp);
}

void write_pid(char *filename, pid_t pid)
{
    FILE * fp = fopen(filename,"w");

    if (0==fp) {
        // logging not enabled yet
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "%d", pid);

    fclose(fp);
}
