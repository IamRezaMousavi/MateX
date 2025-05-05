#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <fcntl.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/socket.h>

#include "command.h"
#include "connection.h"
#include "user.h"

#define PORT 5000

int server_fd;

int init_network() {
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    perror("Socket error");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server_addr = {.sin_family = AF_INET, .sin_addr.s_addr = INADDR_ANY, .sin_port = htons(PORT)};
  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("Bind error");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 10) < 0) {
    perror("Listen error");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  int flags = fcntl(server_fd, F_GETFL, 0);
  if (flags < 0 || fcntl(server_fd, F_SETFL, flags | O_NONBLOCK) < 0) {
    perror("fcntl error");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port %d...\n", PORT);
  return server_fd;
}

void accept_new_connection() {
  int client_fd = accept(server_fd, NULL, NULL);
  if (client_fd < 0)
    return;

  int idx = find_empty_user();
  if (idx < 0) {
    send(client_fd, "Server full\n", 12, 0);
    close(client_fd);
    return;
  }

  users[idx].fd     = client_fd;
  users[idx].status = USER_QUEST;
  strcpy(users[idx].username, "guest");
  char *message = "Welcome to the chess server! Type /help\n";
  send(client_fd, message, strlen(message), 0);
}

void process_network_event(int timeout) {
  fd_set read_fds;
  FD_ZERO(&read_fds);
  FD_SET(server_fd, &read_fds);
  int maxfd = server_fd;
  for (int i = 0; i < MAX_CLIENT; i++) {
    if (users[i].status != USER_EMPTY) {
      FD_SET(users[i].fd, &read_fds);
      if (users[i].fd > maxfd)
        maxfd = users[i].fd;
    }
  }

  struct timeval _timeout = {.tv_sec = timeout, .tv_usec = 0};
  int            ready    = select(maxfd + 1, &read_fds, NULL, NULL, &_timeout);
  if (ready < 0)
    return;

  if (FD_ISSET(server_fd, &read_fds)) {
    accept_new_connection();
  }

  for (int i = 0; i < MAX_USERS; i++) {
    if (users[i].status == USER_EMPTY)
      continue;
    if (FD_ISSET(users[i].fd, &read_fds)) {
      char buffer[1024];
      int  len = recv(users[i].fd, buffer, sizeof(buffer) - 1, 0);
      if (len < 0) {
        cleanup_user(&users[i]);
      } else {
        buffer[len] = '\0';
        handle_command(&users[i], buffer);
      }
    }
  }
}

void cleanup_network() {
  if (server_fd != -1) {
    close(server_fd);
    printf("Closed network socket\n");
  }
  for (int i = 0; i < MAX_USERS; i++) {
    if (users[i].status != USER_EMPTY) {
      cleanup_user(&users[i]);
    }
  }
}

#endif /* __NETWORK_H__ */
