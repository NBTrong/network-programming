// globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

#define STRING_LENGTH 100

extern char currentUser[STRING_LENGTH];

enum LoginStatus
{
  ACCOUNT_NOT_EXIST = 1,
  ACCOUNT_BANNED = 2,
  LOGGED_IN = 3,
  NOT_LOGGED_IN = 4,
  ACCOUNT_VALID = 5,
  LOGGED_IN_ON_ANOTHER_DEVICE = 6,
  UNDEFINED = 7,
};

enum Code
{
  // Connect server
  CONNECTED_SUCCESSFULLY = 100,

  // Login
  ACCOUNT_EXISTS_AND_ACTIVE = 110,
  ACCOUNT_LOCKED = 211,
  ACCOUNT_NOT_FOUND = 212,
  ACCOUNT_ALREADY_LOGGED_IN = 214,
  DATABASE_ERROR = 215,

  // Post
  POST_SUCCESSFULLY = 120,

  // Logout
  LOGOUT_SUCCESSFULLY = 130,
  UNDEFINED_MESSAGE_TYPE = 300,

  // Do not use the service because not logged in
  NOT_HAVE_ACCESS = 221,
};

enum AccountStatus
{
  BAN = 0,
  ACTIVE = 1
};

#endif
