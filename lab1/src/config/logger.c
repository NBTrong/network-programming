#include "logger.h"

void logger(const char *function, const char *user, const char *result, const char* message) {
    time_t now;
    struct tm timestamp;

    // Get current time
    time(&now);
    localtime_r(&now, &timestamp);

    // Open file
    char logFileName[30] = "logs/log_20194693.log";
    FILE *logFile = fopen(logFileName, "a");

    if (logFile != NULL) {
        char timestampStr[30];
        strftime(timestampStr, sizeof(timestampStr), "[%d/%m/%Y %H:%M:%S]", &timestamp);

        // Write file
        fprintf(logFile, "%s $ %s $ %s $ %s\n", timestampStr, function, user, result);
        fclose(logFile);
        
        // Output stdin
        printf("%s", message);
    } else {
        printf("Can not open file log\n");
    }
}