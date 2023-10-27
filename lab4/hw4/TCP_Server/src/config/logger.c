#include "logger.h"

void logger(const char *request, const char *response) {
    time_t now;
    struct tm timestamp;

    // Get current time
    time(&now);
    localtime_r(&now, &timestamp);

    // Open file
    char logFileName[100] = "TCP_Server/logs/log_20194693.log";
    FILE *logFile = fopen(logFileName, "a");

    if (logFile != NULL) {
        char timestampStr[100];
        strftime(timestampStr, sizeof(timestampStr), "[%d/%m/%Y %H:%M:%S]", &timestamp);

        // Write file
        fprintf(logFile, "%s $ %s $ %s\n", timestampStr, request, response);
        fclose(logFile);
        
        // Output stdin
        printf("Response: %s\n", response);
    } else {
        printf("Can not open file log\n");
    }
}