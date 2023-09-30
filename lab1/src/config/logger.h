#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <time.h>

/**
 * @brief Log an event or action with details to a log file and optionally display a message.
 *
 * This function is used to log an event or action, including details such as the function name,
 * the associated user, the result or status, and an optional message, to a log file. It also
 * optionally displays the message on the standard output.
 *
 * @param function A string specifying the name of the function or action being logged.
 * @param user A string indicating the user associated with the event (e.g., username or ID).
 * @param result A string describing the outcome or result of the event (e.g., "success" or "error").
 * @param message An optional string providing additional information or context for the event (can be NULL).
 */
void logger(const char *function, const char *user, const char *result, const char *message);

#endif