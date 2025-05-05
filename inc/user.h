#ifndef __USER_H__
#define __USER_H__

#include <string.h>
#include <unistd.h>

#define MAX_USERS    100
#define USERNAME_LEN 32

enum UserStatus {
  USER_EMPTY,
  USER_QUEST,
  USER_LOGGED_IN,
};

typedef struct User {
  int             fd;
  enum UserStatus status;
  char            username[USERNAME_LEN];
} User;

User users[MAX_USERS];

void init_users() {
  memset(users, 0, sizeof(users));
  for (int i = 0; i < MAX_USERS; i++) {
    users[i].status = USER_EMPTY;
  }
}

int find_empty_user() {
  for (int i = 0; i < MAX_USERS; i++) {
    if (users[i].status == USER_EMPTY)
      return i;
  }
  return -1;
}

void cleanup_user(User *user) {
  if (user->fd != -1)
    close(user->fd);
  user->fd     = -1;
  user->status = USER_EMPTY;
  memset(user->username, 0, USERNAME_LEN);
}

#endif /* __USER_H__ */
