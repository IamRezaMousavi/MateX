#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "user.h"

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

void handle_help(User *user) {
  const char *help_text = "Available commands:\n"
                          " /help            Show this help message\n"
                          " /who             Show online users\n"
                          " /quit            Disconnect\n";
  send(user->fd, help_text, strlen(help_text), 0);
}

void handle_who(User *user) {
  char buffer[1024];
  int  len = snprintf(buffer, sizeof(buffer), "Online users:\n");
  for (int i = 0; i < MAX_USERS; i++) {
    if (users[i].status != USER_EMPTY) {
      len += snprintf(buffer + len, sizeof(buffer) - len, "  - %s\n", users[i].username);
    }
  }
  send(user->fd, buffer, strlen(buffer), 0);
}

void handle_quit(User *user) {
  char *message = "Goodbye\n";
  send(user->fd, message, strlen(message), 0);
  cleanup_user(user);
}

void handle_command(User *user, const char *cmd) {
  if (strncmp(cmd, "/help", 5) == 0) {
    handle_help(user);
  } else if (strncmp(cmd, "/who", 4) == 0) {
    handle_who(user);
  } else if (strncmp(cmd, "/quit", 5) == 0) {
    handle_quit(user);
  } else {
    char *message = "Unknown command. Type /help\n";
    send(user->fd, message, strlen(message), 0);
  }
}

#endif /* __COMMAND_H__ */
