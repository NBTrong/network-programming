#ifndef SESSION_H
#define SESSION_H

#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include "../../global.h"

typedef struct Session
{
  int socket_id;
  char client_addr[STRING_LENGTH];
  int port;
  char client_username[STRING_LENGTH];
  int login_status;

  struct Session *next;
} Session;

extern Session *session_list;

void add_session(int socket_id, const char *client_addr, int port, const char *client_username, int login_status);

void free_session_list();

Session *find_session_by_username(const char *username);

Session *find_session_by_socket_id(int socket_id);

void print_all_sessions();

void delete_session_by_socket_id(int socket_id);

#endif
