// globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

#define STRING_LENGTH 100

extern char currentUser[STRING_LENGTH];

enum LoginStatus {
    ACCOUNT_NOT_EXIST = 1,
    ACCOUNT_BANNED = 2,
    LOGGED_IN = 3,
    NOT_LOGGED_IN = 4,
    ACCOUNT_VALID = 5
};

enum AccountStatus {
    BAN = 0,
    ACTIVE = 1
};

#endif
